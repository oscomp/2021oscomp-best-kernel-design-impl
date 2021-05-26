
/home/ubuntu/rust/ultra-os/codes/riscv-syscalls-testing/user/build/riscv64/close:     file format elf64-littleriscv


Disassembly of section .text:

0000000000001000 <_start>:
.section .text.entry
.globl _start
_start:
    mv a0, sp
    1000:	850a                	mv	a0,sp
    tail __start_main
    1002:	a865                	j	10ba <__start_main>

0000000000001004 <test_close>:
 * "  close success."
 * 测试失败则输出：
 * "  close error."
 */

void test_close(void) {
    1004:	1141                	addi	sp,sp,-16
    TEST_START(__func__);
    1006:	00001517          	auipc	a0,0x1
    100a:	eea50513          	addi	a0,a0,-278 # 1ef0 <__clone+0x28>
void test_close(void) {
    100e:	e406                	sd	ra,8(sp)
    1010:	e022                	sd	s0,0(sp)
    TEST_START(__func__);
    1012:	31a000ef          	jal	ra,132c <puts>
    1016:	00001517          	auipc	a0,0x1
    101a:	f8a50513          	addi	a0,a0,-118 # 1fa0 <__func__.0>
    101e:	30e000ef          	jal	ra,132c <puts>
    1022:	00001517          	auipc	a0,0x1
    1026:	ee650513          	addi	a0,a0,-282 # 1f08 <__clone+0x40>
    102a:	302000ef          	jal	ra,132c <puts>
    int fd = open("test_close.txt", O_CREATE | O_RDWR);
    102e:	04200593          	li	a1,66
    1032:	00001517          	auipc	a0,0x1
    1036:	ee650513          	addi	a0,a0,-282 # 1f18 <__clone+0x50>
    103a:	41d000ef          	jal	ra,1c56 <open>
    103e:	842a                	mv	s0,a0
    //assert(fd > 0);
    const char *str = "  close error.\n";
    int str_len = strlen(str);
    1040:	00001517          	auipc	a0,0x1
    1044:	ee850513          	addi	a0,a0,-280 # 1f28 <__clone+0x60>
    1048:	069000ef          	jal	ra,18b0 <strlen>
    //assert(write(fd, str, str_len) == str_len);
    write(fd, str, str_len);
    104c:	0005061b          	sext.w	a2,a0
    1050:	00001597          	auipc	a1,0x1
    1054:	ed858593          	addi	a1,a1,-296 # 1f28 <__clone+0x60>
    1058:	8522                	mv	a0,s0
    105a:	43b000ef          	jal	ra,1c94 <write>
    int rt = close(fd);	
    105e:	8522                	mv	a0,s0
    1060:	41f000ef          	jal	ra,1c7e <close>
    assert(rt == 0);
    1064:	ed05                	bnez	a0,109c <test_close+0x98>
    printf("  close %d success.\n", fd);
    1066:	85a2                	mv	a1,s0
    1068:	00001517          	auipc	a0,0x1
    106c:	ef050513          	addi	a0,a0,-272 # 1f58 <__clone+0x90>
    1070:	2de000ef          	jal	ra,134e <printf>
	
    TEST_END(__func__);
    1074:	00001517          	auipc	a0,0x1
    1078:	efc50513          	addi	a0,a0,-260 # 1f70 <__clone+0xa8>
    107c:	2b0000ef          	jal	ra,132c <puts>
    1080:	00001517          	auipc	a0,0x1
    1084:	f2050513          	addi	a0,a0,-224 # 1fa0 <__func__.0>
    1088:	2a4000ef          	jal	ra,132c <puts>
}
    108c:	6402                	ld	s0,0(sp)
    108e:	60a2                	ld	ra,8(sp)
    TEST_END(__func__);
    1090:	00001517          	auipc	a0,0x1
    1094:	e7850513          	addi	a0,a0,-392 # 1f08 <__clone+0x40>
}
    1098:	0141                	addi	sp,sp,16
    TEST_END(__func__);
    109a:	ac49                	j	132c <puts>
    assert(rt == 0);
    109c:	00001517          	auipc	a0,0x1
    10a0:	e9c50513          	addi	a0,a0,-356 # 1f38 <__clone+0x70>
    10a4:	52e000ef          	jal	ra,15d2 <panic>
    10a8:	bf7d                	j	1066 <test_close+0x62>

00000000000010aa <main>:

int main(void) {
    10aa:	1141                	addi	sp,sp,-16
    10ac:	e406                	sd	ra,8(sp)
    test_close();
    10ae:	f57ff0ef          	jal	ra,1004 <test_close>
    return 0;
}
    10b2:	60a2                	ld	ra,8(sp)
    10b4:	4501                	li	a0,0
    10b6:	0141                	addi	sp,sp,16
    10b8:	8082                	ret

00000000000010ba <__start_main>:
#include <unistd.h>

extern int main();

int __start_main(long *p)
{
    10ba:	85aa                	mv	a1,a0
	int argc = p[0];
	char **argv = (void *)(p+1);

	exit(main(argc, argv));
    10bc:	4108                	lw	a0,0(a0)
{
    10be:	1141                	addi	sp,sp,-16
	exit(main(argc, argv));
    10c0:	05a1                	addi	a1,a1,8
{
    10c2:	e406                	sd	ra,8(sp)
	exit(main(argc, argv));
    10c4:	fe7ff0ef          	jal	ra,10aa <main>
    10c8:	41d000ef          	jal	ra,1ce4 <exit>
	return 0;
}
    10cc:	60a2                	ld	ra,8(sp)
    10ce:	4501                	li	a0,0
    10d0:	0141                	addi	sp,sp,16
    10d2:	8082                	ret

00000000000010d4 <printint.constprop.0>:
    write(f, s, l);
}

static char digits[] = "0123456789abcdef";

static void printint(int xx, int base, int sign)
    10d4:	7179                	addi	sp,sp,-48
    10d6:	f406                	sd	ra,40(sp)
{
    char buf[16 + 1];
    int i;
    uint x;

    if (sign && (sign = xx < 0))
    10d8:	12054b63          	bltz	a0,120e <printint.constprop.0+0x13a>

    buf[16] = 0;
    i = 15;
    do
    {
        buf[i--] = digits[x % base];
    10dc:	02b577bb          	remuw	a5,a0,a1
    10e0:	00001617          	auipc	a2,0x1
    10e4:	ed060613          	addi	a2,a2,-304 # 1fb0 <digits>
    buf[16] = 0;
    10e8:	00010c23          	sb	zero,24(sp)
        buf[i--] = digits[x % base];
    10ec:	0005871b          	sext.w	a4,a1
    10f0:	1782                	slli	a5,a5,0x20
    10f2:	9381                	srli	a5,a5,0x20
    10f4:	97b2                	add	a5,a5,a2
    10f6:	0007c783          	lbu	a5,0(a5)
    } while ((x /= base) != 0);
    10fa:	02b5583b          	divuw	a6,a0,a1
        buf[i--] = digits[x % base];
    10fe:	00f10ba3          	sb	a5,23(sp)
    } while ((x /= base) != 0);
    1102:	1cb56363          	bltu	a0,a1,12c8 <printint.constprop.0+0x1f4>
        buf[i--] = digits[x % base];
    1106:	45b9                	li	a1,14
    1108:	02e877bb          	remuw	a5,a6,a4
    110c:	1782                	slli	a5,a5,0x20
    110e:	9381                	srli	a5,a5,0x20
    1110:	97b2                	add	a5,a5,a2
    1112:	0007c783          	lbu	a5,0(a5)
    } while ((x /= base) != 0);
    1116:	02e856bb          	divuw	a3,a6,a4
        buf[i--] = digits[x % base];
    111a:	00f10b23          	sb	a5,22(sp)
    } while ((x /= base) != 0);
    111e:	0ce86e63          	bltu	a6,a4,11fa <printint.constprop.0+0x126>
        buf[i--] = digits[x % base];
    1122:	02e6f5bb          	remuw	a1,a3,a4
    } while ((x /= base) != 0);
    1126:	02e6d7bb          	divuw	a5,a3,a4
        buf[i--] = digits[x % base];
    112a:	1582                	slli	a1,a1,0x20
    112c:	9181                	srli	a1,a1,0x20
    112e:	95b2                	add	a1,a1,a2
    1130:	0005c583          	lbu	a1,0(a1)
    1134:	00b10aa3          	sb	a1,21(sp)
    } while ((x /= base) != 0);
    1138:	0007859b          	sext.w	a1,a5
    113c:	12e6ec63          	bltu	a3,a4,1274 <printint.constprop.0+0x1a0>
        buf[i--] = digits[x % base];
    1140:	02e7f6bb          	remuw	a3,a5,a4
    1144:	1682                	slli	a3,a3,0x20
    1146:	9281                	srli	a3,a3,0x20
    1148:	96b2                	add	a3,a3,a2
    114a:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    114e:	02e7d83b          	divuw	a6,a5,a4
        buf[i--] = digits[x % base];
    1152:	00d10a23          	sb	a3,20(sp)
    } while ((x /= base) != 0);
    1156:	12e5e863          	bltu	a1,a4,1286 <printint.constprop.0+0x1b2>
        buf[i--] = digits[x % base];
    115a:	02e876bb          	remuw	a3,a6,a4
    115e:	1682                	slli	a3,a3,0x20
    1160:	9281                	srli	a3,a3,0x20
    1162:	96b2                	add	a3,a3,a2
    1164:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    1168:	02e855bb          	divuw	a1,a6,a4
        buf[i--] = digits[x % base];
    116c:	00d109a3          	sb	a3,19(sp)
    } while ((x /= base) != 0);
    1170:	12e86463          	bltu	a6,a4,1298 <printint.constprop.0+0x1c4>
        buf[i--] = digits[x % base];
    1174:	02e5f6bb          	remuw	a3,a1,a4
    1178:	1682                	slli	a3,a3,0x20
    117a:	9281                	srli	a3,a3,0x20
    117c:	96b2                	add	a3,a3,a2
    117e:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    1182:	02e5d83b          	divuw	a6,a1,a4
        buf[i--] = digits[x % base];
    1186:	00d10923          	sb	a3,18(sp)
    } while ((x /= base) != 0);
    118a:	0ce5ec63          	bltu	a1,a4,1262 <printint.constprop.0+0x18e>
        buf[i--] = digits[x % base];
    118e:	02e876bb          	remuw	a3,a6,a4
    1192:	1682                	slli	a3,a3,0x20
    1194:	9281                	srli	a3,a3,0x20
    1196:	96b2                	add	a3,a3,a2
    1198:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    119c:	02e855bb          	divuw	a1,a6,a4
        buf[i--] = digits[x % base];
    11a0:	00d108a3          	sb	a3,17(sp)
    } while ((x /= base) != 0);
    11a4:	10e86963          	bltu	a6,a4,12b6 <printint.constprop.0+0x1e2>
        buf[i--] = digits[x % base];
    11a8:	02e5f6bb          	remuw	a3,a1,a4
    11ac:	1682                	slli	a3,a3,0x20
    11ae:	9281                	srli	a3,a3,0x20
    11b0:	96b2                	add	a3,a3,a2
    11b2:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    11b6:	02e5d83b          	divuw	a6,a1,a4
        buf[i--] = digits[x % base];
    11ba:	00d10823          	sb	a3,16(sp)
    } while ((x /= base) != 0);
    11be:	10e5e763          	bltu	a1,a4,12cc <printint.constprop.0+0x1f8>
        buf[i--] = digits[x % base];
    11c2:	02e876bb          	remuw	a3,a6,a4
    11c6:	1682                	slli	a3,a3,0x20
    11c8:	9281                	srli	a3,a3,0x20
    11ca:	96b2                	add	a3,a3,a2
    11cc:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    11d0:	02e857bb          	divuw	a5,a6,a4
        buf[i--] = digits[x % base];
    11d4:	00d107a3          	sb	a3,15(sp)
    } while ((x /= base) != 0);
    11d8:	10e86363          	bltu	a6,a4,12de <printint.constprop.0+0x20a>
        buf[i--] = digits[x % base];
    11dc:	1782                	slli	a5,a5,0x20
    11de:	9381                	srli	a5,a5,0x20
    11e0:	97b2                	add	a5,a5,a2
    11e2:	0007c783          	lbu	a5,0(a5)
    11e6:	4599                	li	a1,6
    11e8:	00f10723          	sb	a5,14(sp)

    if (sign)
    11ec:	00055763          	bgez	a0,11fa <printint.constprop.0+0x126>
        buf[i--] = '-';
    11f0:	02d00793          	li	a5,45
    11f4:	00f106a3          	sb	a5,13(sp)
        buf[i--] = digits[x % base];
    11f8:	4595                	li	a1,5
    write(f, s, l);
    11fa:	003c                	addi	a5,sp,8
    11fc:	4641                	li	a2,16
    11fe:	9e0d                	subw	a2,a2,a1
    1200:	4505                	li	a0,1
    1202:	95be                	add	a1,a1,a5
    1204:	291000ef          	jal	ra,1c94 <write>
    i++;
    if (i < 0)
        puts("printint error");
    out(stdout, buf + i, 16 - i);
}
    1208:	70a2                	ld	ra,40(sp)
    120a:	6145                	addi	sp,sp,48
    120c:	8082                	ret
        x = -xx;
    120e:	40a0083b          	negw	a6,a0
        buf[i--] = digits[x % base];
    1212:	02b877bb          	remuw	a5,a6,a1
    1216:	00001617          	auipc	a2,0x1
    121a:	d9a60613          	addi	a2,a2,-614 # 1fb0 <digits>
    buf[16] = 0;
    121e:	00010c23          	sb	zero,24(sp)
        buf[i--] = digits[x % base];
    1222:	0005871b          	sext.w	a4,a1
    1226:	1782                	slli	a5,a5,0x20
    1228:	9381                	srli	a5,a5,0x20
    122a:	97b2                	add	a5,a5,a2
    122c:	0007c783          	lbu	a5,0(a5)
    } while ((x /= base) != 0);
    1230:	02b858bb          	divuw	a7,a6,a1
        buf[i--] = digits[x % base];
    1234:	00f10ba3          	sb	a5,23(sp)
    } while ((x /= base) != 0);
    1238:	06b86963          	bltu	a6,a1,12aa <printint.constprop.0+0x1d6>
        buf[i--] = digits[x % base];
    123c:	02e8f7bb          	remuw	a5,a7,a4
    1240:	1782                	slli	a5,a5,0x20
    1242:	9381                	srli	a5,a5,0x20
    1244:	97b2                	add	a5,a5,a2
    1246:	0007c783          	lbu	a5,0(a5)
    } while ((x /= base) != 0);
    124a:	02e8d6bb          	divuw	a3,a7,a4
        buf[i--] = digits[x % base];
    124e:	00f10b23          	sb	a5,22(sp)
    } while ((x /= base) != 0);
    1252:	ece8f8e3          	bgeu	a7,a4,1122 <printint.constprop.0+0x4e>
        buf[i--] = '-';
    1256:	02d00793          	li	a5,45
    125a:	00f10aa3          	sb	a5,21(sp)
        buf[i--] = digits[x % base];
    125e:	45b5                	li	a1,13
    1260:	bf69                	j	11fa <printint.constprop.0+0x126>
    1262:	45a9                	li	a1,10
    if (sign)
    1264:	f8055be3          	bgez	a0,11fa <printint.constprop.0+0x126>
        buf[i--] = '-';
    1268:	02d00793          	li	a5,45
    126c:	00f108a3          	sb	a5,17(sp)
        buf[i--] = digits[x % base];
    1270:	45a5                	li	a1,9
    1272:	b761                	j	11fa <printint.constprop.0+0x126>
    1274:	45b5                	li	a1,13
    if (sign)
    1276:	f80552e3          	bgez	a0,11fa <printint.constprop.0+0x126>
        buf[i--] = '-';
    127a:	02d00793          	li	a5,45
    127e:	00f10a23          	sb	a5,20(sp)
        buf[i--] = digits[x % base];
    1282:	45b1                	li	a1,12
    1284:	bf9d                	j	11fa <printint.constprop.0+0x126>
    1286:	45b1                	li	a1,12
    if (sign)
    1288:	f60559e3          	bgez	a0,11fa <printint.constprop.0+0x126>
        buf[i--] = '-';
    128c:	02d00793          	li	a5,45
    1290:	00f109a3          	sb	a5,19(sp)
        buf[i--] = digits[x % base];
    1294:	45ad                	li	a1,11
    1296:	b795                	j	11fa <printint.constprop.0+0x126>
    1298:	45ad                	li	a1,11
    if (sign)
    129a:	f60550e3          	bgez	a0,11fa <printint.constprop.0+0x126>
        buf[i--] = '-';
    129e:	02d00793          	li	a5,45
    12a2:	00f10923          	sb	a5,18(sp)
        buf[i--] = digits[x % base];
    12a6:	45a9                	li	a1,10
    12a8:	bf89                	j	11fa <printint.constprop.0+0x126>
        buf[i--] = '-';
    12aa:	02d00793          	li	a5,45
    12ae:	00f10b23          	sb	a5,22(sp)
        buf[i--] = digits[x % base];
    12b2:	45b9                	li	a1,14
    12b4:	b799                	j	11fa <printint.constprop.0+0x126>
    12b6:	45a5                	li	a1,9
    if (sign)
    12b8:	f40551e3          	bgez	a0,11fa <printint.constprop.0+0x126>
        buf[i--] = '-';
    12bc:	02d00793          	li	a5,45
    12c0:	00f10823          	sb	a5,16(sp)
        buf[i--] = digits[x % base];
    12c4:	45a1                	li	a1,8
    12c6:	bf15                	j	11fa <printint.constprop.0+0x126>
    i = 15;
    12c8:	45bd                	li	a1,15
    12ca:	bf05                	j	11fa <printint.constprop.0+0x126>
        buf[i--] = digits[x % base];
    12cc:	45a1                	li	a1,8
    if (sign)
    12ce:	f20556e3          	bgez	a0,11fa <printint.constprop.0+0x126>
        buf[i--] = '-';
    12d2:	02d00793          	li	a5,45
    12d6:	00f107a3          	sb	a5,15(sp)
        buf[i--] = digits[x % base];
    12da:	459d                	li	a1,7
    12dc:	bf39                	j	11fa <printint.constprop.0+0x126>
    12de:	459d                	li	a1,7
    if (sign)
    12e0:	f0055de3          	bgez	a0,11fa <printint.constprop.0+0x126>
        buf[i--] = '-';
    12e4:	02d00793          	li	a5,45
    12e8:	00f10723          	sb	a5,14(sp)
        buf[i--] = digits[x % base];
    12ec:	4599                	li	a1,6
    12ee:	b731                	j	11fa <printint.constprop.0+0x126>

