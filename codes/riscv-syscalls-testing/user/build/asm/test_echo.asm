
/home/ubuntu/rust/ultra-os/codes/riscv-syscalls-testing/user/build/riscv64/test_echo:     file format elf64-littleriscv


Disassembly of section .text:

0000000000001000 <_start>:
.section .text.entry
.globl _start
_start:
    mv a0, sp
    1000:	850a                	mv	a0,sp
    tail __start_main
    1002:	a83d                	j	1040 <__start_main>

0000000000001004 <main>:

/*
 * for execve
 */

int main(int argc, char *argv[]){
    1004:	1141                	addi	sp,sp,-16
    printf("  I am test_echo.\nexecve success.\n");
    1006:	00001517          	auipc	a0,0x1
    100a:	e6250513          	addi	a0,a0,-414 # 1e68 <__clone+0x2c>
int main(int argc, char *argv[]){
    100e:	e406                	sd	ra,8(sp)
    printf("  I am test_echo.\nexecve success.\n");
    1010:	2c4000ef          	jal	ra,12d4 <printf>
    TEST_END(__func__);
    1014:	00001517          	auipc	a0,0x1
    1018:	e7c50513          	addi	a0,a0,-388 # 1e90 <__clone+0x54>
    101c:	296000ef          	jal	ra,12b2 <puts>
    1020:	00001517          	auipc	a0,0x1
    1024:	e9850513          	addi	a0,a0,-360 # 1eb8 <__func__.0>
    1028:	28a000ef          	jal	ra,12b2 <puts>
    102c:	00001517          	auipc	a0,0x1
    1030:	e7450513          	addi	a0,a0,-396 # 1ea0 <__clone+0x64>
    1034:	27e000ef          	jal	ra,12b2 <puts>
    return 0;
}
    1038:	60a2                	ld	ra,8(sp)
    103a:	4501                	li	a0,0
    103c:	0141                	addi	sp,sp,16
    103e:	8082                	ret

0000000000001040 <__start_main>:
#include <unistd.h>

extern int main();

int __start_main(long *p)
{
    1040:	85aa                	mv	a1,a0
	int argc = p[0];
	char **argv = (void *)(p+1);

	exit(main(argc, argv));
    1042:	4108                	lw	a0,0(a0)
{
    1044:	1141                	addi	sp,sp,-16
	exit(main(argc, argv));
    1046:	05a1                	addi	a1,a1,8
{
    1048:	e406                	sd	ra,8(sp)
	exit(main(argc, argv));
    104a:	fbbff0ef          	jal	ra,1004 <main>
    104e:	40b000ef          	jal	ra,1c58 <exit>
	return 0;
}
    1052:	60a2                	ld	ra,8(sp)
    1054:	4501                	li	a0,0
    1056:	0141                	addi	sp,sp,16
    1058:	8082                	ret

000000000000105a <printint.constprop.0>:
    write(f, s, l);
}

static char digits[] = "0123456789abcdef";

static void printint(int xx, int base, int sign)
    105a:	7179                	addi	sp,sp,-48
    105c:	f406                	sd	ra,40(sp)
{
    char buf[16 + 1];
    int i;
    uint x;

    if (sign && (sign = xx < 0))
    105e:	12054b63          	bltz	a0,1194 <printint.constprop.0+0x13a>

    buf[16] = 0;
    i = 15;
    do
    {
        buf[i--] = digits[x % base];
    1062:	02b577bb          	remuw	a5,a0,a1
    1066:	00001617          	auipc	a2,0x1
    106a:	e7260613          	addi	a2,a2,-398 # 1ed8 <digits>
    buf[16] = 0;
    106e:	00010c23          	sb	zero,24(sp)
        buf[i--] = digits[x % base];
    1072:	0005871b          	sext.w	a4,a1
    1076:	1782                	slli	a5,a5,0x20
    1078:	9381                	srli	a5,a5,0x20
    107a:	97b2                	add	a5,a5,a2
    107c:	0007c783          	lbu	a5,0(a5)
    } while ((x /= base) != 0);
    1080:	02b5583b          	divuw	a6,a0,a1
        buf[i--] = digits[x % base];
    1084:	00f10ba3          	sb	a5,23(sp)
    } while ((x /= base) != 0);
    1088:	1cb56363          	bltu	a0,a1,124e <printint.constprop.0+0x1f4>
        buf[i--] = digits[x % base];
    108c:	45b9                	li	a1,14
    108e:	02e877bb          	remuw	a5,a6,a4
    1092:	1782                	slli	a5,a5,0x20
    1094:	9381                	srli	a5,a5,0x20
    1096:	97b2                	add	a5,a5,a2
    1098:	0007c783          	lbu	a5,0(a5)
    } while ((x /= base) != 0);
    109c:	02e856bb          	divuw	a3,a6,a4
        buf[i--] = digits[x % base];
    10a0:	00f10b23          	sb	a5,22(sp)
    } while ((x /= base) != 0);
    10a4:	0ce86e63          	bltu	a6,a4,1180 <printint.constprop.0+0x126>
        buf[i--] = digits[x % base];
    10a8:	02e6f5bb          	remuw	a1,a3,a4
    } while ((x /= base) != 0);
    10ac:	02e6d7bb          	divuw	a5,a3,a4
        buf[i--] = digits[x % base];
    10b0:	1582                	slli	a1,a1,0x20
    10b2:	9181                	srli	a1,a1,0x20
    10b4:	95b2                	add	a1,a1,a2
    10b6:	0005c583          	lbu	a1,0(a1)
    10ba:	00b10aa3          	sb	a1,21(sp)
    } while ((x /= base) != 0);
    10be:	0007859b          	sext.w	a1,a5
    10c2:	12e6ec63          	bltu	a3,a4,11fa <printint.constprop.0+0x1a0>
        buf[i--] = digits[x % base];
    10c6:	02e7f6bb          	remuw	a3,a5,a4
    10ca:	1682                	slli	a3,a3,0x20
    10cc:	9281                	srli	a3,a3,0x20
    10ce:	96b2                	add	a3,a3,a2
    10d0:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    10d4:	02e7d83b          	divuw	a6,a5,a4
        buf[i--] = digits[x % base];
    10d8:	00d10a23          	sb	a3,20(sp)
    } while ((x /= base) != 0);
    10dc:	12e5e863          	bltu	a1,a4,120c <printint.constprop.0+0x1b2>
        buf[i--] = digits[x % base];
    10e0:	02e876bb          	remuw	a3,a6,a4
    10e4:	1682                	slli	a3,a3,0x20
    10e6:	9281                	srli	a3,a3,0x20
    10e8:	96b2                	add	a3,a3,a2
    10ea:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    10ee:	02e855bb          	divuw	a1,a6,a4
        buf[i--] = digits[x % base];
    10f2:	00d109a3          	sb	a3,19(sp)
    } while ((x /= base) != 0);
    10f6:	12e86463          	bltu	a6,a4,121e <printint.constprop.0+0x1c4>
        buf[i--] = digits[x % base];
    10fa:	02e5f6bb          	remuw	a3,a1,a4
    10fe:	1682                	slli	a3,a3,0x20
    1100:	9281                	srli	a3,a3,0x20
    1102:	96b2                	add	a3,a3,a2
    1104:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    1108:	02e5d83b          	divuw	a6,a1,a4
        buf[i--] = digits[x % base];
    110c:	00d10923          	sb	a3,18(sp)
    } while ((x /= base) != 0);
    1110:	0ce5ec63          	bltu	a1,a4,11e8 <printint.constprop.0+0x18e>
        buf[i--] = digits[x % base];
    1114:	02e876bb          	remuw	a3,a6,a4
    1118:	1682                	slli	a3,a3,0x20
    111a:	9281                	srli	a3,a3,0x20
    111c:	96b2                	add	a3,a3,a2
    111e:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    1122:	02e855bb          	divuw	a1,a6,a4
        buf[i--] = digits[x % base];
    1126:	00d108a3          	sb	a3,17(sp)
    } while ((x /= base) != 0);
    112a:	10e86963          	bltu	a6,a4,123c <printint.constprop.0+0x1e2>
        buf[i--] = digits[x % base];
    112e:	02e5f6bb          	remuw	a3,a1,a4
    1132:	1682                	slli	a3,a3,0x20
    1134:	9281                	srli	a3,a3,0x20
    1136:	96b2                	add	a3,a3,a2
    1138:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    113c:	02e5d83b          	divuw	a6,a1,a4
        buf[i--] = digits[x % base];
    1140:	00d10823          	sb	a3,16(sp)
    } while ((x /= base) != 0);
    1144:	10e5e763          	bltu	a1,a4,1252 <printint.constprop.0+0x1f8>
        buf[i--] = digits[x % base];
    1148:	02e876bb          	remuw	a3,a6,a4
    114c:	1682                	slli	a3,a3,0x20
    114e:	9281                	srli	a3,a3,0x20
    1150:	96b2                	add	a3,a3,a2
    1152:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    1156:	02e857bb          	divuw	a5,a6,a4
        buf[i--] = digits[x % base];
    115a:	00d107a3          	sb	a3,15(sp)
    } while ((x /= base) != 0);
    115e:	10e86363          	bltu	a6,a4,1264 <printint.constprop.0+0x20a>
        buf[i--] = digits[x % base];
    1162:	1782                	slli	a5,a5,0x20
    1164:	9381                	srli	a5,a5,0x20
    1166:	97b2                	add	a5,a5,a2
    1168:	0007c783          	lbu	a5,0(a5)
    116c:	4599                	li	a1,6
    116e:	00f10723          	sb	a5,14(sp)

    if (sign)
    1172:	00055763          	bgez	a0,1180 <printint.constprop.0+0x126>
        buf[i--] = '-';
    1176:	02d00793          	li	a5,45
    117a:	00f106a3          	sb	a5,13(sp)
        buf[i--] = digits[x % base];
    117e:	4595                	li	a1,5
    write(f, s, l);
    1180:	003c                	addi	a5,sp,8
    1182:	4641                	li	a2,16
    1184:	9e0d                	subw	a2,a2,a1
    1186:	4505                	li	a0,1
    1188:	95be                	add	a1,a1,a5
    118a:	27f000ef          	jal	ra,1c08 <write>
    i++;
    if (i < 0)
        puts("printint error");
    out(stdout, buf + i, 16 - i);
}
    118e:	70a2                	ld	ra,40(sp)
    1190:	6145                	addi	sp,sp,48
    1192:	8082                	ret
        x = -xx;
    1194:	40a0083b          	negw	a6,a0
        buf[i--] = digits[x % base];
    1198:	02b877bb          	remuw	a5,a6,a1
    119c:	00001617          	auipc	a2,0x1
    11a0:	d3c60613          	addi	a2,a2,-708 # 1ed8 <digits>
    buf[16] = 0;
    11a4:	00010c23          	sb	zero,24(sp)
        buf[i--] = digits[x % base];
    11a8:	0005871b          	sext.w	a4,a1
    11ac:	1782                	slli	a5,a5,0x20
    11ae:	9381                	srli	a5,a5,0x20
    11b0:	97b2                	add	a5,a5,a2
    11b2:	0007c783          	lbu	a5,0(a5)
    } while ((x /= base) != 0);
    11b6:	02b858bb          	divuw	a7,a6,a1
        buf[i--] = digits[x % base];
    11ba:	00f10ba3          	sb	a5,23(sp)
    } while ((x /= base) != 0);
    11be:	06b86963          	bltu	a6,a1,1230 <printint.constprop.0+0x1d6>
        buf[i--] = digits[x % base];
    11c2:	02e8f7bb          	remuw	a5,a7,a4
    11c6:	1782                	slli	a5,a5,0x20
    11c8:	9381                	srli	a5,a5,0x20
    11ca:	97b2                	add	a5,a5,a2
    11cc:	0007c783          	lbu	a5,0(a5)
    } while ((x /= base) != 0);
    11d0:	02e8d6bb          	divuw	a3,a7,a4
        buf[i--] = digits[x % base];
    11d4:	00f10b23          	sb	a5,22(sp)
    } while ((x /= base) != 0);
    11d8:	ece8f8e3          	bgeu	a7,a4,10a8 <printint.constprop.0+0x4e>
        buf[i--] = '-';
    11dc:	02d00793          	li	a5,45
    11e0:	00f10aa3          	sb	a5,21(sp)
        buf[i--] = digits[x % base];
    11e4:	45b5                	li	a1,13
    11e6:	bf69                	j	1180 <printint.constprop.0+0x126>
    11e8:	45a9                	li	a1,10
    if (sign)
    11ea:	f8055be3          	bgez	a0,1180 <printint.constprop.0+0x126>
        buf[i--] = '-';
    11ee:	02d00793          	li	a5,45
    11f2:	00f108a3          	sb	a5,17(sp)
        buf[i--] = digits[x % base];
    11f6:	45a5                	li	a1,9
    11f8:	b761                	j	1180 <printint.constprop.0+0x126>
    11fa:	45b5                	li	a1,13
    if (sign)
    11fc:	f80552e3          	bgez	a0,1180 <printint.constprop.0+0x126>
        buf[i--] = '-';
    1200:	02d00793          	li	a5,45
    1204:	00f10a23          	sb	a5,20(sp)
        buf[i--] = digits[x % base];
    1208:	45b1                	li	a1,12
    120a:	bf9d                	j	1180 <printint.constprop.0+0x126>
    120c:	45b1                	li	a1,12
    if (sign)
    120e:	f60559e3          	bgez	a0,1180 <printint.constprop.0+0x126>
        buf[i--] = '-';
    1212:	02d00793          	li	a5,45
    1216:	00f109a3          	sb	a5,19(sp)
        buf[i--] = digits[x % base];
    121a:	45ad                	li	a1,11
    121c:	b795                	j	1180 <printint.constprop.0+0x126>
    121e:	45ad                	li	a1,11
    if (sign)
    1220:	f60550e3          	bgez	a0,1180 <printint.constprop.0+0x126>
        buf[i--] = '-';
    1224:	02d00793          	li	a5,45
    1228:	00f10923          	sb	a5,18(sp)
        buf[i--] = digits[x % base];
    122c:	45a9                	li	a1,10
    122e:	bf89                	j	1180 <printint.constprop.0+0x126>
        buf[i--] = '-';
    1230:	02d00793          	li	a5,45
    1234:	00f10b23          	sb	a5,22(sp)
        buf[i--] = digits[x % base];
    1238:	45b9                	li	a1,14
    123a:	b799                	j	1180 <printint.constprop.0+0x126>
    123c:	45a5                	li	a1,9
    if (sign)
    123e:	f40551e3          	bgez	a0,1180 <printint.constprop.0+0x126>
        buf[i--] = '-';
    1242:	02d00793          	li	a5,45
    1246:	00f10823          	sb	a5,16(sp)
        buf[i--] = digits[x % base];
    124a:	45a1                	li	a1,8
    124c:	bf15                	j	1180 <printint.constprop.0+0x126>
    i = 15;
    124e:	45bd                	li	a1,15
    1250:	bf05                	j	1180 <printint.constprop.0+0x126>
        buf[i--] = digits[x % base];
    1252:	45a1                	li	a1,8
    if (sign)
    1254:	f20556e3          	bgez	a0,1180 <printint.constprop.0+0x126>
        buf[i--] = '-';
    1258:	02d00793          	li	a5,45
    125c:	00f107a3          	sb	a5,15(sp)
        buf[i--] = digits[x % base];
    1260:	459d                	li	a1,7
    1262:	bf39                	j	1180 <printint.constprop.0+0x126>
    1264:	459d                	li	a1,7
    if (sign)
    1266:	f0055de3          	bgez	a0,1180 <printint.constprop.0+0x126>
        buf[i--] = '-';
    126a:	02d00793          	li	a5,45
    126e:	00f10723          	sb	a5,14(sp)
        buf[i--] = digits[x % base];
    1272:	4599                	li	a1,6
    1274:	b731                	j	1180 <printint.constprop.0+0x126>

