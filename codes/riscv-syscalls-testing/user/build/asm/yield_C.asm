
/home/ubuntu/rust/ultra-os/codes/riscv-syscalls-testing/user/build/riscv64/yield_C:     file format elf64-littleriscv


Disassembly of section .text:

0000000000001000 <_start>:
.section .text.entry
.globl _start
_start:
    mv a0, sp
    1000:	850a                	mv	a0,sp
    tail __start_main
    1002:	a8a1                	j	105a <__start_main>

0000000000001004 <main>:
/*
理想结果：三个程序交替输出 ABC
*/

int main()
{
    1004:	7139                	addi	sp,sp,-64
    1006:	f426                	sd	s1,40(sp)
    for (int i = 0; i < HEIGHT; ++i)
    {
        char buf[WIDTH + 1];
        for (int j = 0; j < WIDTH; ++j)
            buf[j] = 'C';
    1008:	6491                	lui	s1,0x4
{
    100a:	f822                	sd	s0,48(sp)
    100c:	f04a                	sd	s2,32(sp)
    100e:	ec4e                	sd	s3,24(sp)
    1010:	fc06                	sd	ra,56(sp)
    for (int i = 0; i < HEIGHT; ++i)
    1012:	4401                	li	s0,0
            buf[j] = 'C';
    1014:	00001997          	auipc	s3,0x1
    1018:	e849b983          	ld	s3,-380(s3) # 1e98 <__clone+0x42>
    101c:	34348493          	addi	s1,s1,835 # 4343 <digits+0x2483>
        buf[WIDTH] = 0;
        printf("%s [%d/%d]\n", buf, i + 1, HEIGHT);
    1020:	00001917          	auipc	s2,0x1
    1024:	e6090913          	addi	s2,s2,-416 # 1e80 <__clone+0x2a>
    1028:	2405                	addiw	s0,s0,1
    102a:	4695                	li	a3,5
    102c:	8622                	mv	a2,s0
    102e:	858a                	mv	a1,sp
    1030:	854a                	mv	a0,s2
            buf[j] = 'C';
    1032:	e04e                	sd	s3,0(sp)
    1034:	00911423          	sh	s1,8(sp)
        buf[WIDTH] = 0;
    1038:	00010523          	sb	zero,10(sp)
        printf("%s [%d/%d]\n", buf, i + 1, HEIGHT);
    103c:	2b2000ef          	jal	ra,12ee <printf>
        sched_yield();
    1040:	405000ef          	jal	ra,1c44 <sched_yield>
    for (int i = 0; i < HEIGHT; ++i)
    1044:	4795                	li	a5,5
    1046:	fef411e3          	bne	s0,a5,1028 <main+0x24>
    }
    //puts("Test write C OK!");
    return 0;
}
    104a:	70e2                	ld	ra,56(sp)
    104c:	7442                	ld	s0,48(sp)
    104e:	74a2                	ld	s1,40(sp)
    1050:	7902                	ld	s2,32(sp)
    1052:	69e2                	ld	s3,24(sp)
    1054:	4501                	li	a0,0
    1056:	6121                	addi	sp,sp,64
    1058:	8082                	ret

000000000000105a <__start_main>:
#include <unistd.h>

extern int main();

int __start_main(long *p)
{
    105a:	85aa                	mv	a1,a0
	int argc = p[0];
	char **argv = (void *)(p+1);

	exit(main(argc, argv));
    105c:	4108                	lw	a0,0(a0)
{
    105e:	1141                	addi	sp,sp,-16
	exit(main(argc, argv));
    1060:	05a1                	addi	a1,a1,8
{
    1062:	e406                	sd	ra,8(sp)
	exit(main(argc, argv));
    1064:	fa1ff0ef          	jal	ra,1004 <main>
    1068:	40b000ef          	jal	ra,1c72 <exit>
	return 0;
}
    106c:	60a2                	ld	ra,8(sp)
    106e:	4501                	li	a0,0
    1070:	0141                	addi	sp,sp,16
    1072:	8082                	ret

0000000000001074 <printint.constprop.0>:
    write(f, s, l);
}

static char digits[] = "0123456789abcdef";

static void printint(int xx, int base, int sign)
    1074:	7179                	addi	sp,sp,-48
    1076:	f406                	sd	ra,40(sp)
{
    char buf[16 + 1];
    int i;
    uint x;

    if (sign && (sign = xx < 0))
    1078:	12054b63          	bltz	a0,11ae <printint.constprop.0+0x13a>

    buf[16] = 0;
    i = 15;
    do
    {
        buf[i--] = digits[x % base];
    107c:	02b577bb          	remuw	a5,a0,a1
    1080:	00001617          	auipc	a2,0x1
    1084:	e4060613          	addi	a2,a2,-448 # 1ec0 <digits>
    buf[16] = 0;
    1088:	00010c23          	sb	zero,24(sp)
        buf[i--] = digits[x % base];
    108c:	0005871b          	sext.w	a4,a1
    1090:	1782                	slli	a5,a5,0x20
    1092:	9381                	srli	a5,a5,0x20
    1094:	97b2                	add	a5,a5,a2
    1096:	0007c783          	lbu	a5,0(a5)
    } while ((x /= base) != 0);
    109a:	02b5583b          	divuw	a6,a0,a1
        buf[i--] = digits[x % base];
    109e:	00f10ba3          	sb	a5,23(sp)
    } while ((x /= base) != 0);
    10a2:	1cb56363          	bltu	a0,a1,1268 <printint.constprop.0+0x1f4>
        buf[i--] = digits[x % base];
    10a6:	45b9                	li	a1,14
    10a8:	02e877bb          	remuw	a5,a6,a4
    10ac:	1782                	slli	a5,a5,0x20
    10ae:	9381                	srli	a5,a5,0x20
    10b0:	97b2                	add	a5,a5,a2
    10b2:	0007c783          	lbu	a5,0(a5)
    } while ((x /= base) != 0);
    10b6:	02e856bb          	divuw	a3,a6,a4
        buf[i--] = digits[x % base];
    10ba:	00f10b23          	sb	a5,22(sp)
    } while ((x /= base) != 0);
    10be:	0ce86e63          	bltu	a6,a4,119a <printint.constprop.0+0x126>
        buf[i--] = digits[x % base];
    10c2:	02e6f5bb          	remuw	a1,a3,a4
    } while ((x /= base) != 0);
    10c6:	02e6d7bb          	divuw	a5,a3,a4
        buf[i--] = digits[x % base];
    10ca:	1582                	slli	a1,a1,0x20
    10cc:	9181                	srli	a1,a1,0x20
    10ce:	95b2                	add	a1,a1,a2
    10d0:	0005c583          	lbu	a1,0(a1)
    10d4:	00b10aa3          	sb	a1,21(sp)
    } while ((x /= base) != 0);
    10d8:	0007859b          	sext.w	a1,a5
    10dc:	12e6ec63          	bltu	a3,a4,1214 <printint.constprop.0+0x1a0>
        buf[i--] = digits[x % base];
    10e0:	02e7f6bb          	remuw	a3,a5,a4
    10e4:	1682                	slli	a3,a3,0x20
    10e6:	9281                	srli	a3,a3,0x20
    10e8:	96b2                	add	a3,a3,a2
    10ea:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    10ee:	02e7d83b          	divuw	a6,a5,a4
        buf[i--] = digits[x % base];
    10f2:	00d10a23          	sb	a3,20(sp)
    } while ((x /= base) != 0);
    10f6:	12e5e863          	bltu	a1,a4,1226 <printint.constprop.0+0x1b2>
        buf[i--] = digits[x % base];
    10fa:	02e876bb          	remuw	a3,a6,a4
    10fe:	1682                	slli	a3,a3,0x20
    1100:	9281                	srli	a3,a3,0x20
    1102:	96b2                	add	a3,a3,a2
    1104:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    1108:	02e855bb          	divuw	a1,a6,a4
        buf[i--] = digits[x % base];
    110c:	00d109a3          	sb	a3,19(sp)
    } while ((x /= base) != 0);
    1110:	12e86463          	bltu	a6,a4,1238 <printint.constprop.0+0x1c4>
        buf[i--] = digits[x % base];
    1114:	02e5f6bb          	remuw	a3,a1,a4
    1118:	1682                	slli	a3,a3,0x20
    111a:	9281                	srli	a3,a3,0x20
    111c:	96b2                	add	a3,a3,a2
    111e:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    1122:	02e5d83b          	divuw	a6,a1,a4
        buf[i--] = digits[x % base];
    1126:	00d10923          	sb	a3,18(sp)
    } while ((x /= base) != 0);
    112a:	0ce5ec63          	bltu	a1,a4,1202 <printint.constprop.0+0x18e>
        buf[i--] = digits[x % base];
    112e:	02e876bb          	remuw	a3,a6,a4
    1132:	1682                	slli	a3,a3,0x20
    1134:	9281                	srli	a3,a3,0x20
    1136:	96b2                	add	a3,a3,a2
    1138:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    113c:	02e855bb          	divuw	a1,a6,a4
        buf[i--] = digits[x % base];
    1140:	00d108a3          	sb	a3,17(sp)
    } while ((x /= base) != 0);
    1144:	10e86963          	bltu	a6,a4,1256 <printint.constprop.0+0x1e2>
        buf[i--] = digits[x % base];
    1148:	02e5f6bb          	remuw	a3,a1,a4
    114c:	1682                	slli	a3,a3,0x20
    114e:	9281                	srli	a3,a3,0x20
    1150:	96b2                	add	a3,a3,a2
    1152:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    1156:	02e5d83b          	divuw	a6,a1,a4
        buf[i--] = digits[x % base];
    115a:	00d10823          	sb	a3,16(sp)
    } while ((x /= base) != 0);
    115e:	10e5e763          	bltu	a1,a4,126c <printint.constprop.0+0x1f8>
        buf[i--] = digits[x % base];
    1162:	02e876bb          	remuw	a3,a6,a4
    1166:	1682                	slli	a3,a3,0x20
    1168:	9281                	srli	a3,a3,0x20
    116a:	96b2                	add	a3,a3,a2
    116c:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    1170:	02e857bb          	divuw	a5,a6,a4
        buf[i--] = digits[x % base];
    1174:	00d107a3          	sb	a3,15(sp)
    } while ((x /= base) != 0);
    1178:	10e86363          	bltu	a6,a4,127e <printint.constprop.0+0x20a>
        buf[i--] = digits[x % base];
    117c:	1782                	slli	a5,a5,0x20
    117e:	9381                	srli	a5,a5,0x20
    1180:	97b2                	add	a5,a5,a2
    1182:	0007c783          	lbu	a5,0(a5)
    1186:	4599                	li	a1,6
    1188:	00f10723          	sb	a5,14(sp)

    if (sign)
    118c:	00055763          	bgez	a0,119a <printint.constprop.0+0x126>
        buf[i--] = '-';
    1190:	02d00793          	li	a5,45
    1194:	00f106a3          	sb	a5,13(sp)
        buf[i--] = digits[x % base];
    1198:	4595                	li	a1,5
    write(f, s, l);
    119a:	003c                	addi	a5,sp,8
    119c:	4641                	li	a2,16
    119e:	9e0d                	subw	a2,a2,a1
    11a0:	4505                	li	a0,1
    11a2:	95be                	add	a1,a1,a5
    11a4:	27f000ef          	jal	ra,1c22 <write>
    i++;
    if (i < 0)
        puts("printint error");
    out(stdout, buf + i, 16 - i);
}
    11a8:	70a2                	ld	ra,40(sp)
    11aa:	6145                	addi	sp,sp,48
    11ac:	8082                	ret
        x = -xx;
    11ae:	40a0083b          	negw	a6,a0
        buf[i--] = digits[x % base];
    11b2:	02b877bb          	remuw	a5,a6,a1
    11b6:	00001617          	auipc	a2,0x1
    11ba:	d0a60613          	addi	a2,a2,-758 # 1ec0 <digits>
    buf[16] = 0;
    11be:	00010c23          	sb	zero,24(sp)
        buf[i--] = digits[x % base];
    11c2:	0005871b          	sext.w	a4,a1
    11c6:	1782                	slli	a5,a5,0x20
    11c8:	9381                	srli	a5,a5,0x20
    11ca:	97b2                	add	a5,a5,a2
    11cc:	0007c783          	lbu	a5,0(a5)
    } while ((x /= base) != 0);
    11d0:	02b858bb          	divuw	a7,a6,a1
        buf[i--] = digits[x % base];
    11d4:	00f10ba3          	sb	a5,23(sp)
    } while ((x /= base) != 0);
    11d8:	06b86963          	bltu	a6,a1,124a <printint.constprop.0+0x1d6>
        buf[i--] = digits[x % base];
    11dc:	02e8f7bb          	remuw	a5,a7,a4
    11e0:	1782                	slli	a5,a5,0x20
    11e2:	9381                	srli	a5,a5,0x20
    11e4:	97b2                	add	a5,a5,a2
    11e6:	0007c783          	lbu	a5,0(a5)
    } while ((x /= base) != 0);
    11ea:	02e8d6bb          	divuw	a3,a7,a4
        buf[i--] = digits[x % base];
    11ee:	00f10b23          	sb	a5,22(sp)
    } while ((x /= base) != 0);
    11f2:	ece8f8e3          	bgeu	a7,a4,10c2 <printint.constprop.0+0x4e>
        buf[i--] = '-';
    11f6:	02d00793          	li	a5,45
    11fa:	00f10aa3          	sb	a5,21(sp)
        buf[i--] = digits[x % base];
    11fe:	45b5                	li	a1,13
    1200:	bf69                	j	119a <printint.constprop.0+0x126>
    1202:	45a9                	li	a1,10
    if (sign)
    1204:	f8055be3          	bgez	a0,119a <printint.constprop.0+0x126>
        buf[i--] = '-';
    1208:	02d00793          	li	a5,45
    120c:	00f108a3          	sb	a5,17(sp)
        buf[i--] = digits[x % base];
    1210:	45a5                	li	a1,9
    1212:	b761                	j	119a <printint.constprop.0+0x126>
    1214:	45b5                	li	a1,13
    if (sign)
    1216:	f80552e3          	bgez	a0,119a <printint.constprop.0+0x126>
        buf[i--] = '-';
    121a:	02d00793          	li	a5,45
    121e:	00f10a23          	sb	a5,20(sp)
        buf[i--] = digits[x % base];
    1222:	45b1                	li	a1,12
    1224:	bf9d                	j	119a <printint.constprop.0+0x126>
    1226:	45b1                	li	a1,12
    if (sign)
    1228:	f60559e3          	bgez	a0,119a <printint.constprop.0+0x126>
        buf[i--] = '-';
    122c:	02d00793          	li	a5,45
    1230:	00f109a3          	sb	a5,19(sp)
        buf[i--] = digits[x % base];
    1234:	45ad                	li	a1,11
    1236:	b795                	j	119a <printint.constprop.0+0x126>
    1238:	45ad                	li	a1,11
    if (sign)
    123a:	f60550e3          	bgez	a0,119a <printint.constprop.0+0x126>
        buf[i--] = '-';
    123e:	02d00793          	li	a5,45
    1242:	00f10923          	sb	a5,18(sp)
        buf[i--] = digits[x % base];
    1246:	45a9                	li	a1,10
    1248:	bf89                	j	119a <printint.constprop.0+0x126>
        buf[i--] = '-';
    124a:	02d00793          	li	a5,45
    124e:	00f10b23          	sb	a5,22(sp)
        buf[i--] = digits[x % base];
    1252:	45b9                	li	a1,14
    1254:	b799                	j	119a <printint.constprop.0+0x126>
    1256:	45a5                	li	a1,9
    if (sign)
    1258:	f40551e3          	bgez	a0,119a <printint.constprop.0+0x126>
        buf[i--] = '-';
    125c:	02d00793          	li	a5,45
    1260:	00f10823          	sb	a5,16(sp)
        buf[i--] = digits[x % base];
    1264:	45a1                	li	a1,8
    1266:	bf15                	j	119a <printint.constprop.0+0x126>
    i = 15;
    1268:	45bd                	li	a1,15
    126a:	bf05                	j	119a <printint.constprop.0+0x126>
        buf[i--] = digits[x % base];
    126c:	45a1                	li	a1,8
    if (sign)
    126e:	f20556e3          	bgez	a0,119a <printint.constprop.0+0x126>
        buf[i--] = '-';
    1272:	02d00793          	li	a5,45
    1276:	00f107a3          	sb	a5,15(sp)
        buf[i--] = digits[x % base];
    127a:	459d                	li	a1,7
    127c:	bf39                	j	119a <printint.constprop.0+0x126>
    127e:	459d                	li	a1,7
    if (sign)
    1280:	f0055de3          	bgez	a0,119a <printint.constprop.0+0x126>
        buf[i--] = '-';
    1284:	02d00793          	li	a5,45
    1288:	00f10723          	sb	a5,14(sp)
        buf[i--] = digits[x % base];
    128c:	4599                	li	a1,6
    128e:	b731                	j	119a <printint.constprop.0+0x126>