00000000000012f0 <getchar>:
{
    12f0:	1101                	addi	sp,sp,-32
    read(stdin, &byte, 1);
    12f2:	00f10593          	addi	a1,sp,15
    12f6:	4605                	li	a2,1
    12f8:	4501                	li	a0,0
{
    12fa:	ec06                	sd	ra,24(sp)
    char byte = 0;
    12fc:	000107a3          	sb	zero,15(sp)
    read(stdin, &byte, 1);
    1300:	18b000ef          	jal	ra,1c8a <read>
}
    1304:	60e2                	ld	ra,24(sp)
    1306:	00f14503          	lbu	a0,15(sp)
    130a:	6105                	addi	sp,sp,32
    130c:	8082                	ret

000000000000130e <putchar>:
{
    130e:	1101                	addi	sp,sp,-32
    1310:	87aa                	mv	a5,a0
    return write(stdout, &byte, 1);
    1312:	00f10593          	addi	a1,sp,15
    1316:	4605                	li	a2,1
    1318:	4505                	li	a0,1
{
    131a:	ec06                	sd	ra,24(sp)
    char byte = c;
    131c:	00f107a3          	sb	a5,15(sp)
    return write(stdout, &byte, 1);
    1320:	175000ef          	jal	ra,1c94 <write>
}
    1324:	60e2                	ld	ra,24(sp)
    1326:	2501                	sext.w	a0,a0
    1328:	6105                	addi	sp,sp,32
    132a:	8082                	ret

000000000000132c <puts>:
{
    132c:	1141                	addi	sp,sp,-16
    132e:	e406                	sd	ra,8(sp)
    1330:	e022                	sd	s0,0(sp)
    1332:	842a                	mv	s0,a0
    r = -(write(stdout, s, strlen(s)) < 0);
    1334:	57c000ef          	jal	ra,18b0 <strlen>
    1338:	862a                	mv	a2,a0
    133a:	85a2                	mv	a1,s0
    133c:	4505                	li	a0,1
    133e:	157000ef          	jal	ra,1c94 <write>
}
    1342:	60a2                	ld	ra,8(sp)
    1344:	6402                	ld	s0,0(sp)
    r = -(write(stdout, s, strlen(s)) < 0);
    1346:	957d                	srai	a0,a0,0x3f
    return r;
    1348:	2501                	sext.w	a0,a0
}
    134a:	0141                	addi	sp,sp,16
    134c:	8082                	ret

000000000000134e <printf>:
    out(stdout, buf, i);
}