0000000000001276 <getchar>:
{
    1276:	1101                	addi	sp,sp,-32
    read(stdin, &byte, 1);
    1278:	00f10593          	addi	a1,sp,15
    127c:	4605                	li	a2,1
    127e:	4501                	li	a0,0
{
    1280:	ec06                	sd	ra,24(sp)
    char byte = 0;
    1282:	000107a3          	sb	zero,15(sp)
    read(stdin, &byte, 1);
    1286:	179000ef          	jal	ra,1bfe <read>
}
    128a:	60e2                	ld	ra,24(sp)
    128c:	00f14503          	lbu	a0,15(sp)
    1290:	6105                	addi	sp,sp,32
    1292:	8082                	ret

0000000000001294 <putchar>:
{
    1294:	1101                	addi	sp,sp,-32
    1296:	87aa                	mv	a5,a0
    return write(stdout, &byte, 1);
    1298:	00f10593          	addi	a1,sp,15
    129c:	4605                	li	a2,1
    129e:	4505                	li	a0,1
{
    12a0:	ec06                	sd	ra,24(sp)
    char byte = c;
    12a2:	00f107a3          	sb	a5,15(sp)
    return write(stdout, &byte, 1);
    12a6:	163000ef          	jal	ra,1c08 <write>
}
    12aa:	60e2                	ld	ra,24(sp)
    12ac:	2501                	sext.w	a0,a0
    12ae:	6105                	addi	sp,sp,32
    12b0:	8082                	ret

00000000000012b2 <puts>:
{
    12b2:	1141                	addi	sp,sp,-16
    12b4:	e406                	sd	ra,8(sp)
    12b6:	e022                	sd	s0,0(sp)
    12b8:	842a                	mv	s0,a0
    r = -(write(stdout, s, strlen(s)) < 0);
    12ba:	56a000ef          	jal	ra,1824 <strlen>
    12be:	862a                	mv	a2,a0
    12c0:	85a2                	mv	a1,s0
    12c2:	4505                	li	a0,1
    12c4:	145000ef          	jal	ra,1c08 <write>
}
    12c8:	60a2                	ld	ra,8(sp)
    12ca:	6402                	ld	s0,0(sp)
    r = -(write(stdout, s, strlen(s)) < 0);
    12cc:	957d                	srai	a0,a0,0x3f
    return r;
    12ce:	2501                	sext.w	a0,a0
}
    12d0:	0141                	addi	sp,sp,16
    12d2:	8082                	ret

00000000000012d4 <printf>:
    out(stdout, buf, i);
}

