
/home/ubuntu/rust/ultra-os/codes/riscv-syscalls-testing/user/build/riscv64/getppid:     file format elf64-littleriscv


Disassembly of section .text:

0000000000001000 <_start>:
.section .text.entry
.globl _start
_start:
    mv a0, sp
    1000:	850a                	mv	a0,sp
    tail __start_main
    1002:	a051                	j	1086 <__start_main>

0000000000001004 <test_getppid>:
 * 不能通过测试则输出：
 * "  getppid error."
 */

int test_getppid()
{
    1004:	1141                	addi	sp,sp,-16
    TEST_START(__func__);
    1006:	00001517          	auipc	a0,0x1
    100a:	eaa50513          	addi	a0,a0,-342 # 1eb0 <__clone+0x2e>
{
    100e:	e406                	sd	ra,8(sp)
    TEST_START(__func__);
    1010:	2e8000ef          	jal	ra,12f8 <puts>
    1014:	00001517          	auipc	a0,0x1
    1018:	f2c50513          	addi	a0,a0,-212 # 1f40 <__func__.0>
    101c:	2dc000ef          	jal	ra,12f8 <puts>
    1020:	00001517          	auipc	a0,0x1
    1024:	ea850513          	addi	a0,a0,-344 # 1ec8 <__clone+0x46>
    1028:	2d0000ef          	jal	ra,12f8 <puts>
    pid_t ppid = getppid();
    102c:	439000ef          	jal	ra,1c64 <getppid>
    if(ppid > 0) printf("  getppid success. ppid : %d\n", ppid);
    1030:	02a05c63          	blez	a0,1068 <test_getppid+0x64>
    1034:	85aa                	mv	a1,a0
    1036:	00001517          	auipc	a0,0x1
    103a:	ea250513          	addi	a0,a0,-350 # 1ed8 <__clone+0x56>
    103e:	2dc000ef          	jal	ra,131a <printf>
    else printf("  getppid error.\n");
    TEST_END(__func__);
    1042:	00001517          	auipc	a0,0x1
    1046:	ece50513          	addi	a0,a0,-306 # 1f10 <__clone+0x8e>
    104a:	2ae000ef          	jal	ra,12f8 <puts>
    104e:	00001517          	auipc	a0,0x1
    1052:	ef250513          	addi	a0,a0,-270 # 1f40 <__func__.0>
    1056:	2a2000ef          	jal	ra,12f8 <puts>
}
    105a:	60a2                	ld	ra,8(sp)
    TEST_END(__func__);
    105c:	00001517          	auipc	a0,0x1
    1060:	e6c50513          	addi	a0,a0,-404 # 1ec8 <__clone+0x46>
}
    1064:	0141                	addi	sp,sp,16
    TEST_END(__func__);
    1066:	ac49                	j	12f8 <puts>
    else printf("  getppid error.\n");
    1068:	00001517          	auipc	a0,0x1
    106c:	e9050513          	addi	a0,a0,-368 # 1ef8 <__clone+0x76>
    1070:	2aa000ef          	jal	ra,131a <printf>
    1074:	b7f9                	j	1042 <test_getppid+0x3e>

0000000000001076 <main>:

int main(void) {
    1076:	1141                	addi	sp,sp,-16
    1078:	e406                	sd	ra,8(sp)
	test_getppid();
    107a:	f8bff0ef          	jal	ra,1004 <test_getppid>
	return 0;
}
    107e:	60a2                	ld	ra,8(sp)
    1080:	4501                	li	a0,0
    1082:	0141                	addi	sp,sp,16
    1084:	8082                	ret

0000000000001086 <__start_main>:
#include <unistd.h>

extern int main();

int __start_main(long *p)
{
    1086:	85aa                	mv	a1,a0
	int argc = p[0];
	char **argv = (void *)(p+1);

	exit(main(argc, argv));
    1088:	4108                	lw	a0,0(a0)
{
    108a:	1141                	addi	sp,sp,-16
	exit(main(argc, argv));
    108c:	05a1                	addi	a1,a1,8
{
    108e:	e406                	sd	ra,8(sp)
	exit(main(argc, argv));
    1090:	fe7ff0ef          	jal	ra,1076 <main>
    1094:	40b000ef          	jal	ra,1c9e <exit>
	return 0;
}
    1098:	60a2                	ld	ra,8(sp)
    109a:	4501                	li	a0,0
    109c:	0141                	addi	sp,sp,16
    109e:	8082                	ret

00000000000010a0 <printint.constprop.0>:
    write(f, s, l);
}

static char digits[] = "0123456789abcdef";

static void printint(int xx, int base, int sign)
    10a0:	7179                	addi	sp,sp,-48
    10a2:	f406                	sd	ra,40(sp)
{
    char buf[16 + 1];
    int i;
    uint x;

    if (sign && (sign = xx < 0))
    10a4:	12054b63          	bltz	a0,11da <printint.constprop.0+0x13a>

    buf[16] = 0;
    i = 15;
    do
    {
        buf[i--] = digits[x % base];
    10a8:	02b577bb          	remuw	a5,a0,a1
    10ac:	00001617          	auipc	a2,0x1
    10b0:	ea460613          	addi	a2,a2,-348 # 1f50 <digits>
    buf[16] = 0;
    10b4:	00010c23          	sb	zero,24(sp)
        buf[i--] = digits[x % base];
    10b8:	0005871b          	sext.w	a4,a1
    10bc:	1782                	slli	a5,a5,0x20
    10be:	9381                	srli	a5,a5,0x20
    10c0:	97b2                	add	a5,a5,a2
    10c2:	0007c783          	lbu	a5,0(a5)
    } while ((x /= base) != 0);
    10c6:	02b5583b          	divuw	a6,a0,a1
        buf[i--] = digits[x % base];
    10ca:	00f10ba3          	sb	a5,23(sp)
    } while ((x /= base) != 0);
    10ce:	1cb56363          	bltu	a0,a1,1294 <printint.constprop.0+0x1f4>
        buf[i--] = digits[x % base];
    10d2:	45b9                	li	a1,14
    10d4:	02e877bb          	remuw	a5,a6,a4
    10d8:	1782                	slli	a5,a5,0x20
    10da:	9381                	srli	a5,a5,0x20
    10dc:	97b2                	add	a5,a5,a2
    10de:	0007c783          	lbu	a5,0(a5)
    } while ((x /= base) != 0);
    10e2:	02e856bb          	divuw	a3,a6,a4
        buf[i--] = digits[x % base];
    10e6:	00f10b23          	sb	a5,22(sp)
    } while ((x /= base) != 0);
    10ea:	0ce86e63          	bltu	a6,a4,11c6 <printint.constprop.0+0x126>
        buf[i--] = digits[x % base];
    10ee:	02e6f5bb          	remuw	a1,a3,a4
    } while ((x /= base) != 0);
    10f2:	02e6d7bb          	divuw	a5,a3,a4
        buf[i--] = digits[x % base];
    10f6:	1582                	slli	a1,a1,0x20
    10f8:	9181                	srli	a1,a1,0x20
    10fa:	95b2                	add	a1,a1,a2
    10fc:	0005c583          	lbu	a1,0(a1)
    1100:	00b10aa3          	sb	a1,21(sp)
    } while ((x /= base) != 0);
    1104:	0007859b          	sext.w	a1,a5
    1108:	12e6ec63          	bltu	a3,a4,1240 <printint.constprop.0+0x1a0>
        buf[i--] = digits[x % base];
    110c:	02e7f6bb          	remuw	a3,a5,a4
    1110:	1682                	slli	a3,a3,0x20
    1112:	9281                	srli	a3,a3,0x20
    1114:	96b2                	add	a3,a3,a2
    1116:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    111a:	02e7d83b          	divuw	a6,a5,a4
        buf[i--] = digits[x % base];
    111e:	00d10a23          	sb	a3,20(sp)
    } while ((x /= base) != 0);
    1122:	12e5e863          	bltu	a1,a4,1252 <printint.constprop.0+0x1b2>
        buf[i--] = digits[x % base];
    1126:	02e876bb          	remuw	a3,a6,a4
    112a:	1682                	slli	a3,a3,0x20
    112c:	9281                	srli	a3,a3,0x20
    112e:	96b2                	add	a3,a3,a2
    1130:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    1134:	02e855bb          	divuw	a1,a6,a4
        buf[i--] = digits[x % base];
    1138:	00d109a3          	sb	a3,19(sp)
    } while ((x /= base) != 0);
    113c:	12e86463          	bltu	a6,a4,1264 <printint.constprop.0+0x1c4>
        buf[i--] = digits[x % base];
    1140:	02e5f6bb          	remuw	a3,a1,a4
    1144:	1682                	slli	a3,a3,0x20
    1146:	9281                	srli	a3,a3,0x20
    1148:	96b2                	add	a3,a3,a2
    114a:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    114e:	02e5d83b          	divuw	a6,a1,a4
        buf[i--] = digits[x % base];
    1152:	00d10923          	sb	a3,18(sp)
    } while ((x /= base) != 0);
    1156:	0ce5ec63          	bltu	a1,a4,122e <printint.constprop.0+0x18e>
        buf[i--] = digits[x % base];
    115a:	02e876bb          	remuw	a3,a6,a4
    115e:	1682                	slli	a3,a3,0x20
    1160:	9281                	srli	a3,a3,0x20
    1162:	96b2                	add	a3,a3,a2
    1164:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    1168:	02e855bb          	divuw	a1,a6,a4
        buf[i--] = digits[x % base];
    116c:	00d108a3          	sb	a3,17(sp)
    } while ((x /= base) != 0);
    1170:	10e86963          	bltu	a6,a4,1282 <printint.constprop.0+0x1e2>
        buf[i--] = digits[x % base];
    1174:	02e5f6bb          	remuw	a3,a1,a4
    1178:	1682                	slli	a3,a3,0x20
    117a:	9281                	srli	a3,a3,0x20
    117c:	96b2                	add	a3,a3,a2
    117e:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    1182:	02e5d83b          	divuw	a6,a1,a4
        buf[i--] = digits[x % base];
    1186:	00d10823          	sb	a3,16(sp)
    } while ((x /= base) != 0);
    118a:	10e5e763          	bltu	a1,a4,1298 <printint.constprop.0+0x1f8>
        buf[i--] = digits[x % base];
    118e:	02e876bb          	remuw	a3,a6,a4
    1192:	1682                	slli	a3,a3,0x20
    1194:	9281                	srli	a3,a3,0x20
    1196:	96b2                	add	a3,a3,a2
    1198:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    119c:	02e857bb          	divuw	a5,a6,a4
        buf[i--] = digits[x % base];
    11a0:	00d107a3          	sb	a3,15(sp)
    } while ((x /= base) != 0);
    11a4:	10e86363          	bltu	a6,a4,12aa <printint.constprop.0+0x20a>
        buf[i--] = digits[x % base];
    11a8:	1782                	slli	a5,a5,0x20
    11aa:	9381                	srli	a5,a5,0x20
    11ac:	97b2                	add	a5,a5,a2
    11ae:	0007c783          	lbu	a5,0(a5)
    11b2:	4599                	li	a1,6
    11b4:	00f10723          	sb	a5,14(sp)

    if (sign)
    11b8:	00055763          	bgez	a0,11c6 <printint.constprop.0+0x126>
        buf[i--] = '-';
    11bc:	02d00793          	li	a5,45
    11c0:	00f106a3          	sb	a5,13(sp)
        buf[i--] = digits[x % base];
    11c4:	4595                	li	a1,5
    write(f, s, l);
    11c6:	003c                	addi	a5,sp,8
    11c8:	4641                	li	a2,16
    11ca:	9e0d                	subw	a2,a2,a1
    11cc:	4505                	li	a0,1
    11ce:	95be                	add	a1,a1,a5
    11d0:	27f000ef          	jal	ra,1c4e <write>
    i++;
    if (i < 0)
        puts("printint error");
    out(stdout, buf + i, 16 - i);
}
    11d4:	70a2                	ld	ra,40(sp)
    11d6:	6145                	addi	sp,sp,48
    11d8:	8082                	ret
        x = -xx;
    11da:	40a0083b          	negw	a6,a0
        buf[i--] = digits[x % base];
    11de:	02b877bb          	remuw	a5,a6,a1
    11e2:	00001617          	auipc	a2,0x1
    11e6:	d6e60613          	addi	a2,a2,-658 # 1f50 <digits>
    buf[16] = 0;
    11ea:	00010c23          	sb	zero,24(sp)
        buf[i--] = digits[x % base];
    11ee:	0005871b          	sext.w	a4,a1
    11f2:	1782                	slli	a5,a5,0x20
    11f4:	9381                	srli	a5,a5,0x20
    11f6:	97b2                	add	a5,a5,a2
    11f8:	0007c783          	lbu	a5,0(a5)
    } while ((x /= base) != 0);
    11fc:	02b858bb          	divuw	a7,a6,a1
        buf[i--] = digits[x % base];
    1200:	00f10ba3          	sb	a5,23(sp)
    } while ((x /= base) != 0);
    1204:	06b86963          	bltu	a6,a1,1276 <printint.constprop.0+0x1d6>
        buf[i--] = digits[x % base];
    1208:	02e8f7bb          	remuw	a5,a7,a4
    120c:	1782                	slli	a5,a5,0x20
    120e:	9381                	srli	a5,a5,0x20
    1210:	97b2                	add	a5,a5,a2
    1212:	0007c783          	lbu	a5,0(a5)
    } while ((x /= base) != 0);
    1216:	02e8d6bb          	divuw	a3,a7,a4
        buf[i--] = digits[x % base];
    121a:	00f10b23          	sb	a5,22(sp)
    } while ((x /= base) != 0);
    121e:	ece8f8e3          	bgeu	a7,a4,10ee <printint.constprop.0+0x4e>
        buf[i--] = '-';
    1222:	02d00793          	li	a5,45
    1226:	00f10aa3          	sb	a5,21(sp)
        buf[i--] = digits[x % base];
    122a:	45b5                	li	a1,13
    122c:	bf69                	j	11c6 <printint.constprop.0+0x126>
    122e:	45a9                	li	a1,10
    if (sign)
    1230:	f8055be3          	bgez	a0,11c6 <printint.constprop.0+0x126>
        buf[i--] = '-';
    1234:	02d00793          	li	a5,45
    1238:	00f108a3          	sb	a5,17(sp)
        buf[i--] = digits[x % base];
    123c:	45a5                	li	a1,9
    123e:	b761                	j	11c6 <printint.constprop.0+0x126>
    1240:	45b5                	li	a1,13
    if (sign)
    1242:	f80552e3          	bgez	a0,11c6 <printint.constprop.0+0x126>
        buf[i--] = '-';
    1246:	02d00793          	li	a5,45
    124a:	00f10a23          	sb	a5,20(sp)
        buf[i--] = digits[x % base];
    124e:	45b1                	li	a1,12
    1250:	bf9d                	j	11c6 <printint.constprop.0+0x126>
    1252:	45b1                	li	a1,12
    if (sign)
    1254:	f60559e3          	bgez	a0,11c6 <printint.constprop.0+0x126>
        buf[i--] = '-';
    1258:	02d00793          	li	a5,45
    125c:	00f109a3          	sb	a5,19(sp)
        buf[i--] = digits[x % base];
    1260:	45ad                	li	a1,11
    1262:	b795                	j	11c6 <printint.constprop.0+0x126>
    1264:	45ad                	li	a1,11
    if (sign)
    1266:	f60550e3          	bgez	a0,11c6 <printint.constprop.0+0x126>
        buf[i--] = '-';
    126a:	02d00793          	li	a5,45
    126e:	00f10923          	sb	a5,18(sp)
        buf[i--] = digits[x % base];
    1272:	45a9                	li	a1,10
    1274:	bf89                	j	11c6 <printint.constprop.0+0x126>
        buf[i--] = '-';
    1276:	02d00793          	li	a5,45
    127a:	00f10b23          	sb	a5,22(sp)
        buf[i--] = digits[x % base];
    127e:	45b9                	li	a1,14
    1280:	b799                	j	11c6 <printint.constprop.0+0x126>
    1282:	45a5                	li	a1,9
    if (sign)
    1284:	f40551e3          	bgez	a0,11c6 <printint.constprop.0+0x126>
        buf[i--] = '-';
    1288:	02d00793          	li	a5,45
    128c:	00f10823          	sb	a5,16(sp)
        buf[i--] = digits[x % base];
    1290:	45a1                	li	a1,8
    1292:	bf15                	j	11c6 <printint.constprop.0+0x126>
    i = 15;
    1294:	45bd                	li	a1,15
    1296:	bf05                	j	11c6 <printint.constprop.0+0x126>
        buf[i--] = digits[x % base];
    1298:	45a1                	li	a1,8
    if (sign)
    129a:	f20556e3          	bgez	a0,11c6 <printint.constprop.0+0x126>
        buf[i--] = '-';
    129e:	02d00793          	li	a5,45
    12a2:	00f107a3          	sb	a5,15(sp)
        buf[i--] = digits[x % base];
    12a6:	459d                	li	a1,7
    12a8:	bf39                	j	11c6 <printint.constprop.0+0x126>
    12aa:	459d                	li	a1,7
    if (sign)
    12ac:	f0055de3          	bgez	a0,11c6 <printint.constprop.0+0x126>
        buf[i--] = '-';
    12b0:	02d00793          	li	a5,45
    12b4:	00f10723          	sb	a5,14(sp)
        buf[i--] = digits[x % base];
    12b8:	4599                	li	a1,6
    12ba:	b731                	j	11c6 <printint.constprop.0+0x126>

