#ifndef __DEBUG_sdcard
#undef DEBUG
#endif

#include "printf.h"
#include "types.h"
#include "errno.h"
#include "fs/buf.h"
#include "sync/spinlock.h"
#include "sync/waitqueue.h"
#include "sched/proc.h"

#include "hal/riscv.h"
#include "hal/gpiohs.h"
#include "hal/dmac.h"
#include "hal/spi.h"
#include "hal/sdcard.h"

#include "utils/debug.h"

void SD_CS_HIGH(void) {
    gpiohs_set_pin(7, GPIO_PV_HIGH);
}

void SD_CS_LOW(void) {
    gpiohs_set_pin(7, GPIO_PV_LOW);
}

void SD_HIGH_SPEED_ENABLE(void) {
    // spi_set_clk_rate(SPI_DEVICE_0, 10000000);
	spi_set_baudr(SPI_DEVICE_0, 16);
}

static void sd_lowlevel_init(uint8 spi_index) {
    gpiohs_set_drive_mode(7, GPIO_DM_OUTPUT);
    // spi_set_clk_rate(SPI_DEVICE_0, 200000);     /*set clk rate*/
	spi_set_baudr(SPI_DEVICE_0, 1900);
}

static void sd_write_data(uint8 const *data_buff, uint32 length) {
    spi_init(SPI_DEVICE_0, SPI_WORK_MODE_0, SPI_FF_STANDARD, 8, 0);
    spi_send_data_standard(SPI_DEVICE_0, SPI_CHIP_SELECT_3, NULL, 0, data_buff, length);
}

static void sd_read_data(uint8 *data_buff, uint32 length) {
    spi_init(SPI_DEVICE_0, SPI_WORK_MODE_0, SPI_FF_STANDARD, 8, 0);
    spi_receive_data_standard(SPI_DEVICE_0, SPI_CHIP_SELECT_3, NULL, 0, data_buff, length);
}

// static void sd_write_data_dma(uint8 const *data_buff, uint32 length) {
//     // spi_init(SPI_DEVICE_0, SPI_WORK_MODE_0, SPI_FF_STANDARD, 8, 0);
// 	// spi_send_data_standard_dma(DMAC_CHANNEL0, SPI_DEVICE_0, SPI_CHIP_SELECT_3, NULL, 0, data_buff, length);
// 	/**
// 	 * It is wired that the dmac only works with 32-bit data width.
// 	 * So here we cast the uint8 buf into uint32 type, and divide the
// 	 * length by 4 temporarily.
// 	 * ... Perhaps it's the spi FIFO's problem, whose data width is 32-bit.
// 	 */
//     spi_init(SPI_DEVICE_0, SPI_WORK_MODE_0, SPI_FF_STANDARD, 32, 1);
// 	spi_send_data_no_cmd_dma(DMAC_CHANNEL0, SPI_DEVICE_0, SPI_CHIP_SELECT_3, data_buff, length / 4, 1);
// }

static void sd_write_data_dma_no_wait(uint8 const *data_buff, uint32 length) {
    spi_init(SPI_DEVICE_0, SPI_WORK_MODE_0, SPI_FF_STANDARD, 32, 1);
	spi_send_data_no_cmd_dma(DMAC_CHANNEL0, SPI_DEVICE_0, SPI_CHIP_SELECT_3, data_buff, length / 4, 0);
}

static void sd_read_data_dma(uint8 *data_buff, uint32 length) {
    // spi_init(SPI_DEVICE_0, SPI_WORK_MODE_0, SPI_FF_STANDARD, 8, 0);
	// spi_receive_data_standard_dma(-1, DMAC_CHANNEL0, SPI_DEVICE_0, SPI_CHIP_SELECT_3, NULL, 0, data_buff, length);
    spi_init(SPI_DEVICE_0, SPI_WORK_MODE_0, SPI_FF_STANDARD, 32, 1);
	spi_receive_data_no_cmd_dma(DMAC_CHANNEL0, SPI_DEVICE_0, SPI_CHIP_SELECT_3, data_buff, length / 4);
}

/*
 * @brief  Send 5 bytes command to the SD card.
 * @param  Cmd: The user expected command to send to SD card.
 * @param  Arg: The command argument.
 * @param  Crc: The CRC.
 * @retval None
 */
