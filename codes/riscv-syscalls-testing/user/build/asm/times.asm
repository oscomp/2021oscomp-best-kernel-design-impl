
/home/ubuntu/rust/ultra-os/codes/riscv-syscalls-testing/user/build/riscv64/times:     file format elf64-littleriscv


Disassembly of section .text:

0000000000001000 <_start>:
.section .text.entry
.globl _start
_start:
    mv a0, sp
    1000:	850a                	mv	a0,sp
    tail __start_main
    1002:	a869                	j	109c <__start_main>

0000000000001004 <test_times>:
	long tms_cstime; 
};

struct tms mytimes;

void test_times() {
    1004:	1141                	addi	sp,sp,-16
	TEST_START(__func__);
    1006:	00001517          	auipc	a0,0x1
    100a:	ed250513          	addi	a0,a0,-302 # 1ed8 <__clone+0x2e>
void test_times() {
    100e:	e406                	sd	ra,8(sp)
	TEST_START(__func__);
    1010:	2fe000ef          	jal	ra,130e <puts>
    1014:	00001517          	auipc	a0,0x1
    1018:	fac50513          	addi	a0,a0,-84 # 1fc0 <__func__.0>
    101c:	2f2000ef          	jal	ra,130e <puts>
    1020:	00001517          	auipc	a0,0x1
    1024:	ed050513          	addi	a0,a0,-304 # 1ef0 <__clone+0x46>
    1028:	2e6000ef          	jal	ra,130e <puts>

	int test_ret = times(&mytimes);
    102c:	00001517          	auipc	a0,0x1
    1030:	f7450513          	addi	a0,a0,-140 # 1fa0 <mytimes>
    1034:	4c1000ef          	jal	ra,1cf4 <times>
	assert(test_ret >= 0);
    1038:	04054363          	bltz	a0,107e <test_times+0x7a>

	printf("mytimes success\n{tms_utime:%d, tms_stime:%d, tms_cutime:%d, tms_cstime:%d}\n",
    103c:	00001797          	auipc	a5,0x1
    1040:	f6478793          	addi	a5,a5,-156 # 1fa0 <mytimes>
    1044:	6f98                	ld	a4,24(a5)
    1046:	6b94                	ld	a3,16(a5)
    1048:	6790                	ld	a2,8(a5)
    104a:	638c                	ld	a1,0(a5)
    104c:	00001517          	auipc	a0,0x1
    1050:	ed450513          	addi	a0,a0,-300 # 1f20 <__clone+0x76>
    1054:	2dc000ef          	jal	ra,1330 <printf>
		mytimes.tms_utime, mytimes.tms_stime, mytimes.tms_cutime, mytimes.tms_cstime);
	TEST_END(__func__);
    1058:	00001517          	auipc	a0,0x1
    105c:	f1850513          	addi	a0,a0,-232 # 1f70 <__clone+0xc6>
    1060:	2ae000ef          	jal	ra,130e <puts>
    1064:	00001517          	auipc	a0,0x1
    1068:	f5c50513          	addi	a0,a0,-164 # 1fc0 <__func__.0>
    106c:	2a2000ef          	jal	ra,130e <puts>
}
    1070:	60a2                	ld	ra,8(sp)
	TEST_END(__func__);
    1072:	00001517          	auipc	a0,0x1
    1076:	e7e50513          	addi	a0,a0,-386 # 1ef0 <__clone+0x46>
}
    107a:	0141                	addi	sp,sp,16
	TEST_END(__func__);
    107c:	ac49                	j	130e <puts>
	assert(test_ret >= 0);
    107e:	00001517          	auipc	a0,0x1
    1082:	e8250513          	addi	a0,a0,-382 # 1f00 <__clone+0x56>
    1086:	52e000ef          	jal	ra,15b4 <panic>
    108a:	bf4d                	j	103c <test_times+0x38>

000000000000108c <main>:

int main(void) {
    108c:	1141                	addi	sp,sp,-16
    108e:	e406                	sd	ra,8(sp)
	test_times();
    1090:	f75ff0ef          	jal	ra,1004 <test_times>
	return 0;
}
    1094:	60a2                	ld	ra,8(sp)
    1096:	4501                	li	a0,0
    1098:	0141                	addi	sp,sp,16
    109a:	8082                	ret

000000000000109c <__start_main>:
#include <unistd.h>

extern int main();

int __start_main(long *p)
{
    109c:	85aa                	mv	a1,a0
	int argc = p[0];
	char **argv = (void *)(p+1);

	exit(main(argc, argv));
    109e:	4108                	lw	a0,0(a0)
{
    10a0:	1141                	addi	sp,sp,-16
	exit(main(argc, argv));
    10a2:	05a1                	addi	a1,a1,8
{
    10a4:	e406                	sd	ra,8(sp)
	exit(main(argc, argv));
    10a6:	fe7ff0ef          	jal	ra,108c <main>
    10aa:	41d000ef          	jal	ra,1cc6 <exit>
	return 0;
}
    10ae:	60a2                	ld	ra,8(sp)
    10b0:	4501                	li	a0,0
    10b2:	0141                	addi	sp,sp,16
    10b4:	8082                	ret

00000000000010b6 <printint.constprop.0>:
    write(f, s, l);
}

static char digits[] = "0123456789abcdef";

static void printint(int xx, int base, int sign)
    10b6:	7179                	addi	sp,sp,-48
    10b8:	f406                	sd	ra,40(sp)
{
    char buf[16 + 1];
    int i;
    uint x;

    if (sign && (sign = xx < 0))
    10ba:	12054b63          	bltz	a0,11f0 <printint.constprop.0+0x13a>

    buf[16] = 0;
    i = 15;
    do
    {
        buf[i--] = digits[x % base];
    10be:	02b577bb          	remuw	a5,a0,a1
    10c2:	00001617          	auipc	a2,0x1
    10c6:	f0e60613          	addi	a2,a2,-242 # 1fd0 <digits>
    buf[16] = 0;
    10ca:	00010c23          	sb	zero,24(sp)
        buf[i--] = digits[x % base];
    10ce:	0005871b          	sext.w	a4,a1
    10d2:	1782                	slli	a5,a5,0x20
    10d4:	9381                	srli	a5,a5,0x20
    10d6:	97b2                	add	a5,a5,a2
    10d8:	0007c783          	lbu	a5,0(a5)
    } while ((x /= base) != 0);
    10dc:	02b5583b          	divuw	a6,a0,a1
        buf[i--] = digits[x % base];
    10e0:	00f10ba3          	sb	a5,23(sp)
    } while ((x /= base) != 0);
    10e4:	1cb56363          	bltu	a0,a1,12aa <printint.constprop.0+0x1f4>
        buf[i--] = digits[x % base];
    10e8:	45b9                	li	a1,14
    10ea:	02e877bb          	remuw	a5,a6,a4
    10ee:	1782                	slli	a5,a5,0x20
    10f0:	9381                	srli	a5,a5,0x20
    10f2:	97b2                	add	a5,a5,a2
    10f4:	0007c783          	lbu	a5,0(a5)
    } while ((x /= base) != 0);
    10f8:	02e856bb          	divuw	a3,a6,a4
        buf[i--] = digits[x % base];
    10fc:	00f10b23          	sb	a5,22(sp)
    } while ((x /= base) != 0);
    1100:	0ce86e63          	bltu	a6,a4,11dc <printint.constprop.0+0x126>
        buf[i--] = digits[x % base];
    1104:	02e6f5bb          	remuw	a1,a3,a4
    } while ((x /= base) != 0);
    1108:	02e6d7bb          	divuw	a5,a3,a4
        buf[i--] = digits[x % base];
    110c:	1582                	slli	a1,a1,0x20
    110e:	9181                	srli	a1,a1,0x20
    1110:	95b2                	add	a1,a1,a2
    1112:	0005c583          	lbu	a1,0(a1)
    1116:	00b10aa3          	sb	a1,21(sp)
    } while ((x /= base) != 0);
    111a:	0007859b          	sext.w	a1,a5
    111e:	12e6ec63          	bltu	a3,a4,1256 <printint.constprop.0+0x1a0>
        buf[i--] = digits[x % base];
    1122:	02e7f6bb          	remuw	a3,a5,a4
    1126:	1682                	slli	a3,a3,0x20
    1128:	9281                	srli	a3,a3,0x20
    112a:	96b2                	add	a3,a3,a2
    112c:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    1130:	02e7d83b          	divuw	a6,a5,a4
        buf[i--] = digits[x % base];
    1134:	00d10a23          	sb	a3,20(sp)
    } while ((x /= base) != 0);
    1138:	12e5e863          	bltu	a1,a4,1268 <printint.constprop.0+0x1b2>
        buf[i--] = digits[x % base];
    113c:	02e876bb          	remuw	a3,a6,a4
    1140:	1682                	slli	a3,a3,0x20
    1142:	9281                	srli	a3,a3,0x20
    1144:	96b2                	add	a3,a3,a2
    1146:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    114a:	02e855bb          	divuw	a1,a6,a4
        buf[i--] = digits[x % base];
    114e:	00d109a3          	sb	a3,19(sp)
    } while ((x /= base) != 0);
    1152:	12e86463          	bltu	a6,a4,127a <printint.constprop.0+0x1c4>
        buf[i--] = digits[x % base];
    1156:	02e5f6bb          	remuw	a3,a1,a4
    115a:	1682                	slli	a3,a3,0x20
    115c:	9281                	srli	a3,a3,0x20
    115e:	96b2                	add	a3,a3,a2
    1160:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    1164:	02e5d83b          	divuw	a6,a1,a4
        buf[i--] = digits[x % base];
    1168:	00d10923          	sb	a3,18(sp)
    } while ((x /= base) != 0);
    116c:	0ce5ec63          	bltu	a1,a4,1244 <printint.constprop.0+0x18e>
        buf[i--] = digits[x % base];
    1170:	02e876bb          	remuw	a3,a6,a4
    1174:	1682                	slli	a3,a3,0x20
    1176:	9281                	srli	a3,a3,0x20
    1178:	96b2                	add	a3,a3,a2
    117a:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    117e:	02e855bb          	divuw	a1,a6,a4
        buf[i--] = digits[x % base];
    1182:	00d108a3          	sb	a3,17(sp)
    } while ((x /= base) != 0);
    1186:	10e86963          	bltu	a6,a4,1298 <printint.constprop.0+0x1e2>
        buf[i--] = digits[x % base];
    118a:	02e5f6bb          	remuw	a3,a1,a4
    118e:	1682                	slli	a3,a3,0x20
    1190:	9281                	srli	a3,a3,0x20
    1192:	96b2                	add	a3,a3,a2
    1194:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    1198:	02e5d83b          	divuw	a6,a1,a4
        buf[i--] = digits[x % base];
    119c:	00d10823          	sb	a3,16(sp)
    } while ((x /= base) != 0);
    11a0:	10e5e763          	bltu	a1,a4,12ae <printint.constprop.0+0x1f8>
        buf[i--] = digits[x % base];
    11a4:	02e876bb          	remuw	a3,a6,a4
    11a8:	1682                	slli	a3,a3,0x20
    11aa:	9281                	srli	a3,a3,0x20
    11ac:	96b2                	add	a3,a3,a2
    11ae:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    11b2:	02e857bb          	divuw	a5,a6,a4
        buf[i--] = digits[x % base];
    11b6:	00d107a3          	sb	a3,15(sp)
    } while ((x /= base) != 0);
    11ba:	10e86363          	bltu	a6,a4,12c0 <printint.constprop.0+0x20a>
        buf[i--] = digits[x % base];
    11be:	1782                	slli	a5,a5,0x20
    11c0:	9381                	srli	a5,a5,0x20
    11c2:	97b2                	add	a5,a5,a2
    11c4:	0007c783          	lbu	a5,0(a5)
    11c8:	4599                	li	a1,6
    11ca:	00f10723          	sb	a5,14(sp)

    if (sign)
    11ce:	00055763          	bgez	a0,11dc <printint.constprop.0+0x126>
        buf[i--] = '-';
    11d2:	02d00793          	li	a5,45
    11d6:	00f106a3          	sb	a5,13(sp)
        buf[i--] = digits[x % base];
    11da:	4595                	li	a1,5
    write(f, s, l);
    11dc:	003c                	addi	a5,sp,8
    11de:	4641                	li	a2,16
    11e0:	9e0d                	subw	a2,a2,a1
    11e2:	4505                	li	a0,1
    11e4:	95be                	add	a1,a1,a5
    11e6:	291000ef          	jal	ra,1c76 <write>
    i++;
    if (i < 0)
        puts("printint error");
    out(stdout, buf + i, 16 - i);
}
    11ea:	70a2                	ld	ra,40(sp)
    11ec:	6145                	addi	sp,sp,48
    11ee:	8082                	ret
        x = -xx;
    11f0:	40a0083b          	negw	a6,a0
        buf[i--] = digits[x % base];
    11f4:	02b877bb          	remuw	a5,a6,a1
    11f8:	00001617          	auipc	a2,0x1
    11fc:	dd860613          	addi	a2,a2,-552 # 1fd0 <digits>
    buf[16] = 0;
    1200:	00010c23          	sb	zero,24(sp)
        buf[i--] = digits[x % base];
    1204:	0005871b          	sext.w	a4,a1
    1208:	1782                	slli	a5,a5,0x20
    120a:	9381                	srli	a5,a5,0x20
    120c:	97b2                	add	a5,a5,a2
    120e:	0007c783          	lbu	a5,0(a5)
    } while ((x /= base) != 0);
    1212:	02b858bb          	divuw	a7,a6,a1
        buf[i--] = digits[x % base];
    1216:	00f10ba3          	sb	a5,23(sp)
    } while ((x /= base) != 0);
    121a:	06b86963          	bltu	a6,a1,128c <printint.constprop.0+0x1d6>
        buf[i--] = digits[x % base];
    121e:	02e8f7bb          	remuw	a5,a7,a4
    1222:	1782                	slli	a5,a5,0x20
    1224:	9381                	srli	a5,a5,0x20
    1226:	97b2                	add	a5,a5,a2
    1228:	0007c783          	lbu	a5,0(a5)
    } while ((x /= base) != 0);
    122c:	02e8d6bb          	divuw	a3,a7,a4
        buf[i--] = digits[x % base];
    1230:	00f10b23          	sb	a5,22(sp)
    } while ((x /= base) != 0);
    1234:	ece8f8e3          	bgeu	a7,a4,1104 <printint.constprop.0+0x4e>
        buf[i--] = '-';
    1238:	02d00793          	li	a5,45
    123c:	00f10aa3          	sb	a5,21(sp)
        buf[i--] = digits[x % base];
    1240:	45b5                	li	a1,13
    1242:	bf69                	j	11dc <printint.constprop.0+0x126>
    1244:	45a9                	li	a1,10
    if (sign)
    1246:	f8055be3          	bgez	a0,11dc <printint.constprop.0+0x126>
        buf[i--] = '-';
    124a:	02d00793          	li	a5,45
    124e:	00f108a3          	sb	a5,17(sp)
        buf[i--] = digits[x % base];
    1252:	45a5                	li	a1,9
    1254:	b761                	j	11dc <printint.constprop.0+0x126>
    1256:	45b5                	li	a1,13
    if (sign)
    1258:	f80552e3          	bgez	a0,11dc <printint.constprop.0+0x126>
        buf[i--] = '-';
    125c:	02d00793          	li	a5,45
    1260:	00f10a23          	sb	a5,20(sp)
        buf[i--] = digits[x % base];
    1264:	45b1                	li	a1,12
    1266:	bf9d                	j	11dc <printint.constprop.0+0x126>
    1268:	45b1                	li	a1,12
    if (sign)
    126a:	f60559e3          	bgez	a0,11dc <printint.constprop.0+0x126>
        buf[i--] = '-';
    126e:	02d00793          	li	a5,45
    1272:	00f109a3          	sb	a5,19(sp)
        buf[i--] = digits[x % base];
    1276:	45ad                	li	a1,11
    1278:	b795                	j	11dc <printint.constprop.0+0x126>
    127a:	45ad                	li	a1,11
    if (sign)
    127c:	f60550e3          	bgez	a0,11dc <printint.constprop.0+0x126>
        buf[i--] = '-';
    1280:	02d00793          	li	a5,45
    1284:	00f10923          	sb	a5,18(sp)
        buf[i--] = digits[x % base];
    1288:	45a9                	li	a1,10
    128a:	bf89                	j	11dc <printint.constprop.0+0x126>
        buf[i--] = '-';
    128c:	02d00793          	li	a5,45
    1290:	00f10b23          	sb	a5,22(sp)
        buf[i--] = digits[x % base];
    1294:	45b9                	li	a1,14
    1296:	b799                	j	11dc <printint.constprop.0+0x126>
    1298:	45a5                	li	a1,9
    if (sign)
    129a:	f40551e3          	bgez	a0,11dc <printint.constprop.0+0x126>
        buf[i--] = '-';
    129e:	02d00793          	li	a5,45
    12a2:	00f10823          	sb	a5,16(sp)
        buf[i--] = digits[x % base];
    12a6:	45a1                	li	a1,8
    12a8:	bf15                	j	11dc <printint.constprop.0+0x126>
    i = 15;
    12aa:	45bd                	li	a1,15
    12ac:	bf05                	j	11dc <printint.constprop.0+0x126>
        buf[i--] = digits[x % base];
    12ae:	45a1                	li	a1,8
    if (sign)
    12b0:	f20556e3          	bgez	a0,11dc <printint.constprop.0+0x126>
        buf[i--] = '-';
    12b4:	02d00793          	li	a5,45
    12b8:	00f107a3          	sb	a5,15(sp)
        buf[i--] = digits[x % base];
    12bc:	459d                	li	a1,7
    12be:	bf39                	j	11dc <printint.constprop.0+0x126>
    12c0:	459d                	li	a1,7
    if (sign)
    12c2:	f0055de3          	bgez	a0,11dc <printint.constprop.0+0x126>
        buf[i--] = '-';
    12c6:	02d00793          	li	a5,45
    12ca:	00f10723          	sb	a5,14(sp)
        buf[i--] = digits[x % base];
    12ce:	4599                	li	a1,6
    12d0:	b731                	j	11dc <printint.constprop.0+0x126>

