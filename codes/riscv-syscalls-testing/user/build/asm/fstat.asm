
/home/ubuntu/rust/ultra-os/codes/riscv-syscalls-testing/user/build/riscv64/fstat:     file format elf64-littleriscv


Disassembly of section .text:

0000000000001000 <_start>:
.section .text.entry
.globl _start
_start:
    mv a0, sp
    1000:	850a                	mv	a0,sp
    tail __start_main
    1002:	a0f1                	j	10ce <__start_main>

0000000000001004 <test_fstat>:

#define AT_FDCWD (-100) //相对路径

//Stat *kst;
static struct kstat kst;
void test_fstat() {
    1004:	1101                	addi	sp,sp,-32
	TEST_START(__func__);
    1006:	00001517          	auipc	a0,0x1
    100a:	f0250513          	addi	a0,a0,-254 # 1f08 <__clone+0x2c>
void test_fstat() {
    100e:	ec06                	sd	ra,24(sp)
    1010:	e822                	sd	s0,16(sp)
	TEST_START(__func__);
    1012:	32e000ef          	jal	ra,1340 <puts>
    1016:	00001517          	auipc	a0,0x1
    101a:	06a50513          	addi	a0,a0,106 # 2080 <__func__.0>
    101e:	322000ef          	jal	ra,1340 <puts>
    1022:	00001517          	auipc	a0,0x1
    1026:	efe50513          	addi	a0,a0,-258 # 1f20 <__clone+0x44>
    102a:	316000ef          	jal	ra,1340 <puts>
	int fd = open("./text.txt", 0);
    102e:	4581                	li	a1,0
    1030:	00001517          	auipc	a0,0x1
    1034:	f0050513          	addi	a0,a0,-256 # 1f30 <__clone+0x54>
    1038:	433000ef          	jal	ra,1c6a <open>
	int ret = fstat(fd, &kst);
    103c:	00001597          	auipc	a1,0x1
    1040:	fc458593          	addi	a1,a1,-60 # 2000 <kst>
    1044:	5ad000ef          	jal	ra,1df0 <fstat>
    1048:	842a                	mv	s0,a0
	printf("fstat ret: %d\n", ret);
    104a:	85aa                	mv	a1,a0
    104c:	00001517          	auipc	a0,0x1
    1050:	ef450513          	addi	a0,a0,-268 # 1f40 <__clone+0x64>
    1054:	30e000ef          	jal	ra,1362 <printf>
	assert(ret >= 0);
    1058:	04044c63          	bltz	s0,10b0 <test_fstat+0xac>

	printf("fstat: dev: %d, inode: %d, mode: %d, nlink: %d, size: %d, atime: %d, mtime: %d, ctime: %d\n",
    105c:	00001597          	auipc	a1,0x1
    1060:	fa458593          	addi	a1,a1,-92 # 2000 <kst>
    1064:	0685b303          	ld	t1,104(a1)
    1068:	0585b883          	ld	a7,88(a1)
    106c:	0485b803          	ld	a6,72(a1)
    1070:	799c                	ld	a5,48(a1)
    1072:	49d8                	lw	a4,20(a1)
    1074:	4994                	lw	a3,16(a1)
    1076:	6590                	ld	a2,8(a1)
    1078:	618c                	ld	a1,0(a1)
    107a:	00001517          	auipc	a0,0x1
    107e:	ef650513          	addi	a0,a0,-266 # 1f70 <__clone+0x94>
    1082:	e01a                	sd	t1,0(sp)
    1084:	2de000ef          	jal	ra,1362 <printf>
	      kst.st_dev, kst.st_ino, kst.st_mode, kst.st_nlink, kst.st_size, kst.st_atime_sec, kst.st_mtime_sec, kst.st_ctime_sec);

	TEST_END(__func__);
    1088:	00001517          	auipc	a0,0x1
    108c:	f4850513          	addi	a0,a0,-184 # 1fd0 <__clone+0xf4>
    1090:	2b0000ef          	jal	ra,1340 <puts>
    1094:	00001517          	auipc	a0,0x1
    1098:	fec50513          	addi	a0,a0,-20 # 2080 <__func__.0>
    109c:	2a4000ef          	jal	ra,1340 <puts>
}
    10a0:	6442                	ld	s0,16(sp)
    10a2:	60e2                	ld	ra,24(sp)
	TEST_END(__func__);
    10a4:	00001517          	auipc	a0,0x1
    10a8:	e7c50513          	addi	a0,a0,-388 # 1f20 <__clone+0x44>
}
    10ac:	6105                	addi	sp,sp,32
	TEST_END(__func__);
    10ae:	ac49                	j	1340 <puts>
	assert(ret >= 0);
    10b0:	00001517          	auipc	a0,0x1
    10b4:	ea050513          	addi	a0,a0,-352 # 1f50 <__clone+0x74>
    10b8:	52e000ef          	jal	ra,15e6 <panic>
    10bc:	b745                	j	105c <test_fstat+0x58>

00000000000010be <main>:

int main(void) {
    10be:	1141                	addi	sp,sp,-16
    10c0:	e406                	sd	ra,8(sp)
	test_fstat();
    10c2:	f43ff0ef          	jal	ra,1004 <test_fstat>
	return 0;
}
    10c6:	60a2                	ld	ra,8(sp)
    10c8:	4501                	li	a0,0
    10ca:	0141                	addi	sp,sp,16
    10cc:	8082                	ret

00000000000010ce <__start_main>:
#include <unistd.h>

extern int main();