static void sd_send_cmd(uint8 cmd, uint32 arg, uint8 crc) {
	uint8 frame[6];
	frame[0] = (cmd | 0x40);
	frame[1] = (uint8)(arg >> 24);
	frame[2] = (uint8)(arg >> 16);
	frame[3] = (uint8)(arg >> 8);
	frame[4] = (uint8)(arg);
	frame[5] = (crc);
	SD_CS_LOW();
	sd_write_data(frame, 6);
}

static void sd_end_cmd(void) {
	uint8 frame[1] = {0xFF};
	/*!< SD chip select high */
	SD_CS_HIGH();
	/*!< Send the Cmd bytes */
	sd_write_data(frame, 1);
}

/*
 * Be noticed: all commands & responses below 
 * 		are in SPI mode format. May differ from 
 * 		what they are in SD mode. 
 */

#define SD_CMD0 	0 
#define SD_CMD8 	8
#define SD_CMD58 	58 		// READ_OCR
#define SD_CMD55 	55 		// APP_CMD
#define SD_ACMD41 	41 		// SD_SEND_OP_COND
#define SD_CMD16 	16 		// SET_BLOCK_SIZE 
#define SD_CMD17 	17 		// READ_SINGLE_BLOCK
#define SD_CMD24 	24 		// WRITE_SINGLE_BLOCK 
#define SD_CMD13 	13 		// SEND_STATUS
#define SD_CMD18 	18 		// READ_MULTIPLE_BLOCK
#define SD_CMD25 	25 		// WRITE_MULTIPLE_BLOCK
#define SD_CMD12 	12 		// STOP_TRANSMISSION
#define SD_ACMD23 	23 		// SET_WR_BLK_ERASE_COUNT

/*
 * Read sdcard response in R1 type. 
 */
static uint8 sd_get_response_R1(void) {
	uint8 result = 0xff;
	uint16 timeout = 0xff;

	while (timeout--) {
		sd_read_data(&result, 1);
		if (result != 0xff)
			return result;
	}

	// timeout!
	return 0xff;
}

/* 
 * Read the rest of R3 response 
 * Be noticed: frame should be at least 4-byte long 
 */
static void sd_get_response_R3_rest(uint8 *frame) {
	sd_read_data(frame, 4);
}

/* 
 * Read the rest of R7 response 
 * Be noticed: frame should be at least 4-byte long 
 */
static void sd_get_response_R7_rest(uint8 *frame) {
	sd_read_data(frame, 4);
}

static int switch_to_SPI_mode(void) {
	int timeout = 0xff;

	while (--timeout) {
		sd_send_cmd(SD_CMD0, 0, 0x95);
		uint64 result = sd_get_response_R1();
		sd_end_cmd();

		if (0x01 == result) break;
	}
	if (0 == timeout) {
		printf("SD_CMD0 failed\n");
		return 0xff;
	}

	return 0;
}

// verify supply voltage range 
static int verify_operation_condition(void) {
	uint64 result = 0xff;

	// Stores the response reversely. 
	// That means 
	// frame[2] - VCA 
	// frame[3] - Check Pattern 
	uint8 frame[4];

	sd_send_cmd(SD_CMD8, 0x01aa, 0x87);
	result = sd_get_response_R1();
	sd_get_response_R7_rest(frame);
	sd_end_cmd();

	if (0x09 == result) {
		printf("invalid CRC for CMD8\n");
		return 0xff;
	}
	else if (0x01 == result && 0x01 == (frame[2] & 0x0f) && 0xaa == frame[3]) {
		return 0x00;
	}

	printf("verify_operation_condition() fail!\n");
	return 0xff;
}

// read OCR register to check if the voltage range is valid 
// this step is not mandotary, but I advise to use it 
static int read_OCR(void) {
	uint64 result = 0xff;
	uint8 ocr[4];

	int timeout = 0xffffff;
	while (--timeout) {
		sd_send_cmd(SD_CMD58, 0, 0);
		result = sd_get_response_R1();
		sd_get_response_R3_rest(ocr);
		sd_end_cmd();

		if (
			0x01 == result && // R1 response in idle status 
			(ocr[1] & 0x1f) && (ocr[2] & 0x80) 	// voltage range valid 
		) {
			return 0;
		}
	}

	// timeout!
	printf("read_OCR() timeout!\n");
	printf("result = %d\n", result);
	return 0xff;
}

