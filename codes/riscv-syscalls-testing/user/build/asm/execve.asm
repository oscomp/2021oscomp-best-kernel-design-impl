
/home/ubuntu/rust/ultra-os/codes/riscv-syscalls-testing/user/build/riscv64/execve:     file format elf64-littleriscv


Disassembly of section .text:

0000000000001000 <_start>:
.section .text.entry
.globl _start
_start:
    mv a0, sp
    1000:	850a                	mv	a0,sp
    tail __start_main
    1002:	a08d                	j	1064 <__start_main>

0000000000001004 <test_execve>:
 * 测试成功则输出：
 * "  I am test_echo."
 * 测试失败则输出：
 * "  execve error."
 */
void test_execve(void){
    1004:	7179                	addi	sp,sp,-48
    TEST_START(__func__);
    1006:	00001517          	auipc	a0,0x1
    100a:	e8250513          	addi	a0,a0,-382 # 1e88 <__clone+0x28>
void test_execve(void){
    100e:	f406                	sd	ra,40(sp)
    TEST_START(__func__);
    1010:	2c6000ef          	jal	ra,12d6 <puts>
    1014:	00001517          	auipc	a0,0x1
    1018:	ee450513          	addi	a0,a0,-284 # 1ef8 <__func__.0>
    101c:	2ba000ef          	jal	ra,12d6 <puts>
    1020:	00001517          	auipc	a0,0x1
    1024:	e8050513          	addi	a0,a0,-384 # 1ea0 <__clone+0x40>
    1028:	2ae000ef          	jal	ra,12d6 <puts>
    char *newargv[] = {NULL, NULL};
    char *newenviron[] = {NULL};
    execve("test_echo", newargv, newenviron);
    102c:	0030                	addi	a2,sp,8
    102e:	080c                	addi	a1,sp,16
    1030:	00001517          	auipc	a0,0x1
    1034:	e8050513          	addi	a0,a0,-384 # 1eb0 <__clone+0x50>
    char *newargv[] = {NULL, NULL};
    1038:	e802                	sd	zero,16(sp)
    103a:	ec02                	sd	zero,24(sp)
    char *newenviron[] = {NULL};
    103c:	e402                	sd	zero,8(sp)
    execve("test_echo", newargv, newenviron);
    103e:	461000ef          	jal	ra,1c9e <execve>
    printf("  execve error.\n");
    1042:	00001517          	auipc	a0,0x1
    1046:	e7e50513          	addi	a0,a0,-386 # 1ec0 <__clone+0x60>
    104a:	2ae000ef          	jal	ra,12f8 <printf>
    //TEST_END(__func__);
}
    104e:	70a2                	ld	ra,40(sp)
    1050:	6145                	addi	sp,sp,48
    1052:	8082                	ret

0000000000001054 <main>:

int main(void){
    1054:	1141                	addi	sp,sp,-16
    1056:	e406                	sd	ra,8(sp)
    test_execve();
    1058:	fadff0ef          	jal	ra,1004 <test_execve>
    return 0;
}
    105c:	60a2                	ld	ra,8(sp)
    105e:	4501                	li	a0,0
    1060:	0141                	addi	sp,sp,16
    1062:	8082                	ret

0000000000001064 <__start_main>:
#include <unistd.h>

extern int main();

int __start_main(long *p)
{
    1064:	85aa                	mv	a1,a0
	int argc = p[0];
	char **argv = (void *)(p+1);

	exit(main(argc, argv));
    1066:	4108                	lw	a0,0(a0)
{
    1068:	1141                	addi	sp,sp,-16
	exit(main(argc, argv));
    106a:	05a1                	addi	a1,a1,8
{
    106c:	e406                	sd	ra,8(sp)
	exit(main(argc, argv));
    106e:	fe7ff0ef          	jal	ra,1054 <main>
    1072:	40b000ef          	jal	ra,1c7c <exit>
	return 0;
}
    1076:	60a2                	ld	ra,8(sp)
    1078:	4501                	li	a0,0
    107a:	0141                	addi	sp,sp,16
    107c:	8082                	ret

000000000000107e <printint.constprop.0>:
    write(f, s, l);
}

static char digits[] = "0123456789abcdef";

static void printint(int xx, int base, int sign)
    107e:	7179                	addi	sp,sp,-48
    1080:	f406                	sd	ra,40(sp)
{
    char buf[16 + 1];
    int i;
    uint x;

    if (sign && (sign = xx < 0))
    1082:	12054b63          	bltz	a0,11b8 <printint.constprop.0+0x13a>

    buf[16] = 0;
    i = 15;
    do
    {
        buf[i--] = digits[x % base];
    1086:	02b577bb          	remuw	a5,a0,a1
    108a:	00001617          	auipc	a2,0x1
    108e:	e7e60613          	addi	a2,a2,-386 # 1f08 <digits>
    buf[16] = 0;
    1092:	00010c23          	sb	zero,24(sp)
        buf[i--] = digits[x % base];
    1096:	0005871b          	sext.w	a4,a1
    109a:	1782                	slli	a5,a5,0x20
    109c:	9381                	srli	a5,a5,0x20
    109e:	97b2                	add	a5,a5,a2
    10a0:	0007c783          	lbu	a5,0(a5)
    } while ((x /= base) != 0);
    10a4:	02b5583b          	divuw	a6,a0,a1
        buf[i--] = digits[x % base];
    10a8:	00f10ba3          	sb	a5,23(sp)
    } while ((x /= base) != 0);
    10ac:	1cb56363          	bltu	a0,a1,1272 <printint.constprop.0+0x1f4>
        buf[i--] = digits[x % base];
    10b0:	45b9                	li	a1,14
    10b2:	02e877bb          	remuw	a5,a6,a4
    10b6:	1782                	slli	a5,a5,0x20
    10b8:	9381                	srli	a5,a5,0x20
    10ba:	97b2                	add	a5,a5,a2
    10bc:	0007c783          	lbu	a5,0(a5)
    } while ((x /= base) != 0);
    10c0:	02e856bb          	divuw	a3,a6,a4
        buf[i--] = digits[x % base];
    10c4:	00f10b23          	sb	a5,22(sp)
    } while ((x /= base) != 0);
    10c8:	0ce86e63          	bltu	a6,a4,11a4 <printint.constprop.0+0x126>
        buf[i--] = digits[x % base];
    10cc:	02e6f5bb          	remuw	a1,a3,a4
    } while ((x /= base) != 0);
    10d0:	02e6d7bb          	divuw	a5,a3,a4
        buf[i--] = digits[x % base];
    10d4:	1582                	slli	a1,a1,0x20
    10d6:	9181                	srli	a1,a1,0x20
    10d8:	95b2                	add	a1,a1,a2
    10da:	0005c583          	lbu	a1,0(a1)
    10de:	00b10aa3          	sb	a1,21(sp)
    } while ((x /= base) != 0);
    10e2:	0007859b          	sext.w	a1,a5
    10e6:	12e6ec63          	bltu	a3,a4,121e <printint.constprop.0+0x1a0>
        buf[i--] = digits[x % base];
    10ea:	02e7f6bb          	remuw	a3,a5,a4
    10ee:	1682                	slli	a3,a3,0x20
    10f0:	9281                	srli	a3,a3,0x20
    10f2:	96b2                	add	a3,a3,a2
    10f4:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    10f8:	02e7d83b          	divuw	a6,a5,a4
        buf[i--] = digits[x % base];
    10fc:	00d10a23          	sb	a3,20(sp)
    } while ((x /= base) != 0);
    1100:	12e5e863          	bltu	a1,a4,1230 <printint.constprop.0+0x1b2>
        buf[i--] = digits[x % base];
    1104:	02e876bb          	remuw	a3,a6,a4
    1108:	1682                	slli	a3,a3,0x20
    110a:	9281                	srli	a3,a3,0x20
    110c:	96b2                	add	a3,a3,a2
    110e:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    1112:	02e855bb          	divuw	a1,a6,a4
        buf[i--] = digits[x % base];
    1116:	00d109a3          	sb	a3,19(sp)
    } while ((x /= base) != 0);
    111a:	12e86463          	bltu	a6,a4,1242 <printint.constprop.0+0x1c4>
        buf[i--] = digits[x % base];
    111e:	02e5f6bb          	remuw	a3,a1,a4
    1122:	1682                	slli	a3,a3,0x20
    1124:	9281                	srli	a3,a3,0x20
    1126:	96b2                	add	a3,a3,a2
    1128:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    112c:	02e5d83b          	divuw	a6,a1,a4
        buf[i--] = digits[x % base];
    1130:	00d10923          	sb	a3,18(sp)
    } while ((x /= base) != 0);
    1134:	0ce5ec63          	bltu	a1,a4,120c <printint.constprop.0+0x18e>
        buf[i--] = digits[x % base];
    1138:	02e876bb          	remuw	a3,a6,a4
    113c:	1682                	slli	a3,a3,0x20
    113e:	9281                	srli	a3,a3,0x20
    1140:	96b2                	add	a3,a3,a2
    1142:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    1146:	02e855bb          	divuw	a1,a6,a4
        buf[i--] = digits[x % base];
    114a:	00d108a3          	sb	a3,17(sp)
    } while ((x /= base) != 0);
    114e:	10e86963          	bltu	a6,a4,1260 <printint.constprop.0+0x1e2>
        buf[i--] = digits[x % base];
    1152:	02e5f6bb          	remuw	a3,a1,a4
    1156:	1682                	slli	a3,a3,0x20
    1158:	9281                	srli	a3,a3,0x20
    115a:	96b2                	add	a3,a3,a2
    115c:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    1160:	02e5d83b          	divuw	a6,a1,a4
        buf[i--] = digits[x % base];
    1164:	00d10823          	sb	a3,16(sp)
    } while ((x /= base) != 0);
    1168:	10e5e763          	bltu	a1,a4,1276 <printint.constprop.0+0x1f8>
        buf[i--] = digits[x % base];
    116c:	02e876bb          	remuw	a3,a6,a4
    1170:	1682                	slli	a3,a3,0x20
    1172:	9281                	srli	a3,a3,0x20
    1174:	96b2                	add	a3,a3,a2
    1176:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    117a:	02e857bb          	divuw	a5,a6,a4
        buf[i--] = digits[x % base];
    117e:	00d107a3          	sb	a3,15(sp)
    } while ((x /= base) != 0);
    1182:	10e86363          	bltu	a6,a4,1288 <printint.constprop.0+0x20a>
        buf[i--] = digits[x % base];
    1186:	1782                	slli	a5,a5,0x20
    1188:	9381                	srli	a5,a5,0x20
    118a:	97b2                	add	a5,a5,a2
    118c:	0007c783          	lbu	a5,0(a5)
    1190:	4599                	li	a1,6
    1192:	00f10723          	sb	a5,14(sp)

    if (sign)
    1196:	00055763          	bgez	a0,11a4 <printint.constprop.0+0x126>
        buf[i--] = '-';
    119a:	02d00793          	li	a5,45
    119e:	00f106a3          	sb	a5,13(sp)
        buf[i--] = digits[x % base];
    11a2:	4595                	li	a1,5
    write(f, s, l);
    11a4:	003c                	addi	a5,sp,8
    11a6:	4641                	li	a2,16
    11a8:	9e0d                	subw	a2,a2,a1
    11aa:	4505                	li	a0,1
    11ac:	95be                	add	a1,a1,a5
    11ae:	27f000ef          	jal	ra,1c2c <write>
    i++;
    if (i < 0)
        puts("printint error");
    out(stdout, buf + i, 16 - i);
}
    11b2:	70a2                	ld	ra,40(sp)
    11b4:	6145                	addi	sp,sp,48
    11b6:	8082                	ret
        x = -xx;
    11b8:	40a0083b          	negw	a6,a0
        buf[i--] = digits[x % base];
    11bc:	02b877bb          	remuw	a5,a6,a1
    11c0:	00001617          	auipc	a2,0x1
    11c4:	d4860613          	addi	a2,a2,-696 # 1f08 <digits>
    buf[16] = 0;
    11c8:	00010c23          	sb	zero,24(sp)
        buf[i--] = digits[x % base];
    11cc:	0005871b          	sext.w	a4,a1
    11d0:	1782                	slli	a5,a5,0x20
    11d2:	9381                	srli	a5,a5,0x20
    11d4:	97b2                	add	a5,a5,a2
    11d6:	0007c783          	lbu	a5,0(a5)
    } while ((x /= base) != 0);
    11da:	02b858bb          	divuw	a7,a6,a1
        buf[i--] = digits[x % base];
    11de:	00f10ba3          	sb	a5,23(sp)
    } while ((x /= base) != 0);
    11e2:	06b86963          	bltu	a6,a1,1254 <printint.constprop.0+0x1d6>
        buf[i--] = digits[x % base];
    11e6:	02e8f7bb          	remuw	a5,a7,a4
    11ea:	1782                	slli	a5,a5,0x20
    11ec:	9381                	srli	a5,a5,0x20
    11ee:	97b2                	add	a5,a5,a2
    11f0:	0007c783          	lbu	a5,0(a5)
    } while ((x /= base) != 0);
    11f4:	02e8d6bb          	divuw	a3,a7,a4
        buf[i--] = digits[x % base];
    11f8:	00f10b23          	sb	a5,22(sp)
    } while ((x /= base) != 0);
    11fc:	ece8f8e3          	bgeu	a7,a4,10cc <printint.constprop.0+0x4e>
        buf[i--] = '-';
    1200:	02d00793          	li	a5,45
    1204:	00f10aa3          	sb	a5,21(sp)
        buf[i--] = digits[x % base];
    1208:	45b5                	li	a1,13
    120a:	bf69                	j	11a4 <printint.constprop.0+0x126>
    120c:	45a9                	li	a1,10
    if (sign)
    120e:	f8055be3          	bgez	a0,11a4 <printint.constprop.0+0x126>
        buf[i--] = '-';
    1212:	02d00793          	li	a5,45
    1216:	00f108a3          	sb	a5,17(sp)
        buf[i--] = digits[x % base];
    121a:	45a5                	li	a1,9
    121c:	b761                	j	11a4 <printint.constprop.0+0x126>
    121e:	45b5                	li	a1,13
    if (sign)
    1220:	f80552e3          	bgez	a0,11a4 <printint.constprop.0+0x126>
        buf[i--] = '-';
    1224:	02d00793          	li	a5,45
    1228:	00f10a23          	sb	a5,20(sp)
        buf[i--] = digits[x % base];
    122c:	45b1                	li	a1,12
    122e:	bf9d                	j	11a4 <printint.constprop.0+0x126>
    1230:	45b1                	li	a1,12
    if (sign)
    1232:	f60559e3          	bgez	a0,11a4 <printint.constprop.0+0x126>
        buf[i--] = '-';
    1236:	02d00793          	li	a5,45
    123a:	00f109a3          	sb	a5,19(sp)
        buf[i--] = digits[x % base];
    123e:	45ad                	li	a1,11
    1240:	b795                	j	11a4 <printint.constprop.0+0x126>
    1242:	45ad                	li	a1,11
    if (sign)
    1244:	f60550e3          	bgez	a0,11a4 <printint.constprop.0+0x126>
        buf[i--] = '-';
    1248:	02d00793          	li	a5,45
    124c:	00f10923          	sb	a5,18(sp)
        buf[i--] = digits[x % base];
    1250:	45a9                	li	a1,10
    1252:	bf89                	j	11a4 <printint.constprop.0+0x126>
        buf[i--] = '-';
    1254:	02d00793          	li	a5,45
    1258:	00f10b23          	sb	a5,22(sp)
        buf[i--] = digits[x % base];
    125c:	45b9                	li	a1,14
    125e:	b799                	j	11a4 <printint.constprop.0+0x126>
    1260:	45a5                	li	a1,9
    if (sign)
    1262:	f40551e3          	bgez	a0,11a4 <printint.constprop.0+0x126>
        buf[i--] = '-';
    1266:	02d00793          	li	a5,45
    126a:	00f10823          	sb	a5,16(sp)
        buf[i--] = digits[x % base];
    126e:	45a1                	li	a1,8
    1270:	bf15                	j	11a4 <printint.constprop.0+0x126>
    i = 15;
    1272:	45bd                	li	a1,15
    1274:	bf05                	j	11a4 <printint.constprop.0+0x126>
        buf[i--] = digits[x % base];
    1276:	45a1                	li	a1,8
    if (sign)
    1278:	f20556e3          	bgez	a0,11a4 <printint.constprop.0+0x126>
        buf[i--] = '-';
    127c:	02d00793          	li	a5,45
    1280:	00f107a3          	sb	a5,15(sp)
        buf[i--] = digits[x % base];
    1284:	459d                	li	a1,7
    1286:	bf39                	j	11a4 <printint.constprop.0+0x126>
    1288:	459d                	li	a1,7
    if (sign)
    128a:	f0055de3          	bgez	a0,11a4 <printint.constprop.0+0x126>
        buf[i--] = '-';
    128e:	02d00793          	li	a5,45
    1292:	00f10723          	sb	a5,14(sp)
        buf[i--] = digits[x % base];
    1296:	4599                	li	a1,6
    1298:	b731                	j	11a4 <printint.constprop.0+0x126>

