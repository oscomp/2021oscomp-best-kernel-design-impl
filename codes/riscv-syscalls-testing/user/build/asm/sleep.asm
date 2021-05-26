
/home/ubuntu/rust/ultra-os/codes/riscv-syscalls-testing/user/build/riscv64/sleep:     file format elf64-littleriscv


Disassembly of section .text:

0000000000001000 <_start>:
.section .text.entry
.globl _start
_start:
    mv a0, sp
    1000:	850a                	mv	a0,sp
    tail __start_main
    1002:	a8f1                	j	10de <__start_main>

0000000000001004 <test_sleep>:
 * 测试通过时的输出：
 * "sleep success."
 * 测试失败时的输出：
 * "sleep error."
 */
void test_sleep() {
    1004:	1101                	addi	sp,sp,-32
	TEST_START(__func__);
    1006:	00001517          	auipc	a0,0x1
    100a:	f1250513          	addi	a0,a0,-238 # 1f18 <__clone+0x2c>
void test_sleep() {
    100e:	ec06                	sd	ra,24(sp)
    1010:	e426                	sd	s1,8(sp)
    1012:	e822                	sd	s0,16(sp)
	TEST_START(__func__);
    1014:	33c000ef          	jal	ra,1350 <puts>
    1018:	00001517          	auipc	a0,0x1
    101c:	f9850513          	addi	a0,a0,-104 # 1fb0 <__func__.0>
    1020:	330000ef          	jal	ra,1350 <puts>
    1024:	00001517          	auipc	a0,0x1
    1028:	f0c50513          	addi	a0,a0,-244 # 1f30 <__clone+0x44>
    102c:	324000ef          	jal	ra,1350 <puts>

	int time1 = get_time();
    1030:	513000ef          	jal	ra,1d42 <get_time>
    1034:	0005049b          	sext.w	s1,a0
	assert(time1 >= 0);
    1038:	0804c463          	bltz	s1,10c0 <test_sleep+0xbc>
	int ret = sleep(1);
    103c:	4505                	li	a0,1
    103e:	54b000ef          	jal	ra,1d88 <sleep>
	assert(ret == 0);
    1042:	ed21                	bnez	a0,109a <test_sleep+0x96>
	int time2 = get_time();
    1044:	4ff000ef          	jal	ra,1d42 <get_time>
    1048:	0005041b          	sext.w	s0,a0
	assert(time2 >= 0);
    104c:	06044363          	bltz	s0,10b2 <test_sleep+0xae>

	if(time2 - time1 >= 1){	
    1050:	9c05                	subw	s0,s0,s1
    1052:	02805d63          	blez	s0,108c <test_sleep+0x88>
		printf("sleep success.\n");
    1056:	00001517          	auipc	a0,0x1
    105a:	f0a50513          	addi	a0,a0,-246 # 1f60 <__clone+0x74>
    105e:	314000ef          	jal	ra,1372 <printf>
	}else{
		printf("sleep error.\n");
	}
	TEST_END(__func__);
    1062:	00001517          	auipc	a0,0x1
    1066:	f1e50513          	addi	a0,a0,-226 # 1f80 <__clone+0x94>
    106a:	2e6000ef          	jal	ra,1350 <puts>
    106e:	00001517          	auipc	a0,0x1
    1072:	f4250513          	addi	a0,a0,-190 # 1fb0 <__func__.0>
    1076:	2da000ef          	jal	ra,1350 <puts>
}
    107a:	6442                	ld	s0,16(sp)
    107c:	60e2                	ld	ra,24(sp)
    107e:	64a2                	ld	s1,8(sp)
	TEST_END(__func__);
    1080:	00001517          	auipc	a0,0x1
    1084:	eb050513          	addi	a0,a0,-336 # 1f30 <__clone+0x44>
}
    1088:	6105                	addi	sp,sp,32
	TEST_END(__func__);
    108a:	a4d9                	j	1350 <puts>
		printf("sleep error.\n");
    108c:	00001517          	auipc	a0,0x1
    1090:	ee450513          	addi	a0,a0,-284 # 1f70 <__clone+0x84>
    1094:	2de000ef          	jal	ra,1372 <printf>
    1098:	b7e9                	j	1062 <test_sleep+0x5e>
	assert(ret == 0);
    109a:	00001517          	auipc	a0,0x1
    109e:	ea650513          	addi	a0,a0,-346 # 1f40 <__clone+0x54>
    10a2:	554000ef          	jal	ra,15f6 <panic>
	int time2 = get_time();
    10a6:	49d000ef          	jal	ra,1d42 <get_time>
    10aa:	0005041b          	sext.w	s0,a0
	assert(time2 >= 0);
    10ae:	fa0451e3          	bgez	s0,1050 <test_sleep+0x4c>
    10b2:	00001517          	auipc	a0,0x1
    10b6:	e8e50513          	addi	a0,a0,-370 # 1f40 <__clone+0x54>
    10ba:	53c000ef          	jal	ra,15f6 <panic>
    10be:	bf49                	j	1050 <test_sleep+0x4c>
	assert(time1 >= 0);
    10c0:	00001517          	auipc	a0,0x1
    10c4:	e8050513          	addi	a0,a0,-384 # 1f40 <__clone+0x54>
    10c8:	52e000ef          	jal	ra,15f6 <panic>
    10cc:	bf85                	j	103c <test_sleep+0x38>

00000000000010ce <main>:

int main(void) {
    10ce:	1141                	addi	sp,sp,-16
    10d0:	e406                	sd	ra,8(sp)
	test_sleep();
    10d2:	f33ff0ef          	jal	ra,1004 <test_sleep>
	return 0;
}
    10d6:	60a2                	ld	ra,8(sp)
    10d8:	4501                	li	a0,0
    10da:	0141                	addi	sp,sp,16
    10dc:	8082                	ret

00000000000010de <__start_main>:
#include <unistd.h>

extern int main();

