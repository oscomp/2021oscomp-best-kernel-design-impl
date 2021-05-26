
/home/ubuntu/rust/ultra-os/codes/riscv-syscalls-testing/user/build/riscv64/chdir:     file format elf64-littleriscv


Disassembly of section .text:

0000000000001000 <_start>:
.section .text.entry
.globl _start
_start:
    mv a0, sp
    1000:	850a                	mv	a0,sp
    tail __start_main
    1002:	a0c9                	j	10c4 <__start_main>

0000000000001004 <test_chdir>:
#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"

static char buffer[30];
void test_chdir(void){
    1004:	1141                	addi	sp,sp,-16
    TEST_START(__func__);
    1006:	00001517          	auipc	a0,0x1
    100a:	efa50513          	addi	a0,a0,-262 # 1f00 <__clone+0x2e>
void test_chdir(void){
    100e:	e406                	sd	ra,8(sp)
    1010:	e022                	sd	s0,0(sp)
    TEST_START(__func__);
    1012:	324000ef          	jal	ra,1336 <puts>
    1016:	00001517          	auipc	a0,0x1
    101a:	fc250513          	addi	a0,a0,-62 # 1fd8 <__func__.0>
    101e:	318000ef          	jal	ra,1336 <puts>
    1022:	00001517          	auipc	a0,0x1
    1026:	ef650513          	addi	a0,a0,-266 # 1f18 <__clone+0x46>
    102a:	30c000ef          	jal	ra,1336 <puts>
    mkdir("test_chdir", 0666);
    102e:	1b600593          	li	a1,438
    1032:	00001517          	auipc	a0,0x1
    1036:	ef650513          	addi	a0,a0,-266 # 1f28 <__clone+0x56>
    103a:	635000ef          	jal	ra,1e6e <mkdir>
    int ret = chdir("test_chdir");
    103e:	00001517          	auipc	a0,0x1
    1042:	eea50513          	addi	a0,a0,-278 # 1f28 <__clone+0x56>
    1046:	61d000ef          	jal	ra,1e62 <chdir>
    104a:	842a                	mv	s0,a0
    printf("chdir ret: %d\n", ret);
    104c:	85aa                	mv	a1,a0
    104e:	00001517          	auipc	a0,0x1
    1052:	eea50513          	addi	a0,a0,-278 # 1f38 <__clone+0x66>
    1056:	302000ef          	jal	ra,1358 <printf>
    assert(ret == 0);
    105a:	e431                	bnez	s0,10a6 <test_chdir+0xa2>
    getcwd(buffer, 30);
    105c:	45f9                	li	a1,30
    105e:	00001517          	auipc	a0,0x1
    1062:	f5a50513          	addi	a0,a0,-166 # 1fb8 <buffer>
    1066:	5f5000ef          	jal	ra,1e5a <getcwd>
    printf("  current working dir : %s\n", buffer);
    106a:	00001597          	auipc	a1,0x1
    106e:	f4e58593          	addi	a1,a1,-178 # 1fb8 <buffer>
    1072:	00001517          	auipc	a0,0x1
    1076:	ef650513          	addi	a0,a0,-266 # 1f68 <__clone+0x96>
    107a:	2de000ef          	jal	ra,1358 <printf>
    TEST_END(__func__);
    107e:	00001517          	auipc	a0,0x1
    1082:	f0a50513          	addi	a0,a0,-246 # 1f88 <__clone+0xb6>
    1086:	2b0000ef          	jal	ra,1336 <puts>
    108a:	00001517          	auipc	a0,0x1
    108e:	f4e50513          	addi	a0,a0,-178 # 1fd8 <__func__.0>
    1092:	2a4000ef          	jal	ra,1336 <puts>
}
    1096:	6402                	ld	s0,0(sp)
    1098:	60a2                	ld	ra,8(sp)
    TEST_END(__func__);
    109a:	00001517          	auipc	a0,0x1
    109e:	e7e50513          	addi	a0,a0,-386 # 1f18 <__clone+0x46>
}
    10a2:	0141                	addi	sp,sp,16
    TEST_END(__func__);
    10a4:	ac49                	j	1336 <puts>
    assert(ret == 0);
    10a6:	00001517          	auipc	a0,0x1
    10aa:	ea250513          	addi	a0,a0,-350 # 1f48 <__clone+0x76>
    10ae:	52e000ef          	jal	ra,15dc <panic>
    10b2:	b76d                	j	105c <test_chdir+0x58>

00000000000010b4 <main>:

int main(void){
    10b4:	1141                	addi	sp,sp,-16
    10b6:	e406                	sd	ra,8(sp)
    test_chdir();
    10b8:	f4dff0ef          	jal	ra,1004 <test_chdir>
    return 0;
}
    10bc:	60a2                	ld	ra,8(sp)
    10be:	4501                	li	a0,0
    10c0:	0141                	addi	sp,sp,16
    10c2:	8082                	ret

00000000000010c4 <__start_main>:
#include <unistd.h>

extern int main();

int __start_main(long *p)
{
    10c4:	85aa                	mv	a1,a0
	int argc = p[0];
	char **argv = (void *)(p+1);

	exit(main(argc, argv));
    10c6:	4108                	lw	a0,0(a0)
{
    10c8:	1141                	addi	sp,sp,-16
	exit(main(argc, argv));
    10ca:	05a1                	addi	a1,a1,8
{
    10cc:	e406                	sd	ra,8(sp)
	exit(main(argc, argv));
    10ce:	fe7ff0ef          	jal	ra,10b4 <main>
    10d2:	41d000ef          	jal	ra,1cee <exit>
	return 0;
}
    10d6:	60a2                	ld	ra,8(sp)
    10d8:	4501                	li	a0,0
    10da:	0141                	addi	sp,sp,16
    10dc:	8082                	ret

00000000000010de <printint.constprop.0>:
    write(f, s, l);
}

static char digits[] = "0123456789abcdef";

static void printint(int xx, int base, int sign)
    10de:	7179                	addi	sp,sp,-48
    10e0:	f406                	sd	ra,40(sp)
{
    char buf[16 + 1];
    int i;
    uint x;

    if (sign && (sign = xx < 0))
    10e2:	12054b63          	bltz	a0,1218 <printint.constprop.0+0x13a>

    buf[16] = 0;
    i = 15;
    do
    {
        buf[i--] = digits[x % base];
    10e6:	02b577bb          	remuw	a5,a0,a1
    10ea:	00001617          	auipc	a2,0x1
    10ee:	efe60613          	addi	a2,a2,-258 # 1fe8 <digits>
    buf[16] = 0;
    10f2:	00010c23          	sb	zero,24(sp)
        buf[i--] = digits[x % base];
    10f6:	0005871b          	sext.w	a4,a1
    10fa:	1782                	slli	a5,a5,0x20
    10fc:	9381                	srli	a5,a5,0x20
    10fe:	97b2                	add	a5,a5,a2
    1100:	0007c783          	lbu	a5,0(a5)
    } while ((x /= base) != 0);
    1104:	02b5583b          	divuw	a6,a0,a1
        buf[i--] = digits[x % base];
    1108:	00f10ba3          	sb	a5,23(sp)
    } while ((x /= base) != 0);
    110c:	1cb56363          	bltu	a0,a1,12d2 <printint.constprop.0+0x1f4>
        buf[i--] = digits[x % base];
    1110:	45b9                	li	a1,14
    1112:	02e877bb          	remuw	a5,a6,a4
    1116:	1782                	slli	a5,a5,0x20
    1118:	9381                	srli	a5,a5,0x20
    111a:	97b2                	add	a5,a5,a2
    111c:	0007c783          	lbu	a5,0(a5)
    } while ((x /= base) != 0);
    1120:	02e856bb          	divuw	a3,a6,a4
        buf[i--] = digits[x % base];
    1124:	00f10b23          	sb	a5,22(sp)
    } while ((x /= base) != 0);
    1128:	0ce86e63          	bltu	a6,a4,1204 <printint.constprop.0+0x126>
        buf[i--] = digits[x % base];
    112c:	02e6f5bb          	remuw	a1,a3,a4
    } while ((x /= base) != 0);
    1130:	02e6d7bb          	divuw	a5,a3,a4
        buf[i--] = digits[x % base];
    1134:	1582                	slli	a1,a1,0x20
    1136:	9181                	srli	a1,a1,0x20
    1138:	95b2                	add	a1,a1,a2
    113a:	0005c583          	lbu	a1,0(a1)
    113e:	00b10aa3          	sb	a1,21(sp)
    } while ((x /= base) != 0);
    1142:	0007859b          	sext.w	a1,a5
    1146:	12e6ec63          	bltu	a3,a4,127e <printint.constprop.0+0x1a0>
        buf[i--] = digits[x % base];
    114a:	02e7f6bb          	remuw	a3,a5,a4
    114e:	1682                	slli	a3,a3,0x20
    1150:	9281                	srli	a3,a3,0x20
    1152:	96b2                	add	a3,a3,a2
    1154:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    1158:	02e7d83b          	divuw	a6,a5,a4
        buf[i--] = digits[x % base];
    115c:	00d10a23          	sb	a3,20(sp)
    } while ((x /= base) != 0);
    1160:	12e5e863          	bltu	a1,a4,1290 <printint.constprop.0+0x1b2>
        buf[i--] = digits[x % base];
    1164:	02e876bb          	remuw	a3,a6,a4
    1168:	1682                	slli	a3,a3,0x20
    116a:	9281                	srli	a3,a3,0x20
    116c:	96b2                	add	a3,a3,a2
    116e:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    1172:	02e855bb          	divuw	a1,a6,a4
        buf[i--] = digits[x % base];
    1176:	00d109a3          	sb	a3,19(sp)
    } while ((x /= base) != 0);
    117a:	12e86463          	bltu	a6,a4,12a2 <printint.constprop.0+0x1c4>
        buf[i--] = digits[x % base];
    117e:	02e5f6bb          	remuw	a3,a1,a4
    1182:	1682                	slli	a3,a3,0x20
    1184:	9281                	srli	a3,a3,0x20
    1186:	96b2                	add	a3,a3,a2
    1188:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    118c:	02e5d83b          	divuw	a6,a1,a4
        buf[i--] = digits[x % base];
    1190:	00d10923          	sb	a3,18(sp)
    } while ((x /= base) != 0);
    1194:	0ce5ec63          	bltu	a1,a4,126c <printint.constprop.0+0x18e>
        buf[i--] = digits[x % base];
    1198:	02e876bb          	remuw	a3,a6,a4
    119c:	1682                	slli	a3,a3,0x20
    119e:	9281                	srli	a3,a3,0x20
    11a0:	96b2                	add	a3,a3,a2
    11a2:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    11a6:	02e855bb          	divuw	a1,a6,a4
        buf[i--] = digits[x % base];
    11aa:	00d108a3          	sb	a3,17(sp)
    } while ((x /= base) != 0);
    11ae:	10e86963          	bltu	a6,a4,12c0 <printint.constprop.0+0x1e2>
        buf[i--] = digits[x % base];
    11b2:	02e5f6bb          	remuw	a3,a1,a4
    11b6:	1682                	slli	a3,a3,0x20
    11b8:	9281                	srli	a3,a3,0x20
    11ba:	96b2                	add	a3,a3,a2
    11bc:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    11c0:	02e5d83b          	divuw	a6,a1,a4
        buf[i--] = digits[x % base];
    11c4:	00d10823          	sb	a3,16(sp)
    } while ((x /= base) != 0);
    11c8:	10e5e763          	bltu	a1,a4,12d6 <printint.constprop.0+0x1f8>
        buf[i--] = digits[x % base];
    11cc:	02e876bb          	remuw	a3,a6,a4
    11d0:	1682                	slli	a3,a3,0x20
    11d2:	9281                	srli	a3,a3,0x20
    11d4:	96b2                	add	a3,a3,a2
    11d6:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    11da:	02e857bb          	divuw	a5,a6,a4
        buf[i--] = digits[x % base];
    11de:	00d107a3          	sb	a3,15(sp)
    } while ((x /= base) != 0);
    11e2:	10e86363          	bltu	a6,a4,12e8 <printint.constprop.0+0x20a>
        buf[i--] = digits[x % base];
    11e6:	1782                	slli	a5,a5,0x20
    11e8:	9381                	srli	a5,a5,0x20
    11ea:	97b2                	add	a5,a5,a2
    11ec:	0007c783          	lbu	a5,0(a5)
    11f0:	4599                	li	a1,6
    11f2:	00f10723          	sb	a5,14(sp)

    if (sign)
    11f6:	00055763          	bgez	a0,1204 <printint.constprop.0+0x126>
        buf[i--] = '-';
    11fa:	02d00793          	li	a5,45
    11fe:	00f106a3          	sb	a5,13(sp)
        buf[i--] = digits[x % base];
    1202:	4595                	li	a1,5
    write(f, s, l);
    1204:	003c                	addi	a5,sp,8
    1206:	4641                	li	a2,16
    1208:	9e0d                	subw	a2,a2,a1
    120a:	4505                	li	a0,1
    120c:	95be                	add	a1,a1,a5
    120e:	291000ef          	jal	ra,1c9e <write>
    i++;
    if (i < 0)
        puts("printint error");
    out(stdout, buf + i, 16 - i);
}
    1212:	70a2                	ld	ra,40(sp)
    1214:	6145                	addi	sp,sp,48
    1216:	8082                	ret
        x = -xx;
    1218:	40a0083b          	negw	a6,a0
        buf[i--] = digits[x % base];
    121c:	02b877bb          	remuw	a5,a6,a1
    1220:	00001617          	auipc	a2,0x1
    1224:	dc860613          	addi	a2,a2,-568 # 1fe8 <digits>
    buf[16] = 0;
    1228:	00010c23          	sb	zero,24(sp)
        buf[i--] = digits[x % base];
    122c:	0005871b          	sext.w	a4,a1
    1230:	1782                	slli	a5,a5,0x20
    1232:	9381                	srli	a5,a5,0x20
    1234:	97b2                	add	a5,a5,a2
    1236:	0007c783          	lbu	a5,0(a5)
    } while ((x /= base) != 0);
    123a:	02b858bb          	divuw	a7,a6,a1
        buf[i--] = digits[x % base];
    123e:	00f10ba3          	sb	a5,23(sp)
    } while ((x /= base) != 0);
    1242:	06b86963          	bltu	a6,a1,12b4 <printint.constprop.0+0x1d6>
        buf[i--] = digits[x % base];
    1246:	02e8f7bb          	remuw	a5,a7,a4
    124a:	1782                	slli	a5,a5,0x20
    124c:	9381                	srli	a5,a5,0x20
    124e:	97b2                	add	a5,a5,a2
    1250:	0007c783          	lbu	a5,0(a5)
    } while ((x /= base) != 0);
    1254:	02e8d6bb          	divuw	a3,a7,a4
        buf[i--] = digits[x % base];
    1258:	00f10b23          	sb	a5,22(sp)
    } while ((x /= base) != 0);
    125c:	ece8f8e3          	bgeu	a7,a4,112c <printint.constprop.0+0x4e>
        buf[i--] = '-';
    1260:	02d00793          	li	a5,45
    1264:	00f10aa3          	sb	a5,21(sp)
        buf[i--] = digits[x % base];
    1268:	45b5                	li	a1,13
    126a:	bf69                	j	1204 <printint.constprop.0+0x126>
    126c:	45a9                	li	a1,10
    if (sign)
    126e:	f8055be3          	bgez	a0,1204 <printint.constprop.0+0x126>
        buf[i--] = '-';
    1272:	02d00793          	li	a5,45
    1276:	00f108a3          	sb	a5,17(sp)
        buf[i--] = digits[x % base];
    127a:	45a5                	li	a1,9
    127c:	b761                	j	1204 <printint.constprop.0+0x126>
    127e:	45b5                	li	a1,13
    if (sign)
    1280:	f80552e3          	bgez	a0,1204 <printint.constprop.0+0x126>
        buf[i--] = '-';
    1284:	02d00793          	li	a5,45
    1288:	00f10a23          	sb	a5,20(sp)
        buf[i--] = digits[x % base];
    128c:	45b1                	li	a1,12
    128e:	bf9d                	j	1204 <printint.constprop.0+0x126>
    1290:	45b1                	li	a1,12
    if (sign)
    1292:	f60559e3          	bgez	a0,1204 <printint.constprop.0+0x126>
        buf[i--] = '-';
    1296:	02d00793          	li	a5,45
    129a:	00f109a3          	sb	a5,19(sp)
        buf[i--] = digits[x % base];
    129e:	45ad                	li	a1,11
    12a0:	b795                	j	1204 <printint.constprop.0+0x126>
    12a2:	45ad                	li	a1,11
    if (sign)
    12a4:	f60550e3          	bgez	a0,1204 <printint.constprop.0+0x126>
        buf[i--] = '-';
    12a8:	02d00793          	li	a5,45
    12ac:	00f10923          	sb	a5,18(sp)
        buf[i--] = digits[x % base];
    12b0:	45a9                	li	a1,10
    12b2:	bf89                	j	1204 <printint.constprop.0+0x126>
        buf[i--] = '-';
    12b4:	02d00793          	li	a5,45
    12b8:	00f10b23          	sb	a5,22(sp)
        buf[i--] = digits[x % base];
    12bc:	45b9                	li	a1,14
    12be:	b799                	j	1204 <printint.constprop.0+0x126>
    12c0:	45a5                	li	a1,9
    if (sign)
    12c2:	f40551e3          	bgez	a0,1204 <printint.constprop.0+0x126>
        buf[i--] = '-';
    12c6:	02d00793          	li	a5,45
    12ca:	00f10823          	sb	a5,16(sp)
        buf[i--] = digits[x % base];
    12ce:	45a1                	li	a1,8
    12d0:	bf15                	j	1204 <printint.constprop.0+0x126>
    i = 15;
    12d2:	45bd                	li	a1,15
    12d4:	bf05                	j	1204 <printint.constprop.0+0x126>
        buf[i--] = digits[x % base];
    12d6:	45a1                	li	a1,8
    if (sign)
    12d8:	f20556e3          	bgez	a0,1204 <printint.constprop.0+0x126>
        buf[i--] = '-';
    12dc:	02d00793          	li	a5,45
    12e0:	00f107a3          	sb	a5,15(sp)
        buf[i--] = digits[x % base];
    12e4:	459d                	li	a1,7
    12e6:	bf39                	j	1204 <printint.constprop.0+0x126>
    12e8:	459d                	li	a1,7
    if (sign)
    12ea:	f0055de3          	bgez	a0,1204 <printint.constprop.0+0x126>
        buf[i--] = '-';
    12ee:	02d00793          	li	a5,45
    12f2:	00f10723          	sb	a5,14(sp)
        buf[i--] = digits[x % base];
    12f6:	4599                	li	a1,6
    12f8:	b731                	j	1204 <printint.constprop.0+0x126>

