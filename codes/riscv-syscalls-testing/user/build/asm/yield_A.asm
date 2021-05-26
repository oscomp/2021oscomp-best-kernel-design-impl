
/home/ubuntu/rust/ultra-os/codes/riscv-syscalls-testing/user/build/riscv64/yield_A:     file format elf64-littleriscv


Disassembly of section .text:

0000000000001000 <_start>:
.section .text.entry
.globl _start
_start:
    mv a0, sp
    1000:	850a                	mv	a0,sp
    tail __start_main
    1002:	a09d                	j	1068 <__start_main>

0000000000001004 <test_yield>:

测试时，让A, B, C程序按顺序并同时运行
*/

int test_yield()
{
    1004:	7139                	addi	sp,sp,-64
    1006:	f426                	sd	s1,40(sp)
    //TEST_START(__func__);
    for (int i = 0; i < HEIGHT; ++i)
    {
        char buf[WIDTH + 1];
        for (int j = 0; j < WIDTH; ++j)
            buf[j] = 'A';
    1008:	6491                	lui	s1,0x4
{
    100a:	f822                	sd	s0,48(sp)
    100c:	f04a                	sd	s2,32(sp)
    100e:	ec4e                	sd	s3,24(sp)
    1010:	fc06                	sd	ra,56(sp)
    for (int i = 0; i < HEIGHT; ++i)
    1012:	4401                	li	s0,0
            buf[j] = 'A';
    1014:	00001997          	auipc	s3,0x1
    1018:	e949b983          	ld	s3,-364(s3) # 1ea8 <__clone+0x44>
    101c:	14148493          	addi	s1,s1,321 # 4141 <digits+0x2271>
        buf[WIDTH] = 0;
        printf("%s [%d/%d]\n", buf, i + 1, HEIGHT);
    1020:	00001917          	auipc	s2,0x1
    1024:	e7090913          	addi	s2,s2,-400 # 1e90 <__clone+0x2c>
    1028:	2405                	addiw	s0,s0,1
    102a:	4695                	li	a3,5
    102c:	8622                	mv	a2,s0
    102e:	858a                	mv	a1,sp
    1030:	854a                	mv	a0,s2
            buf[j] = 'A';
    1032:	e04e                	sd	s3,0(sp)
    1034:	00911423          	sh	s1,8(sp)
        buf[WIDTH] = 0;
    1038:	00010523          	sb	zero,10(sp)
        printf("%s [%d/%d]\n", buf, i + 1, HEIGHT);
    103c:	2c0000ef          	jal	ra,12fc <printf>
        sched_yield();
    1040:	413000ef          	jal	ra,1c52 <sched_yield>
    for (int i = 0; i < HEIGHT; ++i)
    1044:	4795                	li	a5,5
    1046:	fef411e3          	bne	s0,a5,1028 <test_yield+0x24>
    }
    //puts("Test write A OK!");
    //TEST_END(__func__);
}
    104a:	70e2                	ld	ra,56(sp)
    104c:	7442                	ld	s0,48(sp)
    104e:	74a2                	ld	s1,40(sp)
    1050:	7902                	ld	s2,32(sp)
    1052:	69e2                	ld	s3,24(sp)
    1054:	6121                	addi	sp,sp,64
    1056:	8082                	ret

0000000000001058 <main>:

int main(void) {
    1058:	1141                	addi	sp,sp,-16
    105a:	e406                	sd	ra,8(sp)
	test_yield();
    105c:	fa9ff0ef          	jal	ra,1004 <test_yield>
	return 0;
}
    1060:	60a2                	ld	ra,8(sp)
    1062:	4501                	li	a0,0
    1064:	0141                	addi	sp,sp,16
    1066:	8082                	ret

0000000000001068 <__start_main>:
#include <unistd.h>

extern int main();

int __start_main(long *p)
{
    1068:	85aa                	mv	a1,a0
	int argc = p[0];
	char **argv = (void *)(p+1);

	exit(main(argc, argv));
    106a:	4108                	lw	a0,0(a0)
{
    106c:	1141                	addi	sp,sp,-16
	exit(main(argc, argv));
    106e:	05a1                	addi	a1,a1,8
{
    1070:	e406                	sd	ra,8(sp)
	exit(main(argc, argv));
    1072:	fe7ff0ef          	jal	ra,1058 <main>
    1076:	40b000ef          	jal	ra,1c80 <exit>
	return 0;
}
    107a:	60a2                	ld	ra,8(sp)
    107c:	4501                	li	a0,0
    107e:	0141                	addi	sp,sp,16
    1080:	8082                	ret

0000000000001082 <printint.constprop.0>:
    write(f, s, l);
}

static char digits[] = "0123456789abcdef";

static void printint(int xx, int base, int sign)
    1082:	7179                	addi	sp,sp,-48
    1084:	f406                	sd	ra,40(sp)
{
    char buf[16 + 1];
    int i;
    uint x;

    if (sign && (sign = xx < 0))
    1086:	12054b63          	bltz	a0,11bc <printint.constprop.0+0x13a>

    buf[16] = 0;
    i = 15;
    do
    {
        buf[i--] = digits[x % base];
    108a:	02b577bb          	remuw	a5,a0,a1
    108e:	00001617          	auipc	a2,0x1
    1092:	e4260613          	addi	a2,a2,-446 # 1ed0 <digits>
    buf[16] = 0;
    1096:	00010c23          	sb	zero,24(sp)
        buf[i--] = digits[x % base];
    109a:	0005871b          	sext.w	a4,a1
    109e:	1782                	slli	a5,a5,0x20
    10a0:	9381                	srli	a5,a5,0x20
    10a2:	97b2                	add	a5,a5,a2
    10a4:	0007c783          	lbu	a5,0(a5)
    } while ((x /= base) != 0);
    10a8:	02b5583b          	divuw	a6,a0,a1
        buf[i--] = digits[x % base];
    10ac:	00f10ba3          	sb	a5,23(sp)
    } while ((x /= base) != 0);
    10b0:	1cb56363          	bltu	a0,a1,1276 <printint.constprop.0+0x1f4>
        buf[i--] = digits[x % base];
    10b4:	45b9                	li	a1,14
    10b6:	02e877bb          	remuw	a5,a6,a4
    10ba:	1782                	slli	a5,a5,0x20
    10bc:	9381                	srli	a5,a5,0x20
    10be:	97b2                	add	a5,a5,a2
    10c0:	0007c783          	lbu	a5,0(a5)
    } while ((x /= base) != 0);
    10c4:	02e856bb          	divuw	a3,a6,a4
        buf[i--] = digits[x % base];
    10c8:	00f10b23          	sb	a5,22(sp)
    } while ((x /= base) != 0);
    10cc:	0ce86e63          	bltu	a6,a4,11a8 <printint.constprop.0+0x126>
        buf[i--] = digits[x % base];
    10d0:	02e6f5bb          	remuw	a1,a3,a4
    } while ((x /= base) != 0);
    10d4:	02e6d7bb          	divuw	a5,a3,a4
        buf[i--] = digits[x % base];
    10d8:	1582                	slli	a1,a1,0x20
    10da:	9181                	srli	a1,a1,0x20
    10dc:	95b2                	add	a1,a1,a2
    10de:	0005c583          	lbu	a1,0(a1)
    10e2:	00b10aa3          	sb	a1,21(sp)
    } while ((x /= base) != 0);
    10e6:	0007859b          	sext.w	a1,a5
    10ea:	12e6ec63          	bltu	a3,a4,1222 <printint.constprop.0+0x1a0>
        buf[i--] = digits[x % base];
    10ee:	02e7f6bb          	remuw	a3,a5,a4
    10f2:	1682                	slli	a3,a3,0x20
    10f4:	9281                	srli	a3,a3,0x20
    10f6:	96b2                	add	a3,a3,a2
    10f8:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    10fc:	02e7d83b          	divuw	a6,a5,a4
        buf[i--] = digits[x % base];
    1100:	00d10a23          	sb	a3,20(sp)
    } while ((x /= base) != 0);
    1104:	12e5e863          	bltu	a1,a4,1234 <printint.constprop.0+0x1b2>
        buf[i--] = digits[x % base];
    1108:	02e876bb          	remuw	a3,a6,a4
    110c:	1682                	slli	a3,a3,0x20
    110e:	9281                	srli	a3,a3,0x20
    1110:	96b2                	add	a3,a3,a2
    1112:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    1116:	02e855bb          	divuw	a1,a6,a4
        buf[i--] = digits[x % base];
    111a:	00d109a3          	sb	a3,19(sp)
    } while ((x /= base) != 0);
    111e:	12e86463          	bltu	a6,a4,1246 <printint.constprop.0+0x1c4>
        buf[i--] = digits[x % base];
    1122:	02e5f6bb          	remuw	a3,a1,a4
    1126:	1682                	slli	a3,a3,0x20
    1128:	9281                	srli	a3,a3,0x20
    112a:	96b2                	add	a3,a3,a2
    112c:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    1130:	02e5d83b          	divuw	a6,a1,a4
        buf[i--] = digits[x % base];
    1134:	00d10923          	sb	a3,18(sp)
    } while ((x /= base) != 0);
    1138:	0ce5ec63          	bltu	a1,a4,1210 <printint.constprop.0+0x18e>
        buf[i--] = digits[x % base];
    113c:	02e876bb          	remuw	a3,a6,a4
    1140:	1682                	slli	a3,a3,0x20
    1142:	9281                	srli	a3,a3,0x20
    1144:	96b2                	add	a3,a3,a2
    1146:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    114a:	02e855bb          	divuw	a1,a6,a4
        buf[i--] = digits[x % base];
    114e:	00d108a3          	sb	a3,17(sp)
    } while ((x /= base) != 0);
    1152:	10e86963          	bltu	a6,a4,1264 <printint.constprop.0+0x1e2>
        buf[i--] = digits[x % base];
    1156:	02e5f6bb          	remuw	a3,a1,a4
    115a:	1682                	slli	a3,a3,0x20
    115c:	9281                	srli	a3,a3,0x20
    115e:	96b2                	add	a3,a3,a2
    1160:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    1164:	02e5d83b          	divuw	a6,a1,a4
        buf[i--] = digits[x % base];
    1168:	00d10823          	sb	a3,16(sp)
    } while ((x /= base) != 0);
    116c:	10e5e763          	bltu	a1,a4,127a <printint.constprop.0+0x1f8>
        buf[i--] = digits[x % base];
    1170:	02e876bb          	remuw	a3,a6,a4
    1174:	1682                	slli	a3,a3,0x20
    1176:	9281                	srli	a3,a3,0x20
    1178:	96b2                	add	a3,a3,a2
    117a:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    117e:	02e857bb          	divuw	a5,a6,a4
        buf[i--] = digits[x % base];
    1182:	00d107a3          	sb	a3,15(sp)
    } while ((x /= base) != 0);
    1186:	10e86363          	bltu	a6,a4,128c <printint.constprop.0+0x20a>
        buf[i--] = digits[x % base];
    118a:	1782                	slli	a5,a5,0x20
    118c:	9381                	srli	a5,a5,0x20
    118e:	97b2                	add	a5,a5,a2
    1190:	0007c783          	lbu	a5,0(a5)
    1194:	4599                	li	a1,6
    1196:	00f10723          	sb	a5,14(sp)

    if (sign)
    119a:	00055763          	bgez	a0,11a8 <printint.constprop.0+0x126>
        buf[i--] = '-';
    119e:	02d00793          	li	a5,45
    11a2:	00f106a3          	sb	a5,13(sp)
        buf[i--] = digits[x % base];
    11a6:	4595                	li	a1,5
    write(f, s, l);
    11a8:	003c                	addi	a5,sp,8
    11aa:	4641                	li	a2,16
    11ac:	9e0d                	subw	a2,a2,a1
    11ae:	4505                	li	a0,1
    11b0:	95be                	add	a1,a1,a5
    11b2:	27f000ef          	jal	ra,1c30 <write>
    i++;
    if (i < 0)
        puts("printint error");
    out(stdout, buf + i, 16 - i);
}
    11b6:	70a2                	ld	ra,40(sp)
    11b8:	6145                	addi	sp,sp,48
    11ba:	8082                	ret
        x = -xx;
    11bc:	40a0083b          	negw	a6,a0
        buf[i--] = digits[x % base];
    11c0:	02b877bb          	remuw	a5,a6,a1
    11c4:	00001617          	auipc	a2,0x1
    11c8:	d0c60613          	addi	a2,a2,-756 # 1ed0 <digits>
    buf[16] = 0;
    11cc:	00010c23          	sb	zero,24(sp)
        buf[i--] = digits[x % base];
    11d0:	0005871b          	sext.w	a4,a1
    11d4:	1782                	slli	a5,a5,0x20
    11d6:	9381                	srli	a5,a5,0x20
    11d8:	97b2                	add	a5,a5,a2
    11da:	0007c783          	lbu	a5,0(a5)
    } while ((x /= base) != 0);
    11de:	02b858bb          	divuw	a7,a6,a1
        buf[i--] = digits[x % base];
    11e2:	00f10ba3          	sb	a5,23(sp)
    } while ((x /= base) != 0);
    11e6:	06b86963          	bltu	a6,a1,1258 <printint.constprop.0+0x1d6>
        buf[i--] = digits[x % base];
    11ea:	02e8f7bb          	remuw	a5,a7,a4
    11ee:	1782                	slli	a5,a5,0x20
    11f0:	9381                	srli	a5,a5,0x20
    11f2:	97b2                	add	a5,a5,a2
    11f4:	0007c783          	lbu	a5,0(a5)
    } while ((x /= base) != 0);
    11f8:	02e8d6bb          	divuw	a3,a7,a4
        buf[i--] = digits[x % base];
    11fc:	00f10b23          	sb	a5,22(sp)
    } while ((x /= base) != 0);
    1200:	ece8f8e3          	bgeu	a7,a4,10d0 <printint.constprop.0+0x4e>
        buf[i--] = '-';
    1204:	02d00793          	li	a5,45
    1208:	00f10aa3          	sb	a5,21(sp)
        buf[i--] = digits[x % base];
    120c:	45b5                	li	a1,13
    120e:	bf69                	j	11a8 <printint.constprop.0+0x126>
    1210:	45a9                	li	a1,10
    if (sign)
    1212:	f8055be3          	bgez	a0,11a8 <printint.constprop.0+0x126>
        buf[i--] = '-';
    1216:	02d00793          	li	a5,45
    121a:	00f108a3          	sb	a5,17(sp)
        buf[i--] = digits[x % base];
    121e:	45a5                	li	a1,9
    1220:	b761                	j	11a8 <printint.constprop.0+0x126>
    1222:	45b5                	li	a1,13
    if (sign)
    1224:	f80552e3          	bgez	a0,11a8 <printint.constprop.0+0x126>
        buf[i--] = '-';
    1228:	02d00793          	li	a5,45
    122c:	00f10a23          	sb	a5,20(sp)
        buf[i--] = digits[x % base];
    1230:	45b1                	li	a1,12
    1232:	bf9d                	j	11a8 <printint.constprop.0+0x126>
    1234:	45b1                	li	a1,12
    if (sign)
    1236:	f60559e3          	bgez	a0,11a8 <printint.constprop.0+0x126>
        buf[i--] = '-';
    123a:	02d00793          	li	a5,45
    123e:	00f109a3          	sb	a5,19(sp)
        buf[i--] = digits[x % base];
    1242:	45ad                	li	a1,11
    1244:	b795                	j	11a8 <printint.constprop.0+0x126>
    1246:	45ad                	li	a1,11
    if (sign)
    1248:	f60550e3          	bgez	a0,11a8 <printint.constprop.0+0x126>
        buf[i--] = '-';
    124c:	02d00793          	li	a5,45
    1250:	00f10923          	sb	a5,18(sp)
        buf[i--] = digits[x % base];
    1254:	45a9                	li	a1,10
    1256:	bf89                	j	11a8 <printint.constprop.0+0x126>
        buf[i--] = '-';
    1258:	02d00793          	li	a5,45
    125c:	00f10b23          	sb	a5,22(sp)
        buf[i--] = digits[x % base];
    1260:	45b9                	li	a1,14
    1262:	b799                	j	11a8 <printint.constprop.0+0x126>
    1264:	45a5                	li	a1,9
    if (sign)
    1266:	f40551e3          	bgez	a0,11a8 <printint.constprop.0+0x126>
        buf[i--] = '-';
    126a:	02d00793          	li	a5,45
    126e:	00f10823          	sb	a5,16(sp)
        buf[i--] = digits[x % base];
    1272:	45a1                	li	a1,8
    1274:	bf15                	j	11a8 <printint.constprop.0+0x126>
    i = 15;
    1276:	45bd                	li	a1,15
    1278:	bf05                	j	11a8 <printint.constprop.0+0x126>
        buf[i--] = digits[x % base];
    127a:	45a1                	li	a1,8
    if (sign)
    127c:	f20556e3          	bgez	a0,11a8 <printint.constprop.0+0x126>
        buf[i--] = '-';
    1280:	02d00793          	li	a5,45
    1284:	00f107a3          	sb	a5,15(sp)
        buf[i--] = digits[x % base];
    1288:	459d                	li	a1,7
    128a:	bf39                	j	11a8 <printint.constprop.0+0x126>
    128c:	459d                	li	a1,7
    if (sign)
    128e:	f0055de3          	bgez	a0,11a8 <printint.constprop.0+0x126>
        buf[i--] = '-';
    1292:	02d00793          	li	a5,45
    1296:	00f10723          	sb	a5,14(sp)
        buf[i--] = digits[x % base];
    129a:	4599                	li	a1,6
    129c:	b731                	j	11a8 <printint.constprop.0+0x126>