int __start_main(long *p)
{
    10de:	85aa                	mv	a1,a0
	int argc = p[0];
	char **argv = (void *)(p+1);

	exit(main(argc, argv));
    10e0:	4108                	lw	a0,0(a0)
{
    10e2:	1141                	addi	sp,sp,-16
	exit(main(argc, argv));
    10e4:	05a1                	addi	a1,a1,8
{
    10e6:	e406                	sd	ra,8(sp)
	exit(main(argc, argv));
    10e8:	fe7ff0ef          	jal	ra,10ce <main>
    10ec:	41d000ef          	jal	ra,1d08 <exit>
	return 0;
}
    10f0:	60a2                	ld	ra,8(sp)
    10f2:	4501                	li	a0,0
    10f4:	0141                	addi	sp,sp,16
    10f6:	8082                	ret

00000000000010f8 <printint.constprop.0>:
    write(f, s, l);
}

static char digits[] = "0123456789abcdef";

static void printint(int xx, int base, int sign)
    10f8:	7179                	addi	sp,sp,-48
    10fa:	f406                	sd	ra,40(sp)
{
    char buf[16 + 1];
    int i;
    uint x;

    if (sign && (sign = xx < 0))
    10fc:	12054b63          	bltz	a0,1232 <printint.constprop.0+0x13a>

    buf[16] = 0;
    i = 15;
    do
    {
        buf[i--] = digits[x % base];
    1100:	02b577bb          	remuw	a5,a0,a1
    1104:	00001617          	auipc	a2,0x1
    1108:	ebc60613          	addi	a2,a2,-324 # 1fc0 <digits>
    buf[16] = 0;
    110c:	00010c23          	sb	zero,24(sp)
        buf[i--] = digits[x % base];
    1110:	0005871b          	sext.w	a4,a1
    1114:	1782                	slli	a5,a5,0x20
    1116:	9381                	srli	a5,a5,0x20
    1118:	97b2                	add	a5,a5,a2
    111a:	0007c783          	lbu	a5,0(a5)
    } while ((x /= base) != 0);
    111e:	02b5583b          	divuw	a6,a0,a1
        buf[i--] = digits[x % base];
    1122:	00f10ba3          	sb	a5,23(sp)
    } while ((x /= base) != 0);
    1126:	1cb56363          	bltu	a0,a1,12ec <printint.constprop.0+0x1f4>
        buf[i--] = digits[x % base];
    112a:	45b9                	li	a1,14
    112c:	02e877bb          	remuw	a5,a6,a4
    1130:	1782                	slli	a5,a5,0x20
    1132:	9381                	srli	a5,a5,0x20
    1134:	97b2                	add	a5,a5,a2
    1136:	0007c783          	lbu	a5,0(a5)
    } while ((x /= base) != 0);
    113a:	02e856bb          	divuw	a3,a6,a4
        buf[i--] = digits[x % base];
    113e:	00f10b23          	sb	a5,22(sp)
    } while ((x /= base) != 0);
    1142:	0ce86e63          	bltu	a6,a4,121e <printint.constprop.0+0x126>
        buf[i--] = digits[x % base];
    1146:	02e6f5bb          	remuw	a1,a3,a4
    } while ((x /= base) != 0);
    114a:	02e6d7bb          	divuw	a5,a3,a4
        buf[i--] = digits[x % base];
    114e:	1582                	slli	a1,a1,0x20
    1150:	9181                	srli	a1,a1,0x20
    1152:	95b2                	add	a1,a1,a2
    1154:	0005c583          	lbu	a1,0(a1)
    1158:	00b10aa3          	sb	a1,21(sp)
    } while ((x /= base) != 0);
    115c:	0007859b          	sext.w	a1,a5
    1160:	12e6ec63          	bltu	a3,a4,1298 <printint.constprop.0+0x1a0>
        buf[i--] = digits[x % base];
    1164:	02e7f6bb          	remuw	a3,a5,a4
    1168:	1682                	slli	a3,a3,0x20
    116a:	9281                	srli	a3,a3,0x20
    116c:	96b2                	add	a3,a3,a2
    116e:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    1172:	02e7d83b          	divuw	a6,a5,a4
        buf[i--] = digits[x % base];
    1176:	00d10a23          	sb	a3,20(sp)
    } while ((x /= base) != 0);
    117a:	12e5e863          	bltu	a1,a4,12aa <printint.constprop.0+0x1b2>
        buf[i--] = digits[x % base];
    117e:	02e876bb          	remuw	a3,a6,a4
    1182:	1682                	slli	a3,a3,0x20
    1184:	9281                	srli	a3,a3,0x20
    1186:	96b2                	add	a3,a3,a2
    1188:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    118c:	02e855bb          	divuw	a1,a6,a4
        buf[i--] = digits[x % base];
    1190:	00d109a3          	sb	a3,19(sp)
    } while ((x /= base) != 0);
    1194:	12e86463          	bltu	a6,a4,12bc <printint.constprop.0+0x1c4>
        buf[i--] = digits[x % base];
    1198:	02e5f6bb          	remuw	a3,a1,a4
    119c:	1682                	slli	a3,a3,0x20
    119e:	9281                	srli	a3,a3,0x20
    11a0:	96b2                	add	a3,a3,a2
    11a2:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    11a6:	02e5d83b          	divuw	a6,a1,a4
        buf[i--] = digits[x % base];
    11aa:	00d10923          	sb	a3,18(sp)
    } while ((x /= base) != 0);
    11ae:	0ce5ec63          	bltu	a1,a4,1286 <printint.constprop.0+0x18e>
        buf[i--] = digits[x % base];
    11b2:	02e876bb          	remuw	a3,a6,a4
    11b6:	1682                	slli	a3,a3,0x20
    11b8:	9281                	srli	a3,a3,0x20
    11ba:	96b2                	add	a3,a3,a2
    11bc:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    11c0:	02e855bb          	divuw	a1,a6,a4
        buf[i--] = digits[x % base];
    11c4:	00d108a3          	sb	a3,17(sp)
    } while ((x /= base) != 0);
    11c8:	10e86963          	bltu	a6,a4,12da <printint.constprop.0+0x1e2>
        buf[i--] = digits[x % base];
    11cc:	02e5f6bb          	remuw	a3,a1,a4
    11d0:	1682                	slli	a3,a3,0x20
    11d2:	9281                	srli	a3,a3,0x20
    11d4:	96b2                	add	a3,a3,a2
    11d6:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    11da:	02e5d83b          	divuw	a6,a1,a4
        buf[i--] = digits[x % base];
    11de:	00d10823          	sb	a3,16(sp)
    } while ((x /= base) != 0);
    11e2:	10e5e763          	bltu	a1,a4,12f0 <printint.constprop.0+0x1f8>
        buf[i--] = digits[x % base];
    11e6:	02e876bb          	remuw	a3,a6,a4
    11ea:	1682                	slli	a3,a3,0x20
    11ec:	9281                	srli	a3,a3,0x20
    11ee:	96b2                	add	a3,a3,a2
    11f0:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    11f4:	02e857bb          	divuw	a5,a6,a4
        buf[i--] = digits[x % base];
    11f8:	00d107a3          	sb	a3,15(sp)
    } while ((x /= base) != 0);
    11fc:	10e86363          	bltu	a6,a4,1302 <printint.constprop.0+0x20a>
        buf[i--] = digits[x % base];
    1200:	1782                	slli	a5,a5,0x20
    1202:	9381                	srli	a5,a5,0x20
    1204:	97b2                	add	a5,a5,a2
    1206:	0007c783          	lbu	a5,0(a5)
    120a:	4599                	li	a1,6
    120c:	00f10723          	sb	a5,14(sp)

    if (sign)
    1210:	00055763          	bgez	a0,121e <printint.constprop.0+0x126>
        buf[i--] = '-';
    1214:	02d00793          	li	a5,45
    1218:	00f106a3          	sb	a5,13(sp)
        buf[i--] = digits[x % base];
    121c:	4595                	li	a1,5
    write(f, s, l);
    121e:	003c                	addi	a5,sp,8
    1220:	4641                	li	a2,16
    1222:	9e0d                	subw	a2,a2,a1
    1224:	4505                	li	a0,1
    1226:	95be                	add	a1,a1,a5
    1228:	291000ef          	jal	ra,1cb8 <write>
    i++;
    if (i < 0)
        puts("printint error");
    out(stdout, buf + i, 16 - i);
}
    122c:	70a2                	ld	ra,40(sp)
    122e:	6145                	addi	sp,sp,48
    1230:	8082                	ret
        x = -xx;
    1232:	40a0083b          	negw	a6,a0
        buf[i--] = digits[x % base];
    1236:	02b877bb          	remuw	a5,a6,a1
    123a:	00001617          	auipc	a2,0x1
    123e:	d8660613          	addi	a2,a2,-634 # 1fc0 <digits>
    buf[16] = 0;
    1242:	00010c23          	sb	zero,24(sp)
        buf[i--] = digits[x % base];
    1246:	0005871b          	sext.w	a4,a1
    124a:	1782                	slli	a5,a5,0x20
    124c:	9381                	srli	a5,a5,0x20
    124e:	97b2                	add	a5,a5,a2
    1250:	0007c783          	lbu	a5,0(a5)
    } while ((x /= base) != 0);
    1254:	02b858bb          	divuw	a7,a6,a1
        buf[i--] = digits[x % base];
    1258:	00f10ba3          	sb	a5,23(sp)
    } while ((x /= base) != 0);
    125c:	06b86963          	bltu	a6,a1,12ce <printint.constprop.0+0x1d6>
        buf[i--] = digits[x % base];
    1260:	02e8f7bb          	remuw	a5,a7,a4
    1264:	1782                	slli	a5,a5,0x20
    1266:	9381                	srli	a5,a5,0x20
    1268:	97b2                	add	a5,a5,a2
    126a:	0007c783          	lbu	a5,0(a5)
    } while ((x /= base) != 0);
    126e:	02e8d6bb          	divuw	a3,a7,a4
        buf[i--] = digits[x % base];
    1272:	00f10b23          	sb	a5,22(sp)
    } while ((x /= base) != 0);
    1276:	ece8f8e3          	bgeu	a7,a4,1146 <printint.constprop.0+0x4e>
        buf[i--] = '-';
    127a:	02d00793          	li	a5,45
    127e:	00f10aa3          	sb	a5,21(sp)
        buf[i--] = digits[x % base];
    1282:	45b5                	li	a1,13
    1284:	bf69                	j	121e <printint.constprop.0+0x126>
    1286:	45a9                	li	a1,10
    if (sign)
    1288:	f8055be3          	bgez	a0,121e <printint.constprop.0+0x126>
        buf[i--] = '-';
    128c:	02d00793          	li	a5,45
    1290:	00f108a3          	sb	a5,17(sp)
        buf[i--] = digits[x % base];
    1294:	45a5                	li	a1,9
    1296:	b761                	j	121e <printint.constprop.0+0x126>
    1298:	45b5                	li	a1,13
    if (sign)
    129a:	f80552e3          	bgez	a0,121e <printint.constprop.0+0x126>
        buf[i--] = '-';
    129e:	02d00793          	li	a5,45
    12a2:	00f10a23          	sb	a5,20(sp)
        buf[i--] = digits[x % base];
    12a6:	45b1                	li	a1,12
    12a8:	bf9d                	j	121e <printint.constprop.0+0x126>
    12aa:	45b1                	li	a1,12
    if (sign)
    12ac:	f60559e3          	bgez	a0,121e <printint.constprop.0+0x126>
        buf[i--] = '-';
    12b0:	02d00793          	li	a5,45
    12b4:	00f109a3          	sb	a5,19(sp)
        buf[i--] = digits[x % base];
    12b8:	45ad                	li	a1,11
    12ba:	b795                	j	121e <printint.constprop.0+0x126>
    12bc:	45ad                	li	a1,11
    if (sign)
    12be:	f60550e3          	bgez	a0,121e <printint.constprop.0+0x126>
        buf[i--] = '-';
    12c2:	02d00793          	li	a5,45
    12c6:	00f10923          	sb	a5,18(sp)
        buf[i--] = digits[x % base];
    12ca:	45a9                	li	a1,10
    12cc:	bf89                	j	121e <printint.constprop.0+0x126>
        buf[i--] = '-';
    12ce:	02d00793          	li	a5,45
    12d2:	00f10b23          	sb	a5,22(sp)
        buf[i--] = digits[x % base];
    12d6:	45b9                	li	a1,14
    12d8:	b799                	j	121e <printint.constprop.0+0x126>
    12da:	45a5                	li	a1,9
    if (sign)
    12dc:	f40551e3          	bgez	a0,121e <printint.constprop.0+0x126>
        buf[i--] = '-';
    12e0:	02d00793          	li	a5,45
    12e4:	00f10823          	sb	a5,16(sp)
        buf[i--] = digits[x % base];
    12e8:	45a1                	li	a1,8
    12ea:	bf15                	j	121e <printint.constprop.0+0x126>
    i = 15;
    12ec:	45bd                	li	a1,15
    12ee:	bf05                	j	121e <printint.constprop.0+0x126>
        buf[i--] = digits[x % base];
    12f0:	45a1                	li	a1,8
    if (sign)
    12f2:	f20556e3          	bgez	a0,121e <printint.constprop.0+0x126>
        buf[i--] = '-';
    12f6:	02d00793          	li	a5,45
    12fa:	00f107a3          	sb	a5,15(sp)
        buf[i--] = digits[x % base];
    12fe:	459d                	li	a1,7
    1300:	bf39                	j	121e <printint.constprop.0+0x126>
    1302:	459d                	li	a1,7
    if (sign)
    1304:	f0055de3          	bgez	a0,121e <printint.constprop.0+0x126>
        buf[i--] = '-';
    1308:	02d00793          	li	a5,45
    130c:	00f10723          	sb	a5,14(sp)
        buf[i--] = digits[x % base];
    1310:	4599                	li	a1,6
    1312:	b731                	j	121e <printint.constprop.0+0x126>

0000000000001314 <getchar>:
{
    1314:	1101                	addi	sp,sp,-32
    read(stdin, &byte, 1);
    1316:	00f10593          	addi	a1,sp,15
    131a:	4605                	li	a2,1
    131c:	4501                	li	a0,0
{
    131e:	ec06                	sd	ra,24(sp)
    char byte = 0;
    1320:	000107a3          	sb	zero,15(sp)
    read(stdin, &byte, 1);
    1324:	18b000ef          	jal	ra,1cae <read>
}
    1328:	60e2                	ld	ra,24(sp)
    132a:	00f14503          	lbu	a0,15(sp)
    132e:	6105                	addi	sp,sp,32
    1330:	8082                	ret

0000000000001332 <putchar>:
{
    1332:	1101                	addi	sp,sp,-32
    1334:	87aa                	mv	a5,a0
    return write(stdout, &byte, 1);
    1336:	00f10593          	addi	a1,sp,15
    133a:	4605                	li	a2,1
    133c:	4505                	li	a0,1
{
    133e:	ec06                	sd	ra,24(sp)
    char byte = c;
    1340:	00f107a3          	sb	a5,15(sp)
    return write(stdout, &byte, 1);
    1344:	175000ef          	jal	ra,1cb8 <write>
}
    1348:	60e2                	ld	ra,24(sp)
    134a:	2501                	sext.w	a0,a0
    134c:	6105                	addi	sp,sp,32
    134e:	8082                	ret

0000000000001350 <puts>:
{
    1350:	1141                	addi	sp,sp,-16
    1352:	e406                	sd	ra,8(sp)
    1354:	e022                	sd	s0,0(sp)
    1356:	842a                	mv	s0,a0
    r = -(write(stdout, s, strlen(s)) < 0);
    1358:	57c000ef          	jal	ra,18d4 <strlen>
    135c:	862a                	mv	a2,a0
    135e:	85a2                	mv	a1,s0
    1360:	4505                	li	a0,1
    1362:	157000ef          	jal	ra,1cb8 <write>
}
    1366:	60a2                	ld	ra,8(sp)
    1368:	6402                	ld	s0,0(sp)
    r = -(write(stdout, s, strlen(s)) < 0);
    136a:	957d                	srai	a0,a0,0x3f
    return r;
    136c:	2501                	sext.w	a0,a0
}
    136e:	0141                	addi	sp,sp,16
    1370:	8082                	ret

0000000000001372 <printf>:
    out(stdout, buf, i);
}

