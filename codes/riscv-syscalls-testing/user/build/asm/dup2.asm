
/home/ubuntu/rust/ultra-os/codes/riscv-syscalls-testing/user/build/riscv64/dup2:     file format elf64-littleriscv


Disassembly of section .text:

0000000000001000 <_start>:
.section .text.entry
.globl _start
_start:
    mv a0, sp
    1000:	850a                	mv	a0,sp
    tail __start_main
    1002:	a871                	j	109e <__start_main>

0000000000001004 <test_dup2>:

/*
 * 测试通过时应输出：
 * "  from fd 100"
 */
void test_dup2(){
    1004:	1141                	addi	sp,sp,-16
	TEST_START(__func__);
    1006:	00001517          	auipc	a0,0x1
    100a:	ed250513          	addi	a0,a0,-302 # 1ed8 <__clone+0x2c>
void test_dup2(){
    100e:	e406                	sd	ra,8(sp)
	TEST_START(__func__);
    1010:	300000ef          	jal	ra,1310 <puts>
    1014:	00001517          	auipc	a0,0x1
    1018:	f4c50513          	addi	a0,a0,-180 # 1f60 <__func__.0>
    101c:	2f4000ef          	jal	ra,1310 <puts>
    1020:	00001517          	auipc	a0,0x1
    1024:	ed050513          	addi	a0,a0,-304 # 1ef0 <__clone+0x44>
    1028:	2e8000ef          	jal	ra,1310 <puts>
	int fd = dup2(STDOUT, 100);
    102c:	06400593          	li	a1,100
    1030:	4505                	li	a0,1
    1032:	655000ef          	jal	ra,1e86 <dup2>
	assert(fd != -1);
    1036:	57fd                	li	a5,-1
    1038:	04f50463          	beq	a0,a5,1080 <test_dup2+0x7c>
	const char *str = "  from fd 100\n";
	write(100, str, strlen(str));
    103c:	00001517          	auipc	a0,0x1
    1040:	ee450513          	addi	a0,a0,-284 # 1f20 <__clone+0x74>
    1044:	051000ef          	jal	ra,1894 <strlen>
    1048:	862a                	mv	a2,a0
    104a:	00001597          	auipc	a1,0x1
    104e:	ed658593          	addi	a1,a1,-298 # 1f20 <__clone+0x74>
    1052:	06400513          	li	a0,100
    1056:	423000ef          	jal	ra,1c78 <write>
	TEST_END(__func__);
    105a:	00001517          	auipc	a0,0x1
    105e:	ed650513          	addi	a0,a0,-298 # 1f30 <__clone+0x84>
    1062:	2ae000ef          	jal	ra,1310 <puts>
    1066:	00001517          	auipc	a0,0x1
    106a:	efa50513          	addi	a0,a0,-262 # 1f60 <__func__.0>
    106e:	2a2000ef          	jal	ra,1310 <puts>
}
    1072:	60a2                	ld	ra,8(sp)
	TEST_END(__func__);
    1074:	00001517          	auipc	a0,0x1
    1078:	e7c50513          	addi	a0,a0,-388 # 1ef0 <__clone+0x44>
}
    107c:	0141                	addi	sp,sp,16
	TEST_END(__func__);
    107e:	ac49                	j	1310 <puts>
	assert(fd != -1);
    1080:	00001517          	auipc	a0,0x1
    1084:	e8050513          	addi	a0,a0,-384 # 1f00 <__clone+0x54>
    1088:	52e000ef          	jal	ra,15b6 <panic>
    108c:	bf45                	j	103c <test_dup2+0x38>

000000000000108e <main>:

int main(void) {
    108e:	1141                	addi	sp,sp,-16
    1090:	e406                	sd	ra,8(sp)
	test_dup2();
    1092:	f73ff0ef          	jal	ra,1004 <test_dup2>
	return 0;
}
    1096:	60a2                	ld	ra,8(sp)
    1098:	4501                	li	a0,0
    109a:	0141                	addi	sp,sp,16
    109c:	8082                	ret

000000000000109e <__start_main>:
#include <unistd.h>

extern int main();

int __start_main(long *p)
{
    109e:	85aa                	mv	a1,a0
	int argc = p[0];
	char **argv = (void *)(p+1);

	exit(main(argc, argv));
    10a0:	4108                	lw	a0,0(a0)
{
    10a2:	1141                	addi	sp,sp,-16
	exit(main(argc, argv));
    10a4:	05a1                	addi	a1,a1,8
{
    10a6:	e406                	sd	ra,8(sp)
	exit(main(argc, argv));
    10a8:	fe7ff0ef          	jal	ra,108e <main>
    10ac:	41d000ef          	jal	ra,1cc8 <exit>
	return 0;
}
    10b0:	60a2                	ld	ra,8(sp)
    10b2:	4501                	li	a0,0
    10b4:	0141                	addi	sp,sp,16
    10b6:	8082                	ret

00000000000010b8 <printint.constprop.0>:
    write(f, s, l);
}

static char digits[] = "0123456789abcdef";

static void printint(int xx, int base, int sign)
    10b8:	7179                	addi	sp,sp,-48
    10ba:	f406                	sd	ra,40(sp)
{
    char buf[16 + 1];
    int i;
    uint x;

    if (sign && (sign = xx < 0))
    10bc:	12054b63          	bltz	a0,11f2 <printint.constprop.0+0x13a>

    buf[16] = 0;
    i = 15;
    do
    {
        buf[i--] = digits[x % base];
    10c0:	02b577bb          	remuw	a5,a0,a1
    10c4:	00001617          	auipc	a2,0x1
    10c8:	eac60613          	addi	a2,a2,-340 # 1f70 <digits>
    buf[16] = 0;
    10cc:	00010c23          	sb	zero,24(sp)
        buf[i--] = digits[x % base];
    10d0:	0005871b          	sext.w	a4,a1
    10d4:	1782                	slli	a5,a5,0x20
    10d6:	9381                	srli	a5,a5,0x20
    10d8:	97b2                	add	a5,a5,a2
    10da:	0007c783          	lbu	a5,0(a5)
    } while ((x /= base) != 0);
    10de:	02b5583b          	divuw	a6,a0,a1
        buf[i--] = digits[x % base];
    10e2:	00f10ba3          	sb	a5,23(sp)
    } while ((x /= base) != 0);
    10e6:	1cb56363          	bltu	a0,a1,12ac <printint.constprop.0+0x1f4>
        buf[i--] = digits[x % base];
    10ea:	45b9                	li	a1,14
    10ec:	02e877bb          	remuw	a5,a6,a4
    10f0:	1782                	slli	a5,a5,0x20
    10f2:	9381                	srli	a5,a5,0x20
    10f4:	97b2                	add	a5,a5,a2
    10f6:	0007c783          	lbu	a5,0(a5)
    } while ((x /= base) != 0);
    10fa:	02e856bb          	divuw	a3,a6,a4
        buf[i--] = digits[x % base];
    10fe:	00f10b23          	sb	a5,22(sp)
    } while ((x /= base) != 0);
    1102:	0ce86e63          	bltu	a6,a4,11de <printint.constprop.0+0x126>
        buf[i--] = digits[x % base];
    1106:	02e6f5bb          	remuw	a1,a3,a4
    } while ((x /= base) != 0);
    110a:	02e6d7bb          	divuw	a5,a3,a4
        buf[i--] = digits[x % base];
    110e:	1582                	slli	a1,a1,0x20
    1110:	9181                	srli	a1,a1,0x20
    1112:	95b2                	add	a1,a1,a2
    1114:	0005c583          	lbu	a1,0(a1)
    1118:	00b10aa3          	sb	a1,21(sp)
    } while ((x /= base) != 0);
    111c:	0007859b          	sext.w	a1,a5
    1120:	12e6ec63          	bltu	a3,a4,1258 <printint.constprop.0+0x1a0>
        buf[i--] = digits[x % base];
    1124:	02e7f6bb          	remuw	a3,a5,a4
    1128:	1682                	slli	a3,a3,0x20
    112a:	9281                	srli	a3,a3,0x20
    112c:	96b2                	add	a3,a3,a2
    112e:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    1132:	02e7d83b          	divuw	a6,a5,a4
        buf[i--] = digits[x % base];
    1136:	00d10a23          	sb	a3,20(sp)
    } while ((x /= base) != 0);
    113a:	12e5e863          	bltu	a1,a4,126a <printint.constprop.0+0x1b2>
        buf[i--] = digits[x % base];
    113e:	02e876bb          	remuw	a3,a6,a4
    1142:	1682                	slli	a3,a3,0x20
    1144:	9281                	srli	a3,a3,0x20
    1146:	96b2                	add	a3,a3,a2
    1148:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    114c:	02e855bb          	divuw	a1,a6,a4
        buf[i--] = digits[x % base];
    1150:	00d109a3          	sb	a3,19(sp)
    } while ((x /= base) != 0);
    1154:	12e86463          	bltu	a6,a4,127c <printint.constprop.0+0x1c4>
        buf[i--] = digits[x % base];
    1158:	02e5f6bb          	remuw	a3,a1,a4
    115c:	1682                	slli	a3,a3,0x20
    115e:	9281                	srli	a3,a3,0x20
    1160:	96b2                	add	a3,a3,a2
    1162:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    1166:	02e5d83b          	divuw	a6,a1,a4
        buf[i--] = digits[x % base];
    116a:	00d10923          	sb	a3,18(sp)
    } while ((x /= base) != 0);
    116e:	0ce5ec63          	bltu	a1,a4,1246 <printint.constprop.0+0x18e>
        buf[i--] = digits[x % base];
    1172:	02e876bb          	remuw	a3,a6,a4
    1176:	1682                	slli	a3,a3,0x20
    1178:	9281                	srli	a3,a3,0x20
    117a:	96b2                	add	a3,a3,a2
    117c:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    1180:	02e855bb          	divuw	a1,a6,a4
        buf[i--] = digits[x % base];
    1184:	00d108a3          	sb	a3,17(sp)
    } while ((x /= base) != 0);
    1188:	10e86963          	bltu	a6,a4,129a <printint.constprop.0+0x1e2>
        buf[i--] = digits[x % base];
    118c:	02e5f6bb          	remuw	a3,a1,a4
    1190:	1682                	slli	a3,a3,0x20
    1192:	9281                	srli	a3,a3,0x20
    1194:	96b2                	add	a3,a3,a2
    1196:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    119a:	02e5d83b          	divuw	a6,a1,a4
        buf[i--] = digits[x % base];
    119e:	00d10823          	sb	a3,16(sp)
    } while ((x /= base) != 0);
    11a2:	10e5e763          	bltu	a1,a4,12b0 <printint.constprop.0+0x1f8>
        buf[i--] = digits[x % base];
    11a6:	02e876bb          	remuw	a3,a6,a4
    11aa:	1682                	slli	a3,a3,0x20
    11ac:	9281                	srli	a3,a3,0x20
    11ae:	96b2                	add	a3,a3,a2
    11b0:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    11b4:	02e857bb          	divuw	a5,a6,a4
        buf[i--] = digits[x % base];
    11b8:	00d107a3          	sb	a3,15(sp)
    } while ((x /= base) != 0);
    11bc:	10e86363          	bltu	a6,a4,12c2 <printint.constprop.0+0x20a>
        buf[i--] = digits[x % base];
    11c0:	1782                	slli	a5,a5,0x20
    11c2:	9381                	srli	a5,a5,0x20
    11c4:	97b2                	add	a5,a5,a2
    11c6:	0007c783          	lbu	a5,0(a5)
    11ca:	4599                	li	a1,6
    11cc:	00f10723          	sb	a5,14(sp)

    if (sign)
    11d0:	00055763          	bgez	a0,11de <printint.constprop.0+0x126>
        buf[i--] = '-';
    11d4:	02d00793          	li	a5,45
    11d8:	00f106a3          	sb	a5,13(sp)
        buf[i--] = digits[x % base];
    11dc:	4595                	li	a1,5
    write(f, s, l);
    11de:	003c                	addi	a5,sp,8
    11e0:	4641                	li	a2,16
    11e2:	9e0d                	subw	a2,a2,a1
    11e4:	4505                	li	a0,1
    11e6:	95be                	add	a1,a1,a5
    11e8:	291000ef          	jal	ra,1c78 <write>
    i++;
    if (i < 0)
        puts("printint error");
    out(stdout, buf + i, 16 - i);
}
    11ec:	70a2                	ld	ra,40(sp)
    11ee:	6145                	addi	sp,sp,48
    11f0:	8082                	ret
        x = -xx;
    11f2:	40a0083b          	negw	a6,a0
        buf[i--] = digits[x % base];
    11f6:	02b877bb          	remuw	a5,a6,a1
    11fa:	00001617          	auipc	a2,0x1
    11fe:	d7660613          	addi	a2,a2,-650 # 1f70 <digits>
    buf[16] = 0;
    1202:	00010c23          	sb	zero,24(sp)
        buf[i--] = digits[x % base];
    1206:	0005871b          	sext.w	a4,a1
    120a:	1782                	slli	a5,a5,0x20
    120c:	9381                	srli	a5,a5,0x20
    120e:	97b2                	add	a5,a5,a2
    1210:	0007c783          	lbu	a5,0(a5)
    } while ((x /= base) != 0);
    1214:	02b858bb          	divuw	a7,a6,a1
        buf[i--] = digits[x % base];
    1218:	00f10ba3          	sb	a5,23(sp)
    } while ((x /= base) != 0);
    121c:	06b86963          	bltu	a6,a1,128e <printint.constprop.0+0x1d6>
        buf[i--] = digits[x % base];
    1220:	02e8f7bb          	remuw	a5,a7,a4
    1224:	1782                	slli	a5,a5,0x20
    1226:	9381                	srli	a5,a5,0x20
    1228:	97b2                	add	a5,a5,a2
    122a:	0007c783          	lbu	a5,0(a5)
    } while ((x /= base) != 0);
    122e:	02e8d6bb          	divuw	a3,a7,a4
        buf[i--] = digits[x % base];
    1232:	00f10b23          	sb	a5,22(sp)
    } while ((x /= base) != 0);
    1236:	ece8f8e3          	bgeu	a7,a4,1106 <printint.constprop.0+0x4e>
        buf[i--] = '-';
    123a:	02d00793          	li	a5,45
    123e:	00f10aa3          	sb	a5,21(sp)
        buf[i--] = digits[x % base];
    1242:	45b5                	li	a1,13
    1244:	bf69                	j	11de <printint.constprop.0+0x126>
    1246:	45a9                	li	a1,10
    if (sign)
    1248:	f8055be3          	bgez	a0,11de <printint.constprop.0+0x126>
        buf[i--] = '-';
    124c:	02d00793          	li	a5,45
    1250:	00f108a3          	sb	a5,17(sp)
        buf[i--] = digits[x % base];
    1254:	45a5                	li	a1,9
    1256:	b761                	j	11de <printint.constprop.0+0x126>
    1258:	45b5                	li	a1,13
    if (sign)
    125a:	f80552e3          	bgez	a0,11de <printint.constprop.0+0x126>
        buf[i--] = '-';
    125e:	02d00793          	li	a5,45
    1262:	00f10a23          	sb	a5,20(sp)
        buf[i--] = digits[x % base];
    1266:	45b1                	li	a1,12
    1268:	bf9d                	j	11de <printint.constprop.0+0x126>
    126a:	45b1                	li	a1,12
    if (sign)
    126c:	f60559e3          	bgez	a0,11de <printint.constprop.0+0x126>
        buf[i--] = '-';
    1270:	02d00793          	li	a5,45
    1274:	00f109a3          	sb	a5,19(sp)
        buf[i--] = digits[x % base];
    1278:	45ad                	li	a1,11
    127a:	b795                	j	11de <printint.constprop.0+0x126>
    127c:	45ad                	li	a1,11
    if (sign)
    127e:	f60550e3          	bgez	a0,11de <printint.constprop.0+0x126>
        buf[i--] = '-';
    1282:	02d00793          	li	a5,45
    1286:	00f10923          	sb	a5,18(sp)
        buf[i--] = digits[x % base];
    128a:	45a9                	li	a1,10
    128c:	bf89                	j	11de <printint.constprop.0+0x126>
        buf[i--] = '-';
    128e:	02d00793          	li	a5,45
    1292:	00f10b23          	sb	a5,22(sp)
        buf[i--] = digits[x % base];
    1296:	45b9                	li	a1,14
    1298:	b799                	j	11de <printint.constprop.0+0x126>
    129a:	45a5                	li	a1,9
    if (sign)
    129c:	f40551e3          	bgez	a0,11de <printint.constprop.0+0x126>
        buf[i--] = '-';
    12a0:	02d00793          	li	a5,45
    12a4:	00f10823          	sb	a5,16(sp)
        buf[i--] = digits[x % base];
    12a8:	45a1                	li	a1,8
    12aa:	bf15                	j	11de <printint.constprop.0+0x126>
    i = 15;
    12ac:	45bd                	li	a1,15
    12ae:	bf05                	j	11de <printint.constprop.0+0x126>
        buf[i--] = digits[x % base];
    12b0:	45a1                	li	a1,8
    if (sign)
    12b2:	f20556e3          	bgez	a0,11de <printint.constprop.0+0x126>
        buf[i--] = '-';
    12b6:	02d00793          	li	a5,45
    12ba:	00f107a3          	sb	a5,15(sp)
        buf[i--] = digits[x % base];
    12be:	459d                	li	a1,7
    12c0:	bf39                	j	11de <printint.constprop.0+0x126>
    12c2:	459d                	li	a1,7
    if (sign)
    12c4:	f0055de3          	bgez	a0,11de <printint.constprop.0+0x126>
        buf[i--] = '-';
    12c8:	02d00793          	li	a5,45
    12cc:	00f10723          	sb	a5,14(sp)
        buf[i--] = digits[x % base];
    12d0:	4599                	li	a1,6
    12d2:	b731                	j	11de <printint.constprop.0+0x126>