000000000000129a <getchar>:
{
    129a:	1101                	addi	sp,sp,-32
    read(stdin, &byte, 1);
    129c:	00f10593          	addi	a1,sp,15
    12a0:	4605                	li	a2,1
    12a2:	4501                	li	a0,0
{
    12a4:	ec06                	sd	ra,24(sp)
    char byte = 0;
    12a6:	000107a3          	sb	zero,15(sp)
    read(stdin, &byte, 1);
    12aa:	179000ef          	jal	ra,1c22 <read>
}
    12ae:	60e2                	ld	ra,24(sp)
    12b0:	00f14503          	lbu	a0,15(sp)
    12b4:	6105                	addi	sp,sp,32
    12b6:	8082                	ret

00000000000012b8 <putchar>:
{
    12b8:	1101                	addi	sp,sp,-32
    12ba:	87aa                	mv	a5,a0
    return write(stdout, &byte, 1);
    12bc:	00f10593          	addi	a1,sp,15
    12c0:	4605                	li	a2,1
    12c2:	4505                	li	a0,1
{
    12c4:	ec06                	sd	ra,24(sp)
    char byte = c;
    12c6:	00f107a3          	sb	a5,15(sp)
    return write(stdout, &byte, 1);
    12ca:	163000ef          	jal	ra,1c2c <write>
}
    12ce:	60e2                	ld	ra,24(sp)
    12d0:	2501                	sext.w	a0,a0
    12d2:	6105                	addi	sp,sp,32
    12d4:	8082                	ret

00000000000012d6 <puts>:
{
    12d6:	1141                	addi	sp,sp,-16
    12d8:	e406                	sd	ra,8(sp)
    12da:	e022                	sd	s0,0(sp)
    12dc:	842a                	mv	s0,a0
    r = -(write(stdout, s, strlen(s)) < 0);
    12de:	56a000ef          	jal	ra,1848 <strlen>
    12e2:	862a                	mv	a2,a0
    12e4:	85a2                	mv	a1,s0
    12e6:	4505                	li	a0,1
    12e8:	145000ef          	jal	ra,1c2c <write>
}
    12ec:	60a2                	ld	ra,8(sp)
    12ee:	6402                	ld	s0,0(sp)
    r = -(write(stdout, s, strlen(s)) < 0);
    12f0:	957d                	srai	a0,a0,0x3f
    return r;
    12f2:	2501                	sext.w	a0,a0
}
    12f4:	0141                	addi	sp,sp,16
    12f6:	8082                	ret

00000000000012f8 <printf>:
    out(stdout, buf, i);
}