00000000000012bc <getchar>:
{
    12bc:	1101                	addi	sp,sp,-32
    read(stdin, &byte, 1);
    12be:	00f10593          	addi	a1,sp,15
    12c2:	4605                	li	a2,1
    12c4:	4501                	li	a0,0
{
    12c6:	ec06                	sd	ra,24(sp)
    char byte = 0;
    12c8:	000107a3          	sb	zero,15(sp)
    read(stdin, &byte, 1);
    12cc:	179000ef          	jal	ra,1c44 <read>
}
    12d0:	60e2                	ld	ra,24(sp)
    12d2:	00f14503          	lbu	a0,15(sp)
    12d6:	6105                	addi	sp,sp,32
    12d8:	8082                	ret

00000000000012da <putchar>:
{
    12da:	1101                	addi	sp,sp,-32
    12dc:	87aa                	mv	a5,a0
    return write(stdout, &byte, 1);
    12de:	00f10593          	addi	a1,sp,15
    12e2:	4605                	li	a2,1
    12e4:	4505                	li	a0,1
{
    12e6:	ec06                	sd	ra,24(sp)
    char byte = c;
    12e8:	00f107a3          	sb	a5,15(sp)
    return write(stdout, &byte, 1);
    12ec:	163000ef          	jal	ra,1c4e <write>
}
    12f0:	60e2                	ld	ra,24(sp)
    12f2:	2501                	sext.w	a0,a0
    12f4:	6105                	addi	sp,sp,32
    12f6:	8082                	ret

00000000000012f8 <puts>:
{
    12f8:	1141                	addi	sp,sp,-16
    12fa:	e406                	sd	ra,8(sp)
    12fc:	e022                	sd	s0,0(sp)
    12fe:	842a                	mv	s0,a0
    r = -(write(stdout, s, strlen(s)) < 0);
    1300:	56a000ef          	jal	ra,186a <strlen>
    1304:	862a                	mv	a2,a0
    1306:	85a2                	mv	a1,s0
    1308:	4505                	li	a0,1
    130a:	145000ef          	jal	ra,1c4e <write>
}
    130e:	60a2                	ld	ra,8(sp)
    1310:	6402                	ld	s0,0(sp)
    r = -(write(stdout, s, strlen(s)) < 0);
    1312:	957d                	srai	a0,a0,0x3f
    return r;
    1314:	2501                	sext.w	a0,a0
}
    1316:	0141                	addi	sp,sp,16
    1318:	8082                	ret

000000000000131a <printf>:
    out(stdout, buf, i);
}