// send ACMD41 to tell sdcard to finish initializing 
static int set_SDXC_capacity(void) {
	uint8 result = 0xff;

	int timeout = 0xffffff;
	while (--timeout) {
		sd_send_cmd(SD_CMD55, 0, 0);
		result = sd_get_response_R1();
		sd_end_cmd();
		if (0x01 != result) {
			printf("SD_CMD55 fail! result = %d\n", result);
			return 0xff;
		}

		sd_send_cmd(SD_ACMD41, 0x40000000, 0);
		result = sd_get_response_R1();
		sd_end_cmd();
		if (0 == result) {
			return 0;
		}
	}

	// timeout! 
	printf("set_SDXC_capacity() timeout!\n");
	printf("result = %d\n", result);
	return 0xff;
}

// Used to differ whether sdcard is SDSC type. 
static int is_standard_sd = 0;

// check OCR register to see the type of sdcard, 
// thus determine whether block size is suitable to buffer size
static int check_block_size(void) {
	uint8 result = 0xff;
	uint8 ocr[4];

	int timeout = 0xffffff;
	while (timeout --) {
		sd_send_cmd(SD_CMD58, 0, 0);
		result = sd_get_response_R1();
		sd_get_response_R3_rest(ocr);
		sd_end_cmd();

		if (0 == result) {
			if (ocr[0] & 0x40) {
				printf("SDHC/SDXC detected\n");
				if (512 != BSIZE) {
					printf("BSIZE != 512\n");
					return 0xff;
				}

				is_standard_sd = 0;
			}
			else {
				printf("SDSC detected, setting block size\n");

				// setting SD card block size to BSIZE 
				int timeout = 0xff;
				int result = 0xff;
				while (--timeout) {
					sd_send_cmd(SD_CMD16, BSIZE, 0);
					result = sd_get_response_R1();
					sd_end_cmd();

					if (0 == result) break;
				}
				if (0 == timeout) {
					printf("check_OCR(): fail to set block size");
					return 0xff;
				}

				is_standard_sd = 1;
			}

			return 0;
		}
	}

	// timeout! 
	printf("check_OCR() timeout!\n");
	printf("result = %d\n", result);
	return 0xff;
}

/*
 * @brief  Initializes the SD/SD communication.
 * @param  None
 * @retval The SD Response:
 *         - 0xFF: Sequence failed
 *         - 0: Sequence succeed
 */
static int sd_init(void) {
	uint8 frame[10];

	sd_lowlevel_init(0);
	//SD_CS_HIGH();
	SD_CS_LOW();

	// send dummy bytes for 80 clock cycles 
	for (int i = 0; i < 10; i ++) 
		frame[i] = 0xff;
	sd_write_data(frame, 10);

	if (0 != switch_to_SPI_mode()) 
		return 0xff;
	if (0 != verify_operation_condition()) 
		return 0xff;
	if (0 != read_OCR()) 
		return 0xff;
	if (0 != set_SDXC_capacity()) 
		return 0xff;
	if (0 != check_block_size()) 
		return 0xff;
	SD_HIGH_SPEED_ENABLE();
	return 0;
}

static struct sleeplock sdcard_lock;

// Protected by sdcard_lock above.
static struct {
	int busy;
	int rpending;
	int wpending;
	int wcount;
} sd_status;

// Protected by their own locks.
static struct wait_queue sd_rqueue;
static struct wait_queue sd_wqueue;
static int sd_wqueue_num;

void sdcard_init(void) {
	int result = sd_init();
	initsleeplock(&sdcard_lock, "sdcard");
	wait_queue_init(&sd_rqueue, "sdrq");
	wait_queue_init(&sd_wqueue, "sdwq");
	sd_status.busy = 0;
	sd_status.rpending = 0;
	sd_status.wpending = 0;
	sd_status.wcount = 0;
	sd_wqueue_num = 0;

	if (0 != result) {
		panic("sdcard_init failed");
	}
	#ifdef DEBUG
	printf("sdcard_init\n");
	#endif
}