00000000000012d2 <getchar>:
{
    12d2:	1101                	addi	sp,sp,-32
    read(stdin, &byte, 1);
    12d4:	00f10593          	addi	a1,sp,15
    12d8:	4605                	li	a2,1
    12da:	4501                	li	a0,0
{
    12dc:	ec06                	sd	ra,24(sp)
    char byte = 0;
    12de:	000107a3          	sb	zero,15(sp)
    read(stdin, &byte, 1);
    12e2:	18b000ef          	jal	ra,1c6c <read>
}
    12e6:	60e2                	ld	ra,24(sp)
    12e8:	00f14503          	lbu	a0,15(sp)
    12ec:	6105                	addi	sp,sp,32
    12ee:	8082                	ret

00000000000012f0 <putchar>:
{
    12f0:	1101                	addi	sp,sp,-32
    12f2:	87aa                	mv	a5,a0
    return write(stdout, &byte, 1);
    12f4:	00f10593          	addi	a1,sp,15
    12f8:	4605                	li	a2,1
    12fa:	4505                	li	a0,1
{
    12fc:	ec06                	sd	ra,24(sp)
    char byte = c;
    12fe:	00f107a3          	sb	a5,15(sp)
    return write(stdout, &byte, 1);
    1302:	175000ef          	jal	ra,1c76 <write>
}
    1306:	60e2                	ld	ra,24(sp)
    1308:	2501                	sext.w	a0,a0
    130a:	6105                	addi	sp,sp,32
    130c:	8082                	ret

000000000000130e <puts>:
{
    130e:	1141                	addi	sp,sp,-16
    1310:	e406                	sd	ra,8(sp)
    1312:	e022                	sd	s0,0(sp)
    1314:	842a                	mv	s0,a0
    r = -(write(stdout, s, strlen(s)) < 0);
    1316:	57c000ef          	jal	ra,1892 <strlen>
    131a:	862a                	mv	a2,a0
    131c:	85a2                	mv	a1,s0
    131e:	4505                	li	a0,1
    1320:	157000ef          	jal	ra,1c76 <write>
}
    1324:	60a2                	ld	ra,8(sp)
    1326:	6402                	ld	s0,0(sp)
    r = -(write(stdout, s, strlen(s)) < 0);
    1328:	957d                	srai	a0,a0,0x3f
    return r;
    132a:	2501                	sext.w	a0,a0
}
    132c:	0141                	addi	sp,sp,16
    132e:	8082                	ret

0000000000001330 <printf>:
    out(stdout, buf, i);
}