int __start_main(long *p)
{
    10ce:	85aa                	mv	a1,a0
	int argc = p[0];
	char **argv = (void *)(p+1);

	exit(main(argc, argv));
    10d0:	4108                	lw	a0,0(a0)
{
    10d2:	1141                	addi	sp,sp,-16
	exit(main(argc, argv));
    10d4:	05a1                	addi	a1,a1,8
{
    10d6:	e406                	sd	ra,8(sp)
	exit(main(argc, argv));
    10d8:	fe7ff0ef          	jal	ra,10be <main>
    10dc:	41d000ef          	jal	ra,1cf8 <exit>
	return 0;
}
    10e0:	60a2                	ld	ra,8(sp)
    10e2:	4501                	li	a0,0
    10e4:	0141                	addi	sp,sp,16
    10e6:	8082                	ret

00000000000010e8 <printint.constprop.0>:
    write(f, s, l);
}

static char digits[] = "0123456789abcdef";

static void printint(int xx, int base, int sign)
    10e8:	7179                	addi	sp,sp,-48
    10ea:	f406                	sd	ra,40(sp)
{
    char buf[16 + 1];
    int i;
    uint x;

    if (sign && (sign = xx < 0))
    10ec:	12054b63          	bltz	a0,1222 <printint.constprop.0+0x13a>

    buf[16] = 0;
    i = 15;
    do
    {
        buf[i--] = digits[x % base];
    10f0:	02b577bb          	remuw	a5,a0,a1
    10f4:	00001617          	auipc	a2,0x1
    10f8:	f9c60613          	addi	a2,a2,-100 # 2090 <digits>
    buf[16] = 0;
    10fc:	00010c23          	sb	zero,24(sp)
        buf[i--] = digits[x % base];
    1100:	0005871b          	sext.w	a4,a1
    1104:	1782                	slli	a5,a5,0x20
    1106:	9381                	srli	a5,a5,0x20
    1108:	97b2                	add	a5,a5,a2
    110a:	0007c783          	lbu	a5,0(a5)
    } while ((x /= base) != 0);
    110e:	02b5583b          	divuw	a6,a0,a1
        buf[i--] = digits[x % base];
    1112:	00f10ba3          	sb	a5,23(sp)
    } while ((x /= base) != 0);
    1116:	1cb56363          	bltu	a0,a1,12dc <printint.constprop.0+0x1f4>
        buf[i--] = digits[x % base];
    111a:	45b9                	li	a1,14
    111c:	02e877bb          	remuw	a5,a6,a4
    1120:	1782                	slli	a5,a5,0x20
    1122:	9381                	srli	a5,a5,0x20
    1124:	97b2                	add	a5,a5,a2
    1126:	0007c783          	lbu	a5,0(a5)
    } while ((x /= base) != 0);
    112a:	02e856bb          	divuw	a3,a6,a4
        buf[i--] = digits[x % base];
    112e:	00f10b23          	sb	a5,22(sp)
    } while ((x /= base) != 0);
    1132:	0ce86e63          	bltu	a6,a4,120e <printint.constprop.0+0x126>
        buf[i--] = digits[x % base];
    1136:	02e6f5bb          	remuw	a1,a3,a4
    } while ((x /= base) != 0);
    113a:	02e6d7bb          	divuw	a5,a3,a4
        buf[i--] = digits[x % base];
    113e:	1582                	slli	a1,a1,0x20
    1140:	9181                	srli	a1,a1,0x20
    1142:	95b2                	add	a1,a1,a2
    1144:	0005c583          	lbu	a1,0(a1)
    1148:	00b10aa3          	sb	a1,21(sp)
    } while ((x /= base) != 0);
    114c:	0007859b          	sext.w	a1,a5
    1150:	12e6ec63          	bltu	a3,a4,1288 <printint.constprop.0+0x1a0>
        buf[i--] = digits[x % base];
    1154:	02e7f6bb          	remuw	a3,a5,a4
    1158:	1682                	slli	a3,a3,0x20
    115a:	9281                	srli	a3,a3,0x20
    115c:	96b2                	add	a3,a3,a2
    115e:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    1162:	02e7d83b          	divuw	a6,a5,a4
        buf[i--] = digits[x % base];
    1166:	00d10a23          	sb	a3,20(sp)
    } while ((x /= base) != 0);
    116a:	12e5e863          	bltu	a1,a4,129a <printint.constprop.0+0x1b2>
        buf[i--] = digits[x % base];
    116e:	02e876bb          	remuw	a3,a6,a4
    1172:	1682                	slli	a3,a3,0x20
    1174:	9281                	srli	a3,a3,0x20
    1176:	96b2                	add	a3,a3,a2
    1178:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    117c:	02e855bb          	divuw	a1,a6,a4
        buf[i--] = digits[x % base];
    1180:	00d109a3          	sb	a3,19(sp)
    } while ((x /= base) != 0);
    1184:	12e86463          	bltu	a6,a4,12ac <printint.constprop.0+0x1c4>
        buf[i--] = digits[x % base];
    1188:	02e5f6bb          	remuw	a3,a1,a4
    118c:	1682                	slli	a3,a3,0x20
    118e:	9281                	srli	a3,a3,0x20
    1190:	96b2                	add	a3,a3,a2
    1192:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    1196:	02e5d83b          	divuw	a6,a1,a4
        buf[i--] = digits[x % base];
    119a:	00d10923          	sb	a3,18(sp)
    } while ((x /= base) != 0);
    119e:	0ce5ec63          	bltu	a1,a4,1276 <printint.constprop.0+0x18e>
        buf[i--] = digits[x % base];
    11a2:	02e876bb          	remuw	a3,a6,a4
    11a6:	1682                	slli	a3,a3,0x20
    11a8:	9281                	srli	a3,a3,0x20
    11aa:	96b2                	add	a3,a3,a2
    11ac:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    11b0:	02e855bb          	divuw	a1,a6,a4
        buf[i--] = digits[x % base];
    11b4:	00d108a3          	sb	a3,17(sp)
    } while ((x /= base) != 0);
    11b8:	10e86963          	bltu	a6,a4,12ca <printint.constprop.0+0x1e2>
        buf[i--] = digits[x % base];
    11bc:	02e5f6bb          	remuw	a3,a1,a4
    11c0:	1682                	slli	a3,a3,0x20
    11c2:	9281                	srli	a3,a3,0x20
    11c4:	96b2                	add	a3,a3,a2
    11c6:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    11ca:	02e5d83b          	divuw	a6,a1,a4
        buf[i--] = digits[x % base];
    11ce:	00d10823          	sb	a3,16(sp)
    } while ((x /= base) != 0);
    11d2:	10e5e763          	bltu	a1,a4,12e0 <printint.constprop.0+0x1f8>
        buf[i--] = digits[x % base];
    11d6:	02e876bb          	remuw	a3,a6,a4
    11da:	1682                	slli	a3,a3,0x20
    11dc:	9281                	srli	a3,a3,0x20
    11de:	96b2                	add	a3,a3,a2
    11e0:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    11e4:	02e857bb          	divuw	a5,a6,a4
        buf[i--] = digits[x % base];
    11e8:	00d107a3          	sb	a3,15(sp)
    } while ((x /= base) != 0);
    11ec:	10e86363          	bltu	a6,a4,12f2 <printint.constprop.0+0x20a>
        buf[i--] = digits[x % base];
    11f0:	1782                	slli	a5,a5,0x20
    11f2:	9381                	srli	a5,a5,0x20
    11f4:	97b2                	add	a5,a5,a2
    11f6:	0007c783          	lbu	a5,0(a5)
    11fa:	4599                	li	a1,6
    11fc:	00f10723          	sb	a5,14(sp)

    if (sign)
    1200:	00055763          	bgez	a0,120e <printint.constprop.0+0x126>
        buf[i--] = '-';
    1204:	02d00793          	li	a5,45
    1208:	00f106a3          	sb	a5,13(sp)
        buf[i--] = digits[x % base];
    120c:	4595                	li	a1,5
    write(f, s, l);
    120e:	003c                	addi	a5,sp,8
    1210:	4641                	li	a2,16
    1212:	9e0d                	subw	a2,a2,a1
    1214:	4505                	li	a0,1
    1216:	95be                	add	a1,a1,a5
    1218:	291000ef          	jal	ra,1ca8 <write>
    i++;
    if (i < 0)
        puts("printint error");
    out(stdout, buf + i, 16 - i);
}
    121c:	70a2                	ld	ra,40(sp)
    121e:	6145                	addi	sp,sp,48
    1220:	8082                	ret
        x = -xx;
    1222:	40a0083b          	negw	a6,a0
        buf[i--] = digits[x % base];
    1226:	02b877bb          	remuw	a5,a6,a1
    122a:	00001617          	auipc	a2,0x1
    122e:	e6660613          	addi	a2,a2,-410 # 2090 <digits>
    buf[16] = 0;
    1232:	00010c23          	sb	zero,24(sp)
        buf[i--] = digits[x % base];
    1236:	0005871b          	sext.w	a4,a1
    123a:	1782                	slli	a5,a5,0x20
    123c:	9381                	srli	a5,a5,0x20
    123e:	97b2                	add	a5,a5,a2
    1240:	0007c783          	lbu	a5,0(a5)
    } while ((x /= base) != 0);
    1244:	02b858bb          	divuw	a7,a6,a1
        buf[i--] = digits[x % base];
    1248:	00f10ba3          	sb	a5,23(sp)
    } while ((x /= base) != 0);
    124c:	06b86963          	bltu	a6,a1,12be <printint.constprop.0+0x1d6>
        buf[i--] = digits[x % base];
    1250:	02e8f7bb          	remuw	a5,a7,a4
    1254:	1782                	slli	a5,a5,0x20
    1256:	9381                	srli	a5,a5,0x20
    1258:	97b2                	add	a5,a5,a2
    125a:	0007c783          	lbu	a5,0(a5)
    } while ((x /= base) != 0);
    125e:	02e8d6bb          	divuw	a3,a7,a4
        buf[i--] = digits[x % base];
    1262:	00f10b23          	sb	a5,22(sp)
    } while ((x /= base) != 0);
    1266:	ece8f8e3          	bgeu	a7,a4,1136 <printint.constprop.0+0x4e>
        buf[i--] = '-';
    126a:	02d00793          	li	a5,45
    126e:	00f10aa3          	sb	a5,21(sp)
        buf[i--] = digits[x % base];
    1272:	45b5                	li	a1,13
    1274:	bf69                	j	120e <printint.constprop.0+0x126>
    1276:	45a9                	li	a1,10
    if (sign)
    1278:	f8055be3          	bgez	a0,120e <printint.constprop.0+0x126>
        buf[i--] = '-';
    127c:	02d00793          	li	a5,45
    1280:	00f108a3          	sb	a5,17(sp)
        buf[i--] = digits[x % base];
    1284:	45a5                	li	a1,9
    1286:	b761                	j	120e <printint.constprop.0+0x126>
    1288:	45b5                	li	a1,13
    if (sign)
    128a:	f80552e3          	bgez	a0,120e <printint.constprop.0+0x126>
        buf[i--] = '-';
    128e:	02d00793          	li	a5,45
    1292:	00f10a23          	sb	a5,20(sp)
        buf[i--] = digits[x % base];
    1296:	45b1                	li	a1,12
    1298:	bf9d                	j	120e <printint.constprop.0+0x126>
    129a:	45b1                	li	a1,12
    if (sign)
    129c:	f60559e3          	bgez	a0,120e <printint.constprop.0+0x126>
        buf[i--] = '-';
    12a0:	02d00793          	li	a5,45
    12a4:	00f109a3          	sb	a5,19(sp)
        buf[i--] = digits[x % base];
    12a8:	45ad                	li	a1,11
    12aa:	b795                	j	120e <printint.constprop.0+0x126>
    12ac:	45ad                	li	a1,11
    if (sign)
    12ae:	f60550e3          	bgez	a0,120e <printint.constprop.0+0x126>
        buf[i--] = '-';
    12b2:	02d00793          	li	a5,45
    12b6:	00f10923          	sb	a5,18(sp)
        buf[i--] = digits[x % base];
    12ba:	45a9                	li	a1,10
    12bc:	bf89                	j	120e <printint.constprop.0+0x126>
        buf[i--] = '-';
    12be:	02d00793          	li	a5,45
    12c2:	00f10b23          	sb	a5,22(sp)
        buf[i--] = digits[x % base];
    12c6:	45b9                	li	a1,14
    12c8:	b799                	j	120e <printint.constprop.0+0x126>
    12ca:	45a5                	li	a1,9
    if (sign)
    12cc:	f40551e3          	bgez	a0,120e <printint.constprop.0+0x126>
        buf[i--] = '-';
    12d0:	02d00793          	li	a5,45
    12d4:	00f10823          	sb	a5,16(sp)
        buf[i--] = digits[x % base];
    12d8:	45a1                	li	a1,8
    12da:	bf15                	j	120e <printint.constprop.0+0x126>
    i = 15;
    12dc:	45bd                	li	a1,15
    12de:	bf05                	j	120e <printint.constprop.0+0x126>
        buf[i--] = digits[x % base];
    12e0:	45a1                	li	a1,8
    if (sign)
    12e2:	f20556e3          	bgez	a0,120e <printint.constprop.0+0x126>
        buf[i--] = '-';
    12e6:	02d00793          	li	a5,45
    12ea:	00f107a3          	sb	a5,15(sp)
        buf[i--] = digits[x % base];
    12ee:	459d                	li	a1,7
    12f0:	bf39                	j	120e <printint.constprop.0+0x126>
    12f2:	459d                	li	a1,7
    if (sign)
    12f4:	f0055de3          	bgez	a0,120e <printint.constprop.0+0x126>
        buf[i--] = '-';
    12f8:	02d00793          	li	a5,45
    12fc:	00f10723          	sb	a5,14(sp)
        buf[i--] = digits[x % base];
    1300:	4599                	li	a1,6
    1302:	b731                	j	120e <printint.constprop.0+0x126>

0000000000001304 <getchar>:
{
    1304:	1101                	addi	sp,sp,-32
    read(stdin, &byte, 1);
    1306:	00f10593          	addi	a1,sp,15
    130a:	4605                	li	a2,1
    130c:	4501                	li	a0,0
{
    130e:	ec06                	sd	ra,24(sp)
    char byte = 0;
    1310:	000107a3          	sb	zero,15(sp)
    read(stdin, &byte, 1);
    1314:	18b000ef          	jal	ra,1c9e <read>
}
    1318:	60e2                	ld	ra,24(sp)
    131a:	00f14503          	lbu	a0,15(sp)
    131e:	6105                	addi	sp,sp,32
    1320:	8082                	ret

0000000000001322 <putchar>:
{
    1322:	1101                	addi	sp,sp,-32
    1324:	87aa                	mv	a5,a0
    return write(stdout, &byte, 1);
    1326:	00f10593          	addi	a1,sp,15
    132a:	4605                	li	a2,1
    132c:	4505                	li	a0,1
{
    132e:	ec06                	sd	ra,24(sp)
    char byte = c;
    1330:	00f107a3          	sb	a5,15(sp)
    return write(stdout, &byte, 1);
    1334:	175000ef          	jal	ra,1ca8 <write>
}
    1338:	60e2                	ld	ra,24(sp)
    133a:	2501                	sext.w	a0,a0
    133c:	6105                	addi	sp,sp,32
    133e:	8082                	ret

0000000000001340 <puts>:
{
    1340:	1141                	addi	sp,sp,-16
    1342:	e406                	sd	ra,8(sp)
    1344:	e022                	sd	s0,0(sp)
    1346:	842a                	mv	s0,a0
    r = -(write(stdout, s, strlen(s)) < 0);
    1348:	57c000ef          	jal	ra,18c4 <strlen>
    134c:	862a                	mv	a2,a0
    134e:	85a2                	mv	a1,s0
    1350:	4505                	li	a0,1
    1352:	157000ef          	jal	ra,1ca8 <write>
}
    1356:	60a2                	ld	ra,8(sp)
    1358:	6402                	ld	s0,0(sp)
    r = -(write(stdout, s, strlen(s)) < 0);
    135a:	957d                	srai	a0,a0,0x3f
    return r;
    135c:	2501                	sext.w	a0,a0
}
    135e:	0141                	addi	sp,sp,16
    1360:	8082                	ret

0000000000001362 <printf>:
    out(stdout, buf, i);
}