/*
void sdcard_read_sector(uint8 *buf, int sectorno) {
	uint8 result = 0xff;
	uint32 address;
	uint8 dummy_crc[2];

	__debug_info("sdcard_read_sector", "sector num: %d\n", sectorno);

	if (is_standard_sd) {
		address = sectorno << 9;
	}
	else {
		address = sectorno;
	}

	// enter critical section!
	acquiresleep(&sdcard_lock);

	sd_send_cmd(SD_CMD17, address, 0);
	result = sd_get_response_R1();

	if (0 != result) {
		releasesleep(&sdcard_lock);
		panic("sdcard: fail to read");
	}

	int timeout = 0xffffff;
	while (--timeout) {
		sd_read_data(&result, 1);
		if (0xfe == result) break;
	}
	if (0 == timeout) {
		panic("sdcard: timeout waiting for reading");
	}
	sd_read_data_dma(buf, BSIZE);
	sd_read_data(dummy_crc, 2);

	sd_end_cmd();

	releasesleep(&sdcard_lock);
	// leave critical section!
}

void sdcard_write_sector(uint8 *buf, int sectorno) {
	uint32 address;
	static uint8 const START_BLOCK_TOKEN = 0xfe;
	uint8 dummy_crc[2] = {0xff, 0xff};

	__debug_info("sdcard_write_sector", "sector num: %d\n", sectorno);

	if (is_standard_sd) {
		address = sectorno << 9;
	}
	else {
		address = sectorno;
	}

	// enter critical section!
	acquiresleep(&sdcard_lock);

	sd_send_cmd(SD_CMD24, address, 0);
	if (0 != sd_get_response_R1()) {
		releasesleep(&sdcard_lock);
		panic("sdcard: fail to write");
	}

	// sending data to be written 
	sd_write_data(&START_BLOCK_TOKEN, 1);
	sd_write_data_dma(buf, BSIZE);
	sd_write_data(dummy_crc, 2);

	// waiting for sdcard to finish programming 
	uint8 result = 0xff;
	int timeout = 0xffffff;
	while (--timeout) {
		sd_read_data(&result, 1);
		if (0x05 == (result & 0x1f)) {
			break;
		}
	}
	if (0 == timeout) {
		releasesleep(&sdcard_lock);
		panic("sdcard: invalid response token");
	}
	
	timeout = 0xffffff;
	while (--timeout) {
		sd_read_data(&result, 1);
		if (0 != result) break;
	}
	if (0 == timeout) {
		releasesleep(&sdcard_lock);
		panic("sdcard: timeout waiting for response");
	}
	sd_end_cmd();

	// send SD_CMD13 to check if writing is correctly done 
	uint8 error_code = 0xff;
	sd_send_cmd(SD_CMD13, 0, 0);
	result = sd_get_response_R1();
	sd_read_data(&error_code, 1);
	sd_end_cmd();
	if (0 != result || 0 != error_code) {
		releasesleep(&sdcard_lock);
		printf("result: %x\n", result);
		printf("error_code: %x\n", error_code);
		panic("sdcard: an error occurs when writing");
	}

	releasesleep(&sdcard_lock);
	// leave critical section!
}

// A simple test for sdcard read/write test 
void test_sdcard(void) {
	uint8 buf[BSIZE];

	for (int sec = 0; sec < 5; sec ++) {
		for (int i = 0; i < BSIZE; i ++) {
			buf[i] = 0xaa;		// data to be written 
		}

		sdcard_write_sector(buf, sec);

		for (int i = 0; i < BSIZE; i ++) {
			buf[i] = 0xff;		// fill in junk
		}

		sdcard_read_sector(buf, sec);
		for (int i = 0; i < BSIZE; i ++) {
			if (0 == i % 16) {
				printf("\n");
			}

			printf("%x ", buf[i]);
		}
		printf("\n");
	}

	while (1) ;
}
*/


// for interrupt handler
#define BUSY_READ	1
#define BUSY_WRITE	2


static void sdcard_read_wait(struct buf *b)
{
	acquire(&sd_rqueue.lock);
	// Register to read queue, add to tail.
	dlist_add_before(&sd_rqueue.head, &b->list);
	// whether we are the first one
	while (&b->list != sd_rqueue.head.next)
		// sleep on list to get wakened orderly, unlike sleeplock
		sleep(&b->list, &sd_rqueue.lock);
	
	// watch out the sequence
	acquire(&sdcard_lock.lk);
	release(&sd_rqueue.lock);

	// Check sdcard working status.
	while (sd_status.busy) {
		if (sd_status.busy != BUSY_WRITE)
			panic("sdcard_read_wait: status");
		// let writers see us
		sd_status.rpending = 1;
		sleep(&sd_status.rpending, &sdcard_lock.lk);
	}
	sd_status.rpending = 0;
	sd_status.busy = BUSY_READ;
	release(&sdcard_lock.lk);
}