// Print to the console. only understands %d, %x, %p, %s.
void printf(const char *fmt, ...)
{
    1330:	7171                	addi	sp,sp,-176
    1332:	fc56                	sd	s5,56(sp)
    1334:	ed3e                	sd	a5,152(sp)
    buf[i++] = '0';
    1336:	7ae1                	lui	s5,0xffff8
    va_list ap;
    int cnt = 0, l = 0;
    char *a, *z, *s = (char *)fmt, str;
    int f = stdout;

    va_start(ap, fmt);
    1338:	18bc                	addi	a5,sp,120
{
    133a:	e8ca                	sd	s2,80(sp)
    133c:	e4ce                	sd	s3,72(sp)
    133e:	e0d2                	sd	s4,64(sp)
    1340:	f85a                	sd	s6,48(sp)
    1342:	f486                	sd	ra,104(sp)
    1344:	f0a2                	sd	s0,96(sp)
    1346:	eca6                	sd	s1,88(sp)
    1348:	fcae                	sd	a1,120(sp)
    134a:	e132                	sd	a2,128(sp)
    134c:	e536                	sd	a3,136(sp)
    134e:	e93a                	sd	a4,144(sp)
    1350:	f142                	sd	a6,160(sp)
    1352:	f546                	sd	a7,168(sp)
    va_start(ap, fmt);
    1354:	e03e                	sd	a5,0(sp)
    for (;;)
    {
        if (!*s)
            break;
        for (a = s; *s && *s != '%'; s++)
    1356:	02500913          	li	s2,37
        out(f, a, l);
        if (l)
            continue;
        if (s[1] == 0)
            break;
        switch (s[1])
    135a:	07300a13          	li	s4,115
        case 'p':
            printptr(va_arg(ap, uint64));
            break;
        case 's':
            if ((a = va_arg(ap, char *)) == 0)
                a = "(null)";
    135e:	00001b17          	auipc	s6,0x1
    1362:	c22b0b13          	addi	s6,s6,-990 # 1f80 <__clone+0xd6>
    buf[i++] = '0';
    1366:	830aca93          	xori	s5,s5,-2000
        buf[i++] = digits[x >> (sizeof(uint64) * 8 - 4)];
    136a:	00001997          	auipc	s3,0x1
    136e:	c6698993          	addi	s3,s3,-922 # 1fd0 <digits>
        if (!*s)
    1372:	00054783          	lbu	a5,0(a0)
    1376:	16078a63          	beqz	a5,14ea <printf+0x1ba>
    137a:	862a                	mv	a2,a0
        for (a = s; *s && *s != '%'; s++)
    137c:	19278163          	beq	a5,s2,14fe <printf+0x1ce>
    1380:	00164783          	lbu	a5,1(a2)
    1384:	0605                	addi	a2,a2,1
    1386:	fbfd                	bnez	a5,137c <printf+0x4c>
    1388:	84b2                	mv	s1,a2
        l = z - a;
    138a:	40a6043b          	subw	s0,a2,a0
    write(f, s, l);
    138e:	85aa                	mv	a1,a0
    1390:	8622                	mv	a2,s0
    1392:	4505                	li	a0,1
    1394:	0e3000ef          	jal	ra,1c76 <write>
        if (l)
    1398:	18041c63          	bnez	s0,1530 <printf+0x200>
        if (s[1] == 0)
    139c:	0014c783          	lbu	a5,1(s1)
    13a0:	14078563          	beqz	a5,14ea <printf+0x1ba>
        switch (s[1])
    13a4:	1d478063          	beq	a5,s4,1564 <printf+0x234>
    13a8:	18fa6663          	bltu	s4,a5,1534 <printf+0x204>
    13ac:	06400713          	li	a4,100
    13b0:	1ae78063          	beq	a5,a4,1550 <printf+0x220>
    13b4:	07000713          	li	a4,112
    13b8:	1ce79963          	bne	a5,a4,158a <printf+0x25a>
            printptr(va_arg(ap, uint64));
    13bc:	6702                	ld	a4,0(sp)
    buf[i++] = '0';
    13be:	01511423          	sh	s5,8(sp)
    write(f, s, l);
    13c2:	4649                	li	a2,18
            printptr(va_arg(ap, uint64));
    13c4:	631c                	ld	a5,0(a4)
    13c6:	0721                	addi	a4,a4,8
    13c8:	e03a                	sd	a4,0(sp)
    for (j = 0; j < (sizeof(uint64) * 2); j++, x <<= 4)
    13ca:	00479293          	slli	t0,a5,0x4
    13ce:	00879f93          	slli	t6,a5,0x8
    13d2:	00c79f13          	slli	t5,a5,0xc
    13d6:	01079e93          	slli	t4,a5,0x10
    13da:	01479e13          	slli	t3,a5,0x14
    13de:	01879313          	slli	t1,a5,0x18
    13e2:	01c79893          	slli	a7,a5,0x1c
    13e6:	02479813          	slli	a6,a5,0x24
    13ea:	02879513          	slli	a0,a5,0x28
    13ee:	02c79593          	slli	a1,a5,0x2c
    13f2:	03079693          	slli	a3,a5,0x30
    13f6:	03479713          	slli	a4,a5,0x34
        buf[i++] = digits[x >> (sizeof(uint64) * 8 - 4)];
    13fa:	03c7d413          	srli	s0,a5,0x3c
    13fe:	01c7d39b          	srliw	t2,a5,0x1c
    1402:	03c2d293          	srli	t0,t0,0x3c
    1406:	03cfdf93          	srli	t6,t6,0x3c
    140a:	03cf5f13          	srli	t5,t5,0x3c
    140e:	03cede93          	srli	t4,t4,0x3c
    1412:	03ce5e13          	srli	t3,t3,0x3c
    1416:	03c35313          	srli	t1,t1,0x3c
    141a:	03c8d893          	srli	a7,a7,0x3c
    141e:	03c85813          	srli	a6,a6,0x3c
    1422:	9171                	srli	a0,a0,0x3c
    1424:	91f1                	srli	a1,a1,0x3c
    1426:	92f1                	srli	a3,a3,0x3c
    1428:	9371                	srli	a4,a4,0x3c
    142a:	96ce                	add	a3,a3,s3
    142c:	974e                	add	a4,a4,s3
    142e:	944e                	add	s0,s0,s3
    1430:	92ce                	add	t0,t0,s3
    1432:	9fce                	add	t6,t6,s3
    1434:	9f4e                	add	t5,t5,s3
    1436:	9ece                	add	t4,t4,s3
    1438:	9e4e                	add	t3,t3,s3
    143a:	934e                	add	t1,t1,s3
    143c:	98ce                	add	a7,a7,s3
    143e:	93ce                	add	t2,t2,s3
    1440:	984e                	add	a6,a6,s3
    1442:	954e                	add	a0,a0,s3
    1444:	95ce                	add	a1,a1,s3
    1446:	0006c083          	lbu	ra,0(a3)
    144a:	0002c283          	lbu	t0,0(t0)
    144e:	00074683          	lbu	a3,0(a4)
    1452:	000fcf83          	lbu	t6,0(t6)
    1456:	000f4f03          	lbu	t5,0(t5)
    145a:	000ece83          	lbu	t4,0(t4)
    145e:	000e4e03          	lbu	t3,0(t3)
    1462:	00034303          	lbu	t1,0(t1)
    1466:	0008c883          	lbu	a7,0(a7)
    146a:	0003c383          	lbu	t2,0(t2)
    146e:	00084803          	lbu	a6,0(a6)
    1472:	00054503          	lbu	a0,0(a0)
    1476:	0005c583          	lbu	a1,0(a1)
    147a:	00044403          	lbu	s0,0(s0)
    for (j = 0; j < (sizeof(uint64) * 2); j++, x <<= 4)
    147e:	03879713          	slli	a4,a5,0x38
        buf[i++] = digits[x >> (sizeof(uint64) * 8 - 4)];
    1482:	9371                	srli	a4,a4,0x3c
    1484:	8bbd                	andi	a5,a5,15
    1486:	974e                	add	a4,a4,s3
    1488:	97ce                	add	a5,a5,s3
    148a:	005105a3          	sb	t0,11(sp)
    148e:	01f10623          	sb	t6,12(sp)
    1492:	01e106a3          	sb	t5,13(sp)
    1496:	01d10723          	sb	t4,14(sp)
    149a:	01c107a3          	sb	t3,15(sp)
    149e:	00610823          	sb	t1,16(sp)
    14a2:	011108a3          	sb	a7,17(sp)
    14a6:	00710923          	sb	t2,18(sp)
    14aa:	010109a3          	sb	a6,19(sp)
    14ae:	00a10a23          	sb	a0,20(sp)
    14b2:	00b10aa3          	sb	a1,21(sp)
    14b6:	00110b23          	sb	ra,22(sp)
    14ba:	00d10ba3          	sb	a3,23(sp)
    14be:	00810523          	sb	s0,10(sp)
    14c2:	00074703          	lbu	a4,0(a4)
    14c6:	0007c783          	lbu	a5,0(a5)
    write(f, s, l);
    14ca:	002c                	addi	a1,sp,8
    14cc:	4505                	li	a0,1
        buf[i++] = digits[x >> (sizeof(uint64) * 8 - 4)];
    14ce:	00e10c23          	sb	a4,24(sp)
    14d2:	00f10ca3          	sb	a5,25(sp)
    buf[i] = 0;
    14d6:	00010d23          	sb	zero,26(sp)
    write(f, s, l);
    14da:	79c000ef          	jal	ra,1c76 <write>
            // Print unknown % sequence to draw attention.
            putchar('%');
            putchar(s[1]);
            break;
        }
        s += 2;
    14de:	00248513          	addi	a0,s1,2
        if (!*s)
    14e2:	00054783          	lbu	a5,0(a0)
    14e6:	e8079ae3          	bnez	a5,137a <printf+0x4a>
    }
    va_end(ap);
}
    14ea:	70a6                	ld	ra,104(sp)
    14ec:	7406                	ld	s0,96(sp)
    14ee:	64e6                	ld	s1,88(sp)
    14f0:	6946                	ld	s2,80(sp)
    14f2:	69a6                	ld	s3,72(sp)
    14f4:	6a06                	ld	s4,64(sp)
    14f6:	7ae2                	ld	s5,56(sp)
    14f8:	7b42                	ld	s6,48(sp)
    14fa:	614d                	addi	sp,sp,176
    14fc:	8082                	ret
        for (z = s; s[0] == '%' && s[1] == '%'; z++, s += 2)
    14fe:	00064783          	lbu	a5,0(a2)
    1502:	84b2                	mv	s1,a2
    1504:	01278963          	beq	a5,s2,1516 <printf+0x1e6>
    1508:	b549                	j	138a <printf+0x5a>
    150a:	0024c783          	lbu	a5,2(s1)
    150e:	0605                	addi	a2,a2,1
    1510:	0489                	addi	s1,s1,2
    1512:	e7279ce3          	bne	a5,s2,138a <printf+0x5a>
    1516:	0014c783          	lbu	a5,1(s1)
    151a:	ff2788e3          	beq	a5,s2,150a <printf+0x1da>
        l = z - a;
    151e:	40a6043b          	subw	s0,a2,a0
    write(f, s, l);
    1522:	85aa                	mv	a1,a0
    1524:	8622                	mv	a2,s0
    1526:	4505                	li	a0,1
    1528:	74e000ef          	jal	ra,1c76 <write>
        if (l)
    152c:	e60408e3          	beqz	s0,139c <printf+0x6c>
    1530:	8526                	mv	a0,s1
    1532:	b581                	j	1372 <printf+0x42>
        switch (s[1])
    1534:	07800713          	li	a4,120
    1538:	04e79963          	bne	a5,a4,158a <printf+0x25a>
            printint(va_arg(ap, int), 16, 1);
    153c:	6782                	ld	a5,0(sp)
    153e:	45c1                	li	a1,16
    1540:	4388                	lw	a0,0(a5)
    1542:	07a1                	addi	a5,a5,8
    1544:	e03e                	sd	a5,0(sp)
    1546:	b71ff0ef          	jal	ra,10b6 <printint.constprop.0>
        s += 2;
    154a:	00248513          	addi	a0,s1,2
    154e:	bf51                	j	14e2 <printf+0x1b2>
            printint(va_arg(ap, int), 10, 1);
    1550:	6782                	ld	a5,0(sp)
    1552:	45a9                	li	a1,10
    1554:	4388                	lw	a0,0(a5)
    1556:	07a1                	addi	a5,a5,8
    1558:	e03e                	sd	a5,0(sp)
    155a:	b5dff0ef          	jal	ra,10b6 <printint.constprop.0>
        s += 2;
    155e:	00248513          	addi	a0,s1,2
    1562:	b741                	j	14e2 <printf+0x1b2>
            if ((a = va_arg(ap, char *)) == 0)
    1564:	6782                	ld	a5,0(sp)
    1566:	6380                	ld	s0,0(a5)
    1568:	07a1                	addi	a5,a5,8
    156a:	e03e                	sd	a5,0(sp)
    156c:	c031                	beqz	s0,15b0 <printf+0x280>
            l = strnlen(a, 200);
    156e:	0c800593          	li	a1,200
    1572:	8522                	mv	a0,s0
    1574:	40a000ef          	jal	ra,197e <strnlen>
    write(f, s, l);
    1578:	0005061b          	sext.w	a2,a0
    157c:	85a2                	mv	a1,s0
    157e:	4505                	li	a0,1
    1580:	6f6000ef          	jal	ra,1c76 <write>
        s += 2;
    1584:	00248513          	addi	a0,s1,2
    1588:	bfa9                	j	14e2 <printf+0x1b2>
    return write(stdout, &byte, 1);
    158a:	4605                	li	a2,1
    158c:	002c                	addi	a1,sp,8
    158e:	4505                	li	a0,1
    char byte = c;
    1590:	01210423          	sb	s2,8(sp)
    return write(stdout, &byte, 1);
    1594:	6e2000ef          	jal	ra,1c76 <write>
    char byte = c;
    1598:	0014c783          	lbu	a5,1(s1)
    return write(stdout, &byte, 1);
    159c:	4605                	li	a2,1
    159e:	002c                	addi	a1,sp,8
    15a0:	4505                	li	a0,1
    char byte = c;
    15a2:	00f10423          	sb	a5,8(sp)
    return write(stdout, &byte, 1);
    15a6:	6d0000ef          	jal	ra,1c76 <write>
        s += 2;
    15aa:	00248513          	addi	a0,s1,2
    15ae:	bf15                	j	14e2 <printf+0x1b2>
                a = "(null)";
    15b0:	845a                	mv	s0,s6
    15b2:	bf75                	j	156e <printf+0x23e>

00000000000015b4 <panic>:
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

void panic(char *m)
{
    15b4:	1141                	addi	sp,sp,-16
    15b6:	e406                	sd	ra,8(sp)
    puts(m);
    15b8:	d57ff0ef          	jal	ra,130e <puts>
    exit(-100);
}
    15bc:	60a2                	ld	ra,8(sp)
    exit(-100);
    15be:	f9c00513          	li	a0,-100
}
    15c2:	0141                	addi	sp,sp,16
    exit(-100);
    15c4:	a709                	j	1cc6 <exit>

00000000000015c6 <isspace>:
#define HIGHS (ONES * (UCHAR_MAX / 2 + 1))
#define HASZERO(x) (((x)-ONES) & ~(x)&HIGHS)

int isspace(int c)
{
    return c == ' ' || (unsigned)c - '\t' < 5;
    15c6:	02000793          	li	a5,32
    15ca:	00f50663          	beq	a0,a5,15d6 <isspace+0x10>
    15ce:	355d                	addiw	a0,a0,-9
    15d0:	00553513          	sltiu	a0,a0,5
    15d4:	8082                	ret
    15d6:	4505                	li	a0,1
}
    15d8:	8082                	ret