// Print to the console. only understands %d, %x, %p, %s.
void printf(const char *fmt, ...)
{
    134e:	7171                	addi	sp,sp,-176
    1350:	fc56                	sd	s5,56(sp)
    1352:	ed3e                	sd	a5,152(sp)
    buf[i++] = '0';
    1354:	7ae1                	lui	s5,0xffff8
    va_list ap;
    int cnt = 0, l = 0;
    char *a, *z, *s = (char *)fmt, str;
    int f = stdout;

    va_start(ap, fmt);
    1356:	18bc                	addi	a5,sp,120
{
    1358:	e8ca                	sd	s2,80(sp)
    135a:	e4ce                	sd	s3,72(sp)
    135c:	e0d2                	sd	s4,64(sp)
    135e:	f85a                	sd	s6,48(sp)
    1360:	f486                	sd	ra,104(sp)
    1362:	f0a2                	sd	s0,96(sp)
    1364:	eca6                	sd	s1,88(sp)
    1366:	fcae                	sd	a1,120(sp)
    1368:	e132                	sd	a2,128(sp)
    136a:	e536                	sd	a3,136(sp)
    136c:	e93a                	sd	a4,144(sp)
    136e:	f142                	sd	a6,160(sp)
    1370:	f546                	sd	a7,168(sp)
    va_start(ap, fmt);
    1372:	e03e                	sd	a5,0(sp)
    for (;;)
    {
        if (!*s)
            break;
        for (a = s; *s && *s != '%'; s++)
    1374:	02500913          	li	s2,37
        out(f, a, l);
        if (l)
            continue;
        if (s[1] == 0)
            break;
        switch (s[1])
    1378:	07300a13          	li	s4,115
        case 'p':
            printptr(va_arg(ap, uint64));
            break;
        case 's':
            if ((a = va_arg(ap, char *)) == 0)
                a = "(null)";
    137c:	00001b17          	auipc	s6,0x1
    1380:	c04b0b13          	addi	s6,s6,-1020 # 1f80 <__clone+0xb8>
    buf[i++] = '0';
    1384:	830aca93          	xori	s5,s5,-2000
        buf[i++] = digits[x >> (sizeof(uint64) * 8 - 4)];
    1388:	00001997          	auipc	s3,0x1
    138c:	c2898993          	addi	s3,s3,-984 # 1fb0 <digits>
        if (!*s)
    1390:	00054783          	lbu	a5,0(a0)
    1394:	16078a63          	beqz	a5,1508 <printf+0x1ba>
    1398:	862a                	mv	a2,a0
        for (a = s; *s && *s != '%'; s++)
    139a:	19278163          	beq	a5,s2,151c <printf+0x1ce>
    139e:	00164783          	lbu	a5,1(a2)
    13a2:	0605                	addi	a2,a2,1
    13a4:	fbfd                	bnez	a5,139a <printf+0x4c>
    13a6:	84b2                	mv	s1,a2
        l = z - a;
    13a8:	40a6043b          	subw	s0,a2,a0
    write(f, s, l);
    13ac:	85aa                	mv	a1,a0
    13ae:	8622                	mv	a2,s0
    13b0:	4505                	li	a0,1
    13b2:	0e3000ef          	jal	ra,1c94 <write>
        if (l)
    13b6:	18041c63          	bnez	s0,154e <printf+0x200>
        if (s[1] == 0)
    13ba:	0014c783          	lbu	a5,1(s1)
    13be:	14078563          	beqz	a5,1508 <printf+0x1ba>
        switch (s[1])
    13c2:	1d478063          	beq	a5,s4,1582 <printf+0x234>
    13c6:	18fa6663          	bltu	s4,a5,1552 <printf+0x204>
    13ca:	06400713          	li	a4,100
    13ce:	1ae78063          	beq	a5,a4,156e <printf+0x220>
    13d2:	07000713          	li	a4,112
    13d6:	1ce79963          	bne	a5,a4,15a8 <printf+0x25a>
            printptr(va_arg(ap, uint64));
    13da:	6702                	ld	a4,0(sp)
    buf[i++] = '0';
    13dc:	01511423          	sh	s5,8(sp)
    write(f, s, l);
    13e0:	4649                	li	a2,18
            printptr(va_arg(ap, uint64));
    13e2:	631c                	ld	a5,0(a4)
    13e4:	0721                	addi	a4,a4,8
    13e6:	e03a                	sd	a4,0(sp)
    for (j = 0; j < (sizeof(uint64) * 2); j++, x <<= 4)
    13e8:	00479293          	slli	t0,a5,0x4
    13ec:	00879f93          	slli	t6,a5,0x8
    13f0:	00c79f13          	slli	t5,a5,0xc
    13f4:	01079e93          	slli	t4,a5,0x10
    13f8:	01479e13          	slli	t3,a5,0x14
    13fc:	01879313          	slli	t1,a5,0x18
    1400:	01c79893          	slli	a7,a5,0x1c
    1404:	02479813          	slli	a6,a5,0x24
    1408:	02879513          	slli	a0,a5,0x28
    140c:	02c79593          	slli	a1,a5,0x2c
    1410:	03079693          	slli	a3,a5,0x30
    1414:	03479713          	slli	a4,a5,0x34
        buf[i++] = digits[x >> (sizeof(uint64) * 8 - 4)];
    1418:	03c7d413          	srli	s0,a5,0x3c
    141c:	01c7d39b          	srliw	t2,a5,0x1c
    1420:	03c2d293          	srli	t0,t0,0x3c
    1424:	03cfdf93          	srli	t6,t6,0x3c
    1428:	03cf5f13          	srli	t5,t5,0x3c
    142c:	03cede93          	srli	t4,t4,0x3c
    1430:	03ce5e13          	srli	t3,t3,0x3c
    1434:	03c35313          	srli	t1,t1,0x3c
    1438:	03c8d893          	srli	a7,a7,0x3c
    143c:	03c85813          	srli	a6,a6,0x3c
    1440:	9171                	srli	a0,a0,0x3c
    1442:	91f1                	srli	a1,a1,0x3c
    1444:	92f1                	srli	a3,a3,0x3c
    1446:	9371                	srli	a4,a4,0x3c
    1448:	96ce                	add	a3,a3,s3
    144a:	974e                	add	a4,a4,s3
    144c:	944e                	add	s0,s0,s3
    144e:	92ce                	add	t0,t0,s3
    1450:	9fce                	add	t6,t6,s3
    1452:	9f4e                	add	t5,t5,s3
    1454:	9ece                	add	t4,t4,s3
    1456:	9e4e                	add	t3,t3,s3
    1458:	934e                	add	t1,t1,s3
    145a:	98ce                	add	a7,a7,s3
    145c:	93ce                	add	t2,t2,s3
    145e:	984e                	add	a6,a6,s3
    1460:	954e                	add	a0,a0,s3
    1462:	95ce                	add	a1,a1,s3
    1464:	0006c083          	lbu	ra,0(a3)
    1468:	0002c283          	lbu	t0,0(t0)
    146c:	00074683          	lbu	a3,0(a4)
    1470:	000fcf83          	lbu	t6,0(t6)
    1474:	000f4f03          	lbu	t5,0(t5)
    1478:	000ece83          	lbu	t4,0(t4)
    147c:	000e4e03          	lbu	t3,0(t3)
    1480:	00034303          	lbu	t1,0(t1)
    1484:	0008c883          	lbu	a7,0(a7)
    1488:	0003c383          	lbu	t2,0(t2)
    148c:	00084803          	lbu	a6,0(a6)
    1490:	00054503          	lbu	a0,0(a0)
    1494:	0005c583          	lbu	a1,0(a1)
    1498:	00044403          	lbu	s0,0(s0)
    for (j = 0; j < (sizeof(uint64) * 2); j++, x <<= 4)
    149c:	03879713          	slli	a4,a5,0x38
        buf[i++] = digits[x >> (sizeof(uint64) * 8 - 4)];
    14a0:	9371                	srli	a4,a4,0x3c
    14a2:	8bbd                	andi	a5,a5,15
    14a4:	974e                	add	a4,a4,s3
    14a6:	97ce                	add	a5,a5,s3
    14a8:	005105a3          	sb	t0,11(sp)
    14ac:	01f10623          	sb	t6,12(sp)
    14b0:	01e106a3          	sb	t5,13(sp)
    14b4:	01d10723          	sb	t4,14(sp)
    14b8:	01c107a3          	sb	t3,15(sp)
    14bc:	00610823          	sb	t1,16(sp)
    14c0:	011108a3          	sb	a7,17(sp)
    14c4:	00710923          	sb	t2,18(sp)
    14c8:	010109a3          	sb	a6,19(sp)
    14cc:	00a10a23          	sb	a0,20(sp)
    14d0:	00b10aa3          	sb	a1,21(sp)
    14d4:	00110b23          	sb	ra,22(sp)
    14d8:	00d10ba3          	sb	a3,23(sp)
    14dc:	00810523          	sb	s0,10(sp)
    14e0:	00074703          	lbu	a4,0(a4)
    14e4:	0007c783          	lbu	a5,0(a5)
    write(f, s, l);
    14e8:	002c                	addi	a1,sp,8
    14ea:	4505                	li	a0,1
        buf[i++] = digits[x >> (sizeof(uint64) * 8 - 4)];
    14ec:	00e10c23          	sb	a4,24(sp)
    14f0:	00f10ca3          	sb	a5,25(sp)
    buf[i] = 0;
    14f4:	00010d23          	sb	zero,26(sp)
    write(f, s, l);
    14f8:	79c000ef          	jal	ra,1c94 <write>
            // Print unknown % sequence to draw attention.
            putchar('%');
            putchar(s[1]);
            break;
        }
        s += 2;
    14fc:	00248513          	addi	a0,s1,2
        if (!*s)
    1500:	00054783          	lbu	a5,0(a0)
    1504:	e8079ae3          	bnez	a5,1398 <printf+0x4a>
    }
    va_end(ap);
}
    1508:	70a6                	ld	ra,104(sp)
    150a:	7406                	ld	s0,96(sp)
    150c:	64e6                	ld	s1,88(sp)
    150e:	6946                	ld	s2,80(sp)
    1510:	69a6                	ld	s3,72(sp)
    1512:	6a06                	ld	s4,64(sp)
    1514:	7ae2                	ld	s5,56(sp)
    1516:	7b42                	ld	s6,48(sp)
    1518:	614d                	addi	sp,sp,176
    151a:	8082                	ret
        for (z = s; s[0] == '%' && s[1] == '%'; z++, s += 2)
    151c:	00064783          	lbu	a5,0(a2)
    1520:	84b2                	mv	s1,a2
    1522:	01278963          	beq	a5,s2,1534 <printf+0x1e6>
    1526:	b549                	j	13a8 <printf+0x5a>
    1528:	0024c783          	lbu	a5,2(s1)
    152c:	0605                	addi	a2,a2,1
    152e:	0489                	addi	s1,s1,2
    1530:	e7279ce3          	bne	a5,s2,13a8 <printf+0x5a>
    1534:	0014c783          	lbu	a5,1(s1)
    1538:	ff2788e3          	beq	a5,s2,1528 <printf+0x1da>
        l = z - a;
    153c:	40a6043b          	subw	s0,a2,a0
    write(f, s, l);
    1540:	85aa                	mv	a1,a0
    1542:	8622                	mv	a2,s0
    1544:	4505                	li	a0,1
    1546:	74e000ef          	jal	ra,1c94 <write>
        if (l)
    154a:	e60408e3          	beqz	s0,13ba <printf+0x6c>
    154e:	8526                	mv	a0,s1
    1550:	b581                	j	1390 <printf+0x42>
        switch (s[1])
    1552:	07800713          	li	a4,120
    1556:	04e79963          	bne	a5,a4,15a8 <printf+0x25a>
            printint(va_arg(ap, int), 16, 1);
    155a:	6782                	ld	a5,0(sp)
    155c:	45c1                	li	a1,16
    155e:	4388                	lw	a0,0(a5)
    1560:	07a1                	addi	a5,a5,8
    1562:	e03e                	sd	a5,0(sp)
    1564:	b71ff0ef          	jal	ra,10d4 <printint.constprop.0>
        s += 2;
    1568:	00248513          	addi	a0,s1,2
    156c:	bf51                	j	1500 <printf+0x1b2>
            printint(va_arg(ap, int), 10, 1);
    156e:	6782                	ld	a5,0(sp)
    1570:	45a9                	li	a1,10
    1572:	4388                	lw	a0,0(a5)
    1574:	07a1                	addi	a5,a5,8
    1576:	e03e                	sd	a5,0(sp)
    1578:	b5dff0ef          	jal	ra,10d4 <printint.constprop.0>
        s += 2;
    157c:	00248513          	addi	a0,s1,2
    1580:	b741                	j	1500 <printf+0x1b2>
            if ((a = va_arg(ap, char *)) == 0)
    1582:	6782                	ld	a5,0(sp)
    1584:	6380                	ld	s0,0(a5)
    1586:	07a1                	addi	a5,a5,8
    1588:	e03e                	sd	a5,0(sp)
    158a:	c031                	beqz	s0,15ce <printf+0x280>
            l = strnlen(a, 200);
    158c:	0c800593          	li	a1,200
    1590:	8522                	mv	a0,s0
    1592:	40a000ef          	jal	ra,199c <strnlen>
    write(f, s, l);
    1596:	0005061b          	sext.w	a2,a0
    159a:	85a2                	mv	a1,s0
    159c:	4505                	li	a0,1
    159e:	6f6000ef          	jal	ra,1c94 <write>
        s += 2;
    15a2:	00248513          	addi	a0,s1,2
    15a6:	bfa9                	j	1500 <printf+0x1b2>
    return write(stdout, &byte, 1);
    15a8:	4605                	li	a2,1
    15aa:	002c                	addi	a1,sp,8
    15ac:	4505                	li	a0,1
    char byte = c;
    15ae:	01210423          	sb	s2,8(sp)
    return write(stdout, &byte, 1);
    15b2:	6e2000ef          	jal	ra,1c94 <write>
    char byte = c;
    15b6:	0014c783          	lbu	a5,1(s1)
    return write(stdout, &byte, 1);
    15ba:	4605                	li	a2,1
    15bc:	002c                	addi	a1,sp,8
    15be:	4505                	li	a0,1
    char byte = c;
    15c0:	00f10423          	sb	a5,8(sp)
    return write(stdout, &byte, 1);
    15c4:	6d0000ef          	jal	ra,1c94 <write>
        s += 2;
    15c8:	00248513          	addi	a0,s1,2
    15cc:	bf15                	j	1500 <printf+0x1b2>
                a = "(null)";
    15ce:	845a                	mv	s0,s6
    15d0:	bf75                	j	158c <printf+0x23e>

00000000000015d2 <panic>:
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

void panic(char *m)
{
    15d2:	1141                	addi	sp,sp,-16
    15d4:	e406                	sd	ra,8(sp)
    puts(m);
    15d6:	d57ff0ef          	jal	ra,132c <puts>
    exit(-100);
}
    15da:	60a2                	ld	ra,8(sp)
    exit(-100);
    15dc:	f9c00513          	li	a0,-100
}
    15e0:	0141                	addi	sp,sp,16
    exit(-100);
    15e2:	a709                	j	1ce4 <exit>

00000000000015e4 <isspace>:
#define HIGHS (ONES * (UCHAR_MAX / 2 + 1))
#define HASZERO(x) (((x)-ONES) & ~(x)&HIGHS)