// Print to the console. only understands %d, %x, %p, %s.
void printf(const char *fmt, ...)
{
    12d4:	7171                	addi	sp,sp,-176
    12d6:	fc56                	sd	s5,56(sp)
    12d8:	ed3e                	sd	a5,152(sp)
    buf[i++] = '0';
    12da:	7ae1                	lui	s5,0xffff8
    va_list ap;
    int cnt = 0, l = 0;
    char *a, *z, *s = (char *)fmt, str;
    int f = stdout;

    va_start(ap, fmt);
    12dc:	18bc                	addi	a5,sp,120
{
    12de:	e8ca                	sd	s2,80(sp)
    12e0:	e4ce                	sd	s3,72(sp)
    12e2:	e0d2                	sd	s4,64(sp)
    12e4:	f85a                	sd	s6,48(sp)
    12e6:	f486                	sd	ra,104(sp)
    12e8:	f0a2                	sd	s0,96(sp)
    12ea:	eca6                	sd	s1,88(sp)
    12ec:	fcae                	sd	a1,120(sp)
    12ee:	e132                	sd	a2,128(sp)
    12f0:	e536                	sd	a3,136(sp)
    12f2:	e93a                	sd	a4,144(sp)
    12f4:	f142                	sd	a6,160(sp)
    12f6:	f546                	sd	a7,168(sp)
    va_start(ap, fmt);
    12f8:	e03e                	sd	a5,0(sp)
    for (;;)
    {
        if (!*s)
            break;
        for (a = s; *s && *s != '%'; s++)
    12fa:	02500913          	li	s2,37
        out(f, a, l);
        if (l)
            continue;
        if (s[1] == 0)
            break;
        switch (s[1])
    12fe:	07300a13          	li	s4,115
        case 'p':
            printptr(va_arg(ap, uint64));
            break;
        case 's':
            if ((a = va_arg(ap, char *)) == 0)
                a = "(null)";
    1302:	00001b17          	auipc	s6,0x1
    1306:	baeb0b13          	addi	s6,s6,-1106 # 1eb0 <__clone+0x74>
    buf[i++] = '0';
    130a:	830aca93          	xori	s5,s5,-2000
        buf[i++] = digits[x >> (sizeof(uint64) * 8 - 4)];
    130e:	00001997          	auipc	s3,0x1
    1312:	bca98993          	addi	s3,s3,-1078 # 1ed8 <digits>
        if (!*s)
    1316:	00054783          	lbu	a5,0(a0)
    131a:	16078a63          	beqz	a5,148e <printf+0x1ba>
    131e:	862a                	mv	a2,a0
        for (a = s; *s && *s != '%'; s++)
    1320:	19278163          	beq	a5,s2,14a2 <printf+0x1ce>
    1324:	00164783          	lbu	a5,1(a2)
    1328:	0605                	addi	a2,a2,1
    132a:	fbfd                	bnez	a5,1320 <printf+0x4c>
    132c:	84b2                	mv	s1,a2
        l = z - a;
    132e:	40a6043b          	subw	s0,a2,a0
    write(f, s, l);
    1332:	85aa                	mv	a1,a0
    1334:	8622                	mv	a2,s0
    1336:	4505                	li	a0,1
    1338:	0d1000ef          	jal	ra,1c08 <write>
        if (l)
    133c:	18041c63          	bnez	s0,14d4 <printf+0x200>
        if (s[1] == 0)
    1340:	0014c783          	lbu	a5,1(s1)
    1344:	14078563          	beqz	a5,148e <printf+0x1ba>
        switch (s[1])
    1348:	1d478063          	beq	a5,s4,1508 <printf+0x234>
    134c:	18fa6663          	bltu	s4,a5,14d8 <printf+0x204>
    1350:	06400713          	li	a4,100
    1354:	1ae78063          	beq	a5,a4,14f4 <printf+0x220>
    1358:	07000713          	li	a4,112
    135c:	1ce79963          	bne	a5,a4,152e <printf+0x25a>
            printptr(va_arg(ap, uint64));
    1360:	6702                	ld	a4,0(sp)
    buf[i++] = '0';
    1362:	01511423          	sh	s5,8(sp)
    write(f, s, l);
    1366:	4649                	li	a2,18
            printptr(va_arg(ap, uint64));
    1368:	631c                	ld	a5,0(a4)
    136a:	0721                	addi	a4,a4,8
    136c:	e03a                	sd	a4,0(sp)
    for (j = 0; j < (sizeof(uint64) * 2); j++, x <<= 4)
    136e:	00479293          	slli	t0,a5,0x4
    1372:	00879f93          	slli	t6,a5,0x8
    1376:	00c79f13          	slli	t5,a5,0xc
    137a:	01079e93          	slli	t4,a5,0x10
    137e:	01479e13          	slli	t3,a5,0x14
    1382:	01879313          	slli	t1,a5,0x18
    1386:	01c79893          	slli	a7,a5,0x1c
    138a:	02479813          	slli	a6,a5,0x24
    138e:	02879513          	slli	a0,a5,0x28
    1392:	02c79593          	slli	a1,a5,0x2c
    1396:	03079693          	slli	a3,a5,0x30
    139a:	03479713          	slli	a4,a5,0x34
        buf[i++] = digits[x >> (sizeof(uint64) * 8 - 4)];
    139e:	03c7d413          	srli	s0,a5,0x3c
    13a2:	01c7d39b          	srliw	t2,a5,0x1c
    13a6:	03c2d293          	srli	t0,t0,0x3c
    13aa:	03cfdf93          	srli	t6,t6,0x3c
    13ae:	03cf5f13          	srli	t5,t5,0x3c
    13b2:	03cede93          	srli	t4,t4,0x3c
    13b6:	03ce5e13          	srli	t3,t3,0x3c
    13ba:	03c35313          	srli	t1,t1,0x3c
    13be:	03c8d893          	srli	a7,a7,0x3c
    13c2:	03c85813          	srli	a6,a6,0x3c
    13c6:	9171                	srli	a0,a0,0x3c
    13c8:	91f1                	srli	a1,a1,0x3c
    13ca:	92f1                	srli	a3,a3,0x3c
    13cc:	9371                	srli	a4,a4,0x3c
    13ce:	96ce                	add	a3,a3,s3
    13d0:	974e                	add	a4,a4,s3
    13d2:	944e                	add	s0,s0,s3
    13d4:	92ce                	add	t0,t0,s3
    13d6:	9fce                	add	t6,t6,s3
    13d8:	9f4e                	add	t5,t5,s3
    13da:	9ece                	add	t4,t4,s3
    13dc:	9e4e                	add	t3,t3,s3
    13de:	934e                	add	t1,t1,s3
    13e0:	98ce                	add	a7,a7,s3
    13e2:	93ce                	add	t2,t2,s3
    13e4:	984e                	add	a6,a6,s3
    13e6:	954e                	add	a0,a0,s3
    13e8:	95ce                	add	a1,a1,s3
    13ea:	0006c083          	lbu	ra,0(a3)
    13ee:	0002c283          	lbu	t0,0(t0)
    13f2:	00074683          	lbu	a3,0(a4)
    13f6:	000fcf83          	lbu	t6,0(t6)
    13fa:	000f4f03          	lbu	t5,0(t5)
    13fe:	000ece83          	lbu	t4,0(t4)
    1402:	000e4e03          	lbu	t3,0(t3)
    1406:	00034303          	lbu	t1,0(t1)
    140a:	0008c883          	lbu	a7,0(a7)
    140e:	0003c383          	lbu	t2,0(t2)
    1412:	00084803          	lbu	a6,0(a6)
    1416:	00054503          	lbu	a0,0(a0)
    141a:	0005c583          	lbu	a1,0(a1)
    141e:	00044403          	lbu	s0,0(s0)
    for (j = 0; j < (sizeof(uint64) * 2); j++, x <<= 4)
    1422:	03879713          	slli	a4,a5,0x38
        buf[i++] = digits[x >> (sizeof(uint64) * 8 - 4)];
    1426:	9371                	srli	a4,a4,0x3c
    1428:	8bbd                	andi	a5,a5,15
    142a:	974e                	add	a4,a4,s3
    142c:	97ce                	add	a5,a5,s3
    142e:	005105a3          	sb	t0,11(sp)
    1432:	01f10623          	sb	t6,12(sp)
    1436:	01e106a3          	sb	t5,13(sp)
    143a:	01d10723          	sb	t4,14(sp)
    143e:	01c107a3          	sb	t3,15(sp)
    1442:	00610823          	sb	t1,16(sp)
    1446:	011108a3          	sb	a7,17(sp)
    144a:	00710923          	sb	t2,18(sp)
    144e:	010109a3          	sb	a6,19(sp)
    1452:	00a10a23          	sb	a0,20(sp)
    1456:	00b10aa3          	sb	a1,21(sp)
    145a:	00110b23          	sb	ra,22(sp)
    145e:	00d10ba3          	sb	a3,23(sp)
    1462:	00810523          	sb	s0,10(sp)
    1466:	00074703          	lbu	a4,0(a4)
    146a:	0007c783          	lbu	a5,0(a5)
    write(f, s, l);
    146e:	002c                	addi	a1,sp,8
    1470:	4505                	li	a0,1
        buf[i++] = digits[x >> (sizeof(uint64) * 8 - 4)];
    1472:	00e10c23          	sb	a4,24(sp)
    1476:	00f10ca3          	sb	a5,25(sp)
    buf[i] = 0;
    147a:	00010d23          	sb	zero,26(sp)
    write(f, s, l);
    147e:	78a000ef          	jal	ra,1c08 <write>
            // Print unknown % sequence to draw attention.
            putchar('%');
            putchar(s[1]);
            break;
        }
        s += 2;
    1482:	00248513          	addi	a0,s1,2
        if (!*s)
    1486:	00054783          	lbu	a5,0(a0)
    148a:	e8079ae3          	bnez	a5,131e <printf+0x4a>
    }
    va_end(ap);
}
    148e:	70a6                	ld	ra,104(sp)
    1490:	7406                	ld	s0,96(sp)
    1492:	64e6                	ld	s1,88(sp)
    1494:	6946                	ld	s2,80(sp)
    1496:	69a6                	ld	s3,72(sp)
    1498:	6a06                	ld	s4,64(sp)
    149a:	7ae2                	ld	s5,56(sp)
    149c:	7b42                	ld	s6,48(sp)
    149e:	614d                	addi	sp,sp,176
    14a0:	8082                	ret
        for (z = s; s[0] == '%' && s[1] == '%'; z++, s += 2)
    14a2:	00064783          	lbu	a5,0(a2)
    14a6:	84b2                	mv	s1,a2
    14a8:	01278963          	beq	a5,s2,14ba <printf+0x1e6>
    14ac:	b549                	j	132e <printf+0x5a>
    14ae:	0024c783          	lbu	a5,2(s1)
    14b2:	0605                	addi	a2,a2,1
    14b4:	0489                	addi	s1,s1,2
    14b6:	e7279ce3          	bne	a5,s2,132e <printf+0x5a>
    14ba:	0014c783          	lbu	a5,1(s1)
    14be:	ff2788e3          	beq	a5,s2,14ae <printf+0x1da>
        l = z - a;
    14c2:	40a6043b          	subw	s0,a2,a0
    write(f, s, l);
    14c6:	85aa                	mv	a1,a0
    14c8:	8622                	mv	a2,s0
    14ca:	4505                	li	a0,1
    14cc:	73c000ef          	jal	ra,1c08 <write>
        if (l)
    14d0:	e60408e3          	beqz	s0,1340 <printf+0x6c>
    14d4:	8526                	mv	a0,s1
    14d6:	b581                	j	1316 <printf+0x42>
        switch (s[1])
    14d8:	07800713          	li	a4,120
    14dc:	04e79963          	bne	a5,a4,152e <printf+0x25a>
            printint(va_arg(ap, int), 16, 1);
    14e0:	6782                	ld	a5,0(sp)
    14e2:	45c1                	li	a1,16
    14e4:	4388                	lw	a0,0(a5)
    14e6:	07a1                	addi	a5,a5,8
    14e8:	e03e                	sd	a5,0(sp)
    14ea:	b71ff0ef          	jal	ra,105a <printint.constprop.0>
        s += 2;
    14ee:	00248513          	addi	a0,s1,2
    14f2:	bf51                	j	1486 <printf+0x1b2>
            printint(va_arg(ap, int), 10, 1);
    14f4:	6782                	ld	a5,0(sp)
    14f6:	45a9                	li	a1,10
    14f8:	4388                	lw	a0,0(a5)
    14fa:	07a1                	addi	a5,a5,8
    14fc:	e03e                	sd	a5,0(sp)
    14fe:	b5dff0ef          	jal	ra,105a <printint.constprop.0>
        s += 2;
    1502:	00248513          	addi	a0,s1,2
    1506:	b741                	j	1486 <printf+0x1b2>
            if ((a = va_arg(ap, char *)) == 0)
    1508:	6782                	ld	a5,0(sp)
    150a:	6380                	ld	s0,0(a5)
    150c:	07a1                	addi	a5,a5,8
    150e:	e03e                	sd	a5,0(sp)
    1510:	c031                	beqz	s0,1554 <printf+0x280>
            l = strnlen(a, 200);
    1512:	0c800593          	li	a1,200
    1516:	8522                	mv	a0,s0
    1518:	3f8000ef          	jal	ra,1910 <strnlen>
    write(f, s, l);
    151c:	0005061b          	sext.w	a2,a0
    1520:	85a2                	mv	a1,s0
    1522:	4505                	li	a0,1
    1524:	6e4000ef          	jal	ra,1c08 <write>
        s += 2;
    1528:	00248513          	addi	a0,s1,2
    152c:	bfa9                	j	1486 <printf+0x1b2>
    return write(stdout, &byte, 1);
    152e:	4605                	li	a2,1
    1530:	002c                	addi	a1,sp,8
    1532:	4505                	li	a0,1
    char byte = c;
    1534:	01210423          	sb	s2,8(sp)
    return write(stdout, &byte, 1);
    1538:	6d0000ef          	jal	ra,1c08 <write>
    char byte = c;
    153c:	0014c783          	lbu	a5,1(s1)
    return write(stdout, &byte, 1);
    1540:	4605                	li	a2,1
    1542:	002c                	addi	a1,sp,8
    1544:	4505                	li	a0,1
    char byte = c;
    1546:	00f10423          	sb	a5,8(sp)
    return write(stdout, &byte, 1);
    154a:	6be000ef          	jal	ra,1c08 <write>
        s += 2;
    154e:	00248513          	addi	a0,s1,2
    1552:	bf15                	j	1486 <printf+0x1b2>
                a = "(null)";
    1554:	845a                	mv	s0,s6
    1556:	bf75                	j	1512 <printf+0x23e>

0000000000001558 <isspace>:
#define HIGHS (ONES * (UCHAR_MAX / 2 + 1))
#define HASZERO(x) (((x)-ONES) & ~(x)&HIGHS)

int isspace(int c)
{
    return c == ' ' || (unsigned)c - '\t' < 5;
    1558:	02000793          	li	a5,32
    155c:	00f50663          	beq	a0,a5,1568 <isspace+0x10>
    1560:	355d                	addiw	a0,a0,-9
    1562:	00553513          	sltiu	a0,a0,5
    1566:	8082                	ret
    1568:	4505                	li	a0,1
}
    156a:	8082                	ret

000000000000156c <isdigit>:

int isdigit(int c)
{
    return (unsigned)c - '0' < 10;
    156c:	fd05051b          	addiw	a0,a0,-48
}
    1570:	00a53513          	sltiu	a0,a0,10
    1574:	8082                	ret

0000000000001576 <atoi>:
    return c == ' ' || (unsigned)c - '\t' < 5;
    1576:	02000613          	li	a2,32
    157a:	4591                	li	a1,4

