
/home/ubuntu/rust/ultra-os/codes/riscv-syscalls-testing/user/build/riscv64/read:     file format elf64-littleriscv


Disassembly of section .text:

0000000000001000 <_start>:
.section .text.entry
.globl _start
_start:
    mv a0, sp
    1000:	850a                	mv	a0,sp
    tail __start_main
    1002:	a06d                	j	10ac <__start_main>

0000000000001004 <test_read>:
#include "unistd.h"
#include "stdio.h"
#include "stdlib.h"

void test_read() {
    1004:	712d                	addi	sp,sp,-288
	TEST_START(__func__);
    1006:	00001517          	auipc	a0,0x1
    100a:	ee250513          	addi	a0,a0,-286 # 1ee8 <__clone+0x2e>
void test_read() {
    100e:	ee06                	sd	ra,280(sp)
    1010:	ea22                	sd	s0,272(sp)
	TEST_START(__func__);
    1012:	30c000ef          	jal	ra,131e <puts>
    1016:	00001517          	auipc	a0,0x1
    101a:	f5a50513          	addi	a0,a0,-166 # 1f70 <__func__.0>
    101e:	300000ef          	jal	ra,131e <puts>
    1022:	00001517          	auipc	a0,0x1
    1026:	ede50513          	addi	a0,a0,-290 # 1f00 <__clone+0x46>
    102a:	2f4000ef          	jal	ra,131e <puts>
	int fd = open("./text.txt", 0);
    102e:	4581                	li	a1,0
    1030:	00001517          	auipc	a0,0x1
    1034:	ee050513          	addi	a0,a0,-288 # 1f10 <__clone+0x56>
    1038:	411000ef          	jal	ra,1c48 <open>
	char buf[256];
	int size = read(fd, buf, 256);
    103c:	10000613          	li	a2,256
    1040:	080c                	addi	a1,sp,16
	int fd = open("./text.txt", 0);
    1042:	842a                	mv	s0,a0
	int size = read(fd, buf, 256);
    1044:	439000ef          	jal	ra,1c7c <read>
    1048:	0005061b          	sext.w	a2,a0
	assert(size >= 0);
    104c:	02064f63          	bltz	a2,108a <test_read+0x86>

	write(STDOUT, buf, size);
    1050:	080c                	addi	a1,sp,16
    1052:	4505                	li	a0,1
    1054:	433000ef          	jal	ra,1c86 <write>
	close(fd);
    1058:	8522                	mv	a0,s0
    105a:	417000ef          	jal	ra,1c70 <close>
	TEST_END(__func__);
    105e:	00001517          	auipc	a0,0x1
    1062:	ee250513          	addi	a0,a0,-286 # 1f40 <__clone+0x86>
    1066:	2b8000ef          	jal	ra,131e <puts>
    106a:	00001517          	auipc	a0,0x1
    106e:	f0650513          	addi	a0,a0,-250 # 1f70 <__func__.0>
    1072:	2ac000ef          	jal	ra,131e <puts>
    1076:	00001517          	auipc	a0,0x1
    107a:	e8a50513          	addi	a0,a0,-374 # 1f00 <__clone+0x46>
    107e:	2a0000ef          	jal	ra,131e <puts>
}
    1082:	60f2                	ld	ra,280(sp)
    1084:	6452                	ld	s0,272(sp)
    1086:	6115                	addi	sp,sp,288
    1088:	8082                	ret
	assert(size >= 0);
    108a:	00001517          	auipc	a0,0x1
    108e:	e9650513          	addi	a0,a0,-362 # 1f20 <__clone+0x66>
    1092:	e432                	sd	a2,8(sp)
    1094:	530000ef          	jal	ra,15c4 <panic>
    1098:	6622                	ld	a2,8(sp)
    109a:	bf5d                	j	1050 <test_read+0x4c>

000000000000109c <main>:

int main(void) {
    109c:	1141                	addi	sp,sp,-16
    109e:	e406                	sd	ra,8(sp)
	test_read();
    10a0:	f65ff0ef          	jal	ra,1004 <test_read>
	return 0;
}
    10a4:	60a2                	ld	ra,8(sp)
    10a6:	4501                	li	a0,0
    10a8:	0141                	addi	sp,sp,16
    10aa:	8082                	ret

00000000000010ac <__start_main>:
#include <unistd.h>

extern int main();

int __start_main(long *p)
{
    10ac:	85aa                	mv	a1,a0
	int argc = p[0];
	char **argv = (void *)(p+1);

	exit(main(argc, argv));
    10ae:	4108                	lw	a0,0(a0)
{
    10b0:	1141                	addi	sp,sp,-16
	exit(main(argc, argv));
    10b2:	05a1                	addi	a1,a1,8
{
    10b4:	e406                	sd	ra,8(sp)
	exit(main(argc, argv));
    10b6:	fe7ff0ef          	jal	ra,109c <main>
    10ba:	41d000ef          	jal	ra,1cd6 <exit>
	return 0;
}
    10be:	60a2                	ld	ra,8(sp)
    10c0:	4501                	li	a0,0
    10c2:	0141                	addi	sp,sp,16
    10c4:	8082                	ret

00000000000010c6 <printint.constprop.0>:
    write(f, s, l);
}

static char digits[] = "0123456789abcdef";

static void printint(int xx, int base, int sign)
    10c6:	7179                	addi	sp,sp,-48
    10c8:	f406                	sd	ra,40(sp)
{
    char buf[16 + 1];
    int i;
    uint x;

    if (sign && (sign = xx < 0))
    10ca:	12054b63          	bltz	a0,1200 <printint.constprop.0+0x13a>

    buf[16] = 0;
    i = 15;
    do
    {
        buf[i--] = digits[x % base];
    10ce:	02b577bb          	remuw	a5,a0,a1
    10d2:	00001617          	auipc	a2,0x1
    10d6:	eae60613          	addi	a2,a2,-338 # 1f80 <digits>
    buf[16] = 0;
    10da:	00010c23          	sb	zero,24(sp)
        buf[i--] = digits[x % base];
    10de:	0005871b          	sext.w	a4,a1
    10e2:	1782                	slli	a5,a5,0x20
    10e4:	9381                	srli	a5,a5,0x20
    10e6:	97b2                	add	a5,a5,a2
    10e8:	0007c783          	lbu	a5,0(a5)
    } while ((x /= base) != 0);
    10ec:	02b5583b          	divuw	a6,a0,a1
        buf[i--] = digits[x % base];
    10f0:	00f10ba3          	sb	a5,23(sp)
    } while ((x /= base) != 0);
    10f4:	1cb56363          	bltu	a0,a1,12ba <printint.constprop.0+0x1f4>
        buf[i--] = digits[x % base];
    10f8:	45b9                	li	a1,14
    10fa:	02e877bb          	remuw	a5,a6,a4
    10fe:	1782                	slli	a5,a5,0x20
    1100:	9381                	srli	a5,a5,0x20
    1102:	97b2                	add	a5,a5,a2
    1104:	0007c783          	lbu	a5,0(a5)
    } while ((x /= base) != 0);
    1108:	02e856bb          	divuw	a3,a6,a4
        buf[i--] = digits[x % base];
    110c:	00f10b23          	sb	a5,22(sp)
    } while ((x /= base) != 0);
    1110:	0ce86e63          	bltu	a6,a4,11ec <printint.constprop.0+0x126>
        buf[i--] = digits[x % base];
    1114:	02e6f5bb          	remuw	a1,a3,a4
    } while ((x /= base) != 0);
    1118:	02e6d7bb          	divuw	a5,a3,a4
        buf[i--] = digits[x % base];
    111c:	1582                	slli	a1,a1,0x20
    111e:	9181                	srli	a1,a1,0x20
    1120:	95b2                	add	a1,a1,a2
    1122:	0005c583          	lbu	a1,0(a1)
    1126:	00b10aa3          	sb	a1,21(sp)
    } while ((x /= base) != 0);
    112a:	0007859b          	sext.w	a1,a5
    112e:	12e6ec63          	bltu	a3,a4,1266 <printint.constprop.0+0x1a0>
        buf[i--] = digits[x % base];
    1132:	02e7f6bb          	remuw	a3,a5,a4
    1136:	1682                	slli	a3,a3,0x20
    1138:	9281                	srli	a3,a3,0x20
    113a:	96b2                	add	a3,a3,a2
    113c:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    1140:	02e7d83b          	divuw	a6,a5,a4
        buf[i--] = digits[x % base];
    1144:	00d10a23          	sb	a3,20(sp)
    } while ((x /= base) != 0);
    1148:	12e5e863          	bltu	a1,a4,1278 <printint.constprop.0+0x1b2>
        buf[i--] = digits[x % base];
    114c:	02e876bb          	remuw	a3,a6,a4
    1150:	1682                	slli	a3,a3,0x20
    1152:	9281                	srli	a3,a3,0x20
    1154:	96b2                	add	a3,a3,a2
    1156:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    115a:	02e855bb          	divuw	a1,a6,a4
        buf[i--] = digits[x % base];
    115e:	00d109a3          	sb	a3,19(sp)
    } while ((x /= base) != 0);
    1162:	12e86463          	bltu	a6,a4,128a <printint.constprop.0+0x1c4>
        buf[i--] = digits[x % base];
    1166:	02e5f6bb          	remuw	a3,a1,a4
    116a:	1682                	slli	a3,a3,0x20
    116c:	9281                	srli	a3,a3,0x20
    116e:	96b2                	add	a3,a3,a2
    1170:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    1174:	02e5d83b          	divuw	a6,a1,a4
        buf[i--] = digits[x % base];
    1178:	00d10923          	sb	a3,18(sp)
    } while ((x /= base) != 0);
    117c:	0ce5ec63          	bltu	a1,a4,1254 <printint.constprop.0+0x18e>
        buf[i--] = digits[x % base];
    1180:	02e876bb          	remuw	a3,a6,a4
    1184:	1682                	slli	a3,a3,0x20
    1186:	9281                	srli	a3,a3,0x20
    1188:	96b2                	add	a3,a3,a2
    118a:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    118e:	02e855bb          	divuw	a1,a6,a4
        buf[i--] = digits[x % base];
    1192:	00d108a3          	sb	a3,17(sp)
    } while ((x /= base) != 0);
    1196:	10e86963          	bltu	a6,a4,12a8 <printint.constprop.0+0x1e2>
        buf[i--] = digits[x % base];
    119a:	02e5f6bb          	remuw	a3,a1,a4
    119e:	1682                	slli	a3,a3,0x20
    11a0:	9281                	srli	a3,a3,0x20
    11a2:	96b2                	add	a3,a3,a2
    11a4:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    11a8:	02e5d83b          	divuw	a6,a1,a4
        buf[i--] = digits[x % base];
    11ac:	00d10823          	sb	a3,16(sp)
    } while ((x /= base) != 0);
    11b0:	10e5e763          	bltu	a1,a4,12be <printint.constprop.0+0x1f8>
        buf[i--] = digits[x % base];
    11b4:	02e876bb          	remuw	a3,a6,a4
    11b8:	1682                	slli	a3,a3,0x20
    11ba:	9281                	srli	a3,a3,0x20
    11bc:	96b2                	add	a3,a3,a2
    11be:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    11c2:	02e857bb          	divuw	a5,a6,a4
        buf[i--] = digits[x % base];
    11c6:	00d107a3          	sb	a3,15(sp)
    } while ((x /= base) != 0);
    11ca:	10e86363          	bltu	a6,a4,12d0 <printint.constprop.0+0x20a>
        buf[i--] = digits[x % base];
    11ce:	1782                	slli	a5,a5,0x20
    11d0:	9381                	srli	a5,a5,0x20
    11d2:	97b2                	add	a5,a5,a2
    11d4:	0007c783          	lbu	a5,0(a5)
    11d8:	4599                	li	a1,6
    11da:	00f10723          	sb	a5,14(sp)

    if (sign)
    11de:	00055763          	bgez	a0,11ec <printint.constprop.0+0x126>
        buf[i--] = '-';
    11e2:	02d00793          	li	a5,45
    11e6:	00f106a3          	sb	a5,13(sp)
        buf[i--] = digits[x % base];
    11ea:	4595                	li	a1,5
    write(f, s, l);
    11ec:	003c                	addi	a5,sp,8
    11ee:	4641                	li	a2,16
    11f0:	9e0d                	subw	a2,a2,a1
    11f2:	4505                	li	a0,1
    11f4:	95be                	add	a1,a1,a5
    11f6:	291000ef          	jal	ra,1c86 <write>
    i++;
    if (i < 0)
        puts("printint error");
    out(stdout, buf + i, 16 - i);
}
    11fa:	70a2                	ld	ra,40(sp)
    11fc:	6145                	addi	sp,sp,48
    11fe:	8082                	ret
        x = -xx;
    1200:	40a0083b          	negw	a6,a0
        buf[i--] = digits[x % base];
    1204:	02b877bb          	remuw	a5,a6,a1
    1208:	00001617          	auipc	a2,0x1
    120c:	d7860613          	addi	a2,a2,-648 # 1f80 <digits>
    buf[16] = 0;
    1210:	00010c23          	sb	zero,24(sp)
        buf[i--] = digits[x % base];
    1214:	0005871b          	sext.w	a4,a1
    1218:	1782                	slli	a5,a5,0x20
    121a:	9381                	srli	a5,a5,0x20
    121c:	97b2                	add	a5,a5,a2
    121e:	0007c783          	lbu	a5,0(a5)
    } while ((x /= base) != 0);
    1222:	02b858bb          	divuw	a7,a6,a1
        buf[i--] = digits[x % base];
    1226:	00f10ba3          	sb	a5,23(sp)
    } while ((x /= base) != 0);
    122a:	06b86963          	bltu	a6,a1,129c <printint.constprop.0+0x1d6>
        buf[i--] = digits[x % base];
    122e:	02e8f7bb          	remuw	a5,a7,a4
    1232:	1782                	slli	a5,a5,0x20
    1234:	9381                	srli	a5,a5,0x20
    1236:	97b2                	add	a5,a5,a2
    1238:	0007c783          	lbu	a5,0(a5)
    } while ((x /= base) != 0);
    123c:	02e8d6bb          	divuw	a3,a7,a4
        buf[i--] = digits[x % base];
    1240:	00f10b23          	sb	a5,22(sp)
    } while ((x /= base) != 0);
    1244:	ece8f8e3          	bgeu	a7,a4,1114 <printint.constprop.0+0x4e>
        buf[i--] = '-';
    1248:	02d00793          	li	a5,45
    124c:	00f10aa3          	sb	a5,21(sp)
        buf[i--] = digits[x % base];
    1250:	45b5                	li	a1,13
    1252:	bf69                	j	11ec <printint.constprop.0+0x126>
    1254:	45a9                	li	a1,10
    if (sign)
    1256:	f8055be3          	bgez	a0,11ec <printint.constprop.0+0x126>
        buf[i--] = '-';
    125a:	02d00793          	li	a5,45
    125e:	00f108a3          	sb	a5,17(sp)
        buf[i--] = digits[x % base];
    1262:	45a5                	li	a1,9
    1264:	b761                	j	11ec <printint.constprop.0+0x126>
    1266:	45b5                	li	a1,13
    if (sign)
    1268:	f80552e3          	bgez	a0,11ec <printint.constprop.0+0x126>
        buf[i--] = '-';
    126c:	02d00793          	li	a5,45
    1270:	00f10a23          	sb	a5,20(sp)
        buf[i--] = digits[x % base];
    1274:	45b1                	li	a1,12
    1276:	bf9d                	j	11ec <printint.constprop.0+0x126>
    1278:	45b1                	li	a1,12
    if (sign)
    127a:	f60559e3          	bgez	a0,11ec <printint.constprop.0+0x126>
        buf[i--] = '-';
    127e:	02d00793          	li	a5,45
    1282:	00f109a3          	sb	a5,19(sp)
        buf[i--] = digits[x % base];
    1286:	45ad                	li	a1,11
    1288:	b795                	j	11ec <printint.constprop.0+0x126>
    128a:	45ad                	li	a1,11
    if (sign)
    128c:	f60550e3          	bgez	a0,11ec <printint.constprop.0+0x126>
        buf[i--] = '-';
    1290:	02d00793          	li	a5,45
    1294:	00f10923          	sb	a5,18(sp)
        buf[i--] = digits[x % base];
    1298:	45a9                	li	a1,10
    129a:	bf89                	j	11ec <printint.constprop.0+0x126>
        buf[i--] = '-';
    129c:	02d00793          	li	a5,45
    12a0:	00f10b23          	sb	a5,22(sp)
        buf[i--] = digits[x % base];
    12a4:	45b9                	li	a1,14
    12a6:	b799                	j	11ec <printint.constprop.0+0x126>
    12a8:	45a5                	li	a1,9
    if (sign)
    12aa:	f40551e3          	bgez	a0,11ec <printint.constprop.0+0x126>
        buf[i--] = '-';
    12ae:	02d00793          	li	a5,45
    12b2:	00f10823          	sb	a5,16(sp)
        buf[i--] = digits[x % base];
    12b6:	45a1                	li	a1,8
    12b8:	bf15                	j	11ec <printint.constprop.0+0x126>
    i = 15;
    12ba:	45bd                	li	a1,15
    12bc:	bf05                	j	11ec <printint.constprop.0+0x126>
        buf[i--] = digits[x % base];
    12be:	45a1                	li	a1,8
    if (sign)
    12c0:	f20556e3          	bgez	a0,11ec <printint.constprop.0+0x126>
        buf[i--] = '-';
    12c4:	02d00793          	li	a5,45
    12c8:	00f107a3          	sb	a5,15(sp)
        buf[i--] = digits[x % base];
    12cc:	459d                	li	a1,7
    12ce:	bf39                	j	11ec <printint.constprop.0+0x126>
    12d0:	459d                	li	a1,7
    if (sign)
    12d2:	f0055de3          	bgez	a0,11ec <printint.constprop.0+0x126>
        buf[i--] = '-';
    12d6:	02d00793          	li	a5,45
    12da:	00f10723          	sb	a5,14(sp)
        buf[i--] = digits[x % base];
    12de:	4599                	li	a1,6
    12e0:	b731                	j	11ec <printint.constprop.0+0x126>