// Print to the console. only understands %d, %x, %p, %s.
void printf(const char *fmt, ...)
{
    12f8:	7171                	addi	sp,sp,-176
    12fa:	fc56                	sd	s5,56(sp)
    12fc:	ed3e                	sd	a5,152(sp)
    buf[i++] = '0';
    12fe:	7ae1                	lui	s5,0xffff8
    va_list ap;
    int cnt = 0, l = 0;
    char *a, *z, *s = (char *)fmt, str;
    int f = stdout;

    va_start(ap, fmt);
    1300:	18bc                	addi	a5,sp,120
{
    1302:	e8ca                	sd	s2,80(sp)
    1304:	e4ce                	sd	s3,72(sp)
    1306:	e0d2                	sd	s4,64(sp)
    1308:	f85a                	sd	s6,48(sp)
    130a:	f486                	sd	ra,104(sp)
    130c:	f0a2                	sd	s0,96(sp)
    130e:	eca6                	sd	s1,88(sp)
    1310:	fcae                	sd	a1,120(sp)
    1312:	e132                	sd	a2,128(sp)
    1314:	e536                	sd	a3,136(sp)
    1316:	e93a                	sd	a4,144(sp)
    1318:	f142                	sd	a6,160(sp)
    131a:	f546                	sd	a7,168(sp)
    va_start(ap, fmt);
    131c:	e03e                	sd	a5,0(sp)
    for (;;)
    {
        if (!*s)
            break;
        for (a = s; *s && *s != '%'; s++)
    131e:	02500913          	li	s2,37
        out(f, a, l);
        if (l)
            continue;
        if (s[1] == 0)
            break;
        switch (s[1])
    1322:	07300a13          	li	s4,115
        case 'p':
            printptr(va_arg(ap, uint64));
            break;
        case 's':
            if ((a = va_arg(ap, char *)) == 0)
                a = "(null)";
    1326:	00001b17          	auipc	s6,0x1
    132a:	bb2b0b13          	addi	s6,s6,-1102 # 1ed8 <__clone+0x78>
    buf[i++] = '0';
    132e:	830aca93          	xori	s5,s5,-2000
        buf[i++] = digits[x >> (sizeof(uint64) * 8 - 4)];
    1332:	00001997          	auipc	s3,0x1
    1336:	bd698993          	addi	s3,s3,-1066 # 1f08 <digits>
        if (!*s)
    133a:	00054783          	lbu	a5,0(a0)
    133e:	16078a63          	beqz	a5,14b2 <printf+0x1ba>
    1342:	862a                	mv	a2,a0
        for (a = s; *s && *s != '%'; s++)
    1344:	19278163          	beq	a5,s2,14c6 <printf+0x1ce>
    1348:	00164783          	lbu	a5,1(a2)
    134c:	0605                	addi	a2,a2,1
    134e:	fbfd                	bnez	a5,1344 <printf+0x4c>
    1350:	84b2                	mv	s1,a2
        l = z - a;
    1352:	40a6043b          	subw	s0,a2,a0
    write(f, s, l);
    1356:	85aa                	mv	a1,a0
    1358:	8622                	mv	a2,s0
    135a:	4505                	li	a0,1
    135c:	0d1000ef          	jal	ra,1c2c <write>
        if (l)
    1360:	18041c63          	bnez	s0,14f8 <printf+0x200>
        if (s[1] == 0)
    1364:	0014c783          	lbu	a5,1(s1)
    1368:	14078563          	beqz	a5,14b2 <printf+0x1ba>
        switch (s[1])
    136c:	1d478063          	beq	a5,s4,152c <printf+0x234>
    1370:	18fa6663          	bltu	s4,a5,14fc <printf+0x204>
    1374:	06400713          	li	a4,100
    1378:	1ae78063          	beq	a5,a4,1518 <printf+0x220>
    137c:	07000713          	li	a4,112
    1380:	1ce79963          	bne	a5,a4,1552 <printf+0x25a>
            printptr(va_arg(ap, uint64));
    1384:	6702                	ld	a4,0(sp)
    buf[i++] = '0';
    1386:	01511423          	sh	s5,8(sp)
    write(f, s, l);
    138a:	4649                	li	a2,18
            printptr(va_arg(ap, uint64));
    138c:	631c                	ld	a5,0(a4)
    138e:	0721                	addi	a4,a4,8
    1390:	e03a                	sd	a4,0(sp)
    for (j = 0; j < (sizeof(uint64) * 2); j++, x <<= 4)
    1392:	00479293          	slli	t0,a5,0x4
    1396:	00879f93          	slli	t6,a5,0x8
    139a:	00c79f13          	slli	t5,a5,0xc
    139e:	01079e93          	slli	t4,a5,0x10
    13a2:	01479e13          	slli	t3,a5,0x14
    13a6:	01879313          	slli	t1,a5,0x18
    13aa:	01c79893          	slli	a7,a5,0x1c
    13ae:	02479813          	slli	a6,a5,0x24
    13b2:	02879513          	slli	a0,a5,0x28
    13b6:	02c79593          	slli	a1,a5,0x2c
    13ba:	03079693          	slli	a3,a5,0x30
    13be:	03479713          	slli	a4,a5,0x34
        buf[i++] = digits[x >> (sizeof(uint64) * 8 - 4)];
    13c2:	03c7d413          	srli	s0,a5,0x3c
    13c6:	01c7d39b          	srliw	t2,a5,0x1c
    13ca:	03c2d293          	srli	t0,t0,0x3c
    13ce:	03cfdf93          	srli	t6,t6,0x3c
    13d2:	03cf5f13          	srli	t5,t5,0x3c
    13d6:	03cede93          	srli	t4,t4,0x3c
    13da:	03ce5e13          	srli	t3,t3,0x3c
    13de:	03c35313          	srli	t1,t1,0x3c
    13e2:	03c8d893          	srli	a7,a7,0x3c
    13e6:	03c85813          	srli	a6,a6,0x3c
    13ea:	9171                	srli	a0,a0,0x3c
    13ec:	91f1                	srli	a1,a1,0x3c
    13ee:	92f1                	srli	a3,a3,0x3c
    13f0:	9371                	srli	a4,a4,0x3c
    13f2:	96ce                	add	a3,a3,s3
    13f4:	974e                	add	a4,a4,s3
    13f6:	944e                	add	s0,s0,s3
    13f8:	92ce                	add	t0,t0,s3
    13fa:	9fce                	add	t6,t6,s3
    13fc:	9f4e                	add	t5,t5,s3
    13fe:	9ece                	add	t4,t4,s3
    1400:	9e4e                	add	t3,t3,s3
    1402:	934e                	add	t1,t1,s3
    1404:	98ce                	add	a7,a7,s3
    1406:	93ce                	add	t2,t2,s3
    1408:	984e                	add	a6,a6,s3
    140a:	954e                	add	a0,a0,s3
    140c:	95ce                	add	a1,a1,s3
    140e:	0006c083          	lbu	ra,0(a3)
    1412:	0002c283          	lbu	t0,0(t0)
    1416:	00074683          	lbu	a3,0(a4)
    141a:	000fcf83          	lbu	t6,0(t6)
    141e:	000f4f03          	lbu	t5,0(t5)
    1422:	000ece83          	lbu	t4,0(t4)
    1426:	000e4e03          	lbu	t3,0(t3)
    142a:	00034303          	lbu	t1,0(t1)
    142e:	0008c883          	lbu	a7,0(a7)
    1432:	0003c383          	lbu	t2,0(t2)
    1436:	00084803          	lbu	a6,0(a6)
    143a:	00054503          	lbu	a0,0(a0)
    143e:	0005c583          	lbu	a1,0(a1)
    1442:	00044403          	lbu	s0,0(s0)
    for (j = 0; j < (sizeof(uint64) * 2); j++, x <<= 4)
    1446:	03879713          	slli	a4,a5,0x38
        buf[i++] = digits[x >> (sizeof(uint64) * 8 - 4)];
    144a:	9371                	srli	a4,a4,0x3c
    144c:	8bbd                	andi	a5,a5,15
    144e:	974e                	add	a4,a4,s3
    1450:	97ce                	add	a5,a5,s3
    1452:	005105a3          	sb	t0,11(sp)
    1456:	01f10623          	sb	t6,12(sp)
    145a:	01e106a3          	sb	t5,13(sp)
    145e:	01d10723          	sb	t4,14(sp)
    1462:	01c107a3          	sb	t3,15(sp)
    1466:	00610823          	sb	t1,16(sp)
    146a:	011108a3          	sb	a7,17(sp)
    146e:	00710923          	sb	t2,18(sp)
    1472:	010109a3          	sb	a6,19(sp)
    1476:	00a10a23          	sb	a0,20(sp)
    147a:	00b10aa3          	sb	a1,21(sp)
    147e:	00110b23          	sb	ra,22(sp)
    1482:	00d10ba3          	sb	a3,23(sp)
    1486:	00810523          	sb	s0,10(sp)
    148a:	00074703          	lbu	a4,0(a4)
    148e:	0007c783          	lbu	a5,0(a5)
    write(f, s, l);
    1492:	002c                	addi	a1,sp,8
    1494:	4505                	li	a0,1
        buf[i++] = digits[x >> (sizeof(uint64) * 8 - 4)];
    1496:	00e10c23          	sb	a4,24(sp)
    149a:	00f10ca3          	sb	a5,25(sp)
    buf[i] = 0;
    149e:	00010d23          	sb	zero,26(sp)
    write(f, s, l);
    14a2:	78a000ef          	jal	ra,1c2c <write>
            // Print unknown % sequence to draw attention.
            putchar('%');
            putchar(s[1]);
            break;
        }
        s += 2;
    14a6:	00248513          	addi	a0,s1,2
        if (!*s)
    14aa:	00054783          	lbu	a5,0(a0)
    14ae:	e8079ae3          	bnez	a5,1342 <printf+0x4a>
    }
    va_end(ap);
}
    14b2:	70a6                	ld	ra,104(sp)
    14b4:	7406                	ld	s0,96(sp)
    14b6:	64e6                	ld	s1,88(sp)
    14b8:	6946                	ld	s2,80(sp)
    14ba:	69a6                	ld	s3,72(sp)
    14bc:	6a06                	ld	s4,64(sp)
    14be:	7ae2                	ld	s5,56(sp)
    14c0:	7b42                	ld	s6,48(sp)
    14c2:	614d                	addi	sp,sp,176
    14c4:	8082                	ret
        for (z = s; s[0] == '%' && s[1] == '%'; z++, s += 2)
    14c6:	00064783          	lbu	a5,0(a2)
    14ca:	84b2                	mv	s1,a2
    14cc:	01278963          	beq	a5,s2,14de <printf+0x1e6>
    14d0:	b549                	j	1352 <printf+0x5a>
    14d2:	0024c783          	lbu	a5,2(s1)
    14d6:	0605                	addi	a2,a2,1
    14d8:	0489                	addi	s1,s1,2
    14da:	e7279ce3          	bne	a5,s2,1352 <printf+0x5a>
    14de:	0014c783          	lbu	a5,1(s1)
    14e2:	ff2788e3          	beq	a5,s2,14d2 <printf+0x1da>
        l = z - a;
    14e6:	40a6043b          	subw	s0,a2,a0
    write(f, s, l);
    14ea:	85aa                	mv	a1,a0
    14ec:	8622                	mv	a2,s0
    14ee:	4505                	li	a0,1
    14f0:	73c000ef          	jal	ra,1c2c <write>
        if (l)
    14f4:	e60408e3          	beqz	s0,1364 <printf+0x6c>
    14f8:	8526                	mv	a0,s1
    14fa:	b581                	j	133a <printf+0x42>
        switch (s[1])
    14fc:	07800713          	li	a4,120
    1500:	04e79963          	bne	a5,a4,1552 <printf+0x25a>
            printint(va_arg(ap, int), 16, 1);
    1504:	6782                	ld	a5,0(sp)
    1506:	45c1                	li	a1,16
    1508:	4388                	lw	a0,0(a5)
    150a:	07a1                	addi	a5,a5,8
    150c:	e03e                	sd	a5,0(sp)
    150e:	b71ff0ef          	jal	ra,107e <printint.constprop.0>
        s += 2;
    1512:	00248513          	addi	a0,s1,2
    1516:	bf51                	j	14aa <printf+0x1b2>
            printint(va_arg(ap, int), 10, 1);
    1518:	6782                	ld	a5,0(sp)
    151a:	45a9                	li	a1,10
    151c:	4388                	lw	a0,0(a5)
    151e:	07a1                	addi	a5,a5,8
    1520:	e03e                	sd	a5,0(sp)
    1522:	b5dff0ef          	jal	ra,107e <printint.constprop.0>
        s += 2;
    1526:	00248513          	addi	a0,s1,2
    152a:	b741                	j	14aa <printf+0x1b2>
            if ((a = va_arg(ap, char *)) == 0)
    152c:	6782                	ld	a5,0(sp)
    152e:	6380                	ld	s0,0(a5)
    1530:	07a1                	addi	a5,a5,8
    1532:	e03e                	sd	a5,0(sp)
    1534:	c031                	beqz	s0,1578 <printf+0x280>
            l = strnlen(a, 200);
    1536:	0c800593          	li	a1,200
    153a:	8522                	mv	a0,s0
    153c:	3f8000ef          	jal	ra,1934 <strnlen>
    write(f, s, l);
    1540:	0005061b          	sext.w	a2,a0
    1544:	85a2                	mv	a1,s0
    1546:	4505                	li	a0,1
    1548:	6e4000ef          	jal	ra,1c2c <write>
        s += 2;
    154c:	00248513          	addi	a0,s1,2
    1550:	bfa9                	j	14aa <printf+0x1b2>
    return write(stdout, &byte, 1);
    1552:	4605                	li	a2,1
    1554:	002c                	addi	a1,sp,8
    1556:	4505                	li	a0,1
    char byte = c;
    1558:	01210423          	sb	s2,8(sp)
    return write(stdout, &byte, 1);
    155c:	6d0000ef          	jal	ra,1c2c <write>
    char byte = c;
    1560:	0014c783          	lbu	a5,1(s1)
    return write(stdout, &byte, 1);
    1564:	4605                	li	a2,1
    1566:	002c                	addi	a1,sp,8
    1568:	4505                	li	a0,1
    char byte = c;
    156a:	00f10423          	sb	a5,8(sp)
    return write(stdout, &byte, 1);
    156e:	6be000ef          	jal	ra,1c2c <write>
        s += 2;
    1572:	00248513          	addi	a0,s1,2
    1576:	bf15                	j	14aa <printf+0x1b2>
                a = "(null)";
    1578:	845a                	mv	s0,s6
    157a:	bf75                	j	1536 <printf+0x23e>

000000000000157c <isspace>:
#define HIGHS (ONES * (UCHAR_MAX / 2 + 1))
#define HASZERO(x) (((x)-ONES) & ~(x)&HIGHS)

int isspace(int c)
{
    return c == ' ' || (unsigned)c - '\t' < 5;
    157c:	02000793          	li	a5,32
    1580:	00f50663          	beq	a0,a5,158c <isspace+0x10>
    1584:	355d                	addiw	a0,a0,-9
    1586:	00553513          	sltiu	a0,a0,5
    158a:	8082                	ret
    158c:	4505                	li	a0,1
}
    158e:	8082                	ret

0000000000001590 <isdigit>:

int isdigit(int c)
{
    return (unsigned)c - '0' < 10;
    1590:	fd05051b          	addiw	a0,a0,-48
}
    1594:	00a53513          	sltiu	a0,a0,10
    1598:	8082                	ret

000000000000159a <atoi>:
    return c == ' ' || (unsigned)c - '\t' < 5;
    159a:	02000613          	li	a2,32
    159e:	4591                	li	a1,4

