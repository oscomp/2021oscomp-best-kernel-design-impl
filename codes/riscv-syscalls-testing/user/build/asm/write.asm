
/home/ubuntu/rust/ultra-os/codes/riscv-syscalls-testing/user/build/riscv64/write:     file format elf64-littleriscv


Disassembly of section .text:

0000000000001000 <_start>:
.section .text.entry
.globl _start
_start:
    mv a0, sp
    1000:	850a                	mv	a0,sp
    tail __start_main
    1002:	a851                	j	1096 <__start_main>

0000000000001004 <test_write>:
#include "unistd.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

void test_write(){
    1004:	1141                	addi	sp,sp,-16
	TEST_START(__func__);
    1006:	00001517          	auipc	a0,0x1
    100a:	eca50513          	addi	a0,a0,-310 # 1ed0 <__clone+0x2c>
void test_write(){
    100e:	e406                	sd	ra,8(sp)
    1010:	e022                	sd	s0,0(sp)
	TEST_START(__func__);
    1012:	2f6000ef          	jal	ra,1308 <puts>
    1016:	00001517          	auipc	a0,0x1
    101a:	f5a50513          	addi	a0,a0,-166 # 1f70 <__func__.0>
    101e:	2ea000ef          	jal	ra,1308 <puts>
    1022:	00001517          	auipc	a0,0x1
    1026:	ec650513          	addi	a0,a0,-314 # 1ee8 <__clone+0x44>
    102a:	2de000ef          	jal	ra,1308 <puts>
	const char *str = "Hello operating system contest.\n";
	int str_len = strlen(str);
    102e:	00001517          	auipc	a0,0x1
    1032:	eca50513          	addi	a0,a0,-310 # 1ef8 <__clone+0x54>
    1036:	057000ef          	jal	ra,188c <strlen>
    103a:	0005041b          	sext.w	s0,a0
	assert(write(STDOUT, str, str_len) == str_len);
    103e:	8622                	mv	a2,s0
    1040:	00001597          	auipc	a1,0x1
    1044:	eb858593          	addi	a1,a1,-328 # 1ef8 <__clone+0x54>
    1048:	4505                	li	a0,1
    104a:	427000ef          	jal	ra,1c70 <write>
    104e:	00850863          	beq	a0,s0,105e <test_write+0x5a>
    1052:	00001517          	auipc	a0,0x1
    1056:	ece50513          	addi	a0,a0,-306 # 1f20 <__clone+0x7c>
    105a:	554000ef          	jal	ra,15ae <panic>
	TEST_END(__func__);
    105e:	00001517          	auipc	a0,0x1
    1062:	ee250513          	addi	a0,a0,-286 # 1f40 <__clone+0x9c>
    1066:	2a2000ef          	jal	ra,1308 <puts>
    106a:	00001517          	auipc	a0,0x1
    106e:	f0650513          	addi	a0,a0,-250 # 1f70 <__func__.0>
    1072:	296000ef          	jal	ra,1308 <puts>
}
    1076:	6402                	ld	s0,0(sp)
    1078:	60a2                	ld	ra,8(sp)
	TEST_END(__func__);
    107a:	00001517          	auipc	a0,0x1
    107e:	e6e50513          	addi	a0,a0,-402 # 1ee8 <__clone+0x44>
}
    1082:	0141                	addi	sp,sp,16
	TEST_END(__func__);
    1084:	a451                	j	1308 <puts>

0000000000001086 <main>:

int main(void) {
    1086:	1141                	addi	sp,sp,-16
    1088:	e406                	sd	ra,8(sp)
	test_write();
    108a:	f7bff0ef          	jal	ra,1004 <test_write>
	return 0;
}
    108e:	60a2                	ld	ra,8(sp)
    1090:	4501                	li	a0,0
    1092:	0141                	addi	sp,sp,16
    1094:	8082                	ret

0000000000001096 <__start_main>:
#include <unistd.h>

extern int main();

int __start_main(long *p)
{
    1096:	85aa                	mv	a1,a0
	int argc = p[0];
	char **argv = (void *)(p+1);

	exit(main(argc, argv));
    1098:	4108                	lw	a0,0(a0)
{
    109a:	1141                	addi	sp,sp,-16
	exit(main(argc, argv));
    109c:	05a1                	addi	a1,a1,8
{
    109e:	e406                	sd	ra,8(sp)
	exit(main(argc, argv));
    10a0:	fe7ff0ef          	jal	ra,1086 <main>
    10a4:	41d000ef          	jal	ra,1cc0 <exit>
	return 0;
}
    10a8:	60a2                	ld	ra,8(sp)
    10aa:	4501                	li	a0,0
    10ac:	0141                	addi	sp,sp,16
    10ae:	8082                	ret

00000000000010b0 <printint.constprop.0>:
    write(f, s, l);
}

static char digits[] = "0123456789abcdef";

static void printint(int xx, int base, int sign)
    10b0:	7179                	addi	sp,sp,-48
    10b2:	f406                	sd	ra,40(sp)
{
    char buf[16 + 1];
    int i;
    uint x;

    if (sign && (sign = xx < 0))
    10b4:	12054b63          	bltz	a0,11ea <printint.constprop.0+0x13a>

    buf[16] = 0;
    i = 15;
    do
    {
        buf[i--] = digits[x % base];
    10b8:	02b577bb          	remuw	a5,a0,a1
    10bc:	00001617          	auipc	a2,0x1
    10c0:	ec460613          	addi	a2,a2,-316 # 1f80 <digits>
    buf[16] = 0;
    10c4:	00010c23          	sb	zero,24(sp)
        buf[i--] = digits[x % base];
    10c8:	0005871b          	sext.w	a4,a1
    10cc:	1782                	slli	a5,a5,0x20
    10ce:	9381                	srli	a5,a5,0x20
    10d0:	97b2                	add	a5,a5,a2
    10d2:	0007c783          	lbu	a5,0(a5)
    } while ((x /= base) != 0);
    10d6:	02b5583b          	divuw	a6,a0,a1
        buf[i--] = digits[x % base];
    10da:	00f10ba3          	sb	a5,23(sp)
    } while ((x /= base) != 0);
    10de:	1cb56363          	bltu	a0,a1,12a4 <printint.constprop.0+0x1f4>
        buf[i--] = digits[x % base];
    10e2:	45b9                	li	a1,14
    10e4:	02e877bb          	remuw	a5,a6,a4
    10e8:	1782                	slli	a5,a5,0x20
    10ea:	9381                	srli	a5,a5,0x20
    10ec:	97b2                	add	a5,a5,a2
    10ee:	0007c783          	lbu	a5,0(a5)
    } while ((x /= base) != 0);
    10f2:	02e856bb          	divuw	a3,a6,a4
        buf[i--] = digits[x % base];
    10f6:	00f10b23          	sb	a5,22(sp)
    } while ((x /= base) != 0);
    10fa:	0ce86e63          	bltu	a6,a4,11d6 <printint.constprop.0+0x126>
        buf[i--] = digits[x % base];
    10fe:	02e6f5bb          	remuw	a1,a3,a4
    } while ((x /= base) != 0);
    1102:	02e6d7bb          	divuw	a5,a3,a4
        buf[i--] = digits[x % base];
    1106:	1582                	slli	a1,a1,0x20
    1108:	9181                	srli	a1,a1,0x20
    110a:	95b2                	add	a1,a1,a2
    110c:	0005c583          	lbu	a1,0(a1)
    1110:	00b10aa3          	sb	a1,21(sp)
    } while ((x /= base) != 0);
    1114:	0007859b          	sext.w	a1,a5
    1118:	12e6ec63          	bltu	a3,a4,1250 <printint.constprop.0+0x1a0>
        buf[i--] = digits[x % base];
    111c:	02e7f6bb          	remuw	a3,a5,a4
    1120:	1682                	slli	a3,a3,0x20
    1122:	9281                	srli	a3,a3,0x20
    1124:	96b2                	add	a3,a3,a2
    1126:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    112a:	02e7d83b          	divuw	a6,a5,a4
        buf[i--] = digits[x % base];
    112e:	00d10a23          	sb	a3,20(sp)
    } while ((x /= base) != 0);
    1132:	12e5e863          	bltu	a1,a4,1262 <printint.constprop.0+0x1b2>
        buf[i--] = digits[x % base];
    1136:	02e876bb          	remuw	a3,a6,a4
    113a:	1682                	slli	a3,a3,0x20
    113c:	9281                	srli	a3,a3,0x20
    113e:	96b2                	add	a3,a3,a2
    1140:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    1144:	02e855bb          	divuw	a1,a6,a4
        buf[i--] = digits[x % base];
    1148:	00d109a3          	sb	a3,19(sp)
    } while ((x /= base) != 0);
    114c:	12e86463          	bltu	a6,a4,1274 <printint.constprop.0+0x1c4>
        buf[i--] = digits[x % base];
    1150:	02e5f6bb          	remuw	a3,a1,a4
    1154:	1682                	slli	a3,a3,0x20
    1156:	9281                	srli	a3,a3,0x20
    1158:	96b2                	add	a3,a3,a2
    115a:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    115e:	02e5d83b          	divuw	a6,a1,a4
        buf[i--] = digits[x % base];
    1162:	00d10923          	sb	a3,18(sp)
    } while ((x /= base) != 0);
    1166:	0ce5ec63          	bltu	a1,a4,123e <printint.constprop.0+0x18e>
        buf[i--] = digits[x % base];
    116a:	02e876bb          	remuw	a3,a6,a4
    116e:	1682                	slli	a3,a3,0x20
    1170:	9281                	srli	a3,a3,0x20
    1172:	96b2                	add	a3,a3,a2
    1174:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    1178:	02e855bb          	divuw	a1,a6,a4
        buf[i--] = digits[x % base];
    117c:	00d108a3          	sb	a3,17(sp)
    } while ((x /= base) != 0);
    1180:	10e86963          	bltu	a6,a4,1292 <printint.constprop.0+0x1e2>
        buf[i--] = digits[x % base];
    1184:	02e5f6bb          	remuw	a3,a1,a4
    1188:	1682                	slli	a3,a3,0x20
    118a:	9281                	srli	a3,a3,0x20
    118c:	96b2                	add	a3,a3,a2
    118e:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    1192:	02e5d83b          	divuw	a6,a1,a4
        buf[i--] = digits[x % base];
    1196:	00d10823          	sb	a3,16(sp)
    } while ((x /= base) != 0);
    119a:	10e5e763          	bltu	a1,a4,12a8 <printint.constprop.0+0x1f8>
        buf[i--] = digits[x % base];
    119e:	02e876bb          	remuw	a3,a6,a4
    11a2:	1682                	slli	a3,a3,0x20
    11a4:	9281                	srli	a3,a3,0x20
    11a6:	96b2                	add	a3,a3,a2
    11a8:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    11ac:	02e857bb          	divuw	a5,a6,a4
        buf[i--] = digits[x % base];
    11b0:	00d107a3          	sb	a3,15(sp)
    } while ((x /= base) != 0);
    11b4:	10e86363          	bltu	a6,a4,12ba <printint.constprop.0+0x20a>
        buf[i--] = digits[x % base];
    11b8:	1782                	slli	a5,a5,0x20
    11ba:	9381                	srli	a5,a5,0x20
    11bc:	97b2                	add	a5,a5,a2
    11be:	0007c783          	lbu	a5,0(a5)
    11c2:	4599                	li	a1,6
    11c4:	00f10723          	sb	a5,14(sp)

    if (sign)
    11c8:	00055763          	bgez	a0,11d6 <printint.constprop.0+0x126>
        buf[i--] = '-';
    11cc:	02d00793          	li	a5,45
    11d0:	00f106a3          	sb	a5,13(sp)
        buf[i--] = digits[x % base];
    11d4:	4595                	li	a1,5
    write(f, s, l);
    11d6:	003c                	addi	a5,sp,8
    11d8:	4641                	li	a2,16
    11da:	9e0d                	subw	a2,a2,a1
    11dc:	4505                	li	a0,1
    11de:	95be                	add	a1,a1,a5
    11e0:	291000ef          	jal	ra,1c70 <write>
    i++;
    if (i < 0)
        puts("printint error");
    out(stdout, buf + i, 16 - i);
}
    11e4:	70a2                	ld	ra,40(sp)
    11e6:	6145                	addi	sp,sp,48
    11e8:	8082                	ret
        x = -xx;
    11ea:	40a0083b          	negw	a6,a0
        buf[i--] = digits[x % base];
    11ee:	02b877bb          	remuw	a5,a6,a1
    11f2:	00001617          	auipc	a2,0x1
    11f6:	d8e60613          	addi	a2,a2,-626 # 1f80 <digits>
    buf[16] = 0;
    11fa:	00010c23          	sb	zero,24(sp)
        buf[i--] = digits[x % base];
    11fe:	0005871b          	sext.w	a4,a1
    1202:	1782                	slli	a5,a5,0x20
    1204:	9381                	srli	a5,a5,0x20
    1206:	97b2                	add	a5,a5,a2
    1208:	0007c783          	lbu	a5,0(a5)
    } while ((x /= base) != 0);
    120c:	02b858bb          	divuw	a7,a6,a1
        buf[i--] = digits[x % base];
    1210:	00f10ba3          	sb	a5,23(sp)
    } while ((x /= base) != 0);
    1214:	06b86963          	bltu	a6,a1,1286 <printint.constprop.0+0x1d6>
        buf[i--] = digits[x % base];
    1218:	02e8f7bb          	remuw	a5,a7,a4
    121c:	1782                	slli	a5,a5,0x20
    121e:	9381                	srli	a5,a5,0x20
    1220:	97b2                	add	a5,a5,a2
    1222:	0007c783          	lbu	a5,0(a5)
    } while ((x /= base) != 0);
    1226:	02e8d6bb          	divuw	a3,a7,a4
        buf[i--] = digits[x % base];
    122a:	00f10b23          	sb	a5,22(sp)
    } while ((x /= base) != 0);
    122e:	ece8f8e3          	bgeu	a7,a4,10fe <printint.constprop.0+0x4e>
        buf[i--] = '-';
    1232:	02d00793          	li	a5,45
    1236:	00f10aa3          	sb	a5,21(sp)
        buf[i--] = digits[x % base];
    123a:	45b5                	li	a1,13
    123c:	bf69                	j	11d6 <printint.constprop.0+0x126>
    123e:	45a9                	li	a1,10
    if (sign)
    1240:	f8055be3          	bgez	a0,11d6 <printint.constprop.0+0x126>
        buf[i--] = '-';
    1244:	02d00793          	li	a5,45
    1248:	00f108a3          	sb	a5,17(sp)
        buf[i--] = digits[x % base];
    124c:	45a5                	li	a1,9
    124e:	b761                	j	11d6 <printint.constprop.0+0x126>
    1250:	45b5                	li	a1,13
    if (sign)
    1252:	f80552e3          	bgez	a0,11d6 <printint.constprop.0+0x126>
        buf[i--] = '-';
    1256:	02d00793          	li	a5,45
    125a:	00f10a23          	sb	a5,20(sp)
        buf[i--] = digits[x % base];
    125e:	45b1                	li	a1,12
    1260:	bf9d                	j	11d6 <printint.constprop.0+0x126>
    1262:	45b1                	li	a1,12
    if (sign)
    1264:	f60559e3          	bgez	a0,11d6 <printint.constprop.0+0x126>
        buf[i--] = '-';
    1268:	02d00793          	li	a5,45
    126c:	00f109a3          	sb	a5,19(sp)
        buf[i--] = digits[x % base];
    1270:	45ad                	li	a1,11
    1272:	b795                	j	11d6 <printint.constprop.0+0x126>
    1274:	45ad                	li	a1,11
    if (sign)
    1276:	f60550e3          	bgez	a0,11d6 <printint.constprop.0+0x126>
        buf[i--] = '-';
    127a:	02d00793          	li	a5,45
    127e:	00f10923          	sb	a5,18(sp)
        buf[i--] = digits[x % base];
    1282:	45a9                	li	a1,10
    1284:	bf89                	j	11d6 <printint.constprop.0+0x126>
        buf[i--] = '-';
    1286:	02d00793          	li	a5,45
    128a:	00f10b23          	sb	a5,22(sp)
        buf[i--] = digits[x % base];
    128e:	45b9                	li	a1,14
    1290:	b799                	j	11d6 <printint.constprop.0+0x126>
    1292:	45a5                	li	a1,9
    if (sign)
    1294:	f40551e3          	bgez	a0,11d6 <printint.constprop.0+0x126>
        buf[i--] = '-';
    1298:	02d00793          	li	a5,45
    129c:	00f10823          	sb	a5,16(sp)
        buf[i--] = digits[x % base];
    12a0:	45a1                	li	a1,8
    12a2:	bf15                	j	11d6 <printint.constprop.0+0x126>
    i = 15;
    12a4:	45bd                	li	a1,15
    12a6:	bf05                	j	11d6 <printint.constprop.0+0x126>
        buf[i--] = digits[x % base];
    12a8:	45a1                	li	a1,8
    if (sign)
    12aa:	f20556e3          	bgez	a0,11d6 <printint.constprop.0+0x126>
        buf[i--] = '-';
    12ae:	02d00793          	li	a5,45
    12b2:	00f107a3          	sb	a5,15(sp)
        buf[i--] = digits[x % base];
    12b6:	459d                	li	a1,7
    12b8:	bf39                	j	11d6 <printint.constprop.0+0x126>
    12ba:	459d                	li	a1,7
    if (sign)
    12bc:	f0055de3          	bgez	a0,11d6 <printint.constprop.0+0x126>
        buf[i--] = '-';
    12c0:	02d00793          	li	a5,45
    12c4:	00f10723          	sb	a5,14(sp)
        buf[i--] = digits[x % base];
    12c8:	4599                	li	a1,6
    12ca:	b731                	j	11d6 <printint.constprop.0+0x126>