int atoi(const char *s)
{
    int n = 0, neg = 0;
    while (isspace(*s))
    157c:	00054703          	lbu	a4,0(a0)
    return c == ' ' || (unsigned)c - '\t' < 5;
    1580:	ff77069b          	addiw	a3,a4,-9
    1584:	04c70d63          	beq	a4,a2,15de <atoi+0x68>
    1588:	0007079b          	sext.w	a5,a4
    158c:	04d5f963          	bgeu	a1,a3,15de <atoi+0x68>
        s++;
    switch (*s)
    1590:	02b00693          	li	a3,43
    1594:	04d70a63          	beq	a4,a3,15e8 <atoi+0x72>
    1598:	02d00693          	li	a3,45
    159c:	06d70463          	beq	a4,a3,1604 <atoi+0x8e>
        neg = 1;
    case '+':
        s++;
    }
    /* Compute n as a negative number to avoid overflow on INT_MIN */
    while (isdigit(*s))
    15a0:	fd07859b          	addiw	a1,a5,-48
    15a4:	4625                	li	a2,9
    15a6:	873e                	mv	a4,a5
    15a8:	86aa                	mv	a3,a0
    int n = 0, neg = 0;
    15aa:	4e01                	li	t3,0
    while (isdigit(*s))
    15ac:	04b66a63          	bltu	a2,a1,1600 <atoi+0x8a>
    int n = 0, neg = 0;
    15b0:	4501                	li	a0,0
    while (isdigit(*s))
    15b2:	4825                	li	a6,9
    15b4:	0016c603          	lbu	a2,1(a3)
        n = 10 * n - (*s++ - '0');
    15b8:	0025179b          	slliw	a5,a0,0x2
    15bc:	9d3d                	addw	a0,a0,a5
    15be:	fd07031b          	addiw	t1,a4,-48
    15c2:	0015189b          	slliw	a7,a0,0x1
    while (isdigit(*s))
    15c6:	fd06059b          	addiw	a1,a2,-48
        n = 10 * n - (*s++ - '0');
    15ca:	0685                	addi	a3,a3,1
    15cc:	4068853b          	subw	a0,a7,t1
    while (isdigit(*s))
    15d0:	0006071b          	sext.w	a4,a2
    15d4:	feb870e3          	bgeu	a6,a1,15b4 <atoi+0x3e>
    return neg ? n : -n;
    15d8:	000e0563          	beqz	t3,15e2 <atoi+0x6c>
}
    15dc:	8082                	ret
        s++;
    15de:	0505                	addi	a0,a0,1
    15e0:	bf71                	j	157c <atoi+0x6>
    return neg ? n : -n;
    15e2:	4113053b          	subw	a0,t1,a7
    15e6:	8082                	ret
    while (isdigit(*s))
    15e8:	00154783          	lbu	a5,1(a0)
    15ec:	4625                	li	a2,9
        s++;
    15ee:	00150693          	addi	a3,a0,1
    while (isdigit(*s))
    15f2:	fd07859b          	addiw	a1,a5,-48
    15f6:	0007871b          	sext.w	a4,a5
    int n = 0, neg = 0;
    15fa:	4e01                	li	t3,0
    while (isdigit(*s))
    15fc:	fab67ae3          	bgeu	a2,a1,15b0 <atoi+0x3a>
    1600:	4501                	li	a0,0
}
    1602:	8082                	ret
    while (isdigit(*s))
    1604:	00154783          	lbu	a5,1(a0)
    1608:	4625                	li	a2,9
        s++;
    160a:	00150693          	addi	a3,a0,1
    while (isdigit(*s))
    160e:	fd07859b          	addiw	a1,a5,-48
    1612:	0007871b          	sext.w	a4,a5
    1616:	feb665e3          	bltu	a2,a1,1600 <atoi+0x8a>
        neg = 1;
    161a:	4e05                	li	t3,1
    161c:	bf51                	j	15b0 <atoi+0x3a>

000000000000161e <memset>:

void *memset(void *dest, int c, size_t n)
{
    char *p = dest;
    for (int i = 0; i < n; ++i, *(p++) = c)
    161e:	16060d63          	beqz	a2,1798 <memset+0x17a>
    1622:	40a007b3          	neg	a5,a0
    1626:	8b9d                	andi	a5,a5,7
    1628:	00778713          	addi	a4,a5,7
    162c:	482d                	li	a6,11
    162e:	0ff5f593          	andi	a1,a1,255
    1632:	fff60693          	addi	a3,a2,-1
    1636:	17076263          	bltu	a4,a6,179a <memset+0x17c>
    163a:	16e6ea63          	bltu	a3,a4,17ae <memset+0x190>
    163e:	16078563          	beqz	a5,17a8 <memset+0x18a>
    1642:	00b50023          	sb	a1,0(a0)
    1646:	4705                	li	a4,1
    1648:	00150e93          	addi	t4,a0,1
    164c:	14e78c63          	beq	a5,a4,17a4 <memset+0x186>
    1650:	00b500a3          	sb	a1,1(a0)
    1654:	4709                	li	a4,2
    1656:	00250e93          	addi	t4,a0,2
    165a:	14e78d63          	beq	a5,a4,17b4 <memset+0x196>
    165e:	00b50123          	sb	a1,2(a0)
    1662:	470d                	li	a4,3
    1664:	00350e93          	addi	t4,a0,3
    1668:	12e78b63          	beq	a5,a4,179e <memset+0x180>
    166c:	00b501a3          	sb	a1,3(a0)
    1670:	4711                	li	a4,4
    1672:	00450e93          	addi	t4,a0,4
    1676:	14e78163          	beq	a5,a4,17b8 <memset+0x19a>
    167a:	00b50223          	sb	a1,4(a0)
    167e:	4715                	li	a4,5
    1680:	00550e93          	addi	t4,a0,5
    1684:	12e78c63          	beq	a5,a4,17bc <memset+0x19e>
    1688:	00b502a3          	sb	a1,5(a0)
    168c:	471d                	li	a4,7
    168e:	00650e93          	addi	t4,a0,6
    1692:	12e79763          	bne	a5,a4,17c0 <memset+0x1a2>
    1696:	00750e93          	addi	t4,a0,7
    169a:	00b50323          	sb	a1,6(a0)
    169e:	4f1d                	li	t5,7
    16a0:	00859713          	slli	a4,a1,0x8
    16a4:	8f4d                	or	a4,a4,a1
    16a6:	01059e13          	slli	t3,a1,0x10
    16aa:	01c76e33          	or	t3,a4,t3
    16ae:	01859313          	slli	t1,a1,0x18
    16b2:	006e6333          	or	t1,t3,t1
    16b6:	02059893          	slli	a7,a1,0x20
    16ba:	011368b3          	or	a7,t1,a7
    16be:	02859813          	slli	a6,a1,0x28
    16c2:	40f60333          	sub	t1,a2,a5
    16c6:	0108e833          	or	a6,a7,a6
    16ca:	03059693          	slli	a3,a1,0x30
    16ce:	00d866b3          	or	a3,a6,a3
    16d2:	03859713          	slli	a4,a1,0x38
    16d6:	97aa                	add	a5,a5,a0
    16d8:	ff837813          	andi	a6,t1,-8
    16dc:	8f55                	or	a4,a4,a3
    16de:	00f806b3          	add	a3,a6,a5
    16e2:	e398                	sd	a4,0(a5)
    16e4:	07a1                	addi	a5,a5,8
    16e6:	fed79ee3          	bne	a5,a3,16e2 <memset+0xc4>
    16ea:	ff837693          	andi	a3,t1,-8
    16ee:	00de87b3          	add	a5,t4,a3
    16f2:	01e6873b          	addw	a4,a3,t5
    16f6:	0ad30663          	beq	t1,a3,17a2 <memset+0x184>
    16fa:	00b78023          	sb	a1,0(a5)
    16fe:	0017069b          	addiw	a3,a4,1
    1702:	08c6fb63          	bgeu	a3,a2,1798 <memset+0x17a>
    1706:	00b780a3          	sb	a1,1(a5)
    170a:	0027069b          	addiw	a3,a4,2
    170e:	08c6f563          	bgeu	a3,a2,1798 <memset+0x17a>
    1712:	00b78123          	sb	a1,2(a5)
    1716:	0037069b          	addiw	a3,a4,3
    171a:	06c6ff63          	bgeu	a3,a2,1798 <memset+0x17a>
    171e:	00b781a3          	sb	a1,3(a5)
    1722:	0047069b          	addiw	a3,a4,4
    1726:	06c6f963          	bgeu	a3,a2,1798 <memset+0x17a>
    172a:	00b78223          	sb	a1,4(a5)
    172e:	0057069b          	addiw	a3,a4,5
    1732:	06c6f363          	bgeu	a3,a2,1798 <memset+0x17a>
    1736:	00b782a3          	sb	a1,5(a5)
    173a:	0067069b          	addiw	a3,a4,6
    173e:	04c6fd63          	bgeu	a3,a2,1798 <memset+0x17a>
    1742:	00b78323          	sb	a1,6(a5)
    1746:	0077069b          	addiw	a3,a4,7
    174a:	04c6f763          	bgeu	a3,a2,1798 <memset+0x17a>
    174e:	00b783a3          	sb	a1,7(a5)
    1752:	0087069b          	addiw	a3,a4,8
    1756:	04c6f163          	bgeu	a3,a2,1798 <memset+0x17a>
    175a:	00b78423          	sb	a1,8(a5)
    175e:	0097069b          	addiw	a3,a4,9
    1762:	02c6fb63          	bgeu	a3,a2,1798 <memset+0x17a>
    1766:	00b784a3          	sb	a1,9(a5)
    176a:	00a7069b          	addiw	a3,a4,10
    176e:	02c6f563          	bgeu	a3,a2,1798 <memset+0x17a>
    1772:	00b78523          	sb	a1,10(a5)
    1776:	00b7069b          	addiw	a3,a4,11
    177a:	00c6ff63          	bgeu	a3,a2,1798 <memset+0x17a>
    177e:	00b785a3          	sb	a1,11(a5)
    1782:	00c7069b          	addiw	a3,a4,12
    1786:	00c6f963          	bgeu	a3,a2,1798 <memset+0x17a>
    178a:	00b78623          	sb	a1,12(a5)
    178e:	2735                	addiw	a4,a4,13
    1790:	00c77463          	bgeu	a4,a2,1798 <memset+0x17a>
    1794:	00b786a3          	sb	a1,13(a5)
        ;
    return dest;
}
    1798:	8082                	ret
    179a:	472d                	li	a4,11
    179c:	bd79                	j	163a <memset+0x1c>
    for (int i = 0; i < n; ++i, *(p++) = c)
    179e:	4f0d                	li	t5,3
    17a0:	b701                	j	16a0 <memset+0x82>
    17a2:	8082                	ret
    17a4:	4f05                	li	t5,1
    17a6:	bded                	j	16a0 <memset+0x82>
    17a8:	8eaa                	mv	t4,a0
    17aa:	4f01                	li	t5,0
    17ac:	bdd5                	j	16a0 <memset+0x82>
    17ae:	87aa                	mv	a5,a0
    17b0:	4701                	li	a4,0
    17b2:	b7a1                	j	16fa <memset+0xdc>
    17b4:	4f09                	li	t5,2
    17b6:	b5ed                	j	16a0 <memset+0x82>
    17b8:	4f11                	li	t5,4
    17ba:	b5dd                	j	16a0 <memset+0x82>
    17bc:	4f15                	li	t5,5
    17be:	b5cd                	j	16a0 <memset+0x82>
    17c0:	4f19                	li	t5,6
    17c2:	bdf9                	j	16a0 <memset+0x82>

00000000000017c4 <strcmp>:

int strcmp(const char *l, const char *r)
{
    for (; *l == *r && *l; l++, r++)
    17c4:	00054783          	lbu	a5,0(a0)
    17c8:	0005c703          	lbu	a4,0(a1)
    17cc:	00e79863          	bne	a5,a4,17dc <strcmp+0x18>
    17d0:	0505                	addi	a0,a0,1
    17d2:	0585                	addi	a1,a1,1
    17d4:	fbe5                	bnez	a5,17c4 <strcmp>
    17d6:	4501                	li	a0,0
        ;
    return *(unsigned char *)l - *(unsigned char *)r;
}
    17d8:	9d19                	subw	a0,a0,a4
    17da:	8082                	ret
    return *(unsigned char *)l - *(unsigned char *)r;
    17dc:	0007851b          	sext.w	a0,a5
    17e0:	bfe5                	j	17d8 <strcmp+0x14>

00000000000017e2 <strncmp>:

int strncmp(const char *_l, const char *_r, size_t n)
{
    const unsigned char *l = (void *)_l, *r = (void *)_r;
    if (!n--)
    17e2:	ce05                	beqz	a2,181a <strncmp+0x38>
        return 0;
    for (; *l && *r && n && *l == *r; l++, r++, n--)
    17e4:	00054703          	lbu	a4,0(a0)
    17e8:	0005c783          	lbu	a5,0(a1)
    17ec:	cb0d                	beqz	a4,181e <strncmp+0x3c>
    if (!n--)
    17ee:	167d                	addi	a2,a2,-1
    17f0:	00c506b3          	add	a3,a0,a2
    17f4:	a819                	j	180a <strncmp+0x28>
    for (; *l && *r && n && *l == *r; l++, r++, n--)
    17f6:	00a68e63          	beq	a3,a0,1812 <strncmp+0x30>
    17fa:	0505                	addi	a0,a0,1
    17fc:	00e79b63          	bne	a5,a4,1812 <strncmp+0x30>
    1800:	00054703          	lbu	a4,0(a0)
        ;
    return *l - *r;
    1804:	0005c783          	lbu	a5,0(a1)
    for (; *l && *r && n && *l == *r; l++, r++, n--)
    1808:	cb19                	beqz	a4,181e <strncmp+0x3c>
    180a:	0005c783          	lbu	a5,0(a1)
    180e:	0585                	addi	a1,a1,1
    1810:	f3fd                	bnez	a5,17f6 <strncmp+0x14>
    return *l - *r;
    1812:	0007051b          	sext.w	a0,a4
    1816:	9d1d                	subw	a0,a0,a5
    1818:	8082                	ret
        return 0;
    181a:	4501                	li	a0,0
}
    181c:	8082                	ret
    181e:	4501                	li	a0,0
    return *l - *r;
    1820:	9d1d                	subw	a0,a0,a5
    1822:	8082                	ret

0000000000001824 <strlen>:
size_t strlen(const char *s)
{
    const char *a = s;
    typedef size_t __attribute__((__may_alias__)) word;
    const word *w;
    for (; (uintptr_t)s % SS; s++)
    1824:	00757793          	andi	a5,a0,7
    1828:	cf89                	beqz	a5,1842 <strlen+0x1e>
    182a:	87aa                	mv	a5,a0
    182c:	a029                	j	1836 <strlen+0x12>
    182e:	0785                	addi	a5,a5,1
    1830:	0077f713          	andi	a4,a5,7
    1834:	cb01                	beqz	a4,1844 <strlen+0x20>
        if (!*s)
    1836:	0007c703          	lbu	a4,0(a5)
    183a:	fb75                	bnez	a4,182e <strlen+0xa>
    for (w = (const void *)s; !HASZERO(*w); w++)
        ;
    s = (const void *)w;
    for (; *s; s++)
        ;
    return s - a;
    183c:	40a78533          	sub	a0,a5,a0
}
    1840:	8082                	ret
    for (; (uintptr_t)s % SS; s++)
    1842:	87aa                	mv	a5,a0
    for (w = (const void *)s; !HASZERO(*w); w++)
    1844:	6394                	ld	a3,0(a5)
    1846:	00000597          	auipc	a1,0x0
    184a:	67a5b583          	ld	a1,1658(a1) # 1ec0 <__func__.0+0x8>
    184e:	00000617          	auipc	a2,0x0
    1852:	67a63603          	ld	a2,1658(a2) # 1ec8 <__func__.0+0x10>
    1856:	a019                	j	185c <strlen+0x38>
    1858:	6794                	ld	a3,8(a5)
    185a:	07a1                	addi	a5,a5,8
    185c:	00b68733          	add	a4,a3,a1
    1860:	fff6c693          	not	a3,a3
    1864:	8f75                	and	a4,a4,a3
    1866:	8f71                	and	a4,a4,a2
    1868:	db65                	beqz	a4,1858 <strlen+0x34>
    for (; *s; s++)
    186a:	0007c703          	lbu	a4,0(a5)
    186e:	d779                	beqz	a4,183c <strlen+0x18>
    1870:	0017c703          	lbu	a4,1(a5)
    1874:	0785                	addi	a5,a5,1
    1876:	d379                	beqz	a4,183c <strlen+0x18>
    1878:	0017c703          	lbu	a4,1(a5)
    187c:	0785                	addi	a5,a5,1
    187e:	fb6d                	bnez	a4,1870 <strlen+0x4c>
    1880:	bf75                	j	183c <strlen+0x18>

0000000000001882 <memchr>:

void *memchr(const void *src, int c, size_t n)
{
    const unsigned char *s = src;
    c = (unsigned char)c;
    for (; ((uintptr_t)s & ALIGN) && n && *s != c; s++, n--)
    1882:	00757713          	andi	a4,a0,7
{
    1886:	87aa                	mv	a5,a0
    c = (unsigned char)c;
    1888:	0ff5f593          	andi	a1,a1,255
    for (; ((uintptr_t)s & ALIGN) && n && *s != c; s++, n--)
    188c:	cb19                	beqz	a4,18a2 <memchr+0x20>
    188e:	ce25                	beqz	a2,1906 <memchr+0x84>
    1890:	0007c703          	lbu	a4,0(a5)
    1894:	04b70e63          	beq	a4,a1,18f0 <memchr+0x6e>
    1898:	0785                	addi	a5,a5,1
    189a:	0077f713          	andi	a4,a5,7
    189e:	167d                	addi	a2,a2,-1
    18a0:	f77d                	bnez	a4,188e <memchr+0xc>
            ;
        s = (const void *)w;
    }
    for (; n && *s != c; s++, n--)
        ;
    return n ? (void *)s : 0;
    18a2:	4501                	li	a0,0
    if (n && *s != c)
    18a4:	c235                	beqz	a2,1908 <memchr+0x86>
    18a6:	0007c703          	lbu	a4,0(a5)
    18aa:	04b70363          	beq	a4,a1,18f0 <memchr+0x6e>
        size_t k = ONES * c;
    18ae:	00000517          	auipc	a0,0x0
    18b2:	62253503          	ld	a0,1570(a0) # 1ed0 <__func__.0+0x18>
        for (w = (const void *)s; n >= SS && !HASZERO(*w ^ k); w++, n -= SS)
    18b6:	471d                	li	a4,7
        size_t k = ONES * c;
    18b8:	02a58533          	mul	a0,a1,a0
        for (w = (const void *)s; n >= SS && !HASZERO(*w ^ k); w++, n -= SS)
    18bc:	02c77a63          	bgeu	a4,a2,18f0 <memchr+0x6e>
    18c0:	00000897          	auipc	a7,0x0
    18c4:	6008b883          	ld	a7,1536(a7) # 1ec0 <__func__.0+0x8>
    18c8:	00000817          	auipc	a6,0x0
    18cc:	60083803          	ld	a6,1536(a6) # 1ec8 <__func__.0+0x10>
    18d0:	431d                	li	t1,7
    18d2:	a029                	j	18dc <memchr+0x5a>
    18d4:	1661                	addi	a2,a2,-8
    18d6:	07a1                	addi	a5,a5,8
    18d8:	02c37963          	bgeu	t1,a2,190a <memchr+0x88>
    18dc:	6398                	ld	a4,0(a5)
    18de:	8f29                	xor	a4,a4,a0
    18e0:	011706b3          	add	a3,a4,a7
    18e4:	fff74713          	not	a4,a4
    18e8:	8f75                	and	a4,a4,a3
    18ea:	01077733          	and	a4,a4,a6
    18ee:	d37d                	beqz	a4,18d4 <memchr+0x52>
    18f0:	853e                	mv	a0,a5
    18f2:	97b2                	add	a5,a5,a2
    18f4:	a021                	j	18fc <memchr+0x7a>
    for (; n && *s != c; s++, n--)
    18f6:	0505                	addi	a0,a0,1
    18f8:	00f50763          	beq	a0,a5,1906 <memchr+0x84>
    18fc:	00054703          	lbu	a4,0(a0)
    1900:	feb71be3          	bne	a4,a1,18f6 <memchr+0x74>
    1904:	8082                	ret
    return n ? (void *)s : 0;
    1906:	4501                	li	a0,0
}
    1908:	8082                	ret
    return n ? (void *)s : 0;
    190a:	4501                	li	a0,0
    for (; n && *s != c; s++, n--)
    190c:	f275                	bnez	a2,18f0 <memchr+0x6e>
}
    190e:	8082                	ret

0000000000001910 <strnlen>:

size_t strnlen(const char *s, size_t n)
{
    1910:	1101                	addi	sp,sp,-32
    1912:	e822                	sd	s0,16(sp)
    const char *p = memchr(s, 0, n);
    1914:	862e                	mv	a2,a1
{
    1916:	842e                	mv	s0,a1
    const char *p = memchr(s, 0, n);
    1918:	4581                	li	a1,0
{
    191a:	e426                	sd	s1,8(sp)
    191c:	ec06                	sd	ra,24(sp)
    191e:	84aa                	mv	s1,a0
    const char *p = memchr(s, 0, n);
    1920:	f63ff0ef          	jal	ra,1882 <memchr>
    return p ? p - s : n;
    1924:	c519                	beqz	a0,1932 <strnlen+0x22>
}
    1926:	60e2                	ld	ra,24(sp)
    1928:	6442                	ld	s0,16(sp)
    return p ? p - s : n;
    192a:	8d05                	sub	a0,a0,s1
}
    192c:	64a2                	ld	s1,8(sp)
    192e:	6105                	addi	sp,sp,32
    1930:	8082                	ret
    1932:	60e2                	ld	ra,24(sp)
    return p ? p - s : n;
    1934:	8522                	mv	a0,s0
}
    1936:	6442                	ld	s0,16(sp)
    1938:	64a2                	ld	s1,8(sp)
    193a:	6105                	addi	sp,sp,32
    193c:	8082                	ret

000000000000193e <strcpy>:
char *strcpy(char *restrict d, const char *s)
{
    typedef size_t __attribute__((__may_alias__)) word;
    word *wd;
    const word *ws;
    if ((uintptr_t)s % SS == (uintptr_t)d % SS)
    193e:	00b547b3          	xor	a5,a0,a1
    1942:	8b9d                	andi	a5,a5,7
    1944:	eb95                	bnez	a5,1978 <strcpy+0x3a>
    {
        for (; (uintptr_t)s % SS; s++, d++)
    1946:	0075f793          	andi	a5,a1,7
    194a:	e7b1                	bnez	a5,1996 <strcpy+0x58>
            if (!(*d = *s))
                return d;
        wd = (void *)d;
        ws = (const void *)s;
        for (; !HASZERO(*ws); *wd++ = *ws++)
    194c:	6198                	ld	a4,0(a1)
    194e:	00000617          	auipc	a2,0x0
    1952:	57263603          	ld	a2,1394(a2) # 1ec0 <__func__.0+0x8>
    1956:	00000817          	auipc	a6,0x0
    195a:	57283803          	ld	a6,1394(a6) # 1ec8 <__func__.0+0x10>
    195e:	a029                	j	1968 <strcpy+0x2a>
    1960:	e118                	sd	a4,0(a0)
    1962:	6598                	ld	a4,8(a1)
    1964:	05a1                	addi	a1,a1,8
    1966:	0521                	addi	a0,a0,8
    1968:	00c707b3          	add	a5,a4,a2
    196c:	fff74693          	not	a3,a4
    1970:	8ff5                	and	a5,a5,a3
    1972:	0107f7b3          	and	a5,a5,a6
    1976:	d7ed                	beqz	a5,1960 <strcpy+0x22>
            ;
        d = (void *)wd;
        s = (const void *)ws;
    }
    for (; (*d = *s); s++, d++)
    1978:	0005c783          	lbu	a5,0(a1)
    197c:	00f50023          	sb	a5,0(a0)
    1980:	c785                	beqz	a5,19a8 <strcpy+0x6a>
    1982:	0015c783          	lbu	a5,1(a1)
    1986:	0505                	addi	a0,a0,1
    1988:	0585                	addi	a1,a1,1
    198a:	00f50023          	sb	a5,0(a0)
    198e:	fbf5                	bnez	a5,1982 <strcpy+0x44>
        ;
    return d;
}
    1990:	8082                	ret
        for (; (uintptr_t)s % SS; s++, d++)
    1992:	0505                	addi	a0,a0,1
    1994:	df45                	beqz	a4,194c <strcpy+0xe>
            if (!(*d = *s))
    1996:	0005c783          	lbu	a5,0(a1)
        for (; (uintptr_t)s % SS; s++, d++)
    199a:	0585                	addi	a1,a1,1
    199c:	0075f713          	andi	a4,a1,7
            if (!(*d = *s))
    19a0:	00f50023          	sb	a5,0(a0)
    19a4:	f7fd                	bnez	a5,1992 <strcpy+0x54>
}
    19a6:	8082                	ret
    19a8:	8082                	ret

