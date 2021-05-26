
/home/ubuntu/rust/ultra-os/codes/riscv-syscalls-testing/user/build/riscv64/mkdir_:     file format elf64-littleriscv


Disassembly of section .text:

0000000000001000 <_start>:
.section .text.entry
.globl _start
_start:
    mv a0, sp
    1000:	850a                	mv	a0,sp
    tail __start_main
    1002:	a0f9                	j	10d0 <__start_main>

0000000000001004 <test_mkdir>:
#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"

void test_mkdir(void){
    1004:	1141                	addi	sp,sp,-16
    TEST_START(__func__);
    1006:	00001517          	auipc	a0,0x1
    100a:	f0250513          	addi	a0,a0,-254 # 1f08 <__clone+0x2a>
void test_mkdir(void){
    100e:	e406                	sd	ra,8(sp)
    1010:	e022                	sd	s0,0(sp)
    TEST_START(__func__);
    1012:	330000ef          	jal	ra,1342 <puts>
    1016:	00001517          	auipc	a0,0x1
    101a:	fb250513          	addi	a0,a0,-78 # 1fc8 <__func__.0>
    101e:	324000ef          	jal	ra,1342 <puts>
    1022:	00001517          	auipc	a0,0x1
    1026:	efe50513          	addi	a0,a0,-258 # 1f20 <__clone+0x42>
    102a:	318000ef          	jal	ra,1342 <puts>
    int rt, fd;

    rt = mkdir("test_mkdir", 0666);
    102e:	1b600593          	li	a1,438
    1032:	00001517          	auipc	a0,0x1
    1036:	efe50513          	addi	a0,a0,-258 # 1f30 <__clone+0x52>
    103a:	641000ef          	jal	ra,1e7a <mkdir>
    printf("mkdir ret: %d\n", rt);
    103e:	85aa                	mv	a1,a0
    rt = mkdir("test_mkdir", 0666);
    1040:	842a                	mv	s0,a0
    printf("mkdir ret: %d\n", rt);
    1042:	00001517          	auipc	a0,0x1
    1046:	efe50513          	addi	a0,a0,-258 # 1f40 <__clone+0x62>
    104a:	31a000ef          	jal	ra,1364 <printf>
    assert(rt != -1);
    104e:	57fd                	li	a5,-1
    1050:	06f40163          	beq	s0,a5,10b2 <test_mkdir+0xae>
    fd = open("test_mkdir", O_RDONLY | O_DIRECTORY);
    1054:	002005b7          	lui	a1,0x200
    1058:	00001517          	auipc	a0,0x1
    105c:	ed850513          	addi	a0,a0,-296 # 1f30 <__clone+0x52>
    1060:	40d000ef          	jal	ra,1c6c <open>
    1064:	842a                	mv	s0,a0
    if(fd > 0){
    1066:	02a05f63          	blez	a0,10a4 <test_mkdir+0xa0>
        printf("  mkdir success.\n");
    106a:	00001517          	auipc	a0,0x1
    106e:	f0650513          	addi	a0,a0,-250 # 1f70 <__clone+0x92>
    1072:	2f2000ef          	jal	ra,1364 <printf>
        close(fd);
    1076:	8522                	mv	a0,s0
    1078:	41d000ef          	jal	ra,1c94 <close>
    }
    else printf("  mkdir error.\n");
    TEST_END(__func__);
    107c:	00001517          	auipc	a0,0x1
    1080:	f1c50513          	addi	a0,a0,-228 # 1f98 <__clone+0xba>
    1084:	2be000ef          	jal	ra,1342 <puts>
    1088:	00001517          	auipc	a0,0x1
    108c:	f4050513          	addi	a0,a0,-192 # 1fc8 <__func__.0>
    1090:	2b2000ef          	jal	ra,1342 <puts>
}
    1094:	6402                	ld	s0,0(sp)
    1096:	60a2                	ld	ra,8(sp)
    TEST_END(__func__);
    1098:	00001517          	auipc	a0,0x1
    109c:	e8850513          	addi	a0,a0,-376 # 1f20 <__clone+0x42>
}
    10a0:	0141                	addi	sp,sp,16
    TEST_END(__func__);
    10a2:	a445                	j	1342 <puts>
    else printf("  mkdir error.\n");
    10a4:	00001517          	auipc	a0,0x1
    10a8:	ee450513          	addi	a0,a0,-284 # 1f88 <__clone+0xaa>
    10ac:	2b8000ef          	jal	ra,1364 <printf>
    10b0:	b7f1                	j	107c <test_mkdir+0x78>
    assert(rt != -1);
    10b2:	00001517          	auipc	a0,0x1
    10b6:	e9e50513          	addi	a0,a0,-354 # 1f50 <__clone+0x72>
    10ba:	52e000ef          	jal	ra,15e8 <panic>
    10be:	bf59                	j	1054 <test_mkdir+0x50>

00000000000010c0 <main>:

int main(void){
    10c0:	1141                	addi	sp,sp,-16
    10c2:	e406                	sd	ra,8(sp)
    test_mkdir();
    10c4:	f41ff0ef          	jal	ra,1004 <test_mkdir>
    return 0;
}
    10c8:	60a2                	ld	ra,8(sp)
    10ca:	4501                	li	a0,0
    10cc:	0141                	addi	sp,sp,16
    10ce:	8082                	ret

00000000000010d0 <__start_main>:
#include <unistd.h>

extern int main();