00000000000012fa <getchar>:
{
    12fa:	1101                	addi	sp,sp,-32
    read(stdin, &byte, 1);
    12fc:	00f10593          	addi	a1,sp,15
    1300:	4605                	li	a2,1
    1302:	4501                	li	a0,0
{
    1304:	ec06                	sd	ra,24(sp)
    char byte = 0;
    1306:	000107a3          	sb	zero,15(sp)
    read(stdin, &byte, 1);
    130a:	18b000ef          	jal	ra,1c94 <read>
}
    130e:	60e2                	ld	ra,24(sp)
    1310:	00f14503          	lbu	a0,15(sp)
    1314:	6105                	addi	sp,sp,32
    1316:	8082                	ret

0000000000001318 <putchar>:
{
    1318:	1101                	addi	sp,sp,-32
    131a:	87aa                	mv	a5,a0
    return write(stdout, &byte, 1);
    131c:	00f10593          	addi	a1,sp,15
    1320:	4605                	li	a2,1
    1322:	4505                	li	a0,1
{
    1324:	ec06                	sd	ra,24(sp)
    char byte = c;
    1326:	00f107a3          	sb	a5,15(sp)
    return write(stdout, &byte, 1);
    132a:	175000ef          	jal	ra,1c9e <write>
}
    132e:	60e2                	ld	ra,24(sp)
    1330:	2501                	sext.w	a0,a0
    1332:	6105                	addi	sp,sp,32
    1334:	8082                	ret

0000000000001336 <puts>:
{
    1336:	1141                	addi	sp,sp,-16
    1338:	e406                	sd	ra,8(sp)
    133a:	e022                	sd	s0,0(sp)
    133c:	842a                	mv	s0,a0
    r = -(write(stdout, s, strlen(s)) < 0);
    133e:	57c000ef          	jal	ra,18ba <strlen>
    1342:	862a                	mv	a2,a0
    1344:	85a2                	mv	a1,s0
    1346:	4505                	li	a0,1
    1348:	157000ef          	jal	ra,1c9e <write>
}
    134c:	60a2                	ld	ra,8(sp)
    134e:	6402                	ld	s0,0(sp)
    r = -(write(stdout, s, strlen(s)) < 0);
    1350:	957d                	srai	a0,a0,0x3f
    return r;
    1352:	2501                	sext.w	a0,a0
}
    1354:	0141                	addi	sp,sp,16
    1356:	8082                	ret

0000000000001358 <printf>:
    out(stdout, buf, i);
}