// Print to the console. only understands %d, %x, %p, %s.
void printf(const char *fmt, ...)
{
    131a:	7171                	addi	sp,sp,-176
    131c:	fc56                	sd	s5,56(sp)
    131e:	ed3e                	sd	a5,152(sp)
    buf[i++] = '0';
    1320:	7ae1                	lui	s5,0xffff8
    va_list ap;
    int cnt = 0, l = 0;
    char *a, *z, *s = (char *)fmt, str;
    int f = stdout;

    va_start(ap, fmt);
    1322:	18bc                	addi	a5,sp,120
{
    1324:	e8ca                	sd	s2,80(sp)
    1326:	e4ce                	sd	s3,72(sp)
    1328:	e0d2                	sd	s4,64(sp)
    132a:	f85a                	sd	s6,48(sp)
    132c:	f486                	sd	ra,104(sp)
    132e:	f0a2                	sd	s0,96(sp)
    1330:	eca6                	sd	s1,88(sp)
    1332:	fcae                	sd	a1,120(sp)
    1334:	e132                	sd	a2,128(sp)
    1336:	e536                	sd	a3,136(sp)
    1338:	e93a                	sd	a4,144(sp)
    133a:	f142                	sd	a6,160(sp)
    133c:	f546                	sd	a7,168(sp)
    va_start(ap, fmt);
    133e:	e03e                	sd	a5,0(sp)
    for (;;)
    {
        if (!*s)
            break;
        for (a = s; *s && *s != '%'; s++)
    1340:	02500913          	li	s2,37
        out(f, a, l);
        if (l)
            continue;
        if (s[1] == 0)
            break;
        switch (s[1])
    1344:	07300a13          	li	s4,115
        case 'p':
            printptr(va_arg(ap, uint64));
            break;
        case 's':
            if ((a = va_arg(ap, char *)) == 0)
                a = "(null)";
    1348:	00001b17          	auipc	s6,0x1
    134c:	bd8b0b13          	addi	s6,s6,-1064 # 1f20 <__clone+0x9e>
    buf[i++] = '0';
    1350:	830aca93          	xori	s5,s5,-2000
        buf[i++] = digits[x >> (sizeof(uint64) * 8 - 4)];
    1354:	00001997          	auipc	s3,0x1
    1358:	bfc98993          	addi	s3,s3,-1028 # 1f50 <digits>
        if (!*s)
    135c:	00054783          	lbu	a5,0(a0)
    1360:	16078a63          	beqz	a5,14d4 <printf+0x1ba>
    1364:	862a                	mv	a2,a0
        for (a = s; *s && *s != '%'; s++)
    1366:	19278163          	beq	a5,s2,14e8 <printf+0x1ce>
    136a:	00164783          	lbu	a5,1(a2)
    136e:	0605                	addi	a2,a2,1
    1370:	fbfd                	bnez	a5,1366 <printf+0x4c>
    1372:	84b2                	mv	s1,a2
        l = z - a;
    1374:	40a6043b          	subw	s0,a2,a0
    write(f, s, l);
    1378:	85aa                	mv	a1,a0
    137a:	8622                	mv	a2,s0
    137c:	4505                	li	a0,1
    137e:	0d1000ef          	jal	ra,1c4e <write>
        if (l)
    1382:	18041c63          	bnez	s0,151a <printf+0x200>
        if (s[1] == 0)
    1386:	0014c783          	lbu	a5,1(s1)
    138a:	14078563          	beqz	a5,14d4 <printf+0x1ba>
        switch (s[1])
    138e:	1d478063          	beq	a5,s4,154e <printf+0x234>
    1392:	18fa6663          	bltu	s4,a5,151e <printf+0x204>
    1396:	06400713          	li	a4,100
    139a:	1ae78063          	beq	a5,a4,153a <printf+0x220>
    139e:	07000713          	li	a4,112
    13a2:	1ce79963          	bne	a5,a4,1574 <printf+0x25a>
            printptr(va_arg(ap, uint64));
    13a6:	6702                	ld	a4,0(sp)
    buf[i++] = '0';
    13a8:	01511423          	sh	s5,8(sp)
    write(f, s, l);
    13ac:	4649                	li	a2,18
            printptr(va_arg(ap, uint64));
    13ae:	631c                	ld	a5,0(a4)
    13b0:	0721                	addi	a4,a4,8
    13b2:	e03a                	sd	a4,0(sp)
    for (j = 0; j < (sizeof(uint64) * 2); j++, x <<= 4)
    13b4:	00479293          	slli	t0,a5,0x4
    13b8:	00879f93          	slli	t6,a5,0x8
    13bc:	00c79f13          	slli	t5,a5,0xc
    13c0:	01079e93          	slli	t4,a5,0x10
    13c4:	01479e13          	slli	t3,a5,0x14
    13c8:	01879313          	slli	t1,a5,0x18
    13cc:	01c79893          	slli	a7,a5,0x1c
    13d0:	02479813          	slli	a6,a5,0x24
    13d4:	02879513          	slli	a0,a5,0x28
    13d8:	02c79593          	slli	a1,a5,0x2c
    13dc:	03079693          	slli	a3,a5,0x30
    13e0:	03479713          	slli	a4,a5,0x34
        buf[i++] = digits[x >> (sizeof(uint64) * 8 - 4)];
    13e4:	03c7d413          	srli	s0,a5,0x3c
    13e8:	01c7d39b          	srliw	t2,a5,0x1c
    13ec:	03c2d293          	srli	t0,t0,0x3c
    13f0:	03cfdf93          	srli	t6,t6,0x3c
    13f4:	03cf5f13          	srli	t5,t5,0x3c
    13f8:	03cede93          	srli	t4,t4,0x3c
    13fc:	03ce5e13          	srli	t3,t3,0x3c
    1400:	03c35313          	srli	t1,t1,0x3c
    1404:	03c8d893          	srli	a7,a7,0x3c
    1408:	03c85813          	srli	a6,a6,0x3c
    140c:	9171                	srli	a0,a0,0x3c
    140e:	91f1                	srli	a1,a1,0x3c
    1410:	92f1                	srli	a3,a3,0x3c
    1412:	9371                	srli	a4,a4,0x3c
    1414:	96ce                	add	a3,a3,s3
    1416:	974e                	add	a4,a4,s3
    1418:	944e                	add	s0,s0,s3
    141a:	92ce                	add	t0,t0,s3
    141c:	9fce                	add	t6,t6,s3
    141e:	9f4e                	add	t5,t5,s3
    1420:	9ece                	add	t4,t4,s3
    1422:	9e4e                	add	t3,t3,s3
    1424:	934e                	add	t1,t1,s3
    1426:	98ce                	add	a7,a7,s3
    1428:	93ce                	add	t2,t2,s3
    142a:	984e                	add	a6,a6,s3
    142c:	954e                	add	a0,a0,s3
    142e:	95ce                	add	a1,a1,s3
    1430:	0006c083          	lbu	ra,0(a3)
    1434:	0002c283          	lbu	t0,0(t0)
    1438:	00074683          	lbu	a3,0(a4)
    143c:	000fcf83          	lbu	t6,0(t6)
    1440:	000f4f03          	lbu	t5,0(t5)
    1444:	000ece83          	lbu	t4,0(t4)
    1448:	000e4e03          	lbu	t3,0(t3)
    144c:	00034303          	lbu	t1,0(t1)
    1450:	0008c883          	lbu	a7,0(a7)
    1454:	0003c383          	lbu	t2,0(t2)
    1458:	00084803          	lbu	a6,0(a6)
    145c:	00054503          	lbu	a0,0(a0)
    1460:	0005c583          	lbu	a1,0(a1)
    1464:	00044403          	lbu	s0,0(s0)
    for (j = 0; j < (sizeof(uint64) * 2); j++, x <<= 4)
    1468:	03879713          	slli	a4,a5,0x38
        buf[i++] = digits[x >> (sizeof(uint64) * 8 - 4)];
    146c:	9371                	srli	a4,a4,0x3c
    146e:	8bbd                	andi	a5,a5,15
    1470:	974e                	add	a4,a4,s3
    1472:	97ce                	add	a5,a5,s3
    1474:	005105a3          	sb	t0,11(sp)
    1478:	01f10623          	sb	t6,12(sp)
    147c:	01e106a3          	sb	t5,13(sp)
    1480:	01d10723          	sb	t4,14(sp)
    1484:	01c107a3          	sb	t3,15(sp)
    1488:	00610823          	sb	t1,16(sp)
    148c:	011108a3          	sb	a7,17(sp)
    1490:	00710923          	sb	t2,18(sp)
    1494:	010109a3          	sb	a6,19(sp)
    1498:	00a10a23          	sb	a0,20(sp)
    149c:	00b10aa3          	sb	a1,21(sp)
    14a0:	00110b23          	sb	ra,22(sp)
    14a4:	00d10ba3          	sb	a3,23(sp)
    14a8:	00810523          	sb	s0,10(sp)
    14ac:	00074703          	lbu	a4,0(a4)
    14b0:	0007c783          	lbu	a5,0(a5)
    write(f, s, l);
    14b4:	002c                	addi	a1,sp,8
    14b6:	4505                	li	a0,1
        buf[i++] = digits[x >> (sizeof(uint64) * 8 - 4)];
    14b8:	00e10c23          	sb	a4,24(sp)
    14bc:	00f10ca3          	sb	a5,25(sp)
    buf[i] = 0;
    14c0:	00010d23          	sb	zero,26(sp)
    write(f, s, l);
    14c4:	78a000ef          	jal	ra,1c4e <write>
            // Print unknown % sequence to draw attention.
            putchar('%');
            putchar(s[1]);
            break;
        }
        s += 2;
    14c8:	00248513          	addi	a0,s1,2
        if (!*s)
    14cc:	00054783          	lbu	a5,0(a0)
    14d0:	e8079ae3          	bnez	a5,1364 <printf+0x4a>
    }
    va_end(ap);
}
    14d4:	70a6                	ld	ra,104(sp)
    14d6:	7406                	ld	s0,96(sp)
    14d8:	64e6                	ld	s1,88(sp)
    14da:	6946                	ld	s2,80(sp)
    14dc:	69a6                	ld	s3,72(sp)
    14de:	6a06                	ld	s4,64(sp)
    14e0:	7ae2                	ld	s5,56(sp)
    14e2:	7b42                	ld	s6,48(sp)
    14e4:	614d                	addi	sp,sp,176
    14e6:	8082                	ret
        for (z = s; s[0] == '%' && s[1] == '%'; z++, s += 2)
    14e8:	00064783          	lbu	a5,0(a2)
    14ec:	84b2                	mv	s1,a2
    14ee:	01278963          	beq	a5,s2,1500 <printf+0x1e6>
    14f2:	b549                	j	1374 <printf+0x5a>
    14f4:	0024c783          	lbu	a5,2(s1)
    14f8:	0605                	addi	a2,a2,1
    14fa:	0489                	addi	s1,s1,2
    14fc:	e7279ce3          	bne	a5,s2,1374 <printf+0x5a>
    1500:	0014c783          	lbu	a5,1(s1)
    1504:	ff2788e3          	beq	a5,s2,14f4 <printf+0x1da>
        l = z - a;
    1508:	40a6043b          	subw	s0,a2,a0
    write(f, s, l);
    150c:	85aa                	mv	a1,a0
    150e:	8622                	mv	a2,s0
    1510:	4505                	li	a0,1
    1512:	73c000ef          	jal	ra,1c4e <write>
        if (l)
    1516:	e60408e3          	beqz	s0,1386 <printf+0x6c>
    151a:	8526                	mv	a0,s1
    151c:	b581                	j	135c <printf+0x42>
        switch (s[1])
    151e:	07800713          	li	a4,120
    1522:	04e79963          	bne	a5,a4,1574 <printf+0x25a>
            printint(va_arg(ap, int), 16, 1);
    1526:	6782                	ld	a5,0(sp)
    1528:	45c1                	li	a1,16
    152a:	4388                	lw	a0,0(a5)
    152c:	07a1                	addi	a5,a5,8
    152e:	e03e                	sd	a5,0(sp)
    1530:	b71ff0ef          	jal	ra,10a0 <printint.constprop.0>
        s += 2;
    1534:	00248513          	addi	a0,s1,2
    1538:	bf51                	j	14cc <printf+0x1b2>
            printint(va_arg(ap, int), 10, 1);
    153a:	6782                	ld	a5,0(sp)
    153c:	45a9                	li	a1,10
    153e:	4388                	lw	a0,0(a5)
    1540:	07a1                	addi	a5,a5,8
    1542:	e03e                	sd	a5,0(sp)
    1544:	b5dff0ef          	jal	ra,10a0 <printint.constprop.0>
        s += 2;
    1548:	00248513          	addi	a0,s1,2
    154c:	b741                	j	14cc <printf+0x1b2>
            if ((a = va_arg(ap, char *)) == 0)
    154e:	6782                	ld	a5,0(sp)
    1550:	6380                	ld	s0,0(a5)
    1552:	07a1                	addi	a5,a5,8
    1554:	e03e                	sd	a5,0(sp)
    1556:	c031                	beqz	s0,159a <printf+0x280>
            l = strnlen(a, 200);
    1558:	0c800593          	li	a1,200
    155c:	8522                	mv	a0,s0
    155e:	3f8000ef          	jal	ra,1956 <strnlen>
    write(f, s, l);
    1562:	0005061b          	sext.w	a2,a0
    1566:	85a2                	mv	a1,s0
    1568:	4505                	li	a0,1
    156a:	6e4000ef          	jal	ra,1c4e <write>
        s += 2;
    156e:	00248513          	addi	a0,s1,2
    1572:	bfa9                	j	14cc <printf+0x1b2>
    return write(stdout, &byte, 1);
    1574:	4605                	li	a2,1
    1576:	002c                	addi	a1,sp,8
    1578:	4505                	li	a0,1
    char byte = c;
    157a:	01210423          	sb	s2,8(sp)
    return write(stdout, &byte, 1);
    157e:	6d0000ef          	jal	ra,1c4e <write>
    char byte = c;
    1582:	0014c783          	lbu	a5,1(s1)
    return write(stdout, &byte, 1);
    1586:	4605                	li	a2,1
    1588:	002c                	addi	a1,sp,8
    158a:	4505                	li	a0,1
    char byte = c;
    158c:	00f10423          	sb	a5,8(sp)
    return write(stdout, &byte, 1);
    1590:	6be000ef          	jal	ra,1c4e <write>
        s += 2;
    1594:	00248513          	addi	a0,s1,2
    1598:	bf15                	j	14cc <printf+0x1b2>
                a = "(null)";
    159a:	845a                	mv	s0,s6
    159c:	bf75                	j	1558 <printf+0x23e>

000000000000159e <isspace>:
#define HIGHS (ONES * (UCHAR_MAX / 2 + 1))
#define HASZERO(x) (((x)-ONES) & ~(x)&HIGHS)

int isspace(int c)
{
    return c == ' ' || (unsigned)c - '\t' < 5;
    159e:	02000793          	li	a5,32
    15a2:	00f50663          	beq	a0,a5,15ae <isspace+0x10>
    15a6:	355d                	addiw	a0,a0,-9
    15a8:	00553513          	sltiu	a0,a0,5
    15ac:	8082                	ret
    15ae:	4505                	li	a0,1
}
    15b0:	8082                	ret

00000000000015b2 <isdigit>:

int isdigit(int c)
{
    return (unsigned)c - '0' < 10;
    15b2:	fd05051b          	addiw	a0,a0,-48
}
    15b6:	00a53513          	sltiu	a0,a0,10
    15ba:	8082                	ret

00000000000015bc <atoi>:
    return c == ' ' || (unsigned)c - '\t' < 5;
    15bc:	02000613          	li	a2,32
    15c0:	4591                	li	a1,4