int __start_main(long *p)
{
    10d0:	85aa                	mv	a1,a0
	int argc = p[0];
	char **argv = (void *)(p+1);

	exit(main(argc, argv));
    10d2:	4108                	lw	a0,0(a0)
{
    10d4:	1141                	addi	sp,sp,-16
	exit(main(argc, argv));
    10d6:	05a1                	addi	a1,a1,8
{
    10d8:	e406                	sd	ra,8(sp)
	exit(main(argc, argv));
    10da:	fe7ff0ef          	jal	ra,10c0 <main>
    10de:	41d000ef          	jal	ra,1cfa <exit>
	return 0;
}
    10e2:	60a2                	ld	ra,8(sp)
    10e4:	4501                	li	a0,0
    10e6:	0141                	addi	sp,sp,16
    10e8:	8082                	ret

00000000000010ea <printint.constprop.0>:
    write(f, s, l);
}

static char digits[] = "0123456789abcdef";

static void printint(int xx, int base, int sign)
    10ea:	7179                	addi	sp,sp,-48
    10ec:	f406                	sd	ra,40(sp)
{
    char buf[16 + 1];
    int i;
    uint x;

    if (sign && (sign = xx < 0))
    10ee:	12054b63          	bltz	a0,1224 <printint.constprop.0+0x13a>

    buf[16] = 0;
    i = 15;
    do
    {
        buf[i--] = digits[x % base];
    10f2:	02b577bb          	remuw	a5,a0,a1
    10f6:	00001617          	auipc	a2,0x1
    10fa:	ee260613          	addi	a2,a2,-286 # 1fd8 <digits>
    buf[16] = 0;
    10fe:	00010c23          	sb	zero,24(sp)
        buf[i--] = digits[x % base];
    1102:	0005871b          	sext.w	a4,a1
    1106:	1782                	slli	a5,a5,0x20
    1108:	9381                	srli	a5,a5,0x20
    110a:	97b2                	add	a5,a5,a2
    110c:	0007c783          	lbu	a5,0(a5)
    } while ((x /= base) != 0);
    1110:	02b5583b          	divuw	a6,a0,a1
        buf[i--] = digits[x % base];
    1114:	00f10ba3          	sb	a5,23(sp)
    } while ((x /= base) != 0);
    1118:	1cb56363          	bltu	a0,a1,12de <printint.constprop.0+0x1f4>
        buf[i--] = digits[x % base];
    111c:	45b9                	li	a1,14
    111e:	02e877bb          	remuw	a5,a6,a4
    1122:	1782                	slli	a5,a5,0x20
    1124:	9381                	srli	a5,a5,0x20
    1126:	97b2                	add	a5,a5,a2
    1128:	0007c783          	lbu	a5,0(a5)
    } while ((x /= base) != 0);
    112c:	02e856bb          	divuw	a3,a6,a4
        buf[i--] = digits[x % base];
    1130:	00f10b23          	sb	a5,22(sp)
    } while ((x /= base) != 0);
    1134:	0ce86e63          	bltu	a6,a4,1210 <printint.constprop.0+0x126>
        buf[i--] = digits[x % base];
    1138:	02e6f5bb          	remuw	a1,a3,a4
    } while ((x /= base) != 0);
    113c:	02e6d7bb          	divuw	a5,a3,a4
        buf[i--] = digits[x % base];
    1140:	1582                	slli	a1,a1,0x20
    1142:	9181                	srli	a1,a1,0x20
    1144:	95b2                	add	a1,a1,a2
    1146:	0005c583          	lbu	a1,0(a1) # 200000 <digits+0x1fe028>
    114a:	00b10aa3          	sb	a1,21(sp)
    } while ((x /= base) != 0);
    114e:	0007859b          	sext.w	a1,a5
    1152:	12e6ec63          	bltu	a3,a4,128a <printint.constprop.0+0x1a0>
        buf[i--] = digits[x % base];
    1156:	02e7f6bb          	remuw	a3,a5,a4
    115a:	1682                	slli	a3,a3,0x20
    115c:	9281                	srli	a3,a3,0x20
    115e:	96b2                	add	a3,a3,a2
    1160:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    1164:	02e7d83b          	divuw	a6,a5,a4
        buf[i--] = digits[x % base];
    1168:	00d10a23          	sb	a3,20(sp)
    } while ((x /= base) != 0);
    116c:	12e5e863          	bltu	a1,a4,129c <printint.constprop.0+0x1b2>
        buf[i--] = digits[x % base];
    1170:	02e876bb          	remuw	a3,a6,a4
    1174:	1682                	slli	a3,a3,0x20
    1176:	9281                	srli	a3,a3,0x20
    1178:	96b2                	add	a3,a3,a2
    117a:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    117e:	02e855bb          	divuw	a1,a6,a4
        buf[i--] = digits[x % base];
    1182:	00d109a3          	sb	a3,19(sp)
    } while ((x /= base) != 0);
    1186:	12e86463          	bltu	a6,a4,12ae <printint.constprop.0+0x1c4>
        buf[i--] = digits[x % base];
    118a:	02e5f6bb          	remuw	a3,a1,a4
    118e:	1682                	slli	a3,a3,0x20
    1190:	9281                	srli	a3,a3,0x20
    1192:	96b2                	add	a3,a3,a2
    1194:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    1198:	02e5d83b          	divuw	a6,a1,a4
        buf[i--] = digits[x % base];
    119c:	00d10923          	sb	a3,18(sp)
    } while ((x /= base) != 0);
    11a0:	0ce5ec63          	bltu	a1,a4,1278 <printint.constprop.0+0x18e>
        buf[i--] = digits[x % base];
    11a4:	02e876bb          	remuw	a3,a6,a4
    11a8:	1682                	slli	a3,a3,0x20
    11aa:	9281                	srli	a3,a3,0x20
    11ac:	96b2                	add	a3,a3,a2
    11ae:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    11b2:	02e855bb          	divuw	a1,a6,a4
        buf[i--] = digits[x % base];
    11b6:	00d108a3          	sb	a3,17(sp)
    } while ((x /= base) != 0);
    11ba:	10e86963          	bltu	a6,a4,12cc <printint.constprop.0+0x1e2>
        buf[i--] = digits[x % base];
    11be:	02e5f6bb          	remuw	a3,a1,a4
    11c2:	1682                	slli	a3,a3,0x20
    11c4:	9281                	srli	a3,a3,0x20
    11c6:	96b2                	add	a3,a3,a2
    11c8:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    11cc:	02e5d83b          	divuw	a6,a1,a4
        buf[i--] = digits[x % base];
    11d0:	00d10823          	sb	a3,16(sp)
    } while ((x /= base) != 0);
    11d4:	10e5e763          	bltu	a1,a4,12e2 <printint.constprop.0+0x1f8>
        buf[i--] = digits[x % base];
    11d8:	02e876bb          	remuw	a3,a6,a4
    11dc:	1682                	slli	a3,a3,0x20
    11de:	9281                	srli	a3,a3,0x20
    11e0:	96b2                	add	a3,a3,a2
    11e2:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    11e6:	02e857bb          	divuw	a5,a6,a4
        buf[i--] = digits[x % base];
    11ea:	00d107a3          	sb	a3,15(sp)
    } while ((x /= base) != 0);
    11ee:	10e86363          	bltu	a6,a4,12f4 <printint.constprop.0+0x20a>
        buf[i--] = digits[x % base];
    11f2:	1782                	slli	a5,a5,0x20
    11f4:	9381                	srli	a5,a5,0x20
    11f6:	97b2                	add	a5,a5,a2
    11f8:	0007c783          	lbu	a5,0(a5)
    11fc:	4599                	li	a1,6
    11fe:	00f10723          	sb	a5,14(sp)

    if (sign)
    1202:	00055763          	bgez	a0,1210 <printint.constprop.0+0x126>
        buf[i--] = '-';
    1206:	02d00793          	li	a5,45
    120a:	00f106a3          	sb	a5,13(sp)
        buf[i--] = digits[x % base];
    120e:	4595                	li	a1,5
    write(f, s, l);
    1210:	003c                	addi	a5,sp,8
    1212:	4641                	li	a2,16
    1214:	9e0d                	subw	a2,a2,a1
    1216:	4505                	li	a0,1
    1218:	95be                	add	a1,a1,a5
    121a:	291000ef          	jal	ra,1caa <write>
    i++;
    if (i < 0)
        puts("printint error");
    out(stdout, buf + i, 16 - i);
}
    121e:	70a2                	ld	ra,40(sp)
    1220:	6145                	addi	sp,sp,48
    1222:	8082                	ret
        x = -xx;
    1224:	40a0083b          	negw	a6,a0
        buf[i--] = digits[x % base];
    1228:	02b877bb          	remuw	a5,a6,a1
    122c:	00001617          	auipc	a2,0x1
    1230:	dac60613          	addi	a2,a2,-596 # 1fd8 <digits>
    buf[16] = 0;
    1234:	00010c23          	sb	zero,24(sp)
        buf[i--] = digits[x % base];
    1238:	0005871b          	sext.w	a4,a1
    123c:	1782                	slli	a5,a5,0x20
    123e:	9381                	srli	a5,a5,0x20
    1240:	97b2                	add	a5,a5,a2
    1242:	0007c783          	lbu	a5,0(a5)
    } while ((x /= base) != 0);
    1246:	02b858bb          	divuw	a7,a6,a1
        buf[i--] = digits[x % base];
    124a:	00f10ba3          	sb	a5,23(sp)
    } while ((x /= base) != 0);
    124e:	06b86963          	bltu	a6,a1,12c0 <printint.constprop.0+0x1d6>
        buf[i--] = digits[x % base];
    1252:	02e8f7bb          	remuw	a5,a7,a4
    1256:	1782                	slli	a5,a5,0x20
    1258:	9381                	srli	a5,a5,0x20
    125a:	97b2                	add	a5,a5,a2
    125c:	0007c783          	lbu	a5,0(a5)
    } while ((x /= base) != 0);
    1260:	02e8d6bb          	divuw	a3,a7,a4
        buf[i--] = digits[x % base];
    1264:	00f10b23          	sb	a5,22(sp)
    } while ((x /= base) != 0);
    1268:	ece8f8e3          	bgeu	a7,a4,1138 <printint.constprop.0+0x4e>
        buf[i--] = '-';
    126c:	02d00793          	li	a5,45
    1270:	00f10aa3          	sb	a5,21(sp)
        buf[i--] = digits[x % base];
    1274:	45b5                	li	a1,13
    1276:	bf69                	j	1210 <printint.constprop.0+0x126>
    1278:	45a9                	li	a1,10
    if (sign)
    127a:	f8055be3          	bgez	a0,1210 <printint.constprop.0+0x126>
        buf[i--] = '-';
    127e:	02d00793          	li	a5,45
    1282:	00f108a3          	sb	a5,17(sp)
        buf[i--] = digits[x % base];
    1286:	45a5                	li	a1,9
    1288:	b761                	j	1210 <printint.constprop.0+0x126>
    128a:	45b5                	li	a1,13
    if (sign)
    128c:	f80552e3          	bgez	a0,1210 <printint.constprop.0+0x126>
        buf[i--] = '-';
    1290:	02d00793          	li	a5,45
    1294:	00f10a23          	sb	a5,20(sp)
        buf[i--] = digits[x % base];
    1298:	45b1                	li	a1,12
    129a:	bf9d                	j	1210 <printint.constprop.0+0x126>
    129c:	45b1                	li	a1,12
    if (sign)
    129e:	f60559e3          	bgez	a0,1210 <printint.constprop.0+0x126>
        buf[i--] = '-';
    12a2:	02d00793          	li	a5,45
    12a6:	00f109a3          	sb	a5,19(sp)
        buf[i--] = digits[x % base];
    12aa:	45ad                	li	a1,11
    12ac:	b795                	j	1210 <printint.constprop.0+0x126>
    12ae:	45ad                	li	a1,11
    if (sign)
    12b0:	f60550e3          	bgez	a0,1210 <printint.constprop.0+0x126>
        buf[i--] = '-';
    12b4:	02d00793          	li	a5,45
    12b8:	00f10923          	sb	a5,18(sp)
        buf[i--] = digits[x % base];
    12bc:	45a9                	li	a1,10
    12be:	bf89                	j	1210 <printint.constprop.0+0x126>
        buf[i--] = '-';
    12c0:	02d00793          	li	a5,45
    12c4:	00f10b23          	sb	a5,22(sp)
        buf[i--] = digits[x % base];
    12c8:	45b9                	li	a1,14
    12ca:	b799                	j	1210 <printint.constprop.0+0x126>
    12cc:	45a5                	li	a1,9
    if (sign)
    12ce:	f40551e3          	bgez	a0,1210 <printint.constprop.0+0x126>
        buf[i--] = '-';
    12d2:	02d00793          	li	a5,45
    12d6:	00f10823          	sb	a5,16(sp)
        buf[i--] = digits[x % base];
    12da:	45a1                	li	a1,8
    12dc:	bf15                	j	1210 <printint.constprop.0+0x126>
    i = 15;
    12de:	45bd                	li	a1,15
    12e0:	bf05                	j	1210 <printint.constprop.0+0x126>
        buf[i--] = digits[x % base];
    12e2:	45a1                	li	a1,8
    if (sign)
    12e4:	f20556e3          	bgez	a0,1210 <printint.constprop.0+0x126>
        buf[i--] = '-';
    12e8:	02d00793          	li	a5,45
    12ec:	00f107a3          	sb	a5,15(sp)
        buf[i--] = digits[x % base];
    12f0:	459d                	li	a1,7
    12f2:	bf39                	j	1210 <printint.constprop.0+0x126>
    12f4:	459d                	li	a1,7
    if (sign)
    12f6:	f0055de3          	bgez	a0,1210 <printint.constprop.0+0x126>
        buf[i--] = '-';
    12fa:	02d00793          	li	a5,45
    12fe:	00f10723          	sb	a5,14(sp)
        buf[i--] = digits[x % base];
    1302:	4599                	li	a1,6
    1304:	b731                	j	1210 <printint.constprop.0+0x126>

0000000000001306 <getchar>:
{
    1306:	1101                	addi	sp,sp,-32
    read(stdin, &byte, 1);
    1308:	00f10593          	addi	a1,sp,15
    130c:	4605                	li	a2,1
    130e:	4501                	li	a0,0
{
    1310:	ec06                	sd	ra,24(sp)
    char byte = 0;
    1312:	000107a3          	sb	zero,15(sp)
    read(stdin, &byte, 1);
    1316:	18b000ef          	jal	ra,1ca0 <read>
}
    131a:	60e2                	ld	ra,24(sp)
    131c:	00f14503          	lbu	a0,15(sp)
    1320:	6105                	addi	sp,sp,32
    1322:	8082                	ret

0000000000001324 <putchar>:
{
    1324:	1101                	addi	sp,sp,-32
    1326:	87aa                	mv	a5,a0
    return write(stdout, &byte, 1);
    1328:	00f10593          	addi	a1,sp,15
    132c:	4605                	li	a2,1
    132e:	4505                	li	a0,1
{
    1330:	ec06                	sd	ra,24(sp)
    char byte = c;
    1332:	00f107a3          	sb	a5,15(sp)
    return write(stdout, &byte, 1);
    1336:	175000ef          	jal	ra,1caa <write>
}
    133a:	60e2                	ld	ra,24(sp)
    133c:	2501                	sext.w	a0,a0
    133e:	6105                	addi	sp,sp,32
    1340:	8082                	ret

0000000000001342 <puts>:
{
    1342:	1141                	addi	sp,sp,-16
    1344:	e406                	sd	ra,8(sp)
    1346:	e022                	sd	s0,0(sp)
    1348:	842a                	mv	s0,a0
    r = -(write(stdout, s, strlen(s)) < 0);
    134a:	57c000ef          	jal	ra,18c6 <strlen>
    134e:	862a                	mv	a2,a0
    1350:	85a2                	mv	a1,s0
    1352:	4505                	li	a0,1
    1354:	157000ef          	jal	ra,1caa <write>
}
    1358:	60a2                	ld	ra,8(sp)
    135a:	6402                	ld	s0,0(sp)
    r = -(write(stdout, s, strlen(s)) < 0);
    135c:	957d                	srai	a0,a0,0x3f
    return r;
    135e:	2501                	sext.w	a0,a0
}
    1360:	0141                	addi	sp,sp,16
    1362:	8082                	ret

0000000000001364 <printf>:
    out(stdout, buf, i);
}