00000000000012d4 <getchar>:
{
    12d4:	1101                	addi	sp,sp,-32
    read(stdin, &byte, 1);
    12d6:	00f10593          	addi	a1,sp,15
    12da:	4605                	li	a2,1
    12dc:	4501                	li	a0,0
{
    12de:	ec06                	sd	ra,24(sp)
    char byte = 0;
    12e0:	000107a3          	sb	zero,15(sp)
    read(stdin, &byte, 1);
    12e4:	18b000ef          	jal	ra,1c6e <read>
}
    12e8:	60e2                	ld	ra,24(sp)
    12ea:	00f14503          	lbu	a0,15(sp)
    12ee:	6105                	addi	sp,sp,32
    12f0:	8082                	ret

00000000000012f2 <putchar>:
{
    12f2:	1101                	addi	sp,sp,-32
    12f4:	87aa                	mv	a5,a0
    return write(stdout, &byte, 1);
    12f6:	00f10593          	addi	a1,sp,15
    12fa:	4605                	li	a2,1
    12fc:	4505                	li	a0,1
{
    12fe:	ec06                	sd	ra,24(sp)
    char byte = c;
    1300:	00f107a3          	sb	a5,15(sp)
    return write(stdout, &byte, 1);
    1304:	175000ef          	jal	ra,1c78 <write>
}
    1308:	60e2                	ld	ra,24(sp)
    130a:	2501                	sext.w	a0,a0
    130c:	6105                	addi	sp,sp,32
    130e:	8082                	ret

0000000000001310 <puts>:
{
    1310:	1141                	addi	sp,sp,-16
    1312:	e406                	sd	ra,8(sp)
    1314:	e022                	sd	s0,0(sp)
    1316:	842a                	mv	s0,a0
    r = -(write(stdout, s, strlen(s)) < 0);
    1318:	57c000ef          	jal	ra,1894 <strlen>
    131c:	862a                	mv	a2,a0
    131e:	85a2                	mv	a1,s0
    1320:	4505                	li	a0,1
    1322:	157000ef          	jal	ra,1c78 <write>
}
    1326:	60a2                	ld	ra,8(sp)
    1328:	6402                	ld	s0,0(sp)
    r = -(write(stdout, s, strlen(s)) < 0);
    132a:	957d                	srai	a0,a0,0x3f
    return r;
    132c:	2501                	sext.w	a0,a0
}
    132e:	0141                	addi	sp,sp,16
    1330:	8082                	ret

0000000000001332 <printf>:
    out(stdout, buf, i);
}