int isspace(int c)
{
    return c == ' ' || (unsigned)c - '\t' < 5;
    15e4:	02000793          	li	a5,32
    15e8:	00f50663          	beq	a0,a5,15f4 <isspace+0x10>
    15ec:	355d                	addiw	a0,a0,-9
    15ee:	00553513          	sltiu	a0,a0,5
    15f2:	8082                	ret
    15f4:	4505                	li	a0,1
}
    15f6:	8082                	ret

00000000000015f8 <isdigit>:

int isdigit(int c)
{
    return (unsigned)c - '0' < 10;
    15f8:	fd05051b          	addiw	a0,a0,-48
}
    15fc:	00a53513          	sltiu	a0,a0,10
    1600:	8082                	ret

0000000000001602 <atoi>:
    return c == ' ' || (unsigned)c - '\t' < 5;
    1602:	02000613          	li	a2,32
    1606:	4591                	li	a1,4

int atoi(const char *s)
{
    int n = 0, neg = 0;
    while (isspace(*s))
    1608:	00054703          	lbu	a4,0(a0)
    return c == ' ' || (unsigned)c - '\t' < 5;
    160c:	ff77069b          	addiw	a3,a4,-9
    1610:	04c70d63          	beq	a4,a2,166a <atoi+0x68>
    1614:	0007079b          	sext.w	a5,a4
    1618:	04d5f963          	bgeu	a1,a3,166a <atoi+0x68>
        s++;
    switch (*s)
    161c:	02b00693          	li	a3,43
    1620:	04d70a63          	beq	a4,a3,1674 <atoi+0x72>
    1624:	02d00693          	li	a3,45
    1628:	06d70463          	beq	a4,a3,1690 <atoi+0x8e>
        neg = 1;
    case '+':
        s++;
    }
    /* Compute n as a negative number to avoid overflow on INT_MIN */
    while (isdigit(*s))
    162c:	fd07859b          	addiw	a1,a5,-48
    1630:	4625                	li	a2,9
    1632:	873e                	mv	a4,a5
    1634:	86aa                	mv	a3,a0
    int n = 0, neg = 0;
    1636:	4e01                	li	t3,0
    while (isdigit(*s))
    1638:	04b66a63          	bltu	a2,a1,168c <atoi+0x8a>
    int n = 0, neg = 0;
    163c:	4501                	li	a0,0
    while (isdigit(*s))
    163e:	4825                	li	a6,9
    1640:	0016c603          	lbu	a2,1(a3)
        n = 10 * n - (*s++ - '0');
    1644:	0025179b          	slliw	a5,a0,0x2
    1648:	9d3d                	addw	a0,a0,a5
    164a:	fd07031b          	addiw	t1,a4,-48
    164e:	0015189b          	slliw	a7,a0,0x1
    while (isdigit(*s))
    1652:	fd06059b          	addiw	a1,a2,-48
        n = 10 * n - (*s++ - '0');
    1656:	0685                	addi	a3,a3,1
    1658:	4068853b          	subw	a0,a7,t1
    while (isdigit(*s))
    165c:	0006071b          	sext.w	a4,a2
    1660:	feb870e3          	bgeu	a6,a1,1640 <atoi+0x3e>
    return neg ? n : -n;
    1664:	000e0563          	beqz	t3,166e <atoi+0x6c>
}
    1668:	8082                	ret
        s++;
    166a:	0505                	addi	a0,a0,1
    166c:	bf71                	j	1608 <atoi+0x6>
    return neg ? n : -n;
    166e:	4113053b          	subw	a0,t1,a7
    1672:	8082                	ret
    while (isdigit(*s))
    1674:	00154783          	lbu	a5,1(a0)
    1678:	4625                	li	a2,9
        s++;
    167a:	00150693          	addi	a3,a0,1
    while (isdigit(*s))
    167e:	fd07859b          	addiw	a1,a5,-48
    1682:	0007871b          	sext.w	a4,a5
    int n = 0, neg = 0;
    1686:	4e01                	li	t3,0
    while (isdigit(*s))
    1688:	fab67ae3          	bgeu	a2,a1,163c <atoi+0x3a>
    168c:	4501                	li	a0,0
}
    168e:	8082                	ret
    while (isdigit(*s))
    1690:	00154783          	lbu	a5,1(a0)
    1694:	4625                	li	a2,9
        s++;
    1696:	00150693          	addi	a3,a0,1
    while (isdigit(*s))
    169a:	fd07859b          	addiw	a1,a5,-48
    169e:	0007871b          	sext.w	a4,a5
    16a2:	feb665e3          	bltu	a2,a1,168c <atoi+0x8a>
        neg = 1;
    16a6:	4e05                	li	t3,1
    16a8:	bf51                	j	163c <atoi+0x3a>

00000000000016aa <memset>:

void *memset(void *dest, int c, size_t n)
{
    char *p = dest;
    for (int i = 0; i < n; ++i, *(p++) = c)
    16aa:	16060d63          	beqz	a2,1824 <memset+0x17a>
    16ae:	40a007b3          	neg	a5,a0
    16b2:	8b9d                	andi	a5,a5,7
    16b4:	00778713          	addi	a4,a5,7
    16b8:	482d                	li	a6,11
    16ba:	0ff5f593          	andi	a1,a1,255
    16be:	fff60693          	addi	a3,a2,-1
    16c2:	17076263          	bltu	a4,a6,1826 <memset+0x17c>
    16c6:	16e6ea63          	bltu	a3,a4,183a <memset+0x190>
    16ca:	16078563          	beqz	a5,1834 <memset+0x18a>
    16ce:	00b50023          	sb	a1,0(a0)
    16d2:	4705                	li	a4,1
    16d4:	00150e93          	addi	t4,a0,1
    16d8:	14e78c63          	beq	a5,a4,1830 <memset+0x186>
    16dc:	00b500a3          	sb	a1,1(a0)
    16e0:	4709                	li	a4,2
    16e2:	00250e93          	addi	t4,a0,2
    16e6:	14e78d63          	beq	a5,a4,1840 <memset+0x196>
    16ea:	00b50123          	sb	a1,2(a0)
    16ee:	470d                	li	a4,3
    16f0:	00350e93          	addi	t4,a0,3
    16f4:	12e78b63          	beq	a5,a4,182a <memset+0x180>
    16f8:	00b501a3          	sb	a1,3(a0)
    16fc:	4711                	li	a4,4
    16fe:	00450e93          	addi	t4,a0,4
    1702:	14e78163          	beq	a5,a4,1844 <memset+0x19a>
    1706:	00b50223          	sb	a1,4(a0)
    170a:	4715                	li	a4,5
    170c:	00550e93          	addi	t4,a0,5
    1710:	12e78c63          	beq	a5,a4,1848 <memset+0x19e>
    1714:	00b502a3          	sb	a1,5(a0)
    1718:	471d                	li	a4,7
    171a:	00650e93          	addi	t4,a0,6
    171e:	12e79763          	bne	a5,a4,184c <memset+0x1a2>
    1722:	00750e93          	addi	t4,a0,7
    1726:	00b50323          	sb	a1,6(a0)
    172a:	4f1d                	li	t5,7
    172c:	00859713          	slli	a4,a1,0x8
    1730:	8f4d                	or	a4,a4,a1
    1732:	01059e13          	slli	t3,a1,0x10
    1736:	01c76e33          	or	t3,a4,t3
    173a:	01859313          	slli	t1,a1,0x18
    173e:	006e6333          	or	t1,t3,t1
    1742:	02059893          	slli	a7,a1,0x20
    1746:	011368b3          	or	a7,t1,a7
    174a:	02859813          	slli	a6,a1,0x28
    174e:	40f60333          	sub	t1,a2,a5
    1752:	0108e833          	or	a6,a7,a6
    1756:	03059693          	slli	a3,a1,0x30
    175a:	00d866b3          	or	a3,a6,a3
    175e:	03859713          	slli	a4,a1,0x38
    1762:	97aa                	add	a5,a5,a0
    1764:	ff837813          	andi	a6,t1,-8
    1768:	8f55                	or	a4,a4,a3
    176a:	00f806b3          	add	a3,a6,a5
    176e:	e398                	sd	a4,0(a5)
    1770:	07a1                	addi	a5,a5,8
    1772:	fed79ee3          	bne	a5,a3,176e <memset+0xc4>
    1776:	ff837693          	andi	a3,t1,-8
    177a:	00de87b3          	add	a5,t4,a3
    177e:	01e6873b          	addw	a4,a3,t5
    1782:	0ad30663          	beq	t1,a3,182e <memset+0x184>
    1786:	00b78023          	sb	a1,0(a5)
    178a:	0017069b          	addiw	a3,a4,1
    178e:	08c6fb63          	bgeu	a3,a2,1824 <memset+0x17a>
    1792:	00b780a3          	sb	a1,1(a5)
    1796:	0027069b          	addiw	a3,a4,2
    179a:	08c6f563          	bgeu	a3,a2,1824 <memset+0x17a>
    179e:	00b78123          	sb	a1,2(a5)
    17a2:	0037069b          	addiw	a3,a4,3
    17a6:	06c6ff63          	bgeu	a3,a2,1824 <memset+0x17a>
    17aa:	00b781a3          	sb	a1,3(a5)
    17ae:	0047069b          	addiw	a3,a4,4
    17b2:	06c6f963          	bgeu	a3,a2,1824 <memset+0x17a>
    17b6:	00b78223          	sb	a1,4(a5)
    17ba:	0057069b          	addiw	a3,a4,5
    17be:	06c6f363          	bgeu	a3,a2,1824 <memset+0x17a>
    17c2:	00b782a3          	sb	a1,5(a5)
    17c6:	0067069b          	addiw	a3,a4,6
    17ca:	04c6fd63          	bgeu	a3,a2,1824 <memset+0x17a>
    17ce:	00b78323          	sb	a1,6(a5)
    17d2:	0077069b          	addiw	a3,a4,7
    17d6:	04c6f763          	bgeu	a3,a2,1824 <memset+0x17a>
    17da:	00b783a3          	sb	a1,7(a5)
    17de:	0087069b          	addiw	a3,a4,8
    17e2:	04c6f163          	bgeu	a3,a2,1824 <memset+0x17a>
    17e6:	00b78423          	sb	a1,8(a5)
    17ea:	0097069b          	addiw	a3,a4,9
    17ee:	02c6fb63          	bgeu	a3,a2,1824 <memset+0x17a>
    17f2:	00b784a3          	sb	a1,9(a5)
    17f6:	00a7069b          	addiw	a3,a4,10
    17fa:	02c6f563          	bgeu	a3,a2,1824 <memset+0x17a>
    17fe:	00b78523          	sb	a1,10(a5)
    1802:	00b7069b          	addiw	a3,a4,11
    1806:	00c6ff63          	bgeu	a3,a2,1824 <memset+0x17a>
    180a:	00b785a3          	sb	a1,11(a5)
    180e:	00c7069b          	addiw	a3,a4,12
    1812:	00c6f963          	bgeu	a3,a2,1824 <memset+0x17a>
    1816:	00b78623          	sb	a1,12(a5)
    181a:	2735                	addiw	a4,a4,13
    181c:	00c77463          	bgeu	a4,a2,1824 <memset+0x17a>
    1820:	00b786a3          	sb	a1,13(a5)
        ;
    return dest;
}
    1824:	8082                	ret
    1826:	472d                	li	a4,11
    1828:	bd79                	j	16c6 <memset+0x1c>
    for (int i = 0; i < n; ++i, *(p++) = c)
    182a:	4f0d                	li	t5,3
    182c:	b701                	j	172c <memset+0x82>
    182e:	8082                	ret
    1830:	4f05                	li	t5,1
    1832:	bded                	j	172c <memset+0x82>
    1834:	8eaa                	mv	t4,a0
    1836:	4f01                	li	t5,0
    1838:	bdd5                	j	172c <memset+0x82>
    183a:	87aa                	mv	a5,a0
    183c:	4701                	li	a4,0
    183e:	b7a1                	j	1786 <memset+0xdc>
    1840:	4f09                	li	t5,2
    1842:	b5ed                	j	172c <memset+0x82>
    1844:	4f11                	li	t5,4
    1846:	b5dd                	j	172c <memset+0x82>
    1848:	4f15                	li	t5,5
    184a:	b5cd                	j	172c <memset+0x82>
    184c:	4f19                	li	t5,6
    184e:	bdf9                	j	172c <memset+0x82>