00000000000012cc <getchar>:
{
    12cc:	1101                	addi	sp,sp,-32
    read(stdin, &byte, 1);
    12ce:	00f10593          	addi	a1,sp,15
    12d2:	4605                	li	a2,1
    12d4:	4501                	li	a0,0
{
    12d6:	ec06                	sd	ra,24(sp)
    char byte = 0;
    12d8:	000107a3          	sb	zero,15(sp)
    read(stdin, &byte, 1);
    12dc:	18b000ef          	jal	ra,1c66 <read>
}
    12e0:	60e2                	ld	ra,24(sp)
    12e2:	00f14503          	lbu	a0,15(sp)
    12e6:	6105                	addi	sp,sp,32
    12e8:	8082                	ret

00000000000012ea <putchar>:
{
    12ea:	1101                	addi	sp,sp,-32
    12ec:	87aa                	mv	a5,a0
    return write(stdout, &byte, 1);
    12ee:	00f10593          	addi	a1,sp,15
    12f2:	4605                	li	a2,1
    12f4:	4505                	li	a0,1
{
    12f6:	ec06                	sd	ra,24(sp)
    char byte = c;
    12f8:	00f107a3          	sb	a5,15(sp)
    return write(stdout, &byte, 1);
    12fc:	175000ef          	jal	ra,1c70 <write>
}
    1300:	60e2                	ld	ra,24(sp)
    1302:	2501                	sext.w	a0,a0
    1304:	6105                	addi	sp,sp,32
    1306:	8082                	ret

0000000000001308 <puts>:
{
    1308:	1141                	addi	sp,sp,-16
    130a:	e406                	sd	ra,8(sp)
    130c:	e022                	sd	s0,0(sp)
    130e:	842a                	mv	s0,a0
    r = -(write(stdout, s, strlen(s)) < 0);
    1310:	57c000ef          	jal	ra,188c <strlen>
    1314:	862a                	mv	a2,a0
    1316:	85a2                	mv	a1,s0
    1318:	4505                	li	a0,1
    131a:	157000ef          	jal	ra,1c70 <write>
}
    131e:	60a2                	ld	ra,8(sp)
    1320:	6402                	ld	s0,0(sp)
    r = -(write(stdout, s, strlen(s)) < 0);
    1322:	957d                	srai	a0,a0,0x3f
    return r;
    1324:	2501                	sext.w	a0,a0
}
    1326:	0141                	addi	sp,sp,16
    1328:	8082                	ret

000000000000132a <printf>:
    out(stdout, buf, i);
}