// Print to the console. only understands %d, %x, %p, %s.
void printf(const char *fmt, ...)
{
    1364:	7171                	addi	sp,sp,-176
    1366:	fc56                	sd	s5,56(sp)
    1368:	ed3e                	sd	a5,152(sp)
    buf[i++] = '0';
    136a:	7ae1                	lui	s5,0xffff8
    va_list ap;
    int cnt = 0, l = 0;
    char *a, *z, *s = (char *)fmt, str;
    int f = stdout;

    va_start(ap, fmt);
    136c:	18bc                	addi	a5,sp,120
{
    136e:	e8ca                	sd	s2,80(sp)
    1370:	e4ce                	sd	s3,72(sp)
    1372:	e0d2                	sd	s4,64(sp)
    1374:	f85a                	sd	s6,48(sp)
    1376:	f486                	sd	ra,104(sp)
    1378:	f0a2                	sd	s0,96(sp)
    137a:	eca6                	sd	s1,88(sp)
    137c:	fcae                	sd	a1,120(sp)
    137e:	e132                	sd	a2,128(sp)
    1380:	e536                	sd	a3,136(sp)
    1382:	e93a                	sd	a4,144(sp)
    1384:	f142                	sd	a6,160(sp)
    1386:	f546                	sd	a7,168(sp)
    va_start(ap, fmt);
    1388:	e03e                	sd	a5,0(sp)
    for (;;)
    {
        if (!*s)
            break;
        for (a = s; *s && *s != '%'; s++)
    138a:	02500913          	li	s2,37
        out(f, a, l);
        if (l)
            continue;
        if (s[1] == 0)
            break;
        switch (s[1])
    138e:	07300a13          	li	s4,115
        case 'p':
            printptr(va_arg(ap, uint64));
            break;
        case 's':
            if ((a = va_arg(ap, char *)) == 0)
                a = "(null)";
    1392:	00001b17          	auipc	s6,0x1
    1396:	c16b0b13          	addi	s6,s6,-1002 # 1fa8 <__clone+0xca>
    buf[i++] = '0';
    139a:	830aca93          	xori	s5,s5,-2000
        buf[i++] = digits[x >> (sizeof(uint64) * 8 - 4)];
    139e:	00001997          	auipc	s3,0x1
    13a2:	c3a98993          	addi	s3,s3,-966 # 1fd8 <digits>
        if (!*s)
    13a6:	00054783          	lbu	a5,0(a0)
    13aa:	16078a63          	beqz	a5,151e <printf+0x1ba>
    13ae:	862a                	mv	a2,a0
        for (a = s; *s && *s != '%'; s++)
    13b0:	19278163          	beq	a5,s2,1532 <printf+0x1ce>
    13b4:	00164783          	lbu	a5,1(a2)
    13b8:	0605                	addi	a2,a2,1
    13ba:	fbfd                	bnez	a5,13b0 <printf+0x4c>
    13bc:	84b2                	mv	s1,a2
        l = z - a;
    13be:	40a6043b          	subw	s0,a2,a0
    write(f, s, l);
    13c2:	85aa                	mv	a1,a0
    13c4:	8622                	mv	a2,s0
    13c6:	4505                	li	a0,1
    13c8:	0e3000ef          	jal	ra,1caa <write>
        if (l)
    13cc:	18041c63          	bnez	s0,1564 <printf+0x200>
        if (s[1] == 0)
    13d0:	0014c783          	lbu	a5,1(s1)
    13d4:	14078563          	beqz	a5,151e <printf+0x1ba>
        switch (s[1])
    13d8:	1d478063          	beq	a5,s4,1598 <printf+0x234>
    13dc:	18fa6663          	bltu	s4,a5,1568 <printf+0x204>
    13e0:	06400713          	li	a4,100
    13e4:	1ae78063          	beq	a5,a4,1584 <printf+0x220>
    13e8:	07000713          	li	a4,112
    13ec:	1ce79963          	bne	a5,a4,15be <printf+0x25a>
            printptr(va_arg(ap, uint64));
    13f0:	6702                	ld	a4,0(sp)
    buf[i++] = '0';
    13f2:	01511423          	sh	s5,8(sp)
    write(f, s, l);
    13f6:	4649                	li	a2,18
            printptr(va_arg(ap, uint64));
    13f8:	631c                	ld	a5,0(a4)
    13fa:	0721                	addi	a4,a4,8
    13fc:	e03a                	sd	a4,0(sp)
    for (j = 0; j < (sizeof(uint64) * 2); j++, x <<= 4)
    13fe:	00479293          	slli	t0,a5,0x4
    1402:	00879f93          	slli	t6,a5,0x8
    1406:	00c79f13          	slli	t5,a5,0xc
    140a:	01079e93          	slli	t4,a5,0x10
    140e:	01479e13          	slli	t3,a5,0x14
    1412:	01879313          	slli	t1,a5,0x18
    1416:	01c79893          	slli	a7,a5,0x1c
    141a:	02479813          	slli	a6,a5,0x24
    141e:	02879513          	slli	a0,a5,0x28
    1422:	02c79593          	slli	a1,a5,0x2c
    1426:	03079693          	slli	a3,a5,0x30
    142a:	03479713          	slli	a4,a5,0x34
        buf[i++] = digits[x >> (sizeof(uint64) * 8 - 4)];
    142e:	03c7d413          	srli	s0,a5,0x3c
    1432:	01c7d39b          	srliw	t2,a5,0x1c
    1436:	03c2d293          	srli	t0,t0,0x3c
    143a:	03cfdf93          	srli	t6,t6,0x3c
    143e:	03cf5f13          	srli	t5,t5,0x3c
    1442:	03cede93          	srli	t4,t4,0x3c
    1446:	03ce5e13          	srli	t3,t3,0x3c
    144a:	03c35313          	srli	t1,t1,0x3c
    144e:	03c8d893          	srli	a7,a7,0x3c
    1452:	03c85813          	srli	a6,a6,0x3c
    1456:	9171                	srli	a0,a0,0x3c
    1458:	91f1                	srli	a1,a1,0x3c
    145a:	92f1                	srli	a3,a3,0x3c
    145c:	9371                	srli	a4,a4,0x3c
    145e:	96ce                	add	a3,a3,s3
    1460:	974e                	add	a4,a4,s3
    1462:	944e                	add	s0,s0,s3
    1464:	92ce                	add	t0,t0,s3
    1466:	9fce                	add	t6,t6,s3
    1468:	9f4e                	add	t5,t5,s3
    146a:	9ece                	add	t4,t4,s3
    146c:	9e4e                	add	t3,t3,s3
    146e:	934e                	add	t1,t1,s3
    1470:	98ce                	add	a7,a7,s3
    1472:	93ce                	add	t2,t2,s3
    1474:	984e                	add	a6,a6,s3
    1476:	954e                	add	a0,a0,s3
    1478:	95ce                	add	a1,a1,s3
    147a:	0006c083          	lbu	ra,0(a3)
    147e:	0002c283          	lbu	t0,0(t0)
    1482:	00074683          	lbu	a3,0(a4)
    1486:	000fcf83          	lbu	t6,0(t6)
    148a:	000f4f03          	lbu	t5,0(t5)
    148e:	000ece83          	lbu	t4,0(t4)
    1492:	000e4e03          	lbu	t3,0(t3)
    1496:	00034303          	lbu	t1,0(t1)
    149a:	0008c883          	lbu	a7,0(a7)
    149e:	0003c383          	lbu	t2,0(t2)
    14a2:	00084803          	lbu	a6,0(a6)
    14a6:	00054503          	lbu	a0,0(a0)
    14aa:	0005c583          	lbu	a1,0(a1)
    14ae:	00044403          	lbu	s0,0(s0)
    for (j = 0; j < (sizeof(uint64) * 2); j++, x <<= 4)
    14b2:	03879713          	slli	a4,a5,0x38
        buf[i++] = digits[x >> (sizeof(uint64) * 8 - 4)];
    14b6:	9371                	srli	a4,a4,0x3c
    14b8:	8bbd                	andi	a5,a5,15
    14ba:	974e                	add	a4,a4,s3
    14bc:	97ce                	add	a5,a5,s3
    14be:	005105a3          	sb	t0,11(sp)
    14c2:	01f10623          	sb	t6,12(sp)
    14c6:	01e106a3          	sb	t5,13(sp)
    14ca:	01d10723          	sb	t4,14(sp)
    14ce:	01c107a3          	sb	t3,15(sp)
    14d2:	00610823          	sb	t1,16(sp)
    14d6:	011108a3          	sb	a7,17(sp)
    14da:	00710923          	sb	t2,18(sp)
    14de:	010109a3          	sb	a6,19(sp)
    14e2:	00a10a23          	sb	a0,20(sp)
    14e6:	00b10aa3          	sb	a1,21(sp)
    14ea:	00110b23          	sb	ra,22(sp)
    14ee:	00d10ba3          	sb	a3,23(sp)
    14f2:	00810523          	sb	s0,10(sp)
    14f6:	00074703          	lbu	a4,0(a4)
    14fa:	0007c783          	lbu	a5,0(a5)
    write(f, s, l);
    14fe:	002c                	addi	a1,sp,8
    1500:	4505                	li	a0,1
        buf[i++] = digits[x >> (sizeof(uint64) * 8 - 4)];
    1502:	00e10c23          	sb	a4,24(sp)
    1506:	00f10ca3          	sb	a5,25(sp)
    buf[i] = 0;
    150a:	00010d23          	sb	zero,26(sp)
    write(f, s, l);
    150e:	79c000ef          	jal	ra,1caa <write>
            // Print unknown % sequence to draw attention.
            putchar('%');
            putchar(s[1]);
            break;
        }
        s += 2;
    1512:	00248513          	addi	a0,s1,2
        if (!*s)
    1516:	00054783          	lbu	a5,0(a0)
    151a:	e8079ae3          	bnez	a5,13ae <printf+0x4a>
    }
    va_end(ap);
}
    151e:	70a6                	ld	ra,104(sp)
    1520:	7406                	ld	s0,96(sp)
    1522:	64e6                	ld	s1,88(sp)
    1524:	6946                	ld	s2,80(sp)
    1526:	69a6                	ld	s3,72(sp)
    1528:	6a06                	ld	s4,64(sp)
    152a:	7ae2                	ld	s5,56(sp)
    152c:	7b42                	ld	s6,48(sp)
    152e:	614d                	addi	sp,sp,176
    1530:	8082                	ret
        for (z = s; s[0] == '%' && s[1] == '%'; z++, s += 2)
    1532:	00064783          	lbu	a5,0(a2)
    1536:	84b2                	mv	s1,a2
    1538:	01278963          	beq	a5,s2,154a <printf+0x1e6>
    153c:	b549                	j	13be <printf+0x5a>
    153e:	0024c783          	lbu	a5,2(s1)
    1542:	0605                	addi	a2,a2,1
    1544:	0489                	addi	s1,s1,2
    1546:	e7279ce3          	bne	a5,s2,13be <printf+0x5a>
    154a:	0014c783          	lbu	a5,1(s1)
    154e:	ff2788e3          	beq	a5,s2,153e <printf+0x1da>
        l = z - a;
    1552:	40a6043b          	subw	s0,a2,a0
    write(f, s, l);
    1556:	85aa                	mv	a1,a0
    1558:	8622                	mv	a2,s0
    155a:	4505                	li	a0,1
    155c:	74e000ef          	jal	ra,1caa <write>
        if (l)
    1560:	e60408e3          	beqz	s0,13d0 <printf+0x6c>
    1564:	8526                	mv	a0,s1
    1566:	b581                	j	13a6 <printf+0x42>
        switch (s[1])
    1568:	07800713          	li	a4,120
    156c:	04e79963          	bne	a5,a4,15be <printf+0x25a>
            printint(va_arg(ap, int), 16, 1);
    1570:	6782                	ld	a5,0(sp)
    1572:	45c1                	li	a1,16
    1574:	4388                	lw	a0,0(a5)
    1576:	07a1                	addi	a5,a5,8
    1578:	e03e                	sd	a5,0(sp)
    157a:	b71ff0ef          	jal	ra,10ea <printint.constprop.0>
        s += 2;
    157e:	00248513          	addi	a0,s1,2
    1582:	bf51                	j	1516 <printf+0x1b2>
            printint(va_arg(ap, int), 10, 1);
    1584:	6782                	ld	a5,0(sp)
    1586:	45a9                	li	a1,10
    1588:	4388                	lw	a0,0(a5)
    158a:	07a1                	addi	a5,a5,8
    158c:	e03e                	sd	a5,0(sp)
    158e:	b5dff0ef          	jal	ra,10ea <printint.constprop.0>
        s += 2;
    1592:	00248513          	addi	a0,s1,2
    1596:	b741                	j	1516 <printf+0x1b2>
            if ((a = va_arg(ap, char *)) == 0)
    1598:	6782                	ld	a5,0(sp)
    159a:	6380                	ld	s0,0(a5)
    159c:	07a1                	addi	a5,a5,8
    159e:	e03e                	sd	a5,0(sp)
    15a0:	c031                	beqz	s0,15e4 <printf+0x280>
            l = strnlen(a, 200);
    15a2:	0c800593          	li	a1,200
    15a6:	8522                	mv	a0,s0
    15a8:	40a000ef          	jal	ra,19b2 <strnlen>
    write(f, s, l);
    15ac:	0005061b          	sext.w	a2,a0
    15b0:	85a2                	mv	a1,s0
    15b2:	4505                	li	a0,1
    15b4:	6f6000ef          	jal	ra,1caa <write>
        s += 2;
    15b8:	00248513          	addi	a0,s1,2
    15bc:	bfa9                	j	1516 <printf+0x1b2>
    return write(stdout, &byte, 1);
    15be:	4605                	li	a2,1
    15c0:	002c                	addi	a1,sp,8
    15c2:	4505                	li	a0,1
    char byte = c;
    15c4:	01210423          	sb	s2,8(sp)
    return write(stdout, &byte, 1);
    15c8:	6e2000ef          	jal	ra,1caa <write>
    char byte = c;
    15cc:	0014c783          	lbu	a5,1(s1)
    return write(stdout, &byte, 1);
    15d0:	4605                	li	a2,1
    15d2:	002c                	addi	a1,sp,8
    15d4:	4505                	li	a0,1
    char byte = c;
    15d6:	00f10423          	sb	a5,8(sp)
    return write(stdout, &byte, 1);
    15da:	6d0000ef          	jal	ra,1caa <write>
        s += 2;
    15de:	00248513          	addi	a0,s1,2
    15e2:	bf15                	j	1516 <printf+0x1b2>
                a = "(null)";
    15e4:	845a                	mv	s0,s6
    15e6:	bf75                	j	15a2 <printf+0x23e>

00000000000015e8 <panic>:
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

void panic(char *m)
{
    15e8:	1141                	addi	sp,sp,-16
    15ea:	e406                	sd	ra,8(sp)
    puts(m);
    15ec:	d57ff0ef          	jal	ra,1342 <puts>
    exit(-100);
}
    15f0:	60a2                	ld	ra,8(sp)
    exit(-100);
    15f2:	f9c00513          	li	a0,-100
}
    15f6:	0141                	addi	sp,sp,16
    exit(-100);
    15f8:	a709                	j	1cfa <exit>

