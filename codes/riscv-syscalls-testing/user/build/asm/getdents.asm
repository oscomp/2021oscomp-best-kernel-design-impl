
/home/ubuntu/rust/ultra-os/codes/riscv-syscalls-testing/user/build/riscv64/getdents:     file format elf64-littleriscv


Disassembly of section .text:

0000000000001000 <_start>:
.section .text.entry
.globl _start
_start:
    mv a0, sp
    1000:	850a                	mv	a0,sp
    tail __start_main
    1002:	a0cd                	j	10e4 <__start_main>

0000000000001004 <test_getdents>:
#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"

char buf[512];
void test_getdents(void){
    1004:	1101                	addi	sp,sp,-32
    TEST_START(__func__);
    1006:	00001517          	auipc	a0,0x1
    100a:	f1a50513          	addi	a0,a0,-230 # 1f20 <__clone+0x2e>
void test_getdents(void){
    100e:	ec06                	sd	ra,24(sp)
    1010:	e822                	sd	s0,16(sp)
    1012:	e426                	sd	s1,8(sp)
    TEST_START(__func__);
    1014:	342000ef          	jal	ra,1356 <puts>
    1018:	00001517          	auipc	a0,0x1
    101c:	1c050513          	addi	a0,a0,448 # 21d8 <__func__.0>
    1020:	336000ef          	jal	ra,1356 <puts>
    1024:	00001517          	auipc	a0,0x1
    1028:	f1450513          	addi	a0,a0,-236 # 1f38 <__clone+0x46>
    102c:	32a000ef          	jal	ra,1356 <puts>
    int fd, nread;
    struct linux_dirent64 *dirp64;
    dirp64 = buf;
    //fd = open(".", O_DIRECTORY);
    fd = open(".", O_RDONLY);
    1030:	4581                	li	a1,0
    1032:	00001517          	auipc	a0,0x1
    1036:	f1650513          	addi	a0,a0,-234 # 1f48 <__clone+0x56>
    103a:	447000ef          	jal	ra,1c80 <open>
    printf("open fd:%d\n", fd);
    103e:	85aa                	mv	a1,a0
    fd = open(".", O_RDONLY);
    1040:	842a                	mv	s0,a0
    printf("open fd:%d\n", fd);
    1042:	00001517          	auipc	a0,0x1
    1046:	f0e50513          	addi	a0,a0,-242 # 1f50 <__clone+0x5e>
    104a:	32e000ef          	jal	ra,1378 <printf>

	nread = getdents(fd, dirp64, 512);
    104e:	20000613          	li	a2,512
    1052:	00001597          	auipc	a1,0x1
    1056:	f8658593          	addi	a1,a1,-122 # 1fd8 <buf>
    105a:	8522                	mv	a0,s0
    105c:	64d000ef          	jal	ra,1ea8 <getdents>
	printf("getdents fd:%d\n", nread);
    1060:	85aa                	mv	a1,a0
	nread = getdents(fd, dirp64, 512);
    1062:	84aa                	mv	s1,a0
	printf("getdents fd:%d\n", nread);
    1064:	00001517          	auipc	a0,0x1
    1068:	efc50513          	addi	a0,a0,-260 # 1f60 <__clone+0x6e>
    106c:	30c000ef          	jal	ra,1378 <printf>
	assert(nread != -1);
    1070:	57fd                	li	a5,-1
    1072:	04f48a63          	beq	s1,a5,10c6 <test_getdents+0xc2>
	printf("getdents success.\n%s\n", dirp64->d_name);
    1076:	00001597          	auipc	a1,0x1
    107a:	f7558593          	addi	a1,a1,-139 # 1feb <buf+0x13>
    107e:	00001517          	auipc	a0,0x1
    1082:	f1250513          	addi	a0,a0,-238 # 1f90 <__clone+0x9e>
    1086:	2f2000ef          	jal	ra,1378 <printf>
	    printf(  "%s\t", d->d_name);
	    bpos += d->d_reclen;
	}
	*/

    printf("\n");
    108a:	00001517          	auipc	a0,0x1
    108e:	efe50513          	addi	a0,a0,-258 # 1f88 <__clone+0x96>
    1092:	2e6000ef          	jal	ra,1378 <printf>
    close(fd);
    1096:	8522                	mv	a0,s0
    1098:	411000ef          	jal	ra,1ca8 <close>
    TEST_END(__func__);
    109c:	00001517          	auipc	a0,0x1
    10a0:	f0c50513          	addi	a0,a0,-244 # 1fa8 <__clone+0xb6>
    10a4:	2b2000ef          	jal	ra,1356 <puts>
    10a8:	00001517          	auipc	a0,0x1
    10ac:	13050513          	addi	a0,a0,304 # 21d8 <__func__.0>
    10b0:	2a6000ef          	jal	ra,1356 <puts>
}
    10b4:	6442                	ld	s0,16(sp)
    10b6:	60e2                	ld	ra,24(sp)
    10b8:	64a2                	ld	s1,8(sp)
    TEST_END(__func__);
    10ba:	00001517          	auipc	a0,0x1
    10be:	e7e50513          	addi	a0,a0,-386 # 1f38 <__clone+0x46>
}
    10c2:	6105                	addi	sp,sp,32
    TEST_END(__func__);
    10c4:	ac49                	j	1356 <puts>
	assert(nread != -1);
    10c6:	00001517          	auipc	a0,0x1
    10ca:	eaa50513          	addi	a0,a0,-342 # 1f70 <__clone+0x7e>
    10ce:	52e000ef          	jal	ra,15fc <panic>
    10d2:	b755                	j	1076 <test_getdents+0x72>

00000000000010d4 <main>:

int main(void){
    10d4:	1141                	addi	sp,sp,-16
    10d6:	e406                	sd	ra,8(sp)
    test_getdents();
    10d8:	f2dff0ef          	jal	ra,1004 <test_getdents>
    return 0;
}
    10dc:	60a2                	ld	ra,8(sp)
    10de:	4501                	li	a0,0
    10e0:	0141                	addi	sp,sp,16
    10e2:	8082                	ret

00000000000010e4 <__start_main>:
#include <unistd.h>

extern int main();