// Print to the console. only understands %d, %x, %p, %s.
void printf(const char *fmt, ...)
{
    132a:	7171                	addi	sp,sp,-176
    132c:	fc56                	sd	s5,56(sp)
    132e:	ed3e                	sd	a5,152(sp)
    buf[i++] = '0';
    1330:	7ae1                	lui	s5,0xffff8
    va_list ap;
    int cnt = 0, l = 0;
    char *a, *z, *s = (char *)fmt, str;
    int f = stdout;

    va_start(ap, fmt);
    1332:	18bc                	addi	a5,sp,120
{
    1334:	e8ca                	sd	s2,80(sp)
    1336:	e4ce                	sd	s3,72(sp)
    1338:	e0d2                	sd	s4,64(sp)
    133a:	f85a                	sd	s6,48(sp)
    133c:	f486                	sd	ra,104(sp)
    133e:	f0a2                	sd	s0,96(sp)
    1340:	eca6                	sd	s1,88(sp)
    1342:	fcae                	sd	a1,120(sp)
    1344:	e132                	sd	a2,128(sp)
    1346:	e536                	sd	a3,136(sp)
    1348:	e93a                	sd	a4,144(sp)
    134a:	f142                	sd	a6,160(sp)
    134c:	f546                	sd	a7,168(sp)
    va_start(ap, fmt);
    134e:	e03e                	sd	a5,0(sp)
    for (;;)
    {
        if (!*s)
            break;
        for (a = s; *s && *s != '%'; s++)
    1350:	02500913          	li	s2,37
        out(f, a, l);
        if (l)
            continue;
        if (s[1] == 0)
            break;
        switch (s[1])
    1354:	07300a13          	li	s4,115
        case 'p':
            printptr(va_arg(ap, uint64));
            break;
        case 's':
            if ((a = va_arg(ap, char *)) == 0)
                a = "(null)";
    1358:	00001b17          	auipc	s6,0x1
    135c:	bf8b0b13          	addi	s6,s6,-1032 # 1f50 <__clone+0xac>
    buf[i++] = '0';
    1360:	830aca93          	xori	s5,s5,-2000
        buf[i++] = digits[x >> (sizeof(uint64) * 8 - 4)];
    1364:	00001997          	auipc	s3,0x1
    1368:	c1c98993          	addi	s3,s3,-996 # 1f80 <digits>
        if (!*s)
    136c:	00054783          	lbu	a5,0(a0)
    1370:	16078a63          	beqz	a5,14e4 <printf+0x1ba>
    1374:	862a                	mv	a2,a0
        for (a = s; *s && *s != '%'; s++)
    1376:	19278163          	beq	a5,s2,14f8 <printf+0x1ce>
    137a:	00164783          	lbu	a5,1(a2)
    137e:	0605                	addi	a2,a2,1
    1380:	fbfd                	bnez	a5,1376 <printf+0x4c>
    1382:	84b2                	mv	s1,a2
        l = z - a;
    1384:	40a6043b          	subw	s0,a2,a0
    write(f, s, l);
    1388:	85aa                	mv	a1,a0
    138a:	8622                	mv	a2,s0
    138c:	4505                	li	a0,1
    138e:	0e3000ef          	jal	ra,1c70 <write>
        if (l)
    1392:	18041c63          	bnez	s0,152a <printf+0x200>
        if (s[1] == 0)
    1396:	0014c783          	lbu	a5,1(s1)
    139a:	14078563          	beqz	a5,14e4 <printf+0x1ba>
        switch (s[1])
    139e:	1d478063          	beq	a5,s4,155e <printf+0x234>
    13a2:	18fa6663          	bltu	s4,a5,152e <printf+0x204>
    13a6:	06400713          	li	a4,100
    13aa:	1ae78063          	beq	a5,a4,154a <printf+0x220>
    13ae:	07000713          	li	a4,112
    13b2:	1ce79963          	bne	a5,a4,1584 <printf+0x25a>
            printptr(va_arg(ap, uint64));
    13b6:	6702                	ld	a4,0(sp)
    buf[i++] = '0';
    13b8:	01511423          	sh	s5,8(sp)
    write(f, s, l);
    13bc:	4649                	li	a2,18
            printptr(va_arg(ap, uint64));
    13be:	631c                	ld	a5,0(a4)
    13c0:	0721                	addi	a4,a4,8
    13c2:	e03a                	sd	a4,0(sp)
    for (j = 0; j < (sizeof(uint64) * 2); j++, x <<= 4)
    13c4:	00479293          	slli	t0,a5,0x4
    13c8:	00879f93          	slli	t6,a5,0x8
    13cc:	00c79f13          	slli	t5,a5,0xc
    13d0:	01079e93          	slli	t4,a5,0x10
    13d4:	01479e13          	slli	t3,a5,0x14
    13d8:	01879313          	slli	t1,a5,0x18
    13dc:	01c79893          	slli	a7,a5,0x1c
    13e0:	02479813          	slli	a6,a5,0x24
    13e4:	02879513          	slli	a0,a5,0x28
    13e8:	02c79593          	slli	a1,a5,0x2c
    13ec:	03079693          	slli	a3,a5,0x30
    13f0:	03479713          	slli	a4,a5,0x34
        buf[i++] = digits[x >> (sizeof(uint64) * 8 - 4)];
    13f4:	03c7d413          	srli	s0,a5,0x3c
    13f8:	01c7d39b          	srliw	t2,a5,0x1c
    13fc:	03c2d293          	srli	t0,t0,0x3c
    1400:	03cfdf93          	srli	t6,t6,0x3c
    1404:	03cf5f13          	srli	t5,t5,0x3c
    1408:	03cede93          	srli	t4,t4,0x3c
    140c:	03ce5e13          	srli	t3,t3,0x3c
    1410:	03c35313          	srli	t1,t1,0x3c
    1414:	03c8d893          	srli	a7,a7,0x3c
    1418:	03c85813          	srli	a6,a6,0x3c
    141c:	9171                	srli	a0,a0,0x3c
    141e:	91f1                	srli	a1,a1,0x3c
    1420:	92f1                	srli	a3,a3,0x3c
    1422:	9371                	srli	a4,a4,0x3c
    1424:	96ce                	add	a3,a3,s3
    1426:	974e                	add	a4,a4,s3
    1428:	944e                	add	s0,s0,s3
    142a:	92ce                	add	t0,t0,s3
    142c:	9fce                	add	t6,t6,s3
    142e:	9f4e                	add	t5,t5,s3
    1430:	9ece                	add	t4,t4,s3
    1432:	9e4e                	add	t3,t3,s3
    1434:	934e                	add	t1,t1,s3
    1436:	98ce                	add	a7,a7,s3
    1438:	93ce                	add	t2,t2,s3
    143a:	984e                	add	a6,a6,s3
    143c:	954e                	add	a0,a0,s3
    143e:	95ce                	add	a1,a1,s3
    1440:	0006c083          	lbu	ra,0(a3)
    1444:	0002c283          	lbu	t0,0(t0)
    1448:	00074683          	lbu	a3,0(a4)
    144c:	000fcf83          	lbu	t6,0(t6)
    1450:	000f4f03          	lbu	t5,0(t5)
    1454:	000ece83          	lbu	t4,0(t4)
    1458:	000e4e03          	lbu	t3,0(t3)
    145c:	00034303          	lbu	t1,0(t1)
    1460:	0008c883          	lbu	a7,0(a7)
    1464:	0003c383          	lbu	t2,0(t2)
    1468:	00084803          	lbu	a6,0(a6)
    146c:	00054503          	lbu	a0,0(a0)
    1470:	0005c583          	lbu	a1,0(a1)
    1474:	00044403          	lbu	s0,0(s0)
    for (j = 0; j < (sizeof(uint64) * 2); j++, x <<= 4)
    1478:	03879713          	slli	a4,a5,0x38
        buf[i++] = digits[x >> (sizeof(uint64) * 8 - 4)];
    147c:	9371                	srli	a4,a4,0x3c
    147e:	8bbd                	andi	a5,a5,15
    1480:	974e                	add	a4,a4,s3
    1482:	97ce                	add	a5,a5,s3
    1484:	005105a3          	sb	t0,11(sp)
    1488:	01f10623          	sb	t6,12(sp)
    148c:	01e106a3          	sb	t5,13(sp)
    1490:	01d10723          	sb	t4,14(sp)
    1494:	01c107a3          	sb	t3,15(sp)
    1498:	00610823          	sb	t1,16(sp)
    149c:	011108a3          	sb	a7,17(sp)
    14a0:	00710923          	sb	t2,18(sp)
    14a4:	010109a3          	sb	a6,19(sp)
    14a8:	00a10a23          	sb	a0,20(sp)
    14ac:	00b10aa3          	sb	a1,21(sp)
    14b0:	00110b23          	sb	ra,22(sp)
    14b4:	00d10ba3          	sb	a3,23(sp)
    14b8:	00810523          	sb	s0,10(sp)
    14bc:	00074703          	lbu	a4,0(a4)
    14c0:	0007c783          	lbu	a5,0(a5)
    write(f, s, l);
    14c4:	002c                	addi	a1,sp,8
    14c6:	4505                	li	a0,1
        buf[i++] = digits[x >> (sizeof(uint64) * 8 - 4)];
    14c8:	00e10c23          	sb	a4,24(sp)
    14cc:	00f10ca3          	sb	a5,25(sp)
    buf[i] = 0;
    14d0:	00010d23          	sb	zero,26(sp)
    write(f, s, l);
    14d4:	79c000ef          	jal	ra,1c70 <write>
            // Print unknown % sequence to draw attention.
            putchar('%');
            putchar(s[1]);
            break;
        }
        s += 2;
    14d8:	00248513          	addi	a0,s1,2
        if (!*s)
    14dc:	00054783          	lbu	a5,0(a0)
    14e0:	e8079ae3          	bnez	a5,1374 <printf+0x4a>
    }
    va_end(ap);
}
    14e4:	70a6                	ld	ra,104(sp)
    14e6:	7406                	ld	s0,96(sp)
    14e8:	64e6                	ld	s1,88(sp)
    14ea:	6946                	ld	s2,80(sp)
    14ec:	69a6                	ld	s3,72(sp)
    14ee:	6a06                	ld	s4,64(sp)
    14f0:	7ae2                	ld	s5,56(sp)
    14f2:	7b42                	ld	s6,48(sp)
    14f4:	614d                	addi	sp,sp,176
    14f6:	8082                	ret
        for (z = s; s[0] == '%' && s[1] == '%'; z++, s += 2)
    14f8:	00064783          	lbu	a5,0(a2)
    14fc:	84b2                	mv	s1,a2
    14fe:	01278963          	beq	a5,s2,1510 <printf+0x1e6>
    1502:	b549                	j	1384 <printf+0x5a>
    1504:	0024c783          	lbu	a5,2(s1)
    1508:	0605                	addi	a2,a2,1
    150a:	0489                	addi	s1,s1,2
    150c:	e7279ce3          	bne	a5,s2,1384 <printf+0x5a>
    1510:	0014c783          	lbu	a5,1(s1)
    1514:	ff2788e3          	beq	a5,s2,1504 <printf+0x1da>
        l = z - a;
    1518:	40a6043b          	subw	s0,a2,a0
    write(f, s, l);
    151c:	85aa                	mv	a1,a0
    151e:	8622                	mv	a2,s0
    1520:	4505                	li	a0,1
    1522:	74e000ef          	jal	ra,1c70 <write>
        if (l)
    1526:	e60408e3          	beqz	s0,1396 <printf+0x6c>
    152a:	8526                	mv	a0,s1
    152c:	b581                	j	136c <printf+0x42>
        switch (s[1])
    152e:	07800713          	li	a4,120
    1532:	04e79963          	bne	a5,a4,1584 <printf+0x25a>
            printint(va_arg(ap, int), 16, 1);
    1536:	6782                	ld	a5,0(sp)
    1538:	45c1                	li	a1,16
    153a:	4388                	lw	a0,0(a5)
    153c:	07a1                	addi	a5,a5,8
    153e:	e03e                	sd	a5,0(sp)
    1540:	b71ff0ef          	jal	ra,10b0 <printint.constprop.0>
        s += 2;
    1544:	00248513          	addi	a0,s1,2
    1548:	bf51                	j	14dc <printf+0x1b2>
            printint(va_arg(ap, int), 10, 1);
    154a:	6782                	ld	a5,0(sp)
    154c:	45a9                	li	a1,10
    154e:	4388                	lw	a0,0(a5)
    1550:	07a1                	addi	a5,a5,8
    1552:	e03e                	sd	a5,0(sp)
    1554:	b5dff0ef          	jal	ra,10b0 <printint.constprop.0>
        s += 2;
    1558:	00248513          	addi	a0,s1,2
    155c:	b741                	j	14dc <printf+0x1b2>
            if ((a = va_arg(ap, char *)) == 0)
    155e:	6782                	ld	a5,0(sp)
    1560:	6380                	ld	s0,0(a5)
    1562:	07a1                	addi	a5,a5,8
    1564:	e03e                	sd	a5,0(sp)
    1566:	c031                	beqz	s0,15aa <printf+0x280>
            l = strnlen(a, 200);
    1568:	0c800593          	li	a1,200
    156c:	8522                	mv	a0,s0
    156e:	40a000ef          	jal	ra,1978 <strnlen>
    write(f, s, l);
    1572:	0005061b          	sext.w	a2,a0
    1576:	85a2                	mv	a1,s0
    1578:	4505                	li	a0,1
    157a:	6f6000ef          	jal	ra,1c70 <write>
        s += 2;
    157e:	00248513          	addi	a0,s1,2
    1582:	bfa9                	j	14dc <printf+0x1b2>
    return write(stdout, &byte, 1);
    1584:	4605                	li	a2,1
    1586:	002c                	addi	a1,sp,8
    1588:	4505                	li	a0,1
    char byte = c;
    158a:	01210423          	sb	s2,8(sp)
    return write(stdout, &byte, 1);
    158e:	6e2000ef          	jal	ra,1c70 <write>
    char byte = c;
    1592:	0014c783          	lbu	a5,1(s1)
    return write(stdout, &byte, 1);
    1596:	4605                	li	a2,1
    1598:	002c                	addi	a1,sp,8
    159a:	4505                	li	a0,1
    char byte = c;
    159c:	00f10423          	sb	a5,8(sp)
    return write(stdout, &byte, 1);
    15a0:	6d0000ef          	jal	ra,1c70 <write>
        s += 2;
    15a4:	00248513          	addi	a0,s1,2
    15a8:	bf15                	j	14dc <printf+0x1b2>
                a = "(null)";
    15aa:	845a                	mv	s0,s6
    15ac:	bf75                	j	1568 <printf+0x23e>

00000000000015ae <panic>:
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

void panic(char *m)
{
    15ae:	1141                	addi	sp,sp,-16
    15b0:	e406                	sd	ra,8(sp)
    puts(m);
    15b2:	d57ff0ef          	jal	ra,1308 <puts>
    exit(-100);
}
    15b6:	60a2                	ld	ra,8(sp)
    exit(-100);
    15b8:	f9c00513          	li	a0,-100
}
    15bc:	0141                	addi	sp,sp,16
    exit(-100);
    15be:	a709                	j	1cc0 <exit>

00000000000015c0 <isspace>:
#define HIGHS (ONES * (UCHAR_MAX / 2 + 1))
#define HASZERO(x) (((x)-ONES) & ~(x)&HIGHS)

int isspace(int c)
{
    return c == ' ' || (unsigned)c - '\t' < 5;
    15c0:	02000793          	li	a5,32
    15c4:	00f50663          	beq	a0,a5,15d0 <isspace+0x10>
    15c8:	355d                	addiw	a0,a0,-9
    15ca:	00553513          	sltiu	a0,a0,5
    15ce:	8082                	ret
    15d0:	4505                	li	a0,1
}
    15d2:	8082                	ret

00000000000015d4 <isdigit>:

int isdigit(int c)
{
    return (unsigned)c - '0' < 10;
    15d4:	fd05051b          	addiw	a0,a0,-48
}
    15d8:	00a53513          	sltiu	a0,a0,10
    15dc:	8082                	ret

00000000000015de <atoi>:
    return c == ' ' || (unsigned)c - '\t' < 5;
    15de:	02000613          	li	a2,32
    15e2:	4591                	li	a1,4

int atoi(const char *s)
{
    int n = 0, neg = 0;
    while (isspace(*s))
    15e4:	00054703          	lbu	a4,0(a0)
    return c == ' ' || (unsigned)c - '\t' < 5;
    15e8:	ff77069b          	addiw	a3,a4,-9
    15ec:	04c70d63          	beq	a4,a2,1646 <atoi+0x68>
    15f0:	0007079b          	sext.w	a5,a4
    15f4:	04d5f963          	bgeu	a1,a3,1646 <atoi+0x68>
        s++;
    switch (*s)
    15f8:	02b00693          	li	a3,43
    15fc:	04d70a63          	beq	a4,a3,1650 <atoi+0x72>
    1600:	02d00693          	li	a3,45
    1604:	06d70463          	beq	a4,a3,166c <atoi+0x8e>
        neg = 1;
    case '+':
        s++;
    }
    /* Compute n as a negative number to avoid overflow on INT_MIN */
    while (isdigit(*s))
    1608:	fd07859b          	addiw	a1,a5,-48
    160c:	4625                	li	a2,9
    160e:	873e                	mv	a4,a5
    1610:	86aa                	mv	a3,a0
    int n = 0, neg = 0;
    1612:	4e01                	li	t3,0
    while (isdigit(*s))
    1614:	04b66a63          	bltu	a2,a1,1668 <atoi+0x8a>
    int n = 0, neg = 0;
    1618:	4501                	li	a0,0
    while (isdigit(*s))
    161a:	4825                	li	a6,9
    161c:	0016c603          	lbu	a2,1(a3)
        n = 10 * n - (*s++ - '0');
    1620:	0025179b          	slliw	a5,a0,0x2
    1624:	9d3d                	addw	a0,a0,a5
    1626:	fd07031b          	addiw	t1,a4,-48
    162a:	0015189b          	slliw	a7,a0,0x1
    while (isdigit(*s))
    162e:	fd06059b          	addiw	a1,a2,-48
        n = 10 * n - (*s++ - '0');
    1632:	0685                	addi	a3,a3,1
    1634:	4068853b          	subw	a0,a7,t1
    while (isdigit(*s))
    1638:	0006071b          	sext.w	a4,a2
    163c:	feb870e3          	bgeu	a6,a1,161c <atoi+0x3e>
    return neg ? n : -n;
    1640:	000e0563          	beqz	t3,164a <atoi+0x6c>
}
    1644:	8082                	ret
        s++;
    1646:	0505                	addi	a0,a0,1
    1648:	bf71                	j	15e4 <atoi+0x6>
    return neg ? n : -n;
    164a:	4113053b          	subw	a0,t1,a7
    164e:	8082                	ret
    while (isdigit(*s))
    1650:	00154783          	lbu	a5,1(a0)
    1654:	4625                	li	a2,9
        s++;
    1656:	00150693          	addi	a3,a0,1
    while (isdigit(*s))
    165a:	fd07859b          	addiw	a1,a5,-48
    165e:	0007871b          	sext.w	a4,a5
    int n = 0, neg = 0;
    1662:	4e01                	li	t3,0
    while (isdigit(*s))
    1664:	fab67ae3          	bgeu	a2,a1,1618 <atoi+0x3a>
    1668:	4501                	li	a0,0
}
    166a:	8082                	ret
    while (isdigit(*s))
    166c:	00154783          	lbu	a5,1(a0)
    1670:	4625                	li	a2,9
        s++;
    1672:	00150693          	addi	a3,a0,1
    while (isdigit(*s))
    1676:	fd07859b          	addiw	a1,a5,-48
    167a:	0007871b          	sext.w	a4,a5
    167e:	feb665e3          	bltu	a2,a1,1668 <atoi+0x8a>
        neg = 1;
    1682:	4e05                	li	t3,1
    1684:	bf51                	j	1618 <atoi+0x3a>

0000000000001686 <memset>:

void *memset(void *dest, int c, size_t n)
{
    char *p = dest;
    for (int i = 0; i < n; ++i, *(p++) = c)
    1686:	16060d63          	beqz	a2,1800 <memset+0x17a>
    168a:	40a007b3          	neg	a5,a0
    168e:	8b9d                	andi	a5,a5,7
    1690:	00778713          	addi	a4,a5,7
    1694:	482d                	li	a6,11
    1696:	0ff5f593          	andi	a1,a1,255
    169a:	fff60693          	addi	a3,a2,-1
    169e:	17076263          	bltu	a4,a6,1802 <memset+0x17c>
    16a2:	16e6ea63          	bltu	a3,a4,1816 <memset+0x190>
    16a6:	16078563          	beqz	a5,1810 <memset+0x18a>
    16aa:	00b50023          	sb	a1,0(a0)
    16ae:	4705                	li	a4,1
    16b0:	00150e93          	addi	t4,a0,1
    16b4:	14e78c63          	beq	a5,a4,180c <memset+0x186>
    16b8:	00b500a3          	sb	a1,1(a0)
    16bc:	4709                	li	a4,2
    16be:	00250e93          	addi	t4,a0,2
    16c2:	14e78d63          	beq	a5,a4,181c <memset+0x196>
    16c6:	00b50123          	sb	a1,2(a0)
    16ca:	470d                	li	a4,3
    16cc:	00350e93          	addi	t4,a0,3
    16d0:	12e78b63          	beq	a5,a4,1806 <memset+0x180>
    16d4:	00b501a3          	sb	a1,3(a0)
    16d8:	4711                	li	a4,4
    16da:	00450e93          	addi	t4,a0,4
    16de:	14e78163          	beq	a5,a4,1820 <memset+0x19a>
    16e2:	00b50223          	sb	a1,4(a0)
    16e6:	4715                	li	a4,5
    16e8:	00550e93          	addi	t4,a0,5
    16ec:	12e78c63          	beq	a5,a4,1824 <memset+0x19e>
    16f0:	00b502a3          	sb	a1,5(a0)
    16f4:	471d                	li	a4,7
    16f6:	00650e93          	addi	t4,a0,6
    16fa:	12e79763          	bne	a5,a4,1828 <memset+0x1a2>
    16fe:	00750e93          	addi	t4,a0,7
    1702:	00b50323          	sb	a1,6(a0)
    1706:	4f1d                	li	t5,7
    1708:	00859713          	slli	a4,a1,0x8
    170c:	8f4d                	or	a4,a4,a1
    170e:	01059e13          	slli	t3,a1,0x10
    1712:	01c76e33          	or	t3,a4,t3
    1716:	01859313          	slli	t1,a1,0x18
    171a:	006e6333          	or	t1,t3,t1
    171e:	02059893          	slli	a7,a1,0x20
    1722:	011368b3          	or	a7,t1,a7
    1726:	02859813          	slli	a6,a1,0x28
    172a:	40f60333          	sub	t1,a2,a5
    172e:	0108e833          	or	a6,a7,a6
    1732:	03059693          	slli	a3,a1,0x30
    1736:	00d866b3          	or	a3,a6,a3
    173a:	03859713          	slli	a4,a1,0x38
    173e:	97aa                	add	a5,a5,a0
    1740:	ff837813          	andi	a6,t1,-8
    1744:	8f55                	or	a4,a4,a3
    1746:	00f806b3          	add	a3,a6,a5
    174a:	e398                	sd	a4,0(a5)
    174c:	07a1                	addi	a5,a5,8
    174e:	fed79ee3          	bne	a5,a3,174a <memset+0xc4>
    1752:	ff837693          	andi	a3,t1,-8
    1756:	00de87b3          	add	a5,t4,a3
    175a:	01e6873b          	addw	a4,a3,t5
    175e:	0ad30663          	beq	t1,a3,180a <memset+0x184>
    1762:	00b78023          	sb	a1,0(a5)
    1766:	0017069b          	addiw	a3,a4,1
    176a:	08c6fb63          	bgeu	a3,a2,1800 <memset+0x17a>
    176e:	00b780a3          	sb	a1,1(a5)
    1772:	0027069b          	addiw	a3,a4,2
    1776:	08c6f563          	bgeu	a3,a2,1800 <memset+0x17a>
    177a:	00b78123          	sb	a1,2(a5)
    177e:	0037069b          	addiw	a3,a4,3
    1782:	06c6ff63          	bgeu	a3,a2,1800 <memset+0x17a>
    1786:	00b781a3          	sb	a1,3(a5)
    178a:	0047069b          	addiw	a3,a4,4
    178e:	06c6f963          	bgeu	a3,a2,1800 <memset+0x17a>
    1792:	00b78223          	sb	a1,4(a5)
    1796:	0057069b          	addiw	a3,a4,5
    179a:	06c6f363          	bgeu	a3,a2,1800 <memset+0x17a>
    179e:	00b782a3          	sb	a1,5(a5)
    17a2:	0067069b          	addiw	a3,a4,6
    17a6:	04c6fd63          	bgeu	a3,a2,1800 <memset+0x17a>
    17aa:	00b78323          	sb	a1,6(a5)
    17ae:	0077069b          	addiw	a3,a4,7
    17b2:	04c6f763          	bgeu	a3,a2,1800 <memset+0x17a>
    17b6:	00b783a3          	sb	a1,7(a5)
    17ba:	0087069b          	addiw	a3,a4,8
    17be:	04c6f163          	bgeu	a3,a2,1800 <memset+0x17a>
    17c2:	00b78423          	sb	a1,8(a5)
    17c6:	0097069b          	addiw	a3,a4,9
    17ca:	02c6fb63          	bgeu	a3,a2,1800 <memset+0x17a>
    17ce:	00b784a3          	sb	a1,9(a5)
    17d2:	00a7069b          	addiw	a3,a4,10
    17d6:	02c6f563          	bgeu	a3,a2,1800 <memset+0x17a>
    17da:	00b78523          	sb	a1,10(a5)
    17de:	00b7069b          	addiw	a3,a4,11
    17e2:	00c6ff63          	bgeu	a3,a2,1800 <memset+0x17a>
    17e6:	00b785a3          	sb	a1,11(a5)
    17ea:	00c7069b          	addiw	a3,a4,12
    17ee:	00c6f963          	bgeu	a3,a2,1800 <memset+0x17a>
    17f2:	00b78623          	sb	a1,12(a5)
    17f6:	2735                	addiw	a4,a4,13
    17f8:	00c77463          	bgeu	a4,a2,1800 <memset+0x17a>
    17fc:	00b786a3          	sb	a1,13(a5)
        ;
    return dest;
}
    1800:	8082                	ret
    1802:	472d                	li	a4,11
    1804:	bd79                	j	16a2 <memset+0x1c>
    for (int i = 0; i < n; ++i, *(p++) = c)
    1806:	4f0d                	li	t5,3
    1808:	b701                	j	1708 <memset+0x82>
    180a:	8082                	ret
    180c:	4f05                	li	t5,1
    180e:	bded                	j	1708 <memset+0x82>
    1810:	8eaa                	mv	t4,a0
    1812:	4f01                	li	t5,0
    1814:	bdd5                	j	1708 <memset+0x82>
    1816:	87aa                	mv	a5,a0
    1818:	4701                	li	a4,0
    181a:	b7a1                	j	1762 <memset+0xdc>
    181c:	4f09                	li	t5,2
    181e:	b5ed                	j	1708 <memset+0x82>
    1820:	4f11                	li	t5,4
    1822:	b5dd                	j	1708 <memset+0x82>
    1824:	4f15                	li	t5,5
    1826:	b5cd                	j	1708 <memset+0x82>
    1828:	4f19                	li	t5,6
    182a:	bdf9                	j	1708 <memset+0x82>

000000000000182c <strcmp>:

int strcmp(const char *l, const char *r)
{
    for (; *l == *r && *l; l++, r++)
    182c:	00054783          	lbu	a5,0(a0)
    1830:	0005c703          	lbu	a4,0(a1)
    1834:	00e79863          	bne	a5,a4,1844 <strcmp+0x18>
    1838:	0505                	addi	a0,a0,1
    183a:	0585                	addi	a1,a1,1
    183c:	fbe5                	bnez	a5,182c <strcmp>
    183e:	4501                	li	a0,0
        ;
    return *(unsigned char *)l - *(unsigned char *)r;
}
    1840:	9d19                	subw	a0,a0,a4
    1842:	8082                	ret
    return *(unsigned char *)l - *(unsigned char *)r;
    1844:	0007851b          	sext.w	a0,a5
    1848:	bfe5                	j	1840 <strcmp+0x14>

000000000000184a <strncmp>:

int strncmp(const char *_l, const char *_r, size_t n)
{
    const unsigned char *l = (void *)_l, *r = (void *)_r;
    if (!n--)
    184a:	ce05                	beqz	a2,1882 <strncmp+0x38>
        return 0;
    for (; *l && *r && n && *l == *r; l++, r++, n--)
    184c:	00054703          	lbu	a4,0(a0)
    1850:	0005c783          	lbu	a5,0(a1)
    1854:	cb0d                	beqz	a4,1886 <strncmp+0x3c>
    if (!n--)
    1856:	167d                	addi	a2,a2,-1
    1858:	00c506b3          	add	a3,a0,a2
    185c:	a819                	j	1872 <strncmp+0x28>
    for (; *l && *r && n && *l == *r; l++, r++, n--)
    185e:	00a68e63          	beq	a3,a0,187a <strncmp+0x30>
    1862:	0505                	addi	a0,a0,1
    1864:	00e79b63          	bne	a5,a4,187a <strncmp+0x30>
    1868:	00054703          	lbu	a4,0(a0)
        ;
    return *l - *r;
    186c:	0005c783          	lbu	a5,0(a1)
    for (; *l && *r && n && *l == *r; l++, r++, n--)
    1870:	cb19                	beqz	a4,1886 <strncmp+0x3c>
    1872:	0005c783          	lbu	a5,0(a1)
    1876:	0585                	addi	a1,a1,1
    1878:	f3fd                	bnez	a5,185e <strncmp+0x14>
    return *l - *r;
    187a:	0007051b          	sext.w	a0,a4
    187e:	9d1d                	subw	a0,a0,a5
    1880:	8082                	ret
        return 0;
    1882:	4501                	li	a0,0
}
    1884:	8082                	ret
    1886:	4501                	li	a0,0
    return *l - *r;
    1888:	9d1d                	subw	a0,a0,a5
    188a:	8082                	ret

000000000000188c <strlen>:
size_t strlen(const char *s)
{
    const char *a = s;
    typedef size_t __attribute__((__may_alias__)) word;
    const word *w;
    for (; (uintptr_t)s % SS; s++)
    188c:	00757793          	andi	a5,a0,7
    1890:	cf89                	beqz	a5,18aa <strlen+0x1e>
    1892:	87aa                	mv	a5,a0
    1894:	a029                	j	189e <strlen+0x12>
    1896:	0785                	addi	a5,a5,1
    1898:	0077f713          	andi	a4,a5,7
    189c:	cb01                	beqz	a4,18ac <strlen+0x20>
        if (!*s)
    189e:	0007c703          	lbu	a4,0(a5)
    18a2:	fb75                	bnez	a4,1896 <strlen+0xa>
    for (w = (const void *)s; !HASZERO(*w); w++)
        ;
    s = (const void *)w;
    for (; *s; s++)
        ;
    return s - a;
    18a4:	40a78533          	sub	a0,a5,a0
}
    18a8:	8082                	ret
    for (; (uintptr_t)s % SS; s++)
    18aa:	87aa                	mv	a5,a0
    for (w = (const void *)s; !HASZERO(*w); w++)
    18ac:	6394                	ld	a3,0(a5)
    18ae:	00000597          	auipc	a1,0x0
    18b2:	6aa5b583          	ld	a1,1706(a1) # 1f58 <__clone+0xb4>
    18b6:	00000617          	auipc	a2,0x0
    18ba:	6aa63603          	ld	a2,1706(a2) # 1f60 <__clone+0xbc>
    18be:	a019                	j	18c4 <strlen+0x38>
    18c0:	6794                	ld	a3,8(a5)
    18c2:	07a1                	addi	a5,a5,8
    18c4:	00b68733          	add	a4,a3,a1
    18c8:	fff6c693          	not	a3,a3
    18cc:	8f75                	and	a4,a4,a3
    18ce:	8f71                	and	a4,a4,a2
    18d0:	db65                	beqz	a4,18c0 <strlen+0x34>
    for (; *s; s++)
    18d2:	0007c703          	lbu	a4,0(a5)
    18d6:	d779                	beqz	a4,18a4 <strlen+0x18>
    18d8:	0017c703          	lbu	a4,1(a5)
    18dc:	0785                	addi	a5,a5,1
    18de:	d379                	beqz	a4,18a4 <strlen+0x18>
    18e0:	0017c703          	lbu	a4,1(a5)
    18e4:	0785                	addi	a5,a5,1
    18e6:	fb6d                	bnez	a4,18d8 <strlen+0x4c>
    18e8:	bf75                	j	18a4 <strlen+0x18>

00000000000018ea <memchr>:

void *memchr(const void *src, int c, size_t n)
{
    const unsigned char *s = src;
    c = (unsigned char)c;
    for (; ((uintptr_t)s & ALIGN) && n && *s != c; s++, n--)
    18ea:	00757713          	andi	a4,a0,7
{
    18ee:	87aa                	mv	a5,a0
    c = (unsigned char)c;
    18f0:	0ff5f593          	andi	a1,a1,255
    for (; ((uintptr_t)s & ALIGN) && n && *s != c; s++, n--)
    18f4:	cb19                	beqz	a4,190a <memchr+0x20>
    18f6:	ce25                	beqz	a2,196e <memchr+0x84>
    18f8:	0007c703          	lbu	a4,0(a5)
    18fc:	04b70e63          	beq	a4,a1,1958 <memchr+0x6e>
    1900:	0785                	addi	a5,a5,1
    1902:	0077f713          	andi	a4,a5,7
    1906:	167d                	addi	a2,a2,-1
    1908:	f77d                	bnez	a4,18f6 <memchr+0xc>
            ;
        s = (const void *)w;
    }
    for (; n && *s != c; s++, n--)
        ;
    return n ? (void *)s : 0;
    190a:	4501                	li	a0,0
    if (n && *s != c)
    190c:	c235                	beqz	a2,1970 <memchr+0x86>
    190e:	0007c703          	lbu	a4,0(a5)
    1912:	04b70363          	beq	a4,a1,1958 <memchr+0x6e>
        size_t k = ONES * c;
    1916:	00000517          	auipc	a0,0x0
    191a:	65253503          	ld	a0,1618(a0) # 1f68 <__clone+0xc4>
        for (w = (const void *)s; n >= SS && !HASZERO(*w ^ k); w++, n -= SS)
    191e:	471d                	li	a4,7
        size_t k = ONES * c;
    1920:	02a58533          	mul	a0,a1,a0
        for (w = (const void *)s; n >= SS && !HASZERO(*w ^ k); w++, n -= SS)
    1924:	02c77a63          	bgeu	a4,a2,1958 <memchr+0x6e>
    1928:	00000897          	auipc	a7,0x0
    192c:	6308b883          	ld	a7,1584(a7) # 1f58 <__clone+0xb4>
    1930:	00000817          	auipc	a6,0x0
    1934:	63083803          	ld	a6,1584(a6) # 1f60 <__clone+0xbc>
    1938:	431d                	li	t1,7
    193a:	a029                	j	1944 <memchr+0x5a>
    193c:	1661                	addi	a2,a2,-8
    193e:	07a1                	addi	a5,a5,8
    1940:	02c37963          	bgeu	t1,a2,1972 <memchr+0x88>
    1944:	6398                	ld	a4,0(a5)
    1946:	8f29                	xor	a4,a4,a0
    1948:	011706b3          	add	a3,a4,a7
    194c:	fff74713          	not	a4,a4
    1950:	8f75                	and	a4,a4,a3
    1952:	01077733          	and	a4,a4,a6
    1956:	d37d                	beqz	a4,193c <memchr+0x52>
    1958:	853e                	mv	a0,a5
    195a:	97b2                	add	a5,a5,a2
    195c:	a021                	j	1964 <memchr+0x7a>
    for (; n && *s != c; s++, n--)
    195e:	0505                	addi	a0,a0,1
    1960:	00f50763          	beq	a0,a5,196e <memchr+0x84>
    1964:	00054703          	lbu	a4,0(a0)
    1968:	feb71be3          	bne	a4,a1,195e <memchr+0x74>
    196c:	8082                	ret
    return n ? (void *)s : 0;
    196e:	4501                	li	a0,0
}
    1970:	8082                	ret
    return n ? (void *)s : 0;
    1972:	4501                	li	a0,0
    for (; n && *s != c; s++, n--)
    1974:	f275                	bnez	a2,1958 <memchr+0x6e>
}
    1976:	8082                	ret

0000000000001978 <strnlen>:

size_t strnlen(const char *s, size_t n)
{
    1978:	1101                	addi	sp,sp,-32
    197a:	e822                	sd	s0,16(sp)
    const char *p = memchr(s, 0, n);
    197c:	862e                	mv	a2,a1
{
    197e:	842e                	mv	s0,a1
    const char *p = memchr(s, 0, n);
    1980:	4581                	li	a1,0
{
    1982:	e426                	sd	s1,8(sp)
    1984:	ec06                	sd	ra,24(sp)
    1986:	84aa                	mv	s1,a0
    const char *p = memchr(s, 0, n);
    1988:	f63ff0ef          	jal	ra,18ea <memchr>
    return p ? p - s : n;
    198c:	c519                	beqz	a0,199a <strnlen+0x22>
}
    198e:	60e2                	ld	ra,24(sp)
    1990:	6442                	ld	s0,16(sp)
    return p ? p - s : n;
    1992:	8d05                	sub	a0,a0,s1
}
    1994:	64a2                	ld	s1,8(sp)
    1996:	6105                	addi	sp,sp,32
    1998:	8082                	ret
    199a:	60e2                	ld	ra,24(sp)
    return p ? p - s : n;
    199c:	8522                	mv	a0,s0
}
    199e:	6442                	ld	s0,16(sp)
    19a0:	64a2                	ld	s1,8(sp)
    19a2:	6105                	addi	sp,sp,32
    19a4:	8082                	ret