0000000000001290 <getchar>:
{
    1290:	1101                	addi	sp,sp,-32
    read(stdin, &byte, 1);
    1292:	00f10593          	addi	a1,sp,15
    1296:	4605                	li	a2,1
    1298:	4501                	li	a0,0
{
    129a:	ec06                	sd	ra,24(sp)
    char byte = 0;
    129c:	000107a3          	sb	zero,15(sp)
    read(stdin, &byte, 1);
    12a0:	179000ef          	jal	ra,1c18 <read>
}
    12a4:	60e2                	ld	ra,24(sp)
    12a6:	00f14503          	lbu	a0,15(sp)
    12aa:	6105                	addi	sp,sp,32
    12ac:	8082                	ret

00000000000012ae <putchar>:
{
    12ae:	1101                	addi	sp,sp,-32
    12b0:	87aa                	mv	a5,a0
    return write(stdout, &byte, 1);
    12b2:	00f10593          	addi	a1,sp,15
    12b6:	4605                	li	a2,1
    12b8:	4505                	li	a0,1
{
    12ba:	ec06                	sd	ra,24(sp)
    char byte = c;
    12bc:	00f107a3          	sb	a5,15(sp)
    return write(stdout, &byte, 1);
    12c0:	163000ef          	jal	ra,1c22 <write>
}
    12c4:	60e2                	ld	ra,24(sp)
    12c6:	2501                	sext.w	a0,a0
    12c8:	6105                	addi	sp,sp,32
    12ca:	8082                	ret

00000000000012cc <puts>:
{
    12cc:	1141                	addi	sp,sp,-16
    12ce:	e406                	sd	ra,8(sp)
    12d0:	e022                	sd	s0,0(sp)
    12d2:	842a                	mv	s0,a0
    r = -(write(stdout, s, strlen(s)) < 0);
    12d4:	56a000ef          	jal	ra,183e <strlen>
    12d8:	862a                	mv	a2,a0
    12da:	85a2                	mv	a1,s0
    12dc:	4505                	li	a0,1
    12de:	145000ef          	jal	ra,1c22 <write>
}
    12e2:	60a2                	ld	ra,8(sp)
    12e4:	6402                	ld	s0,0(sp)
    r = -(write(stdout, s, strlen(s)) < 0);
    12e6:	957d                	srai	a0,a0,0x3f
    return r;
    12e8:	2501                	sext.w	a0,a0
}
    12ea:	0141                	addi	sp,sp,16
    12ec:	8082                	ret

00000000000012ee <printf>:
    out(stdout, buf, i);
}