00000000000012e2 <getchar>:
{
    12e2:	1101                	addi	sp,sp,-32
    read(stdin, &byte, 1);
    12e4:	00f10593          	addi	a1,sp,15
    12e8:	4605                	li	a2,1
    12ea:	4501                	li	a0,0
{
    12ec:	ec06                	sd	ra,24(sp)
    char byte = 0;
    12ee:	000107a3          	sb	zero,15(sp)
    read(stdin, &byte, 1);
    12f2:	18b000ef          	jal	ra,1c7c <read>
}
    12f6:	60e2                	ld	ra,24(sp)
    12f8:	00f14503          	lbu	a0,15(sp)
    12fc:	6105                	addi	sp,sp,32
    12fe:	8082                	ret

0000000000001300 <putchar>:
{
    1300:	1101                	addi	sp,sp,-32
    1302:	87aa                	mv	a5,a0
    return write(stdout, &byte, 1);
    1304:	00f10593          	addi	a1,sp,15
    1308:	4605                	li	a2,1
    130a:	4505                	li	a0,1
{
    130c:	ec06                	sd	ra,24(sp)
    char byte = c;
    130e:	00f107a3          	sb	a5,15(sp)
    return write(stdout, &byte, 1);
    1312:	175000ef          	jal	ra,1c86 <write>
}
    1316:	60e2                	ld	ra,24(sp)
    1318:	2501                	sext.w	a0,a0
    131a:	6105                	addi	sp,sp,32
    131c:	8082                	ret

000000000000131e <puts>:
{
    131e:	1141                	addi	sp,sp,-16
    1320:	e406                	sd	ra,8(sp)
    1322:	e022                	sd	s0,0(sp)
    1324:	842a                	mv	s0,a0
    r = -(write(stdout, s, strlen(s)) < 0);
    1326:	57c000ef          	jal	ra,18a2 <strlen>
    132a:	862a                	mv	a2,a0
    132c:	85a2                	mv	a1,s0
    132e:	4505                	li	a0,1
    1330:	157000ef          	jal	ra,1c86 <write>
}
    1334:	60a2                	ld	ra,8(sp)
    1336:	6402                	ld	s0,0(sp)
    r = -(write(stdout, s, strlen(s)) < 0);
    1338:	957d                	srai	a0,a0,0x3f
    return r;
    133a:	2501                	sext.w	a0,a0
}
    133c:	0141                	addi	sp,sp,16
    133e:	8082                	ret

0000000000001340 <printf>:
    out(stdout, buf, i);
}