// Print to the console. only understands %d, %x, %p, %s.
void printf(const char *fmt, ...)
{
    1372:	7171                	addi	sp,sp,-176
    1374:	fc56                	sd	s5,56(sp)
    1376:	ed3e                	sd	a5,152(sp)
    buf[i++] = '0';
    1378:	7ae1                	lui	s5,0xffff8
    va_list ap;
    int cnt = 0, l = 0;
    char *a, *z, *s = (char *)fmt, str;
    int f = stdout;

    va_start(ap, fmt);
    137a:	18bc                	addi	a5,sp,120
{
    137c:	e8ca                	sd	s2,80(sp)
    137e:	e4ce                	sd	s3,72(sp)
    1380:	e0d2                	sd	s4,64(sp)
    1382:	f85a                	sd	s6,48(sp)
    1384:	f486                	sd	ra,104(sp)
    1386:	f0a2                	sd	s0,96(sp)
    1388:	eca6                	sd	s1,88(sp)
    138a:	fcae                	sd	a1,120(sp)
    138c:	e132                	sd	a2,128(sp)
    138e:	e536                	sd	a3,136(sp)
    1390:	e93a                	sd	a4,144(sp)
    1392:	f142                	sd	a6,160(sp)
    1394:	f546                	sd	a7,168(sp)
    va_start(ap, fmt);
    1396:	e03e                	sd	a5,0(sp)
    for (;;)
    {
        if (!*s)
            break;
        for (a = s; *s && *s != '%'; s++)
    1398:	02500913          	li	s2,37
        out(f, a, l);
        if (l)
            continue;
        if (s[1] == 0)
            break;
        switch (s[1])
    139c:	07300a13          	li	s4,115
        case 'p':
            printptr(va_arg(ap, uint64));
            break;
        case 's':
            if ((a = va_arg(ap, char *)) == 0)
                a = "(null)";
    13a0:	00001b17          	auipc	s6,0x1
    13a4:	bf0b0b13          	addi	s6,s6,-1040 # 1f90 <__clone+0xa4>
    buf[i++] = '0';
    13a8:	830aca93          	xori	s5,s5,-2000
        buf[i++] = digits[x >> (sizeof(uint64) * 8 - 4)];
    13ac:	00001997          	auipc	s3,0x1
    13b0:	c1498993          	addi	s3,s3,-1004 # 1fc0 <digits>
        if (!*s)
    13b4:	00054783          	lbu	a5,0(a0)
    13b8:	16078a63          	beqz	a5,152c <printf+0x1ba>
    13bc:	862a                	mv	a2,a0
        for (a = s; *s && *s != '%'; s++)
    13be:	19278163          	beq	a5,s2,1540 <printf+0x1ce>
    13c2:	00164783          	lbu	a5,1(a2)
    13c6:	0605                	addi	a2,a2,1
    13c8:	fbfd                	bnez	a5,13be <printf+0x4c>
    13ca:	84b2                	mv	s1,a2
        l = z - a;
    13cc:	40a6043b          	subw	s0,a2,a0
    write(f, s, l);
    13d0:	85aa                	mv	a1,a0
    13d2:	8622                	mv	a2,s0
    13d4:	4505                	li	a0,1
    13d6:	0e3000ef          	jal	ra,1cb8 <write>
        if (l)
    13da:	18041c63          	bnez	s0,1572 <printf+0x200>
        if (s[1] == 0)
    13de:	0014c783          	lbu	a5,1(s1)
    13e2:	14078563          	beqz	a5,152c <printf+0x1ba>
        switch (s[1])
    13e6:	1d478063          	beq	a5,s4,15a6 <printf+0x234>
    13ea:	18fa6663          	bltu	s4,a5,1576 <printf+0x204>
    13ee:	06400713          	li	a4,100
    13f2:	1ae78063          	beq	a5,a4,1592 <printf+0x220>
    13f6:	07000713          	li	a4,112
    13fa:	1ce79963          	bne	a5,a4,15cc <printf+0x25a>
            printptr(va_arg(ap, uint64));
    13fe:	6702                	ld	a4,0(sp)
    buf[i++] = '0';
    1400:	01511423          	sh	s5,8(sp)
    write(f, s, l);
    1404:	4649                	li	a2,18
            printptr(va_arg(ap, uint64));
    1406:	631c                	ld	a5,0(a4)
    1408:	0721                	addi	a4,a4,8
    140a:	e03a                	sd	a4,0(sp)
    for (j = 0; j < (sizeof(uint64) * 2); j++, x <<= 4)
    140c:	00479293          	slli	t0,a5,0x4
    1410:	00879f93          	slli	t6,a5,0x8
    1414:	00c79f13          	slli	t5,a5,0xc
    1418:	01079e93          	slli	t4,a5,0x10
    141c:	01479e13          	slli	t3,a5,0x14
    1420:	01879313          	slli	t1,a5,0x18
    1424:	01c79893          	slli	a7,a5,0x1c
    1428:	02479813          	slli	a6,a5,0x24
    142c:	02879513          	slli	a0,a5,0x28
    1430:	02c79593          	slli	a1,a5,0x2c
    1434:	03079693          	slli	a3,a5,0x30
    1438:	03479713          	slli	a4,a5,0x34
        buf[i++] = digits[x >> (sizeof(uint64) * 8 - 4)];
    143c:	03c7d413          	srli	s0,a5,0x3c
    1440:	01c7d39b          	srliw	t2,a5,0x1c
    1444:	03c2d293          	srli	t0,t0,0x3c
    1448:	03cfdf93          	srli	t6,t6,0x3c
    144c:	03cf5f13          	srli	t5,t5,0x3c
    1450:	03cede93          	srli	t4,t4,0x3c
    1454:	03ce5e13          	srli	t3,t3,0x3c
    1458:	03c35313          	srli	t1,t1,0x3c
    145c:	03c8d893          	srli	a7,a7,0x3c
    1460:	03c85813          	srli	a6,a6,0x3c
    1464:	9171                	srli	a0,a0,0x3c
    1466:	91f1                	srli	a1,a1,0x3c
    1468:	92f1                	srli	a3,a3,0x3c
    146a:	9371                	srli	a4,a4,0x3c
    146c:	96ce                	add	a3,a3,s3
    146e:	974e                	add	a4,a4,s3
    1470:	944e                	add	s0,s0,s3
    1472:	92ce                	add	t0,t0,s3
    1474:	9fce                	add	t6,t6,s3
    1476:	9f4e                	add	t5,t5,s3
    1478:	9ece                	add	t4,t4,s3
    147a:	9e4e                	add	t3,t3,s3
    147c:	934e                	add	t1,t1,s3
    147e:	98ce                	add	a7,a7,s3
    1480:	93ce                	add	t2,t2,s3
    1482:	984e                	add	a6,a6,s3
    1484:	954e                	add	a0,a0,s3
    1486:	95ce                	add	a1,a1,s3
    1488:	0006c083          	lbu	ra,0(a3)
    148c:	0002c283          	lbu	t0,0(t0)
    1490:	00074683          	lbu	a3,0(a4)
    1494:	000fcf83          	lbu	t6,0(t6)
    1498:	000f4f03          	lbu	t5,0(t5)
    149c:	000ece83          	lbu	t4,0(t4)
    14a0:	000e4e03          	lbu	t3,0(t3)
    14a4:	00034303          	lbu	t1,0(t1)
    14a8:	0008c883          	lbu	a7,0(a7)
    14ac:	0003c383          	lbu	t2,0(t2)
    14b0:	00084803          	lbu	a6,0(a6)
    14b4:	00054503          	lbu	a0,0(a0)
    14b8:	0005c583          	lbu	a1,0(a1)
    14bc:	00044403          	lbu	s0,0(s0)
    for (j = 0; j < (sizeof(uint64) * 2); j++, x <<= 4)
    14c0:	03879713          	slli	a4,a5,0x38
        buf[i++] = digits[x >> (sizeof(uint64) * 8 - 4)];
    14c4:	9371                	srli	a4,a4,0x3c
    14c6:	8bbd                	andi	a5,a5,15
    14c8:	974e                	add	a4,a4,s3
    14ca:	97ce                	add	a5,a5,s3
    14cc:	005105a3          	sb	t0,11(sp)
    14d0:	01f10623          	sb	t6,12(sp)
    14d4:	01e106a3          	sb	t5,13(sp)
    14d8:	01d10723          	sb	t4,14(sp)
    14dc:	01c107a3          	sb	t3,15(sp)
    14e0:	00610823          	sb	t1,16(sp)
    14e4:	011108a3          	sb	a7,17(sp)
    14e8:	00710923          	sb	t2,18(sp)
    14ec:	010109a3          	sb	a6,19(sp)
    14f0:	00a10a23          	sb	a0,20(sp)
    14f4:	00b10aa3          	sb	a1,21(sp)
    14f8:	00110b23          	sb	ra,22(sp)
    14fc:	00d10ba3          	sb	a3,23(sp)
    1500:	00810523          	sb	s0,10(sp)
    1504:	00074703          	lbu	a4,0(a4)
    1508:	0007c783          	lbu	a5,0(a5)
    write(f, s, l);
    150c:	002c                	addi	a1,sp,8
    150e:	4505                	li	a0,1
        buf[i++] = digits[x >> (sizeof(uint64) * 8 - 4)];
    1510:	00e10c23          	sb	a4,24(sp)
    1514:	00f10ca3          	sb	a5,25(sp)
    buf[i] = 0;
    1518:	00010d23          	sb	zero,26(sp)
    write(f, s, l);
    151c:	79c000ef          	jal	ra,1cb8 <write>
            // Print unknown % sequence to draw attention.
            putchar('%');
            putchar(s[1]);
            break;
        }
        s += 2;
    1520:	00248513          	addi	a0,s1,2
        if (!*s)
    1524:	00054783          	lbu	a5,0(a0)
    1528:	e8079ae3          	bnez	a5,13bc <printf+0x4a>
    }
    va_end(ap);
}
    152c:	70a6                	ld	ra,104(sp)
    152e:	7406                	ld	s0,96(sp)
    1530:	64e6                	ld	s1,88(sp)
    1532:	6946                	ld	s2,80(sp)
    1534:	69a6                	ld	s3,72(sp)
    1536:	6a06                	ld	s4,64(sp)
    1538:	7ae2                	ld	s5,56(sp)
    153a:	7b42                	ld	s6,48(sp)
    153c:	614d                	addi	sp,sp,176
    153e:	8082                	ret
        for (z = s; s[0] == '%' && s[1] == '%'; z++, s += 2)
    1540:	00064783          	lbu	a5,0(a2)
    1544:	84b2                	mv	s1,a2
    1546:	01278963          	beq	a5,s2,1558 <printf+0x1e6>
    154a:	b549                	j	13cc <printf+0x5a>
    154c:	0024c783          	lbu	a5,2(s1)
    1550:	0605                	addi	a2,a2,1
    1552:	0489                	addi	s1,s1,2
    1554:	e7279ce3          	bne	a5,s2,13cc <printf+0x5a>
    1558:	0014c783          	lbu	a5,1(s1)
    155c:	ff2788e3          	beq	a5,s2,154c <printf+0x1da>
        l = z - a;
    1560:	40a6043b          	subw	s0,a2,a0
    write(f, s, l);
    1564:	85aa                	mv	a1,a0
    1566:	8622                	mv	a2,s0
    1568:	4505                	li	a0,1
    156a:	74e000ef          	jal	ra,1cb8 <write>
        if (l)
    156e:	e60408e3          	beqz	s0,13de <printf+0x6c>
    1572:	8526                	mv	a0,s1
    1574:	b581                	j	13b4 <printf+0x42>
        switch (s[1])
    1576:	07800713          	li	a4,120
    157a:	04e79963          	bne	a5,a4,15cc <printf+0x25a>
            printint(va_arg(ap, int), 16, 1);
    157e:	6782                	ld	a5,0(sp)
    1580:	45c1                	li	a1,16
    1582:	4388                	lw	a0,0(a5)
    1584:	07a1                	addi	a5,a5,8
    1586:	e03e                	sd	a5,0(sp)
    1588:	b71ff0ef          	jal	ra,10f8 <printint.constprop.0>
        s += 2;
    158c:	00248513          	addi	a0,s1,2
    1590:	bf51                	j	1524 <printf+0x1b2>
            printint(va_arg(ap, int), 10, 1);
    1592:	6782                	ld	a5,0(sp)
    1594:	45a9                	li	a1,10
    1596:	4388                	lw	a0,0(a5)
    1598:	07a1                	addi	a5,a5,8
    159a:	e03e                	sd	a5,0(sp)
    159c:	b5dff0ef          	jal	ra,10f8 <printint.constprop.0>
        s += 2;
    15a0:	00248513          	addi	a0,s1,2
    15a4:	b741                	j	1524 <printf+0x1b2>
            if ((a = va_arg(ap, char *)) == 0)
    15a6:	6782                	ld	a5,0(sp)
    15a8:	6380                	ld	s0,0(a5)
    15aa:	07a1                	addi	a5,a5,8
    15ac:	e03e                	sd	a5,0(sp)
    15ae:	c031                	beqz	s0,15f2 <printf+0x280>
            l = strnlen(a, 200);
    15b0:	0c800593          	li	a1,200
    15b4:	8522                	mv	a0,s0
    15b6:	40a000ef          	jal	ra,19c0 <strnlen>
    write(f, s, l);
    15ba:	0005061b          	sext.w	a2,a0
    15be:	85a2                	mv	a1,s0
    15c0:	4505                	li	a0,1
    15c2:	6f6000ef          	jal	ra,1cb8 <write>
        s += 2;
    15c6:	00248513          	addi	a0,s1,2
    15ca:	bfa9                	j	1524 <printf+0x1b2>
    return write(stdout, &byte, 1);
    15cc:	4605                	li	a2,1
    15ce:	002c                	addi	a1,sp,8
    15d0:	4505                	li	a0,1
    char byte = c;
    15d2:	01210423          	sb	s2,8(sp)
    return write(stdout, &byte, 1);
    15d6:	6e2000ef          	jal	ra,1cb8 <write>
    char byte = c;
    15da:	0014c783          	lbu	a5,1(s1)
    return write(stdout, &byte, 1);
    15de:	4605                	li	a2,1
    15e0:	002c                	addi	a1,sp,8
    15e2:	4505                	li	a0,1
    char byte = c;
    15e4:	00f10423          	sb	a5,8(sp)
    return write(stdout, &byte, 1);
    15e8:	6d0000ef          	jal	ra,1cb8 <write>
        s += 2;
    15ec:	00248513          	addi	a0,s1,2
    15f0:	bf15                	j	1524 <printf+0x1b2>
                a = "(null)";
    15f2:	845a                	mv	s0,s6
    15f4:	bf75                	j	15b0 <printf+0x23e>

00000000000015f6 <panic>:
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