000000000000129e <getchar>:
{
    129e:	1101                	addi	sp,sp,-32
    read(stdin, &byte, 1);
    12a0:	00f10593          	addi	a1,sp,15
    12a4:	4605                	li	a2,1
    12a6:	4501                	li	a0,0
{
    12a8:	ec06                	sd	ra,24(sp)
    char byte = 0;
    12aa:	000107a3          	sb	zero,15(sp)
    read(stdin, &byte, 1);
    12ae:	179000ef          	jal	ra,1c26 <read>
}
    12b2:	60e2                	ld	ra,24(sp)
    12b4:	00f14503          	lbu	a0,15(sp)
    12b8:	6105                	addi	sp,sp,32
    12ba:	8082                	ret

00000000000012bc <putchar>:
{
    12bc:	1101                	addi	sp,sp,-32
    12be:	87aa                	mv	a5,a0
    return write(stdout, &byte, 1);
    12c0:	00f10593          	addi	a1,sp,15
    12c4:	4605                	li	a2,1
    12c6:	4505                	li	a0,1
{
    12c8:	ec06                	sd	ra,24(sp)
    char byte = c;
    12ca:	00f107a3          	sb	a5,15(sp)
    return write(stdout, &byte, 1);
    12ce:	163000ef          	jal	ra,1c30 <write>
}
    12d2:	60e2                	ld	ra,24(sp)
    12d4:	2501                	sext.w	a0,a0
    12d6:	6105                	addi	sp,sp,32
    12d8:	8082                	ret

00000000000012da <puts>:
{
    12da:	1141                	addi	sp,sp,-16
    12dc:	e406                	sd	ra,8(sp)
    12de:	e022                	sd	s0,0(sp)
    12e0:	842a                	mv	s0,a0
    r = -(write(stdout, s, strlen(s)) < 0);
    12e2:	56a000ef          	jal	ra,184c <strlen>
    12e6:	862a                	mv	a2,a0
    12e8:	85a2                	mv	a1,s0
    12ea:	4505                	li	a0,1
    12ec:	145000ef          	jal	ra,1c30 <write>
}
    12f0:	60a2                	ld	ra,8(sp)
    12f2:	6402                	ld	s0,0(sp)
    r = -(write(stdout, s, strlen(s)) < 0);
    12f4:	957d                	srai	a0,a0,0x3f
    return r;
    12f6:	2501                	sext.w	a0,a0
}
    12f8:	0141                	addi	sp,sp,16
    12fa:	8082                	ret

00000000000012fc <printf>:
    out(stdout, buf, i);
}