// Print to the console. only understands %d, %x, %p, %s.
void printf(const char *fmt, ...)
{
    1332:	7171                	addi	sp,sp,-176
    1334:	fc56                	sd	s5,56(sp)
    1336:	ed3e                	sd	a5,152(sp)
    buf[i++] = '0';
    1338:	7ae1                	lui	s5,0xffff8
    va_list ap;
    int cnt = 0, l = 0;
    char *a, *z, *s = (char *)fmt, str;
    int f = stdout;

    va_start(ap, fmt);
    133a:	18bc                	addi	a5,sp,120
{
    133c:	e8ca                	sd	s2,80(sp)
    133e:	e4ce                	sd	s3,72(sp)
    1340:	e0d2                	sd	s4,64(sp)
    1342:	f85a                	sd	s6,48(sp)
    1344:	f486                	sd	ra,104(sp)
    1346:	f0a2                	sd	s0,96(sp)
    1348:	eca6                	sd	s1,88(sp)
    134a:	fcae                	sd	a1,120(sp)
    134c:	e132                	sd	a2,128(sp)
    134e:	e536                	sd	a3,136(sp)
    1350:	e93a                	sd	a4,144(sp)
    1352:	f142                	sd	a6,160(sp)
    1354:	f546                	sd	a7,168(sp)
    va_start(ap, fmt);
    1356:	e03e                	sd	a5,0(sp)
    for (;;)
    {
        if (!*s)
            break;
        for (a = s; *s && *s != '%'; s++)
    1358:	02500913          	li	s2,37
        out(f, a, l);
        if (l)
            continue;
        if (s[1] == 0)
            break;
        switch (s[1])
    135c:	07300a13          	li	s4,115
        case 'p':
            printptr(va_arg(ap, uint64));
            break;
        case 's':
            if ((a = va_arg(ap, char *)) == 0)
                a = "(null)";
    1360:	00001b17          	auipc	s6,0x1
    1364:	be0b0b13          	addi	s6,s6,-1056 # 1f40 <__clone+0x94>
    buf[i++] = '0';
    1368:	830aca93          	xori	s5,s5,-2000
        buf[i++] = digits[x >> (sizeof(uint64) * 8 - 4)];
    136c:	00001997          	auipc	s3,0x1
    1370:	c0498993          	addi	s3,s3,-1020 # 1f70 <digits>
        if (!*s)
    1374:	00054783          	lbu	a5,0(a0)
    1378:	16078a63          	beqz	a5,14ec <printf+0x1ba>
    137c:	862a                	mv	a2,a0
        for (a = s; *s && *s != '%'; s++)
    137e:	19278163          	beq	a5,s2,1500 <printf+0x1ce>
    1382:	00164783          	lbu	a5,1(a2)
    1386:	0605                	addi	a2,a2,1
    1388:	fbfd                	bnez	a5,137e <printf+0x4c>
    138a:	84b2                	mv	s1,a2
        l = z - a;
    138c:	40a6043b          	subw	s0,a2,a0
    write(f, s, l);
    1390:	85aa                	mv	a1,a0
    1392:	8622                	mv	a2,s0
    1394:	4505                	li	a0,1
    1396:	0e3000ef          	jal	ra,1c78 <write>
        if (l)
    139a:	18041c63          	bnez	s0,1532 <printf+0x200>
        if (s[1] == 0)
    139e:	0014c783          	lbu	a5,1(s1)
    13a2:	14078563          	beqz	a5,14ec <printf+0x1ba>
        switch (s[1])
    13a6:	1d478063          	beq	a5,s4,1566 <printf+0x234>
    13aa:	18fa6663          	bltu	s4,a5,1536 <printf+0x204>
    13ae:	06400713          	li	a4,100
    13b2:	1ae78063          	beq	a5,a4,1552 <printf+0x220>
    13b6:	07000713          	li	a4,112
    13ba:	1ce79963          	bne	a5,a4,158c <printf+0x25a>
            printptr(va_arg(ap, uint64));
    13be:	6702                	ld	a4,0(sp)
    buf[i++] = '0';
    13c0:	01511423          	sh	s5,8(sp)
    write(f, s, l);
    13c4:	4649                	li	a2,18
            printptr(va_arg(ap, uint64));
    13c6:	631c                	ld	a5,0(a4)
    13c8:	0721                	addi	a4,a4,8
    13ca:	e03a                	sd	a4,0(sp)
    for (j = 0; j < (sizeof(uint64) * 2); j++, x <<= 4)
    13cc:	00479293          	slli	t0,a5,0x4
    13d0:	00879f93          	slli	t6,a5,0x8
    13d4:	00c79f13          	slli	t5,a5,0xc
    13d8:	01079e93          	slli	t4,a5,0x10
    13dc:	01479e13          	slli	t3,a5,0x14
    13e0:	01879313          	slli	t1,a5,0x18
    13e4:	01c79893          	slli	a7,a5,0x1c
    13e8:	02479813          	slli	a6,a5,0x24
    13ec:	02879513          	slli	a0,a5,0x28
    13f0:	02c79593          	slli	a1,a5,0x2c
    13f4:	03079693          	slli	a3,a5,0x30
    13f8:	03479713          	slli	a4,a5,0x34
        buf[i++] = digits[x >> (sizeof(uint64) * 8 - 4)];
    13fc:	03c7d413          	srli	s0,a5,0x3c
    1400:	01c7d39b          	srliw	t2,a5,0x1c
    1404:	03c2d293          	srli	t0,t0,0x3c
    1408:	03cfdf93          	srli	t6,t6,0x3c
    140c:	03cf5f13          	srli	t5,t5,0x3c
    1410:	03cede93          	srli	t4,t4,0x3c
    1414:	03ce5e13          	srli	t3,t3,0x3c
    1418:	03c35313          	srli	t1,t1,0x3c
    141c:	03c8d893          	srli	a7,a7,0x3c
    1420:	03c85813          	srli	a6,a6,0x3c
    1424:	9171                	srli	a0,a0,0x3c
    1426:	91f1                	srli	a1,a1,0x3c
    1428:	92f1                	srli	a3,a3,0x3c
    142a:	9371                	srli	a4,a4,0x3c
    142c:	96ce                	add	a3,a3,s3
    142e:	974e                	add	a4,a4,s3
    1430:	944e                	add	s0,s0,s3
    1432:	92ce                	add	t0,t0,s3
    1434:	9fce                	add	t6,t6,s3
    1436:	9f4e                	add	t5,t5,s3
    1438:	9ece                	add	t4,t4,s3
    143a:	9e4e                	add	t3,t3,s3
    143c:	934e                	add	t1,t1,s3
    143e:	98ce                	add	a7,a7,s3
    1440:	93ce                	add	t2,t2,s3
    1442:	984e                	add	a6,a6,s3
    1444:	954e                	add	a0,a0,s3
    1446:	95ce                	add	a1,a1,s3
    1448:	0006c083          	lbu	ra,0(a3)
    144c:	0002c283          	lbu	t0,0(t0)
    1450:	00074683          	lbu	a3,0(a4)
    1454:	000fcf83          	lbu	t6,0(t6)
    1458:	000f4f03          	lbu	t5,0(t5)
    145c:	000ece83          	lbu	t4,0(t4)
    1460:	000e4e03          	lbu	t3,0(t3)
    1464:	00034303          	lbu	t1,0(t1)
    1468:	0008c883          	lbu	a7,0(a7)
    146c:	0003c383          	lbu	t2,0(t2)
    1470:	00084803          	lbu	a6,0(a6)
    1474:	00054503          	lbu	a0,0(a0)
    1478:	0005c583          	lbu	a1,0(a1)
    147c:	00044403          	lbu	s0,0(s0)
    for (j = 0; j < (sizeof(uint64) * 2); j++, x <<= 4)
    1480:	03879713          	slli	a4,a5,0x38
        buf[i++] = digits[x >> (sizeof(uint64) * 8 - 4)];
    1484:	9371                	srli	a4,a4,0x3c
    1486:	8bbd                	andi	a5,a5,15
    1488:	974e                	add	a4,a4,s3
    148a:	97ce                	add	a5,a5,s3
    148c:	005105a3          	sb	t0,11(sp)
    1490:	01f10623          	sb	t6,12(sp)
    1494:	01e106a3          	sb	t5,13(sp)
    1498:	01d10723          	sb	t4,14(sp)
    149c:	01c107a3          	sb	t3,15(sp)
    14a0:	00610823          	sb	t1,16(sp)
    14a4:	011108a3          	sb	a7,17(sp)
    14a8:	00710923          	sb	t2,18(sp)
    14ac:	010109a3          	sb	a6,19(sp)
    14b0:	00a10a23          	sb	a0,20(sp)
    14b4:	00b10aa3          	sb	a1,21(sp)
    14b8:	00110b23          	sb	ra,22(sp)
    14bc:	00d10ba3          	sb	a3,23(sp)
    14c0:	00810523          	sb	s0,10(sp)
    14c4:	00074703          	lbu	a4,0(a4)
    14c8:	0007c783          	lbu	a5,0(a5)
    write(f, s, l);
    14cc:	002c                	addi	a1,sp,8
    14ce:	4505                	li	a0,1
        buf[i++] = digits[x >> (sizeof(uint64) * 8 - 4)];
    14d0:	00e10c23          	sb	a4,24(sp)
    14d4:	00f10ca3          	sb	a5,25(sp)
    buf[i] = 0;
    14d8:	00010d23          	sb	zero,26(sp)
    write(f, s, l);
    14dc:	79c000ef          	jal	ra,1c78 <write>
            // Print unknown % sequence to draw attention.
            putchar('%');
            putchar(s[1]);
            break;
        }
        s += 2;
    14e0:	00248513          	addi	a0,s1,2
        if (!*s)
    14e4:	00054783          	lbu	a5,0(a0)
    14e8:	e8079ae3          	bnez	a5,137c <printf+0x4a>
    }
    va_end(ap);
}
    14ec:	70a6                	ld	ra,104(sp)
    14ee:	7406                	ld	s0,96(sp)
    14f0:	64e6                	ld	s1,88(sp)
    14f2:	6946                	ld	s2,80(sp)
    14f4:	69a6                	ld	s3,72(sp)
    14f6:	6a06                	ld	s4,64(sp)
    14f8:	7ae2                	ld	s5,56(sp)
    14fa:	7b42                	ld	s6,48(sp)
    14fc:	614d                	addi	sp,sp,176
    14fe:	8082                	ret
        for (z = s; s[0] == '%' && s[1] == '%'; z++, s += 2)
    1500:	00064783          	lbu	a5,0(a2)
    1504:	84b2                	mv	s1,a2
    1506:	01278963          	beq	a5,s2,1518 <printf+0x1e6>
    150a:	b549                	j	138c <printf+0x5a>
    150c:	0024c783          	lbu	a5,2(s1)
    1510:	0605                	addi	a2,a2,1
    1512:	0489                	addi	s1,s1,2
    1514:	e7279ce3          	bne	a5,s2,138c <printf+0x5a>
    1518:	0014c783          	lbu	a5,1(s1)
    151c:	ff2788e3          	beq	a5,s2,150c <printf+0x1da>
        l = z - a;
    1520:	40a6043b          	subw	s0,a2,a0
    write(f, s, l);
    1524:	85aa                	mv	a1,a0
    1526:	8622                	mv	a2,s0
    1528:	4505                	li	a0,1
    152a:	74e000ef          	jal	ra,1c78 <write>
        if (l)
    152e:	e60408e3          	beqz	s0,139e <printf+0x6c>
    1532:	8526                	mv	a0,s1
    1534:	b581                	j	1374 <printf+0x42>
        switch (s[1])
    1536:	07800713          	li	a4,120
    153a:	04e79963          	bne	a5,a4,158c <printf+0x25a>
            printint(va_arg(ap, int), 16, 1);
    153e:	6782                	ld	a5,0(sp)
    1540:	45c1                	li	a1,16
    1542:	4388                	lw	a0,0(a5)
    1544:	07a1                	addi	a5,a5,8
    1546:	e03e                	sd	a5,0(sp)
    1548:	b71ff0ef          	jal	ra,10b8 <printint.constprop.0>
        s += 2;
    154c:	00248513          	addi	a0,s1,2
    1550:	bf51                	j	14e4 <printf+0x1b2>
            printint(va_arg(ap, int), 10, 1);
    1552:	6782                	ld	a5,0(sp)
    1554:	45a9                	li	a1,10
    1556:	4388                	lw	a0,0(a5)
    1558:	07a1                	addi	a5,a5,8
    155a:	e03e                	sd	a5,0(sp)
    155c:	b5dff0ef          	jal	ra,10b8 <printint.constprop.0>
        s += 2;
    1560:	00248513          	addi	a0,s1,2
    1564:	b741                	j	14e4 <printf+0x1b2>
            if ((a = va_arg(ap, char *)) == 0)
    1566:	6782                	ld	a5,0(sp)
    1568:	6380                	ld	s0,0(a5)
    156a:	07a1                	addi	a5,a5,8
    156c:	e03e                	sd	a5,0(sp)
    156e:	c031                	beqz	s0,15b2 <printf+0x280>
            l = strnlen(a, 200);
    1570:	0c800593          	li	a1,200
    1574:	8522                	mv	a0,s0
    1576:	40a000ef          	jal	ra,1980 <strnlen>
    write(f, s, l);
    157a:	0005061b          	sext.w	a2,a0
    157e:	85a2                	mv	a1,s0
    1580:	4505                	li	a0,1
    1582:	6f6000ef          	jal	ra,1c78 <write>
        s += 2;
    1586:	00248513          	addi	a0,s1,2
    158a:	bfa9                	j	14e4 <printf+0x1b2>
    return write(stdout, &byte, 1);
    158c:	4605                	li	a2,1
    158e:	002c                	addi	a1,sp,8
    1590:	4505                	li	a0,1
    char byte = c;
    1592:	01210423          	sb	s2,8(sp)
    return write(stdout, &byte, 1);
    1596:	6e2000ef          	jal	ra,1c78 <write>
    char byte = c;
    159a:	0014c783          	lbu	a5,1(s1)
    return write(stdout, &byte, 1);
    159e:	4605                	li	a2,1
    15a0:	002c                	addi	a1,sp,8
    15a2:	4505                	li	a0,1
    char byte = c;
    15a4:	00f10423          	sb	a5,8(sp)
    return write(stdout, &byte, 1);
    15a8:	6d0000ef          	jal	ra,1c78 <write>
        s += 2;
    15ac:	00248513          	addi	a0,s1,2
    15b0:	bf15                	j	14e4 <printf+0x1b2>
                a = "(null)";
    15b2:	845a                	mv	s0,s6
    15b4:	bf75                	j	1570 <printf+0x23e>

00000000000015b6 <panic>:
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

void panic(char *m)
{
    15b6:	1141                	addi	sp,sp,-16
    15b8:	e406                	sd	ra,8(sp)
    puts(m);
    15ba:	d57ff0ef          	jal	ra,1310 <puts>
    exit(-100);
}
    15be:	60a2                	ld	ra,8(sp)
    exit(-100);
    15c0:	f9c00513          	li	a0,-100
}
    15c4:	0141                	addi	sp,sp,16
    exit(-100);
    15c6:	a709                	j	1cc8 <exit>

00000000000015c8 <isspace>:
#define HIGHS (ONES * (UCHAR_MAX / 2 + 1))
#define HASZERO(x) (((x)-ONES) & ~(x)&HIGHS)

int isspace(int c)
{
    return c == ' ' || (unsigned)c - '\t' < 5;
    15c8:	02000793          	li	a5,32
    15cc:	00f50663          	beq	a0,a5,15d8 <isspace+0x10>
    15d0:	355d                	addiw	a0,a0,-9
    15d2:	00553513          	sltiu	a0,a0,5
    15d6:	8082                	ret
    15d8:	4505                	li	a0,1
}
    15da:	8082                	ret