// Print to the console. only understands %d, %x, %p, %s.
void printf(const char *fmt, ...)
{
    1362:	7171                	addi	sp,sp,-176
    1364:	fc56                	sd	s5,56(sp)
    1366:	ed3e                	sd	a5,152(sp)
    buf[i++] = '0';
    1368:	7ae1                	lui	s5,0xffff8
    va_list ap;
    int cnt = 0, l = 0;
    char *a, *z, *s = (char *)fmt, str;
    int f = stdout;

    va_start(ap, fmt);
    136a:	18bc                	addi	a5,sp,120
{
    136c:	e8ca                	sd	s2,80(sp)
    136e:	e4ce                	sd	s3,72(sp)
    1370:	e0d2                	sd	s4,64(sp)
    1372:	f85a                	sd	s6,48(sp)
    1374:	f486                	sd	ra,104(sp)
    1376:	f0a2                	sd	s0,96(sp)
    1378:	eca6                	sd	s1,88(sp)
    137a:	fcae                	sd	a1,120(sp)
    137c:	e132                	sd	a2,128(sp)
    137e:	e536                	sd	a3,136(sp)
    1380:	e93a                	sd	a4,144(sp)
    1382:	f142                	sd	a6,160(sp)
    1384:	f546                	sd	a7,168(sp)
    va_start(ap, fmt);
    1386:	e03e                	sd	a5,0(sp)
    for (;;)
    {
        if (!*s)
            break;
        for (a = s; *s && *s != '%'; s++)
    1388:	02500913          	li	s2,37
        out(f, a, l);
        if (l)
            continue;
        if (s[1] == 0)
            break;
        switch (s[1])
    138c:	07300a13          	li	s4,115
        case 'p':
            printptr(va_arg(ap, uint64));
            break;
        case 's':
            if ((a = va_arg(ap, char *)) == 0)
                a = "(null)";
    1390:	00001b17          	auipc	s6,0x1
    1394:	c50b0b13          	addi	s6,s6,-944 # 1fe0 <__clone+0x104>
    buf[i++] = '0';
    1398:	830aca93          	xori	s5,s5,-2000
        buf[i++] = digits[x >> (sizeof(uint64) * 8 - 4)];
    139c:	00001997          	auipc	s3,0x1
    13a0:	cf498993          	addi	s3,s3,-780 # 2090 <digits>
        if (!*s)
    13a4:	00054783          	lbu	a5,0(a0)
    13a8:	16078a63          	beqz	a5,151c <printf+0x1ba>
    13ac:	862a                	mv	a2,a0
        for (a = s; *s && *s != '%'; s++)
    13ae:	19278163          	beq	a5,s2,1530 <printf+0x1ce>
    13b2:	00164783          	lbu	a5,1(a2)
    13b6:	0605                	addi	a2,a2,1
    13b8:	fbfd                	bnez	a5,13ae <printf+0x4c>
    13ba:	84b2                	mv	s1,a2
        l = z - a;
    13bc:	40a6043b          	subw	s0,a2,a0
    write(f, s, l);
    13c0:	85aa                	mv	a1,a0
    13c2:	8622                	mv	a2,s0
    13c4:	4505                	li	a0,1
    13c6:	0e3000ef          	jal	ra,1ca8 <write>
        if (l)
    13ca:	18041c63          	bnez	s0,1562 <printf+0x200>
        if (s[1] == 0)
    13ce:	0014c783          	lbu	a5,1(s1)
    13d2:	14078563          	beqz	a5,151c <printf+0x1ba>
        switch (s[1])
    13d6:	1d478063          	beq	a5,s4,1596 <printf+0x234>
    13da:	18fa6663          	bltu	s4,a5,1566 <printf+0x204>
    13de:	06400713          	li	a4,100
    13e2:	1ae78063          	beq	a5,a4,1582 <printf+0x220>
    13e6:	07000713          	li	a4,112
    13ea:	1ce79963          	bne	a5,a4,15bc <printf+0x25a>
            printptr(va_arg(ap, uint64));
    13ee:	6702                	ld	a4,0(sp)
    buf[i++] = '0';
    13f0:	01511423          	sh	s5,8(sp)
    write(f, s, l);
    13f4:	4649                	li	a2,18
            printptr(va_arg(ap, uint64));
    13f6:	631c                	ld	a5,0(a4)
    13f8:	0721                	addi	a4,a4,8
    13fa:	e03a                	sd	a4,0(sp)
    for (j = 0; j < (sizeof(uint64) * 2); j++, x <<= 4)
    13fc:	00479293          	slli	t0,a5,0x4
    1400:	00879f93          	slli	t6,a5,0x8
    1404:	00c79f13          	slli	t5,a5,0xc
    1408:	01079e93          	slli	t4,a5,0x10
    140c:	01479e13          	slli	t3,a5,0x14
    1410:	01879313          	slli	t1,a5,0x18
    1414:	01c79893          	slli	a7,a5,0x1c
    1418:	02479813          	slli	a6,a5,0x24
    141c:	02879513          	slli	a0,a5,0x28
    1420:	02c79593          	slli	a1,a5,0x2c
    1424:	03079693          	slli	a3,a5,0x30
    1428:	03479713          	slli	a4,a5,0x34
        buf[i++] = digits[x >> (sizeof(uint64) * 8 - 4)];
    142c:	03c7d413          	srli	s0,a5,0x3c
    1430:	01c7d39b          	srliw	t2,a5,0x1c
    1434:	03c2d293          	srli	t0,t0,0x3c
    1438:	03cfdf93          	srli	t6,t6,0x3c
    143c:	03cf5f13          	srli	t5,t5,0x3c
    1440:	03cede93          	srli	t4,t4,0x3c
    1444:	03ce5e13          	srli	t3,t3,0x3c
    1448:	03c35313          	srli	t1,t1,0x3c
    144c:	03c8d893          	srli	a7,a7,0x3c
    1450:	03c85813          	srli	a6,a6,0x3c
    1454:	9171                	srli	a0,a0,0x3c
    1456:	91f1                	srli	a1,a1,0x3c
    1458:	92f1                	srli	a3,a3,0x3c
    145a:	9371                	srli	a4,a4,0x3c
    145c:	96ce                	add	a3,a3,s3
    145e:	974e                	add	a4,a4,s3
    1460:	944e                	add	s0,s0,s3
    1462:	92ce                	add	t0,t0,s3
    1464:	9fce                	add	t6,t6,s3
    1466:	9f4e                	add	t5,t5,s3
    1468:	9ece                	add	t4,t4,s3
    146a:	9e4e                	add	t3,t3,s3
    146c:	934e                	add	t1,t1,s3
    146e:	98ce                	add	a7,a7,s3
    1470:	93ce                	add	t2,t2,s3
    1472:	984e                	add	a6,a6,s3
    1474:	954e                	add	a0,a0,s3
    1476:	95ce                	add	a1,a1,s3
    1478:	0006c083          	lbu	ra,0(a3)
    147c:	0002c283          	lbu	t0,0(t0)
    1480:	00074683          	lbu	a3,0(a4)
    1484:	000fcf83          	lbu	t6,0(t6)
    1488:	000f4f03          	lbu	t5,0(t5)
    148c:	000ece83          	lbu	t4,0(t4)
    1490:	000e4e03          	lbu	t3,0(t3)
    1494:	00034303          	lbu	t1,0(t1)
    1498:	0008c883          	lbu	a7,0(a7)
    149c:	0003c383          	lbu	t2,0(t2)
    14a0:	00084803          	lbu	a6,0(a6)
    14a4:	00054503          	lbu	a0,0(a0)
    14a8:	0005c583          	lbu	a1,0(a1)
    14ac:	00044403          	lbu	s0,0(s0)
    for (j = 0; j < (sizeof(uint64) * 2); j++, x <<= 4)
    14b0:	03879713          	slli	a4,a5,0x38
        buf[i++] = digits[x >> (sizeof(uint64) * 8 - 4)];
    14b4:	9371                	srli	a4,a4,0x3c
    14b6:	8bbd                	andi	a5,a5,15
    14b8:	974e                	add	a4,a4,s3
    14ba:	97ce                	add	a5,a5,s3
    14bc:	005105a3          	sb	t0,11(sp)
    14c0:	01f10623          	sb	t6,12(sp)
    14c4:	01e106a3          	sb	t5,13(sp)
    14c8:	01d10723          	sb	t4,14(sp)
    14cc:	01c107a3          	sb	t3,15(sp)
    14d0:	00610823          	sb	t1,16(sp)
    14d4:	011108a3          	sb	a7,17(sp)
    14d8:	00710923          	sb	t2,18(sp)
    14dc:	010109a3          	sb	a6,19(sp)
    14e0:	00a10a23          	sb	a0,20(sp)
    14e4:	00b10aa3          	sb	a1,21(sp)
    14e8:	00110b23          	sb	ra,22(sp)
    14ec:	00d10ba3          	sb	a3,23(sp)
    14f0:	00810523          	sb	s0,10(sp)
    14f4:	00074703          	lbu	a4,0(a4)
    14f8:	0007c783          	lbu	a5,0(a5)
    write(f, s, l);
    14fc:	002c                	addi	a1,sp,8
    14fe:	4505                	li	a0,1
        buf[i++] = digits[x >> (sizeof(uint64) * 8 - 4)];
    1500:	00e10c23          	sb	a4,24(sp)
    1504:	00f10ca3          	sb	a5,25(sp)
    buf[i] = 0;
    1508:	00010d23          	sb	zero,26(sp)
    write(f, s, l);
    150c:	79c000ef          	jal	ra,1ca8 <write>
            // Print unknown % sequence to draw attention.
            putchar('%');
            putchar(s[1]);
            break;
        }
        s += 2;
    1510:	00248513          	addi	a0,s1,2
        if (!*s)
    1514:	00054783          	lbu	a5,0(a0)
    1518:	e8079ae3          	bnez	a5,13ac <printf+0x4a>
    }
    va_end(ap);
}
    151c:	70a6                	ld	ra,104(sp)
    151e:	7406                	ld	s0,96(sp)
    1520:	64e6                	ld	s1,88(sp)
    1522:	6946                	ld	s2,80(sp)
    1524:	69a6                	ld	s3,72(sp)
    1526:	6a06                	ld	s4,64(sp)
    1528:	7ae2                	ld	s5,56(sp)
    152a:	7b42                	ld	s6,48(sp)
    152c:	614d                	addi	sp,sp,176
    152e:	8082                	ret
        for (z = s; s[0] == '%' && s[1] == '%'; z++, s += 2)
    1530:	00064783          	lbu	a5,0(a2)
    1534:	84b2                	mv	s1,a2
    1536:	01278963          	beq	a5,s2,1548 <printf+0x1e6>
    153a:	b549                	j	13bc <printf+0x5a>
    153c:	0024c783          	lbu	a5,2(s1)
    1540:	0605                	addi	a2,a2,1
    1542:	0489                	addi	s1,s1,2
    1544:	e7279ce3          	bne	a5,s2,13bc <printf+0x5a>
    1548:	0014c783          	lbu	a5,1(s1)
    154c:	ff2788e3          	beq	a5,s2,153c <printf+0x1da>
        l = z - a;
    1550:	40a6043b          	subw	s0,a2,a0
    write(f, s, l);
    1554:	85aa                	mv	a1,a0
    1556:	8622                	mv	a2,s0
    1558:	4505                	li	a0,1
    155a:	74e000ef          	jal	ra,1ca8 <write>
        if (l)
    155e:	e60408e3          	beqz	s0,13ce <printf+0x6c>
    1562:	8526                	mv	a0,s1
    1564:	b581                	j	13a4 <printf+0x42>
        switch (s[1])
    1566:	07800713          	li	a4,120
    156a:	04e79963          	bne	a5,a4,15bc <printf+0x25a>
            printint(va_arg(ap, int), 16, 1);
    156e:	6782                	ld	a5,0(sp)
    1570:	45c1                	li	a1,16
    1572:	4388                	lw	a0,0(a5)
    1574:	07a1                	addi	a5,a5,8
    1576:	e03e                	sd	a5,0(sp)
    1578:	b71ff0ef          	jal	ra,10e8 <printint.constprop.0>
        s += 2;
    157c:	00248513          	addi	a0,s1,2
    1580:	bf51                	j	1514 <printf+0x1b2>
            printint(va_arg(ap, int), 10, 1);
    1582:	6782                	ld	a5,0(sp)
    1584:	45a9                	li	a1,10
    1586:	4388                	lw	a0,0(a5)
    1588:	07a1                	addi	a5,a5,8
    158a:	e03e                	sd	a5,0(sp)
    158c:	b5dff0ef          	jal	ra,10e8 <printint.constprop.0>
        s += 2;
    1590:	00248513          	addi	a0,s1,2
    1594:	b741                	j	1514 <printf+0x1b2>
            if ((a = va_arg(ap, char *)) == 0)
    1596:	6782                	ld	a5,0(sp)
    1598:	6380                	ld	s0,0(a5)
    159a:	07a1                	addi	a5,a5,8
    159c:	e03e                	sd	a5,0(sp)
    159e:	c031                	beqz	s0,15e2 <printf+0x280>
            l = strnlen(a, 200);
    15a0:	0c800593          	li	a1,200
    15a4:	8522                	mv	a0,s0
    15a6:	40a000ef          	jal	ra,19b0 <strnlen>
    write(f, s, l);
    15aa:	0005061b          	sext.w	a2,a0
    15ae:	85a2                	mv	a1,s0
    15b0:	4505                	li	a0,1
    15b2:	6f6000ef          	jal	ra,1ca8 <write>
        s += 2;
    15b6:	00248513          	addi	a0,s1,2
    15ba:	bfa9                	j	1514 <printf+0x1b2>
    return write(stdout, &byte, 1);
    15bc:	4605                	li	a2,1
    15be:	002c                	addi	a1,sp,8
    15c0:	4505                	li	a0,1
    char byte = c;
    15c2:	01210423          	sb	s2,8(sp)
    return write(stdout, &byte, 1);
    15c6:	6e2000ef          	jal	ra,1ca8 <write>
    char byte = c;
    15ca:	0014c783          	lbu	a5,1(s1)
    return write(stdout, &byte, 1);
    15ce:	4605                	li	a2,1
    15d0:	002c                	addi	a1,sp,8
    15d2:	4505                	li	a0,1
    char byte = c;
    15d4:	00f10423          	sb	a5,8(sp)
    return write(stdout, &byte, 1);
    15d8:	6d0000ef          	jal	ra,1ca8 <write>
        s += 2;
    15dc:	00248513          	addi	a0,s1,2
    15e0:	bf15                	j	1514 <printf+0x1b2>
                a = "(null)";
    15e2:	845a                	mv	s0,s6
    15e4:	bf75                	j	15a0 <printf+0x23e>

00000000000015e6 <panic>:
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

void panic(char *m)
{
    15e6:	1141                	addi	sp,sp,-16
    15e8:	e406                	sd	ra,8(sp)
    puts(m);
    15ea:	d57ff0ef          	jal	ra,1340 <puts>
    exit(-100);
}
    15ee:	60a2                	ld	ra,8(sp)
    exit(-100);
    15f0:	f9c00513          	li	a0,-100
}
    15f4:	0141                	addi	sp,sp,16
    exit(-100);
    15f6:	a709                	j	1cf8 <exit>