// Print to the console. only understands %d, %x, %p, %s.
void printf(const char *fmt, ...)
{
    1358:	7171                	addi	sp,sp,-176
    135a:	fc56                	sd	s5,56(sp)
    135c:	ed3e                	sd	a5,152(sp)
    buf[i++] = '0';
    135e:	7ae1                	lui	s5,0xffff8
    va_list ap;
    int cnt = 0, l = 0;
    char *a, *z, *s = (char *)fmt, str;
    int f = stdout;

    va_start(ap, fmt);
    1360:	18bc                	addi	a5,sp,120
{
    1362:	e8ca                	sd	s2,80(sp)
    1364:	e4ce                	sd	s3,72(sp)
    1366:	e0d2                	sd	s4,64(sp)
    1368:	f85a                	sd	s6,48(sp)
    136a:	f486                	sd	ra,104(sp)
    136c:	f0a2                	sd	s0,96(sp)
    136e:	eca6                	sd	s1,88(sp)
    1370:	fcae                	sd	a1,120(sp)
    1372:	e132                	sd	a2,128(sp)
    1374:	e536                	sd	a3,136(sp)
    1376:	e93a                	sd	a4,144(sp)
    1378:	f142                	sd	a6,160(sp)
    137a:	f546                	sd	a7,168(sp)
    va_start(ap, fmt);
    137c:	e03e                	sd	a5,0(sp)
    for (;;)
    {
        if (!*s)
            break;
        for (a = s; *s && *s != '%'; s++)
    137e:	02500913          	li	s2,37
        out(f, a, l);
        if (l)
            continue;
        if (s[1] == 0)
            break;
        switch (s[1])
    1382:	07300a13          	li	s4,115
        case 'p':
            printptr(va_arg(ap, uint64));
            break;
        case 's':
            if ((a = va_arg(ap, char *)) == 0)
                a = "(null)";
    1386:	00001b17          	auipc	s6,0x1
    138a:	c12b0b13          	addi	s6,s6,-1006 # 1f98 <__clone+0xc6>
    buf[i++] = '0';
    138e:	830aca93          	xori	s5,s5,-2000
        buf[i++] = digits[x >> (sizeof(uint64) * 8 - 4)];
    1392:	00001997          	auipc	s3,0x1
    1396:	c5698993          	addi	s3,s3,-938 # 1fe8 <digits>
        if (!*s)
    139a:	00054783          	lbu	a5,0(a0)
    139e:	16078a63          	beqz	a5,1512 <printf+0x1ba>
    13a2:	862a                	mv	a2,a0
        for (a = s; *s && *s != '%'; s++)
    13a4:	19278163          	beq	a5,s2,1526 <printf+0x1ce>
    13a8:	00164783          	lbu	a5,1(a2)
    13ac:	0605                	addi	a2,a2,1
    13ae:	fbfd                	bnez	a5,13a4 <printf+0x4c>
    13b0:	84b2                	mv	s1,a2
        l = z - a;
    13b2:	40a6043b          	subw	s0,a2,a0
    write(f, s, l);
    13b6:	85aa                	mv	a1,a0
    13b8:	8622                	mv	a2,s0
    13ba:	4505                	li	a0,1
    13bc:	0e3000ef          	jal	ra,1c9e <write>
        if (l)
    13c0:	18041c63          	bnez	s0,1558 <printf+0x200>
        if (s[1] == 0)
    13c4:	0014c783          	lbu	a5,1(s1)
    13c8:	14078563          	beqz	a5,1512 <printf+0x1ba>
        switch (s[1])
    13cc:	1d478063          	beq	a5,s4,158c <printf+0x234>
    13d0:	18fa6663          	bltu	s4,a5,155c <printf+0x204>
    13d4:	06400713          	li	a4,100
    13d8:	1ae78063          	beq	a5,a4,1578 <printf+0x220>
    13dc:	07000713          	li	a4,112
    13e0:	1ce79963          	bne	a5,a4,15b2 <printf+0x25a>
            printptr(va_arg(ap, uint64));
    13e4:	6702                	ld	a4,0(sp)
    buf[i++] = '0';
    13e6:	01511423          	sh	s5,8(sp)
    write(f, s, l);
    13ea:	4649                	li	a2,18
            printptr(va_arg(ap, uint64));
    13ec:	631c                	ld	a5,0(a4)
    13ee:	0721                	addi	a4,a4,8
    13f0:	e03a                	sd	a4,0(sp)
    for (j = 0; j < (sizeof(uint64) * 2); j++, x <<= 4)
    13f2:	00479293          	slli	t0,a5,0x4
    13f6:	00879f93          	slli	t6,a5,0x8
    13fa:	00c79f13          	slli	t5,a5,0xc
    13fe:	01079e93          	slli	t4,a5,0x10
    1402:	01479e13          	slli	t3,a5,0x14
    1406:	01879313          	slli	t1,a5,0x18
    140a:	01c79893          	slli	a7,a5,0x1c
    140e:	02479813          	slli	a6,a5,0x24
    1412:	02879513          	slli	a0,a5,0x28
    1416:	02c79593          	slli	a1,a5,0x2c
    141a:	03079693          	slli	a3,a5,0x30
    141e:	03479713          	slli	a4,a5,0x34
        buf[i++] = digits[x >> (sizeof(uint64) * 8 - 4)];
    1422:	03c7d413          	srli	s0,a5,0x3c
    1426:	01c7d39b          	srliw	t2,a5,0x1c
    142a:	03c2d293          	srli	t0,t0,0x3c
    142e:	03cfdf93          	srli	t6,t6,0x3c
    1432:	03cf5f13          	srli	t5,t5,0x3c
    1436:	03cede93          	srli	t4,t4,0x3c
    143a:	03ce5e13          	srli	t3,t3,0x3c
    143e:	03c35313          	srli	t1,t1,0x3c
    1442:	03c8d893          	srli	a7,a7,0x3c
    1446:	03c85813          	srli	a6,a6,0x3c
    144a:	9171                	srli	a0,a0,0x3c
    144c:	91f1                	srli	a1,a1,0x3c
    144e:	92f1                	srli	a3,a3,0x3c
    1450:	9371                	srli	a4,a4,0x3c
    1452:	96ce                	add	a3,a3,s3
    1454:	974e                	add	a4,a4,s3
    1456:	944e                	add	s0,s0,s3
    1458:	92ce                	add	t0,t0,s3
    145a:	9fce                	add	t6,t6,s3
    145c:	9f4e                	add	t5,t5,s3
    145e:	9ece                	add	t4,t4,s3
    1460:	9e4e                	add	t3,t3,s3
    1462:	934e                	add	t1,t1,s3
    1464:	98ce                	add	a7,a7,s3
    1466:	93ce                	add	t2,t2,s3
    1468:	984e                	add	a6,a6,s3
    146a:	954e                	add	a0,a0,s3
    146c:	95ce                	add	a1,a1,s3
    146e:	0006c083          	lbu	ra,0(a3)
    1472:	0002c283          	lbu	t0,0(t0)
    1476:	00074683          	lbu	a3,0(a4)
    147a:	000fcf83          	lbu	t6,0(t6)
    147e:	000f4f03          	lbu	t5,0(t5)
    1482:	000ece83          	lbu	t4,0(t4)
    1486:	000e4e03          	lbu	t3,0(t3)
    148a:	00034303          	lbu	t1,0(t1)
    148e:	0008c883          	lbu	a7,0(a7)
    1492:	0003c383          	lbu	t2,0(t2)
    1496:	00084803          	lbu	a6,0(a6)
    149a:	00054503          	lbu	a0,0(a0)
    149e:	0005c583          	lbu	a1,0(a1)
    14a2:	00044403          	lbu	s0,0(s0)
    for (j = 0; j < (sizeof(uint64) * 2); j++, x <<= 4)
    14a6:	03879713          	slli	a4,a5,0x38
        buf[i++] = digits[x >> (sizeof(uint64) * 8 - 4)];
    14aa:	9371                	srli	a4,a4,0x3c
    14ac:	8bbd                	andi	a5,a5,15
    14ae:	974e                	add	a4,a4,s3
    14b0:	97ce                	add	a5,a5,s3
    14b2:	005105a3          	sb	t0,11(sp)
    14b6:	01f10623          	sb	t6,12(sp)
    14ba:	01e106a3          	sb	t5,13(sp)
    14be:	01d10723          	sb	t4,14(sp)
    14c2:	01c107a3          	sb	t3,15(sp)
    14c6:	00610823          	sb	t1,16(sp)
    14ca:	011108a3          	sb	a7,17(sp)
    14ce:	00710923          	sb	t2,18(sp)
    14d2:	010109a3          	sb	a6,19(sp)
    14d6:	00a10a23          	sb	a0,20(sp)
    14da:	00b10aa3          	sb	a1,21(sp)
    14de:	00110b23          	sb	ra,22(sp)
    14e2:	00d10ba3          	sb	a3,23(sp)
    14e6:	00810523          	sb	s0,10(sp)
    14ea:	00074703          	lbu	a4,0(a4)
    14ee:	0007c783          	lbu	a5,0(a5)
    write(f, s, l);
    14f2:	002c                	addi	a1,sp,8
    14f4:	4505                	li	a0,1
        buf[i++] = digits[x >> (sizeof(uint64) * 8 - 4)];
    14f6:	00e10c23          	sb	a4,24(sp)
    14fa:	00f10ca3          	sb	a5,25(sp)
    buf[i] = 0;
    14fe:	00010d23          	sb	zero,26(sp)
    write(f, s, l);
    1502:	79c000ef          	jal	ra,1c9e <write>
            // Print unknown % sequence to draw attention.
            putchar('%');
            putchar(s[1]);
            break;
        }
        s += 2;
    1506:	00248513          	addi	a0,s1,2
        if (!*s)
    150a:	00054783          	lbu	a5,0(a0)
    150e:	e8079ae3          	bnez	a5,13a2 <printf+0x4a>
    }
    va_end(ap);
}
    1512:	70a6                	ld	ra,104(sp)
    1514:	7406                	ld	s0,96(sp)
    1516:	64e6                	ld	s1,88(sp)
    1518:	6946                	ld	s2,80(sp)
    151a:	69a6                	ld	s3,72(sp)
    151c:	6a06                	ld	s4,64(sp)
    151e:	7ae2                	ld	s5,56(sp)
    1520:	7b42                	ld	s6,48(sp)
    1522:	614d                	addi	sp,sp,176
    1524:	8082                	ret
        for (z = s; s[0] == '%' && s[1] == '%'; z++, s += 2)
    1526:	00064783          	lbu	a5,0(a2)
    152a:	84b2                	mv	s1,a2
    152c:	01278963          	beq	a5,s2,153e <printf+0x1e6>
    1530:	b549                	j	13b2 <printf+0x5a>
    1532:	0024c783          	lbu	a5,2(s1)
    1536:	0605                	addi	a2,a2,1
    1538:	0489                	addi	s1,s1,2
    153a:	e7279ce3          	bne	a5,s2,13b2 <printf+0x5a>
    153e:	0014c783          	lbu	a5,1(s1)
    1542:	ff2788e3          	beq	a5,s2,1532 <printf+0x1da>
        l = z - a;
    1546:	40a6043b          	subw	s0,a2,a0
    write(f, s, l);
    154a:	85aa                	mv	a1,a0
    154c:	8622                	mv	a2,s0
    154e:	4505                	li	a0,1
    1550:	74e000ef          	jal	ra,1c9e <write>
        if (l)
    1554:	e60408e3          	beqz	s0,13c4 <printf+0x6c>
    1558:	8526                	mv	a0,s1
    155a:	b581                	j	139a <printf+0x42>
        switch (s[1])
    155c:	07800713          	li	a4,120
    1560:	04e79963          	bne	a5,a4,15b2 <printf+0x25a>
            printint(va_arg(ap, int), 16, 1);
    1564:	6782                	ld	a5,0(sp)
    1566:	45c1                	li	a1,16
    1568:	4388                	lw	a0,0(a5)
    156a:	07a1                	addi	a5,a5,8
    156c:	e03e                	sd	a5,0(sp)
    156e:	b71ff0ef          	jal	ra,10de <printint.constprop.0>
        s += 2;
    1572:	00248513          	addi	a0,s1,2
    1576:	bf51                	j	150a <printf+0x1b2>
            printint(va_arg(ap, int), 10, 1);
    1578:	6782                	ld	a5,0(sp)
    157a:	45a9                	li	a1,10
    157c:	4388                	lw	a0,0(a5)
    157e:	07a1                	addi	a5,a5,8
    1580:	e03e                	sd	a5,0(sp)
    1582:	b5dff0ef          	jal	ra,10de <printint.constprop.0>
        s += 2;
    1586:	00248513          	addi	a0,s1,2
    158a:	b741                	j	150a <printf+0x1b2>
            if ((a = va_arg(ap, char *)) == 0)
    158c:	6782                	ld	a5,0(sp)
    158e:	6380                	ld	s0,0(a5)
    1590:	07a1                	addi	a5,a5,8
    1592:	e03e                	sd	a5,0(sp)
    1594:	c031                	beqz	s0,15d8 <printf+0x280>
            l = strnlen(a, 200);
    1596:	0c800593          	li	a1,200
    159a:	8522                	mv	a0,s0
    159c:	40a000ef          	jal	ra,19a6 <strnlen>
    write(f, s, l);
    15a0:	0005061b          	sext.w	a2,a0
    15a4:	85a2                	mv	a1,s0
    15a6:	4505                	li	a0,1
    15a8:	6f6000ef          	jal	ra,1c9e <write>
        s += 2;
    15ac:	00248513          	addi	a0,s1,2
    15b0:	bfa9                	j	150a <printf+0x1b2>
    return write(stdout, &byte, 1);
    15b2:	4605                	li	a2,1
    15b4:	002c                	addi	a1,sp,8
    15b6:	4505                	li	a0,1
    char byte = c;
    15b8:	01210423          	sb	s2,8(sp)
    return write(stdout, &byte, 1);
    15bc:	6e2000ef          	jal	ra,1c9e <write>
    char byte = c;
    15c0:	0014c783          	lbu	a5,1(s1)
    return write(stdout, &byte, 1);
    15c4:	4605                	li	a2,1
    15c6:	002c                	addi	a1,sp,8
    15c8:	4505                	li	a0,1
    char byte = c;
    15ca:	00f10423          	sb	a5,8(sp)
    return write(stdout, &byte, 1);
    15ce:	6d0000ef          	jal	ra,1c9e <write>
        s += 2;
    15d2:	00248513          	addi	a0,s1,2
    15d6:	bf15                	j	150a <printf+0x1b2>
                a = "(null)";
    15d8:	845a                	mv	s0,s6
    15da:	bf75                	j	1596 <printf+0x23e>

00000000000015dc <panic>:
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

void panic(char *m)
{
    15dc:	1141                	addi	sp,sp,-16
    15de:	e406                	sd	ra,8(sp)
    puts(m);
    15e0:	d57ff0ef          	jal	ra,1336 <puts>
    exit(-100);
}
    15e4:	60a2                	ld	ra,8(sp)
    exit(-100);
    15e6:	f9c00513          	li	a0,-100
}
    15ea:	0141                	addi	sp,sp,16
    exit(-100);
    15ec:	a709                	j	1cee <exit>