// Print to the console. only understands %d, %x, %p, %s.
void printf(const char *fmt, ...)
{
    12fc:	7171                	addi	sp,sp,-176
    12fe:	fc56                	sd	s5,56(sp)
    1300:	ed3e                	sd	a5,152(sp)
    buf[i++] = '0';
    1302:	7ae1                	lui	s5,0xffff8
    va_list ap;
    int cnt = 0, l = 0;
    char *a, *z, *s = (char *)fmt, str;
    int f = stdout;

    va_start(ap, fmt);
    1304:	18bc                	addi	a5,sp,120
{
    1306:	e8ca                	sd	s2,80(sp)
    1308:	e4ce                	sd	s3,72(sp)
    130a:	e0d2                	sd	s4,64(sp)
    130c:	f85a                	sd	s6,48(sp)
    130e:	f486                	sd	ra,104(sp)
    1310:	f0a2                	sd	s0,96(sp)
    1312:	eca6                	sd	s1,88(sp)
    1314:	fcae                	sd	a1,120(sp)
    1316:	e132                	sd	a2,128(sp)
    1318:	e536                	sd	a3,136(sp)
    131a:	e93a                	sd	a4,144(sp)
    131c:	f142                	sd	a6,160(sp)
    131e:	f546                	sd	a7,168(sp)
    va_start(ap, fmt);
    1320:	e03e                	sd	a5,0(sp)
    for (;;)
    {
        if (!*s)
            break;
        for (a = s; *s && *s != '%'; s++)
    1322:	02500913          	li	s2,37
        out(f, a, l);
        if (l)
            continue;
        if (s[1] == 0)
            break;
        switch (s[1])
    1326:	07300a13          	li	s4,115
        case 'p':
            printptr(va_arg(ap, uint64));
            break;
        case 's':
            if ((a = va_arg(ap, char *)) == 0)
                a = "(null)";
    132a:	00001b17          	auipc	s6,0x1
    132e:	b76b0b13          	addi	s6,s6,-1162 # 1ea0 <__clone+0x3c>
    buf[i++] = '0';
    1332:	830aca93          	xori	s5,s5,-2000
        buf[i++] = digits[x >> (sizeof(uint64) * 8 - 4)];
    1336:	00001997          	auipc	s3,0x1
    133a:	b9a98993          	addi	s3,s3,-1126 # 1ed0 <digits>
        if (!*s)
    133e:	00054783          	lbu	a5,0(a0)
    1342:	16078a63          	beqz	a5,14b6 <printf+0x1ba>
    1346:	862a                	mv	a2,a0
        for (a = s; *s && *s != '%'; s++)
    1348:	19278163          	beq	a5,s2,14ca <printf+0x1ce>
    134c:	00164783          	lbu	a5,1(a2)
    1350:	0605                	addi	a2,a2,1
    1352:	fbfd                	bnez	a5,1348 <printf+0x4c>
    1354:	84b2                	mv	s1,a2
        l = z - a;
    1356:	40a6043b          	subw	s0,a2,a0
    write(f, s, l);
    135a:	85aa                	mv	a1,a0
    135c:	8622                	mv	a2,s0
    135e:	4505                	li	a0,1
    1360:	0d1000ef          	jal	ra,1c30 <write>
        if (l)
    1364:	18041c63          	bnez	s0,14fc <printf+0x200>
        if (s[1] == 0)
    1368:	0014c783          	lbu	a5,1(s1)
    136c:	14078563          	beqz	a5,14b6 <printf+0x1ba>
        switch (s[1])
    1370:	1d478063          	beq	a5,s4,1530 <printf+0x234>
    1374:	18fa6663          	bltu	s4,a5,1500 <printf+0x204>
    1378:	06400713          	li	a4,100
    137c:	1ae78063          	beq	a5,a4,151c <printf+0x220>
    1380:	07000713          	li	a4,112
    1384:	1ce79963          	bne	a5,a4,1556 <printf+0x25a>
            printptr(va_arg(ap, uint64));
    1388:	6702                	ld	a4,0(sp)
    buf[i++] = '0';
    138a:	01511423          	sh	s5,8(sp)
    write(f, s, l);
    138e:	4649                	li	a2,18
            printptr(va_arg(ap, uint64));
    1390:	631c                	ld	a5,0(a4)
    1392:	0721                	addi	a4,a4,8
    1394:	e03a                	sd	a4,0(sp)
    for (j = 0; j < (sizeof(uint64) * 2); j++, x <<= 4)
    1396:	00479293          	slli	t0,a5,0x4
    139a:	00879f93          	slli	t6,a5,0x8
    139e:	00c79f13          	slli	t5,a5,0xc
    13a2:	01079e93          	slli	t4,a5,0x10
    13a6:	01479e13          	slli	t3,a5,0x14
    13aa:	01879313          	slli	t1,a5,0x18
    13ae:	01c79893          	slli	a7,a5,0x1c
    13b2:	02479813          	slli	a6,a5,0x24
    13b6:	02879513          	slli	a0,a5,0x28
    13ba:	02c79593          	slli	a1,a5,0x2c
    13be:	03079693          	slli	a3,a5,0x30
    13c2:	03479713          	slli	a4,a5,0x34
        buf[i++] = digits[x >> (sizeof(uint64) * 8 - 4)];
    13c6:	03c7d413          	srli	s0,a5,0x3c
    13ca:	01c7d39b          	srliw	t2,a5,0x1c
    13ce:	03c2d293          	srli	t0,t0,0x3c
    13d2:	03cfdf93          	srli	t6,t6,0x3c
    13d6:	03cf5f13          	srli	t5,t5,0x3c
    13da:	03cede93          	srli	t4,t4,0x3c
    13de:	03ce5e13          	srli	t3,t3,0x3c
    13e2:	03c35313          	srli	t1,t1,0x3c
    13e6:	03c8d893          	srli	a7,a7,0x3c
    13ea:	03c85813          	srli	a6,a6,0x3c
    13ee:	9171                	srli	a0,a0,0x3c
    13f0:	91f1                	srli	a1,a1,0x3c
    13f2:	92f1                	srli	a3,a3,0x3c
    13f4:	9371                	srli	a4,a4,0x3c
    13f6:	96ce                	add	a3,a3,s3
    13f8:	974e                	add	a4,a4,s3
    13fa:	944e                	add	s0,s0,s3
    13fc:	92ce                	add	t0,t0,s3
    13fe:	9fce                	add	t6,t6,s3
    1400:	9f4e                	add	t5,t5,s3
    1402:	9ece                	add	t4,t4,s3
    1404:	9e4e                	add	t3,t3,s3
    1406:	934e                	add	t1,t1,s3
    1408:	98ce                	add	a7,a7,s3
    140a:	93ce                	add	t2,t2,s3
    140c:	984e                	add	a6,a6,s3
    140e:	954e                	add	a0,a0,s3
    1410:	95ce                	add	a1,a1,s3
    1412:	0006c083          	lbu	ra,0(a3)
    1416:	0002c283          	lbu	t0,0(t0)
    141a:	00074683          	lbu	a3,0(a4)
    141e:	000fcf83          	lbu	t6,0(t6)
    1422:	000f4f03          	lbu	t5,0(t5)
    1426:	000ece83          	lbu	t4,0(t4)
    142a:	000e4e03          	lbu	t3,0(t3)
    142e:	00034303          	lbu	t1,0(t1)
    1432:	0008c883          	lbu	a7,0(a7)
    1436:	0003c383          	lbu	t2,0(t2)
    143a:	00084803          	lbu	a6,0(a6)
    143e:	00054503          	lbu	a0,0(a0)
    1442:	0005c583          	lbu	a1,0(a1)
    1446:	00044403          	lbu	s0,0(s0)
    for (j = 0; j < (sizeof(uint64) * 2); j++, x <<= 4)
    144a:	03879713          	slli	a4,a5,0x38
        buf[i++] = digits[x >> (sizeof(uint64) * 8 - 4)];
    144e:	9371                	srli	a4,a4,0x3c
    1450:	8bbd                	andi	a5,a5,15
    1452:	974e                	add	a4,a4,s3
    1454:	97ce                	add	a5,a5,s3
    1456:	005105a3          	sb	t0,11(sp)
    145a:	01f10623          	sb	t6,12(sp)
    145e:	01e106a3          	sb	t5,13(sp)
    1462:	01d10723          	sb	t4,14(sp)
    1466:	01c107a3          	sb	t3,15(sp)
    146a:	00610823          	sb	t1,16(sp)
    146e:	011108a3          	sb	a7,17(sp)
    1472:	00710923          	sb	t2,18(sp)
    1476:	010109a3          	sb	a6,19(sp)
    147a:	00a10a23          	sb	a0,20(sp)
    147e:	00b10aa3          	sb	a1,21(sp)
    1482:	00110b23          	sb	ra,22(sp)
    1486:	00d10ba3          	sb	a3,23(sp)
    148a:	00810523          	sb	s0,10(sp)
    148e:	00074703          	lbu	a4,0(a4)
    1492:	0007c783          	lbu	a5,0(a5)
    write(f, s, l);
    1496:	002c                	addi	a1,sp,8
    1498:	4505                	li	a0,1
        buf[i++] = digits[x >> (sizeof(uint64) * 8 - 4)];
    149a:	00e10c23          	sb	a4,24(sp)
    149e:	00f10ca3          	sb	a5,25(sp)
    buf[i] = 0;
    14a2:	00010d23          	sb	zero,26(sp)
    write(f, s, l);
    14a6:	78a000ef          	jal	ra,1c30 <write>
            // Print unknown % sequence to draw attention.
            putchar('%');
            putchar(s[1]);
            break;
        }
        s += 2;
    14aa:	00248513          	addi	a0,s1,2
        if (!*s)
    14ae:	00054783          	lbu	a5,0(a0)
    14b2:	e8079ae3          	bnez	a5,1346 <printf+0x4a>
    }
    va_end(ap);
}
    14b6:	70a6                	ld	ra,104(sp)
    14b8:	7406                	ld	s0,96(sp)
    14ba:	64e6                	ld	s1,88(sp)
    14bc:	6946                	ld	s2,80(sp)
    14be:	69a6                	ld	s3,72(sp)
    14c0:	6a06                	ld	s4,64(sp)
    14c2:	7ae2                	ld	s5,56(sp)
    14c4:	7b42                	ld	s6,48(sp)
    14c6:	614d                	addi	sp,sp,176
    14c8:	8082                	ret
        for (z = s; s[0] == '%' && s[1] == '%'; z++, s += 2)
    14ca:	00064783          	lbu	a5,0(a2)
    14ce:	84b2                	mv	s1,a2
    14d0:	01278963          	beq	a5,s2,14e2 <printf+0x1e6>
    14d4:	b549                	j	1356 <printf+0x5a>
    14d6:	0024c783          	lbu	a5,2(s1)
    14da:	0605                	addi	a2,a2,1
    14dc:	0489                	addi	s1,s1,2
    14de:	e7279ce3          	bne	a5,s2,1356 <printf+0x5a>
    14e2:	0014c783          	lbu	a5,1(s1)
    14e6:	ff2788e3          	beq	a5,s2,14d6 <printf+0x1da>
        l = z - a;
    14ea:	40a6043b          	subw	s0,a2,a0
    write(f, s, l);
    14ee:	85aa                	mv	a1,a0
    14f0:	8622                	mv	a2,s0
    14f2:	4505                	li	a0,1
    14f4:	73c000ef          	jal	ra,1c30 <write>
        if (l)
    14f8:	e60408e3          	beqz	s0,1368 <printf+0x6c>
    14fc:	8526                	mv	a0,s1
    14fe:	b581                	j	133e <printf+0x42>
        switch (s[1])
    1500:	07800713          	li	a4,120
    1504:	04e79963          	bne	a5,a4,1556 <printf+0x25a>
            printint(va_arg(ap, int), 16, 1);
    1508:	6782                	ld	a5,0(sp)
    150a:	45c1                	li	a1,16
    150c:	4388                	lw	a0,0(a5)
    150e:	07a1                	addi	a5,a5,8
    1510:	e03e                	sd	a5,0(sp)
    1512:	b71ff0ef          	jal	ra,1082 <printint.constprop.0>
        s += 2;
    1516:	00248513          	addi	a0,s1,2
    151a:	bf51                	j	14ae <printf+0x1b2>
            printint(va_arg(ap, int), 10, 1);
    151c:	6782                	ld	a5,0(sp)
    151e:	45a9                	li	a1,10
    1520:	4388                	lw	a0,0(a5)
    1522:	07a1                	addi	a5,a5,8
    1524:	e03e                	sd	a5,0(sp)
    1526:	b5dff0ef          	jal	ra,1082 <printint.constprop.0>
        s += 2;
    152a:	00248513          	addi	a0,s1,2
    152e:	b741                	j	14ae <printf+0x1b2>
            if ((a = va_arg(ap, char *)) == 0)
    1530:	6782                	ld	a5,0(sp)
    1532:	6380                	ld	s0,0(a5)
    1534:	07a1                	addi	a5,a5,8
    1536:	e03e                	sd	a5,0(sp)
    1538:	c031                	beqz	s0,157c <printf+0x280>
            l = strnlen(a, 200);
    153a:	0c800593          	li	a1,200
    153e:	8522                	mv	a0,s0
    1540:	3f8000ef          	jal	ra,1938 <strnlen>
    write(f, s, l);
    1544:	0005061b          	sext.w	a2,a0
    1548:	85a2                	mv	a1,s0
    154a:	4505                	li	a0,1
    154c:	6e4000ef          	jal	ra,1c30 <write>
        s += 2;
    1550:	00248513          	addi	a0,s1,2
    1554:	bfa9                	j	14ae <printf+0x1b2>
    return write(stdout, &byte, 1);
    1556:	4605                	li	a2,1
    1558:	002c                	addi	a1,sp,8
    155a:	4505                	li	a0,1
    char byte = c;
    155c:	01210423          	sb	s2,8(sp)
    return write(stdout, &byte, 1);
    1560:	6d0000ef          	jal	ra,1c30 <write>
    char byte = c;
    1564:	0014c783          	lbu	a5,1(s1)
    return write(stdout, &byte, 1);
    1568:	4605                	li	a2,1
    156a:	002c                	addi	a1,sp,8
    156c:	4505                	li	a0,1
    char byte = c;
    156e:	00f10423          	sb	a5,8(sp)
    return write(stdout, &byte, 1);
    1572:	6be000ef          	jal	ra,1c30 <write>
        s += 2;
    1576:	00248513          	addi	a0,s1,2
    157a:	bf15                	j	14ae <printf+0x1b2>
                a = "(null)";
    157c:	845a                	mv	s0,s6
    157e:	bf75                	j	153a <printf+0x23e>

0000000000001580 <isspace>:
#define HIGHS (ONES * (UCHAR_MAX / 2 + 1))
#define HASZERO(x) (((x)-ONES) & ~(x)&HIGHS)

int isspace(int c)
{
    return c == ' ' || (unsigned)c - '\t' < 5;
    1580:	02000793          	li	a5,32
    1584:	00f50663          	beq	a0,a5,1590 <isspace+0x10>
    1588:	355d                	addiw	a0,a0,-9
    158a:	00553513          	sltiu	a0,a0,5
    158e:	8082                	ret
    1590:	4505                	li	a0,1
}
    1592:	8082                	ret

0000000000001594 <isdigit>:

int isdigit(int c)
{
    return (unsigned)c - '0' < 10;
    1594:	fd05051b          	addiw	a0,a0,-48
}
    1598:	00a53513          	sltiu	a0,a0,10
    159c:	8082                	ret

000000000000159e <atoi>:
    return c == ' ' || (unsigned)c - '\t' < 5;
    159e:	02000613          	li	a2,32
    15a2:	4591                	li	a1,4