00000000000015f8 <isspace>:
#define HIGHS (ONES * (UCHAR_MAX / 2 + 1))
#define HASZERO(x) (((x)-ONES) & ~(x)&HIGHS)

int isspace(int c)
{
    return c == ' ' || (unsigned)c - '\t' < 5;
    15f8:	02000793          	li	a5,32
    15fc:	00f50663          	beq	a0,a5,1608 <isspace+0x10>
    1600:	355d                	addiw	a0,a0,-9
    1602:	00553513          	sltiu	a0,a0,5
    1606:	8082                	ret
    1608:	4505                	li	a0,1
}
    160a:	8082                	ret

000000000000160c <isdigit>:

int isdigit(int c)
{
    return (unsigned)c - '0' < 10;
    160c:	fd05051b          	addiw	a0,a0,-48
}
    1610:	00a53513          	sltiu	a0,a0,10
    1614:	8082                	ret

0000000000001616 <atoi>:
    return c == ' ' || (unsigned)c - '\t' < 5;
    1616:	02000613          	li	a2,32
    161a:	4591                	li	a1,4

int atoi(const char *s)
{
    int n = 0, neg = 0;
    while (isspace(*s))
    161c:	00054703          	lbu	a4,0(a0)
    return c == ' ' || (unsigned)c - '\t' < 5;
    1620:	ff77069b          	addiw	a3,a4,-9
    1624:	04c70d63          	beq	a4,a2,167e <atoi+0x68>
    1628:	0007079b          	sext.w	a5,a4
    162c:	04d5f963          	bgeu	a1,a3,167e <atoi+0x68>
        s++;
    switch (*s)
    1630:	02b00693          	li	a3,43
    1634:	04d70a63          	beq	a4,a3,1688 <atoi+0x72>
    1638:	02d00693          	li	a3,45
    163c:	06d70463          	beq	a4,a3,16a4 <atoi+0x8e>
        neg = 1;
    case '+':
        s++;
    }
    /* Compute n as a negative number to avoid overflow on INT_MIN */
    while (isdigit(*s))
    1640:	fd07859b          	addiw	a1,a5,-48
    1644:	4625                	li	a2,9
    1646:	873e                	mv	a4,a5
    1648:	86aa                	mv	a3,a0
    int n = 0, neg = 0;
    164a:	4e01                	li	t3,0
    while (isdigit(*s))
    164c:	04b66a63          	bltu	a2,a1,16a0 <atoi+0x8a>
    int n = 0, neg = 0;
    1650:	4501                	li	a0,0
    while (isdigit(*s))
    1652:	4825                	li	a6,9
    1654:	0016c603          	lbu	a2,1(a3)
        n = 10 * n - (*s++ - '0');
    1658:	0025179b          	slliw	a5,a0,0x2
    165c:	9d3d                	addw	a0,a0,a5
    165e:	fd07031b          	addiw	t1,a4,-48
    1662:	0015189b          	slliw	a7,a0,0x1
    while (isdigit(*s))
    1666:	fd06059b          	addiw	a1,a2,-48
        n = 10 * n - (*s++ - '0');
    166a:	0685                	addi	a3,a3,1
    166c:	4068853b          	subw	a0,a7,t1
    while (isdigit(*s))
    1670:	0006071b          	sext.w	a4,a2
    1674:	feb870e3          	bgeu	a6,a1,1654 <atoi+0x3e>
    return neg ? n : -n;
    1678:	000e0563          	beqz	t3,1682 <atoi+0x6c>
}
    167c:	8082                	ret
        s++;
    167e:	0505                	addi	a0,a0,1
    1680:	bf71                	j	161c <atoi+0x6>
    return neg ? n : -n;
    1682:	4113053b          	subw	a0,t1,a7
    1686:	8082                	ret
    while (isdigit(*s))
    1688:	00154783          	lbu	a5,1(a0)
    168c:	4625                	li	a2,9
        s++;
    168e:	00150693          	addi	a3,a0,1
    while (isdigit(*s))
    1692:	fd07859b          	addiw	a1,a5,-48
    1696:	0007871b          	sext.w	a4,a5
    int n = 0, neg = 0;
    169a:	4e01                	li	t3,0
    while (isdigit(*s))
    169c:	fab67ae3          	bgeu	a2,a1,1650 <atoi+0x3a>
    16a0:	4501                	li	a0,0
}
    16a2:	8082                	ret
    while (isdigit(*s))
    16a4:	00154783          	lbu	a5,1(a0)
    16a8:	4625                	li	a2,9
        s++;
    16aa:	00150693          	addi	a3,a0,1
    while (isdigit(*s))
    16ae:	fd07859b          	addiw	a1,a5,-48
    16b2:	0007871b          	sext.w	a4,a5
    16b6:	feb665e3          	bltu	a2,a1,16a0 <atoi+0x8a>
        neg = 1;
    16ba:	4e05                	li	t3,1
    16bc:	bf51                	j	1650 <atoi+0x3a>

00000000000016be <memset>:

void *memset(void *dest, int c, size_t n)
{
    char *p = dest;
    for (int i = 0; i < n; ++i, *(p++) = c)
    16be:	16060d63          	beqz	a2,1838 <memset+0x17a>
    16c2:	40a007b3          	neg	a5,a0
    16c6:	8b9d                	andi	a5,a5,7
    16c8:	00778713          	addi	a4,a5,7
    16cc:	482d                	li	a6,11
    16ce:	0ff5f593          	andi	a1,a1,255
    16d2:	fff60693          	addi	a3,a2,-1
    16d6:	17076263          	bltu	a4,a6,183a <memset+0x17c>
    16da:	16e6ea63          	bltu	a3,a4,184e <memset+0x190>
    16de:	16078563          	beqz	a5,1848 <memset+0x18a>
    16e2:	00b50023          	sb	a1,0(a0)
    16e6:	4705                	li	a4,1
    16e8:	00150e93          	addi	t4,a0,1
    16ec:	14e78c63          	beq	a5,a4,1844 <memset+0x186>
    16f0:	00b500a3          	sb	a1,1(a0)
    16f4:	4709                	li	a4,2
    16f6:	00250e93          	addi	t4,a0,2
    16fa:	14e78d63          	beq	a5,a4,1854 <memset+0x196>
    16fe:	00b50123          	sb	a1,2(a0)
    1702:	470d                	li	a4,3
    1704:	00350e93          	addi	t4,a0,3
    1708:	12e78b63          	beq	a5,a4,183e <memset+0x180>
    170c:	00b501a3          	sb	a1,3(a0)
    1710:	4711                	li	a4,4
    1712:	00450e93          	addi	t4,a0,4
    1716:	14e78163          	beq	a5,a4,1858 <memset+0x19a>
    171a:	00b50223          	sb	a1,4(a0)
    171e:	4715                	li	a4,5
    1720:	00550e93          	addi	t4,a0,5
    1724:	12e78c63          	beq	a5,a4,185c <memset+0x19e>
    1728:	00b502a3          	sb	a1,5(a0)
    172c:	471d                	li	a4,7
    172e:	00650e93          	addi	t4,a0,6
    1732:	12e79763          	bne	a5,a4,1860 <memset+0x1a2>
    1736:	00750e93          	addi	t4,a0,7
    173a:	00b50323          	sb	a1,6(a0)
    173e:	4f1d                	li	t5,7
    1740:	00859713          	slli	a4,a1,0x8
    1744:	8f4d                	or	a4,a4,a1
    1746:	01059e13          	slli	t3,a1,0x10
    174a:	01c76e33          	or	t3,a4,t3
    174e:	01859313          	slli	t1,a1,0x18
    1752:	006e6333          	or	t1,t3,t1
    1756:	02059893          	slli	a7,a1,0x20
    175a:	011368b3          	or	a7,t1,a7
    175e:	02859813          	slli	a6,a1,0x28
    1762:	40f60333          	sub	t1,a2,a5
    1766:	0108e833          	or	a6,a7,a6
    176a:	03059693          	slli	a3,a1,0x30
    176e:	00d866b3          	or	a3,a6,a3
    1772:	03859713          	slli	a4,a1,0x38
    1776:	97aa                	add	a5,a5,a0
    1778:	ff837813          	andi	a6,t1,-8
    177c:	8f55                	or	a4,a4,a3
    177e:	00f806b3          	add	a3,a6,a5
    1782:	e398                	sd	a4,0(a5)
    1784:	07a1                	addi	a5,a5,8
    1786:	fed79ee3          	bne	a5,a3,1782 <memset+0xc4>
    178a:	ff837693          	andi	a3,t1,-8
    178e:	00de87b3          	add	a5,t4,a3
    1792:	01e6873b          	addw	a4,a3,t5
    1796:	0ad30663          	beq	t1,a3,1842 <memset+0x184>
    179a:	00b78023          	sb	a1,0(a5)
    179e:	0017069b          	addiw	a3,a4,1
    17a2:	08c6fb63          	bgeu	a3,a2,1838 <memset+0x17a>
    17a6:	00b780a3          	sb	a1,1(a5)
    17aa:	0027069b          	addiw	a3,a4,2
    17ae:	08c6f563          	bgeu	a3,a2,1838 <memset+0x17a>
    17b2:	00b78123          	sb	a1,2(a5)
    17b6:	0037069b          	addiw	a3,a4,3
    17ba:	06c6ff63          	bgeu	a3,a2,1838 <memset+0x17a>
    17be:	00b781a3          	sb	a1,3(a5)
    17c2:	0047069b          	addiw	a3,a4,4
    17c6:	06c6f963          	bgeu	a3,a2,1838 <memset+0x17a>
    17ca:	00b78223          	sb	a1,4(a5)
    17ce:	0057069b          	addiw	a3,a4,5
    17d2:	06c6f363          	bgeu	a3,a2,1838 <memset+0x17a>
    17d6:	00b782a3          	sb	a1,5(a5)
    17da:	0067069b          	addiw	a3,a4,6
    17de:	04c6fd63          	bgeu	a3,a2,1838 <memset+0x17a>
    17e2:	00b78323          	sb	a1,6(a5)
    17e6:	0077069b          	addiw	a3,a4,7
    17ea:	04c6f763          	bgeu	a3,a2,1838 <memset+0x17a>
    17ee:	00b783a3          	sb	a1,7(a5)
    17f2:	0087069b          	addiw	a3,a4,8
    17f6:	04c6f163          	bgeu	a3,a2,1838 <memset+0x17a>
    17fa:	00b78423          	sb	a1,8(a5)
    17fe:	0097069b          	addiw	a3,a4,9
    1802:	02c6fb63          	bgeu	a3,a2,1838 <memset+0x17a>
    1806:	00b784a3          	sb	a1,9(a5)
    180a:	00a7069b          	addiw	a3,a4,10
    180e:	02c6f563          	bgeu	a3,a2,1838 <memset+0x17a>
    1812:	00b78523          	sb	a1,10(a5)
    1816:	00b7069b          	addiw	a3,a4,11
    181a:	00c6ff63          	bgeu	a3,a2,1838 <memset+0x17a>
    181e:	00b785a3          	sb	a1,11(a5)
    1822:	00c7069b          	addiw	a3,a4,12
    1826:	00c6f963          	bgeu	a3,a2,1838 <memset+0x17a>
    182a:	00b78623          	sb	a1,12(a5)
    182e:	2735                	addiw	a4,a4,13
    1830:	00c77463          	bgeu	a4,a2,1838 <memset+0x17a>
    1834:	00b786a3          	sb	a1,13(a5)
        ;
    return dest;
}
    1838:	8082                	ret
    183a:	472d                	li	a4,11
    183c:	bd79                	j	16da <memset+0x1c>
    for (int i = 0; i < n; ++i, *(p++) = c)
    183e:	4f0d                	li	t5,3
    1840:	b701                	j	1740 <memset+0x82>
    1842:	8082                	ret
    1844:	4f05                	li	t5,1
    1846:	bded                	j	1740 <memset+0x82>
    1848:	8eaa                	mv	t4,a0
    184a:	4f01                	li	t5,0
    184c:	bdd5                	j	1740 <memset+0x82>
    184e:	87aa                	mv	a5,a0
    1850:	4701                	li	a4,0
    1852:	b7a1                	j	179a <memset+0xdc>
    1854:	4f09                	li	t5,2
    1856:	b5ed                	j	1740 <memset+0x82>
    1858:	4f11                	li	t5,4
    185a:	b5dd                	j	1740 <memset+0x82>
    185c:	4f15                	li	t5,5
    185e:	b5cd                	j	1740 <memset+0x82>
    1860:	4f19                	li	t5,6
    1862:	bdf9                	j	1740 <memset+0x82>