0000000000001850 <strcmp>:

int strcmp(const char *l, const char *r)
{
    for (; *l == *r && *l; l++, r++)
    1850:	00054783          	lbu	a5,0(a0)
    1854:	0005c703          	lbu	a4,0(a1)
    1858:	00e79863          	bne	a5,a4,1868 <strcmp+0x18>
    185c:	0505                	addi	a0,a0,1
    185e:	0585                	addi	a1,a1,1
    1860:	fbe5                	bnez	a5,1850 <strcmp>
    1862:	4501                	li	a0,0
        ;
    return *(unsigned char *)l - *(unsigned char *)r;
}
    1864:	9d19                	subw	a0,a0,a4
    1866:	8082                	ret
    return *(unsigned char *)l - *(unsigned char *)r;
    1868:	0007851b          	sext.w	a0,a5
    186c:	bfe5                	j	1864 <strcmp+0x14>

000000000000186e <strncmp>:

int strncmp(const char *_l, const char *_r, size_t n)
{
    const unsigned char *l = (void *)_l, *r = (void *)_r;
    if (!n--)
    186e:	ce05                	beqz	a2,18a6 <strncmp+0x38>
        return 0;
    for (; *l && *r && n && *l == *r; l++, r++, n--)
    1870:	00054703          	lbu	a4,0(a0)
    1874:	0005c783          	lbu	a5,0(a1)
    1878:	cb0d                	beqz	a4,18aa <strncmp+0x3c>
    if (!n--)
    187a:	167d                	addi	a2,a2,-1
    187c:	00c506b3          	add	a3,a0,a2
    1880:	a819                	j	1896 <strncmp+0x28>
    for (; *l && *r && n && *l == *r; l++, r++, n--)
    1882:	00a68e63          	beq	a3,a0,189e <strncmp+0x30>
    1886:	0505                	addi	a0,a0,1
    1888:	00e79b63          	bne	a5,a4,189e <strncmp+0x30>
    188c:	00054703          	lbu	a4,0(a0)
        ;
    return *l - *r;
    1890:	0005c783          	lbu	a5,0(a1)
    for (; *l && *r && n && *l == *r; l++, r++, n--)
    1894:	cb19                	beqz	a4,18aa <strncmp+0x3c>
    1896:	0005c783          	lbu	a5,0(a1)
    189a:	0585                	addi	a1,a1,1
    189c:	f3fd                	bnez	a5,1882 <strncmp+0x14>
    return *l - *r;
    189e:	0007051b          	sext.w	a0,a4
    18a2:	9d1d                	subw	a0,a0,a5
    18a4:	8082                	ret
        return 0;
    18a6:	4501                	li	a0,0
}
    18a8:	8082                	ret
    18aa:	4501                	li	a0,0
    return *l - *r;
    18ac:	9d1d                	subw	a0,a0,a5
    18ae:	8082                	ret

00000000000018b0 <strlen>:
size_t strlen(const char *s)
{
    const char *a = s;
    typedef size_t __attribute__((__may_alias__)) word;
    const word *w;
    for (; (uintptr_t)s % SS; s++)
    18b0:	00757793          	andi	a5,a0,7
    18b4:	cf89                	beqz	a5,18ce <strlen+0x1e>
    18b6:	87aa                	mv	a5,a0
    18b8:	a029                	j	18c2 <strlen+0x12>
    18ba:	0785                	addi	a5,a5,1
    18bc:	0077f713          	andi	a4,a5,7
    18c0:	cb01                	beqz	a4,18d0 <strlen+0x20>
        if (!*s)
    18c2:	0007c703          	lbu	a4,0(a5)
    18c6:	fb75                	bnez	a4,18ba <strlen+0xa>
    for (w = (const void *)s; !HASZERO(*w); w++)
        ;
    s = (const void *)w;
    for (; *s; s++)
        ;
    return s - a;
    18c8:	40a78533          	sub	a0,a5,a0
}
    18cc:	8082                	ret
    for (; (uintptr_t)s % SS; s++)
    18ce:	87aa                	mv	a5,a0
    for (w = (const void *)s; !HASZERO(*w); w++)
    18d0:	6394                	ld	a3,0(a5)
    18d2:	00000597          	auipc	a1,0x0
    18d6:	6b65b583          	ld	a1,1718(a1) # 1f88 <__clone+0xc0>
    18da:	00000617          	auipc	a2,0x0
    18de:	6b663603          	ld	a2,1718(a2) # 1f90 <__clone+0xc8>
    18e2:	a019                	j	18e8 <strlen+0x38>
    18e4:	6794                	ld	a3,8(a5)
    18e6:	07a1                	addi	a5,a5,8
    18e8:	00b68733          	add	a4,a3,a1
    18ec:	fff6c693          	not	a3,a3
    18f0:	8f75                	and	a4,a4,a3
    18f2:	8f71                	and	a4,a4,a2
    18f4:	db65                	beqz	a4,18e4 <strlen+0x34>
    for (; *s; s++)
    18f6:	0007c703          	lbu	a4,0(a5)
    18fa:	d779                	beqz	a4,18c8 <strlen+0x18>
    18fc:	0017c703          	lbu	a4,1(a5)
    1900:	0785                	addi	a5,a5,1
    1902:	d379                	beqz	a4,18c8 <strlen+0x18>
    1904:	0017c703          	lbu	a4,1(a5)
    1908:	0785                	addi	a5,a5,1
    190a:	fb6d                	bnez	a4,18fc <strlen+0x4c>
    190c:	bf75                	j	18c8 <strlen+0x18>

000000000000190e <memchr>:

void *memchr(const void *src, int c, size_t n)
{
    const unsigned char *s = src;
    c = (unsigned char)c;
    for (; ((uintptr_t)s & ALIGN) && n && *s != c; s++, n--)
    190e:	00757713          	andi	a4,a0,7
{
    1912:	87aa                	mv	a5,a0
    c = (unsigned char)c;
    1914:	0ff5f593          	andi	a1,a1,255
    for (; ((uintptr_t)s & ALIGN) && n && *s != c; s++, n--)
    1918:	cb19                	beqz	a4,192e <memchr+0x20>
    191a:	ce25                	beqz	a2,1992 <memchr+0x84>
    191c:	0007c703          	lbu	a4,0(a5)
    1920:	04b70e63          	beq	a4,a1,197c <memchr+0x6e>
    1924:	0785                	addi	a5,a5,1
    1926:	0077f713          	andi	a4,a5,7
    192a:	167d                	addi	a2,a2,-1
    192c:	f77d                	bnez	a4,191a <memchr+0xc>
            ;
        s = (const void *)w;
    }
    for (; n && *s != c; s++, n--)
        ;
    return n ? (void *)s : 0;
    192e:	4501                	li	a0,0
    if (n && *s != c)
    1930:	c235                	beqz	a2,1994 <memchr+0x86>
    1932:	0007c703          	lbu	a4,0(a5)
    1936:	04b70363          	beq	a4,a1,197c <memchr+0x6e>
        size_t k = ONES * c;
    193a:	00000517          	auipc	a0,0x0
    193e:	65e53503          	ld	a0,1630(a0) # 1f98 <__clone+0xd0>
        for (w = (const void *)s; n >= SS && !HASZERO(*w ^ k); w++, n -= SS)
    1942:	471d                	li	a4,7
        size_t k = ONES * c;
    1944:	02a58533          	mul	a0,a1,a0
        for (w = (const void *)s; n >= SS && !HASZERO(*w ^ k); w++, n -= SS)
    1948:	02c77a63          	bgeu	a4,a2,197c <memchr+0x6e>
    194c:	00000897          	auipc	a7,0x0
    1950:	63c8b883          	ld	a7,1596(a7) # 1f88 <__clone+0xc0>
    1954:	00000817          	auipc	a6,0x0
    1958:	63c83803          	ld	a6,1596(a6) # 1f90 <__clone+0xc8>
    195c:	431d                	li	t1,7
    195e:	a029                	j	1968 <memchr+0x5a>
    1960:	1661                	addi	a2,a2,-8
    1962:	07a1                	addi	a5,a5,8
    1964:	02c37963          	bgeu	t1,a2,1996 <memchr+0x88>
    1968:	6398                	ld	a4,0(a5)
    196a:	8f29                	xor	a4,a4,a0
    196c:	011706b3          	add	a3,a4,a7
    1970:	fff74713          	not	a4,a4
    1974:	8f75                	and	a4,a4,a3
    1976:	01077733          	and	a4,a4,a6
    197a:	d37d                	beqz	a4,1960 <memchr+0x52>
    197c:	853e                	mv	a0,a5
    197e:	97b2                	add	a5,a5,a2
    1980:	a021                	j	1988 <memchr+0x7a>
    for (; n && *s != c; s++, n--)
    1982:	0505                	addi	a0,a0,1
    1984:	00f50763          	beq	a0,a5,1992 <memchr+0x84>
    1988:	00054703          	lbu	a4,0(a0)
    198c:	feb71be3          	bne	a4,a1,1982 <memchr+0x74>
    1990:	8082                	ret
    return n ? (void *)s : 0;
    1992:	4501                	li	a0,0
}
    1994:	8082                	ret
    return n ? (void *)s : 0;
    1996:	4501                	li	a0,0
    for (; n && *s != c; s++, n--)
    1998:	f275                	bnez	a2,197c <memchr+0x6e>
}
    199a:	8082                	ret

000000000000199c <strnlen>:

size_t strnlen(const char *s, size_t n)
{
    199c:	1101                	addi	sp,sp,-32
    199e:	e822                	sd	s0,16(sp)
    const char *p = memchr(s, 0, n);
    19a0:	862e                	mv	a2,a1
{
    19a2:	842e                	mv	s0,a1
    const char *p = memchr(s, 0, n);
    19a4:	4581                	li	a1,0
{
    19a6:	e426                	sd	s1,8(sp)
    19a8:	ec06                	sd	ra,24(sp)
    19aa:	84aa                	mv	s1,a0
    const char *p = memchr(s, 0, n);
    19ac:	f63ff0ef          	jal	ra,190e <memchr>
    return p ? p - s : n;
    19b0:	c519                	beqz	a0,19be <strnlen+0x22>
}
    19b2:	60e2                	ld	ra,24(sp)
    19b4:	6442                	ld	s0,16(sp)
    return p ? p - s : n;
    19b6:	8d05                	sub	a0,a0,s1
}
    19b8:	64a2                	ld	s1,8(sp)
    19ba:	6105                	addi	sp,sp,32
    19bc:	8082                	ret
    19be:	60e2                	ld	ra,24(sp)
    return p ? p - s : n;
    19c0:	8522                	mv	a0,s0
}
    19c2:	6442                	ld	s0,16(sp)
    19c4:	64a2                	ld	s1,8(sp)
    19c6:	6105                	addi	sp,sp,32
    19c8:	8082                	ret