int atoi(const char *s)
{
    int n = 0, neg = 0;
    while (isspace(*s))
    15a4:	00054703          	lbu	a4,0(a0)
    return c == ' ' || (unsigned)c - '\t' < 5;
    15a8:	ff77069b          	addiw	a3,a4,-9
    15ac:	04c70d63          	beq	a4,a2,1606 <atoi+0x68>
    15b0:	0007079b          	sext.w	a5,a4
    15b4:	04d5f963          	bgeu	a1,a3,1606 <atoi+0x68>
        s++;
    switch (*s)
    15b8:	02b00693          	li	a3,43
    15bc:	04d70a63          	beq	a4,a3,1610 <atoi+0x72>
    15c0:	02d00693          	li	a3,45
    15c4:	06d70463          	beq	a4,a3,162c <atoi+0x8e>
        neg = 1;
    case '+':
        s++;
    }
    /* Compute n as a negative number to avoid overflow on INT_MIN */
    while (isdigit(*s))
    15c8:	fd07859b          	addiw	a1,a5,-48
    15cc:	4625                	li	a2,9
    15ce:	873e                	mv	a4,a5
    15d0:	86aa                	mv	a3,a0
    int n = 0, neg = 0;
    15d2:	4e01                	li	t3,0
    while (isdigit(*s))
    15d4:	04b66a63          	bltu	a2,a1,1628 <atoi+0x8a>
    int n = 0, neg = 0;
    15d8:	4501                	li	a0,0
    while (isdigit(*s))
    15da:	4825                	li	a6,9
    15dc:	0016c603          	lbu	a2,1(a3)
        n = 10 * n - (*s++ - '0');
    15e0:	0025179b          	slliw	a5,a0,0x2
    15e4:	9d3d                	addw	a0,a0,a5
    15e6:	fd07031b          	addiw	t1,a4,-48
    15ea:	0015189b          	slliw	a7,a0,0x1
    while (isdigit(*s))
    15ee:	fd06059b          	addiw	a1,a2,-48
        n = 10 * n - (*s++ - '0');
    15f2:	0685                	addi	a3,a3,1
    15f4:	4068853b          	subw	a0,a7,t1
    while (isdigit(*s))
    15f8:	0006071b          	sext.w	a4,a2
    15fc:	feb870e3          	bgeu	a6,a1,15dc <atoi+0x3e>
    return neg ? n : -n;
    1600:	000e0563          	beqz	t3,160a <atoi+0x6c>
}
    1604:	8082                	ret
        s++;
    1606:	0505                	addi	a0,a0,1
    1608:	bf71                	j	15a4 <atoi+0x6>
    return neg ? n : -n;
    160a:	4113053b          	subw	a0,t1,a7
    160e:	8082                	ret
    while (isdigit(*s))
    1610:	00154783          	lbu	a5,1(a0)
    1614:	4625                	li	a2,9
        s++;
    1616:	00150693          	addi	a3,a0,1
    while (isdigit(*s))
    161a:	fd07859b          	addiw	a1,a5,-48
    161e:	0007871b          	sext.w	a4,a5
    int n = 0, neg = 0;
    1622:	4e01                	li	t3,0
    while (isdigit(*s))
    1624:	fab67ae3          	bgeu	a2,a1,15d8 <atoi+0x3a>
    1628:	4501                	li	a0,0
}
    162a:	8082                	ret
    while (isdigit(*s))
    162c:	00154783          	lbu	a5,1(a0)
    1630:	4625                	li	a2,9
        s++;
    1632:	00150693          	addi	a3,a0,1
    while (isdigit(*s))
    1636:	fd07859b          	addiw	a1,a5,-48
    163a:	0007871b          	sext.w	a4,a5
    163e:	feb665e3          	bltu	a2,a1,1628 <atoi+0x8a>
        neg = 1;
    1642:	4e05                	li	t3,1
    1644:	bf51                	j	15d8 <atoi+0x3a>

0000000000001646 <memset>:

void *memset(void *dest, int c, size_t n)
{
    char *p = dest;
    for (int i = 0; i < n; ++i, *(p++) = c)
    1646:	16060d63          	beqz	a2,17c0 <memset+0x17a>
    164a:	40a007b3          	neg	a5,a0
    164e:	8b9d                	andi	a5,a5,7
    1650:	00778713          	addi	a4,a5,7
    1654:	482d                	li	a6,11
    1656:	0ff5f593          	andi	a1,a1,255
    165a:	fff60693          	addi	a3,a2,-1
    165e:	17076263          	bltu	a4,a6,17c2 <memset+0x17c>
    1662:	16e6ea63          	bltu	a3,a4,17d6 <memset+0x190>
    1666:	16078563          	beqz	a5,17d0 <memset+0x18a>
    166a:	00b50023          	sb	a1,0(a0)
    166e:	4705                	li	a4,1
    1670:	00150e93          	addi	t4,a0,1
    1674:	14e78c63          	beq	a5,a4,17cc <memset+0x186>
    1678:	00b500a3          	sb	a1,1(a0)
    167c:	4709                	li	a4,2
    167e:	00250e93          	addi	t4,a0,2
    1682:	14e78d63          	beq	a5,a4,17dc <memset+0x196>
    1686:	00b50123          	sb	a1,2(a0)
    168a:	470d                	li	a4,3
    168c:	00350e93          	addi	t4,a0,3
    1690:	12e78b63          	beq	a5,a4,17c6 <memset+0x180>
    1694:	00b501a3          	sb	a1,3(a0)
    1698:	4711                	li	a4,4
    169a:	00450e93          	addi	t4,a0,4
    169e:	14e78163          	beq	a5,a4,17e0 <memset+0x19a>
    16a2:	00b50223          	sb	a1,4(a0)
    16a6:	4715                	li	a4,5
    16a8:	00550e93          	addi	t4,a0,5
    16ac:	12e78c63          	beq	a5,a4,17e4 <memset+0x19e>
    16b0:	00b502a3          	sb	a1,5(a0)
    16b4:	471d                	li	a4,7
    16b6:	00650e93          	addi	t4,a0,6
    16ba:	12e79763          	bne	a5,a4,17e8 <memset+0x1a2>
    16be:	00750e93          	addi	t4,a0,7
    16c2:	00b50323          	sb	a1,6(a0)
    16c6:	4f1d                	li	t5,7
    16c8:	00859713          	slli	a4,a1,0x8
    16cc:	8f4d                	or	a4,a4,a1
    16ce:	01059e13          	slli	t3,a1,0x10
    16d2:	01c76e33          	or	t3,a4,t3
    16d6:	01859313          	slli	t1,a1,0x18
    16da:	006e6333          	or	t1,t3,t1
    16de:	02059893          	slli	a7,a1,0x20
    16e2:	011368b3          	or	a7,t1,a7
    16e6:	02859813          	slli	a6,a1,0x28
    16ea:	40f60333          	sub	t1,a2,a5
    16ee:	0108e833          	or	a6,a7,a6
    16f2:	03059693          	slli	a3,a1,0x30
    16f6:	00d866b3          	or	a3,a6,a3
    16fa:	03859713          	slli	a4,a1,0x38
    16fe:	97aa                	add	a5,a5,a0
    1700:	ff837813          	andi	a6,t1,-8
    1704:	8f55                	or	a4,a4,a3
    1706:	00f806b3          	add	a3,a6,a5
    170a:	e398                	sd	a4,0(a5)
    170c:	07a1                	addi	a5,a5,8
    170e:	fed79ee3          	bne	a5,a3,170a <memset+0xc4>
    1712:	ff837693          	andi	a3,t1,-8
    1716:	00de87b3          	add	a5,t4,a3
    171a:	01e6873b          	addw	a4,a3,t5
    171e:	0ad30663          	beq	t1,a3,17ca <memset+0x184>
    1722:	00b78023          	sb	a1,0(a5)
    1726:	0017069b          	addiw	a3,a4,1
    172a:	08c6fb63          	bgeu	a3,a2,17c0 <memset+0x17a>
    172e:	00b780a3          	sb	a1,1(a5)
    1732:	0027069b          	addiw	a3,a4,2
    1736:	08c6f563          	bgeu	a3,a2,17c0 <memset+0x17a>
    173a:	00b78123          	sb	a1,2(a5)
    173e:	0037069b          	addiw	a3,a4,3
    1742:	06c6ff63          	bgeu	a3,a2,17c0 <memset+0x17a>
    1746:	00b781a3          	sb	a1,3(a5)
    174a:	0047069b          	addiw	a3,a4,4
    174e:	06c6f963          	bgeu	a3,a2,17c0 <memset+0x17a>
    1752:	00b78223          	sb	a1,4(a5)
    1756:	0057069b          	addiw	a3,a4,5
    175a:	06c6f363          	bgeu	a3,a2,17c0 <memset+0x17a>
    175e:	00b782a3          	sb	a1,5(a5)
    1762:	0067069b          	addiw	a3,a4,6
    1766:	04c6fd63          	bgeu	a3,a2,17c0 <memset+0x17a>
    176a:	00b78323          	sb	a1,6(a5)
    176e:	0077069b          	addiw	a3,a4,7
    1772:	04c6f763          	bgeu	a3,a2,17c0 <memset+0x17a>
    1776:	00b783a3          	sb	a1,7(a5)
    177a:	0087069b          	addiw	a3,a4,8
    177e:	04c6f163          	bgeu	a3,a2,17c0 <memset+0x17a>
    1782:	00b78423          	sb	a1,8(a5)
    1786:	0097069b          	addiw	a3,a4,9
    178a:	02c6fb63          	bgeu	a3,a2,17c0 <memset+0x17a>
    178e:	00b784a3          	sb	a1,9(a5)
    1792:	00a7069b          	addiw	a3,a4,10
    1796:	02c6f563          	bgeu	a3,a2,17c0 <memset+0x17a>
    179a:	00b78523          	sb	a1,10(a5)
    179e:	00b7069b          	addiw	a3,a4,11
    17a2:	00c6ff63          	bgeu	a3,a2,17c0 <memset+0x17a>
    17a6:	00b785a3          	sb	a1,11(a5)
    17aa:	00c7069b          	addiw	a3,a4,12
    17ae:	00c6f963          	bgeu	a3,a2,17c0 <memset+0x17a>
    17b2:	00b78623          	sb	a1,12(a5)
    17b6:	2735                	addiw	a4,a4,13
    17b8:	00c77463          	bgeu	a4,a2,17c0 <memset+0x17a>
    17bc:	00b786a3          	sb	a1,13(a5)
        ;
    return dest;
}
    17c0:	8082                	ret
    17c2:	472d                	li	a4,11
    17c4:	bd79                	j	1662 <memset+0x1c>
    for (int i = 0; i < n; ++i, *(p++) = c)
    17c6:	4f0d                	li	t5,3
    17c8:	b701                	j	16c8 <memset+0x82>
    17ca:	8082                	ret
    17cc:	4f05                	li	t5,1
    17ce:	bded                	j	16c8 <memset+0x82>
    17d0:	8eaa                	mv	t4,a0
    17d2:	4f01                	li	t5,0
    17d4:	bdd5                	j	16c8 <memset+0x82>
    17d6:	87aa                	mv	a5,a0
    17d8:	4701                	li	a4,0
    17da:	b7a1                	j	1722 <memset+0xdc>
    17dc:	4f09                	li	t5,2
    17de:	b5ed                	j	16c8 <memset+0x82>
    17e0:	4f11                	li	t5,4
    17e2:	b5dd                	j	16c8 <memset+0x82>
    17e4:	4f15                	li	t5,5
    17e6:	b5cd                	j	16c8 <memset+0x82>
    17e8:	4f19                	li	t5,6
    17ea:	bdf9                	j	16c8 <memset+0x82>

00000000000017ec <strcmp>:

int strcmp(const char *l, const char *r)
{
    for (; *l == *r && *l; l++, r++)
    17ec:	00054783          	lbu	a5,0(a0)
    17f0:	0005c703          	lbu	a4,0(a1)
    17f4:	00e79863          	bne	a5,a4,1804 <strcmp+0x18>
    17f8:	0505                	addi	a0,a0,1
    17fa:	0585                	addi	a1,a1,1
    17fc:	fbe5                	bnez	a5,17ec <strcmp>
    17fe:	4501                	li	a0,0
        ;
    return *(unsigned char *)l - *(unsigned char *)r;
}
    1800:	9d19                	subw	a0,a0,a4
    1802:	8082                	ret
    return *(unsigned char *)l - *(unsigned char *)r;
    1804:	0007851b          	sext.w	a0,a5
    1808:	bfe5                	j	1800 <strcmp+0x14>

000000000000180a <strncmp>:

int strncmp(const char *_l, const char *_r, size_t n)
{
    const unsigned char *l = (void *)_l, *r = (void *)_r;
    if (!n--)
    180a:	ce05                	beqz	a2,1842 <strncmp+0x38>
        return 0;
    for (; *l && *r && n && *l == *r; l++, r++, n--)
    180c:	00054703          	lbu	a4,0(a0)
    1810:	0005c783          	lbu	a5,0(a1)
    1814:	cb0d                	beqz	a4,1846 <strncmp+0x3c>
    if (!n--)
    1816:	167d                	addi	a2,a2,-1
    1818:	00c506b3          	add	a3,a0,a2
    181c:	a819                	j	1832 <strncmp+0x28>
    for (; *l && *r && n && *l == *r; l++, r++, n--)
    181e:	00a68e63          	beq	a3,a0,183a <strncmp+0x30>
    1822:	0505                	addi	a0,a0,1
    1824:	00e79b63          	bne	a5,a4,183a <strncmp+0x30>
    1828:	00054703          	lbu	a4,0(a0)
        ;
    return *l - *r;
    182c:	0005c783          	lbu	a5,0(a1)
    for (; *l && *r && n && *l == *r; l++, r++, n--)
    1830:	cb19                	beqz	a4,1846 <strncmp+0x3c>
    1832:	0005c783          	lbu	a5,0(a1)
    1836:	0585                	addi	a1,a1,1
    1838:	f3fd                	bnez	a5,181e <strncmp+0x14>
    return *l - *r;
    183a:	0007051b          	sext.w	a0,a4
    183e:	9d1d                	subw	a0,a0,a5
    1840:	8082                	ret
        return 0;
    1842:	4501                	li	a0,0
}
    1844:	8082                	ret
    1846:	4501                	li	a0,0
    return *l - *r;
    1848:	9d1d                	subw	a0,a0,a5
    184a:	8082                	ret

000000000000184c <strlen>:
size_t strlen(const char *s)
{
    const char *a = s;
    typedef size_t __attribute__((__may_alias__)) word;
    const word *w;
    for (; (uintptr_t)s % SS; s++)
    184c:	00757793          	andi	a5,a0,7
    1850:	cf89                	beqz	a5,186a <strlen+0x1e>
    1852:	87aa                	mv	a5,a0
    1854:	a029                	j	185e <strlen+0x12>
    1856:	0785                	addi	a5,a5,1
    1858:	0077f713          	andi	a4,a5,7
    185c:	cb01                	beqz	a4,186c <strlen+0x20>
        if (!*s)
    185e:	0007c703          	lbu	a4,0(a5)
    1862:	fb75                	bnez	a4,1856 <strlen+0xa>
    for (w = (const void *)s; !HASZERO(*w); w++)
        ;
    s = (const void *)w;
    for (; *s; s++)
        ;
    return s - a;
    1864:	40a78533          	sub	a0,a5,a0
}
    1868:	8082                	ret
    for (; (uintptr_t)s % SS; s++)
    186a:	87aa                	mv	a5,a0
    for (w = (const void *)s; !HASZERO(*w); w++)
    186c:	6394                	ld	a3,0(a5)
    186e:	00000597          	auipc	a1,0x0
    1872:	6425b583          	ld	a1,1602(a1) # 1eb0 <__clone+0x4c>
    1876:	00000617          	auipc	a2,0x0
    187a:	64263603          	ld	a2,1602(a2) # 1eb8 <__clone+0x54>
    187e:	a019                	j	1884 <strlen+0x38>
    1880:	6794                	ld	a3,8(a5)
    1882:	07a1                	addi	a5,a5,8
    1884:	00b68733          	add	a4,a3,a1
    1888:	fff6c693          	not	a3,a3
    188c:	8f75                	and	a4,a4,a3
    188e:	8f71                	and	a4,a4,a2
    1890:	db65                	beqz	a4,1880 <strlen+0x34>
    for (; *s; s++)
    1892:	0007c703          	lbu	a4,0(a5)
    1896:	d779                	beqz	a4,1864 <strlen+0x18>
    1898:	0017c703          	lbu	a4,1(a5)
    189c:	0785                	addi	a5,a5,1
    189e:	d379                	beqz	a4,1864 <strlen+0x18>
    18a0:	0017c703          	lbu	a4,1(a5)
    18a4:	0785                	addi	a5,a5,1
    18a6:	fb6d                	bnez	a4,1898 <strlen+0x4c>
    18a8:	bf75                	j	1864 <strlen+0x18>

00000000000018aa <memchr>:

void *memchr(const void *src, int c, size_t n)
{
    const unsigned char *s = src;
    c = (unsigned char)c;
    for (; ((uintptr_t)s & ALIGN) && n && *s != c; s++, n--)
    18aa:	00757713          	andi	a4,a0,7
{
    18ae:	87aa                	mv	a5,a0
    c = (unsigned char)c;
    18b0:	0ff5f593          	andi	a1,a1,255
    for (; ((uintptr_t)s & ALIGN) && n && *s != c; s++, n--)
    18b4:	cb19                	beqz	a4,18ca <memchr+0x20>
    18b6:	ce25                	beqz	a2,192e <memchr+0x84>
    18b8:	0007c703          	lbu	a4,0(a5)
    18bc:	04b70e63          	beq	a4,a1,1918 <memchr+0x6e>
    18c0:	0785                	addi	a5,a5,1
    18c2:	0077f713          	andi	a4,a5,7
    18c6:	167d                	addi	a2,a2,-1
    18c8:	f77d                	bnez	a4,18b6 <memchr+0xc>
            ;
        s = (const void *)w;
    }
    for (; n && *s != c; s++, n--)
        ;
    return n ? (void *)s : 0;
    18ca:	4501                	li	a0,0
    if (n && *s != c)
    18cc:	c235                	beqz	a2,1930 <memchr+0x86>
    18ce:	0007c703          	lbu	a4,0(a5)
    18d2:	04b70363          	beq	a4,a1,1918 <memchr+0x6e>
        size_t k = ONES * c;
    18d6:	00000517          	auipc	a0,0x0
    18da:	5ea53503          	ld	a0,1514(a0) # 1ec0 <__clone+0x5c>
        for (w = (const void *)s; n >= SS && !HASZERO(*w ^ k); w++, n -= SS)
    18de:	471d                	li	a4,7
        size_t k = ONES * c;
    18e0:	02a58533          	mul	a0,a1,a0
        for (w = (const void *)s; n >= SS && !HASZERO(*w ^ k); w++, n -= SS)
    18e4:	02c77a63          	bgeu	a4,a2,1918 <memchr+0x6e>
    18e8:	00000897          	auipc	a7,0x0
    18ec:	5c88b883          	ld	a7,1480(a7) # 1eb0 <__clone+0x4c>
    18f0:	00000817          	auipc	a6,0x0
    18f4:	5c883803          	ld	a6,1480(a6) # 1eb8 <__clone+0x54>
    18f8:	431d                	li	t1,7
    18fa:	a029                	j	1904 <memchr+0x5a>
    18fc:	1661                	addi	a2,a2,-8
    18fe:	07a1                	addi	a5,a5,8
    1900:	02c37963          	bgeu	t1,a2,1932 <memchr+0x88>
    1904:	6398                	ld	a4,0(a5)
    1906:	8f29                	xor	a4,a4,a0
    1908:	011706b3          	add	a3,a4,a7
    190c:	fff74713          	not	a4,a4
    1910:	8f75                	and	a4,a4,a3
    1912:	01077733          	and	a4,a4,a6
    1916:	d37d                	beqz	a4,18fc <memchr+0x52>
    1918:	853e                	mv	a0,a5
    191a:	97b2                	add	a5,a5,a2
    191c:	a021                	j	1924 <memchr+0x7a>
    for (; n && *s != c; s++, n--)
    191e:	0505                	addi	a0,a0,1
    1920:	00f50763          	beq	a0,a5,192e <memchr+0x84>
    1924:	00054703          	lbu	a4,0(a0)
    1928:	feb71be3          	bne	a4,a1,191e <memchr+0x74>
    192c:	8082                	ret
    return n ? (void *)s : 0;
    192e:	4501                	li	a0,0
}
    1930:	8082                	ret
    return n ? (void *)s : 0;
    1932:	4501                	li	a0,0
    for (; n && *s != c; s++, n--)
    1934:	f275                	bnez	a2,1918 <memchr+0x6e>
}
    1936:	8082                	ret

0000000000001938 <strnlen>:

size_t strnlen(const char *s, size_t n)
{
    1938:	1101                	addi	sp,sp,-32
    193a:	e822                	sd	s0,16(sp)
    const char *p = memchr(s, 0, n);
    193c:	862e                	mv	a2,a1
{
    193e:	842e                	mv	s0,a1
    const char *p = memchr(s, 0, n);
    1940:	4581                	li	a1,0
{
    1942:	e426                	sd	s1,8(sp)
    1944:	ec06                	sd	ra,24(sp)
    1946:	84aa                	mv	s1,a0
    const char *p = memchr(s, 0, n);
    1948:	f63ff0ef          	jal	ra,18aa <memchr>
    return p ? p - s : n;
    194c:	c519                	beqz	a0,195a <strnlen+0x22>
}
    194e:	60e2                	ld	ra,24(sp)
    1950:	6442                	ld	s0,16(sp)
    return p ? p - s : n;
    1952:	8d05                	sub	a0,a0,s1
}
    1954:	64a2                	ld	s1,8(sp)
    1956:	6105                	addi	sp,sp,32
    1958:	8082                	ret
    195a:	60e2                	ld	ra,24(sp)
    return p ? p - s : n;
    195c:	8522                	mv	a0,s0
}
    195e:	6442                	ld	s0,16(sp)
    1960:	64a2                	ld	s1,8(sp)
    1962:	6105                	addi	sp,sp,32
    1964:	8082                	ret

0000000000001966 <strcpy>:
char *strcpy(char *restrict d, const char *s)
{
    typedef size_t __attribute__((__may_alias__)) word;
    word *wd;
    const word *ws;
    if ((uintptr_t)s % SS == (uintptr_t)d % SS)
    1966:	00b547b3          	xor	a5,a0,a1
    196a:	8b9d                	andi	a5,a5,7
    196c:	eb95                	bnez	a5,19a0 <strcpy+0x3a>
    {
        for (; (uintptr_t)s % SS; s++, d++)
    196e:	0075f793          	andi	a5,a1,7
    1972:	e7b1                	bnez	a5,19be <strcpy+0x58>
            if (!(*d = *s))
                return d;
        wd = (void *)d;
        ws = (const void *)s;
        for (; !HASZERO(*ws); *wd++ = *ws++)
    1974:	6198                	ld	a4,0(a1)
    1976:	00000617          	auipc	a2,0x0
    197a:	53a63603          	ld	a2,1338(a2) # 1eb0 <__clone+0x4c>
    197e:	00000817          	auipc	a6,0x0
    1982:	53a83803          	ld	a6,1338(a6) # 1eb8 <__clone+0x54>
    1986:	a029                	j	1990 <strcpy+0x2a>
    1988:	e118                	sd	a4,0(a0)
    198a:	6598                	ld	a4,8(a1)
    198c:	05a1                	addi	a1,a1,8
    198e:	0521                	addi	a0,a0,8
    1990:	00c707b3          	add	a5,a4,a2
    1994:	fff74693          	not	a3,a4
    1998:	8ff5                	and	a5,a5,a3
    199a:	0107f7b3          	and	a5,a5,a6
    199e:	d7ed                	beqz	a5,1988 <strcpy+0x22>
            ;
        d = (void *)wd;
        s = (const void *)ws;
    }
    for (; (*d = *s); s++, d++)
    19a0:	0005c783          	lbu	a5,0(a1)
    19a4:	00f50023          	sb	a5,0(a0)
    19a8:	c785                	beqz	a5,19d0 <strcpy+0x6a>
    19aa:	0015c783          	lbu	a5,1(a1)
    19ae:	0505                	addi	a0,a0,1
    19b0:	0585                	addi	a1,a1,1
    19b2:	00f50023          	sb	a5,0(a0)
    19b6:	fbf5                	bnez	a5,19aa <strcpy+0x44>
        ;
    return d;
}
    19b8:	8082                	ret
        for (; (uintptr_t)s % SS; s++, d++)
    19ba:	0505                	addi	a0,a0,1
    19bc:	df45                	beqz	a4,1974 <strcpy+0xe>
            if (!(*d = *s))
    19be:	0005c783          	lbu	a5,0(a1)
        for (; (uintptr_t)s % SS; s++, d++)
    19c2:	0585                	addi	a1,a1,1
    19c4:	0075f713          	andi	a4,a1,7
            if (!(*d = *s))
    19c8:	00f50023          	sb	a5,0(a0)
    19cc:	f7fd                	bnez	a5,19ba <strcpy+0x54>
}
    19ce:	8082                	ret
    19d0:	8082                	ret