// Print to the console. only understands %d, %x, %p, %s.
void printf(const char *fmt, ...)
{
    1340:	7171                	addi	sp,sp,-176
    1342:	fc56                	sd	s5,56(sp)
    1344:	ed3e                	sd	a5,152(sp)
    buf[i++] = '0';
    1346:	7ae1                	lui	s5,0xffff8
    va_list ap;
    int cnt = 0, l = 0;
    char *a, *z, *s = (char *)fmt, str;
    int f = stdout;

    va_start(ap, fmt);
    1348:	18bc                	addi	a5,sp,120
{
    134a:	e8ca                	sd	s2,80(sp)
    134c:	e4ce                	sd	s3,72(sp)
    134e:	e0d2                	sd	s4,64(sp)
    1350:	f85a                	sd	s6,48(sp)
    1352:	f486                	sd	ra,104(sp)
    1354:	f0a2                	sd	s0,96(sp)
    1356:	eca6                	sd	s1,88(sp)
    1358:	fcae                	sd	a1,120(sp)
    135a:	e132                	sd	a2,128(sp)
    135c:	e536                	sd	a3,136(sp)
    135e:	e93a                	sd	a4,144(sp)
    1360:	f142                	sd	a6,160(sp)
    1362:	f546                	sd	a7,168(sp)
    va_start(ap, fmt);
    1364:	e03e                	sd	a5,0(sp)
    for (;;)
    {
        if (!*s)
            break;
        for (a = s; *s && *s != '%'; s++)
    1366:	02500913          	li	s2,37
        out(f, a, l);
        if (l)
            continue;
        if (s[1] == 0)
            break;
        switch (s[1])
    136a:	07300a13          	li	s4,115
        case 'p':
            printptr(va_arg(ap, uint64));
            break;
        case 's':
            if ((a = va_arg(ap, char *)) == 0)
                a = "(null)";
    136e:	00001b17          	auipc	s6,0x1
    1372:	be2b0b13          	addi	s6,s6,-1054 # 1f50 <__clone+0x96>
    buf[i++] = '0';
    1376:	830aca93          	xori	s5,s5,-2000
        buf[i++] = digits[x >> (sizeof(uint64) * 8 - 4)];
    137a:	00001997          	auipc	s3,0x1
    137e:	c0698993          	addi	s3,s3,-1018 # 1f80 <digits>
        if (!*s)
    1382:	00054783          	lbu	a5,0(a0)
    1386:	16078a63          	beqz	a5,14fa <printf+0x1ba>
    138a:	862a                	mv	a2,a0
        for (a = s; *s && *s != '%'; s++)
    138c:	19278163          	beq	a5,s2,150e <printf+0x1ce>
    1390:	00164783          	lbu	a5,1(a2)
    1394:	0605                	addi	a2,a2,1
    1396:	fbfd                	bnez	a5,138c <printf+0x4c>
    1398:	84b2                	mv	s1,a2
        l = z - a;
    139a:	40a6043b          	subw	s0,a2,a0
    write(f, s, l);
    139e:	85aa                	mv	a1,a0
    13a0:	8622                	mv	a2,s0
    13a2:	4505                	li	a0,1
    13a4:	0e3000ef          	jal	ra,1c86 <write>
        if (l)
    13a8:	18041c63          	bnez	s0,1540 <printf+0x200>
        if (s[1] == 0)
    13ac:	0014c783          	lbu	a5,1(s1)
    13b0:	14078563          	beqz	a5,14fa <printf+0x1ba>
        switch (s[1])
    13b4:	1d478063          	beq	a5,s4,1574 <printf+0x234>
    13b8:	18fa6663          	bltu	s4,a5,1544 <printf+0x204>
    13bc:	06400713          	li	a4,100
    13c0:	1ae78063          	beq	a5,a4,1560 <printf+0x220>
    13c4:	07000713          	li	a4,112
    13c8:	1ce79963          	bne	a5,a4,159a <printf+0x25a>
            printptr(va_arg(ap, uint64));
    13cc:	6702                	ld	a4,0(sp)
    buf[i++] = '0';
    13ce:	01511423          	sh	s5,8(sp)
    write(f, s, l);
    13d2:	4649                	li	a2,18
            printptr(va_arg(ap, uint64));
    13d4:	631c                	ld	a5,0(a4)
    13d6:	0721                	addi	a4,a4,8
    13d8:	e03a                	sd	a4,0(sp)
    for (j = 0; j < (sizeof(uint64) * 2); j++, x <<= 4)
    13da:	00479293          	slli	t0,a5,0x4
    13de:	00879f93          	slli	t6,a5,0x8
    13e2:	00c79f13          	slli	t5,a5,0xc
    13e6:	01079e93          	slli	t4,a5,0x10
    13ea:	01479e13          	slli	t3,a5,0x14
    13ee:	01879313          	slli	t1,a5,0x18
    13f2:	01c79893          	slli	a7,a5,0x1c
    13f6:	02479813          	slli	a6,a5,0x24
    13fa:	02879513          	slli	a0,a5,0x28
    13fe:	02c79593          	slli	a1,a5,0x2c
    1402:	03079693          	slli	a3,a5,0x30
    1406:	03479713          	slli	a4,a5,0x34
        buf[i++] = digits[x >> (sizeof(uint64) * 8 - 4)];
    140a:	03c7d413          	srli	s0,a5,0x3c
    140e:	01c7d39b          	srliw	t2,a5,0x1c
    1412:	03c2d293          	srli	t0,t0,0x3c
    1416:	03cfdf93          	srli	t6,t6,0x3c
    141a:	03cf5f13          	srli	t5,t5,0x3c
    141e:	03cede93          	srli	t4,t4,0x3c
    1422:	03ce5e13          	srli	t3,t3,0x3c
    1426:	03c35313          	srli	t1,t1,0x3c
    142a:	03c8d893          	srli	a7,a7,0x3c
    142e:	03c85813          	srli	a6,a6,0x3c
    1432:	9171                	srli	a0,a0,0x3c
    1434:	91f1                	srli	a1,a1,0x3c
    1436:	92f1                	srli	a3,a3,0x3c
    1438:	9371                	srli	a4,a4,0x3c
    143a:	96ce                	add	a3,a3,s3
    143c:	974e                	add	a4,a4,s3
    143e:	944e                	add	s0,s0,s3
    1440:	92ce                	add	t0,t0,s3
    1442:	9fce                	add	t6,t6,s3
    1444:	9f4e                	add	t5,t5,s3
    1446:	9ece                	add	t4,t4,s3
    1448:	9e4e                	add	t3,t3,s3
    144a:	934e                	add	t1,t1,s3
    144c:	98ce                	add	a7,a7,s3
    144e:	93ce                	add	t2,t2,s3
    1450:	984e                	add	a6,a6,s3
    1452:	954e                	add	a0,a0,s3
    1454:	95ce                	add	a1,a1,s3
    1456:	0006c083          	lbu	ra,0(a3)
    145a:	0002c283          	lbu	t0,0(t0)
    145e:	00074683          	lbu	a3,0(a4)
    1462:	000fcf83          	lbu	t6,0(t6)
    1466:	000f4f03          	lbu	t5,0(t5)
    146a:	000ece83          	lbu	t4,0(t4)
    146e:	000e4e03          	lbu	t3,0(t3)
    1472:	00034303          	lbu	t1,0(t1)
    1476:	0008c883          	lbu	a7,0(a7)
    147a:	0003c383          	lbu	t2,0(t2)
    147e:	00084803          	lbu	a6,0(a6)
    1482:	00054503          	lbu	a0,0(a0)
    1486:	0005c583          	lbu	a1,0(a1)
    148a:	00044403          	lbu	s0,0(s0)
    for (j = 0; j < (sizeof(uint64) * 2); j++, x <<= 4)
    148e:	03879713          	slli	a4,a5,0x38
        buf[i++] = digits[x >> (sizeof(uint64) * 8 - 4)];
    1492:	9371                	srli	a4,a4,0x3c
    1494:	8bbd                	andi	a5,a5,15
    1496:	974e                	add	a4,a4,s3
    1498:	97ce                	add	a5,a5,s3
    149a:	005105a3          	sb	t0,11(sp)
    149e:	01f10623          	sb	t6,12(sp)
    14a2:	01e106a3          	sb	t5,13(sp)
    14a6:	01d10723          	sb	t4,14(sp)
    14aa:	01c107a3          	sb	t3,15(sp)
    14ae:	00610823          	sb	t1,16(sp)
    14b2:	011108a3          	sb	a7,17(sp)
    14b6:	00710923          	sb	t2,18(sp)
    14ba:	010109a3          	sb	a6,19(sp)
    14be:	00a10a23          	sb	a0,20(sp)
    14c2:	00b10aa3          	sb	a1,21(sp)
    14c6:	00110b23          	sb	ra,22(sp)
    14ca:	00d10ba3          	sb	a3,23(sp)
    14ce:	00810523          	sb	s0,10(sp)
    14d2:	00074703          	lbu	a4,0(a4)
    14d6:	0007c783          	lbu	a5,0(a5)
    write(f, s, l);
    14da:	002c                	addi	a1,sp,8
    14dc:	4505                	li	a0,1
        buf[i++] = digits[x >> (sizeof(uint64) * 8 - 4)];
    14de:	00e10c23          	sb	a4,24(sp)
    14e2:	00f10ca3          	sb	a5,25(sp)
    buf[i] = 0;
    14e6:	00010d23          	sb	zero,26(sp)
    write(f, s, l);
    14ea:	79c000ef          	jal	ra,1c86 <write>
            // Print unknown % sequence to draw attention.
            putchar('%');
            putchar(s[1]);
            break;
        }
        s += 2;
    14ee:	00248513          	addi	a0,s1,2
        if (!*s)
    14f2:	00054783          	lbu	a5,0(a0)
    14f6:	e8079ae3          	bnez	a5,138a <printf+0x4a>
    }
    va_end(ap);
}
    14fa:	70a6                	ld	ra,104(sp)
    14fc:	7406                	ld	s0,96(sp)
    14fe:	64e6                	ld	s1,88(sp)
    1500:	6946                	ld	s2,80(sp)
    1502:	69a6                	ld	s3,72(sp)
    1504:	6a06                	ld	s4,64(sp)
    1506:	7ae2                	ld	s5,56(sp)
    1508:	7b42                	ld	s6,48(sp)
    150a:	614d                	addi	sp,sp,176
    150c:	8082                	ret
        for (z = s; s[0] == '%' && s[1] == '%'; z++, s += 2)
    150e:	00064783          	lbu	a5,0(a2)
    1512:	84b2                	mv	s1,a2
    1514:	01278963          	beq	a5,s2,1526 <printf+0x1e6>
    1518:	b549                	j	139a <printf+0x5a>
    151a:	0024c783          	lbu	a5,2(s1)
    151e:	0605                	addi	a2,a2,1
    1520:	0489                	addi	s1,s1,2
    1522:	e7279ce3          	bne	a5,s2,139a <printf+0x5a>
    1526:	0014c783          	lbu	a5,1(s1)
    152a:	ff2788e3          	beq	a5,s2,151a <printf+0x1da>
        l = z - a;
    152e:	40a6043b          	subw	s0,a2,a0
    write(f, s, l);
    1532:	85aa                	mv	a1,a0
    1534:	8622                	mv	a2,s0
    1536:	4505                	li	a0,1
    1538:	74e000ef          	jal	ra,1c86 <write>
        if (l)
    153c:	e60408e3          	beqz	s0,13ac <printf+0x6c>
    1540:	8526                	mv	a0,s1
    1542:	b581                	j	1382 <printf+0x42>
        switch (s[1])
    1544:	07800713          	li	a4,120
    1548:	04e79963          	bne	a5,a4,159a <printf+0x25a>
            printint(va_arg(ap, int), 16, 1);
    154c:	6782                	ld	a5,0(sp)
    154e:	45c1                	li	a1,16
    1550:	4388                	lw	a0,0(a5)
    1552:	07a1                	addi	a5,a5,8
    1554:	e03e                	sd	a5,0(sp)
    1556:	b71ff0ef          	jal	ra,10c6 <printint.constprop.0>
        s += 2;
    155a:	00248513          	addi	a0,s1,2
    155e:	bf51                	j	14f2 <printf+0x1b2>
            printint(va_arg(ap, int), 10, 1);
    1560:	6782                	ld	a5,0(sp)
    1562:	45a9                	li	a1,10
    1564:	4388                	lw	a0,0(a5)
    1566:	07a1                	addi	a5,a5,8
    1568:	e03e                	sd	a5,0(sp)
    156a:	b5dff0ef          	jal	ra,10c6 <printint.constprop.0>
        s += 2;
    156e:	00248513          	addi	a0,s1,2
    1572:	b741                	j	14f2 <printf+0x1b2>
            if ((a = va_arg(ap, char *)) == 0)
    1574:	6782                	ld	a5,0(sp)
    1576:	6380                	ld	s0,0(a5)
    1578:	07a1                	addi	a5,a5,8
    157a:	e03e                	sd	a5,0(sp)
    157c:	c031                	beqz	s0,15c0 <printf+0x280>
            l = strnlen(a, 200);
    157e:	0c800593          	li	a1,200
    1582:	8522                	mv	a0,s0
    1584:	40a000ef          	jal	ra,198e <strnlen>
    write(f, s, l);
    1588:	0005061b          	sext.w	a2,a0
    158c:	85a2                	mv	a1,s0
    158e:	4505                	li	a0,1
    1590:	6f6000ef          	jal	ra,1c86 <write>
        s += 2;
    1594:	00248513          	addi	a0,s1,2
    1598:	bfa9                	j	14f2 <printf+0x1b2>
    return write(stdout, &byte, 1);
    159a:	4605                	li	a2,1
    159c:	002c                	addi	a1,sp,8
    159e:	4505                	li	a0,1
    char byte = c;
    15a0:	01210423          	sb	s2,8(sp)
    return write(stdout, &byte, 1);
    15a4:	6e2000ef          	jal	ra,1c86 <write>
    char byte = c;
    15a8:	0014c783          	lbu	a5,1(s1)
    return write(stdout, &byte, 1);
    15ac:	4605                	li	a2,1
    15ae:	002c                	addi	a1,sp,8
    15b0:	4505                	li	a0,1
    char byte = c;
    15b2:	00f10423          	sb	a5,8(sp)
    return write(stdout, &byte, 1);
    15b6:	6d0000ef          	jal	ra,1c86 <write>
        s += 2;
    15ba:	00248513          	addi	a0,s1,2
    15be:	bf15                	j	14f2 <printf+0x1b2>
                a = "(null)";
    15c0:	845a                	mv	s0,s6
    15c2:	bf75                	j	157e <printf+0x23e>

00000000000015c4 <panic>:
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

void panic(char *m)
{
    15c4:	1141                	addi	sp,sp,-16
    15c6:	e406                	sd	ra,8(sp)
    puts(m);
    15c8:	d57ff0ef          	jal	ra,131e <puts>
    exit(-100);
}
    15cc:	60a2                	ld	ra,8(sp)
    exit(-100);
    15ce:	f9c00513          	li	a0,-100
}
    15d2:	0141                	addi	sp,sp,16
    exit(-100);
    15d4:	a709                	j	1cd6 <exit>

00000000000015d6 <isspace>:
#define HIGHS (ONES * (UCHAR_MAX / 2 + 1))
#define HASZERO(x) (((x)-ONES) & ~(x)&HIGHS)

int isspace(int c)
{
    return c == ' ' || (unsigned)c - '\t' < 5;
    15d6:	02000793          	li	a5,32
    15da:	00f50663          	beq	a0,a5,15e6 <isspace+0x10>
    15de:	355d                	addiw	a0,a0,-9
    15e0:	00553513          	sltiu	a0,a0,5
    15e4:	8082                	ret
    15e6:	4505                	li	a0,1
}
    15e8:	8082                	ret