void panic(char *m)
{
    15f6:	1141                	addi	sp,sp,-16
    15f8:	e406                	sd	ra,8(sp)
    puts(m);
    15fa:	d57ff0ef          	jal	ra,1350 <puts>
    exit(-100);
}
    15fe:	60a2                	ld	ra,8(sp)
    exit(-100);
    1600:	f9c00513          	li	a0,-100
}
    1604:	0141                	addi	sp,sp,16
    exit(-100);
    1606:	a709                	j	1d08 <exit>

0000000000001608 <isspace>:
#define HIGHS (ONES * (UCHAR_MAX / 2 + 1))
#define HASZERO(x) (((x)-ONES) & ~(x)&HIGHS)

int isspace(int c)
{
    return c == ' ' || (unsigned)c - '\t' < 5;
    1608:	02000793          	li	a5,32
    160c:	00f50663          	beq	a0,a5,1618 <isspace+0x10>
    1610:	355d                	addiw	a0,a0,-9
    1612:	00553513          	sltiu	a0,a0,5
    1616:	8082                	ret
    1618:	4505                	li	a0,1
}
    161a:	8082                	ret

000000000000161c <isdigit>:

int isdigit(int c)
{
    return (unsigned)c - '0' < 10;
    161c:	fd05051b          	addiw	a0,a0,-48
}
    1620:	00a53513          	sltiu	a0,a0,10
    1624:	8082                	ret

0000000000001626 <atoi>:
    return c == ' ' || (unsigned)c - '\t' < 5;
    1626:	02000613          	li	a2,32
    162a:	4591                	li	a1,4

int atoi(const char *s)
{
    int n = 0, neg = 0;
    while (isspace(*s))
    162c:	00054703          	lbu	a4,0(a0)
    return c == ' ' || (unsigned)c - '\t' < 5;
    1630:	ff77069b          	addiw	a3,a4,-9
    1634:	04c70d63          	beq	a4,a2,168e <atoi+0x68>
    1638:	0007079b          	sext.w	a5,a4
    163c:	04d5f963          	bgeu	a1,a3,168e <atoi+0x68>
        s++;
    switch (*s)
    1640:	02b00693          	li	a3,43
    1644:	04d70a63          	beq	a4,a3,1698 <atoi+0x72>
    1648:	02d00693          	li	a3,45
    164c:	06d70463          	beq	a4,a3,16b4 <atoi+0x8e>
        neg = 1;
    case '+':
        s++;
    }
    /* Compute n as a negative number to avoid overflow on INT_MIN */
    while (isdigit(*s))
    1650:	fd07859b          	addiw	a1,a5,-48
    1654:	4625                	li	a2,9
    1656:	873e                	mv	a4,a5
    1658:	86aa                	mv	a3,a0
    int n = 0, neg = 0;
    165a:	4e01                	li	t3,0
    while (isdigit(*s))
    165c:	04b66a63          	bltu	a2,a1,16b0 <atoi+0x8a>
    int n = 0, neg = 0;
    1660:	4501                	li	a0,0
    while (isdigit(*s))
    1662:	4825                	li	a6,9
    1664:	0016c603          	lbu	a2,1(a3)
        n = 10 * n - (*s++ - '0');
    1668:	0025179b          	slliw	a5,a0,0x2
    166c:	9d3d                	addw	a0,a0,a5
    166e:	fd07031b          	addiw	t1,a4,-48
    1672:	0015189b          	slliw	a7,a0,0x1
    while (isdigit(*s))
    1676:	fd06059b          	addiw	a1,a2,-48
        n = 10 * n - (*s++ - '0');
    167a:	0685                	addi	a3,a3,1
    167c:	4068853b          	subw	a0,a7,t1
    while (isdigit(*s))
    1680:	0006071b          	sext.w	a4,a2
    1684:	feb870e3          	bgeu	a6,a1,1664 <atoi+0x3e>
    return neg ? n : -n;
    1688:	000e0563          	beqz	t3,1692 <atoi+0x6c>
}
    168c:	8082                	ret
        s++;
    168e:	0505                	addi	a0,a0,1
    1690:	bf71                	j	162c <atoi+0x6>
    return neg ? n : -n;
    1692:	4113053b          	subw	a0,t1,a7
    1696:	8082                	ret
    while (isdigit(*s))
    1698:	00154783          	lbu	a5,1(a0)
    169c:	4625                	li	a2,9
        s++;
    169e:	00150693          	addi	a3,a0,1
    while (isdigit(*s))
    16a2:	fd07859b          	addiw	a1,a5,-48
    16a6:	0007871b          	sext.w	a4,a5
    int n = 0, neg = 0;
    16aa:	4e01                	li	t3,0
    while (isdigit(*s))
    16ac:	fab67ae3          	bgeu	a2,a1,1660 <atoi+0x3a>
    16b0:	4501                	li	a0,0
}
    16b2:	8082                	ret
    while (isdigit(*s))
    16b4:	00154783          	lbu	a5,1(a0)
    16b8:	4625                	li	a2,9
        s++;
    16ba:	00150693          	addi	a3,a0,1
    while (isdigit(*s))
    16be:	fd07859b          	addiw	a1,a5,-48
    16c2:	0007871b          	sext.w	a4,a5
    16c6:	feb665e3          	bltu	a2,a1,16b0 <atoi+0x8a>
        neg = 1;
    16ca:	4e05                	li	t3,1
    16cc:	bf51                	j	1660 <atoi+0x3a>

00000000000016ce <memset>:

void *memset(void *dest, int c, size_t n)
{
    char *p = dest;
    for (int i = 0; i < n; ++i, *(p++) = c)
    16ce:	16060d63          	beqz	a2,1848 <memset+0x17a>
    16d2:	40a007b3          	neg	a5,a0
    16d6:	8b9d                	andi	a5,a5,7
    16d8:	00778713          	addi	a4,a5,7
    16dc:	482d                	li	a6,11
    16de:	0ff5f593          	andi	a1,a1,255
    16e2:	fff60693          	addi	a3,a2,-1
    16e6:	17076263          	bltu	a4,a6,184a <memset+0x17c>
    16ea:	16e6ea63          	bltu	a3,a4,185e <memset+0x190>
    16ee:	16078563          	beqz	a5,1858 <memset+0x18a>
    16f2:	00b50023          	sb	a1,0(a0)
    16f6:	4705                	li	a4,1
    16f8:	00150e93          	addi	t4,a0,1
    16fc:	14e78c63          	beq	a5,a4,1854 <memset+0x186>
    1700:	00b500a3          	sb	a1,1(a0)
    1704:	4709                	li	a4,2
    1706:	00250e93          	addi	t4,a0,2
    170a:	14e78d63          	beq	a5,a4,1864 <memset+0x196>
    170e:	00b50123          	sb	a1,2(a0)
    1712:	470d                	li	a4,3
    1714:	00350e93          	addi	t4,a0,3
    1718:	12e78b63          	beq	a5,a4,184e <memset+0x180>
    171c:	00b501a3          	sb	a1,3(a0)
    1720:	4711                	li	a4,4
    1722:	00450e93          	addi	t4,a0,4
    1726:	14e78163          	beq	a5,a4,1868 <memset+0x19a>
    172a:	00b50223          	sb	a1,4(a0)
    172e:	4715                	li	a4,5
    1730:	00550e93          	addi	t4,a0,5
    1734:	12e78c63          	beq	a5,a4,186c <memset+0x19e>
    1738:	00b502a3          	sb	a1,5(a0)
    173c:	471d                	li	a4,7
    173e:	00650e93          	addi	t4,a0,6
    1742:	12e79763          	bne	a5,a4,1870 <memset+0x1a2>
    1746:	00750e93          	addi	t4,a0,7
    174a:	00b50323          	sb	a1,6(a0)
    174e:	4f1d                	li	t5,7
    1750:	00859713          	slli	a4,a1,0x8
    1754:	8f4d                	or	a4,a4,a1
    1756:	01059e13          	slli	t3,a1,0x10
    175a:	01c76e33          	or	t3,a4,t3
    175e:	01859313          	slli	t1,a1,0x18
    1762:	006e6333          	or	t1,t3,t1
    1766:	02059893          	slli	a7,a1,0x20
    176a:	011368b3          	or	a7,t1,a7
    176e:	02859813          	slli	a6,a1,0x28
    1772:	40f60333          	sub	t1,a2,a5
    1776:	0108e833          	or	a6,a7,a6
    177a:	03059693          	slli	a3,a1,0x30
    177e:	00d866b3          	or	a3,a6,a3
    1782:	03859713          	slli	a4,a1,0x38
    1786:	97aa                	add	a5,a5,a0
    1788:	ff837813          	andi	a6,t1,-8
    178c:	8f55                	or	a4,a4,a3
    178e:	00f806b3          	add	a3,a6,a5
    1792:	e398                	sd	a4,0(a5)
    1794:	07a1                	addi	a5,a5,8
    1796:	fed79ee3          	bne	a5,a3,1792 <memset+0xc4>
    179a:	ff837693          	andi	a3,t1,-8
    179e:	00de87b3          	add	a5,t4,a3
    17a2:	01e6873b          	addw	a4,a3,t5
    17a6:	0ad30663          	beq	t1,a3,1852 <memset+0x184>
    17aa:	00b78023          	sb	a1,0(a5)
    17ae:	0017069b          	addiw	a3,a4,1
    17b2:	08c6fb63          	bgeu	a3,a2,1848 <memset+0x17a>
    17b6:	00b780a3          	sb	a1,1(a5)
    17ba:	0027069b          	addiw	a3,a4,2
    17be:	08c6f563          	bgeu	a3,a2,1848 <memset+0x17a>
    17c2:	00b78123          	sb	a1,2(a5)
    17c6:	0037069b          	addiw	a3,a4,3
    17ca:	06c6ff63          	bgeu	a3,a2,1848 <memset+0x17a>
    17ce:	00b781a3          	sb	a1,3(a5)
    17d2:	0047069b          	addiw	a3,a4,4
    17d6:	06c6f963          	bgeu	a3,a2,1848 <memset+0x17a>
    17da:	00b78223          	sb	a1,4(a5)
    17de:	0057069b          	addiw	a3,a4,5
    17e2:	06c6f363          	bgeu	a3,a2,1848 <memset+0x17a>
    17e6:	00b782a3          	sb	a1,5(a5)
    17ea:	0067069b          	addiw	a3,a4,6
    17ee:	04c6fd63          	bgeu	a3,a2,1848 <memset+0x17a>
    17f2:	00b78323          	sb	a1,6(a5)
    17f6:	0077069b          	addiw	a3,a4,7
    17fa:	04c6f763          	bgeu	a3,a2,1848 <memset+0x17a>
    17fe:	00b783a3          	sb	a1,7(a5)
    1802:	0087069b          	addiw	a3,a4,8
    1806:	04c6f163          	bgeu	a3,a2,1848 <memset+0x17a>
    180a:	00b78423          	sb	a1,8(a5)
    180e:	0097069b          	addiw	a3,a4,9
    1812:	02c6fb63          	bgeu	a3,a2,1848 <memset+0x17a>
    1816:	00b784a3          	sb	a1,9(a5)
    181a:	00a7069b          	addiw	a3,a4,10
    181e:	02c6f563          	bgeu	a3,a2,1848 <memset+0x17a>
    1822:	00b78523          	sb	a1,10(a5)
    1826:	00b7069b          	addiw	a3,a4,11
    182a:	00c6ff63          	bgeu	a3,a2,1848 <memset+0x17a>
    182e:	00b785a3          	sb	a1,11(a5)
    1832:	00c7069b          	addiw	a3,a4,12
    1836:	00c6f963          	bgeu	a3,a2,1848 <memset+0x17a>
    183a:	00b78623          	sb	a1,12(a5)
    183e:	2735                	addiw	a4,a4,13
    1840:	00c77463          	bgeu	a4,a2,1848 <memset+0x17a>
    1844:	00b786a3          	sb	a1,13(a5)
        ;
    return dest;
}
    1848:	8082                	ret
    184a:	472d                	li	a4,11
    184c:	bd79                	j	16ea <memset+0x1c>
    for (int i = 0; i < n; ++i, *(p++) = c)
    184e:	4f0d                	li	t5,3
    1850:	b701                	j	1750 <memset+0x82>
    1852:	8082                	ret
    1854:	4f05                	li	t5,1
    1856:	bded                	j	1750 <memset+0x82>
    1858:	8eaa                	mv	t4,a0
    185a:	4f01                	li	t5,0
    185c:	bdd5                	j	1750 <memset+0x82>
    185e:	87aa                	mv	a5,a0
    1860:	4701                	li	a4,0
    1862:	b7a1                	j	17aa <memset+0xdc>
    1864:	4f09                	li	t5,2
    1866:	b5ed                	j	1750 <memset+0x82>
    1868:	4f11                	li	t5,4
    186a:	b5dd                	j	1750 <memset+0x82>
    186c:	4f15                	li	t5,5
    186e:	b5cd                	j	1750 <memset+0x82>
    1870:	4f19                	li	t5,6
    1872:	bdf9                	j	1750 <memset+0x82>