00000000000015dc <isdigit>:

int isdigit(int c)
{
    return (unsigned)c - '0' < 10;
    15dc:	fd05051b          	addiw	a0,a0,-48
}
    15e0:	00a53513          	sltiu	a0,a0,10
    15e4:	8082                	ret

00000000000015e6 <atoi>:
    return c == ' ' || (unsigned)c - '\t' < 5;
    15e6:	02000613          	li	a2,32
    15ea:	4591                	li	a1,4

int atoi(const char *s)
{
    int n = 0, neg = 0;
    while (isspace(*s))
    15ec:	00054703          	lbu	a4,0(a0)
    return c == ' ' || (unsigned)c - '\t' < 5;
    15f0:	ff77069b          	addiw	a3,a4,-9
    15f4:	04c70d63          	beq	a4,a2,164e <atoi+0x68>
    15f8:	0007079b          	sext.w	a5,a4
    15fc:	04d5f963          	bgeu	a1,a3,164e <atoi+0x68>
        s++;
    switch (*s)
    1600:	02b00693          	li	a3,43
    1604:	04d70a63          	beq	a4,a3,1658 <atoi+0x72>
    1608:	02d00693          	li	a3,45
    160c:	06d70463          	beq	a4,a3,1674 <atoi+0x8e>
        neg = 1;
    case '+':
        s++;
    }
    /* Compute n as a negative number to avoid overflow on INT_MIN */
    while (isdigit(*s))
    1610:	fd07859b          	addiw	a1,a5,-48
    1614:	4625                	li	a2,9
    1616:	873e                	mv	a4,a5
    1618:	86aa                	mv	a3,a0
    int n = 0, neg = 0;
    161a:	4e01                	li	t3,0
    while (isdigit(*s))
    161c:	04b66a63          	bltu	a2,a1,1670 <atoi+0x8a>
    int n = 0, neg = 0;
    1620:	4501                	li	a0,0
    while (isdigit(*s))
    1622:	4825                	li	a6,9
    1624:	0016c603          	lbu	a2,1(a3)
        n = 10 * n - (*s++ - '0');
    1628:	0025179b          	slliw	a5,a0,0x2
    162c:	9d3d                	addw	a0,a0,a5
    162e:	fd07031b          	addiw	t1,a4,-48
    1632:	0015189b          	slliw	a7,a0,0x1
    while (isdigit(*s))
    1636:	fd06059b          	addiw	a1,a2,-48
        n = 10 * n - (*s++ - '0');
    163a:	0685                	addi	a3,a3,1
    163c:	4068853b          	subw	a0,a7,t1
    while (isdigit(*s))
    1640:	0006071b          	sext.w	a4,a2
    1644:	feb870e3          	bgeu	a6,a1,1624 <atoi+0x3e>
    return neg ? n : -n;
    1648:	000e0563          	beqz	t3,1652 <atoi+0x6c>
}
    164c:	8082                	ret
        s++;
    164e:	0505                	addi	a0,a0,1
    1650:	bf71                	j	15ec <atoi+0x6>
    return neg ? n : -n;
    1652:	4113053b          	subw	a0,t1,a7
    1656:	8082                	ret
    while (isdigit(*s))
    1658:	00154783          	lbu	a5,1(a0)
    165c:	4625                	li	a2,9
        s++;
    165e:	00150693          	addi	a3,a0,1
    while (isdigit(*s))
    1662:	fd07859b          	addiw	a1,a5,-48
    1666:	0007871b          	sext.w	a4,a5
    int n = 0, neg = 0;
    166a:	4e01                	li	t3,0
    while (isdigit(*s))
    166c:	fab67ae3          	bgeu	a2,a1,1620 <atoi+0x3a>
    1670:	4501                	li	a0,0
}
    1672:	8082                	ret
    while (isdigit(*s))
    1674:	00154783          	lbu	a5,1(a0)
    1678:	4625                	li	a2,9
        s++;
    167a:	00150693          	addi	a3,a0,1
    while (isdigit(*s))
    167e:	fd07859b          	addiw	a1,a5,-48
    1682:	0007871b          	sext.w	a4,a5
    1686:	feb665e3          	bltu	a2,a1,1670 <atoi+0x8a>
        neg = 1;
    168a:	4e05                	li	t3,1
    168c:	bf51                	j	1620 <atoi+0x3a>

000000000000168e <memset>:

void *memset(void *dest, int c, size_t n)
{
    char *p = dest;
    for (int i = 0; i < n; ++i, *(p++) = c)
    168e:	16060d63          	beqz	a2,1808 <memset+0x17a>
    1692:	40a007b3          	neg	a5,a0
    1696:	8b9d                	andi	a5,a5,7
    1698:	00778713          	addi	a4,a5,7
    169c:	482d                	li	a6,11
    169e:	0ff5f593          	andi	a1,a1,255
    16a2:	fff60693          	addi	a3,a2,-1
    16a6:	17076263          	bltu	a4,a6,180a <memset+0x17c>
    16aa:	16e6ea63          	bltu	a3,a4,181e <memset+0x190>
    16ae:	16078563          	beqz	a5,1818 <memset+0x18a>
    16b2:	00b50023          	sb	a1,0(a0)
    16b6:	4705                	li	a4,1
    16b8:	00150e93          	addi	t4,a0,1
    16bc:	14e78c63          	beq	a5,a4,1814 <memset+0x186>
    16c0:	00b500a3          	sb	a1,1(a0)
    16c4:	4709                	li	a4,2
    16c6:	00250e93          	addi	t4,a0,2
    16ca:	14e78d63          	beq	a5,a4,1824 <memset+0x196>
    16ce:	00b50123          	sb	a1,2(a0)
    16d2:	470d                	li	a4,3
    16d4:	00350e93          	addi	t4,a0,3
    16d8:	12e78b63          	beq	a5,a4,180e <memset+0x180>
    16dc:	00b501a3          	sb	a1,3(a0)
    16e0:	4711                	li	a4,4
    16e2:	00450e93          	addi	t4,a0,4
    16e6:	14e78163          	beq	a5,a4,1828 <memset+0x19a>
    16ea:	00b50223          	sb	a1,4(a0)
    16ee:	4715                	li	a4,5
    16f0:	00550e93          	addi	t4,a0,5
    16f4:	12e78c63          	beq	a5,a4,182c <memset+0x19e>
    16f8:	00b502a3          	sb	a1,5(a0)
    16fc:	471d                	li	a4,7
    16fe:	00650e93          	addi	t4,a0,6
    1702:	12e79763          	bne	a5,a4,1830 <memset+0x1a2>
    1706:	00750e93          	addi	t4,a0,7
    170a:	00b50323          	sb	a1,6(a0)
    170e:	4f1d                	li	t5,7
    1710:	00859713          	slli	a4,a1,0x8
    1714:	8f4d                	or	a4,a4,a1
    1716:	01059e13          	slli	t3,a1,0x10
    171a:	01c76e33          	or	t3,a4,t3
    171e:	01859313          	slli	t1,a1,0x18
    1722:	006e6333          	or	t1,t3,t1
    1726:	02059893          	slli	a7,a1,0x20
    172a:	011368b3          	or	a7,t1,a7
    172e:	02859813          	slli	a6,a1,0x28
    1732:	40f60333          	sub	t1,a2,a5
    1736:	0108e833          	or	a6,a7,a6
    173a:	03059693          	slli	a3,a1,0x30
    173e:	00d866b3          	or	a3,a6,a3
    1742:	03859713          	slli	a4,a1,0x38
    1746:	97aa                	add	a5,a5,a0
    1748:	ff837813          	andi	a6,t1,-8
    174c:	8f55                	or	a4,a4,a3
    174e:	00f806b3          	add	a3,a6,a5
    1752:	e398                	sd	a4,0(a5)
    1754:	07a1                	addi	a5,a5,8
    1756:	fed79ee3          	bne	a5,a3,1752 <memset+0xc4>
    175a:	ff837693          	andi	a3,t1,-8
    175e:	00de87b3          	add	a5,t4,a3
    1762:	01e6873b          	addw	a4,a3,t5
    1766:	0ad30663          	beq	t1,a3,1812 <memset+0x184>
    176a:	00b78023          	sb	a1,0(a5)
    176e:	0017069b          	addiw	a3,a4,1
    1772:	08c6fb63          	bgeu	a3,a2,1808 <memset+0x17a>
    1776:	00b780a3          	sb	a1,1(a5)
    177a:	0027069b          	addiw	a3,a4,2
    177e:	08c6f563          	bgeu	a3,a2,1808 <memset+0x17a>
    1782:	00b78123          	sb	a1,2(a5)
    1786:	0037069b          	addiw	a3,a4,3
    178a:	06c6ff63          	bgeu	a3,a2,1808 <memset+0x17a>
    178e:	00b781a3          	sb	a1,3(a5)
    1792:	0047069b          	addiw	a3,a4,4
    1796:	06c6f963          	bgeu	a3,a2,1808 <memset+0x17a>
    179a:	00b78223          	sb	a1,4(a5)
    179e:	0057069b          	addiw	a3,a4,5
    17a2:	06c6f363          	bgeu	a3,a2,1808 <memset+0x17a>
    17a6:	00b782a3          	sb	a1,5(a5)
    17aa:	0067069b          	addiw	a3,a4,6
    17ae:	04c6fd63          	bgeu	a3,a2,1808 <memset+0x17a>
    17b2:	00b78323          	sb	a1,6(a5)
    17b6:	0077069b          	addiw	a3,a4,7
    17ba:	04c6f763          	bgeu	a3,a2,1808 <memset+0x17a>
    17be:	00b783a3          	sb	a1,7(a5)
    17c2:	0087069b          	addiw	a3,a4,8
    17c6:	04c6f163          	bgeu	a3,a2,1808 <memset+0x17a>
    17ca:	00b78423          	sb	a1,8(a5)
    17ce:	0097069b          	addiw	a3,a4,9
    17d2:	02c6fb63          	bgeu	a3,a2,1808 <memset+0x17a>
    17d6:	00b784a3          	sb	a1,9(a5)
    17da:	00a7069b          	addiw	a3,a4,10
    17de:	02c6f563          	bgeu	a3,a2,1808 <memset+0x17a>
    17e2:	00b78523          	sb	a1,10(a5)
    17e6:	00b7069b          	addiw	a3,a4,11
    17ea:	00c6ff63          	bgeu	a3,a2,1808 <memset+0x17a>
    17ee:	00b785a3          	sb	a1,11(a5)
    17f2:	00c7069b          	addiw	a3,a4,12
    17f6:	00c6f963          	bgeu	a3,a2,1808 <memset+0x17a>
    17fa:	00b78623          	sb	a1,12(a5)
    17fe:	2735                	addiw	a4,a4,13
    1800:	00c77463          	bgeu	a4,a2,1808 <memset+0x17a>
    1804:	00b786a3          	sb	a1,13(a5)
        ;
    return dest;
}
    1808:	8082                	ret
    180a:	472d                	li	a4,11
    180c:	bd79                	j	16aa <memset+0x1c>
    for (int i = 0; i < n; ++i, *(p++) = c)
    180e:	4f0d                	li	t5,3
    1810:	b701                	j	1710 <memset+0x82>
    1812:	8082                	ret
    1814:	4f05                	li	t5,1
    1816:	bded                	j	1710 <memset+0x82>
    1818:	8eaa                	mv	t4,a0
    181a:	4f01                	li	t5,0
    181c:	bdd5                	j	1710 <memset+0x82>
    181e:	87aa                	mv	a5,a0
    1820:	4701                	li	a4,0
    1822:	b7a1                	j	176a <memset+0xdc>
    1824:	4f09                	li	t5,2
    1826:	b5ed                	j	1710 <memset+0x82>
    1828:	4f11                	li	t5,4
    182a:	b5dd                	j	1710 <memset+0x82>
    182c:	4f15                	li	t5,5
    182e:	b5cd                	j	1710 <memset+0x82>
    1830:	4f19                	li	t5,6
    1832:	bdf9                	j	1710 <memset+0x82>

0000000000001834 <strcmp>:

int strcmp(const char *l, const char *r)
{
    for (; *l == *r && *l; l++, r++)
    1834:	00054783          	lbu	a5,0(a0)
    1838:	0005c703          	lbu	a4,0(a1)
    183c:	00e79863          	bne	a5,a4,184c <strcmp+0x18>
    1840:	0505                	addi	a0,a0,1
    1842:	0585                	addi	a1,a1,1
    1844:	fbe5                	bnez	a5,1834 <strcmp>
    1846:	4501                	li	a0,0
        ;
    return *(unsigned char *)l - *(unsigned char *)r;
}
    1848:	9d19                	subw	a0,a0,a4
    184a:	8082                	ret
    return *(unsigned char *)l - *(unsigned char *)r;
    184c:	0007851b          	sext.w	a0,a5
    1850:	bfe5                	j	1848 <strcmp+0x14>

0000000000001852 <strncmp>:

int strncmp(const char *_l, const char *_r, size_t n)
{
    const unsigned char *l = (void *)_l, *r = (void *)_r;
    if (!n--)
    1852:	ce05                	beqz	a2,188a <strncmp+0x38>
        return 0;
    for (; *l && *r && n && *l == *r; l++, r++, n--)
    1854:	00054703          	lbu	a4,0(a0)
    1858:	0005c783          	lbu	a5,0(a1)
    185c:	cb0d                	beqz	a4,188e <strncmp+0x3c>
    if (!n--)
    185e:	167d                	addi	a2,a2,-1
    1860:	00c506b3          	add	a3,a0,a2
    1864:	a819                	j	187a <strncmp+0x28>
    for (; *l && *r && n && *l == *r; l++, r++, n--)
    1866:	00a68e63          	beq	a3,a0,1882 <strncmp+0x30>
    186a:	0505                	addi	a0,a0,1
    186c:	00e79b63          	bne	a5,a4,1882 <strncmp+0x30>
    1870:	00054703          	lbu	a4,0(a0)
        ;
    return *l - *r;
    1874:	0005c783          	lbu	a5,0(a1)
    for (; *l && *r && n && *l == *r; l++, r++, n--)
    1878:	cb19                	beqz	a4,188e <strncmp+0x3c>
    187a:	0005c783          	lbu	a5,0(a1)
    187e:	0585                	addi	a1,a1,1
    1880:	f3fd                	bnez	a5,1866 <strncmp+0x14>
    return *l - *r;
    1882:	0007051b          	sext.w	a0,a4
    1886:	9d1d                	subw	a0,a0,a5
    1888:	8082                	ret
        return 0;
    188a:	4501                	li	a0,0
}
    188c:	8082                	ret
    188e:	4501                	li	a0,0
    return *l - *r;
    1890:	9d1d                	subw	a0,a0,a5
    1892:	8082                	ret

0000000000001894 <strlen>:
size_t strlen(const char *s)
{
    const char *a = s;
    typedef size_t __attribute__((__may_alias__)) word;
    const word *w;
    for (; (uintptr_t)s % SS; s++)
    1894:	00757793          	andi	a5,a0,7
    1898:	cf89                	beqz	a5,18b2 <strlen+0x1e>
    189a:	87aa                	mv	a5,a0
    189c:	a029                	j	18a6 <strlen+0x12>
    189e:	0785                	addi	a5,a5,1
    18a0:	0077f713          	andi	a4,a5,7
    18a4:	cb01                	beqz	a4,18b4 <strlen+0x20>
        if (!*s)
    18a6:	0007c703          	lbu	a4,0(a5)
    18aa:	fb75                	bnez	a4,189e <strlen+0xa>
    for (w = (const void *)s; !HASZERO(*w); w++)
        ;
    s = (const void *)w;
    for (; *s; s++)
        ;
    return s - a;
    18ac:	40a78533          	sub	a0,a5,a0
}
    18b0:	8082                	ret
    for (; (uintptr_t)s % SS; s++)
    18b2:	87aa                	mv	a5,a0
    for (w = (const void *)s; !HASZERO(*w); w++)
    18b4:	6394                	ld	a3,0(a5)
    18b6:	00000597          	auipc	a1,0x0
    18ba:	6925b583          	ld	a1,1682(a1) # 1f48 <__clone+0x9c>
    18be:	00000617          	auipc	a2,0x0
    18c2:	69263603          	ld	a2,1682(a2) # 1f50 <__clone+0xa4>
    18c6:	a019                	j	18cc <strlen+0x38>
    18c8:	6794                	ld	a3,8(a5)
    18ca:	07a1                	addi	a5,a5,8
    18cc:	00b68733          	add	a4,a3,a1
    18d0:	fff6c693          	not	a3,a3
    18d4:	8f75                	and	a4,a4,a3
    18d6:	8f71                	and	a4,a4,a2
    18d8:	db65                	beqz	a4,18c8 <strlen+0x34>
    for (; *s; s++)
    18da:	0007c703          	lbu	a4,0(a5)
    18de:	d779                	beqz	a4,18ac <strlen+0x18>
    18e0:	0017c703          	lbu	a4,1(a5)
    18e4:	0785                	addi	a5,a5,1
    18e6:	d379                	beqz	a4,18ac <strlen+0x18>
    18e8:	0017c703          	lbu	a4,1(a5)
    18ec:	0785                	addi	a5,a5,1
    18ee:	fb6d                	bnez	a4,18e0 <strlen+0x4c>
    18f0:	bf75                	j	18ac <strlen+0x18>

00000000000018f2 <memchr>:

void *memchr(const void *src, int c, size_t n)
{
    const unsigned char *s = src;
    c = (unsigned char)c;
    for (; ((uintptr_t)s & ALIGN) && n && *s != c; s++, n--)
    18f2:	00757713          	andi	a4,a0,7
{
    18f6:	87aa                	mv	a5,a0
    c = (unsigned char)c;
    18f8:	0ff5f593          	andi	a1,a1,255
    for (; ((uintptr_t)s & ALIGN) && n && *s != c; s++, n--)
    18fc:	cb19                	beqz	a4,1912 <memchr+0x20>
    18fe:	ce25                	beqz	a2,1976 <memchr+0x84>
    1900:	0007c703          	lbu	a4,0(a5)
    1904:	04b70e63          	beq	a4,a1,1960 <memchr+0x6e>
    1908:	0785                	addi	a5,a5,1
    190a:	0077f713          	andi	a4,a5,7
    190e:	167d                	addi	a2,a2,-1
    1910:	f77d                	bnez	a4,18fe <memchr+0xc>
            ;
        s = (const void *)w;
    }
    for (; n && *s != c; s++, n--)
        ;
    return n ? (void *)s : 0;
    1912:	4501                	li	a0,0
    if (n && *s != c)
    1914:	c235                	beqz	a2,1978 <memchr+0x86>
    1916:	0007c703          	lbu	a4,0(a5)
    191a:	04b70363          	beq	a4,a1,1960 <memchr+0x6e>
        size_t k = ONES * c;
    191e:	00000517          	auipc	a0,0x0
    1922:	63a53503          	ld	a0,1594(a0) # 1f58 <__clone+0xac>
        for (w = (const void *)s; n >= SS && !HASZERO(*w ^ k); w++, n -= SS)
    1926:	471d                	li	a4,7
        size_t k = ONES * c;
    1928:	02a58533          	mul	a0,a1,a0
        for (w = (const void *)s; n >= SS && !HASZERO(*w ^ k); w++, n -= SS)
    192c:	02c77a63          	bgeu	a4,a2,1960 <memchr+0x6e>
    1930:	00000897          	auipc	a7,0x0
    1934:	6188b883          	ld	a7,1560(a7) # 1f48 <__clone+0x9c>
    1938:	00000817          	auipc	a6,0x0
    193c:	61883803          	ld	a6,1560(a6) # 1f50 <__clone+0xa4>
    1940:	431d                	li	t1,7
    1942:	a029                	j	194c <memchr+0x5a>
    1944:	1661                	addi	a2,a2,-8
    1946:	07a1                	addi	a5,a5,8
    1948:	02c37963          	bgeu	t1,a2,197a <memchr+0x88>
    194c:	6398                	ld	a4,0(a5)
    194e:	8f29                	xor	a4,a4,a0
    1950:	011706b3          	add	a3,a4,a7
    1954:	fff74713          	not	a4,a4
    1958:	8f75                	and	a4,a4,a3
    195a:	01077733          	and	a4,a4,a6
    195e:	d37d                	beqz	a4,1944 <memchr+0x52>
    1960:	853e                	mv	a0,a5
    1962:	97b2                	add	a5,a5,a2
    1964:	a021                	j	196c <memchr+0x7a>
    for (; n && *s != c; s++, n--)
    1966:	0505                	addi	a0,a0,1
    1968:	00f50763          	beq	a0,a5,1976 <memchr+0x84>
    196c:	00054703          	lbu	a4,0(a0)
    1970:	feb71be3          	bne	a4,a1,1966 <memchr+0x74>
    1974:	8082                	ret
    return n ? (void *)s : 0;
    1976:	4501                	li	a0,0
}
    1978:	8082                	ret
    return n ? (void *)s : 0;
    197a:	4501                	li	a0,0
    for (; n && *s != c; s++, n--)
    197c:	f275                	bnez	a2,1960 <memchr+0x6e>
}
    197e:	8082                	ret

0000000000001980 <strnlen>:

size_t strnlen(const char *s, size_t n)
{
    1980:	1101                	addi	sp,sp,-32
    1982:	e822                	sd	s0,16(sp)
    const char *p = memchr(s, 0, n);
    1984:	862e                	mv	a2,a1
{
    1986:	842e                	mv	s0,a1
    const char *p = memchr(s, 0, n);
    1988:	4581                	li	a1,0
{
    198a:	e426                	sd	s1,8(sp)
    198c:	ec06                	sd	ra,24(sp)
    198e:	84aa                	mv	s1,a0
    const char *p = memchr(s, 0, n);
    1990:	f63ff0ef          	jal	ra,18f2 <memchr>
    return p ? p - s : n;
    1994:	c519                	beqz	a0,19a2 <strnlen+0x22>
}
    1996:	60e2                	ld	ra,24(sp)
    1998:	6442                	ld	s0,16(sp)
    return p ? p - s : n;
    199a:	8d05                	sub	a0,a0,s1
}
    199c:	64a2                	ld	s1,8(sp)
    199e:	6105                	addi	sp,sp,32
    19a0:	8082                	ret
    19a2:	60e2                	ld	ra,24(sp)
    return p ? p - s : n;
    19a4:	8522                	mv	a0,s0
}
    19a6:	6442                	ld	s0,16(sp)
    19a8:	64a2                	ld	s1,8(sp)
    19aa:	6105                	addi	sp,sp,32
    19ac:	8082                	ret