00000000000015fa <isspace>:
#define HIGHS (ONES * (UCHAR_MAX / 2 + 1))
#define HASZERO(x) (((x)-ONES) & ~(x)&HIGHS)

int isspace(int c)
{
    return c == ' ' || (unsigned)c - '\t' < 5;
    15fa:	02000793          	li	a5,32
    15fe:	00f50663          	beq	a0,a5,160a <isspace+0x10>
    1602:	355d                	addiw	a0,a0,-9
    1604:	00553513          	sltiu	a0,a0,5
    1608:	8082                	ret
    160a:	4505                	li	a0,1
}
    160c:	8082                	ret

000000000000160e <isdigit>:

int isdigit(int c)
{
    return (unsigned)c - '0' < 10;
    160e:	fd05051b          	addiw	a0,a0,-48
}
    1612:	00a53513          	sltiu	a0,a0,10
    1616:	8082                	ret

0000000000001618 <atoi>:
    return c == ' ' || (unsigned)c - '\t' < 5;
    1618:	02000613          	li	a2,32
    161c:	4591                	li	a1,4

int atoi(const char *s)
{
    int n = 0, neg = 0;
    while (isspace(*s))
    161e:	00054703          	lbu	a4,0(a0)
    return c == ' ' || (unsigned)c - '\t' < 5;
    1622:	ff77069b          	addiw	a3,a4,-9
    1626:	04c70d63          	beq	a4,a2,1680 <atoi+0x68>
    162a:	0007079b          	sext.w	a5,a4
    162e:	04d5f963          	bgeu	a1,a3,1680 <atoi+0x68>
        s++;
    switch (*s)
    1632:	02b00693          	li	a3,43
    1636:	04d70a63          	beq	a4,a3,168a <atoi+0x72>
    163a:	02d00693          	li	a3,45
    163e:	06d70463          	beq	a4,a3,16a6 <atoi+0x8e>
        neg = 1;
    case '+':
        s++;
    }
    /* Compute n as a negative number to avoid overflow on INT_MIN */
    while (isdigit(*s))
    1642:	fd07859b          	addiw	a1,a5,-48
    1646:	4625                	li	a2,9
    1648:	873e                	mv	a4,a5
    164a:	86aa                	mv	a3,a0
    int n = 0, neg = 0;
    164c:	4e01                	li	t3,0
    while (isdigit(*s))
    164e:	04b66a63          	bltu	a2,a1,16a2 <atoi+0x8a>
    int n = 0, neg = 0;
    1652:	4501                	li	a0,0
    while (isdigit(*s))
    1654:	4825                	li	a6,9
    1656:	0016c603          	lbu	a2,1(a3)
        n = 10 * n - (*s++ - '0');
    165a:	0025179b          	slliw	a5,a0,0x2
    165e:	9d3d                	addw	a0,a0,a5
    1660:	fd07031b          	addiw	t1,a4,-48
    1664:	0015189b          	slliw	a7,a0,0x1
    while (isdigit(*s))
    1668:	fd06059b          	addiw	a1,a2,-48
        n = 10 * n - (*s++ - '0');
    166c:	0685                	addi	a3,a3,1
    166e:	4068853b          	subw	a0,a7,t1
    while (isdigit(*s))
    1672:	0006071b          	sext.w	a4,a2
    1676:	feb870e3          	bgeu	a6,a1,1656 <atoi+0x3e>
    return neg ? n : -n;
    167a:	000e0563          	beqz	t3,1684 <atoi+0x6c>
}
    167e:	8082                	ret
        s++;
    1680:	0505                	addi	a0,a0,1
    1682:	bf71                	j	161e <atoi+0x6>
    return neg ? n : -n;
    1684:	4113053b          	subw	a0,t1,a7
    1688:	8082                	ret
    while (isdigit(*s))
    168a:	00154783          	lbu	a5,1(a0)
    168e:	4625                	li	a2,9
        s++;
    1690:	00150693          	addi	a3,a0,1
    while (isdigit(*s))
    1694:	fd07859b          	addiw	a1,a5,-48
    1698:	0007871b          	sext.w	a4,a5
    int n = 0, neg = 0;
    169c:	4e01                	li	t3,0
    while (isdigit(*s))
    169e:	fab67ae3          	bgeu	a2,a1,1652 <atoi+0x3a>
    16a2:	4501                	li	a0,0
}
    16a4:	8082                	ret
    while (isdigit(*s))
    16a6:	00154783          	lbu	a5,1(a0)
    16aa:	4625                	li	a2,9
        s++;
    16ac:	00150693          	addi	a3,a0,1
    while (isdigit(*s))
    16b0:	fd07859b          	addiw	a1,a5,-48
    16b4:	0007871b          	sext.w	a4,a5
    16b8:	feb665e3          	bltu	a2,a1,16a2 <atoi+0x8a>
        neg = 1;
    16bc:	4e05                	li	t3,1
    16be:	bf51                	j	1652 <atoi+0x3a>

00000000000016c0 <memset>:

void *memset(void *dest, int c, size_t n)
{
    char *p = dest;
    for (int i = 0; i < n; ++i, *(p++) = c)
    16c0:	16060d63          	beqz	a2,183a <memset+0x17a>
    16c4:	40a007b3          	neg	a5,a0
    16c8:	8b9d                	andi	a5,a5,7
    16ca:	00778713          	addi	a4,a5,7
    16ce:	482d                	li	a6,11
    16d0:	0ff5f593          	andi	a1,a1,255
    16d4:	fff60693          	addi	a3,a2,-1
    16d8:	17076263          	bltu	a4,a6,183c <memset+0x17c>
    16dc:	16e6ea63          	bltu	a3,a4,1850 <memset+0x190>
    16e0:	16078563          	beqz	a5,184a <memset+0x18a>
    16e4:	00b50023          	sb	a1,0(a0)
    16e8:	4705                	li	a4,1
    16ea:	00150e93          	addi	t4,a0,1
    16ee:	14e78c63          	beq	a5,a4,1846 <memset+0x186>
    16f2:	00b500a3          	sb	a1,1(a0)
    16f6:	4709                	li	a4,2
    16f8:	00250e93          	addi	t4,a0,2
    16fc:	14e78d63          	beq	a5,a4,1856 <memset+0x196>
    1700:	00b50123          	sb	a1,2(a0)
    1704:	470d                	li	a4,3
    1706:	00350e93          	addi	t4,a0,3
    170a:	12e78b63          	beq	a5,a4,1840 <memset+0x180>
    170e:	00b501a3          	sb	a1,3(a0)
    1712:	4711                	li	a4,4
    1714:	00450e93          	addi	t4,a0,4
    1718:	14e78163          	beq	a5,a4,185a <memset+0x19a>
    171c:	00b50223          	sb	a1,4(a0)
    1720:	4715                	li	a4,5
    1722:	00550e93          	addi	t4,a0,5
    1726:	12e78c63          	beq	a5,a4,185e <memset+0x19e>
    172a:	00b502a3          	sb	a1,5(a0)
    172e:	471d                	li	a4,7
    1730:	00650e93          	addi	t4,a0,6
    1734:	12e79763          	bne	a5,a4,1862 <memset+0x1a2>
    1738:	00750e93          	addi	t4,a0,7
    173c:	00b50323          	sb	a1,6(a0)
    1740:	4f1d                	li	t5,7
    1742:	00859713          	slli	a4,a1,0x8
    1746:	8f4d                	or	a4,a4,a1
    1748:	01059e13          	slli	t3,a1,0x10
    174c:	01c76e33          	or	t3,a4,t3
    1750:	01859313          	slli	t1,a1,0x18
    1754:	006e6333          	or	t1,t3,t1
    1758:	02059893          	slli	a7,a1,0x20
    175c:	011368b3          	or	a7,t1,a7
    1760:	02859813          	slli	a6,a1,0x28
    1764:	40f60333          	sub	t1,a2,a5
    1768:	0108e833          	or	a6,a7,a6
    176c:	03059693          	slli	a3,a1,0x30
    1770:	00d866b3          	or	a3,a6,a3
    1774:	03859713          	slli	a4,a1,0x38
    1778:	97aa                	add	a5,a5,a0
    177a:	ff837813          	andi	a6,t1,-8
    177e:	8f55                	or	a4,a4,a3
    1780:	00f806b3          	add	a3,a6,a5
    1784:	e398                	sd	a4,0(a5)
    1786:	07a1                	addi	a5,a5,8
    1788:	fed79ee3          	bne	a5,a3,1784 <memset+0xc4>
    178c:	ff837693          	andi	a3,t1,-8
    1790:	00de87b3          	add	a5,t4,a3
    1794:	01e6873b          	addw	a4,a3,t5
    1798:	0ad30663          	beq	t1,a3,1844 <memset+0x184>
    179c:	00b78023          	sb	a1,0(a5)
    17a0:	0017069b          	addiw	a3,a4,1
    17a4:	08c6fb63          	bgeu	a3,a2,183a <memset+0x17a>
    17a8:	00b780a3          	sb	a1,1(a5)
    17ac:	0027069b          	addiw	a3,a4,2
    17b0:	08c6f563          	bgeu	a3,a2,183a <memset+0x17a>
    17b4:	00b78123          	sb	a1,2(a5)
    17b8:	0037069b          	addiw	a3,a4,3
    17bc:	06c6ff63          	bgeu	a3,a2,183a <memset+0x17a>
    17c0:	00b781a3          	sb	a1,3(a5)
    17c4:	0047069b          	addiw	a3,a4,4
    17c8:	06c6f963          	bgeu	a3,a2,183a <memset+0x17a>
    17cc:	00b78223          	sb	a1,4(a5)
    17d0:	0057069b          	addiw	a3,a4,5
    17d4:	06c6f363          	bgeu	a3,a2,183a <memset+0x17a>
    17d8:	00b782a3          	sb	a1,5(a5)
    17dc:	0067069b          	addiw	a3,a4,6
    17e0:	04c6fd63          	bgeu	a3,a2,183a <memset+0x17a>
    17e4:	00b78323          	sb	a1,6(a5)
    17e8:	0077069b          	addiw	a3,a4,7
    17ec:	04c6f763          	bgeu	a3,a2,183a <memset+0x17a>
    17f0:	00b783a3          	sb	a1,7(a5)
    17f4:	0087069b          	addiw	a3,a4,8
    17f8:	04c6f163          	bgeu	a3,a2,183a <memset+0x17a>
    17fc:	00b78423          	sb	a1,8(a5)
    1800:	0097069b          	addiw	a3,a4,9
    1804:	02c6fb63          	bgeu	a3,a2,183a <memset+0x17a>
    1808:	00b784a3          	sb	a1,9(a5)
    180c:	00a7069b          	addiw	a3,a4,10
    1810:	02c6f563          	bgeu	a3,a2,183a <memset+0x17a>
    1814:	00b78523          	sb	a1,10(a5)
    1818:	00b7069b          	addiw	a3,a4,11
    181c:	00c6ff63          	bgeu	a3,a2,183a <memset+0x17a>
    1820:	00b785a3          	sb	a1,11(a5)
    1824:	00c7069b          	addiw	a3,a4,12
    1828:	00c6f963          	bgeu	a3,a2,183a <memset+0x17a>
    182c:	00b78623          	sb	a1,12(a5)
    1830:	2735                	addiw	a4,a4,13
    1832:	00c77463          	bgeu	a4,a2,183a <memset+0x17a>
    1836:	00b786a3          	sb	a1,13(a5)
        ;
    return dest;
}
    183a:	8082                	ret
    183c:	472d                	li	a4,11
    183e:	bd79                	j	16dc <memset+0x1c>
    for (int i = 0; i < n; ++i, *(p++) = c)
    1840:	4f0d                	li	t5,3
    1842:	b701                	j	1742 <memset+0x82>
    1844:	8082                	ret
    1846:	4f05                	li	t5,1
    1848:	bded                	j	1742 <memset+0x82>
    184a:	8eaa                	mv	t4,a0
    184c:	4f01                	li	t5,0
    184e:	bdd5                	j	1742 <memset+0x82>
    1850:	87aa                	mv	a5,a0
    1852:	4701                	li	a4,0
    1854:	b7a1                	j	179c <memset+0xdc>
    1856:	4f09                	li	t5,2
    1858:	b5ed                	j	1742 <memset+0x82>
    185a:	4f11                	li	t5,4
    185c:	b5dd                	j	1742 <memset+0x82>
    185e:	4f15                	li	t5,5
    1860:	b5cd                	j	1742 <memset+0x82>
    1862:	4f19                	li	t5,6
    1864:	bdf9                	j	1742 <memset+0x82>