int __start_main(long *p)
{
    10e4:	85aa                	mv	a1,a0
	int argc = p[0];
	char **argv = (void *)(p+1);

	exit(main(argc, argv));
    10e6:	4108                	lw	a0,0(a0)
{
    10e8:	1141                	addi	sp,sp,-16
	exit(main(argc, argv));
    10ea:	05a1                	addi	a1,a1,8
{
    10ec:	e406                	sd	ra,8(sp)
	exit(main(argc, argv));
    10ee:	fe7ff0ef          	jal	ra,10d4 <main>
    10f2:	41d000ef          	jal	ra,1d0e <exit>
	return 0;
}
    10f6:	60a2                	ld	ra,8(sp)
    10f8:	4501                	li	a0,0
    10fa:	0141                	addi	sp,sp,16
    10fc:	8082                	ret

00000000000010fe <printint.constprop.0>:
    write(f, s, l);
}

static char digits[] = "0123456789abcdef";

static void printint(int xx, int base, int sign)
    10fe:	7179                	addi	sp,sp,-48
    1100:	f406                	sd	ra,40(sp)
{
    char buf[16 + 1];
    int i;
    uint x;

    if (sign && (sign = xx < 0))
    1102:	12054b63          	bltz	a0,1238 <printint.constprop.0+0x13a>

    buf[16] = 0;
    i = 15;
    do
    {
        buf[i--] = digits[x % base];
    1106:	02b577bb          	remuw	a5,a0,a1
    110a:	00001617          	auipc	a2,0x1
    110e:	0de60613          	addi	a2,a2,222 # 21e8 <digits>
    buf[16] = 0;
    1112:	00010c23          	sb	zero,24(sp)
        buf[i--] = digits[x % base];
    1116:	0005871b          	sext.w	a4,a1
    111a:	1782                	slli	a5,a5,0x20
    111c:	9381                	srli	a5,a5,0x20
    111e:	97b2                	add	a5,a5,a2
    1120:	0007c783          	lbu	a5,0(a5)
    } while ((x /= base) != 0);
    1124:	02b5583b          	divuw	a6,a0,a1
        buf[i--] = digits[x % base];
    1128:	00f10ba3          	sb	a5,23(sp)
    } while ((x /= base) != 0);
    112c:	1cb56363          	bltu	a0,a1,12f2 <printint.constprop.0+0x1f4>
        buf[i--] = digits[x % base];
    1130:	45b9                	li	a1,14
    1132:	02e877bb          	remuw	a5,a6,a4
    1136:	1782                	slli	a5,a5,0x20
    1138:	9381                	srli	a5,a5,0x20
    113a:	97b2                	add	a5,a5,a2
    113c:	0007c783          	lbu	a5,0(a5)
    } while ((x /= base) != 0);
    1140:	02e856bb          	divuw	a3,a6,a4
        buf[i--] = digits[x % base];
    1144:	00f10b23          	sb	a5,22(sp)
    } while ((x /= base) != 0);
    1148:	0ce86e63          	bltu	a6,a4,1224 <printint.constprop.0+0x126>
        buf[i--] = digits[x % base];
    114c:	02e6f5bb          	remuw	a1,a3,a4
    } while ((x /= base) != 0);
    1150:	02e6d7bb          	divuw	a5,a3,a4
        buf[i--] = digits[x % base];
    1154:	1582                	slli	a1,a1,0x20
    1156:	9181                	srli	a1,a1,0x20
    1158:	95b2                	add	a1,a1,a2
    115a:	0005c583          	lbu	a1,0(a1)
    115e:	00b10aa3          	sb	a1,21(sp)
    } while ((x /= base) != 0);
    1162:	0007859b          	sext.w	a1,a5
    1166:	12e6ec63          	bltu	a3,a4,129e <printint.constprop.0+0x1a0>
        buf[i--] = digits[x % base];
    116a:	02e7f6bb          	remuw	a3,a5,a4
    116e:	1682                	slli	a3,a3,0x20
    1170:	9281                	srli	a3,a3,0x20
    1172:	96b2                	add	a3,a3,a2
    1174:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    1178:	02e7d83b          	divuw	a6,a5,a4
        buf[i--] = digits[x % base];
    117c:	00d10a23          	sb	a3,20(sp)
    } while ((x /= base) != 0);
    1180:	12e5e863          	bltu	a1,a4,12b0 <printint.constprop.0+0x1b2>
        buf[i--] = digits[x % base];
    1184:	02e876bb          	remuw	a3,a6,a4
    1188:	1682                	slli	a3,a3,0x20
    118a:	9281                	srli	a3,a3,0x20
    118c:	96b2                	add	a3,a3,a2
    118e:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    1192:	02e855bb          	divuw	a1,a6,a4
        buf[i--] = digits[x % base];
    1196:	00d109a3          	sb	a3,19(sp)
    } while ((x /= base) != 0);
    119a:	12e86463          	bltu	a6,a4,12c2 <printint.constprop.0+0x1c4>
        buf[i--] = digits[x % base];
    119e:	02e5f6bb          	remuw	a3,a1,a4
    11a2:	1682                	slli	a3,a3,0x20
    11a4:	9281                	srli	a3,a3,0x20
    11a6:	96b2                	add	a3,a3,a2
    11a8:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    11ac:	02e5d83b          	divuw	a6,a1,a4
        buf[i--] = digits[x % base];
    11b0:	00d10923          	sb	a3,18(sp)
    } while ((x /= base) != 0);
    11b4:	0ce5ec63          	bltu	a1,a4,128c <printint.constprop.0+0x18e>
        buf[i--] = digits[x % base];
    11b8:	02e876bb          	remuw	a3,a6,a4
    11bc:	1682                	slli	a3,a3,0x20
    11be:	9281                	srli	a3,a3,0x20
    11c0:	96b2                	add	a3,a3,a2
    11c2:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    11c6:	02e855bb          	divuw	a1,a6,a4
        buf[i--] = digits[x % base];
    11ca:	00d108a3          	sb	a3,17(sp)
    } while ((x /= base) != 0);
    11ce:	10e86963          	bltu	a6,a4,12e0 <printint.constprop.0+0x1e2>
        buf[i--] = digits[x % base];
    11d2:	02e5f6bb          	remuw	a3,a1,a4
    11d6:	1682                	slli	a3,a3,0x20
    11d8:	9281                	srli	a3,a3,0x20
    11da:	96b2                	add	a3,a3,a2
    11dc:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    11e0:	02e5d83b          	divuw	a6,a1,a4
        buf[i--] = digits[x % base];
    11e4:	00d10823          	sb	a3,16(sp)
    } while ((x /= base) != 0);
    11e8:	10e5e763          	bltu	a1,a4,12f6 <printint.constprop.0+0x1f8>
        buf[i--] = digits[x % base];
    11ec:	02e876bb          	remuw	a3,a6,a4
    11f0:	1682                	slli	a3,a3,0x20
    11f2:	9281                	srli	a3,a3,0x20
    11f4:	96b2                	add	a3,a3,a2
    11f6:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    11fa:	02e857bb          	divuw	a5,a6,a4
        buf[i--] = digits[x % base];
    11fe:	00d107a3          	sb	a3,15(sp)
    } while ((x /= base) != 0);
    1202:	10e86363          	bltu	a6,a4,1308 <printint.constprop.0+0x20a>
        buf[i--] = digits[x % base];
    1206:	1782                	slli	a5,a5,0x20
    1208:	9381                	srli	a5,a5,0x20
    120a:	97b2                	add	a5,a5,a2
    120c:	0007c783          	lbu	a5,0(a5)
    1210:	4599                	li	a1,6
    1212:	00f10723          	sb	a5,14(sp)

    if (sign)
    1216:	00055763          	bgez	a0,1224 <printint.constprop.0+0x126>
        buf[i--] = '-';
    121a:	02d00793          	li	a5,45
    121e:	00f106a3          	sb	a5,13(sp)
        buf[i--] = digits[x % base];
    1222:	4595                	li	a1,5
    write(f, s, l);
    1224:	003c                	addi	a5,sp,8
    1226:	4641                	li	a2,16
    1228:	9e0d                	subw	a2,a2,a1
    122a:	4505                	li	a0,1
    122c:	95be                	add	a1,a1,a5
    122e:	291000ef          	jal	ra,1cbe <write>
    i++;
    if (i < 0)
        puts("printint error");
    out(stdout, buf + i, 16 - i);
}
    1232:	70a2                	ld	ra,40(sp)
    1234:	6145                	addi	sp,sp,48
    1236:	8082                	ret
        x = -xx;
    1238:	40a0083b          	negw	a6,a0
        buf[i--] = digits[x % base];
    123c:	02b877bb          	remuw	a5,a6,a1
    1240:	00001617          	auipc	a2,0x1
    1244:	fa860613          	addi	a2,a2,-88 # 21e8 <digits>
    buf[16] = 0;
    1248:	00010c23          	sb	zero,24(sp)
        buf[i--] = digits[x % base];
    124c:	0005871b          	sext.w	a4,a1
    1250:	1782                	slli	a5,a5,0x20
    1252:	9381                	srli	a5,a5,0x20
    1254:	97b2                	add	a5,a5,a2
    1256:	0007c783          	lbu	a5,0(a5)
    } while ((x /= base) != 0);
    125a:	02b858bb          	divuw	a7,a6,a1
        buf[i--] = digits[x % base];
    125e:	00f10ba3          	sb	a5,23(sp)
    } while ((x /= base) != 0);
    1262:	06b86963          	bltu	a6,a1,12d4 <printint.constprop.0+0x1d6>
        buf[i--] = digits[x % base];
    1266:	02e8f7bb          	remuw	a5,a7,a4
    126a:	1782                	slli	a5,a5,0x20
    126c:	9381                	srli	a5,a5,0x20
    126e:	97b2                	add	a5,a5,a2
    1270:	0007c783          	lbu	a5,0(a5)
    } while ((x /= base) != 0);
    1274:	02e8d6bb          	divuw	a3,a7,a4
        buf[i--] = digits[x % base];
    1278:	00f10b23          	sb	a5,22(sp)
    } while ((x /= base) != 0);
    127c:	ece8f8e3          	bgeu	a7,a4,114c <printint.constprop.0+0x4e>
        buf[i--] = '-';
    1280:	02d00793          	li	a5,45
    1284:	00f10aa3          	sb	a5,21(sp)
        buf[i--] = digits[x % base];
    1288:	45b5                	li	a1,13
    128a:	bf69                	j	1224 <printint.constprop.0+0x126>
    128c:	45a9                	li	a1,10
    if (sign)
    128e:	f8055be3          	bgez	a0,1224 <printint.constprop.0+0x126>
        buf[i--] = '-';
    1292:	02d00793          	li	a5,45
    1296:	00f108a3          	sb	a5,17(sp)
        buf[i--] = digits[x % base];
    129a:	45a5                	li	a1,9
    129c:	b761                	j	1224 <printint.constprop.0+0x126>
    129e:	45b5                	li	a1,13
    if (sign)
    12a0:	f80552e3          	bgez	a0,1224 <printint.constprop.0+0x126>
        buf[i--] = '-';
    12a4:	02d00793          	li	a5,45
    12a8:	00f10a23          	sb	a5,20(sp)
        buf[i--] = digits[x % base];
    12ac:	45b1                	li	a1,12
    12ae:	bf9d                	j	1224 <printint.constprop.0+0x126>
    12b0:	45b1                	li	a1,12
    if (sign)
    12b2:	f60559e3          	bgez	a0,1224 <printint.constprop.0+0x126>
        buf[i--] = '-';
    12b6:	02d00793          	li	a5,45
    12ba:	00f109a3          	sb	a5,19(sp)
        buf[i--] = digits[x % base];
    12be:	45ad                	li	a1,11
    12c0:	b795                	j	1224 <printint.constprop.0+0x126>
    12c2:	45ad                	li	a1,11
    if (sign)
    12c4:	f60550e3          	bgez	a0,1224 <printint.constprop.0+0x126>
        buf[i--] = '-';
    12c8:	02d00793          	li	a5,45
    12cc:	00f10923          	sb	a5,18(sp)
        buf[i--] = digits[x % base];
    12d0:	45a9                	li	a1,10
    12d2:	bf89                	j	1224 <printint.constprop.0+0x126>
        buf[i--] = '-';
    12d4:	02d00793          	li	a5,45
    12d8:	00f10b23          	sb	a5,22(sp)
        buf[i--] = digits[x % base];
    12dc:	45b9                	li	a1,14
    12de:	b799                	j	1224 <printint.constprop.0+0x126>
    12e0:	45a5                	li	a1,9
    if (sign)
    12e2:	f40551e3          	bgez	a0,1224 <printint.constprop.0+0x126>
        buf[i--] = '-';
    12e6:	02d00793          	li	a5,45
    12ea:	00f10823          	sb	a5,16(sp)
        buf[i--] = digits[x % base];
    12ee:	45a1                	li	a1,8
    12f0:	bf15                	j	1224 <printint.constprop.0+0x126>
    i = 15;
    12f2:	45bd                	li	a1,15
    12f4:	bf05                	j	1224 <printint.constprop.0+0x126>
        buf[i--] = digits[x % base];
    12f6:	45a1                	li	a1,8
    if (sign)
    12f8:	f20556e3          	bgez	a0,1224 <printint.constprop.0+0x126>
        buf[i--] = '-';
    12fc:	02d00793          	li	a5,45
    1300:	00f107a3          	sb	a5,15(sp)
        buf[i--] = digits[x % base];
    1304:	459d                	li	a1,7
    1306:	bf39                	j	1224 <printint.constprop.0+0x126>
    1308:	459d                	li	a1,7
    if (sign)
    130a:	f0055de3          	bgez	a0,1224 <printint.constprop.0+0x126>
        buf[i--] = '-';
    130e:	02d00793          	li	a5,45
    1312:	00f10723          	sb	a5,14(sp)
        buf[i--] = digits[x % base];
    1316:	4599                	li	a1,6
    1318:	b731                	j	1224 <printint.constprop.0+0x126>

000000000000131a <getchar>:
{
    131a:	1101                	addi	sp,sp,-32
    read(stdin, &byte, 1);
    131c:	00f10593          	addi	a1,sp,15
    1320:	4605                	li	a2,1
    1322:	4501                	li	a0,0
{
    1324:	ec06                	sd	ra,24(sp)
    char byte = 0;
    1326:	000107a3          	sb	zero,15(sp)
    read(stdin, &byte, 1);
    132a:	18b000ef          	jal	ra,1cb4 <read>
}
    132e:	60e2                	ld	ra,24(sp)
    1330:	00f14503          	lbu	a0,15(sp)
    1334:	6105                	addi	sp,sp,32
    1336:	8082                	ret

0000000000001338 <putchar>:
{
    1338:	1101                	addi	sp,sp,-32
    133a:	87aa                	mv	a5,a0
    return write(stdout, &byte, 1);
    133c:	00f10593          	addi	a1,sp,15
    1340:	4605                	li	a2,1
    1342:	4505                	li	a0,1
{
    1344:	ec06                	sd	ra,24(sp)
    char byte = c;
    1346:	00f107a3          	sb	a5,15(sp)
    return write(stdout, &byte, 1);
    134a:	175000ef          	jal	ra,1cbe <write>
}
    134e:	60e2                	ld	ra,24(sp)
    1350:	2501                	sext.w	a0,a0
    1352:	6105                	addi	sp,sp,32
    1354:	8082                	ret

0000000000001356 <puts>:
{
    1356:	1141                	addi	sp,sp,-16
    1358:	e406                	sd	ra,8(sp)
    135a:	e022                	sd	s0,0(sp)
    135c:	842a                	mv	s0,a0
    r = -(write(stdout, s, strlen(s)) < 0);
    135e:	57c000ef          	jal	ra,18da <strlen>
    1362:	862a                	mv	a2,a0
    1364:	85a2                	mv	a1,s0
    1366:	4505                	li	a0,1
    1368:	157000ef          	jal	ra,1cbe <write>
}
    136c:	60a2                	ld	ra,8(sp)
    136e:	6402                	ld	s0,0(sp)
    r = -(write(stdout, s, strlen(s)) < 0);
    1370:	957d                	srai	a0,a0,0x3f
    return r;
    1372:	2501                	sext.w	a0,a0
}
    1374:	0141                	addi	sp,sp,16
    1376:	8082                	ret

0000000000001378 <printf>:
    out(stdout, buf, i);
}