static int sdcard_read_done(struct buf *b)
{
	acquire(&sd_rqueue.lock);
	if (&b->list != sd_rqueue.head.next)
		panic("sdcard_read_done: wrong order");
	
	dlist_del(&b->list);

	struct d_list *dl = sd_rqueue.head.next;
	int done = 0;
	// more readers?
	acquire(&sdcard_lock.lk);
	sd_status.busy = 0;
	if (dl != &sd_rqueue.head) {
		wakeup(dl);
		sd_status.rpending = 1;
	}
	else if (sd_status.wpending)
		done = 1;

	release(&sd_rqueue.lock);
	release(&sdcard_lock.lk);
	return done;
}


int sdcard_read(struct buf *b)
{
	int ret = -EIO;
	uint8 dummy_crc[2];
	uint8 result = 0xff;
	uint32 address = is_standard_sd ?
						b->sectorno << 9 :
						b->sectorno;

	// wait our turn
	sdcard_read_wait(b);

	sd_send_cmd(SD_CMD17, address, 0);
	if (sd_get_response_R1() != 0) {
		printf(__ERROR("sdcard")" CMD17 fail\n");	// we'd better print it
		goto end;
	}

	for (int timeout = 0xffff; timeout > 0 && result != 0xfe; timeout--)
		sd_read_data(&result, 1);
		
	if (result != 0xfe) {
		printf(__ERROR("sdcard")" CMD17 read fail\n");
		goto end;
	}

	sd_read_data_dma(b->data, BSIZE);
	sd_read_data(dummy_crc, 2);
	ret = 0;

end:
	sd_end_cmd();
	if (sdcard_read_done(b))
		sdcard_write_start();
	return ret;
}


int sdcard_read_sectors(struct buf * restrict bufs[], int nbuf)
{
	int ret = 0;
	uint32 address = is_standard_sd ?
						bufs[0]->sectorno << 9 :
						bufs[0]->sectorno;

	// wait our turn
	sdcard_read_wait(bufs[0]);

	sd_send_cmd(SD_CMD18, address, 0);
	if (sd_get_response_R1() != 0) {
		ret = -EIO;
		printf(__WARN("sdread")" response! sec %d\n", bufs[0]->sectorno);
		goto out;
	}

	for (int i = 0; i < nbuf; i++) {
		uint8 result = 0xff;
		int timeout = 0xffff;
		while (timeout-- > 0 && result != 0xfe)
			sd_read_data(&result, 1);

		if (result != 0xfe) {
			ret = -EIO;
			printf(__WARN("sdread")" %d\n", bufs[i]->sectorno);
			break;
		}

		sd_read_data_dma(bufs[i]->data, BSIZE);
		
		uint8 dummy_crc[2];
		sd_read_data(dummy_crc, 2);
	}
	sd_end_cmd();

	sd_send_cmd(SD_CMD12, 0, 0);
	sd_get_response_R1();
	sd_end_cmd();	// this is needed

out:
	sd_end_cmd();
	if (sdcard_read_done(bufs[0]))
		sdcard_write_start();
	return ret;
}


static void sdcard_write(struct buf *b)
{
	// uint8 start_token = 0xfe;
	uint8 start_token = 0xfc;
	sd_write_data(&start_token, 1);

	// we don't block inside
	sd_write_data_dma_no_wait(b->data, BSIZE);
}


// Caller must hold the control before calling to here
static int sdcard_multiple_write(struct buf *b, int nbuf)
{
	uint32 address = is_standard_sd ?
						b->sectorno << 9 :
						b->sectorno;

	if (nbuf >= 16) {
		int ret;
		sd_send_cmd(SD_CMD55, 0, 0);
		ret = sd_get_response_R1();
		sd_end_cmd();
		if (ret != 0) {
			printf(__ERROR("CMD55")" error\n");
			return -EIO;
		}

		sd_send_cmd(SD_ACMD23, nbuf, 0);
		ret = sd_get_response_R1();
		sd_end_cmd();
		if (ret != 0) {
			printf(__ERROR("ACMD23")" error\n");
			return -EIO;
		}
	}

	sd_send_cmd(SD_CMD25, address, 0);
	if (sd_get_response_R1() != 0) {
		sd_end_cmd();
		printf(__ERROR("CMD25")" error\n");
		return -EIO;
	}
	
	sd_status.wcount = nbuf;
	sdcard_write(b);

	// see you in the interrupt handler	
	return 0;
}