00000000000019aa <strncpy>:
char *strncpy(char *restrict d, const char *s, size_t n)
{
    typedef size_t __attribute__((__may_alias__)) word;
    word *wd;
    const word *ws;
    if (((uintptr_t)s & ALIGN) == ((uintptr_t)d & ALIGN))
    19aa:	00b547b3          	xor	a5,a0,a1
    19ae:	8b9d                	andi	a5,a5,7
    19b0:	1a079863          	bnez	a5,1b60 <strncpy+0x1b6>
    {
        for (; ((uintptr_t)s & ALIGN) && n && (*d = *s); n--, s++, d++)
    19b4:	0075f793          	andi	a5,a1,7
    19b8:	16078463          	beqz	a5,1b20 <strncpy+0x176>
    19bc:	ea01                	bnez	a2,19cc <strncpy+0x22>
    19be:	a421                	j	1bc6 <strncpy+0x21c>
    19c0:	167d                	addi	a2,a2,-1
    19c2:	0505                	addi	a0,a0,1
    19c4:	14070e63          	beqz	a4,1b20 <strncpy+0x176>
    19c8:	1a060863          	beqz	a2,1b78 <strncpy+0x1ce>
    19cc:	0005c783          	lbu	a5,0(a1)
    19d0:	0585                	addi	a1,a1,1
    19d2:	0075f713          	andi	a4,a1,7
    19d6:	00f50023          	sb	a5,0(a0)
    19da:	f3fd                	bnez	a5,19c0 <strncpy+0x16>
    19dc:	4805                	li	a6,1
    19de:	1a061863          	bnez	a2,1b8e <strncpy+0x1e4>
    19e2:	40a007b3          	neg	a5,a0
    19e6:	8b9d                	andi	a5,a5,7
    19e8:	4681                	li	a3,0
    19ea:	18061a63          	bnez	a2,1b7e <strncpy+0x1d4>
    19ee:	00778713          	addi	a4,a5,7
    19f2:	45ad                	li	a1,11
    19f4:	18b76363          	bltu	a4,a1,1b7a <strncpy+0x1d0>
    19f8:	1ae6eb63          	bltu	a3,a4,1bae <strncpy+0x204>
    19fc:	1a078363          	beqz	a5,1ba2 <strncpy+0x1f8>
    for (int i = 0; i < n; ++i, *(p++) = c)
    1a00:	00050023          	sb	zero,0(a0)
    1a04:	4685                	li	a3,1
    1a06:	00150713          	addi	a4,a0,1
    1a0a:	18d78f63          	beq	a5,a3,1ba8 <strncpy+0x1fe>
    1a0e:	000500a3          	sb	zero,1(a0)
    1a12:	4689                	li	a3,2
    1a14:	00250713          	addi	a4,a0,2
    1a18:	18d78e63          	beq	a5,a3,1bb4 <strncpy+0x20a>
    1a1c:	00050123          	sb	zero,2(a0)
    1a20:	468d                	li	a3,3
    1a22:	00350713          	addi	a4,a0,3
    1a26:	16d78c63          	beq	a5,a3,1b9e <strncpy+0x1f4>
    1a2a:	000501a3          	sb	zero,3(a0)
    1a2e:	4691                	li	a3,4
    1a30:	00450713          	addi	a4,a0,4
    1a34:	18d78263          	beq	a5,a3,1bb8 <strncpy+0x20e>
    1a38:	00050223          	sb	zero,4(a0)
    1a3c:	4695                	li	a3,5
    1a3e:	00550713          	addi	a4,a0,5
    1a42:	16d78d63          	beq	a5,a3,1bbc <strncpy+0x212>
    1a46:	000502a3          	sb	zero,5(a0)
    1a4a:	469d                	li	a3,7
    1a4c:	00650713          	addi	a4,a0,6
    1a50:	16d79863          	bne	a5,a3,1bc0 <strncpy+0x216>
    1a54:	00750713          	addi	a4,a0,7
    1a58:	00050323          	sb	zero,6(a0)
    1a5c:	40f80833          	sub	a6,a6,a5
    1a60:	ff887593          	andi	a1,a6,-8
    1a64:	97aa                	add	a5,a5,a0
    1a66:	95be                	add	a1,a1,a5
    1a68:	0007b023          	sd	zero,0(a5)
    1a6c:	07a1                	addi	a5,a5,8
    1a6e:	feb79de3          	bne	a5,a1,1a68 <strncpy+0xbe>
    1a72:	ff887593          	andi	a1,a6,-8
    1a76:	9ead                	addw	a3,a3,a1
    1a78:	00b707b3          	add	a5,a4,a1
    1a7c:	12b80863          	beq	a6,a1,1bac <strncpy+0x202>
    1a80:	00078023          	sb	zero,0(a5)
    1a84:	0016871b          	addiw	a4,a3,1
    1a88:	0ec77863          	bgeu	a4,a2,1b78 <strncpy+0x1ce>
    1a8c:	000780a3          	sb	zero,1(a5)
    1a90:	0026871b          	addiw	a4,a3,2
    1a94:	0ec77263          	bgeu	a4,a2,1b78 <strncpy+0x1ce>
    1a98:	00078123          	sb	zero,2(a5)
    1a9c:	0036871b          	addiw	a4,a3,3
    1aa0:	0cc77c63          	bgeu	a4,a2,1b78 <strncpy+0x1ce>
    1aa4:	000781a3          	sb	zero,3(a5)
    1aa8:	0046871b          	addiw	a4,a3,4
    1aac:	0cc77663          	bgeu	a4,a2,1b78 <strncpy+0x1ce>
    1ab0:	00078223          	sb	zero,4(a5)
    1ab4:	0056871b          	addiw	a4,a3,5
    1ab8:	0cc77063          	bgeu	a4,a2,1b78 <strncpy+0x1ce>
    1abc:	000782a3          	sb	zero,5(a5)
    1ac0:	0066871b          	addiw	a4,a3,6
    1ac4:	0ac77a63          	bgeu	a4,a2,1b78 <strncpy+0x1ce>
    1ac8:	00078323          	sb	zero,6(a5)
    1acc:	0076871b          	addiw	a4,a3,7
    1ad0:	0ac77463          	bgeu	a4,a2,1b78 <strncpy+0x1ce>
    1ad4:	000783a3          	sb	zero,7(a5)
    1ad8:	0086871b          	addiw	a4,a3,8
    1adc:	08c77e63          	bgeu	a4,a2,1b78 <strncpy+0x1ce>
    1ae0:	00078423          	sb	zero,8(a5)
    1ae4:	0096871b          	addiw	a4,a3,9
    1ae8:	08c77863          	bgeu	a4,a2,1b78 <strncpy+0x1ce>
    1aec:	000784a3          	sb	zero,9(a5)
    1af0:	00a6871b          	addiw	a4,a3,10
    1af4:	08c77263          	bgeu	a4,a2,1b78 <strncpy+0x1ce>
    1af8:	00078523          	sb	zero,10(a5)
    1afc:	00b6871b          	addiw	a4,a3,11
    1b00:	06c77c63          	bgeu	a4,a2,1b78 <strncpy+0x1ce>
    1b04:	000785a3          	sb	zero,11(a5)
    1b08:	00c6871b          	addiw	a4,a3,12
    1b0c:	06c77663          	bgeu	a4,a2,1b78 <strncpy+0x1ce>
    1b10:	00078623          	sb	zero,12(a5)
    1b14:	26b5                	addiw	a3,a3,13
    1b16:	06c6f163          	bgeu	a3,a2,1b78 <strncpy+0x1ce>
    1b1a:	000786a3          	sb	zero,13(a5)
    1b1e:	8082                	ret
            ;
        if (!n || !*s)
    1b20:	c645                	beqz	a2,1bc8 <strncpy+0x21e>
    1b22:	0005c783          	lbu	a5,0(a1)
    1b26:	ea078be3          	beqz	a5,19dc <strncpy+0x32>
            goto tail;
        wd = (void *)d;
        ws = (const void *)s;
        for (; n >= sizeof(size_t) && !HASZERO(*ws); n -= sizeof(size_t), ws++, wd++)
    1b2a:	479d                	li	a5,7
    1b2c:	02c7ff63          	bgeu	a5,a2,1b6a <strncpy+0x1c0>
    1b30:	00000897          	auipc	a7,0x0
    1b34:	3908b883          	ld	a7,912(a7) # 1ec0 <__func__.0+0x8>
    1b38:	00000817          	auipc	a6,0x0
    1b3c:	39083803          	ld	a6,912(a6) # 1ec8 <__func__.0+0x10>
    1b40:	431d                	li	t1,7
    1b42:	6198                	ld	a4,0(a1)
    1b44:	011707b3          	add	a5,a4,a7
    1b48:	fff74693          	not	a3,a4
    1b4c:	8ff5                	and	a5,a5,a3
    1b4e:	0107f7b3          	and	a5,a5,a6
    1b52:	ef81                	bnez	a5,1b6a <strncpy+0x1c0>
            *wd = *ws;
    1b54:	e118                	sd	a4,0(a0)
        for (; n >= sizeof(size_t) && !HASZERO(*ws); n -= sizeof(size_t), ws++, wd++)
    1b56:	1661                	addi	a2,a2,-8
    1b58:	05a1                	addi	a1,a1,8
    1b5a:	0521                	addi	a0,a0,8
    1b5c:	fec363e3          	bltu	t1,a2,1b42 <strncpy+0x198>
        d = (void *)wd;
        s = (const void *)ws;
    }
    for (; n && (*d = *s); n--, s++, d++)
    1b60:	e609                	bnez	a2,1b6a <strncpy+0x1c0>
    1b62:	a08d                	j	1bc4 <strncpy+0x21a>
    1b64:	167d                	addi	a2,a2,-1
    1b66:	0505                	addi	a0,a0,1
    1b68:	ca01                	beqz	a2,1b78 <strncpy+0x1ce>
    1b6a:	0005c783          	lbu	a5,0(a1)
    1b6e:	0585                	addi	a1,a1,1
    1b70:	00f50023          	sb	a5,0(a0)
    1b74:	fbe5                	bnez	a5,1b64 <strncpy+0x1ba>
        ;
tail:
    1b76:	b59d                	j	19dc <strncpy+0x32>
    memset(d, 0, n);
    return d;
}
    1b78:	8082                	ret
    1b7a:	472d                	li	a4,11
    1b7c:	bdb5                	j	19f8 <strncpy+0x4e>
    1b7e:	00778713          	addi	a4,a5,7
    1b82:	45ad                	li	a1,11
    1b84:	fff60693          	addi	a3,a2,-1
    1b88:	e6b778e3          	bgeu	a4,a1,19f8 <strncpy+0x4e>
    1b8c:	b7fd                	j	1b7a <strncpy+0x1d0>
    1b8e:	40a007b3          	neg	a5,a0
    1b92:	8832                	mv	a6,a2
    1b94:	8b9d                	andi	a5,a5,7
    1b96:	4681                	li	a3,0
    1b98:	e4060be3          	beqz	a2,19ee <strncpy+0x44>
    1b9c:	b7cd                	j	1b7e <strncpy+0x1d4>
    for (int i = 0; i < n; ++i, *(p++) = c)
    1b9e:	468d                	li	a3,3
    1ba0:	bd75                	j	1a5c <strncpy+0xb2>
    1ba2:	872a                	mv	a4,a0
    1ba4:	4681                	li	a3,0
    1ba6:	bd5d                	j	1a5c <strncpy+0xb2>
    1ba8:	4685                	li	a3,1
    1baa:	bd4d                	j	1a5c <strncpy+0xb2>
    1bac:	8082                	ret
    1bae:	87aa                	mv	a5,a0
    1bb0:	4681                	li	a3,0
    1bb2:	b5f9                	j	1a80 <strncpy+0xd6>
    1bb4:	4689                	li	a3,2
    1bb6:	b55d                	j	1a5c <strncpy+0xb2>
    1bb8:	4691                	li	a3,4
    1bba:	b54d                	j	1a5c <strncpy+0xb2>
    1bbc:	4695                	li	a3,5
    1bbe:	bd79                	j	1a5c <strncpy+0xb2>
    1bc0:	4699                	li	a3,6
    1bc2:	bd69                	j	1a5c <strncpy+0xb2>
    1bc4:	8082                	ret
    1bc6:	8082                	ret
    1bc8:	8082                	ret