// Print to the console. only understands %d, %x, %p, %s.
void printf(const char *fmt, ...)
{
    1378:	7171                	addi	sp,sp,-176
    137a:	fc56                	sd	s5,56(sp)
    137c:	ed3e                	sd	a5,152(sp)
    buf[i++] = '0';
    137e:	7ae1                	lui	s5,0xffff8
    va_list ap;
    int cnt = 0, l = 0;
    char *a, *z, *s = (char *)fmt, str;
    int f = stdout;

    va_start(ap, fmt);
    1380:	18bc                	addi	a5,sp,120
{
    1382:	e8ca                	sd	s2,80(sp)
    1384:	e4ce                	sd	s3,72(sp)
    1386:	e0d2                	sd	s4,64(sp)
    1388:	f85a                	sd	s6,48(sp)
    138a:	f486                	sd	ra,104(sp)
    138c:	f0a2                	sd	s0,96(sp)
    138e:	eca6                	sd	s1,88(sp)
    1390:	fcae                	sd	a1,120(sp)
    1392:	e132                	sd	a2,128(sp)
    1394:	e536                	sd	a3,136(sp)
    1396:	e93a                	sd	a4,144(sp)
    1398:	f142                	sd	a6,160(sp)
    139a:	f546                	sd	a7,168(sp)
    va_start(ap, fmt);
    139c:	e03e                	sd	a5,0(sp)
    for (;;)
    {
        if (!*s)
            break;
        for (a = s; *s && *s != '%'; s++)
    139e:	02500913          	li	s2,37
        out(f, a, l);
        if (l)
            continue;
        if (s[1] == 0)
            break;
        switch (s[1])
    13a2:	07300a13          	li	s4,115
        case 'p':
            printptr(va_arg(ap, uint64));
            break;
        case 's':
            if ((a = va_arg(ap, char *)) == 0)
                a = "(null)";
    13a6:	00001b17          	auipc	s6,0x1
    13aa:	c12b0b13          	addi	s6,s6,-1006 # 1fb8 <__clone+0xc6>
    buf[i++] = '0';
    13ae:	830aca93          	xori	s5,s5,-2000
        buf[i++] = digits[x >> (sizeof(uint64) * 8 - 4)];
    13b2:	00001997          	auipc	s3,0x1
    13b6:	e3698993          	addi	s3,s3,-458 # 21e8 <digits>
        if (!*s)
    13ba:	00054783          	lbu	a5,0(a0)
    13be:	16078a63          	beqz	a5,1532 <printf+0x1ba>
    13c2:	862a                	mv	a2,a0
        for (a = s; *s && *s != '%'; s++)
    13c4:	19278163          	beq	a5,s2,1546 <printf+0x1ce>
    13c8:	00164783          	lbu	a5,1(a2)
    13cc:	0605                	addi	a2,a2,1
    13ce:	fbfd                	bnez	a5,13c4 <printf+0x4c>
    13d0:	84b2                	mv	s1,a2
        l = z - a;
    13d2:	40a6043b          	subw	s0,a2,a0
    write(f, s, l);
    13d6:	85aa                	mv	a1,a0
    13d8:	8622                	mv	a2,s0
    13da:	4505                	li	a0,1
    13dc:	0e3000ef          	jal	ra,1cbe <write>
        if (l)
    13e0:	18041c63          	bnez	s0,1578 <printf+0x200>
        if (s[1] == 0)
    13e4:	0014c783          	lbu	a5,1(s1)
    13e8:	14078563          	beqz	a5,1532 <printf+0x1ba>
        switch (s[1])
    13ec:	1d478063          	beq	a5,s4,15ac <printf+0x234>
    13f0:	18fa6663          	bltu	s4,a5,157c <printf+0x204>
    13f4:	06400713          	li	a4,100
    13f8:	1ae78063          	beq	a5,a4,1598 <printf+0x220>
    13fc:	07000713          	li	a4,112
    1400:	1ce79963          	bne	a5,a4,15d2 <printf+0x25a>
            printptr(va_arg(ap, uint64));
    1404:	6702                	ld	a4,0(sp)
    buf[i++] = '0';
    1406:	01511423          	sh	s5,8(sp)
    write(f, s, l);
    140a:	4649                	li	a2,18
            printptr(va_arg(ap, uint64));
    140c:	631c                	ld	a5,0(a4)
    140e:	0721                	addi	a4,a4,8
    1410:	e03a                	sd	a4,0(sp)
    for (j = 0; j < (sizeof(uint64) * 2); j++, x <<= 4)
    1412:	00479293          	slli	t0,a5,0x4
    1416:	00879f93          	slli	t6,a5,0x8
    141a:	00c79f13          	slli	t5,a5,0xc
    141e:	01079e93          	slli	t4,a5,0x10
    1422:	01479e13          	slli	t3,a5,0x14
    1426:	01879313          	slli	t1,a5,0x18
    142a:	01c79893          	slli	a7,a5,0x1c
    142e:	02479813          	slli	a6,a5,0x24
    1432:	02879513          	slli	a0,a5,0x28
    1436:	02c79593          	slli	a1,a5,0x2c
    143a:	03079693          	slli	a3,a5,0x30
    143e:	03479713          	slli	a4,a5,0x34
        buf[i++] = digits[x >> (sizeof(uint64) * 8 - 4)];
    1442:	03c7d413          	srli	s0,a5,0x3c
    1446:	01c7d39b          	srliw	t2,a5,0x1c
    144a:	03c2d293          	srli	t0,t0,0x3c
    144e:	03cfdf93          	srli	t6,t6,0x3c
    1452:	03cf5f13          	srli	t5,t5,0x3c
    1456:	03cede93          	srli	t4,t4,0x3c
    145a:	03ce5e13          	srli	t3,t3,0x3c
    145e:	03c35313          	srli	t1,t1,0x3c
    1462:	03c8d893          	srli	a7,a7,0x3c
    1466:	03c85813          	srli	a6,a6,0x3c
    146a:	9171                	srli	a0,a0,0x3c
    146c:	91f1                	srli	a1,a1,0x3c
    146e:	92f1                	srli	a3,a3,0x3c
    1470:	9371                	srli	a4,a4,0x3c
    1472:	96ce                	add	a3,a3,s3
    1474:	974e                	add	a4,a4,s3
    1476:	944e                	add	s0,s0,s3
    1478:	92ce                	add	t0,t0,s3
    147a:	9fce                	add	t6,t6,s3
    147c:	9f4e                	add	t5,t5,s3
    147e:	9ece                	add	t4,t4,s3
    1480:	9e4e                	add	t3,t3,s3
    1482:	934e                	add	t1,t1,s3
    1484:	98ce                	add	a7,a7,s3
    1486:	93ce                	add	t2,t2,s3
    1488:	984e                	add	a6,a6,s3
    148a:	954e                	add	a0,a0,s3
    148c:	95ce                	add	a1,a1,s3
    148e:	0006c083          	lbu	ra,0(a3)
    1492:	0002c283          	lbu	t0,0(t0)
    1496:	00074683          	lbu	a3,0(a4)
    149a:	000fcf83          	lbu	t6,0(t6)
    149e:	000f4f03          	lbu	t5,0(t5)
    14a2:	000ece83          	lbu	t4,0(t4)
    14a6:	000e4e03          	lbu	t3,0(t3)
    14aa:	00034303          	lbu	t1,0(t1)
    14ae:	0008c883          	lbu	a7,0(a7)
    14b2:	0003c383          	lbu	t2,0(t2)
    14b6:	00084803          	lbu	a6,0(a6)
    14ba:	00054503          	lbu	a0,0(a0)
    14be:	0005c583          	lbu	a1,0(a1)
    14c2:	00044403          	lbu	s0,0(s0)
    for (j = 0; j < (sizeof(uint64) * 2); j++, x <<= 4)
    14c6:	03879713          	slli	a4,a5,0x38
        buf[i++] = digits[x >> (sizeof(uint64) * 8 - 4)];
    14ca:	9371                	srli	a4,a4,0x3c
    14cc:	8bbd                	andi	a5,a5,15
    14ce:	974e                	add	a4,a4,s3
    14d0:	97ce                	add	a5,a5,s3
    14d2:	005105a3          	sb	t0,11(sp)
    14d6:	01f10623          	sb	t6,12(sp)
    14da:	01e106a3          	sb	t5,13(sp)
    14de:	01d10723          	sb	t4,14(sp)
    14e2:	01c107a3          	sb	t3,15(sp)
    14e6:	00610823          	sb	t1,16(sp)
    14ea:	011108a3          	sb	a7,17(sp)
    14ee:	00710923          	sb	t2,18(sp)
    14f2:	010109a3          	sb	a6,19(sp)
    14f6:	00a10a23          	sb	a0,20(sp)
    14fa:	00b10aa3          	sb	a1,21(sp)
    14fe:	00110b23          	sb	ra,22(sp)
    1502:	00d10ba3          	sb	a3,23(sp)
    1506:	00810523          	sb	s0,10(sp)
    150a:	00074703          	lbu	a4,0(a4)
    150e:	0007c783          	lbu	a5,0(a5)
    write(f, s, l);
    1512:	002c                	addi	a1,sp,8
    1514:	4505                	li	a0,1
        buf[i++] = digits[x >> (sizeof(uint64) * 8 - 4)];
    1516:	00e10c23          	sb	a4,24(sp)
    151a:	00f10ca3          	sb	a5,25(sp)
    buf[i] = 0;
    151e:	00010d23          	sb	zero,26(sp)
    write(f, s, l);
    1522:	79c000ef          	jal	ra,1cbe <write>
            // Print unknown % sequence to draw attention.
            putchar('%');
            putchar(s[1]);
            break;
        }
        s += 2;
    1526:	00248513          	addi	a0,s1,2
        if (!*s)
    152a:	00054783          	lbu	a5,0(a0)
    152e:	e8079ae3          	bnez	a5,13c2 <printf+0x4a>
    }
    va_end(ap);
}
    1532:	70a6                	ld	ra,104(sp)
    1534:	7406                	ld	s0,96(sp)
    1536:	64e6                	ld	s1,88(sp)
    1538:	6946                	ld	s2,80(sp)
    153a:	69a6                	ld	s3,72(sp)
    153c:	6a06                	ld	s4,64(sp)
    153e:	7ae2                	ld	s5,56(sp)
    1540:	7b42                	ld	s6,48(sp)
    1542:	614d                	addi	sp,sp,176
    1544:	8082                	ret
        for (z = s; s[0] == '%' && s[1] == '%'; z++, s += 2)
    1546:	00064783          	lbu	a5,0(a2)
    154a:	84b2                	mv	s1,a2
    154c:	01278963          	beq	a5,s2,155e <printf+0x1e6>
    1550:	b549                	j	13d2 <printf+0x5a>
    1552:	0024c783          	lbu	a5,2(s1)
    1556:	0605                	addi	a2,a2,1
    1558:	0489                	addi	s1,s1,2
    155a:	e7279ce3          	bne	a5,s2,13d2 <printf+0x5a>
    155e:	0014c783          	lbu	a5,1(s1)
    1562:	ff2788e3          	beq	a5,s2,1552 <printf+0x1da>
        l = z - a;
    1566:	40a6043b          	subw	s0,a2,a0
    write(f, s, l);
    156a:	85aa                	mv	a1,a0
    156c:	8622                	mv	a2,s0
    156e:	4505                	li	a0,1
    1570:	74e000ef          	jal	ra,1cbe <write>
        if (l)
    1574:	e60408e3          	beqz	s0,13e4 <printf+0x6c>
    1578:	8526                	mv	a0,s1
    157a:	b581                	j	13ba <printf+0x42>
        switch (s[1])
    157c:	07800713          	li	a4,120
    1580:	04e79963          	bne	a5,a4,15d2 <printf+0x25a>
            printint(va_arg(ap, int), 16, 1);
    1584:	6782                	ld	a5,0(sp)
    1586:	45c1                	li	a1,16
    1588:	4388                	lw	a0,0(a5)
    158a:	07a1                	addi	a5,a5,8
    158c:	e03e                	sd	a5,0(sp)
    158e:	b71ff0ef          	jal	ra,10fe <printint.constprop.0>
        s += 2;
    1592:	00248513          	addi	a0,s1,2
    1596:	bf51                	j	152a <printf+0x1b2>
            printint(va_arg(ap, int), 10, 1);
    1598:	6782                	ld	a5,0(sp)
    159a:	45a9                	li	a1,10
    159c:	4388                	lw	a0,0(a5)
    159e:	07a1                	addi	a5,a5,8
    15a0:	e03e                	sd	a5,0(sp)
    15a2:	b5dff0ef          	jal	ra,10fe <printint.constprop.0>
        s += 2;
    15a6:	00248513          	addi	a0,s1,2
    15aa:	b741                	j	152a <printf+0x1b2>
            if ((a = va_arg(ap, char *)) == 0)
    15ac:	6782                	ld	a5,0(sp)
    15ae:	6380                	ld	s0,0(a5)
    15b0:	07a1                	addi	a5,a5,8
    15b2:	e03e                	sd	a5,0(sp)
    15b4:	c031                	beqz	s0,15f8 <printf+0x280>
            l = strnlen(a, 200);
    15b6:	0c800593          	li	a1,200
    15ba:	8522                	mv	a0,s0
    15bc:	40a000ef          	jal	ra,19c6 <strnlen>
    write(f, s, l);
    15c0:	0005061b          	sext.w	a2,a0
    15c4:	85a2                	mv	a1,s0
    15c6:	4505                	li	a0,1
    15c8:	6f6000ef          	jal	ra,1cbe <write>
        s += 2;
    15cc:	00248513          	addi	a0,s1,2
    15d0:	bfa9                	j	152a <printf+0x1b2>
    return write(stdout, &byte, 1);
    15d2:	4605                	li	a2,1
    15d4:	002c                	addi	a1,sp,8
    15d6:	4505                	li	a0,1
    char byte = c;
    15d8:	01210423          	sb	s2,8(sp)
    return write(stdout, &byte, 1);
    15dc:	6e2000ef          	jal	ra,1cbe <write>
    char byte = c;
    15e0:	0014c783          	lbu	a5,1(s1)
    return write(stdout, &byte, 1);
    15e4:	4605                	li	a2,1
    15e6:	002c                	addi	a1,sp,8
    15e8:	4505                	li	a0,1
    char byte = c;
    15ea:	00f10423          	sb	a5,8(sp)
    return write(stdout, &byte, 1);
    15ee:	6d0000ef          	jal	ra,1cbe <write>
        s += 2;
    15f2:	00248513          	addi	a0,s1,2
    15f6:	bf15                	j	152a <printf+0x1b2>
                a = "(null)";
    15f8:	845a                	mv	s0,s6
    15fa:	bf75                	j	15b6 <printf+0x23e>