0000000000001874 <strcmp>:

int strcmp(const char *l, const char *r)
{
    for (; *l == *r && *l; l++, r++)
    1874:	00054783          	lbu	a5,0(a0)
    1878:	0005c703          	lbu	a4,0(a1)
    187c:	00e79863          	bne	a5,a4,188c <strcmp+0x18>
    1880:	0505                	addi	a0,a0,1
    1882:	0585                	addi	a1,a1,1
    1884:	fbe5                	bnez	a5,1874 <strcmp>
    1886:	4501                	li	a0,0
        ;
    return *(unsigned char *)l - *(unsigned char *)r;
}
    1888:	9d19                	subw	a0,a0,a4
    188a:	8082                	ret
    return *(unsigned char *)l - *(unsigned char *)r;
    188c:	0007851b          	sext.w	a0,a5
    1890:	bfe5                	j	1888 <strcmp+0x14>

0000000000001892 <strncmp>:

int strncmp(const char *_l, const char *_r, size_t n)
{
    const unsigned char *l = (void *)_l, *r = (void *)_r;
    if (!n--)
    1892:	ce05                	beqz	a2,18ca <strncmp+0x38>
        return 0;
    for (; *l && *r && n && *l == *r; l++, r++, n--)
    1894:	00054703          	lbu	a4,0(a0)
    1898:	0005c783          	lbu	a5,0(a1)
    189c:	cb0d                	beqz	a4,18ce <strncmp+0x3c>
    if (!n--)
    189e:	167d                	addi	a2,a2,-1
    18a0:	00c506b3          	add	a3,a0,a2
    18a4:	a819                	j	18ba <strncmp+0x28>
    for (; *l && *r && n && *l == *r; l++, r++, n--)
    18a6:	00a68e63          	beq	a3,a0,18c2 <strncmp+0x30>
    18aa:	0505                	addi	a0,a0,1
    18ac:	00e79b63          	bne	a5,a4,18c2 <strncmp+0x30>
    18b0:	00054703          	lbu	a4,0(a0)
        ;
    return *l - *r;
    18b4:	0005c783          	lbu	a5,0(a1)
    for (; *l && *r && n && *l == *r; l++, r++, n--)
    18b8:	cb19                	beqz	a4,18ce <strncmp+0x3c>
    18ba:	0005c783          	lbu	a5,0(a1)
    18be:	0585                	addi	a1,a1,1
    18c0:	f3fd                	bnez	a5,18a6 <strncmp+0x14>
    return *l - *r;
    18c2:	0007051b          	sext.w	a0,a4
    18c6:	9d1d                	subw	a0,a0,a5
    18c8:	8082                	ret
        return 0;
    18ca:	4501                	li	a0,0
}
    18cc:	8082                	ret
    18ce:	4501                	li	a0,0
    return *l - *r;
    18d0:	9d1d                	subw	a0,a0,a5
    18d2:	8082                	ret

00000000000018d4 <strlen>:
size_t strlen(const char *s)
{
    const char *a = s;
    typedef size_t __attribute__((__may_alias__)) word;
    const word *w;
    for (; (uintptr_t)s % SS; s++)
    18d4:	00757793          	andi	a5,a0,7
    18d8:	cf89                	beqz	a5,18f2 <strlen+0x1e>
    18da:	87aa                	mv	a5,a0
    18dc:	a029                	j	18e6 <strlen+0x12>
    18de:	0785                	addi	a5,a5,1
    18e0:	0077f713          	andi	a4,a5,7
    18e4:	cb01                	beqz	a4,18f4 <strlen+0x20>
        if (!*s)
    18e6:	0007c703          	lbu	a4,0(a5)
    18ea:	fb75                	bnez	a4,18de <strlen+0xa>
    for (w = (const void *)s; !HASZERO(*w); w++)
        ;
    s = (const void *)w;
    for (; *s; s++)
        ;
    return s - a;
    18ec:	40a78533          	sub	a0,a5,a0
}
    18f0:	8082                	ret
    for (; (uintptr_t)s % SS; s++)
    18f2:	87aa                	mv	a5,a0
    for (w = (const void *)s; !HASZERO(*w); w++)
    18f4:	6394                	ld	a3,0(a5)
    18f6:	00000597          	auipc	a1,0x0
    18fa:	6a25b583          	ld	a1,1698(a1) # 1f98 <__clone+0xac>
    18fe:	00000617          	auipc	a2,0x0
    1902:	6a263603          	ld	a2,1698(a2) # 1fa0 <__clone+0xb4>
    1906:	a019                	j	190c <strlen+0x38>
    1908:	6794                	ld	a3,8(a5)
    190a:	07a1                	addi	a5,a5,8
    190c:	00b68733          	add	a4,a3,a1
    1910:	fff6c693          	not	a3,a3
    1914:	8f75                	and	a4,a4,a3
    1916:	8f71                	and	a4,a4,a2
    1918:	db65                	beqz	a4,1908 <strlen+0x34>
    for (; *s; s++)
    191a:	0007c703          	lbu	a4,0(a5)
    191e:	d779                	beqz	a4,18ec <strlen+0x18>
    1920:	0017c703          	lbu	a4,1(a5)
    1924:	0785                	addi	a5,a5,1
    1926:	d379                	beqz	a4,18ec <strlen+0x18>
    1928:	0017c703          	lbu	a4,1(a5)
    192c:	0785                	addi	a5,a5,1
    192e:	fb6d                	bnez	a4,1920 <strlen+0x4c>
    1930:	bf75                	j	18ec <strlen+0x18>

0000000000001932 <memchr>:

void *memchr(const void *src, int c, size_t n)
{
    const unsigned char *s = src;
    c = (unsigned char)c;
    for (; ((uintptr_t)s & ALIGN) && n && *s != c; s++, n--)
    1932:	00757713          	andi	a4,a0,7
{
    1936:	87aa                	mv	a5,a0
    c = (unsigned char)c;
    1938:	0ff5f593          	andi	a1,a1,255
    for (; ((uintptr_t)s & ALIGN) && n && *s != c; s++, n--)
    193c:	cb19                	beqz	a4,1952 <memchr+0x20>
    193e:	ce25                	beqz	a2,19b6 <memchr+0x84>
    1940:	0007c703          	lbu	a4,0(a5)
    1944:	04b70e63          	beq	a4,a1,19a0 <memchr+0x6e>
    1948:	0785                	addi	a5,a5,1
    194a:	0077f713          	andi	a4,a5,7
    194e:	167d                	addi	a2,a2,-1
    1950:	f77d                	bnez	a4,193e <memchr+0xc>
            ;
        s = (const void *)w;
    }
    for (; n && *s != c; s++, n--)
        ;
    return n ? (void *)s : 0;
    1952:	4501                	li	a0,0
    if (n && *s != c)
    1954:	c235                	beqz	a2,19b8 <memchr+0x86>
    1956:	0007c703          	lbu	a4,0(a5)
    195a:	04b70363          	beq	a4,a1,19a0 <memchr+0x6e>
        size_t k = ONES * c;
    195e:	00000517          	auipc	a0,0x0
    1962:	64a53503          	ld	a0,1610(a0) # 1fa8 <__clone+0xbc>
        for (w = (const void *)s; n >= SS && !HASZERO(*w ^ k); w++, n -= SS)
    1966:	471d                	li	a4,7
        size_t k = ONES * c;
    1968:	02a58533          	mul	a0,a1,a0
        for (w = (const void *)s; n >= SS && !HASZERO(*w ^ k); w++, n -= SS)
    196c:	02c77a63          	bgeu	a4,a2,19a0 <memchr+0x6e>
    1970:	00000897          	auipc	a7,0x0
    1974:	6288b883          	ld	a7,1576(a7) # 1f98 <__clone+0xac>
    1978:	00000817          	auipc	a6,0x0
    197c:	62883803          	ld	a6,1576(a6) # 1fa0 <__clone+0xb4>
    1980:	431d                	li	t1,7
    1982:	a029                	j	198c <memchr+0x5a>
    1984:	1661                	addi	a2,a2,-8
    1986:	07a1                	addi	a5,a5,8
    1988:	02c37963          	bgeu	t1,a2,19ba <memchr+0x88>
    198c:	6398                	ld	a4,0(a5)
    198e:	8f29                	xor	a4,a4,a0
    1990:	011706b3          	add	a3,a4,a7
    1994:	fff74713          	not	a4,a4
    1998:	8f75                	and	a4,a4,a3
    199a:	01077733          	and	a4,a4,a6
    199e:	d37d                	beqz	a4,1984 <memchr+0x52>
    19a0:	853e                	mv	a0,a5
    19a2:	97b2                	add	a5,a5,a2
    19a4:	a021                	j	19ac <memchr+0x7a>
    for (; n && *s != c; s++, n--)
    19a6:	0505                	addi	a0,a0,1
    19a8:	00f50763          	beq	a0,a5,19b6 <memchr+0x84>
    19ac:	00054703          	lbu	a4,0(a0)
    19b0:	feb71be3          	bne	a4,a1,19a6 <memchr+0x74>
    19b4:	8082                	ret
    return n ? (void *)s : 0;
    19b6:	4501                	li	a0,0
}
    19b8:	8082                	ret
    return n ? (void *)s : 0;
    19ba:	4501                	li	a0,0
    for (; n && *s != c; s++, n--)
    19bc:	f275                	bnez	a2,19a0 <memchr+0x6e>
}
    19be:	8082                	ret

00000000000019c0 <strnlen>:

size_t strnlen(const char *s, size_t n)
{
    19c0:	1101                	addi	sp,sp,-32
    19c2:	e822                	sd	s0,16(sp)
    const char *p = memchr(s, 0, n);
    19c4:	862e                	mv	a2,a1
{
    19c6:	842e                	mv	s0,a1
    const char *p = memchr(s, 0, n);
    19c8:	4581                	li	a1,0
{
    19ca:	e426                	sd	s1,8(sp)
    19cc:	ec06                	sd	ra,24(sp)
    19ce:	84aa                	mv	s1,a0
    const char *p = memchr(s, 0, n);
    19d0:	f63ff0ef          	jal	ra,1932 <memchr>
    return p ? p - s : n;
    19d4:	c519                	beqz	a0,19e2 <strnlen+0x22>
}
    19d6:	60e2                	ld	ra,24(sp)
    19d8:	6442                	ld	s0,16(sp)
    return p ? p - s : n;
    19da:	8d05                	sub	a0,a0,s1
}
    19dc:	64a2                	ld	s1,8(sp)
    19de:	6105                	addi	sp,sp,32
    19e0:	8082                	ret
    19e2:	60e2                	ld	ra,24(sp)
    return p ? p - s : n;
    19e4:	8522                	mv	a0,s0
}
    19e6:	6442                	ld	s0,16(sp)
    19e8:	64a2                	ld	s1,8(sp)
    19ea:	6105                	addi	sp,sp,32
    19ec:	8082                	ret