int atoi(const char *s)
{
    int n = 0, neg = 0;
    while (isspace(*s))
    15c2:	00054703          	lbu	a4,0(a0)
    return c == ' ' || (unsigned)c - '\t' < 5;
    15c6:	ff77069b          	addiw	a3,a4,-9
    15ca:	04c70d63          	beq	a4,a2,1624 <atoi+0x68>
    15ce:	0007079b          	sext.w	a5,a4
    15d2:	04d5f963          	bgeu	a1,a3,1624 <atoi+0x68>
        s++;
    switch (*s)
    15d6:	02b00693          	li	a3,43
    15da:	04d70a63          	beq	a4,a3,162e <atoi+0x72>
    15de:	02d00693          	li	a3,45
    15e2:	06d70463          	beq	a4,a3,164a <atoi+0x8e>
        neg = 1;
    case '+':
        s++;
    }
    /* Compute n as a negative number to avoid overflow on INT_MIN */
    while (isdigit(*s))
    15e6:	fd07859b          	addiw	a1,a5,-48
    15ea:	4625                	li	a2,9
    15ec:	873e                	mv	a4,a5
    15ee:	86aa                	mv	a3,a0
    int n = 0, neg = 0;
    15f0:	4e01                	li	t3,0
    while (isdigit(*s))
    15f2:	04b66a63          	bltu	a2,a1,1646 <atoi+0x8a>
    int n = 0, neg = 0;
    15f6:	4501                	li	a0,0
    while (isdigit(*s))
    15f8:	4825                	li	a6,9
    15fa:	0016c603          	lbu	a2,1(a3)
        n = 10 * n - (*s++ - '0');
    15fe:	0025179b          	slliw	a5,a0,0x2
    1602:	9d3d                	addw	a0,a0,a5
    1604:	fd07031b          	addiw	t1,a4,-48
    1608:	0015189b          	slliw	a7,a0,0x1
    while (isdigit(*s))
    160c:	fd06059b          	addiw	a1,a2,-48
        n = 10 * n - (*s++ - '0');
    1610:	0685                	addi	a3,a3,1
    1612:	4068853b          	subw	a0,a7,t1
    while (isdigit(*s))
    1616:	0006071b          	sext.w	a4,a2
    161a:	feb870e3          	bgeu	a6,a1,15fa <atoi+0x3e>
    return neg ? n : -n;
    161e:	000e0563          	beqz	t3,1628 <atoi+0x6c>
}
    1622:	8082                	ret
        s++;
    1624:	0505                	addi	a0,a0,1
    1626:	bf71                	j	15c2 <atoi+0x6>
    return neg ? n : -n;
    1628:	4113053b          	subw	a0,t1,a7
    162c:	8082                	ret
    while (isdigit(*s))
    162e:	00154783          	lbu	a5,1(a0)
    1632:	4625                	li	a2,9
        s++;
    1634:	00150693          	addi	a3,a0,1
    while (isdigit(*s))
    1638:	fd07859b          	addiw	a1,a5,-48
    163c:	0007871b          	sext.w	a4,a5
    int n = 0, neg = 0;
    1640:	4e01                	li	t3,0
    while (isdigit(*s))
    1642:	fab67ae3          	bgeu	a2,a1,15f6 <atoi+0x3a>
    1646:	4501                	li	a0,0
}
    1648:	8082                	ret
    while (isdigit(*s))
    164a:	00154783          	lbu	a5,1(a0)
    164e:	4625                	li	a2,9
        s++;
    1650:	00150693          	addi	a3,a0,1
    while (isdigit(*s))
    1654:	fd07859b          	addiw	a1,a5,-48
    1658:	0007871b          	sext.w	a4,a5
    165c:	feb665e3          	bltu	a2,a1,1646 <atoi+0x8a>
        neg = 1;
    1660:	4e05                	li	t3,1
    1662:	bf51                	j	15f6 <atoi+0x3a>

0000000000001664 <memset>:

void *memset(void *dest, int c, size_t n)
{
    char *p = dest;
    for (int i = 0; i < n; ++i, *(p++) = c)
    1664:	16060d63          	beqz	a2,17de <memset+0x17a>
    1668:	40a007b3          	neg	a5,a0
    166c:	8b9d                	andi	a5,a5,7
    166e:	00778713          	addi	a4,a5,7
    1672:	482d                	li	a6,11
    1674:	0ff5f593          	andi	a1,a1,255
    1678:	fff60693          	addi	a3,a2,-1
    167c:	17076263          	bltu	a4,a6,17e0 <memset+0x17c>
    1680:	16e6ea63          	bltu	a3,a4,17f4 <memset+0x190>
    1684:	16078563          	beqz	a5,17ee <memset+0x18a>
    1688:	00b50023          	sb	a1,0(a0)
    168c:	4705                	li	a4,1
    168e:	00150e93          	addi	t4,a0,1
    1692:	14e78c63          	beq	a5,a4,17ea <memset+0x186>
    1696:	00b500a3          	sb	a1,1(a0)
    169a:	4709                	li	a4,2
    169c:	00250e93          	addi	t4,a0,2
    16a0:	14e78d63          	beq	a5,a4,17fa <memset+0x196>
    16a4:	00b50123          	sb	a1,2(a0)
    16a8:	470d                	li	a4,3
    16aa:	00350e93          	addi	t4,a0,3
    16ae:	12e78b63          	beq	a5,a4,17e4 <memset+0x180>
    16b2:	00b501a3          	sb	a1,3(a0)
    16b6:	4711                	li	a4,4
    16b8:	00450e93          	addi	t4,a0,4
    16bc:	14e78163          	beq	a5,a4,17fe <memset+0x19a>
    16c0:	00b50223          	sb	a1,4(a0)
    16c4:	4715                	li	a4,5
    16c6:	00550e93          	addi	t4,a0,5
    16ca:	12e78c63          	beq	a5,a4,1802 <memset+0x19e>
    16ce:	00b502a3          	sb	a1,5(a0)
    16d2:	471d                	li	a4,7
    16d4:	00650e93          	addi	t4,a0,6
    16d8:	12e79763          	bne	a5,a4,1806 <memset+0x1a2>
    16dc:	00750e93          	addi	t4,a0,7
    16e0:	00b50323          	sb	a1,6(a0)
    16e4:	4f1d                	li	t5,7
    16e6:	00859713          	slli	a4,a1,0x8
    16ea:	8f4d                	or	a4,a4,a1
    16ec:	01059e13          	slli	t3,a1,0x10
    16f0:	01c76e33          	or	t3,a4,t3
    16f4:	01859313          	slli	t1,a1,0x18
    16f8:	006e6333          	or	t1,t3,t1
    16fc:	02059893          	slli	a7,a1,0x20
    1700:	011368b3          	or	a7,t1,a7
    1704:	02859813          	slli	a6,a1,0x28
    1708:	40f60333          	sub	t1,a2,a5
    170c:	0108e833          	or	a6,a7,a6
    1710:	03059693          	slli	a3,a1,0x30
    1714:	00d866b3          	or	a3,a6,a3
    1718:	03859713          	slli	a4,a1,0x38
    171c:	97aa                	add	a5,a5,a0
    171e:	ff837813          	andi	a6,t1,-8
    1722:	8f55                	or	a4,a4,a3
    1724:	00f806b3          	add	a3,a6,a5
    1728:	e398                	sd	a4,0(a5)
    172a:	07a1                	addi	a5,a5,8
    172c:	fed79ee3          	bne	a5,a3,1728 <memset+0xc4>
    1730:	ff837693          	andi	a3,t1,-8
    1734:	00de87b3          	add	a5,t4,a3
    1738:	01e6873b          	addw	a4,a3,t5
    173c:	0ad30663          	beq	t1,a3,17e8 <memset+0x184>
    1740:	00b78023          	sb	a1,0(a5)
    1744:	0017069b          	addiw	a3,a4,1
    1748:	08c6fb63          	bgeu	a3,a2,17de <memset+0x17a>
    174c:	00b780a3          	sb	a1,1(a5)
    1750:	0027069b          	addiw	a3,a4,2
    1754:	08c6f563          	bgeu	a3,a2,17de <memset+0x17a>
    1758:	00b78123          	sb	a1,2(a5)
    175c:	0037069b          	addiw	a3,a4,3
    1760:	06c6ff63          	bgeu	a3,a2,17de <memset+0x17a>
    1764:	00b781a3          	sb	a1,3(a5)
    1768:	0047069b          	addiw	a3,a4,4
    176c:	06c6f963          	bgeu	a3,a2,17de <memset+0x17a>
    1770:	00b78223          	sb	a1,4(a5)
    1774:	0057069b          	addiw	a3,a4,5
    1778:	06c6f363          	bgeu	a3,a2,17de <memset+0x17a>
    177c:	00b782a3          	sb	a1,5(a5)
    1780:	0067069b          	addiw	a3,a4,6
    1784:	04c6fd63          	bgeu	a3,a2,17de <memset+0x17a>
    1788:	00b78323          	sb	a1,6(a5)
    178c:	0077069b          	addiw	a3,a4,7
    1790:	04c6f763          	bgeu	a3,a2,17de <memset+0x17a>
    1794:	00b783a3          	sb	a1,7(a5)
    1798:	0087069b          	addiw	a3,a4,8
    179c:	04c6f163          	bgeu	a3,a2,17de <memset+0x17a>
    17a0:	00b78423          	sb	a1,8(a5)
    17a4:	0097069b          	addiw	a3,a4,9
    17a8:	02c6fb63          	bgeu	a3,a2,17de <memset+0x17a>
    17ac:	00b784a3          	sb	a1,9(a5)
    17b0:	00a7069b          	addiw	a3,a4,10
    17b4:	02c6f563          	bgeu	a3,a2,17de <memset+0x17a>
    17b8:	00b78523          	sb	a1,10(a5)
    17bc:	00b7069b          	addiw	a3,a4,11
    17c0:	00c6ff63          	bgeu	a3,a2,17de <memset+0x17a>
    17c4:	00b785a3          	sb	a1,11(a5)
    17c8:	00c7069b          	addiw	a3,a4,12
    17cc:	00c6f963          	bgeu	a3,a2,17de <memset+0x17a>
    17d0:	00b78623          	sb	a1,12(a5)
    17d4:	2735                	addiw	a4,a4,13
    17d6:	00c77463          	bgeu	a4,a2,17de <memset+0x17a>
    17da:	00b786a3          	sb	a1,13(a5)
        ;
    return dest;
}
    17de:	8082                	ret
    17e0:	472d                	li	a4,11
    17e2:	bd79                	j	1680 <memset+0x1c>
    for (int i = 0; i < n; ++i, *(p++) = c)
    17e4:	4f0d                	li	t5,3
    17e6:	b701                	j	16e6 <memset+0x82>
    17e8:	8082                	ret
    17ea:	4f05                	li	t5,1
    17ec:	bded                	j	16e6 <memset+0x82>
    17ee:	8eaa                	mv	t4,a0
    17f0:	4f01                	li	t5,0
    17f2:	bdd5                	j	16e6 <memset+0x82>
    17f4:	87aa                	mv	a5,a0
    17f6:	4701                	li	a4,0
    17f8:	b7a1                	j	1740 <memset+0xdc>
    17fa:	4f09                	li	t5,2
    17fc:	b5ed                	j	16e6 <memset+0x82>
    17fe:	4f11                	li	t5,4
    1800:	b5dd                	j	16e6 <memset+0x82>
    1802:	4f15                	li	t5,5
    1804:	b5cd                	j	16e6 <memset+0x82>
    1806:	4f19                	li	t5,6
    1808:	bdf9                	j	16e6 <memset+0x82>

000000000000180a <strcmp>:

int strcmp(const char *l, const char *r)
{
    for (; *l == *r && *l; l++, r++)
    180a:	00054783          	lbu	a5,0(a0)
    180e:	0005c703          	lbu	a4,0(a1)
    1812:	00e79863          	bne	a5,a4,1822 <strcmp+0x18>
    1816:	0505                	addi	a0,a0,1
    1818:	0585                	addi	a1,a1,1
    181a:	fbe5                	bnez	a5,180a <strcmp>
    181c:	4501                	li	a0,0
        ;
    return *(unsigned char *)l - *(unsigned char *)r;
}
    181e:	9d19                	subw	a0,a0,a4
    1820:	8082                	ret
    return *(unsigned char *)l - *(unsigned char *)r;
    1822:	0007851b          	sext.w	a0,a5
    1826:	bfe5                	j	181e <strcmp+0x14>