0000000000001866 <strcmp>:

int strcmp(const char *l, const char *r)
{
    for (; *l == *r && *l; l++, r++)
    1866:	00054783          	lbu	a5,0(a0)
    186a:	0005c703          	lbu	a4,0(a1)
    186e:	00e79863          	bne	a5,a4,187e <strcmp+0x18>
    1872:	0505                	addi	a0,a0,1
    1874:	0585                	addi	a1,a1,1
    1876:	fbe5                	bnez	a5,1866 <strcmp>
    1878:	4501                	li	a0,0
        ;
    return *(unsigned char *)l - *(unsigned char *)r;
}
    187a:	9d19                	subw	a0,a0,a4
    187c:	8082                	ret
    return *(unsigned char *)l - *(unsigned char *)r;
    187e:	0007851b          	sext.w	a0,a5
    1882:	bfe5                	j	187a <strcmp+0x14>

0000000000001884 <strncmp>:

int strncmp(const char *_l, const char *_r, size_t n)
{
    const unsigned char *l = (void *)_l, *r = (void *)_r;
    if (!n--)
    1884:	ce05                	beqz	a2,18bc <strncmp+0x38>
        return 0;
    for (; *l && *r && n && *l == *r; l++, r++, n--)
    1886:	00054703          	lbu	a4,0(a0)
    188a:	0005c783          	lbu	a5,0(a1)
    188e:	cb0d                	beqz	a4,18c0 <strncmp+0x3c>
    if (!n--)
    1890:	167d                	addi	a2,a2,-1
    1892:	00c506b3          	add	a3,a0,a2
    1896:	a819                	j	18ac <strncmp+0x28>
    for (; *l && *r && n && *l == *r; l++, r++, n--)
    1898:	00a68e63          	beq	a3,a0,18b4 <strncmp+0x30>
    189c:	0505                	addi	a0,a0,1
    189e:	00e79b63          	bne	a5,a4,18b4 <strncmp+0x30>
    18a2:	00054703          	lbu	a4,0(a0)
        ;
    return *l - *r;
    18a6:	0005c783          	lbu	a5,0(a1)
    for (; *l && *r && n && *l == *r; l++, r++, n--)
    18aa:	cb19                	beqz	a4,18c0 <strncmp+0x3c>
    18ac:	0005c783          	lbu	a5,0(a1)
    18b0:	0585                	addi	a1,a1,1
    18b2:	f3fd                	bnez	a5,1898 <strncmp+0x14>
    return *l - *r;
    18b4:	0007051b          	sext.w	a0,a4
    18b8:	9d1d                	subw	a0,a0,a5
    18ba:	8082                	ret
        return 0;
    18bc:	4501                	li	a0,0
}
    18be:	8082                	ret
    18c0:	4501                	li	a0,0
    return *l - *r;
    18c2:	9d1d                	subw	a0,a0,a5
    18c4:	8082                	ret

00000000000018c6 <strlen>:
size_t strlen(const char *s)
{
    const char *a = s;
    typedef size_t __attribute__((__may_alias__)) word;
    const word *w;
    for (; (uintptr_t)s % SS; s++)
    18c6:	00757793          	andi	a5,a0,7
    18ca:	cf89                	beqz	a5,18e4 <strlen+0x1e>
    18cc:	87aa                	mv	a5,a0
    18ce:	a029                	j	18d8 <strlen+0x12>
    18d0:	0785                	addi	a5,a5,1
    18d2:	0077f713          	andi	a4,a5,7
    18d6:	cb01                	beqz	a4,18e6 <strlen+0x20>
        if (!*s)
    18d8:	0007c703          	lbu	a4,0(a5)
    18dc:	fb75                	bnez	a4,18d0 <strlen+0xa>
    for (w = (const void *)s; !HASZERO(*w); w++)
        ;
    s = (const void *)w;
    for (; *s; s++)
        ;
    return s - a;
    18de:	40a78533          	sub	a0,a5,a0
}
    18e2:	8082                	ret
    for (; (uintptr_t)s % SS; s++)
    18e4:	87aa                	mv	a5,a0
    for (w = (const void *)s; !HASZERO(*w); w++)
    18e6:	6394                	ld	a3,0(a5)
    18e8:	00000597          	auipc	a1,0x0
    18ec:	6c85b583          	ld	a1,1736(a1) # 1fb0 <__clone+0xd2>
    18f0:	00000617          	auipc	a2,0x0
    18f4:	6c863603          	ld	a2,1736(a2) # 1fb8 <__clone+0xda>
    18f8:	a019                	j	18fe <strlen+0x38>
    18fa:	6794                	ld	a3,8(a5)
    18fc:	07a1                	addi	a5,a5,8
    18fe:	00b68733          	add	a4,a3,a1
    1902:	fff6c693          	not	a3,a3
    1906:	8f75                	and	a4,a4,a3
    1908:	8f71                	and	a4,a4,a2
    190a:	db65                	beqz	a4,18fa <strlen+0x34>
    for (; *s; s++)
    190c:	0007c703          	lbu	a4,0(a5)
    1910:	d779                	beqz	a4,18de <strlen+0x18>
    1912:	0017c703          	lbu	a4,1(a5)
    1916:	0785                	addi	a5,a5,1
    1918:	d379                	beqz	a4,18de <strlen+0x18>
    191a:	0017c703          	lbu	a4,1(a5)
    191e:	0785                	addi	a5,a5,1
    1920:	fb6d                	bnez	a4,1912 <strlen+0x4c>
    1922:	bf75                	j	18de <strlen+0x18>

0000000000001924 <memchr>:

void *memchr(const void *src, int c, size_t n)
{
    const unsigned char *s = src;
    c = (unsigned char)c;
    for (; ((uintptr_t)s & ALIGN) && n && *s != c; s++, n--)
    1924:	00757713          	andi	a4,a0,7
{
    1928:	87aa                	mv	a5,a0
    c = (unsigned char)c;
    192a:	0ff5f593          	andi	a1,a1,255
    for (; ((uintptr_t)s & ALIGN) && n && *s != c; s++, n--)
    192e:	cb19                	beqz	a4,1944 <memchr+0x20>
    1930:	ce25                	beqz	a2,19a8 <memchr+0x84>
    1932:	0007c703          	lbu	a4,0(a5)
    1936:	04b70e63          	beq	a4,a1,1992 <memchr+0x6e>
    193a:	0785                	addi	a5,a5,1
    193c:	0077f713          	andi	a4,a5,7
    1940:	167d                	addi	a2,a2,-1
    1942:	f77d                	bnez	a4,1930 <memchr+0xc>
            ;
        s = (const void *)w;
    }
    for (; n && *s != c; s++, n--)
        ;
    return n ? (void *)s : 0;
    1944:	4501                	li	a0,0
    if (n && *s != c)
    1946:	c235                	beqz	a2,19aa <memchr+0x86>
    1948:	0007c703          	lbu	a4,0(a5)
    194c:	04b70363          	beq	a4,a1,1992 <memchr+0x6e>
        size_t k = ONES * c;
    1950:	00000517          	auipc	a0,0x0
    1954:	67053503          	ld	a0,1648(a0) # 1fc0 <__clone+0xe2>
        for (w = (const void *)s; n >= SS && !HASZERO(*w ^ k); w++, n -= SS)
    1958:	471d                	li	a4,7
        size_t k = ONES * c;
    195a:	02a58533          	mul	a0,a1,a0
        for (w = (const void *)s; n >= SS && !HASZERO(*w ^ k); w++, n -= SS)
    195e:	02c77a63          	bgeu	a4,a2,1992 <memchr+0x6e>
    1962:	00000897          	auipc	a7,0x0
    1966:	64e8b883          	ld	a7,1614(a7) # 1fb0 <__clone+0xd2>
    196a:	00000817          	auipc	a6,0x0
    196e:	64e83803          	ld	a6,1614(a6) # 1fb8 <__clone+0xda>
    1972:	431d                	li	t1,7
    1974:	a029                	j	197e <memchr+0x5a>
    1976:	1661                	addi	a2,a2,-8
    1978:	07a1                	addi	a5,a5,8
    197a:	02c37963          	bgeu	t1,a2,19ac <memchr+0x88>
    197e:	6398                	ld	a4,0(a5)
    1980:	8f29                	xor	a4,a4,a0
    1982:	011706b3          	add	a3,a4,a7
    1986:	fff74713          	not	a4,a4
    198a:	8f75                	and	a4,a4,a3
    198c:	01077733          	and	a4,a4,a6
    1990:	d37d                	beqz	a4,1976 <memchr+0x52>
    1992:	853e                	mv	a0,a5
    1994:	97b2                	add	a5,a5,a2
    1996:	a021                	j	199e <memchr+0x7a>
    for (; n && *s != c; s++, n--)
    1998:	0505                	addi	a0,a0,1
    199a:	00f50763          	beq	a0,a5,19a8 <memchr+0x84>
    199e:	00054703          	lbu	a4,0(a0)
    19a2:	feb71be3          	bne	a4,a1,1998 <memchr+0x74>
    19a6:	8082                	ret
    return n ? (void *)s : 0;
    19a8:	4501                	li	a0,0
}
    19aa:	8082                	ret
    return n ? (void *)s : 0;
    19ac:	4501                	li	a0,0
    for (; n && *s != c; s++, n--)
    19ae:	f275                	bnez	a2,1992 <memchr+0x6e>
}
    19b0:	8082                	ret

00000000000019b2 <strnlen>:

size_t strnlen(const char *s, size_t n)
{
    19b2:	1101                	addi	sp,sp,-32
    19b4:	e822                	sd	s0,16(sp)
    const char *p = memchr(s, 0, n);
    19b6:	862e                	mv	a2,a1
{
    19b8:	842e                	mv	s0,a1
    const char *p = memchr(s, 0, n);
    19ba:	4581                	li	a1,0
{
    19bc:	e426                	sd	s1,8(sp)
    19be:	ec06                	sd	ra,24(sp)
    19c0:	84aa                	mv	s1,a0
    const char *p = memchr(s, 0, n);
    19c2:	f63ff0ef          	jal	ra,1924 <memchr>
    return p ? p - s : n;
    19c6:	c519                	beqz	a0,19d4 <strnlen+0x22>
}
    19c8:	60e2                	ld	ra,24(sp)
    19ca:	6442                	ld	s0,16(sp)
    return p ? p - s : n;
    19cc:	8d05                	sub	a0,a0,s1
}
    19ce:	64a2                	ld	s1,8(sp)
    19d0:	6105                	addi	sp,sp,32
    19d2:	8082                	ret
    19d4:	60e2                	ld	ra,24(sp)
    return p ? p - s : n;
    19d6:	8522                	mv	a0,s0
}
    19d8:	6442                	ld	s0,16(sp)
    19da:	64a2                	ld	s1,8(sp)
    19dc:	6105                	addi	sp,sp,32
    19de:	8082                	ret