00000000000019ca <strcpy>:
char *strcpy(char *restrict d, const char *s)
{
    typedef size_t __attribute__((__may_alias__)) word;
    word *wd;
    const word *ws;
    if ((uintptr_t)s % SS == (uintptr_t)d % SS)
    19ca:	00b547b3          	xor	a5,a0,a1
    19ce:	8b9d                	andi	a5,a5,7
    19d0:	eb95                	bnez	a5,1a04 <strcpy+0x3a>
    {
        for (; (uintptr_t)s % SS; s++, d++)
    19d2:	0075f793          	andi	a5,a1,7
    19d6:	e7b1                	bnez	a5,1a22 <strcpy+0x58>
            if (!(*d = *s))
                return d;
        wd = (void *)d;
        ws = (const void *)s;
        for (; !HASZERO(*ws); *wd++ = *ws++)
    19d8:	6198                	ld	a4,0(a1)
    19da:	00000617          	auipc	a2,0x0
    19de:	5ae63603          	ld	a2,1454(a2) # 1f88 <__clone+0xc0>
    19e2:	00000817          	auipc	a6,0x0
    19e6:	5ae83803          	ld	a6,1454(a6) # 1f90 <__clone+0xc8>
    19ea:	a029                	j	19f4 <strcpy+0x2a>
    19ec:	e118                	sd	a4,0(a0)
    19ee:	6598                	ld	a4,8(a1)
    19f0:	05a1                	addi	a1,a1,8
    19f2:	0521                	addi	a0,a0,8
    19f4:	00c707b3          	add	a5,a4,a2
    19f8:	fff74693          	not	a3,a4
    19fc:	8ff5                	and	a5,a5,a3
    19fe:	0107f7b3          	and	a5,a5,a6
    1a02:	d7ed                	beqz	a5,19ec <strcpy+0x22>
            ;
        d = (void *)wd;
        s = (const void *)ws;
    }
    for (; (*d = *s); s++, d++)
    1a04:	0005c783          	lbu	a5,0(a1)
    1a08:	00f50023          	sb	a5,0(a0)
    1a0c:	c785                	beqz	a5,1a34 <strcpy+0x6a>
    1a0e:	0015c783          	lbu	a5,1(a1)
    1a12:	0505                	addi	a0,a0,1
    1a14:	0585                	addi	a1,a1,1
    1a16:	00f50023          	sb	a5,0(a0)
    1a1a:	fbf5                	bnez	a5,1a0e <strcpy+0x44>
        ;
    return d;
}
    1a1c:	8082                	ret
        for (; (uintptr_t)s % SS; s++, d++)
    1a1e:	0505                	addi	a0,a0,1
    1a20:	df45                	beqz	a4,19d8 <strcpy+0xe>
            if (!(*d = *s))
    1a22:	0005c783          	lbu	a5,0(a1)
        for (; (uintptr_t)s % SS; s++, d++)
    1a26:	0585                	addi	a1,a1,1
    1a28:	0075f713          	andi	a4,a1,7
            if (!(*d = *s))
    1a2c:	00f50023          	sb	a5,0(a0)
    1a30:	f7fd                	bnez	a5,1a1e <strcpy+0x54>
}
    1a32:	8082                	ret
    1a34:	8082                	ret

0000000000001a36 <strncpy>:
char *strncpy(char *restrict d, const char *s, size_t n)
{
    typedef size_t __attribute__((__may_alias__)) word;
    word *wd;
    const word *ws;
    if (((uintptr_t)s & ALIGN) == ((uintptr_t)d & ALIGN))
    1a36:	00b547b3          	xor	a5,a0,a1
    1a3a:	8b9d                	andi	a5,a5,7
    1a3c:	1a079863          	bnez	a5,1bec <strncpy+0x1b6>
    {
        for (; ((uintptr_t)s & ALIGN) && n && (*d = *s); n--, s++, d++)
    1a40:	0075f793          	andi	a5,a1,7
    1a44:	16078463          	beqz	a5,1bac <strncpy+0x176>
    1a48:	ea01                	bnez	a2,1a58 <strncpy+0x22>
    1a4a:	a421                	j	1c52 <strncpy+0x21c>
    1a4c:	167d                	addi	a2,a2,-1
    1a4e:	0505                	addi	a0,a0,1
    1a50:	14070e63          	beqz	a4,1bac <strncpy+0x176>
    1a54:	1a060863          	beqz	a2,1c04 <strncpy+0x1ce>
    1a58:	0005c783          	lbu	a5,0(a1)
    1a5c:	0585                	addi	a1,a1,1
    1a5e:	0075f713          	andi	a4,a1,7
    1a62:	00f50023          	sb	a5,0(a0)
    1a66:	f3fd                	bnez	a5,1a4c <strncpy+0x16>
    1a68:	4805                	li	a6,1
    1a6a:	1a061863          	bnez	a2,1c1a <strncpy+0x1e4>
    1a6e:	40a007b3          	neg	a5,a0
    1a72:	8b9d                	andi	a5,a5,7
    1a74:	4681                	li	a3,0
    1a76:	18061a63          	bnez	a2,1c0a <strncpy+0x1d4>
    1a7a:	00778713          	addi	a4,a5,7
    1a7e:	45ad                	li	a1,11
    1a80:	18b76363          	bltu	a4,a1,1c06 <strncpy+0x1d0>
    1a84:	1ae6eb63          	bltu	a3,a4,1c3a <strncpy+0x204>
    1a88:	1a078363          	beqz	a5,1c2e <strncpy+0x1f8>
    for (int i = 0; i < n; ++i, *(p++) = c)
    1a8c:	00050023          	sb	zero,0(a0)
    1a90:	4685                	li	a3,1
    1a92:	00150713          	addi	a4,a0,1
    1a96:	18d78f63          	beq	a5,a3,1c34 <strncpy+0x1fe>
    1a9a:	000500a3          	sb	zero,1(a0)
    1a9e:	4689                	li	a3,2
    1aa0:	00250713          	addi	a4,a0,2
    1aa4:	18d78e63          	beq	a5,a3,1c40 <strncpy+0x20a>
    1aa8:	00050123          	sb	zero,2(a0)
    1aac:	468d                	li	a3,3
    1aae:	00350713          	addi	a4,a0,3
    1ab2:	16d78c63          	beq	a5,a3,1c2a <strncpy+0x1f4>
    1ab6:	000501a3          	sb	zero,3(a0)
    1aba:	4691                	li	a3,4
    1abc:	00450713          	addi	a4,a0,4
    1ac0:	18d78263          	beq	a5,a3,1c44 <strncpy+0x20e>
    1ac4:	00050223          	sb	zero,4(a0)
    1ac8:	4695                	li	a3,5
    1aca:	00550713          	addi	a4,a0,5
    1ace:	16d78d63          	beq	a5,a3,1c48 <strncpy+0x212>
    1ad2:	000502a3          	sb	zero,5(a0)
    1ad6:	469d                	li	a3,7
    1ad8:	00650713          	addi	a4,a0,6
    1adc:	16d79863          	bne	a5,a3,1c4c <strncpy+0x216>
    1ae0:	00750713          	addi	a4,a0,7
    1ae4:	00050323          	sb	zero,6(a0)
    1ae8:	40f80833          	sub	a6,a6,a5
    1aec:	ff887593          	andi	a1,a6,-8
    1af0:	97aa                	add	a5,a5,a0
    1af2:	95be                	add	a1,a1,a5
    1af4:	0007b023          	sd	zero,0(a5)
    1af8:	07a1                	addi	a5,a5,8
    1afa:	feb79de3          	bne	a5,a1,1af4 <strncpy+0xbe>
    1afe:	ff887593          	andi	a1,a6,-8
    1b02:	9ead                	addw	a3,a3,a1
    1b04:	00b707b3          	add	a5,a4,a1
    1b08:	12b80863          	beq	a6,a1,1c38 <strncpy+0x202>
    1b0c:	00078023          	sb	zero,0(a5)
    1b10:	0016871b          	addiw	a4,a3,1
    1b14:	0ec77863          	bgeu	a4,a2,1c04 <strncpy+0x1ce>
    1b18:	000780a3          	sb	zero,1(a5)
    1b1c:	0026871b          	addiw	a4,a3,2
    1b20:	0ec77263          	bgeu	a4,a2,1c04 <strncpy+0x1ce>
    1b24:	00078123          	sb	zero,2(a5)
    1b28:	0036871b          	addiw	a4,a3,3
    1b2c:	0cc77c63          	bgeu	a4,a2,1c04 <strncpy+0x1ce>
    1b30:	000781a3          	sb	zero,3(a5)
    1b34:	0046871b          	addiw	a4,a3,4
    1b38:	0cc77663          	bgeu	a4,a2,1c04 <strncpy+0x1ce>
    1b3c:	00078223          	sb	zero,4(a5)
    1b40:	0056871b          	addiw	a4,a3,5
    1b44:	0cc77063          	bgeu	a4,a2,1c04 <strncpy+0x1ce>
    1b48:	000782a3          	sb	zero,5(a5)
    1b4c:	0066871b          	addiw	a4,a3,6
    1b50:	0ac77a63          	bgeu	a4,a2,1c04 <strncpy+0x1ce>
    1b54:	00078323          	sb	zero,6(a5)
    1b58:	0076871b          	addiw	a4,a3,7
    1b5c:	0ac77463          	bgeu	a4,a2,1c04 <strncpy+0x1ce>
    1b60:	000783a3          	sb	zero,7(a5)
    1b64:	0086871b          	addiw	a4,a3,8
    1b68:	08c77e63          	bgeu	a4,a2,1c04 <strncpy+0x1ce>
    1b6c:	00078423          	sb	zero,8(a5)
    1b70:	0096871b          	addiw	a4,a3,9
    1b74:	08c77863          	bgeu	a4,a2,1c04 <strncpy+0x1ce>
    1b78:	000784a3          	sb	zero,9(a5)
    1b7c:	00a6871b          	addiw	a4,a3,10
    1b80:	08c77263          	bgeu	a4,a2,1c04 <strncpy+0x1ce>
    1b84:	00078523          	sb	zero,10(a5)
    1b88:	00b6871b          	addiw	a4,a3,11
    1b8c:	06c77c63          	bgeu	a4,a2,1c04 <strncpy+0x1ce>
    1b90:	000785a3          	sb	zero,11(a5)
    1b94:	00c6871b          	addiw	a4,a3,12
    1b98:	06c77663          	bgeu	a4,a2,1c04 <strncpy+0x1ce>
    1b9c:	00078623          	sb	zero,12(a5)
    1ba0:	26b5                	addiw	a3,a3,13
    1ba2:	06c6f163          	bgeu	a3,a2,1c04 <strncpy+0x1ce>
    1ba6:	000786a3          	sb	zero,13(a5)
    1baa:	8082                	ret
            ;
        if (!n || !*s)
    1bac:	c645                	beqz	a2,1c54 <strncpy+0x21e>
    1bae:	0005c783          	lbu	a5,0(a1)
    1bb2:	ea078be3          	beqz	a5,1a68 <strncpy+0x32>
            goto tail;
        wd = (void *)d;
        ws = (const void *)s;
        for (; n >= sizeof(size_t) && !HASZERO(*ws); n -= sizeof(size_t), ws++, wd++)
    1bb6:	479d                	li	a5,7
    1bb8:	02c7ff63          	bgeu	a5,a2,1bf6 <strncpy+0x1c0>
    1bbc:	00000897          	auipc	a7,0x0
    1bc0:	3cc8b883          	ld	a7,972(a7) # 1f88 <__clone+0xc0>
    1bc4:	00000817          	auipc	a6,0x0
    1bc8:	3cc83803          	ld	a6,972(a6) # 1f90 <__clone+0xc8>
    1bcc:	431d                	li	t1,7
    1bce:	6198                	ld	a4,0(a1)
    1bd0:	011707b3          	add	a5,a4,a7
    1bd4:	fff74693          	not	a3,a4
    1bd8:	8ff5                	and	a5,a5,a3
    1bda:	0107f7b3          	and	a5,a5,a6
    1bde:	ef81                	bnez	a5,1bf6 <strncpy+0x1c0>
            *wd = *ws;
    1be0:	e118                	sd	a4,0(a0)
        for (; n >= sizeof(size_t) && !HASZERO(*ws); n -= sizeof(size_t), ws++, wd++)
    1be2:	1661                	addi	a2,a2,-8
    1be4:	05a1                	addi	a1,a1,8
    1be6:	0521                	addi	a0,a0,8
    1be8:	fec363e3          	bltu	t1,a2,1bce <strncpy+0x198>
        d = (void *)wd;
        s = (const void *)ws;
    }
    for (; n && (*d = *s); n--, s++, d++)
    1bec:	e609                	bnez	a2,1bf6 <strncpy+0x1c0>
    1bee:	a08d                	j	1c50 <strncpy+0x21a>
    1bf0:	167d                	addi	a2,a2,-1
    1bf2:	0505                	addi	a0,a0,1
    1bf4:	ca01                	beqz	a2,1c04 <strncpy+0x1ce>
    1bf6:	0005c783          	lbu	a5,0(a1)
    1bfa:	0585                	addi	a1,a1,1
    1bfc:	00f50023          	sb	a5,0(a0)
    1c00:	fbe5                	bnez	a5,1bf0 <strncpy+0x1ba>
        ;
tail:
    1c02:	b59d                	j	1a68 <strncpy+0x32>
    memset(d, 0, n);
    return d;
}
    1c04:	8082                	ret
    1c06:	472d                	li	a4,11
    1c08:	bdb5                	j	1a84 <strncpy+0x4e>
    1c0a:	00778713          	addi	a4,a5,7
    1c0e:	45ad                	li	a1,11
    1c10:	fff60693          	addi	a3,a2,-1
    1c14:	e6b778e3          	bgeu	a4,a1,1a84 <strncpy+0x4e>
    1c18:	b7fd                	j	1c06 <strncpy+0x1d0>
    1c1a:	40a007b3          	neg	a5,a0
    1c1e:	8832                	mv	a6,a2
    1c20:	8b9d                	andi	a5,a5,7
    1c22:	4681                	li	a3,0
    1c24:	e4060be3          	beqz	a2,1a7a <strncpy+0x44>
    1c28:	b7cd                	j	1c0a <strncpy+0x1d4>
    for (int i = 0; i < n; ++i, *(p++) = c)
    1c2a:	468d                	li	a3,3
    1c2c:	bd75                	j	1ae8 <strncpy+0xb2>
    1c2e:	872a                	mv	a4,a0
    1c30:	4681                	li	a3,0
    1c32:	bd5d                	j	1ae8 <strncpy+0xb2>
    1c34:	4685                	li	a3,1
    1c36:	bd4d                	j	1ae8 <strncpy+0xb2>
    1c38:	8082                	ret
    1c3a:	87aa                	mv	a5,a0
    1c3c:	4681                	li	a3,0
    1c3e:	b5f9                	j	1b0c <strncpy+0xd6>
    1c40:	4689                	li	a3,2
    1c42:	b55d                	j	1ae8 <strncpy+0xb2>
    1c44:	4691                	li	a3,4
    1c46:	b54d                	j	1ae8 <strncpy+0xb2>
    1c48:	4695                	li	a3,5
    1c4a:	bd79                	j	1ae8 <strncpy+0xb2>
    1c4c:	4699                	li	a3,6
    1c4e:	bd69                	j	1ae8 <strncpy+0xb2>
    1c50:	8082                	ret
    1c52:	8082                	ret
    1c54:	8082                	ret