00000000000019a6 <strcpy>:
char *strcpy(char *restrict d, const char *s)
{
    typedef size_t __attribute__((__may_alias__)) word;
    word *wd;
    const word *ws;
    if ((uintptr_t)s % SS == (uintptr_t)d % SS)
    19a6:	00b547b3          	xor	a5,a0,a1
    19aa:	8b9d                	andi	a5,a5,7
    19ac:	eb95                	bnez	a5,19e0 <strcpy+0x3a>
    {
        for (; (uintptr_t)s % SS; s++, d++)
    19ae:	0075f793          	andi	a5,a1,7
    19b2:	e7b1                	bnez	a5,19fe <strcpy+0x58>
            if (!(*d = *s))
                return d;
        wd = (void *)d;
        ws = (const void *)s;
        for (; !HASZERO(*ws); *wd++ = *ws++)
    19b4:	6198                	ld	a4,0(a1)
    19b6:	00000617          	auipc	a2,0x0
    19ba:	5a263603          	ld	a2,1442(a2) # 1f58 <__clone+0xb4>
    19be:	00000817          	auipc	a6,0x0
    19c2:	5a283803          	ld	a6,1442(a6) # 1f60 <__clone+0xbc>
    19c6:	a029                	j	19d0 <strcpy+0x2a>
    19c8:	e118                	sd	a4,0(a0)
    19ca:	6598                	ld	a4,8(a1)
    19cc:	05a1                	addi	a1,a1,8
    19ce:	0521                	addi	a0,a0,8
    19d0:	00c707b3          	add	a5,a4,a2
    19d4:	fff74693          	not	a3,a4
    19d8:	8ff5                	and	a5,a5,a3
    19da:	0107f7b3          	and	a5,a5,a6
    19de:	d7ed                	beqz	a5,19c8 <strcpy+0x22>
            ;
        d = (void *)wd;
        s = (const void *)ws;
    }
    for (; (*d = *s); s++, d++)
    19e0:	0005c783          	lbu	a5,0(a1)
    19e4:	00f50023          	sb	a5,0(a0)
    19e8:	c785                	beqz	a5,1a10 <strcpy+0x6a>
    19ea:	0015c783          	lbu	a5,1(a1)
    19ee:	0505                	addi	a0,a0,1
    19f0:	0585                	addi	a1,a1,1
    19f2:	00f50023          	sb	a5,0(a0)
    19f6:	fbf5                	bnez	a5,19ea <strcpy+0x44>
        ;
    return d;
}
    19f8:	8082                	ret
        for (; (uintptr_t)s % SS; s++, d++)
    19fa:	0505                	addi	a0,a0,1
    19fc:	df45                	beqz	a4,19b4 <strcpy+0xe>
            if (!(*d = *s))
    19fe:	0005c783          	lbu	a5,0(a1)
        for (; (uintptr_t)s % SS; s++, d++)
    1a02:	0585                	addi	a1,a1,1
    1a04:	0075f713          	andi	a4,a1,7
            if (!(*d = *s))
    1a08:	00f50023          	sb	a5,0(a0)
    1a0c:	f7fd                	bnez	a5,19fa <strcpy+0x54>
}
    1a0e:	8082                	ret
    1a10:	8082                	ret

0000000000001a12 <strncpy>:
char *strncpy(char *restrict d, const char *s, size_t n)
{
    typedef size_t __attribute__((__may_alias__)) word;
    word *wd;
    const word *ws;
    if (((uintptr_t)s & ALIGN) == ((uintptr_t)d & ALIGN))
    1a12:	00b547b3          	xor	a5,a0,a1
    1a16:	8b9d                	andi	a5,a5,7
    1a18:	1a079863          	bnez	a5,1bc8 <strncpy+0x1b6>
    {
        for (; ((uintptr_t)s & ALIGN) && n && (*d = *s); n--, s++, d++)
    1a1c:	0075f793          	andi	a5,a1,7
    1a20:	16078463          	beqz	a5,1b88 <strncpy+0x176>
    1a24:	ea01                	bnez	a2,1a34 <strncpy+0x22>
    1a26:	a421                	j	1c2e <strncpy+0x21c>
    1a28:	167d                	addi	a2,a2,-1
    1a2a:	0505                	addi	a0,a0,1
    1a2c:	14070e63          	beqz	a4,1b88 <strncpy+0x176>
    1a30:	1a060863          	beqz	a2,1be0 <strncpy+0x1ce>
    1a34:	0005c783          	lbu	a5,0(a1)
    1a38:	0585                	addi	a1,a1,1
    1a3a:	0075f713          	andi	a4,a1,7
    1a3e:	00f50023          	sb	a5,0(a0)
    1a42:	f3fd                	bnez	a5,1a28 <strncpy+0x16>
    1a44:	4805                	li	a6,1
    1a46:	1a061863          	bnez	a2,1bf6 <strncpy+0x1e4>
    1a4a:	40a007b3          	neg	a5,a0
    1a4e:	8b9d                	andi	a5,a5,7
    1a50:	4681                	li	a3,0
    1a52:	18061a63          	bnez	a2,1be6 <strncpy+0x1d4>
    1a56:	00778713          	addi	a4,a5,7
    1a5a:	45ad                	li	a1,11
    1a5c:	18b76363          	bltu	a4,a1,1be2 <strncpy+0x1d0>
    1a60:	1ae6eb63          	bltu	a3,a4,1c16 <strncpy+0x204>
    1a64:	1a078363          	beqz	a5,1c0a <strncpy+0x1f8>
    for (int i = 0; i < n; ++i, *(p++) = c)
    1a68:	00050023          	sb	zero,0(a0)
    1a6c:	4685                	li	a3,1
    1a6e:	00150713          	addi	a4,a0,1
    1a72:	18d78f63          	beq	a5,a3,1c10 <strncpy+0x1fe>
    1a76:	000500a3          	sb	zero,1(a0)
    1a7a:	4689                	li	a3,2
    1a7c:	00250713          	addi	a4,a0,2
    1a80:	18d78e63          	beq	a5,a3,1c1c <strncpy+0x20a>
    1a84:	00050123          	sb	zero,2(a0)
    1a88:	468d                	li	a3,3
    1a8a:	00350713          	addi	a4,a0,3
    1a8e:	16d78c63          	beq	a5,a3,1c06 <strncpy+0x1f4>
    1a92:	000501a3          	sb	zero,3(a0)
    1a96:	4691                	li	a3,4
    1a98:	00450713          	addi	a4,a0,4
    1a9c:	18d78263          	beq	a5,a3,1c20 <strncpy+0x20e>
    1aa0:	00050223          	sb	zero,4(a0)
    1aa4:	4695                	li	a3,5
    1aa6:	00550713          	addi	a4,a0,5
    1aaa:	16d78d63          	beq	a5,a3,1c24 <strncpy+0x212>
    1aae:	000502a3          	sb	zero,5(a0)
    1ab2:	469d                	li	a3,7
    1ab4:	00650713          	addi	a4,a0,6
    1ab8:	16d79863          	bne	a5,a3,1c28 <strncpy+0x216>
    1abc:	00750713          	addi	a4,a0,7
    1ac0:	00050323          	sb	zero,6(a0)
    1ac4:	40f80833          	sub	a6,a6,a5
    1ac8:	ff887593          	andi	a1,a6,-8
    1acc:	97aa                	add	a5,a5,a0
    1ace:	95be                	add	a1,a1,a5
    1ad0:	0007b023          	sd	zero,0(a5)
    1ad4:	07a1                	addi	a5,a5,8
    1ad6:	feb79de3          	bne	a5,a1,1ad0 <strncpy+0xbe>
    1ada:	ff887593          	andi	a1,a6,-8
    1ade:	9ead                	addw	a3,a3,a1
    1ae0:	00b707b3          	add	a5,a4,a1
    1ae4:	12b80863          	beq	a6,a1,1c14 <strncpy+0x202>
    1ae8:	00078023          	sb	zero,0(a5)
    1aec:	0016871b          	addiw	a4,a3,1
    1af0:	0ec77863          	bgeu	a4,a2,1be0 <strncpy+0x1ce>
    1af4:	000780a3          	sb	zero,1(a5)
    1af8:	0026871b          	addiw	a4,a3,2
    1afc:	0ec77263          	bgeu	a4,a2,1be0 <strncpy+0x1ce>
    1b00:	00078123          	sb	zero,2(a5)
    1b04:	0036871b          	addiw	a4,a3,3
    1b08:	0cc77c63          	bgeu	a4,a2,1be0 <strncpy+0x1ce>
    1b0c:	000781a3          	sb	zero,3(a5)
    1b10:	0046871b          	addiw	a4,a3,4
    1b14:	0cc77663          	bgeu	a4,a2,1be0 <strncpy+0x1ce>
    1b18:	00078223          	sb	zero,4(a5)
    1b1c:	0056871b          	addiw	a4,a3,5
    1b20:	0cc77063          	bgeu	a4,a2,1be0 <strncpy+0x1ce>
    1b24:	000782a3          	sb	zero,5(a5)
    1b28:	0066871b          	addiw	a4,a3,6
    1b2c:	0ac77a63          	bgeu	a4,a2,1be0 <strncpy+0x1ce>
    1b30:	00078323          	sb	zero,6(a5)
    1b34:	0076871b          	addiw	a4,a3,7
    1b38:	0ac77463          	bgeu	a4,a2,1be0 <strncpy+0x1ce>
    1b3c:	000783a3          	sb	zero,7(a5)
    1b40:	0086871b          	addiw	a4,a3,8
    1b44:	08c77e63          	bgeu	a4,a2,1be0 <strncpy+0x1ce>
    1b48:	00078423          	sb	zero,8(a5)
    1b4c:	0096871b          	addiw	a4,a3,9
    1b50:	08c77863          	bgeu	a4,a2,1be0 <strncpy+0x1ce>
    1b54:	000784a3          	sb	zero,9(a5)
    1b58:	00a6871b          	addiw	a4,a3,10
    1b5c:	08c77263          	bgeu	a4,a2,1be0 <strncpy+0x1ce>
    1b60:	00078523          	sb	zero,10(a5)
    1b64:	00b6871b          	addiw	a4,a3,11
    1b68:	06c77c63          	bgeu	a4,a2,1be0 <strncpy+0x1ce>
    1b6c:	000785a3          	sb	zero,11(a5)
    1b70:	00c6871b          	addiw	a4,a3,12
    1b74:	06c77663          	bgeu	a4,a2,1be0 <strncpy+0x1ce>
    1b78:	00078623          	sb	zero,12(a5)
    1b7c:	26b5                	addiw	a3,a3,13
    1b7e:	06c6f163          	bgeu	a3,a2,1be0 <strncpy+0x1ce>
    1b82:	000786a3          	sb	zero,13(a5)
    1b86:	8082                	ret
            ;
        if (!n || !*s)
    1b88:	c645                	beqz	a2,1c30 <strncpy+0x21e>
    1b8a:	0005c783          	lbu	a5,0(a1)
    1b8e:	ea078be3          	beqz	a5,1a44 <strncpy+0x32>
            goto tail;
        wd = (void *)d;
        ws = (const void *)s;
        for (; n >= sizeof(size_t) && !HASZERO(*ws); n -= sizeof(size_t), ws++, wd++)
    1b92:	479d                	li	a5,7
    1b94:	02c7ff63          	bgeu	a5,a2,1bd2 <strncpy+0x1c0>
    1b98:	00000897          	auipc	a7,0x0
    1b9c:	3c08b883          	ld	a7,960(a7) # 1f58 <__clone+0xb4>
    1ba0:	00000817          	auipc	a6,0x0
    1ba4:	3c083803          	ld	a6,960(a6) # 1f60 <__clone+0xbc>
    1ba8:	431d                	li	t1,7
    1baa:	6198                	ld	a4,0(a1)
    1bac:	011707b3          	add	a5,a4,a7
    1bb0:	fff74693          	not	a3,a4
    1bb4:	8ff5                	and	a5,a5,a3
    1bb6:	0107f7b3          	and	a5,a5,a6
    1bba:	ef81                	bnez	a5,1bd2 <strncpy+0x1c0>
            *wd = *ws;
    1bbc:	e118                	sd	a4,0(a0)
        for (; n >= sizeof(size_t) && !HASZERO(*ws); n -= sizeof(size_t), ws++, wd++)
    1bbe:	1661                	addi	a2,a2,-8
    1bc0:	05a1                	addi	a1,a1,8
    1bc2:	0521                	addi	a0,a0,8
    1bc4:	fec363e3          	bltu	t1,a2,1baa <strncpy+0x198>
        d = (void *)wd;
        s = (const void *)ws;
    }
    for (; n && (*d = *s); n--, s++, d++)
    1bc8:	e609                	bnez	a2,1bd2 <strncpy+0x1c0>
    1bca:	a08d                	j	1c2c <strncpy+0x21a>
    1bcc:	167d                	addi	a2,a2,-1
    1bce:	0505                	addi	a0,a0,1
    1bd0:	ca01                	beqz	a2,1be0 <strncpy+0x1ce>
    1bd2:	0005c783          	lbu	a5,0(a1)
    1bd6:	0585                	addi	a1,a1,1
    1bd8:	00f50023          	sb	a5,0(a0)
    1bdc:	fbe5                	bnez	a5,1bcc <strncpy+0x1ba>
        ;
tail:
    1bde:	b59d                	j	1a44 <strncpy+0x32>
    memset(d, 0, n);
    return d;
}
    1be0:	8082                	ret
    1be2:	472d                	li	a4,11
    1be4:	bdb5                	j	1a60 <strncpy+0x4e>
    1be6:	00778713          	addi	a4,a5,7
    1bea:	45ad                	li	a1,11
    1bec:	fff60693          	addi	a3,a2,-1
    1bf0:	e6b778e3          	bgeu	a4,a1,1a60 <strncpy+0x4e>
    1bf4:	b7fd                	j	1be2 <strncpy+0x1d0>
    1bf6:	40a007b3          	neg	a5,a0
    1bfa:	8832                	mv	a6,a2
    1bfc:	8b9d                	andi	a5,a5,7
    1bfe:	4681                	li	a3,0
    1c00:	e4060be3          	beqz	a2,1a56 <strncpy+0x44>
    1c04:	b7cd                	j	1be6 <strncpy+0x1d4>
    for (int i = 0; i < n; ++i, *(p++) = c)
    1c06:	468d                	li	a3,3
    1c08:	bd75                	j	1ac4 <strncpy+0xb2>
    1c0a:	872a                	mv	a4,a0
    1c0c:	4681                	li	a3,0
    1c0e:	bd5d                	j	1ac4 <strncpy+0xb2>
    1c10:	4685                	li	a3,1
    1c12:	bd4d                	j	1ac4 <strncpy+0xb2>
    1c14:	8082                	ret
    1c16:	87aa                	mv	a5,a0
    1c18:	4681                	li	a3,0
    1c1a:	b5f9                	j	1ae8 <strncpy+0xd6>
    1c1c:	4689                	li	a3,2
    1c1e:	b55d                	j	1ac4 <strncpy+0xb2>
    1c20:	4691                	li	a3,4
    1c22:	b54d                	j	1ac4 <strncpy+0xb2>
    1c24:	4695                	li	a3,5
    1c26:	bd79                	j	1ac4 <strncpy+0xb2>
    1c28:	4699                	li	a3,6
    1c2a:	bd69                	j	1ac4 <strncpy+0xb2>
    1c2c:	8082                	ret
    1c2e:	8082                	ret
    1c30:	8082                	ret