00000000000015ea <isdigit>:

int isdigit(int c)
{
    return (unsigned)c - '0' < 10;
    15ea:	fd05051b          	addiw	a0,a0,-48
}
    15ee:	00a53513          	sltiu	a0,a0,10
    15f2:	8082                	ret

00000000000015f4 <atoi>:
    return c == ' ' || (unsigned)c - '\t' < 5;
    15f4:	02000613          	li	a2,32
    15f8:	4591                	li	a1,4

int atoi(const char *s)
{
    int n = 0, neg = 0;
    while (isspace(*s))
    15fa:	00054703          	lbu	a4,0(a0)
    return c == ' ' || (unsigned)c - '\t' < 5;
    15fe:	ff77069b          	addiw	a3,a4,-9
    1602:	04c70d63          	beq	a4,a2,165c <atoi+0x68>
    1606:	0007079b          	sext.w	a5,a4
    160a:	04d5f963          	bgeu	a1,a3,165c <atoi+0x68>
        s++;
    switch (*s)
    160e:	02b00693          	li	a3,43
    1612:	04d70a63          	beq	a4,a3,1666 <atoi+0x72>
    1616:	02d00693          	li	a3,45
    161a:	06d70463          	beq	a4,a3,1682 <atoi+0x8e>
        neg = 1;
    case '+':
        s++;
    }
    /* Compute n as a negative number to avoid overflow on INT_MIN */
    while (isdigit(*s))
    161e:	fd07859b          	addiw	a1,a5,-48
    1622:	4625                	li	a2,9
    1624:	873e                	mv	a4,a5
    1626:	86aa                	mv	a3,a0
    int n = 0, neg = 0;
    1628:	4e01                	li	t3,0
    while (isdigit(*s))
    162a:	04b66a63          	bltu	a2,a1,167e <atoi+0x8a>
    int n = 0, neg = 0;
    162e:	4501                	li	a0,0
    while (isdigit(*s))
    1630:	4825                	li	a6,9
    1632:	0016c603          	lbu	a2,1(a3)
        n = 10 * n - (*s++ - '0');
    1636:	0025179b          	slliw	a5,a0,0x2
    163a:	9d3d                	addw	a0,a0,a5
    163c:	fd07031b          	addiw	t1,a4,-48
    1640:	0015189b          	slliw	a7,a0,0x1
    while (isdigit(*s))
    1644:	fd06059b          	addiw	a1,a2,-48
        n = 10 * n - (*s++ - '0');
    1648:	0685                	addi	a3,a3,1
    164a:	4068853b          	subw	a0,a7,t1
    while (isdigit(*s))
    164e:	0006071b          	sext.w	a4,a2
    1652:	feb870e3          	bgeu	a6,a1,1632 <atoi+0x3e>
    return neg ? n : -n;
    1656:	000e0563          	beqz	t3,1660 <atoi+0x6c>
}
    165a:	8082                	ret
        s++;
    165c:	0505                	addi	a0,a0,1
    165e:	bf71                	j	15fa <atoi+0x6>
    return neg ? n : -n;
    1660:	4113053b          	subw	a0,t1,a7
    1664:	8082                	ret
    while (isdigit(*s))
    1666:	00154783          	lbu	a5,1(a0)
    166a:	4625                	li	a2,9
        s++;
    166c:	00150693          	addi	a3,a0,1
    while (isdigit(*s))
    1670:	fd07859b          	addiw	a1,a5,-48
    1674:	0007871b          	sext.w	a4,a5
    int n = 0, neg = 0;
    1678:	4e01                	li	t3,0
    while (isdigit(*s))
    167a:	fab67ae3          	bgeu	a2,a1,162e <atoi+0x3a>
    167e:	4501                	li	a0,0
}
    1680:	8082                	ret
    while (isdigit(*s))
    1682:	00154783          	lbu	a5,1(a0)
    1686:	4625                	li	a2,9
        s++;
    1688:	00150693          	addi	a3,a0,1
    while (isdigit(*s))
    168c:	fd07859b          	addiw	a1,a5,-48
    1690:	0007871b          	sext.w	a4,a5
    1694:	feb665e3          	bltu	a2,a1,167e <atoi+0x8a>
        neg = 1;
    1698:	4e05                	li	t3,1
    169a:	bf51                	j	162e <atoi+0x3a>

000000000000169c <memset>:

void *memset(void *dest, int c, size_t n)
{
    char *p = dest;
    for (int i = 0; i < n; ++i, *(p++) = c)
    169c:	16060d63          	beqz	a2,1816 <memset+0x17a>
    16a0:	40a007b3          	neg	a5,a0
    16a4:	8b9d                	andi	a5,a5,7
    16a6:	00778713          	addi	a4,a5,7
    16aa:	482d                	li	a6,11
    16ac:	0ff5f593          	andi	a1,a1,255
    16b0:	fff60693          	addi	a3,a2,-1
    16b4:	17076263          	bltu	a4,a6,1818 <memset+0x17c>
    16b8:	16e6ea63          	bltu	a3,a4,182c <memset+0x190>
    16bc:	16078563          	beqz	a5,1826 <memset+0x18a>
    16c0:	00b50023          	sb	a1,0(a0)
    16c4:	4705                	li	a4,1
    16c6:	00150e93          	addi	t4,a0,1
    16ca:	14e78c63          	beq	a5,a4,1822 <memset+0x186>
    16ce:	00b500a3          	sb	a1,1(a0)
    16d2:	4709                	li	a4,2
    16d4:	00250e93          	addi	t4,a0,2
    16d8:	14e78d63          	beq	a5,a4,1832 <memset+0x196>
    16dc:	00b50123          	sb	a1,2(a0)
    16e0:	470d                	li	a4,3
    16e2:	00350e93          	addi	t4,a0,3
    16e6:	12e78b63          	beq	a5,a4,181c <memset+0x180>
    16ea:	00b501a3          	sb	a1,3(a0)
    16ee:	4711                	li	a4,4
    16f0:	00450e93          	addi	t4,a0,4
    16f4:	14e78163          	beq	a5,a4,1836 <memset+0x19a>
    16f8:	00b50223          	sb	a1,4(a0)
    16fc:	4715                	li	a4,5
    16fe:	00550e93          	addi	t4,a0,5
    1702:	12e78c63          	beq	a5,a4,183a <memset+0x19e>
    1706:	00b502a3          	sb	a1,5(a0)
    170a:	471d                	li	a4,7
    170c:	00650e93          	addi	t4,a0,6
    1710:	12e79763          	bne	a5,a4,183e <memset+0x1a2>
    1714:	00750e93          	addi	t4,a0,7
    1718:	00b50323          	sb	a1,6(a0)
    171c:	4f1d                	li	t5,7
    171e:	00859713          	slli	a4,a1,0x8
    1722:	8f4d                	or	a4,a4,a1
    1724:	01059e13          	slli	t3,a1,0x10
    1728:	01c76e33          	or	t3,a4,t3
    172c:	01859313          	slli	t1,a1,0x18
    1730:	006e6333          	or	t1,t3,t1
    1734:	02059893          	slli	a7,a1,0x20
    1738:	011368b3          	or	a7,t1,a7
    173c:	02859813          	slli	a6,a1,0x28
    1740:	40f60333          	sub	t1,a2,a5
    1744:	0108e833          	or	a6,a7,a6
    1748:	03059693          	slli	a3,a1,0x30
    174c:	00d866b3          	or	a3,a6,a3
    1750:	03859713          	slli	a4,a1,0x38
    1754:	97aa                	add	a5,a5,a0
    1756:	ff837813          	andi	a6,t1,-8
    175a:	8f55                	or	a4,a4,a3
    175c:	00f806b3          	add	a3,a6,a5
    1760:	e398                	sd	a4,0(a5)
    1762:	07a1                	addi	a5,a5,8
    1764:	fed79ee3          	bne	a5,a3,1760 <memset+0xc4>
    1768:	ff837693          	andi	a3,t1,-8
    176c:	00de87b3          	add	a5,t4,a3
    1770:	01e6873b          	addw	a4,a3,t5
    1774:	0ad30663          	beq	t1,a3,1820 <memset+0x184>
    1778:	00b78023          	sb	a1,0(a5)
    177c:	0017069b          	addiw	a3,a4,1
    1780:	08c6fb63          	bgeu	a3,a2,1816 <memset+0x17a>
    1784:	00b780a3          	sb	a1,1(a5)
    1788:	0027069b          	addiw	a3,a4,2
    178c:	08c6f563          	bgeu	a3,a2,1816 <memset+0x17a>
    1790:	00b78123          	sb	a1,2(a5)
    1794:	0037069b          	addiw	a3,a4,3
    1798:	06c6ff63          	bgeu	a3,a2,1816 <memset+0x17a>
    179c:	00b781a3          	sb	a1,3(a5)
    17a0:	0047069b          	addiw	a3,a4,4
    17a4:	06c6f963          	bgeu	a3,a2,1816 <memset+0x17a>
    17a8:	00b78223          	sb	a1,4(a5)
    17ac:	0057069b          	addiw	a3,a4,5
    17b0:	06c6f363          	bgeu	a3,a2,1816 <memset+0x17a>
    17b4:	00b782a3          	sb	a1,5(a5)
    17b8:	0067069b          	addiw	a3,a4,6
    17bc:	04c6fd63          	bgeu	a3,a2,1816 <memset+0x17a>
    17c0:	00b78323          	sb	a1,6(a5)
    17c4:	0077069b          	addiw	a3,a4,7
    17c8:	04c6f763          	bgeu	a3,a2,1816 <memset+0x17a>
    17cc:	00b783a3          	sb	a1,7(a5)
    17d0:	0087069b          	addiw	a3,a4,8
    17d4:	04c6f163          	bgeu	a3,a2,1816 <memset+0x17a>
    17d8:	00b78423          	sb	a1,8(a5)
    17dc:	0097069b          	addiw	a3,a4,9
    17e0:	02c6fb63          	bgeu	a3,a2,1816 <memset+0x17a>
    17e4:	00b784a3          	sb	a1,9(a5)
    17e8:	00a7069b          	addiw	a3,a4,10
    17ec:	02c6f563          	bgeu	a3,a2,1816 <memset+0x17a>
    17f0:	00b78523          	sb	a1,10(a5)
    17f4:	00b7069b          	addiw	a3,a4,11
    17f8:	00c6ff63          	bgeu	a3,a2,1816 <memset+0x17a>
    17fc:	00b785a3          	sb	a1,11(a5)
    1800:	00c7069b          	addiw	a3,a4,12
    1804:	00c6f963          	bgeu	a3,a2,1816 <memset+0x17a>
    1808:	00b78623          	sb	a1,12(a5)
    180c:	2735                	addiw	a4,a4,13
    180e:	00c77463          	bgeu	a4,a2,1816 <memset+0x17a>
    1812:	00b786a3          	sb	a1,13(a5)
        ;
    return dest;
}
    1816:	8082                	ret
    1818:	472d                	li	a4,11
    181a:	bd79                	j	16b8 <memset+0x1c>
    for (int i = 0; i < n; ++i, *(p++) = c)
    181c:	4f0d                	li	t5,3
    181e:	b701                	j	171e <memset+0x82>
    1820:	8082                	ret
    1822:	4f05                	li	t5,1
    1824:	bded                	j	171e <memset+0x82>
    1826:	8eaa                	mv	t4,a0
    1828:	4f01                	li	t5,0
    182a:	bdd5                	j	171e <memset+0x82>
    182c:	87aa                	mv	a5,a0
    182e:	4701                	li	a4,0
    1830:	b7a1                	j	1778 <memset+0xdc>
    1832:	4f09                	li	t5,2
    1834:	b5ed                	j	171e <memset+0x82>
    1836:	4f11                	li	t5,4
    1838:	b5dd                	j	171e <memset+0x82>
    183a:	4f15                	li	t5,5
    183c:	b5cd                	j	171e <memset+0x82>
    183e:	4f19                	li	t5,6
    1840:	bdf9                	j	171e <memset+0x82>

0000000000001842 <strcmp>:

int strcmp(const char *l, const char *r)
{
    for (; *l == *r && *l; l++, r++)
    1842:	00054783          	lbu	a5,0(a0)
    1846:	0005c703          	lbu	a4,0(a1)
    184a:	00e79863          	bne	a5,a4,185a <strcmp+0x18>
    184e:	0505                	addi	a0,a0,1
    1850:	0585                	addi	a1,a1,1
    1852:	fbe5                	bnez	a5,1842 <strcmp>
    1854:	4501                	li	a0,0
        ;
    return *(unsigned char *)l - *(unsigned char *)r;
}
    1856:	9d19                	subw	a0,a0,a4
    1858:	8082                	ret
    return *(unsigned char *)l - *(unsigned char *)r;
    185a:	0007851b          	sext.w	a0,a5
    185e:	bfe5                	j	1856 <strcmp+0x14>

0000000000001860 <strncmp>:

int strncmp(const char *_l, const char *_r, size_t n)
{
    const unsigned char *l = (void *)_l, *r = (void *)_r;
    if (!n--)
    1860:	ce05                	beqz	a2,1898 <strncmp+0x38>
        return 0;
    for (; *l && *r && n && *l == *r; l++, r++, n--)
    1862:	00054703          	lbu	a4,0(a0)
    1866:	0005c783          	lbu	a5,0(a1)
    186a:	cb0d                	beqz	a4,189c <strncmp+0x3c>
    if (!n--)
    186c:	167d                	addi	a2,a2,-1
    186e:	00c506b3          	add	a3,a0,a2
    1872:	a819                	j	1888 <strncmp+0x28>
    for (; *l && *r && n && *l == *r; l++, r++, n--)
    1874:	00a68e63          	beq	a3,a0,1890 <strncmp+0x30>
    1878:	0505                	addi	a0,a0,1
    187a:	00e79b63          	bne	a5,a4,1890 <strncmp+0x30>
    187e:	00054703          	lbu	a4,0(a0)
        ;
    return *l - *r;
    1882:	0005c783          	lbu	a5,0(a1)
    for (; *l && *r && n && *l == *r; l++, r++, n--)
    1886:	cb19                	beqz	a4,189c <strncmp+0x3c>
    1888:	0005c783          	lbu	a5,0(a1)
    188c:	0585                	addi	a1,a1,1
    188e:	f3fd                	bnez	a5,1874 <strncmp+0x14>
    return *l - *r;
    1890:	0007051b          	sext.w	a0,a4
    1894:	9d1d                	subw	a0,a0,a5
    1896:	8082                	ret
        return 0;
    1898:	4501                	li	a0,0
}
    189a:	8082                	ret
    189c:	4501                	li	a0,0
    return *l - *r;
    189e:	9d1d                	subw	a0,a0,a5
    18a0:	8082                	ret

00000000000018a2 <strlen>:
size_t strlen(const char *s)
{
    const char *a = s;
    typedef size_t __attribute__((__may_alias__)) word;
    const word *w;
    for (; (uintptr_t)s % SS; s++)
    18a2:	00757793          	andi	a5,a0,7
    18a6:	cf89                	beqz	a5,18c0 <strlen+0x1e>
    18a8:	87aa                	mv	a5,a0
    18aa:	a029                	j	18b4 <strlen+0x12>
    18ac:	0785                	addi	a5,a5,1
    18ae:	0077f713          	andi	a4,a5,7
    18b2:	cb01                	beqz	a4,18c2 <strlen+0x20>
        if (!*s)
    18b4:	0007c703          	lbu	a4,0(a5)
    18b8:	fb75                	bnez	a4,18ac <strlen+0xa>
    for (w = (const void *)s; !HASZERO(*w); w++)
        ;
    s = (const void *)w;
    for (; *s; s++)
        ;
    return s - a;
    18ba:	40a78533          	sub	a0,a5,a0
}
    18be:	8082                	ret
    for (; (uintptr_t)s % SS; s++)
    18c0:	87aa                	mv	a5,a0
    for (w = (const void *)s; !HASZERO(*w); w++)
    18c2:	6394                	ld	a3,0(a5)
    18c4:	00000597          	auipc	a1,0x0
    18c8:	6945b583          	ld	a1,1684(a1) # 1f58 <__clone+0x9e>
    18cc:	00000617          	auipc	a2,0x0
    18d0:	69463603          	ld	a2,1684(a2) # 1f60 <__clone+0xa6>
    18d4:	a019                	j	18da <strlen+0x38>
    18d6:	6794                	ld	a3,8(a5)
    18d8:	07a1                	addi	a5,a5,8
    18da:	00b68733          	add	a4,a3,a1
    18de:	fff6c693          	not	a3,a3
    18e2:	8f75                	and	a4,a4,a3
    18e4:	8f71                	and	a4,a4,a2
    18e6:	db65                	beqz	a4,18d6 <strlen+0x34>
    for (; *s; s++)
    18e8:	0007c703          	lbu	a4,0(a5)
    18ec:	d779                	beqz	a4,18ba <strlen+0x18>
    18ee:	0017c703          	lbu	a4,1(a5)
    18f2:	0785                	addi	a5,a5,1
    18f4:	d379                	beqz	a4,18ba <strlen+0x18>
    18f6:	0017c703          	lbu	a4,1(a5)
    18fa:	0785                	addi	a5,a5,1
    18fc:	fb6d                	bnez	a4,18ee <strlen+0x4c>
    18fe:	bf75                	j	18ba <strlen+0x18>

0000000000001900 <memchr>:

void *memchr(const void *src, int c, size_t n)
{
    const unsigned char *s = src;
    c = (unsigned char)c;
    for (; ((uintptr_t)s & ALIGN) && n && *s != c; s++, n--)
    1900:	00757713          	andi	a4,a0,7
{
    1904:	87aa                	mv	a5,a0
    c = (unsigned char)c;
    1906:	0ff5f593          	andi	a1,a1,255
    for (; ((uintptr_t)s & ALIGN) && n && *s != c; s++, n--)
    190a:	cb19                	beqz	a4,1920 <memchr+0x20>
    190c:	ce25                	beqz	a2,1984 <memchr+0x84>
    190e:	0007c703          	lbu	a4,0(a5)
    1912:	04b70e63          	beq	a4,a1,196e <memchr+0x6e>
    1916:	0785                	addi	a5,a5,1
    1918:	0077f713          	andi	a4,a5,7
    191c:	167d                	addi	a2,a2,-1
    191e:	f77d                	bnez	a4,190c <memchr+0xc>
            ;
        s = (const void *)w;
    }
    for (; n && *s != c; s++, n--)
        ;
    return n ? (void *)s : 0;
    1920:	4501                	li	a0,0
    if (n && *s != c)
    1922:	c235                	beqz	a2,1986 <memchr+0x86>
    1924:	0007c703          	lbu	a4,0(a5)
    1928:	04b70363          	beq	a4,a1,196e <memchr+0x6e>
        size_t k = ONES * c;
    192c:	00000517          	auipc	a0,0x0
    1930:	63c53503          	ld	a0,1596(a0) # 1f68 <__clone+0xae>
        for (w = (const void *)s; n >= SS && !HASZERO(*w ^ k); w++, n -= SS)
    1934:	471d                	li	a4,7
        size_t k = ONES * c;
    1936:	02a58533          	mul	a0,a1,a0
        for (w = (const void *)s; n >= SS && !HASZERO(*w ^ k); w++, n -= SS)
    193a:	02c77a63          	bgeu	a4,a2,196e <memchr+0x6e>
    193e:	00000897          	auipc	a7,0x0
    1942:	61a8b883          	ld	a7,1562(a7) # 1f58 <__clone+0x9e>
    1946:	00000817          	auipc	a6,0x0
    194a:	61a83803          	ld	a6,1562(a6) # 1f60 <__clone+0xa6>
    194e:	431d                	li	t1,7
    1950:	a029                	j	195a <memchr+0x5a>
    1952:	1661                	addi	a2,a2,-8
    1954:	07a1                	addi	a5,a5,8
    1956:	02c37963          	bgeu	t1,a2,1988 <memchr+0x88>
    195a:	6398                	ld	a4,0(a5)
    195c:	8f29                	xor	a4,a4,a0
    195e:	011706b3          	add	a3,a4,a7
    1962:	fff74713          	not	a4,a4
    1966:	8f75                	and	a4,a4,a3
    1968:	01077733          	and	a4,a4,a6
    196c:	d37d                	beqz	a4,1952 <memchr+0x52>
    196e:	853e                	mv	a0,a5
    1970:	97b2                	add	a5,a5,a2
    1972:	a021                	j	197a <memchr+0x7a>
    for (; n && *s != c; s++, n--)
    1974:	0505                	addi	a0,a0,1
    1976:	00f50763          	beq	a0,a5,1984 <memchr+0x84>
    197a:	00054703          	lbu	a4,0(a0)
    197e:	feb71be3          	bne	a4,a1,1974 <memchr+0x74>
    1982:	8082                	ret
    return n ? (void *)s : 0;
    1984:	4501                	li	a0,0
}
    1986:	8082                	ret
    return n ? (void *)s : 0;
    1988:	4501                	li	a0,0
    for (; n && *s != c; s++, n--)
    198a:	f275                	bnez	a2,196e <memchr+0x6e>
}
    198c:	8082                	ret

000000000000198e <strnlen>:

size_t strnlen(const char *s, size_t n)
{
    198e:	1101                	addi	sp,sp,-32
    1990:	e822                	sd	s0,16(sp)
    const char *p = memchr(s, 0, n);
    1992:	862e                	mv	a2,a1
{
    1994:	842e                	mv	s0,a1
    const char *p = memchr(s, 0, n);
    1996:	4581                	li	a1,0
{
    1998:	e426                	sd	s1,8(sp)
    199a:	ec06                	sd	ra,24(sp)
    199c:	84aa                	mv	s1,a0
    const char *p = memchr(s, 0, n);
    199e:	f63ff0ef          	jal	ra,1900 <memchr>
    return p ? p - s : n;
    19a2:	c519                	beqz	a0,19b0 <strnlen+0x22>
}
    19a4:	60e2                	ld	ra,24(sp)
    19a6:	6442                	ld	s0,16(sp)
    return p ? p - s : n;
    19a8:	8d05                	sub	a0,a0,s1
}
    19aa:	64a2                	ld	s1,8(sp)
    19ac:	6105                	addi	sp,sp,32
    19ae:	8082                	ret
    19b0:	60e2                	ld	ra,24(sp)
    return p ? p - s : n;
    19b2:	8522                	mv	a0,s0
}
    19b4:	6442                	ld	s0,16(sp)
    19b6:	64a2                	ld	s1,8(sp)
    19b8:	6105                	addi	sp,sp,32
    19ba:	8082                	ret