// Print to the console. only understands %d, %x, %p, %s.
void printf(const char *fmt, ...)
{
    12ee:	7171                	addi	sp,sp,-176
    12f0:	fc56                	sd	s5,56(sp)
    12f2:	ed3e                	sd	a5,152(sp)
    buf[i++] = '0';
    12f4:	7ae1                	lui	s5,0xffff8
    va_list ap;
    int cnt = 0, l = 0;
    char *a, *z, *s = (char *)fmt, str;
    int f = stdout;

    va_start(ap, fmt);
    12f6:	18bc                	addi	a5,sp,120
{
    12f8:	e8ca                	sd	s2,80(sp)
    12fa:	e4ce                	sd	s3,72(sp)
    12fc:	e0d2                	sd	s4,64(sp)
    12fe:	f85a                	sd	s6,48(sp)
    1300:	f486                	sd	ra,104(sp)
    1302:	f0a2                	sd	s0,96(sp)
    1304:	eca6                	sd	s1,88(sp)
    1306:	fcae                	sd	a1,120(sp)
    1308:	e132                	sd	a2,128(sp)
    130a:	e536                	sd	a3,136(sp)
    130c:	e93a                	sd	a4,144(sp)
    130e:	f142                	sd	a6,160(sp)
    1310:	f546                	sd	a7,168(sp)
    va_start(ap, fmt);
    1312:	e03e                	sd	a5,0(sp)
    for (;;)
    {
        if (!*s)
            break;
        for (a = s; *s && *s != '%'; s++)
    1314:	02500913          	li	s2,37
        out(f, a, l);
        if (l)
            continue;
        if (s[1] == 0)
            break;
        switch (s[1])
    1318:	07300a13          	li	s4,115
        case 'p':
            printptr(va_arg(ap, uint64));
            break;
        case 's':
            if ((a = va_arg(ap, char *)) == 0)
                a = "(null)";
    131c:	00001b17          	auipc	s6,0x1
    1320:	b74b0b13          	addi	s6,s6,-1164 # 1e90 <__clone+0x3a>
    buf[i++] = '0';
    1324:	830aca93          	xori	s5,s5,-2000
        buf[i++] = digits[x >> (sizeof(uint64) * 8 - 4)];
    1328:	00001997          	auipc	s3,0x1
    132c:	b9898993          	addi	s3,s3,-1128 # 1ec0 <digits>
        if (!*s)
    1330:	00054783          	lbu	a5,0(a0)
    1334:	16078a63          	beqz	a5,14a8 <printf+0x1ba>
    1338:	862a                	mv	a2,a0
        for (a = s; *s && *s != '%'; s++)
    133a:	19278163          	beq	a5,s2,14bc <printf+0x1ce>
    133e:	00164783          	lbu	a5,1(a2)
    1342:	0605                	addi	a2,a2,1
    1344:	fbfd                	bnez	a5,133a <printf+0x4c>
    1346:	84b2                	mv	s1,a2
        l = z - a;
    1348:	40a6043b          	subw	s0,a2,a0
    write(f, s, l);
    134c:	85aa                	mv	a1,a0
    134e:	8622                	mv	a2,s0
    1350:	4505                	li	a0,1
    1352:	0d1000ef          	jal	ra,1c22 <write>
        if (l)
    1356:	18041c63          	bnez	s0,14ee <printf+0x200>
        if (s[1] == 0)
    135a:	0014c783          	lbu	a5,1(s1)
    135e:	14078563          	beqz	a5,14a8 <printf+0x1ba>
        switch (s[1])
    1362:	1d478063          	beq	a5,s4,1522 <printf+0x234>
    1366:	18fa6663          	bltu	s4,a5,14f2 <printf+0x204>
    136a:	06400713          	li	a4,100
    136e:	1ae78063          	beq	a5,a4,150e <printf+0x220>
    1372:	07000713          	li	a4,112
    1376:	1ce79963          	bne	a5,a4,1548 <printf+0x25a>
            printptr(va_arg(ap, uint64));
    137a:	6702                	ld	a4,0(sp)
    buf[i++] = '0';
    137c:	01511423          	sh	s5,8(sp)
    write(f, s, l);
    1380:	4649                	li	a2,18
            printptr(va_arg(ap, uint64));
    1382:	631c                	ld	a5,0(a4)
    1384:	0721                	addi	a4,a4,8
    1386:	e03a                	sd	a4,0(sp)
    for (j = 0; j < (sizeof(uint64) * 2); j++, x <<= 4)
    1388:	00479293          	slli	t0,a5,0x4
    138c:	00879f93          	slli	t6,a5,0x8
    1390:	00c79f13          	slli	t5,a5,0xc
    1394:	01079e93          	slli	t4,a5,0x10
    1398:	01479e13          	slli	t3,a5,0x14
    139c:	01879313          	slli	t1,a5,0x18
    13a0:	01c79893          	slli	a7,a5,0x1c
    13a4:	02479813          	slli	a6,a5,0x24
    13a8:	02879513          	slli	a0,a5,0x28
    13ac:	02c79593          	slli	a1,a5,0x2c
    13b0:	03079693          	slli	a3,a5,0x30
    13b4:	03479713          	slli	a4,a5,0x34
        buf[i++] = digits[x >> (sizeof(uint64) * 8 - 4)];
    13b8:	03c7d413          	srli	s0,a5,0x3c
    13bc:	01c7d39b          	srliw	t2,a5,0x1c
    13c0:	03c2d293          	srli	t0,t0,0x3c
    13c4:	03cfdf93          	srli	t6,t6,0x3c
    13c8:	03cf5f13          	srli	t5,t5,0x3c
    13cc:	03cede93          	srli	t4,t4,0x3c
    13d0:	03ce5e13          	srli	t3,t3,0x3c
    13d4:	03c35313          	srli	t1,t1,0x3c
    13d8:	03c8d893          	srli	a7,a7,0x3c
    13dc:	03c85813          	srli	a6,a6,0x3c
    13e0:	9171                	srli	a0,a0,0x3c
    13e2:	91f1                	srli	a1,a1,0x3c
    13e4:	92f1                	srli	a3,a3,0x3c
    13e6:	9371                	srli	a4,a4,0x3c
    13e8:	96ce                	add	a3,a3,s3
    13ea:	974e                	add	a4,a4,s3
    13ec:	944e                	add	s0,s0,s3
    13ee:	92ce                	add	t0,t0,s3
    13f0:	9fce                	add	t6,t6,s3
    13f2:	9f4e                	add	t5,t5,s3
    13f4:	9ece                	add	t4,t4,s3
    13f6:	9e4e                	add	t3,t3,s3
    13f8:	934e                	add	t1,t1,s3
    13fa:	98ce                	add	a7,a7,s3
    13fc:	93ce                	add	t2,t2,s3
    13fe:	984e                	add	a6,a6,s3
    1400:	954e                	add	a0,a0,s3
    1402:	95ce                	add	a1,a1,s3
    1404:	0006c083          	lbu	ra,0(a3)
    1408:	0002c283          	lbu	t0,0(t0)
    140c:	00074683          	lbu	a3,0(a4)
    1410:	000fcf83          	lbu	t6,0(t6)
    1414:	000f4f03          	lbu	t5,0(t5)
    1418:	000ece83          	lbu	t4,0(t4)
    141c:	000e4e03          	lbu	t3,0(t3)
    1420:	00034303          	lbu	t1,0(t1)
    1424:	0008c883          	lbu	a7,0(a7)
    1428:	0003c383          	lbu	t2,0(t2)
    142c:	00084803          	lbu	a6,0(a6)
    1430:	00054503          	lbu	a0,0(a0)
    1434:	0005c583          	lbu	a1,0(a1)
    1438:	00044403          	lbu	s0,0(s0)
    for (j = 0; j < (sizeof(uint64) * 2); j++, x <<= 4)
    143c:	03879713          	slli	a4,a5,0x38
        buf[i++] = digits[x >> (sizeof(uint64) * 8 - 4)];
    1440:	9371                	srli	a4,a4,0x3c
    1442:	8bbd                	andi	a5,a5,15
    1444:	974e                	add	a4,a4,s3
    1446:	97ce                	add	a5,a5,s3
    1448:	005105a3          	sb	t0,11(sp)
    144c:	01f10623          	sb	t6,12(sp)
    1450:	01e106a3          	sb	t5,13(sp)
    1454:	01d10723          	sb	t4,14(sp)
    1458:	01c107a3          	sb	t3,15(sp)
    145c:	00610823          	sb	t1,16(sp)
    1460:	011108a3          	sb	a7,17(sp)
    1464:	00710923          	sb	t2,18(sp)
    1468:	010109a3          	sb	a6,19(sp)
    146c:	00a10a23          	sb	a0,20(sp)
    1470:	00b10aa3          	sb	a1,21(sp)
    1474:	00110b23          	sb	ra,22(sp)
    1478:	00d10ba3          	sb	a3,23(sp)
    147c:	00810523          	sb	s0,10(sp)
    1480:	00074703          	lbu	a4,0(a4)
    1484:	0007c783          	lbu	a5,0(a5)
    write(f, s, l);
    1488:	002c                	addi	a1,sp,8
    148a:	4505                	li	a0,1
        buf[i++] = digits[x >> (sizeof(uint64) * 8 - 4)];
    148c:	00e10c23          	sb	a4,24(sp)
    1490:	00f10ca3          	sb	a5,25(sp)
    buf[i] = 0;
    1494:	00010d23          	sb	zero,26(sp)
    write(f, s, l);
    1498:	78a000ef          	jal	ra,1c22 <write>
            // Print unknown % sequence to draw attention.
            putchar('%');
            putchar(s[1]);
            break;
        }
        s += 2;
    149c:	00248513          	addi	a0,s1,2
        if (!*s)
    14a0:	00054783          	lbu	a5,0(a0)
    14a4:	e8079ae3          	bnez	a5,1338 <printf+0x4a>
    }
    va_end(ap);
}
    14a8:	70a6                	ld	ra,104(sp)
    14aa:	7406                	ld	s0,96(sp)
    14ac:	64e6                	ld	s1,88(sp)
    14ae:	6946                	ld	s2,80(sp)
    14b0:	69a6                	ld	s3,72(sp)
    14b2:	6a06                	ld	s4,64(sp)
    14b4:	7ae2                	ld	s5,56(sp)
    14b6:	7b42                	ld	s6,48(sp)
    14b8:	614d                	addi	sp,sp,176
    14ba:	8082                	ret
        for (z = s; s[0] == '%' && s[1] == '%'; z++, s += 2)
    14bc:	00064783          	lbu	a5,0(a2)
    14c0:	84b2                	mv	s1,a2
    14c2:	01278963          	beq	a5,s2,14d4 <printf+0x1e6>
    14c6:	b549                	j	1348 <printf+0x5a>
    14c8:	0024c783          	lbu	a5,2(s1)
    14cc:	0605                	addi	a2,a2,1
    14ce:	0489                	addi	s1,s1,2
    14d0:	e7279ce3          	bne	a5,s2,1348 <printf+0x5a>
    14d4:	0014c783          	lbu	a5,1(s1)
    14d8:	ff2788e3          	beq	a5,s2,14c8 <printf+0x1da>
        l = z - a;
    14dc:	40a6043b          	subw	s0,a2,a0
    write(f, s, l);
    14e0:	85aa                	mv	a1,a0
    14e2:	8622                	mv	a2,s0
    14e4:	4505                	li	a0,1
    14e6:	73c000ef          	jal	ra,1c22 <write>
        if (l)
    14ea:	e60408e3          	beqz	s0,135a <printf+0x6c>
    14ee:	8526                	mv	a0,s1
    14f0:	b581                	j	1330 <printf+0x42>
        switch (s[1])
    14f2:	07800713          	li	a4,120
    14f6:	04e79963          	bne	a5,a4,1548 <printf+0x25a>
            printint(va_arg(ap, int), 16, 1);
    14fa:	6782                	ld	a5,0(sp)
    14fc:	45c1                	li	a1,16
    14fe:	4388                	lw	a0,0(a5)
    1500:	07a1                	addi	a5,a5,8
    1502:	e03e                	sd	a5,0(sp)
    1504:	b71ff0ef          	jal	ra,1074 <printint.constprop.0>
        s += 2;
    1508:	00248513          	addi	a0,s1,2
    150c:	bf51                	j	14a0 <printf+0x1b2>
            printint(va_arg(ap, int), 10, 1);
    150e:	6782                	ld	a5,0(sp)
    1510:	45a9                	li	a1,10
    1512:	4388                	lw	a0,0(a5)
    1514:	07a1                	addi	a5,a5,8
    1516:	e03e                	sd	a5,0(sp)
    1518:	b5dff0ef          	jal	ra,1074 <printint.constprop.0>
        s += 2;
    151c:	00248513          	addi	a0,s1,2
    1520:	b741                	j	14a0 <printf+0x1b2>
            if ((a = va_arg(ap, char *)) == 0)
    1522:	6782                	ld	a5,0(sp)
    1524:	6380                	ld	s0,0(a5)
    1526:	07a1                	addi	a5,a5,8
    1528:	e03e                	sd	a5,0(sp)
    152a:	c031                	beqz	s0,156e <printf+0x280>
            l = strnlen(a, 200);
    152c:	0c800593          	li	a1,200
    1530:	8522                	mv	a0,s0
    1532:	3f8000ef          	jal	ra,192a <strnlen>
    write(f, s, l);
    1536:	0005061b          	sext.w	a2,a0
    153a:	85a2                	mv	a1,s0
    153c:	4505                	li	a0,1
    153e:	6e4000ef          	jal	ra,1c22 <write>
        s += 2;
    1542:	00248513          	addi	a0,s1,2
    1546:	bfa9                	j	14a0 <printf+0x1b2>
    return write(stdout, &byte, 1);
    1548:	4605                	li	a2,1
    154a:	002c                	addi	a1,sp,8
    154c:	4505                	li	a0,1
    char byte = c;
    154e:	01210423          	sb	s2,8(sp)
    return write(stdout, &byte, 1);
    1552:	6d0000ef          	jal	ra,1c22 <write>
    char byte = c;
    1556:	0014c783          	lbu	a5,1(s1)
    return write(stdout, &byte, 1);
    155a:	4605                	li	a2,1
    155c:	002c                	addi	a1,sp,8
    155e:	4505                	li	a0,1
    char byte = c;
    1560:	00f10423          	sb	a5,8(sp)
    return write(stdout, &byte, 1);
    1564:	6be000ef          	jal	ra,1c22 <write>
        s += 2;
    1568:	00248513          	addi	a0,s1,2
    156c:	bf15                	j	14a0 <printf+0x1b2>
                a = "(null)";
    156e:	845a                	mv	s0,s6
    1570:	bf75                	j	152c <printf+0x23e>

0000000000001572 <isspace>:
#define HIGHS (ONES * (UCHAR_MAX / 2 + 1))
#define HASZERO(x) (((x)-ONES) & ~(x)&HIGHS)

int isspace(int c)
{
    return c == ' ' || (unsigned)c - '\t' < 5;
    1572:	02000793          	li	a5,32
    1576:	00f50663          	beq	a0,a5,1582 <isspace+0x10>
    157a:	355d                	addiw	a0,a0,-9
    157c:	00553513          	sltiu	a0,a0,5
    1580:	8082                	ret
    1582:	4505                	li	a0,1
}
    1584:	8082                	ret

0000000000001586 <isdigit>:

int isdigit(int c)
{
    return (unsigned)c - '0' < 10;
    1586:	fd05051b          	addiw	a0,a0,-48
}
    158a:	00a53513          	sltiu	a0,a0,10
    158e:	8082                	ret

0000000000001590 <atoi>:
    return c == ' ' || (unsigned)c - '\t' < 5;
    1590:	02000613          	li	a2,32
    1594:	4591                	li	a1,4