int atoi(const char *s)
{
    int n = 0, neg = 0;
    while (isspace(*s))
    15a0:	00054703          	lbu	a4,0(a0)
    return c == ' ' || (unsigned)c - '\t' < 5;
    15a4:	ff77069b          	addiw	a3,a4,-9
    15a8:	04c70d63          	beq	a4,a2,1602 <atoi+0x68>
    15ac:	0007079b          	sext.w	a5,a4
    15b0:	04d5f963          	bgeu	a1,a3,1602 <atoi+0x68>
        s++;
    switch (*s)
    15b4:	02b00693          	li	a3,43
    15b8:	04d70a63          	beq	a4,a3,160c <atoi+0x72>
    15bc:	02d00693          	li	a3,45
    15c0:	06d70463          	beq	a4,a3,1628 <atoi+0x8e>
        neg = 1;
    case '+':
        s++;
    }
    /* Compute n as a negative number to avoid overflow on INT_MIN */
    while (isdigit(*s))
    15c4:	fd07859b          	addiw	a1,a5,-48
    15c8:	4625                	li	a2,9
    15ca:	873e                	mv	a4,a5
    15cc:	86aa                	mv	a3,a0
    int n = 0, neg = 0;
    15ce:	4e01                	li	t3,0
    while (isdigit(*s))
    15d0:	04b66a63          	bltu	a2,a1,1624 <atoi+0x8a>
    int n = 0, neg = 0;
    15d4:	4501                	li	a0,0
    while (isdigit(*s))
    15d6:	4825                	li	a6,9
    15d8:	0016c603          	lbu	a2,1(a3)
        n = 10 * n - (*s++ - '0');
    15dc:	0025179b          	slliw	a5,a0,0x2
    15e0:	9d3d                	addw	a0,a0,a5
    15e2:	fd07031b          	addiw	t1,a4,-48
    15e6:	0015189b          	slliw	a7,a0,0x1
    while (isdigit(*s))
    15ea:	fd06059b          	addiw	a1,a2,-48
        n = 10 * n - (*s++ - '0');
    15ee:	0685                	addi	a3,a3,1
    15f0:	4068853b          	subw	a0,a7,t1
    while (isdigit(*s))
    15f4:	0006071b          	sext.w	a4,a2
    15f8:	feb870e3          	bgeu	a6,a1,15d8 <atoi+0x3e>
    return neg ? n : -n;
    15fc:	000e0563          	beqz	t3,1606 <atoi+0x6c>
}
    1600:	8082                	ret
        s++;
    1602:	0505                	addi	a0,a0,1
    1604:	bf71                	j	15a0 <atoi+0x6>
    return neg ? n : -n;
    1606:	4113053b          	subw	a0,t1,a7
    160a:	8082                	ret
    while (isdigit(*s))
    160c:	00154783          	lbu	a5,1(a0)
    1610:	4625                	li	a2,9
        s++;
    1612:	00150693          	addi	a3,a0,1
    while (isdigit(*s))
    1616:	fd07859b          	addiw	a1,a5,-48
    161a:	0007871b          	sext.w	a4,a5
    int n = 0, neg = 0;
    161e:	4e01                	li	t3,0
    while (isdigit(*s))
    1620:	fab67ae3          	bgeu	a2,a1,15d4 <atoi+0x3a>
    1624:	4501                	li	a0,0
}
    1626:	8082                	ret
    while (isdigit(*s))
    1628:	00154783          	lbu	a5,1(a0)
    162c:	4625                	li	a2,9
        s++;
    162e:	00150693          	addi	a3,a0,1
    while (isdigit(*s))
    1632:	fd07859b          	addiw	a1,a5,-48
    1636:	0007871b          	sext.w	a4,a5
    163a:	feb665e3          	bltu	a2,a1,1624 <atoi+0x8a>
        neg = 1;
    163e:	4e05                	li	t3,1
    1640:	bf51                	j	15d4 <atoi+0x3a>

0000000000001642 <memset>:

void *memset(void *dest, int c, size_t n)
{
    char *p = dest;
    for (int i = 0; i < n; ++i, *(p++) = c)
    1642:	16060d63          	beqz	a2,17bc <memset+0x17a>
    1646:	40a007b3          	neg	a5,a0
    164a:	8b9d                	andi	a5,a5,7
    164c:	00778713          	addi	a4,a5,7
    1650:	482d                	li	a6,11
    1652:	0ff5f593          	andi	a1,a1,255
    1656:	fff60693          	addi	a3,a2,-1
    165a:	17076263          	bltu	a4,a6,17be <memset+0x17c>
    165e:	16e6ea63          	bltu	a3,a4,17d2 <memset+0x190>
    1662:	16078563          	beqz	a5,17cc <memset+0x18a>
    1666:	00b50023          	sb	a1,0(a0)
    166a:	4705                	li	a4,1
    166c:	00150e93          	addi	t4,a0,1
    1670:	14e78c63          	beq	a5,a4,17c8 <memset+0x186>
    1674:	00b500a3          	sb	a1,1(a0)
    1678:	4709                	li	a4,2
    167a:	00250e93          	addi	t4,a0,2
    167e:	14e78d63          	beq	a5,a4,17d8 <memset+0x196>
    1682:	00b50123          	sb	a1,2(a0)
    1686:	470d                	li	a4,3
    1688:	00350e93          	addi	t4,a0,3
    168c:	12e78b63          	beq	a5,a4,17c2 <memset+0x180>
    1690:	00b501a3          	sb	a1,3(a0)
    1694:	4711                	li	a4,4
    1696:	00450e93          	addi	t4,a0,4
    169a:	14e78163          	beq	a5,a4,17dc <memset+0x19a>
    169e:	00b50223          	sb	a1,4(a0)
    16a2:	4715                	li	a4,5
    16a4:	00550e93          	addi	t4,a0,5
    16a8:	12e78c63          	beq	a5,a4,17e0 <memset+0x19e>
    16ac:	00b502a3          	sb	a1,5(a0)
    16b0:	471d                	li	a4,7
    16b2:	00650e93          	addi	t4,a0,6
    16b6:	12e79763          	bne	a5,a4,17e4 <memset+0x1a2>
    16ba:	00750e93          	addi	t4,a0,7
    16be:	00b50323          	sb	a1,6(a0)
    16c2:	4f1d                	li	t5,7
    16c4:	00859713          	slli	a4,a1,0x8
    16c8:	8f4d                	or	a4,a4,a1
    16ca:	01059e13          	slli	t3,a1,0x10
    16ce:	01c76e33          	or	t3,a4,t3
    16d2:	01859313          	slli	t1,a1,0x18
    16d6:	006e6333          	or	t1,t3,t1
    16da:	02059893          	slli	a7,a1,0x20
    16de:	011368b3          	or	a7,t1,a7
    16e2:	02859813          	slli	a6,a1,0x28
    16e6:	40f60333          	sub	t1,a2,a5
    16ea:	0108e833          	or	a6,a7,a6
    16ee:	03059693          	slli	a3,a1,0x30
    16f2:	00d866b3          	or	a3,a6,a3
    16f6:	03859713          	slli	a4,a1,0x38
    16fa:	97aa                	add	a5,a5,a0
    16fc:	ff837813          	andi	a6,t1,-8
    1700:	8f55                	or	a4,a4,a3
    1702:	00f806b3          	add	a3,a6,a5
    1706:	e398                	sd	a4,0(a5)
    1708:	07a1                	addi	a5,a5,8
    170a:	fed79ee3          	bne	a5,a3,1706 <memset+0xc4>
    170e:	ff837693          	andi	a3,t1,-8
    1712:	00de87b3          	add	a5,t4,a3
    1716:	01e6873b          	addw	a4,a3,t5
    171a:	0ad30663          	beq	t1,a3,17c6 <memset+0x184>
    171e:	00b78023          	sb	a1,0(a5)
    1722:	0017069b          	addiw	a3,a4,1
    1726:	08c6fb63          	bgeu	a3,a2,17bc <memset+0x17a>
    172a:	00b780a3          	sb	a1,1(a5)
    172e:	0027069b          	addiw	a3,a4,2
    1732:	08c6f563          	bgeu	a3,a2,17bc <memset+0x17a>
    1736:	00b78123          	sb	a1,2(a5)
    173a:	0037069b          	addiw	a3,a4,3
    173e:	06c6ff63          	bgeu	a3,a2,17bc <memset+0x17a>
    1742:	00b781a3          	sb	a1,3(a5)
    1746:	0047069b          	addiw	a3,a4,4
    174a:	06c6f963          	bgeu	a3,a2,17bc <memset+0x17a>
    174e:	00b78223          	sb	a1,4(a5)
    1752:	0057069b          	addiw	a3,a4,5
    1756:	06c6f363          	bgeu	a3,a2,17bc <memset+0x17a>
    175a:	00b782a3          	sb	a1,5(a5)
    175e:	0067069b          	addiw	a3,a4,6
    1762:	04c6fd63          	bgeu	a3,a2,17bc <memset+0x17a>
    1766:	00b78323          	sb	a1,6(a5)
    176a:	0077069b          	addiw	a3,a4,7
    176e:	04c6f763          	bgeu	a3,a2,17bc <memset+0x17a>
    1772:	00b783a3          	sb	a1,7(a5)
    1776:	0087069b          	addiw	a3,a4,8
    177a:	04c6f163          	bgeu	a3,a2,17bc <memset+0x17a>
    177e:	00b78423          	sb	a1,8(a5)
    1782:	0097069b          	addiw	a3,a4,9
    1786:	02c6fb63          	bgeu	a3,a2,17bc <memset+0x17a>
    178a:	00b784a3          	sb	a1,9(a5)
    178e:	00a7069b          	addiw	a3,a4,10
    1792:	02c6f563          	bgeu	a3,a2,17bc <memset+0x17a>
    1796:	00b78523          	sb	a1,10(a5)
    179a:	00b7069b          	addiw	a3,a4,11
    179e:	00c6ff63          	bgeu	a3,a2,17bc <memset+0x17a>
    17a2:	00b785a3          	sb	a1,11(a5)
    17a6:	00c7069b          	addiw	a3,a4,12
    17aa:	00c6f963          	bgeu	a3,a2,17bc <memset+0x17a>
    17ae:	00b78623          	sb	a1,12(a5)
    17b2:	2735                	addiw	a4,a4,13
    17b4:	00c77463          	bgeu	a4,a2,17bc <memset+0x17a>
    17b8:	00b786a3          	sb	a1,13(a5)
        ;
    return dest;
}
    17bc:	8082                	ret
    17be:	472d                	li	a4,11
    17c0:	bd79                	j	165e <memset+0x1c>
    for (int i = 0; i < n; ++i, *(p++) = c)
    17c2:	4f0d                	li	t5,3
    17c4:	b701                	j	16c4 <memset+0x82>
    17c6:	8082                	ret
    17c8:	4f05                	li	t5,1
    17ca:	bded                	j	16c4 <memset+0x82>
    17cc:	8eaa                	mv	t4,a0
    17ce:	4f01                	li	t5,0
    17d0:	bdd5                	j	16c4 <memset+0x82>
    17d2:	87aa                	mv	a5,a0
    17d4:	4701                	li	a4,0
    17d6:	b7a1                	j	171e <memset+0xdc>
    17d8:	4f09                	li	t5,2
    17da:	b5ed                	j	16c4 <memset+0x82>
    17dc:	4f11                	li	t5,4
    17de:	b5dd                	j	16c4 <memset+0x82>
    17e0:	4f15                	li	t5,5
    17e2:	b5cd                	j	16c4 <memset+0x82>
    17e4:	4f19                	li	t5,6
    17e6:	bdf9                	j	16c4 <memset+0x82>

00000000000017e8 <strcmp>:

int strcmp(const char *l, const char *r)
{
    for (; *l == *r && *l; l++, r++)
    17e8:	00054783          	lbu	a5,0(a0)
    17ec:	0005c703          	lbu	a4,0(a1)
    17f0:	00e79863          	bne	a5,a4,1800 <strcmp+0x18>
    17f4:	0505                	addi	a0,a0,1
    17f6:	0585                	addi	a1,a1,1
    17f8:	fbe5                	bnez	a5,17e8 <strcmp>
    17fa:	4501                	li	a0,0
        ;
    return *(unsigned char *)l - *(unsigned char *)r;
}
    17fc:	9d19                	subw	a0,a0,a4
    17fe:	8082                	ret
    return *(unsigned char *)l - *(unsigned char *)r;
    1800:	0007851b          	sext.w	a0,a5
    1804:	bfe5                	j	17fc <strcmp+0x14>