0000000000001c56 <open>:
#include <unistd.h>

#include "syscall.h"

int open(const char *path, int flags)
{
    1c56:	87aa                	mv	a5,a0
    1c58:	862e                	mv	a2,a1
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
}

static inline long __syscall4(long n, long a, long b, long c, long d)
{
    register long a7 __asm__("a7") = n;
    1c5a:	03800893          	li	a7,56
    register long a0 __asm__("a0") = a;
    1c5e:	f9c00513          	li	a0,-100
    register long a1 __asm__("a1") = b;
    1c62:	85be                	mv	a1,a5
    register long a2 __asm__("a2") = c;
    register long a3 __asm__("a3") = d;
    1c64:	4689                	li	a3,2
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3))
    1c66:	00000073          	ecall
    return syscall(SYS_openat, AT_FDCWD, path, flags, O_RDWR);
}
    1c6a:	2501                	sext.w	a0,a0
    1c6c:	8082                	ret

0000000000001c6e <openat>:
    register long a7 __asm__("a7") = n;
    1c6e:	03800893          	li	a7,56
    register long a3 __asm__("a3") = d;
    1c72:	18000693          	li	a3,384
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3))
    1c76:	00000073          	ecall

int openat(int dirfd,const char *path, int flags)
{
    return syscall(SYS_openat, dirfd, path, flags, 0600);
}
    1c7a:	2501                	sext.w	a0,a0
    1c7c:	8082                	ret

0000000000001c7e <close>:
    register long a7 __asm__("a7") = n;
    1c7e:	03900893          	li	a7,57
    __asm_syscall("r"(a7), "0"(a0))
    1c82:	00000073          	ecall

int close(int fd)
{
    return syscall(SYS_close, fd);
}
    1c86:	2501                	sext.w	a0,a0
    1c88:	8082                	ret

0000000000001c8a <read>:
    register long a7 __asm__("a7") = n;
    1c8a:	03f00893          	li	a7,63
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1c8e:	00000073          	ecall

ssize_t read(int fd, void *buf, size_t len)
{
    return syscall(SYS_read, fd, buf, len);
}
    1c92:	8082                	ret

0000000000001c94 <write>:
    register long a7 __asm__("a7") = n;
    1c94:	04000893          	li	a7,64
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1c98:	00000073          	ecall

ssize_t write(int fd, const void *buf, size_t len)
{
    return syscall(SYS_write, fd, buf, len);
}
    1c9c:	8082                	ret

0000000000001c9e <getpid>:
    register long a7 __asm__("a7") = n;
    1c9e:	0ac00893          	li	a7,172
    __asm_syscall("r"(a7))
    1ca2:	00000073          	ecall

pid_t getpid(void)
{
    return syscall(SYS_getpid);
}
    1ca6:	2501                	sext.w	a0,a0
    1ca8:	8082                	ret

0000000000001caa <getppid>:
    register long a7 __asm__("a7") = n;
    1caa:	0ad00893          	li	a7,173
    __asm_syscall("r"(a7))
    1cae:	00000073          	ecall

pid_t getppid(void)
{
    return syscall(SYS_getppid);
}
    1cb2:	2501                	sext.w	a0,a0
    1cb4:	8082                	ret

0000000000001cb6 <sched_yield>:
    register long a7 __asm__("a7") = n;
    1cb6:	07c00893          	li	a7,124
    __asm_syscall("r"(a7))
    1cba:	00000073          	ecall

int sched_yield(void)
{
    return syscall(SYS_sched_yield);
}
    1cbe:	2501                	sext.w	a0,a0
    1cc0:	8082                	ret

0000000000001cc2 <fork>:
    register long a7 __asm__("a7") = n;
    1cc2:	0dc00893          	li	a7,220
    register long a0 __asm__("a0") = a;
    1cc6:	4545                	li	a0,17
    register long a1 __asm__("a1") = b;
    1cc8:	4581                	li	a1,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1cca:	00000073          	ecall

pid_t fork(void)
{
    return syscall(SYS_clone, SIGCHLD, 0);
}
    1cce:	2501                	sext.w	a0,a0
    1cd0:	8082                	ret

0000000000001cd2 <clone>:

pid_t clone(int (*fn)(void *arg), void *arg, void *stack, size_t stack_size, unsigned long flags)
{
    1cd2:	85b2                	mv	a1,a2
    1cd4:	863a                	mv	a2,a4
    if (stack)
    1cd6:	c191                	beqz	a1,1cda <clone+0x8>
	stack += stack_size;
    1cd8:	95b6                	add	a1,a1,a3

    return __clone(fn, stack, flags, NULL, NULL, NULL);
    1cda:	4781                	li	a5,0
    1cdc:	4701                	li	a4,0
    1cde:	4681                	li	a3,0
    1ce0:	2601                	sext.w	a2,a2
    1ce2:	a2dd                	j	1ec8 <__clone>

0000000000001ce4 <exit>:
    register long a7 __asm__("a7") = n;
    1ce4:	05d00893          	li	a7,93
    __asm_syscall("r"(a7), "0"(a0))
    1ce8:	00000073          	ecall
    //return syscall(SYS_clone, fn, stack, flags, NULL, NULL, NULL);
}
void exit(int code)
{
    syscall(SYS_exit, code);
}
    1cec:	8082                	ret

0000000000001cee <waitpid>:
    register long a7 __asm__("a7") = n;
    1cee:	10400893          	li	a7,260
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1cf2:	00000073          	ecall

int waitpid(int pid, int *code, int options)
{
    return syscall(SYS_wait4, pid, code, options);
}
    1cf6:	2501                	sext.w	a0,a0
    1cf8:	8082                	ret

0000000000001cfa <exec>:
    register long a7 __asm__("a7") = n;
    1cfa:	0dd00893          	li	a7,221
    __asm_syscall("r"(a7), "0"(a0))
    1cfe:	00000073          	ecall

int exec(char *name)
{
    return syscall(SYS_execve, name);
}
    1d02:	2501                	sext.w	a0,a0
    1d04:	8082                	ret

0000000000001d06 <execve>:
    register long a7 __asm__("a7") = n;
    1d06:	0dd00893          	li	a7,221
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1d0a:	00000073          	ecall

int execve(const char *name, char *const argv[], char *const argp[])
{
    return syscall(SYS_execve, name, argv, argp);
}
    1d0e:	2501                	sext.w	a0,a0
    1d10:	8082                	ret

0000000000001d12 <times>:
    register long a7 __asm__("a7") = n;
    1d12:	09900893          	li	a7,153
    __asm_syscall("r"(a7), "0"(a0))
    1d16:	00000073          	ecall

int times(void *mytimes)
{
	return syscall(SYS_times, mytimes);
}
    1d1a:	2501                	sext.w	a0,a0
    1d1c:	8082                	ret

0000000000001d1e <get_time>:

int64 get_time()
{
    1d1e:	1141                	addi	sp,sp,-16
    register long a7 __asm__("a7") = n;
    1d20:	0a900893          	li	a7,169
    register long a0 __asm__("a0") = a;
    1d24:	850a                	mv	a0,sp
    register long a1 __asm__("a1") = b;
    1d26:	4581                	li	a1,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1d28:	00000073          	ecall
    TimeVal time;
    int err = sys_get_time(&time, 0);
    if (err == 0)
    1d2c:	2501                	sext.w	a0,a0
    1d2e:	ed09                	bnez	a0,1d48 <get_time+0x2a>
    {
        return ((time.sec & 0xffff) * 1000 + time.usec / 1000);
    1d30:	67a2                	ld	a5,8(sp)
    1d32:	3e800713          	li	a4,1000
    1d36:	00015503          	lhu	a0,0(sp)
    1d3a:	02e7d7b3          	divu	a5,a5,a4
    1d3e:	02e50533          	mul	a0,a0,a4
    1d42:	953e                	add	a0,a0,a5
    }
    else
    {
        return -1;
    }
}
    1d44:	0141                	addi	sp,sp,16
    1d46:	8082                	ret
        return -1;
    1d48:	557d                	li	a0,-1
    1d4a:	bfed                	j	1d44 <get_time+0x26>

0000000000001d4c <sys_get_time>:
    register long a7 __asm__("a7") = n;
    1d4c:	0a900893          	li	a7,169
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1d50:	00000073          	ecall

int sys_get_time(TimeVal *ts, int tz)
{
    return syscall(SYS_gettimeofday, ts, tz);
}
    1d54:	2501                	sext.w	a0,a0
    1d56:	8082                	ret

0000000000001d58 <time>:
    register long a7 __asm__("a7") = n;
    1d58:	42600893          	li	a7,1062
    __asm_syscall("r"(a7), "0"(a0))
    1d5c:	00000073          	ecall

int time(unsigned long *tloc)
{
    return syscall(SYS_time, tloc);
}
    1d60:	2501                	sext.w	a0,a0
    1d62:	8082                	ret

0000000000001d64 <sleep>:

int sleep(unsigned long long time)
{
    1d64:	1141                	addi	sp,sp,-16
    TimeVal tv = {.sec = time, .usec = 0};
    1d66:	e02a                	sd	a0,0(sp)
    register long a0 __asm__("a0") = a;
    1d68:	850a                	mv	a0,sp
    1d6a:	e402                	sd	zero,8(sp)
    register long a7 __asm__("a7") = n;
    1d6c:	06500893          	li	a7,101
    register long a1 __asm__("a1") = b;
    1d70:	85aa                	mv	a1,a0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1d72:	00000073          	ecall
    if (syscall(SYS_nanosleep, &tv, &tv)) return tv.sec;
    1d76:	e501                	bnez	a0,1d7e <sleep+0x1a>
    return 0;
    1d78:	4501                	li	a0,0
}
    1d7a:	0141                	addi	sp,sp,16
    1d7c:	8082                	ret
    if (syscall(SYS_nanosleep, &tv, &tv)) return tv.sec;
    1d7e:	4502                	lw	a0,0(sp)
}
    1d80:	0141                	addi	sp,sp,16
    1d82:	8082                	ret

0000000000001d84 <set_priority>:
    register long a7 __asm__("a7") = n;
    1d84:	08c00893          	li	a7,140
    __asm_syscall("r"(a7), "0"(a0))
    1d88:	00000073          	ecall