00000000000015fc <panic>:
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

void panic(char *m)
{
    15fc:	1141                	addi	sp,sp,-16
    15fe:	e406                	sd	ra,8(sp)
    puts(m);
    1600:	d57ff0ef          	jal	ra,1356 <puts>
    exit(-100);
}
    1604:	60a2                	ld	ra,8(sp)
    exit(-100);
    1606:	f9c00513          	li	a0,-100
}
    160a:	0141                	addi	sp,sp,16
    exit(-100);
    160c:	a709                	j	1d0e <exit>

000000000000160e <isspace>:
#define HIGHS (ONES * (UCHAR_MAX / 2 + 1))
#define HASZERO(x) (((x)-ONES) & ~(x)&HIGHS)

int isspace(int c)
{
    return c == ' ' || (unsigned)c - '\t' < 5;
    160e:	02000793          	li	a5,32
    1612:	00f50663          	beq	a0,a5,161e <isspace+0x10>
    1616:	355d                	addiw	a0,a0,-9
    1618:	00553513          	sltiu	a0,a0,5
    161c:	8082                	ret
    161e:	4505                	li	a0,1
}
    1620:	8082                	ret

0000000000001622 <isdigit>:

int isdigit(int c)
{
    return (unsigned)c - '0' < 10;
    1622:	fd05051b          	addiw	a0,a0,-48
}
    1626:	00a53513          	sltiu	a0,a0,10
    162a:	8082                	ret

000000000000162c <atoi>:
    return c == ' ' || (unsigned)c - '\t' < 5;
    162c:	02000613          	li	a2,32
    1630:	4591                	li	a1,4

int atoi(const char *s)
{
    int n = 0, neg = 0;
    while (isspace(*s))
    1632:	00054703          	lbu	a4,0(a0)
    return c == ' ' || (unsigned)c - '\t' < 5;
    1636:	ff77069b          	addiw	a3,a4,-9
    163a:	04c70d63          	beq	a4,a2,1694 <atoi+0x68>
    163e:	0007079b          	sext.w	a5,a4
    1642:	04d5f963          	bgeu	a1,a3,1694 <atoi+0x68>
        s++;
    switch (*s)
    1646:	02b00693          	li	a3,43
    164a:	04d70a63          	beq	a4,a3,169e <atoi+0x72>
    164e:	02d00693          	li	a3,45
    1652:	06d70463          	beq	a4,a3,16ba <atoi+0x8e>
        neg = 1;
    case '+':
        s++;
    }
    /* Compute n as a negative number to avoid overflow on INT_MIN */
    while (isdigit(*s))
    1656:	fd07859b          	addiw	a1,a5,-48
    165a:	4625                	li	a2,9
    165c:	873e                	mv	a4,a5
    165e:	86aa                	mv	a3,a0
    int n = 0, neg = 0;
    1660:	4e01                	li	t3,0
    while (isdigit(*s))
    1662:	04b66a63          	bltu	a2,a1,16b6 <atoi+0x8a>
    int n = 0, neg = 0;
    1666:	4501                	li	a0,0
    while (isdigit(*s))
    1668:	4825                	li	a6,9
    166a:	0016c603          	lbu	a2,1(a3)
        n = 10 * n - (*s++ - '0');
    166e:	0025179b          	slliw	a5,a0,0x2
    1672:	9d3d                	addw	a0,a0,a5
    1674:	fd07031b          	addiw	t1,a4,-48
    1678:	0015189b          	slliw	a7,a0,0x1
    while (isdigit(*s))
    167c:	fd06059b          	addiw	a1,a2,-48
        n = 10 * n - (*s++ - '0');
    1680:	0685                	addi	a3,a3,1
    1682:	4068853b          	subw	a0,a7,t1
    while (isdigit(*s))
    1686:	0006071b          	sext.w	a4,a2
    168a:	feb870e3          	bgeu	a6,a1,166a <atoi+0x3e>
    return neg ? n : -n;
    168e:	000e0563          	beqz	t3,1698 <atoi+0x6c>
}
    1692:	8082                	ret
        s++;
    1694:	0505                	addi	a0,a0,1
    1696:	bf71                	j	1632 <atoi+0x6>
    return neg ? n : -n;
    1698:	4113053b          	subw	a0,t1,a7
    169c:	8082                	ret
    while (isdigit(*s))
    169e:	00154783          	lbu	a5,1(a0)
    16a2:	4625                	li	a2,9
        s++;
    16a4:	00150693          	addi	a3,a0,1
    while (isdigit(*s))
    16a8:	fd07859b          	addiw	a1,a5,-48
    16ac:	0007871b          	sext.w	a4,a5
    int n = 0, neg = 0;
    16b0:	4e01                	li	t3,0
    while (isdigit(*s))
    16b2:	fab67ae3          	bgeu	a2,a1,1666 <atoi+0x3a>
    16b6:	4501                	li	a0,0
}
    16b8:	8082                	ret
    while (isdigit(*s))
    16ba:	00154783          	lbu	a5,1(a0)
    16be:	4625                	li	a2,9
        s++;
    16c0:	00150693          	addi	a3,a0,1
    while (isdigit(*s))
    16c4:	fd07859b          	addiw	a1,a5,-48
    16c8:	0007871b          	sext.w	a4,a5
    16cc:	feb665e3          	bltu	a2,a1,16b6 <atoi+0x8a>
        neg = 1;
    16d0:	4e05                	li	t3,1
    16d2:	bf51                	j	1666 <atoi+0x3a>

00000000000016d4 <memset>:

void *memset(void *dest, int c, size_t n)
{
    char *p = dest;
    for (int i = 0; i < n; ++i, *(p++) = c)
    16d4:	16060d63          	beqz	a2,184e <memset+0x17a>
    16d8:	40a007b3          	neg	a5,a0
    16dc:	8b9d                	andi	a5,a5,7
    16de:	00778713          	addi	a4,a5,7
    16e2:	482d                	li	a6,11
    16e4:	0ff5f593          	andi	a1,a1,255
    16e8:	fff60693          	addi	a3,a2,-1
    16ec:	17076263          	bltu	a4,a6,1850 <memset+0x17c>
    16f0:	16e6ea63          	bltu	a3,a4,1864 <memset+0x190>
    16f4:	16078563          	beqz	a5,185e <memset+0x18a>
    16f8:	00b50023          	sb	a1,0(a0)
    16fc:	4705                	li	a4,1
    16fe:	00150e93          	addi	t4,a0,1
    1702:	14e78c63          	beq	a5,a4,185a <memset+0x186>
    1706:	00b500a3          	sb	a1,1(a0)
    170a:	4709                	li	a4,2
    170c:	00250e93          	addi	t4,a0,2
    1710:	14e78d63          	beq	a5,a4,186a <memset+0x196>
    1714:	00b50123          	sb	a1,2(a0)
    1718:	470d                	li	a4,3
    171a:	00350e93          	addi	t4,a0,3
    171e:	12e78b63          	beq	a5,a4,1854 <memset+0x180>
    1722:	00b501a3          	sb	a1,3(a0)
    1726:	4711                	li	a4,4
    1728:	00450e93          	addi	t4,a0,4
    172c:	14e78163          	beq	a5,a4,186e <memset+0x19a>
    1730:	00b50223          	sb	a1,4(a0)
    1734:	4715                	li	a4,5
    1736:	00550e93          	addi	t4,a0,5
    173a:	12e78c63          	beq	a5,a4,1872 <memset+0x19e>
    173e:	00b502a3          	sb	a1,5(a0)
    1742:	471d                	li	a4,7
    1744:	00650e93          	addi	t4,a0,6
    1748:	12e79763          	bne	a5,a4,1876 <memset+0x1a2>
    174c:	00750e93          	addi	t4,a0,7
    1750:	00b50323          	sb	a1,6(a0)
    1754:	4f1d                	li	t5,7
    1756:	00859713          	slli	a4,a1,0x8
    175a:	8f4d                	or	a4,a4,a1
    175c:	01059e13          	slli	t3,a1,0x10
    1760:	01c76e33          	or	t3,a4,t3
    1764:	01859313          	slli	t1,a1,0x18
    1768:	006e6333          	or	t1,t3,t1
    176c:	02059893          	slli	a7,a1,0x20
    1770:	011368b3          	or	a7,t1,a7
    1774:	02859813          	slli	a6,a1,0x28
    1778:	40f60333          	sub	t1,a2,a5
    177c:	0108e833          	or	a6,a7,a6
    1780:	03059693          	slli	a3,a1,0x30
    1784:	00d866b3          	or	a3,a6,a3
    1788:	03859713          	slli	a4,a1,0x38
    178c:	97aa                	add	a5,a5,a0
    178e:	ff837813          	andi	a6,t1,-8
    1792:	8f55                	or	a4,a4,a3
    1794:	00f806b3          	add	a3,a6,a5
    1798:	e398                	sd	a4,0(a5)
    179a:	07a1                	addi	a5,a5,8
    179c:	fed79ee3          	bne	a5,a3,1798 <memset+0xc4>
    17a0:	ff837693          	andi	a3,t1,-8
    17a4:	00de87b3          	add	a5,t4,a3
    17a8:	01e6873b          	addw	a4,a3,t5
    17ac:	0ad30663          	beq	t1,a3,1858 <memset+0x184>
    17b0:	00b78023          	sb	a1,0(a5)
    17b4:	0017069b          	addiw	a3,a4,1
    17b8:	08c6fb63          	bgeu	a3,a2,184e <memset+0x17a>
    17bc:	00b780a3          	sb	a1,1(a5)
    17c0:	0027069b          	addiw	a3,a4,2
    17c4:	08c6f563          	bgeu	a3,a2,184e <memset+0x17a>
    17c8:	00b78123          	sb	a1,2(a5)
    17cc:	0037069b          	addiw	a3,a4,3
    17d0:	06c6ff63          	bgeu	a3,a2,184e <memset+0x17a>
    17d4:	00b781a3          	sb	a1,3(a5)
    17d8:	0047069b          	addiw	a3,a4,4
    17dc:	06c6f963          	bgeu	a3,a2,184e <memset+0x17a>
    17e0:	00b78223          	sb	a1,4(a5)
    17e4:	0057069b          	addiw	a3,a4,5
    17e8:	06c6f363          	bgeu	a3,a2,184e <memset+0x17a>
    17ec:	00b782a3          	sb	a1,5(a5)
    17f0:	0067069b          	addiw	a3,a4,6
    17f4:	04c6fd63          	bgeu	a3,a2,184e <memset+0x17a>
    17f8:	00b78323          	sb	a1,6(a5)
    17fc:	0077069b          	addiw	a3,a4,7
    1800:	04c6f763          	bgeu	a3,a2,184e <memset+0x17a>
    1804:	00b783a3          	sb	a1,7(a5)
    1808:	0087069b          	addiw	a3,a4,8
    180c:	04c6f163          	bgeu	a3,a2,184e <memset+0x17a>
    1810:	00b78423          	sb	a1,8(a5)
    1814:	0097069b          	addiw	a3,a4,9
    1818:	02c6fb63          	bgeu	a3,a2,184e <memset+0x17a>
    181c:	00b784a3          	sb	a1,9(a5)
    1820:	00a7069b          	addiw	a3,a4,10
    1824:	02c6f563          	bgeu	a3,a2,184e <memset+0x17a>
    1828:	00b78523          	sb	a1,10(a5)
    182c:	00b7069b          	addiw	a3,a4,11
    1830:	00c6ff63          	bgeu	a3,a2,184e <memset+0x17a>
    1834:	00b785a3          	sb	a1,11(a5)
    1838:	00c7069b          	addiw	a3,a4,12
    183c:	00c6f963          	bgeu	a3,a2,184e <memset+0x17a>
    1840:	00b78623          	sb	a1,12(a5)
    1844:	2735                	addiw	a4,a4,13
    1846:	00c77463          	bgeu	a4,a2,184e <memset+0x17a>
    184a:	00b786a3          	sb	a1,13(a5)
        ;
    return dest;
}
    184e:	8082                	ret
    1850:	472d                	li	a4,11
    1852:	bd79                	j	16f0 <memset+0x1c>
    for (int i = 0; i < n; ++i, *(p++) = c)
    1854:	4f0d                	li	t5,3
    1856:	b701                	j	1756 <memset+0x82>
    1858:	8082                	ret
    185a:	4f05                	li	t5,1
    185c:	bded                	j	1756 <memset+0x82>
    185e:	8eaa                	mv	t4,a0
    1860:	4f01                	li	t5,0
    1862:	bdd5                	j	1756 <memset+0x82>
    1864:	87aa                	mv	a5,a0
    1866:	4701                	li	a4,0
    1868:	b7a1                	j	17b0 <memset+0xdc>
    186a:	4f09                	li	t5,2
    186c:	b5ed                	j	1756 <memset+0x82>
    186e:	4f11                	li	t5,4
    1870:	b5dd                	j	1756 <memset+0x82>
    1872:	4f15                	li	t5,5
    1874:	b5cd                	j	1756 <memset+0x82>
    1876:	4f19                	li	t5,6
    1878:	bdf9                	j	1756 <memset+0x82>