0000000000001806 <strncmp>:

int strncmp(const char *_l, const char *_r, size_t n)
{
    const unsigned char *l = (void *)_l, *r = (void *)_r;
    if (!n--)
    1806:	ce05                	beqz	a2,183e <strncmp+0x38>
        return 0;
    for (; *l && *r && n && *l == *r; l++, r++, n--)
    1808:	00054703          	lbu	a4,0(a0)
    180c:	0005c783          	lbu	a5,0(a1)
    1810:	cb0d                	beqz	a4,1842 <strncmp+0x3c>
    if (!n--)
    1812:	167d                	addi	a2,a2,-1
    1814:	00c506b3          	add	a3,a0,a2
    1818:	a819                	j	182e <strncmp+0x28>
    for (; *l && *r && n && *l == *r; l++, r++, n--)
    181a:	00a68e63          	beq	a3,a0,1836 <strncmp+0x30>
    181e:	0505                	addi	a0,a0,1
    1820:	00e79b63          	bne	a5,a4,1836 <strncmp+0x30>
    1824:	00054703          	lbu	a4,0(a0)
        ;
    return *l - *r;
    1828:	0005c783          	lbu	a5,0(a1)
    for (; *l && *r && n && *l == *r; l++, r++, n--)
    182c:	cb19                	beqz	a4,1842 <strncmp+0x3c>
    182e:	0005c783          	lbu	a5,0(a1)
    1832:	0585                	addi	a1,a1,1
    1834:	f3fd                	bnez	a5,181a <strncmp+0x14>
    return *l - *r;
    1836:	0007051b          	sext.w	a0,a4
    183a:	9d1d                	subw	a0,a0,a5
    183c:	8082                	ret
        return 0;
    183e:	4501                	li	a0,0
}
    1840:	8082                	ret
    1842:	4501                	li	a0,0
    return *l - *r;
    1844:	9d1d                	subw	a0,a0,a5
    1846:	8082                	ret

0000000000001848 <strlen>:
size_t strlen(const char *s)
{
    const char *a = s;
    typedef size_t __attribute__((__may_alias__)) word;
    const word *w;
    for (; (uintptr_t)s % SS; s++)
    1848:	00757793          	andi	a5,a0,7
    184c:	cf89                	beqz	a5,1866 <strlen+0x1e>
    184e:	87aa                	mv	a5,a0
    1850:	a029                	j	185a <strlen+0x12>
    1852:	0785                	addi	a5,a5,1
    1854:	0077f713          	andi	a4,a5,7
    1858:	cb01                	beqz	a4,1868 <strlen+0x20>
        if (!*s)
    185a:	0007c703          	lbu	a4,0(a5)
    185e:	fb75                	bnez	a4,1852 <strlen+0xa>
    for (w = (const void *)s; !HASZERO(*w); w++)
        ;
    s = (const void *)w;
    for (; *s; s++)
        ;
    return s - a;
    1860:	40a78533          	sub	a0,a5,a0
}
    1864:	8082                	ret
    for (; (uintptr_t)s % SS; s++)
    1866:	87aa                	mv	a5,a0
    for (w = (const void *)s; !HASZERO(*w); w++)
    1868:	6394                	ld	a3,0(a5)
    186a:	00000597          	auipc	a1,0x0
    186e:	6765b583          	ld	a1,1654(a1) # 1ee0 <__clone+0x80>
    1872:	00000617          	auipc	a2,0x0
    1876:	67663603          	ld	a2,1654(a2) # 1ee8 <__clone+0x88>
    187a:	a019                	j	1880 <strlen+0x38>
    187c:	6794                	ld	a3,8(a5)
    187e:	07a1                	addi	a5,a5,8
    1880:	00b68733          	add	a4,a3,a1
    1884:	fff6c693          	not	a3,a3
    1888:	8f75                	and	a4,a4,a3
    188a:	8f71                	and	a4,a4,a2
    188c:	db65                	beqz	a4,187c <strlen+0x34>
    for (; *s; s++)
    188e:	0007c703          	lbu	a4,0(a5)
    1892:	d779                	beqz	a4,1860 <strlen+0x18>
    1894:	0017c703          	lbu	a4,1(a5)
    1898:	0785                	addi	a5,a5,1
    189a:	d379                	beqz	a4,1860 <strlen+0x18>
    189c:	0017c703          	lbu	a4,1(a5)
    18a0:	0785                	addi	a5,a5,1
    18a2:	fb6d                	bnez	a4,1894 <strlen+0x4c>
    18a4:	bf75                	j	1860 <strlen+0x18>

00000000000018a6 <memchr>:

void *memchr(const void *src, int c, size_t n)
{
    const unsigned char *s = src;
    c = (unsigned char)c;
    for (; ((uintptr_t)s & ALIGN) && n && *s != c; s++, n--)
    18a6:	00757713          	andi	a4,a0,7
{
    18aa:	87aa                	mv	a5,a0
    c = (unsigned char)c;
    18ac:	0ff5f593          	andi	a1,a1,255
    for (; ((uintptr_t)s & ALIGN) && n && *s != c; s++, n--)
    18b0:	cb19                	beqz	a4,18c6 <memchr+0x20>
    18b2:	ce25                	beqz	a2,192a <memchr+0x84>
    18b4:	0007c703          	lbu	a4,0(a5)
    18b8:	04b70e63          	beq	a4,a1,1914 <memchr+0x6e>
    18bc:	0785                	addi	a5,a5,1
    18be:	0077f713          	andi	a4,a5,7
    18c2:	167d                	addi	a2,a2,-1
    18c4:	f77d                	bnez	a4,18b2 <memchr+0xc>
            ;
        s = (const void *)w;
    }
    for (; n && *s != c; s++, n--)
        ;
    return n ? (void *)s : 0;
    18c6:	4501                	li	a0,0
    if (n && *s != c)
    18c8:	c235                	beqz	a2,192c <memchr+0x86>
    18ca:	0007c703          	lbu	a4,0(a5)
    18ce:	04b70363          	beq	a4,a1,1914 <memchr+0x6e>
        size_t k = ONES * c;
    18d2:	00000517          	auipc	a0,0x0
    18d6:	61e53503          	ld	a0,1566(a0) # 1ef0 <__clone+0x90>
        for (w = (const void *)s; n >= SS && !HASZERO(*w ^ k); w++, n -= SS)
    18da:	471d                	li	a4,7
        size_t k = ONES * c;
    18dc:	02a58533          	mul	a0,a1,a0
        for (w = (const void *)s; n >= SS && !HASZERO(*w ^ k); w++, n -= SS)
    18e0:	02c77a63          	bgeu	a4,a2,1914 <memchr+0x6e>
    18e4:	00000897          	auipc	a7,0x0
    18e8:	5fc8b883          	ld	a7,1532(a7) # 1ee0 <__clone+0x80>
    18ec:	00000817          	auipc	a6,0x0
    18f0:	5fc83803          	ld	a6,1532(a6) # 1ee8 <__clone+0x88>
    18f4:	431d                	li	t1,7
    18f6:	a029                	j	1900 <memchr+0x5a>
    18f8:	1661                	addi	a2,a2,-8
    18fa:	07a1                	addi	a5,a5,8
    18fc:	02c37963          	bgeu	t1,a2,192e <memchr+0x88>
    1900:	6398                	ld	a4,0(a5)
    1902:	8f29                	xor	a4,a4,a0
    1904:	011706b3          	add	a3,a4,a7
    1908:	fff74713          	not	a4,a4
    190c:	8f75                	and	a4,a4,a3
    190e:	01077733          	and	a4,a4,a6
    1912:	d37d                	beqz	a4,18f8 <memchr+0x52>
    1914:	853e                	mv	a0,a5
    1916:	97b2                	add	a5,a5,a2
    1918:	a021                	j	1920 <memchr+0x7a>
    for (; n && *s != c; s++, n--)
    191a:	0505                	addi	a0,a0,1
    191c:	00f50763          	beq	a0,a5,192a <memchr+0x84>
    1920:	00054703          	lbu	a4,0(a0)
    1924:	feb71be3          	bne	a4,a1,191a <memchr+0x74>
    1928:	8082                	ret
    return n ? (void *)s : 0;
    192a:	4501                	li	a0,0
}
    192c:	8082                	ret
    return n ? (void *)s : 0;
    192e:	4501                	li	a0,0
    for (; n && *s != c; s++, n--)
    1930:	f275                	bnez	a2,1914 <memchr+0x6e>
}
    1932:	8082                	ret

0000000000001934 <strnlen>:

size_t strnlen(const char *s, size_t n)
{
    1934:	1101                	addi	sp,sp,-32
    1936:	e822                	sd	s0,16(sp)
    const char *p = memchr(s, 0, n);
    1938:	862e                	mv	a2,a1
{
    193a:	842e                	mv	s0,a1
    const char *p = memchr(s, 0, n);
    193c:	4581                	li	a1,0
{
    193e:	e426                	sd	s1,8(sp)
    1940:	ec06                	sd	ra,24(sp)
    1942:	84aa                	mv	s1,a0
    const char *p = memchr(s, 0, n);
    1944:	f63ff0ef          	jal	ra,18a6 <memchr>
    return p ? p - s : n;
    1948:	c519                	beqz	a0,1956 <strnlen+0x22>
}
    194a:	60e2                	ld	ra,24(sp)
    194c:	6442                	ld	s0,16(sp)
    return p ? p - s : n;
    194e:	8d05                	sub	a0,a0,s1
}
    1950:	64a2                	ld	s1,8(sp)
    1952:	6105                	addi	sp,sp,32
    1954:	8082                	ret
    1956:	60e2                	ld	ra,24(sp)
    return p ? p - s : n;
    1958:	8522                	mv	a0,s0
}
    195a:	6442                	ld	s0,16(sp)
    195c:	64a2                	ld	s1,8(sp)
    195e:	6105                	addi	sp,sp,32
    1960:	8082                	ret

0000000000001962 <strcpy>:
char *strcpy(char *restrict d, const char *s)
{
    typedef size_t __attribute__((__may_alias__)) word;
    word *wd;
    const word *ws;
    if ((uintptr_t)s % SS == (uintptr_t)d % SS)
    1962:	00b547b3          	xor	a5,a0,a1
    1966:	8b9d                	andi	a5,a5,7
    1968:	eb95                	bnez	a5,199c <strcpy+0x3a>
    {
        for (; (uintptr_t)s % SS; s++, d++)
    196a:	0075f793          	andi	a5,a1,7
    196e:	e7b1                	bnez	a5,19ba <strcpy+0x58>
            if (!(*d = *s))
                return d;
        wd = (void *)d;
        ws = (const void *)s;
        for (; !HASZERO(*ws); *wd++ = *ws++)
    1970:	6198                	ld	a4,0(a1)
    1972:	00000617          	auipc	a2,0x0
    1976:	56e63603          	ld	a2,1390(a2) # 1ee0 <__clone+0x80>
    197a:	00000817          	auipc	a6,0x0
    197e:	56e83803          	ld	a6,1390(a6) # 1ee8 <__clone+0x88>
    1982:	a029                	j	198c <strcpy+0x2a>
    1984:	e118                	sd	a4,0(a0)
    1986:	6598                	ld	a4,8(a1)
    1988:	05a1                	addi	a1,a1,8
    198a:	0521                	addi	a0,a0,8
    198c:	00c707b3          	add	a5,a4,a2
    1990:	fff74693          	not	a3,a4
    1994:	8ff5                	and	a5,a5,a3
    1996:	0107f7b3          	and	a5,a5,a6
    199a:	d7ed                	beqz	a5,1984 <strcpy+0x22>
            ;
        d = (void *)wd;
        s = (const void *)ws;
    }
    for (; (*d = *s); s++, d++)
    199c:	0005c783          	lbu	a5,0(a1)
    19a0:	00f50023          	sb	a5,0(a0)
    19a4:	c785                	beqz	a5,19cc <strcpy+0x6a>
    19a6:	0015c783          	lbu	a5,1(a1)
    19aa:	0505                	addi	a0,a0,1
    19ac:	0585                	addi	a1,a1,1
    19ae:	00f50023          	sb	a5,0(a0)
    19b2:	fbf5                	bnez	a5,19a6 <strcpy+0x44>
        ;
    return d;
}
    19b4:	8082                	ret
        for (; (uintptr_t)s % SS; s++, d++)
    19b6:	0505                	addi	a0,a0,1
    19b8:	df45                	beqz	a4,1970 <strcpy+0xe>
            if (!(*d = *s))
    19ba:	0005c783          	lbu	a5,0(a1)
        for (; (uintptr_t)s % SS; s++, d++)
    19be:	0585                	addi	a1,a1,1
    19c0:	0075f713          	andi	a4,a1,7
            if (!(*d = *s))
    19c4:	00f50023          	sb	a5,0(a0)
    19c8:	f7fd                	bnez	a5,19b6 <strcpy+0x54>
}
    19ca:	8082                	ret
    19cc:	8082                	ret