int atoi(const char *s)
{
    int n = 0, neg = 0;
    while (isspace(*s))
    1596:	00054703          	lbu	a4,0(a0)
    return c == ' ' || (unsigned)c - '\t' < 5;
    159a:	ff77069b          	addiw	a3,a4,-9
    159e:	04c70d63          	beq	a4,a2,15f8 <atoi+0x68>
    15a2:	0007079b          	sext.w	a5,a4
    15a6:	04d5f963          	bgeu	a1,a3,15f8 <atoi+0x68>
        s++;
    switch (*s)
    15aa:	02b00693          	li	a3,43
    15ae:	04d70a63          	beq	a4,a3,1602 <atoi+0x72>
    15b2:	02d00693          	li	a3,45
    15b6:	06d70463          	beq	a4,a3,161e <atoi+0x8e>
        neg = 1;
    case '+':
        s++;
    }
    /* Compute n as a negative number to avoid overflow on INT_MIN */
    while (isdigit(*s))
    15ba:	fd07859b          	addiw	a1,a5,-48
    15be:	4625                	li	a2,9
    15c0:	873e                	mv	a4,a5
    15c2:	86aa                	mv	a3,a0
    int n = 0, neg = 0;
    15c4:	4e01                	li	t3,0
    while (isdigit(*s))
    15c6:	04b66a63          	bltu	a2,a1,161a <atoi+0x8a>
    int n = 0, neg = 0;
    15ca:	4501                	li	a0,0
    while (isdigit(*s))
    15cc:	4825                	li	a6,9
    15ce:	0016c603          	lbu	a2,1(a3)
        n = 10 * n - (*s++ - '0');
    15d2:	0025179b          	slliw	a5,a0,0x2
    15d6:	9d3d                	addw	a0,a0,a5
    15d8:	fd07031b          	addiw	t1,a4,-48
    15dc:	0015189b          	slliw	a7,a0,0x1
    while (isdigit(*s))
    15e0:	fd06059b          	addiw	a1,a2,-48
        n = 10 * n - (*s++ - '0');
    15e4:	0685                	addi	a3,a3,1
    15e6:	4068853b          	subw	a0,a7,t1
    while (isdigit(*s))
    15ea:	0006071b          	sext.w	a4,a2
    15ee:	feb870e3          	bgeu	a6,a1,15ce <atoi+0x3e>
    return neg ? n : -n;
    15f2:	000e0563          	beqz	t3,15fc <atoi+0x6c>
}
    15f6:	8082                	ret
        s++;
    15f8:	0505                	addi	a0,a0,1
    15fa:	bf71                	j	1596 <atoi+0x6>
    return neg ? n : -n;
    15fc:	4113053b          	subw	a0,t1,a7
    1600:	8082                	ret
    while (isdigit(*s))
    1602:	00154783          	lbu	a5,1(a0)
    1606:	4625                	li	a2,9
        s++;
    1608:	00150693          	addi	a3,a0,1
    while (isdigit(*s))
    160c:	fd07859b          	addiw	a1,a5,-48
    1610:	0007871b          	sext.w	a4,a5
    int n = 0, neg = 0;
    1614:	4e01                	li	t3,0
    while (isdigit(*s))
    1616:	fab67ae3          	bgeu	a2,a1,15ca <atoi+0x3a>
    161a:	4501                	li	a0,0
}
    161c:	8082                	ret
    while (isdigit(*s))
    161e:	00154783          	lbu	a5,1(a0)
    1622:	4625                	li	a2,9
        s++;
    1624:	00150693          	addi	a3,a0,1
    while (isdigit(*s))
    1628:	fd07859b          	addiw	a1,a5,-48
    162c:	0007871b          	sext.w	a4,a5
    1630:	feb665e3          	bltu	a2,a1,161a <atoi+0x8a>
        neg = 1;
    1634:	4e05                	li	t3,1
    1636:	bf51                	j	15ca <atoi+0x3a>

0000000000001638 <memset>:

void *memset(void *dest, int c, size_t n)
{
    char *p = dest;
    for (int i = 0; i < n; ++i, *(p++) = c)
    1638:	16060d63          	beqz	a2,17b2 <memset+0x17a>
    163c:	40a007b3          	neg	a5,a0
    1640:	8b9d                	andi	a5,a5,7
    1642:	00778713          	addi	a4,a5,7
    1646:	482d                	li	a6,11
    1648:	0ff5f593          	andi	a1,a1,255
    164c:	fff60693          	addi	a3,a2,-1
    1650:	17076263          	bltu	a4,a6,17b4 <memset+0x17c>
    1654:	16e6ea63          	bltu	a3,a4,17c8 <memset+0x190>
    1658:	16078563          	beqz	a5,17c2 <memset+0x18a>
    165c:	00b50023          	sb	a1,0(a0)
    1660:	4705                	li	a4,1
    1662:	00150e93          	addi	t4,a0,1
    1666:	14e78c63          	beq	a5,a4,17be <memset+0x186>
    166a:	00b500a3          	sb	a1,1(a0)
    166e:	4709                	li	a4,2
    1670:	00250e93          	addi	t4,a0,2
    1674:	14e78d63          	beq	a5,a4,17ce <memset+0x196>
    1678:	00b50123          	sb	a1,2(a0)
    167c:	470d                	li	a4,3
    167e:	00350e93          	addi	t4,a0,3
    1682:	12e78b63          	beq	a5,a4,17b8 <memset+0x180>
    1686:	00b501a3          	sb	a1,3(a0)
    168a:	4711                	li	a4,4
    168c:	00450e93          	addi	t4,a0,4
    1690:	14e78163          	beq	a5,a4,17d2 <memset+0x19a>
    1694:	00b50223          	sb	a1,4(a0)
    1698:	4715                	li	a4,5
    169a:	00550e93          	addi	t4,a0,5
    169e:	12e78c63          	beq	a5,a4,17d6 <memset+0x19e>
    16a2:	00b502a3          	sb	a1,5(a0)
    16a6:	471d                	li	a4,7
    16a8:	00650e93          	addi	t4,a0,6
    16ac:	12e79763          	bne	a5,a4,17da <memset+0x1a2>
    16b0:	00750e93          	addi	t4,a0,7
    16b4:	00b50323          	sb	a1,6(a0)
    16b8:	4f1d                	li	t5,7
    16ba:	00859713          	slli	a4,a1,0x8
    16be:	8f4d                	or	a4,a4,a1
    16c0:	01059e13          	slli	t3,a1,0x10
    16c4:	01c76e33          	or	t3,a4,t3
    16c8:	01859313          	slli	t1,a1,0x18
    16cc:	006e6333          	or	t1,t3,t1
    16d0:	02059893          	slli	a7,a1,0x20
    16d4:	011368b3          	or	a7,t1,a7
    16d8:	02859813          	slli	a6,a1,0x28
    16dc:	40f60333          	sub	t1,a2,a5
    16e0:	0108e833          	or	a6,a7,a6
    16e4:	03059693          	slli	a3,a1,0x30
    16e8:	00d866b3          	or	a3,a6,a3
    16ec:	03859713          	slli	a4,a1,0x38
    16f0:	97aa                	add	a5,a5,a0
    16f2:	ff837813          	andi	a6,t1,-8
    16f6:	8f55                	or	a4,a4,a3
    16f8:	00f806b3          	add	a3,a6,a5
    16fc:	e398                	sd	a4,0(a5)
    16fe:	07a1                	addi	a5,a5,8
    1700:	fed79ee3          	bne	a5,a3,16fc <memset+0xc4>
    1704:	ff837693          	andi	a3,t1,-8
    1708:	00de87b3          	add	a5,t4,a3
    170c:	01e6873b          	addw	a4,a3,t5
    1710:	0ad30663          	beq	t1,a3,17bc <memset+0x184>
    1714:	00b78023          	sb	a1,0(a5)
    1718:	0017069b          	addiw	a3,a4,1
    171c:	08c6fb63          	bgeu	a3,a2,17b2 <memset+0x17a>
    1720:	00b780a3          	sb	a1,1(a5)
    1724:	0027069b          	addiw	a3,a4,2
    1728:	08c6f563          	bgeu	a3,a2,17b2 <memset+0x17a>
    172c:	00b78123          	sb	a1,2(a5)
    1730:	0037069b          	addiw	a3,a4,3
    1734:	06c6ff63          	bgeu	a3,a2,17b2 <memset+0x17a>
    1738:	00b781a3          	sb	a1,3(a5)
    173c:	0047069b          	addiw	a3,a4,4
    1740:	06c6f963          	bgeu	a3,a2,17b2 <memset+0x17a>
    1744:	00b78223          	sb	a1,4(a5)
    1748:	0057069b          	addiw	a3,a4,5
    174c:	06c6f363          	bgeu	a3,a2,17b2 <memset+0x17a>
    1750:	00b782a3          	sb	a1,5(a5)
    1754:	0067069b          	addiw	a3,a4,6
    1758:	04c6fd63          	bgeu	a3,a2,17b2 <memset+0x17a>
    175c:	00b78323          	sb	a1,6(a5)
    1760:	0077069b          	addiw	a3,a4,7
    1764:	04c6f763          	bgeu	a3,a2,17b2 <memset+0x17a>
    1768:	00b783a3          	sb	a1,7(a5)
    176c:	0087069b          	addiw	a3,a4,8
    1770:	04c6f163          	bgeu	a3,a2,17b2 <memset+0x17a>
    1774:	00b78423          	sb	a1,8(a5)
    1778:	0097069b          	addiw	a3,a4,9
    177c:	02c6fb63          	bgeu	a3,a2,17b2 <memset+0x17a>
    1780:	00b784a3          	sb	a1,9(a5)
    1784:	00a7069b          	addiw	a3,a4,10
    1788:	02c6f563          	bgeu	a3,a2,17b2 <memset+0x17a>
    178c:	00b78523          	sb	a1,10(a5)
    1790:	00b7069b          	addiw	a3,a4,11
    1794:	00c6ff63          	bgeu	a3,a2,17b2 <memset+0x17a>
    1798:	00b785a3          	sb	a1,11(a5)
    179c:	00c7069b          	addiw	a3,a4,12
    17a0:	00c6f963          	bgeu	a3,a2,17b2 <memset+0x17a>
    17a4:	00b78623          	sb	a1,12(a5)
    17a8:	2735                	addiw	a4,a4,13
    17aa:	00c77463          	bgeu	a4,a2,17b2 <memset+0x17a>
    17ae:	00b786a3          	sb	a1,13(a5)
        ;
    return dest;
}
    17b2:	8082                	ret
    17b4:	472d                	li	a4,11
    17b6:	bd79                	j	1654 <memset+0x1c>
    for (int i = 0; i < n; ++i, *(p++) = c)
    17b8:	4f0d                	li	t5,3
    17ba:	b701                	j	16ba <memset+0x82>
    17bc:	8082                	ret
    17be:	4f05                	li	t5,1
    17c0:	bded                	j	16ba <memset+0x82>
    17c2:	8eaa                	mv	t4,a0
    17c4:	4f01                	li	t5,0
    17c6:	bdd5                	j	16ba <memset+0x82>
    17c8:	87aa                	mv	a5,a0
    17ca:	4701                	li	a4,0
    17cc:	b7a1                	j	1714 <memset+0xdc>
    17ce:	4f09                	li	t5,2
    17d0:	b5ed                	j	16ba <memset+0x82>
    17d2:	4f11                	li	t5,4
    17d4:	b5dd                	j	16ba <memset+0x82>
    17d6:	4f15                	li	t5,5
    17d8:	b5cd                	j	16ba <memset+0x82>
    17da:	4f19                	li	t5,6
    17dc:	bdf9                	j	16ba <memset+0x82>

00000000000017de <strcmp>:

int strcmp(const char *l, const char *r)
{
    for (; *l == *r && *l; l++, r++)
    17de:	00054783          	lbu	a5,0(a0)
    17e2:	0005c703          	lbu	a4,0(a1)
    17e6:	00e79863          	bne	a5,a4,17f6 <strcmp+0x18>
    17ea:	0505                	addi	a0,a0,1
    17ec:	0585                	addi	a1,a1,1
    17ee:	fbe5                	bnez	a5,17de <strcmp>
    17f0:	4501                	li	a0,0
        ;
    return *(unsigned char *)l - *(unsigned char *)r;
}
    17f2:	9d19                	subw	a0,a0,a4
    17f4:	8082                	ret
    return *(unsigned char *)l - *(unsigned char *)r;
    17f6:	0007851b          	sext.w	a0,a5
    17fa:	bfe5                	j	17f2 <strcmp+0x14>