00000000000015ee <isspace>:
#define HIGHS (ONES * (UCHAR_MAX / 2 + 1))
#define HASZERO(x) (((x)-ONES) & ~(x)&HIGHS)

int isspace(int c)
{
    return c == ' ' || (unsigned)c - '\t' < 5;
    15ee:	02000793          	li	a5,32
    15f2:	00f50663          	beq	a0,a5,15fe <isspace+0x10>
    15f6:	355d                	addiw	a0,a0,-9
    15f8:	00553513          	sltiu	a0,a0,5
    15fc:	8082                	ret
    15fe:	4505                	li	a0,1
}
    1600:	8082                	ret

0000000000001602 <isdigit>:

int isdigit(int c)
{
    return (unsigned)c - '0' < 10;
    1602:	fd05051b          	addiw	a0,a0,-48
}
    1606:	00a53513          	sltiu	a0,a0,10
    160a:	8082                	ret

000000000000160c <atoi>:
    return c == ' ' || (unsigned)c - '\t' < 5;
    160c:	02000613          	li	a2,32
    1610:	4591                	li	a1,4

int atoi(const char *s)
{
    int n = 0, neg = 0;
    while (isspace(*s))
    1612:	00054703          	lbu	a4,0(a0)
    return c == ' ' || (unsigned)c - '\t' < 5;
    1616:	ff77069b          	addiw	a3,a4,-9
    161a:	04c70d63          	beq	a4,a2,1674 <atoi+0x68>
    161e:	0007079b          	sext.w	a5,a4
    1622:	04d5f963          	bgeu	a1,a3,1674 <atoi+0x68>
        s++;
    switch (*s)
    1626:	02b00693          	li	a3,43
    162a:	04d70a63          	beq	a4,a3,167e <atoi+0x72>
    162e:	02d00693          	li	a3,45
    1632:	06d70463          	beq	a4,a3,169a <atoi+0x8e>
        neg = 1;
    case '+':
        s++;
    }
    /* Compute n as a negative number to avoid overflow on INT_MIN */
    while (isdigit(*s))
    1636:	fd07859b          	addiw	a1,a5,-48
    163a:	4625                	li	a2,9
    163c:	873e                	mv	a4,a5
    163e:	86aa                	mv	a3,a0
    int n = 0, neg = 0;
    1640:	4e01                	li	t3,0
    while (isdigit(*s))
    1642:	04b66a63          	bltu	a2,a1,1696 <atoi+0x8a>
    int n = 0, neg = 0;
    1646:	4501                	li	a0,0
    while (isdigit(*s))
    1648:	4825                	li	a6,9
    164a:	0016c603          	lbu	a2,1(a3)
        n = 10 * n - (*s++ - '0');
    164e:	0025179b          	slliw	a5,a0,0x2
    1652:	9d3d                	addw	a0,a0,a5
    1654:	fd07031b          	addiw	t1,a4,-48
    1658:	0015189b          	slliw	a7,a0,0x1
    while (isdigit(*s))
    165c:	fd06059b          	addiw	a1,a2,-48
        n = 10 * n - (*s++ - '0');
    1660:	0685                	addi	a3,a3,1
    1662:	4068853b          	subw	a0,a7,t1
    while (isdigit(*s))
    1666:	0006071b          	sext.w	a4,a2
    166a:	feb870e3          	bgeu	a6,a1,164a <atoi+0x3e>
    return neg ? n : -n;
    166e:	000e0563          	beqz	t3,1678 <atoi+0x6c>
}
    1672:	8082                	ret
        s++;
    1674:	0505                	addi	a0,a0,1
    1676:	bf71                	j	1612 <atoi+0x6>
    return neg ? n : -n;
    1678:	4113053b          	subw	a0,t1,a7
    167c:	8082                	ret
    while (isdigit(*s))
    167e:	00154783          	lbu	a5,1(a0)
    1682:	4625                	li	a2,9
        s++;
    1684:	00150693          	addi	a3,a0,1
    while (isdigit(*s))
    1688:	fd07859b          	addiw	a1,a5,-48
    168c:	0007871b          	sext.w	a4,a5
    int n = 0, neg = 0;
    1690:	4e01                	li	t3,0
    while (isdigit(*s))
    1692:	fab67ae3          	bgeu	a2,a1,1646 <atoi+0x3a>
    1696:	4501                	li	a0,0
}
    1698:	8082                	ret
    while (isdigit(*s))
    169a:	00154783          	lbu	a5,1(a0)
    169e:	4625                	li	a2,9
        s++;
    16a0:	00150693          	addi	a3,a0,1
    while (isdigit(*s))
    16a4:	fd07859b          	addiw	a1,a5,-48
    16a8:	0007871b          	sext.w	a4,a5
    16ac:	feb665e3          	bltu	a2,a1,1696 <atoi+0x8a>
        neg = 1;
    16b0:	4e05                	li	t3,1
    16b2:	bf51                	j	1646 <atoi+0x3a>

00000000000016b4 <memset>:

void *memset(void *dest, int c, size_t n)
{
    char *p = dest;
    for (int i = 0; i < n; ++i, *(p++) = c)
    16b4:	16060d63          	beqz	a2,182e <memset+0x17a>
    16b8:	40a007b3          	neg	a5,a0
    16bc:	8b9d                	andi	a5,a5,7
    16be:	00778713          	addi	a4,a5,7
    16c2:	482d                	li	a6,11
    16c4:	0ff5f593          	andi	a1,a1,255
    16c8:	fff60693          	addi	a3,a2,-1
    16cc:	17076263          	bltu	a4,a6,1830 <memset+0x17c>
    16d0:	16e6ea63          	bltu	a3,a4,1844 <memset+0x190>
    16d4:	16078563          	beqz	a5,183e <memset+0x18a>
    16d8:	00b50023          	sb	a1,0(a0)
    16dc:	4705                	li	a4,1
    16de:	00150e93          	addi	t4,a0,1
    16e2:	14e78c63          	beq	a5,a4,183a <memset+0x186>
    16e6:	00b500a3          	sb	a1,1(a0)
    16ea:	4709                	li	a4,2
    16ec:	00250e93          	addi	t4,a0,2
    16f0:	14e78d63          	beq	a5,a4,184a <memset+0x196>
    16f4:	00b50123          	sb	a1,2(a0)
    16f8:	470d                	li	a4,3
    16fa:	00350e93          	addi	t4,a0,3
    16fe:	12e78b63          	beq	a5,a4,1834 <memset+0x180>
    1702:	00b501a3          	sb	a1,3(a0)
    1706:	4711                	li	a4,4
    1708:	00450e93          	addi	t4,a0,4
    170c:	14e78163          	beq	a5,a4,184e <memset+0x19a>
    1710:	00b50223          	sb	a1,4(a0)
    1714:	4715                	li	a4,5
    1716:	00550e93          	addi	t4,a0,5
    171a:	12e78c63          	beq	a5,a4,1852 <memset+0x19e>
    171e:	00b502a3          	sb	a1,5(a0)
    1722:	471d                	li	a4,7
    1724:	00650e93          	addi	t4,a0,6
    1728:	12e79763          	bne	a5,a4,1856 <memset+0x1a2>
    172c:	00750e93          	addi	t4,a0,7
    1730:	00b50323          	sb	a1,6(a0)
    1734:	4f1d                	li	t5,7
    1736:	00859713          	slli	a4,a1,0x8
    173a:	8f4d                	or	a4,a4,a1
    173c:	01059e13          	slli	t3,a1,0x10
    1740:	01c76e33          	or	t3,a4,t3
    1744:	01859313          	slli	t1,a1,0x18
    1748:	006e6333          	or	t1,t3,t1
    174c:	02059893          	slli	a7,a1,0x20
    1750:	011368b3          	or	a7,t1,a7
    1754:	02859813          	slli	a6,a1,0x28
    1758:	40f60333          	sub	t1,a2,a5
    175c:	0108e833          	or	a6,a7,a6
    1760:	03059693          	slli	a3,a1,0x30
    1764:	00d866b3          	or	a3,a6,a3
    1768:	03859713          	slli	a4,a1,0x38
    176c:	97aa                	add	a5,a5,a0
    176e:	ff837813          	andi	a6,t1,-8
    1772:	8f55                	or	a4,a4,a3
    1774:	00f806b3          	add	a3,a6,a5
    1778:	e398                	sd	a4,0(a5)
    177a:	07a1                	addi	a5,a5,8
    177c:	fed79ee3          	bne	a5,a3,1778 <memset+0xc4>
    1780:	ff837693          	andi	a3,t1,-8
    1784:	00de87b3          	add	a5,t4,a3
    1788:	01e6873b          	addw	a4,a3,t5
    178c:	0ad30663          	beq	t1,a3,1838 <memset+0x184>
    1790:	00b78023          	sb	a1,0(a5)
    1794:	0017069b          	addiw	a3,a4,1
    1798:	08c6fb63          	bgeu	a3,a2,182e <memset+0x17a>
    179c:	00b780a3          	sb	a1,1(a5)
    17a0:	0027069b          	addiw	a3,a4,2
    17a4:	08c6f563          	bgeu	a3,a2,182e <memset+0x17a>
    17a8:	00b78123          	sb	a1,2(a5)
    17ac:	0037069b          	addiw	a3,a4,3
    17b0:	06c6ff63          	bgeu	a3,a2,182e <memset+0x17a>
    17b4:	00b781a3          	sb	a1,3(a5)
    17b8:	0047069b          	addiw	a3,a4,4
    17bc:	06c6f963          	bgeu	a3,a2,182e <memset+0x17a>
    17c0:	00b78223          	sb	a1,4(a5)
    17c4:	0057069b          	addiw	a3,a4,5
    17c8:	06c6f363          	bgeu	a3,a2,182e <memset+0x17a>
    17cc:	00b782a3          	sb	a1,5(a5)
    17d0:	0067069b          	addiw	a3,a4,6
    17d4:	04c6fd63          	bgeu	a3,a2,182e <memset+0x17a>
    17d8:	00b78323          	sb	a1,6(a5)
    17dc:	0077069b          	addiw	a3,a4,7
    17e0:	04c6f763          	bgeu	a3,a2,182e <memset+0x17a>
    17e4:	00b783a3          	sb	a1,7(a5)
    17e8:	0087069b          	addiw	a3,a4,8
    17ec:	04c6f163          	bgeu	a3,a2,182e <memset+0x17a>
    17f0:	00b78423          	sb	a1,8(a5)
    17f4:	0097069b          	addiw	a3,a4,9
    17f8:	02c6fb63          	bgeu	a3,a2,182e <memset+0x17a>
    17fc:	00b784a3          	sb	a1,9(a5)
    1800:	00a7069b          	addiw	a3,a4,10
    1804:	02c6f563          	bgeu	a3,a2,182e <memset+0x17a>
    1808:	00b78523          	sb	a1,10(a5)
    180c:	00b7069b          	addiw	a3,a4,11
    1810:	00c6ff63          	bgeu	a3,a2,182e <memset+0x17a>
    1814:	00b785a3          	sb	a1,11(a5)
    1818:	00c7069b          	addiw	a3,a4,12
    181c:	00c6f963          	bgeu	a3,a2,182e <memset+0x17a>
    1820:	00b78623          	sb	a1,12(a5)
    1824:	2735                	addiw	a4,a4,13
    1826:	00c77463          	bgeu	a4,a2,182e <memset+0x17a>
    182a:	00b786a3          	sb	a1,13(a5)
        ;
    return dest;
}
    182e:	8082                	ret
    1830:	472d                	li	a4,11
    1832:	bd79                	j	16d0 <memset+0x1c>
    for (int i = 0; i < n; ++i, *(p++) = c)
    1834:	4f0d                	li	t5,3
    1836:	b701                	j	1736 <memset+0x82>
    1838:	8082                	ret
    183a:	4f05                	li	t5,1
    183c:	bded                	j	1736 <memset+0x82>
    183e:	8eaa                	mv	t4,a0
    1840:	4f01                	li	t5,0
    1842:	bdd5                	j	1736 <memset+0x82>
    1844:	87aa                	mv	a5,a0
    1846:	4701                	li	a4,0
    1848:	b7a1                	j	1790 <memset+0xdc>
    184a:	4f09                	li	t5,2
    184c:	b5ed                	j	1736 <memset+0x82>
    184e:	4f11                	li	t5,4
    1850:	b5dd                	j	1736 <memset+0x82>
    1852:	4f15                	li	t5,5
    1854:	b5cd                	j	1736 <memset+0x82>
    1856:	4f19                	li	t5,6
    1858:	bdf9                	j	1736 <memset+0x82>