00000000000019ce <strncpy>:
char *strncpy(char *restrict d, const char *s, size_t n)
{
    typedef size_t __attribute__((__may_alias__)) word;
    word *wd;
    const word *ws;
    if (((uintptr_t)s & ALIGN) == ((uintptr_t)d & ALIGN))
    19ce:	00b547b3          	xor	a5,a0,a1
    19d2:	8b9d                	andi	a5,a5,7
    19d4:	1a079863          	bnez	a5,1b84 <strncpy+0x1b6>
    {
        for (; ((uintptr_t)s & ALIGN) && n && (*d = *s); n--, s++, d++)
    19d8:	0075f793          	andi	a5,a1,7
    19dc:	16078463          	beqz	a5,1b44 <strncpy+0x176>
    19e0:	ea01                	bnez	a2,19f0 <strncpy+0x22>
    19e2:	a421                	j	1bea <strncpy+0x21c>
    19e4:	167d                	addi	a2,a2,-1
    19e6:	0505                	addi	a0,a0,1
    19e8:	14070e63          	beqz	a4,1b44 <strncpy+0x176>
    19ec:	1a060863          	beqz	a2,1b9c <strncpy+0x1ce>
    19f0:	0005c783          	lbu	a5,0(a1)
    19f4:	0585                	addi	a1,a1,1
    19f6:	0075f713          	andi	a4,a1,7
    19fa:	00f50023          	sb	a5,0(a0)
    19fe:	f3fd                	bnez	a5,19e4 <strncpy+0x16>
    1a00:	4805                	li	a6,1
    1a02:	1a061863          	bnez	a2,1bb2 <strncpy+0x1e4>
    1a06:	40a007b3          	neg	a5,a0
    1a0a:	8b9d                	andi	a5,a5,7
    1a0c:	4681                	li	a3,0
    1a0e:	18061a63          	bnez	a2,1ba2 <strncpy+0x1d4>
    1a12:	00778713          	addi	a4,a5,7
    1a16:	45ad                	li	a1,11
    1a18:	18b76363          	bltu	a4,a1,1b9e <strncpy+0x1d0>
    1a1c:	1ae6eb63          	bltu	a3,a4,1bd2 <strncpy+0x204>
    1a20:	1a078363          	beqz	a5,1bc6 <strncpy+0x1f8>
    for (int i = 0; i < n; ++i, *(p++) = c)
    1a24:	00050023          	sb	zero,0(a0)
    1a28:	4685                	li	a3,1
    1a2a:	00150713          	addi	a4,a0,1
    1a2e:	18d78f63          	beq	a5,a3,1bcc <strncpy+0x1fe>
    1a32:	000500a3          	sb	zero,1(a0)
    1a36:	4689                	li	a3,2
    1a38:	00250713          	addi	a4,a0,2
    1a3c:	18d78e63          	beq	a5,a3,1bd8 <strncpy+0x20a>
    1a40:	00050123          	sb	zero,2(a0)
    1a44:	468d                	li	a3,3
    1a46:	00350713          	addi	a4,a0,3
    1a4a:	16d78c63          	beq	a5,a3,1bc2 <strncpy+0x1f4>
    1a4e:	000501a3          	sb	zero,3(a0)
    1a52:	4691                	li	a3,4
    1a54:	00450713          	addi	a4,a0,4
    1a58:	18d78263          	beq	a5,a3,1bdc <strncpy+0x20e>
    1a5c:	00050223          	sb	zero,4(a0)
    1a60:	4695                	li	a3,5
    1a62:	00550713          	addi	a4,a0,5
    1a66:	16d78d63          	beq	a5,a3,1be0 <strncpy+0x212>
    1a6a:	000502a3          	sb	zero,5(a0)
    1a6e:	469d                	li	a3,7
    1a70:	00650713          	addi	a4,a0,6
    1a74:	16d79863          	bne	a5,a3,1be4 <strncpy+0x216>
    1a78:	00750713          	addi	a4,a0,7
    1a7c:	00050323          	sb	zero,6(a0)
    1a80:	40f80833          	sub	a6,a6,a5
    1a84:	ff887593          	andi	a1,a6,-8
    1a88:	97aa                	add	a5,a5,a0
    1a8a:	95be                	add	a1,a1,a5
    1a8c:	0007b023          	sd	zero,0(a5)
    1a90:	07a1                	addi	a5,a5,8
    1a92:	feb79de3          	bne	a5,a1,1a8c <strncpy+0xbe>
    1a96:	ff887593          	andi	a1,a6,-8
    1a9a:	9ead                	addw	a3,a3,a1
    1a9c:	00b707b3          	add	a5,a4,a1
    1aa0:	12b80863          	beq	a6,a1,1bd0 <strncpy+0x202>
    1aa4:	00078023          	sb	zero,0(a5)
    1aa8:	0016871b          	addiw	a4,a3,1
    1aac:	0ec77863          	bgeu	a4,a2,1b9c <strncpy+0x1ce>
    1ab0:	000780a3          	sb	zero,1(a5)
    1ab4:	0026871b          	addiw	a4,a3,2
    1ab8:	0ec77263          	bgeu	a4,a2,1b9c <strncpy+0x1ce>
    1abc:	00078123          	sb	zero,2(a5)
    1ac0:	0036871b          	addiw	a4,a3,3
    1ac4:	0cc77c63          	bgeu	a4,a2,1b9c <strncpy+0x1ce>
    1ac8:	000781a3          	sb	zero,3(a5)
    1acc:	0046871b          	addiw	a4,a3,4
    1ad0:	0cc77663          	bgeu	a4,a2,1b9c <strncpy+0x1ce>
    1ad4:	00078223          	sb	zero,4(a5)
    1ad8:	0056871b          	addiw	a4,a3,5
    1adc:	0cc77063          	bgeu	a4,a2,1b9c <strncpy+0x1ce>
    1ae0:	000782a3          	sb	zero,5(a5)
    1ae4:	0066871b          	addiw	a4,a3,6
    1ae8:	0ac77a63          	bgeu	a4,a2,1b9c <strncpy+0x1ce>
    1aec:	00078323          	sb	zero,6(a5)
    1af0:	0076871b          	addiw	a4,a3,7
    1af4:	0ac77463          	bgeu	a4,a2,1b9c <strncpy+0x1ce>
    1af8:	000783a3          	sb	zero,7(a5)
    1afc:	0086871b          	addiw	a4,a3,8
    1b00:	08c77e63          	bgeu	a4,a2,1b9c <strncpy+0x1ce>
    1b04:	00078423          	sb	zero,8(a5)
    1b08:	0096871b          	addiw	a4,a3,9
    1b0c:	08c77863          	bgeu	a4,a2,1b9c <strncpy+0x1ce>
    1b10:	000784a3          	sb	zero,9(a5)
    1b14:	00a6871b          	addiw	a4,a3,10
    1b18:	08c77263          	bgeu	a4,a2,1b9c <strncpy+0x1ce>
    1b1c:	00078523          	sb	zero,10(a5)
    1b20:	00b6871b          	addiw	a4,a3,11
    1b24:	06c77c63          	bgeu	a4,a2,1b9c <strncpy+0x1ce>
    1b28:	000785a3          	sb	zero,11(a5)
    1b2c:	00c6871b          	addiw	a4,a3,12
    1b30:	06c77663          	bgeu	a4,a2,1b9c <strncpy+0x1ce>
    1b34:	00078623          	sb	zero,12(a5)
    1b38:	26b5                	addiw	a3,a3,13
    1b3a:	06c6f163          	bgeu	a3,a2,1b9c <strncpy+0x1ce>
    1b3e:	000786a3          	sb	zero,13(a5)
    1b42:	8082                	ret
            ;
        if (!n || !*s)
    1b44:	c645                	beqz	a2,1bec <strncpy+0x21e>
    1b46:	0005c783          	lbu	a5,0(a1)
    1b4a:	ea078be3          	beqz	a5,1a00 <strncpy+0x32>
            goto tail;
        wd = (void *)d;
        ws = (const void *)s;
        for (; n >= sizeof(size_t) && !HASZERO(*ws); n -= sizeof(size_t), ws++, wd++)
    1b4e:	479d                	li	a5,7
    1b50:	02c7ff63          	bgeu	a5,a2,1b8e <strncpy+0x1c0>
    1b54:	00000897          	auipc	a7,0x0
    1b58:	38c8b883          	ld	a7,908(a7) # 1ee0 <__clone+0x80>
    1b5c:	00000817          	auipc	a6,0x0
    1b60:	38c83803          	ld	a6,908(a6) # 1ee8 <__clone+0x88>
    1b64:	431d                	li	t1,7
    1b66:	6198                	ld	a4,0(a1)
    1b68:	011707b3          	add	a5,a4,a7
    1b6c:	fff74693          	not	a3,a4
    1b70:	8ff5                	and	a5,a5,a3
    1b72:	0107f7b3          	and	a5,a5,a6
    1b76:	ef81                	bnez	a5,1b8e <strncpy+0x1c0>
            *wd = *ws;
    1b78:	e118                	sd	a4,0(a0)
        for (; n >= sizeof(size_t) && !HASZERO(*ws); n -= sizeof(size_t), ws++, wd++)
    1b7a:	1661                	addi	a2,a2,-8
    1b7c:	05a1                	addi	a1,a1,8
    1b7e:	0521                	addi	a0,a0,8
    1b80:	fec363e3          	bltu	t1,a2,1b66 <strncpy+0x198>
        d = (void *)wd;
        s = (const void *)ws;
    }
    for (; n && (*d = *s); n--, s++, d++)
    1b84:	e609                	bnez	a2,1b8e <strncpy+0x1c0>
    1b86:	a08d                	j	1be8 <strncpy+0x21a>
    1b88:	167d                	addi	a2,a2,-1
    1b8a:	0505                	addi	a0,a0,1
    1b8c:	ca01                	beqz	a2,1b9c <strncpy+0x1ce>
    1b8e:	0005c783          	lbu	a5,0(a1)
    1b92:	0585                	addi	a1,a1,1
    1b94:	00f50023          	sb	a5,0(a0)
    1b98:	fbe5                	bnez	a5,1b88 <strncpy+0x1ba>
        ;
tail:
    1b9a:	b59d                	j	1a00 <strncpy+0x32>
    memset(d, 0, n);
    return d;
}
    1b9c:	8082                	ret
    1b9e:	472d                	li	a4,11
    1ba0:	bdb5                	j	1a1c <strncpy+0x4e>
    1ba2:	00778713          	addi	a4,a5,7
    1ba6:	45ad                	li	a1,11
    1ba8:	fff60693          	addi	a3,a2,-1
    1bac:	e6b778e3          	bgeu	a4,a1,1a1c <strncpy+0x4e>
    1bb0:	b7fd                	j	1b9e <strncpy+0x1d0>
    1bb2:	40a007b3          	neg	a5,a0
    1bb6:	8832                	mv	a6,a2
    1bb8:	8b9d                	andi	a5,a5,7
    1bba:	4681                	li	a3,0
    1bbc:	e4060be3          	beqz	a2,1a12 <strncpy+0x44>
    1bc0:	b7cd                	j	1ba2 <strncpy+0x1d4>
    for (int i = 0; i < n; ++i, *(p++) = c)
    1bc2:	468d                	li	a3,3
    1bc4:	bd75                	j	1a80 <strncpy+0xb2>
    1bc6:	872a                	mv	a4,a0
    1bc8:	4681                	li	a3,0
    1bca:	bd5d                	j	1a80 <strncpy+0xb2>
    1bcc:	4685                	li	a3,1
    1bce:	bd4d                	j	1a80 <strncpy+0xb2>
    1bd0:	8082                	ret
    1bd2:	87aa                	mv	a5,a0
    1bd4:	4681                	li	a3,0
    1bd6:	b5f9                	j	1aa4 <strncpy+0xd6>
    1bd8:	4689                	li	a3,2
    1bda:	b55d                	j	1a80 <strncpy+0xb2>
    1bdc:	4691                	li	a3,4
    1bde:	b54d                	j	1a80 <strncpy+0xb2>
    1be0:	4695                	li	a3,5
    1be2:	bd79                	j	1a80 <strncpy+0xb2>
    1be4:	4699                	li	a3,6
    1be6:	bd69                	j	1a80 <strncpy+0xb2>
    1be8:	8082                	ret
    1bea:	8082                	ret
    1bec:	8082                	ret