0000000000001828 <strncmp>:

int strncmp(const char *_l, const char *_r, size_t n)
{
    const unsigned char *l = (void *)_l, *r = (void *)_r;
    if (!n--)
    1828:	ce05                	beqz	a2,1860 <strncmp+0x38>
        return 0;
    for (; *l && *r && n && *l == *r; l++, r++, n--)
    182a:	00054703          	lbu	a4,0(a0)
    182e:	0005c783          	lbu	a5,0(a1)
    1832:	cb0d                	beqz	a4,1864 <strncmp+0x3c>
    if (!n--)
    1834:	167d                	addi	a2,a2,-1
    1836:	00c506b3          	add	a3,a0,a2
    183a:	a819                	j	1850 <strncmp+0x28>
    for (; *l && *r && n && *l == *r; l++, r++, n--)
    183c:	00a68e63          	beq	a3,a0,1858 <strncmp+0x30>
    1840:	0505                	addi	a0,a0,1
    1842:	00e79b63          	bne	a5,a4,1858 <strncmp+0x30>
    1846:	00054703          	lbu	a4,0(a0)
        ;
    return *l - *r;
    184a:	0005c783          	lbu	a5,0(a1)
    for (; *l && *r && n && *l == *r; l++, r++, n--)
    184e:	cb19                	beqz	a4,1864 <strncmp+0x3c>
    1850:	0005c783          	lbu	a5,0(a1)
    1854:	0585                	addi	a1,a1,1
    1856:	f3fd                	bnez	a5,183c <strncmp+0x14>
    return *l - *r;
    1858:	0007051b          	sext.w	a0,a4
    185c:	9d1d                	subw	a0,a0,a5
    185e:	8082                	ret
        return 0;
    1860:	4501                	li	a0,0
}
    1862:	8082                	ret
    1864:	4501                	li	a0,0
    return *l - *r;
    1866:	9d1d                	subw	a0,a0,a5
    1868:	8082                	ret

000000000000186a <strlen>:
size_t strlen(const char *s)
{
    const char *a = s;
    typedef size_t __attribute__((__may_alias__)) word;
    const word *w;
    for (; (uintptr_t)s % SS; s++)
    186a:	00757793          	andi	a5,a0,7
    186e:	cf89                	beqz	a5,1888 <strlen+0x1e>
    1870:	87aa                	mv	a5,a0
    1872:	a029                	j	187c <strlen+0x12>
    1874:	0785                	addi	a5,a5,1
    1876:	0077f713          	andi	a4,a5,7
    187a:	cb01                	beqz	a4,188a <strlen+0x20>
        if (!*s)
    187c:	0007c703          	lbu	a4,0(a5)
    1880:	fb75                	bnez	a4,1874 <strlen+0xa>
    for (w = (const void *)s; !HASZERO(*w); w++)
        ;
    s = (const void *)w;
    for (; *s; s++)
        ;
    return s - a;
    1882:	40a78533          	sub	a0,a5,a0
}
    1886:	8082                	ret
    for (; (uintptr_t)s % SS; s++)
    1888:	87aa                	mv	a5,a0
    for (w = (const void *)s; !HASZERO(*w); w++)
    188a:	6394                	ld	a3,0(a5)
    188c:	00000597          	auipc	a1,0x0
    1890:	69c5b583          	ld	a1,1692(a1) # 1f28 <__clone+0xa6>
    1894:	00000617          	auipc	a2,0x0
    1898:	69c63603          	ld	a2,1692(a2) # 1f30 <__clone+0xae>
    189c:	a019                	j	18a2 <strlen+0x38>
    189e:	6794                	ld	a3,8(a5)
    18a0:	07a1                	addi	a5,a5,8
    18a2:	00b68733          	add	a4,a3,a1
    18a6:	fff6c693          	not	a3,a3
    18aa:	8f75                	and	a4,a4,a3
    18ac:	8f71                	and	a4,a4,a2
    18ae:	db65                	beqz	a4,189e <strlen+0x34>
    for (; *s; s++)
    18b0:	0007c703          	lbu	a4,0(a5)
    18b4:	d779                	beqz	a4,1882 <strlen+0x18>
    18b6:	0017c703          	lbu	a4,1(a5)
    18ba:	0785                	addi	a5,a5,1
    18bc:	d379                	beqz	a4,1882 <strlen+0x18>
    18be:	0017c703          	lbu	a4,1(a5)
    18c2:	0785                	addi	a5,a5,1
    18c4:	fb6d                	bnez	a4,18b6 <strlen+0x4c>
    18c6:	bf75                	j	1882 <strlen+0x18>

00000000000018c8 <memchr>:

void *memchr(const void *src, int c, size_t n)
{
    const unsigned char *s = src;
    c = (unsigned char)c;
    for (; ((uintptr_t)s & ALIGN) && n && *s != c; s++, n--)
    18c8:	00757713          	andi	a4,a0,7
{
    18cc:	87aa                	mv	a5,a0
    c = (unsigned char)c;
    18ce:	0ff5f593          	andi	a1,a1,255
    for (; ((uintptr_t)s & ALIGN) && n && *s != c; s++, n--)
    18d2:	cb19                	beqz	a4,18e8 <memchr+0x20>
    18d4:	ce25                	beqz	a2,194c <memchr+0x84>
    18d6:	0007c703          	lbu	a4,0(a5)
    18da:	04b70e63          	beq	a4,a1,1936 <memchr+0x6e>
    18de:	0785                	addi	a5,a5,1
    18e0:	0077f713          	andi	a4,a5,7
    18e4:	167d                	addi	a2,a2,-1
    18e6:	f77d                	bnez	a4,18d4 <memchr+0xc>
            ;
        s = (const void *)w;
    }
    for (; n && *s != c; s++, n--)
        ;
    return n ? (void *)s : 0;
    18e8:	4501                	li	a0,0
    if (n && *s != c)
    18ea:	c235                	beqz	a2,194e <memchr+0x86>
    18ec:	0007c703          	lbu	a4,0(a5)
    18f0:	04b70363          	beq	a4,a1,1936 <memchr+0x6e>
        size_t k = ONES * c;
    18f4:	00000517          	auipc	a0,0x0
    18f8:	64453503          	ld	a0,1604(a0) # 1f38 <__clone+0xb6>
        for (w = (const void *)s; n >= SS && !HASZERO(*w ^ k); w++, n -= SS)
    18fc:	471d                	li	a4,7
        size_t k = ONES * c;
    18fe:	02a58533          	mul	a0,a1,a0
        for (w = (const void *)s; n >= SS && !HASZERO(*w ^ k); w++, n -= SS)
    1902:	02c77a63          	bgeu	a4,a2,1936 <memchr+0x6e>
    1906:	00000897          	auipc	a7,0x0
    190a:	6228b883          	ld	a7,1570(a7) # 1f28 <__clone+0xa6>
    190e:	00000817          	auipc	a6,0x0
    1912:	62283803          	ld	a6,1570(a6) # 1f30 <__clone+0xae>
    1916:	431d                	li	t1,7
    1918:	a029                	j	1922 <memchr+0x5a>
    191a:	1661                	addi	a2,a2,-8
    191c:	07a1                	addi	a5,a5,8
    191e:	02c37963          	bgeu	t1,a2,1950 <memchr+0x88>
    1922:	6398                	ld	a4,0(a5)
    1924:	8f29                	xor	a4,a4,a0
    1926:	011706b3          	add	a3,a4,a7
    192a:	fff74713          	not	a4,a4
    192e:	8f75                	and	a4,a4,a3
    1930:	01077733          	and	a4,a4,a6
    1934:	d37d                	beqz	a4,191a <memchr+0x52>
    1936:	853e                	mv	a0,a5
    1938:	97b2                	add	a5,a5,a2
    193a:	a021                	j	1942 <memchr+0x7a>
    for (; n && *s != c; s++, n--)
    193c:	0505                	addi	a0,a0,1
    193e:	00f50763          	beq	a0,a5,194c <memchr+0x84>
    1942:	00054703          	lbu	a4,0(a0)
    1946:	feb71be3          	bne	a4,a1,193c <memchr+0x74>
    194a:	8082                	ret
    return n ? (void *)s : 0;
    194c:	4501                	li	a0,0
}
    194e:	8082                	ret
    return n ? (void *)s : 0;
    1950:	4501                	li	a0,0
    for (; n && *s != c; s++, n--)
    1952:	f275                	bnez	a2,1936 <memchr+0x6e>
}
    1954:	8082                	ret

0000000000001956 <strnlen>:

size_t strnlen(const char *s, size_t n)
{
    1956:	1101                	addi	sp,sp,-32
    1958:	e822                	sd	s0,16(sp)
    const char *p = memchr(s, 0, n);
    195a:	862e                	mv	a2,a1
{
    195c:	842e                	mv	s0,a1
    const char *p = memchr(s, 0, n);
    195e:	4581                	li	a1,0
{
    1960:	e426                	sd	s1,8(sp)
    1962:	ec06                	sd	ra,24(sp)
    1964:	84aa                	mv	s1,a0
    const char *p = memchr(s, 0, n);
    1966:	f63ff0ef          	jal	ra,18c8 <memchr>
    return p ? p - s : n;
    196a:	c519                	beqz	a0,1978 <strnlen+0x22>
}
    196c:	60e2                	ld	ra,24(sp)
    196e:	6442                	ld	s0,16(sp)
    return p ? p - s : n;
    1970:	8d05                	sub	a0,a0,s1
}
    1972:	64a2                	ld	s1,8(sp)
    1974:	6105                	addi	sp,sp,32
    1976:	8082                	ret
    1978:	60e2                	ld	ra,24(sp)
    return p ? p - s : n;
    197a:	8522                	mv	a0,s0
}
    197c:	6442                	ld	s0,16(sp)
    197e:	64a2                	ld	s1,8(sp)
    1980:	6105                	addi	sp,sp,32
    1982:	8082                	ret

0000000000001984 <strcpy>:
char *strcpy(char *restrict d, const char *s)
{
    typedef size_t __attribute__((__may_alias__)) word;
    word *wd;
    const word *ws;
    if ((uintptr_t)s % SS == (uintptr_t)d % SS)
    1984:	00b547b3          	xor	a5,a0,a1
    1988:	8b9d                	andi	a5,a5,7
    198a:	eb95                	bnez	a5,19be <strcpy+0x3a>
    {
        for (; (uintptr_t)s % SS; s++, d++)
    198c:	0075f793          	andi	a5,a1,7
    1990:	e7b1                	bnez	a5,19dc <strcpy+0x58>
            if (!(*d = *s))
                return d;
        wd = (void *)d;
        ws = (const void *)s;
        for (; !HASZERO(*ws); *wd++ = *ws++)
    1992:	6198                	ld	a4,0(a1)
    1994:	00000617          	auipc	a2,0x0
    1998:	59463603          	ld	a2,1428(a2) # 1f28 <__clone+0xa6>
    199c:	00000817          	auipc	a6,0x0
    19a0:	59483803          	ld	a6,1428(a6) # 1f30 <__clone+0xae>
    19a4:	a029                	j	19ae <strcpy+0x2a>
    19a6:	e118                	sd	a4,0(a0)
    19a8:	6598                	ld	a4,8(a1)
    19aa:	05a1                	addi	a1,a1,8
    19ac:	0521                	addi	a0,a0,8
    19ae:	00c707b3          	add	a5,a4,a2
    19b2:	fff74693          	not	a3,a4
    19b6:	8ff5                	and	a5,a5,a3
    19b8:	0107f7b3          	and	a5,a5,a6
    19bc:	d7ed                	beqz	a5,19a6 <strcpy+0x22>
            ;
        d = (void *)wd;
        s = (const void *)ws;
    }
    for (; (*d = *s); s++, d++)
    19be:	0005c783          	lbu	a5,0(a1)
    19c2:	00f50023          	sb	a5,0(a0)
    19c6:	c785                	beqz	a5,19ee <strcpy+0x6a>
    19c8:	0015c783          	lbu	a5,1(a1)
    19cc:	0505                	addi	a0,a0,1
    19ce:	0585                	addi	a1,a1,1
    19d0:	00f50023          	sb	a5,0(a0)
    19d4:	fbf5                	bnez	a5,19c8 <strcpy+0x44>
        ;
    return d;
}
    19d6:	8082                	ret
        for (; (uintptr_t)s % SS; s++, d++)
    19d8:	0505                	addi	a0,a0,1
    19da:	df45                	beqz	a4,1992 <strcpy+0xe>
            if (!(*d = *s))
    19dc:	0005c783          	lbu	a5,0(a1)
        for (; (uintptr_t)s % SS; s++, d++)
    19e0:	0585                	addi	a1,a1,1
    19e2:	0075f713          	andi	a4,a1,7
            if (!(*d = *s))
    19e6:	00f50023          	sb	a5,0(a0)
    19ea:	f7fd                	bnez	a5,19d8 <strcpy+0x54>
}
    19ec:	8082                	ret
    19ee:	8082                	ret