000000000000185a <strcmp>:

int strcmp(const char *l, const char *r)
{
    for (; *l == *r && *l; l++, r++)
    185a:	00054783          	lbu	a5,0(a0)
    185e:	0005c703          	lbu	a4,0(a1)
    1862:	00e79863          	bne	a5,a4,1872 <strcmp+0x18>
    1866:	0505                	addi	a0,a0,1
    1868:	0585                	addi	a1,a1,1
    186a:	fbe5                	bnez	a5,185a <strcmp>
    186c:	4501                	li	a0,0
        ;
    return *(unsigned char *)l - *(unsigned char *)r;
}
    186e:	9d19                	subw	a0,a0,a4
    1870:	8082                	ret
    return *(unsigned char *)l - *(unsigned char *)r;
    1872:	0007851b          	sext.w	a0,a5
    1876:	bfe5                	j	186e <strcmp+0x14>

0000000000001878 <strncmp>:

int strncmp(const char *_l, const char *_r, size_t n)
{
    const unsigned char *l = (void *)_l, *r = (void *)_r;
    if (!n--)
    1878:	ce05                	beqz	a2,18b0 <strncmp+0x38>
        return 0;
    for (; *l && *r && n && *l == *r; l++, r++, n--)
    187a:	00054703          	lbu	a4,0(a0)
    187e:	0005c783          	lbu	a5,0(a1)
    1882:	cb0d                	beqz	a4,18b4 <strncmp+0x3c>
    if (!n--)
    1884:	167d                	addi	a2,a2,-1
    1886:	00c506b3          	add	a3,a0,a2
    188a:	a819                	j	18a0 <strncmp+0x28>
    for (; *l && *r && n && *l == *r; l++, r++, n--)
    188c:	00a68e63          	beq	a3,a0,18a8 <strncmp+0x30>
    1890:	0505                	addi	a0,a0,1
    1892:	00e79b63          	bne	a5,a4,18a8 <strncmp+0x30>
    1896:	00054703          	lbu	a4,0(a0)
        ;
    return *l - *r;
    189a:	0005c783          	lbu	a5,0(a1)
    for (; *l && *r && n && *l == *r; l++, r++, n--)
    189e:	cb19                	beqz	a4,18b4 <strncmp+0x3c>
    18a0:	0005c783          	lbu	a5,0(a1)
    18a4:	0585                	addi	a1,a1,1
    18a6:	f3fd                	bnez	a5,188c <strncmp+0x14>
    return *l - *r;
    18a8:	0007051b          	sext.w	a0,a4
    18ac:	9d1d                	subw	a0,a0,a5
    18ae:	8082                	ret
        return 0;
    18b0:	4501                	li	a0,0
}
    18b2:	8082                	ret
    18b4:	4501                	li	a0,0
    return *l - *r;
    18b6:	9d1d                	subw	a0,a0,a5
    18b8:	8082                	ret

00000000000018ba <strlen>:
size_t strlen(const char *s)
{
    const char *a = s;
    typedef size_t __attribute__((__may_alias__)) word;
    const word *w;
    for (; (uintptr_t)s % SS; s++)
    18ba:	00757793          	andi	a5,a0,7
    18be:	cf89                	beqz	a5,18d8 <strlen+0x1e>
    18c0:	87aa                	mv	a5,a0
    18c2:	a029                	j	18cc <strlen+0x12>
    18c4:	0785                	addi	a5,a5,1
    18c6:	0077f713          	andi	a4,a5,7
    18ca:	cb01                	beqz	a4,18da <strlen+0x20>
        if (!*s)
    18cc:	0007c703          	lbu	a4,0(a5)
    18d0:	fb75                	bnez	a4,18c4 <strlen+0xa>
    for (w = (const void *)s; !HASZERO(*w); w++)
        ;
    s = (const void *)w;
    for (; *s; s++)
        ;
    return s - a;
    18d2:	40a78533          	sub	a0,a5,a0
}
    18d6:	8082                	ret
    for (; (uintptr_t)s % SS; s++)
    18d8:	87aa                	mv	a5,a0
    for (w = (const void *)s; !HASZERO(*w); w++)
    18da:	6394                	ld	a3,0(a5)
    18dc:	00000597          	auipc	a1,0x0
    18e0:	6c45b583          	ld	a1,1732(a1) # 1fa0 <__clone+0xce>
    18e4:	00000617          	auipc	a2,0x0
    18e8:	6c463603          	ld	a2,1732(a2) # 1fa8 <__clone+0xd6>
    18ec:	a019                	j	18f2 <strlen+0x38>
    18ee:	6794                	ld	a3,8(a5)
    18f0:	07a1                	addi	a5,a5,8
    18f2:	00b68733          	add	a4,a3,a1
    18f6:	fff6c693          	not	a3,a3
    18fa:	8f75                	and	a4,a4,a3
    18fc:	8f71                	and	a4,a4,a2
    18fe:	db65                	beqz	a4,18ee <strlen+0x34>
    for (; *s; s++)
    1900:	0007c703          	lbu	a4,0(a5)
    1904:	d779                	beqz	a4,18d2 <strlen+0x18>
    1906:	0017c703          	lbu	a4,1(a5)
    190a:	0785                	addi	a5,a5,1
    190c:	d379                	beqz	a4,18d2 <strlen+0x18>
    190e:	0017c703          	lbu	a4,1(a5)
    1912:	0785                	addi	a5,a5,1
    1914:	fb6d                	bnez	a4,1906 <strlen+0x4c>
    1916:	bf75                	j	18d2 <strlen+0x18>

0000000000001918 <memchr>:

void *memchr(const void *src, int c, size_t n)
{
    const unsigned char *s = src;
    c = (unsigned char)c;
    for (; ((uintptr_t)s & ALIGN) && n && *s != c; s++, n--)
    1918:	00757713          	andi	a4,a0,7
{
    191c:	87aa                	mv	a5,a0
    c = (unsigned char)c;
    191e:	0ff5f593          	andi	a1,a1,255
    for (; ((uintptr_t)s & ALIGN) && n && *s != c; s++, n--)
    1922:	cb19                	beqz	a4,1938 <memchr+0x20>
    1924:	ce25                	beqz	a2,199c <memchr+0x84>
    1926:	0007c703          	lbu	a4,0(a5)
    192a:	04b70e63          	beq	a4,a1,1986 <memchr+0x6e>
    192e:	0785                	addi	a5,a5,1
    1930:	0077f713          	andi	a4,a5,7
    1934:	167d                	addi	a2,a2,-1
    1936:	f77d                	bnez	a4,1924 <memchr+0xc>
            ;
        s = (const void *)w;
    }
    for (; n && *s != c; s++, n--)
        ;
    return n ? (void *)s : 0;
    1938:	4501                	li	a0,0
    if (n && *s != c)
    193a:	c235                	beqz	a2,199e <memchr+0x86>
    193c:	0007c703          	lbu	a4,0(a5)
    1940:	04b70363          	beq	a4,a1,1986 <memchr+0x6e>
        size_t k = ONES * c;
    1944:	00000517          	auipc	a0,0x0
    1948:	66c53503          	ld	a0,1644(a0) # 1fb0 <__clone+0xde>
        for (w = (const void *)s; n >= SS && !HASZERO(*w ^ k); w++, n -= SS)
    194c:	471d                	li	a4,7
        size_t k = ONES * c;
    194e:	02a58533          	mul	a0,a1,a0
        for (w = (const void *)s; n >= SS && !HASZERO(*w ^ k); w++, n -= SS)
    1952:	02c77a63          	bgeu	a4,a2,1986 <memchr+0x6e>
    1956:	00000897          	auipc	a7,0x0
    195a:	64a8b883          	ld	a7,1610(a7) # 1fa0 <__clone+0xce>
    195e:	00000817          	auipc	a6,0x0
    1962:	64a83803          	ld	a6,1610(a6) # 1fa8 <__clone+0xd6>
    1966:	431d                	li	t1,7
    1968:	a029                	j	1972 <memchr+0x5a>
    196a:	1661                	addi	a2,a2,-8
    196c:	07a1                	addi	a5,a5,8
    196e:	02c37963          	bgeu	t1,a2,19a0 <memchr+0x88>
    1972:	6398                	ld	a4,0(a5)
    1974:	8f29                	xor	a4,a4,a0
    1976:	011706b3          	add	a3,a4,a7
    197a:	fff74713          	not	a4,a4
    197e:	8f75                	and	a4,a4,a3
    1980:	01077733          	and	a4,a4,a6
    1984:	d37d                	beqz	a4,196a <memchr+0x52>
    1986:	853e                	mv	a0,a5
    1988:	97b2                	add	a5,a5,a2
    198a:	a021                	j	1992 <memchr+0x7a>
    for (; n && *s != c; s++, n--)
    198c:	0505                	addi	a0,a0,1
    198e:	00f50763          	beq	a0,a5,199c <memchr+0x84>
    1992:	00054703          	lbu	a4,0(a0)
    1996:	feb71be3          	bne	a4,a1,198c <memchr+0x74>
    199a:	8082                	ret
    return n ? (void *)s : 0;
    199c:	4501                	li	a0,0
}
    199e:	8082                	ret
    return n ? (void *)s : 0;
    19a0:	4501                	li	a0,0
    for (; n && *s != c; s++, n--)
    19a2:	f275                	bnez	a2,1986 <memchr+0x6e>
}
    19a4:	8082                	ret

00000000000019a6 <strnlen>:

size_t strnlen(const char *s, size_t n)
{
    19a6:	1101                	addi	sp,sp,-32
    19a8:	e822                	sd	s0,16(sp)
    const char *p = memchr(s, 0, n);
    19aa:	862e                	mv	a2,a1
{
    19ac:	842e                	mv	s0,a1
    const char *p = memchr(s, 0, n);
    19ae:	4581                	li	a1,0
{
    19b0:	e426                	sd	s1,8(sp)
    19b2:	ec06                	sd	ra,24(sp)
    19b4:	84aa                	mv	s1,a0
    const char *p = memchr(s, 0, n);
    19b6:	f63ff0ef          	jal	ra,1918 <memchr>
    return p ? p - s : n;
    19ba:	c519                	beqz	a0,19c8 <strnlen+0x22>
}
    19bc:	60e2                	ld	ra,24(sp)
    19be:	6442                	ld	s0,16(sp)
    return p ? p - s : n;
    19c0:	8d05                	sub	a0,a0,s1
}
    19c2:	64a2                	ld	s1,8(sp)
    19c4:	6105                	addi	sp,sp,32
    19c6:	8082                	ret
    19c8:	60e2                	ld	ra,24(sp)
    return p ? p - s : n;
    19ca:	8522                	mv	a0,s0
}
    19cc:	6442                	ld	s0,16(sp)
    19ce:	64a2                	ld	s1,8(sp)
    19d0:	6105                	addi	sp,sp,32
    19d2:	8082                	ret

