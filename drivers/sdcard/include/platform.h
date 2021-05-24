/* Copyright 2018 Canaan Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _BSP_PLATFORM_H
#define _BSP_PLATFORM_H

#ifdef __cplusplus
extern "C" {
#endif

/* clang-format off */
/* Register base address */

/* Under Coreplex */
#define CLINT_BASE_ADDR     (0xffffffff02000000U)
#define PLIC_BASE_ADDR      (0xffffffff0C000000U)

/* Under TileLink */
#define UARTHS_BASE_ADDR    (0xffffffff38000000U)
#define GPIOHS_BASE_ADDR    (0xffffffff38001000U)

/* Under AXI 64 bit */
#define RAM_BASE_ADDR       (0xffffffff80000000U)
#define RAM_SIZE            (6 * 1024 * 1024U)

#define IO_BASE_ADDR        (0xffffffff40000000U)
#define IO_SIZE             (6 * 1024 * 1024U)

#define AI_RAM_BASE_ADDR    (0xffffffff80600000U)
#define AI_RAM_SIZE         (2 * 1024 * 1024U)

#define AI_IO_BASE_ADDR     (0xffffffff40600000U)
#define AI_IO_SIZE          (2 * 1024 * 1024U)

#define AI_BASE_ADDR        (0xffffffff40800000U)
#define AI_SIZE             (12 * 1024 * 1024U)

#define FFT_BASE_ADDR       (0xffffffff42000000U)
#define FFT_SIZE            (4 * 1024 * 1024U)

#define ROM_BASE_ADDR       (0xffffffff88000000U)
#define ROM_SIZE            (128 * 1024U)

/* Under AHB 32 bit */
#define DMAC_BASE_ADDR      (0xffffffff50000000U)

/* Under APB1 32 bit */
#define GPIO_BASE_ADDR      (0xffffffff50200000U)
#define UART1_BASE_ADDR     (0xffffffff50210000U)
#define UART2_BASE_ADDR     (0xffffffff50220000U)
#define UART3_BASE_ADDR     (0xffffffff50230000U)
#define SPI_SLAVE_BASE_ADDR (0xffffffff50240000U)
#define I2S0_BASE_ADDR      (0xffffffff50250000U)
#define I2S1_BASE_ADDR      (0xffffffff50260000U)
#define I2S2_BASE_ADDR      (0xffffffff50270000U)
#define I2C0_BASE_ADDR      (0xffffffff50280000U)
#define I2C1_BASE_ADDR      (0xffffffff50290000U)
#define I2C2_BASE_ADDR      (0xffffffff502A0000U)
#define FPIOA_BASE_ADDR     (0xffffffff502B0000U)
#define SHA256_BASE_ADDR    (0xffffffff502C0000U)
#define TIMER0_BASE_ADDR    (0xffffffff502D0000U)
#define TIMER1_BASE_ADDR    (0xffffffff502E0000U)
#define TIMER2_BASE_ADDR    (0xffffffff502F0000U)

/* Under APB2 32 bit */
#define WDT0_BASE_ADDR      (0xffffffff50400000U)
#define WDT1_BASE_ADDR      (0xffffffff50410000U)
#define OTP_BASE_ADDR       (0xffffffff50420000U)
#define DVP_BASE_ADDR       (0xffffffff50430000U)
#define SYSCTL_BASE_ADDR    (0xffffffff50440000U)
#define AES_BASE_ADDR       (0xffffffff50450000U)
#define RTC_BASE_ADDR       (0xffffffff50460000U)


/* Under APB3 32 bit */
#define SPI0_BASE_ADDR      (0xffffffff52000000U)
#define SPI1_BASE_ADDR      (0xffffffff53000000U)
#define SPI3_BASE_ADDR      (0xffffffff54000000U)

/* clang-format on */

#ifdef __cplusplus
}
#endif

#endif /* _BSP_PLATFORM_H */