00000000000019d2 <strncpy>:
char *strncpy(char *restrict d, const char *s, size_t n)
{
    typedef size_t __attribute__((__may_alias__)) word;
    word *wd;
    const word *ws;
    if (((uintptr_t)s & ALIGN) == ((uintptr_t)d & ALIGN))
    19d2:	00b547b3          	xor	a5,a0,a1
    19d6:	8b9d                	andi	a5,a5,7
    19d8:	1a079863          	bnez	a5,1b88 <strncpy+0x1b6>
    {
        for (; ((uintptr_t)s & ALIGN) && n && (*d = *s); n--, s++, d++)
    19dc:	0075f793          	andi	a5,a1,7
    19e0:	16078463          	beqz	a5,1b48 <strncpy+0x176>
    19e4:	ea01                	bnez	a2,19f4 <strncpy+0x22>
    19e6:	a421                	j	1bee <strncpy+0x21c>
    19e8:	167d                	addi	a2,a2,-1
    19ea:	0505                	addi	a0,a0,1
    19ec:	14070e63          	beqz	a4,1b48 <strncpy+0x176>
    19f0:	1a060863          	beqz	a2,1ba0 <strncpy+0x1ce>
    19f4:	0005c783          	lbu	a5,0(a1)
    19f8:	0585                	addi	a1,a1,1
    19fa:	0075f713          	andi	a4,a1,7
    19fe:	00f50023          	sb	a5,0(a0)
    1a02:	f3fd                	bnez	a5,19e8 <strncpy+0x16>
    1a04:	4805                	li	a6,1
    1a06:	1a061863          	bnez	a2,1bb6 <strncpy+0x1e4>
    1a0a:	40a007b3          	neg	a5,a0
    1a0e:	8b9d                	andi	a5,a5,7
    1a10:	4681                	li	a3,0
    1a12:	18061a63          	bnez	a2,1ba6 <strncpy+0x1d4>
    1a16:	00778713          	addi	a4,a5,7
    1a1a:	45ad                	li	a1,11
    1a1c:	18b76363          	bltu	a4,a1,1ba2 <strncpy+0x1d0>
    1a20:	1ae6eb63          	bltu	a3,a4,1bd6 <strncpy+0x204>
    1a24:	1a078363          	beqz	a5,1bca <strncpy+0x1f8>
    for (int i = 0; i < n; ++i, *(p++) = c)
    1a28:	00050023          	sb	zero,0(a0)
    1a2c:	4685                	li	a3,1
    1a2e:	00150713          	addi	a4,a0,1
    1a32:	18d78f63          	beq	a5,a3,1bd0 <strncpy+0x1fe>
    1a36:	000500a3          	sb	zero,1(a0)
    1a3a:	4689                	li	a3,2
    1a3c:	00250713          	addi	a4,a0,2
    1a40:	18d78e63          	beq	a5,a3,1bdc <strncpy+0x20a>
    1a44:	00050123          	sb	zero,2(a0)
    1a48:	468d                	li	a3,3
    1a4a:	00350713          	addi	a4,a0,3
    1a4e:	16d78c63          	beq	a5,a3,1bc6 <strncpy+0x1f4>
    1a52:	000501a3          	sb	zero,3(a0)
    1a56:	4691                	li	a3,4
    1a58:	00450713          	addi	a4,a0,4
    1a5c:	18d78263          	beq	a5,a3,1be0 <strncpy+0x20e>
    1a60:	00050223          	sb	zero,4(a0)
    1a64:	4695                	li	a3,5
    1a66:	00550713          	addi	a4,a0,5
    1a6a:	16d78d63          	beq	a5,a3,1be4 <strncpy+0x212>
    1a6e:	000502a3          	sb	zero,5(a0)
    1a72:	469d                	li	a3,7
    1a74:	00650713          	addi	a4,a0,6
    1a78:	16d79863          	bne	a5,a3,1be8 <strncpy+0x216>
    1a7c:	00750713          	addi	a4,a0,7
    1a80:	00050323          	sb	zero,6(a0)
    1a84:	40f80833          	sub	a6,a6,a5
    1a88:	ff887593          	andi	a1,a6,-8
    1a8c:	97aa                	add	a5,a5,a0
    1a8e:	95be                	add	a1,a1,a5
    1a90:	0007b023          	sd	zero,0(a5)
    1a94:	07a1                	addi	a5,a5,8
    1a96:	feb79de3          	bne	a5,a1,1a90 <strncpy+0xbe>
    1a9a:	ff887593          	andi	a1,a6,-8
    1a9e:	9ead                	addw	a3,a3,a1
    1aa0:	00b707b3          	add	a5,a4,a1
    1aa4:	12b80863          	beq	a6,a1,1bd4 <strncpy+0x202>
    1aa8:	00078023          	sb	zero,0(a5)
    1aac:	0016871b          	addiw	a4,a3,1
    1ab0:	0ec77863          	bgeu	a4,a2,1ba0 <strncpy+0x1ce>
    1ab4:	000780a3          	sb	zero,1(a5)
    1ab8:	0026871b          	addiw	a4,a3,2
    1abc:	0ec77263          	bgeu	a4,a2,1ba0 <strncpy+0x1ce>
    1ac0:	00078123          	sb	zero,2(a5)
    1ac4:	0036871b          	addiw	a4,a3,3
    1ac8:	0cc77c63          	bgeu	a4,a2,1ba0 <strncpy+0x1ce>
    1acc:	000781a3          	sb	zero,3(a5)
    1ad0:	0046871b          	addiw	a4,a3,4
    1ad4:	0cc77663          	bgeu	a4,a2,1ba0 <strncpy+0x1ce>
    1ad8:	00078223          	sb	zero,4(a5)
    1adc:	0056871b          	addiw	a4,a3,5
    1ae0:	0cc77063          	bgeu	a4,a2,1ba0 <strncpy+0x1ce>
    1ae4:	000782a3          	sb	zero,5(a5)
    1ae8:	0066871b          	addiw	a4,a3,6
    1aec:	0ac77a63          	bgeu	a4,a2,1ba0 <strncpy+0x1ce>
    1af0:	00078323          	sb	zero,6(a5)
    1af4:	0076871b          	addiw	a4,a3,7
    1af8:	0ac77463          	bgeu	a4,a2,1ba0 <strncpy+0x1ce>
    1afc:	000783a3          	sb	zero,7(a5)
    1b00:	0086871b          	addiw	a4,a3,8
    1b04:	08c77e63          	bgeu	a4,a2,1ba0 <strncpy+0x1ce>
    1b08:	00078423          	sb	zero,8(a5)
    1b0c:	0096871b          	addiw	a4,a3,9
    1b10:	08c77863          	bgeu	a4,a2,1ba0 <strncpy+0x1ce>
    1b14:	000784a3          	sb	zero,9(a5)
    1b18:	00a6871b          	addiw	a4,a3,10
    1b1c:	08c77263          	bgeu	a4,a2,1ba0 <strncpy+0x1ce>
    1b20:	00078523          	sb	zero,10(a5)
    1b24:	00b6871b          	addiw	a4,a3,11
    1b28:	06c77c63          	bgeu	a4,a2,1ba0 <strncpy+0x1ce>
    1b2c:	000785a3          	sb	zero,11(a5)
    1b30:	00c6871b          	addiw	a4,a3,12
    1b34:	06c77663          	bgeu	a4,a2,1ba0 <strncpy+0x1ce>
    1b38:	00078623          	sb	zero,12(a5)
    1b3c:	26b5                	addiw	a3,a3,13
    1b3e:	06c6f163          	bgeu	a3,a2,1ba0 <strncpy+0x1ce>
    1b42:	000786a3          	sb	zero,13(a5)
    1b46:	8082                	ret
            ;
        if (!n || !*s)
    1b48:	c645                	beqz	a2,1bf0 <strncpy+0x21e>
    1b4a:	0005c783          	lbu	a5,0(a1)
    1b4e:	ea078be3          	beqz	a5,1a04 <strncpy+0x32>
            goto tail;
        wd = (void *)d;
        ws = (const void *)s;
        for (; n >= sizeof(size_t) && !HASZERO(*ws); n -= sizeof(size_t), ws++, wd++)
    1b52:	479d                	li	a5,7
    1b54:	02c7ff63          	bgeu	a5,a2,1b92 <strncpy+0x1c0>
    1b58:	00000897          	auipc	a7,0x0
    1b5c:	3588b883          	ld	a7,856(a7) # 1eb0 <__clone+0x4c>
    1b60:	00000817          	auipc	a6,0x0
    1b64:	35883803          	ld	a6,856(a6) # 1eb8 <__clone+0x54>
    1b68:	431d                	li	t1,7
    1b6a:	6198                	ld	a4,0(a1)
    1b6c:	011707b3          	add	a5,a4,a7
    1b70:	fff74693          	not	a3,a4
    1b74:	8ff5                	and	a5,a5,a3
    1b76:	0107f7b3          	and	a5,a5,a6
    1b7a:	ef81                	bnez	a5,1b92 <strncpy+0x1c0>
            *wd = *ws;
    1b7c:	e118                	sd	a4,0(a0)
        for (; n >= sizeof(size_t) && !HASZERO(*ws); n -= sizeof(size_t), ws++, wd++)
    1b7e:	1661                	addi	a2,a2,-8
    1b80:	05a1                	addi	a1,a1,8
    1b82:	0521                	addi	a0,a0,8
    1b84:	fec363e3          	bltu	t1,a2,1b6a <strncpy+0x198>
        d = (void *)wd;
        s = (const void *)ws;
    }
    for (; n && (*d = *s); n--, s++, d++)
    1b88:	e609                	bnez	a2,1b92 <strncpy+0x1c0>
    1b8a:	a08d                	j	1bec <strncpy+0x21a>
    1b8c:	167d                	addi	a2,a2,-1
    1b8e:	0505                	addi	a0,a0,1
    1b90:	ca01                	beqz	a2,1ba0 <strncpy+0x1ce>
    1b92:	0005c783          	lbu	a5,0(a1)
    1b96:	0585                	addi	a1,a1,1
    1b98:	00f50023          	sb	a5,0(a0)
    1b9c:	fbe5                	bnez	a5,1b8c <strncpy+0x1ba>
        ;
tail:
    1b9e:	b59d                	j	1a04 <strncpy+0x32>
    memset(d, 0, n);
    return d;
}
    1ba0:	8082                	ret
    1ba2:	472d                	li	a4,11
    1ba4:	bdb5                	j	1a20 <strncpy+0x4e>
    1ba6:	00778713          	addi	a4,a5,7
    1baa:	45ad                	li	a1,11
    1bac:	fff60693          	addi	a3,a2,-1
    1bb0:	e6b778e3          	bgeu	a4,a1,1a20 <strncpy+0x4e>
    1bb4:	b7fd                	j	1ba2 <strncpy+0x1d0>
    1bb6:	40a007b3          	neg	a5,a0
    1bba:	8832                	mv	a6,a2
    1bbc:	8b9d                	andi	a5,a5,7
    1bbe:	4681                	li	a3,0
    1bc0:	e4060be3          	beqz	a2,1a16 <strncpy+0x44>
    1bc4:	b7cd                	j	1ba6 <strncpy+0x1d4>
    for (int i = 0; i < n; ++i, *(p++) = c)
    1bc6:	468d                	li	a3,3
    1bc8:	bd75                	j	1a84 <strncpy+0xb2>
    1bca:	872a                	mv	a4,a0
    1bcc:	4681                	li	a3,0
    1bce:	bd5d                	j	1a84 <strncpy+0xb2>
    1bd0:	4685                	li	a3,1
    1bd2:	bd4d                	j	1a84 <strncpy+0xb2>
    1bd4:	8082                	ret
    1bd6:	87aa                	mv	a5,a0
    1bd8:	4681                	li	a3,0
    1bda:	b5f9                	j	1aa8 <strncpy+0xd6>
    1bdc:	4689                	li	a3,2
    1bde:	b55d                	j	1a84 <strncpy+0xb2>
    1be0:	4691                	li	a3,4
    1be2:	b54d                	j	1a84 <strncpy+0xb2>
    1be4:	4695                	li	a3,5
    1be6:	bd79                	j	1a84 <strncpy+0xb2>
    1be8:	4699                	li	a3,6
    1bea:	bd69                	j	1a84 <strncpy+0xb2>
    1bec:	8082                	ret
    1bee:	8082                	ret
    1bf0:	8082                	ret