00000000000019d4 <strcpy>:
char *strcpy(char *restrict d, const char *s)
{
    typedef size_t __attribute__((__may_alias__)) word;
    word *wd;
    const word *ws;
    if ((uintptr_t)s % SS == (uintptr_t)d % SS)
    19d4:	00b547b3          	xor	a5,a0,a1
    19d8:	8b9d                	andi	a5,a5,7
    19da:	eb95                	bnez	a5,1a0e <strcpy+0x3a>
    {
        for (; (uintptr_t)s % SS; s++, d++)
    19dc:	0075f793          	andi	a5,a1,7
    19e0:	e7b1                	bnez	a5,1a2c <strcpy+0x58>
            if (!(*d = *s))
                return d;
        wd = (void *)d;
        ws = (const void *)s;
        for (; !HASZERO(*ws); *wd++ = *ws++)
    19e2:	6198                	ld	a4,0(a1)
    19e4:	00000617          	auipc	a2,0x0
    19e8:	5bc63603          	ld	a2,1468(a2) # 1fa0 <__clone+0xce>
    19ec:	00000817          	auipc	a6,0x0
    19f0:	5bc83803          	ld	a6,1468(a6) # 1fa8 <__clone+0xd6>
    19f4:	a029                	j	19fe <strcpy+0x2a>
    19f6:	e118                	sd	a4,0(a0)
    19f8:	6598                	ld	a4,8(a1)
    19fa:	05a1                	addi	a1,a1,8
    19fc:	0521                	addi	a0,a0,8
    19fe:	00c707b3          	add	a5,a4,a2
    1a02:	fff74693          	not	a3,a4
    1a06:	8ff5                	and	a5,a5,a3
    1a08:	0107f7b3          	and	a5,a5,a6
    1a0c:	d7ed                	beqz	a5,19f6 <strcpy+0x22>
            ;
        d = (void *)wd;
        s = (const void *)ws;
    }
    for (; (*d = *s); s++, d++)
    1a0e:	0005c783          	lbu	a5,0(a1)
    1a12:	00f50023          	sb	a5,0(a0)
    1a16:	c785                	beqz	a5,1a3e <strcpy+0x6a>
    1a18:	0015c783          	lbu	a5,1(a1)
    1a1c:	0505                	addi	a0,a0,1
    1a1e:	0585                	addi	a1,a1,1
    1a20:	00f50023          	sb	a5,0(a0)
    1a24:	fbf5                	bnez	a5,1a18 <strcpy+0x44>
        ;
    return d;
}
    1a26:	8082                	ret
        for (; (uintptr_t)s % SS; s++, d++)
    1a28:	0505                	addi	a0,a0,1
    1a2a:	df45                	beqz	a4,19e2 <strcpy+0xe>
            if (!(*d = *s))
    1a2c:	0005c783          	lbu	a5,0(a1)
        for (; (uintptr_t)s % SS; s++, d++)
    1a30:	0585                	addi	a1,a1,1
    1a32:	0075f713          	andi	a4,a1,7
            if (!(*d = *s))
    1a36:	00f50023          	sb	a5,0(a0)
    1a3a:	f7fd                	bnez	a5,1a28 <strcpy+0x54>
}
    1a3c:	8082                	ret
    1a3e:	8082                	ret

0000000000001a40 <strncpy>:
char *strncpy(char *restrict d, const char *s, size_t n)
{
    typedef size_t __attribute__((__may_alias__)) word;
    word *wd;
    const word *ws;
    if (((uintptr_t)s & ALIGN) == ((uintptr_t)d & ALIGN))
    1a40:	00b547b3          	xor	a5,a0,a1
    1a44:	8b9d                	andi	a5,a5,7
    1a46:	1a079863          	bnez	a5,1bf6 <strncpy+0x1b6>
    {
        for (; ((uintptr_t)s & ALIGN) && n && (*d = *s); n--, s++, d++)
    1a4a:	0075f793          	andi	a5,a1,7
    1a4e:	16078463          	beqz	a5,1bb6 <strncpy+0x176>
    1a52:	ea01                	bnez	a2,1a62 <strncpy+0x22>
    1a54:	a421                	j	1c5c <strncpy+0x21c>
    1a56:	167d                	addi	a2,a2,-1
    1a58:	0505                	addi	a0,a0,1
    1a5a:	14070e63          	beqz	a4,1bb6 <strncpy+0x176>
    1a5e:	1a060863          	beqz	a2,1c0e <strncpy+0x1ce>
    1a62:	0005c783          	lbu	a5,0(a1)
    1a66:	0585                	addi	a1,a1,1
    1a68:	0075f713          	andi	a4,a1,7
    1a6c:	00f50023          	sb	a5,0(a0)
    1a70:	f3fd                	bnez	a5,1a56 <strncpy+0x16>
    1a72:	4805                	li	a6,1
    1a74:	1a061863          	bnez	a2,1c24 <strncpy+0x1e4>
    1a78:	40a007b3          	neg	a5,a0
    1a7c:	8b9d                	andi	a5,a5,7
    1a7e:	4681                	li	a3,0
    1a80:	18061a63          	bnez	a2,1c14 <strncpy+0x1d4>
    1a84:	00778713          	addi	a4,a5,7
    1a88:	45ad                	li	a1,11
    1a8a:	18b76363          	bltu	a4,a1,1c10 <strncpy+0x1d0>
    1a8e:	1ae6eb63          	bltu	a3,a4,1c44 <strncpy+0x204>
    1a92:	1a078363          	beqz	a5,1c38 <strncpy+0x1f8>
    for (int i = 0; i < n; ++i, *(p++) = c)
    1a96:	00050023          	sb	zero,0(a0)
    1a9a:	4685                	li	a3,1
    1a9c:	00150713          	addi	a4,a0,1
    1aa0:	18d78f63          	beq	a5,a3,1c3e <strncpy+0x1fe>
    1aa4:	000500a3          	sb	zero,1(a0)
    1aa8:	4689                	li	a3,2
    1aaa:	00250713          	addi	a4,a0,2
    1aae:	18d78e63          	beq	a5,a3,1c4a <strncpy+0x20a>
    1ab2:	00050123          	sb	zero,2(a0)
    1ab6:	468d                	li	a3,3
    1ab8:	00350713          	addi	a4,a0,3
    1abc:	16d78c63          	beq	a5,a3,1c34 <strncpy+0x1f4>
    1ac0:	000501a3          	sb	zero,3(a0)
    1ac4:	4691                	li	a3,4
    1ac6:	00450713          	addi	a4,a0,4
    1aca:	18d78263          	beq	a5,a3,1c4e <strncpy+0x20e>
    1ace:	00050223          	sb	zero,4(a0)
    1ad2:	4695                	li	a3,5
    1ad4:	00550713          	addi	a4,a0,5
    1ad8:	16d78d63          	beq	a5,a3,1c52 <strncpy+0x212>
    1adc:	000502a3          	sb	zero,5(a0)
    1ae0:	469d                	li	a3,7
    1ae2:	00650713          	addi	a4,a0,6
    1ae6:	16d79863          	bne	a5,a3,1c56 <strncpy+0x216>
    1aea:	00750713          	addi	a4,a0,7
    1aee:	00050323          	sb	zero,6(a0)
    1af2:	40f80833          	sub	a6,a6,a5
    1af6:	ff887593          	andi	a1,a6,-8
    1afa:	97aa                	add	a5,a5,a0
    1afc:	95be                	add	a1,a1,a5
    1afe:	0007b023          	sd	zero,0(a5)
    1b02:	07a1                	addi	a5,a5,8
    1b04:	feb79de3          	bne	a5,a1,1afe <strncpy+0xbe>
    1b08:	ff887593          	andi	a1,a6,-8
    1b0c:	9ead                	addw	a3,a3,a1
    1b0e:	00b707b3          	add	a5,a4,a1
    1b12:	12b80863          	beq	a6,a1,1c42 <strncpy+0x202>
    1b16:	00078023          	sb	zero,0(a5)
    1b1a:	0016871b          	addiw	a4,a3,1
    1b1e:	0ec77863          	bgeu	a4,a2,1c0e <strncpy+0x1ce>
    1b22:	000780a3          	sb	zero,1(a5)
    1b26:	0026871b          	addiw	a4,a3,2
    1b2a:	0ec77263          	bgeu	a4,a2,1c0e <strncpy+0x1ce>
    1b2e:	00078123          	sb	zero,2(a5)
    1b32:	0036871b          	addiw	a4,a3,3
    1b36:	0cc77c63          	bgeu	a4,a2,1c0e <strncpy+0x1ce>
    1b3a:	000781a3          	sb	zero,3(a5)
    1b3e:	0046871b          	addiw	a4,a3,4
    1b42:	0cc77663          	bgeu	a4,a2,1c0e <strncpy+0x1ce>
    1b46:	00078223          	sb	zero,4(a5)
    1b4a:	0056871b          	addiw	a4,a3,5
    1b4e:	0cc77063          	bgeu	a4,a2,1c0e <strncpy+0x1ce>
    1b52:	000782a3          	sb	zero,5(a5)
    1b56:	0066871b          	addiw	a4,a3,6
    1b5a:	0ac77a63          	bgeu	a4,a2,1c0e <strncpy+0x1ce>
    1b5e:	00078323          	sb	zero,6(a5)
    1b62:	0076871b          	addiw	a4,a3,7
    1b66:	0ac77463          	bgeu	a4,a2,1c0e <strncpy+0x1ce>
    1b6a:	000783a3          	sb	zero,7(a5)
    1b6e:	0086871b          	addiw	a4,a3,8
    1b72:	08c77e63          	bgeu	a4,a2,1c0e <strncpy+0x1ce>
    1b76:	00078423          	sb	zero,8(a5)
    1b7a:	0096871b          	addiw	a4,a3,9
    1b7e:	08c77863          	bgeu	a4,a2,1c0e <strncpy+0x1ce>
    1b82:	000784a3          	sb	zero,9(a5)
    1b86:	00a6871b          	addiw	a4,a3,10
    1b8a:	08c77263          	bgeu	a4,a2,1c0e <strncpy+0x1ce>
    1b8e:	00078523          	sb	zero,10(a5)
    1b92:	00b6871b          	addiw	a4,a3,11
    1b96:	06c77c63          	bgeu	a4,a2,1c0e <strncpy+0x1ce>
    1b9a:	000785a3          	sb	zero,11(a5)
    1b9e:	00c6871b          	addiw	a4,a3,12
    1ba2:	06c77663          	bgeu	a4,a2,1c0e <strncpy+0x1ce>
    1ba6:	00078623          	sb	zero,12(a5)
    1baa:	26b5                	addiw	a3,a3,13
    1bac:	06c6f163          	bgeu	a3,a2,1c0e <strncpy+0x1ce>
    1bb0:	000786a3          	sb	zero,13(a5)
    1bb4:	8082                	ret
            ;
        if (!n || !*s)
    1bb6:	c645                	beqz	a2,1c5e <strncpy+0x21e>
    1bb8:	0005c783          	lbu	a5,0(a1)
    1bbc:	ea078be3          	beqz	a5,1a72 <strncpy+0x32>
            goto tail;
        wd = (void *)d;
        ws = (const void *)s;
        for (; n >= sizeof(size_t) && !HASZERO(*ws); n -= sizeof(size_t), ws++, wd++)
    1bc0:	479d                	li	a5,7
    1bc2:	02c7ff63          	bgeu	a5,a2,1c00 <strncpy+0x1c0>
    1bc6:	00000897          	auipc	a7,0x0
    1bca:	3da8b883          	ld	a7,986(a7) # 1fa0 <__clone+0xce>
    1bce:	00000817          	auipc	a6,0x0
    1bd2:	3da83803          	ld	a6,986(a6) # 1fa8 <__clone+0xd6>
    1bd6:	431d                	li	t1,7
    1bd8:	6198                	ld	a4,0(a1)
    1bda:	011707b3          	add	a5,a4,a7
    1bde:	fff74693          	not	a3,a4
    1be2:	8ff5                	and	a5,a5,a3
    1be4:	0107f7b3          	and	a5,a5,a6
    1be8:	ef81                	bnez	a5,1c00 <strncpy+0x1c0>
            *wd = *ws;
    1bea:	e118                	sd	a4,0(a0)
        for (; n >= sizeof(size_t) && !HASZERO(*ws); n -= sizeof(size_t), ws++, wd++)
    1bec:	1661                	addi	a2,a2,-8
    1bee:	05a1                	addi	a1,a1,8
    1bf0:	0521                	addi	a0,a0,8
    1bf2:	fec363e3          	bltu	t1,a2,1bd8 <strncpy+0x198>
        d = (void *)wd;
        s = (const void *)ws;
    }
    for (; n && (*d = *s); n--, s++, d++)
    1bf6:	e609                	bnez	a2,1c00 <strncpy+0x1c0>
    1bf8:	a08d                	j	1c5a <strncpy+0x21a>
    1bfa:	167d                	addi	a2,a2,-1
    1bfc:	0505                	addi	a0,a0,1
    1bfe:	ca01                	beqz	a2,1c0e <strncpy+0x1ce>
    1c00:	0005c783          	lbu	a5,0(a1)
    1c04:	0585                	addi	a1,a1,1
    1c06:	00f50023          	sb	a5,0(a0)
    1c0a:	fbe5                	bnez	a5,1bfa <strncpy+0x1ba>
        ;
tail:
    1c0c:	b59d                	j	1a72 <strncpy+0x32>
    memset(d, 0, n);
    return d;
}
    1c0e:	8082                	ret
    1c10:	472d                	li	a4,11
    1c12:	bdb5                	j	1a8e <strncpy+0x4e>
    1c14:	00778713          	addi	a4,a5,7
    1c18:	45ad                	li	a1,11
    1c1a:	fff60693          	addi	a3,a2,-1
    1c1e:	e6b778e3          	bgeu	a4,a1,1a8e <strncpy+0x4e>
    1c22:	b7fd                	j	1c10 <strncpy+0x1d0>
    1c24:	40a007b3          	neg	a5,a0
    1c28:	8832                	mv	a6,a2
    1c2a:	8b9d                	andi	a5,a5,7
    1c2c:	4681                	li	a3,0
    1c2e:	e4060be3          	beqz	a2,1a84 <strncpy+0x44>
    1c32:	b7cd                	j	1c14 <strncpy+0x1d4>
    for (int i = 0; i < n; ++i, *(p++) = c)
    1c34:	468d                	li	a3,3
    1c36:	bd75                	j	1af2 <strncpy+0xb2>
    1c38:	872a                	mv	a4,a0
    1c3a:	4681                	li	a3,0
    1c3c:	bd5d                	j	1af2 <strncpy+0xb2>
    1c3e:	4685                	li	a3,1
    1c40:	bd4d                	j	1af2 <strncpy+0xb2>
    1c42:	8082                	ret
    1c44:	87aa                	mv	a5,a0
    1c46:	4681                	li	a3,0
    1c48:	b5f9                	j	1b16 <strncpy+0xd6>
    1c4a:	4689                	li	a3,2
    1c4c:	b55d                	j	1af2 <strncpy+0xb2>
    1c4e:	4691                	li	a3,4
    1c50:	b54d                	j	1af2 <strncpy+0xb2>
    1c52:	4695                	li	a3,5
    1c54:	bd79                	j	1af2 <strncpy+0xb2>
    1c56:	4699                	li	a3,6
    1c58:	bd69                	j	1af2 <strncpy+0xb2>
    1c5a:	8082                	ret
    1c5c:	8082                	ret
    1c5e:	8082                	ret