0000000000001c32 <open>:
#include <unistd.h>

#include "syscall.h"

int open(const char *path, int flags)
{
    1c32:	87aa                	mv	a5,a0
    1c34:	862e                	mv	a2,a1
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
}

static inline long __syscall4(long n, long a, long b, long c, long d)
{
    register long a7 __asm__("a7") = n;
    1c36:	03800893          	li	a7,56
    register long a0 __asm__("a0") = a;
    1c3a:	f9c00513          	li	a0,-100
    register long a1 __asm__("a1") = b;
    1c3e:	85be                	mv	a1,a5
    register long a2 __asm__("a2") = c;
    register long a3 __asm__("a3") = d;
    1c40:	4689                	li	a3,2
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3))
    1c42:	00000073          	ecall
    return syscall(SYS_openat, AT_FDCWD, path, flags, O_RDWR);
}
    1c46:	2501                	sext.w	a0,a0
    1c48:	8082                	ret

0000000000001c4a <openat>:
    register long a7 __asm__("a7") = n;
    1c4a:	03800893          	li	a7,56
    register long a3 __asm__("a3") = d;
    1c4e:	18000693          	li	a3,384
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3))
    1c52:	00000073          	ecall

int openat(int dirfd,const char *path, int flags)
{
    return syscall(SYS_openat, dirfd, path, flags, 0600);
}
    1c56:	2501                	sext.w	a0,a0
    1c58:	8082                	ret

0000000000001c5a <close>:
    register long a7 __asm__("a7") = n;
    1c5a:	03900893          	li	a7,57
    __asm_syscall("r"(a7), "0"(a0))
    1c5e:	00000073          	ecall

int close(int fd)
{
    return syscall(SYS_close, fd);
}
    1c62:	2501                	sext.w	a0,a0
    1c64:	8082                	ret

0000000000001c66 <read>:
    register long a7 __asm__("a7") = n;
    1c66:	03f00893          	li	a7,63
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1c6a:	00000073          	ecall

ssize_t read(int fd, void *buf, size_t len)
{
    return syscall(SYS_read, fd, buf, len);
}
    1c6e:	8082                	ret

0000000000001c70 <write>:
    register long a7 __asm__("a7") = n;
    1c70:	04000893          	li	a7,64
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1c74:	00000073          	ecall

ssize_t write(int fd, const void *buf, size_t len)
{
    return syscall(SYS_write, fd, buf, len);
}
    1c78:	8082                	ret

0000000000001c7a <getpid>:
    register long a7 __asm__("a7") = n;
    1c7a:	0ac00893          	li	a7,172
    __asm_syscall("r"(a7))
    1c7e:	00000073          	ecall

pid_t getpid(void)
{
    return syscall(SYS_getpid);
}
    1c82:	2501                	sext.w	a0,a0
    1c84:	8082                	ret

0000000000001c86 <getppid>:
    register long a7 __asm__("a7") = n;
    1c86:	0ad00893          	li	a7,173
    __asm_syscall("r"(a7))
    1c8a:	00000073          	ecall

pid_t getppid(void)
{
    return syscall(SYS_getppid);
}
    1c8e:	2501                	sext.w	a0,a0
    1c90:	8082                	ret

0000000000001c92 <sched_yield>:
    register long a7 __asm__("a7") = n;
    1c92:	07c00893          	li	a7,124
    __asm_syscall("r"(a7))
    1c96:	00000073          	ecall

int sched_yield(void)
{
    return syscall(SYS_sched_yield);
}
    1c9a:	2501                	sext.w	a0,a0
    1c9c:	8082                	ret

0000000000001c9e <fork>:
    register long a7 __asm__("a7") = n;
    1c9e:	0dc00893          	li	a7,220
    register long a0 __asm__("a0") = a;
    1ca2:	4545                	li	a0,17
    register long a1 __asm__("a1") = b;
    1ca4:	4581                	li	a1,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1ca6:	00000073          	ecall

pid_t fork(void)
{
    return syscall(SYS_clone, SIGCHLD, 0);
}
    1caa:	2501                	sext.w	a0,a0
    1cac:	8082                	ret

0000000000001cae <clone>:

pid_t clone(int (*fn)(void *arg), void *arg, void *stack, size_t stack_size, unsigned long flags)
{
    1cae:	85b2                	mv	a1,a2
    1cb0:	863a                	mv	a2,a4
    if (stack)
    1cb2:	c191                	beqz	a1,1cb6 <clone+0x8>
	stack += stack_size;
    1cb4:	95b6                	add	a1,a1,a3

    return __clone(fn, stack, flags, NULL, NULL, NULL);
    1cb6:	4781                	li	a5,0
    1cb8:	4701                	li	a4,0
    1cba:	4681                	li	a3,0
    1cbc:	2601                	sext.w	a2,a2
    1cbe:	a2dd                	j	1ea4 <__clone>

0000000000001cc0 <exit>:
    register long a7 __asm__("a7") = n;
    1cc0:	05d00893          	li	a7,93
    __asm_syscall("r"(a7), "0"(a0))
    1cc4:	00000073          	ecall
    //return syscall(SYS_clone, fn, stack, flags, NULL, NULL, NULL);
}
void exit(int code)
{
    syscall(SYS_exit, code);
}
    1cc8:	8082                	ret

0000000000001cca <waitpid>:
    register long a7 __asm__("a7") = n;
    1cca:	10400893          	li	a7,260
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1cce:	00000073          	ecall

int waitpid(int pid, int *code, int options)
{
    return syscall(SYS_wait4, pid, code, options);
}
    1cd2:	2501                	sext.w	a0,a0
    1cd4:	8082                	ret

0000000000001cd6 <exec>:
    register long a7 __asm__("a7") = n;
    1cd6:	0dd00893          	li	a7,221
    __asm_syscall("r"(a7), "0"(a0))
    1cda:	00000073          	ecall

int exec(char *name)
{
    return syscall(SYS_execve, name);
}
    1cde:	2501                	sext.w	a0,a0
    1ce0:	8082                	ret

0000000000001ce2 <execve>:
    register long a7 __asm__("a7") = n;
    1ce2:	0dd00893          	li	a7,221
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1ce6:	00000073          	ecall

int execve(const char *name, char *const argv[], char *const argp[])
{
    return syscall(SYS_execve, name, argv, argp);
}
    1cea:	2501                	sext.w	a0,a0
    1cec:	8082                	ret

0000000000001cee <times>:
    register long a7 __asm__("a7") = n;
    1cee:	09900893          	li	a7,153
    __asm_syscall("r"(a7), "0"(a0))
    1cf2:	00000073          	ecall

int times(void *mytimes)
{
	return syscall(SYS_times, mytimes);
}
    1cf6:	2501                	sext.w	a0,a0
    1cf8:	8082                	ret

0000000000001cfa <get_time>:

int64 get_time()
{
    1cfa:	1141                	addi	sp,sp,-16
    register long a7 __asm__("a7") = n;
    1cfc:	0a900893          	li	a7,169
    register long a0 __asm__("a0") = a;
    1d00:	850a                	mv	a0,sp
    register long a1 __asm__("a1") = b;
    1d02:	4581                	li	a1,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1d04:	00000073          	ecall
    TimeVal time;
    int err = sys_get_time(&time, 0);
    if (err == 0)
    1d08:	2501                	sext.w	a0,a0
    1d0a:	ed09                	bnez	a0,1d24 <get_time+0x2a>
    {
        return ((time.sec & 0xffff) * 1000 + time.usec / 1000);
    1d0c:	67a2                	ld	a5,8(sp)
    1d0e:	3e800713          	li	a4,1000
    1d12:	00015503          	lhu	a0,0(sp)
    1d16:	02e7d7b3          	divu	a5,a5,a4
    1d1a:	02e50533          	mul	a0,a0,a4
    1d1e:	953e                	add	a0,a0,a5
    }
    else
    {
        return -1;
    }
}
    1d20:	0141                	addi	sp,sp,16
    1d22:	8082                	ret
        return -1;
    1d24:	557d                	li	a0,-1
    1d26:	bfed                	j	1d20 <get_time+0x26>

0000000000001d28 <sys_get_time>:
    register long a7 __asm__("a7") = n;
    1d28:	0a900893          	li	a7,169
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1d2c:	00000073          	ecall

int sys_get_time(TimeVal *ts, int tz)
{
    return syscall(SYS_gettimeofday, ts, tz);
}
    1d30:	2501                	sext.w	a0,a0
    1d32:	8082                	ret

0000000000001d34 <time>:
    register long a7 __asm__("a7") = n;
    1d34:	42600893          	li	a7,1062
    __asm_syscall("r"(a7), "0"(a0))
    1d38:	00000073          	ecall

int time(unsigned long *tloc)
{
    return syscall(SYS_time, tloc);
}
    1d3c:	2501                	sext.w	a0,a0
    1d3e:	8082                	ret

0000000000001d40 <sleep>:

int sleep(unsigned long long time)
{
    1d40:	1141                	addi	sp,sp,-16
    TimeVal tv = {.sec = time, .usec = 0};
    1d42:	e02a                	sd	a0,0(sp)
    register long a0 __asm__("a0") = a;
    1d44:	850a                	mv	a0,sp
    1d46:	e402                	sd	zero,8(sp)
    register long a7 __asm__("a7") = n;
    1d48:	06500893          	li	a7,101
    register long a1 __asm__("a1") = b;
    1d4c:	85aa                	mv	a1,a0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1d4e:	00000073          	ecall
    if (syscall(SYS_nanosleep, &tv, &tv)) return tv.sec;
    1d52:	e501                	bnez	a0,1d5a <sleep+0x1a>
    return 0;
    1d54:	4501                	li	a0,0
}
    1d56:	0141                	addi	sp,sp,16
    1d58:	8082                	ret
    if (syscall(SYS_nanosleep, &tv, &tv)) return tv.sec;
    1d5a:	4502                	lw	a0,0(sp)
}
    1d5c:	0141                	addi	sp,sp,16
    1d5e:	8082                	ret

0000000000001d60 <set_priority>:
    register long a7 __asm__("a7") = n;
    1d60:	08c00893          	li	a7,140
    __asm_syscall("r"(a7), "0"(a0))
    1d64:	00000073          	ecall