// clean up last write
static void sdcard_multiple_write_wait(void)
{
	dmac_ch_clear_intr(DMAC_CHANNEL0, 0);
	spi_send_data_dma_clean_up(SPI_DEVICE_0);

	uint8 dummy_crc[2] = {0xff, 0xff};
	sd_write_data(dummy_crc, 2);

	// waiting sdcard programming, dma can't do this
	uint8 result = 0xff;
	int64 timeout;
	for (timeout = 0xffffff; timeout >= 0 && (result & 0x1f) != 0x5; timeout--)
		sd_read_data(&result, 1);
	// if (timeout < 0)
	// 	panic("sdcard_intr: response 1");	// really don't know what to do
	
	for (timeout = 0xffffff, result = 0; timeout >= 0 && result == 0; timeout--)
		sd_read_data(&result, 1);
	// if (timeout < 0)
	// 	panic("sdcard_intr: response 2");	// really don't know what to do
}


static void sdcard_multiple_write_stop(void)
{
	uint8 token = 0xfd;		// stop token
	sd_write_data(&token, 1);

	for (int timeout = 0xffff; timeout >= 0 && token != 0xff; timeout--)
		sd_read_data(&token, 1);
	
	sd_end_cmd();

	sd_send_cmd(SD_CMD12, 0, 0);
	sd_get_response_R1();
	sd_end_cmd();

	// check writing result
	sd_send_cmd(SD_CMD13, 0, 0);
	if (sd_get_response_R1() != 0)
		panic("sdcard_intr: CMD13 bad response");

	uint8 error = 0xff;
	sd_read_data(&error, 1);
	sd_end_cmd();
	if (error) {
		printf(__ERROR("sdcard_intr")" write error %x\n", error);
		panic("sdcard_intr");
	}
}

#define	WRITE_SHRESHOLD	512

void sdcard_write_start(void)
{
	acquire(&sd_wqueue.lock);

	struct d_list *dl = sd_wqueue.head.next;
	// is the queue empty?
	if (dl == &sd_wqueue.head || sd_wqueue_num < WRITE_SHRESHOLD) {
		release(&sd_wqueue.lock);
		return;
	}

	acquire(&sdcard_lock.lk);
	// check sdcard status
	if (sd_status.busy || sd_status.rpending) {
		/**
		 * If we enter here where busy == 0 and rpending is non-zero,
		 * it means this rpending is set in sdcard_read_done(), where
		 * the proc there will waken the next reader. Thus, we don't
		 * need to do a wake up.
		 */
		sd_status.wpending = 1;
		release(&sdcard_lock.lk);
		release(&sd_wqueue.lock);
		return;
	}

	// we get the control now
	sd_status.busy = BUSY_WRITE;
	release(&sdcard_lock.lk);

	struct buf *b = container_of(dl, struct buf, list);
	int nbuf = 1;

	#define __NBUF_WRITE	32

	uint32 sec = b->sectorno;
	for (dl = dl->next; dl != &sd_wqueue.head && nbuf < __NBUF_WRITE; dl = dl->next) {
		struct buf *next = container_of(dl, struct buf, list);
		if (next->sectorno == ++sec)	// consecutive sector
			nbuf++;
		else
			break;
	}
	
	b->disk = 1;
	b->dirty = 0;
	release(&sd_wqueue.lock);

	if (sdcard_multiple_write(b, nbuf) < 0) {
		// unset status
		acquire(&sd_wqueue.lock);
		b->disk = 0;
		b->dirty = 1;

		acquire(&sdcard_lock.lk);
		release(&sd_wqueue.lock);

		sd_status.busy = 0;
		sd_status.wpending = 1;
		if (sd_status.rpending)
			/**
			 * We enter here because rpending is non-zero.
			 * But this case is different, since the busy
			 * flag was set by us. So this rpending is set
			 * in sdcard_read(), where there is a reader
			 * waiting for the control, and only us can
			 * wake him up.
			 */
			wakeup(&sd_status.rpending);
		release(&sdcard_lock.lk);
	}

}