00000000000015da <isdigit>:

int isdigit(int c)
{
    return (unsigned)c - '0' < 10;
    15da:	fd05051b          	addiw	a0,a0,-48
}
    15de:	00a53513          	sltiu	a0,a0,10
    15e2:	8082                	ret

00000000000015e4 <atoi>:
    return c == ' ' || (unsigned)c - '\t' < 5;
    15e4:	02000613          	li	a2,32
    15e8:	4591                	li	a1,4

int atoi(const char *s)
{
    int n = 0, neg = 0;
    while (isspace(*s))
    15ea:	00054703          	lbu	a4,0(a0)
    return c == ' ' || (unsigned)c - '\t' < 5;
    15ee:	ff77069b          	addiw	a3,a4,-9
    15f2:	04c70d63          	beq	a4,a2,164c <atoi+0x68>
    15f6:	0007079b          	sext.w	a5,a4
    15fa:	04d5f963          	bgeu	a1,a3,164c <atoi+0x68>
        s++;
    switch (*s)
    15fe:	02b00693          	li	a3,43
    1602:	04d70a63          	beq	a4,a3,1656 <atoi+0x72>
    1606:	02d00693          	li	a3,45
    160a:	06d70463          	beq	a4,a3,1672 <atoi+0x8e>
        neg = 1;
    case '+':
        s++;
    }
    /* Compute n as a negative number to avoid overflow on INT_MIN */
    while (isdigit(*s))
    160e:	fd07859b          	addiw	a1,a5,-48
    1612:	4625                	li	a2,9
    1614:	873e                	mv	a4,a5
    1616:	86aa                	mv	a3,a0
    int n = 0, neg = 0;
    1618:	4e01                	li	t3,0
    while (isdigit(*s))
    161a:	04b66a63          	bltu	a2,a1,166e <atoi+0x8a>
    int n = 0, neg = 0;
    161e:	4501                	li	a0,0
    while (isdigit(*s))
    1620:	4825                	li	a6,9
    1622:	0016c603          	lbu	a2,1(a3)
        n = 10 * n - (*s++ - '0');
    1626:	0025179b          	slliw	a5,a0,0x2
    162a:	9d3d                	addw	a0,a0,a5
    162c:	fd07031b          	addiw	t1,a4,-48
    1630:	0015189b          	slliw	a7,a0,0x1
    while (isdigit(*s))
    1634:	fd06059b          	addiw	a1,a2,-48
        n = 10 * n - (*s++ - '0');
    1638:	0685                	addi	a3,a3,1
    163a:	4068853b          	subw	a0,a7,t1
    while (isdigit(*s))
    163e:	0006071b          	sext.w	a4,a2
    1642:	feb870e3          	bgeu	a6,a1,1622 <atoi+0x3e>
    return neg ? n : -n;
    1646:	000e0563          	beqz	t3,1650 <atoi+0x6c>
}
    164a:	8082                	ret
        s++;
    164c:	0505                	addi	a0,a0,1
    164e:	bf71                	j	15ea <atoi+0x6>
    return neg ? n : -n;
    1650:	4113053b          	subw	a0,t1,a7
    1654:	8082                	ret
    while (isdigit(*s))
    1656:	00154783          	lbu	a5,1(a0)
    165a:	4625                	li	a2,9
        s++;
    165c:	00150693          	addi	a3,a0,1
    while (isdigit(*s))
    1660:	fd07859b          	addiw	a1,a5,-48
    1664:	0007871b          	sext.w	a4,a5
    int n = 0, neg = 0;
    1668:	4e01                	li	t3,0
    while (isdigit(*s))
    166a:	fab67ae3          	bgeu	a2,a1,161e <atoi+0x3a>
    166e:	4501                	li	a0,0
}
    1670:	8082                	ret
    while (isdigit(*s))
    1672:	00154783          	lbu	a5,1(a0)
    1676:	4625                	li	a2,9
        s++;
    1678:	00150693          	addi	a3,a0,1
    while (isdigit(*s))
    167c:	fd07859b          	addiw	a1,a5,-48
    1680:	0007871b          	sext.w	a4,a5
    1684:	feb665e3          	bltu	a2,a1,166e <atoi+0x8a>
        neg = 1;
    1688:	4e05                	li	t3,1
    168a:	bf51                	j	161e <atoi+0x3a>

000000000000168c <memset>:

void *memset(void *dest, int c, size_t n)
{
    char *p = dest;
    for (int i = 0; i < n; ++i, *(p++) = c)
    168c:	16060d63          	beqz	a2,1806 <memset+0x17a>
    1690:	40a007b3          	neg	a5,a0
    1694:	8b9d                	andi	a5,a5,7
    1696:	00778713          	addi	a4,a5,7
    169a:	482d                	li	a6,11
    169c:	0ff5f593          	andi	a1,a1,255
    16a0:	fff60693          	addi	a3,a2,-1
    16a4:	17076263          	bltu	a4,a6,1808 <memset+0x17c>
    16a8:	16e6ea63          	bltu	a3,a4,181c <memset+0x190>
    16ac:	16078563          	beqz	a5,1816 <memset+0x18a>
    16b0:	00b50023          	sb	a1,0(a0)
    16b4:	4705                	li	a4,1
    16b6:	00150e93          	addi	t4,a0,1
    16ba:	14e78c63          	beq	a5,a4,1812 <memset+0x186>
    16be:	00b500a3          	sb	a1,1(a0)
    16c2:	4709                	li	a4,2
    16c4:	00250e93          	addi	t4,a0,2
    16c8:	14e78d63          	beq	a5,a4,1822 <memset+0x196>
    16cc:	00b50123          	sb	a1,2(a0)
    16d0:	470d                	li	a4,3
    16d2:	00350e93          	addi	t4,a0,3
    16d6:	12e78b63          	beq	a5,a4,180c <memset+0x180>
    16da:	00b501a3          	sb	a1,3(a0)
    16de:	4711                	li	a4,4
    16e0:	00450e93          	addi	t4,a0,4
    16e4:	14e78163          	beq	a5,a4,1826 <memset+0x19a>
    16e8:	00b50223          	sb	a1,4(a0)
    16ec:	4715                	li	a4,5
    16ee:	00550e93          	addi	t4,a0,5
    16f2:	12e78c63          	beq	a5,a4,182a <memset+0x19e>
    16f6:	00b502a3          	sb	a1,5(a0)
    16fa:	471d                	li	a4,7
    16fc:	00650e93          	addi	t4,a0,6
    1700:	12e79763          	bne	a5,a4,182e <memset+0x1a2>
    1704:	00750e93          	addi	t4,a0,7
    1708:	00b50323          	sb	a1,6(a0)
    170c:	4f1d                	li	t5,7
    170e:	00859713          	slli	a4,a1,0x8
    1712:	8f4d                	or	a4,a4,a1
    1714:	01059e13          	slli	t3,a1,0x10
    1718:	01c76e33          	or	t3,a4,t3
    171c:	01859313          	slli	t1,a1,0x18
    1720:	006e6333          	or	t1,t3,t1
    1724:	02059893          	slli	a7,a1,0x20
    1728:	011368b3          	or	a7,t1,a7
    172c:	02859813          	slli	a6,a1,0x28
    1730:	40f60333          	sub	t1,a2,a5
    1734:	0108e833          	or	a6,a7,a6
    1738:	03059693          	slli	a3,a1,0x30
    173c:	00d866b3          	or	a3,a6,a3
    1740:	03859713          	slli	a4,a1,0x38
    1744:	97aa                	add	a5,a5,a0
    1746:	ff837813          	andi	a6,t1,-8
    174a:	8f55                	or	a4,a4,a3
    174c:	00f806b3          	add	a3,a6,a5
    1750:	e398                	sd	a4,0(a5)
    1752:	07a1                	addi	a5,a5,8
    1754:	fed79ee3          	bne	a5,a3,1750 <memset+0xc4>
    1758:	ff837693          	andi	a3,t1,-8
    175c:	00de87b3          	add	a5,t4,a3
    1760:	01e6873b          	addw	a4,a3,t5
    1764:	0ad30663          	beq	t1,a3,1810 <memset+0x184>
    1768:	00b78023          	sb	a1,0(a5)
    176c:	0017069b          	addiw	a3,a4,1
    1770:	08c6fb63          	bgeu	a3,a2,1806 <memset+0x17a>
    1774:	00b780a3          	sb	a1,1(a5)
    1778:	0027069b          	addiw	a3,a4,2
    177c:	08c6f563          	bgeu	a3,a2,1806 <memset+0x17a>
    1780:	00b78123          	sb	a1,2(a5)
    1784:	0037069b          	addiw	a3,a4,3
    1788:	06c6ff63          	bgeu	a3,a2,1806 <memset+0x17a>
    178c:	00b781a3          	sb	a1,3(a5)
    1790:	0047069b          	addiw	a3,a4,4
    1794:	06c6f963          	bgeu	a3,a2,1806 <memset+0x17a>
    1798:	00b78223          	sb	a1,4(a5)
    179c:	0057069b          	addiw	a3,a4,5
    17a0:	06c6f363          	bgeu	a3,a2,1806 <memset+0x17a>
    17a4:	00b782a3          	sb	a1,5(a5)
    17a8:	0067069b          	addiw	a3,a4,6
    17ac:	04c6fd63          	bgeu	a3,a2,1806 <memset+0x17a>
    17b0:	00b78323          	sb	a1,6(a5)
    17b4:	0077069b          	addiw	a3,a4,7
    17b8:	04c6f763          	bgeu	a3,a2,1806 <memset+0x17a>
    17bc:	00b783a3          	sb	a1,7(a5)
    17c0:	0087069b          	addiw	a3,a4,8
    17c4:	04c6f163          	bgeu	a3,a2,1806 <memset+0x17a>
    17c8:	00b78423          	sb	a1,8(a5)
    17cc:	0097069b          	addiw	a3,a4,9
    17d0:	02c6fb63          	bgeu	a3,a2,1806 <memset+0x17a>
    17d4:	00b784a3          	sb	a1,9(a5)
    17d8:	00a7069b          	addiw	a3,a4,10
    17dc:	02c6f563          	bgeu	a3,a2,1806 <memset+0x17a>
    17e0:	00b78523          	sb	a1,10(a5)
    17e4:	00b7069b          	addiw	a3,a4,11
    17e8:	00c6ff63          	bgeu	a3,a2,1806 <memset+0x17a>
    17ec:	00b785a3          	sb	a1,11(a5)
    17f0:	00c7069b          	addiw	a3,a4,12
    17f4:	00c6f963          	bgeu	a3,a2,1806 <memset+0x17a>
    17f8:	00b78623          	sb	a1,12(a5)
    17fc:	2735                	addiw	a4,a4,13
    17fe:	00c77463          	bgeu	a4,a2,1806 <memset+0x17a>
    1802:	00b786a3          	sb	a1,13(a5)
        ;
    return dest;
}
    1806:	8082                	ret
    1808:	472d                	li	a4,11
    180a:	bd79                	j	16a8 <memset+0x1c>
    for (int i = 0; i < n; ++i, *(p++) = c)
    180c:	4f0d                	li	t5,3
    180e:	b701                	j	170e <memset+0x82>
    1810:	8082                	ret
    1812:	4f05                	li	t5,1
    1814:	bded                	j	170e <memset+0x82>
    1816:	8eaa                	mv	t4,a0
    1818:	4f01                	li	t5,0
    181a:	bdd5                	j	170e <memset+0x82>
    181c:	87aa                	mv	a5,a0
    181e:	4701                	li	a4,0
    1820:	b7a1                	j	1768 <memset+0xdc>
    1822:	4f09                	li	t5,2
    1824:	b5ed                	j	170e <memset+0x82>
    1826:	4f11                	li	t5,4
    1828:	b5dd                	j	170e <memset+0x82>
    182a:	4f15                	li	t5,5
    182c:	b5cd                	j	170e <memset+0x82>
    182e:	4f19                	li	t5,6
    1830:	bdf9                	j	170e <memset+0x82>