int set_priority(int prio)
{
    return syscall(SYS_setpriority, prio);
}
    1d68:	2501                	sext.w	a0,a0
    1d6a:	8082                	ret

0000000000001d6c <mmap>:
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4))
}

static inline long __syscall6(long n, long a, long b, long c, long d, long e, long f)
{
    register long a7 __asm__("a7") = n;
    1d6c:	0de00893          	li	a7,222
    register long a1 __asm__("a1") = b;
    register long a2 __asm__("a2") = c;
    register long a3 __asm__("a3") = d;
    register long a4 __asm__("a4") = e;
    register long a5 __asm__("a5") = f;
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4), "r"(a5))
    1d70:	00000073          	ecall

void *mmap(void *start, size_t len, int prot, int flags, int fd, off_t off)
{
    return syscall(SYS_mmap, start, len, prot, flags, fd, off);
}
    1d74:	8082                	ret

0000000000001d76 <munmap>:
    register long a7 __asm__("a7") = n;
    1d76:	0d700893          	li	a7,215
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1d7a:	00000073          	ecall

int munmap(void *start, size_t len)
{
    return syscall(SYS_munmap, start, len);
}
    1d7e:	2501                	sext.w	a0,a0
    1d80:	8082                	ret

0000000000001d82 <wait>:

int wait(int *code)
{
    1d82:	85aa                	mv	a1,a0
    register long a7 __asm__("a7") = n;
    1d84:	10400893          	li	a7,260
    register long a0 __asm__("a0") = a;
    1d88:	557d                	li	a0,-1
    register long a2 __asm__("a2") = c;
    1d8a:	4601                	li	a2,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1d8c:	00000073          	ecall
    return waitpid(-1, code, 0);
}
    1d90:	2501                	sext.w	a0,a0
    1d92:	8082                	ret

0000000000001d94 <spawn>:
    register long a7 __asm__("a7") = n;
    1d94:	19000893          	li	a7,400
    __asm_syscall("r"(a7), "0"(a0))
    1d98:	00000073          	ecall

int spawn(char *file)
{
    return syscall(SYS_spawn, file);
}
    1d9c:	2501                	sext.w	a0,a0
    1d9e:	8082                	ret

0000000000001da0 <mailread>:
    register long a7 __asm__("a7") = n;
    1da0:	19100893          	li	a7,401
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1da4:	00000073          	ecall

int mailread(void *buf, int len)
{
    return syscall(SYS_mailread, buf, len);
}
    1da8:	2501                	sext.w	a0,a0
    1daa:	8082                	ret

0000000000001dac <mailwrite>:
    register long a7 __asm__("a7") = n;
    1dac:	19200893          	li	a7,402
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1db0:	00000073          	ecall

int mailwrite(int pid, void *buf, int len)
{
    return syscall(SYS_mailwrite, pid, buf, len);
}
    1db4:	2501                	sext.w	a0,a0
    1db6:	8082                	ret

0000000000001db8 <fstat>:
    register long a7 __asm__("a7") = n;
    1db8:	05000893          	li	a7,80
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1dbc:	00000073          	ecall

int fstat(int fd, struct kstat *st)
{
    return syscall(SYS_fstat, fd, st);
}
    1dc0:	2501                	sext.w	a0,a0
    1dc2:	8082                	ret

0000000000001dc4 <sys_linkat>:
    register long a4 __asm__("a4") = e;
    1dc4:	1702                	slli	a4,a4,0x20
    register long a7 __asm__("a7") = n;
    1dc6:	02500893          	li	a7,37
    register long a4 __asm__("a4") = e;
    1dca:	9301                	srli	a4,a4,0x20
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4))
    1dcc:	00000073          	ecall

int sys_linkat(int olddirfd, char *oldpath, int newdirfd, char *newpath, unsigned int flags)
{
    return syscall(SYS_linkat, olddirfd, oldpath, newdirfd, newpath, flags);
}
    1dd0:	2501                	sext.w	a0,a0
    1dd2:	8082                	ret

0000000000001dd4 <sys_unlinkat>:
    register long a2 __asm__("a2") = c;
    1dd4:	1602                	slli	a2,a2,0x20
    register long a7 __asm__("a7") = n;
    1dd6:	02300893          	li	a7,35
    register long a2 __asm__("a2") = c;
    1dda:	9201                	srli	a2,a2,0x20
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1ddc:	00000073          	ecall

int sys_unlinkat(int dirfd, char *path, unsigned int flags)
{
    return syscall(SYS_unlinkat, dirfd, path, flags);
}
    1de0:	2501                	sext.w	a0,a0
    1de2:	8082                	ret

0000000000001de4 <link>:

int link(char *old_path, char *new_path)
{
    1de4:	87aa                	mv	a5,a0
    1de6:	86ae                	mv	a3,a1
    register long a7 __asm__("a7") = n;
    1de8:	02500893          	li	a7,37
    register long a0 __asm__("a0") = a;
    1dec:	f9c00513          	li	a0,-100
    register long a1 __asm__("a1") = b;
    1df0:	85be                	mv	a1,a5
    register long a2 __asm__("a2") = c;
    1df2:	f9c00613          	li	a2,-100
    register long a4 __asm__("a4") = e;
    1df6:	4701                	li	a4,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4))
    1df8:	00000073          	ecall
    return sys_linkat(AT_FDCWD, old_path, AT_FDCWD, new_path, 0);
}
    1dfc:	2501                	sext.w	a0,a0
    1dfe:	8082                	ret

0000000000001e00 <unlink>:

int unlink(char *path)
{
    1e00:	85aa                	mv	a1,a0
    register long a7 __asm__("a7") = n;
    1e02:	02300893          	li	a7,35
    register long a0 __asm__("a0") = a;
    1e06:	f9c00513          	li	a0,-100
    register long a2 __asm__("a2") = c;
    1e0a:	4601                	li	a2,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1e0c:	00000073          	ecall
    return sys_unlinkat(AT_FDCWD, path, 0);
}
    1e10:	2501                	sext.w	a0,a0
    1e12:	8082                	ret

0000000000001e14 <uname>:
    register long a7 __asm__("a7") = n;
    1e14:	0a000893          	li	a7,160
    __asm_syscall("r"(a7), "0"(a0))
    1e18:	00000073          	ecall

int uname(void *buf)
{
    return syscall(SYS_uname, buf);
}
    1e1c:	2501                	sext.w	a0,a0
    1e1e:	8082                	ret

0000000000001e20 <brk>:
    register long a7 __asm__("a7") = n;
    1e20:	0d600893          	li	a7,214
    __asm_syscall("r"(a7), "0"(a0))
    1e24:	00000073          	ecall

int brk(void *addr)
{
    return syscall(SYS_brk, addr);
}
    1e28:	2501                	sext.w	a0,a0
    1e2a:	8082                	ret

0000000000001e2c <getcwd>:
    register long a7 __asm__("a7") = n;
    1e2c:	48c5                	li	a7,17
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1e2e:	00000073          	ecall

char *getcwd(char *buf, size_t size){
    return syscall(SYS_getcwd, buf, size);
}
    1e32:	8082                	ret

0000000000001e34 <chdir>:
    register long a7 __asm__("a7") = n;
    1e34:	03100893          	li	a7,49
    __asm_syscall("r"(a7), "0"(a0))
    1e38:	00000073          	ecall

int chdir(const char *path){
    return syscall(SYS_chdir, path);
}
    1e3c:	2501                	sext.w	a0,a0
    1e3e:	8082                	ret

0000000000001e40 <mkdir>:

int mkdir(const char *path, mode_t mode){
    1e40:	862e                	mv	a2,a1
    1e42:	87aa                	mv	a5,a0
    register long a2 __asm__("a2") = c;
    1e44:	1602                	slli	a2,a2,0x20
    register long a7 __asm__("a7") = n;
    1e46:	02200893          	li	a7,34
    register long a0 __asm__("a0") = a;
    1e4a:	f9c00513          	li	a0,-100
    register long a1 __asm__("a1") = b;
    1e4e:	85be                	mv	a1,a5
    register long a2 __asm__("a2") = c;
    1e50:	9201                	srli	a2,a2,0x20
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1e52:	00000073          	ecall
    return syscall(SYS_mkdirat, AT_FDCWD, path, mode);
}
    1e56:	2501                	sext.w	a0,a0
    1e58:	8082                	ret

0000000000001e5a <getdents>:
    register long a7 __asm__("a7") = n;
    1e5a:	03d00893          	li	a7,61
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1e5e:	00000073          	ecall

int getdents(int fd, struct linux_dirent64 *dirp64, unsigned long len){
    //return syscall(SYS_getdents64, fd, dirp64, len);
    return syscall(SYS_getdents64, fd, dirp64, len);
}
    1e62:	2501                	sext.w	a0,a0
    1e64:	8082                	ret

0000000000001e66 <pipe>:
    register long a7 __asm__("a7") = n;
    1e66:	03b00893          	li	a7,59
    register long a1 __asm__("a1") = b;
    1e6a:	4581                	li	a1,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1e6c:	00000073          	ecall

int pipe(int fd[2]){
    return syscall(SYS_pipe2, fd, 0);
}
    1e70:	2501                	sext.w	a0,a0
    1e72:	8082                	ret

0000000000001e74 <dup>:
    register long a7 __asm__("a7") = n;
    1e74:	48dd                	li	a7,23
    __asm_syscall("r"(a7), "0"(a0))
    1e76:	00000073          	ecall

int dup(int fd){
    return syscall(SYS_dup, fd);
}
    1e7a:	2501                	sext.w	a0,a0
    1e7c:	8082                	ret

0000000000001e7e <dup2>:
    register long a7 __asm__("a7") = n;
    1e7e:	48e1                	li	a7,24
    register long a2 __asm__("a2") = c;
    1e80:	4601                	li	a2,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1e82:	00000073          	ecall

int dup2(int old, int new){
    return syscall(SYS_dup3, old, new, 0);
}
    1e86:	2501                	sext.w	a0,a0
    1e88:	8082                	ret

0000000000001e8a <mount>:
    register long a7 __asm__("a7") = n;
    1e8a:	02800893          	li	a7,40
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4))
    1e8e:	00000073          	ecall

int mount(const char *special, const char *dir, const char *fstype, unsigned long flags, const void *data)
{
        return syscall(SYS_mount, special, dir, fstype, flags, data);
}
    1e92:	2501                	sext.w	a0,a0
    1e94:	8082                	ret

0000000000001e96 <umount>:
    register long a7 __asm__("a7") = n;
    1e96:	02700893          	li	a7,39
    register long a1 __asm__("a1") = b;
    1e9a:	4581                	li	a1,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1e9c:	00000073          	ecall

int umount(const char *special)
{
        return syscall(SYS_umount2, special, 0);
}
    1ea0:	2501                	sext.w	a0,a0
    1ea2:	8082                	ret

0000000000001ea4 <__clone>:

.global __clone
.type  __clone, %function
__clone:
	# Save func and arg to stack
	addi a1, a1, -16
    1ea4:	15c1                	addi	a1,a1,-16
	sd a0, 0(a1)
    1ea6:	e188                	sd	a0,0(a1)
	sd a3, 8(a1)
    1ea8:	e594                	sd	a3,8(a1)

	# Call SYS_clone
	mv a0, a2
    1eaa:	8532                	mv	a0,a2
	mv a2, a4
    1eac:	863a                	mv	a2,a4
	mv a3, a5
    1eae:	86be                	mv	a3,a5
	mv a4, a6
    1eb0:	8742                	mv	a4,a6
	li a7, 220 # SYS_clone
    1eb2:	0dc00893          	li	a7,220
	ecall
    1eb6:	00000073          	ecall

	beqz a0, 1f
    1eba:	c111                	beqz	a0,1ebe <__clone+0x1a>
	# Parent
	ret
    1ebc:	8082                	ret

	# Child
1:      ld a1, 0(sp)
    1ebe:	6582                	ld	a1,0(sp)
	ld a0, 8(sp)
    1ec0:	6522                	ld	a0,8(sp)
	jalr a1
    1ec2:	9582                	jalr	a1

	# Exit
	li a7, 93 # SYS_exit
    1ec4:	05d00893          	li	a7,93
	ecall
    1ec8:	00000073          	ecall