00000000000019ee <strcpy>:
char *strcpy(char *restrict d, const char *s)
{
    typedef size_t __attribute__((__may_alias__)) word;
    word *wd;
    const word *ws;
    if ((uintptr_t)s % SS == (uintptr_t)d % SS)
    19ee:	00b547b3          	xor	a5,a0,a1
    19f2:	8b9d                	andi	a5,a5,7
    19f4:	eb95                	bnez	a5,1a28 <strcpy+0x3a>
    {
        for (; (uintptr_t)s % SS; s++, d++)
    19f6:	0075f793          	andi	a5,a1,7
    19fa:	e7b1                	bnez	a5,1a46 <strcpy+0x58>
            if (!(*d = *s))
                return d;
        wd = (void *)d;
        ws = (const void *)s;
        for (; !HASZERO(*ws); *wd++ = *ws++)
    19fc:	6198                	ld	a4,0(a1)
    19fe:	00000617          	auipc	a2,0x0
    1a02:	59a63603          	ld	a2,1434(a2) # 1f98 <__clone+0xac>
    1a06:	00000817          	auipc	a6,0x0
    1a0a:	59a83803          	ld	a6,1434(a6) # 1fa0 <__clone+0xb4>
    1a0e:	a029                	j	1a18 <strcpy+0x2a>
    1a10:	e118                	sd	a4,0(a0)
    1a12:	6598                	ld	a4,8(a1)
    1a14:	05a1                	addi	a1,a1,8
    1a16:	0521                	addi	a0,a0,8
    1a18:	00c707b3          	add	a5,a4,a2
    1a1c:	fff74693          	not	a3,a4
    1a20:	8ff5                	and	a5,a5,a3
    1a22:	0107f7b3          	and	a5,a5,a6
    1a26:	d7ed                	beqz	a5,1a10 <strcpy+0x22>
            ;
        d = (void *)wd;
        s = (const void *)ws;
    }
    for (; (*d = *s); s++, d++)
    1a28:	0005c783          	lbu	a5,0(a1)
    1a2c:	00f50023          	sb	a5,0(a0)
    1a30:	c785                	beqz	a5,1a58 <strcpy+0x6a>
    1a32:	0015c783          	lbu	a5,1(a1)
    1a36:	0505                	addi	a0,a0,1
    1a38:	0585                	addi	a1,a1,1
    1a3a:	00f50023          	sb	a5,0(a0)
    1a3e:	fbf5                	bnez	a5,1a32 <strcpy+0x44>
        ;
    return d;
}
    1a40:	8082                	ret
        for (; (uintptr_t)s % SS; s++, d++)
    1a42:	0505                	addi	a0,a0,1
    1a44:	df45                	beqz	a4,19fc <strcpy+0xe>
            if (!(*d = *s))
    1a46:	0005c783          	lbu	a5,0(a1)
        for (; (uintptr_t)s % SS; s++, d++)
    1a4a:	0585                	addi	a1,a1,1
    1a4c:	0075f713          	andi	a4,a1,7
            if (!(*d = *s))
    1a50:	00f50023          	sb	a5,0(a0)
    1a54:	f7fd                	bnez	a5,1a42 <strcpy+0x54>
}
    1a56:	8082                	ret
    1a58:	8082                	ret

0000000000001a5a <strncpy>:
char *strncpy(char *restrict d, const char *s, size_t n)
{
    typedef size_t __attribute__((__may_alias__)) word;
    word *wd;
    const word *ws;
    if (((uintptr_t)s & ALIGN) == ((uintptr_t)d & ALIGN))
    1a5a:	00b547b3          	xor	a5,a0,a1
    1a5e:	8b9d                	andi	a5,a5,7
    1a60:	1a079863          	bnez	a5,1c10 <strncpy+0x1b6>
    {
        for (; ((uintptr_t)s & ALIGN) && n && (*d = *s); n--, s++, d++)
    1a64:	0075f793          	andi	a5,a1,7
    1a68:	16078463          	beqz	a5,1bd0 <strncpy+0x176>
    1a6c:	ea01                	bnez	a2,1a7c <strncpy+0x22>
    1a6e:	a421                	j	1c76 <strncpy+0x21c>
    1a70:	167d                	addi	a2,a2,-1
    1a72:	0505                	addi	a0,a0,1
    1a74:	14070e63          	beqz	a4,1bd0 <strncpy+0x176>
    1a78:	1a060863          	beqz	a2,1c28 <strncpy+0x1ce>
    1a7c:	0005c783          	lbu	a5,0(a1)
    1a80:	0585                	addi	a1,a1,1
    1a82:	0075f713          	andi	a4,a1,7
    1a86:	00f50023          	sb	a5,0(a0)
    1a8a:	f3fd                	bnez	a5,1a70 <strncpy+0x16>
    1a8c:	4805                	li	a6,1
    1a8e:	1a061863          	bnez	a2,1c3e <strncpy+0x1e4>
    1a92:	40a007b3          	neg	a5,a0
    1a96:	8b9d                	andi	a5,a5,7
    1a98:	4681                	li	a3,0
    1a9a:	18061a63          	bnez	a2,1c2e <strncpy+0x1d4>
    1a9e:	00778713          	addi	a4,a5,7
    1aa2:	45ad                	li	a1,11
    1aa4:	18b76363          	bltu	a4,a1,1c2a <strncpy+0x1d0>
    1aa8:	1ae6eb63          	bltu	a3,a4,1c5e <strncpy+0x204>
    1aac:	1a078363          	beqz	a5,1c52 <strncpy+0x1f8>
    for (int i = 0; i < n; ++i, *(p++) = c)
    1ab0:	00050023          	sb	zero,0(a0)
    1ab4:	4685                	li	a3,1
    1ab6:	00150713          	addi	a4,a0,1
    1aba:	18d78f63          	beq	a5,a3,1c58 <strncpy+0x1fe>
    1abe:	000500a3          	sb	zero,1(a0)
    1ac2:	4689                	li	a3,2
    1ac4:	00250713          	addi	a4,a0,2
    1ac8:	18d78e63          	beq	a5,a3,1c64 <strncpy+0x20a>
    1acc:	00050123          	sb	zero,2(a0)
    1ad0:	468d                	li	a3,3
    1ad2:	00350713          	addi	a4,a0,3
    1ad6:	16d78c63          	beq	a5,a3,1c4e <strncpy+0x1f4>
    1ada:	000501a3          	sb	zero,3(a0)
    1ade:	4691                	li	a3,4
    1ae0:	00450713          	addi	a4,a0,4
    1ae4:	18d78263          	beq	a5,a3,1c68 <strncpy+0x20e>
    1ae8:	00050223          	sb	zero,4(a0)
    1aec:	4695                	li	a3,5
    1aee:	00550713          	addi	a4,a0,5
    1af2:	16d78d63          	beq	a5,a3,1c6c <strncpy+0x212>
    1af6:	000502a3          	sb	zero,5(a0)
    1afa:	469d                	li	a3,7
    1afc:	00650713          	addi	a4,a0,6
    1b00:	16d79863          	bne	a5,a3,1c70 <strncpy+0x216>
    1b04:	00750713          	addi	a4,a0,7
    1b08:	00050323          	sb	zero,6(a0)
    1b0c:	40f80833          	sub	a6,a6,a5
    1b10:	ff887593          	andi	a1,a6,-8
    1b14:	97aa                	add	a5,a5,a0
    1b16:	95be                	add	a1,a1,a5
    1b18:	0007b023          	sd	zero,0(a5)
    1b1c:	07a1                	addi	a5,a5,8
    1b1e:	feb79de3          	bne	a5,a1,1b18 <strncpy+0xbe>
    1b22:	ff887593          	andi	a1,a6,-8
    1b26:	9ead                	addw	a3,a3,a1
    1b28:	00b707b3          	add	a5,a4,a1
    1b2c:	12b80863          	beq	a6,a1,1c5c <strncpy+0x202>
    1b30:	00078023          	sb	zero,0(a5)
    1b34:	0016871b          	addiw	a4,a3,1
    1b38:	0ec77863          	bgeu	a4,a2,1c28 <strncpy+0x1ce>
    1b3c:	000780a3          	sb	zero,1(a5)
    1b40:	0026871b          	addiw	a4,a3,2
    1b44:	0ec77263          	bgeu	a4,a2,1c28 <strncpy+0x1ce>
    1b48:	00078123          	sb	zero,2(a5)
    1b4c:	0036871b          	addiw	a4,a3,3
    1b50:	0cc77c63          	bgeu	a4,a2,1c28 <strncpy+0x1ce>
    1b54:	000781a3          	sb	zero,3(a5)
    1b58:	0046871b          	addiw	a4,a3,4
    1b5c:	0cc77663          	bgeu	a4,a2,1c28 <strncpy+0x1ce>
    1b60:	00078223          	sb	zero,4(a5)
    1b64:	0056871b          	addiw	a4,a3,5
    1b68:	0cc77063          	bgeu	a4,a2,1c28 <strncpy+0x1ce>
    1b6c:	000782a3          	sb	zero,5(a5)
    1b70:	0066871b          	addiw	a4,a3,6
    1b74:	0ac77a63          	bgeu	a4,a2,1c28 <strncpy+0x1ce>
    1b78:	00078323          	sb	zero,6(a5)
    1b7c:	0076871b          	addiw	a4,a3,7
    1b80:	0ac77463          	bgeu	a4,a2,1c28 <strncpy+0x1ce>
    1b84:	000783a3          	sb	zero,7(a5)
    1b88:	0086871b          	addiw	a4,a3,8
    1b8c:	08c77e63          	bgeu	a4,a2,1c28 <strncpy+0x1ce>
    1b90:	00078423          	sb	zero,8(a5)
    1b94:	0096871b          	addiw	a4,a3,9
    1b98:	08c77863          	bgeu	a4,a2,1c28 <strncpy+0x1ce>
    1b9c:	000784a3          	sb	zero,9(a5)
    1ba0:	00a6871b          	addiw	a4,a3,10
    1ba4:	08c77263          	bgeu	a4,a2,1c28 <strncpy+0x1ce>
    1ba8:	00078523          	sb	zero,10(a5)
    1bac:	00b6871b          	addiw	a4,a3,11
    1bb0:	06c77c63          	bgeu	a4,a2,1c28 <strncpy+0x1ce>
    1bb4:	000785a3          	sb	zero,11(a5)
    1bb8:	00c6871b          	addiw	a4,a3,12
    1bbc:	06c77663          	bgeu	a4,a2,1c28 <strncpy+0x1ce>
    1bc0:	00078623          	sb	zero,12(a5)
    1bc4:	26b5                	addiw	a3,a3,13
    1bc6:	06c6f163          	bgeu	a3,a2,1c28 <strncpy+0x1ce>
    1bca:	000786a3          	sb	zero,13(a5)
    1bce:	8082                	ret
            ;
        if (!n || !*s)
    1bd0:	c645                	beqz	a2,1c78 <strncpy+0x21e>
    1bd2:	0005c783          	lbu	a5,0(a1)
    1bd6:	ea078be3          	beqz	a5,1a8c <strncpy+0x32>
            goto tail;
        wd = (void *)d;
        ws = (const void *)s;
        for (; n >= sizeof(size_t) && !HASZERO(*ws); n -= sizeof(size_t), ws++, wd++)
    1bda:	479d                	li	a5,7
    1bdc:	02c7ff63          	bgeu	a5,a2,1c1a <strncpy+0x1c0>
    1be0:	00000897          	auipc	a7,0x0
    1be4:	3b88b883          	ld	a7,952(a7) # 1f98 <__clone+0xac>
    1be8:	00000817          	auipc	a6,0x0
    1bec:	3b883803          	ld	a6,952(a6) # 1fa0 <__clone+0xb4>
    1bf0:	431d                	li	t1,7
    1bf2:	6198                	ld	a4,0(a1)
    1bf4:	011707b3          	add	a5,a4,a7
    1bf8:	fff74693          	not	a3,a4
    1bfc:	8ff5                	and	a5,a5,a3
    1bfe:	0107f7b3          	and	a5,a5,a6
    1c02:	ef81                	bnez	a5,1c1a <strncpy+0x1c0>
            *wd = *ws;
    1c04:	e118                	sd	a4,0(a0)
        for (; n >= sizeof(size_t) && !HASZERO(*ws); n -= sizeof(size_t), ws++, wd++)
    1c06:	1661                	addi	a2,a2,-8
    1c08:	05a1                	addi	a1,a1,8
    1c0a:	0521                	addi	a0,a0,8
    1c0c:	fec363e3          	bltu	t1,a2,1bf2 <strncpy+0x198>
        d = (void *)wd;
        s = (const void *)ws;
    }
    for (; n && (*d = *s); n--, s++, d++)
    1c10:	e609                	bnez	a2,1c1a <strncpy+0x1c0>
    1c12:	a08d                	j	1c74 <strncpy+0x21a>
    1c14:	167d                	addi	a2,a2,-1
    1c16:	0505                	addi	a0,a0,1
    1c18:	ca01                	beqz	a2,1c28 <strncpy+0x1ce>
    1c1a:	0005c783          	lbu	a5,0(a1)
    1c1e:	0585                	addi	a1,a1,1
    1c20:	00f50023          	sb	a5,0(a0)
    1c24:	fbe5                	bnez	a5,1c14 <strncpy+0x1ba>
        ;
tail:
    1c26:	b59d                	j	1a8c <strncpy+0x32>
    memset(d, 0, n);
    return d;
}
    1c28:	8082                	ret
    1c2a:	472d                	li	a4,11
    1c2c:	bdb5                	j	1aa8 <strncpy+0x4e>
    1c2e:	00778713          	addi	a4,a5,7
    1c32:	45ad                	li	a1,11
    1c34:	fff60693          	addi	a3,a2,-1
    1c38:	e6b778e3          	bgeu	a4,a1,1aa8 <strncpy+0x4e>
    1c3c:	b7fd                	j	1c2a <strncpy+0x1d0>
    1c3e:	40a007b3          	neg	a5,a0
    1c42:	8832                	mv	a6,a2
    1c44:	8b9d                	andi	a5,a5,7
    1c46:	4681                	li	a3,0
    1c48:	e4060be3          	beqz	a2,1a9e <strncpy+0x44>
    1c4c:	b7cd                	j	1c2e <strncpy+0x1d4>
    for (int i = 0; i < n; ++i, *(p++) = c)
    1c4e:	468d                	li	a3,3
    1c50:	bd75                	j	1b0c <strncpy+0xb2>
    1c52:	872a                	mv	a4,a0
    1c54:	4681                	li	a3,0
    1c56:	bd5d                	j	1b0c <strncpy+0xb2>
    1c58:	4685                	li	a3,1
    1c5a:	bd4d                	j	1b0c <strncpy+0xb2>
    1c5c:	8082                	ret
    1c5e:	87aa                	mv	a5,a0
    1c60:	4681                	li	a3,0
    1c62:	b5f9                	j	1b30 <strncpy+0xd6>
    1c64:	4689                	li	a3,2
    1c66:	b55d                	j	1b0c <strncpy+0xb2>
    1c68:	4691                	li	a3,4
    1c6a:	b54d                	j	1b0c <strncpy+0xb2>
    1c6c:	4695                	li	a3,5
    1c6e:	bd79                	j	1b0c <strncpy+0xb2>
    1c70:	4699                	li	a3,6
    1c72:	bd69                	j	1b0c <strncpy+0xb2>
    1c74:	8082                	ret
    1c76:	8082                	ret
    1c78:	8082                	ret