0000000000001bf2 <open>:
#include <unistd.h>

#include "syscall.h"

int open(const char *path, int flags)
{
    1bf2:	87aa                	mv	a5,a0
    1bf4:	862e                	mv	a2,a1
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
}

static inline long __syscall4(long n, long a, long b, long c, long d)
{
    register long a7 __asm__("a7") = n;
    1bf6:	03800893          	li	a7,56
    register long a0 __asm__("a0") = a;
    1bfa:	f9c00513          	li	a0,-100
    register long a1 __asm__("a1") = b;
    1bfe:	85be                	mv	a1,a5
    register long a2 __asm__("a2") = c;
    register long a3 __asm__("a3") = d;
    1c00:	4689                	li	a3,2
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3))
    1c02:	00000073          	ecall
    return syscall(SYS_openat, AT_FDCWD, path, flags, O_RDWR);
}
    1c06:	2501                	sext.w	a0,a0
    1c08:	8082                	ret

0000000000001c0a <openat>:
    register long a7 __asm__("a7") = n;
    1c0a:	03800893          	li	a7,56
    register long a3 __asm__("a3") = d;
    1c0e:	18000693          	li	a3,384
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3))
    1c12:	00000073          	ecall

int openat(int dirfd,const char *path, int flags)
{
    return syscall(SYS_openat, dirfd, path, flags, 0600);
}
    1c16:	2501                	sext.w	a0,a0
    1c18:	8082                	ret

0000000000001c1a <close>:
    register long a7 __asm__("a7") = n;
    1c1a:	03900893          	li	a7,57
    __asm_syscall("r"(a7), "0"(a0))
    1c1e:	00000073          	ecall

int close(int fd)
{
    return syscall(SYS_close, fd);
}
    1c22:	2501                	sext.w	a0,a0
    1c24:	8082                	ret

0000000000001c26 <read>:
    register long a7 __asm__("a7") = n;
    1c26:	03f00893          	li	a7,63
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1c2a:	00000073          	ecall

ssize_t read(int fd, void *buf, size_t len)
{
    return syscall(SYS_read, fd, buf, len);
}
    1c2e:	8082                	ret

0000000000001c30 <write>:
    register long a7 __asm__("a7") = n;
    1c30:	04000893          	li	a7,64
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1c34:	00000073          	ecall

ssize_t write(int fd, const void *buf, size_t len)
{
    return syscall(SYS_write, fd, buf, len);
}
    1c38:	8082                	ret

0000000000001c3a <getpid>:
    register long a7 __asm__("a7") = n;
    1c3a:	0ac00893          	li	a7,172
    __asm_syscall("r"(a7))
    1c3e:	00000073          	ecall

pid_t getpid(void)
{
    return syscall(SYS_getpid);
}
    1c42:	2501                	sext.w	a0,a0
    1c44:	8082                	ret

0000000000001c46 <getppid>:
    register long a7 __asm__("a7") = n;
    1c46:	0ad00893          	li	a7,173
    __asm_syscall("r"(a7))
    1c4a:	00000073          	ecall

pid_t getppid(void)
{
    return syscall(SYS_getppid);
}
    1c4e:	2501                	sext.w	a0,a0
    1c50:	8082                	ret

0000000000001c52 <sched_yield>:
    register long a7 __asm__("a7") = n;
    1c52:	07c00893          	li	a7,124
    __asm_syscall("r"(a7))
    1c56:	00000073          	ecall

int sched_yield(void)
{
    return syscall(SYS_sched_yield);
}
    1c5a:	2501                	sext.w	a0,a0
    1c5c:	8082                	ret

0000000000001c5e <fork>:
    register long a7 __asm__("a7") = n;
    1c5e:	0dc00893          	li	a7,220
    register long a0 __asm__("a0") = a;
    1c62:	4545                	li	a0,17
    register long a1 __asm__("a1") = b;
    1c64:	4581                	li	a1,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1c66:	00000073          	ecall

pid_t fork(void)
{
    return syscall(SYS_clone, SIGCHLD, 0);
}
    1c6a:	2501                	sext.w	a0,a0
    1c6c:	8082                	ret

0000000000001c6e <clone>:

pid_t clone(int (*fn)(void *arg), void *arg, void *stack, size_t stack_size, unsigned long flags)
{
    1c6e:	85b2                	mv	a1,a2
    1c70:	863a                	mv	a2,a4
    if (stack)
    1c72:	c191                	beqz	a1,1c76 <clone+0x8>
	stack += stack_size;
    1c74:	95b6                	add	a1,a1,a3

    return __clone(fn, stack, flags, NULL, NULL, NULL);
    1c76:	4781                	li	a5,0
    1c78:	4701                	li	a4,0
    1c7a:	4681                	li	a3,0
    1c7c:	2601                	sext.w	a2,a2
    1c7e:	a2dd                	j	1e64 <__clone>

0000000000001c80 <exit>:
    register long a7 __asm__("a7") = n;
    1c80:	05d00893          	li	a7,93
    __asm_syscall("r"(a7), "0"(a0))
    1c84:	00000073          	ecall
    //return syscall(SYS_clone, fn, stack, flags, NULL, NULL, NULL);
}
void exit(int code)
{
    syscall(SYS_exit, code);
}
    1c88:	8082                	ret

0000000000001c8a <waitpid>:
    register long a7 __asm__("a7") = n;
    1c8a:	10400893          	li	a7,260
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1c8e:	00000073          	ecall

int waitpid(int pid, int *code, int options)
{
    return syscall(SYS_wait4, pid, code, options);
}
    1c92:	2501                	sext.w	a0,a0
    1c94:	8082                	ret

0000000000001c96 <exec>:
    register long a7 __asm__("a7") = n;
    1c96:	0dd00893          	li	a7,221
    __asm_syscall("r"(a7), "0"(a0))
    1c9a:	00000073          	ecall

int exec(char *name)
{
    return syscall(SYS_execve, name);
}
    1c9e:	2501                	sext.w	a0,a0
    1ca0:	8082                	ret

0000000000001ca2 <execve>:
    register long a7 __asm__("a7") = n;
    1ca2:	0dd00893          	li	a7,221
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1ca6:	00000073          	ecall

int execve(const char *name, char *const argv[], char *const argp[])
{
    return syscall(SYS_execve, name, argv, argp);
}
    1caa:	2501                	sext.w	a0,a0
    1cac:	8082                	ret

0000000000001cae <times>:
    register long a7 __asm__("a7") = n;
    1cae:	09900893          	li	a7,153
    __asm_syscall("r"(a7), "0"(a0))
    1cb2:	00000073          	ecall

int times(void *mytimes)
{
	return syscall(SYS_times, mytimes);
}
    1cb6:	2501                	sext.w	a0,a0
    1cb8:	8082                	ret

0000000000001cba <get_time>:

int64 get_time()
{
    1cba:	1141                	addi	sp,sp,-16
    register long a7 __asm__("a7") = n;
    1cbc:	0a900893          	li	a7,169
    register long a0 __asm__("a0") = a;
    1cc0:	850a                	mv	a0,sp
    register long a1 __asm__("a1") = b;
    1cc2:	4581                	li	a1,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1cc4:	00000073          	ecall
    TimeVal time;
    int err = sys_get_time(&time, 0);
    if (err == 0)
    1cc8:	2501                	sext.w	a0,a0
    1cca:	ed09                	bnez	a0,1ce4 <get_time+0x2a>
    {
        return ((time.sec & 0xffff) * 1000 + time.usec / 1000);
    1ccc:	67a2                	ld	a5,8(sp)
    1cce:	3e800713          	li	a4,1000
    1cd2:	00015503          	lhu	a0,0(sp)
    1cd6:	02e7d7b3          	divu	a5,a5,a4
    1cda:	02e50533          	mul	a0,a0,a4
    1cde:	953e                	add	a0,a0,a5
    }
    else
    {
        return -1;
    }
}
    1ce0:	0141                	addi	sp,sp,16
    1ce2:	8082                	ret
        return -1;
    1ce4:	557d                	li	a0,-1
    1ce6:	bfed                	j	1ce0 <get_time+0x26>

0000000000001ce8 <sys_get_time>:
    register long a7 __asm__("a7") = n;
    1ce8:	0a900893          	li	a7,169
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1cec:	00000073          	ecall

int sys_get_time(TimeVal *ts, int tz)
{
    return syscall(SYS_gettimeofday, ts, tz);
}
    1cf0:	2501                	sext.w	a0,a0
    1cf2:	8082                	ret

0000000000001cf4 <time>:
    register long a7 __asm__("a7") = n;
    1cf4:	42600893          	li	a7,1062
    __asm_syscall("r"(a7), "0"(a0))
    1cf8:	00000073          	ecall

int time(unsigned long *tloc)
{
    return syscall(SYS_time, tloc);
}
    1cfc:	2501                	sext.w	a0,a0
    1cfe:	8082                	ret

0000000000001d00 <sleep>:

int sleep(unsigned long long time)
{
    1d00:	1141                	addi	sp,sp,-16
    TimeVal tv = {.sec = time, .usec = 0};
    1d02:	e02a                	sd	a0,0(sp)
    register long a0 __asm__("a0") = a;
    1d04:	850a                	mv	a0,sp
    1d06:	e402                	sd	zero,8(sp)
    register long a7 __asm__("a7") = n;
    1d08:	06500893          	li	a7,101
    register long a1 __asm__("a1") = b;
    1d0c:	85aa                	mv	a1,a0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1d0e:	00000073          	ecall
    if (syscall(SYS_nanosleep, &tv, &tv)) return tv.sec;
    1d12:	e501                	bnez	a0,1d1a <sleep+0x1a>
    return 0;
    1d14:	4501                	li	a0,0
}
    1d16:	0141                	addi	sp,sp,16
    1d18:	8082                	ret
    if (syscall(SYS_nanosleep, &tv, &tv)) return tv.sec;
    1d1a:	4502                	lw	a0,0(sp)
}
    1d1c:	0141                	addi	sp,sp,16
    1d1e:	8082                	ret

0000000000001d20 <set_priority>:
    register long a7 __asm__("a7") = n;
    1d20:	08c00893          	li	a7,140
    __asm_syscall("r"(a7), "0"(a0))
    1d24:	00000073          	ecall

int set_priority(int prio)
{
    return syscall(SYS_setpriority, prio);
}
    1d28:	2501                	sext.w	a0,a0
    1d2a:	8082                	ret