00000000000019ae <strcpy>:
char *strcpy(char *restrict d, const char *s)
{
    typedef size_t __attribute__((__may_alias__)) word;
    word *wd;
    const word *ws;
    if ((uintptr_t)s % SS == (uintptr_t)d % SS)
    19ae:	00b547b3          	xor	a5,a0,a1
    19b2:	8b9d                	andi	a5,a5,7
    19b4:	eb95                	bnez	a5,19e8 <strcpy+0x3a>
    {
        for (; (uintptr_t)s % SS; s++, d++)
    19b6:	0075f793          	andi	a5,a1,7
    19ba:	e7b1                	bnez	a5,1a06 <strcpy+0x58>
            if (!(*d = *s))
                return d;
        wd = (void *)d;
        ws = (const void *)s;
        for (; !HASZERO(*ws); *wd++ = *ws++)
    19bc:	6198                	ld	a4,0(a1)
    19be:	00000617          	auipc	a2,0x0
    19c2:	58a63603          	ld	a2,1418(a2) # 1f48 <__clone+0x9c>
    19c6:	00000817          	auipc	a6,0x0
    19ca:	58a83803          	ld	a6,1418(a6) # 1f50 <__clone+0xa4>
    19ce:	a029                	j	19d8 <strcpy+0x2a>
    19d0:	e118                	sd	a4,0(a0)
    19d2:	6598                	ld	a4,8(a1)
    19d4:	05a1                	addi	a1,a1,8
    19d6:	0521                	addi	a0,a0,8
    19d8:	00c707b3          	add	a5,a4,a2
    19dc:	fff74693          	not	a3,a4
    19e0:	8ff5                	and	a5,a5,a3
    19e2:	0107f7b3          	and	a5,a5,a6
    19e6:	d7ed                	beqz	a5,19d0 <strcpy+0x22>
            ;
        d = (void *)wd;
        s = (const void *)ws;
    }
    for (; (*d = *s); s++, d++)
    19e8:	0005c783          	lbu	a5,0(a1)
    19ec:	00f50023          	sb	a5,0(a0)
    19f0:	c785                	beqz	a5,1a18 <strcpy+0x6a>
    19f2:	0015c783          	lbu	a5,1(a1)
    19f6:	0505                	addi	a0,a0,1
    19f8:	0585                	addi	a1,a1,1
    19fa:	00f50023          	sb	a5,0(a0)
    19fe:	fbf5                	bnez	a5,19f2 <strcpy+0x44>
        ;
    return d;
}
    1a00:	8082                	ret
        for (; (uintptr_t)s % SS; s++, d++)
    1a02:	0505                	addi	a0,a0,1
    1a04:	df45                	beqz	a4,19bc <strcpy+0xe>
            if (!(*d = *s))
    1a06:	0005c783          	lbu	a5,0(a1)
        for (; (uintptr_t)s % SS; s++, d++)
    1a0a:	0585                	addi	a1,a1,1
    1a0c:	0075f713          	andi	a4,a1,7
            if (!(*d = *s))
    1a10:	00f50023          	sb	a5,0(a0)
    1a14:	f7fd                	bnez	a5,1a02 <strcpy+0x54>
}
    1a16:	8082                	ret
    1a18:	8082                	ret

0000000000001a1a <strncpy>:
char *strncpy(char *restrict d, const char *s, size_t n)
{
    typedef size_t __attribute__((__may_alias__)) word;
    word *wd;
    const word *ws;
    if (((uintptr_t)s & ALIGN) == ((uintptr_t)d & ALIGN))
    1a1a:	00b547b3          	xor	a5,a0,a1
    1a1e:	8b9d                	andi	a5,a5,7
    1a20:	1a079863          	bnez	a5,1bd0 <strncpy+0x1b6>
    {
        for (; ((uintptr_t)s & ALIGN) && n && (*d = *s); n--, s++, d++)
    1a24:	0075f793          	andi	a5,a1,7
    1a28:	16078463          	beqz	a5,1b90 <strncpy+0x176>
    1a2c:	ea01                	bnez	a2,1a3c <strncpy+0x22>
    1a2e:	a421                	j	1c36 <strncpy+0x21c>
    1a30:	167d                	addi	a2,a2,-1
    1a32:	0505                	addi	a0,a0,1
    1a34:	14070e63          	beqz	a4,1b90 <strncpy+0x176>
    1a38:	1a060863          	beqz	a2,1be8 <strncpy+0x1ce>
    1a3c:	0005c783          	lbu	a5,0(a1)
    1a40:	0585                	addi	a1,a1,1
    1a42:	0075f713          	andi	a4,a1,7
    1a46:	00f50023          	sb	a5,0(a0)
    1a4a:	f3fd                	bnez	a5,1a30 <strncpy+0x16>
    1a4c:	4805                	li	a6,1
    1a4e:	1a061863          	bnez	a2,1bfe <strncpy+0x1e4>
    1a52:	40a007b3          	neg	a5,a0
    1a56:	8b9d                	andi	a5,a5,7
    1a58:	4681                	li	a3,0
    1a5a:	18061a63          	bnez	a2,1bee <strncpy+0x1d4>
    1a5e:	00778713          	addi	a4,a5,7
    1a62:	45ad                	li	a1,11
    1a64:	18b76363          	bltu	a4,a1,1bea <strncpy+0x1d0>
    1a68:	1ae6eb63          	bltu	a3,a4,1c1e <strncpy+0x204>
    1a6c:	1a078363          	beqz	a5,1c12 <strncpy+0x1f8>
    for (int i = 0; i < n; ++i, *(p++) = c)
    1a70:	00050023          	sb	zero,0(a0)
    1a74:	4685                	li	a3,1
    1a76:	00150713          	addi	a4,a0,1
    1a7a:	18d78f63          	beq	a5,a3,1c18 <strncpy+0x1fe>
    1a7e:	000500a3          	sb	zero,1(a0)
    1a82:	4689                	li	a3,2
    1a84:	00250713          	addi	a4,a0,2
    1a88:	18d78e63          	beq	a5,a3,1c24 <strncpy+0x20a>
    1a8c:	00050123          	sb	zero,2(a0)
    1a90:	468d                	li	a3,3
    1a92:	00350713          	addi	a4,a0,3
    1a96:	16d78c63          	beq	a5,a3,1c0e <strncpy+0x1f4>
    1a9a:	000501a3          	sb	zero,3(a0)
    1a9e:	4691                	li	a3,4
    1aa0:	00450713          	addi	a4,a0,4
    1aa4:	18d78263          	beq	a5,a3,1c28 <strncpy+0x20e>
    1aa8:	00050223          	sb	zero,4(a0)
    1aac:	4695                	li	a3,5
    1aae:	00550713          	addi	a4,a0,5
    1ab2:	16d78d63          	beq	a5,a3,1c2c <strncpy+0x212>
    1ab6:	000502a3          	sb	zero,5(a0)
    1aba:	469d                	li	a3,7
    1abc:	00650713          	addi	a4,a0,6
    1ac0:	16d79863          	bne	a5,a3,1c30 <strncpy+0x216>
    1ac4:	00750713          	addi	a4,a0,7
    1ac8:	00050323          	sb	zero,6(a0)
    1acc:	40f80833          	sub	a6,a6,a5
    1ad0:	ff887593          	andi	a1,a6,-8
    1ad4:	97aa                	add	a5,a5,a0
    1ad6:	95be                	add	a1,a1,a5
    1ad8:	0007b023          	sd	zero,0(a5)
    1adc:	07a1                	addi	a5,a5,8
    1ade:	feb79de3          	bne	a5,a1,1ad8 <strncpy+0xbe>
    1ae2:	ff887593          	andi	a1,a6,-8
    1ae6:	9ead                	addw	a3,a3,a1
    1ae8:	00b707b3          	add	a5,a4,a1
    1aec:	12b80863          	beq	a6,a1,1c1c <strncpy+0x202>
    1af0:	00078023          	sb	zero,0(a5)
    1af4:	0016871b          	addiw	a4,a3,1
    1af8:	0ec77863          	bgeu	a4,a2,1be8 <strncpy+0x1ce>
    1afc:	000780a3          	sb	zero,1(a5)
    1b00:	0026871b          	addiw	a4,a3,2
    1b04:	0ec77263          	bgeu	a4,a2,1be8 <strncpy+0x1ce>
    1b08:	00078123          	sb	zero,2(a5)
    1b0c:	0036871b          	addiw	a4,a3,3
    1b10:	0cc77c63          	bgeu	a4,a2,1be8 <strncpy+0x1ce>
    1b14:	000781a3          	sb	zero,3(a5)
    1b18:	0046871b          	addiw	a4,a3,4
    1b1c:	0cc77663          	bgeu	a4,a2,1be8 <strncpy+0x1ce>
    1b20:	00078223          	sb	zero,4(a5)
    1b24:	0056871b          	addiw	a4,a3,5
    1b28:	0cc77063          	bgeu	a4,a2,1be8 <strncpy+0x1ce>
    1b2c:	000782a3          	sb	zero,5(a5)
    1b30:	0066871b          	addiw	a4,a3,6
    1b34:	0ac77a63          	bgeu	a4,a2,1be8 <strncpy+0x1ce>
    1b38:	00078323          	sb	zero,6(a5)
    1b3c:	0076871b          	addiw	a4,a3,7
    1b40:	0ac77463          	bgeu	a4,a2,1be8 <strncpy+0x1ce>
    1b44:	000783a3          	sb	zero,7(a5)
    1b48:	0086871b          	addiw	a4,a3,8
    1b4c:	08c77e63          	bgeu	a4,a2,1be8 <strncpy+0x1ce>
    1b50:	00078423          	sb	zero,8(a5)
    1b54:	0096871b          	addiw	a4,a3,9
    1b58:	08c77863          	bgeu	a4,a2,1be8 <strncpy+0x1ce>
    1b5c:	000784a3          	sb	zero,9(a5)
    1b60:	00a6871b          	addiw	a4,a3,10
    1b64:	08c77263          	bgeu	a4,a2,1be8 <strncpy+0x1ce>
    1b68:	00078523          	sb	zero,10(a5)
    1b6c:	00b6871b          	addiw	a4,a3,11
    1b70:	06c77c63          	bgeu	a4,a2,1be8 <strncpy+0x1ce>
    1b74:	000785a3          	sb	zero,11(a5)
    1b78:	00c6871b          	addiw	a4,a3,12
    1b7c:	06c77663          	bgeu	a4,a2,1be8 <strncpy+0x1ce>
    1b80:	00078623          	sb	zero,12(a5)
    1b84:	26b5                	addiw	a3,a3,13
    1b86:	06c6f163          	bgeu	a3,a2,1be8 <strncpy+0x1ce>
    1b8a:	000786a3          	sb	zero,13(a5)
    1b8e:	8082                	ret
            ;
        if (!n || !*s)
    1b90:	c645                	beqz	a2,1c38 <strncpy+0x21e>
    1b92:	0005c783          	lbu	a5,0(a1)
    1b96:	ea078be3          	beqz	a5,1a4c <strncpy+0x32>
            goto tail;
        wd = (void *)d;
        ws = (const void *)s;
        for (; n >= sizeof(size_t) && !HASZERO(*ws); n -= sizeof(size_t), ws++, wd++)
    1b9a:	479d                	li	a5,7
    1b9c:	02c7ff63          	bgeu	a5,a2,1bda <strncpy+0x1c0>
    1ba0:	00000897          	auipc	a7,0x0
    1ba4:	3a88b883          	ld	a7,936(a7) # 1f48 <__clone+0x9c>
    1ba8:	00000817          	auipc	a6,0x0
    1bac:	3a883803          	ld	a6,936(a6) # 1f50 <__clone+0xa4>
    1bb0:	431d                	li	t1,7
    1bb2:	6198                	ld	a4,0(a1)
    1bb4:	011707b3          	add	a5,a4,a7
    1bb8:	fff74693          	not	a3,a4
    1bbc:	8ff5                	and	a5,a5,a3
    1bbe:	0107f7b3          	and	a5,a5,a6
    1bc2:	ef81                	bnez	a5,1bda <strncpy+0x1c0>
            *wd = *ws;
    1bc4:	e118                	sd	a4,0(a0)
        for (; n >= sizeof(size_t) && !HASZERO(*ws); n -= sizeof(size_t), ws++, wd++)
    1bc6:	1661                	addi	a2,a2,-8
    1bc8:	05a1                	addi	a1,a1,8
    1bca:	0521                	addi	a0,a0,8
    1bcc:	fec363e3          	bltu	t1,a2,1bb2 <strncpy+0x198>
        d = (void *)wd;
        s = (const void *)ws;
    }
    for (; n && (*d = *s); n--, s++, d++)
    1bd0:	e609                	bnez	a2,1bda <strncpy+0x1c0>
    1bd2:	a08d                	j	1c34 <strncpy+0x21a>
    1bd4:	167d                	addi	a2,a2,-1
    1bd6:	0505                	addi	a0,a0,1
    1bd8:	ca01                	beqz	a2,1be8 <strncpy+0x1ce>
    1bda:	0005c783          	lbu	a5,0(a1)
    1bde:	0585                	addi	a1,a1,1
    1be0:	00f50023          	sb	a5,0(a0)
    1be4:	fbe5                	bnez	a5,1bd4 <strncpy+0x1ba>
        ;
tail:
    1be6:	b59d                	j	1a4c <strncpy+0x32>
    memset(d, 0, n);
    return d;
}
    1be8:	8082                	ret
    1bea:	472d                	li	a4,11
    1bec:	bdb5                	j	1a68 <strncpy+0x4e>
    1bee:	00778713          	addi	a4,a5,7
    1bf2:	45ad                	li	a1,11
    1bf4:	fff60693          	addi	a3,a2,-1
    1bf8:	e6b778e3          	bgeu	a4,a1,1a68 <strncpy+0x4e>
    1bfc:	b7fd                	j	1bea <strncpy+0x1d0>
    1bfe:	40a007b3          	neg	a5,a0
    1c02:	8832                	mv	a6,a2
    1c04:	8b9d                	andi	a5,a5,7
    1c06:	4681                	li	a3,0
    1c08:	e4060be3          	beqz	a2,1a5e <strncpy+0x44>
    1c0c:	b7cd                	j	1bee <strncpy+0x1d4>
    for (int i = 0; i < n; ++i, *(p++) = c)
    1c0e:	468d                	li	a3,3
    1c10:	bd75                	j	1acc <strncpy+0xb2>
    1c12:	872a                	mv	a4,a0
    1c14:	4681                	li	a3,0
    1c16:	bd5d                	j	1acc <strncpy+0xb2>
    1c18:	4685                	li	a3,1
    1c1a:	bd4d                	j	1acc <strncpy+0xb2>
    1c1c:	8082                	ret
    1c1e:	87aa                	mv	a5,a0
    1c20:	4681                	li	a3,0
    1c22:	b5f9                	j	1af0 <strncpy+0xd6>
    1c24:	4689                	li	a3,2
    1c26:	b55d                	j	1acc <strncpy+0xb2>
    1c28:	4691                	li	a3,4
    1c2a:	b54d                	j	1acc <strncpy+0xb2>
    1c2c:	4695                	li	a3,5
    1c2e:	bd79                	j	1acc <strncpy+0xb2>
    1c30:	4699                	li	a3,6
    1c32:	bd69                	j	1acc <strncpy+0xb2>
    1c34:	8082                	ret
    1c36:	8082                	ret
    1c38:	8082                	ret