0000000000001bee <open>:
#include <unistd.h>

#include "syscall.h"

int open(const char *path, int flags)
{
    1bee:	87aa                	mv	a5,a0
    1bf0:	862e                	mv	a2,a1
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
}

static inline long __syscall4(long n, long a, long b, long c, long d)
{
    register long a7 __asm__("a7") = n;
    1bf2:	03800893          	li	a7,56
    register long a0 __asm__("a0") = a;
    1bf6:	f9c00513          	li	a0,-100
    register long a1 __asm__("a1") = b;
    1bfa:	85be                	mv	a1,a5
    register long a2 __asm__("a2") = c;
    register long a3 __asm__("a3") = d;
    1bfc:	4689                	li	a3,2
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3))
    1bfe:	00000073          	ecall
    return syscall(SYS_openat, AT_FDCWD, path, flags, O_RDWR);
}
    1c02:	2501                	sext.w	a0,a0
    1c04:	8082                	ret

0000000000001c06 <openat>:
    register long a7 __asm__("a7") = n;
    1c06:	03800893          	li	a7,56
    register long a3 __asm__("a3") = d;
    1c0a:	18000693          	li	a3,384
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3))
    1c0e:	00000073          	ecall

int openat(int dirfd,const char *path, int flags)
{
    return syscall(SYS_openat, dirfd, path, flags, 0600);
}
    1c12:	2501                	sext.w	a0,a0
    1c14:	8082                	ret

0000000000001c16 <close>:
    register long a7 __asm__("a7") = n;
    1c16:	03900893          	li	a7,57
    __asm_syscall("r"(a7), "0"(a0))
    1c1a:	00000073          	ecall

int close(int fd)
{
    return syscall(SYS_close, fd);
}
    1c1e:	2501                	sext.w	a0,a0
    1c20:	8082                	ret

0000000000001c22 <read>:
    register long a7 __asm__("a7") = n;
    1c22:	03f00893          	li	a7,63
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1c26:	00000073          	ecall

ssize_t read(int fd, void *buf, size_t len)
{
    return syscall(SYS_read, fd, buf, len);
}
    1c2a:	8082                	ret

0000000000001c2c <write>:
    register long a7 __asm__("a7") = n;
    1c2c:	04000893          	li	a7,64
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1c30:	00000073          	ecall

ssize_t write(int fd, const void *buf, size_t len)
{
    return syscall(SYS_write, fd, buf, len);
}
    1c34:	8082                	ret

0000000000001c36 <getpid>:
    register long a7 __asm__("a7") = n;
    1c36:	0ac00893          	li	a7,172
    __asm_syscall("r"(a7))
    1c3a:	00000073          	ecall

pid_t getpid(void)
{
    return syscall(SYS_getpid);
}
    1c3e:	2501                	sext.w	a0,a0
    1c40:	8082                	ret

0000000000001c42 <getppid>:
    register long a7 __asm__("a7") = n;
    1c42:	0ad00893          	li	a7,173
    __asm_syscall("r"(a7))
    1c46:	00000073          	ecall

pid_t getppid(void)
{
    return syscall(SYS_getppid);
}
    1c4a:	2501                	sext.w	a0,a0
    1c4c:	8082                	ret

0000000000001c4e <sched_yield>:
    register long a7 __asm__("a7") = n;
    1c4e:	07c00893          	li	a7,124
    __asm_syscall("r"(a7))
    1c52:	00000073          	ecall

int sched_yield(void)
{
    return syscall(SYS_sched_yield);
}
    1c56:	2501                	sext.w	a0,a0
    1c58:	8082                	ret

0000000000001c5a <fork>:
    register long a7 __asm__("a7") = n;
    1c5a:	0dc00893          	li	a7,220
    register long a0 __asm__("a0") = a;
    1c5e:	4545                	li	a0,17
    register long a1 __asm__("a1") = b;
    1c60:	4581                	li	a1,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1c62:	00000073          	ecall

pid_t fork(void)
{
    return syscall(SYS_clone, SIGCHLD, 0);
}
    1c66:	2501                	sext.w	a0,a0
    1c68:	8082                	ret

0000000000001c6a <clone>:

pid_t clone(int (*fn)(void *arg), void *arg, void *stack, size_t stack_size, unsigned long flags)
{
    1c6a:	85b2                	mv	a1,a2
    1c6c:	863a                	mv	a2,a4
    if (stack)
    1c6e:	c191                	beqz	a1,1c72 <clone+0x8>
	stack += stack_size;
    1c70:	95b6                	add	a1,a1,a3

    return __clone(fn, stack, flags, NULL, NULL, NULL);
    1c72:	4781                	li	a5,0
    1c74:	4701                	li	a4,0
    1c76:	4681                	li	a3,0
    1c78:	2601                	sext.w	a2,a2
    1c7a:	a2dd                	j	1e60 <__clone>

0000000000001c7c <exit>:
    register long a7 __asm__("a7") = n;
    1c7c:	05d00893          	li	a7,93
    __asm_syscall("r"(a7), "0"(a0))
    1c80:	00000073          	ecall
    //return syscall(SYS_clone, fn, stack, flags, NULL, NULL, NULL);
}
void exit(int code)
{
    syscall(SYS_exit, code);
}
    1c84:	8082                	ret

0000000000001c86 <waitpid>:
    register long a7 __asm__("a7") = n;
    1c86:	10400893          	li	a7,260
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1c8a:	00000073          	ecall

int waitpid(int pid, int *code, int options)
{
    return syscall(SYS_wait4, pid, code, options);
}
    1c8e:	2501                	sext.w	a0,a0
    1c90:	8082                	ret

0000000000001c92 <exec>:
    register long a7 __asm__("a7") = n;
    1c92:	0dd00893          	li	a7,221
    __asm_syscall("r"(a7), "0"(a0))
    1c96:	00000073          	ecall

int exec(char *name)
{
    return syscall(SYS_execve, name);
}
    1c9a:	2501                	sext.w	a0,a0
    1c9c:	8082                	ret

0000000000001c9e <execve>:
    register long a7 __asm__("a7") = n;
    1c9e:	0dd00893          	li	a7,221
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1ca2:	00000073          	ecall

int execve(const char *name, char *const argv[], char *const argp[])
{
    return syscall(SYS_execve, name, argv, argp);
}
    1ca6:	2501                	sext.w	a0,a0
    1ca8:	8082                	ret

0000000000001caa <times>:
    register long a7 __asm__("a7") = n;
    1caa:	09900893          	li	a7,153
    __asm_syscall("r"(a7), "0"(a0))
    1cae:	00000073          	ecall

int times(void *mytimes)
{
	return syscall(SYS_times, mytimes);
}
    1cb2:	2501                	sext.w	a0,a0
    1cb4:	8082                	ret

0000000000001cb6 <get_time>:

int64 get_time()
{
    1cb6:	1141                	addi	sp,sp,-16
    register long a7 __asm__("a7") = n;
    1cb8:	0a900893          	li	a7,169
    register long a0 __asm__("a0") = a;
    1cbc:	850a                	mv	a0,sp
    register long a1 __asm__("a1") = b;
    1cbe:	4581                	li	a1,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1cc0:	00000073          	ecall
    TimeVal time;
    int err = sys_get_time(&time, 0);
    if (err == 0)
    1cc4:	2501                	sext.w	a0,a0
    1cc6:	ed09                	bnez	a0,1ce0 <get_time+0x2a>
    {
        return ((time.sec & 0xffff) * 1000 + time.usec / 1000);
    1cc8:	67a2                	ld	a5,8(sp)
    1cca:	3e800713          	li	a4,1000
    1cce:	00015503          	lhu	a0,0(sp)
    1cd2:	02e7d7b3          	divu	a5,a5,a4
    1cd6:	02e50533          	mul	a0,a0,a4
    1cda:	953e                	add	a0,a0,a5
    }
    else
    {
        return -1;
    }
}
    1cdc:	0141                	addi	sp,sp,16
    1cde:	8082                	ret
        return -1;
    1ce0:	557d                	li	a0,-1
    1ce2:	bfed                	j	1cdc <get_time+0x26>

0000000000001ce4 <sys_get_time>:
    register long a7 __asm__("a7") = n;
    1ce4:	0a900893          	li	a7,169
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1ce8:	00000073          	ecall

int sys_get_time(TimeVal *ts, int tz)
{
    return syscall(SYS_gettimeofday, ts, tz);
}
    1cec:	2501                	sext.w	a0,a0
    1cee:	8082                	ret

0000000000001cf0 <time>:
    register long a7 __asm__("a7") = n;
    1cf0:	42600893          	li	a7,1062
    __asm_syscall("r"(a7), "0"(a0))
    1cf4:	00000073          	ecall

int time(unsigned long *tloc)
{
    return syscall(SYS_time, tloc);
}
    1cf8:	2501                	sext.w	a0,a0
    1cfa:	8082                	ret

0000000000001cfc <sleep>:

int sleep(unsigned long long time)
{
    1cfc:	1141                	addi	sp,sp,-16
    TimeVal tv = {.sec = time, .usec = 0};
    1cfe:	e02a                	sd	a0,0(sp)
    register long a0 __asm__("a0") = a;
    1d00:	850a                	mv	a0,sp
    1d02:	e402                	sd	zero,8(sp)
    register long a7 __asm__("a7") = n;
    1d04:	06500893          	li	a7,101
    register long a1 __asm__("a1") = b;
    1d08:	85aa                	mv	a1,a0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1d0a:	00000073          	ecall
    if (syscall(SYS_nanosleep, &tv, &tv)) return tv.sec;
    1d0e:	e501                	bnez	a0,1d16 <sleep+0x1a>
    return 0;
    1d10:	4501                	li	a0,0
}
    1d12:	0141                	addi	sp,sp,16
    1d14:	8082                	ret
    if (syscall(SYS_nanosleep, &tv, &tv)) return tv.sec;
    1d16:	4502                	lw	a0,0(sp)
}
    1d18:	0141                	addi	sp,sp,16
    1d1a:	8082                	ret

0000000000001d1c <set_priority>:
    register long a7 __asm__("a7") = n;
    1d1c:	08c00893          	li	a7,140
    __asm_syscall("r"(a7), "0"(a0))
    1d20:	00000073          	ecall

int set_priority(int prio)
{
    return syscall(SYS_setpriority, prio);
}
    1d24:	2501                	sext.w	a0,a0
    1d26:	8082                	ret

0000000000001d28 <mmap>:
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4))
}