0000000000001832 <strcmp>:

int strcmp(const char *l, const char *r)
{
    for (; *l == *r && *l; l++, r++)
    1832:	00054783          	lbu	a5,0(a0)
    1836:	0005c703          	lbu	a4,0(a1)
    183a:	00e79863          	bne	a5,a4,184a <strcmp+0x18>
    183e:	0505                	addi	a0,a0,1
    1840:	0585                	addi	a1,a1,1
    1842:	fbe5                	bnez	a5,1832 <strcmp>
    1844:	4501                	li	a0,0
        ;
    return *(unsigned char *)l - *(unsigned char *)r;
}
    1846:	9d19                	subw	a0,a0,a4
    1848:	8082                	ret
    return *(unsigned char *)l - *(unsigned char *)r;
    184a:	0007851b          	sext.w	a0,a5
    184e:	bfe5                	j	1846 <strcmp+0x14>

0000000000001850 <strncmp>:

int strncmp(const char *_l, const char *_r, size_t n)
{
    const unsigned char *l = (void *)_l, *r = (void *)_r;
    if (!n--)
    1850:	ce05                	beqz	a2,1888 <strncmp+0x38>
        return 0;
    for (; *l && *r && n && *l == *r; l++, r++, n--)
    1852:	00054703          	lbu	a4,0(a0)
    1856:	0005c783          	lbu	a5,0(a1)
    185a:	cb0d                	beqz	a4,188c <strncmp+0x3c>
    if (!n--)
    185c:	167d                	addi	a2,a2,-1
    185e:	00c506b3          	add	a3,a0,a2
    1862:	a819                	j	1878 <strncmp+0x28>
    for (; *l && *r && n && *l == *r; l++, r++, n--)
    1864:	00a68e63          	beq	a3,a0,1880 <strncmp+0x30>
    1868:	0505                	addi	a0,a0,1
    186a:	00e79b63          	bne	a5,a4,1880 <strncmp+0x30>
    186e:	00054703          	lbu	a4,0(a0)
        ;
    return *l - *r;
    1872:	0005c783          	lbu	a5,0(a1)
    for (; *l && *r && n && *l == *r; l++, r++, n--)
    1876:	cb19                	beqz	a4,188c <strncmp+0x3c>
    1878:	0005c783          	lbu	a5,0(a1)
    187c:	0585                	addi	a1,a1,1
    187e:	f3fd                	bnez	a5,1864 <strncmp+0x14>
    return *l - *r;
    1880:	0007051b          	sext.w	a0,a4
    1884:	9d1d                	subw	a0,a0,a5
    1886:	8082                	ret
        return 0;
    1888:	4501                	li	a0,0
}
    188a:	8082                	ret
    188c:	4501                	li	a0,0
    return *l - *r;
    188e:	9d1d                	subw	a0,a0,a5
    1890:	8082                	ret

0000000000001892 <strlen>:
size_t strlen(const char *s)
{
    const char *a = s;
    typedef size_t __attribute__((__may_alias__)) word;
    const word *w;
    for (; (uintptr_t)s % SS; s++)
    1892:	00757793          	andi	a5,a0,7
    1896:	cf89                	beqz	a5,18b0 <strlen+0x1e>
    1898:	87aa                	mv	a5,a0
    189a:	a029                	j	18a4 <strlen+0x12>
    189c:	0785                	addi	a5,a5,1
    189e:	0077f713          	andi	a4,a5,7
    18a2:	cb01                	beqz	a4,18b2 <strlen+0x20>
        if (!*s)
    18a4:	0007c703          	lbu	a4,0(a5)
    18a8:	fb75                	bnez	a4,189c <strlen+0xa>
    for (w = (const void *)s; !HASZERO(*w); w++)
        ;
    s = (const void *)w;
    for (; *s; s++)
        ;
    return s - a;
    18aa:	40a78533          	sub	a0,a5,a0
}
    18ae:	8082                	ret
    for (; (uintptr_t)s % SS; s++)
    18b0:	87aa                	mv	a5,a0
    for (w = (const void *)s; !HASZERO(*w); w++)
    18b2:	6394                	ld	a3,0(a5)
    18b4:	00000597          	auipc	a1,0x0
    18b8:	6d45b583          	ld	a1,1748(a1) # 1f88 <__clone+0xde>
    18bc:	00000617          	auipc	a2,0x0
    18c0:	6d463603          	ld	a2,1748(a2) # 1f90 <__clone+0xe6>
    18c4:	a019                	j	18ca <strlen+0x38>
    18c6:	6794                	ld	a3,8(a5)
    18c8:	07a1                	addi	a5,a5,8
    18ca:	00b68733          	add	a4,a3,a1
    18ce:	fff6c693          	not	a3,a3
    18d2:	8f75                	and	a4,a4,a3
    18d4:	8f71                	and	a4,a4,a2
    18d6:	db65                	beqz	a4,18c6 <strlen+0x34>
    for (; *s; s++)
    18d8:	0007c703          	lbu	a4,0(a5)
    18dc:	d779                	beqz	a4,18aa <strlen+0x18>
    18de:	0017c703          	lbu	a4,1(a5)
    18e2:	0785                	addi	a5,a5,1
    18e4:	d379                	beqz	a4,18aa <strlen+0x18>
    18e6:	0017c703          	lbu	a4,1(a5)
    18ea:	0785                	addi	a5,a5,1
    18ec:	fb6d                	bnez	a4,18de <strlen+0x4c>
    18ee:	bf75                	j	18aa <strlen+0x18>

00000000000018f0 <memchr>:

void *memchr(const void *src, int c, size_t n)
{
    const unsigned char *s = src;
    c = (unsigned char)c;
    for (; ((uintptr_t)s & ALIGN) && n && *s != c; s++, n--)
    18f0:	00757713          	andi	a4,a0,7
{
    18f4:	87aa                	mv	a5,a0
    c = (unsigned char)c;
    18f6:	0ff5f593          	andi	a1,a1,255
    for (; ((uintptr_t)s & ALIGN) && n && *s != c; s++, n--)
    18fa:	cb19                	beqz	a4,1910 <memchr+0x20>
    18fc:	ce25                	beqz	a2,1974 <memchr+0x84>
    18fe:	0007c703          	lbu	a4,0(a5)
    1902:	04b70e63          	beq	a4,a1,195e <memchr+0x6e>
    1906:	0785                	addi	a5,a5,1
    1908:	0077f713          	andi	a4,a5,7
    190c:	167d                	addi	a2,a2,-1
    190e:	f77d                	bnez	a4,18fc <memchr+0xc>
            ;
        s = (const void *)w;
    }
    for (; n && *s != c; s++, n--)
        ;
    return n ? (void *)s : 0;
    1910:	4501                	li	a0,0
    if (n && *s != c)
    1912:	c235                	beqz	a2,1976 <memchr+0x86>
    1914:	0007c703          	lbu	a4,0(a5)
    1918:	04b70363          	beq	a4,a1,195e <memchr+0x6e>
        size_t k = ONES * c;
    191c:	00000517          	auipc	a0,0x0
    1920:	67c53503          	ld	a0,1660(a0) # 1f98 <__clone+0xee>
        for (w = (const void *)s; n >= SS && !HASZERO(*w ^ k); w++, n -= SS)
    1924:	471d                	li	a4,7
        size_t k = ONES * c;
    1926:	02a58533          	mul	a0,a1,a0
        for (w = (const void *)s; n >= SS && !HASZERO(*w ^ k); w++, n -= SS)
    192a:	02c77a63          	bgeu	a4,a2,195e <memchr+0x6e>
    192e:	00000897          	auipc	a7,0x0
    1932:	65a8b883          	ld	a7,1626(a7) # 1f88 <__clone+0xde>
    1936:	00000817          	auipc	a6,0x0
    193a:	65a83803          	ld	a6,1626(a6) # 1f90 <__clone+0xe6>
    193e:	431d                	li	t1,7
    1940:	a029                	j	194a <memchr+0x5a>
    1942:	1661                	addi	a2,a2,-8
    1944:	07a1                	addi	a5,a5,8
    1946:	02c37963          	bgeu	t1,a2,1978 <memchr+0x88>
    194a:	6398                	ld	a4,0(a5)
    194c:	8f29                	xor	a4,a4,a0
    194e:	011706b3          	add	a3,a4,a7
    1952:	fff74713          	not	a4,a4
    1956:	8f75                	and	a4,a4,a3
    1958:	01077733          	and	a4,a4,a6
    195c:	d37d                	beqz	a4,1942 <memchr+0x52>
    195e:	853e                	mv	a0,a5
    1960:	97b2                	add	a5,a5,a2
    1962:	a021                	j	196a <memchr+0x7a>
    for (; n && *s != c; s++, n--)
    1964:	0505                	addi	a0,a0,1
    1966:	00f50763          	beq	a0,a5,1974 <memchr+0x84>
    196a:	00054703          	lbu	a4,0(a0)
    196e:	feb71be3          	bne	a4,a1,1964 <memchr+0x74>
    1972:	8082                	ret
    return n ? (void *)s : 0;
    1974:	4501                	li	a0,0
}
    1976:	8082                	ret
    return n ? (void *)s : 0;
    1978:	4501                	li	a0,0
    for (; n && *s != c; s++, n--)
    197a:	f275                	bnez	a2,195e <memchr+0x6e>
}
    197c:	8082                	ret

000000000000197e <strnlen>:

size_t strnlen(const char *s, size_t n)
{
    197e:	1101                	addi	sp,sp,-32
    1980:	e822                	sd	s0,16(sp)
    const char *p = memchr(s, 0, n);
    1982:	862e                	mv	a2,a1
{
    1984:	842e                	mv	s0,a1
    const char *p = memchr(s, 0, n);
    1986:	4581                	li	a1,0
{
    1988:	e426                	sd	s1,8(sp)
    198a:	ec06                	sd	ra,24(sp)
    198c:	84aa                	mv	s1,a0
    const char *p = memchr(s, 0, n);
    198e:	f63ff0ef          	jal	ra,18f0 <memchr>
    return p ? p - s : n;
    1992:	c519                	beqz	a0,19a0 <strnlen+0x22>
}
    1994:	60e2                	ld	ra,24(sp)
    1996:	6442                	ld	s0,16(sp)
    return p ? p - s : n;
    1998:	8d05                	sub	a0,a0,s1
}
    199a:	64a2                	ld	s1,8(sp)
    199c:	6105                	addi	sp,sp,32
    199e:	8082                	ret
    19a0:	60e2                	ld	ra,24(sp)
    return p ? p - s : n;
    19a2:	8522                	mv	a0,s0
}
    19a4:	6442                	ld	s0,16(sp)
    19a6:	64a2                	ld	s1,8(sp)
    19a8:	6105                	addi	sp,sp,32
    19aa:	8082                	ret