0000000000001d2c <mmap>:
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4))
}

static inline long __syscall6(long n, long a, long b, long c, long d, long e, long f)
{
    register long a7 __asm__("a7") = n;
    1d2c:	0de00893          	li	a7,222
    register long a1 __asm__("a1") = b;
    register long a2 __asm__("a2") = c;
    register long a3 __asm__("a3") = d;
    register long a4 __asm__("a4") = e;
    register long a5 __asm__("a5") = f;
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4), "r"(a5))
    1d30:	00000073          	ecall

void *mmap(void *start, size_t len, int prot, int flags, int fd, off_t off)
{
    return syscall(SYS_mmap, start, len, prot, flags, fd, off);
}
    1d34:	8082                	ret

0000000000001d36 <munmap>:
    register long a7 __asm__("a7") = n;
    1d36:	0d700893          	li	a7,215
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1d3a:	00000073          	ecall

int munmap(void *start, size_t len)
{
    return syscall(SYS_munmap, start, len);
}
    1d3e:	2501                	sext.w	a0,a0
    1d40:	8082                	ret

0000000000001d42 <wait>:

int wait(int *code)
{
    1d42:	85aa                	mv	a1,a0
    register long a7 __asm__("a7") = n;
    1d44:	10400893          	li	a7,260
    register long a0 __asm__("a0") = a;
    1d48:	557d                	li	a0,-1
    register long a2 __asm__("a2") = c;
    1d4a:	4601                	li	a2,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1d4c:	00000073          	ecall
    return waitpid(-1, code, 0);
}
    1d50:	2501                	sext.w	a0,a0
    1d52:	8082                	ret

0000000000001d54 <spawn>:
    register long a7 __asm__("a7") = n;
    1d54:	19000893          	li	a7,400
    __asm_syscall("r"(a7), "0"(a0))
    1d58:	00000073          	ecall

int spawn(char *file)
{
    return syscall(SYS_spawn, file);
}
    1d5c:	2501                	sext.w	a0,a0
    1d5e:	8082                	ret

0000000000001d60 <mailread>:
    register long a7 __asm__("a7") = n;
    1d60:	19100893          	li	a7,401
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1d64:	00000073          	ecall

int mailread(void *buf, int len)
{
    return syscall(SYS_mailread, buf, len);
}
    1d68:	2501                	sext.w	a0,a0
    1d6a:	8082                	ret

0000000000001d6c <mailwrite>:
    register long a7 __asm__("a7") = n;
    1d6c:	19200893          	li	a7,402
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1d70:	00000073          	ecall

int mailwrite(int pid, void *buf, int len)
{
    return syscall(SYS_mailwrite, pid, buf, len);
}
    1d74:	2501                	sext.w	a0,a0
    1d76:	8082                	ret

0000000000001d78 <fstat>:
    register long a7 __asm__("a7") = n;
    1d78:	05000893          	li	a7,80
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1d7c:	00000073          	ecall

int fstat(int fd, struct kstat *st)
{
    return syscall(SYS_fstat, fd, st);
}
    1d80:	2501                	sext.w	a0,a0
    1d82:	8082                	ret

0000000000001d84 <sys_linkat>:
    register long a4 __asm__("a4") = e;
    1d84:	1702                	slli	a4,a4,0x20
    register long a7 __asm__("a7") = n;
    1d86:	02500893          	li	a7,37
    register long a4 __asm__("a4") = e;
    1d8a:	9301                	srli	a4,a4,0x20
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4))
    1d8c:	00000073          	ecall

int sys_linkat(int olddirfd, char *oldpath, int newdirfd, char *newpath, unsigned int flags)
{
    return syscall(SYS_linkat, olddirfd, oldpath, newdirfd, newpath, flags);
}
    1d90:	2501                	sext.w	a0,a0
    1d92:	8082                	ret

0000000000001d94 <sys_unlinkat>:
    register long a2 __asm__("a2") = c;
    1d94:	1602                	slli	a2,a2,0x20
    register long a7 __asm__("a7") = n;
    1d96:	02300893          	li	a7,35
    register long a2 __asm__("a2") = c;
    1d9a:	9201                	srli	a2,a2,0x20
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1d9c:	00000073          	ecall

int sys_unlinkat(int dirfd, char *path, unsigned int flags)
{
    return syscall(SYS_unlinkat, dirfd, path, flags);
}
    1da0:	2501                	sext.w	a0,a0
    1da2:	8082                	ret

0000000000001da4 <link>:

int link(char *old_path, char *new_path)
{
    1da4:	87aa                	mv	a5,a0
    1da6:	86ae                	mv	a3,a1
    register long a7 __asm__("a7") = n;
    1da8:	02500893          	li	a7,37
    register long a0 __asm__("a0") = a;
    1dac:	f9c00513          	li	a0,-100
    register long a1 __asm__("a1") = b;
    1db0:	85be                	mv	a1,a5
    register long a2 __asm__("a2") = c;
    1db2:	f9c00613          	li	a2,-100
    register long a4 __asm__("a4") = e;
    1db6:	4701                	li	a4,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4))
    1db8:	00000073          	ecall
    return sys_linkat(AT_FDCWD, old_path, AT_FDCWD, new_path, 0);
}
    1dbc:	2501                	sext.w	a0,a0
    1dbe:	8082                	ret

0000000000001dc0 <unlink>:

int unlink(char *path)
{
    1dc0:	85aa                	mv	a1,a0
    register long a7 __asm__("a7") = n;
    1dc2:	02300893          	li	a7,35
    register long a0 __asm__("a0") = a;
    1dc6:	f9c00513          	li	a0,-100
    register long a2 __asm__("a2") = c;
    1dca:	4601                	li	a2,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1dcc:	00000073          	ecall
    return sys_unlinkat(AT_FDCWD, path, 0);
}
    1dd0:	2501                	sext.w	a0,a0
    1dd2:	8082                	ret

0000000000001dd4 <uname>:
    register long a7 __asm__("a7") = n;
    1dd4:	0a000893          	li	a7,160
    __asm_syscall("r"(a7), "0"(a0))
    1dd8:	00000073          	ecall

int uname(void *buf)
{
    return syscall(SYS_uname, buf);
}
    1ddc:	2501                	sext.w	a0,a0
    1dde:	8082                	ret

0000000000001de0 <brk>:
    register long a7 __asm__("a7") = n;
    1de0:	0d600893          	li	a7,214
    __asm_syscall("r"(a7), "0"(a0))
    1de4:	00000073          	ecall

int brk(void *addr)
{
    return syscall(SYS_brk, addr);
}
    1de8:	2501                	sext.w	a0,a0
    1dea:	8082                	ret

0000000000001dec <getcwd>:
    register long a7 __asm__("a7") = n;
    1dec:	48c5                	li	a7,17
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1dee:	00000073          	ecall

char *getcwd(char *buf, size_t size){
    return syscall(SYS_getcwd, buf, size);
}
    1df2:	8082                	ret

0000000000001df4 <chdir>:
    register long a7 __asm__("a7") = n;
    1df4:	03100893          	li	a7,49
    __asm_syscall("r"(a7), "0"(a0))
    1df8:	00000073          	ecall

int chdir(const char *path){
    return syscall(SYS_chdir, path);
}
    1dfc:	2501                	sext.w	a0,a0
    1dfe:	8082                	ret

0000000000001e00 <mkdir>:

int mkdir(const char *path, mode_t mode){
    1e00:	862e                	mv	a2,a1
    1e02:	87aa                	mv	a5,a0
    register long a2 __asm__("a2") = c;
    1e04:	1602                	slli	a2,a2,0x20
    register long a7 __asm__("a7") = n;
    1e06:	02200893          	li	a7,34
    register long a0 __asm__("a0") = a;
    1e0a:	f9c00513          	li	a0,-100
    register long a1 __asm__("a1") = b;
    1e0e:	85be                	mv	a1,a5
    register long a2 __asm__("a2") = c;
    1e10:	9201                	srli	a2,a2,0x20
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1e12:	00000073          	ecall
    return syscall(SYS_mkdirat, AT_FDCWD, path, mode);
}
    1e16:	2501                	sext.w	a0,a0
    1e18:	8082                	ret

0000000000001e1a <getdents>:
    register long a7 __asm__("a7") = n;
    1e1a:	03d00893          	li	a7,61
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1e1e:	00000073          	ecall

int getdents(int fd, struct linux_dirent64 *dirp64, unsigned long len){
    //return syscall(SYS_getdents64, fd, dirp64, len);
    return syscall(SYS_getdents64, fd, dirp64, len);
}
    1e22:	2501                	sext.w	a0,a0
    1e24:	8082                	ret

0000000000001e26 <pipe>:
    register long a7 __asm__("a7") = n;
    1e26:	03b00893          	li	a7,59
    register long a1 __asm__("a1") = b;
    1e2a:	4581                	li	a1,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1e2c:	00000073          	ecall

int pipe(int fd[2]){
    return syscall(SYS_pipe2, fd, 0);
}
    1e30:	2501                	sext.w	a0,a0
    1e32:	8082                	ret

0000000000001e34 <dup>:
    register long a7 __asm__("a7") = n;
    1e34:	48dd                	li	a7,23
    __asm_syscall("r"(a7), "0"(a0))
    1e36:	00000073          	ecall

int dup(int fd){
    return syscall(SYS_dup, fd);
}
    1e3a:	2501                	sext.w	a0,a0
    1e3c:	8082                	ret

0000000000001e3e <dup2>:
    register long a7 __asm__("a7") = n;
    1e3e:	48e1                	li	a7,24
    register long a2 __asm__("a2") = c;
    1e40:	4601                	li	a2,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1e42:	00000073          	ecall

int dup2(int old, int new){
    return syscall(SYS_dup3, old, new, 0);
}
    1e46:	2501                	sext.w	a0,a0
    1e48:	8082                	ret

0000000000001e4a <mount>:
    register long a7 __asm__("a7") = n;
    1e4a:	02800893          	li	a7,40
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4))
    1e4e:	00000073          	ecall

int mount(const char *special, const char *dir, const char *fstype, unsigned long flags, const void *data)
{
        return syscall(SYS_mount, special, dir, fstype, flags, data);
}
    1e52:	2501                	sext.w	a0,a0
    1e54:	8082                	ret

0000000000001e56 <umount>:
    register long a7 __asm__("a7") = n;
    1e56:	02700893          	li	a7,39
    register long a1 __asm__("a1") = b;
    1e5a:	4581                	li	a1,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1e5c:	00000073          	ecall

int umount(const char *special)
{
        return syscall(SYS_umount2, special, 0);
}
    1e60:	2501                	sext.w	a0,a0
    1e62:	8082                	ret

0000000000001e64 <__clone>:

.global __clone
.type  __clone, %function
__clone:
	# Save func and arg to stack
	addi a1, a1, -16
    1e64:	15c1                	addi	a1,a1,-16
	sd a0, 0(a1)
    1e66:	e188                	sd	a0,0(a1)
	sd a3, 8(a1)
    1e68:	e594                	sd	a3,8(a1)

	# Call SYS_clone
	mv a0, a2
    1e6a:	8532                	mv	a0,a2
	mv a2, a4
    1e6c:	863a                	mv	a2,a4
	mv a3, a5
    1e6e:	86be                	mv	a3,a5
	mv a4, a6
    1e70:	8742                	mv	a4,a6
	li a7, 220 # SYS_clone
    1e72:	0dc00893          	li	a7,220
	ecall
    1e76:	00000073          	ecall

	beqz a0, 1f
    1e7a:	c111                	beqz	a0,1e7e <__clone+0x1a>
	# Parent
	ret
    1e7c:	8082                	ret

	# Child
1:      ld a1, 0(sp)
    1e7e:	6582                	ld	a1,0(sp)
	ld a0, 8(sp)
    1e80:	6522                	ld	a0,8(sp)
	jalr a1
    1e82:	9582                	jalr	a1

	# Exit
	li a7, 93 # SYS_exit
    1e84:	05d00893          	li	a7,93
	ecall
    1e88:	00000073          	ecall