000000000000187a <strcmp>:

int strcmp(const char *l, const char *r)
{
    for (; *l == *r && *l; l++, r++)
    187a:	00054783          	lbu	a5,0(a0)
    187e:	0005c703          	lbu	a4,0(a1)
    1882:	00e79863          	bne	a5,a4,1892 <strcmp+0x18>
    1886:	0505                	addi	a0,a0,1
    1888:	0585                	addi	a1,a1,1
    188a:	fbe5                	bnez	a5,187a <strcmp>
    188c:	4501                	li	a0,0
        ;
    return *(unsigned char *)l - *(unsigned char *)r;
}
    188e:	9d19                	subw	a0,a0,a4
    1890:	8082                	ret
    return *(unsigned char *)l - *(unsigned char *)r;
    1892:	0007851b          	sext.w	a0,a5
    1896:	bfe5                	j	188e <strcmp+0x14>

0000000000001898 <strncmp>:

int strncmp(const char *_l, const char *_r, size_t n)
{
    const unsigned char *l = (void *)_l, *r = (void *)_r;
    if (!n--)
    1898:	ce05                	beqz	a2,18d0 <strncmp+0x38>
        return 0;
    for (; *l && *r && n && *l == *r; l++, r++, n--)
    189a:	00054703          	lbu	a4,0(a0)
    189e:	0005c783          	lbu	a5,0(a1)
    18a2:	cb0d                	beqz	a4,18d4 <strncmp+0x3c>
    if (!n--)
    18a4:	167d                	addi	a2,a2,-1
    18a6:	00c506b3          	add	a3,a0,a2
    18aa:	a819                	j	18c0 <strncmp+0x28>
    for (; *l && *r && n && *l == *r; l++, r++, n--)
    18ac:	00a68e63          	beq	a3,a0,18c8 <strncmp+0x30>
    18b0:	0505                	addi	a0,a0,1
    18b2:	00e79b63          	bne	a5,a4,18c8 <strncmp+0x30>
    18b6:	00054703          	lbu	a4,0(a0)
        ;
    return *l - *r;
    18ba:	0005c783          	lbu	a5,0(a1)
    for (; *l && *r && n && *l == *r; l++, r++, n--)
    18be:	cb19                	beqz	a4,18d4 <strncmp+0x3c>
    18c0:	0005c783          	lbu	a5,0(a1)
    18c4:	0585                	addi	a1,a1,1
    18c6:	f3fd                	bnez	a5,18ac <strncmp+0x14>
    return *l - *r;
    18c8:	0007051b          	sext.w	a0,a4
    18cc:	9d1d                	subw	a0,a0,a5
    18ce:	8082                	ret
        return 0;
    18d0:	4501                	li	a0,0
}
    18d2:	8082                	ret
    18d4:	4501                	li	a0,0
    return *l - *r;
    18d6:	9d1d                	subw	a0,a0,a5
    18d8:	8082                	ret

00000000000018da <strlen>:
size_t strlen(const char *s)
{
    const char *a = s;
    typedef size_t __attribute__((__may_alias__)) word;
    const word *w;
    for (; (uintptr_t)s % SS; s++)
    18da:	00757793          	andi	a5,a0,7
    18de:	cf89                	beqz	a5,18f8 <strlen+0x1e>
    18e0:	87aa                	mv	a5,a0
    18e2:	a029                	j	18ec <strlen+0x12>
    18e4:	0785                	addi	a5,a5,1
    18e6:	0077f713          	andi	a4,a5,7
    18ea:	cb01                	beqz	a4,18fa <strlen+0x20>
        if (!*s)
    18ec:	0007c703          	lbu	a4,0(a5)
    18f0:	fb75                	bnez	a4,18e4 <strlen+0xa>
    for (w = (const void *)s; !HASZERO(*w); w++)
        ;
    s = (const void *)w;
    for (; *s; s++)
        ;
    return s - a;
    18f2:	40a78533          	sub	a0,a5,a0
}
    18f6:	8082                	ret
    for (; (uintptr_t)s % SS; s++)
    18f8:	87aa                	mv	a5,a0
    for (w = (const void *)s; !HASZERO(*w); w++)
    18fa:	6394                	ld	a3,0(a5)
    18fc:	00000597          	auipc	a1,0x0
    1900:	6c45b583          	ld	a1,1732(a1) # 1fc0 <__clone+0xce>
    1904:	00000617          	auipc	a2,0x0
    1908:	6c463603          	ld	a2,1732(a2) # 1fc8 <__clone+0xd6>
    190c:	a019                	j	1912 <strlen+0x38>
    190e:	6794                	ld	a3,8(a5)
    1910:	07a1                	addi	a5,a5,8
    1912:	00b68733          	add	a4,a3,a1
    1916:	fff6c693          	not	a3,a3
    191a:	8f75                	and	a4,a4,a3
    191c:	8f71                	and	a4,a4,a2
    191e:	db65                	beqz	a4,190e <strlen+0x34>
    for (; *s; s++)
    1920:	0007c703          	lbu	a4,0(a5)
    1924:	d779                	beqz	a4,18f2 <strlen+0x18>
    1926:	0017c703          	lbu	a4,1(a5)
    192a:	0785                	addi	a5,a5,1
    192c:	d379                	beqz	a4,18f2 <strlen+0x18>
    192e:	0017c703          	lbu	a4,1(a5)
    1932:	0785                	addi	a5,a5,1
    1934:	fb6d                	bnez	a4,1926 <strlen+0x4c>
    1936:	bf75                	j	18f2 <strlen+0x18>

0000000000001938 <memchr>:

void *memchr(const void *src, int c, size_t n)
{
    const unsigned char *s = src;
    c = (unsigned char)c;
    for (; ((uintptr_t)s & ALIGN) && n && *s != c; s++, n--)
    1938:	00757713          	andi	a4,a0,7
{
    193c:	87aa                	mv	a5,a0
    c = (unsigned char)c;
    193e:	0ff5f593          	andi	a1,a1,255
    for (; ((uintptr_t)s & ALIGN) && n && *s != c; s++, n--)
    1942:	cb19                	beqz	a4,1958 <memchr+0x20>
    1944:	ce25                	beqz	a2,19bc <memchr+0x84>
    1946:	0007c703          	lbu	a4,0(a5)
    194a:	04b70e63          	beq	a4,a1,19a6 <memchr+0x6e>
    194e:	0785                	addi	a5,a5,1
    1950:	0077f713          	andi	a4,a5,7
    1954:	167d                	addi	a2,a2,-1
    1956:	f77d                	bnez	a4,1944 <memchr+0xc>
            ;
        s = (const void *)w;
    }
    for (; n && *s != c; s++, n--)
        ;
    return n ? (void *)s : 0;
    1958:	4501                	li	a0,0
    if (n && *s != c)
    195a:	c235                	beqz	a2,19be <memchr+0x86>
    195c:	0007c703          	lbu	a4,0(a5)
    1960:	04b70363          	beq	a4,a1,19a6 <memchr+0x6e>
        size_t k = ONES * c;
    1964:	00000517          	auipc	a0,0x0
    1968:	66c53503          	ld	a0,1644(a0) # 1fd0 <__clone+0xde>
        for (w = (const void *)s; n >= SS && !HASZERO(*w ^ k); w++, n -= SS)
    196c:	471d                	li	a4,7
        size_t k = ONES * c;
    196e:	02a58533          	mul	a0,a1,a0
        for (w = (const void *)s; n >= SS && !HASZERO(*w ^ k); w++, n -= SS)
    1972:	02c77a63          	bgeu	a4,a2,19a6 <memchr+0x6e>
    1976:	00000897          	auipc	a7,0x0
    197a:	64a8b883          	ld	a7,1610(a7) # 1fc0 <__clone+0xce>
    197e:	00000817          	auipc	a6,0x0
    1982:	64a83803          	ld	a6,1610(a6) # 1fc8 <__clone+0xd6>
    1986:	431d                	li	t1,7
    1988:	a029                	j	1992 <memchr+0x5a>
    198a:	1661                	addi	a2,a2,-8
    198c:	07a1                	addi	a5,a5,8
    198e:	02c37963          	bgeu	t1,a2,19c0 <memchr+0x88>
    1992:	6398                	ld	a4,0(a5)
    1994:	8f29                	xor	a4,a4,a0
    1996:	011706b3          	add	a3,a4,a7
    199a:	fff74713          	not	a4,a4
    199e:	8f75                	and	a4,a4,a3
    19a0:	01077733          	and	a4,a4,a6
    19a4:	d37d                	beqz	a4,198a <memchr+0x52>
    19a6:	853e                	mv	a0,a5
    19a8:	97b2                	add	a5,a5,a2
    19aa:	a021                	j	19b2 <memchr+0x7a>
    for (; n && *s != c; s++, n--)
    19ac:	0505                	addi	a0,a0,1
    19ae:	00f50763          	beq	a0,a5,19bc <memchr+0x84>
    19b2:	00054703          	lbu	a4,0(a0)
    19b6:	feb71be3          	bne	a4,a1,19ac <memchr+0x74>
    19ba:	8082                	ret
    return n ? (void *)s : 0;
    19bc:	4501                	li	a0,0
}
    19be:	8082                	ret
    return n ? (void *)s : 0;
    19c0:	4501                	li	a0,0
    for (; n && *s != c; s++, n--)
    19c2:	f275                	bnez	a2,19a6 <memchr+0x6e>
}
    19c4:	8082                	ret

00000000000019c6 <strnlen>:

size_t strnlen(const char *s, size_t n)
{
    19c6:	1101                	addi	sp,sp,-32
    19c8:	e822                	sd	s0,16(sp)
    const char *p = memchr(s, 0, n);
    19ca:	862e                	mv	a2,a1
{
    19cc:	842e                	mv	s0,a1
    const char *p = memchr(s, 0, n);
    19ce:	4581                	li	a1,0
{
    19d0:	e426                	sd	s1,8(sp)
    19d2:	ec06                	sd	ra,24(sp)
    19d4:	84aa                	mv	s1,a0
    const char *p = memchr(s, 0, n);
    19d6:	f63ff0ef          	jal	ra,1938 <memchr>
    return p ? p - s : n;
    19da:	c519                	beqz	a0,19e8 <strnlen+0x22>
}
    19dc:	60e2                	ld	ra,24(sp)
    19de:	6442                	ld	s0,16(sp)
    return p ? p - s : n;
    19e0:	8d05                	sub	a0,a0,s1
}
    19e2:	64a2                	ld	s1,8(sp)
    19e4:	6105                	addi	sp,sp,32
    19e6:	8082                	ret
    19e8:	60e2                	ld	ra,24(sp)
    return p ? p - s : n;
    19ea:	8522                	mv	a0,s0
}
    19ec:	6442                	ld	s0,16(sp)
    19ee:	64a2                	ld	s1,8(sp)
    19f0:	6105                	addi	sp,sp,32
    19f2:	8082                	ret