00000000000017fc <strncmp>:

int strncmp(const char *_l, const char *_r, size_t n)
{
    const unsigned char *l = (void *)_l, *r = (void *)_r;
    if (!n--)
    17fc:	ce05                	beqz	a2,1834 <strncmp+0x38>
        return 0;
    for (; *l && *r && n && *l == *r; l++, r++, n--)
    17fe:	00054703          	lbu	a4,0(a0)
    1802:	0005c783          	lbu	a5,0(a1)
    1806:	cb0d                	beqz	a4,1838 <strncmp+0x3c>
    if (!n--)
    1808:	167d                	addi	a2,a2,-1
    180a:	00c506b3          	add	a3,a0,a2
    180e:	a819                	j	1824 <strncmp+0x28>
    for (; *l && *r && n && *l == *r; l++, r++, n--)
    1810:	00a68e63          	beq	a3,a0,182c <strncmp+0x30>
    1814:	0505                	addi	a0,a0,1
    1816:	00e79b63          	bne	a5,a4,182c <strncmp+0x30>
    181a:	00054703          	lbu	a4,0(a0)
        ;
    return *l - *r;
    181e:	0005c783          	lbu	a5,0(a1)
    for (; *l && *r && n && *l == *r; l++, r++, n--)
    1822:	cb19                	beqz	a4,1838 <strncmp+0x3c>
    1824:	0005c783          	lbu	a5,0(a1)
    1828:	0585                	addi	a1,a1,1
    182a:	f3fd                	bnez	a5,1810 <strncmp+0x14>
    return *l - *r;
    182c:	0007051b          	sext.w	a0,a4
    1830:	9d1d                	subw	a0,a0,a5
    1832:	8082                	ret
        return 0;
    1834:	4501                	li	a0,0
}
    1836:	8082                	ret
    1838:	4501                	li	a0,0
    return *l - *r;
    183a:	9d1d                	subw	a0,a0,a5
    183c:	8082                	ret

000000000000183e <strlen>:
size_t strlen(const char *s)
{
    const char *a = s;
    typedef size_t __attribute__((__may_alias__)) word;
    const word *w;
    for (; (uintptr_t)s % SS; s++)
    183e:	00757793          	andi	a5,a0,7
    1842:	cf89                	beqz	a5,185c <strlen+0x1e>
    1844:	87aa                	mv	a5,a0
    1846:	a029                	j	1850 <strlen+0x12>
    1848:	0785                	addi	a5,a5,1
    184a:	0077f713          	andi	a4,a5,7
    184e:	cb01                	beqz	a4,185e <strlen+0x20>
        if (!*s)
    1850:	0007c703          	lbu	a4,0(a5)
    1854:	fb75                	bnez	a4,1848 <strlen+0xa>
    for (w = (const void *)s; !HASZERO(*w); w++)
        ;
    s = (const void *)w;
    for (; *s; s++)
        ;
    return s - a;
    1856:	40a78533          	sub	a0,a5,a0
}
    185a:	8082                	ret
    for (; (uintptr_t)s % SS; s++)
    185c:	87aa                	mv	a5,a0
    for (w = (const void *)s; !HASZERO(*w); w++)
    185e:	6394                	ld	a3,0(a5)
    1860:	00000597          	auipc	a1,0x0
    1864:	6405b583          	ld	a1,1600(a1) # 1ea0 <__clone+0x4a>
    1868:	00000617          	auipc	a2,0x0
    186c:	64063603          	ld	a2,1600(a2) # 1ea8 <__clone+0x52>
    1870:	a019                	j	1876 <strlen+0x38>
    1872:	6794                	ld	a3,8(a5)
    1874:	07a1                	addi	a5,a5,8
    1876:	00b68733          	add	a4,a3,a1
    187a:	fff6c693          	not	a3,a3
    187e:	8f75                	and	a4,a4,a3
    1880:	8f71                	and	a4,a4,a2
    1882:	db65                	beqz	a4,1872 <strlen+0x34>
    for (; *s; s++)
    1884:	0007c703          	lbu	a4,0(a5)
    1888:	d779                	beqz	a4,1856 <strlen+0x18>
    188a:	0017c703          	lbu	a4,1(a5)
    188e:	0785                	addi	a5,a5,1
    1890:	d379                	beqz	a4,1856 <strlen+0x18>
    1892:	0017c703          	lbu	a4,1(a5)
    1896:	0785                	addi	a5,a5,1
    1898:	fb6d                	bnez	a4,188a <strlen+0x4c>
    189a:	bf75                	j	1856 <strlen+0x18>

000000000000189c <memchr>:

void *memchr(const void *src, int c, size_t n)
{
    const unsigned char *s = src;
    c = (unsigned char)c;
    for (; ((uintptr_t)s & ALIGN) && n && *s != c; s++, n--)
    189c:	00757713          	andi	a4,a0,7
{
    18a0:	87aa                	mv	a5,a0
    c = (unsigned char)c;
    18a2:	0ff5f593          	andi	a1,a1,255
    for (; ((uintptr_t)s & ALIGN) && n && *s != c; s++, n--)
    18a6:	cb19                	beqz	a4,18bc <memchr+0x20>
    18a8:	ce25                	beqz	a2,1920 <memchr+0x84>
    18aa:	0007c703          	lbu	a4,0(a5)
    18ae:	04b70e63          	beq	a4,a1,190a <memchr+0x6e>
    18b2:	0785                	addi	a5,a5,1
    18b4:	0077f713          	andi	a4,a5,7
    18b8:	167d                	addi	a2,a2,-1
    18ba:	f77d                	bnez	a4,18a8 <memchr+0xc>
            ;
        s = (const void *)w;
    }
    for (; n && *s != c; s++, n--)
        ;
    return n ? (void *)s : 0;
    18bc:	4501                	li	a0,0
    if (n && *s != c)
    18be:	c235                	beqz	a2,1922 <memchr+0x86>
    18c0:	0007c703          	lbu	a4,0(a5)
    18c4:	04b70363          	beq	a4,a1,190a <memchr+0x6e>
        size_t k = ONES * c;
    18c8:	00000517          	auipc	a0,0x0
    18cc:	5e853503          	ld	a0,1512(a0) # 1eb0 <__clone+0x5a>
        for (w = (const void *)s; n >= SS && !HASZERO(*w ^ k); w++, n -= SS)
    18d0:	471d                	li	a4,7
        size_t k = ONES * c;
    18d2:	02a58533          	mul	a0,a1,a0
        for (w = (const void *)s; n >= SS && !HASZERO(*w ^ k); w++, n -= SS)
    18d6:	02c77a63          	bgeu	a4,a2,190a <memchr+0x6e>
    18da:	00000897          	auipc	a7,0x0
    18de:	5c68b883          	ld	a7,1478(a7) # 1ea0 <__clone+0x4a>
    18e2:	00000817          	auipc	a6,0x0
    18e6:	5c683803          	ld	a6,1478(a6) # 1ea8 <__clone+0x52>
    18ea:	431d                	li	t1,7
    18ec:	a029                	j	18f6 <memchr+0x5a>
    18ee:	1661                	addi	a2,a2,-8
    18f0:	07a1                	addi	a5,a5,8
    18f2:	02c37963          	bgeu	t1,a2,1924 <memchr+0x88>
    18f6:	6398                	ld	a4,0(a5)
    18f8:	8f29                	xor	a4,a4,a0
    18fa:	011706b3          	add	a3,a4,a7
    18fe:	fff74713          	not	a4,a4
    1902:	8f75                	and	a4,a4,a3
    1904:	01077733          	and	a4,a4,a6
    1908:	d37d                	beqz	a4,18ee <memchr+0x52>
    190a:	853e                	mv	a0,a5
    190c:	97b2                	add	a5,a5,a2
    190e:	a021                	j	1916 <memchr+0x7a>
    for (; n && *s != c; s++, n--)
    1910:	0505                	addi	a0,a0,1
    1912:	00f50763          	beq	a0,a5,1920 <memchr+0x84>
    1916:	00054703          	lbu	a4,0(a0)
    191a:	feb71be3          	bne	a4,a1,1910 <memchr+0x74>
    191e:	8082                	ret
    return n ? (void *)s : 0;
    1920:	4501                	li	a0,0
}
    1922:	8082                	ret
    return n ? (void *)s : 0;
    1924:	4501                	li	a0,0
    for (; n && *s != c; s++, n--)
    1926:	f275                	bnez	a2,190a <memchr+0x6e>
}
    1928:	8082                	ret

000000000000192a <strnlen>:

size_t strnlen(const char *s, size_t n)
{
    192a:	1101                	addi	sp,sp,-32
    192c:	e822                	sd	s0,16(sp)
    const char *p = memchr(s, 0, n);
    192e:	862e                	mv	a2,a1
{
    1930:	842e                	mv	s0,a1
    const char *p = memchr(s, 0, n);
    1932:	4581                	li	a1,0
{
    1934:	e426                	sd	s1,8(sp)
    1936:	ec06                	sd	ra,24(sp)
    1938:	84aa                	mv	s1,a0
    const char *p = memchr(s, 0, n);
    193a:	f63ff0ef          	jal	ra,189c <memchr>
    return p ? p - s : n;
    193e:	c519                	beqz	a0,194c <strnlen+0x22>
}
    1940:	60e2                	ld	ra,24(sp)
    1942:	6442                	ld	s0,16(sp)
    return p ? p - s : n;
    1944:	8d05                	sub	a0,a0,s1
}
    1946:	64a2                	ld	s1,8(sp)
    1948:	6105                	addi	sp,sp,32
    194a:	8082                	ret
    194c:	60e2                	ld	ra,24(sp)
    return p ? p - s : n;
    194e:	8522                	mv	a0,s0
}
    1950:	6442                	ld	s0,16(sp)
    1952:	64a2                	ld	s1,8(sp)
    1954:	6105                	addi	sp,sp,32
    1956:	8082                	ret

0000000000001958 <strcpy>:
char *strcpy(char *restrict d, const char *s)
{
    typedef size_t __attribute__((__may_alias__)) word;
    word *wd;
    const word *ws;
    if ((uintptr_t)s % SS == (uintptr_t)d % SS)
    1958:	00b547b3          	xor	a5,a0,a1
    195c:	8b9d                	andi	a5,a5,7
    195e:	eb95                	bnez	a5,1992 <strcpy+0x3a>
    {
        for (; (uintptr_t)s % SS; s++, d++)
    1960:	0075f793          	andi	a5,a1,7
    1964:	e7b1                	bnez	a5,19b0 <strcpy+0x58>
            if (!(*d = *s))
                return d;
        wd = (void *)d;
        ws = (const void *)s;
        for (; !HASZERO(*ws); *wd++ = *ws++)
    1966:	6198                	ld	a4,0(a1)
    1968:	00000617          	auipc	a2,0x0
    196c:	53863603          	ld	a2,1336(a2) # 1ea0 <__clone+0x4a>
    1970:	00000817          	auipc	a6,0x0
    1974:	53883803          	ld	a6,1336(a6) # 1ea8 <__clone+0x52>
    1978:	a029                	j	1982 <strcpy+0x2a>
    197a:	e118                	sd	a4,0(a0)
    197c:	6598                	ld	a4,8(a1)
    197e:	05a1                	addi	a1,a1,8
    1980:	0521                	addi	a0,a0,8
    1982:	00c707b3          	add	a5,a4,a2
    1986:	fff74693          	not	a3,a4
    198a:	8ff5                	and	a5,a5,a3
    198c:	0107f7b3          	and	a5,a5,a6
    1990:	d7ed                	beqz	a5,197a <strcpy+0x22>
            ;
        d = (void *)wd;
        s = (const void *)ws;
    }
    for (; (*d = *s); s++, d++)
    1992:	0005c783          	lbu	a5,0(a1)
    1996:	00f50023          	sb	a5,0(a0)
    199a:	c785                	beqz	a5,19c2 <strcpy+0x6a>
    199c:	0015c783          	lbu	a5,1(a1)
    19a0:	0505                	addi	a0,a0,1
    19a2:	0585                	addi	a1,a1,1
    19a4:	00f50023          	sb	a5,0(a0)
    19a8:	fbf5                	bnez	a5,199c <strcpy+0x44>
        ;
    return d;
}
    19aa:	8082                	ret
        for (; (uintptr_t)s % SS; s++, d++)
    19ac:	0505                	addi	a0,a0,1
    19ae:	df45                	beqz	a4,1966 <strcpy+0xe>
            if (!(*d = *s))
    19b0:	0005c783          	lbu	a5,0(a1)
        for (; (uintptr_t)s % SS; s++, d++)
    19b4:	0585                	addi	a1,a1,1
    19b6:	0075f713          	andi	a4,a1,7
            if (!(*d = *s))
    19ba:	00f50023          	sb	a5,0(a0)
    19be:	f7fd                	bnez	a5,19ac <strcpy+0x54>
}
    19c0:	8082                	ret
    19c2:	8082                	ret