int set_priority(int prio)
{
    return syscall(SYS_setpriority, prio);
}
    1d8c:	2501                	sext.w	a0,a0
    1d8e:	8082                	ret

0000000000001d90 <mmap>:
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4))
}

static inline long __syscall6(long n, long a, long b, long c, long d, long e, long f)
{
    register long a7 __asm__("a7") = n;
    1d90:	0de00893          	li	a7,222
    register long a1 __asm__("a1") = b;
    register long a2 __asm__("a2") = c;
    register long a3 __asm__("a3") = d;
    register long a4 __asm__("a4") = e;
    register long a5 __asm__("a5") = f;
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4), "r"(a5))
    1d94:	00000073          	ecall

void *mmap(void *start, size_t len, int prot, int flags, int fd, off_t off)
{
    return syscall(SYS_mmap, start, len, prot, flags, fd, off);
}
    1d98:	8082                	ret

0000000000001d9a <munmap>:
    register long a7 __asm__("a7") = n;
    1d9a:	0d700893          	li	a7,215
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1d9e:	00000073          	ecall

int munmap(void *start, size_t len)
{
    return syscall(SYS_munmap, start, len);
}
    1da2:	2501                	sext.w	a0,a0
    1da4:	8082                	ret

0000000000001da6 <wait>:

int wait(int *code)
{
    1da6:	85aa                	mv	a1,a0
    register long a7 __asm__("a7") = n;
    1da8:	10400893          	li	a7,260
    register long a0 __asm__("a0") = a;
    1dac:	557d                	li	a0,-1
    register long a2 __asm__("a2") = c;
    1dae:	4601                	li	a2,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1db0:	00000073          	ecall
    return waitpid(-1, code, 0);
}
    1db4:	2501                	sext.w	a0,a0
    1db6:	8082                	ret

0000000000001db8 <spawn>:
    register long a7 __asm__("a7") = n;
    1db8:	19000893          	li	a7,400
    __asm_syscall("r"(a7), "0"(a0))
    1dbc:	00000073          	ecall

int spawn(char *file)
{
    return syscall(SYS_spawn, file);
}
    1dc0:	2501                	sext.w	a0,a0
    1dc2:	8082                	ret

0000000000001dc4 <mailread>:
    register long a7 __asm__("a7") = n;
    1dc4:	19100893          	li	a7,401
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1dc8:	00000073          	ecall

int mailread(void *buf, int len)
{
    return syscall(SYS_mailread, buf, len);
}
    1dcc:	2501                	sext.w	a0,a0
    1dce:	8082                	ret

0000000000001dd0 <mailwrite>:
    register long a7 __asm__("a7") = n;
    1dd0:	19200893          	li	a7,402
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1dd4:	00000073          	ecall

int mailwrite(int pid, void *buf, int len)
{
    return syscall(SYS_mailwrite, pid, buf, len);
}
    1dd8:	2501                	sext.w	a0,a0
    1dda:	8082                	ret

0000000000001ddc <fstat>:
    register long a7 __asm__("a7") = n;
    1ddc:	05000893          	li	a7,80
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1de0:	00000073          	ecall

int fstat(int fd, struct kstat *st)
{
    return syscall(SYS_fstat, fd, st);
}
    1de4:	2501                	sext.w	a0,a0
    1de6:	8082                	ret

0000000000001de8 <sys_linkat>:
    register long a4 __asm__("a4") = e;
    1de8:	1702                	slli	a4,a4,0x20
    register long a7 __asm__("a7") = n;
    1dea:	02500893          	li	a7,37
    register long a4 __asm__("a4") = e;
    1dee:	9301                	srli	a4,a4,0x20
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4))
    1df0:	00000073          	ecall

int sys_linkat(int olddirfd, char *oldpath, int newdirfd, char *newpath, unsigned int flags)
{
    return syscall(SYS_linkat, olddirfd, oldpath, newdirfd, newpath, flags);
}
    1df4:	2501                	sext.w	a0,a0
    1df6:	8082                	ret

0000000000001df8 <sys_unlinkat>:
    register long a2 __asm__("a2") = c;
    1df8:	1602                	slli	a2,a2,0x20
    register long a7 __asm__("a7") = n;
    1dfa:	02300893          	li	a7,35
    register long a2 __asm__("a2") = c;
    1dfe:	9201                	srli	a2,a2,0x20
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1e00:	00000073          	ecall

int sys_unlinkat(int dirfd, char *path, unsigned int flags)
{
    return syscall(SYS_unlinkat, dirfd, path, flags);
}
    1e04:	2501                	sext.w	a0,a0
    1e06:	8082                	ret

0000000000001e08 <link>:

int link(char *old_path, char *new_path)
{
    1e08:	87aa                	mv	a5,a0
    1e0a:	86ae                	mv	a3,a1
    register long a7 __asm__("a7") = n;
    1e0c:	02500893          	li	a7,37
    register long a0 __asm__("a0") = a;
    1e10:	f9c00513          	li	a0,-100
    register long a1 __asm__("a1") = b;
    1e14:	85be                	mv	a1,a5
    register long a2 __asm__("a2") = c;
    1e16:	f9c00613          	li	a2,-100
    register long a4 __asm__("a4") = e;
    1e1a:	4701                	li	a4,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4))
    1e1c:	00000073          	ecall
    return sys_linkat(AT_FDCWD, old_path, AT_FDCWD, new_path, 0);
}
    1e20:	2501                	sext.w	a0,a0
    1e22:	8082                	ret

0000000000001e24 <unlink>:

int unlink(char *path)
{
    1e24:	85aa                	mv	a1,a0
    register long a7 __asm__("a7") = n;
    1e26:	02300893          	li	a7,35
    register long a0 __asm__("a0") = a;
    1e2a:	f9c00513          	li	a0,-100
    register long a2 __asm__("a2") = c;
    1e2e:	4601                	li	a2,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1e30:	00000073          	ecall
    return sys_unlinkat(AT_FDCWD, path, 0);
}
    1e34:	2501                	sext.w	a0,a0
    1e36:	8082                	ret

0000000000001e38 <uname>:
    register long a7 __asm__("a7") = n;
    1e38:	0a000893          	li	a7,160
    __asm_syscall("r"(a7), "0"(a0))
    1e3c:	00000073          	ecall

int uname(void *buf)
{
    return syscall(SYS_uname, buf);
}
    1e40:	2501                	sext.w	a0,a0
    1e42:	8082                	ret

0000000000001e44 <brk>:
    register long a7 __asm__("a7") = n;
    1e44:	0d600893          	li	a7,214
    __asm_syscall("r"(a7), "0"(a0))
    1e48:	00000073          	ecall

int brk(void *addr)
{
    return syscall(SYS_brk, addr);
}
    1e4c:	2501                	sext.w	a0,a0
    1e4e:	8082                	ret

0000000000001e50 <getcwd>:
    register long a7 __asm__("a7") = n;
    1e50:	48c5                	li	a7,17
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1e52:	00000073          	ecall

char *getcwd(char *buf, size_t size){
    return syscall(SYS_getcwd, buf, size);
}
    1e56:	8082                	ret

0000000000001e58 <chdir>:
    register long a7 __asm__("a7") = n;
    1e58:	03100893          	li	a7,49
    __asm_syscall("r"(a7), "0"(a0))
    1e5c:	00000073          	ecall

int chdir(const char *path){
    return syscall(SYS_chdir, path);
}
    1e60:	2501                	sext.w	a0,a0
    1e62:	8082                	ret

0000000000001e64 <mkdir>:

int mkdir(const char *path, mode_t mode){
    1e64:	862e                	mv	a2,a1
    1e66:	87aa                	mv	a5,a0
    register long a2 __asm__("a2") = c;
    1e68:	1602                	slli	a2,a2,0x20
    register long a7 __asm__("a7") = n;
    1e6a:	02200893          	li	a7,34
    register long a0 __asm__("a0") = a;
    1e6e:	f9c00513          	li	a0,-100
    register long a1 __asm__("a1") = b;
    1e72:	85be                	mv	a1,a5
    register long a2 __asm__("a2") = c;
    1e74:	9201                	srli	a2,a2,0x20
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1e76:	00000073          	ecall
    return syscall(SYS_mkdirat, AT_FDCWD, path, mode);
}
    1e7a:	2501                	sext.w	a0,a0
    1e7c:	8082                	ret

0000000000001e7e <getdents>:
    register long a7 __asm__("a7") = n;
    1e7e:	03d00893          	li	a7,61
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1e82:	00000073          	ecall

int getdents(int fd, struct linux_dirent64 *dirp64, unsigned long len){
    //return syscall(SYS_getdents64, fd, dirp64, len);
    return syscall(SYS_getdents64, fd, dirp64, len);
}
    1e86:	2501                	sext.w	a0,a0
    1e88:	8082                	ret

0000000000001e8a <pipe>:
    register long a7 __asm__("a7") = n;
    1e8a:	03b00893          	li	a7,59
    register long a1 __asm__("a1") = b;
    1e8e:	4581                	li	a1,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1e90:	00000073          	ecall

int pipe(int fd[2]){
    return syscall(SYS_pipe2, fd, 0);
}
    1e94:	2501                	sext.w	a0,a0
    1e96:	8082                	ret

0000000000001e98 <dup>:
    register long a7 __asm__("a7") = n;
    1e98:	48dd                	li	a7,23
    __asm_syscall("r"(a7), "0"(a0))
    1e9a:	00000073          	ecall

int dup(int fd){
    return syscall(SYS_dup, fd);
}
    1e9e:	2501                	sext.w	a0,a0
    1ea0:	8082                	ret

0000000000001ea2 <dup2>:
    register long a7 __asm__("a7") = n;
    1ea2:	48e1                	li	a7,24
    register long a2 __asm__("a2") = c;
    1ea4:	4601                	li	a2,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1ea6:	00000073          	ecall

int dup2(int old, int new){
    return syscall(SYS_dup3, old, new, 0);
}
    1eaa:	2501                	sext.w	a0,a0
    1eac:	8082                	ret

0000000000001eae <mount>:
    register long a7 __asm__("a7") = n;
    1eae:	02800893          	li	a7,40
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4))
    1eb2:	00000073          	ecall

int mount(const char *special, const char *dir, const char *fstype, unsigned long flags, const void *data)
{
        return syscall(SYS_mount, special, dir, fstype, flags, data);
}
    1eb6:	2501                	sext.w	a0,a0
    1eb8:	8082                	ret

0000000000001eba <umount>:
    register long a7 __asm__("a7") = n;
    1eba:	02700893          	li	a7,39
    register long a1 __asm__("a1") = b;
    1ebe:	4581                	li	a1,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1ec0:	00000073          	ecall

int umount(const char *special)
{
        return syscall(SYS_umount2, special, 0);
}
    1ec4:	2501                	sext.w	a0,a0
    1ec6:	8082                	ret

0000000000001ec8 <__clone>:

.global __clone
.type  __clone, %function
__clone:
	# Save func and arg to stack
	addi a1, a1, -16
    1ec8:	15c1                	addi	a1,a1,-16
	sd a0, 0(a1)
    1eca:	e188                	sd	a0,0(a1)
	sd a3, 8(a1)
    1ecc:	e594                	sd	a3,8(a1)

	# Call SYS_clone
	mv a0, a2
    1ece:	8532                	mv	a0,a2
	mv a2, a4
    1ed0:	863a                	mv	a2,a4
	mv a3, a5
    1ed2:	86be                	mv	a3,a5
	mv a4, a6
    1ed4:	8742                	mv	a4,a6
	li a7, 220 # SYS_clone
    1ed6:	0dc00893          	li	a7,220
	ecall
    1eda:	00000073          	ecall

	beqz a0, 1f
    1ede:	c111                	beqz	a0,1ee2 <__clone+0x1a>
	# Parent
	ret
    1ee0:	8082                	ret

	# Child
1:      ld a1, 0(sp)
    1ee2:	6582                	ld	a1,0(sp)
	ld a0, 8(sp)
    1ee4:	6522                	ld	a0,8(sp)
	jalr a1
    1ee6:	9582                	jalr	a1

	# Exit
	li a7, 93 # SYS_exit
    1ee8:	05d00893          	li	a7,93
	ecall
    1eec:	00000073          	ecall