0000000000001bca <open>:
#include <unistd.h>

#include "syscall.h"

int open(const char *path, int flags)
{
    1bca:	87aa                	mv	a5,a0
    1bcc:	862e                	mv	a2,a1
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
}

static inline long __syscall4(long n, long a, long b, long c, long d)
{
    register long a7 __asm__("a7") = n;
    1bce:	03800893          	li	a7,56
    register long a0 __asm__("a0") = a;
    1bd2:	f9c00513          	li	a0,-100
    register long a1 __asm__("a1") = b;
    1bd6:	85be                	mv	a1,a5
    register long a2 __asm__("a2") = c;
    register long a3 __asm__("a3") = d;
    1bd8:	4689                	li	a3,2
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3))
    1bda:	00000073          	ecall
    return syscall(SYS_openat, AT_FDCWD, path, flags, O_RDWR);
}
    1bde:	2501                	sext.w	a0,a0
    1be0:	8082                	ret

0000000000001be2 <openat>:
    register long a7 __asm__("a7") = n;
    1be2:	03800893          	li	a7,56
    register long a3 __asm__("a3") = d;
    1be6:	18000693          	li	a3,384
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3))
    1bea:	00000073          	ecall

int openat(int dirfd,const char *path, int flags)
{
    return syscall(SYS_openat, dirfd, path, flags, 0600);
}
    1bee:	2501                	sext.w	a0,a0
    1bf0:	8082                	ret

0000000000001bf2 <close>:
    register long a7 __asm__("a7") = n;
    1bf2:	03900893          	li	a7,57
    __asm_syscall("r"(a7), "0"(a0))
    1bf6:	00000073          	ecall

int close(int fd)
{
    return syscall(SYS_close, fd);
}
    1bfa:	2501                	sext.w	a0,a0
    1bfc:	8082                	ret

0000000000001bfe <read>:
    register long a7 __asm__("a7") = n;
    1bfe:	03f00893          	li	a7,63
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1c02:	00000073          	ecall

ssize_t read(int fd, void *buf, size_t len)
{
    return syscall(SYS_read, fd, buf, len);
}
    1c06:	8082                	ret

0000000000001c08 <write>:
    register long a7 __asm__("a7") = n;
    1c08:	04000893          	li	a7,64
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1c0c:	00000073          	ecall

ssize_t write(int fd, const void *buf, size_t len)
{
    return syscall(SYS_write, fd, buf, len);
}
    1c10:	8082                	ret

0000000000001c12 <getpid>:
    register long a7 __asm__("a7") = n;
    1c12:	0ac00893          	li	a7,172
    __asm_syscall("r"(a7))
    1c16:	00000073          	ecall

pid_t getpid(void)
{
    return syscall(SYS_getpid);
}
    1c1a:	2501                	sext.w	a0,a0
    1c1c:	8082                	ret

0000000000001c1e <getppid>:
    register long a7 __asm__("a7") = n;
    1c1e:	0ad00893          	li	a7,173
    __asm_syscall("r"(a7))
    1c22:	00000073          	ecall

pid_t getppid(void)
{
    return syscall(SYS_getppid);
}
    1c26:	2501                	sext.w	a0,a0
    1c28:	8082                	ret

0000000000001c2a <sched_yield>:
    register long a7 __asm__("a7") = n;
    1c2a:	07c00893          	li	a7,124
    __asm_syscall("r"(a7))
    1c2e:	00000073          	ecall

int sched_yield(void)
{
    return syscall(SYS_sched_yield);
}
    1c32:	2501                	sext.w	a0,a0
    1c34:	8082                	ret

0000000000001c36 <fork>:
    register long a7 __asm__("a7") = n;
    1c36:	0dc00893          	li	a7,220
    register long a0 __asm__("a0") = a;
    1c3a:	4545                	li	a0,17
    register long a1 __asm__("a1") = b;
    1c3c:	4581                	li	a1,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1c3e:	00000073          	ecall

pid_t fork(void)
{
    return syscall(SYS_clone, SIGCHLD, 0);
}
    1c42:	2501                	sext.w	a0,a0
    1c44:	8082                	ret

0000000000001c46 <clone>:

pid_t clone(int (*fn)(void *arg), void *arg, void *stack, size_t stack_size, unsigned long flags)
{
    1c46:	85b2                	mv	a1,a2
    1c48:	863a                	mv	a2,a4
    if (stack)
    1c4a:	c191                	beqz	a1,1c4e <clone+0x8>
	stack += stack_size;
    1c4c:	95b6                	add	a1,a1,a3

    return __clone(fn, stack, flags, NULL, NULL, NULL);
    1c4e:	4781                	li	a5,0
    1c50:	4701                	li	a4,0
    1c52:	4681                	li	a3,0
    1c54:	2601                	sext.w	a2,a2
    1c56:	a2dd                	j	1e3c <__clone>

0000000000001c58 <exit>:
    register long a7 __asm__("a7") = n;
    1c58:	05d00893          	li	a7,93
    __asm_syscall("r"(a7), "0"(a0))
    1c5c:	00000073          	ecall
    //return syscall(SYS_clone, fn, stack, flags, NULL, NULL, NULL);
}
void exit(int code)
{
    syscall(SYS_exit, code);
}
    1c60:	8082                	ret

0000000000001c62 <waitpid>:
    register long a7 __asm__("a7") = n;
    1c62:	10400893          	li	a7,260
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1c66:	00000073          	ecall

int waitpid(int pid, int *code, int options)
{
    return syscall(SYS_wait4, pid, code, options);
}
    1c6a:	2501                	sext.w	a0,a0
    1c6c:	8082                	ret

0000000000001c6e <exec>:
    register long a7 __asm__("a7") = n;
    1c6e:	0dd00893          	li	a7,221
    __asm_syscall("r"(a7), "0"(a0))
    1c72:	00000073          	ecall

int exec(char *name)
{
    return syscall(SYS_execve, name);
}
    1c76:	2501                	sext.w	a0,a0
    1c78:	8082                	ret

0000000000001c7a <execve>:
    register long a7 __asm__("a7") = n;
    1c7a:	0dd00893          	li	a7,221
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1c7e:	00000073          	ecall

int execve(const char *name, char *const argv[], char *const argp[])
{
    return syscall(SYS_execve, name, argv, argp);
}
    1c82:	2501                	sext.w	a0,a0
    1c84:	8082                	ret

0000000000001c86 <times>:
    register long a7 __asm__("a7") = n;
    1c86:	09900893          	li	a7,153
    __asm_syscall("r"(a7), "0"(a0))
    1c8a:	00000073          	ecall

int times(void *mytimes)
{
	return syscall(SYS_times, mytimes);
}
    1c8e:	2501                	sext.w	a0,a0
    1c90:	8082                	ret

0000000000001c92 <get_time>:

int64 get_time()
{
    1c92:	1141                	addi	sp,sp,-16
    register long a7 __asm__("a7") = n;
    1c94:	0a900893          	li	a7,169
    register long a0 __asm__("a0") = a;
    1c98:	850a                	mv	a0,sp
    register long a1 __asm__("a1") = b;
    1c9a:	4581                	li	a1,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1c9c:	00000073          	ecall
    TimeVal time;
    int err = sys_get_time(&time, 0);
    if (err == 0)
    1ca0:	2501                	sext.w	a0,a0
    1ca2:	ed09                	bnez	a0,1cbc <get_time+0x2a>
    {
        return ((time.sec & 0xffff) * 1000 + time.usec / 1000);
    1ca4:	67a2                	ld	a5,8(sp)
    1ca6:	3e800713          	li	a4,1000
    1caa:	00015503          	lhu	a0,0(sp)
    1cae:	02e7d7b3          	divu	a5,a5,a4
    1cb2:	02e50533          	mul	a0,a0,a4
    1cb6:	953e                	add	a0,a0,a5
    }
    else
    {
        return -1;
    }
}
    1cb8:	0141                	addi	sp,sp,16
    1cba:	8082                	ret
        return -1;
    1cbc:	557d                	li	a0,-1
    1cbe:	bfed                	j	1cb8 <get_time+0x26>

0000000000001cc0 <sys_get_time>:
    register long a7 __asm__("a7") = n;
    1cc0:	0a900893          	li	a7,169
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1cc4:	00000073          	ecall

int sys_get_time(TimeVal *ts, int tz)
{
    return syscall(SYS_gettimeofday, ts, tz);
}
    1cc8:	2501                	sext.w	a0,a0
    1cca:	8082                	ret

0000000000001ccc <time>:
    register long a7 __asm__("a7") = n;
    1ccc:	42600893          	li	a7,1062
    __asm_syscall("r"(a7), "0"(a0))
    1cd0:	00000073          	ecall

int time(unsigned long *tloc)
{
    return syscall(SYS_time, tloc);
}
    1cd4:	2501                	sext.w	a0,a0
    1cd6:	8082                	ret

0000000000001cd8 <sleep>:

int sleep(unsigned long long time)
{
    1cd8:	1141                	addi	sp,sp,-16
    TimeVal tv = {.sec = time, .usec = 0};
    1cda:	e02a                	sd	a0,0(sp)
    register long a0 __asm__("a0") = a;
    1cdc:	850a                	mv	a0,sp
    1cde:	e402                	sd	zero,8(sp)
    register long a7 __asm__("a7") = n;
    1ce0:	06500893          	li	a7,101
    register long a1 __asm__("a1") = b;
    1ce4:	85aa                	mv	a1,a0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1ce6:	00000073          	ecall
    if (syscall(SYS_nanosleep, &tv, &tv)) return tv.sec;
    1cea:	e501                	bnez	a0,1cf2 <sleep+0x1a>
    return 0;
    1cec:	4501                	li	a0,0
}
    1cee:	0141                	addi	sp,sp,16
    1cf0:	8082                	ret
    if (syscall(SYS_nanosleep, &tv, &tv)) return tv.sec;
    1cf2:	4502                	lw	a0,0(sp)
}
    1cf4:	0141                	addi	sp,sp,16
    1cf6:	8082                	ret

0000000000001cf8 <set_priority>:
    register long a7 __asm__("a7") = n;
    1cf8:	08c00893          	li	a7,140
    __asm_syscall("r"(a7), "0"(a0))
    1cfc:	00000073          	ecall

int set_priority(int prio)
{
    return syscall(SYS_setpriority, prio);
}
    1d00:	2501                	sext.w	a0,a0
    1d02:	8082                	ret

0000000000001d04 <mmap>:
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4))
}