0000000000001864 <strcmp>:

int strcmp(const char *l, const char *r)
{
    for (; *l == *r && *l; l++, r++)
    1864:	00054783          	lbu	a5,0(a0)
    1868:	0005c703          	lbu	a4,0(a1)
    186c:	00e79863          	bne	a5,a4,187c <strcmp+0x18>
    1870:	0505                	addi	a0,a0,1
    1872:	0585                	addi	a1,a1,1
    1874:	fbe5                	bnez	a5,1864 <strcmp>
    1876:	4501                	li	a0,0
        ;
    return *(unsigned char *)l - *(unsigned char *)r;
}
    1878:	9d19                	subw	a0,a0,a4
    187a:	8082                	ret
    return *(unsigned char *)l - *(unsigned char *)r;
    187c:	0007851b          	sext.w	a0,a5
    1880:	bfe5                	j	1878 <strcmp+0x14>

0000000000001882 <strncmp>:

int strncmp(const char *_l, const char *_r, size_t n)
{
    const unsigned char *l = (void *)_l, *r = (void *)_r;
    if (!n--)
    1882:	ce05                	beqz	a2,18ba <strncmp+0x38>
        return 0;
    for (; *l && *r && n && *l == *r; l++, r++, n--)
    1884:	00054703          	lbu	a4,0(a0)
    1888:	0005c783          	lbu	a5,0(a1)
    188c:	cb0d                	beqz	a4,18be <strncmp+0x3c>
    if (!n--)
    188e:	167d                	addi	a2,a2,-1
    1890:	00c506b3          	add	a3,a0,a2
    1894:	a819                	j	18aa <strncmp+0x28>
    for (; *l && *r && n && *l == *r; l++, r++, n--)
    1896:	00a68e63          	beq	a3,a0,18b2 <strncmp+0x30>
    189a:	0505                	addi	a0,a0,1
    189c:	00e79b63          	bne	a5,a4,18b2 <strncmp+0x30>
    18a0:	00054703          	lbu	a4,0(a0)
        ;
    return *l - *r;
    18a4:	0005c783          	lbu	a5,0(a1)
    for (; *l && *r && n && *l == *r; l++, r++, n--)
    18a8:	cb19                	beqz	a4,18be <strncmp+0x3c>
    18aa:	0005c783          	lbu	a5,0(a1)
    18ae:	0585                	addi	a1,a1,1
    18b0:	f3fd                	bnez	a5,1896 <strncmp+0x14>
    return *l - *r;
    18b2:	0007051b          	sext.w	a0,a4
    18b6:	9d1d                	subw	a0,a0,a5
    18b8:	8082                	ret
        return 0;
    18ba:	4501                	li	a0,0
}
    18bc:	8082                	ret
    18be:	4501                	li	a0,0
    return *l - *r;
    18c0:	9d1d                	subw	a0,a0,a5
    18c2:	8082                	ret

00000000000018c4 <strlen>:
size_t strlen(const char *s)
{
    const char *a = s;
    typedef size_t __attribute__((__may_alias__)) word;
    const word *w;
    for (; (uintptr_t)s % SS; s++)
    18c4:	00757793          	andi	a5,a0,7
    18c8:	cf89                	beqz	a5,18e2 <strlen+0x1e>
    18ca:	87aa                	mv	a5,a0
    18cc:	a029                	j	18d6 <strlen+0x12>
    18ce:	0785                	addi	a5,a5,1
    18d0:	0077f713          	andi	a4,a5,7
    18d4:	cb01                	beqz	a4,18e4 <strlen+0x20>
        if (!*s)
    18d6:	0007c703          	lbu	a4,0(a5)
    18da:	fb75                	bnez	a4,18ce <strlen+0xa>
    for (w = (const void *)s; !HASZERO(*w); w++)
        ;
    s = (const void *)w;
    for (; *s; s++)
        ;
    return s - a;
    18dc:	40a78533          	sub	a0,a5,a0
}
    18e0:	8082                	ret
    for (; (uintptr_t)s % SS; s++)
    18e2:	87aa                	mv	a5,a0
    for (w = (const void *)s; !HASZERO(*w); w++)
    18e4:	6394                	ld	a3,0(a5)
    18e6:	00000597          	auipc	a1,0x0
    18ea:	7025b583          	ld	a1,1794(a1) # 1fe8 <__clone+0x10c>
    18ee:	00000617          	auipc	a2,0x0
    18f2:	70263603          	ld	a2,1794(a2) # 1ff0 <__clone+0x114>
    18f6:	a019                	j	18fc <strlen+0x38>
    18f8:	6794                	ld	a3,8(a5)
    18fa:	07a1                	addi	a5,a5,8
    18fc:	00b68733          	add	a4,a3,a1
    1900:	fff6c693          	not	a3,a3
    1904:	8f75                	and	a4,a4,a3
    1906:	8f71                	and	a4,a4,a2
    1908:	db65                	beqz	a4,18f8 <strlen+0x34>
    for (; *s; s++)
    190a:	0007c703          	lbu	a4,0(a5)
    190e:	d779                	beqz	a4,18dc <strlen+0x18>
    1910:	0017c703          	lbu	a4,1(a5)
    1914:	0785                	addi	a5,a5,1
    1916:	d379                	beqz	a4,18dc <strlen+0x18>
    1918:	0017c703          	lbu	a4,1(a5)
    191c:	0785                	addi	a5,a5,1
    191e:	fb6d                	bnez	a4,1910 <strlen+0x4c>
    1920:	bf75                	j	18dc <strlen+0x18>

0000000000001922 <memchr>:

void *memchr(const void *src, int c, size_t n)
{
    const unsigned char *s = src;
    c = (unsigned char)c;
    for (; ((uintptr_t)s & ALIGN) && n && *s != c; s++, n--)
    1922:	00757713          	andi	a4,a0,7
{
    1926:	87aa                	mv	a5,a0
    c = (unsigned char)c;
    1928:	0ff5f593          	andi	a1,a1,255
    for (; ((uintptr_t)s & ALIGN) && n && *s != c; s++, n--)
    192c:	cb19                	beqz	a4,1942 <memchr+0x20>
    192e:	ce25                	beqz	a2,19a6 <memchr+0x84>
    1930:	0007c703          	lbu	a4,0(a5)
    1934:	04b70e63          	beq	a4,a1,1990 <memchr+0x6e>
    1938:	0785                	addi	a5,a5,1
    193a:	0077f713          	andi	a4,a5,7
    193e:	167d                	addi	a2,a2,-1
    1940:	f77d                	bnez	a4,192e <memchr+0xc>
            ;
        s = (const void *)w;
    }
    for (; n && *s != c; s++, n--)
        ;
    return n ? (void *)s : 0;
    1942:	4501                	li	a0,0
    if (n && *s != c)
    1944:	c235                	beqz	a2,19a8 <memchr+0x86>
    1946:	0007c703          	lbu	a4,0(a5)
    194a:	04b70363          	beq	a4,a1,1990 <memchr+0x6e>
        size_t k = ONES * c;
    194e:	00000517          	auipc	a0,0x0
    1952:	6aa53503          	ld	a0,1706(a0) # 1ff8 <__clone+0x11c>
        for (w = (const void *)s; n >= SS && !HASZERO(*w ^ k); w++, n -= SS)
    1956:	471d                	li	a4,7
        size_t k = ONES * c;
    1958:	02a58533          	mul	a0,a1,a0
        for (w = (const void *)s; n >= SS && !HASZERO(*w ^ k); w++, n -= SS)
    195c:	02c77a63          	bgeu	a4,a2,1990 <memchr+0x6e>
    1960:	00000897          	auipc	a7,0x0
    1964:	6888b883          	ld	a7,1672(a7) # 1fe8 <__clone+0x10c>
    1968:	00000817          	auipc	a6,0x0
    196c:	68883803          	ld	a6,1672(a6) # 1ff0 <__clone+0x114>
    1970:	431d                	li	t1,7
    1972:	a029                	j	197c <memchr+0x5a>
    1974:	1661                	addi	a2,a2,-8
    1976:	07a1                	addi	a5,a5,8
    1978:	02c37963          	bgeu	t1,a2,19aa <memchr+0x88>
    197c:	6398                	ld	a4,0(a5)
    197e:	8f29                	xor	a4,a4,a0
    1980:	011706b3          	add	a3,a4,a7
    1984:	fff74713          	not	a4,a4
    1988:	8f75                	and	a4,a4,a3
    198a:	01077733          	and	a4,a4,a6
    198e:	d37d                	beqz	a4,1974 <memchr+0x52>
    1990:	853e                	mv	a0,a5
    1992:	97b2                	add	a5,a5,a2
    1994:	a021                	j	199c <memchr+0x7a>
    for (; n && *s != c; s++, n--)
    1996:	0505                	addi	a0,a0,1
    1998:	00f50763          	beq	a0,a5,19a6 <memchr+0x84>
    199c:	00054703          	lbu	a4,0(a0)
    19a0:	feb71be3          	bne	a4,a1,1996 <memchr+0x74>
    19a4:	8082                	ret
    return n ? (void *)s : 0;
    19a6:	4501                	li	a0,0
}
    19a8:	8082                	ret
    return n ? (void *)s : 0;
    19aa:	4501                	li	a0,0
    for (; n && *s != c; s++, n--)
    19ac:	f275                	bnez	a2,1990 <memchr+0x6e>
}
    19ae:	8082                	ret

00000000000019b0 <strnlen>:

size_t strnlen(const char *s, size_t n)
{
    19b0:	1101                	addi	sp,sp,-32
    19b2:	e822                	sd	s0,16(sp)
    const char *p = memchr(s, 0, n);
    19b4:	862e                	mv	a2,a1
{
    19b6:	842e                	mv	s0,a1
    const char *p = memchr(s, 0, n);
    19b8:	4581                	li	a1,0
{
    19ba:	e426                	sd	s1,8(sp)
    19bc:	ec06                	sd	ra,24(sp)
    19be:	84aa                	mv	s1,a0
    const char *p = memchr(s, 0, n);
    19c0:	f63ff0ef          	jal	ra,1922 <memchr>
    return p ? p - s : n;
    19c4:	c519                	beqz	a0,19d2 <strnlen+0x22>
}
    19c6:	60e2                	ld	ra,24(sp)
    19c8:	6442                	ld	s0,16(sp)
    return p ? p - s : n;
    19ca:	8d05                	sub	a0,a0,s1
}
    19cc:	64a2                	ld	s1,8(sp)
    19ce:	6105                	addi	sp,sp,32
    19d0:	8082                	ret
    19d2:	60e2                	ld	ra,24(sp)
    return p ? p - s : n;
    19d4:	8522                	mv	a0,s0
}
    19d6:	6442                	ld	s0,16(sp)
    19d8:	64a2                	ld	s1,8(sp)
    19da:	6105                	addi	sp,sp,32
    19dc:	8082                	ret