00000000000019f4 <strcpy>:
char *strcpy(char *restrict d, const char *s)
{
    typedef size_t __attribute__((__may_alias__)) word;
    word *wd;
    const word *ws;
    if ((uintptr_t)s % SS == (uintptr_t)d % SS)
    19f4:	00b547b3          	xor	a5,a0,a1
    19f8:	8b9d                	andi	a5,a5,7
    19fa:	eb95                	bnez	a5,1a2e <strcpy+0x3a>
    {
        for (; (uintptr_t)s % SS; s++, d++)
    19fc:	0075f793          	andi	a5,a1,7
    1a00:	e7b1                	bnez	a5,1a4c <strcpy+0x58>
            if (!(*d = *s))
                return d;
        wd = (void *)d;
        ws = (const void *)s;
        for (; !HASZERO(*ws); *wd++ = *ws++)
    1a02:	6198                	ld	a4,0(a1)
    1a04:	00000617          	auipc	a2,0x0
    1a08:	5bc63603          	ld	a2,1468(a2) # 1fc0 <__clone+0xce>
    1a0c:	00000817          	auipc	a6,0x0
    1a10:	5bc83803          	ld	a6,1468(a6) # 1fc8 <__clone+0xd6>
    1a14:	a029                	j	1a1e <strcpy+0x2a>
    1a16:	e118                	sd	a4,0(a0)
    1a18:	6598                	ld	a4,8(a1)
    1a1a:	05a1                	addi	a1,a1,8
    1a1c:	0521                	addi	a0,a0,8
    1a1e:	00c707b3          	add	a5,a4,a2
    1a22:	fff74693          	not	a3,a4
    1a26:	8ff5                	and	a5,a5,a3
    1a28:	0107f7b3          	and	a5,a5,a6
    1a2c:	d7ed                	beqz	a5,1a16 <strcpy+0x22>
            ;
        d = (void *)wd;
        s = (const void *)ws;
    }
    for (; (*d = *s); s++, d++)
    1a2e:	0005c783          	lbu	a5,0(a1)
    1a32:	00f50023          	sb	a5,0(a0)
    1a36:	c785                	beqz	a5,1a5e <strcpy+0x6a>
    1a38:	0015c783          	lbu	a5,1(a1)
    1a3c:	0505                	addi	a0,a0,1
    1a3e:	0585                	addi	a1,a1,1
    1a40:	00f50023          	sb	a5,0(a0)
    1a44:	fbf5                	bnez	a5,1a38 <strcpy+0x44>
        ;
    return d;
}
    1a46:	8082                	ret
        for (; (uintptr_t)s % SS; s++, d++)
    1a48:	0505                	addi	a0,a0,1
    1a4a:	df45                	beqz	a4,1a02 <strcpy+0xe>
            if (!(*d = *s))
    1a4c:	0005c783          	lbu	a5,0(a1)
        for (; (uintptr_t)s % SS; s++, d++)
    1a50:	0585                	addi	a1,a1,1
    1a52:	0075f713          	andi	a4,a1,7
            if (!(*d = *s))
    1a56:	00f50023          	sb	a5,0(a0)
    1a5a:	f7fd                	bnez	a5,1a48 <strcpy+0x54>
}
    1a5c:	8082                	ret
    1a5e:	8082                	ret

0000000000001a60 <strncpy>:
char *strncpy(char *restrict d, const char *s, size_t n)
{
    typedef size_t __attribute__((__may_alias__)) word;
    word *wd;
    const word *ws;
    if (((uintptr_t)s & ALIGN) == ((uintptr_t)d & ALIGN))
    1a60:	00b547b3          	xor	a5,a0,a1
    1a64:	8b9d                	andi	a5,a5,7
    1a66:	1a079863          	bnez	a5,1c16 <strncpy+0x1b6>
    {
        for (; ((uintptr_t)s & ALIGN) && n && (*d = *s); n--, s++, d++)
    1a6a:	0075f793          	andi	a5,a1,7
    1a6e:	16078463          	beqz	a5,1bd6 <strncpy+0x176>
    1a72:	ea01                	bnez	a2,1a82 <strncpy+0x22>
    1a74:	a421                	j	1c7c <strncpy+0x21c>
    1a76:	167d                	addi	a2,a2,-1
    1a78:	0505                	addi	a0,a0,1
    1a7a:	14070e63          	beqz	a4,1bd6 <strncpy+0x176>
    1a7e:	1a060863          	beqz	a2,1c2e <strncpy+0x1ce>
    1a82:	0005c783          	lbu	a5,0(a1)
    1a86:	0585                	addi	a1,a1,1
    1a88:	0075f713          	andi	a4,a1,7
    1a8c:	00f50023          	sb	a5,0(a0)
    1a90:	f3fd                	bnez	a5,1a76 <strncpy+0x16>
    1a92:	4805                	li	a6,1
    1a94:	1a061863          	bnez	a2,1c44 <strncpy+0x1e4>
    1a98:	40a007b3          	neg	a5,a0
    1a9c:	8b9d                	andi	a5,a5,7
    1a9e:	4681                	li	a3,0
    1aa0:	18061a63          	bnez	a2,1c34 <strncpy+0x1d4>
    1aa4:	00778713          	addi	a4,a5,7
    1aa8:	45ad                	li	a1,11
    1aaa:	18b76363          	bltu	a4,a1,1c30 <strncpy+0x1d0>
    1aae:	1ae6eb63          	bltu	a3,a4,1c64 <strncpy+0x204>
    1ab2:	1a078363          	beqz	a5,1c58 <strncpy+0x1f8>
    for (int i = 0; i < n; ++i, *(p++) = c)
    1ab6:	00050023          	sb	zero,0(a0)
    1aba:	4685                	li	a3,1
    1abc:	00150713          	addi	a4,a0,1
    1ac0:	18d78f63          	beq	a5,a3,1c5e <strncpy+0x1fe>
    1ac4:	000500a3          	sb	zero,1(a0)
    1ac8:	4689                	li	a3,2
    1aca:	00250713          	addi	a4,a0,2
    1ace:	18d78e63          	beq	a5,a3,1c6a <strncpy+0x20a>
    1ad2:	00050123          	sb	zero,2(a0)
    1ad6:	468d                	li	a3,3
    1ad8:	00350713          	addi	a4,a0,3
    1adc:	16d78c63          	beq	a5,a3,1c54 <strncpy+0x1f4>
    1ae0:	000501a3          	sb	zero,3(a0)
    1ae4:	4691                	li	a3,4
    1ae6:	00450713          	addi	a4,a0,4
    1aea:	18d78263          	beq	a5,a3,1c6e <strncpy+0x20e>
    1aee:	00050223          	sb	zero,4(a0)
    1af2:	4695                	li	a3,5
    1af4:	00550713          	addi	a4,a0,5
    1af8:	16d78d63          	beq	a5,a3,1c72 <strncpy+0x212>
    1afc:	000502a3          	sb	zero,5(a0)
    1b00:	469d                	li	a3,7
    1b02:	00650713          	addi	a4,a0,6
    1b06:	16d79863          	bne	a5,a3,1c76 <strncpy+0x216>
    1b0a:	00750713          	addi	a4,a0,7
    1b0e:	00050323          	sb	zero,6(a0)
    1b12:	40f80833          	sub	a6,a6,a5
    1b16:	ff887593          	andi	a1,a6,-8
    1b1a:	97aa                	add	a5,a5,a0
    1b1c:	95be                	add	a1,a1,a5
    1b1e:	0007b023          	sd	zero,0(a5)
    1b22:	07a1                	addi	a5,a5,8
    1b24:	feb79de3          	bne	a5,a1,1b1e <strncpy+0xbe>
    1b28:	ff887593          	andi	a1,a6,-8
    1b2c:	9ead                	addw	a3,a3,a1
    1b2e:	00b707b3          	add	a5,a4,a1
    1b32:	12b80863          	beq	a6,a1,1c62 <strncpy+0x202>
    1b36:	00078023          	sb	zero,0(a5)
    1b3a:	0016871b          	addiw	a4,a3,1
    1b3e:	0ec77863          	bgeu	a4,a2,1c2e <strncpy+0x1ce>
    1b42:	000780a3          	sb	zero,1(a5)
    1b46:	0026871b          	addiw	a4,a3,2
    1b4a:	0ec77263          	bgeu	a4,a2,1c2e <strncpy+0x1ce>
    1b4e:	00078123          	sb	zero,2(a5)
    1b52:	0036871b          	addiw	a4,a3,3
    1b56:	0cc77c63          	bgeu	a4,a2,1c2e <strncpy+0x1ce>
    1b5a:	000781a3          	sb	zero,3(a5)
    1b5e:	0046871b          	addiw	a4,a3,4
    1b62:	0cc77663          	bgeu	a4,a2,1c2e <strncpy+0x1ce>
    1b66:	00078223          	sb	zero,4(a5)
    1b6a:	0056871b          	addiw	a4,a3,5
    1b6e:	0cc77063          	bgeu	a4,a2,1c2e <strncpy+0x1ce>
    1b72:	000782a3          	sb	zero,5(a5)
    1b76:	0066871b          	addiw	a4,a3,6
    1b7a:	0ac77a63          	bgeu	a4,a2,1c2e <strncpy+0x1ce>
    1b7e:	00078323          	sb	zero,6(a5)
    1b82:	0076871b          	addiw	a4,a3,7
    1b86:	0ac77463          	bgeu	a4,a2,1c2e <strncpy+0x1ce>
    1b8a:	000783a3          	sb	zero,7(a5)
    1b8e:	0086871b          	addiw	a4,a3,8
    1b92:	08c77e63          	bgeu	a4,a2,1c2e <strncpy+0x1ce>
    1b96:	00078423          	sb	zero,8(a5)
    1b9a:	0096871b          	addiw	a4,a3,9
    1b9e:	08c77863          	bgeu	a4,a2,1c2e <strncpy+0x1ce>
    1ba2:	000784a3          	sb	zero,9(a5)
    1ba6:	00a6871b          	addiw	a4,a3,10
    1baa:	08c77263          	bgeu	a4,a2,1c2e <strncpy+0x1ce>
    1bae:	00078523          	sb	zero,10(a5)
    1bb2:	00b6871b          	addiw	a4,a3,11
    1bb6:	06c77c63          	bgeu	a4,a2,1c2e <strncpy+0x1ce>
    1bba:	000785a3          	sb	zero,11(a5)
    1bbe:	00c6871b          	addiw	a4,a3,12
    1bc2:	06c77663          	bgeu	a4,a2,1c2e <strncpy+0x1ce>
    1bc6:	00078623          	sb	zero,12(a5)
    1bca:	26b5                	addiw	a3,a3,13
    1bcc:	06c6f163          	bgeu	a3,a2,1c2e <strncpy+0x1ce>
    1bd0:	000786a3          	sb	zero,13(a5)
    1bd4:	8082                	ret
            ;
        if (!n || !*s)
    1bd6:	c645                	beqz	a2,1c7e <strncpy+0x21e>
    1bd8:	0005c783          	lbu	a5,0(a1)
    1bdc:	ea078be3          	beqz	a5,1a92 <strncpy+0x32>
            goto tail;
        wd = (void *)d;
        ws = (const void *)s;
        for (; n >= sizeof(size_t) && !HASZERO(*ws); n -= sizeof(size_t), ws++, wd++)
    1be0:	479d                	li	a5,7
    1be2:	02c7ff63          	bgeu	a5,a2,1c20 <strncpy+0x1c0>
    1be6:	00000897          	auipc	a7,0x0
    1bea:	3da8b883          	ld	a7,986(a7) # 1fc0 <__clone+0xce>
    1bee:	00000817          	auipc	a6,0x0
    1bf2:	3da83803          	ld	a6,986(a6) # 1fc8 <__clone+0xd6>
    1bf6:	431d                	li	t1,7
    1bf8:	6198                	ld	a4,0(a1)
    1bfa:	011707b3          	add	a5,a4,a7
    1bfe:	fff74693          	not	a3,a4
    1c02:	8ff5                	and	a5,a5,a3
    1c04:	0107f7b3          	and	a5,a5,a6
    1c08:	ef81                	bnez	a5,1c20 <strncpy+0x1c0>
            *wd = *ws;
    1c0a:	e118                	sd	a4,0(a0)
        for (; n >= sizeof(size_t) && !HASZERO(*ws); n -= sizeof(size_t), ws++, wd++)
    1c0c:	1661                	addi	a2,a2,-8
    1c0e:	05a1                	addi	a1,a1,8
    1c10:	0521                	addi	a0,a0,8
    1c12:	fec363e3          	bltu	t1,a2,1bf8 <strncpy+0x198>
        d = (void *)wd;
        s = (const void *)ws;
    }
    for (; n && (*d = *s); n--, s++, d++)
    1c16:	e609                	bnez	a2,1c20 <strncpy+0x1c0>
    1c18:	a08d                	j	1c7a <strncpy+0x21a>
    1c1a:	167d                	addi	a2,a2,-1
    1c1c:	0505                	addi	a0,a0,1
    1c1e:	ca01                	beqz	a2,1c2e <strncpy+0x1ce>
    1c20:	0005c783          	lbu	a5,0(a1)
    1c24:	0585                	addi	a1,a1,1
    1c26:	00f50023          	sb	a5,0(a0)
    1c2a:	fbe5                	bnez	a5,1c1a <strncpy+0x1ba>
        ;
tail:
    1c2c:	b59d                	j	1a92 <strncpy+0x32>
    memset(d, 0, n);
    return d;
}
    1c2e:	8082                	ret
    1c30:	472d                	li	a4,11
    1c32:	bdb5                	j	1aae <strncpy+0x4e>
    1c34:	00778713          	addi	a4,a5,7
    1c38:	45ad                	li	a1,11
    1c3a:	fff60693          	addi	a3,a2,-1
    1c3e:	e6b778e3          	bgeu	a4,a1,1aae <strncpy+0x4e>
    1c42:	b7fd                	j	1c30 <strncpy+0x1d0>
    1c44:	40a007b3          	neg	a5,a0
    1c48:	8832                	mv	a6,a2
    1c4a:	8b9d                	andi	a5,a5,7
    1c4c:	4681                	li	a3,0
    1c4e:	e4060be3          	beqz	a2,1aa4 <strncpy+0x44>
    1c52:	b7cd                	j	1c34 <strncpy+0x1d4>
    for (int i = 0; i < n; ++i, *(p++) = c)
    1c54:	468d                	li	a3,3
    1c56:	bd75                	j	1b12 <strncpy+0xb2>
    1c58:	872a                	mv	a4,a0
    1c5a:	4681                	li	a3,0
    1c5c:	bd5d                	j	1b12 <strncpy+0xb2>
    1c5e:	4685                	li	a3,1
    1c60:	bd4d                	j	1b12 <strncpy+0xb2>
    1c62:	8082                	ret
    1c64:	87aa                	mv	a5,a0
    1c66:	4681                	li	a3,0
    1c68:	b5f9                	j	1b36 <strncpy+0xd6>
    1c6a:	4689                	li	a3,2
    1c6c:	b55d                	j	1b12 <strncpy+0xb2>
    1c6e:	4691                	li	a3,4
    1c70:	b54d                	j	1b12 <strncpy+0xb2>
    1c72:	4695                	li	a3,5
    1c74:	bd79                	j	1b12 <strncpy+0xb2>
    1c76:	4699                	li	a3,6
    1c78:	bd69                	j	1b12 <strncpy+0xb2>
    1c7a:	8082                	ret
    1c7c:	8082                	ret
    1c7e:	8082                	ret