static inline long __syscall6(long n, long a, long b, long c, long d, long e, long f)
{
    register long a7 __asm__("a7") = n;
    1d28:	0de00893          	li	a7,222
    register long a1 __asm__("a1") = b;
    register long a2 __asm__("a2") = c;
    register long a3 __asm__("a3") = d;
    register long a4 __asm__("a4") = e;
    register long a5 __asm__("a5") = f;
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4), "r"(a5))
    1d2c:	00000073          	ecall

void *mmap(void *start, size_t len, int prot, int flags, int fd, off_t off)
{
    return syscall(SYS_mmap, start, len, prot, flags, fd, off);
}
    1d30:	8082                	ret

0000000000001d32 <munmap>:
    register long a7 __asm__("a7") = n;
    1d32:	0d700893          	li	a7,215
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1d36:	00000073          	ecall

int munmap(void *start, size_t len)
{
    return syscall(SYS_munmap, start, len);
}
    1d3a:	2501                	sext.w	a0,a0
    1d3c:	8082                	ret

0000000000001d3e <wait>:

int wait(int *code)
{
    1d3e:	85aa                	mv	a1,a0
    register long a7 __asm__("a7") = n;
    1d40:	10400893          	li	a7,260
    register long a0 __asm__("a0") = a;
    1d44:	557d                	li	a0,-1
    register long a2 __asm__("a2") = c;
    1d46:	4601                	li	a2,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1d48:	00000073          	ecall
    return waitpid(-1, code, 0);
}
    1d4c:	2501                	sext.w	a0,a0
    1d4e:	8082                	ret

0000000000001d50 <spawn>:
    register long a7 __asm__("a7") = n;
    1d50:	19000893          	li	a7,400
    __asm_syscall("r"(a7), "0"(a0))
    1d54:	00000073          	ecall

int spawn(char *file)
{
    return syscall(SYS_spawn, file);
}
    1d58:	2501                	sext.w	a0,a0
    1d5a:	8082                	ret

0000000000001d5c <mailread>:
    register long a7 __asm__("a7") = n;
    1d5c:	19100893          	li	a7,401
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1d60:	00000073          	ecall

int mailread(void *buf, int len)
{
    return syscall(SYS_mailread, buf, len);
}
    1d64:	2501                	sext.w	a0,a0
    1d66:	8082                	ret

0000000000001d68 <mailwrite>:
    register long a7 __asm__("a7") = n;
    1d68:	19200893          	li	a7,402
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1d6c:	00000073          	ecall

int mailwrite(int pid, void *buf, int len)
{
    return syscall(SYS_mailwrite, pid, buf, len);
}
    1d70:	2501                	sext.w	a0,a0
    1d72:	8082                	ret

0000000000001d74 <fstat>:
    register long a7 __asm__("a7") = n;
    1d74:	05000893          	li	a7,80
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1d78:	00000073          	ecall

int fstat(int fd, struct kstat *st)
{
    return syscall(SYS_fstat, fd, st);
}
    1d7c:	2501                	sext.w	a0,a0
    1d7e:	8082                	ret

0000000000001d80 <sys_linkat>:
    register long a4 __asm__("a4") = e;
    1d80:	1702                	slli	a4,a4,0x20
    register long a7 __asm__("a7") = n;
    1d82:	02500893          	li	a7,37
    register long a4 __asm__("a4") = e;
    1d86:	9301                	srli	a4,a4,0x20
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4))
    1d88:	00000073          	ecall

int sys_linkat(int olddirfd, char *oldpath, int newdirfd, char *newpath, unsigned int flags)
{
    return syscall(SYS_linkat, olddirfd, oldpath, newdirfd, newpath, flags);
}
    1d8c:	2501                	sext.w	a0,a0
    1d8e:	8082                	ret

0000000000001d90 <sys_unlinkat>:
    register long a2 __asm__("a2") = c;
    1d90:	1602                	slli	a2,a2,0x20
    register long a7 __asm__("a7") = n;
    1d92:	02300893          	li	a7,35
    register long a2 __asm__("a2") = c;
    1d96:	9201                	srli	a2,a2,0x20
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1d98:	00000073          	ecall

int sys_unlinkat(int dirfd, char *path, unsigned int flags)
{
    return syscall(SYS_unlinkat, dirfd, path, flags);
}
    1d9c:	2501                	sext.w	a0,a0
    1d9e:	8082                	ret

0000000000001da0 <link>:

int link(char *old_path, char *new_path)
{
    1da0:	87aa                	mv	a5,a0
    1da2:	86ae                	mv	a3,a1
    register long a7 __asm__("a7") = n;
    1da4:	02500893          	li	a7,37
    register long a0 __asm__("a0") = a;
    1da8:	f9c00513          	li	a0,-100
    register long a1 __asm__("a1") = b;
    1dac:	85be                	mv	a1,a5
    register long a2 __asm__("a2") = c;
    1dae:	f9c00613          	li	a2,-100
    register long a4 __asm__("a4") = e;
    1db2:	4701                	li	a4,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4))
    1db4:	00000073          	ecall
    return sys_linkat(AT_FDCWD, old_path, AT_FDCWD, new_path, 0);
}
    1db8:	2501                	sext.w	a0,a0
    1dba:	8082                	ret

0000000000001dbc <unlink>:

int unlink(char *path)
{
    1dbc:	85aa                	mv	a1,a0
    register long a7 __asm__("a7") = n;
    1dbe:	02300893          	li	a7,35
    register long a0 __asm__("a0") = a;
    1dc2:	f9c00513          	li	a0,-100
    register long a2 __asm__("a2") = c;
    1dc6:	4601                	li	a2,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1dc8:	00000073          	ecall
    return sys_unlinkat(AT_FDCWD, path, 0);
}
    1dcc:	2501                	sext.w	a0,a0
    1dce:	8082                	ret

0000000000001dd0 <uname>:
    register long a7 __asm__("a7") = n;
    1dd0:	0a000893          	li	a7,160
    __asm_syscall("r"(a7), "0"(a0))
    1dd4:	00000073          	ecall

int uname(void *buf)
{
    return syscall(SYS_uname, buf);
}
    1dd8:	2501                	sext.w	a0,a0
    1dda:	8082                	ret

0000000000001ddc <brk>:
    register long a7 __asm__("a7") = n;
    1ddc:	0d600893          	li	a7,214
    __asm_syscall("r"(a7), "0"(a0))
    1de0:	00000073          	ecall

int brk(void *addr)
{
    return syscall(SYS_brk, addr);
}
    1de4:	2501                	sext.w	a0,a0
    1de6:	8082                	ret

0000000000001de8 <getcwd>:
    register long a7 __asm__("a7") = n;
    1de8:	48c5                	li	a7,17
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1dea:	00000073          	ecall

char *getcwd(char *buf, size_t size){
    return syscall(SYS_getcwd, buf, size);
}
    1dee:	8082                	ret

0000000000001df0 <chdir>:
    register long a7 __asm__("a7") = n;
    1df0:	03100893          	li	a7,49
    __asm_syscall("r"(a7), "0"(a0))
    1df4:	00000073          	ecall

int chdir(const char *path){
    return syscall(SYS_chdir, path);
}
    1df8:	2501                	sext.w	a0,a0
    1dfa:	8082                	ret

0000000000001dfc <mkdir>:

int mkdir(const char *path, mode_t mode){
    1dfc:	862e                	mv	a2,a1
    1dfe:	87aa                	mv	a5,a0
    register long a2 __asm__("a2") = c;
    1e00:	1602                	slli	a2,a2,0x20
    register long a7 __asm__("a7") = n;
    1e02:	02200893          	li	a7,34
    register long a0 __asm__("a0") = a;
    1e06:	f9c00513          	li	a0,-100
    register long a1 __asm__("a1") = b;
    1e0a:	85be                	mv	a1,a5
    register long a2 __asm__("a2") = c;
    1e0c:	9201                	srli	a2,a2,0x20
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1e0e:	00000073          	ecall
    return syscall(SYS_mkdirat, AT_FDCWD, path, mode);
}
    1e12:	2501                	sext.w	a0,a0
    1e14:	8082                	ret

0000000000001e16 <getdents>:
    register long a7 __asm__("a7") = n;
    1e16:	03d00893          	li	a7,61
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1e1a:	00000073          	ecall

int getdents(int fd, struct linux_dirent64 *dirp64, unsigned long len){
    //return syscall(SYS_getdents64, fd, dirp64, len);
    return syscall(SYS_getdents64, fd, dirp64, len);
}
    1e1e:	2501                	sext.w	a0,a0
    1e20:	8082                	ret

0000000000001e22 <pipe>:
    register long a7 __asm__("a7") = n;
    1e22:	03b00893          	li	a7,59
    register long a1 __asm__("a1") = b;
    1e26:	4581                	li	a1,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1e28:	00000073          	ecall

int pipe(int fd[2]){
    return syscall(SYS_pipe2, fd, 0);
}
    1e2c:	2501                	sext.w	a0,a0
    1e2e:	8082                	ret

0000000000001e30 <dup>:
    register long a7 __asm__("a7") = n;
    1e30:	48dd                	li	a7,23
    __asm_syscall("r"(a7), "0"(a0))
    1e32:	00000073          	ecall

int dup(int fd){
    return syscall(SYS_dup, fd);
}
    1e36:	2501                	sext.w	a0,a0
    1e38:	8082                	ret

0000000000001e3a <dup2>:
    register long a7 __asm__("a7") = n;
    1e3a:	48e1                	li	a7,24
    register long a2 __asm__("a2") = c;
    1e3c:	4601                	li	a2,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1e3e:	00000073          	ecall

int dup2(int old, int new){
    return syscall(SYS_dup3, old, new, 0);
}
    1e42:	2501                	sext.w	a0,a0
    1e44:	8082                	ret

0000000000001e46 <mount>:
    register long a7 __asm__("a7") = n;
    1e46:	02800893          	li	a7,40
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4))
    1e4a:	00000073          	ecall

int mount(const char *special, const char *dir, const char *fstype, unsigned long flags, const void *data)
{
        return syscall(SYS_mount, special, dir, fstype, flags, data);
}
    1e4e:	2501                	sext.w	a0,a0
    1e50:	8082                	ret

0000000000001e52 <umount>:
    register long a7 __asm__("a7") = n;
    1e52:	02700893          	li	a7,39
    register long a1 __asm__("a1") = b;
    1e56:	4581                	li	a1,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1e58:	00000073          	ecall

int umount(const char *special)
{
        return syscall(SYS_umount2, special, 0);
}
    1e5c:	2501                	sext.w	a0,a0
    1e5e:	8082                	ret

0000000000001e60 <__clone>:

.global __clone
.type  __clone, %function
__clone:
	# Save func and arg to stack
	addi a1, a1, -16
    1e60:	15c1                	addi	a1,a1,-16
	sd a0, 0(a1)
    1e62:	e188                	sd	a0,0(a1)
	sd a3, 8(a1)
    1e64:	e594                	sd	a3,8(a1)

	# Call SYS_clone
	mv a0, a2
    1e66:	8532                	mv	a0,a2
	mv a2, a4
    1e68:	863a                	mv	a2,a4
	mv a3, a5
    1e6a:	86be                	mv	a3,a5
	mv a4, a6
    1e6c:	8742                	mv	a4,a6
	li a7, 220 # SYS_clone
    1e6e:	0dc00893          	li	a7,220
	ecall
    1e72:	00000073          	ecall

	beqz a0, 1f
    1e76:	c111                	beqz	a0,1e7a <__clone+0x1a>
	# Parent
	ret
    1e78:	8082                	ret

	# Child
1:      ld a1, 0(sp)
    1e7a:	6582                	ld	a1,0(sp)
	ld a0, 8(sp)
    1e7c:	6522                	ld	a0,8(sp)
	jalr a1
    1e7e:	9582                	jalr	a1

	# Exit
	li a7, 93 # SYS_exit
    1e80:	05d00893          	li	a7,93
	ecall
    1e84:	00000073          	ecall