00000000000019e0 <strcpy>:
char *strcpy(char *restrict d, const char *s)
{
    typedef size_t __attribute__((__may_alias__)) word;
    word *wd;
    const word *ws;
    if ((uintptr_t)s % SS == (uintptr_t)d % SS)
    19e0:	00b547b3          	xor	a5,a0,a1
    19e4:	8b9d                	andi	a5,a5,7
    19e6:	eb95                	bnez	a5,1a1a <strcpy+0x3a>
    {
        for (; (uintptr_t)s % SS; s++, d++)
    19e8:	0075f793          	andi	a5,a1,7
    19ec:	e7b1                	bnez	a5,1a38 <strcpy+0x58>
            if (!(*d = *s))
                return d;
        wd = (void *)d;
        ws = (const void *)s;
        for (; !HASZERO(*ws); *wd++ = *ws++)
    19ee:	6198                	ld	a4,0(a1)
    19f0:	00000617          	auipc	a2,0x0
    19f4:	5c063603          	ld	a2,1472(a2) # 1fb0 <__clone+0xd2>
    19f8:	00000817          	auipc	a6,0x0
    19fc:	5c083803          	ld	a6,1472(a6) # 1fb8 <__clone+0xda>
    1a00:	a029                	j	1a0a <strcpy+0x2a>
    1a02:	e118                	sd	a4,0(a0)
    1a04:	6598                	ld	a4,8(a1)
    1a06:	05a1                	addi	a1,a1,8
    1a08:	0521                	addi	a0,a0,8
    1a0a:	00c707b3          	add	a5,a4,a2
    1a0e:	fff74693          	not	a3,a4
    1a12:	8ff5                	and	a5,a5,a3
    1a14:	0107f7b3          	and	a5,a5,a6
    1a18:	d7ed                	beqz	a5,1a02 <strcpy+0x22>
            ;
        d = (void *)wd;
        s = (const void *)ws;
    }
    for (; (*d = *s); s++, d++)
    1a1a:	0005c783          	lbu	a5,0(a1)
    1a1e:	00f50023          	sb	a5,0(a0)
    1a22:	c785                	beqz	a5,1a4a <strcpy+0x6a>
    1a24:	0015c783          	lbu	a5,1(a1)
    1a28:	0505                	addi	a0,a0,1
    1a2a:	0585                	addi	a1,a1,1
    1a2c:	00f50023          	sb	a5,0(a0)
    1a30:	fbf5                	bnez	a5,1a24 <strcpy+0x44>
        ;
    return d;
}
    1a32:	8082                	ret
        for (; (uintptr_t)s % SS; s++, d++)
    1a34:	0505                	addi	a0,a0,1
    1a36:	df45                	beqz	a4,19ee <strcpy+0xe>
            if (!(*d = *s))
    1a38:	0005c783          	lbu	a5,0(a1)
        for (; (uintptr_t)s % SS; s++, d++)
    1a3c:	0585                	addi	a1,a1,1
    1a3e:	0075f713          	andi	a4,a1,7
            if (!(*d = *s))
    1a42:	00f50023          	sb	a5,0(a0)
    1a46:	f7fd                	bnez	a5,1a34 <strcpy+0x54>
}
    1a48:	8082                	ret
    1a4a:	8082                	ret

0000000000001a4c <strncpy>:
char *strncpy(char *restrict d, const char *s, size_t n)
{
    typedef size_t __attribute__((__may_alias__)) word;
    word *wd;
    const word *ws;
    if (((uintptr_t)s & ALIGN) == ((uintptr_t)d & ALIGN))
    1a4c:	00b547b3          	xor	a5,a0,a1
    1a50:	8b9d                	andi	a5,a5,7
    1a52:	1a079863          	bnez	a5,1c02 <strncpy+0x1b6>
    {
        for (; ((uintptr_t)s & ALIGN) && n && (*d = *s); n--, s++, d++)
    1a56:	0075f793          	andi	a5,a1,7
    1a5a:	16078463          	beqz	a5,1bc2 <strncpy+0x176>
    1a5e:	ea01                	bnez	a2,1a6e <strncpy+0x22>
    1a60:	a421                	j	1c68 <strncpy+0x21c>
    1a62:	167d                	addi	a2,a2,-1
    1a64:	0505                	addi	a0,a0,1
    1a66:	14070e63          	beqz	a4,1bc2 <strncpy+0x176>
    1a6a:	1a060863          	beqz	a2,1c1a <strncpy+0x1ce>
    1a6e:	0005c783          	lbu	a5,0(a1)
    1a72:	0585                	addi	a1,a1,1
    1a74:	0075f713          	andi	a4,a1,7
    1a78:	00f50023          	sb	a5,0(a0)
    1a7c:	f3fd                	bnez	a5,1a62 <strncpy+0x16>
    1a7e:	4805                	li	a6,1
    1a80:	1a061863          	bnez	a2,1c30 <strncpy+0x1e4>
    1a84:	40a007b3          	neg	a5,a0
    1a88:	8b9d                	andi	a5,a5,7
    1a8a:	4681                	li	a3,0
    1a8c:	18061a63          	bnez	a2,1c20 <strncpy+0x1d4>
    1a90:	00778713          	addi	a4,a5,7
    1a94:	45ad                	li	a1,11
    1a96:	18b76363          	bltu	a4,a1,1c1c <strncpy+0x1d0>
    1a9a:	1ae6eb63          	bltu	a3,a4,1c50 <strncpy+0x204>
    1a9e:	1a078363          	beqz	a5,1c44 <strncpy+0x1f8>
    for (int i = 0; i < n; ++i, *(p++) = c)
    1aa2:	00050023          	sb	zero,0(a0)
    1aa6:	4685                	li	a3,1
    1aa8:	00150713          	addi	a4,a0,1
    1aac:	18d78f63          	beq	a5,a3,1c4a <strncpy+0x1fe>
    1ab0:	000500a3          	sb	zero,1(a0)
    1ab4:	4689                	li	a3,2
    1ab6:	00250713          	addi	a4,a0,2
    1aba:	18d78e63          	beq	a5,a3,1c56 <strncpy+0x20a>
    1abe:	00050123          	sb	zero,2(a0)
    1ac2:	468d                	li	a3,3
    1ac4:	00350713          	addi	a4,a0,3
    1ac8:	16d78c63          	beq	a5,a3,1c40 <strncpy+0x1f4>
    1acc:	000501a3          	sb	zero,3(a0)
    1ad0:	4691                	li	a3,4
    1ad2:	00450713          	addi	a4,a0,4
    1ad6:	18d78263          	beq	a5,a3,1c5a <strncpy+0x20e>
    1ada:	00050223          	sb	zero,4(a0)
    1ade:	4695                	li	a3,5
    1ae0:	00550713          	addi	a4,a0,5
    1ae4:	16d78d63          	beq	a5,a3,1c5e <strncpy+0x212>
    1ae8:	000502a3          	sb	zero,5(a0)
    1aec:	469d                	li	a3,7
    1aee:	00650713          	addi	a4,a0,6
    1af2:	16d79863          	bne	a5,a3,1c62 <strncpy+0x216>
    1af6:	00750713          	addi	a4,a0,7
    1afa:	00050323          	sb	zero,6(a0)
    1afe:	40f80833          	sub	a6,a6,a5
    1b02:	ff887593          	andi	a1,a6,-8
    1b06:	97aa                	add	a5,a5,a0
    1b08:	95be                	add	a1,a1,a5
    1b0a:	0007b023          	sd	zero,0(a5)
    1b0e:	07a1                	addi	a5,a5,8
    1b10:	feb79de3          	bne	a5,a1,1b0a <strncpy+0xbe>
    1b14:	ff887593          	andi	a1,a6,-8
    1b18:	9ead                	addw	a3,a3,a1
    1b1a:	00b707b3          	add	a5,a4,a1
    1b1e:	12b80863          	beq	a6,a1,1c4e <strncpy+0x202>
    1b22:	00078023          	sb	zero,0(a5)
    1b26:	0016871b          	addiw	a4,a3,1
    1b2a:	0ec77863          	bgeu	a4,a2,1c1a <strncpy+0x1ce>
    1b2e:	000780a3          	sb	zero,1(a5)
    1b32:	0026871b          	addiw	a4,a3,2
    1b36:	0ec77263          	bgeu	a4,a2,1c1a <strncpy+0x1ce>
    1b3a:	00078123          	sb	zero,2(a5)
    1b3e:	0036871b          	addiw	a4,a3,3
    1b42:	0cc77c63          	bgeu	a4,a2,1c1a <strncpy+0x1ce>
    1b46:	000781a3          	sb	zero,3(a5)
    1b4a:	0046871b          	addiw	a4,a3,4
    1b4e:	0cc77663          	bgeu	a4,a2,1c1a <strncpy+0x1ce>
    1b52:	00078223          	sb	zero,4(a5)
    1b56:	0056871b          	addiw	a4,a3,5
    1b5a:	0cc77063          	bgeu	a4,a2,1c1a <strncpy+0x1ce>
    1b5e:	000782a3          	sb	zero,5(a5)
    1b62:	0066871b          	addiw	a4,a3,6
    1b66:	0ac77a63          	bgeu	a4,a2,1c1a <strncpy+0x1ce>
    1b6a:	00078323          	sb	zero,6(a5)
    1b6e:	0076871b          	addiw	a4,a3,7
    1b72:	0ac77463          	bgeu	a4,a2,1c1a <strncpy+0x1ce>
    1b76:	000783a3          	sb	zero,7(a5)
    1b7a:	0086871b          	addiw	a4,a3,8
    1b7e:	08c77e63          	bgeu	a4,a2,1c1a <strncpy+0x1ce>
    1b82:	00078423          	sb	zero,8(a5)
    1b86:	0096871b          	addiw	a4,a3,9
    1b8a:	08c77863          	bgeu	a4,a2,1c1a <strncpy+0x1ce>
    1b8e:	000784a3          	sb	zero,9(a5)
    1b92:	00a6871b          	addiw	a4,a3,10
    1b96:	08c77263          	bgeu	a4,a2,1c1a <strncpy+0x1ce>
    1b9a:	00078523          	sb	zero,10(a5)
    1b9e:	00b6871b          	addiw	a4,a3,11
    1ba2:	06c77c63          	bgeu	a4,a2,1c1a <strncpy+0x1ce>
    1ba6:	000785a3          	sb	zero,11(a5)
    1baa:	00c6871b          	addiw	a4,a3,12
    1bae:	06c77663          	bgeu	a4,a2,1c1a <strncpy+0x1ce>
    1bb2:	00078623          	sb	zero,12(a5)
    1bb6:	26b5                	addiw	a3,a3,13
    1bb8:	06c6f163          	bgeu	a3,a2,1c1a <strncpy+0x1ce>
    1bbc:	000786a3          	sb	zero,13(a5)
    1bc0:	8082                	ret
            ;
        if (!n || !*s)
    1bc2:	c645                	beqz	a2,1c6a <strncpy+0x21e>
    1bc4:	0005c783          	lbu	a5,0(a1)
    1bc8:	ea078be3          	beqz	a5,1a7e <strncpy+0x32>
            goto tail;
        wd = (void *)d;
        ws = (const void *)s;
        for (; n >= sizeof(size_t) && !HASZERO(*ws); n -= sizeof(size_t), ws++, wd++)
    1bcc:	479d                	li	a5,7
    1bce:	02c7ff63          	bgeu	a5,a2,1c0c <strncpy+0x1c0>
    1bd2:	00000897          	auipc	a7,0x0
    1bd6:	3de8b883          	ld	a7,990(a7) # 1fb0 <__clone+0xd2>
    1bda:	00000817          	auipc	a6,0x0
    1bde:	3de83803          	ld	a6,990(a6) # 1fb8 <__clone+0xda>
    1be2:	431d                	li	t1,7
    1be4:	6198                	ld	a4,0(a1)
    1be6:	011707b3          	add	a5,a4,a7
    1bea:	fff74693          	not	a3,a4
    1bee:	8ff5                	and	a5,a5,a3
    1bf0:	0107f7b3          	and	a5,a5,a6
    1bf4:	ef81                	bnez	a5,1c0c <strncpy+0x1c0>
            *wd = *ws;
    1bf6:	e118                	sd	a4,0(a0)
        for (; n >= sizeof(size_t) && !HASZERO(*ws); n -= sizeof(size_t), ws++, wd++)
    1bf8:	1661                	addi	a2,a2,-8
    1bfa:	05a1                	addi	a1,a1,8
    1bfc:	0521                	addi	a0,a0,8
    1bfe:	fec363e3          	bltu	t1,a2,1be4 <strncpy+0x198>
        d = (void *)wd;
        s = (const void *)ws;
    }
    for (; n && (*d = *s); n--, s++, d++)
    1c02:	e609                	bnez	a2,1c0c <strncpy+0x1c0>
    1c04:	a08d                	j	1c66 <strncpy+0x21a>
    1c06:	167d                	addi	a2,a2,-1
    1c08:	0505                	addi	a0,a0,1
    1c0a:	ca01                	beqz	a2,1c1a <strncpy+0x1ce>
    1c0c:	0005c783          	lbu	a5,0(a1)
    1c10:	0585                	addi	a1,a1,1
    1c12:	00f50023          	sb	a5,0(a0)
    1c16:	fbe5                	bnez	a5,1c06 <strncpy+0x1ba>
        ;
tail:
    1c18:	b59d                	j	1a7e <strncpy+0x32>
    memset(d, 0, n);
    return d;
}
    1c1a:	8082                	ret
    1c1c:	472d                	li	a4,11
    1c1e:	bdb5                	j	1a9a <strncpy+0x4e>
    1c20:	00778713          	addi	a4,a5,7
    1c24:	45ad                	li	a1,11
    1c26:	fff60693          	addi	a3,a2,-1
    1c2a:	e6b778e3          	bgeu	a4,a1,1a9a <strncpy+0x4e>
    1c2e:	b7fd                	j	1c1c <strncpy+0x1d0>
    1c30:	40a007b3          	neg	a5,a0
    1c34:	8832                	mv	a6,a2
    1c36:	8b9d                	andi	a5,a5,7
    1c38:	4681                	li	a3,0
    1c3a:	e4060be3          	beqz	a2,1a90 <strncpy+0x44>
    1c3e:	b7cd                	j	1c20 <strncpy+0x1d4>
    for (int i = 0; i < n; ++i, *(p++) = c)
    1c40:	468d                	li	a3,3
    1c42:	bd75                	j	1afe <strncpy+0xb2>
    1c44:	872a                	mv	a4,a0
    1c46:	4681                	li	a3,0
    1c48:	bd5d                	j	1afe <strncpy+0xb2>
    1c4a:	4685                	li	a3,1
    1c4c:	bd4d                	j	1afe <strncpy+0xb2>
    1c4e:	8082                	ret
    1c50:	87aa                	mv	a5,a0
    1c52:	4681                	li	a3,0
    1c54:	b5f9                	j	1b22 <strncpy+0xd6>
    1c56:	4689                	li	a3,2
    1c58:	b55d                	j	1afe <strncpy+0xb2>
    1c5a:	4691                	li	a3,4
    1c5c:	b54d                	j	1afe <strncpy+0xb2>
    1c5e:	4695                	li	a3,5
    1c60:	bd79                	j	1afe <strncpy+0xb2>
    1c62:	4699                	li	a3,6
    1c64:	bd69                	j	1afe <strncpy+0xb2>
    1c66:	8082                	ret
    1c68:	8082                	ret
    1c6a:	8082                	ret