0000000000001c3a <open>:
#include <unistd.h>

#include "syscall.h"

int open(const char *path, int flags)
{
    1c3a:	87aa                	mv	a5,a0
    1c3c:	862e                	mv	a2,a1
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
}

static inline long __syscall4(long n, long a, long b, long c, long d)
{
    register long a7 __asm__("a7") = n;
    1c3e:	03800893          	li	a7,56
    register long a0 __asm__("a0") = a;
    1c42:	f9c00513          	li	a0,-100
    register long a1 __asm__("a1") = b;
    1c46:	85be                	mv	a1,a5
    register long a2 __asm__("a2") = c;
    register long a3 __asm__("a3") = d;
    1c48:	4689                	li	a3,2
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3))
    1c4a:	00000073          	ecall
    return syscall(SYS_openat, AT_FDCWD, path, flags, O_RDWR);
}
    1c4e:	2501                	sext.w	a0,a0
    1c50:	8082                	ret

0000000000001c52 <openat>:
    register long a7 __asm__("a7") = n;
    1c52:	03800893          	li	a7,56
    register long a3 __asm__("a3") = d;
    1c56:	18000693          	li	a3,384
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3))
    1c5a:	00000073          	ecall

int openat(int dirfd,const char *path, int flags)
{
    return syscall(SYS_openat, dirfd, path, flags, 0600);
}
    1c5e:	2501                	sext.w	a0,a0
    1c60:	8082                	ret

0000000000001c62 <close>:
    register long a7 __asm__("a7") = n;
    1c62:	03900893          	li	a7,57
    __asm_syscall("r"(a7), "0"(a0))
    1c66:	00000073          	ecall

int close(int fd)
{
    return syscall(SYS_close, fd);
}
    1c6a:	2501                	sext.w	a0,a0
    1c6c:	8082                	ret

0000000000001c6e <read>:
    register long a7 __asm__("a7") = n;
    1c6e:	03f00893          	li	a7,63
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1c72:	00000073          	ecall

ssize_t read(int fd, void *buf, size_t len)
{
    return syscall(SYS_read, fd, buf, len);
}
    1c76:	8082                	ret

0000000000001c78 <write>:
    register long a7 __asm__("a7") = n;
    1c78:	04000893          	li	a7,64
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1c7c:	00000073          	ecall

ssize_t write(int fd, const void *buf, size_t len)
{
    return syscall(SYS_write, fd, buf, len);
}
    1c80:	8082                	ret

0000000000001c82 <getpid>:
    register long a7 __asm__("a7") = n;
    1c82:	0ac00893          	li	a7,172
    __asm_syscall("r"(a7))
    1c86:	00000073          	ecall

pid_t getpid(void)
{
    return syscall(SYS_getpid);
}
    1c8a:	2501                	sext.w	a0,a0
    1c8c:	8082                	ret

0000000000001c8e <getppid>:
    register long a7 __asm__("a7") = n;
    1c8e:	0ad00893          	li	a7,173
    __asm_syscall("r"(a7))
    1c92:	00000073          	ecall

pid_t getppid(void)
{
    return syscall(SYS_getppid);
}
    1c96:	2501                	sext.w	a0,a0
    1c98:	8082                	ret

0000000000001c9a <sched_yield>:
    register long a7 __asm__("a7") = n;
    1c9a:	07c00893          	li	a7,124
    __asm_syscall("r"(a7))
    1c9e:	00000073          	ecall

int sched_yield(void)
{
    return syscall(SYS_sched_yield);
}
    1ca2:	2501                	sext.w	a0,a0
    1ca4:	8082                	ret

0000000000001ca6 <fork>:
    register long a7 __asm__("a7") = n;
    1ca6:	0dc00893          	li	a7,220
    register long a0 __asm__("a0") = a;
    1caa:	4545                	li	a0,17
    register long a1 __asm__("a1") = b;
    1cac:	4581                	li	a1,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1cae:	00000073          	ecall

pid_t fork(void)
{
    return syscall(SYS_clone, SIGCHLD, 0);
}
    1cb2:	2501                	sext.w	a0,a0
    1cb4:	8082                	ret

0000000000001cb6 <clone>:

pid_t clone(int (*fn)(void *arg), void *arg, void *stack, size_t stack_size, unsigned long flags)
{
    1cb6:	85b2                	mv	a1,a2
    1cb8:	863a                	mv	a2,a4
    if (stack)
    1cba:	c191                	beqz	a1,1cbe <clone+0x8>
	stack += stack_size;
    1cbc:	95b6                	add	a1,a1,a3

    return __clone(fn, stack, flags, NULL, NULL, NULL);
    1cbe:	4781                	li	a5,0
    1cc0:	4701                	li	a4,0
    1cc2:	4681                	li	a3,0
    1cc4:	2601                	sext.w	a2,a2
    1cc6:	a2dd                	j	1eac <__clone>

0000000000001cc8 <exit>:
    register long a7 __asm__("a7") = n;
    1cc8:	05d00893          	li	a7,93
    __asm_syscall("r"(a7), "0"(a0))
    1ccc:	00000073          	ecall
    //return syscall(SYS_clone, fn, stack, flags, NULL, NULL, NULL);
}
void exit(int code)
{
    syscall(SYS_exit, code);
}
    1cd0:	8082                	ret

0000000000001cd2 <waitpid>:
    register long a7 __asm__("a7") = n;
    1cd2:	10400893          	li	a7,260
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1cd6:	00000073          	ecall

int waitpid(int pid, int *code, int options)
{
    return syscall(SYS_wait4, pid, code, options);
}
    1cda:	2501                	sext.w	a0,a0
    1cdc:	8082                	ret

0000000000001cde <exec>:
    register long a7 __asm__("a7") = n;
    1cde:	0dd00893          	li	a7,221
    __asm_syscall("r"(a7), "0"(a0))
    1ce2:	00000073          	ecall

int exec(char *name)
{
    return syscall(SYS_execve, name);
}
    1ce6:	2501                	sext.w	a0,a0
    1ce8:	8082                	ret

0000000000001cea <execve>:
    register long a7 __asm__("a7") = n;
    1cea:	0dd00893          	li	a7,221
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1cee:	00000073          	ecall

int execve(const char *name, char *const argv[], char *const argp[])
{
    return syscall(SYS_execve, name, argv, argp);
}
    1cf2:	2501                	sext.w	a0,a0
    1cf4:	8082                	ret

0000000000001cf6 <times>:
    register long a7 __asm__("a7") = n;
    1cf6:	09900893          	li	a7,153
    __asm_syscall("r"(a7), "0"(a0))
    1cfa:	00000073          	ecall

int times(void *mytimes)
{
	return syscall(SYS_times, mytimes);
}
    1cfe:	2501                	sext.w	a0,a0
    1d00:	8082                	ret

0000000000001d02 <get_time>:

int64 get_time()
{
    1d02:	1141                	addi	sp,sp,-16
    register long a7 __asm__("a7") = n;
    1d04:	0a900893          	li	a7,169
    register long a0 __asm__("a0") = a;
    1d08:	850a                	mv	a0,sp
    register long a1 __asm__("a1") = b;
    1d0a:	4581                	li	a1,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1d0c:	00000073          	ecall
    TimeVal time;
    int err = sys_get_time(&time, 0);
    if (err == 0)
    1d10:	2501                	sext.w	a0,a0
    1d12:	ed09                	bnez	a0,1d2c <get_time+0x2a>
    {
        return ((time.sec & 0xffff) * 1000 + time.usec / 1000);
    1d14:	67a2                	ld	a5,8(sp)
    1d16:	3e800713          	li	a4,1000
    1d1a:	00015503          	lhu	a0,0(sp)
    1d1e:	02e7d7b3          	divu	a5,a5,a4
    1d22:	02e50533          	mul	a0,a0,a4
    1d26:	953e                	add	a0,a0,a5
    }
    else
    {
        return -1;
    }
}
    1d28:	0141                	addi	sp,sp,16
    1d2a:	8082                	ret
        return -1;
    1d2c:	557d                	li	a0,-1
    1d2e:	bfed                	j	1d28 <get_time+0x26>

0000000000001d30 <sys_get_time>:
    register long a7 __asm__("a7") = n;
    1d30:	0a900893          	li	a7,169
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1d34:	00000073          	ecall

int sys_get_time(TimeVal *ts, int tz)
{
    return syscall(SYS_gettimeofday, ts, tz);
}
    1d38:	2501                	sext.w	a0,a0
    1d3a:	8082                	ret

0000000000001d3c <time>:
    register long a7 __asm__("a7") = n;
    1d3c:	42600893          	li	a7,1062
    __asm_syscall("r"(a7), "0"(a0))
    1d40:	00000073          	ecall

int time(unsigned long *tloc)
{
    return syscall(SYS_time, tloc);
}
    1d44:	2501                	sext.w	a0,a0
    1d46:	8082                	ret

0000000000001d48 <sleep>:

int sleep(unsigned long long time)
{
    1d48:	1141                	addi	sp,sp,-16
    TimeVal tv = {.sec = time, .usec = 0};
    1d4a:	e02a                	sd	a0,0(sp)
    register long a0 __asm__("a0") = a;
    1d4c:	850a                	mv	a0,sp
    1d4e:	e402                	sd	zero,8(sp)
    register long a7 __asm__("a7") = n;
    1d50:	06500893          	li	a7,101
    register long a1 __asm__("a1") = b;
    1d54:	85aa                	mv	a1,a0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1d56:	00000073          	ecall
    if (syscall(SYS_nanosleep, &tv, &tv)) return tv.sec;
    1d5a:	e501                	bnez	a0,1d62 <sleep+0x1a>
    return 0;
    1d5c:	4501                	li	a0,0
}
    1d5e:	0141                	addi	sp,sp,16
    1d60:	8082                	ret
    if (syscall(SYS_nanosleep, &tv, &tv)) return tv.sec;
    1d62:	4502                	lw	a0,0(sp)
}
    1d64:	0141                	addi	sp,sp,16
    1d66:	8082                	ret

0000000000001d68 <set_priority>:
    register long a7 __asm__("a7") = n;
    1d68:	08c00893          	li	a7,140
    __asm_syscall("r"(a7), "0"(a0))
    1d6c:	00000073          	ecall