00000000000019c4 <strncpy>:
char *strncpy(char *restrict d, const char *s, size_t n)
{
    typedef size_t __attribute__((__may_alias__)) word;
    word *wd;
    const word *ws;
    if (((uintptr_t)s & ALIGN) == ((uintptr_t)d & ALIGN))
    19c4:	00b547b3          	xor	a5,a0,a1
    19c8:	8b9d                	andi	a5,a5,7
    19ca:	1a079863          	bnez	a5,1b7a <strncpy+0x1b6>
    {
        for (; ((uintptr_t)s & ALIGN) && n && (*d = *s); n--, s++, d++)
    19ce:	0075f793          	andi	a5,a1,7
    19d2:	16078463          	beqz	a5,1b3a <strncpy+0x176>
    19d6:	ea01                	bnez	a2,19e6 <strncpy+0x22>
    19d8:	a421                	j	1be0 <strncpy+0x21c>
    19da:	167d                	addi	a2,a2,-1
    19dc:	0505                	addi	a0,a0,1
    19de:	14070e63          	beqz	a4,1b3a <strncpy+0x176>
    19e2:	1a060863          	beqz	a2,1b92 <strncpy+0x1ce>
    19e6:	0005c783          	lbu	a5,0(a1)
    19ea:	0585                	addi	a1,a1,1
    19ec:	0075f713          	andi	a4,a1,7
    19f0:	00f50023          	sb	a5,0(a0)
    19f4:	f3fd                	bnez	a5,19da <strncpy+0x16>
    19f6:	4805                	li	a6,1
    19f8:	1a061863          	bnez	a2,1ba8 <strncpy+0x1e4>
    19fc:	40a007b3          	neg	a5,a0
    1a00:	8b9d                	andi	a5,a5,7
    1a02:	4681                	li	a3,0
    1a04:	18061a63          	bnez	a2,1b98 <strncpy+0x1d4>
    1a08:	00778713          	addi	a4,a5,7
    1a0c:	45ad                	li	a1,11
    1a0e:	18b76363          	bltu	a4,a1,1b94 <strncpy+0x1d0>
    1a12:	1ae6eb63          	bltu	a3,a4,1bc8 <strncpy+0x204>
    1a16:	1a078363          	beqz	a5,1bbc <strncpy+0x1f8>
    for (int i = 0; i < n; ++i, *(p++) = c)
    1a1a:	00050023          	sb	zero,0(a0)
    1a1e:	4685                	li	a3,1
    1a20:	00150713          	addi	a4,a0,1
    1a24:	18d78f63          	beq	a5,a3,1bc2 <strncpy+0x1fe>
    1a28:	000500a3          	sb	zero,1(a0)
    1a2c:	4689                	li	a3,2
    1a2e:	00250713          	addi	a4,a0,2
    1a32:	18d78e63          	beq	a5,a3,1bce <strncpy+0x20a>
    1a36:	00050123          	sb	zero,2(a0)
    1a3a:	468d                	li	a3,3
    1a3c:	00350713          	addi	a4,a0,3
    1a40:	16d78c63          	beq	a5,a3,1bb8 <strncpy+0x1f4>
    1a44:	000501a3          	sb	zero,3(a0)
    1a48:	4691                	li	a3,4
    1a4a:	00450713          	addi	a4,a0,4
    1a4e:	18d78263          	beq	a5,a3,1bd2 <strncpy+0x20e>
    1a52:	00050223          	sb	zero,4(a0)
    1a56:	4695                	li	a3,5
    1a58:	00550713          	addi	a4,a0,5
    1a5c:	16d78d63          	beq	a5,a3,1bd6 <strncpy+0x212>
    1a60:	000502a3          	sb	zero,5(a0)
    1a64:	469d                	li	a3,7
    1a66:	00650713          	addi	a4,a0,6
    1a6a:	16d79863          	bne	a5,a3,1bda <strncpy+0x216>
    1a6e:	00750713          	addi	a4,a0,7
    1a72:	00050323          	sb	zero,6(a0)
    1a76:	40f80833          	sub	a6,a6,a5
    1a7a:	ff887593          	andi	a1,a6,-8
    1a7e:	97aa                	add	a5,a5,a0
    1a80:	95be                	add	a1,a1,a5
    1a82:	0007b023          	sd	zero,0(a5)
    1a86:	07a1                	addi	a5,a5,8
    1a88:	feb79de3          	bne	a5,a1,1a82 <strncpy+0xbe>
    1a8c:	ff887593          	andi	a1,a6,-8
    1a90:	9ead                	addw	a3,a3,a1
    1a92:	00b707b3          	add	a5,a4,a1
    1a96:	12b80863          	beq	a6,a1,1bc6 <strncpy+0x202>
    1a9a:	00078023          	sb	zero,0(a5)
    1a9e:	0016871b          	addiw	a4,a3,1
    1aa2:	0ec77863          	bgeu	a4,a2,1b92 <strncpy+0x1ce>
    1aa6:	000780a3          	sb	zero,1(a5)
    1aaa:	0026871b          	addiw	a4,a3,2
    1aae:	0ec77263          	bgeu	a4,a2,1b92 <strncpy+0x1ce>
    1ab2:	00078123          	sb	zero,2(a5)
    1ab6:	0036871b          	addiw	a4,a3,3
    1aba:	0cc77c63          	bgeu	a4,a2,1b92 <strncpy+0x1ce>
    1abe:	000781a3          	sb	zero,3(a5)
    1ac2:	0046871b          	addiw	a4,a3,4
    1ac6:	0cc77663          	bgeu	a4,a2,1b92 <strncpy+0x1ce>
    1aca:	00078223          	sb	zero,4(a5)
    1ace:	0056871b          	addiw	a4,a3,5
    1ad2:	0cc77063          	bgeu	a4,a2,1b92 <strncpy+0x1ce>
    1ad6:	000782a3          	sb	zero,5(a5)
    1ada:	0066871b          	addiw	a4,a3,6
    1ade:	0ac77a63          	bgeu	a4,a2,1b92 <strncpy+0x1ce>
    1ae2:	00078323          	sb	zero,6(a5)
    1ae6:	0076871b          	addiw	a4,a3,7
    1aea:	0ac77463          	bgeu	a4,a2,1b92 <strncpy+0x1ce>
    1aee:	000783a3          	sb	zero,7(a5)
    1af2:	0086871b          	addiw	a4,a3,8
    1af6:	08c77e63          	bgeu	a4,a2,1b92 <strncpy+0x1ce>
    1afa:	00078423          	sb	zero,8(a5)
    1afe:	0096871b          	addiw	a4,a3,9
    1b02:	08c77863          	bgeu	a4,a2,1b92 <strncpy+0x1ce>
    1b06:	000784a3          	sb	zero,9(a5)
    1b0a:	00a6871b          	addiw	a4,a3,10
    1b0e:	08c77263          	bgeu	a4,a2,1b92 <strncpy+0x1ce>
    1b12:	00078523          	sb	zero,10(a5)
    1b16:	00b6871b          	addiw	a4,a3,11
    1b1a:	06c77c63          	bgeu	a4,a2,1b92 <strncpy+0x1ce>
    1b1e:	000785a3          	sb	zero,11(a5)
    1b22:	00c6871b          	addiw	a4,a3,12
    1b26:	06c77663          	bgeu	a4,a2,1b92 <strncpy+0x1ce>
    1b2a:	00078623          	sb	zero,12(a5)
    1b2e:	26b5                	addiw	a3,a3,13
    1b30:	06c6f163          	bgeu	a3,a2,1b92 <strncpy+0x1ce>
    1b34:	000786a3          	sb	zero,13(a5)
    1b38:	8082                	ret
            ;
        if (!n || !*s)
    1b3a:	c645                	beqz	a2,1be2 <strncpy+0x21e>
    1b3c:	0005c783          	lbu	a5,0(a1)
    1b40:	ea078be3          	beqz	a5,19f6 <strncpy+0x32>
            goto tail;
        wd = (void *)d;
        ws = (const void *)s;
        for (; n >= sizeof(size_t) && !HASZERO(*ws); n -= sizeof(size_t), ws++, wd++)
    1b44:	479d                	li	a5,7
    1b46:	02c7ff63          	bgeu	a5,a2,1b84 <strncpy+0x1c0>
    1b4a:	00000897          	auipc	a7,0x0
    1b4e:	3568b883          	ld	a7,854(a7) # 1ea0 <__clone+0x4a>
    1b52:	00000817          	auipc	a6,0x0
    1b56:	35683803          	ld	a6,854(a6) # 1ea8 <__clone+0x52>
    1b5a:	431d                	li	t1,7
    1b5c:	6198                	ld	a4,0(a1)
    1b5e:	011707b3          	add	a5,a4,a7
    1b62:	fff74693          	not	a3,a4
    1b66:	8ff5                	and	a5,a5,a3
    1b68:	0107f7b3          	and	a5,a5,a6
    1b6c:	ef81                	bnez	a5,1b84 <strncpy+0x1c0>
            *wd = *ws;
    1b6e:	e118                	sd	a4,0(a0)
        for (; n >= sizeof(size_t) && !HASZERO(*ws); n -= sizeof(size_t), ws++, wd++)
    1b70:	1661                	addi	a2,a2,-8
    1b72:	05a1                	addi	a1,a1,8
    1b74:	0521                	addi	a0,a0,8
    1b76:	fec363e3          	bltu	t1,a2,1b5c <strncpy+0x198>
        d = (void *)wd;
        s = (const void *)ws;
    }
    for (; n && (*d = *s); n--, s++, d++)
    1b7a:	e609                	bnez	a2,1b84 <strncpy+0x1c0>
    1b7c:	a08d                	j	1bde <strncpy+0x21a>
    1b7e:	167d                	addi	a2,a2,-1
    1b80:	0505                	addi	a0,a0,1
    1b82:	ca01                	beqz	a2,1b92 <strncpy+0x1ce>
    1b84:	0005c783          	lbu	a5,0(a1)
    1b88:	0585                	addi	a1,a1,1
    1b8a:	00f50023          	sb	a5,0(a0)
    1b8e:	fbe5                	bnez	a5,1b7e <strncpy+0x1ba>
        ;
tail:
    1b90:	b59d                	j	19f6 <strncpy+0x32>
    memset(d, 0, n);
    return d;
}
    1b92:	8082                	ret
    1b94:	472d                	li	a4,11
    1b96:	bdb5                	j	1a12 <strncpy+0x4e>
    1b98:	00778713          	addi	a4,a5,7
    1b9c:	45ad                	li	a1,11
    1b9e:	fff60693          	addi	a3,a2,-1
    1ba2:	e6b778e3          	bgeu	a4,a1,1a12 <strncpy+0x4e>
    1ba6:	b7fd                	j	1b94 <strncpy+0x1d0>
    1ba8:	40a007b3          	neg	a5,a0
    1bac:	8832                	mv	a6,a2
    1bae:	8b9d                	andi	a5,a5,7
    1bb0:	4681                	li	a3,0
    1bb2:	e4060be3          	beqz	a2,1a08 <strncpy+0x44>
    1bb6:	b7cd                	j	1b98 <strncpy+0x1d4>
    for (int i = 0; i < n; ++i, *(p++) = c)
    1bb8:	468d                	li	a3,3
    1bba:	bd75                	j	1a76 <strncpy+0xb2>
    1bbc:	872a                	mv	a4,a0
    1bbe:	4681                	li	a3,0
    1bc0:	bd5d                	j	1a76 <strncpy+0xb2>
    1bc2:	4685                	li	a3,1
    1bc4:	bd4d                	j	1a76 <strncpy+0xb2>
    1bc6:	8082                	ret
    1bc8:	87aa                	mv	a5,a0
    1bca:	4681                	li	a3,0
    1bcc:	b5f9                	j	1a9a <strncpy+0xd6>
    1bce:	4689                	li	a3,2
    1bd0:	b55d                	j	1a76 <strncpy+0xb2>
    1bd2:	4691                	li	a3,4
    1bd4:	b54d                	j	1a76 <strncpy+0xb2>
    1bd6:	4695                	li	a3,5
    1bd8:	bd79                	j	1a76 <strncpy+0xb2>
    1bda:	4699                	li	a3,6
    1bdc:	bd69                	j	1a76 <strncpy+0xb2>
    1bde:	8082                	ret
    1be0:	8082                	ret
    1be2:	8082                	ret