0000000000001c7a <open>:
#include <unistd.h>

#include "syscall.h"

int open(const char *path, int flags)
{
    1c7a:	87aa                	mv	a5,a0
    1c7c:	862e                	mv	a2,a1
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
}

static inline long __syscall4(long n, long a, long b, long c, long d)
{
    register long a7 __asm__("a7") = n;
    1c7e:	03800893          	li	a7,56
    register long a0 __asm__("a0") = a;
    1c82:	f9c00513          	li	a0,-100
    register long a1 __asm__("a1") = b;
    1c86:	85be                	mv	a1,a5
    register long a2 __asm__("a2") = c;
    register long a3 __asm__("a3") = d;
    1c88:	4689                	li	a3,2
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3))
    1c8a:	00000073          	ecall
    return syscall(SYS_openat, AT_FDCWD, path, flags, O_RDWR);
}
    1c8e:	2501                	sext.w	a0,a0
    1c90:	8082                	ret

0000000000001c92 <openat>:
    register long a7 __asm__("a7") = n;
    1c92:	03800893          	li	a7,56
    register long a3 __asm__("a3") = d;
    1c96:	18000693          	li	a3,384
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3))
    1c9a:	00000073          	ecall

int openat(int dirfd,const char *path, int flags)
{
    return syscall(SYS_openat, dirfd, path, flags, 0600);
}
    1c9e:	2501                	sext.w	a0,a0
    1ca0:	8082                	ret

0000000000001ca2 <close>:
    register long a7 __asm__("a7") = n;
    1ca2:	03900893          	li	a7,57
    __asm_syscall("r"(a7), "0"(a0))
    1ca6:	00000073          	ecall

int close(int fd)
{
    return syscall(SYS_close, fd);
}
    1caa:	2501                	sext.w	a0,a0
    1cac:	8082                	ret

0000000000001cae <read>:
    register long a7 __asm__("a7") = n;
    1cae:	03f00893          	li	a7,63
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1cb2:	00000073          	ecall

ssize_t read(int fd, void *buf, size_t len)
{
    return syscall(SYS_read, fd, buf, len);
}
    1cb6:	8082                	ret

0000000000001cb8 <write>:
    register long a7 __asm__("a7") = n;
    1cb8:	04000893          	li	a7,64
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1cbc:	00000073          	ecall

ssize_t write(int fd, const void *buf, size_t len)
{
    return syscall(SYS_write, fd, buf, len);
}
    1cc0:	8082                	ret

0000000000001cc2 <getpid>:
    register long a7 __asm__("a7") = n;
    1cc2:	0ac00893          	li	a7,172
    __asm_syscall("r"(a7))
    1cc6:	00000073          	ecall

pid_t getpid(void)
{
    return syscall(SYS_getpid);
}
    1cca:	2501                	sext.w	a0,a0
    1ccc:	8082                	ret

0000000000001cce <getppid>:
    register long a7 __asm__("a7") = n;
    1cce:	0ad00893          	li	a7,173
    __asm_syscall("r"(a7))
    1cd2:	00000073          	ecall

pid_t getppid(void)
{
    return syscall(SYS_getppid);
}
    1cd6:	2501                	sext.w	a0,a0
    1cd8:	8082                	ret

0000000000001cda <sched_yield>:
    register long a7 __asm__("a7") = n;
    1cda:	07c00893          	li	a7,124
    __asm_syscall("r"(a7))
    1cde:	00000073          	ecall

int sched_yield(void)
{
    return syscall(SYS_sched_yield);
}
    1ce2:	2501                	sext.w	a0,a0
    1ce4:	8082                	ret

0000000000001ce6 <fork>:
    register long a7 __asm__("a7") = n;
    1ce6:	0dc00893          	li	a7,220
    register long a0 __asm__("a0") = a;
    1cea:	4545                	li	a0,17
    register long a1 __asm__("a1") = b;
    1cec:	4581                	li	a1,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1cee:	00000073          	ecall

pid_t fork(void)
{
    return syscall(SYS_clone, SIGCHLD, 0);
}
    1cf2:	2501                	sext.w	a0,a0
    1cf4:	8082                	ret

0000000000001cf6 <clone>:

pid_t clone(int (*fn)(void *arg), void *arg, void *stack, size_t stack_size, unsigned long flags)
{
    1cf6:	85b2                	mv	a1,a2
    1cf8:	863a                	mv	a2,a4
    if (stack)
    1cfa:	c191                	beqz	a1,1cfe <clone+0x8>
	stack += stack_size;
    1cfc:	95b6                	add	a1,a1,a3

    return __clone(fn, stack, flags, NULL, NULL, NULL);
    1cfe:	4781                	li	a5,0
    1d00:	4701                	li	a4,0
    1d02:	4681                	li	a3,0
    1d04:	2601                	sext.w	a2,a2
    1d06:	a2dd                	j	1eec <__clone>

0000000000001d08 <exit>:
    register long a7 __asm__("a7") = n;
    1d08:	05d00893          	li	a7,93
    __asm_syscall("r"(a7), "0"(a0))
    1d0c:	00000073          	ecall
    //return syscall(SYS_clone, fn, stack, flags, NULL, NULL, NULL);
}
void exit(int code)
{
    syscall(SYS_exit, code);
}
    1d10:	8082                	ret

0000000000001d12 <waitpid>:
    register long a7 __asm__("a7") = n;
    1d12:	10400893          	li	a7,260
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1d16:	00000073          	ecall

int waitpid(int pid, int *code, int options)
{
    return syscall(SYS_wait4, pid, code, options);
}
    1d1a:	2501                	sext.w	a0,a0
    1d1c:	8082                	ret

0000000000001d1e <exec>:
    register long a7 __asm__("a7") = n;
    1d1e:	0dd00893          	li	a7,221
    __asm_syscall("r"(a7), "0"(a0))
    1d22:	00000073          	ecall

int exec(char *name)
{
    return syscall(SYS_execve, name);
}
    1d26:	2501                	sext.w	a0,a0
    1d28:	8082                	ret

0000000000001d2a <execve>:
    register long a7 __asm__("a7") = n;
    1d2a:	0dd00893          	li	a7,221
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1d2e:	00000073          	ecall

int execve(const char *name, char *const argv[], char *const argp[])
{
    return syscall(SYS_execve, name, argv, argp);
}
    1d32:	2501                	sext.w	a0,a0
    1d34:	8082                	ret

0000000000001d36 <times>:
    register long a7 __asm__("a7") = n;
    1d36:	09900893          	li	a7,153
    __asm_syscall("r"(a7), "0"(a0))
    1d3a:	00000073          	ecall

int times(void *mytimes)
{
	return syscall(SYS_times, mytimes);
}
    1d3e:	2501                	sext.w	a0,a0
    1d40:	8082                	ret

0000000000001d42 <get_time>:

int64 get_time()
{
    1d42:	1141                	addi	sp,sp,-16
    register long a7 __asm__("a7") = n;
    1d44:	0a900893          	li	a7,169
    register long a0 __asm__("a0") = a;
    1d48:	850a                	mv	a0,sp
    register long a1 __asm__("a1") = b;
    1d4a:	4581                	li	a1,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1d4c:	00000073          	ecall
    TimeVal time;
    int err = sys_get_time(&time, 0);
    if (err == 0)
    1d50:	2501                	sext.w	a0,a0
    1d52:	ed09                	bnez	a0,1d6c <get_time+0x2a>
    {
        return ((time.sec & 0xffff) * 1000 + time.usec / 1000);
    1d54:	67a2                	ld	a5,8(sp)
    1d56:	3e800713          	li	a4,1000
    1d5a:	00015503          	lhu	a0,0(sp)
    1d5e:	02e7d7b3          	divu	a5,a5,a4
    1d62:	02e50533          	mul	a0,a0,a4
    1d66:	953e                	add	a0,a0,a5
    }
    else
    {
        return -1;
    }
}
    1d68:	0141                	addi	sp,sp,16
    1d6a:	8082                	ret
        return -1;
    1d6c:	557d                	li	a0,-1
    1d6e:	bfed                	j	1d68 <get_time+0x26>

0000000000001d70 <sys_get_time>:
    register long a7 __asm__("a7") = n;
    1d70:	0a900893          	li	a7,169
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1d74:	00000073          	ecall

int sys_get_time(TimeVal *ts, int tz)
{
    return syscall(SYS_gettimeofday, ts, tz);
}
    1d78:	2501                	sext.w	a0,a0
    1d7a:	8082                	ret

0000000000001d7c <time>:
    register long a7 __asm__("a7") = n;
    1d7c:	42600893          	li	a7,1062
    __asm_syscall("r"(a7), "0"(a0))
    1d80:	00000073          	ecall

int time(unsigned long *tloc)
{
    return syscall(SYS_time, tloc);
}
    1d84:	2501                	sext.w	a0,a0
    1d86:	8082                	ret

0000000000001d88 <sleep>:

int sleep(unsigned long long time)
{
    1d88:	1141                	addi	sp,sp,-16
    TimeVal tv = {.sec = time, .usec = 0};
    1d8a:	e02a                	sd	a0,0(sp)
    register long a0 __asm__("a0") = a;
    1d8c:	850a                	mv	a0,sp
    1d8e:	e402                	sd	zero,8(sp)
    register long a7 __asm__("a7") = n;
    1d90:	06500893          	li	a7,101
    register long a1 __asm__("a1") = b;
    1d94:	85aa                	mv	a1,a0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1d96:	00000073          	ecall
    if (syscall(SYS_nanosleep, &tv, &tv)) return tv.sec;
    1d9a:	e501                	bnez	a0,1da2 <sleep+0x1a>
    return 0;
    1d9c:	4501                	li	a0,0
}
    1d9e:	0141                	addi	sp,sp,16
    1da0:	8082                	ret
    if (syscall(SYS_nanosleep, &tv, &tv)) return tv.sec;
    1da2:	4502                	lw	a0,0(sp)
}
    1da4:	0141                	addi	sp,sp,16
    1da6:	8082                	ret