00000000000019bc <strcpy>:
char *strcpy(char *restrict d, const char *s)
{
    typedef size_t __attribute__((__may_alias__)) word;
    word *wd;
    const word *ws;
    if ((uintptr_t)s % SS == (uintptr_t)d % SS)
    19bc:	00b547b3          	xor	a5,a0,a1
    19c0:	8b9d                	andi	a5,a5,7
    19c2:	eb95                	bnez	a5,19f6 <strcpy+0x3a>
    {
        for (; (uintptr_t)s % SS; s++, d++)
    19c4:	0075f793          	andi	a5,a1,7
    19c8:	e7b1                	bnez	a5,1a14 <strcpy+0x58>
            if (!(*d = *s))
                return d;
        wd = (void *)d;
        ws = (const void *)s;
        for (; !HASZERO(*ws); *wd++ = *ws++)
    19ca:	6198                	ld	a4,0(a1)
    19cc:	00000617          	auipc	a2,0x0
    19d0:	58c63603          	ld	a2,1420(a2) # 1f58 <__clone+0x9e>
    19d4:	00000817          	auipc	a6,0x0
    19d8:	58c83803          	ld	a6,1420(a6) # 1f60 <__clone+0xa6>
    19dc:	a029                	j	19e6 <strcpy+0x2a>
    19de:	e118                	sd	a4,0(a0)
    19e0:	6598                	ld	a4,8(a1)
    19e2:	05a1                	addi	a1,a1,8
    19e4:	0521                	addi	a0,a0,8
    19e6:	00c707b3          	add	a5,a4,a2
    19ea:	fff74693          	not	a3,a4
    19ee:	8ff5                	and	a5,a5,a3
    19f0:	0107f7b3          	and	a5,a5,a6
    19f4:	d7ed                	beqz	a5,19de <strcpy+0x22>
            ;
        d = (void *)wd;
        s = (const void *)ws;
    }
    for (; (*d = *s); s++, d++)
    19f6:	0005c783          	lbu	a5,0(a1)
    19fa:	00f50023          	sb	a5,0(a0)
    19fe:	c785                	beqz	a5,1a26 <strcpy+0x6a>
    1a00:	0015c783          	lbu	a5,1(a1)
    1a04:	0505                	addi	a0,a0,1
    1a06:	0585                	addi	a1,a1,1
    1a08:	00f50023          	sb	a5,0(a0)
    1a0c:	fbf5                	bnez	a5,1a00 <strcpy+0x44>
        ;
    return d;
}
    1a0e:	8082                	ret
        for (; (uintptr_t)s % SS; s++, d++)
    1a10:	0505                	addi	a0,a0,1
    1a12:	df45                	beqz	a4,19ca <strcpy+0xe>
            if (!(*d = *s))
    1a14:	0005c783          	lbu	a5,0(a1)
        for (; (uintptr_t)s % SS; s++, d++)
    1a18:	0585                	addi	a1,a1,1
    1a1a:	0075f713          	andi	a4,a1,7
            if (!(*d = *s))
    1a1e:	00f50023          	sb	a5,0(a0)
    1a22:	f7fd                	bnez	a5,1a10 <strcpy+0x54>
}
    1a24:	8082                	ret
    1a26:	8082                	ret

0000000000001a28 <strncpy>:
char *strncpy(char *restrict d, const char *s, size_t n)
{
    typedef size_t __attribute__((__may_alias__)) word;
    word *wd;
    const word *ws;
    if (((uintptr_t)s & ALIGN) == ((uintptr_t)d & ALIGN))
    1a28:	00b547b3          	xor	a5,a0,a1
    1a2c:	8b9d                	andi	a5,a5,7
    1a2e:	1a079863          	bnez	a5,1bde <strncpy+0x1b6>
    {
        for (; ((uintptr_t)s & ALIGN) && n && (*d = *s); n--, s++, d++)
    1a32:	0075f793          	andi	a5,a1,7
    1a36:	16078463          	beqz	a5,1b9e <strncpy+0x176>
    1a3a:	ea01                	bnez	a2,1a4a <strncpy+0x22>
    1a3c:	a421                	j	1c44 <strncpy+0x21c>
    1a3e:	167d                	addi	a2,a2,-1
    1a40:	0505                	addi	a0,a0,1
    1a42:	14070e63          	beqz	a4,1b9e <strncpy+0x176>
    1a46:	1a060863          	beqz	a2,1bf6 <strncpy+0x1ce>
    1a4a:	0005c783          	lbu	a5,0(a1)
    1a4e:	0585                	addi	a1,a1,1
    1a50:	0075f713          	andi	a4,a1,7
    1a54:	00f50023          	sb	a5,0(a0)
    1a58:	f3fd                	bnez	a5,1a3e <strncpy+0x16>
    1a5a:	4805                	li	a6,1
    1a5c:	1a061863          	bnez	a2,1c0c <strncpy+0x1e4>
    1a60:	40a007b3          	neg	a5,a0
    1a64:	8b9d                	andi	a5,a5,7
    1a66:	4681                	li	a3,0
    1a68:	18061a63          	bnez	a2,1bfc <strncpy+0x1d4>
    1a6c:	00778713          	addi	a4,a5,7
    1a70:	45ad                	li	a1,11
    1a72:	18b76363          	bltu	a4,a1,1bf8 <strncpy+0x1d0>
    1a76:	1ae6eb63          	bltu	a3,a4,1c2c <strncpy+0x204>
    1a7a:	1a078363          	beqz	a5,1c20 <strncpy+0x1f8>
    for (int i = 0; i < n; ++i, *(p++) = c)
    1a7e:	00050023          	sb	zero,0(a0)
    1a82:	4685                	li	a3,1
    1a84:	00150713          	addi	a4,a0,1
    1a88:	18d78f63          	beq	a5,a3,1c26 <strncpy+0x1fe>
    1a8c:	000500a3          	sb	zero,1(a0)
    1a90:	4689                	li	a3,2
    1a92:	00250713          	addi	a4,a0,2
    1a96:	18d78e63          	beq	a5,a3,1c32 <strncpy+0x20a>
    1a9a:	00050123          	sb	zero,2(a0)
    1a9e:	468d                	li	a3,3
    1aa0:	00350713          	addi	a4,a0,3
    1aa4:	16d78c63          	beq	a5,a3,1c1c <strncpy+0x1f4>
    1aa8:	000501a3          	sb	zero,3(a0)
    1aac:	4691                	li	a3,4
    1aae:	00450713          	addi	a4,a0,4
    1ab2:	18d78263          	beq	a5,a3,1c36 <strncpy+0x20e>
    1ab6:	00050223          	sb	zero,4(a0)
    1aba:	4695                	li	a3,5
    1abc:	00550713          	addi	a4,a0,5
    1ac0:	16d78d63          	beq	a5,a3,1c3a <strncpy+0x212>
    1ac4:	000502a3          	sb	zero,5(a0)
    1ac8:	469d                	li	a3,7
    1aca:	00650713          	addi	a4,a0,6
    1ace:	16d79863          	bne	a5,a3,1c3e <strncpy+0x216>
    1ad2:	00750713          	addi	a4,a0,7
    1ad6:	00050323          	sb	zero,6(a0)
    1ada:	40f80833          	sub	a6,a6,a5
    1ade:	ff887593          	andi	a1,a6,-8
    1ae2:	97aa                	add	a5,a5,a0
    1ae4:	95be                	add	a1,a1,a5
    1ae6:	0007b023          	sd	zero,0(a5)
    1aea:	07a1                	addi	a5,a5,8
    1aec:	feb79de3          	bne	a5,a1,1ae6 <strncpy+0xbe>
    1af0:	ff887593          	andi	a1,a6,-8
    1af4:	9ead                	addw	a3,a3,a1
    1af6:	00b707b3          	add	a5,a4,a1
    1afa:	12b80863          	beq	a6,a1,1c2a <strncpy+0x202>
    1afe:	00078023          	sb	zero,0(a5)
    1b02:	0016871b          	addiw	a4,a3,1
    1b06:	0ec77863          	bgeu	a4,a2,1bf6 <strncpy+0x1ce>
    1b0a:	000780a3          	sb	zero,1(a5)
    1b0e:	0026871b          	addiw	a4,a3,2
    1b12:	0ec77263          	bgeu	a4,a2,1bf6 <strncpy+0x1ce>
    1b16:	00078123          	sb	zero,2(a5)
    1b1a:	0036871b          	addiw	a4,a3,3
    1b1e:	0cc77c63          	bgeu	a4,a2,1bf6 <strncpy+0x1ce>
    1b22:	000781a3          	sb	zero,3(a5)
    1b26:	0046871b          	addiw	a4,a3,4
    1b2a:	0cc77663          	bgeu	a4,a2,1bf6 <strncpy+0x1ce>
    1b2e:	00078223          	sb	zero,4(a5)
    1b32:	0056871b          	addiw	a4,a3,5
    1b36:	0cc77063          	bgeu	a4,a2,1bf6 <strncpy+0x1ce>
    1b3a:	000782a3          	sb	zero,5(a5)
    1b3e:	0066871b          	addiw	a4,a3,6
    1b42:	0ac77a63          	bgeu	a4,a2,1bf6 <strncpy+0x1ce>
    1b46:	00078323          	sb	zero,6(a5)
    1b4a:	0076871b          	addiw	a4,a3,7
    1b4e:	0ac77463          	bgeu	a4,a2,1bf6 <strncpy+0x1ce>
    1b52:	000783a3          	sb	zero,7(a5)
    1b56:	0086871b          	addiw	a4,a3,8
    1b5a:	08c77e63          	bgeu	a4,a2,1bf6 <strncpy+0x1ce>
    1b5e:	00078423          	sb	zero,8(a5)
    1b62:	0096871b          	addiw	a4,a3,9
    1b66:	08c77863          	bgeu	a4,a2,1bf6 <strncpy+0x1ce>
    1b6a:	000784a3          	sb	zero,9(a5)
    1b6e:	00a6871b          	addiw	a4,a3,10
    1b72:	08c77263          	bgeu	a4,a2,1bf6 <strncpy+0x1ce>
    1b76:	00078523          	sb	zero,10(a5)
    1b7a:	00b6871b          	addiw	a4,a3,11
    1b7e:	06c77c63          	bgeu	a4,a2,1bf6 <strncpy+0x1ce>
    1b82:	000785a3          	sb	zero,11(a5)
    1b86:	00c6871b          	addiw	a4,a3,12
    1b8a:	06c77663          	bgeu	a4,a2,1bf6 <strncpy+0x1ce>
    1b8e:	00078623          	sb	zero,12(a5)
    1b92:	26b5                	addiw	a3,a3,13
    1b94:	06c6f163          	bgeu	a3,a2,1bf6 <strncpy+0x1ce>
    1b98:	000786a3          	sb	zero,13(a5)
    1b9c:	8082                	ret
            ;
        if (!n || !*s)
    1b9e:	c645                	beqz	a2,1c46 <strncpy+0x21e>
    1ba0:	0005c783          	lbu	a5,0(a1)
    1ba4:	ea078be3          	beqz	a5,1a5a <strncpy+0x32>
            goto tail;
        wd = (void *)d;
        ws = (const void *)s;
        for (; n >= sizeof(size_t) && !HASZERO(*ws); n -= sizeof(size_t), ws++, wd++)
    1ba8:	479d                	li	a5,7
    1baa:	02c7ff63          	bgeu	a5,a2,1be8 <strncpy+0x1c0>
    1bae:	00000897          	auipc	a7,0x0
    1bb2:	3aa8b883          	ld	a7,938(a7) # 1f58 <__clone+0x9e>
    1bb6:	00000817          	auipc	a6,0x0
    1bba:	3aa83803          	ld	a6,938(a6) # 1f60 <__clone+0xa6>
    1bbe:	431d                	li	t1,7
    1bc0:	6198                	ld	a4,0(a1)
    1bc2:	011707b3          	add	a5,a4,a7
    1bc6:	fff74693          	not	a3,a4
    1bca:	8ff5                	and	a5,a5,a3
    1bcc:	0107f7b3          	and	a5,a5,a6
    1bd0:	ef81                	bnez	a5,1be8 <strncpy+0x1c0>
            *wd = *ws;
    1bd2:	e118                	sd	a4,0(a0)
        for (; n >= sizeof(size_t) && !HASZERO(*ws); n -= sizeof(size_t), ws++, wd++)
    1bd4:	1661                	addi	a2,a2,-8
    1bd6:	05a1                	addi	a1,a1,8
    1bd8:	0521                	addi	a0,a0,8
    1bda:	fec363e3          	bltu	t1,a2,1bc0 <strncpy+0x198>
        d = (void *)wd;
        s = (const void *)ws;
    }
    for (; n && (*d = *s); n--, s++, d++)
    1bde:	e609                	bnez	a2,1be8 <strncpy+0x1c0>
    1be0:	a08d                	j	1c42 <strncpy+0x21a>
    1be2:	167d                	addi	a2,a2,-1
    1be4:	0505                	addi	a0,a0,1
    1be6:	ca01                	beqz	a2,1bf6 <strncpy+0x1ce>
    1be8:	0005c783          	lbu	a5,0(a1)
    1bec:	0585                	addi	a1,a1,1
    1bee:	00f50023          	sb	a5,0(a0)
    1bf2:	fbe5                	bnez	a5,1be2 <strncpy+0x1ba>
        ;
tail:
    1bf4:	b59d                	j	1a5a <strncpy+0x32>
    memset(d, 0, n);
    return d;
}
    1bf6:	8082                	ret
    1bf8:	472d                	li	a4,11
    1bfa:	bdb5                	j	1a76 <strncpy+0x4e>
    1bfc:	00778713          	addi	a4,a5,7
    1c00:	45ad                	li	a1,11
    1c02:	fff60693          	addi	a3,a2,-1
    1c06:	e6b778e3          	bgeu	a4,a1,1a76 <strncpy+0x4e>
    1c0a:	b7fd                	j	1bf8 <strncpy+0x1d0>
    1c0c:	40a007b3          	neg	a5,a0
    1c10:	8832                	mv	a6,a2
    1c12:	8b9d                	andi	a5,a5,7
    1c14:	4681                	li	a3,0
    1c16:	e4060be3          	beqz	a2,1a6c <strncpy+0x44>
    1c1a:	b7cd                	j	1bfc <strncpy+0x1d4>
    for (int i = 0; i < n; ++i, *(p++) = c)
    1c1c:	468d                	li	a3,3
    1c1e:	bd75                	j	1ada <strncpy+0xb2>
    1c20:	872a                	mv	a4,a0
    1c22:	4681                	li	a3,0
    1c24:	bd5d                	j	1ada <strncpy+0xb2>
    1c26:	4685                	li	a3,1
    1c28:	bd4d                	j	1ada <strncpy+0xb2>
    1c2a:	8082                	ret
    1c2c:	87aa                	mv	a5,a0
    1c2e:	4681                	li	a3,0
    1c30:	b5f9                	j	1afe <strncpy+0xd6>
    1c32:	4689                	li	a3,2
    1c34:	b55d                	j	1ada <strncpy+0xb2>
    1c36:	4691                	li	a3,4
    1c38:	b54d                	j	1ada <strncpy+0xb2>
    1c3a:	4695                	li	a3,5
    1c3c:	bd79                	j	1ada <strncpy+0xb2>
    1c3e:	4699                	li	a3,6
    1c40:	bd69                	j	1ada <strncpy+0xb2>
    1c42:	8082                	ret
    1c44:	8082                	ret
    1c46:	8082                	ret