0000000000001be4 <open>:
#include <unistd.h>

#include "syscall.h"

int open(const char *path, int flags)
{
    1be4:	87aa                	mv	a5,a0
    1be6:	862e                	mv	a2,a1
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
}

static inline long __syscall4(long n, long a, long b, long c, long d)
{
    register long a7 __asm__("a7") = n;
    1be8:	03800893          	li	a7,56
    register long a0 __asm__("a0") = a;
    1bec:	f9c00513          	li	a0,-100
    register long a1 __asm__("a1") = b;
    1bf0:	85be                	mv	a1,a5
    register long a2 __asm__("a2") = c;
    register long a3 __asm__("a3") = d;
    1bf2:	4689                	li	a3,2
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3))
    1bf4:	00000073          	ecall
    return syscall(SYS_openat, AT_FDCWD, path, flags, O_RDWR);
}
    1bf8:	2501                	sext.w	a0,a0
    1bfa:	8082                	ret

0000000000001bfc <openat>:
    register long a7 __asm__("a7") = n;
    1bfc:	03800893          	li	a7,56
    register long a3 __asm__("a3") = d;
    1c00:	18000693          	li	a3,384
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3))
    1c04:	00000073          	ecall

int openat(int dirfd,const char *path, int flags)
{
    return syscall(SYS_openat, dirfd, path, flags, 0600);
}
    1c08:	2501                	sext.w	a0,a0
    1c0a:	8082                	ret

0000000000001c0c <close>:
    register long a7 __asm__("a7") = n;
    1c0c:	03900893          	li	a7,57
    __asm_syscall("r"(a7), "0"(a0))
    1c10:	00000073          	ecall

int close(int fd)
{
    return syscall(SYS_close, fd);
}
    1c14:	2501                	sext.w	a0,a0
    1c16:	8082                	ret

0000000000001c18 <read>:
    register long a7 __asm__("a7") = n;
    1c18:	03f00893          	li	a7,63
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1c1c:	00000073          	ecall

ssize_t read(int fd, void *buf, size_t len)
{
    return syscall(SYS_read, fd, buf, len);
}
    1c20:	8082                	ret

0000000000001c22 <write>:
    register long a7 __asm__("a7") = n;
    1c22:	04000893          	li	a7,64
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1c26:	00000073          	ecall

ssize_t write(int fd, const void *buf, size_t len)
{
    return syscall(SYS_write, fd, buf, len);
}
    1c2a:	8082                	ret

0000000000001c2c <getpid>:
    register long a7 __asm__("a7") = n;
    1c2c:	0ac00893          	li	a7,172
    __asm_syscall("r"(a7))
    1c30:	00000073          	ecall

pid_t getpid(void)
{
    return syscall(SYS_getpid);
}
    1c34:	2501                	sext.w	a0,a0
    1c36:	8082                	ret

0000000000001c38 <getppid>:
    register long a7 __asm__("a7") = n;
    1c38:	0ad00893          	li	a7,173
    __asm_syscall("r"(a7))
    1c3c:	00000073          	ecall

pid_t getppid(void)
{
    return syscall(SYS_getppid);
}
    1c40:	2501                	sext.w	a0,a0
    1c42:	8082                	ret

0000000000001c44 <sched_yield>:
    register long a7 __asm__("a7") = n;
    1c44:	07c00893          	li	a7,124
    __asm_syscall("r"(a7))
    1c48:	00000073          	ecall

int sched_yield(void)
{
    return syscall(SYS_sched_yield);
}
    1c4c:	2501                	sext.w	a0,a0
    1c4e:	8082                	ret

0000000000001c50 <fork>:
    register long a7 __asm__("a7") = n;
    1c50:	0dc00893          	li	a7,220
    register long a0 __asm__("a0") = a;
    1c54:	4545                	li	a0,17
    register long a1 __asm__("a1") = b;
    1c56:	4581                	li	a1,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1c58:	00000073          	ecall

pid_t fork(void)
{
    return syscall(SYS_clone, SIGCHLD, 0);
}
    1c5c:	2501                	sext.w	a0,a0
    1c5e:	8082                	ret

0000000000001c60 <clone>:

pid_t clone(int (*fn)(void *arg), void *arg, void *stack, size_t stack_size, unsigned long flags)
{
    1c60:	85b2                	mv	a1,a2
    1c62:	863a                	mv	a2,a4
    if (stack)
    1c64:	c191                	beqz	a1,1c68 <clone+0x8>
	stack += stack_size;
    1c66:	95b6                	add	a1,a1,a3

    return __clone(fn, stack, flags, NULL, NULL, NULL);
    1c68:	4781                	li	a5,0
    1c6a:	4701                	li	a4,0
    1c6c:	4681                	li	a3,0
    1c6e:	2601                	sext.w	a2,a2
    1c70:	a2dd                	j	1e56 <__clone>

0000000000001c72 <exit>:
    register long a7 __asm__("a7") = n;
    1c72:	05d00893          	li	a7,93
    __asm_syscall("r"(a7), "0"(a0))
    1c76:	00000073          	ecall
    //return syscall(SYS_clone, fn, stack, flags, NULL, NULL, NULL);
}
void exit(int code)
{
    syscall(SYS_exit, code);
}
    1c7a:	8082                	ret

0000000000001c7c <waitpid>:
    register long a7 __asm__("a7") = n;
    1c7c:	10400893          	li	a7,260
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1c80:	00000073          	ecall

int waitpid(int pid, int *code, int options)
{
    return syscall(SYS_wait4, pid, code, options);
}
    1c84:	2501                	sext.w	a0,a0
    1c86:	8082                	ret

0000000000001c88 <exec>:
    register long a7 __asm__("a7") = n;
    1c88:	0dd00893          	li	a7,221
    __asm_syscall("r"(a7), "0"(a0))
    1c8c:	00000073          	ecall

int exec(char *name)
{
    return syscall(SYS_execve, name);
}
    1c90:	2501                	sext.w	a0,a0
    1c92:	8082                	ret

0000000000001c94 <execve>:
    register long a7 __asm__("a7") = n;
    1c94:	0dd00893          	li	a7,221
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1c98:	00000073          	ecall

int execve(const char *name, char *const argv[], char *const argp[])
{
    return syscall(SYS_execve, name, argv, argp);
}
    1c9c:	2501                	sext.w	a0,a0
    1c9e:	8082                	ret

0000000000001ca0 <times>:
    register long a7 __asm__("a7") = n;
    1ca0:	09900893          	li	a7,153
    __asm_syscall("r"(a7), "0"(a0))
    1ca4:	00000073          	ecall

int times(void *mytimes)
{
	return syscall(SYS_times, mytimes);
}
    1ca8:	2501                	sext.w	a0,a0
    1caa:	8082                	ret

0000000000001cac <get_time>:

int64 get_time()
{
    1cac:	1141                	addi	sp,sp,-16
    register long a7 __asm__("a7") = n;
    1cae:	0a900893          	li	a7,169
    register long a0 __asm__("a0") = a;
    1cb2:	850a                	mv	a0,sp
    register long a1 __asm__("a1") = b;
    1cb4:	4581                	li	a1,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1cb6:	00000073          	ecall
    TimeVal time;
    int err = sys_get_time(&time, 0);
    if (err == 0)
    1cba:	2501                	sext.w	a0,a0
    1cbc:	ed09                	bnez	a0,1cd6 <get_time+0x2a>
    {
        return ((time.sec & 0xffff) * 1000 + time.usec / 1000);
    1cbe:	67a2                	ld	a5,8(sp)
    1cc0:	3e800713          	li	a4,1000
    1cc4:	00015503          	lhu	a0,0(sp)
    1cc8:	02e7d7b3          	divu	a5,a5,a4
    1ccc:	02e50533          	mul	a0,a0,a4
    1cd0:	953e                	add	a0,a0,a5
    }
    else
    {
        return -1;
    }
}
    1cd2:	0141                	addi	sp,sp,16
    1cd4:	8082                	ret
        return -1;
    1cd6:	557d                	li	a0,-1
    1cd8:	bfed                	j	1cd2 <get_time+0x26>

0000000000001cda <sys_get_time>:
    register long a7 __asm__("a7") = n;
    1cda:	0a900893          	li	a7,169
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1cde:	00000073          	ecall

int sys_get_time(TimeVal *ts, int tz)
{
    return syscall(SYS_gettimeofday, ts, tz);
}
    1ce2:	2501                	sext.w	a0,a0
    1ce4:	8082                	ret

0000000000001ce6 <time>:
    register long a7 __asm__("a7") = n;
    1ce6:	42600893          	li	a7,1062
    __asm_syscall("r"(a7), "0"(a0))
    1cea:	00000073          	ecall

int time(unsigned long *tloc)
{
    return syscall(SYS_time, tloc);
}
    1cee:	2501                	sext.w	a0,a0
    1cf0:	8082                	ret

0000000000001cf2 <sleep>:

int sleep(unsigned long long time)
{
    1cf2:	1141                	addi	sp,sp,-16
    TimeVal tv = {.sec = time, .usec = 0};
    1cf4:	e02a                	sd	a0,0(sp)
    register long a0 __asm__("a0") = a;
    1cf6:	850a                	mv	a0,sp
    1cf8:	e402                	sd	zero,8(sp)
    register long a7 __asm__("a7") = n;
    1cfa:	06500893          	li	a7,101
    register long a1 __asm__("a1") = b;
    1cfe:	85aa                	mv	a1,a0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1d00:	00000073          	ecall
    if (syscall(SYS_nanosleep, &tv, &tv)) return tv.sec;
    1d04:	e501                	bnez	a0,1d0c <sleep+0x1a>
    return 0;
    1d06:	4501                	li	a0,0
}
    1d08:	0141                	addi	sp,sp,16
    1d0a:	8082                	ret
    if (syscall(SYS_nanosleep, &tv, &tv)) return tv.sec;
    1d0c:	4502                	lw	a0,0(sp)
}
    1d0e:	0141                	addi	sp,sp,16
    1d10:	8082                	ret

0000000000001d12 <set_priority>:
    register long a7 __asm__("a7") = n;
    1d12:	08c00893          	li	a7,140
    __asm_syscall("r"(a7), "0"(a0))
    1d16:	00000073          	ecall

int set_priority(int prio)
{
    return syscall(SYS_setpriority, prio);
}
    1d1a:	2501                	sext.w	a0,a0
    1d1c:	8082                	ret

0000000000001d1e <mmap>:
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4))
}