0000000000001da8 <set_priority>:
    register long a7 __asm__("a7") = n;
    1da8:	08c00893          	li	a7,140
    __asm_syscall("r"(a7), "0"(a0))
    1dac:	00000073          	ecall

int set_priority(int prio)
{
    return syscall(SYS_setpriority, prio);
}
    1db0:	2501                	sext.w	a0,a0
    1db2:	8082                	ret

0000000000001db4 <mmap>:
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4))
}

static inline long __syscall6(long n, long a, long b, long c, long d, long e, long f)
{
    register long a7 __asm__("a7") = n;
    1db4:	0de00893          	li	a7,222
    register long a1 __asm__("a1") = b;
    register long a2 __asm__("a2") = c;
    register long a3 __asm__("a3") = d;
    register long a4 __asm__("a4") = e;
    register long a5 __asm__("a5") = f;
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4), "r"(a5))
    1db8:	00000073          	ecall

void *mmap(void *start, size_t len, int prot, int flags, int fd, off_t off)
{
    return syscall(SYS_mmap, start, len, prot, flags, fd, off);
}
    1dbc:	8082                	ret

0000000000001dbe <munmap>:
    register long a7 __asm__("a7") = n;
    1dbe:	0d700893          	li	a7,215
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1dc2:	00000073          	ecall

int munmap(void *start, size_t len)
{
    return syscall(SYS_munmap, start, len);
}
    1dc6:	2501                	sext.w	a0,a0
    1dc8:	8082                	ret

0000000000001dca <wait>:

int wait(int *code)
{
    1dca:	85aa                	mv	a1,a0
    register long a7 __asm__("a7") = n;
    1dcc:	10400893          	li	a7,260
    register long a0 __asm__("a0") = a;
    1dd0:	557d                	li	a0,-1
    register long a2 __asm__("a2") = c;
    1dd2:	4601                	li	a2,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1dd4:	00000073          	ecall
    return waitpid(-1, code, 0);
}
    1dd8:	2501                	sext.w	a0,a0
    1dda:	8082                	ret

0000000000001ddc <spawn>:
    register long a7 __asm__("a7") = n;
    1ddc:	19000893          	li	a7,400
    __asm_syscall("r"(a7), "0"(a0))
    1de0:	00000073          	ecall

int spawn(char *file)
{
    return syscall(SYS_spawn, file);
}
    1de4:	2501                	sext.w	a0,a0
    1de6:	8082                	ret

0000000000001de8 <mailread>:
    register long a7 __asm__("a7") = n;
    1de8:	19100893          	li	a7,401
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1dec:	00000073          	ecall

int mailread(void *buf, int len)
{
    return syscall(SYS_mailread, buf, len);
}
    1df0:	2501                	sext.w	a0,a0
    1df2:	8082                	ret

0000000000001df4 <mailwrite>:
    register long a7 __asm__("a7") = n;
    1df4:	19200893          	li	a7,402
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1df8:	00000073          	ecall

int mailwrite(int pid, void *buf, int len)
{
    return syscall(SYS_mailwrite, pid, buf, len);
}
    1dfc:	2501                	sext.w	a0,a0
    1dfe:	8082                	ret

0000000000001e00 <fstat>:
    register long a7 __asm__("a7") = n;
    1e00:	05000893          	li	a7,80
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1e04:	00000073          	ecall

int fstat(int fd, struct kstat *st)
{
    return syscall(SYS_fstat, fd, st);
}
    1e08:	2501                	sext.w	a0,a0
    1e0a:	8082                	ret

0000000000001e0c <sys_linkat>:
    register long a4 __asm__("a4") = e;
    1e0c:	1702                	slli	a4,a4,0x20
    register long a7 __asm__("a7") = n;
    1e0e:	02500893          	li	a7,37
    register long a4 __asm__("a4") = e;
    1e12:	9301                	srli	a4,a4,0x20
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4))
    1e14:	00000073          	ecall

int sys_linkat(int olddirfd, char *oldpath, int newdirfd, char *newpath, unsigned int flags)
{
    return syscall(SYS_linkat, olddirfd, oldpath, newdirfd, newpath, flags);
}
    1e18:	2501                	sext.w	a0,a0
    1e1a:	8082                	ret

0000000000001e1c <sys_unlinkat>:
    register long a2 __asm__("a2") = c;
    1e1c:	1602                	slli	a2,a2,0x20
    register long a7 __asm__("a7") = n;
    1e1e:	02300893          	li	a7,35
    register long a2 __asm__("a2") = c;
    1e22:	9201                	srli	a2,a2,0x20
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1e24:	00000073          	ecall

int sys_unlinkat(int dirfd, char *path, unsigned int flags)
{
    return syscall(SYS_unlinkat, dirfd, path, flags);
}
    1e28:	2501                	sext.w	a0,a0
    1e2a:	8082                	ret

0000000000001e2c <link>:

int link(char *old_path, char *new_path)
{
    1e2c:	87aa                	mv	a5,a0
    1e2e:	86ae                	mv	a3,a1
    register long a7 __asm__("a7") = n;
    1e30:	02500893          	li	a7,37
    register long a0 __asm__("a0") = a;
    1e34:	f9c00513          	li	a0,-100
    register long a1 __asm__("a1") = b;
    1e38:	85be                	mv	a1,a5
    register long a2 __asm__("a2") = c;
    1e3a:	f9c00613          	li	a2,-100
    register long a4 __asm__("a4") = e;
    1e3e:	4701                	li	a4,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4))
    1e40:	00000073          	ecall
    return sys_linkat(AT_FDCWD, old_path, AT_FDCWD, new_path, 0);
}
    1e44:	2501                	sext.w	a0,a0
    1e46:	8082                	ret

0000000000001e48 <unlink>:

int unlink(char *path)
{
    1e48:	85aa                	mv	a1,a0
    register long a7 __asm__("a7") = n;
    1e4a:	02300893          	li	a7,35
    register long a0 __asm__("a0") = a;
    1e4e:	f9c00513          	li	a0,-100
    register long a2 __asm__("a2") = c;
    1e52:	4601                	li	a2,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1e54:	00000073          	ecall
    return sys_unlinkat(AT_FDCWD, path, 0);
}
    1e58:	2501                	sext.w	a0,a0
    1e5a:	8082                	ret

0000000000001e5c <uname>:
    register long a7 __asm__("a7") = n;
    1e5c:	0a000893          	li	a7,160
    __asm_syscall("r"(a7), "0"(a0))
    1e60:	00000073          	ecall

int uname(void *buf)
{
    return syscall(SYS_uname, buf);
}
    1e64:	2501                	sext.w	a0,a0
    1e66:	8082                	ret

0000000000001e68 <brk>:
    register long a7 __asm__("a7") = n;
    1e68:	0d600893          	li	a7,214
    __asm_syscall("r"(a7), "0"(a0))
    1e6c:	00000073          	ecall

int brk(void *addr)
{
    return syscall(SYS_brk, addr);
}
    1e70:	2501                	sext.w	a0,a0
    1e72:	8082                	ret

0000000000001e74 <getcwd>:
    register long a7 __asm__("a7") = n;
    1e74:	48c5                	li	a7,17
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1e76:	00000073          	ecall

char *getcwd(char *buf, size_t size){
    return syscall(SYS_getcwd, buf, size);
}
    1e7a:	8082                	ret

0000000000001e7c <chdir>:
    register long a7 __asm__("a7") = n;
    1e7c:	03100893          	li	a7,49
    __asm_syscall("r"(a7), "0"(a0))
    1e80:	00000073          	ecall

int chdir(const char *path){
    return syscall(SYS_chdir, path);
}
    1e84:	2501                	sext.w	a0,a0
    1e86:	8082                	ret

0000000000001e88 <mkdir>:

int mkdir(const char *path, mode_t mode){
    1e88:	862e                	mv	a2,a1
    1e8a:	87aa                	mv	a5,a0
    register long a2 __asm__("a2") = c;
    1e8c:	1602                	slli	a2,a2,0x20
    register long a7 __asm__("a7") = n;
    1e8e:	02200893          	li	a7,34
    register long a0 __asm__("a0") = a;
    1e92:	f9c00513          	li	a0,-100
    register long a1 __asm__("a1") = b;
    1e96:	85be                	mv	a1,a5
    register long a2 __asm__("a2") = c;
    1e98:	9201                	srli	a2,a2,0x20
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1e9a:	00000073          	ecall
    return syscall(SYS_mkdirat, AT_FDCWD, path, mode);
}
    1e9e:	2501                	sext.w	a0,a0
    1ea0:	8082                	ret

0000000000001ea2 <getdents>:
    register long a7 __asm__("a7") = n;
    1ea2:	03d00893          	li	a7,61
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1ea6:	00000073          	ecall

int getdents(int fd, struct linux_dirent64 *dirp64, unsigned long len){
    //return syscall(SYS_getdents64, fd, dirp64, len);
    return syscall(SYS_getdents64, fd, dirp64, len);
}
    1eaa:	2501                	sext.w	a0,a0
    1eac:	8082                	ret

0000000000001eae <pipe>:
    register long a7 __asm__("a7") = n;
    1eae:	03b00893          	li	a7,59
    register long a1 __asm__("a1") = b;
    1eb2:	4581                	li	a1,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1eb4:	00000073          	ecall

int pipe(int fd[2]){
    return syscall(SYS_pipe2, fd, 0);
}
    1eb8:	2501                	sext.w	a0,a0
    1eba:	8082                	ret

0000000000001ebc <dup>:
    register long a7 __asm__("a7") = n;
    1ebc:	48dd                	li	a7,23
    __asm_syscall("r"(a7), "0"(a0))
    1ebe:	00000073          	ecall

int dup(int fd){
    return syscall(SYS_dup, fd);
}
    1ec2:	2501                	sext.w	a0,a0
    1ec4:	8082                	ret

0000000000001ec6 <dup2>:
    register long a7 __asm__("a7") = n;
    1ec6:	48e1                	li	a7,24
    register long a2 __asm__("a2") = c;
    1ec8:	4601                	li	a2,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1eca:	00000073          	ecall

int dup2(int old, int new){
    return syscall(SYS_dup3, old, new, 0);
}
    1ece:	2501                	sext.w	a0,a0
    1ed0:	8082                	ret

0000000000001ed2 <mount>:
    register long a7 __asm__("a7") = n;
    1ed2:	02800893          	li	a7,40
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4))
    1ed6:	00000073          	ecall

int mount(const char *special, const char *dir, const char *fstype, unsigned long flags, const void *data)
{
        return syscall(SYS_mount, special, dir, fstype, flags, data);
}
    1eda:	2501                	sext.w	a0,a0
    1edc:	8082                	ret

0000000000001ede <umount>:
    register long a7 __asm__("a7") = n;
    1ede:	02700893          	li	a7,39
    register long a1 __asm__("a1") = b;
    1ee2:	4581                	li	a1,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1ee4:	00000073          	ecall

int umount(const char *special)
{
        return syscall(SYS_umount2, special, 0);
}
    1ee8:	2501                	sext.w	a0,a0
    1eea:	8082                	ret

0000000000001eec <__clone>:

.global __clone
.type  __clone, %function
__clone:
	# Save func and arg to stack
	addi a1, a1, -16
    1eec:	15c1                	addi	a1,a1,-16
	sd a0, 0(a1)
    1eee:	e188                	sd	a0,0(a1)
	sd a3, 8(a1)
    1ef0:	e594                	sd	a3,8(a1)

	# Call SYS_clone
	mv a0, a2
    1ef2:	8532                	mv	a0,a2
	mv a2, a4
    1ef4:	863a                	mv	a2,a4
	mv a3, a5
    1ef6:	86be                	mv	a3,a5
	mv a4, a6
    1ef8:	8742                	mv	a4,a6
	li a7, 220 # SYS_clone
    1efa:	0dc00893          	li	a7,220
	ecall
    1efe:	00000073          	ecall

	beqz a0, 1f
    1f02:	c111                	beqz	a0,1f06 <__clone+0x1a>
	# Parent
	ret
    1f04:	8082                	ret

	# Child
1:      ld a1, 0(sp)
    1f06:	6582                	ld	a1,0(sp)
	ld a0, 8(sp)
    1f08:	6522                	ld	a0,8(sp)
	jalr a1
    1f0a:	9582                	jalr	a1

	# Exit
	li a7, 93 # SYS_exit
    1f0c:	05d00893          	li	a7,93
	ecall
    1f10:	00000073          	ecall