00000000000019f0 <strncpy>:
char *strncpy(char *restrict d, const char *s, size_t n)
{
    typedef size_t __attribute__((__may_alias__)) word;
    word *wd;
    const word *ws;
    if (((uintptr_t)s & ALIGN) == ((uintptr_t)d & ALIGN))
    19f0:	00b547b3          	xor	a5,a0,a1
    19f4:	8b9d                	andi	a5,a5,7
    19f6:	1a079863          	bnez	a5,1ba6 <strncpy+0x1b6>
    {
        for (; ((uintptr_t)s & ALIGN) && n && (*d = *s); n--, s++, d++)
    19fa:	0075f793          	andi	a5,a1,7
    19fe:	16078463          	beqz	a5,1b66 <strncpy+0x176>
    1a02:	ea01                	bnez	a2,1a12 <strncpy+0x22>
    1a04:	a421                	j	1c0c <strncpy+0x21c>
    1a06:	167d                	addi	a2,a2,-1
    1a08:	0505                	addi	a0,a0,1
    1a0a:	14070e63          	beqz	a4,1b66 <strncpy+0x176>
    1a0e:	1a060863          	beqz	a2,1bbe <strncpy+0x1ce>
    1a12:	0005c783          	lbu	a5,0(a1)
    1a16:	0585                	addi	a1,a1,1
    1a18:	0075f713          	andi	a4,a1,7
    1a1c:	00f50023          	sb	a5,0(a0)
    1a20:	f3fd                	bnez	a5,1a06 <strncpy+0x16>
    1a22:	4805                	li	a6,1
    1a24:	1a061863          	bnez	a2,1bd4 <strncpy+0x1e4>
    1a28:	40a007b3          	neg	a5,a0
    1a2c:	8b9d                	andi	a5,a5,7
    1a2e:	4681                	li	a3,0
    1a30:	18061a63          	bnez	a2,1bc4 <strncpy+0x1d4>
    1a34:	00778713          	addi	a4,a5,7
    1a38:	45ad                	li	a1,11
    1a3a:	18b76363          	bltu	a4,a1,1bc0 <strncpy+0x1d0>
    1a3e:	1ae6eb63          	bltu	a3,a4,1bf4 <strncpy+0x204>
    1a42:	1a078363          	beqz	a5,1be8 <strncpy+0x1f8>
    for (int i = 0; i < n; ++i, *(p++) = c)
    1a46:	00050023          	sb	zero,0(a0)
    1a4a:	4685                	li	a3,1
    1a4c:	00150713          	addi	a4,a0,1
    1a50:	18d78f63          	beq	a5,a3,1bee <strncpy+0x1fe>
    1a54:	000500a3          	sb	zero,1(a0)
    1a58:	4689                	li	a3,2
    1a5a:	00250713          	addi	a4,a0,2
    1a5e:	18d78e63          	beq	a5,a3,1bfa <strncpy+0x20a>
    1a62:	00050123          	sb	zero,2(a0)
    1a66:	468d                	li	a3,3
    1a68:	00350713          	addi	a4,a0,3
    1a6c:	16d78c63          	beq	a5,a3,1be4 <strncpy+0x1f4>
    1a70:	000501a3          	sb	zero,3(a0)
    1a74:	4691                	li	a3,4
    1a76:	00450713          	addi	a4,a0,4
    1a7a:	18d78263          	beq	a5,a3,1bfe <strncpy+0x20e>
    1a7e:	00050223          	sb	zero,4(a0)
    1a82:	4695                	li	a3,5
    1a84:	00550713          	addi	a4,a0,5
    1a88:	16d78d63          	beq	a5,a3,1c02 <strncpy+0x212>
    1a8c:	000502a3          	sb	zero,5(a0)
    1a90:	469d                	li	a3,7
    1a92:	00650713          	addi	a4,a0,6
    1a96:	16d79863          	bne	a5,a3,1c06 <strncpy+0x216>
    1a9a:	00750713          	addi	a4,a0,7
    1a9e:	00050323          	sb	zero,6(a0)
    1aa2:	40f80833          	sub	a6,a6,a5
    1aa6:	ff887593          	andi	a1,a6,-8
    1aaa:	97aa                	add	a5,a5,a0
    1aac:	95be                	add	a1,a1,a5
    1aae:	0007b023          	sd	zero,0(a5)
    1ab2:	07a1                	addi	a5,a5,8
    1ab4:	feb79de3          	bne	a5,a1,1aae <strncpy+0xbe>
    1ab8:	ff887593          	andi	a1,a6,-8
    1abc:	9ead                	addw	a3,a3,a1
    1abe:	00b707b3          	add	a5,a4,a1
    1ac2:	12b80863          	beq	a6,a1,1bf2 <strncpy+0x202>
    1ac6:	00078023          	sb	zero,0(a5)
    1aca:	0016871b          	addiw	a4,a3,1
    1ace:	0ec77863          	bgeu	a4,a2,1bbe <strncpy+0x1ce>
    1ad2:	000780a3          	sb	zero,1(a5)
    1ad6:	0026871b          	addiw	a4,a3,2
    1ada:	0ec77263          	bgeu	a4,a2,1bbe <strncpy+0x1ce>
    1ade:	00078123          	sb	zero,2(a5)
    1ae2:	0036871b          	addiw	a4,a3,3
    1ae6:	0cc77c63          	bgeu	a4,a2,1bbe <strncpy+0x1ce>
    1aea:	000781a3          	sb	zero,3(a5)
    1aee:	0046871b          	addiw	a4,a3,4
    1af2:	0cc77663          	bgeu	a4,a2,1bbe <strncpy+0x1ce>
    1af6:	00078223          	sb	zero,4(a5)
    1afa:	0056871b          	addiw	a4,a3,5
    1afe:	0cc77063          	bgeu	a4,a2,1bbe <strncpy+0x1ce>
    1b02:	000782a3          	sb	zero,5(a5)
    1b06:	0066871b          	addiw	a4,a3,6
    1b0a:	0ac77a63          	bgeu	a4,a2,1bbe <strncpy+0x1ce>
    1b0e:	00078323          	sb	zero,6(a5)
    1b12:	0076871b          	addiw	a4,a3,7
    1b16:	0ac77463          	bgeu	a4,a2,1bbe <strncpy+0x1ce>
    1b1a:	000783a3          	sb	zero,7(a5)
    1b1e:	0086871b          	addiw	a4,a3,8
    1b22:	08c77e63          	bgeu	a4,a2,1bbe <strncpy+0x1ce>
    1b26:	00078423          	sb	zero,8(a5)
    1b2a:	0096871b          	addiw	a4,a3,9
    1b2e:	08c77863          	bgeu	a4,a2,1bbe <strncpy+0x1ce>
    1b32:	000784a3          	sb	zero,9(a5)
    1b36:	00a6871b          	addiw	a4,a3,10
    1b3a:	08c77263          	bgeu	a4,a2,1bbe <strncpy+0x1ce>
    1b3e:	00078523          	sb	zero,10(a5)
    1b42:	00b6871b          	addiw	a4,a3,11
    1b46:	06c77c63          	bgeu	a4,a2,1bbe <strncpy+0x1ce>
    1b4a:	000785a3          	sb	zero,11(a5)
    1b4e:	00c6871b          	addiw	a4,a3,12
    1b52:	06c77663          	bgeu	a4,a2,1bbe <strncpy+0x1ce>
    1b56:	00078623          	sb	zero,12(a5)
    1b5a:	26b5                	addiw	a3,a3,13
    1b5c:	06c6f163          	bgeu	a3,a2,1bbe <strncpy+0x1ce>
    1b60:	000786a3          	sb	zero,13(a5)
    1b64:	8082                	ret
            ;
        if (!n || !*s)
    1b66:	c645                	beqz	a2,1c0e <strncpy+0x21e>
    1b68:	0005c783          	lbu	a5,0(a1)
    1b6c:	ea078be3          	beqz	a5,1a22 <strncpy+0x32>
            goto tail;
        wd = (void *)d;
        ws = (const void *)s;
        for (; n >= sizeof(size_t) && !HASZERO(*ws); n -= sizeof(size_t), ws++, wd++)
    1b70:	479d                	li	a5,7
    1b72:	02c7ff63          	bgeu	a5,a2,1bb0 <strncpy+0x1c0>
    1b76:	00000897          	auipc	a7,0x0
    1b7a:	3b28b883          	ld	a7,946(a7) # 1f28 <__clone+0xa6>
    1b7e:	00000817          	auipc	a6,0x0
    1b82:	3b283803          	ld	a6,946(a6) # 1f30 <__clone+0xae>
    1b86:	431d                	li	t1,7
    1b88:	6198                	ld	a4,0(a1)
    1b8a:	011707b3          	add	a5,a4,a7
    1b8e:	fff74693          	not	a3,a4
    1b92:	8ff5                	and	a5,a5,a3
    1b94:	0107f7b3          	and	a5,a5,a6
    1b98:	ef81                	bnez	a5,1bb0 <strncpy+0x1c0>
            *wd = *ws;
    1b9a:	e118                	sd	a4,0(a0)
        for (; n >= sizeof(size_t) && !HASZERO(*ws); n -= sizeof(size_t), ws++, wd++)
    1b9c:	1661                	addi	a2,a2,-8
    1b9e:	05a1                	addi	a1,a1,8
    1ba0:	0521                	addi	a0,a0,8
    1ba2:	fec363e3          	bltu	t1,a2,1b88 <strncpy+0x198>
        d = (void *)wd;
        s = (const void *)ws;
    }
    for (; n && (*d = *s); n--, s++, d++)
    1ba6:	e609                	bnez	a2,1bb0 <strncpy+0x1c0>
    1ba8:	a08d                	j	1c0a <strncpy+0x21a>
    1baa:	167d                	addi	a2,a2,-1
    1bac:	0505                	addi	a0,a0,1
    1bae:	ca01                	beqz	a2,1bbe <strncpy+0x1ce>
    1bb0:	0005c783          	lbu	a5,0(a1)
    1bb4:	0585                	addi	a1,a1,1
    1bb6:	00f50023          	sb	a5,0(a0)
    1bba:	fbe5                	bnez	a5,1baa <strncpy+0x1ba>
        ;
tail:
    1bbc:	b59d                	j	1a22 <strncpy+0x32>
    memset(d, 0, n);
    return d;
}
    1bbe:	8082                	ret
    1bc0:	472d                	li	a4,11
    1bc2:	bdb5                	j	1a3e <strncpy+0x4e>
    1bc4:	00778713          	addi	a4,a5,7
    1bc8:	45ad                	li	a1,11
    1bca:	fff60693          	addi	a3,a2,-1
    1bce:	e6b778e3          	bgeu	a4,a1,1a3e <strncpy+0x4e>
    1bd2:	b7fd                	j	1bc0 <strncpy+0x1d0>
    1bd4:	40a007b3          	neg	a5,a0
    1bd8:	8832                	mv	a6,a2
    1bda:	8b9d                	andi	a5,a5,7
    1bdc:	4681                	li	a3,0
    1bde:	e4060be3          	beqz	a2,1a34 <strncpy+0x44>
    1be2:	b7cd                	j	1bc4 <strncpy+0x1d4>
    for (int i = 0; i < n; ++i, *(p++) = c)
    1be4:	468d                	li	a3,3
    1be6:	bd75                	j	1aa2 <strncpy+0xb2>
    1be8:	872a                	mv	a4,a0
    1bea:	4681                	li	a3,0
    1bec:	bd5d                	j	1aa2 <strncpy+0xb2>
    1bee:	4685                	li	a3,1
    1bf0:	bd4d                	j	1aa2 <strncpy+0xb2>
    1bf2:	8082                	ret
    1bf4:	87aa                	mv	a5,a0
    1bf6:	4681                	li	a3,0
    1bf8:	b5f9                	j	1ac6 <strncpy+0xd6>
    1bfa:	4689                	li	a3,2
    1bfc:	b55d                	j	1aa2 <strncpy+0xb2>
    1bfe:	4691                	li	a3,4
    1c00:	b54d                	j	1aa2 <strncpy+0xb2>
    1c02:	4695                	li	a3,5
    1c04:	bd79                	j	1aa2 <strncpy+0xb2>
    1c06:	4699                	li	a3,6
    1c08:	bd69                	j	1aa2 <strncpy+0xb2>
    1c0a:	8082                	ret
    1c0c:	8082                	ret
    1c0e:	8082                	ret