0000000000001c80 <open>:
#include <unistd.h>

#include "syscall.h"

int open(const char *path, int flags)
{
    1c80:	87aa                	mv	a5,a0
    1c82:	862e                	mv	a2,a1
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
}

static inline long __syscall4(long n, long a, long b, long c, long d)
{
    register long a7 __asm__("a7") = n;
    1c84:	03800893          	li	a7,56
    register long a0 __asm__("a0") = a;
    1c88:	f9c00513          	li	a0,-100
    register long a1 __asm__("a1") = b;
    1c8c:	85be                	mv	a1,a5
    register long a2 __asm__("a2") = c;
    register long a3 __asm__("a3") = d;
    1c8e:	4689                	li	a3,2
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3))
    1c90:	00000073          	ecall
    return syscall(SYS_openat, AT_FDCWD, path, flags, O_RDWR);
}
    1c94:	2501                	sext.w	a0,a0
    1c96:	8082                	ret

0000000000001c98 <openat>:
    register long a7 __asm__("a7") = n;
    1c98:	03800893          	li	a7,56
    register long a3 __asm__("a3") = d;
    1c9c:	18000693          	li	a3,384
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3))
    1ca0:	00000073          	ecall

int openat(int dirfd,const char *path, int flags)
{
    return syscall(SYS_openat, dirfd, path, flags, 0600);
}
    1ca4:	2501                	sext.w	a0,a0
    1ca6:	8082                	ret

0000000000001ca8 <close>:
    register long a7 __asm__("a7") = n;
    1ca8:	03900893          	li	a7,57
    __asm_syscall("r"(a7), "0"(a0))
    1cac:	00000073          	ecall

int close(int fd)
{
    return syscall(SYS_close, fd);
}
    1cb0:	2501                	sext.w	a0,a0
    1cb2:	8082                	ret

0000000000001cb4 <read>:
    register long a7 __asm__("a7") = n;
    1cb4:	03f00893          	li	a7,63
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1cb8:	00000073          	ecall

ssize_t read(int fd, void *buf, size_t len)
{
    return syscall(SYS_read, fd, buf, len);
}
    1cbc:	8082                	ret

0000000000001cbe <write>:
    register long a7 __asm__("a7") = n;
    1cbe:	04000893          	li	a7,64
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1cc2:	00000073          	ecall

ssize_t write(int fd, const void *buf, size_t len)
{
    return syscall(SYS_write, fd, buf, len);
}
    1cc6:	8082                	ret

0000000000001cc8 <getpid>:
    register long a7 __asm__("a7") = n;
    1cc8:	0ac00893          	li	a7,172
    __asm_syscall("r"(a7))
    1ccc:	00000073          	ecall

pid_t getpid(void)
{
    return syscall(SYS_getpid);
}
    1cd0:	2501                	sext.w	a0,a0
    1cd2:	8082                	ret

0000000000001cd4 <getppid>:
    register long a7 __asm__("a7") = n;
    1cd4:	0ad00893          	li	a7,173
    __asm_syscall("r"(a7))
    1cd8:	00000073          	ecall

pid_t getppid(void)
{
    return syscall(SYS_getppid);
}
    1cdc:	2501                	sext.w	a0,a0
    1cde:	8082                	ret

0000000000001ce0 <sched_yield>:
    register long a7 __asm__("a7") = n;
    1ce0:	07c00893          	li	a7,124
    __asm_syscall("r"(a7))
    1ce4:	00000073          	ecall

int sched_yield(void)
{
    return syscall(SYS_sched_yield);
}
    1ce8:	2501                	sext.w	a0,a0
    1cea:	8082                	ret

0000000000001cec <fork>:
    register long a7 __asm__("a7") = n;
    1cec:	0dc00893          	li	a7,220
    register long a0 __asm__("a0") = a;
    1cf0:	4545                	li	a0,17
    register long a1 __asm__("a1") = b;
    1cf2:	4581                	li	a1,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1cf4:	00000073          	ecall

pid_t fork(void)
{
    return syscall(SYS_clone, SIGCHLD, 0);
}
    1cf8:	2501                	sext.w	a0,a0
    1cfa:	8082                	ret

0000000000001cfc <clone>:

pid_t clone(int (*fn)(void *arg), void *arg, void *stack, size_t stack_size, unsigned long flags)
{
    1cfc:	85b2                	mv	a1,a2
    1cfe:	863a                	mv	a2,a4
    if (stack)
    1d00:	c191                	beqz	a1,1d04 <clone+0x8>
	stack += stack_size;
    1d02:	95b6                	add	a1,a1,a3

    return __clone(fn, stack, flags, NULL, NULL, NULL);
    1d04:	4781                	li	a5,0
    1d06:	4701                	li	a4,0
    1d08:	4681                	li	a3,0
    1d0a:	2601                	sext.w	a2,a2
    1d0c:	a2dd                	j	1ef2 <__clone>

0000000000001d0e <exit>:
    register long a7 __asm__("a7") = n;
    1d0e:	05d00893          	li	a7,93
    __asm_syscall("r"(a7), "0"(a0))
    1d12:	00000073          	ecall
    //return syscall(SYS_clone, fn, stack, flags, NULL, NULL, NULL);
}
void exit(int code)
{
    syscall(SYS_exit, code);
}
    1d16:	8082                	ret

0000000000001d18 <waitpid>:
    register long a7 __asm__("a7") = n;
    1d18:	10400893          	li	a7,260
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1d1c:	00000073          	ecall

int waitpid(int pid, int *code, int options)
{
    return syscall(SYS_wait4, pid, code, options);
}
    1d20:	2501                	sext.w	a0,a0
    1d22:	8082                	ret

0000000000001d24 <exec>:
    register long a7 __asm__("a7") = n;
    1d24:	0dd00893          	li	a7,221
    __asm_syscall("r"(a7), "0"(a0))
    1d28:	00000073          	ecall

int exec(char *name)
{
    return syscall(SYS_execve, name);
}
    1d2c:	2501                	sext.w	a0,a0
    1d2e:	8082                	ret

0000000000001d30 <execve>:
    register long a7 __asm__("a7") = n;
    1d30:	0dd00893          	li	a7,221
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1d34:	00000073          	ecall

int execve(const char *name, char *const argv[], char *const argp[])
{
    return syscall(SYS_execve, name, argv, argp);
}
    1d38:	2501                	sext.w	a0,a0
    1d3a:	8082                	ret

0000000000001d3c <times>:
    register long a7 __asm__("a7") = n;
    1d3c:	09900893          	li	a7,153
    __asm_syscall("r"(a7), "0"(a0))
    1d40:	00000073          	ecall

int times(void *mytimes)
{
	return syscall(SYS_times, mytimes);
}
    1d44:	2501                	sext.w	a0,a0
    1d46:	8082                	ret

0000000000001d48 <get_time>:

int64 get_time()
{
    1d48:	1141                	addi	sp,sp,-16
    register long a7 __asm__("a7") = n;
    1d4a:	0a900893          	li	a7,169
    register long a0 __asm__("a0") = a;
    1d4e:	850a                	mv	a0,sp
    register long a1 __asm__("a1") = b;
    1d50:	4581                	li	a1,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1d52:	00000073          	ecall
    TimeVal time;
    int err = sys_get_time(&time, 0);
    if (err == 0)
    1d56:	2501                	sext.w	a0,a0
    1d58:	ed09                	bnez	a0,1d72 <get_time+0x2a>
    {
        return ((time.sec & 0xffff) * 1000 + time.usec / 1000);
    1d5a:	67a2                	ld	a5,8(sp)
    1d5c:	3e800713          	li	a4,1000
    1d60:	00015503          	lhu	a0,0(sp)
    1d64:	02e7d7b3          	divu	a5,a5,a4
    1d68:	02e50533          	mul	a0,a0,a4
    1d6c:	953e                	add	a0,a0,a5
    }
    else
    {
        return -1;
    }
}
    1d6e:	0141                	addi	sp,sp,16
    1d70:	8082                	ret
        return -1;
    1d72:	557d                	li	a0,-1
    1d74:	bfed                	j	1d6e <get_time+0x26>

0000000000001d76 <sys_get_time>:
    register long a7 __asm__("a7") = n;
    1d76:	0a900893          	li	a7,169
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1d7a:	00000073          	ecall

int sys_get_time(TimeVal *ts, int tz)
{
    return syscall(SYS_gettimeofday, ts, tz);
}
    1d7e:	2501                	sext.w	a0,a0
    1d80:	8082                	ret

0000000000001d82 <time>:
    register long a7 __asm__("a7") = n;
    1d82:	42600893          	li	a7,1062
    __asm_syscall("r"(a7), "0"(a0))
    1d86:	00000073          	ecall

int time(unsigned long *tloc)
{
    return syscall(SYS_time, tloc);
}
    1d8a:	2501                	sext.w	a0,a0
    1d8c:	8082                	ret

0000000000001d8e <sleep>:

int sleep(unsigned long long time)
{
    1d8e:	1141                	addi	sp,sp,-16
    TimeVal tv = {.sec = time, .usec = 0};
    1d90:	e02a                	sd	a0,0(sp)
    register long a0 __asm__("a0") = a;
    1d92:	850a                	mv	a0,sp
    1d94:	e402                	sd	zero,8(sp)
    register long a7 __asm__("a7") = n;
    1d96:	06500893          	li	a7,101
    register long a1 __asm__("a1") = b;
    1d9a:	85aa                	mv	a1,a0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1d9c:	00000073          	ecall
    if (syscall(SYS_nanosleep, &tv, &tv)) return tv.sec;
    1da0:	e501                	bnez	a0,1da8 <sleep+0x1a>
    return 0;
    1da2:	4501                	li	a0,0
}
    1da4:	0141                	addi	sp,sp,16
    1da6:	8082                	ret
    if (syscall(SYS_nanosleep, &tv, &tv)) return tv.sec;
    1da8:	4502                	lw	a0,0(sp)
}
    1daa:	0141                	addi	sp,sp,16
    1dac:	8082                	ret