0000000000001c48 <open>:
#include <unistd.h>

#include "syscall.h"

int open(const char *path, int flags)
{
    1c48:	87aa                	mv	a5,a0
    1c4a:	862e                	mv	a2,a1
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
}

static inline long __syscall4(long n, long a, long b, long c, long d)
{
    register long a7 __asm__("a7") = n;
    1c4c:	03800893          	li	a7,56
    register long a0 __asm__("a0") = a;
    1c50:	f9c00513          	li	a0,-100
    register long a1 __asm__("a1") = b;
    1c54:	85be                	mv	a1,a5
    register long a2 __asm__("a2") = c;
    register long a3 __asm__("a3") = d;
    1c56:	4689                	li	a3,2
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3))
    1c58:	00000073          	ecall
    return syscall(SYS_openat, AT_FDCWD, path, flags, O_RDWR);
}
    1c5c:	2501                	sext.w	a0,a0
    1c5e:	8082                	ret

0000000000001c60 <openat>:
    register long a7 __asm__("a7") = n;
    1c60:	03800893          	li	a7,56
    register long a3 __asm__("a3") = d;
    1c64:	18000693          	li	a3,384
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3))
    1c68:	00000073          	ecall

int openat(int dirfd,const char *path, int flags)
{
    return syscall(SYS_openat, dirfd, path, flags, 0600);
}
    1c6c:	2501                	sext.w	a0,a0
    1c6e:	8082                	ret

0000000000001c70 <close>:
    register long a7 __asm__("a7") = n;
    1c70:	03900893          	li	a7,57
    __asm_syscall("r"(a7), "0"(a0))
    1c74:	00000073          	ecall

int close(int fd)
{
    return syscall(SYS_close, fd);
}
    1c78:	2501                	sext.w	a0,a0
    1c7a:	8082                	ret

0000000000001c7c <read>:
    register long a7 __asm__("a7") = n;
    1c7c:	03f00893          	li	a7,63
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1c80:	00000073          	ecall

ssize_t read(int fd, void *buf, size_t len)
{
    return syscall(SYS_read, fd, buf, len);
}
    1c84:	8082                	ret

0000000000001c86 <write>:
    register long a7 __asm__("a7") = n;
    1c86:	04000893          	li	a7,64
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1c8a:	00000073          	ecall

ssize_t write(int fd, const void *buf, size_t len)
{
    return syscall(SYS_write, fd, buf, len);
}
    1c8e:	8082                	ret

0000000000001c90 <getpid>:
    register long a7 __asm__("a7") = n;
    1c90:	0ac00893          	li	a7,172
    __asm_syscall("r"(a7))
    1c94:	00000073          	ecall

pid_t getpid(void)
{
    return syscall(SYS_getpid);
}
    1c98:	2501                	sext.w	a0,a0
    1c9a:	8082                	ret

0000000000001c9c <getppid>:
    register long a7 __asm__("a7") = n;
    1c9c:	0ad00893          	li	a7,173
    __asm_syscall("r"(a7))
    1ca0:	00000073          	ecall

pid_t getppid(void)
{
    return syscall(SYS_getppid);
}
    1ca4:	2501                	sext.w	a0,a0
    1ca6:	8082                	ret

0000000000001ca8 <sched_yield>:
    register long a7 __asm__("a7") = n;
    1ca8:	07c00893          	li	a7,124
    __asm_syscall("r"(a7))
    1cac:	00000073          	ecall

int sched_yield(void)
{
    return syscall(SYS_sched_yield);
}
    1cb0:	2501                	sext.w	a0,a0
    1cb2:	8082                	ret

0000000000001cb4 <fork>:
    register long a7 __asm__("a7") = n;
    1cb4:	0dc00893          	li	a7,220
    register long a0 __asm__("a0") = a;
    1cb8:	4545                	li	a0,17
    register long a1 __asm__("a1") = b;
    1cba:	4581                	li	a1,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1cbc:	00000073          	ecall

pid_t fork(void)
{
    return syscall(SYS_clone, SIGCHLD, 0);
}
    1cc0:	2501                	sext.w	a0,a0
    1cc2:	8082                	ret

0000000000001cc4 <clone>:

pid_t clone(int (*fn)(void *arg), void *arg, void *stack, size_t stack_size, unsigned long flags)
{
    1cc4:	85b2                	mv	a1,a2
    1cc6:	863a                	mv	a2,a4
    if (stack)
    1cc8:	c191                	beqz	a1,1ccc <clone+0x8>
	stack += stack_size;
    1cca:	95b6                	add	a1,a1,a3

    return __clone(fn, stack, flags, NULL, NULL, NULL);
    1ccc:	4781                	li	a5,0
    1cce:	4701                	li	a4,0
    1cd0:	4681                	li	a3,0
    1cd2:	2601                	sext.w	a2,a2
    1cd4:	a2dd                	j	1eba <__clone>

0000000000001cd6 <exit>:
    register long a7 __asm__("a7") = n;
    1cd6:	05d00893          	li	a7,93
    __asm_syscall("r"(a7), "0"(a0))
    1cda:	00000073          	ecall
    //return syscall(SYS_clone, fn, stack, flags, NULL, NULL, NULL);
}
void exit(int code)
{
    syscall(SYS_exit, code);
}
    1cde:	8082                	ret

0000000000001ce0 <waitpid>:
    register long a7 __asm__("a7") = n;
    1ce0:	10400893          	li	a7,260
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1ce4:	00000073          	ecall

int waitpid(int pid, int *code, int options)
{
    return syscall(SYS_wait4, pid, code, options);
}
    1ce8:	2501                	sext.w	a0,a0
    1cea:	8082                	ret

0000000000001cec <exec>:
    register long a7 __asm__("a7") = n;
    1cec:	0dd00893          	li	a7,221
    __asm_syscall("r"(a7), "0"(a0))
    1cf0:	00000073          	ecall

int exec(char *name)
{
    return syscall(SYS_execve, name);
}
    1cf4:	2501                	sext.w	a0,a0
    1cf6:	8082                	ret

0000000000001cf8 <execve>:
    register long a7 __asm__("a7") = n;
    1cf8:	0dd00893          	li	a7,221
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1cfc:	00000073          	ecall

int execve(const char *name, char *const argv[], char *const argp[])
{
    return syscall(SYS_execve, name, argv, argp);
}
    1d00:	2501                	sext.w	a0,a0
    1d02:	8082                	ret

0000000000001d04 <times>:
    register long a7 __asm__("a7") = n;
    1d04:	09900893          	li	a7,153
    __asm_syscall("r"(a7), "0"(a0))
    1d08:	00000073          	ecall

int times(void *mytimes)
{
	return syscall(SYS_times, mytimes);
}
    1d0c:	2501                	sext.w	a0,a0
    1d0e:	8082                	ret

0000000000001d10 <get_time>:

int64 get_time()
{
    1d10:	1141                	addi	sp,sp,-16
    register long a7 __asm__("a7") = n;
    1d12:	0a900893          	li	a7,169
    register long a0 __asm__("a0") = a;
    1d16:	850a                	mv	a0,sp
    register long a1 __asm__("a1") = b;
    1d18:	4581                	li	a1,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1d1a:	00000073          	ecall
    TimeVal time;
    int err = sys_get_time(&time, 0);
    if (err == 0)
    1d1e:	2501                	sext.w	a0,a0
    1d20:	ed09                	bnez	a0,1d3a <get_time+0x2a>
    {
        return ((time.sec & 0xffff) * 1000 + time.usec / 1000);
    1d22:	67a2                	ld	a5,8(sp)
    1d24:	3e800713          	li	a4,1000
    1d28:	00015503          	lhu	a0,0(sp)
    1d2c:	02e7d7b3          	divu	a5,a5,a4
    1d30:	02e50533          	mul	a0,a0,a4
    1d34:	953e                	add	a0,a0,a5
    }
    else
    {
        return -1;
    }
}
    1d36:	0141                	addi	sp,sp,16
    1d38:	8082                	ret
        return -1;
    1d3a:	557d                	li	a0,-1
    1d3c:	bfed                	j	1d36 <get_time+0x26>

0000000000001d3e <sys_get_time>:
    register long a7 __asm__("a7") = n;
    1d3e:	0a900893          	li	a7,169
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1d42:	00000073          	ecall

int sys_get_time(TimeVal *ts, int tz)
{
    return syscall(SYS_gettimeofday, ts, tz);
}
    1d46:	2501                	sext.w	a0,a0
    1d48:	8082                	ret

0000000000001d4a <time>:
    register long a7 __asm__("a7") = n;
    1d4a:	42600893          	li	a7,1062
    __asm_syscall("r"(a7), "0"(a0))
    1d4e:	00000073          	ecall

int time(unsigned long *tloc)
{
    return syscall(SYS_time, tloc);
}
    1d52:	2501                	sext.w	a0,a0
    1d54:	8082                	ret

0000000000001d56 <sleep>:

int sleep(unsigned long long time)
{
    1d56:	1141                	addi	sp,sp,-16
    TimeVal tv = {.sec = time, .usec = 0};
    1d58:	e02a                	sd	a0,0(sp)
    register long a0 __asm__("a0") = a;
    1d5a:	850a                	mv	a0,sp
    1d5c:	e402                	sd	zero,8(sp)
    register long a7 __asm__("a7") = n;
    1d5e:	06500893          	li	a7,101
    register long a1 __asm__("a1") = b;
    1d62:	85aa                	mv	a1,a0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1d64:	00000073          	ecall
    if (syscall(SYS_nanosleep, &tv, &tv)) return tv.sec;
    1d68:	e501                	bnez	a0,1d70 <sleep+0x1a>
    return 0;
    1d6a:	4501                	li	a0,0
}
    1d6c:	0141                	addi	sp,sp,16
    1d6e:	8082                	ret
    if (syscall(SYS_nanosleep, &tv, &tv)) return tv.sec;
    1d70:	4502                	lw	a0,0(sp)
}
    1d72:	0141                	addi	sp,sp,16
    1d74:	8082                	ret

0000000000001d76 <set_priority>:
    register long a7 __asm__("a7") = n;
    1d76:	08c00893          	li	a7,140
    __asm_syscall("r"(a7), "0"(a0))
    1d7a:	00000073          	ecall