0000000000001c10 <open>:
#include <unistd.h>

#include "syscall.h"

int open(const char *path, int flags)
{
    1c10:	87aa                	mv	a5,a0
    1c12:	862e                	mv	a2,a1
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
}

static inline long __syscall4(long n, long a, long b, long c, long d)
{
    register long a7 __asm__("a7") = n;
    1c14:	03800893          	li	a7,56
    register long a0 __asm__("a0") = a;
    1c18:	f9c00513          	li	a0,-100
    register long a1 __asm__("a1") = b;
    1c1c:	85be                	mv	a1,a5
    register long a2 __asm__("a2") = c;
    register long a3 __asm__("a3") = d;
    1c1e:	4689                	li	a3,2
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3))
    1c20:	00000073          	ecall
    return syscall(SYS_openat, AT_FDCWD, path, flags, O_RDWR);
}
    1c24:	2501                	sext.w	a0,a0
    1c26:	8082                	ret

0000000000001c28 <openat>:
    register long a7 __asm__("a7") = n;
    1c28:	03800893          	li	a7,56
    register long a3 __asm__("a3") = d;
    1c2c:	18000693          	li	a3,384
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3))
    1c30:	00000073          	ecall

int openat(int dirfd,const char *path, int flags)
{
    return syscall(SYS_openat, dirfd, path, flags, 0600);
}
    1c34:	2501                	sext.w	a0,a0
    1c36:	8082                	ret

0000000000001c38 <close>:
    register long a7 __asm__("a7") = n;
    1c38:	03900893          	li	a7,57
    __asm_syscall("r"(a7), "0"(a0))
    1c3c:	00000073          	ecall

int close(int fd)
{
    return syscall(SYS_close, fd);
}
    1c40:	2501                	sext.w	a0,a0
    1c42:	8082                	ret

0000000000001c44 <read>:
    register long a7 __asm__("a7") = n;
    1c44:	03f00893          	li	a7,63
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1c48:	00000073          	ecall

ssize_t read(int fd, void *buf, size_t len)
{
    return syscall(SYS_read, fd, buf, len);
}
    1c4c:	8082                	ret

0000000000001c4e <write>:
    register long a7 __asm__("a7") = n;
    1c4e:	04000893          	li	a7,64
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1c52:	00000073          	ecall

ssize_t write(int fd, const void *buf, size_t len)
{
    return syscall(SYS_write, fd, buf, len);
}
    1c56:	8082                	ret

0000000000001c58 <getpid>:
    register long a7 __asm__("a7") = n;
    1c58:	0ac00893          	li	a7,172
    __asm_syscall("r"(a7))
    1c5c:	00000073          	ecall

pid_t getpid(void)
{
    return syscall(SYS_getpid);
}
    1c60:	2501                	sext.w	a0,a0
    1c62:	8082                	ret

0000000000001c64 <getppid>:
    register long a7 __asm__("a7") = n;
    1c64:	0ad00893          	li	a7,173
    __asm_syscall("r"(a7))
    1c68:	00000073          	ecall

pid_t getppid(void)
{
    return syscall(SYS_getppid);
}
    1c6c:	2501                	sext.w	a0,a0
    1c6e:	8082                	ret

0000000000001c70 <sched_yield>:
    register long a7 __asm__("a7") = n;
    1c70:	07c00893          	li	a7,124
    __asm_syscall("r"(a7))
    1c74:	00000073          	ecall

int sched_yield(void)
{
    return syscall(SYS_sched_yield);
}
    1c78:	2501                	sext.w	a0,a0
    1c7a:	8082                	ret

0000000000001c7c <fork>:
    register long a7 __asm__("a7") = n;
    1c7c:	0dc00893          	li	a7,220
    register long a0 __asm__("a0") = a;
    1c80:	4545                	li	a0,17
    register long a1 __asm__("a1") = b;
    1c82:	4581                	li	a1,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1c84:	00000073          	ecall

pid_t fork(void)
{
    return syscall(SYS_clone, SIGCHLD, 0);
}
    1c88:	2501                	sext.w	a0,a0
    1c8a:	8082                	ret

0000000000001c8c <clone>:

pid_t clone(int (*fn)(void *arg), void *arg, void *stack, size_t stack_size, unsigned long flags)
{
    1c8c:	85b2                	mv	a1,a2
    1c8e:	863a                	mv	a2,a4
    if (stack)
    1c90:	c191                	beqz	a1,1c94 <clone+0x8>
	stack += stack_size;
    1c92:	95b6                	add	a1,a1,a3

    return __clone(fn, stack, flags, NULL, NULL, NULL);
    1c94:	4781                	li	a5,0
    1c96:	4701                	li	a4,0
    1c98:	4681                	li	a3,0
    1c9a:	2601                	sext.w	a2,a2
    1c9c:	a2dd                	j	1e82 <__clone>

0000000000001c9e <exit>:
    register long a7 __asm__("a7") = n;
    1c9e:	05d00893          	li	a7,93
    __asm_syscall("r"(a7), "0"(a0))
    1ca2:	00000073          	ecall
    //return syscall(SYS_clone, fn, stack, flags, NULL, NULL, NULL);
}
void exit(int code)
{
    syscall(SYS_exit, code);
}
    1ca6:	8082                	ret

0000000000001ca8 <waitpid>:
    register long a7 __asm__("a7") = n;
    1ca8:	10400893          	li	a7,260
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1cac:	00000073          	ecall

int waitpid(int pid, int *code, int options)
{
    return syscall(SYS_wait4, pid, code, options);
}
    1cb0:	2501                	sext.w	a0,a0
    1cb2:	8082                	ret

0000000000001cb4 <exec>:
    register long a7 __asm__("a7") = n;
    1cb4:	0dd00893          	li	a7,221
    __asm_syscall("r"(a7), "0"(a0))
    1cb8:	00000073          	ecall

int exec(char *name)
{
    return syscall(SYS_execve, name);
}
    1cbc:	2501                	sext.w	a0,a0
    1cbe:	8082                	ret

0000000000001cc0 <execve>:
    register long a7 __asm__("a7") = n;
    1cc0:	0dd00893          	li	a7,221
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1cc4:	00000073          	ecall

int execve(const char *name, char *const argv[], char *const argp[])
{
    return syscall(SYS_execve, name, argv, argp);
}
    1cc8:	2501                	sext.w	a0,a0
    1cca:	8082                	ret

0000000000001ccc <times>:
    register long a7 __asm__("a7") = n;
    1ccc:	09900893          	li	a7,153
    __asm_syscall("r"(a7), "0"(a0))
    1cd0:	00000073          	ecall

int times(void *mytimes)
{
	return syscall(SYS_times, mytimes);
}
    1cd4:	2501                	sext.w	a0,a0
    1cd6:	8082                	ret

0000000000001cd8 <get_time>:

int64 get_time()
{
    1cd8:	1141                	addi	sp,sp,-16
    register long a7 __asm__("a7") = n;
    1cda:	0a900893          	li	a7,169
    register long a0 __asm__("a0") = a;
    1cde:	850a                	mv	a0,sp
    register long a1 __asm__("a1") = b;
    1ce0:	4581                	li	a1,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1ce2:	00000073          	ecall
    TimeVal time;
    int err = sys_get_time(&time, 0);
    if (err == 0)
    1ce6:	2501                	sext.w	a0,a0
    1ce8:	ed09                	bnez	a0,1d02 <get_time+0x2a>
    {
        return ((time.sec & 0xffff) * 1000 + time.usec / 1000);
    1cea:	67a2                	ld	a5,8(sp)
    1cec:	3e800713          	li	a4,1000
    1cf0:	00015503          	lhu	a0,0(sp)
    1cf4:	02e7d7b3          	divu	a5,a5,a4
    1cf8:	02e50533          	mul	a0,a0,a4
    1cfc:	953e                	add	a0,a0,a5
    }
    else
    {
        return -1;
    }
}
    1cfe:	0141                	addi	sp,sp,16
    1d00:	8082                	ret
        return -1;
    1d02:	557d                	li	a0,-1
    1d04:	bfed                	j	1cfe <get_time+0x26>

0000000000001d06 <sys_get_time>:
    register long a7 __asm__("a7") = n;
    1d06:	0a900893          	li	a7,169
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1d0a:	00000073          	ecall

int sys_get_time(TimeVal *ts, int tz)
{
    return syscall(SYS_gettimeofday, ts, tz);
}
    1d0e:	2501                	sext.w	a0,a0
    1d10:	8082                	ret

0000000000001d12 <time>:
    register long a7 __asm__("a7") = n;
    1d12:	42600893          	li	a7,1062
    __asm_syscall("r"(a7), "0"(a0))
    1d16:	00000073          	ecall

int time(unsigned long *tloc)
{
    return syscall(SYS_time, tloc);
}
    1d1a:	2501                	sext.w	a0,a0
    1d1c:	8082                	ret

0000000000001d1e <sleep>:

int sleep(unsigned long long time)
{
    1d1e:	1141                	addi	sp,sp,-16
    TimeVal tv = {.sec = time, .usec = 0};
    1d20:	e02a                	sd	a0,0(sp)
    register long a0 __asm__("a0") = a;
    1d22:	850a                	mv	a0,sp
    1d24:	e402                	sd	zero,8(sp)
    register long a7 __asm__("a7") = n;
    1d26:	06500893          	li	a7,101
    register long a1 __asm__("a1") = b;
    1d2a:	85aa                	mv	a1,a0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1d2c:	00000073          	ecall
    if (syscall(SYS_nanosleep, &tv, &tv)) return tv.sec;
    1d30:	e501                	bnez	a0,1d38 <sleep+0x1a>
    return 0;
    1d32:	4501                	li	a0,0
}
    1d34:	0141                	addi	sp,sp,16
    1d36:	8082                	ret
    if (syscall(SYS_nanosleep, &tv, &tv)) return tv.sec;
    1d38:	4502                	lw	a0,0(sp)
}
    1d3a:	0141                	addi	sp,sp,16
    1d3c:	8082                	ret

0000000000001d3e <set_priority>:
    register long a7 __asm__("a7") = n;
    1d3e:	08c00893          	li	a7,140
    __asm_syscall("r"(a7), "0"(a0))
    1d42:	00000073          	ecall

int set_priority(int prio)
{
    return syscall(SYS_setpriority, prio);
}
    1d46:	2501                	sext.w	a0,a0
    1d48:	8082                	ret