0000000000001c6c <open>:
#include <unistd.h>

#include "syscall.h"

int open(const char *path, int flags)
{
    1c6c:	87aa                	mv	a5,a0
    1c6e:	862e                	mv	a2,a1
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
}

static inline long __syscall4(long n, long a, long b, long c, long d)
{
    register long a7 __asm__("a7") = n;
    1c70:	03800893          	li	a7,56
    register long a0 __asm__("a0") = a;
    1c74:	f9c00513          	li	a0,-100
    register long a1 __asm__("a1") = b;
    1c78:	85be                	mv	a1,a5
    register long a2 __asm__("a2") = c;
    register long a3 __asm__("a3") = d;
    1c7a:	4689                	li	a3,2
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3))
    1c7c:	00000073          	ecall
    return syscall(SYS_openat, AT_FDCWD, path, flags, O_RDWR);
}
    1c80:	2501                	sext.w	a0,a0
    1c82:	8082                	ret

0000000000001c84 <openat>:
    register long a7 __asm__("a7") = n;
    1c84:	03800893          	li	a7,56
    register long a3 __asm__("a3") = d;
    1c88:	18000693          	li	a3,384
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3))
    1c8c:	00000073          	ecall

int openat(int dirfd,const char *path, int flags)
{
    return syscall(SYS_openat, dirfd, path, flags, 0600);
}
    1c90:	2501                	sext.w	a0,a0
    1c92:	8082                	ret

0000000000001c94 <close>:
    register long a7 __asm__("a7") = n;
    1c94:	03900893          	li	a7,57
    __asm_syscall("r"(a7), "0"(a0))
    1c98:	00000073          	ecall

int close(int fd)
{
    return syscall(SYS_close, fd);
}
    1c9c:	2501                	sext.w	a0,a0
    1c9e:	8082                	ret

0000000000001ca0 <read>:
    register long a7 __asm__("a7") = n;
    1ca0:	03f00893          	li	a7,63
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1ca4:	00000073          	ecall

ssize_t read(int fd, void *buf, size_t len)
{
    return syscall(SYS_read, fd, buf, len);
}
    1ca8:	8082                	ret

0000000000001caa <write>:
    register long a7 __asm__("a7") = n;
    1caa:	04000893          	li	a7,64
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1cae:	00000073          	ecall

ssize_t write(int fd, const void *buf, size_t len)
{
    return syscall(SYS_write, fd, buf, len);
}
    1cb2:	8082                	ret

0000000000001cb4 <getpid>:
    register long a7 __asm__("a7") = n;
    1cb4:	0ac00893          	li	a7,172
    __asm_syscall("r"(a7))
    1cb8:	00000073          	ecall

pid_t getpid(void)
{
    return syscall(SYS_getpid);
}
    1cbc:	2501                	sext.w	a0,a0
    1cbe:	8082                	ret

0000000000001cc0 <getppid>:
    register long a7 __asm__("a7") = n;
    1cc0:	0ad00893          	li	a7,173
    __asm_syscall("r"(a7))
    1cc4:	00000073          	ecall

pid_t getppid(void)
{
    return syscall(SYS_getppid);
}
    1cc8:	2501                	sext.w	a0,a0
    1cca:	8082                	ret

0000000000001ccc <sched_yield>:
    register long a7 __asm__("a7") = n;
    1ccc:	07c00893          	li	a7,124
    __asm_syscall("r"(a7))
    1cd0:	00000073          	ecall

int sched_yield(void)
{
    return syscall(SYS_sched_yield);
}
    1cd4:	2501                	sext.w	a0,a0
    1cd6:	8082                	ret

0000000000001cd8 <fork>:
    register long a7 __asm__("a7") = n;
    1cd8:	0dc00893          	li	a7,220
    register long a0 __asm__("a0") = a;
    1cdc:	4545                	li	a0,17
    register long a1 __asm__("a1") = b;
    1cde:	4581                	li	a1,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1ce0:	00000073          	ecall

pid_t fork(void)
{
    return syscall(SYS_clone, SIGCHLD, 0);
}
    1ce4:	2501                	sext.w	a0,a0
    1ce6:	8082                	ret

0000000000001ce8 <clone>:

pid_t clone(int (*fn)(void *arg), void *arg, void *stack, size_t stack_size, unsigned long flags)
{
    1ce8:	85b2                	mv	a1,a2
    1cea:	863a                	mv	a2,a4
    if (stack)
    1cec:	c191                	beqz	a1,1cf0 <clone+0x8>
	stack += stack_size;
    1cee:	95b6                	add	a1,a1,a3

    return __clone(fn, stack, flags, NULL, NULL, NULL);
    1cf0:	4781                	li	a5,0
    1cf2:	4701                	li	a4,0
    1cf4:	4681                	li	a3,0
    1cf6:	2601                	sext.w	a2,a2
    1cf8:	a2dd                	j	1ede <__clone>

0000000000001cfa <exit>:
    register long a7 __asm__("a7") = n;
    1cfa:	05d00893          	li	a7,93
    __asm_syscall("r"(a7), "0"(a0))
    1cfe:	00000073          	ecall
    //return syscall(SYS_clone, fn, stack, flags, NULL, NULL, NULL);
}
void exit(int code)
{
    syscall(SYS_exit, code);
}
    1d02:	8082                	ret

0000000000001d04 <waitpid>:
    register long a7 __asm__("a7") = n;
    1d04:	10400893          	li	a7,260
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1d08:	00000073          	ecall

int waitpid(int pid, int *code, int options)
{
    return syscall(SYS_wait4, pid, code, options);
}
    1d0c:	2501                	sext.w	a0,a0
    1d0e:	8082                	ret

0000000000001d10 <exec>:
    register long a7 __asm__("a7") = n;
    1d10:	0dd00893          	li	a7,221
    __asm_syscall("r"(a7), "0"(a0))
    1d14:	00000073          	ecall

int exec(char *name)
{
    return syscall(SYS_execve, name);
}
    1d18:	2501                	sext.w	a0,a0
    1d1a:	8082                	ret

0000000000001d1c <execve>:
    register long a7 __asm__("a7") = n;
    1d1c:	0dd00893          	li	a7,221
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1d20:	00000073          	ecall

int execve(const char *name, char *const argv[], char *const argp[])
{
    return syscall(SYS_execve, name, argv, argp);
}
    1d24:	2501                	sext.w	a0,a0
    1d26:	8082                	ret

0000000000001d28 <times>:
    register long a7 __asm__("a7") = n;
    1d28:	09900893          	li	a7,153
    __asm_syscall("r"(a7), "0"(a0))
    1d2c:	00000073          	ecall

int times(void *mytimes)
{
	return syscall(SYS_times, mytimes);
}
    1d30:	2501                	sext.w	a0,a0
    1d32:	8082                	ret

0000000000001d34 <get_time>:

int64 get_time()
{
    1d34:	1141                	addi	sp,sp,-16
    register long a7 __asm__("a7") = n;
    1d36:	0a900893          	li	a7,169
    register long a0 __asm__("a0") = a;
    1d3a:	850a                	mv	a0,sp
    register long a1 __asm__("a1") = b;
    1d3c:	4581                	li	a1,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1d3e:	00000073          	ecall
    TimeVal time;
    int err = sys_get_time(&time, 0);
    if (err == 0)
    1d42:	2501                	sext.w	a0,a0
    1d44:	ed09                	bnez	a0,1d5e <get_time+0x2a>
    {
        return ((time.sec & 0xffff) * 1000 + time.usec / 1000);
    1d46:	67a2                	ld	a5,8(sp)
    1d48:	3e800713          	li	a4,1000
    1d4c:	00015503          	lhu	a0,0(sp)
    1d50:	02e7d7b3          	divu	a5,a5,a4
    1d54:	02e50533          	mul	a0,a0,a4
    1d58:	953e                	add	a0,a0,a5
    }
    else
    {
        return -1;
    }
}
    1d5a:	0141                	addi	sp,sp,16
    1d5c:	8082                	ret
        return -1;
    1d5e:	557d                	li	a0,-1
    1d60:	bfed                	j	1d5a <get_time+0x26>

0000000000001d62 <sys_get_time>:
    register long a7 __asm__("a7") = n;
    1d62:	0a900893          	li	a7,169
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1d66:	00000073          	ecall

int sys_get_time(TimeVal *ts, int tz)
{
    return syscall(SYS_gettimeofday, ts, tz);
}
    1d6a:	2501                	sext.w	a0,a0
    1d6c:	8082                	ret

0000000000001d6e <time>:
    register long a7 __asm__("a7") = n;
    1d6e:	42600893          	li	a7,1062
    __asm_syscall("r"(a7), "0"(a0))
    1d72:	00000073          	ecall

int time(unsigned long *tloc)
{
    return syscall(SYS_time, tloc);
}
    1d76:	2501                	sext.w	a0,a0
    1d78:	8082                	ret

0000000000001d7a <sleep>:

int sleep(unsigned long long time)
{
    1d7a:	1141                	addi	sp,sp,-16
    TimeVal tv = {.sec = time, .usec = 0};
    1d7c:	e02a                	sd	a0,0(sp)
    register long a0 __asm__("a0") = a;
    1d7e:	850a                	mv	a0,sp
    1d80:	e402                	sd	zero,8(sp)
    register long a7 __asm__("a7") = n;
    1d82:	06500893          	li	a7,101
    register long a1 __asm__("a1") = b;
    1d86:	85aa                	mv	a1,a0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1d88:	00000073          	ecall
    if (syscall(SYS_nanosleep, &tv, &tv)) return tv.sec;
    1d8c:	e501                	bnez	a0,1d94 <sleep+0x1a>
    return 0;
    1d8e:	4501                	li	a0,0
}
    1d90:	0141                	addi	sp,sp,16
    1d92:	8082                	ret
    if (syscall(SYS_nanosleep, &tv, &tv)) return tv.sec;
    1d94:	4502                	lw	a0,0(sp)
}
    1d96:	0141                	addi	sp,sp,16
    1d98:	8082                	ret

0000000000001d9a <set_priority>:
    register long a7 __asm__("a7") = n;
    1d9a:	08c00893          	li	a7,140
    __asm_syscall("r"(a7), "0"(a0))
    1d9e:	00000073          	ecall