0000000000001c60 <open>:
#include <unistd.h>

#include "syscall.h"

int open(const char *path, int flags)
{
    1c60:	87aa                	mv	a5,a0
    1c62:	862e                	mv	a2,a1
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
}

static inline long __syscall4(long n, long a, long b, long c, long d)
{
    register long a7 __asm__("a7") = n;
    1c64:	03800893          	li	a7,56
    register long a0 __asm__("a0") = a;
    1c68:	f9c00513          	li	a0,-100
    register long a1 __asm__("a1") = b;
    1c6c:	85be                	mv	a1,a5
    register long a2 __asm__("a2") = c;
    register long a3 __asm__("a3") = d;
    1c6e:	4689                	li	a3,2
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3))
    1c70:	00000073          	ecall
    return syscall(SYS_openat, AT_FDCWD, path, flags, O_RDWR);
}
    1c74:	2501                	sext.w	a0,a0
    1c76:	8082                	ret

0000000000001c78 <openat>:
    register long a7 __asm__("a7") = n;
    1c78:	03800893          	li	a7,56
    register long a3 __asm__("a3") = d;
    1c7c:	18000693          	li	a3,384
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3))
    1c80:	00000073          	ecall

int openat(int dirfd,const char *path, int flags)
{
    return syscall(SYS_openat, dirfd, path, flags, 0600);
}
    1c84:	2501                	sext.w	a0,a0
    1c86:	8082                	ret

0000000000001c88 <close>:
    register long a7 __asm__("a7") = n;
    1c88:	03900893          	li	a7,57
    __asm_syscall("r"(a7), "0"(a0))
    1c8c:	00000073          	ecall

int close(int fd)
{
    return syscall(SYS_close, fd);
}
    1c90:	2501                	sext.w	a0,a0
    1c92:	8082                	ret

0000000000001c94 <read>:
    register long a7 __asm__("a7") = n;
    1c94:	03f00893          	li	a7,63
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1c98:	00000073          	ecall

ssize_t read(int fd, void *buf, size_t len)
{
    return syscall(SYS_read, fd, buf, len);
}
    1c9c:	8082                	ret

0000000000001c9e <write>:
    register long a7 __asm__("a7") = n;
    1c9e:	04000893          	li	a7,64
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1ca2:	00000073          	ecall

ssize_t write(int fd, const void *buf, size_t len)
{
    return syscall(SYS_write, fd, buf, len);
}
    1ca6:	8082                	ret

0000000000001ca8 <getpid>:
    register long a7 __asm__("a7") = n;
    1ca8:	0ac00893          	li	a7,172
    __asm_syscall("r"(a7))
    1cac:	00000073          	ecall

pid_t getpid(void)
{
    return syscall(SYS_getpid);
}
    1cb0:	2501                	sext.w	a0,a0
    1cb2:	8082                	ret

0000000000001cb4 <getppid>:
    register long a7 __asm__("a7") = n;
    1cb4:	0ad00893          	li	a7,173
    __asm_syscall("r"(a7))
    1cb8:	00000073          	ecall

pid_t getppid(void)
{
    return syscall(SYS_getppid);
}
    1cbc:	2501                	sext.w	a0,a0
    1cbe:	8082                	ret

0000000000001cc0 <sched_yield>:
    register long a7 __asm__("a7") = n;
    1cc0:	07c00893          	li	a7,124
    __asm_syscall("r"(a7))
    1cc4:	00000073          	ecall

int sched_yield(void)
{
    return syscall(SYS_sched_yield);
}
    1cc8:	2501                	sext.w	a0,a0
    1cca:	8082                	ret

0000000000001ccc <fork>:
    register long a7 __asm__("a7") = n;
    1ccc:	0dc00893          	li	a7,220
    register long a0 __asm__("a0") = a;
    1cd0:	4545                	li	a0,17
    register long a1 __asm__("a1") = b;
    1cd2:	4581                	li	a1,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1cd4:	00000073          	ecall

pid_t fork(void)
{
    return syscall(SYS_clone, SIGCHLD, 0);
}
    1cd8:	2501                	sext.w	a0,a0
    1cda:	8082                	ret

0000000000001cdc <clone>:

pid_t clone(int (*fn)(void *arg), void *arg, void *stack, size_t stack_size, unsigned long flags)
{
    1cdc:	85b2                	mv	a1,a2
    1cde:	863a                	mv	a2,a4
    if (stack)
    1ce0:	c191                	beqz	a1,1ce4 <clone+0x8>
	stack += stack_size;
    1ce2:	95b6                	add	a1,a1,a3

    return __clone(fn, stack, flags, NULL, NULL, NULL);
    1ce4:	4781                	li	a5,0
    1ce6:	4701                	li	a4,0
    1ce8:	4681                	li	a3,0
    1cea:	2601                	sext.w	a2,a2
    1cec:	a2dd                	j	1ed2 <__clone>

0000000000001cee <exit>:
    register long a7 __asm__("a7") = n;
    1cee:	05d00893          	li	a7,93
    __asm_syscall("r"(a7), "0"(a0))
    1cf2:	00000073          	ecall
    //return syscall(SYS_clone, fn, stack, flags, NULL, NULL, NULL);
}
void exit(int code)
{
    syscall(SYS_exit, code);
}
    1cf6:	8082                	ret

0000000000001cf8 <waitpid>:
    register long a7 __asm__("a7") = n;
    1cf8:	10400893          	li	a7,260
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1cfc:	00000073          	ecall

int waitpid(int pid, int *code, int options)
{
    return syscall(SYS_wait4, pid, code, options);
}
    1d00:	2501                	sext.w	a0,a0
    1d02:	8082                	ret

0000000000001d04 <exec>:
    register long a7 __asm__("a7") = n;
    1d04:	0dd00893          	li	a7,221
    __asm_syscall("r"(a7), "0"(a0))
    1d08:	00000073          	ecall

int exec(char *name)
{
    return syscall(SYS_execve, name);
}
    1d0c:	2501                	sext.w	a0,a0
    1d0e:	8082                	ret

0000000000001d10 <execve>:
    register long a7 __asm__("a7") = n;
    1d10:	0dd00893          	li	a7,221
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1d14:	00000073          	ecall

int execve(const char *name, char *const argv[], char *const argp[])
{
    return syscall(SYS_execve, name, argv, argp);
}
    1d18:	2501                	sext.w	a0,a0
    1d1a:	8082                	ret

0000000000001d1c <times>:
    register long a7 __asm__("a7") = n;
    1d1c:	09900893          	li	a7,153
    __asm_syscall("r"(a7), "0"(a0))
    1d20:	00000073          	ecall

int times(void *mytimes)
{
	return syscall(SYS_times, mytimes);
}
    1d24:	2501                	sext.w	a0,a0
    1d26:	8082                	ret

0000000000001d28 <get_time>:

int64 get_time()
{
    1d28:	1141                	addi	sp,sp,-16
    register long a7 __asm__("a7") = n;
    1d2a:	0a900893          	li	a7,169
    register long a0 __asm__("a0") = a;
    1d2e:	850a                	mv	a0,sp
    register long a1 __asm__("a1") = b;
    1d30:	4581                	li	a1,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1d32:	00000073          	ecall
    TimeVal time;
    int err = sys_get_time(&time, 0);
    if (err == 0)
    1d36:	2501                	sext.w	a0,a0
    1d38:	ed09                	bnez	a0,1d52 <get_time+0x2a>
    {
        return ((time.sec & 0xffff) * 1000 + time.usec / 1000);
    1d3a:	67a2                	ld	a5,8(sp)
    1d3c:	3e800713          	li	a4,1000
    1d40:	00015503          	lhu	a0,0(sp)
    1d44:	02e7d7b3          	divu	a5,a5,a4
    1d48:	02e50533          	mul	a0,a0,a4
    1d4c:	953e                	add	a0,a0,a5
    }
    else
    {
        return -1;
    }
}
    1d4e:	0141                	addi	sp,sp,16
    1d50:	8082                	ret
        return -1;
    1d52:	557d                	li	a0,-1
    1d54:	bfed                	j	1d4e <get_time+0x26>

0000000000001d56 <sys_get_time>:
    register long a7 __asm__("a7") = n;
    1d56:	0a900893          	li	a7,169
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1d5a:	00000073          	ecall

int sys_get_time(TimeVal *ts, int tz)
{
    return syscall(SYS_gettimeofday, ts, tz);
}
    1d5e:	2501                	sext.w	a0,a0
    1d60:	8082                	ret

0000000000001d62 <time>:
    register long a7 __asm__("a7") = n;
    1d62:	42600893          	li	a7,1062
    __asm_syscall("r"(a7), "0"(a0))
    1d66:	00000073          	ecall

int time(unsigned long *tloc)
{
    return syscall(SYS_time, tloc);
}
    1d6a:	2501                	sext.w	a0,a0
    1d6c:	8082                	ret

0000000000001d6e <sleep>:

int sleep(unsigned long long time)
{
    1d6e:	1141                	addi	sp,sp,-16
    TimeVal tv = {.sec = time, .usec = 0};
    1d70:	e02a                	sd	a0,0(sp)
    register long a0 __asm__("a0") = a;
    1d72:	850a                	mv	a0,sp
    1d74:	e402                	sd	zero,8(sp)
    register long a7 __asm__("a7") = n;
    1d76:	06500893          	li	a7,101
    register long a1 __asm__("a1") = b;
    1d7a:	85aa                	mv	a1,a0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1d7c:	00000073          	ecall
    if (syscall(SYS_nanosleep, &tv, &tv)) return tv.sec;
    1d80:	e501                	bnez	a0,1d88 <sleep+0x1a>
    return 0;
    1d82:	4501                	li	a0,0
}
    1d84:	0141                	addi	sp,sp,16
    1d86:	8082                	ret
    if (syscall(SYS_nanosleep, &tv, &tv)) return tv.sec;
    1d88:	4502                	lw	a0,0(sp)
}
    1d8a:	0141                	addi	sp,sp,16
    1d8c:	8082                	ret

0000000000001d8e <set_priority>:
    register long a7 __asm__("a7") = n;
    1d8e:	08c00893          	li	a7,140
    __asm_syscall("r"(a7), "0"(a0))
    1d92:	00000073          	ecall