int sdcard_submit(struct buf *b)
{
	int res = 0;
	acquire(&sd_wqueue.lock);

	if (b->dirty == 0 && b->disk == 0) {
		dlist_add_before(&sd_wqueue.head, &b->list);
		res = 0x1;
		sd_wqueue_num++;
	}
	b->dirty = 1;

	acquire(&sdcard_lock.lk);
	release(&sd_wqueue.lock);
	sd_status.wpending = 1;
	if (sd_status.busy)	// tell the caller that transmission is on
		res |= 0x2;
	release(&sdcard_lock.lk);
	return res;
}


// Called by dmac_intr(), if it is a read done,
// tell him to wake up the relative proc.
void sdcard_intr(void)
{
	// acquire(&sdcard_lock.lk);
	if (sd_status.busy == BUSY_READ) {
		// release(&sdcard_lock.lk);
		dmac_ch_clear_intr(DMAC_CHANNEL0, 1);
		// reader is waiting out there
		// him will do the clean work himself
		return;
	} else if (sd_status.busy != BUSY_WRITE)
		panic("sdcard_intr: unknown busy");
	
	// but writer doesn't wait, so we need to clean up for him
	// release(&sdcard_lock.lk);

	// waiting for programming
	sdcard_multiple_write_wait();

	acquire(&sd_wqueue.lock);
	struct d_list *dl = sd_wqueue.head.next;
	struct buf *b = container_of(dl, struct buf, list);
	struct buf *bnext;
	int redo = 0;
	
	dlist_del(dl);
	b->disk = 0;
	// whether some one wrote this buf when we transmitting it
	if (b->dirty) {
		redo = 1;
		dlist_add_before(&sd_wqueue.head, dl);
	} else {
		sd_wqueue_num--;
	}

	dl = sd_wqueue.head.next;
	bnext = (dl == &sd_wqueue.head) ? NULL :
			container_of(dl, struct buf, list);
	
	if (--sd_status.wcount > 0) {
		// if (bnext == NULL || bnext->sectorno != b->sectorno + 1) {
		// 	sdcard_multiple_write_stop();
		// 	panic("sdcard_intr: bad sd_status.wcount");
		// }
		bnext->dirty = 0;
		bnext->disk = 1;
		release(&sd_wqueue.lock);
		sdcard_write(bnext);
		goto out;
	}
	else if (sd_status.rpending || bnext == NULL ||
			sd_wqueue_num < WRITE_SHRESHOLD / 4)
	{	// race read
		release(&sd_wqueue.lock);
		sdcard_multiple_write_stop();
	}
	else {
		int nbuf = 1;
		uint32 sec = bnext->sectorno;

		// preview the consecutive count
		for (dl = dl->next; dl != &sd_wqueue.head && nbuf < __NBUF_WRITE; dl = dl->next) {
			struct buf *next = container_of(dl, struct buf, list);
			if (next->sectorno == ++sec)	// consecutive sector
				nbuf++;
			else
				break;
		}

		bnext->dirty = 0;
		bnext->disk = 1;
		release(&sd_wqueue.lock);
		// printf("nbuf = %d\n", nbuf);
		if (nbuf < 16 && bnext->sectorno == b->sectorno + 1) {
			sd_status.wcount = nbuf;
			sdcard_write(bnext);
		} else {
			sdcard_multiple_write_stop();

			if (sdcard_multiple_write(bnext, nbuf) < 0) {
				// unset status
				acquire(&sd_wqueue.lock);
				bnext->dirty = 1;
				bnext->disk = 0;
				release(&sd_wqueue.lock);
				goto stop_out;
			}
		}
		goto out;
	}

stop_out:
	acquire(&sdcard_lock.lk);
	sd_status.busy = 0;
	if (sd_status.rpending)
		wakeup(&sd_status.rpending);
	if (bnext)
		sd_status.wpending = 1;
	release(&sdcard_lock.lk);

out:
	if (!redo)
		bput(b);
}

int sdcard_wqueue_empty(void)
{
	// race test
	return sd_wqueue.head.next == &sd_wqueue.head && 
			sd_wqueue.head.prev == &sd_wqueue.head;
}