static inline long __syscall6(long n, long a, long b, long c, long d, long e, long f)
{
    register long a7 __asm__("a7") = n;
    1d1e:	0de00893          	li	a7,222
    register long a1 __asm__("a1") = b;
    register long a2 __asm__("a2") = c;
    register long a3 __asm__("a3") = d;
    register long a4 __asm__("a4") = e;
    register long a5 __asm__("a5") = f;
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4), "r"(a5))
    1d22:	00000073          	ecall

void *mmap(void *start, size_t len, int prot, int flags, int fd, off_t off)
{
    return syscall(SYS_mmap, start, len, prot, flags, fd, off);
}
    1d26:	8082                	ret

0000000000001d28 <munmap>:
    register long a7 __asm__("a7") = n;
    1d28:	0d700893          	li	a7,215
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1d2c:	00000073          	ecall

int munmap(void *start, size_t len)
{
    return syscall(SYS_munmap, start, len);
}
    1d30:	2501                	sext.w	a0,a0
    1d32:	8082                	ret

0000000000001d34 <wait>:

int wait(int *code)
{
    1d34:	85aa                	mv	a1,a0
    register long a7 __asm__("a7") = n;
    1d36:	10400893          	li	a7,260
    register long a0 __asm__("a0") = a;
    1d3a:	557d                	li	a0,-1
    register long a2 __asm__("a2") = c;
    1d3c:	4601                	li	a2,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1d3e:	00000073          	ecall
    return waitpid(-1, code, 0);
}
    1d42:	2501                	sext.w	a0,a0
    1d44:	8082                	ret

0000000000001d46 <spawn>:
    register long a7 __asm__("a7") = n;
    1d46:	19000893          	li	a7,400
    __asm_syscall("r"(a7), "0"(a0))
    1d4a:	00000073          	ecall

int spawn(char *file)
{
    return syscall(SYS_spawn, file);
}
    1d4e:	2501                	sext.w	a0,a0
    1d50:	8082                	ret

0000000000001d52 <mailread>:
    register long a7 __asm__("a7") = n;
    1d52:	19100893          	li	a7,401
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1d56:	00000073          	ecall

int mailread(void *buf, int len)
{
    return syscall(SYS_mailread, buf, len);
}
    1d5a:	2501                	sext.w	a0,a0
    1d5c:	8082                	ret

0000000000001d5e <mailwrite>:
    register long a7 __asm__("a7") = n;
    1d5e:	19200893          	li	a7,402
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1d62:	00000073          	ecall

int mailwrite(int pid, void *buf, int len)
{
    return syscall(SYS_mailwrite, pid, buf, len);
}
    1d66:	2501                	sext.w	a0,a0
    1d68:	8082                	ret

0000000000001d6a <fstat>:
    register long a7 __asm__("a7") = n;
    1d6a:	05000893          	li	a7,80
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1d6e:	00000073          	ecall

int fstat(int fd, struct kstat *st)
{
    return syscall(SYS_fstat, fd, st);
}
    1d72:	2501                	sext.w	a0,a0
    1d74:	8082                	ret

0000000000001d76 <sys_linkat>:
    register long a4 __asm__("a4") = e;
    1d76:	1702                	slli	a4,a4,0x20
    register long a7 __asm__("a7") = n;
    1d78:	02500893          	li	a7,37
    register long a4 __asm__("a4") = e;
    1d7c:	9301                	srli	a4,a4,0x20
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4))
    1d7e:	00000073          	ecall

int sys_linkat(int olddirfd, char *oldpath, int newdirfd, char *newpath, unsigned int flags)
{
    return syscall(SYS_linkat, olddirfd, oldpath, newdirfd, newpath, flags);
}
    1d82:	2501                	sext.w	a0,a0
    1d84:	8082                	ret

0000000000001d86 <sys_unlinkat>:
    register long a2 __asm__("a2") = c;
    1d86:	1602                	slli	a2,a2,0x20
    register long a7 __asm__("a7") = n;
    1d88:	02300893          	li	a7,35
    register long a2 __asm__("a2") = c;
    1d8c:	9201                	srli	a2,a2,0x20
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1d8e:	00000073          	ecall

int sys_unlinkat(int dirfd, char *path, unsigned int flags)
{
    return syscall(SYS_unlinkat, dirfd, path, flags);
}
    1d92:	2501                	sext.w	a0,a0
    1d94:	8082                	ret

0000000000001d96 <link>:

int link(char *old_path, char *new_path)
{
    1d96:	87aa                	mv	a5,a0
    1d98:	86ae                	mv	a3,a1
    register long a7 __asm__("a7") = n;
    1d9a:	02500893          	li	a7,37
    register long a0 __asm__("a0") = a;
    1d9e:	f9c00513          	li	a0,-100
    register long a1 __asm__("a1") = b;
    1da2:	85be                	mv	a1,a5
    register long a2 __asm__("a2") = c;
    1da4:	f9c00613          	li	a2,-100
    register long a4 __asm__("a4") = e;
    1da8:	4701                	li	a4,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4))
    1daa:	00000073          	ecall
    return sys_linkat(AT_FDCWD, old_path, AT_FDCWD, new_path, 0);
}
    1dae:	2501                	sext.w	a0,a0
    1db0:	8082                	ret

0000000000001db2 <unlink>:

int unlink(char *path)
{
    1db2:	85aa                	mv	a1,a0
    register long a7 __asm__("a7") = n;
    1db4:	02300893          	li	a7,35
    register long a0 __asm__("a0") = a;
    1db8:	f9c00513          	li	a0,-100
    register long a2 __asm__("a2") = c;
    1dbc:	4601                	li	a2,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1dbe:	00000073          	ecall
    return sys_unlinkat(AT_FDCWD, path, 0);
}
    1dc2:	2501                	sext.w	a0,a0
    1dc4:	8082                	ret

0000000000001dc6 <uname>:
    register long a7 __asm__("a7") = n;
    1dc6:	0a000893          	li	a7,160
    __asm_syscall("r"(a7), "0"(a0))
    1dca:	00000073          	ecall

int uname(void *buf)
{
    return syscall(SYS_uname, buf);
}
    1dce:	2501                	sext.w	a0,a0
    1dd0:	8082                	ret

0000000000001dd2 <brk>:
    register long a7 __asm__("a7") = n;
    1dd2:	0d600893          	li	a7,214
    __asm_syscall("r"(a7), "0"(a0))
    1dd6:	00000073          	ecall

int brk(void *addr)
{
    return syscall(SYS_brk, addr);
}
    1dda:	2501                	sext.w	a0,a0
    1ddc:	8082                	ret

0000000000001dde <getcwd>:
    register long a7 __asm__("a7") = n;
    1dde:	48c5                	li	a7,17
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1de0:	00000073          	ecall

char *getcwd(char *buf, size_t size){
    return syscall(SYS_getcwd, buf, size);
}
    1de4:	8082                	ret

0000000000001de6 <chdir>:
    register long a7 __asm__("a7") = n;
    1de6:	03100893          	li	a7,49
    __asm_syscall("r"(a7), "0"(a0))
    1dea:	00000073          	ecall

int chdir(const char *path){
    return syscall(SYS_chdir, path);
}
    1dee:	2501                	sext.w	a0,a0
    1df0:	8082                	ret

0000000000001df2 <mkdir>:

int mkdir(const char *path, mode_t mode){
    1df2:	862e                	mv	a2,a1
    1df4:	87aa                	mv	a5,a0
    register long a2 __asm__("a2") = c;
    1df6:	1602                	slli	a2,a2,0x20
    register long a7 __asm__("a7") = n;
    1df8:	02200893          	li	a7,34
    register long a0 __asm__("a0") = a;
    1dfc:	f9c00513          	li	a0,-100
    register long a1 __asm__("a1") = b;
    1e00:	85be                	mv	a1,a5
    register long a2 __asm__("a2") = c;
    1e02:	9201                	srli	a2,a2,0x20
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1e04:	00000073          	ecall
    return syscall(SYS_mkdirat, AT_FDCWD, path, mode);
}
    1e08:	2501                	sext.w	a0,a0
    1e0a:	8082                	ret

0000000000001e0c <getdents>:
    register long a7 __asm__("a7") = n;
    1e0c:	03d00893          	li	a7,61
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1e10:	00000073          	ecall

int getdents(int fd, struct linux_dirent64 *dirp64, unsigned long len){
    //return syscall(SYS_getdents64, fd, dirp64, len);
    return syscall(SYS_getdents64, fd, dirp64, len);
}
    1e14:	2501                	sext.w	a0,a0
    1e16:	8082                	ret

0000000000001e18 <pipe>:
    register long a7 __asm__("a7") = n;
    1e18:	03b00893          	li	a7,59
    register long a1 __asm__("a1") = b;
    1e1c:	4581                	li	a1,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1e1e:	00000073          	ecall

int pipe(int fd[2]){
    return syscall(SYS_pipe2, fd, 0);
}
    1e22:	2501                	sext.w	a0,a0
    1e24:	8082                	ret

0000000000001e26 <dup>:
    register long a7 __asm__("a7") = n;
    1e26:	48dd                	li	a7,23
    __asm_syscall("r"(a7), "0"(a0))
    1e28:	00000073          	ecall

int dup(int fd){
    return syscall(SYS_dup, fd);
}
    1e2c:	2501                	sext.w	a0,a0
    1e2e:	8082                	ret

0000000000001e30 <dup2>:
    register long a7 __asm__("a7") = n;
    1e30:	48e1                	li	a7,24
    register long a2 __asm__("a2") = c;
    1e32:	4601                	li	a2,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1e34:	00000073          	ecall

int dup2(int old, int new){
    return syscall(SYS_dup3, old, new, 0);
}
    1e38:	2501                	sext.w	a0,a0
    1e3a:	8082                	ret

0000000000001e3c <mount>:
    register long a7 __asm__("a7") = n;
    1e3c:	02800893          	li	a7,40
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4))
    1e40:	00000073          	ecall

int mount(const char *special, const char *dir, const char *fstype, unsigned long flags, const void *data)
{
        return syscall(SYS_mount, special, dir, fstype, flags, data);
}
    1e44:	2501                	sext.w	a0,a0
    1e46:	8082                	ret

0000000000001e48 <umount>:
    register long a7 __asm__("a7") = n;
    1e48:	02700893          	li	a7,39
    register long a1 __asm__("a1") = b;
    1e4c:	4581                	li	a1,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1e4e:	00000073          	ecall

int umount(const char *special)
{
        return syscall(SYS_umount2, special, 0);
}
    1e52:	2501                	sext.w	a0,a0
    1e54:	8082                	ret

0000000000001e56 <__clone>:

.global __clone
.type  __clone, %function
__clone:
	# Save func and arg to stack
	addi a1, a1, -16
    1e56:	15c1                	addi	a1,a1,-16
	sd a0, 0(a1)
    1e58:	e188                	sd	a0,0(a1)
	sd a3, 8(a1)
    1e5a:	e594                	sd	a3,8(a1)

	# Call SYS_clone
	mv a0, a2
    1e5c:	8532                	mv	a0,a2
	mv a2, a4
    1e5e:	863a                	mv	a2,a4
	mv a3, a5
    1e60:	86be                	mv	a3,a5
	mv a4, a6
    1e62:	8742                	mv	a4,a6
	li a7, 220 # SYS_clone
    1e64:	0dc00893          	li	a7,220
	ecall
    1e68:	00000073          	ecall

	beqz a0, 1f
    1e6c:	c111                	beqz	a0,1e70 <__clone+0x1a>
	# Parent
	ret
    1e6e:	8082                	ret

	# Child
1:      ld a1, 0(sp)
    1e70:	6582                	ld	a1,0(sp)
	ld a0, 8(sp)
    1e72:	6522                	ld	a0,8(sp)
	jalr a1
    1e74:	9582                	jalr	a1

	# Exit
	li a7, 93 # SYS_exit
    1e76:	05d00893          	li	a7,93
	ecall
    1e7a:	00000073          	ecall