int set_priority(int prio)
{
    return syscall(SYS_setpriority, prio);
}
    1da2:	2501                	sext.w	a0,a0
    1da4:	8082                	ret

0000000000001da6 <mmap>:
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4))
}

static inline long __syscall6(long n, long a, long b, long c, long d, long e, long f)
{
    register long a7 __asm__("a7") = n;
    1da6:	0de00893          	li	a7,222
    register long a1 __asm__("a1") = b;
    register long a2 __asm__("a2") = c;
    register long a3 __asm__("a3") = d;
    register long a4 __asm__("a4") = e;
    register long a5 __asm__("a5") = f;
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4), "r"(a5))
    1daa:	00000073          	ecall

void *mmap(void *start, size_t len, int prot, int flags, int fd, off_t off)
{
    return syscall(SYS_mmap, start, len, prot, flags, fd, off);
}
    1dae:	8082                	ret

0000000000001db0 <munmap>:
    register long a7 __asm__("a7") = n;
    1db0:	0d700893          	li	a7,215
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1db4:	00000073          	ecall

int munmap(void *start, size_t len)
{
    return syscall(SYS_munmap, start, len);
}
    1db8:	2501                	sext.w	a0,a0
    1dba:	8082                	ret

0000000000001dbc <wait>:

int wait(int *code)
{
    1dbc:	85aa                	mv	a1,a0
    register long a7 __asm__("a7") = n;
    1dbe:	10400893          	li	a7,260
    register long a0 __asm__("a0") = a;
    1dc2:	557d                	li	a0,-1
    register long a2 __asm__("a2") = c;
    1dc4:	4601                	li	a2,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1dc6:	00000073          	ecall
    return waitpid(-1, code, 0);
}
    1dca:	2501                	sext.w	a0,a0
    1dcc:	8082                	ret

0000000000001dce <spawn>:
    register long a7 __asm__("a7") = n;
    1dce:	19000893          	li	a7,400
    __asm_syscall("r"(a7), "0"(a0))
    1dd2:	00000073          	ecall

int spawn(char *file)
{
    return syscall(SYS_spawn, file);
}
    1dd6:	2501                	sext.w	a0,a0
    1dd8:	8082                	ret

0000000000001dda <mailread>:
    register long a7 __asm__("a7") = n;
    1dda:	19100893          	li	a7,401
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1dde:	00000073          	ecall

int mailread(void *buf, int len)
{
    return syscall(SYS_mailread, buf, len);
}
    1de2:	2501                	sext.w	a0,a0
    1de4:	8082                	ret

0000000000001de6 <mailwrite>:
    register long a7 __asm__("a7") = n;
    1de6:	19200893          	li	a7,402
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1dea:	00000073          	ecall

int mailwrite(int pid, void *buf, int len)
{
    return syscall(SYS_mailwrite, pid, buf, len);
}
    1dee:	2501                	sext.w	a0,a0
    1df0:	8082                	ret

0000000000001df2 <fstat>:
    register long a7 __asm__("a7") = n;
    1df2:	05000893          	li	a7,80
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1df6:	00000073          	ecall

int fstat(int fd, struct kstat *st)
{
    return syscall(SYS_fstat, fd, st);
}
    1dfa:	2501                	sext.w	a0,a0
    1dfc:	8082                	ret

0000000000001dfe <sys_linkat>:
    register long a4 __asm__("a4") = e;
    1dfe:	1702                	slli	a4,a4,0x20
    register long a7 __asm__("a7") = n;
    1e00:	02500893          	li	a7,37
    register long a4 __asm__("a4") = e;
    1e04:	9301                	srli	a4,a4,0x20
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4))
    1e06:	00000073          	ecall

int sys_linkat(int olddirfd, char *oldpath, int newdirfd, char *newpath, unsigned int flags)
{
    return syscall(SYS_linkat, olddirfd, oldpath, newdirfd, newpath, flags);
}
    1e0a:	2501                	sext.w	a0,a0
    1e0c:	8082                	ret

0000000000001e0e <sys_unlinkat>:
    register long a2 __asm__("a2") = c;
    1e0e:	1602                	slli	a2,a2,0x20
    register long a7 __asm__("a7") = n;
    1e10:	02300893          	li	a7,35
    register long a2 __asm__("a2") = c;
    1e14:	9201                	srli	a2,a2,0x20
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1e16:	00000073          	ecall

int sys_unlinkat(int dirfd, char *path, unsigned int flags)
{
    return syscall(SYS_unlinkat, dirfd, path, flags);
}
    1e1a:	2501                	sext.w	a0,a0
    1e1c:	8082                	ret

0000000000001e1e <link>:

int link(char *old_path, char *new_path)
{
    1e1e:	87aa                	mv	a5,a0
    1e20:	86ae                	mv	a3,a1
    register long a7 __asm__("a7") = n;
    1e22:	02500893          	li	a7,37
    register long a0 __asm__("a0") = a;
    1e26:	f9c00513          	li	a0,-100
    register long a1 __asm__("a1") = b;
    1e2a:	85be                	mv	a1,a5
    register long a2 __asm__("a2") = c;
    1e2c:	f9c00613          	li	a2,-100
    register long a4 __asm__("a4") = e;
    1e30:	4701                	li	a4,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4))
    1e32:	00000073          	ecall
    return sys_linkat(AT_FDCWD, old_path, AT_FDCWD, new_path, 0);
}
    1e36:	2501                	sext.w	a0,a0
    1e38:	8082                	ret

0000000000001e3a <unlink>:

int unlink(char *path)
{
    1e3a:	85aa                	mv	a1,a0
    register long a7 __asm__("a7") = n;
    1e3c:	02300893          	li	a7,35
    register long a0 __asm__("a0") = a;
    1e40:	f9c00513          	li	a0,-100
    register long a2 __asm__("a2") = c;
    1e44:	4601                	li	a2,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1e46:	00000073          	ecall
    return sys_unlinkat(AT_FDCWD, path, 0);
}
    1e4a:	2501                	sext.w	a0,a0
    1e4c:	8082                	ret

0000000000001e4e <uname>:
    register long a7 __asm__("a7") = n;
    1e4e:	0a000893          	li	a7,160
    __asm_syscall("r"(a7), "0"(a0))
    1e52:	00000073          	ecall

int uname(void *buf)
{
    return syscall(SYS_uname, buf);
}
    1e56:	2501                	sext.w	a0,a0
    1e58:	8082                	ret

0000000000001e5a <brk>:
    register long a7 __asm__("a7") = n;
    1e5a:	0d600893          	li	a7,214
    __asm_syscall("r"(a7), "0"(a0))
    1e5e:	00000073          	ecall

int brk(void *addr)
{
    return syscall(SYS_brk, addr);
}
    1e62:	2501                	sext.w	a0,a0
    1e64:	8082                	ret

0000000000001e66 <getcwd>:
    register long a7 __asm__("a7") = n;
    1e66:	48c5                	li	a7,17
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1e68:	00000073          	ecall

char *getcwd(char *buf, size_t size){
    return syscall(SYS_getcwd, buf, size);
}
    1e6c:	8082                	ret

0000000000001e6e <chdir>:
    register long a7 __asm__("a7") = n;
    1e6e:	03100893          	li	a7,49
    __asm_syscall("r"(a7), "0"(a0))
    1e72:	00000073          	ecall

int chdir(const char *path){
    return syscall(SYS_chdir, path);
}
    1e76:	2501                	sext.w	a0,a0
    1e78:	8082                	ret

0000000000001e7a <mkdir>:

int mkdir(const char *path, mode_t mode){
    1e7a:	862e                	mv	a2,a1
    1e7c:	87aa                	mv	a5,a0
    register long a2 __asm__("a2") = c;
    1e7e:	1602                	slli	a2,a2,0x20
    register long a7 __asm__("a7") = n;
    1e80:	02200893          	li	a7,34
    register long a0 __asm__("a0") = a;
    1e84:	f9c00513          	li	a0,-100
    register long a1 __asm__("a1") = b;
    1e88:	85be                	mv	a1,a5
    register long a2 __asm__("a2") = c;
    1e8a:	9201                	srli	a2,a2,0x20
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1e8c:	00000073          	ecall
    return syscall(SYS_mkdirat, AT_FDCWD, path, mode);
}
    1e90:	2501                	sext.w	a0,a0
    1e92:	8082                	ret

0000000000001e94 <getdents>:
    register long a7 __asm__("a7") = n;
    1e94:	03d00893          	li	a7,61
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1e98:	00000073          	ecall

int getdents(int fd, struct linux_dirent64 *dirp64, unsigned long len){
    //return syscall(SYS_getdents64, fd, dirp64, len);
    return syscall(SYS_getdents64, fd, dirp64, len);
}
    1e9c:	2501                	sext.w	a0,a0
    1e9e:	8082                	ret

0000000000001ea0 <pipe>:
    register long a7 __asm__("a7") = n;
    1ea0:	03b00893          	li	a7,59
    register long a1 __asm__("a1") = b;
    1ea4:	4581                	li	a1,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1ea6:	00000073          	ecall

int pipe(int fd[2]){
    return syscall(SYS_pipe2, fd, 0);
}
    1eaa:	2501                	sext.w	a0,a0
    1eac:	8082                	ret

0000000000001eae <dup>:
    register long a7 __asm__("a7") = n;
    1eae:	48dd                	li	a7,23
    __asm_syscall("r"(a7), "0"(a0))
    1eb0:	00000073          	ecall

int dup(int fd){
    return syscall(SYS_dup, fd);
}
    1eb4:	2501                	sext.w	a0,a0
    1eb6:	8082                	ret

0000000000001eb8 <dup2>:
    register long a7 __asm__("a7") = n;
    1eb8:	48e1                	li	a7,24
    register long a2 __asm__("a2") = c;
    1eba:	4601                	li	a2,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1ebc:	00000073          	ecall

int dup2(int old, int new){
    return syscall(SYS_dup3, old, new, 0);
}
    1ec0:	2501                	sext.w	a0,a0
    1ec2:	8082                	ret

0000000000001ec4 <mount>:
    register long a7 __asm__("a7") = n;
    1ec4:	02800893          	li	a7,40
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4))
    1ec8:	00000073          	ecall

int mount(const char *special, const char *dir, const char *fstype, unsigned long flags, const void *data)
{
        return syscall(SYS_mount, special, dir, fstype, flags, data);
}
    1ecc:	2501                	sext.w	a0,a0
    1ece:	8082                	ret

0000000000001ed0 <umount>:
    register long a7 __asm__("a7") = n;
    1ed0:	02700893          	li	a7,39
    register long a1 __asm__("a1") = b;
    1ed4:	4581                	li	a1,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1ed6:	00000073          	ecall

int umount(const char *special)
{
        return syscall(SYS_umount2, special, 0);
}
    1eda:	2501                	sext.w	a0,a0
    1edc:	8082                	ret

0000000000001ede <__clone>:

.global __clone
.type  __clone, %function
__clone:
	# Save func and arg to stack
	addi a1, a1, -16
    1ede:	15c1                	addi	a1,a1,-16
	sd a0, 0(a1)
    1ee0:	e188                	sd	a0,0(a1)
	sd a3, 8(a1)
    1ee2:	e594                	sd	a3,8(a1)

	# Call SYS_clone
	mv a0, a2
    1ee4:	8532                	mv	a0,a2
	mv a2, a4
    1ee6:	863a                	mv	a2,a4
	mv a3, a5
    1ee8:	86be                	mv	a3,a5
	mv a4, a6
    1eea:	8742                	mv	a4,a6
	li a7, 220 # SYS_clone
    1eec:	0dc00893          	li	a7,220
	ecall
    1ef0:	00000073          	ecall

	beqz a0, 1f
    1ef4:	c111                	beqz	a0,1ef8 <__clone+0x1a>
	# Parent
	ret
    1ef6:	8082                	ret

	# Child
1:      ld a1, 0(sp)
    1ef8:	6582                	ld	a1,0(sp)
	ld a0, 8(sp)
    1efa:	6522                	ld	a0,8(sp)
	jalr a1
    1efc:	9582                	jalr	a1

	# Exit
	li a7, 93 # SYS_exit
    1efe:	05d00893          	li	a7,93
	ecall
    1f02:	00000073          	ecall