00000000000019ac <strcpy>:
char *strcpy(char *restrict d, const char *s)
{
    typedef size_t __attribute__((__may_alias__)) word;
    word *wd;
    const word *ws;
    if ((uintptr_t)s % SS == (uintptr_t)d % SS)
    19ac:	00b547b3          	xor	a5,a0,a1
    19b0:	8b9d                	andi	a5,a5,7
    19b2:	eb95                	bnez	a5,19e6 <strcpy+0x3a>
    {
        for (; (uintptr_t)s % SS; s++, d++)
    19b4:	0075f793          	andi	a5,a1,7
    19b8:	e7b1                	bnez	a5,1a04 <strcpy+0x58>
            if (!(*d = *s))
                return d;
        wd = (void *)d;
        ws = (const void *)s;
        for (; !HASZERO(*ws); *wd++ = *ws++)
    19ba:	6198                	ld	a4,0(a1)
    19bc:	00000617          	auipc	a2,0x0
    19c0:	5cc63603          	ld	a2,1484(a2) # 1f88 <__clone+0xde>
    19c4:	00000817          	auipc	a6,0x0
    19c8:	5cc83803          	ld	a6,1484(a6) # 1f90 <__clone+0xe6>
    19cc:	a029                	j	19d6 <strcpy+0x2a>
    19ce:	e118                	sd	a4,0(a0)
    19d0:	6598                	ld	a4,8(a1)
    19d2:	05a1                	addi	a1,a1,8
    19d4:	0521                	addi	a0,a0,8
    19d6:	00c707b3          	add	a5,a4,a2
    19da:	fff74693          	not	a3,a4
    19de:	8ff5                	and	a5,a5,a3
    19e0:	0107f7b3          	and	a5,a5,a6
    19e4:	d7ed                	beqz	a5,19ce <strcpy+0x22>
            ;
        d = (void *)wd;
        s = (const void *)ws;
    }
    for (; (*d = *s); s++, d++)
    19e6:	0005c783          	lbu	a5,0(a1)
    19ea:	00f50023          	sb	a5,0(a0)
    19ee:	c785                	beqz	a5,1a16 <strcpy+0x6a>
    19f0:	0015c783          	lbu	a5,1(a1)
    19f4:	0505                	addi	a0,a0,1
    19f6:	0585                	addi	a1,a1,1
    19f8:	00f50023          	sb	a5,0(a0)
    19fc:	fbf5                	bnez	a5,19f0 <strcpy+0x44>
        ;
    return d;
}
    19fe:	8082                	ret
        for (; (uintptr_t)s % SS; s++, d++)
    1a00:	0505                	addi	a0,a0,1
    1a02:	df45                	beqz	a4,19ba <strcpy+0xe>
            if (!(*d = *s))
    1a04:	0005c783          	lbu	a5,0(a1)
        for (; (uintptr_t)s % SS; s++, d++)
    1a08:	0585                	addi	a1,a1,1
    1a0a:	0075f713          	andi	a4,a1,7
            if (!(*d = *s))
    1a0e:	00f50023          	sb	a5,0(a0)
    1a12:	f7fd                	bnez	a5,1a00 <strcpy+0x54>
}
    1a14:	8082                	ret
    1a16:	8082                	ret

0000000000001a18 <strncpy>:
char *strncpy(char *restrict d, const char *s, size_t n)
{
    typedef size_t __attribute__((__may_alias__)) word;
    word *wd;
    const word *ws;
    if (((uintptr_t)s & ALIGN) == ((uintptr_t)d & ALIGN))
    1a18:	00b547b3          	xor	a5,a0,a1
    1a1c:	8b9d                	andi	a5,a5,7
    1a1e:	1a079863          	bnez	a5,1bce <strncpy+0x1b6>
    {
        for (; ((uintptr_t)s & ALIGN) && n && (*d = *s); n--, s++, d++)
    1a22:	0075f793          	andi	a5,a1,7
    1a26:	16078463          	beqz	a5,1b8e <strncpy+0x176>
    1a2a:	ea01                	bnez	a2,1a3a <strncpy+0x22>
    1a2c:	a421                	j	1c34 <strncpy+0x21c>
    1a2e:	167d                	addi	a2,a2,-1
    1a30:	0505                	addi	a0,a0,1
    1a32:	14070e63          	beqz	a4,1b8e <strncpy+0x176>
    1a36:	1a060863          	beqz	a2,1be6 <strncpy+0x1ce>
    1a3a:	0005c783          	lbu	a5,0(a1)
    1a3e:	0585                	addi	a1,a1,1
    1a40:	0075f713          	andi	a4,a1,7
    1a44:	00f50023          	sb	a5,0(a0)
    1a48:	f3fd                	bnez	a5,1a2e <strncpy+0x16>
    1a4a:	4805                	li	a6,1
    1a4c:	1a061863          	bnez	a2,1bfc <strncpy+0x1e4>
    1a50:	40a007b3          	neg	a5,a0
    1a54:	8b9d                	andi	a5,a5,7
    1a56:	4681                	li	a3,0
    1a58:	18061a63          	bnez	a2,1bec <strncpy+0x1d4>
    1a5c:	00778713          	addi	a4,a5,7
    1a60:	45ad                	li	a1,11
    1a62:	18b76363          	bltu	a4,a1,1be8 <strncpy+0x1d0>
    1a66:	1ae6eb63          	bltu	a3,a4,1c1c <strncpy+0x204>
    1a6a:	1a078363          	beqz	a5,1c10 <strncpy+0x1f8>
    for (int i = 0; i < n; ++i, *(p++) = c)
    1a6e:	00050023          	sb	zero,0(a0)
    1a72:	4685                	li	a3,1
    1a74:	00150713          	addi	a4,a0,1
    1a78:	18d78f63          	beq	a5,a3,1c16 <strncpy+0x1fe>
    1a7c:	000500a3          	sb	zero,1(a0)
    1a80:	4689                	li	a3,2
    1a82:	00250713          	addi	a4,a0,2
    1a86:	18d78e63          	beq	a5,a3,1c22 <strncpy+0x20a>
    1a8a:	00050123          	sb	zero,2(a0)
    1a8e:	468d                	li	a3,3
    1a90:	00350713          	addi	a4,a0,3
    1a94:	16d78c63          	beq	a5,a3,1c0c <strncpy+0x1f4>
    1a98:	000501a3          	sb	zero,3(a0)
    1a9c:	4691                	li	a3,4
    1a9e:	00450713          	addi	a4,a0,4
    1aa2:	18d78263          	beq	a5,a3,1c26 <strncpy+0x20e>
    1aa6:	00050223          	sb	zero,4(a0)
    1aaa:	4695                	li	a3,5
    1aac:	00550713          	addi	a4,a0,5
    1ab0:	16d78d63          	beq	a5,a3,1c2a <strncpy+0x212>
    1ab4:	000502a3          	sb	zero,5(a0)
    1ab8:	469d                	li	a3,7
    1aba:	00650713          	addi	a4,a0,6
    1abe:	16d79863          	bne	a5,a3,1c2e <strncpy+0x216>
    1ac2:	00750713          	addi	a4,a0,7
    1ac6:	00050323          	sb	zero,6(a0)
    1aca:	40f80833          	sub	a6,a6,a5
    1ace:	ff887593          	andi	a1,a6,-8
    1ad2:	97aa                	add	a5,a5,a0
    1ad4:	95be                	add	a1,a1,a5
    1ad6:	0007b023          	sd	zero,0(a5)
    1ada:	07a1                	addi	a5,a5,8
    1adc:	feb79de3          	bne	a5,a1,1ad6 <strncpy+0xbe>
    1ae0:	ff887593          	andi	a1,a6,-8
    1ae4:	9ead                	addw	a3,a3,a1
    1ae6:	00b707b3          	add	a5,a4,a1
    1aea:	12b80863          	beq	a6,a1,1c1a <strncpy+0x202>
    1aee:	00078023          	sb	zero,0(a5)
    1af2:	0016871b          	addiw	a4,a3,1
    1af6:	0ec77863          	bgeu	a4,a2,1be6 <strncpy+0x1ce>
    1afa:	000780a3          	sb	zero,1(a5)
    1afe:	0026871b          	addiw	a4,a3,2
    1b02:	0ec77263          	bgeu	a4,a2,1be6 <strncpy+0x1ce>
    1b06:	00078123          	sb	zero,2(a5)
    1b0a:	0036871b          	addiw	a4,a3,3
    1b0e:	0cc77c63          	bgeu	a4,a2,1be6 <strncpy+0x1ce>
    1b12:	000781a3          	sb	zero,3(a5)
    1b16:	0046871b          	addiw	a4,a3,4
    1b1a:	0cc77663          	bgeu	a4,a2,1be6 <strncpy+0x1ce>
    1b1e:	00078223          	sb	zero,4(a5)
    1b22:	0056871b          	addiw	a4,a3,5
    1b26:	0cc77063          	bgeu	a4,a2,1be6 <strncpy+0x1ce>
    1b2a:	000782a3          	sb	zero,5(a5)
    1b2e:	0066871b          	addiw	a4,a3,6
    1b32:	0ac77a63          	bgeu	a4,a2,1be6 <strncpy+0x1ce>
    1b36:	00078323          	sb	zero,6(a5)
    1b3a:	0076871b          	addiw	a4,a3,7
    1b3e:	0ac77463          	bgeu	a4,a2,1be6 <strncpy+0x1ce>
    1b42:	000783a3          	sb	zero,7(a5)
    1b46:	0086871b          	addiw	a4,a3,8
    1b4a:	08c77e63          	bgeu	a4,a2,1be6 <strncpy+0x1ce>
    1b4e:	00078423          	sb	zero,8(a5)
    1b52:	0096871b          	addiw	a4,a3,9
    1b56:	08c77863          	bgeu	a4,a2,1be6 <strncpy+0x1ce>
    1b5a:	000784a3          	sb	zero,9(a5)
    1b5e:	00a6871b          	addiw	a4,a3,10
    1b62:	08c77263          	bgeu	a4,a2,1be6 <strncpy+0x1ce>
    1b66:	00078523          	sb	zero,10(a5)
    1b6a:	00b6871b          	addiw	a4,a3,11
    1b6e:	06c77c63          	bgeu	a4,a2,1be6 <strncpy+0x1ce>
    1b72:	000785a3          	sb	zero,11(a5)
    1b76:	00c6871b          	addiw	a4,a3,12
    1b7a:	06c77663          	bgeu	a4,a2,1be6 <strncpy+0x1ce>
    1b7e:	00078623          	sb	zero,12(a5)
    1b82:	26b5                	addiw	a3,a3,13
    1b84:	06c6f163          	bgeu	a3,a2,1be6 <strncpy+0x1ce>
    1b88:	000786a3          	sb	zero,13(a5)
    1b8c:	8082                	ret
            ;
        if (!n || !*s)
    1b8e:	c645                	beqz	a2,1c36 <strncpy+0x21e>
    1b90:	0005c783          	lbu	a5,0(a1)
    1b94:	ea078be3          	beqz	a5,1a4a <strncpy+0x32>
            goto tail;
        wd = (void *)d;
        ws = (const void *)s;
        for (; n >= sizeof(size_t) && !HASZERO(*ws); n -= sizeof(size_t), ws++, wd++)
    1b98:	479d                	li	a5,7
    1b9a:	02c7ff63          	bgeu	a5,a2,1bd8 <strncpy+0x1c0>
    1b9e:	00000897          	auipc	a7,0x0
    1ba2:	3ea8b883          	ld	a7,1002(a7) # 1f88 <__clone+0xde>
    1ba6:	00000817          	auipc	a6,0x0
    1baa:	3ea83803          	ld	a6,1002(a6) # 1f90 <__clone+0xe6>
    1bae:	431d                	li	t1,7
    1bb0:	6198                	ld	a4,0(a1)
    1bb2:	011707b3          	add	a5,a4,a7
    1bb6:	fff74693          	not	a3,a4
    1bba:	8ff5                	and	a5,a5,a3
    1bbc:	0107f7b3          	and	a5,a5,a6
    1bc0:	ef81                	bnez	a5,1bd8 <strncpy+0x1c0>
            *wd = *ws;
    1bc2:	e118                	sd	a4,0(a0)
        for (; n >= sizeof(size_t) && !HASZERO(*ws); n -= sizeof(size_t), ws++, wd++)
    1bc4:	1661                	addi	a2,a2,-8
    1bc6:	05a1                	addi	a1,a1,8
    1bc8:	0521                	addi	a0,a0,8
    1bca:	fec363e3          	bltu	t1,a2,1bb0 <strncpy+0x198>
        d = (void *)wd;
        s = (const void *)ws;
    }
    for (; n && (*d = *s); n--, s++, d++)
    1bce:	e609                	bnez	a2,1bd8 <strncpy+0x1c0>
    1bd0:	a08d                	j	1c32 <strncpy+0x21a>
    1bd2:	167d                	addi	a2,a2,-1
    1bd4:	0505                	addi	a0,a0,1
    1bd6:	ca01                	beqz	a2,1be6 <strncpy+0x1ce>
    1bd8:	0005c783          	lbu	a5,0(a1)
    1bdc:	0585                	addi	a1,a1,1
    1bde:	00f50023          	sb	a5,0(a0)
    1be2:	fbe5                	bnez	a5,1bd2 <strncpy+0x1ba>
        ;
tail:
    1be4:	b59d                	j	1a4a <strncpy+0x32>
    memset(d, 0, n);
    return d;
}
    1be6:	8082                	ret
    1be8:	472d                	li	a4,11
    1bea:	bdb5                	j	1a66 <strncpy+0x4e>
    1bec:	00778713          	addi	a4,a5,7
    1bf0:	45ad                	li	a1,11
    1bf2:	fff60693          	addi	a3,a2,-1
    1bf6:	e6b778e3          	bgeu	a4,a1,1a66 <strncpy+0x4e>
    1bfa:	b7fd                	j	1be8 <strncpy+0x1d0>
    1bfc:	40a007b3          	neg	a5,a0
    1c00:	8832                	mv	a6,a2
    1c02:	8b9d                	andi	a5,a5,7
    1c04:	4681                	li	a3,0
    1c06:	e4060be3          	beqz	a2,1a5c <strncpy+0x44>
    1c0a:	b7cd                	j	1bec <strncpy+0x1d4>
    for (int i = 0; i < n; ++i, *(p++) = c)
    1c0c:	468d                	li	a3,3
    1c0e:	bd75                	j	1aca <strncpy+0xb2>
    1c10:	872a                	mv	a4,a0
    1c12:	4681                	li	a3,0
    1c14:	bd5d                	j	1aca <strncpy+0xb2>
    1c16:	4685                	li	a3,1
    1c18:	bd4d                	j	1aca <strncpy+0xb2>
    1c1a:	8082                	ret
    1c1c:	87aa                	mv	a5,a0
    1c1e:	4681                	li	a3,0
    1c20:	b5f9                	j	1aee <strncpy+0xd6>
    1c22:	4689                	li	a3,2
    1c24:	b55d                	j	1aca <strncpy+0xb2>
    1c26:	4691                	li	a3,4
    1c28:	b54d                	j	1aca <strncpy+0xb2>
    1c2a:	4695                	li	a3,5
    1c2c:	bd79                	j	1aca <strncpy+0xb2>
    1c2e:	4699                	li	a3,6
    1c30:	bd69                	j	1aca <strncpy+0xb2>
    1c32:	8082                	ret
    1c34:	8082                	ret
    1c36:	8082                	ret