0000000000001d4a <mmap>:
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4))
}

static inline long __syscall6(long n, long a, long b, long c, long d, long e, long f)
{
    register long a7 __asm__("a7") = n;
    1d4a:	0de00893          	li	a7,222
    register long a1 __asm__("a1") = b;
    register long a2 __asm__("a2") = c;
    register long a3 __asm__("a3") = d;
    register long a4 __asm__("a4") = e;
    register long a5 __asm__("a5") = f;
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4), "r"(a5))
    1d4e:	00000073          	ecall

void *mmap(void *start, size_t len, int prot, int flags, int fd, off_t off)
{
    return syscall(SYS_mmap, start, len, prot, flags, fd, off);
}
    1d52:	8082                	ret

0000000000001d54 <munmap>:
    register long a7 __asm__("a7") = n;
    1d54:	0d700893          	li	a7,215
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1d58:	00000073          	ecall

int munmap(void *start, size_t len)
{
    return syscall(SYS_munmap, start, len);
}
    1d5c:	2501                	sext.w	a0,a0
    1d5e:	8082                	ret

0000000000001d60 <wait>:

int wait(int *code)
{
    1d60:	85aa                	mv	a1,a0
    register long a7 __asm__("a7") = n;
    1d62:	10400893          	li	a7,260
    register long a0 __asm__("a0") = a;
    1d66:	557d                	li	a0,-1
    register long a2 __asm__("a2") = c;
    1d68:	4601                	li	a2,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1d6a:	00000073          	ecall
    return waitpid(-1, code, 0);
}
    1d6e:	2501                	sext.w	a0,a0
    1d70:	8082                	ret

0000000000001d72 <spawn>:
    register long a7 __asm__("a7") = n;
    1d72:	19000893          	li	a7,400
    __asm_syscall("r"(a7), "0"(a0))
    1d76:	00000073          	ecall

int spawn(char *file)
{
    return syscall(SYS_spawn, file);
}
    1d7a:	2501                	sext.w	a0,a0
    1d7c:	8082                	ret

0000000000001d7e <mailread>:
    register long a7 __asm__("a7") = n;
    1d7e:	19100893          	li	a7,401
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1d82:	00000073          	ecall

int mailread(void *buf, int len)
{
    return syscall(SYS_mailread, buf, len);
}
    1d86:	2501                	sext.w	a0,a0
    1d88:	8082                	ret

0000000000001d8a <mailwrite>:
    register long a7 __asm__("a7") = n;
    1d8a:	19200893          	li	a7,402
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1d8e:	00000073          	ecall

int mailwrite(int pid, void *buf, int len)
{
    return syscall(SYS_mailwrite, pid, buf, len);
}
    1d92:	2501                	sext.w	a0,a0
    1d94:	8082                	ret

0000000000001d96 <fstat>:
    register long a7 __asm__("a7") = n;
    1d96:	05000893          	li	a7,80
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1d9a:	00000073          	ecall

int fstat(int fd, struct kstat *st)
{
    return syscall(SYS_fstat, fd, st);
}
    1d9e:	2501                	sext.w	a0,a0
    1da0:	8082                	ret

0000000000001da2 <sys_linkat>:
    register long a4 __asm__("a4") = e;
    1da2:	1702                	slli	a4,a4,0x20
    register long a7 __asm__("a7") = n;
    1da4:	02500893          	li	a7,37
    register long a4 __asm__("a4") = e;
    1da8:	9301                	srli	a4,a4,0x20
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4))
    1daa:	00000073          	ecall

int sys_linkat(int olddirfd, char *oldpath, int newdirfd, char *newpath, unsigned int flags)
{
    return syscall(SYS_linkat, olddirfd, oldpath, newdirfd, newpath, flags);
}
    1dae:	2501                	sext.w	a0,a0
    1db0:	8082                	ret

0000000000001db2 <sys_unlinkat>:
    register long a2 __asm__("a2") = c;
    1db2:	1602                	slli	a2,a2,0x20
    register long a7 __asm__("a7") = n;
    1db4:	02300893          	li	a7,35
    register long a2 __asm__("a2") = c;
    1db8:	9201                	srli	a2,a2,0x20
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1dba:	00000073          	ecall

int sys_unlinkat(int dirfd, char *path, unsigned int flags)
{
    return syscall(SYS_unlinkat, dirfd, path, flags);
}
    1dbe:	2501                	sext.w	a0,a0
    1dc0:	8082                	ret

0000000000001dc2 <link>:

int link(char *old_path, char *new_path)
{
    1dc2:	87aa                	mv	a5,a0
    1dc4:	86ae                	mv	a3,a1
    register long a7 __asm__("a7") = n;
    1dc6:	02500893          	li	a7,37
    register long a0 __asm__("a0") = a;
    1dca:	f9c00513          	li	a0,-100
    register long a1 __asm__("a1") = b;
    1dce:	85be                	mv	a1,a5
    register long a2 __asm__("a2") = c;
    1dd0:	f9c00613          	li	a2,-100
    register long a4 __asm__("a4") = e;
    1dd4:	4701                	li	a4,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4))
    1dd6:	00000073          	ecall
    return sys_linkat(AT_FDCWD, old_path, AT_FDCWD, new_path, 0);
}
    1dda:	2501                	sext.w	a0,a0
    1ddc:	8082                	ret

0000000000001dde <unlink>:

int unlink(char *path)
{
    1dde:	85aa                	mv	a1,a0
    register long a7 __asm__("a7") = n;
    1de0:	02300893          	li	a7,35
    register long a0 __asm__("a0") = a;
    1de4:	f9c00513          	li	a0,-100
    register long a2 __asm__("a2") = c;
    1de8:	4601                	li	a2,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1dea:	00000073          	ecall
    return sys_unlinkat(AT_FDCWD, path, 0);
}
    1dee:	2501                	sext.w	a0,a0
    1df0:	8082                	ret

0000000000001df2 <uname>:
    register long a7 __asm__("a7") = n;
    1df2:	0a000893          	li	a7,160
    __asm_syscall("r"(a7), "0"(a0))
    1df6:	00000073          	ecall

int uname(void *buf)
{
    return syscall(SYS_uname, buf);
}
    1dfa:	2501                	sext.w	a0,a0
    1dfc:	8082                	ret

0000000000001dfe <brk>:
    register long a7 __asm__("a7") = n;
    1dfe:	0d600893          	li	a7,214
    __asm_syscall("r"(a7), "0"(a0))
    1e02:	00000073          	ecall

int brk(void *addr)
{
    return syscall(SYS_brk, addr);
}
    1e06:	2501                	sext.w	a0,a0
    1e08:	8082                	ret

0000000000001e0a <getcwd>:
    register long a7 __asm__("a7") = n;
    1e0a:	48c5                	li	a7,17
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1e0c:	00000073          	ecall

char *getcwd(char *buf, size_t size){
    return syscall(SYS_getcwd, buf, size);
}
    1e10:	8082                	ret

0000000000001e12 <chdir>:
    register long a7 __asm__("a7") = n;
    1e12:	03100893          	li	a7,49
    __asm_syscall("r"(a7), "0"(a0))
    1e16:	00000073          	ecall

int chdir(const char *path){
    return syscall(SYS_chdir, path);
}
    1e1a:	2501                	sext.w	a0,a0
    1e1c:	8082                	ret

0000000000001e1e <mkdir>:

int mkdir(const char *path, mode_t mode){
    1e1e:	862e                	mv	a2,a1
    1e20:	87aa                	mv	a5,a0
    register long a2 __asm__("a2") = c;
    1e22:	1602                	slli	a2,a2,0x20
    register long a7 __asm__("a7") = n;
    1e24:	02200893          	li	a7,34
    register long a0 __asm__("a0") = a;
    1e28:	f9c00513          	li	a0,-100
    register long a1 __asm__("a1") = b;
    1e2c:	85be                	mv	a1,a5
    register long a2 __asm__("a2") = c;
    1e2e:	9201                	srli	a2,a2,0x20
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1e30:	00000073          	ecall
    return syscall(SYS_mkdirat, AT_FDCWD, path, mode);
}
    1e34:	2501                	sext.w	a0,a0
    1e36:	8082                	ret

0000000000001e38 <getdents>:
    register long a7 __asm__("a7") = n;
    1e38:	03d00893          	li	a7,61
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1e3c:	00000073          	ecall

int getdents(int fd, struct linux_dirent64 *dirp64, unsigned long len){
    //return syscall(SYS_getdents64, fd, dirp64, len);
    return syscall(SYS_getdents64, fd, dirp64, len);
}
    1e40:	2501                	sext.w	a0,a0
    1e42:	8082                	ret

0000000000001e44 <pipe>:
    register long a7 __asm__("a7") = n;
    1e44:	03b00893          	li	a7,59
    register long a1 __asm__("a1") = b;
    1e48:	4581                	li	a1,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1e4a:	00000073          	ecall

int pipe(int fd[2]){
    return syscall(SYS_pipe2, fd, 0);
}
    1e4e:	2501                	sext.w	a0,a0
    1e50:	8082                	ret

0000000000001e52 <dup>:
    register long a7 __asm__("a7") = n;
    1e52:	48dd                	li	a7,23
    __asm_syscall("r"(a7), "0"(a0))
    1e54:	00000073          	ecall

int dup(int fd){
    return syscall(SYS_dup, fd);
}
    1e58:	2501                	sext.w	a0,a0
    1e5a:	8082                	ret

0000000000001e5c <dup2>:
    register long a7 __asm__("a7") = n;
    1e5c:	48e1                	li	a7,24
    register long a2 __asm__("a2") = c;
    1e5e:	4601                	li	a2,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1e60:	00000073          	ecall

int dup2(int old, int new){
    return syscall(SYS_dup3, old, new, 0);
}
    1e64:	2501                	sext.w	a0,a0
    1e66:	8082                	ret

0000000000001e68 <mount>:
    register long a7 __asm__("a7") = n;
    1e68:	02800893          	li	a7,40
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4))
    1e6c:	00000073          	ecall

int mount(const char *special, const char *dir, const char *fstype, unsigned long flags, const void *data)
{
        return syscall(SYS_mount, special, dir, fstype, flags, data);
}
    1e70:	2501                	sext.w	a0,a0
    1e72:	8082                	ret

0000000000001e74 <umount>:
    register long a7 __asm__("a7") = n;
    1e74:	02700893          	li	a7,39
    register long a1 __asm__("a1") = b;
    1e78:	4581                	li	a1,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1e7a:	00000073          	ecall

int umount(const char *special)
{
        return syscall(SYS_umount2, special, 0);
}
    1e7e:	2501                	sext.w	a0,a0
    1e80:	8082                	ret

0000000000001e82 <__clone>:

.global __clone
.type  __clone, %function
__clone:
	# Save func and arg to stack
	addi a1, a1, -16
    1e82:	15c1                	addi	a1,a1,-16
	sd a0, 0(a1)
    1e84:	e188                	sd	a0,0(a1)
	sd a3, 8(a1)
    1e86:	e594                	sd	a3,8(a1)

	# Call SYS_clone
	mv a0, a2
    1e88:	8532                	mv	a0,a2
	mv a2, a4
    1e8a:	863a                	mv	a2,a4
	mv a3, a5
    1e8c:	86be                	mv	a3,a5
	mv a4, a6
    1e8e:	8742                	mv	a4,a6
	li a7, 220 # SYS_clone
    1e90:	0dc00893          	li	a7,220
	ecall
    1e94:	00000073          	ecall

	beqz a0, 1f
    1e98:	c111                	beqz	a0,1e9c <__clone+0x1a>
	# Parent
	ret
    1e9a:	8082                	ret

	# Child
1:      ld a1, 0(sp)
    1e9c:	6582                	ld	a1,0(sp)
	ld a0, 8(sp)
    1e9e:	6522                	ld	a0,8(sp)
	jalr a1
    1ea0:	9582                	jalr	a1

	# Exit
	li a7, 93 # SYS_exit
    1ea2:	05d00893          	li	a7,93
	ecall
    1ea6:	00000073          	ecall