0000000000001dae <set_priority>:
    register long a7 __asm__("a7") = n;
    1dae:	08c00893          	li	a7,140
    __asm_syscall("r"(a7), "0"(a0))
    1db2:	00000073          	ecall

int set_priority(int prio)
{
    return syscall(SYS_setpriority, prio);
}
    1db6:	2501                	sext.w	a0,a0
    1db8:	8082                	ret

0000000000001dba <mmap>:
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4))
}

static inline long __syscall6(long n, long a, long b, long c, long d, long e, long f)
{
    register long a7 __asm__("a7") = n;
    1dba:	0de00893          	li	a7,222
    register long a1 __asm__("a1") = b;
    register long a2 __asm__("a2") = c;
    register long a3 __asm__("a3") = d;
    register long a4 __asm__("a4") = e;
    register long a5 __asm__("a5") = f;
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4), "r"(a5))
    1dbe:	00000073          	ecall

void *mmap(void *start, size_t len, int prot, int flags, int fd, off_t off)
{
    return syscall(SYS_mmap, start, len, prot, flags, fd, off);
}
    1dc2:	8082                	ret

0000000000001dc4 <munmap>:
    register long a7 __asm__("a7") = n;
    1dc4:	0d700893          	li	a7,215
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1dc8:	00000073          	ecall

int munmap(void *start, size_t len)
{
    return syscall(SYS_munmap, start, len);
}
    1dcc:	2501                	sext.w	a0,a0
    1dce:	8082                	ret

0000000000001dd0 <wait>:

int wait(int *code)
{
    1dd0:	85aa                	mv	a1,a0
    register long a7 __asm__("a7") = n;
    1dd2:	10400893          	li	a7,260
    register long a0 __asm__("a0") = a;
    1dd6:	557d                	li	a0,-1
    register long a2 __asm__("a2") = c;
    1dd8:	4601                	li	a2,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1dda:	00000073          	ecall
    return waitpid(-1, code, 0);
}
    1dde:	2501                	sext.w	a0,a0
    1de0:	8082                	ret

0000000000001de2 <spawn>:
    register long a7 __asm__("a7") = n;
    1de2:	19000893          	li	a7,400
    __asm_syscall("r"(a7), "0"(a0))
    1de6:	00000073          	ecall

int spawn(char *file)
{
    return syscall(SYS_spawn, file);
}
    1dea:	2501                	sext.w	a0,a0
    1dec:	8082                	ret

0000000000001dee <mailread>:
    register long a7 __asm__("a7") = n;
    1dee:	19100893          	li	a7,401
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1df2:	00000073          	ecall

int mailread(void *buf, int len)
{
    return syscall(SYS_mailread, buf, len);
}
    1df6:	2501                	sext.w	a0,a0
    1df8:	8082                	ret

0000000000001dfa <mailwrite>:
    register long a7 __asm__("a7") = n;
    1dfa:	19200893          	li	a7,402
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1dfe:	00000073          	ecall

int mailwrite(int pid, void *buf, int len)
{
    return syscall(SYS_mailwrite, pid, buf, len);
}
    1e02:	2501                	sext.w	a0,a0
    1e04:	8082                	ret

0000000000001e06 <fstat>:
    register long a7 __asm__("a7") = n;
    1e06:	05000893          	li	a7,80
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1e0a:	00000073          	ecall

int fstat(int fd, struct kstat *st)
{
    return syscall(SYS_fstat, fd, st);
}
    1e0e:	2501                	sext.w	a0,a0
    1e10:	8082                	ret

0000000000001e12 <sys_linkat>:
    register long a4 __asm__("a4") = e;
    1e12:	1702                	slli	a4,a4,0x20
    register long a7 __asm__("a7") = n;
    1e14:	02500893          	li	a7,37
    register long a4 __asm__("a4") = e;
    1e18:	9301                	srli	a4,a4,0x20
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4))
    1e1a:	00000073          	ecall

int sys_linkat(int olddirfd, char *oldpath, int newdirfd, char *newpath, unsigned int flags)
{
    return syscall(SYS_linkat, olddirfd, oldpath, newdirfd, newpath, flags);
}
    1e1e:	2501                	sext.w	a0,a0
    1e20:	8082                	ret

0000000000001e22 <sys_unlinkat>:
    register long a2 __asm__("a2") = c;
    1e22:	1602                	slli	a2,a2,0x20
    register long a7 __asm__("a7") = n;
    1e24:	02300893          	li	a7,35
    register long a2 __asm__("a2") = c;
    1e28:	9201                	srli	a2,a2,0x20
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1e2a:	00000073          	ecall

int sys_unlinkat(int dirfd, char *path, unsigned int flags)
{
    return syscall(SYS_unlinkat, dirfd, path, flags);
}
    1e2e:	2501                	sext.w	a0,a0
    1e30:	8082                	ret

0000000000001e32 <link>:

int link(char *old_path, char *new_path)
{
    1e32:	87aa                	mv	a5,a0
    1e34:	86ae                	mv	a3,a1
    register long a7 __asm__("a7") = n;
    1e36:	02500893          	li	a7,37
    register long a0 __asm__("a0") = a;
    1e3a:	f9c00513          	li	a0,-100
    register long a1 __asm__("a1") = b;
    1e3e:	85be                	mv	a1,a5
    register long a2 __asm__("a2") = c;
    1e40:	f9c00613          	li	a2,-100
    register long a4 __asm__("a4") = e;
    1e44:	4701                	li	a4,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4))
    1e46:	00000073          	ecall
    return sys_linkat(AT_FDCWD, old_path, AT_FDCWD, new_path, 0);
}
    1e4a:	2501                	sext.w	a0,a0
    1e4c:	8082                	ret

0000000000001e4e <unlink>:

int unlink(char *path)
{
    1e4e:	85aa                	mv	a1,a0
    register long a7 __asm__("a7") = n;
    1e50:	02300893          	li	a7,35
    register long a0 __asm__("a0") = a;
    1e54:	f9c00513          	li	a0,-100
    register long a2 __asm__("a2") = c;
    1e58:	4601                	li	a2,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1e5a:	00000073          	ecall
    return sys_unlinkat(AT_FDCWD, path, 0);
}
    1e5e:	2501                	sext.w	a0,a0
    1e60:	8082                	ret

0000000000001e62 <uname>:
    register long a7 __asm__("a7") = n;
    1e62:	0a000893          	li	a7,160
    __asm_syscall("r"(a7), "0"(a0))
    1e66:	00000073          	ecall

int uname(void *buf)
{
    return syscall(SYS_uname, buf);
}
    1e6a:	2501                	sext.w	a0,a0
    1e6c:	8082                	ret

0000000000001e6e <brk>:
    register long a7 __asm__("a7") = n;
    1e6e:	0d600893          	li	a7,214
    __asm_syscall("r"(a7), "0"(a0))
    1e72:	00000073          	ecall

int brk(void *addr)
{
    return syscall(SYS_brk, addr);
}
    1e76:	2501                	sext.w	a0,a0
    1e78:	8082                	ret

0000000000001e7a <getcwd>:
    register long a7 __asm__("a7") = n;
    1e7a:	48c5                	li	a7,17
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1e7c:	00000073          	ecall

char *getcwd(char *buf, size_t size){
    return syscall(SYS_getcwd, buf, size);
}
    1e80:	8082                	ret

0000000000001e82 <chdir>:
    register long a7 __asm__("a7") = n;
    1e82:	03100893          	li	a7,49
    __asm_syscall("r"(a7), "0"(a0))
    1e86:	00000073          	ecall

int chdir(const char *path){
    return syscall(SYS_chdir, path);
}
    1e8a:	2501                	sext.w	a0,a0
    1e8c:	8082                	ret

0000000000001e8e <mkdir>:

int mkdir(const char *path, mode_t mode){
    1e8e:	862e                	mv	a2,a1
    1e90:	87aa                	mv	a5,a0
    register long a2 __asm__("a2") = c;
    1e92:	1602                	slli	a2,a2,0x20
    register long a7 __asm__("a7") = n;
    1e94:	02200893          	li	a7,34
    register long a0 __asm__("a0") = a;
    1e98:	f9c00513          	li	a0,-100
    register long a1 __asm__("a1") = b;
    1e9c:	85be                	mv	a1,a5
    register long a2 __asm__("a2") = c;
    1e9e:	9201                	srli	a2,a2,0x20
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1ea0:	00000073          	ecall
    return syscall(SYS_mkdirat, AT_FDCWD, path, mode);
}
    1ea4:	2501                	sext.w	a0,a0
    1ea6:	8082                	ret

0000000000001ea8 <getdents>:
    register long a7 __asm__("a7") = n;
    1ea8:	03d00893          	li	a7,61
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1eac:	00000073          	ecall

int getdents(int fd, struct linux_dirent64 *dirp64, unsigned long len){
    //return syscall(SYS_getdents64, fd, dirp64, len);
    return syscall(SYS_getdents64, fd, dirp64, len);
}
    1eb0:	2501                	sext.w	a0,a0
    1eb2:	8082                	ret

0000000000001eb4 <pipe>:
    register long a7 __asm__("a7") = n;
    1eb4:	03b00893          	li	a7,59
    register long a1 __asm__("a1") = b;
    1eb8:	4581                	li	a1,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1eba:	00000073          	ecall

int pipe(int fd[2]){
    return syscall(SYS_pipe2, fd, 0);
}
    1ebe:	2501                	sext.w	a0,a0
    1ec0:	8082                	ret

0000000000001ec2 <dup>:
    register long a7 __asm__("a7") = n;
    1ec2:	48dd                	li	a7,23
    __asm_syscall("r"(a7), "0"(a0))
    1ec4:	00000073          	ecall

int dup(int fd){
    return syscall(SYS_dup, fd);
}
    1ec8:	2501                	sext.w	a0,a0
    1eca:	8082                	ret

0000000000001ecc <dup2>:
    register long a7 __asm__("a7") = n;
    1ecc:	48e1                	li	a7,24
    register long a2 __asm__("a2") = c;
    1ece:	4601                	li	a2,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1ed0:	00000073          	ecall

int dup2(int old, int new){
    return syscall(SYS_dup3, old, new, 0);
}
    1ed4:	2501                	sext.w	a0,a0
    1ed6:	8082                	ret

0000000000001ed8 <mount>:
    register long a7 __asm__("a7") = n;
    1ed8:	02800893          	li	a7,40
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4))
    1edc:	00000073          	ecall

int mount(const char *special, const char *dir, const char *fstype, unsigned long flags, const void *data)
{
        return syscall(SYS_mount, special, dir, fstype, flags, data);
}
    1ee0:	2501                	sext.w	a0,a0
    1ee2:	8082                	ret

0000000000001ee4 <umount>:
    register long a7 __asm__("a7") = n;
    1ee4:	02700893          	li	a7,39
    register long a1 __asm__("a1") = b;
    1ee8:	4581                	li	a1,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1eea:	00000073          	ecall

int umount(const char *special)
{
        return syscall(SYS_umount2, special, 0);
}
    1eee:	2501                	sext.w	a0,a0
    1ef0:	8082                	ret

0000000000001ef2 <__clone>:

.global __clone
.type  __clone, %function
__clone:
	# Save func and arg to stack
	addi a1, a1, -16
    1ef2:	15c1                	addi	a1,a1,-16
	sd a0, 0(a1)
    1ef4:	e188                	sd	a0,0(a1)
	sd a3, 8(a1)
    1ef6:	e594                	sd	a3,8(a1)

	# Call SYS_clone
	mv a0, a2
    1ef8:	8532                	mv	a0,a2
	mv a2, a4
    1efa:	863a                	mv	a2,a4
	mv a3, a5
    1efc:	86be                	mv	a3,a5
	mv a4, a6
    1efe:	8742                	mv	a4,a6
	li a7, 220 # SYS_clone
    1f00:	0dc00893          	li	a7,220
	ecall
    1f04:	00000073          	ecall

	beqz a0, 1f
    1f08:	c111                	beqz	a0,1f0c <__clone+0x1a>
	# Parent
	ret
    1f0a:	8082                	ret

	# Child
1:      ld a1, 0(sp)
    1f0c:	6582                	ld	a1,0(sp)
	ld a0, 8(sp)
    1f0e:	6522                	ld	a0,8(sp)
	jalr a1
    1f10:	9582                	jalr	a1

	# Exit
	li a7, 93 # SYS_exit
    1f12:	05d00893          	li	a7,93
	ecall
    1f16:	00000073          	ecall