int set_priority(int prio)
{
    return syscall(SYS_setpriority, prio);
}
    1d7e:	2501                	sext.w	a0,a0
    1d80:	8082                	ret

0000000000001d82 <mmap>:
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4))
}

static inline long __syscall6(long n, long a, long b, long c, long d, long e, long f)
{
    register long a7 __asm__("a7") = n;
    1d82:	0de00893          	li	a7,222
    register long a1 __asm__("a1") = b;
    register long a2 __asm__("a2") = c;
    register long a3 __asm__("a3") = d;
    register long a4 __asm__("a4") = e;
    register long a5 __asm__("a5") = f;
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4), "r"(a5))
    1d86:	00000073          	ecall

void *mmap(void *start, size_t len, int prot, int flags, int fd, off_t off)
{
    return syscall(SYS_mmap, start, len, prot, flags, fd, off);
}
    1d8a:	8082                	ret

0000000000001d8c <munmap>:
    register long a7 __asm__("a7") = n;
    1d8c:	0d700893          	li	a7,215
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1d90:	00000073          	ecall

int munmap(void *start, size_t len)
{
    return syscall(SYS_munmap, start, len);
}
    1d94:	2501                	sext.w	a0,a0
    1d96:	8082                	ret

0000000000001d98 <wait>:

int wait(int *code)
{
    1d98:	85aa                	mv	a1,a0
    register long a7 __asm__("a7") = n;
    1d9a:	10400893          	li	a7,260
    register long a0 __asm__("a0") = a;
    1d9e:	557d                	li	a0,-1
    register long a2 __asm__("a2") = c;
    1da0:	4601                	li	a2,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1da2:	00000073          	ecall
    return waitpid(-1, code, 0);
}
    1da6:	2501                	sext.w	a0,a0
    1da8:	8082                	ret

0000000000001daa <spawn>:
    register long a7 __asm__("a7") = n;
    1daa:	19000893          	li	a7,400
    __asm_syscall("r"(a7), "0"(a0))
    1dae:	00000073          	ecall

int spawn(char *file)
{
    return syscall(SYS_spawn, file);
}
    1db2:	2501                	sext.w	a0,a0
    1db4:	8082                	ret

0000000000001db6 <mailread>:
    register long a7 __asm__("a7") = n;
    1db6:	19100893          	li	a7,401
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1dba:	00000073          	ecall

int mailread(void *buf, int len)
{
    return syscall(SYS_mailread, buf, len);
}
    1dbe:	2501                	sext.w	a0,a0
    1dc0:	8082                	ret

0000000000001dc2 <mailwrite>:
    register long a7 __asm__("a7") = n;
    1dc2:	19200893          	li	a7,402
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1dc6:	00000073          	ecall

int mailwrite(int pid, void *buf, int len)
{
    return syscall(SYS_mailwrite, pid, buf, len);
}
    1dca:	2501                	sext.w	a0,a0
    1dcc:	8082                	ret

0000000000001dce <fstat>:
    register long a7 __asm__("a7") = n;
    1dce:	05000893          	li	a7,80
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1dd2:	00000073          	ecall

int fstat(int fd, struct kstat *st)
{
    return syscall(SYS_fstat, fd, st);
}
    1dd6:	2501                	sext.w	a0,a0
    1dd8:	8082                	ret

0000000000001dda <sys_linkat>:
    register long a4 __asm__("a4") = e;
    1dda:	1702                	slli	a4,a4,0x20
    register long a7 __asm__("a7") = n;
    1ddc:	02500893          	li	a7,37
    register long a4 __asm__("a4") = e;
    1de0:	9301                	srli	a4,a4,0x20
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4))
    1de2:	00000073          	ecall

int sys_linkat(int olddirfd, char *oldpath, int newdirfd, char *newpath, unsigned int flags)
{
    return syscall(SYS_linkat, olddirfd, oldpath, newdirfd, newpath, flags);
}
    1de6:	2501                	sext.w	a0,a0
    1de8:	8082                	ret

0000000000001dea <sys_unlinkat>:
    register long a2 __asm__("a2") = c;
    1dea:	1602                	slli	a2,a2,0x20
    register long a7 __asm__("a7") = n;
    1dec:	02300893          	li	a7,35
    register long a2 __asm__("a2") = c;
    1df0:	9201                	srli	a2,a2,0x20
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1df2:	00000073          	ecall

int sys_unlinkat(int dirfd, char *path, unsigned int flags)
{
    return syscall(SYS_unlinkat, dirfd, path, flags);
}
    1df6:	2501                	sext.w	a0,a0
    1df8:	8082                	ret

0000000000001dfa <link>:

int link(char *old_path, char *new_path)
{
    1dfa:	87aa                	mv	a5,a0
    1dfc:	86ae                	mv	a3,a1
    register long a7 __asm__("a7") = n;
    1dfe:	02500893          	li	a7,37
    register long a0 __asm__("a0") = a;
    1e02:	f9c00513          	li	a0,-100
    register long a1 __asm__("a1") = b;
    1e06:	85be                	mv	a1,a5
    register long a2 __asm__("a2") = c;
    1e08:	f9c00613          	li	a2,-100
    register long a4 __asm__("a4") = e;
    1e0c:	4701                	li	a4,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4))
    1e0e:	00000073          	ecall
    return sys_linkat(AT_FDCWD, old_path, AT_FDCWD, new_path, 0);
}
    1e12:	2501                	sext.w	a0,a0
    1e14:	8082                	ret

0000000000001e16 <unlink>:

int unlink(char *path)
{
    1e16:	85aa                	mv	a1,a0
    register long a7 __asm__("a7") = n;
    1e18:	02300893          	li	a7,35
    register long a0 __asm__("a0") = a;
    1e1c:	f9c00513          	li	a0,-100
    register long a2 __asm__("a2") = c;
    1e20:	4601                	li	a2,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1e22:	00000073          	ecall
    return sys_unlinkat(AT_FDCWD, path, 0);
}
    1e26:	2501                	sext.w	a0,a0
    1e28:	8082                	ret

0000000000001e2a <uname>:
    register long a7 __asm__("a7") = n;
    1e2a:	0a000893          	li	a7,160
    __asm_syscall("r"(a7), "0"(a0))
    1e2e:	00000073          	ecall

int uname(void *buf)
{
    return syscall(SYS_uname, buf);
}
    1e32:	2501                	sext.w	a0,a0
    1e34:	8082                	ret

0000000000001e36 <brk>:
    register long a7 __asm__("a7") = n;
    1e36:	0d600893          	li	a7,214
    __asm_syscall("r"(a7), "0"(a0))
    1e3a:	00000073          	ecall

int brk(void *addr)
{
    return syscall(SYS_brk, addr);
}
    1e3e:	2501                	sext.w	a0,a0
    1e40:	8082                	ret

0000000000001e42 <getcwd>:
    register long a7 __asm__("a7") = n;
    1e42:	48c5                	li	a7,17
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1e44:	00000073          	ecall

char *getcwd(char *buf, size_t size){
    return syscall(SYS_getcwd, buf, size);
}
    1e48:	8082                	ret

0000000000001e4a <chdir>:
    register long a7 __asm__("a7") = n;
    1e4a:	03100893          	li	a7,49
    __asm_syscall("r"(a7), "0"(a0))
    1e4e:	00000073          	ecall

int chdir(const char *path){
    return syscall(SYS_chdir, path);
}
    1e52:	2501                	sext.w	a0,a0
    1e54:	8082                	ret

0000000000001e56 <mkdir>:

int mkdir(const char *path, mode_t mode){
    1e56:	862e                	mv	a2,a1
    1e58:	87aa                	mv	a5,a0
    register long a2 __asm__("a2") = c;
    1e5a:	1602                	slli	a2,a2,0x20
    register long a7 __asm__("a7") = n;
    1e5c:	02200893          	li	a7,34
    register long a0 __asm__("a0") = a;
    1e60:	f9c00513          	li	a0,-100
    register long a1 __asm__("a1") = b;
    1e64:	85be                	mv	a1,a5
    register long a2 __asm__("a2") = c;
    1e66:	9201                	srli	a2,a2,0x20
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1e68:	00000073          	ecall
    return syscall(SYS_mkdirat, AT_FDCWD, path, mode);
}
    1e6c:	2501                	sext.w	a0,a0
    1e6e:	8082                	ret

0000000000001e70 <getdents>:
    register long a7 __asm__("a7") = n;
    1e70:	03d00893          	li	a7,61
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1e74:	00000073          	ecall

int getdents(int fd, struct linux_dirent64 *dirp64, unsigned long len){
    //return syscall(SYS_getdents64, fd, dirp64, len);
    return syscall(SYS_getdents64, fd, dirp64, len);
}
    1e78:	2501                	sext.w	a0,a0
    1e7a:	8082                	ret

0000000000001e7c <pipe>:
    register long a7 __asm__("a7") = n;
    1e7c:	03b00893          	li	a7,59
    register long a1 __asm__("a1") = b;
    1e80:	4581                	li	a1,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1e82:	00000073          	ecall

int pipe(int fd[2]){
    return syscall(SYS_pipe2, fd, 0);
}
    1e86:	2501                	sext.w	a0,a0
    1e88:	8082                	ret

0000000000001e8a <dup>:
    register long a7 __asm__("a7") = n;
    1e8a:	48dd                	li	a7,23
    __asm_syscall("r"(a7), "0"(a0))
    1e8c:	00000073          	ecall

int dup(int fd){
    return syscall(SYS_dup, fd);
}
    1e90:	2501                	sext.w	a0,a0
    1e92:	8082                	ret

0000000000001e94 <dup2>:
    register long a7 __asm__("a7") = n;
    1e94:	48e1                	li	a7,24
    register long a2 __asm__("a2") = c;
    1e96:	4601                	li	a2,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1e98:	00000073          	ecall

int dup2(int old, int new){
    return syscall(SYS_dup3, old, new, 0);
}
    1e9c:	2501                	sext.w	a0,a0
    1e9e:	8082                	ret

0000000000001ea0 <mount>:
    register long a7 __asm__("a7") = n;
    1ea0:	02800893          	li	a7,40
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4))
    1ea4:	00000073          	ecall

int mount(const char *special, const char *dir, const char *fstype, unsigned long flags, const void *data)
{
        return syscall(SYS_mount, special, dir, fstype, flags, data);
}
    1ea8:	2501                	sext.w	a0,a0
    1eaa:	8082                	ret

0000000000001eac <umount>:
    register long a7 __asm__("a7") = n;
    1eac:	02700893          	li	a7,39
    register long a1 __asm__("a1") = b;
    1eb0:	4581                	li	a1,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1eb2:	00000073          	ecall

int umount(const char *special)
{
        return syscall(SYS_umount2, special, 0);
}
    1eb6:	2501                	sext.w	a0,a0
    1eb8:	8082                	ret

0000000000001eba <__clone>:

.global __clone
.type  __clone, %function
__clone:
	# Save func and arg to stack
	addi a1, a1, -16
    1eba:	15c1                	addi	a1,a1,-16
	sd a0, 0(a1)
    1ebc:	e188                	sd	a0,0(a1)
	sd a3, 8(a1)
    1ebe:	e594                	sd	a3,8(a1)

	# Call SYS_clone
	mv a0, a2
    1ec0:	8532                	mv	a0,a2
	mv a2, a4
    1ec2:	863a                	mv	a2,a4
	mv a3, a5
    1ec4:	86be                	mv	a3,a5
	mv a4, a6
    1ec6:	8742                	mv	a4,a6
	li a7, 220 # SYS_clone
    1ec8:	0dc00893          	li	a7,220
	ecall
    1ecc:	00000073          	ecall

	beqz a0, 1f
    1ed0:	c111                	beqz	a0,1ed4 <__clone+0x1a>
	# Parent
	ret
    1ed2:	8082                	ret

	# Child
1:      ld a1, 0(sp)
    1ed4:	6582                	ld	a1,0(sp)
	ld a0, 8(sp)
    1ed6:	6522                	ld	a0,8(sp)
	jalr a1
    1ed8:	9582                	jalr	a1

	# Exit
	li a7, 93 # SYS_exit
    1eda:	05d00893          	li	a7,93
	ecall
    1ede:	00000073          	ecall