00000000000019de <strcpy>:
char *strcpy(char *restrict d, const char *s)
{
    typedef size_t __attribute__((__may_alias__)) word;
    word *wd;
    const word *ws;
    if ((uintptr_t)s % SS == (uintptr_t)d % SS)
    19de:	00b547b3          	xor	a5,a0,a1
    19e2:	8b9d                	andi	a5,a5,7
    19e4:	eb95                	bnez	a5,1a18 <strcpy+0x3a>
    {
        for (; (uintptr_t)s % SS; s++, d++)
    19e6:	0075f793          	andi	a5,a1,7
    19ea:	e7b1                	bnez	a5,1a36 <strcpy+0x58>
            if (!(*d = *s))
                return d;
        wd = (void *)d;
        ws = (const void *)s;
        for (; !HASZERO(*ws); *wd++ = *ws++)
    19ec:	6198                	ld	a4,0(a1)
    19ee:	00000617          	auipc	a2,0x0
    19f2:	5fa63603          	ld	a2,1530(a2) # 1fe8 <__clone+0x10c>
    19f6:	00000817          	auipc	a6,0x0
    19fa:	5fa83803          	ld	a6,1530(a6) # 1ff0 <__clone+0x114>
    19fe:	a029                	j	1a08 <strcpy+0x2a>
    1a00:	e118                	sd	a4,0(a0)
    1a02:	6598                	ld	a4,8(a1)
    1a04:	05a1                	addi	a1,a1,8
    1a06:	0521                	addi	a0,a0,8
    1a08:	00c707b3          	add	a5,a4,a2
    1a0c:	fff74693          	not	a3,a4
    1a10:	8ff5                	and	a5,a5,a3
    1a12:	0107f7b3          	and	a5,a5,a6
    1a16:	d7ed                	beqz	a5,1a00 <strcpy+0x22>
            ;
        d = (void *)wd;
        s = (const void *)ws;
    }
    for (; (*d = *s); s++, d++)
    1a18:	0005c783          	lbu	a5,0(a1)
    1a1c:	00f50023          	sb	a5,0(a0)
    1a20:	c785                	beqz	a5,1a48 <strcpy+0x6a>
    1a22:	0015c783          	lbu	a5,1(a1)
    1a26:	0505                	addi	a0,a0,1
    1a28:	0585                	addi	a1,a1,1
    1a2a:	00f50023          	sb	a5,0(a0)
    1a2e:	fbf5                	bnez	a5,1a22 <strcpy+0x44>
        ;
    return d;
}
    1a30:	8082                	ret
        for (; (uintptr_t)s % SS; s++, d++)
    1a32:	0505                	addi	a0,a0,1
    1a34:	df45                	beqz	a4,19ec <strcpy+0xe>
            if (!(*d = *s))
    1a36:	0005c783          	lbu	a5,0(a1)
        for (; (uintptr_t)s % SS; s++, d++)
    1a3a:	0585                	addi	a1,a1,1
    1a3c:	0075f713          	andi	a4,a1,7
            if (!(*d = *s))
    1a40:	00f50023          	sb	a5,0(a0)
    1a44:	f7fd                	bnez	a5,1a32 <strcpy+0x54>
}
    1a46:	8082                	ret
    1a48:	8082                	ret

0000000000001a4a <strncpy>:
char *strncpy(char *restrict d, const char *s, size_t n)
{
    typedef size_t __attribute__((__may_alias__)) word;
    word *wd;
    const word *ws;
    if (((uintptr_t)s & ALIGN) == ((uintptr_t)d & ALIGN))
    1a4a:	00b547b3          	xor	a5,a0,a1
    1a4e:	8b9d                	andi	a5,a5,7
    1a50:	1a079863          	bnez	a5,1c00 <strncpy+0x1b6>
    {
        for (; ((uintptr_t)s & ALIGN) && n && (*d = *s); n--, s++, d++)
    1a54:	0075f793          	andi	a5,a1,7
    1a58:	16078463          	beqz	a5,1bc0 <strncpy+0x176>
    1a5c:	ea01                	bnez	a2,1a6c <strncpy+0x22>
    1a5e:	a421                	j	1c66 <strncpy+0x21c>
    1a60:	167d                	addi	a2,a2,-1
    1a62:	0505                	addi	a0,a0,1
    1a64:	14070e63          	beqz	a4,1bc0 <strncpy+0x176>
    1a68:	1a060863          	beqz	a2,1c18 <strncpy+0x1ce>
    1a6c:	0005c783          	lbu	a5,0(a1)
    1a70:	0585                	addi	a1,a1,1
    1a72:	0075f713          	andi	a4,a1,7
    1a76:	00f50023          	sb	a5,0(a0)
    1a7a:	f3fd                	bnez	a5,1a60 <strncpy+0x16>
    1a7c:	4805                	li	a6,1
    1a7e:	1a061863          	bnez	a2,1c2e <strncpy+0x1e4>
    1a82:	40a007b3          	neg	a5,a0
    1a86:	8b9d                	andi	a5,a5,7
    1a88:	4681                	li	a3,0
    1a8a:	18061a63          	bnez	a2,1c1e <strncpy+0x1d4>
    1a8e:	00778713          	addi	a4,a5,7
    1a92:	45ad                	li	a1,11
    1a94:	18b76363          	bltu	a4,a1,1c1a <strncpy+0x1d0>
    1a98:	1ae6eb63          	bltu	a3,a4,1c4e <strncpy+0x204>
    1a9c:	1a078363          	beqz	a5,1c42 <strncpy+0x1f8>
    for (int i = 0; i < n; ++i, *(p++) = c)
    1aa0:	00050023          	sb	zero,0(a0)
    1aa4:	4685                	li	a3,1
    1aa6:	00150713          	addi	a4,a0,1
    1aaa:	18d78f63          	beq	a5,a3,1c48 <strncpy+0x1fe>
    1aae:	000500a3          	sb	zero,1(a0)
    1ab2:	4689                	li	a3,2
    1ab4:	00250713          	addi	a4,a0,2
    1ab8:	18d78e63          	beq	a5,a3,1c54 <strncpy+0x20a>
    1abc:	00050123          	sb	zero,2(a0)
    1ac0:	468d                	li	a3,3
    1ac2:	00350713          	addi	a4,a0,3
    1ac6:	16d78c63          	beq	a5,a3,1c3e <strncpy+0x1f4>
    1aca:	000501a3          	sb	zero,3(a0)
    1ace:	4691                	li	a3,4
    1ad0:	00450713          	addi	a4,a0,4
    1ad4:	18d78263          	beq	a5,a3,1c58 <strncpy+0x20e>
    1ad8:	00050223          	sb	zero,4(a0)
    1adc:	4695                	li	a3,5
    1ade:	00550713          	addi	a4,a0,5
    1ae2:	16d78d63          	beq	a5,a3,1c5c <strncpy+0x212>
    1ae6:	000502a3          	sb	zero,5(a0)
    1aea:	469d                	li	a3,7
    1aec:	00650713          	addi	a4,a0,6
    1af0:	16d79863          	bne	a5,a3,1c60 <strncpy+0x216>
    1af4:	00750713          	addi	a4,a0,7
    1af8:	00050323          	sb	zero,6(a0)
    1afc:	40f80833          	sub	a6,a6,a5
    1b00:	ff887593          	andi	a1,a6,-8
    1b04:	97aa                	add	a5,a5,a0
    1b06:	95be                	add	a1,a1,a5
    1b08:	0007b023          	sd	zero,0(a5)
    1b0c:	07a1                	addi	a5,a5,8
    1b0e:	feb79de3          	bne	a5,a1,1b08 <strncpy+0xbe>
    1b12:	ff887593          	andi	a1,a6,-8
    1b16:	9ead                	addw	a3,a3,a1
    1b18:	00b707b3          	add	a5,a4,a1
    1b1c:	12b80863          	beq	a6,a1,1c4c <strncpy+0x202>
    1b20:	00078023          	sb	zero,0(a5)
    1b24:	0016871b          	addiw	a4,a3,1
    1b28:	0ec77863          	bgeu	a4,a2,1c18 <strncpy+0x1ce>
    1b2c:	000780a3          	sb	zero,1(a5)
    1b30:	0026871b          	addiw	a4,a3,2
    1b34:	0ec77263          	bgeu	a4,a2,1c18 <strncpy+0x1ce>
    1b38:	00078123          	sb	zero,2(a5)
    1b3c:	0036871b          	addiw	a4,a3,3
    1b40:	0cc77c63          	bgeu	a4,a2,1c18 <strncpy+0x1ce>
    1b44:	000781a3          	sb	zero,3(a5)
    1b48:	0046871b          	addiw	a4,a3,4
    1b4c:	0cc77663          	bgeu	a4,a2,1c18 <strncpy+0x1ce>
    1b50:	00078223          	sb	zero,4(a5)
    1b54:	0056871b          	addiw	a4,a3,5
    1b58:	0cc77063          	bgeu	a4,a2,1c18 <strncpy+0x1ce>
    1b5c:	000782a3          	sb	zero,5(a5)
    1b60:	0066871b          	addiw	a4,a3,6
    1b64:	0ac77a63          	bgeu	a4,a2,1c18 <strncpy+0x1ce>
    1b68:	00078323          	sb	zero,6(a5)
    1b6c:	0076871b          	addiw	a4,a3,7
    1b70:	0ac77463          	bgeu	a4,a2,1c18 <strncpy+0x1ce>
    1b74:	000783a3          	sb	zero,7(a5)
    1b78:	0086871b          	addiw	a4,a3,8
    1b7c:	08c77e63          	bgeu	a4,a2,1c18 <strncpy+0x1ce>
    1b80:	00078423          	sb	zero,8(a5)
    1b84:	0096871b          	addiw	a4,a3,9
    1b88:	08c77863          	bgeu	a4,a2,1c18 <strncpy+0x1ce>
    1b8c:	000784a3          	sb	zero,9(a5)
    1b90:	00a6871b          	addiw	a4,a3,10
    1b94:	08c77263          	bgeu	a4,a2,1c18 <strncpy+0x1ce>
    1b98:	00078523          	sb	zero,10(a5)
    1b9c:	00b6871b          	addiw	a4,a3,11
    1ba0:	06c77c63          	bgeu	a4,a2,1c18 <strncpy+0x1ce>
    1ba4:	000785a3          	sb	zero,11(a5)
    1ba8:	00c6871b          	addiw	a4,a3,12
    1bac:	06c77663          	bgeu	a4,a2,1c18 <strncpy+0x1ce>
    1bb0:	00078623          	sb	zero,12(a5)
    1bb4:	26b5                	addiw	a3,a3,13
    1bb6:	06c6f163          	bgeu	a3,a2,1c18 <strncpy+0x1ce>
    1bba:	000786a3          	sb	zero,13(a5)
    1bbe:	8082                	ret
            ;
        if (!n || !*s)
    1bc0:	c645                	beqz	a2,1c68 <strncpy+0x21e>
    1bc2:	0005c783          	lbu	a5,0(a1)
    1bc6:	ea078be3          	beqz	a5,1a7c <strncpy+0x32>
            goto tail;
        wd = (void *)d;
        ws = (const void *)s;
        for (; n >= sizeof(size_t) && !HASZERO(*ws); n -= sizeof(size_t), ws++, wd++)
    1bca:	479d                	li	a5,7
    1bcc:	02c7ff63          	bgeu	a5,a2,1c0a <strncpy+0x1c0>
    1bd0:	00000897          	auipc	a7,0x0
    1bd4:	4188b883          	ld	a7,1048(a7) # 1fe8 <__clone+0x10c>
    1bd8:	00000817          	auipc	a6,0x0
    1bdc:	41883803          	ld	a6,1048(a6) # 1ff0 <__clone+0x114>
    1be0:	431d                	li	t1,7
    1be2:	6198                	ld	a4,0(a1)
    1be4:	011707b3          	add	a5,a4,a7
    1be8:	fff74693          	not	a3,a4
    1bec:	8ff5                	and	a5,a5,a3
    1bee:	0107f7b3          	and	a5,a5,a6
    1bf2:	ef81                	bnez	a5,1c0a <strncpy+0x1c0>
            *wd = *ws;
    1bf4:	e118                	sd	a4,0(a0)
        for (; n >= sizeof(size_t) && !HASZERO(*ws); n -= sizeof(size_t), ws++, wd++)
    1bf6:	1661                	addi	a2,a2,-8
    1bf8:	05a1                	addi	a1,a1,8
    1bfa:	0521                	addi	a0,a0,8
    1bfc:	fec363e3          	bltu	t1,a2,1be2 <strncpy+0x198>
        d = (void *)wd;
        s = (const void *)ws;
    }
    for (; n && (*d = *s); n--, s++, d++)
    1c00:	e609                	bnez	a2,1c0a <strncpy+0x1c0>
    1c02:	a08d                	j	1c64 <strncpy+0x21a>
    1c04:	167d                	addi	a2,a2,-1
    1c06:	0505                	addi	a0,a0,1
    1c08:	ca01                	beqz	a2,1c18 <strncpy+0x1ce>
    1c0a:	0005c783          	lbu	a5,0(a1)
    1c0e:	0585                	addi	a1,a1,1
    1c10:	00f50023          	sb	a5,0(a0)
    1c14:	fbe5                	bnez	a5,1c04 <strncpy+0x1ba>
        ;
tail:
    1c16:	b59d                	j	1a7c <strncpy+0x32>
    memset(d, 0, n);
    return d;
}
    1c18:	8082                	ret
    1c1a:	472d                	li	a4,11
    1c1c:	bdb5                	j	1a98 <strncpy+0x4e>
    1c1e:	00778713          	addi	a4,a5,7
    1c22:	45ad                	li	a1,11
    1c24:	fff60693          	addi	a3,a2,-1
    1c28:	e6b778e3          	bgeu	a4,a1,1a98 <strncpy+0x4e>
    1c2c:	b7fd                	j	1c1a <strncpy+0x1d0>
    1c2e:	40a007b3          	neg	a5,a0
    1c32:	8832                	mv	a6,a2
    1c34:	8b9d                	andi	a5,a5,7
    1c36:	4681                	li	a3,0
    1c38:	e4060be3          	beqz	a2,1a8e <strncpy+0x44>
    1c3c:	b7cd                	j	1c1e <strncpy+0x1d4>
    for (int i = 0; i < n; ++i, *(p++) = c)
    1c3e:	468d                	li	a3,3
    1c40:	bd75                	j	1afc <strncpy+0xb2>
    1c42:	872a                	mv	a4,a0
    1c44:	4681                	li	a3,0
    1c46:	bd5d                	j	1afc <strncpy+0xb2>
    1c48:	4685                	li	a3,1
    1c4a:	bd4d                	j	1afc <strncpy+0xb2>
    1c4c:	8082                	ret
    1c4e:	87aa                	mv	a5,a0
    1c50:	4681                	li	a3,0
    1c52:	b5f9                	j	1b20 <strncpy+0xd6>
    1c54:	4689                	li	a3,2
    1c56:	b55d                	j	1afc <strncpy+0xb2>
    1c58:	4691                	li	a3,4
    1c5a:	b54d                	j	1afc <strncpy+0xb2>
    1c5c:	4695                	li	a3,5
    1c5e:	bd79                	j	1afc <strncpy+0xb2>
    1c60:	4699                	li	a3,6
    1c62:	bd69                	j	1afc <strncpy+0xb2>
    1c64:	8082                	ret
    1c66:	8082                	ret
    1c68:	8082                	ret