int set_priority(int prio)
{
    return syscall(SYS_setpriority, prio);
}
    1d70:	2501                	sext.w	a0,a0
    1d72:	8082                	ret

0000000000001d74 <mmap>:
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4))
}

static inline long __syscall6(long n, long a, long b, long c, long d, long e, long f)
{
    register long a7 __asm__("a7") = n;
    1d74:	0de00893          	li	a7,222
    register long a1 __asm__("a1") = b;
    register long a2 __asm__("a2") = c;
    register long a3 __asm__("a3") = d;
    register long a4 __asm__("a4") = e;
    register long a5 __asm__("a5") = f;
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4), "r"(a5))
    1d78:	00000073          	ecall

void *mmap(void *start, size_t len, int prot, int flags, int fd, off_t off)
{
    return syscall(SYS_mmap, start, len, prot, flags, fd, off);
}
    1d7c:	8082                	ret

0000000000001d7e <munmap>:
    register long a7 __asm__("a7") = n;
    1d7e:	0d700893          	li	a7,215
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1d82:	00000073          	ecall

int munmap(void *start, size_t len)
{
    return syscall(SYS_munmap, start, len);
}
    1d86:	2501                	sext.w	a0,a0
    1d88:	8082                	ret

0000000000001d8a <wait>:

int wait(int *code)
{
    1d8a:	85aa                	mv	a1,a0
    register long a7 __asm__("a7") = n;
    1d8c:	10400893          	li	a7,260
    register long a0 __asm__("a0") = a;
    1d90:	557d                	li	a0,-1
    register long a2 __asm__("a2") = c;
    1d92:	4601                	li	a2,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1d94:	00000073          	ecall
    return waitpid(-1, code, 0);
}
    1d98:	2501                	sext.w	a0,a0
    1d9a:	8082                	ret

0000000000001d9c <spawn>:
    register long a7 __asm__("a7") = n;
    1d9c:	19000893          	li	a7,400
    __asm_syscall("r"(a7), "0"(a0))
    1da0:	00000073          	ecall

int spawn(char *file)
{
    return syscall(SYS_spawn, file);
}
    1da4:	2501                	sext.w	a0,a0
    1da6:	8082                	ret

0000000000001da8 <mailread>:
    register long a7 __asm__("a7") = n;
    1da8:	19100893          	li	a7,401
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1dac:	00000073          	ecall

int mailread(void *buf, int len)
{
    return syscall(SYS_mailread, buf, len);
}
    1db0:	2501                	sext.w	a0,a0
    1db2:	8082                	ret

0000000000001db4 <mailwrite>:
    register long a7 __asm__("a7") = n;
    1db4:	19200893          	li	a7,402
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1db8:	00000073          	ecall

int mailwrite(int pid, void *buf, int len)
{
    return syscall(SYS_mailwrite, pid, buf, len);
}
    1dbc:	2501                	sext.w	a0,a0
    1dbe:	8082                	ret

0000000000001dc0 <fstat>:
    register long a7 __asm__("a7") = n;
    1dc0:	05000893          	li	a7,80
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1dc4:	00000073          	ecall

int fstat(int fd, struct kstat *st)
{
    return syscall(SYS_fstat, fd, st);
}
    1dc8:	2501                	sext.w	a0,a0
    1dca:	8082                	ret

0000000000001dcc <sys_linkat>:
    register long a4 __asm__("a4") = e;
    1dcc:	1702                	slli	a4,a4,0x20
    register long a7 __asm__("a7") = n;
    1dce:	02500893          	li	a7,37
    register long a4 __asm__("a4") = e;
    1dd2:	9301                	srli	a4,a4,0x20
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4))
    1dd4:	00000073          	ecall

int sys_linkat(int olddirfd, char *oldpath, int newdirfd, char *newpath, unsigned int flags)
{
    return syscall(SYS_linkat, olddirfd, oldpath, newdirfd, newpath, flags);
}
    1dd8:	2501                	sext.w	a0,a0
    1dda:	8082                	ret

0000000000001ddc <sys_unlinkat>:
    register long a2 __asm__("a2") = c;
    1ddc:	1602                	slli	a2,a2,0x20
    register long a7 __asm__("a7") = n;
    1dde:	02300893          	li	a7,35
    register long a2 __asm__("a2") = c;
    1de2:	9201                	srli	a2,a2,0x20
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1de4:	00000073          	ecall

int sys_unlinkat(int dirfd, char *path, unsigned int flags)
{
    return syscall(SYS_unlinkat, dirfd, path, flags);
}
    1de8:	2501                	sext.w	a0,a0
    1dea:	8082                	ret

0000000000001dec <link>:

int link(char *old_path, char *new_path)
{
    1dec:	87aa                	mv	a5,a0
    1dee:	86ae                	mv	a3,a1
    register long a7 __asm__("a7") = n;
    1df0:	02500893          	li	a7,37
    register long a0 __asm__("a0") = a;
    1df4:	f9c00513          	li	a0,-100
    register long a1 __asm__("a1") = b;
    1df8:	85be                	mv	a1,a5
    register long a2 __asm__("a2") = c;
    1dfa:	f9c00613          	li	a2,-100
    register long a4 __asm__("a4") = e;
    1dfe:	4701                	li	a4,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4))
    1e00:	00000073          	ecall
    return sys_linkat(AT_FDCWD, old_path, AT_FDCWD, new_path, 0);
}
    1e04:	2501                	sext.w	a0,a0
    1e06:	8082                	ret

0000000000001e08 <unlink>:

int unlink(char *path)
{
    1e08:	85aa                	mv	a1,a0
    register long a7 __asm__("a7") = n;
    1e0a:	02300893          	li	a7,35
    register long a0 __asm__("a0") = a;
    1e0e:	f9c00513          	li	a0,-100
    register long a2 __asm__("a2") = c;
    1e12:	4601                	li	a2,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1e14:	00000073          	ecall
    return sys_unlinkat(AT_FDCWD, path, 0);
}
    1e18:	2501                	sext.w	a0,a0
    1e1a:	8082                	ret

0000000000001e1c <uname>:
    register long a7 __asm__("a7") = n;
    1e1c:	0a000893          	li	a7,160
    __asm_syscall("r"(a7), "0"(a0))
    1e20:	00000073          	ecall

int uname(void *buf)
{
    return syscall(SYS_uname, buf);
}
    1e24:	2501                	sext.w	a0,a0
    1e26:	8082                	ret

0000000000001e28 <brk>:
    register long a7 __asm__("a7") = n;
    1e28:	0d600893          	li	a7,214
    __asm_syscall("r"(a7), "0"(a0))
    1e2c:	00000073          	ecall

int brk(void *addr)
{
    return syscall(SYS_brk, addr);
}
    1e30:	2501                	sext.w	a0,a0
    1e32:	8082                	ret

0000000000001e34 <getcwd>:
    register long a7 __asm__("a7") = n;
    1e34:	48c5                	li	a7,17
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1e36:	00000073          	ecall

char *getcwd(char *buf, size_t size){
    return syscall(SYS_getcwd, buf, size);
}
    1e3a:	8082                	ret

0000000000001e3c <chdir>:
    register long a7 __asm__("a7") = n;
    1e3c:	03100893          	li	a7,49
    __asm_syscall("r"(a7), "0"(a0))
    1e40:	00000073          	ecall

int chdir(const char *path){
    return syscall(SYS_chdir, path);
}
    1e44:	2501                	sext.w	a0,a0
    1e46:	8082                	ret

0000000000001e48 <mkdir>:

int mkdir(const char *path, mode_t mode){
    1e48:	862e                	mv	a2,a1
    1e4a:	87aa                	mv	a5,a0
    register long a2 __asm__("a2") = c;
    1e4c:	1602                	slli	a2,a2,0x20
    register long a7 __asm__("a7") = n;
    1e4e:	02200893          	li	a7,34
    register long a0 __asm__("a0") = a;
    1e52:	f9c00513          	li	a0,-100
    register long a1 __asm__("a1") = b;
    1e56:	85be                	mv	a1,a5
    register long a2 __asm__("a2") = c;
    1e58:	9201                	srli	a2,a2,0x20
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1e5a:	00000073          	ecall
    return syscall(SYS_mkdirat, AT_FDCWD, path, mode);
}
    1e5e:	2501                	sext.w	a0,a0
    1e60:	8082                	ret

0000000000001e62 <getdents>:
    register long a7 __asm__("a7") = n;
    1e62:	03d00893          	li	a7,61
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1e66:	00000073          	ecall

int getdents(int fd, struct linux_dirent64 *dirp64, unsigned long len){
    //return syscall(SYS_getdents64, fd, dirp64, len);
    return syscall(SYS_getdents64, fd, dirp64, len);
}
    1e6a:	2501                	sext.w	a0,a0
    1e6c:	8082                	ret

0000000000001e6e <pipe>:
    register long a7 __asm__("a7") = n;
    1e6e:	03b00893          	li	a7,59
    register long a1 __asm__("a1") = b;
    1e72:	4581                	li	a1,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1e74:	00000073          	ecall

int pipe(int fd[2]){
    return syscall(SYS_pipe2, fd, 0);
}
    1e78:	2501                	sext.w	a0,a0
    1e7a:	8082                	ret

0000000000001e7c <dup>:
    register long a7 __asm__("a7") = n;
    1e7c:	48dd                	li	a7,23
    __asm_syscall("r"(a7), "0"(a0))
    1e7e:	00000073          	ecall

int dup(int fd){
    return syscall(SYS_dup, fd);
}
    1e82:	2501                	sext.w	a0,a0
    1e84:	8082                	ret

0000000000001e86 <dup2>:
    register long a7 __asm__("a7") = n;
    1e86:	48e1                	li	a7,24
    register long a2 __asm__("a2") = c;
    1e88:	4601                	li	a2,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1e8a:	00000073          	ecall

int dup2(int old, int new){
    return syscall(SYS_dup3, old, new, 0);
}
    1e8e:	2501                	sext.w	a0,a0
    1e90:	8082                	ret

0000000000001e92 <mount>:
    register long a7 __asm__("a7") = n;
    1e92:	02800893          	li	a7,40
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4))
    1e96:	00000073          	ecall

int mount(const char *special, const char *dir, const char *fstype, unsigned long flags, const void *data)
{
        return syscall(SYS_mount, special, dir, fstype, flags, data);
}
    1e9a:	2501                	sext.w	a0,a0
    1e9c:	8082                	ret

0000000000001e9e <umount>:
    register long a7 __asm__("a7") = n;
    1e9e:	02700893          	li	a7,39
    register long a1 __asm__("a1") = b;
    1ea2:	4581                	li	a1,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1ea4:	00000073          	ecall

int umount(const char *special)
{
        return syscall(SYS_umount2, special, 0);
}
    1ea8:	2501                	sext.w	a0,a0
    1eaa:	8082                	ret

0000000000001eac <__clone>:

.global __clone
.type  __clone, %function
__clone:
	# Save func and arg to stack
	addi a1, a1, -16
    1eac:	15c1                	addi	a1,a1,-16
	sd a0, 0(a1)
    1eae:	e188                	sd	a0,0(a1)
	sd a3, 8(a1)
    1eb0:	e594                	sd	a3,8(a1)

	# Call SYS_clone
	mv a0, a2
    1eb2:	8532                	mv	a0,a2
	mv a2, a4
    1eb4:	863a                	mv	a2,a4
	mv a3, a5
    1eb6:	86be                	mv	a3,a5
	mv a4, a6
    1eb8:	8742                	mv	a4,a6
	li a7, 220 # SYS_clone
    1eba:	0dc00893          	li	a7,220
	ecall
    1ebe:	00000073          	ecall

	beqz a0, 1f
    1ec2:	c111                	beqz	a0,1ec6 <__clone+0x1a>
	# Parent
	ret
    1ec4:	8082                	ret

	# Child
1:      ld a1, 0(sp)
    1ec6:	6582                	ld	a1,0(sp)
	ld a0, 8(sp)
    1ec8:	6522                	ld	a0,8(sp)
	jalr a1
    1eca:	9582                	jalr	a1

	# Exit
	li a7, 93 # SYS_exit
    1ecc:	05d00893          	li	a7,93
	ecall
    1ed0:	00000073          	ecall