0000000000001c38 <open>:
#include <unistd.h>

#include "syscall.h"

int open(const char *path, int flags)
{
    1c38:	87aa                	mv	a5,a0
    1c3a:	862e                	mv	a2,a1
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
}

static inline long __syscall4(long n, long a, long b, long c, long d)
{
    register long a7 __asm__("a7") = n;
    1c3c:	03800893          	li	a7,56
    register long a0 __asm__("a0") = a;
    1c40:	f9c00513          	li	a0,-100
    register long a1 __asm__("a1") = b;
    1c44:	85be                	mv	a1,a5
    register long a2 __asm__("a2") = c;
    register long a3 __asm__("a3") = d;
    1c46:	4689                	li	a3,2
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3))
    1c48:	00000073          	ecall
    return syscall(SYS_openat, AT_FDCWD, path, flags, O_RDWR);
}
    1c4c:	2501                	sext.w	a0,a0
    1c4e:	8082                	ret

0000000000001c50 <openat>:
    register long a7 __asm__("a7") = n;
    1c50:	03800893          	li	a7,56
    register long a3 __asm__("a3") = d;
    1c54:	18000693          	li	a3,384
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3))
    1c58:	00000073          	ecall

int openat(int dirfd,const char *path, int flags)
{
    return syscall(SYS_openat, dirfd, path, flags, 0600);
}
    1c5c:	2501                	sext.w	a0,a0
    1c5e:	8082                	ret

0000000000001c60 <close>:
    register long a7 __asm__("a7") = n;
    1c60:	03900893          	li	a7,57
    __asm_syscall("r"(a7), "0"(a0))
    1c64:	00000073          	ecall

int close(int fd)
{
    return syscall(SYS_close, fd);
}
    1c68:	2501                	sext.w	a0,a0
    1c6a:	8082                	ret

0000000000001c6c <read>:
    register long a7 __asm__("a7") = n;
    1c6c:	03f00893          	li	a7,63
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1c70:	00000073          	ecall

ssize_t read(int fd, void *buf, size_t len)
{
    return syscall(SYS_read, fd, buf, len);
}
    1c74:	8082                	ret

0000000000001c76 <write>:
    register long a7 __asm__("a7") = n;
    1c76:	04000893          	li	a7,64
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1c7a:	00000073          	ecall

ssize_t write(int fd, const void *buf, size_t len)
{
    return syscall(SYS_write, fd, buf, len);
}
    1c7e:	8082                	ret

0000000000001c80 <getpid>:
    register long a7 __asm__("a7") = n;
    1c80:	0ac00893          	li	a7,172
    __asm_syscall("r"(a7))
    1c84:	00000073          	ecall

pid_t getpid(void)
{
    return syscall(SYS_getpid);
}
    1c88:	2501                	sext.w	a0,a0
    1c8a:	8082                	ret

0000000000001c8c <getppid>:
    register long a7 __asm__("a7") = n;
    1c8c:	0ad00893          	li	a7,173
    __asm_syscall("r"(a7))
    1c90:	00000073          	ecall

pid_t getppid(void)
{
    return syscall(SYS_getppid);
}
    1c94:	2501                	sext.w	a0,a0
    1c96:	8082                	ret

0000000000001c98 <sched_yield>:
    register long a7 __asm__("a7") = n;
    1c98:	07c00893          	li	a7,124
    __asm_syscall("r"(a7))
    1c9c:	00000073          	ecall

int sched_yield(void)
{
    return syscall(SYS_sched_yield);
}
    1ca0:	2501                	sext.w	a0,a0
    1ca2:	8082                	ret

0000000000001ca4 <fork>:
    register long a7 __asm__("a7") = n;
    1ca4:	0dc00893          	li	a7,220
    register long a0 __asm__("a0") = a;
    1ca8:	4545                	li	a0,17
    register long a1 __asm__("a1") = b;
    1caa:	4581                	li	a1,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1cac:	00000073          	ecall

pid_t fork(void)
{
    return syscall(SYS_clone, SIGCHLD, 0);
}
    1cb0:	2501                	sext.w	a0,a0
    1cb2:	8082                	ret

0000000000001cb4 <clone>:

pid_t clone(int (*fn)(void *arg), void *arg, void *stack, size_t stack_size, unsigned long flags)
{
    1cb4:	85b2                	mv	a1,a2
    1cb6:	863a                	mv	a2,a4
    if (stack)
    1cb8:	c191                	beqz	a1,1cbc <clone+0x8>
	stack += stack_size;
    1cba:	95b6                	add	a1,a1,a3

    return __clone(fn, stack, flags, NULL, NULL, NULL);
    1cbc:	4781                	li	a5,0
    1cbe:	4701                	li	a4,0
    1cc0:	4681                	li	a3,0
    1cc2:	2601                	sext.w	a2,a2
    1cc4:	a2dd                	j	1eaa <__clone>

0000000000001cc6 <exit>:
    register long a7 __asm__("a7") = n;
    1cc6:	05d00893          	li	a7,93
    __asm_syscall("r"(a7), "0"(a0))
    1cca:	00000073          	ecall
    //return syscall(SYS_clone, fn, stack, flags, NULL, NULL, NULL);
}
void exit(int code)
{
    syscall(SYS_exit, code);
}
    1cce:	8082                	ret

0000000000001cd0 <waitpid>:
    register long a7 __asm__("a7") = n;
    1cd0:	10400893          	li	a7,260
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1cd4:	00000073          	ecall

int waitpid(int pid, int *code, int options)
{
    return syscall(SYS_wait4, pid, code, options);
}
    1cd8:	2501                	sext.w	a0,a0
    1cda:	8082                	ret

0000000000001cdc <exec>:
    register long a7 __asm__("a7") = n;
    1cdc:	0dd00893          	li	a7,221
    __asm_syscall("r"(a7), "0"(a0))
    1ce0:	00000073          	ecall

int exec(char *name)
{
    return syscall(SYS_execve, name);
}
    1ce4:	2501                	sext.w	a0,a0
    1ce6:	8082                	ret

0000000000001ce8 <execve>:
    register long a7 __asm__("a7") = n;
    1ce8:	0dd00893          	li	a7,221
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1cec:	00000073          	ecall

int execve(const char *name, char *const argv[], char *const argp[])
{
    return syscall(SYS_execve, name, argv, argp);
}
    1cf0:	2501                	sext.w	a0,a0
    1cf2:	8082                	ret

0000000000001cf4 <times>:
    register long a7 __asm__("a7") = n;
    1cf4:	09900893          	li	a7,153
    __asm_syscall("r"(a7), "0"(a0))
    1cf8:	00000073          	ecall

int times(void *mytimes)
{
	return syscall(SYS_times, mytimes);
}
    1cfc:	2501                	sext.w	a0,a0
    1cfe:	8082                	ret

0000000000001d00 <get_time>:

int64 get_time()
{
    1d00:	1141                	addi	sp,sp,-16
    register long a7 __asm__("a7") = n;
    1d02:	0a900893          	li	a7,169
    register long a0 __asm__("a0") = a;
    1d06:	850a                	mv	a0,sp
    register long a1 __asm__("a1") = b;
    1d08:	4581                	li	a1,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1d0a:	00000073          	ecall
    TimeVal time;
    int err = sys_get_time(&time, 0);
    if (err == 0)
    1d0e:	2501                	sext.w	a0,a0
    1d10:	ed09                	bnez	a0,1d2a <get_time+0x2a>
    {
        return ((time.sec & 0xffff) * 1000 + time.usec / 1000);
    1d12:	67a2                	ld	a5,8(sp)
    1d14:	3e800713          	li	a4,1000
    1d18:	00015503          	lhu	a0,0(sp)
    1d1c:	02e7d7b3          	divu	a5,a5,a4
    1d20:	02e50533          	mul	a0,a0,a4
    1d24:	953e                	add	a0,a0,a5
    }
    else
    {
        return -1;
    }
}
    1d26:	0141                	addi	sp,sp,16
    1d28:	8082                	ret
        return -1;
    1d2a:	557d                	li	a0,-1
    1d2c:	bfed                	j	1d26 <get_time+0x26>

0000000000001d2e <sys_get_time>:
    register long a7 __asm__("a7") = n;
    1d2e:	0a900893          	li	a7,169
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1d32:	00000073          	ecall

int sys_get_time(TimeVal *ts, int tz)
{
    return syscall(SYS_gettimeofday, ts, tz);
}
    1d36:	2501                	sext.w	a0,a0
    1d38:	8082                	ret

0000000000001d3a <time>:
    register long a7 __asm__("a7") = n;
    1d3a:	42600893          	li	a7,1062
    __asm_syscall("r"(a7), "0"(a0))
    1d3e:	00000073          	ecall

int time(unsigned long *tloc)
{
    return syscall(SYS_time, tloc);
}
    1d42:	2501                	sext.w	a0,a0
    1d44:	8082                	ret

0000000000001d46 <sleep>:

int sleep(unsigned long long time)
{
    1d46:	1141                	addi	sp,sp,-16
    TimeVal tv = {.sec = time, .usec = 0};
    1d48:	e02a                	sd	a0,0(sp)
    register long a0 __asm__("a0") = a;
    1d4a:	850a                	mv	a0,sp
    1d4c:	e402                	sd	zero,8(sp)
    register long a7 __asm__("a7") = n;
    1d4e:	06500893          	li	a7,101
    register long a1 __asm__("a1") = b;
    1d52:	85aa                	mv	a1,a0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1d54:	00000073          	ecall
    if (syscall(SYS_nanosleep, &tv, &tv)) return tv.sec;
    1d58:	e501                	bnez	a0,1d60 <sleep+0x1a>
    return 0;
    1d5a:	4501                	li	a0,0
}
    1d5c:	0141                	addi	sp,sp,16
    1d5e:	8082                	ret
    if (syscall(SYS_nanosleep, &tv, &tv)) return tv.sec;
    1d60:	4502                	lw	a0,0(sp)
}
    1d62:	0141                	addi	sp,sp,16
    1d64:	8082                	ret

0000000000001d66 <set_priority>:
    register long a7 __asm__("a7") = n;
    1d66:	08c00893          	li	a7,140
    __asm_syscall("r"(a7), "0"(a0))
    1d6a:	00000073          	ecall