0000000000001c6a <open>:
#include <unistd.h>

#include "syscall.h"

int open(const char *path, int flags)
{
    1c6a:	87aa                	mv	a5,a0
    1c6c:	862e                	mv	a2,a1
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
}

static inline long __syscall4(long n, long a, long b, long c, long d)
{
    register long a7 __asm__("a7") = n;
    1c6e:	03800893          	li	a7,56
    register long a0 __asm__("a0") = a;
    1c72:	f9c00513          	li	a0,-100
    register long a1 __asm__("a1") = b;
    1c76:	85be                	mv	a1,a5
    register long a2 __asm__("a2") = c;
    register long a3 __asm__("a3") = d;
    1c78:	4689                	li	a3,2
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3))
    1c7a:	00000073          	ecall
    return syscall(SYS_openat, AT_FDCWD, path, flags, O_RDWR);
}
    1c7e:	2501                	sext.w	a0,a0
    1c80:	8082                	ret

0000000000001c82 <openat>:
    register long a7 __asm__("a7") = n;
    1c82:	03800893          	li	a7,56
    register long a3 __asm__("a3") = d;
    1c86:	18000693          	li	a3,384
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3))
    1c8a:	00000073          	ecall

int openat(int dirfd,const char *path, int flags)
{
    return syscall(SYS_openat, dirfd, path, flags, 0600);
}
    1c8e:	2501                	sext.w	a0,a0
    1c90:	8082                	ret

0000000000001c92 <close>:
    register long a7 __asm__("a7") = n;
    1c92:	03900893          	li	a7,57
    __asm_syscall("r"(a7), "0"(a0))
    1c96:	00000073          	ecall

int close(int fd)
{
    return syscall(SYS_close, fd);
}
    1c9a:	2501                	sext.w	a0,a0
    1c9c:	8082                	ret

0000000000001c9e <read>:
    register long a7 __asm__("a7") = n;
    1c9e:	03f00893          	li	a7,63
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1ca2:	00000073          	ecall

ssize_t read(int fd, void *buf, size_t len)
{
    return syscall(SYS_read, fd, buf, len);
}
    1ca6:	8082                	ret

0000000000001ca8 <write>:
    register long a7 __asm__("a7") = n;
    1ca8:	04000893          	li	a7,64
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1cac:	00000073          	ecall

ssize_t write(int fd, const void *buf, size_t len)
{
    return syscall(SYS_write, fd, buf, len);
}
    1cb0:	8082                	ret

0000000000001cb2 <getpid>:
    register long a7 __asm__("a7") = n;
    1cb2:	0ac00893          	li	a7,172
    __asm_syscall("r"(a7))
    1cb6:	00000073          	ecall

pid_t getpid(void)
{
    return syscall(SYS_getpid);
}
    1cba:	2501                	sext.w	a0,a0
    1cbc:	8082                	ret

0000000000001cbe <getppid>:
    register long a7 __asm__("a7") = n;
    1cbe:	0ad00893          	li	a7,173
    __asm_syscall("r"(a7))
    1cc2:	00000073          	ecall

pid_t getppid(void)
{
    return syscall(SYS_getppid);
}
    1cc6:	2501                	sext.w	a0,a0
    1cc8:	8082                	ret

0000000000001cca <sched_yield>:
    register long a7 __asm__("a7") = n;
    1cca:	07c00893          	li	a7,124
    __asm_syscall("r"(a7))
    1cce:	00000073          	ecall

int sched_yield(void)
{
    return syscall(SYS_sched_yield);
}
    1cd2:	2501                	sext.w	a0,a0
    1cd4:	8082                	ret

0000000000001cd6 <fork>:
    register long a7 __asm__("a7") = n;
    1cd6:	0dc00893          	li	a7,220
    register long a0 __asm__("a0") = a;
    1cda:	4545                	li	a0,17
    register long a1 __asm__("a1") = b;
    1cdc:	4581                	li	a1,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1cde:	00000073          	ecall

pid_t fork(void)
{
    return syscall(SYS_clone, SIGCHLD, 0);
}
    1ce2:	2501                	sext.w	a0,a0
    1ce4:	8082                	ret

0000000000001ce6 <clone>:

pid_t clone(int (*fn)(void *arg), void *arg, void *stack, size_t stack_size, unsigned long flags)
{
    1ce6:	85b2                	mv	a1,a2
    1ce8:	863a                	mv	a2,a4
    if (stack)
    1cea:	c191                	beqz	a1,1cee <clone+0x8>
	stack += stack_size;
    1cec:	95b6                	add	a1,a1,a3

    return __clone(fn, stack, flags, NULL, NULL, NULL);
    1cee:	4781                	li	a5,0
    1cf0:	4701                	li	a4,0
    1cf2:	4681                	li	a3,0
    1cf4:	2601                	sext.w	a2,a2
    1cf6:	a2dd                	j	1edc <__clone>

0000000000001cf8 <exit>:
    register long a7 __asm__("a7") = n;
    1cf8:	05d00893          	li	a7,93
    __asm_syscall("r"(a7), "0"(a0))
    1cfc:	00000073          	ecall
    //return syscall(SYS_clone, fn, stack, flags, NULL, NULL, NULL);
}
void exit(int code)
{
    syscall(SYS_exit, code);
}
    1d00:	8082                	ret

0000000000001d02 <waitpid>:
    register long a7 __asm__("a7") = n;
    1d02:	10400893          	li	a7,260
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1d06:	00000073          	ecall

int waitpid(int pid, int *code, int options)
{
    return syscall(SYS_wait4, pid, code, options);
}
    1d0a:	2501                	sext.w	a0,a0
    1d0c:	8082                	ret

0000000000001d0e <exec>:
    register long a7 __asm__("a7") = n;
    1d0e:	0dd00893          	li	a7,221
    __asm_syscall("r"(a7), "0"(a0))
    1d12:	00000073          	ecall

int exec(char *name)
{
    return syscall(SYS_execve, name);
}
    1d16:	2501                	sext.w	a0,a0
    1d18:	8082                	ret

0000000000001d1a <execve>:
    register long a7 __asm__("a7") = n;
    1d1a:	0dd00893          	li	a7,221
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1d1e:	00000073          	ecall

int execve(const char *name, char *const argv[], char *const argp[])
{
    return syscall(SYS_execve, name, argv, argp);
}
    1d22:	2501                	sext.w	a0,a0
    1d24:	8082                	ret

0000000000001d26 <times>:
    register long a7 __asm__("a7") = n;
    1d26:	09900893          	li	a7,153
    __asm_syscall("r"(a7), "0"(a0))
    1d2a:	00000073          	ecall

int times(void *mytimes)
{
	return syscall(SYS_times, mytimes);
}
    1d2e:	2501                	sext.w	a0,a0
    1d30:	8082                	ret

0000000000001d32 <get_time>:

int64 get_time()
{
    1d32:	1141                	addi	sp,sp,-16
    register long a7 __asm__("a7") = n;
    1d34:	0a900893          	li	a7,169
    register long a0 __asm__("a0") = a;
    1d38:	850a                	mv	a0,sp
    register long a1 __asm__("a1") = b;
    1d3a:	4581                	li	a1,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1d3c:	00000073          	ecall
    TimeVal time;
    int err = sys_get_time(&time, 0);
    if (err == 0)
    1d40:	2501                	sext.w	a0,a0
    1d42:	ed09                	bnez	a0,1d5c <get_time+0x2a>
    {
        return ((time.sec & 0xffff) * 1000 + time.usec / 1000);
    1d44:	67a2                	ld	a5,8(sp)
    1d46:	3e800713          	li	a4,1000
    1d4a:	00015503          	lhu	a0,0(sp)
    1d4e:	02e7d7b3          	divu	a5,a5,a4
    1d52:	02e50533          	mul	a0,a0,a4
    1d56:	953e                	add	a0,a0,a5
    }
    else
    {
        return -1;
    }
}
    1d58:	0141                	addi	sp,sp,16
    1d5a:	8082                	ret
        return -1;
    1d5c:	557d                	li	a0,-1
    1d5e:	bfed                	j	1d58 <get_time+0x26>

0000000000001d60 <sys_get_time>:
    register long a7 __asm__("a7") = n;
    1d60:	0a900893          	li	a7,169
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1d64:	00000073          	ecall

int sys_get_time(TimeVal *ts, int tz)
{
    return syscall(SYS_gettimeofday, ts, tz);
}
    1d68:	2501                	sext.w	a0,a0
    1d6a:	8082                	ret

0000000000001d6c <time>:
    register long a7 __asm__("a7") = n;
    1d6c:	42600893          	li	a7,1062
    __asm_syscall("r"(a7), "0"(a0))
    1d70:	00000073          	ecall

int time(unsigned long *tloc)
{
    return syscall(SYS_time, tloc);
}
    1d74:	2501                	sext.w	a0,a0
    1d76:	8082                	ret

0000000000001d78 <sleep>:

int sleep(unsigned long long time)
{
    1d78:	1141                	addi	sp,sp,-16
    TimeVal tv = {.sec = time, .usec = 0};
    1d7a:	e02a                	sd	a0,0(sp)
    register long a0 __asm__("a0") = a;
    1d7c:	850a                	mv	a0,sp
    1d7e:	e402                	sd	zero,8(sp)
    register long a7 __asm__("a7") = n;
    1d80:	06500893          	li	a7,101
    register long a1 __asm__("a1") = b;
    1d84:	85aa                	mv	a1,a0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1d86:	00000073          	ecall
    if (syscall(SYS_nanosleep, &tv, &tv)) return tv.sec;
    1d8a:	e501                	bnez	a0,1d92 <sleep+0x1a>
    return 0;
    1d8c:	4501                	li	a0,0
}
    1d8e:	0141                	addi	sp,sp,16
    1d90:	8082                	ret
    if (syscall(SYS_nanosleep, &tv, &tv)) return tv.sec;
    1d92:	4502                	lw	a0,0(sp)
}
    1d94:	0141                	addi	sp,sp,16
    1d96:	8082                	ret

0000000000001d98 <set_priority>:
    register long a7 __asm__("a7") = n;
    1d98:	08c00893          	li	a7,140
    __asm_syscall("r"(a7), "0"(a0))
    1d9c:	00000073          	ecall