static inline long __syscall6(long n, long a, long b, long c, long d, long e, long f)
{
    register long a7 __asm__("a7") = n;
    1d04:	0de00893          	li	a7,222
    register long a1 __asm__("a1") = b;
    register long a2 __asm__("a2") = c;
    register long a3 __asm__("a3") = d;
    register long a4 __asm__("a4") = e;
    register long a5 __asm__("a5") = f;
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4), "r"(a5))
    1d08:	00000073          	ecall

void *mmap(void *start, size_t len, int prot, int flags, int fd, off_t off)
{
    return syscall(SYS_mmap, start, len, prot, flags, fd, off);
}
    1d0c:	8082                	ret

0000000000001d0e <munmap>:
    register long a7 __asm__("a7") = n;
    1d0e:	0d700893          	li	a7,215
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1d12:	00000073          	ecall

int munmap(void *start, size_t len)
{
    return syscall(SYS_munmap, start, len);
}
    1d16:	2501                	sext.w	a0,a0
    1d18:	8082                	ret

0000000000001d1a <wait>:

int wait(int *code)
{
    1d1a:	85aa                	mv	a1,a0
    register long a7 __asm__("a7") = n;
    1d1c:	10400893          	li	a7,260
    register long a0 __asm__("a0") = a;
    1d20:	557d                	li	a0,-1
    register long a2 __asm__("a2") = c;
    1d22:	4601                	li	a2,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1d24:	00000073          	ecall
    return waitpid(-1, code, 0);
}
    1d28:	2501                	sext.w	a0,a0
    1d2a:	8082                	ret

0000000000001d2c <spawn>:
    register long a7 __asm__("a7") = n;
    1d2c:	19000893          	li	a7,400
    __asm_syscall("r"(a7), "0"(a0))
    1d30:	00000073          	ecall

int spawn(char *file)
{
    return syscall(SYS_spawn, file);
}
    1d34:	2501                	sext.w	a0,a0
    1d36:	8082                	ret

0000000000001d38 <mailread>:
    register long a7 __asm__("a7") = n;
    1d38:	19100893          	li	a7,401
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1d3c:	00000073          	ecall

int mailread(void *buf, int len)
{
    return syscall(SYS_mailread, buf, len);
}
    1d40:	2501                	sext.w	a0,a0
    1d42:	8082                	ret

0000000000001d44 <mailwrite>:
    register long a7 __asm__("a7") = n;
    1d44:	19200893          	li	a7,402
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1d48:	00000073          	ecall

int mailwrite(int pid, void *buf, int len)
{
    return syscall(SYS_mailwrite, pid, buf, len);
}
    1d4c:	2501                	sext.w	a0,a0
    1d4e:	8082                	ret

0000000000001d50 <fstat>:
    register long a7 __asm__("a7") = n;
    1d50:	05000893          	li	a7,80
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1d54:	00000073          	ecall

int fstat(int fd, struct kstat *st)
{
    return syscall(SYS_fstat, fd, st);
}
    1d58:	2501                	sext.w	a0,a0
    1d5a:	8082                	ret

0000000000001d5c <sys_linkat>:
    register long a4 __asm__("a4") = e;
    1d5c:	1702                	slli	a4,a4,0x20
    register long a7 __asm__("a7") = n;
    1d5e:	02500893          	li	a7,37
    register long a4 __asm__("a4") = e;
    1d62:	9301                	srli	a4,a4,0x20
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4))
    1d64:	00000073          	ecall

int sys_linkat(int olddirfd, char *oldpath, int newdirfd, char *newpath, unsigned int flags)
{
    return syscall(SYS_linkat, olddirfd, oldpath, newdirfd, newpath, flags);
}
    1d68:	2501                	sext.w	a0,a0
    1d6a:	8082                	ret

0000000000001d6c <sys_unlinkat>:
    register long a2 __asm__("a2") = c;
    1d6c:	1602                	slli	a2,a2,0x20
    register long a7 __asm__("a7") = n;
    1d6e:	02300893          	li	a7,35
    register long a2 __asm__("a2") = c;
    1d72:	9201                	srli	a2,a2,0x20
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1d74:	00000073          	ecall

int sys_unlinkat(int dirfd, char *path, unsigned int flags)
{
    return syscall(SYS_unlinkat, dirfd, path, flags);
}
    1d78:	2501                	sext.w	a0,a0
    1d7a:	8082                	ret

0000000000001d7c <link>:

int link(char *old_path, char *new_path)
{
    1d7c:	87aa                	mv	a5,a0
    1d7e:	86ae                	mv	a3,a1
    register long a7 __asm__("a7") = n;
    1d80:	02500893          	li	a7,37
    register long a0 __asm__("a0") = a;
    1d84:	f9c00513          	li	a0,-100
    register long a1 __asm__("a1") = b;
    1d88:	85be                	mv	a1,a5
    register long a2 __asm__("a2") = c;
    1d8a:	f9c00613          	li	a2,-100
    register long a4 __asm__("a4") = e;
    1d8e:	4701                	li	a4,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4))
    1d90:	00000073          	ecall
    return sys_linkat(AT_FDCWD, old_path, AT_FDCWD, new_path, 0);
}
    1d94:	2501                	sext.w	a0,a0
    1d96:	8082                	ret

0000000000001d98 <unlink>:

int unlink(char *path)
{
    1d98:	85aa                	mv	a1,a0
    register long a7 __asm__("a7") = n;
    1d9a:	02300893          	li	a7,35
    register long a0 __asm__("a0") = a;
    1d9e:	f9c00513          	li	a0,-100
    register long a2 __asm__("a2") = c;
    1da2:	4601                	li	a2,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1da4:	00000073          	ecall
    return sys_unlinkat(AT_FDCWD, path, 0);
}
    1da8:	2501                	sext.w	a0,a0
    1daa:	8082                	ret

0000000000001dac <uname>:
    register long a7 __asm__("a7") = n;
    1dac:	0a000893          	li	a7,160
    __asm_syscall("r"(a7), "0"(a0))
    1db0:	00000073          	ecall

int uname(void *buf)
{
    return syscall(SYS_uname, buf);
}
    1db4:	2501                	sext.w	a0,a0
    1db6:	8082                	ret

0000000000001db8 <brk>:
    register long a7 __asm__("a7") = n;
    1db8:	0d600893          	li	a7,214
    __asm_syscall("r"(a7), "0"(a0))
    1dbc:	00000073          	ecall

int brk(void *addr)
{
    return syscall(SYS_brk, addr);
}
    1dc0:	2501                	sext.w	a0,a0
    1dc2:	8082                	ret

0000000000001dc4 <getcwd>:
    register long a7 __asm__("a7") = n;
    1dc4:	48c5                	li	a7,17
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1dc6:	00000073          	ecall

char *getcwd(char *buf, size_t size){
    return syscall(SYS_getcwd, buf, size);
}
    1dca:	8082                	ret

0000000000001dcc <chdir>:
    register long a7 __asm__("a7") = n;
    1dcc:	03100893          	li	a7,49
    __asm_syscall("r"(a7), "0"(a0))
    1dd0:	00000073          	ecall

int chdir(const char *path){
    return syscall(SYS_chdir, path);
}
    1dd4:	2501                	sext.w	a0,a0
    1dd6:	8082                	ret

0000000000001dd8 <mkdir>:

int mkdir(const char *path, mode_t mode){
    1dd8:	862e                	mv	a2,a1
    1dda:	87aa                	mv	a5,a0
    register long a2 __asm__("a2") = c;
    1ddc:	1602                	slli	a2,a2,0x20
    register long a7 __asm__("a7") = n;
    1dde:	02200893          	li	a7,34
    register long a0 __asm__("a0") = a;
    1de2:	f9c00513          	li	a0,-100
    register long a1 __asm__("a1") = b;
    1de6:	85be                	mv	a1,a5
    register long a2 __asm__("a2") = c;
    1de8:	9201                	srli	a2,a2,0x20
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1dea:	00000073          	ecall
    return syscall(SYS_mkdirat, AT_FDCWD, path, mode);
}
    1dee:	2501                	sext.w	a0,a0
    1df0:	8082                	ret

0000000000001df2 <getdents>:
    register long a7 __asm__("a7") = n;
    1df2:	03d00893          	li	a7,61
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1df6:	00000073          	ecall

int getdents(int fd, struct linux_dirent64 *dirp64, unsigned long len){
    //return syscall(SYS_getdents64, fd, dirp64, len);
    return syscall(SYS_getdents64, fd, dirp64, len);
}
    1dfa:	2501                	sext.w	a0,a0
    1dfc:	8082                	ret

0000000000001dfe <pipe>:
    register long a7 __asm__("a7") = n;
    1dfe:	03b00893          	li	a7,59
    register long a1 __asm__("a1") = b;
    1e02:	4581                	li	a1,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1e04:	00000073          	ecall

int pipe(int fd[2]){
    return syscall(SYS_pipe2, fd, 0);
}
    1e08:	2501                	sext.w	a0,a0
    1e0a:	8082                	ret

0000000000001e0c <dup>:
    register long a7 __asm__("a7") = n;
    1e0c:	48dd                	li	a7,23
    __asm_syscall("r"(a7), "0"(a0))
    1e0e:	00000073          	ecall

int dup(int fd){
    return syscall(SYS_dup, fd);
}
    1e12:	2501                	sext.w	a0,a0
    1e14:	8082                	ret

0000000000001e16 <dup2>:
    register long a7 __asm__("a7") = n;
    1e16:	48e1                	li	a7,24
    register long a2 __asm__("a2") = c;
    1e18:	4601                	li	a2,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1e1a:	00000073          	ecall

int dup2(int old, int new){
    return syscall(SYS_dup3, old, new, 0);
}
    1e1e:	2501                	sext.w	a0,a0
    1e20:	8082                	ret

0000000000001e22 <mount>:
    register long a7 __asm__("a7") = n;
    1e22:	02800893          	li	a7,40
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4))
    1e26:	00000073          	ecall

int mount(const char *special, const char *dir, const char *fstype, unsigned long flags, const void *data)
{
        return syscall(SYS_mount, special, dir, fstype, flags, data);
}
    1e2a:	2501                	sext.w	a0,a0
    1e2c:	8082                	ret

0000000000001e2e <umount>:
    register long a7 __asm__("a7") = n;
    1e2e:	02700893          	li	a7,39
    register long a1 __asm__("a1") = b;
    1e32:	4581                	li	a1,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1e34:	00000073          	ecall

int umount(const char *special)
{
        return syscall(SYS_umount2, special, 0);
}
    1e38:	2501                	sext.w	a0,a0
    1e3a:	8082                	ret

0000000000001e3c <__clone>:

.global __clone
.type  __clone, %function
__clone:
	# Save func and arg to stack
	addi a1, a1, -16
    1e3c:	15c1                	addi	a1,a1,-16
	sd a0, 0(a1)
    1e3e:	e188                	sd	a0,0(a1)
	sd a3, 8(a1)
    1e40:	e594                	sd	a3,8(a1)

	# Call SYS_clone
	mv a0, a2
    1e42:	8532                	mv	a0,a2
	mv a2, a4
    1e44:	863a                	mv	a2,a4
	mv a3, a5
    1e46:	86be                	mv	a3,a5
	mv a4, a6
    1e48:	8742                	mv	a4,a6
	li a7, 220 # SYS_clone
    1e4a:	0dc00893          	li	a7,220
	ecall
    1e4e:	00000073          	ecall

	beqz a0, 1f
    1e52:	c111                	beqz	a0,1e56 <__clone+0x1a>
	# Parent
	ret
    1e54:	8082                	ret

	# Child
1:      ld a1, 0(sp)
    1e56:	6582                	ld	a1,0(sp)
	ld a0, 8(sp)
    1e58:	6522                	ld	a0,8(sp)
	jalr a1
    1e5a:	9582                	jalr	a1

	# Exit
	li a7, 93 # SYS_exit
    1e5c:	05d00893          	li	a7,93
	ecall
    1e60:	00000073          	ecall