int set_priority(int prio)
{
    return syscall(SYS_setpriority, prio);
}
    1d6e:	2501                	sext.w	a0,a0
    1d70:	8082                	ret

0000000000001d72 <mmap>:
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4))
}

static inline long __syscall6(long n, long a, long b, long c, long d, long e, long f)
{
    register long a7 __asm__("a7") = n;
    1d72:	0de00893          	li	a7,222
    register long a1 __asm__("a1") = b;
    register long a2 __asm__("a2") = c;
    register long a3 __asm__("a3") = d;
    register long a4 __asm__("a4") = e;
    register long a5 __asm__("a5") = f;
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4), "r"(a5))
    1d76:	00000073          	ecall

void *mmap(void *start, size_t len, int prot, int flags, int fd, off_t off)
{
    return syscall(SYS_mmap, start, len, prot, flags, fd, off);
}
    1d7a:	8082                	ret

0000000000001d7c <munmap>:
    register long a7 __asm__("a7") = n;
    1d7c:	0d700893          	li	a7,215
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1d80:	00000073          	ecall

int munmap(void *start, size_t len)
{
    return syscall(SYS_munmap, start, len);
}
    1d84:	2501                	sext.w	a0,a0
    1d86:	8082                	ret

0000000000001d88 <wait>:

int wait(int *code)
{
    1d88:	85aa                	mv	a1,a0
    register long a7 __asm__("a7") = n;
    1d8a:	10400893          	li	a7,260
    register long a0 __asm__("a0") = a;
    1d8e:	557d                	li	a0,-1
    register long a2 __asm__("a2") = c;
    1d90:	4601                	li	a2,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1d92:	00000073          	ecall
    return waitpid(-1, code, 0);
}
    1d96:	2501                	sext.w	a0,a0
    1d98:	8082                	ret

0000000000001d9a <spawn>:
    register long a7 __asm__("a7") = n;
    1d9a:	19000893          	li	a7,400
    __asm_syscall("r"(a7), "0"(a0))
    1d9e:	00000073          	ecall

int spawn(char *file)
{
    return syscall(SYS_spawn, file);
}
    1da2:	2501                	sext.w	a0,a0
    1da4:	8082                	ret

0000000000001da6 <mailread>:
    register long a7 __asm__("a7") = n;
    1da6:	19100893          	li	a7,401
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1daa:	00000073          	ecall

int mailread(void *buf, int len)
{
    return syscall(SYS_mailread, buf, len);
}
    1dae:	2501                	sext.w	a0,a0
    1db0:	8082                	ret

0000000000001db2 <mailwrite>:
    register long a7 __asm__("a7") = n;
    1db2:	19200893          	li	a7,402
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1db6:	00000073          	ecall

int mailwrite(int pid, void *buf, int len)
{
    return syscall(SYS_mailwrite, pid, buf, len);
}
    1dba:	2501                	sext.w	a0,a0
    1dbc:	8082                	ret

0000000000001dbe <fstat>:
    register long a7 __asm__("a7") = n;
    1dbe:	05000893          	li	a7,80
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1dc2:	00000073          	ecall

int fstat(int fd, struct kstat *st)
{
    return syscall(SYS_fstat, fd, st);
}
    1dc6:	2501                	sext.w	a0,a0
    1dc8:	8082                	ret

0000000000001dca <sys_linkat>:
    register long a4 __asm__("a4") = e;
    1dca:	1702                	slli	a4,a4,0x20
    register long a7 __asm__("a7") = n;
    1dcc:	02500893          	li	a7,37
    register long a4 __asm__("a4") = e;
    1dd0:	9301                	srli	a4,a4,0x20
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4))
    1dd2:	00000073          	ecall

int sys_linkat(int olddirfd, char *oldpath, int newdirfd, char *newpath, unsigned int flags)
{
    return syscall(SYS_linkat, olddirfd, oldpath, newdirfd, newpath, flags);
}
    1dd6:	2501                	sext.w	a0,a0
    1dd8:	8082                	ret

0000000000001dda <sys_unlinkat>:
    register long a2 __asm__("a2") = c;
    1dda:	1602                	slli	a2,a2,0x20
    register long a7 __asm__("a7") = n;
    1ddc:	02300893          	li	a7,35
    register long a2 __asm__("a2") = c;
    1de0:	9201                	srli	a2,a2,0x20
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1de2:	00000073          	ecall

int sys_unlinkat(int dirfd, char *path, unsigned int flags)
{
    return syscall(SYS_unlinkat, dirfd, path, flags);
}
    1de6:	2501                	sext.w	a0,a0
    1de8:	8082                	ret

0000000000001dea <link>:

int link(char *old_path, char *new_path)
{
    1dea:	87aa                	mv	a5,a0
    1dec:	86ae                	mv	a3,a1
    register long a7 __asm__("a7") = n;
    1dee:	02500893          	li	a7,37
    register long a0 __asm__("a0") = a;
    1df2:	f9c00513          	li	a0,-100
    register long a1 __asm__("a1") = b;
    1df6:	85be                	mv	a1,a5
    register long a2 __asm__("a2") = c;
    1df8:	f9c00613          	li	a2,-100
    register long a4 __asm__("a4") = e;
    1dfc:	4701                	li	a4,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4))
    1dfe:	00000073          	ecall
    return sys_linkat(AT_FDCWD, old_path, AT_FDCWD, new_path, 0);
}
    1e02:	2501                	sext.w	a0,a0
    1e04:	8082                	ret

0000000000001e06 <unlink>:

int unlink(char *path)
{
    1e06:	85aa                	mv	a1,a0
    register long a7 __asm__("a7") = n;
    1e08:	02300893          	li	a7,35
    register long a0 __asm__("a0") = a;
    1e0c:	f9c00513          	li	a0,-100
    register long a2 __asm__("a2") = c;
    1e10:	4601                	li	a2,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1e12:	00000073          	ecall
    return sys_unlinkat(AT_FDCWD, path, 0);
}
    1e16:	2501                	sext.w	a0,a0
    1e18:	8082                	ret

0000000000001e1a <uname>:
    register long a7 __asm__("a7") = n;
    1e1a:	0a000893          	li	a7,160
    __asm_syscall("r"(a7), "0"(a0))
    1e1e:	00000073          	ecall

int uname(void *buf)
{
    return syscall(SYS_uname, buf);
}
    1e22:	2501                	sext.w	a0,a0
    1e24:	8082                	ret

0000000000001e26 <brk>:
    register long a7 __asm__("a7") = n;
    1e26:	0d600893          	li	a7,214
    __asm_syscall("r"(a7), "0"(a0))
    1e2a:	00000073          	ecall

int brk(void *addr)
{
    return syscall(SYS_brk, addr);
}
    1e2e:	2501                	sext.w	a0,a0
    1e30:	8082                	ret

0000000000001e32 <getcwd>:
    register long a7 __asm__("a7") = n;
    1e32:	48c5                	li	a7,17
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1e34:	00000073          	ecall

char *getcwd(char *buf, size_t size){
    return syscall(SYS_getcwd, buf, size);
}
    1e38:	8082                	ret

0000000000001e3a <chdir>:
    register long a7 __asm__("a7") = n;
    1e3a:	03100893          	li	a7,49
    __asm_syscall("r"(a7), "0"(a0))
    1e3e:	00000073          	ecall

int chdir(const char *path){
    return syscall(SYS_chdir, path);
}
    1e42:	2501                	sext.w	a0,a0
    1e44:	8082                	ret

0000000000001e46 <mkdir>:

int mkdir(const char *path, mode_t mode){
    1e46:	862e                	mv	a2,a1
    1e48:	87aa                	mv	a5,a0
    register long a2 __asm__("a2") = c;
    1e4a:	1602                	slli	a2,a2,0x20
    register long a7 __asm__("a7") = n;
    1e4c:	02200893          	li	a7,34
    register long a0 __asm__("a0") = a;
    1e50:	f9c00513          	li	a0,-100
    register long a1 __asm__("a1") = b;
    1e54:	85be                	mv	a1,a5
    register long a2 __asm__("a2") = c;
    1e56:	9201                	srli	a2,a2,0x20
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1e58:	00000073          	ecall
    return syscall(SYS_mkdirat, AT_FDCWD, path, mode);
}
    1e5c:	2501                	sext.w	a0,a0
    1e5e:	8082                	ret

0000000000001e60 <getdents>:
    register long a7 __asm__("a7") = n;
    1e60:	03d00893          	li	a7,61
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1e64:	00000073          	ecall

int getdents(int fd, struct linux_dirent64 *dirp64, unsigned long len){
    //return syscall(SYS_getdents64, fd, dirp64, len);
    return syscall(SYS_getdents64, fd, dirp64, len);
}
    1e68:	2501                	sext.w	a0,a0
    1e6a:	8082                	ret

0000000000001e6c <pipe>:
    register long a7 __asm__("a7") = n;
    1e6c:	03b00893          	li	a7,59
    register long a1 __asm__("a1") = b;
    1e70:	4581                	li	a1,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1e72:	00000073          	ecall

int pipe(int fd[2]){
    return syscall(SYS_pipe2, fd, 0);
}
    1e76:	2501                	sext.w	a0,a0
    1e78:	8082                	ret

0000000000001e7a <dup>:
    register long a7 __asm__("a7") = n;
    1e7a:	48dd                	li	a7,23
    __asm_syscall("r"(a7), "0"(a0))
    1e7c:	00000073          	ecall

int dup(int fd){
    return syscall(SYS_dup, fd);
}
    1e80:	2501                	sext.w	a0,a0
    1e82:	8082                	ret

0000000000001e84 <dup2>:
    register long a7 __asm__("a7") = n;
    1e84:	48e1                	li	a7,24
    register long a2 __asm__("a2") = c;
    1e86:	4601                	li	a2,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1e88:	00000073          	ecall

int dup2(int old, int new){
    return syscall(SYS_dup3, old, new, 0);
}
    1e8c:	2501                	sext.w	a0,a0
    1e8e:	8082                	ret

0000000000001e90 <mount>:
    register long a7 __asm__("a7") = n;
    1e90:	02800893          	li	a7,40
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4))
    1e94:	00000073          	ecall

int mount(const char *special, const char *dir, const char *fstype, unsigned long flags, const void *data)
{
        return syscall(SYS_mount, special, dir, fstype, flags, data);
}
    1e98:	2501                	sext.w	a0,a0
    1e9a:	8082                	ret

0000000000001e9c <umount>:
    register long a7 __asm__("a7") = n;
    1e9c:	02700893          	li	a7,39
    register long a1 __asm__("a1") = b;
    1ea0:	4581                	li	a1,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1ea2:	00000073          	ecall

int umount(const char *special)
{
        return syscall(SYS_umount2, special, 0);
}
    1ea6:	2501                	sext.w	a0,a0
    1ea8:	8082                	ret

0000000000001eaa <__clone>:

.global __clone
.type  __clone, %function
__clone:
	# Save func and arg to stack
	addi a1, a1, -16
    1eaa:	15c1                	addi	a1,a1,-16
	sd a0, 0(a1)
    1eac:	e188                	sd	a0,0(a1)
	sd a3, 8(a1)
    1eae:	e594                	sd	a3,8(a1)

	# Call SYS_clone
	mv a0, a2
    1eb0:	8532                	mv	a0,a2
	mv a2, a4
    1eb2:	863a                	mv	a2,a4
	mv a3, a5
    1eb4:	86be                	mv	a3,a5
	mv a4, a6
    1eb6:	8742                	mv	a4,a6
	li a7, 220 # SYS_clone
    1eb8:	0dc00893          	li	a7,220
	ecall
    1ebc:	00000073          	ecall

	beqz a0, 1f
    1ec0:	c111                	beqz	a0,1ec4 <__clone+0x1a>
	# Parent
	ret
    1ec2:	8082                	ret

	# Child
1:      ld a1, 0(sp)
    1ec4:	6582                	ld	a1,0(sp)
	ld a0, 8(sp)
    1ec6:	6522                	ld	a0,8(sp)
	jalr a1
    1ec8:	9582                	jalr	a1

	# Exit
	li a7, 93 # SYS_exit
    1eca:	05d00893          	li	a7,93
	ecall
    1ece:	00000073          	ecall