int set_priority(int prio)
{
    return syscall(SYS_setpriority, prio);
}
    1da0:	2501                	sext.w	a0,a0
    1da2:	8082                	ret

0000000000001da4 <mmap>:
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4))
}

static inline long __syscall6(long n, long a, long b, long c, long d, long e, long f)
{
    register long a7 __asm__("a7") = n;
    1da4:	0de00893          	li	a7,222
    register long a1 __asm__("a1") = b;
    register long a2 __asm__("a2") = c;
    register long a3 __asm__("a3") = d;
    register long a4 __asm__("a4") = e;
    register long a5 __asm__("a5") = f;
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4), "r"(a5))
    1da8:	00000073          	ecall

void *mmap(void *start, size_t len, int prot, int flags, int fd, off_t off)
{
    return syscall(SYS_mmap, start, len, prot, flags, fd, off);
}
    1dac:	8082                	ret

0000000000001dae <munmap>:
    register long a7 __asm__("a7") = n;
    1dae:	0d700893          	li	a7,215
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1db2:	00000073          	ecall

int munmap(void *start, size_t len)
{
    return syscall(SYS_munmap, start, len);
}
    1db6:	2501                	sext.w	a0,a0
    1db8:	8082                	ret

0000000000001dba <wait>:

int wait(int *code)
{
    1dba:	85aa                	mv	a1,a0
    register long a7 __asm__("a7") = n;
    1dbc:	10400893          	li	a7,260
    register long a0 __asm__("a0") = a;
    1dc0:	557d                	li	a0,-1
    register long a2 __asm__("a2") = c;
    1dc2:	4601                	li	a2,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1dc4:	00000073          	ecall
    return waitpid(-1, code, 0);
}
    1dc8:	2501                	sext.w	a0,a0
    1dca:	8082                	ret

0000000000001dcc <spawn>:
    register long a7 __asm__("a7") = n;
    1dcc:	19000893          	li	a7,400
    __asm_syscall("r"(a7), "0"(a0))
    1dd0:	00000073          	ecall

int spawn(char *file)
{
    return syscall(SYS_spawn, file);
}
    1dd4:	2501                	sext.w	a0,a0
    1dd6:	8082                	ret

0000000000001dd8 <mailread>:
    register long a7 __asm__("a7") = n;
    1dd8:	19100893          	li	a7,401
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1ddc:	00000073          	ecall

int mailread(void *buf, int len)
{
    return syscall(SYS_mailread, buf, len);
}
    1de0:	2501                	sext.w	a0,a0
    1de2:	8082                	ret

0000000000001de4 <mailwrite>:
    register long a7 __asm__("a7") = n;
    1de4:	19200893          	li	a7,402
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1de8:	00000073          	ecall

int mailwrite(int pid, void *buf, int len)
{
    return syscall(SYS_mailwrite, pid, buf, len);
}
    1dec:	2501                	sext.w	a0,a0
    1dee:	8082                	ret

0000000000001df0 <fstat>:
    register long a7 __asm__("a7") = n;
    1df0:	05000893          	li	a7,80
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1df4:	00000073          	ecall

int fstat(int fd, struct kstat *st)
{
    return syscall(SYS_fstat, fd, st);
}
    1df8:	2501                	sext.w	a0,a0
    1dfa:	8082                	ret

0000000000001dfc <sys_linkat>:
    register long a4 __asm__("a4") = e;
    1dfc:	1702                	slli	a4,a4,0x20
    register long a7 __asm__("a7") = n;
    1dfe:	02500893          	li	a7,37
    register long a4 __asm__("a4") = e;
    1e02:	9301                	srli	a4,a4,0x20
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4))
    1e04:	00000073          	ecall

int sys_linkat(int olddirfd, char *oldpath, int newdirfd, char *newpath, unsigned int flags)
{
    return syscall(SYS_linkat, olddirfd, oldpath, newdirfd, newpath, flags);
}
    1e08:	2501                	sext.w	a0,a0
    1e0a:	8082                	ret

0000000000001e0c <sys_unlinkat>:
    register long a2 __asm__("a2") = c;
    1e0c:	1602                	slli	a2,a2,0x20
    register long a7 __asm__("a7") = n;
    1e0e:	02300893          	li	a7,35
    register long a2 __asm__("a2") = c;
    1e12:	9201                	srli	a2,a2,0x20
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1e14:	00000073          	ecall

int sys_unlinkat(int dirfd, char *path, unsigned int flags)
{
    return syscall(SYS_unlinkat, dirfd, path, flags);
}
    1e18:	2501                	sext.w	a0,a0
    1e1a:	8082                	ret

0000000000001e1c <link>:

int link(char *old_path, char *new_path)
{
    1e1c:	87aa                	mv	a5,a0
    1e1e:	86ae                	mv	a3,a1
    register long a7 __asm__("a7") = n;
    1e20:	02500893          	li	a7,37
    register long a0 __asm__("a0") = a;
    1e24:	f9c00513          	li	a0,-100
    register long a1 __asm__("a1") = b;
    1e28:	85be                	mv	a1,a5
    register long a2 __asm__("a2") = c;
    1e2a:	f9c00613          	li	a2,-100
    register long a4 __asm__("a4") = e;
    1e2e:	4701                	li	a4,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4))
    1e30:	00000073          	ecall
    return sys_linkat(AT_FDCWD, old_path, AT_FDCWD, new_path, 0);
}
    1e34:	2501                	sext.w	a0,a0
    1e36:	8082                	ret

0000000000001e38 <unlink>:

int unlink(char *path)
{
    1e38:	85aa                	mv	a1,a0
    register long a7 __asm__("a7") = n;
    1e3a:	02300893          	li	a7,35
    register long a0 __asm__("a0") = a;
    1e3e:	f9c00513          	li	a0,-100
    register long a2 __asm__("a2") = c;
    1e42:	4601                	li	a2,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1e44:	00000073          	ecall
    return sys_unlinkat(AT_FDCWD, path, 0);
}
    1e48:	2501                	sext.w	a0,a0
    1e4a:	8082                	ret

0000000000001e4c <uname>:
    register long a7 __asm__("a7") = n;
    1e4c:	0a000893          	li	a7,160
    __asm_syscall("r"(a7), "0"(a0))
    1e50:	00000073          	ecall

int uname(void *buf)
{
    return syscall(SYS_uname, buf);
}
    1e54:	2501                	sext.w	a0,a0
    1e56:	8082                	ret

0000000000001e58 <brk>:
    register long a7 __asm__("a7") = n;
    1e58:	0d600893          	li	a7,214
    __asm_syscall("r"(a7), "0"(a0))
    1e5c:	00000073          	ecall

int brk(void *addr)
{
    return syscall(SYS_brk, addr);
}
    1e60:	2501                	sext.w	a0,a0
    1e62:	8082                	ret

0000000000001e64 <getcwd>:
    register long a7 __asm__("a7") = n;
    1e64:	48c5                	li	a7,17
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1e66:	00000073          	ecall

char *getcwd(char *buf, size_t size){
    return syscall(SYS_getcwd, buf, size);
}
    1e6a:	8082                	ret

0000000000001e6c <chdir>:
    register long a7 __asm__("a7") = n;
    1e6c:	03100893          	li	a7,49
    __asm_syscall("r"(a7), "0"(a0))
    1e70:	00000073          	ecall

int chdir(const char *path){
    return syscall(SYS_chdir, path);
}
    1e74:	2501                	sext.w	a0,a0
    1e76:	8082                	ret

0000000000001e78 <mkdir>:

int mkdir(const char *path, mode_t mode){
    1e78:	862e                	mv	a2,a1
    1e7a:	87aa                	mv	a5,a0
    register long a2 __asm__("a2") = c;
    1e7c:	1602                	slli	a2,a2,0x20
    register long a7 __asm__("a7") = n;
    1e7e:	02200893          	li	a7,34
    register long a0 __asm__("a0") = a;
    1e82:	f9c00513          	li	a0,-100
    register long a1 __asm__("a1") = b;
    1e86:	85be                	mv	a1,a5
    register long a2 __asm__("a2") = c;
    1e88:	9201                	srli	a2,a2,0x20
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1e8a:	00000073          	ecall
    return syscall(SYS_mkdirat, AT_FDCWD, path, mode);
}
    1e8e:	2501                	sext.w	a0,a0
    1e90:	8082                	ret

0000000000001e92 <getdents>:
    register long a7 __asm__("a7") = n;
    1e92:	03d00893          	li	a7,61
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1e96:	00000073          	ecall

int getdents(int fd, struct linux_dirent64 *dirp64, unsigned long len){
    //return syscall(SYS_getdents64, fd, dirp64, len);
    return syscall(SYS_getdents64, fd, dirp64, len);
}
    1e9a:	2501                	sext.w	a0,a0
    1e9c:	8082                	ret

0000000000001e9e <pipe>:
    register long a7 __asm__("a7") = n;
    1e9e:	03b00893          	li	a7,59
    register long a1 __asm__("a1") = b;
    1ea2:	4581                	li	a1,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1ea4:	00000073          	ecall

int pipe(int fd[2]){
    return syscall(SYS_pipe2, fd, 0);
}
    1ea8:	2501                	sext.w	a0,a0
    1eaa:	8082                	ret

0000000000001eac <dup>:
    register long a7 __asm__("a7") = n;
    1eac:	48dd                	li	a7,23
    __asm_syscall("r"(a7), "0"(a0))
    1eae:	00000073          	ecall

int dup(int fd){
    return syscall(SYS_dup, fd);
}
    1eb2:	2501                	sext.w	a0,a0
    1eb4:	8082                	ret

0000000000001eb6 <dup2>:
    register long a7 __asm__("a7") = n;
    1eb6:	48e1                	li	a7,24
    register long a2 __asm__("a2") = c;
    1eb8:	4601                	li	a2,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1eba:	00000073          	ecall

int dup2(int old, int new){
    return syscall(SYS_dup3, old, new, 0);
}
    1ebe:	2501                	sext.w	a0,a0
    1ec0:	8082                	ret

0000000000001ec2 <mount>:
    register long a7 __asm__("a7") = n;
    1ec2:	02800893          	li	a7,40
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4))
    1ec6:	00000073          	ecall

int mount(const char *special, const char *dir, const char *fstype, unsigned long flags, const void *data)
{
        return syscall(SYS_mount, special, dir, fstype, flags, data);
}
    1eca:	2501                	sext.w	a0,a0
    1ecc:	8082                	ret

0000000000001ece <umount>:
    register long a7 __asm__("a7") = n;
    1ece:	02700893          	li	a7,39
    register long a1 __asm__("a1") = b;
    1ed2:	4581                	li	a1,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1ed4:	00000073          	ecall

int umount(const char *special)
{
        return syscall(SYS_umount2, special, 0);
}
    1ed8:	2501                	sext.w	a0,a0
    1eda:	8082                	ret

0000000000001edc <__clone>:

.global __clone
.type  __clone, %function
__clone:
	# Save func and arg to stack
	addi a1, a1, -16
    1edc:	15c1                	addi	a1,a1,-16
	sd a0, 0(a1)
    1ede:	e188                	sd	a0,0(a1)
	sd a3, 8(a1)
    1ee0:	e594                	sd	a3,8(a1)

	# Call SYS_clone
	mv a0, a2
    1ee2:	8532                	mv	a0,a2
	mv a2, a4
    1ee4:	863a                	mv	a2,a4
	mv a3, a5
    1ee6:	86be                	mv	a3,a5
	mv a4, a6
    1ee8:	8742                	mv	a4,a6
	li a7, 220 # SYS_clone
    1eea:	0dc00893          	li	a7,220
	ecall
    1eee:	00000073          	ecall

	beqz a0, 1f
    1ef2:	c111                	beqz	a0,1ef6 <__clone+0x1a>
	# Parent
	ret
    1ef4:	8082                	ret

	# Child
1:      ld a1, 0(sp)
    1ef6:	6582                	ld	a1,0(sp)
	ld a0, 8(sp)
    1ef8:	6522                	ld	a0,8(sp)
	jalr a1
    1efa:	9582                	jalr	a1

	# Exit
	li a7, 93 # SYS_exit
    1efc:	05d00893          	li	a7,93
	ecall
    1f00:	00000073          	ecall