int set_priority(int prio)
{
    return syscall(SYS_setpriority, prio);
}
    1d96:	2501                	sext.w	a0,a0
    1d98:	8082                	ret

0000000000001d9a <mmap>:
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4))
}

static inline long __syscall6(long n, long a, long b, long c, long d, long e, long f)
{
    register long a7 __asm__("a7") = n;
    1d9a:	0de00893          	li	a7,222
    register long a1 __asm__("a1") = b;
    register long a2 __asm__("a2") = c;
    register long a3 __asm__("a3") = d;
    register long a4 __asm__("a4") = e;
    register long a5 __asm__("a5") = f;
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4), "r"(a5))
    1d9e:	00000073          	ecall

void *mmap(void *start, size_t len, int prot, int flags, int fd, off_t off)
{
    return syscall(SYS_mmap, start, len, prot, flags, fd, off);
}
    1da2:	8082                	ret

0000000000001da4 <munmap>:
    register long a7 __asm__("a7") = n;
    1da4:	0d700893          	li	a7,215
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1da8:	00000073          	ecall

int munmap(void *start, size_t len)
{
    return syscall(SYS_munmap, start, len);
}
    1dac:	2501                	sext.w	a0,a0
    1dae:	8082                	ret

0000000000001db0 <wait>:

int wait(int *code)
{
    1db0:	85aa                	mv	a1,a0
    register long a7 __asm__("a7") = n;
    1db2:	10400893          	li	a7,260
    register long a0 __asm__("a0") = a;
    1db6:	557d                	li	a0,-1
    register long a2 __asm__("a2") = c;
    1db8:	4601                	li	a2,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1dba:	00000073          	ecall
    return waitpid(-1, code, 0);
}
    1dbe:	2501                	sext.w	a0,a0
    1dc0:	8082                	ret

0000000000001dc2 <spawn>:
    register long a7 __asm__("a7") = n;
    1dc2:	19000893          	li	a7,400
    __asm_syscall("r"(a7), "0"(a0))
    1dc6:	00000073          	ecall

int spawn(char *file)
{
    return syscall(SYS_spawn, file);
}
    1dca:	2501                	sext.w	a0,a0
    1dcc:	8082                	ret

0000000000001dce <mailread>:
    register long a7 __asm__("a7") = n;
    1dce:	19100893          	li	a7,401
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1dd2:	00000073          	ecall

int mailread(void *buf, int len)
{
    return syscall(SYS_mailread, buf, len);
}
    1dd6:	2501                	sext.w	a0,a0
    1dd8:	8082                	ret

0000000000001dda <mailwrite>:
    register long a7 __asm__("a7") = n;
    1dda:	19200893          	li	a7,402
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1dde:	00000073          	ecall

int mailwrite(int pid, void *buf, int len)
{
    return syscall(SYS_mailwrite, pid, buf, len);
}
    1de2:	2501                	sext.w	a0,a0
    1de4:	8082                	ret

0000000000001de6 <fstat>:
    register long a7 __asm__("a7") = n;
    1de6:	05000893          	li	a7,80
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1dea:	00000073          	ecall

int fstat(int fd, struct kstat *st)
{
    return syscall(SYS_fstat, fd, st);
}
    1dee:	2501                	sext.w	a0,a0
    1df0:	8082                	ret

0000000000001df2 <sys_linkat>:
    register long a4 __asm__("a4") = e;
    1df2:	1702                	slli	a4,a4,0x20
    register long a7 __asm__("a7") = n;
    1df4:	02500893          	li	a7,37
    register long a4 __asm__("a4") = e;
    1df8:	9301                	srli	a4,a4,0x20
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4))
    1dfa:	00000073          	ecall

int sys_linkat(int olddirfd, char *oldpath, int newdirfd, char *newpath, unsigned int flags)
{
    return syscall(SYS_linkat, olddirfd, oldpath, newdirfd, newpath, flags);
}
    1dfe:	2501                	sext.w	a0,a0
    1e00:	8082                	ret

0000000000001e02 <sys_unlinkat>:
    register long a2 __asm__("a2") = c;
    1e02:	1602                	slli	a2,a2,0x20
    register long a7 __asm__("a7") = n;
    1e04:	02300893          	li	a7,35
    register long a2 __asm__("a2") = c;
    1e08:	9201                	srli	a2,a2,0x20
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1e0a:	00000073          	ecall

int sys_unlinkat(int dirfd, char *path, unsigned int flags)
{
    return syscall(SYS_unlinkat, dirfd, path, flags);
}
    1e0e:	2501                	sext.w	a0,a0
    1e10:	8082                	ret

0000000000001e12 <link>:

int link(char *old_path, char *new_path)
{
    1e12:	87aa                	mv	a5,a0
    1e14:	86ae                	mv	a3,a1
    register long a7 __asm__("a7") = n;
    1e16:	02500893          	li	a7,37
    register long a0 __asm__("a0") = a;
    1e1a:	f9c00513          	li	a0,-100
    register long a1 __asm__("a1") = b;
    1e1e:	85be                	mv	a1,a5
    register long a2 __asm__("a2") = c;
    1e20:	f9c00613          	li	a2,-100
    register long a4 __asm__("a4") = e;
    1e24:	4701                	li	a4,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4))
    1e26:	00000073          	ecall
    return sys_linkat(AT_FDCWD, old_path, AT_FDCWD, new_path, 0);
}
    1e2a:	2501                	sext.w	a0,a0
    1e2c:	8082                	ret

0000000000001e2e <unlink>:

int unlink(char *path)
{
    1e2e:	85aa                	mv	a1,a0
    register long a7 __asm__("a7") = n;
    1e30:	02300893          	li	a7,35
    register long a0 __asm__("a0") = a;
    1e34:	f9c00513          	li	a0,-100
    register long a2 __asm__("a2") = c;
    1e38:	4601                	li	a2,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1e3a:	00000073          	ecall
    return sys_unlinkat(AT_FDCWD, path, 0);
}
    1e3e:	2501                	sext.w	a0,a0
    1e40:	8082                	ret

0000000000001e42 <uname>:
    register long a7 __asm__("a7") = n;
    1e42:	0a000893          	li	a7,160
    __asm_syscall("r"(a7), "0"(a0))
    1e46:	00000073          	ecall

int uname(void *buf)
{
    return syscall(SYS_uname, buf);
}
    1e4a:	2501                	sext.w	a0,a0
    1e4c:	8082                	ret

0000000000001e4e <brk>:
    register long a7 __asm__("a7") = n;
    1e4e:	0d600893          	li	a7,214
    __asm_syscall("r"(a7), "0"(a0))
    1e52:	00000073          	ecall

int brk(void *addr)
{
    return syscall(SYS_brk, addr);
}
    1e56:	2501                	sext.w	a0,a0
    1e58:	8082                	ret

0000000000001e5a <getcwd>:
    register long a7 __asm__("a7") = n;
    1e5a:	48c5                	li	a7,17
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1e5c:	00000073          	ecall

char *getcwd(char *buf, size_t size){
    return syscall(SYS_getcwd, buf, size);
}
    1e60:	8082                	ret

0000000000001e62 <chdir>:
    register long a7 __asm__("a7") = n;
    1e62:	03100893          	li	a7,49
    __asm_syscall("r"(a7), "0"(a0))
    1e66:	00000073          	ecall

int chdir(const char *path){
    return syscall(SYS_chdir, path);
}
    1e6a:	2501                	sext.w	a0,a0
    1e6c:	8082                	ret

0000000000001e6e <mkdir>:

int mkdir(const char *path, mode_t mode){
    1e6e:	862e                	mv	a2,a1
    1e70:	87aa                	mv	a5,a0
    register long a2 __asm__("a2") = c;
    1e72:	1602                	slli	a2,a2,0x20
    register long a7 __asm__("a7") = n;
    1e74:	02200893          	li	a7,34
    register long a0 __asm__("a0") = a;
    1e78:	f9c00513          	li	a0,-100
    register long a1 __asm__("a1") = b;
    1e7c:	85be                	mv	a1,a5
    register long a2 __asm__("a2") = c;
    1e7e:	9201                	srli	a2,a2,0x20
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1e80:	00000073          	ecall
    return syscall(SYS_mkdirat, AT_FDCWD, path, mode);
}
    1e84:	2501                	sext.w	a0,a0
    1e86:	8082                	ret

0000000000001e88 <getdents>:
    register long a7 __asm__("a7") = n;
    1e88:	03d00893          	li	a7,61
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1e8c:	00000073          	ecall

int getdents(int fd, struct linux_dirent64 *dirp64, unsigned long len){
    //return syscall(SYS_getdents64, fd, dirp64, len);
    return syscall(SYS_getdents64, fd, dirp64, len);
}
    1e90:	2501                	sext.w	a0,a0
    1e92:	8082                	ret

0000000000001e94 <pipe>:
    register long a7 __asm__("a7") = n;
    1e94:	03b00893          	li	a7,59
    register long a1 __asm__("a1") = b;
    1e98:	4581                	li	a1,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1e9a:	00000073          	ecall

int pipe(int fd[2]){
    return syscall(SYS_pipe2, fd, 0);
}
    1e9e:	2501                	sext.w	a0,a0
    1ea0:	8082                	ret

0000000000001ea2 <dup>:
    register long a7 __asm__("a7") = n;
    1ea2:	48dd                	li	a7,23
    __asm_syscall("r"(a7), "0"(a0))
    1ea4:	00000073          	ecall

int dup(int fd){
    return syscall(SYS_dup, fd);
}
    1ea8:	2501                	sext.w	a0,a0
    1eaa:	8082                	ret

0000000000001eac <dup2>:
    register long a7 __asm__("a7") = n;
    1eac:	48e1                	li	a7,24
    register long a2 __asm__("a2") = c;
    1eae:	4601                	li	a2,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1eb0:	00000073          	ecall

int dup2(int old, int new){
    return syscall(SYS_dup3, old, new, 0);
}
    1eb4:	2501                	sext.w	a0,a0
    1eb6:	8082                	ret

0000000000001eb8 <mount>:
    register long a7 __asm__("a7") = n;
    1eb8:	02800893          	li	a7,40
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4))
    1ebc:	00000073          	ecall

int mount(const char *special, const char *dir, const char *fstype, unsigned long flags, const void *data)
{
        return syscall(SYS_mount, special, dir, fstype, flags, data);
}
    1ec0:	2501                	sext.w	a0,a0
    1ec2:	8082                	ret

0000000000001ec4 <umount>:
    register long a7 __asm__("a7") = n;
    1ec4:	02700893          	li	a7,39
    register long a1 __asm__("a1") = b;
    1ec8:	4581                	li	a1,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1eca:	00000073          	ecall

int umount(const char *special)
{
        return syscall(SYS_umount2, special, 0);
}
    1ece:	2501                	sext.w	a0,a0
    1ed0:	8082                	ret

0000000000001ed2 <__clone>:

.global __clone
.type  __clone, %function
__clone:
	# Save func and arg to stack
	addi a1, a1, -16
    1ed2:	15c1                	addi	a1,a1,-16
	sd a0, 0(a1)
    1ed4:	e188                	sd	a0,0(a1)
	sd a3, 8(a1)
    1ed6:	e594                	sd	a3,8(a1)

	# Call SYS_clone
	mv a0, a2
    1ed8:	8532                	mv	a0,a2
	mv a2, a4
    1eda:	863a                	mv	a2,a4
	mv a3, a5
    1edc:	86be                	mv	a3,a5
	mv a4, a6
    1ede:	8742                	mv	a4,a6
	li a7, 220 # SYS_clone
    1ee0:	0dc00893          	li	a7,220
	ecall
    1ee4:	00000073          	ecall

	beqz a0, 1f
    1ee8:	c111                	beqz	a0,1eec <__clone+0x1a>
	# Parent
	ret
    1eea:	8082                	ret

	# Child
1:      ld a1, 0(sp)
    1eec:	6582                	ld	a1,0(sp)
	ld a0, 8(sp)
    1eee:	6522                	ld	a0,8(sp)
	jalr a1
    1ef0:	9582                	jalr	a1

	# Exit
	li a7, 93 # SYS_exit
    1ef2:	05d00893          	li	a7,93
	ecall
    1ef6:	00000073          	ecall
