
/home/ubuntu/rust/ultra-os/codes/riscv-syscalls-testing/user/build/riscv64/openat:     file format elf64-littleriscv


Disassembly of section .text:

0000000000001000 <_start>:
.section .text.entry
.globl _start
_start:
    mv a0, sp
    1000:	850a                	mv	a0,sp
    tail __start_main
    1002:	a0e9                	j	10cc <__start_main>

0000000000001004 <test_openat>:
#include "unistd.h"
#include "stdio.h"
#include "stdlib.h"

void test_openat(void) {
    1004:	1141                	addi	sp,sp,-16
    TEST_START(__func__);
    1006:	00001517          	auipc	a0,0x1
    100a:	f0250513          	addi	a0,a0,-254 # 1f08 <__clone+0x2e>
void test_openat(void) {
    100e:	e406                	sd	ra,8(sp)
    1010:	e022                	sd	s0,0(sp)
    TEST_START(__func__);
    1012:	32c000ef          	jal	ra,133e <puts>
    1016:	00001517          	auipc	a0,0x1
    101a:	fc250513          	addi	a0,a0,-62 # 1fd8 <__func__.0>
    101e:	320000ef          	jal	ra,133e <puts>
    1022:	00001517          	auipc	a0,0x1
    1026:	efe50513          	addi	a0,a0,-258 # 1f20 <__clone+0x46>
    102a:	314000ef          	jal	ra,133e <puts>
    //int fd_dir = open(".", O_RDONLY | O_CREATE);
    int fd_dir = open("./mnt", O_DIRECTORY);
    102e:	002005b7          	lui	a1,0x200
    1032:	00001517          	auipc	a0,0x1
    1036:	efe50513          	addi	a0,a0,-258 # 1f30 <__clone+0x56>
    103a:	42f000ef          	jal	ra,1c68 <open>
    103e:	842a                	mv	s0,a0
    printf("open dir fd: %d\n", fd_dir);
    1040:	85aa                	mv	a1,a0
    1042:	00001517          	auipc	a0,0x1
    1046:	ef650513          	addi	a0,a0,-266 # 1f38 <__clone+0x5e>
    104a:	316000ef          	jal	ra,1360 <printf>
    int fd = openat(fd_dir, "test_openat.txt", O_CREATE | O_RDWR);
    104e:	00001597          	auipc	a1,0x1
    1052:	f0258593          	addi	a1,a1,-254 # 1f50 <__clone+0x76>
    1056:	8522                	mv	a0,s0
    1058:	04200613          	li	a2,66
    105c:	425000ef          	jal	ra,1c80 <openat>
    1060:	842a                	mv	s0,a0
    printf("openat fd: %d\n", fd);
    1062:	85aa                	mv	a1,a0
    1064:	00001517          	auipc	a0,0x1
    1068:	efc50513          	addi	a0,a0,-260 # 1f60 <__clone+0x86>
    106c:	2f4000ef          	jal	ra,1360 <printf>
    assert(fd > 0);
    1070:	02805f63          	blez	s0,10ae <test_openat+0xaa>
    printf("openat success.\n");
    1074:	00001517          	auipc	a0,0x1
    1078:	f1c50513          	addi	a0,a0,-228 # 1f90 <__clone+0xb6>
    107c:	2e4000ef          	jal	ra,1360 <printf>
    write(fd, buf, strlen(buf));
    int size = read(fd, buf, 256);
    if (size > 0) printf("  openat success.\n");
    else printf("  openat error.\n");
    */
    close(fd);	
    1080:	8522                	mv	a0,s0
    1082:	40f000ef          	jal	ra,1c90 <close>
	
    TEST_END(__func__);
    1086:	00001517          	auipc	a0,0x1
    108a:	f2250513          	addi	a0,a0,-222 # 1fa8 <__clone+0xce>
    108e:	2b0000ef          	jal	ra,133e <puts>
    1092:	00001517          	auipc	a0,0x1
    1096:	f4650513          	addi	a0,a0,-186 # 1fd8 <__func__.0>
    109a:	2a4000ef          	jal	ra,133e <puts>
}
    109e:	6402                	ld	s0,0(sp)
    10a0:	60a2                	ld	ra,8(sp)
    TEST_END(__func__);
    10a2:	00001517          	auipc	a0,0x1
    10a6:	e7e50513          	addi	a0,a0,-386 # 1f20 <__clone+0x46>
}
    10aa:	0141                	addi	sp,sp,16
    TEST_END(__func__);
    10ac:	ac49                	j	133e <puts>
    assert(fd > 0);
    10ae:	00001517          	auipc	a0,0x1
    10b2:	ec250513          	addi	a0,a0,-318 # 1f70 <__clone+0x96>
    10b6:	52e000ef          	jal	ra,15e4 <panic>
    10ba:	bf6d                	j	1074 <test_openat+0x70>

00000000000010bc <main>:

int main(void) {
    10bc:	1141                	addi	sp,sp,-16
    10be:	e406                	sd	ra,8(sp)
    test_openat();
    10c0:	f45ff0ef          	jal	ra,1004 <test_openat>
    return 0;
}
    10c4:	60a2                	ld	ra,8(sp)
    10c6:	4501                	li	a0,0
    10c8:	0141                	addi	sp,sp,16
    10ca:	8082                	ret

00000000000010cc <__start_main>:
#include <unistd.h>

extern int main();

int __start_main(long *p)
{
    10cc:	85aa                	mv	a1,a0
	int argc = p[0];
	char **argv = (void *)(p+1);

	exit(main(argc, argv));
    10ce:	4108                	lw	a0,0(a0)
{
    10d0:	1141                	addi	sp,sp,-16
	exit(main(argc, argv));
    10d2:	05a1                	addi	a1,a1,8
{
    10d4:	e406                	sd	ra,8(sp)
	exit(main(argc, argv));
    10d6:	fe7ff0ef          	jal	ra,10bc <main>
    10da:	41d000ef          	jal	ra,1cf6 <exit>
	return 0;
}
    10de:	60a2                	ld	ra,8(sp)
    10e0:	4501                	li	a0,0
    10e2:	0141                	addi	sp,sp,16
    10e4:	8082                	ret

00000000000010e6 <printint.constprop.0>:
    write(f, s, l);
}

static char digits[] = "0123456789abcdef";

static void printint(int xx, int base, int sign)
    10e6:	7179                	addi	sp,sp,-48
    10e8:	f406                	sd	ra,40(sp)
{
    char buf[16 + 1];
    int i;
    uint x;

    if (sign && (sign = xx < 0))
    10ea:	12054b63          	bltz	a0,1220 <printint.constprop.0+0x13a>

    buf[16] = 0;
    i = 15;
    do
    {
        buf[i--] = digits[x % base];
    10ee:	02b577bb          	remuw	a5,a0,a1
    10f2:	00001617          	auipc	a2,0x1
    10f6:	ef660613          	addi	a2,a2,-266 # 1fe8 <digits>
    buf[16] = 0;
    10fa:	00010c23          	sb	zero,24(sp)
        buf[i--] = digits[x % base];
    10fe:	0005871b          	sext.w	a4,a1
    1102:	1782                	slli	a5,a5,0x20
    1104:	9381                	srli	a5,a5,0x20
    1106:	97b2                	add	a5,a5,a2
    1108:	0007c783          	lbu	a5,0(a5)
    } while ((x /= base) != 0);
    110c:	02b5583b          	divuw	a6,a0,a1
        buf[i--] = digits[x % base];
    1110:	00f10ba3          	sb	a5,23(sp)
    } while ((x /= base) != 0);
    1114:	1cb56363          	bltu	a0,a1,12da <printint.constprop.0+0x1f4>
        buf[i--] = digits[x % base];
    1118:	45b9                	li	a1,14
    111a:	02e877bb          	remuw	a5,a6,a4
    111e:	1782                	slli	a5,a5,0x20
    1120:	9381                	srli	a5,a5,0x20
    1122:	97b2                	add	a5,a5,a2
    1124:	0007c783          	lbu	a5,0(a5)
    } while ((x /= base) != 0);
    1128:	02e856bb          	divuw	a3,a6,a4
        buf[i--] = digits[x % base];
    112c:	00f10b23          	sb	a5,22(sp)
    } while ((x /= base) != 0);
    1130:	0ce86e63          	bltu	a6,a4,120c <printint.constprop.0+0x126>
        buf[i--] = digits[x % base];
    1134:	02e6f5bb          	remuw	a1,a3,a4
    } while ((x /= base) != 0);
    1138:	02e6d7bb          	divuw	a5,a3,a4
        buf[i--] = digits[x % base];
    113c:	1582                	slli	a1,a1,0x20
    113e:	9181                	srli	a1,a1,0x20
    1140:	95b2                	add	a1,a1,a2
    1142:	0005c583          	lbu	a1,0(a1)
    1146:	00b10aa3          	sb	a1,21(sp)
    } while ((x /= base) != 0);
    114a:	0007859b          	sext.w	a1,a5
    114e:	12e6ec63          	bltu	a3,a4,1286 <printint.constprop.0+0x1a0>
        buf[i--] = digits[x % base];
    1152:	02e7f6bb          	remuw	a3,a5,a4
    1156:	1682                	slli	a3,a3,0x20
    1158:	9281                	srli	a3,a3,0x20
    115a:	96b2                	add	a3,a3,a2
    115c:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    1160:	02e7d83b          	divuw	a6,a5,a4
        buf[i--] = digits[x % base];
    1164:	00d10a23          	sb	a3,20(sp)
    } while ((x /= base) != 0);
    1168:	12e5e863          	bltu	a1,a4,1298 <printint.constprop.0+0x1b2>
        buf[i--] = digits[x % base];
    116c:	02e876bb          	remuw	a3,a6,a4
    1170:	1682                	slli	a3,a3,0x20
    1172:	9281                	srli	a3,a3,0x20
    1174:	96b2                	add	a3,a3,a2
    1176:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    117a:	02e855bb          	divuw	a1,a6,a4
        buf[i--] = digits[x % base];
    117e:	00d109a3          	sb	a3,19(sp)
    } while ((x /= base) != 0);
    1182:	12e86463          	bltu	a6,a4,12aa <printint.constprop.0+0x1c4>
        buf[i--] = digits[x % base];
    1186:	02e5f6bb          	remuw	a3,a1,a4
    118a:	1682                	slli	a3,a3,0x20
    118c:	9281                	srli	a3,a3,0x20
    118e:	96b2                	add	a3,a3,a2
    1190:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    1194:	02e5d83b          	divuw	a6,a1,a4
        buf[i--] = digits[x % base];
    1198:	00d10923          	sb	a3,18(sp)
    } while ((x /= base) != 0);
    119c:	0ce5ec63          	bltu	a1,a4,1274 <printint.constprop.0+0x18e>
        buf[i--] = digits[x % base];
    11a0:	02e876bb          	remuw	a3,a6,a4
    11a4:	1682                	slli	a3,a3,0x20
    11a6:	9281                	srli	a3,a3,0x20
    11a8:	96b2                	add	a3,a3,a2
    11aa:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    11ae:	02e855bb          	divuw	a1,a6,a4
        buf[i--] = digits[x % base];
    11b2:	00d108a3          	sb	a3,17(sp)
    } while ((x /= base) != 0);
    11b6:	10e86963          	bltu	a6,a4,12c8 <printint.constprop.0+0x1e2>
        buf[i--] = digits[x % base];
    11ba:	02e5f6bb          	remuw	a3,a1,a4
    11be:	1682                	slli	a3,a3,0x20
    11c0:	9281                	srli	a3,a3,0x20
    11c2:	96b2                	add	a3,a3,a2
    11c4:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    11c8:	02e5d83b          	divuw	a6,a1,a4
        buf[i--] = digits[x % base];
    11cc:	00d10823          	sb	a3,16(sp)
    } while ((x /= base) != 0);
    11d0:	10e5e763          	bltu	a1,a4,12de <printint.constprop.0+0x1f8>
        buf[i--] = digits[x % base];
    11d4:	02e876bb          	remuw	a3,a6,a4
    11d8:	1682                	slli	a3,a3,0x20
    11da:	9281                	srli	a3,a3,0x20
    11dc:	96b2                	add	a3,a3,a2
    11de:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    11e2:	02e857bb          	divuw	a5,a6,a4
        buf[i--] = digits[x % base];
    11e6:	00d107a3          	sb	a3,15(sp)
    } while ((x /= base) != 0);
    11ea:	10e86363          	bltu	a6,a4,12f0 <printint.constprop.0+0x20a>
        buf[i--] = digits[x % base];
    11ee:	1782                	slli	a5,a5,0x20
    11f0:	9381                	srli	a5,a5,0x20
    11f2:	97b2                	add	a5,a5,a2
    11f4:	0007c783          	lbu	a5,0(a5)
    11f8:	4599                	li	a1,6
    11fa:	00f10723          	sb	a5,14(sp)

    if (sign)
    11fe:	00055763          	bgez	a0,120c <printint.constprop.0+0x126>
        buf[i--] = '-';
    1202:	02d00793          	li	a5,45
    1206:	00f106a3          	sb	a5,13(sp)
        buf[i--] = digits[x % base];
    120a:	4595                	li	a1,5
    write(f, s, l);
    120c:	003c                	addi	a5,sp,8
    120e:	4641                	li	a2,16
    1210:	9e0d                	subw	a2,a2,a1
    1212:	4505                	li	a0,1
    1214:	95be                	add	a1,a1,a5
    1216:	291000ef          	jal	ra,1ca6 <write>
    i++;
    if (i < 0)
        puts("printint error");
    out(stdout, buf + i, 16 - i);
}
    121a:	70a2                	ld	ra,40(sp)
    121c:	6145                	addi	sp,sp,48
    121e:	8082                	ret
        x = -xx;
    1220:	40a0083b          	negw	a6,a0
        buf[i--] = digits[x % base];
    1224:	02b877bb          	remuw	a5,a6,a1
    1228:	00001617          	auipc	a2,0x1
    122c:	dc060613          	addi	a2,a2,-576 # 1fe8 <digits>
    buf[16] = 0;
    1230:	00010c23          	sb	zero,24(sp)
        buf[i--] = digits[x % base];
    1234:	0005871b          	sext.w	a4,a1
    1238:	1782                	slli	a5,a5,0x20
    123a:	9381                	srli	a5,a5,0x20
    123c:	97b2                	add	a5,a5,a2
    123e:	0007c783          	lbu	a5,0(a5)
    } while ((x /= base) != 0);
    1242:	02b858bb          	divuw	a7,a6,a1
        buf[i--] = digits[x % base];
    1246:	00f10ba3          	sb	a5,23(sp)
    } while ((x /= base) != 0);
    124a:	06b86963          	bltu	a6,a1,12bc <printint.constprop.0+0x1d6>
        buf[i--] = digits[x % base];
    124e:	02e8f7bb          	remuw	a5,a7,a4
    1252:	1782                	slli	a5,a5,0x20
    1254:	9381                	srli	a5,a5,0x20
    1256:	97b2                	add	a5,a5,a2
    1258:	0007c783          	lbu	a5,0(a5)
    } while ((x /= base) != 0);
    125c:	02e8d6bb          	divuw	a3,a7,a4
        buf[i--] = digits[x % base];
    1260:	00f10b23          	sb	a5,22(sp)
    } while ((x /= base) != 0);
    1264:	ece8f8e3          	bgeu	a7,a4,1134 <printint.constprop.0+0x4e>
        buf[i--] = '-';
    1268:	02d00793          	li	a5,45
    126c:	00f10aa3          	sb	a5,21(sp)
        buf[i--] = digits[x % base];
    1270:	45b5                	li	a1,13
    1272:	bf69                	j	120c <printint.constprop.0+0x126>
    1274:	45a9                	li	a1,10
    if (sign)
    1276:	f8055be3          	bgez	a0,120c <printint.constprop.0+0x126>
        buf[i--] = '-';
    127a:	02d00793          	li	a5,45
    127e:	00f108a3          	sb	a5,17(sp)
        buf[i--] = digits[x % base];
    1282:	45a5                	li	a1,9
    1284:	b761                	j	120c <printint.constprop.0+0x126>
    1286:	45b5                	li	a1,13
    if (sign)
    1288:	f80552e3          	bgez	a0,120c <printint.constprop.0+0x126>
        buf[i--] = '-';
    128c:	02d00793          	li	a5,45
    1290:	00f10a23          	sb	a5,20(sp)
        buf[i--] = digits[x % base];
    1294:	45b1                	li	a1,12
    1296:	bf9d                	j	120c <printint.constprop.0+0x126>
    1298:	45b1                	li	a1,12
    if (sign)
    129a:	f60559e3          	bgez	a0,120c <printint.constprop.0+0x126>
        buf[i--] = '-';
    129e:	02d00793          	li	a5,45
    12a2:	00f109a3          	sb	a5,19(sp)
        buf[i--] = digits[x % base];
    12a6:	45ad                	li	a1,11
    12a8:	b795                	j	120c <printint.constprop.0+0x126>
    12aa:	45ad                	li	a1,11
    if (sign)
    12ac:	f60550e3          	bgez	a0,120c <printint.constprop.0+0x126>
        buf[i--] = '-';
    12b0:	02d00793          	li	a5,45
    12b4:	00f10923          	sb	a5,18(sp)
        buf[i--] = digits[x % base];
    12b8:	45a9                	li	a1,10
    12ba:	bf89                	j	120c <printint.constprop.0+0x126>
        buf[i--] = '-';
    12bc:	02d00793          	li	a5,45
    12c0:	00f10b23          	sb	a5,22(sp)
        buf[i--] = digits[x % base];
    12c4:	45b9                	li	a1,14
    12c6:	b799                	j	120c <printint.constprop.0+0x126>
    12c8:	45a5                	li	a1,9
    if (sign)
    12ca:	f40551e3          	bgez	a0,120c <printint.constprop.0+0x126>
        buf[i--] = '-';
    12ce:	02d00793          	li	a5,45
    12d2:	00f10823          	sb	a5,16(sp)
        buf[i--] = digits[x % base];
    12d6:	45a1                	li	a1,8
    12d8:	bf15                	j	120c <printint.constprop.0+0x126>
    i = 15;
    12da:	45bd                	li	a1,15
    12dc:	bf05                	j	120c <printint.constprop.0+0x126>
        buf[i--] = digits[x % base];
    12de:	45a1                	li	a1,8
    if (sign)
    12e0:	f20556e3          	bgez	a0,120c <printint.constprop.0+0x126>
        buf[i--] = '-';
    12e4:	02d00793          	li	a5,45
    12e8:	00f107a3          	sb	a5,15(sp)
        buf[i--] = digits[x % base];
    12ec:	459d                	li	a1,7
    12ee:	bf39                	j	120c <printint.constprop.0+0x126>
    12f0:	459d                	li	a1,7
    if (sign)
    12f2:	f0055de3          	bgez	a0,120c <printint.constprop.0+0x126>
        buf[i--] = '-';
    12f6:	02d00793          	li	a5,45
    12fa:	00f10723          	sb	a5,14(sp)
        buf[i--] = digits[x % base];
    12fe:	4599                	li	a1,6
    1300:	b731                	j	120c <printint.constprop.0+0x126>

0000000000001302 <getchar>:
{
    1302:	1101                	addi	sp,sp,-32
    read(stdin, &byte, 1);
    1304:	00f10593          	addi	a1,sp,15
    1308:	4605                	li	a2,1
    130a:	4501                	li	a0,0
{
    130c:	ec06                	sd	ra,24(sp)
    char byte = 0;
    130e:	000107a3          	sb	zero,15(sp)
    read(stdin, &byte, 1);
    1312:	18b000ef          	jal	ra,1c9c <read>
}
    1316:	60e2                	ld	ra,24(sp)
    1318:	00f14503          	lbu	a0,15(sp)
    131c:	6105                	addi	sp,sp,32
    131e:	8082                	ret

0000000000001320 <putchar>:
{
    1320:	1101                	addi	sp,sp,-32
    1322:	87aa                	mv	a5,a0
    return write(stdout, &byte, 1);
    1324:	00f10593          	addi	a1,sp,15
    1328:	4605                	li	a2,1
    132a:	4505                	li	a0,1
{
    132c:	ec06                	sd	ra,24(sp)
    char byte = c;
    132e:	00f107a3          	sb	a5,15(sp)
    return write(stdout, &byte, 1);
    1332:	175000ef          	jal	ra,1ca6 <write>
}
    1336:	60e2                	ld	ra,24(sp)
    1338:	2501                	sext.w	a0,a0
    133a:	6105                	addi	sp,sp,32
    133c:	8082                	ret

000000000000133e <puts>:
{
    133e:	1141                	addi	sp,sp,-16
    1340:	e406                	sd	ra,8(sp)
    1342:	e022                	sd	s0,0(sp)
    1344:	842a                	mv	s0,a0
    r = -(write(stdout, s, strlen(s)) < 0);
    1346:	57c000ef          	jal	ra,18c2 <strlen>
    134a:	862a                	mv	a2,a0
    134c:	85a2                	mv	a1,s0
    134e:	4505                	li	a0,1
    1350:	157000ef          	jal	ra,1ca6 <write>
}
    1354:	60a2                	ld	ra,8(sp)
    1356:	6402                	ld	s0,0(sp)
    r = -(write(stdout, s, strlen(s)) < 0);
    1358:	957d                	srai	a0,a0,0x3f
    return r;
    135a:	2501                	sext.w	a0,a0
}
    135c:	0141                	addi	sp,sp,16
    135e:	8082                	ret

0000000000001360 <printf>:
    out(stdout, buf, i);
}

// Print to the console. only understands %d, %x, %p, %s.
void printf(const char *fmt, ...)
{
    1360:	7171                	addi	sp,sp,-176
    1362:	fc56                	sd	s5,56(sp)
    1364:	ed3e                	sd	a5,152(sp)
    buf[i++] = '0';
    1366:	7ae1                	lui	s5,0xffff8
    va_list ap;
    int cnt = 0, l = 0;
    char *a, *z, *s = (char *)fmt, str;
    int f = stdout;

    va_start(ap, fmt);
    1368:	18bc                	addi	a5,sp,120
{
    136a:	e8ca                	sd	s2,80(sp)
    136c:	e4ce                	sd	s3,72(sp)
    136e:	e0d2                	sd	s4,64(sp)
    1370:	f85a                	sd	s6,48(sp)
    1372:	f486                	sd	ra,104(sp)
    1374:	f0a2                	sd	s0,96(sp)
    1376:	eca6                	sd	s1,88(sp)
    1378:	fcae                	sd	a1,120(sp)
    137a:	e132                	sd	a2,128(sp)
    137c:	e536                	sd	a3,136(sp)
    137e:	e93a                	sd	a4,144(sp)
    1380:	f142                	sd	a6,160(sp)
    1382:	f546                	sd	a7,168(sp)
    va_start(ap, fmt);
    1384:	e03e                	sd	a5,0(sp)
    for (;;)
    {
        if (!*s)
            break;
        for (a = s; *s && *s != '%'; s++)
    1386:	02500913          	li	s2,37
        out(f, a, l);
        if (l)
            continue;
        if (s[1] == 0)
            break;
        switch (s[1])
    138a:	07300a13          	li	s4,115
        case 'p':
            printptr(va_arg(ap, uint64));
            break;
        case 's':
            if ((a = va_arg(ap, char *)) == 0)
                a = "(null)";
    138e:	00001b17          	auipc	s6,0x1
    1392:	c2ab0b13          	addi	s6,s6,-982 # 1fb8 <__clone+0xde>
    buf[i++] = '0';
    1396:	830aca93          	xori	s5,s5,-2000
        buf[i++] = digits[x >> (sizeof(uint64) * 8 - 4)];
    139a:	00001997          	auipc	s3,0x1
    139e:	c4e98993          	addi	s3,s3,-946 # 1fe8 <digits>
        if (!*s)
    13a2:	00054783          	lbu	a5,0(a0)
    13a6:	16078a63          	beqz	a5,151a <printf+0x1ba>
    13aa:	862a                	mv	a2,a0
        for (a = s; *s && *s != '%'; s++)
    13ac:	19278163          	beq	a5,s2,152e <printf+0x1ce>
    13b0:	00164783          	lbu	a5,1(a2)
    13b4:	0605                	addi	a2,a2,1
    13b6:	fbfd                	bnez	a5,13ac <printf+0x4c>
    13b8:	84b2                	mv	s1,a2
        l = z - a;
    13ba:	40a6043b          	subw	s0,a2,a0
    write(f, s, l);
    13be:	85aa                	mv	a1,a0
    13c0:	8622                	mv	a2,s0
    13c2:	4505                	li	a0,1
    13c4:	0e3000ef          	jal	ra,1ca6 <write>
        if (l)
    13c8:	18041c63          	bnez	s0,1560 <printf+0x200>
        if (s[1] == 0)
    13cc:	0014c783          	lbu	a5,1(s1)
    13d0:	14078563          	beqz	a5,151a <printf+0x1ba>
        switch (s[1])
    13d4:	1d478063          	beq	a5,s4,1594 <printf+0x234>
    13d8:	18fa6663          	bltu	s4,a5,1564 <printf+0x204>
    13dc:	06400713          	li	a4,100
    13e0:	1ae78063          	beq	a5,a4,1580 <printf+0x220>
    13e4:	07000713          	li	a4,112
    13e8:	1ce79963          	bne	a5,a4,15ba <printf+0x25a>
            printptr(va_arg(ap, uint64));
    13ec:	6702                	ld	a4,0(sp)
    buf[i++] = '0';
    13ee:	01511423          	sh	s5,8(sp)
    write(f, s, l);
    13f2:	4649                	li	a2,18
            printptr(va_arg(ap, uint64));
    13f4:	631c                	ld	a5,0(a4)
    13f6:	0721                	addi	a4,a4,8
    13f8:	e03a                	sd	a4,0(sp)
    for (j = 0; j < (sizeof(uint64) * 2); j++, x <<= 4)
    13fa:	00479293          	slli	t0,a5,0x4
    13fe:	00879f93          	slli	t6,a5,0x8
    1402:	00c79f13          	slli	t5,a5,0xc
    1406:	01079e93          	slli	t4,a5,0x10
    140a:	01479e13          	slli	t3,a5,0x14
    140e:	01879313          	slli	t1,a5,0x18
    1412:	01c79893          	slli	a7,a5,0x1c
    1416:	02479813          	slli	a6,a5,0x24
    141a:	02879513          	slli	a0,a5,0x28
    141e:	02c79593          	slli	a1,a5,0x2c
    1422:	03079693          	slli	a3,a5,0x30
    1426:	03479713          	slli	a4,a5,0x34
        buf[i++] = digits[x >> (sizeof(uint64) * 8 - 4)];
    142a:	03c7d413          	srli	s0,a5,0x3c
    142e:	01c7d39b          	srliw	t2,a5,0x1c
    1432:	03c2d293          	srli	t0,t0,0x3c
    1436:	03cfdf93          	srli	t6,t6,0x3c
    143a:	03cf5f13          	srli	t5,t5,0x3c
    143e:	03cede93          	srli	t4,t4,0x3c
    1442:	03ce5e13          	srli	t3,t3,0x3c
    1446:	03c35313          	srli	t1,t1,0x3c
    144a:	03c8d893          	srli	a7,a7,0x3c
    144e:	03c85813          	srli	a6,a6,0x3c
    1452:	9171                	srli	a0,a0,0x3c
    1454:	91f1                	srli	a1,a1,0x3c
    1456:	92f1                	srli	a3,a3,0x3c
    1458:	9371                	srli	a4,a4,0x3c
    145a:	96ce                	add	a3,a3,s3
    145c:	974e                	add	a4,a4,s3
    145e:	944e                	add	s0,s0,s3
    1460:	92ce                	add	t0,t0,s3
    1462:	9fce                	add	t6,t6,s3
    1464:	9f4e                	add	t5,t5,s3
    1466:	9ece                	add	t4,t4,s3
    1468:	9e4e                	add	t3,t3,s3
    146a:	934e                	add	t1,t1,s3
    146c:	98ce                	add	a7,a7,s3
    146e:	93ce                	add	t2,t2,s3
    1470:	984e                	add	a6,a6,s3
    1472:	954e                	add	a0,a0,s3
    1474:	95ce                	add	a1,a1,s3
    1476:	0006c083          	lbu	ra,0(a3)
    147a:	0002c283          	lbu	t0,0(t0)
    147e:	00074683          	lbu	a3,0(a4)
    1482:	000fcf83          	lbu	t6,0(t6)
    1486:	000f4f03          	lbu	t5,0(t5)
    148a:	000ece83          	lbu	t4,0(t4)
    148e:	000e4e03          	lbu	t3,0(t3)
    1492:	00034303          	lbu	t1,0(t1)
    1496:	0008c883          	lbu	a7,0(a7)
    149a:	0003c383          	lbu	t2,0(t2)
    149e:	00084803          	lbu	a6,0(a6)
    14a2:	00054503          	lbu	a0,0(a0)
    14a6:	0005c583          	lbu	a1,0(a1)
    14aa:	00044403          	lbu	s0,0(s0)
    for (j = 0; j < (sizeof(uint64) * 2); j++, x <<= 4)
    14ae:	03879713          	slli	a4,a5,0x38
        buf[i++] = digits[x >> (sizeof(uint64) * 8 - 4)];
    14b2:	9371                	srli	a4,a4,0x3c
    14b4:	8bbd                	andi	a5,a5,15
    14b6:	974e                	add	a4,a4,s3
    14b8:	97ce                	add	a5,a5,s3
    14ba:	005105a3          	sb	t0,11(sp)
    14be:	01f10623          	sb	t6,12(sp)
    14c2:	01e106a3          	sb	t5,13(sp)
    14c6:	01d10723          	sb	t4,14(sp)
    14ca:	01c107a3          	sb	t3,15(sp)
    14ce:	00610823          	sb	t1,16(sp)
    14d2:	011108a3          	sb	a7,17(sp)
    14d6:	00710923          	sb	t2,18(sp)
    14da:	010109a3          	sb	a6,19(sp)
    14de:	00a10a23          	sb	a0,20(sp)
    14e2:	00b10aa3          	sb	a1,21(sp)
    14e6:	00110b23          	sb	ra,22(sp)
    14ea:	00d10ba3          	sb	a3,23(sp)
    14ee:	00810523          	sb	s0,10(sp)
    14f2:	00074703          	lbu	a4,0(a4)
    14f6:	0007c783          	lbu	a5,0(a5)
    write(f, s, l);
    14fa:	002c                	addi	a1,sp,8
    14fc:	4505                	li	a0,1
        buf[i++] = digits[x >> (sizeof(uint64) * 8 - 4)];
    14fe:	00e10c23          	sb	a4,24(sp)
    1502:	00f10ca3          	sb	a5,25(sp)
    buf[i] = 0;
    1506:	00010d23          	sb	zero,26(sp)
    write(f, s, l);
    150a:	79c000ef          	jal	ra,1ca6 <write>
            // Print unknown % sequence to draw attention.
            putchar('%');
            putchar(s[1]);
            break;
        }
        s += 2;
    150e:	00248513          	addi	a0,s1,2
        if (!*s)
    1512:	00054783          	lbu	a5,0(a0)
    1516:	e8079ae3          	bnez	a5,13aa <printf+0x4a>
    }
    va_end(ap);
}
    151a:	70a6                	ld	ra,104(sp)
    151c:	7406                	ld	s0,96(sp)
    151e:	64e6                	ld	s1,88(sp)
    1520:	6946                	ld	s2,80(sp)
    1522:	69a6                	ld	s3,72(sp)
    1524:	6a06                	ld	s4,64(sp)
    1526:	7ae2                	ld	s5,56(sp)
    1528:	7b42                	ld	s6,48(sp)
    152a:	614d                	addi	sp,sp,176
    152c:	8082                	ret
        for (z = s; s[0] == '%' && s[1] == '%'; z++, s += 2)
    152e:	00064783          	lbu	a5,0(a2)
    1532:	84b2                	mv	s1,a2
    1534:	01278963          	beq	a5,s2,1546 <printf+0x1e6>
    1538:	b549                	j	13ba <printf+0x5a>
    153a:	0024c783          	lbu	a5,2(s1)
    153e:	0605                	addi	a2,a2,1
    1540:	0489                	addi	s1,s1,2
    1542:	e7279ce3          	bne	a5,s2,13ba <printf+0x5a>
    1546:	0014c783          	lbu	a5,1(s1)
    154a:	ff2788e3          	beq	a5,s2,153a <printf+0x1da>
        l = z - a;
    154e:	40a6043b          	subw	s0,a2,a0
    write(f, s, l);
    1552:	85aa                	mv	a1,a0
    1554:	8622                	mv	a2,s0
    1556:	4505                	li	a0,1
    1558:	74e000ef          	jal	ra,1ca6 <write>
        if (l)
    155c:	e60408e3          	beqz	s0,13cc <printf+0x6c>
    1560:	8526                	mv	a0,s1
    1562:	b581                	j	13a2 <printf+0x42>
        switch (s[1])
    1564:	07800713          	li	a4,120
    1568:	04e79963          	bne	a5,a4,15ba <printf+0x25a>
            printint(va_arg(ap, int), 16, 1);
    156c:	6782                	ld	a5,0(sp)
    156e:	45c1                	li	a1,16
    1570:	4388                	lw	a0,0(a5)
    1572:	07a1                	addi	a5,a5,8
    1574:	e03e                	sd	a5,0(sp)
    1576:	b71ff0ef          	jal	ra,10e6 <printint.constprop.0>
        s += 2;
    157a:	00248513          	addi	a0,s1,2
    157e:	bf51                	j	1512 <printf+0x1b2>
            printint(va_arg(ap, int), 10, 1);
    1580:	6782                	ld	a5,0(sp)
    1582:	45a9                	li	a1,10
    1584:	4388                	lw	a0,0(a5)
    1586:	07a1                	addi	a5,a5,8
    1588:	e03e                	sd	a5,0(sp)
    158a:	b5dff0ef          	jal	ra,10e6 <printint.constprop.0>
        s += 2;
    158e:	00248513          	addi	a0,s1,2
    1592:	b741                	j	1512 <printf+0x1b2>
            if ((a = va_arg(ap, char *)) == 0)
    1594:	6782                	ld	a5,0(sp)
    1596:	6380                	ld	s0,0(a5)
    1598:	07a1                	addi	a5,a5,8
    159a:	e03e                	sd	a5,0(sp)
    159c:	c031                	beqz	s0,15e0 <printf+0x280>
            l = strnlen(a, 200);
    159e:	0c800593          	li	a1,200
    15a2:	8522                	mv	a0,s0
    15a4:	40a000ef          	jal	ra,19ae <strnlen>
    write(f, s, l);
    15a8:	0005061b          	sext.w	a2,a0
    15ac:	85a2                	mv	a1,s0
    15ae:	4505                	li	a0,1
    15b0:	6f6000ef          	jal	ra,1ca6 <write>
        s += 2;
    15b4:	00248513          	addi	a0,s1,2
    15b8:	bfa9                	j	1512 <printf+0x1b2>
    return write(stdout, &byte, 1);
    15ba:	4605                	li	a2,1
    15bc:	002c                	addi	a1,sp,8
    15be:	4505                	li	a0,1
    char byte = c;
    15c0:	01210423          	sb	s2,8(sp)
    return write(stdout, &byte, 1);
    15c4:	6e2000ef          	jal	ra,1ca6 <write>
    char byte = c;
    15c8:	0014c783          	lbu	a5,1(s1)
    return write(stdout, &byte, 1);
    15cc:	4605                	li	a2,1
    15ce:	002c                	addi	a1,sp,8
    15d0:	4505                	li	a0,1
    char byte = c;
    15d2:	00f10423          	sb	a5,8(sp)
    return write(stdout, &byte, 1);
    15d6:	6d0000ef          	jal	ra,1ca6 <write>
        s += 2;
    15da:	00248513          	addi	a0,s1,2
    15de:	bf15                	j	1512 <printf+0x1b2>
                a = "(null)";
    15e0:	845a                	mv	s0,s6
    15e2:	bf75                	j	159e <printf+0x23e>

00000000000015e4 <panic>:
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

void panic(char *m)
{
    15e4:	1141                	addi	sp,sp,-16
    15e6:	e406                	sd	ra,8(sp)
    puts(m);
    15e8:	d57ff0ef          	jal	ra,133e <puts>
    exit(-100);
}
    15ec:	60a2                	ld	ra,8(sp)
    exit(-100);
    15ee:	f9c00513          	li	a0,-100
}
    15f2:	0141                	addi	sp,sp,16
    exit(-100);
    15f4:	a709                	j	1cf6 <exit>

00000000000015f6 <isspace>:
#define HIGHS (ONES * (UCHAR_MAX / 2 + 1))
#define HASZERO(x) (((x)-ONES) & ~(x)&HIGHS)

int isspace(int c)
{
    return c == ' ' || (unsigned)c - '\t' < 5;
    15f6:	02000793          	li	a5,32
    15fa:	00f50663          	beq	a0,a5,1606 <isspace+0x10>
    15fe:	355d                	addiw	a0,a0,-9
    1600:	00553513          	sltiu	a0,a0,5
    1604:	8082                	ret
    1606:	4505                	li	a0,1
}
    1608:	8082                	ret

000000000000160a <isdigit>:

int isdigit(int c)
{
    return (unsigned)c - '0' < 10;
    160a:	fd05051b          	addiw	a0,a0,-48
}
    160e:	00a53513          	sltiu	a0,a0,10
    1612:	8082                	ret

0000000000001614 <atoi>:
    return c == ' ' || (unsigned)c - '\t' < 5;
    1614:	02000613          	li	a2,32
    1618:	4591                	li	a1,4

int atoi(const char *s)
{
    int n = 0, neg = 0;
    while (isspace(*s))
    161a:	00054703          	lbu	a4,0(a0)
    return c == ' ' || (unsigned)c - '\t' < 5;
    161e:	ff77069b          	addiw	a3,a4,-9
    1622:	04c70d63          	beq	a4,a2,167c <atoi+0x68>
    1626:	0007079b          	sext.w	a5,a4
    162a:	04d5f963          	bgeu	a1,a3,167c <atoi+0x68>
        s++;
    switch (*s)
    162e:	02b00693          	li	a3,43
    1632:	04d70a63          	beq	a4,a3,1686 <atoi+0x72>
    1636:	02d00693          	li	a3,45
    163a:	06d70463          	beq	a4,a3,16a2 <atoi+0x8e>
        neg = 1;
    case '+':
        s++;
    }
    /* Compute n as a negative number to avoid overflow on INT_MIN */
    while (isdigit(*s))
    163e:	fd07859b          	addiw	a1,a5,-48
    1642:	4625                	li	a2,9
    1644:	873e                	mv	a4,a5
    1646:	86aa                	mv	a3,a0
    int n = 0, neg = 0;
    1648:	4e01                	li	t3,0
    while (isdigit(*s))
    164a:	04b66a63          	bltu	a2,a1,169e <atoi+0x8a>
    int n = 0, neg = 0;
    164e:	4501                	li	a0,0
    while (isdigit(*s))
    1650:	4825                	li	a6,9
    1652:	0016c603          	lbu	a2,1(a3)
        n = 10 * n - (*s++ - '0');
    1656:	0025179b          	slliw	a5,a0,0x2
    165a:	9d3d                	addw	a0,a0,a5
    165c:	fd07031b          	addiw	t1,a4,-48
    1660:	0015189b          	slliw	a7,a0,0x1
    while (isdigit(*s))
    1664:	fd06059b          	addiw	a1,a2,-48
        n = 10 * n - (*s++ - '0');
    1668:	0685                	addi	a3,a3,1
    166a:	4068853b          	subw	a0,a7,t1
    while (isdigit(*s))
    166e:	0006071b          	sext.w	a4,a2
    1672:	feb870e3          	bgeu	a6,a1,1652 <atoi+0x3e>
    return neg ? n : -n;
    1676:	000e0563          	beqz	t3,1680 <atoi+0x6c>
}
    167a:	8082                	ret
        s++;
    167c:	0505                	addi	a0,a0,1
    167e:	bf71                	j	161a <atoi+0x6>
    return neg ? n : -n;
    1680:	4113053b          	subw	a0,t1,a7
    1684:	8082                	ret
    while (isdigit(*s))
    1686:	00154783          	lbu	a5,1(a0)
    168a:	4625                	li	a2,9
        s++;
    168c:	00150693          	addi	a3,a0,1
    while (isdigit(*s))
    1690:	fd07859b          	addiw	a1,a5,-48
    1694:	0007871b          	sext.w	a4,a5
    int n = 0, neg = 0;
    1698:	4e01                	li	t3,0
    while (isdigit(*s))
    169a:	fab67ae3          	bgeu	a2,a1,164e <atoi+0x3a>
    169e:	4501                	li	a0,0
}
    16a0:	8082                	ret
    while (isdigit(*s))
    16a2:	00154783          	lbu	a5,1(a0)
    16a6:	4625                	li	a2,9
        s++;
    16a8:	00150693          	addi	a3,a0,1
    while (isdigit(*s))
    16ac:	fd07859b          	addiw	a1,a5,-48
    16b0:	0007871b          	sext.w	a4,a5
    16b4:	feb665e3          	bltu	a2,a1,169e <atoi+0x8a>
        neg = 1;
    16b8:	4e05                	li	t3,1
    16ba:	bf51                	j	164e <atoi+0x3a>

00000000000016bc <memset>:

void *memset(void *dest, int c, size_t n)
{
    char *p = dest;
    for (int i = 0; i < n; ++i, *(p++) = c)
    16bc:	16060d63          	beqz	a2,1836 <memset+0x17a>
    16c0:	40a007b3          	neg	a5,a0
    16c4:	8b9d                	andi	a5,a5,7
    16c6:	00778713          	addi	a4,a5,7
    16ca:	482d                	li	a6,11
    16cc:	0ff5f593          	andi	a1,a1,255
    16d0:	fff60693          	addi	a3,a2,-1
    16d4:	17076263          	bltu	a4,a6,1838 <memset+0x17c>
    16d8:	16e6ea63          	bltu	a3,a4,184c <memset+0x190>
    16dc:	16078563          	beqz	a5,1846 <memset+0x18a>
    16e0:	00b50023          	sb	a1,0(a0)
    16e4:	4705                	li	a4,1
    16e6:	00150e93          	addi	t4,a0,1
    16ea:	14e78c63          	beq	a5,a4,1842 <memset+0x186>
    16ee:	00b500a3          	sb	a1,1(a0)
    16f2:	4709                	li	a4,2
    16f4:	00250e93          	addi	t4,a0,2
    16f8:	14e78d63          	beq	a5,a4,1852 <memset+0x196>
    16fc:	00b50123          	sb	a1,2(a0)
    1700:	470d                	li	a4,3
    1702:	00350e93          	addi	t4,a0,3
    1706:	12e78b63          	beq	a5,a4,183c <memset+0x180>
    170a:	00b501a3          	sb	a1,3(a0)
    170e:	4711                	li	a4,4
    1710:	00450e93          	addi	t4,a0,4
    1714:	14e78163          	beq	a5,a4,1856 <memset+0x19a>
    1718:	00b50223          	sb	a1,4(a0)
    171c:	4715                	li	a4,5
    171e:	00550e93          	addi	t4,a0,5
    1722:	12e78c63          	beq	a5,a4,185a <memset+0x19e>
    1726:	00b502a3          	sb	a1,5(a0)
    172a:	471d                	li	a4,7
    172c:	00650e93          	addi	t4,a0,6
    1730:	12e79763          	bne	a5,a4,185e <memset+0x1a2>
    1734:	00750e93          	addi	t4,a0,7
    1738:	00b50323          	sb	a1,6(a0)
    173c:	4f1d                	li	t5,7
    173e:	00859713          	slli	a4,a1,0x8
    1742:	8f4d                	or	a4,a4,a1
    1744:	01059e13          	slli	t3,a1,0x10
    1748:	01c76e33          	or	t3,a4,t3
    174c:	01859313          	slli	t1,a1,0x18
    1750:	006e6333          	or	t1,t3,t1
    1754:	02059893          	slli	a7,a1,0x20
    1758:	011368b3          	or	a7,t1,a7
    175c:	02859813          	slli	a6,a1,0x28
    1760:	40f60333          	sub	t1,a2,a5
    1764:	0108e833          	or	a6,a7,a6
    1768:	03059693          	slli	a3,a1,0x30
    176c:	00d866b3          	or	a3,a6,a3
    1770:	03859713          	slli	a4,a1,0x38
    1774:	97aa                	add	a5,a5,a0
    1776:	ff837813          	andi	a6,t1,-8
    177a:	8f55                	or	a4,a4,a3
    177c:	00f806b3          	add	a3,a6,a5
    1780:	e398                	sd	a4,0(a5)
    1782:	07a1                	addi	a5,a5,8
    1784:	fed79ee3          	bne	a5,a3,1780 <memset+0xc4>
    1788:	ff837693          	andi	a3,t1,-8
    178c:	00de87b3          	add	a5,t4,a3
    1790:	01e6873b          	addw	a4,a3,t5
    1794:	0ad30663          	beq	t1,a3,1840 <memset+0x184>
    1798:	00b78023          	sb	a1,0(a5)
    179c:	0017069b          	addiw	a3,a4,1
    17a0:	08c6fb63          	bgeu	a3,a2,1836 <memset+0x17a>
    17a4:	00b780a3          	sb	a1,1(a5)
    17a8:	0027069b          	addiw	a3,a4,2
    17ac:	08c6f563          	bgeu	a3,a2,1836 <memset+0x17a>
    17b0:	00b78123          	sb	a1,2(a5)
    17b4:	0037069b          	addiw	a3,a4,3
    17b8:	06c6ff63          	bgeu	a3,a2,1836 <memset+0x17a>
    17bc:	00b781a3          	sb	a1,3(a5)
    17c0:	0047069b          	addiw	a3,a4,4
    17c4:	06c6f963          	bgeu	a3,a2,1836 <memset+0x17a>
    17c8:	00b78223          	sb	a1,4(a5)
    17cc:	0057069b          	addiw	a3,a4,5
    17d0:	06c6f363          	bgeu	a3,a2,1836 <memset+0x17a>
    17d4:	00b782a3          	sb	a1,5(a5)
    17d8:	0067069b          	addiw	a3,a4,6
    17dc:	04c6fd63          	bgeu	a3,a2,1836 <memset+0x17a>
    17e0:	00b78323          	sb	a1,6(a5)
    17e4:	0077069b          	addiw	a3,a4,7
    17e8:	04c6f763          	bgeu	a3,a2,1836 <memset+0x17a>
    17ec:	00b783a3          	sb	a1,7(a5)
    17f0:	0087069b          	addiw	a3,a4,8
    17f4:	04c6f163          	bgeu	a3,a2,1836 <memset+0x17a>
    17f8:	00b78423          	sb	a1,8(a5)
    17fc:	0097069b          	addiw	a3,a4,9
    1800:	02c6fb63          	bgeu	a3,a2,1836 <memset+0x17a>
    1804:	00b784a3          	sb	a1,9(a5)
    1808:	00a7069b          	addiw	a3,a4,10
    180c:	02c6f563          	bgeu	a3,a2,1836 <memset+0x17a>
    1810:	00b78523          	sb	a1,10(a5)
    1814:	00b7069b          	addiw	a3,a4,11
    1818:	00c6ff63          	bgeu	a3,a2,1836 <memset+0x17a>
    181c:	00b785a3          	sb	a1,11(a5)
    1820:	00c7069b          	addiw	a3,a4,12
    1824:	00c6f963          	bgeu	a3,a2,1836 <memset+0x17a>
    1828:	00b78623          	sb	a1,12(a5)
    182c:	2735                	addiw	a4,a4,13
    182e:	00c77463          	bgeu	a4,a2,1836 <memset+0x17a>
    1832:	00b786a3          	sb	a1,13(a5)
        ;
    return dest;
}
    1836:	8082                	ret
    1838:	472d                	li	a4,11
    183a:	bd79                	j	16d8 <memset+0x1c>
    for (int i = 0; i < n; ++i, *(p++) = c)
    183c:	4f0d                	li	t5,3
    183e:	b701                	j	173e <memset+0x82>
    1840:	8082                	ret
    1842:	4f05                	li	t5,1
    1844:	bded                	j	173e <memset+0x82>
    1846:	8eaa                	mv	t4,a0
    1848:	4f01                	li	t5,0
    184a:	bdd5                	j	173e <memset+0x82>
    184c:	87aa                	mv	a5,a0
    184e:	4701                	li	a4,0
    1850:	b7a1                	j	1798 <memset+0xdc>
    1852:	4f09                	li	t5,2
    1854:	b5ed                	j	173e <memset+0x82>
    1856:	4f11                	li	t5,4
    1858:	b5dd                	j	173e <memset+0x82>
    185a:	4f15                	li	t5,5
    185c:	b5cd                	j	173e <memset+0x82>
    185e:	4f19                	li	t5,6
    1860:	bdf9                	j	173e <memset+0x82>

0000000000001862 <strcmp>:

int strcmp(const char *l, const char *r)
{
    for (; *l == *r && *l; l++, r++)
    1862:	00054783          	lbu	a5,0(a0)
    1866:	0005c703          	lbu	a4,0(a1)
    186a:	00e79863          	bne	a5,a4,187a <strcmp+0x18>
    186e:	0505                	addi	a0,a0,1
    1870:	0585                	addi	a1,a1,1
    1872:	fbe5                	bnez	a5,1862 <strcmp>
    1874:	4501                	li	a0,0
        ;
    return *(unsigned char *)l - *(unsigned char *)r;
}
    1876:	9d19                	subw	a0,a0,a4
    1878:	8082                	ret
    return *(unsigned char *)l - *(unsigned char *)r;
    187a:	0007851b          	sext.w	a0,a5
    187e:	bfe5                	j	1876 <strcmp+0x14>

0000000000001880 <strncmp>:

int strncmp(const char *_l, const char *_r, size_t n)
{
    const unsigned char *l = (void *)_l, *r = (void *)_r;
    if (!n--)
    1880:	ce05                	beqz	a2,18b8 <strncmp+0x38>
        return 0;
    for (; *l && *r && n && *l == *r; l++, r++, n--)
    1882:	00054703          	lbu	a4,0(a0)
    1886:	0005c783          	lbu	a5,0(a1)
    188a:	cb0d                	beqz	a4,18bc <strncmp+0x3c>
    if (!n--)
    188c:	167d                	addi	a2,a2,-1
    188e:	00c506b3          	add	a3,a0,a2
    1892:	a819                	j	18a8 <strncmp+0x28>
    for (; *l && *r && n && *l == *r; l++, r++, n--)
    1894:	00a68e63          	beq	a3,a0,18b0 <strncmp+0x30>
    1898:	0505                	addi	a0,a0,1
    189a:	00e79b63          	bne	a5,a4,18b0 <strncmp+0x30>
    189e:	00054703          	lbu	a4,0(a0)
        ;
    return *l - *r;
    18a2:	0005c783          	lbu	a5,0(a1)
    for (; *l && *r && n && *l == *r; l++, r++, n--)
    18a6:	cb19                	beqz	a4,18bc <strncmp+0x3c>
    18a8:	0005c783          	lbu	a5,0(a1)
    18ac:	0585                	addi	a1,a1,1
    18ae:	f3fd                	bnez	a5,1894 <strncmp+0x14>
    return *l - *r;
    18b0:	0007051b          	sext.w	a0,a4
    18b4:	9d1d                	subw	a0,a0,a5
    18b6:	8082                	ret
        return 0;
    18b8:	4501                	li	a0,0
}
    18ba:	8082                	ret
    18bc:	4501                	li	a0,0
    return *l - *r;
    18be:	9d1d                	subw	a0,a0,a5
    18c0:	8082                	ret

00000000000018c2 <strlen>:
size_t strlen(const char *s)
{
    const char *a = s;
    typedef size_t __attribute__((__may_alias__)) word;
    const word *w;
    for (; (uintptr_t)s % SS; s++)
    18c2:	00757793          	andi	a5,a0,7
    18c6:	cf89                	beqz	a5,18e0 <strlen+0x1e>
    18c8:	87aa                	mv	a5,a0
    18ca:	a029                	j	18d4 <strlen+0x12>
    18cc:	0785                	addi	a5,a5,1
    18ce:	0077f713          	andi	a4,a5,7
    18d2:	cb01                	beqz	a4,18e2 <strlen+0x20>
        if (!*s)
    18d4:	0007c703          	lbu	a4,0(a5)
    18d8:	fb75                	bnez	a4,18cc <strlen+0xa>
    for (w = (const void *)s; !HASZERO(*w); w++)
        ;
    s = (const void *)w;
    for (; *s; s++)
        ;
    return s - a;
    18da:	40a78533          	sub	a0,a5,a0
}
    18de:	8082                	ret
    for (; (uintptr_t)s % SS; s++)
    18e0:	87aa                	mv	a5,a0
    for (w = (const void *)s; !HASZERO(*w); w++)
    18e2:	6394                	ld	a3,0(a5)
    18e4:	00000597          	auipc	a1,0x0
    18e8:	6dc5b583          	ld	a1,1756(a1) # 1fc0 <__clone+0xe6>
    18ec:	00000617          	auipc	a2,0x0
    18f0:	6dc63603          	ld	a2,1756(a2) # 1fc8 <__clone+0xee>
    18f4:	a019                	j	18fa <strlen+0x38>
    18f6:	6794                	ld	a3,8(a5)
    18f8:	07a1                	addi	a5,a5,8
    18fa:	00b68733          	add	a4,a3,a1
    18fe:	fff6c693          	not	a3,a3
    1902:	8f75                	and	a4,a4,a3
    1904:	8f71                	and	a4,a4,a2
    1906:	db65                	beqz	a4,18f6 <strlen+0x34>
    for (; *s; s++)
    1908:	0007c703          	lbu	a4,0(a5)
    190c:	d779                	beqz	a4,18da <strlen+0x18>
    190e:	0017c703          	lbu	a4,1(a5)
    1912:	0785                	addi	a5,a5,1
    1914:	d379                	beqz	a4,18da <strlen+0x18>
    1916:	0017c703          	lbu	a4,1(a5)
    191a:	0785                	addi	a5,a5,1
    191c:	fb6d                	bnez	a4,190e <strlen+0x4c>
    191e:	bf75                	j	18da <strlen+0x18>

0000000000001920 <memchr>:

void *memchr(const void *src, int c, size_t n)
{
    const unsigned char *s = src;
    c = (unsigned char)c;
    for (; ((uintptr_t)s & ALIGN) && n && *s != c; s++, n--)
    1920:	00757713          	andi	a4,a0,7
{
    1924:	87aa                	mv	a5,a0
    c = (unsigned char)c;
    1926:	0ff5f593          	andi	a1,a1,255
    for (; ((uintptr_t)s & ALIGN) && n && *s != c; s++, n--)
    192a:	cb19                	beqz	a4,1940 <memchr+0x20>
    192c:	ce25                	beqz	a2,19a4 <memchr+0x84>
    192e:	0007c703          	lbu	a4,0(a5)
    1932:	04b70e63          	beq	a4,a1,198e <memchr+0x6e>
    1936:	0785                	addi	a5,a5,1
    1938:	0077f713          	andi	a4,a5,7
    193c:	167d                	addi	a2,a2,-1
    193e:	f77d                	bnez	a4,192c <memchr+0xc>
            ;
        s = (const void *)w;
    }
    for (; n && *s != c; s++, n--)
        ;
    return n ? (void *)s : 0;
    1940:	4501                	li	a0,0
    if (n && *s != c)
    1942:	c235                	beqz	a2,19a6 <memchr+0x86>
    1944:	0007c703          	lbu	a4,0(a5)
    1948:	04b70363          	beq	a4,a1,198e <memchr+0x6e>
        size_t k = ONES * c;
    194c:	00000517          	auipc	a0,0x0
    1950:	68453503          	ld	a0,1668(a0) # 1fd0 <__clone+0xf6>
        for (w = (const void *)s; n >= SS && !HASZERO(*w ^ k); w++, n -= SS)
    1954:	471d                	li	a4,7
        size_t k = ONES * c;
    1956:	02a58533          	mul	a0,a1,a0
        for (w = (const void *)s; n >= SS && !HASZERO(*w ^ k); w++, n -= SS)
    195a:	02c77a63          	bgeu	a4,a2,198e <memchr+0x6e>
    195e:	00000897          	auipc	a7,0x0
    1962:	6628b883          	ld	a7,1634(a7) # 1fc0 <__clone+0xe6>
    1966:	00000817          	auipc	a6,0x0
    196a:	66283803          	ld	a6,1634(a6) # 1fc8 <__clone+0xee>
    196e:	431d                	li	t1,7
    1970:	a029                	j	197a <memchr+0x5a>
    1972:	1661                	addi	a2,a2,-8
    1974:	07a1                	addi	a5,a5,8
    1976:	02c37963          	bgeu	t1,a2,19a8 <memchr+0x88>
    197a:	6398                	ld	a4,0(a5)
    197c:	8f29                	xor	a4,a4,a0
    197e:	011706b3          	add	a3,a4,a7
    1982:	fff74713          	not	a4,a4
    1986:	8f75                	and	a4,a4,a3
    1988:	01077733          	and	a4,a4,a6
    198c:	d37d                	beqz	a4,1972 <memchr+0x52>
    198e:	853e                	mv	a0,a5
    1990:	97b2                	add	a5,a5,a2
    1992:	a021                	j	199a <memchr+0x7a>
    for (; n && *s != c; s++, n--)
    1994:	0505                	addi	a0,a0,1
    1996:	00f50763          	beq	a0,a5,19a4 <memchr+0x84>
    199a:	00054703          	lbu	a4,0(a0)
    199e:	feb71be3          	bne	a4,a1,1994 <memchr+0x74>
    19a2:	8082                	ret
    return n ? (void *)s : 0;
    19a4:	4501                	li	a0,0
}
    19a6:	8082                	ret
    return n ? (void *)s : 0;
    19a8:	4501                	li	a0,0
    for (; n && *s != c; s++, n--)
    19aa:	f275                	bnez	a2,198e <memchr+0x6e>
}
    19ac:	8082                	ret

00000000000019ae <strnlen>:

size_t strnlen(const char *s, size_t n)
{
    19ae:	1101                	addi	sp,sp,-32
    19b0:	e822                	sd	s0,16(sp)
    const char *p = memchr(s, 0, n);
    19b2:	862e                	mv	a2,a1
{
    19b4:	842e                	mv	s0,a1
    const char *p = memchr(s, 0, n);
    19b6:	4581                	li	a1,0
{
    19b8:	e426                	sd	s1,8(sp)
    19ba:	ec06                	sd	ra,24(sp)
    19bc:	84aa                	mv	s1,a0
    const char *p = memchr(s, 0, n);
    19be:	f63ff0ef          	jal	ra,1920 <memchr>
    return p ? p - s : n;
    19c2:	c519                	beqz	a0,19d0 <strnlen+0x22>
}
    19c4:	60e2                	ld	ra,24(sp)
    19c6:	6442                	ld	s0,16(sp)
    return p ? p - s : n;
    19c8:	8d05                	sub	a0,a0,s1
}
    19ca:	64a2                	ld	s1,8(sp)
    19cc:	6105                	addi	sp,sp,32
    19ce:	8082                	ret
    19d0:	60e2                	ld	ra,24(sp)
    return p ? p - s : n;
    19d2:	8522                	mv	a0,s0
}
    19d4:	6442                	ld	s0,16(sp)
    19d6:	64a2                	ld	s1,8(sp)
    19d8:	6105                	addi	sp,sp,32
    19da:	8082                	ret

00000000000019dc <strcpy>:
char *strcpy(char *restrict d, const char *s)
{
    typedef size_t __attribute__((__may_alias__)) word;
    word *wd;
    const word *ws;
    if ((uintptr_t)s % SS == (uintptr_t)d % SS)
    19dc:	00b547b3          	xor	a5,a0,a1
    19e0:	8b9d                	andi	a5,a5,7
    19e2:	eb95                	bnez	a5,1a16 <strcpy+0x3a>
    {
        for (; (uintptr_t)s % SS; s++, d++)
    19e4:	0075f793          	andi	a5,a1,7
    19e8:	e7b1                	bnez	a5,1a34 <strcpy+0x58>
            if (!(*d = *s))
                return d;
        wd = (void *)d;
        ws = (const void *)s;
        for (; !HASZERO(*ws); *wd++ = *ws++)
    19ea:	6198                	ld	a4,0(a1)
    19ec:	00000617          	auipc	a2,0x0
    19f0:	5d463603          	ld	a2,1492(a2) # 1fc0 <__clone+0xe6>
    19f4:	00000817          	auipc	a6,0x0
    19f8:	5d483803          	ld	a6,1492(a6) # 1fc8 <__clone+0xee>
    19fc:	a029                	j	1a06 <strcpy+0x2a>
    19fe:	e118                	sd	a4,0(a0)
    1a00:	6598                	ld	a4,8(a1)
    1a02:	05a1                	addi	a1,a1,8
    1a04:	0521                	addi	a0,a0,8
    1a06:	00c707b3          	add	a5,a4,a2
    1a0a:	fff74693          	not	a3,a4
    1a0e:	8ff5                	and	a5,a5,a3
    1a10:	0107f7b3          	and	a5,a5,a6
    1a14:	d7ed                	beqz	a5,19fe <strcpy+0x22>
            ;
        d = (void *)wd;
        s = (const void *)ws;
    }
    for (; (*d = *s); s++, d++)
    1a16:	0005c783          	lbu	a5,0(a1)
    1a1a:	00f50023          	sb	a5,0(a0)
    1a1e:	c785                	beqz	a5,1a46 <strcpy+0x6a>
    1a20:	0015c783          	lbu	a5,1(a1)
    1a24:	0505                	addi	a0,a0,1
    1a26:	0585                	addi	a1,a1,1
    1a28:	00f50023          	sb	a5,0(a0)
    1a2c:	fbf5                	bnez	a5,1a20 <strcpy+0x44>
        ;
    return d;
}
    1a2e:	8082                	ret
        for (; (uintptr_t)s % SS; s++, d++)
    1a30:	0505                	addi	a0,a0,1
    1a32:	df45                	beqz	a4,19ea <strcpy+0xe>
            if (!(*d = *s))
    1a34:	0005c783          	lbu	a5,0(a1)
        for (; (uintptr_t)s % SS; s++, d++)
    1a38:	0585                	addi	a1,a1,1
    1a3a:	0075f713          	andi	a4,a1,7
            if (!(*d = *s))
    1a3e:	00f50023          	sb	a5,0(a0)
    1a42:	f7fd                	bnez	a5,1a30 <strcpy+0x54>
}
    1a44:	8082                	ret
    1a46:	8082                	ret

0000000000001a48 <strncpy>:
char *strncpy(char *restrict d, const char *s, size_t n)
{
    typedef size_t __attribute__((__may_alias__)) word;
    word *wd;
    const word *ws;
    if (((uintptr_t)s & ALIGN) == ((uintptr_t)d & ALIGN))
    1a48:	00b547b3          	xor	a5,a0,a1
    1a4c:	8b9d                	andi	a5,a5,7
    1a4e:	1a079863          	bnez	a5,1bfe <strncpy+0x1b6>
    {
        for (; ((uintptr_t)s & ALIGN) && n && (*d = *s); n--, s++, d++)
    1a52:	0075f793          	andi	a5,a1,7
    1a56:	16078463          	beqz	a5,1bbe <strncpy+0x176>
    1a5a:	ea01                	bnez	a2,1a6a <strncpy+0x22>
    1a5c:	a421                	j	1c64 <strncpy+0x21c>
    1a5e:	167d                	addi	a2,a2,-1
    1a60:	0505                	addi	a0,a0,1
    1a62:	14070e63          	beqz	a4,1bbe <strncpy+0x176>
    1a66:	1a060863          	beqz	a2,1c16 <strncpy+0x1ce>
    1a6a:	0005c783          	lbu	a5,0(a1)
    1a6e:	0585                	addi	a1,a1,1
    1a70:	0075f713          	andi	a4,a1,7
    1a74:	00f50023          	sb	a5,0(a0)
    1a78:	f3fd                	bnez	a5,1a5e <strncpy+0x16>
    1a7a:	4805                	li	a6,1
    1a7c:	1a061863          	bnez	a2,1c2c <strncpy+0x1e4>
    1a80:	40a007b3          	neg	a5,a0
    1a84:	8b9d                	andi	a5,a5,7
    1a86:	4681                	li	a3,0
    1a88:	18061a63          	bnez	a2,1c1c <strncpy+0x1d4>
    1a8c:	00778713          	addi	a4,a5,7
    1a90:	45ad                	li	a1,11
    1a92:	18b76363          	bltu	a4,a1,1c18 <strncpy+0x1d0>
    1a96:	1ae6eb63          	bltu	a3,a4,1c4c <strncpy+0x204>
    1a9a:	1a078363          	beqz	a5,1c40 <strncpy+0x1f8>
    for (int i = 0; i < n; ++i, *(p++) = c)
    1a9e:	00050023          	sb	zero,0(a0)
    1aa2:	4685                	li	a3,1
    1aa4:	00150713          	addi	a4,a0,1
    1aa8:	18d78f63          	beq	a5,a3,1c46 <strncpy+0x1fe>
    1aac:	000500a3          	sb	zero,1(a0)
    1ab0:	4689                	li	a3,2
    1ab2:	00250713          	addi	a4,a0,2
    1ab6:	18d78e63          	beq	a5,a3,1c52 <strncpy+0x20a>
    1aba:	00050123          	sb	zero,2(a0)
    1abe:	468d                	li	a3,3
    1ac0:	00350713          	addi	a4,a0,3
    1ac4:	16d78c63          	beq	a5,a3,1c3c <strncpy+0x1f4>
    1ac8:	000501a3          	sb	zero,3(a0)
    1acc:	4691                	li	a3,4
    1ace:	00450713          	addi	a4,a0,4
    1ad2:	18d78263          	beq	a5,a3,1c56 <strncpy+0x20e>
    1ad6:	00050223          	sb	zero,4(a0)
    1ada:	4695                	li	a3,5
    1adc:	00550713          	addi	a4,a0,5
    1ae0:	16d78d63          	beq	a5,a3,1c5a <strncpy+0x212>
    1ae4:	000502a3          	sb	zero,5(a0)
    1ae8:	469d                	li	a3,7
    1aea:	00650713          	addi	a4,a0,6
    1aee:	16d79863          	bne	a5,a3,1c5e <strncpy+0x216>
    1af2:	00750713          	addi	a4,a0,7
    1af6:	00050323          	sb	zero,6(a0)
    1afa:	40f80833          	sub	a6,a6,a5
    1afe:	ff887593          	andi	a1,a6,-8
    1b02:	97aa                	add	a5,a5,a0
    1b04:	95be                	add	a1,a1,a5
    1b06:	0007b023          	sd	zero,0(a5)
    1b0a:	07a1                	addi	a5,a5,8
    1b0c:	feb79de3          	bne	a5,a1,1b06 <strncpy+0xbe>
    1b10:	ff887593          	andi	a1,a6,-8
    1b14:	9ead                	addw	a3,a3,a1
    1b16:	00b707b3          	add	a5,a4,a1
    1b1a:	12b80863          	beq	a6,a1,1c4a <strncpy+0x202>
    1b1e:	00078023          	sb	zero,0(a5)
    1b22:	0016871b          	addiw	a4,a3,1
    1b26:	0ec77863          	bgeu	a4,a2,1c16 <strncpy+0x1ce>
    1b2a:	000780a3          	sb	zero,1(a5)
    1b2e:	0026871b          	addiw	a4,a3,2
    1b32:	0ec77263          	bgeu	a4,a2,1c16 <strncpy+0x1ce>
    1b36:	00078123          	sb	zero,2(a5)
    1b3a:	0036871b          	addiw	a4,a3,3
    1b3e:	0cc77c63          	bgeu	a4,a2,1c16 <strncpy+0x1ce>
    1b42:	000781a3          	sb	zero,3(a5)
    1b46:	0046871b          	addiw	a4,a3,4
    1b4a:	0cc77663          	bgeu	a4,a2,1c16 <strncpy+0x1ce>
    1b4e:	00078223          	sb	zero,4(a5)
    1b52:	0056871b          	addiw	a4,a3,5
    1b56:	0cc77063          	bgeu	a4,a2,1c16 <strncpy+0x1ce>
    1b5a:	000782a3          	sb	zero,5(a5)
    1b5e:	0066871b          	addiw	a4,a3,6
    1b62:	0ac77a63          	bgeu	a4,a2,1c16 <strncpy+0x1ce>
    1b66:	00078323          	sb	zero,6(a5)
    1b6a:	0076871b          	addiw	a4,a3,7
    1b6e:	0ac77463          	bgeu	a4,a2,1c16 <strncpy+0x1ce>
    1b72:	000783a3          	sb	zero,7(a5)
    1b76:	0086871b          	addiw	a4,a3,8
    1b7a:	08c77e63          	bgeu	a4,a2,1c16 <strncpy+0x1ce>
    1b7e:	00078423          	sb	zero,8(a5)
    1b82:	0096871b          	addiw	a4,a3,9
    1b86:	08c77863          	bgeu	a4,a2,1c16 <strncpy+0x1ce>
    1b8a:	000784a3          	sb	zero,9(a5)
    1b8e:	00a6871b          	addiw	a4,a3,10
    1b92:	08c77263          	bgeu	a4,a2,1c16 <strncpy+0x1ce>
    1b96:	00078523          	sb	zero,10(a5)
    1b9a:	00b6871b          	addiw	a4,a3,11
    1b9e:	06c77c63          	bgeu	a4,a2,1c16 <strncpy+0x1ce>
    1ba2:	000785a3          	sb	zero,11(a5)
    1ba6:	00c6871b          	addiw	a4,a3,12
    1baa:	06c77663          	bgeu	a4,a2,1c16 <strncpy+0x1ce>
    1bae:	00078623          	sb	zero,12(a5)
    1bb2:	26b5                	addiw	a3,a3,13
    1bb4:	06c6f163          	bgeu	a3,a2,1c16 <strncpy+0x1ce>
    1bb8:	000786a3          	sb	zero,13(a5)
    1bbc:	8082                	ret
            ;
        if (!n || !*s)
    1bbe:	c645                	beqz	a2,1c66 <strncpy+0x21e>
    1bc0:	0005c783          	lbu	a5,0(a1)
    1bc4:	ea078be3          	beqz	a5,1a7a <strncpy+0x32>
            goto tail;
        wd = (void *)d;
        ws = (const void *)s;
        for (; n >= sizeof(size_t) && !HASZERO(*ws); n -= sizeof(size_t), ws++, wd++)
    1bc8:	479d                	li	a5,7
    1bca:	02c7ff63          	bgeu	a5,a2,1c08 <strncpy+0x1c0>
    1bce:	00000897          	auipc	a7,0x0
    1bd2:	3f28b883          	ld	a7,1010(a7) # 1fc0 <__clone+0xe6>
    1bd6:	00000817          	auipc	a6,0x0
    1bda:	3f283803          	ld	a6,1010(a6) # 1fc8 <__clone+0xee>
    1bde:	431d                	li	t1,7
    1be0:	6198                	ld	a4,0(a1)
    1be2:	011707b3          	add	a5,a4,a7
    1be6:	fff74693          	not	a3,a4
    1bea:	8ff5                	and	a5,a5,a3
    1bec:	0107f7b3          	and	a5,a5,a6
    1bf0:	ef81                	bnez	a5,1c08 <strncpy+0x1c0>
            *wd = *ws;
    1bf2:	e118                	sd	a4,0(a0)
        for (; n >= sizeof(size_t) && !HASZERO(*ws); n -= sizeof(size_t), ws++, wd++)
    1bf4:	1661                	addi	a2,a2,-8
    1bf6:	05a1                	addi	a1,a1,8
    1bf8:	0521                	addi	a0,a0,8
    1bfa:	fec363e3          	bltu	t1,a2,1be0 <strncpy+0x198>
        d = (void *)wd;
        s = (const void *)ws;
    }
    for (; n && (*d = *s); n--, s++, d++)
    1bfe:	e609                	bnez	a2,1c08 <strncpy+0x1c0>
    1c00:	a08d                	j	1c62 <strncpy+0x21a>
    1c02:	167d                	addi	a2,a2,-1
    1c04:	0505                	addi	a0,a0,1
    1c06:	ca01                	beqz	a2,1c16 <strncpy+0x1ce>
    1c08:	0005c783          	lbu	a5,0(a1)
    1c0c:	0585                	addi	a1,a1,1
    1c0e:	00f50023          	sb	a5,0(a0)
    1c12:	fbe5                	bnez	a5,1c02 <strncpy+0x1ba>
        ;
tail:
    1c14:	b59d                	j	1a7a <strncpy+0x32>
    memset(d, 0, n);
    return d;
}
    1c16:	8082                	ret
    1c18:	472d                	li	a4,11
    1c1a:	bdb5                	j	1a96 <strncpy+0x4e>
    1c1c:	00778713          	addi	a4,a5,7
    1c20:	45ad                	li	a1,11
    1c22:	fff60693          	addi	a3,a2,-1
    1c26:	e6b778e3          	bgeu	a4,a1,1a96 <strncpy+0x4e>
    1c2a:	b7fd                	j	1c18 <strncpy+0x1d0>
    1c2c:	40a007b3          	neg	a5,a0
    1c30:	8832                	mv	a6,a2
    1c32:	8b9d                	andi	a5,a5,7
    1c34:	4681                	li	a3,0
    1c36:	e4060be3          	beqz	a2,1a8c <strncpy+0x44>
    1c3a:	b7cd                	j	1c1c <strncpy+0x1d4>
    for (int i = 0; i < n; ++i, *(p++) = c)
    1c3c:	468d                	li	a3,3
    1c3e:	bd75                	j	1afa <strncpy+0xb2>
    1c40:	872a                	mv	a4,a0
    1c42:	4681                	li	a3,0
    1c44:	bd5d                	j	1afa <strncpy+0xb2>
    1c46:	4685                	li	a3,1
    1c48:	bd4d                	j	1afa <strncpy+0xb2>
    1c4a:	8082                	ret
    1c4c:	87aa                	mv	a5,a0
    1c4e:	4681                	li	a3,0
    1c50:	b5f9                	j	1b1e <strncpy+0xd6>
    1c52:	4689                	li	a3,2
    1c54:	b55d                	j	1afa <strncpy+0xb2>
    1c56:	4691                	li	a3,4
    1c58:	b54d                	j	1afa <strncpy+0xb2>
    1c5a:	4695                	li	a3,5
    1c5c:	bd79                	j	1afa <strncpy+0xb2>
    1c5e:	4699                	li	a3,6
    1c60:	bd69                	j	1afa <strncpy+0xb2>
    1c62:	8082                	ret
    1c64:	8082                	ret
    1c66:	8082                	ret

0000000000001c68 <open>:
#include <unistd.h>

#include "syscall.h"

int open(const char *path, int flags)
{
    1c68:	87aa                	mv	a5,a0
    1c6a:	862e                	mv	a2,a1
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
}

static inline long __syscall4(long n, long a, long b, long c, long d)
{
    register long a7 __asm__("a7") = n;
    1c6c:	03800893          	li	a7,56
    register long a0 __asm__("a0") = a;
    1c70:	f9c00513          	li	a0,-100
    register long a1 __asm__("a1") = b;
    1c74:	85be                	mv	a1,a5
    register long a2 __asm__("a2") = c;
    register long a3 __asm__("a3") = d;
    1c76:	4689                	li	a3,2
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3))
    1c78:	00000073          	ecall
    return syscall(SYS_openat, AT_FDCWD, path, flags, O_RDWR);
}
    1c7c:	2501                	sext.w	a0,a0
    1c7e:	8082                	ret

0000000000001c80 <openat>:
    register long a7 __asm__("a7") = n;
    1c80:	03800893          	li	a7,56
    register long a3 __asm__("a3") = d;
    1c84:	18000693          	li	a3,384
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3))
    1c88:	00000073          	ecall

int openat(int dirfd,const char *path, int flags)
{
    return syscall(SYS_openat, dirfd, path, flags, 0600);
}
    1c8c:	2501                	sext.w	a0,a0
    1c8e:	8082                	ret

0000000000001c90 <close>:
    register long a7 __asm__("a7") = n;
    1c90:	03900893          	li	a7,57
    __asm_syscall("r"(a7), "0"(a0))
    1c94:	00000073          	ecall

int close(int fd)
{
    return syscall(SYS_close, fd);
}
    1c98:	2501                	sext.w	a0,a0
    1c9a:	8082                	ret

0000000000001c9c <read>:
    register long a7 __asm__("a7") = n;
    1c9c:	03f00893          	li	a7,63
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1ca0:	00000073          	ecall

ssize_t read(int fd, void *buf, size_t len)
{
    return syscall(SYS_read, fd, buf, len);
}
    1ca4:	8082                	ret

0000000000001ca6 <write>:
    register long a7 __asm__("a7") = n;
    1ca6:	04000893          	li	a7,64
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1caa:	00000073          	ecall

ssize_t write(int fd, const void *buf, size_t len)
{
    return syscall(SYS_write, fd, buf, len);
}
    1cae:	8082                	ret

0000000000001cb0 <getpid>:
    register long a7 __asm__("a7") = n;
    1cb0:	0ac00893          	li	a7,172
    __asm_syscall("r"(a7))
    1cb4:	00000073          	ecall

pid_t getpid(void)
{
    return syscall(SYS_getpid);
}
    1cb8:	2501                	sext.w	a0,a0
    1cba:	8082                	ret

0000000000001cbc <getppid>:
    register long a7 __asm__("a7") = n;
    1cbc:	0ad00893          	li	a7,173
    __asm_syscall("r"(a7))
    1cc0:	00000073          	ecall

pid_t getppid(void)
{
    return syscall(SYS_getppid);
}
    1cc4:	2501                	sext.w	a0,a0
    1cc6:	8082                	ret

0000000000001cc8 <sched_yield>:
    register long a7 __asm__("a7") = n;
    1cc8:	07c00893          	li	a7,124
    __asm_syscall("r"(a7))
    1ccc:	00000073          	ecall

int sched_yield(void)
{
    return syscall(SYS_sched_yield);
}
    1cd0:	2501                	sext.w	a0,a0
    1cd2:	8082                	ret

0000000000001cd4 <fork>:
    register long a7 __asm__("a7") = n;
    1cd4:	0dc00893          	li	a7,220
    register long a0 __asm__("a0") = a;
    1cd8:	4545                	li	a0,17
    register long a1 __asm__("a1") = b;
    1cda:	4581                	li	a1,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1cdc:	00000073          	ecall

pid_t fork(void)
{
    return syscall(SYS_clone, SIGCHLD, 0);
}
    1ce0:	2501                	sext.w	a0,a0
    1ce2:	8082                	ret

0000000000001ce4 <clone>:

pid_t clone(int (*fn)(void *arg), void *arg, void *stack, size_t stack_size, unsigned long flags)
{
    1ce4:	85b2                	mv	a1,a2
    1ce6:	863a                	mv	a2,a4
    if (stack)
    1ce8:	c191                	beqz	a1,1cec <clone+0x8>
	stack += stack_size;
    1cea:	95b6                	add	a1,a1,a3

    return __clone(fn, stack, flags, NULL, NULL, NULL);
    1cec:	4781                	li	a5,0
    1cee:	4701                	li	a4,0
    1cf0:	4681                	li	a3,0
    1cf2:	2601                	sext.w	a2,a2
    1cf4:	a2dd                	j	1eda <__clone>

0000000000001cf6 <exit>:
    register long a7 __asm__("a7") = n;
    1cf6:	05d00893          	li	a7,93
    __asm_syscall("r"(a7), "0"(a0))
    1cfa:	00000073          	ecall
    //return syscall(SYS_clone, fn, stack, flags, NULL, NULL, NULL);
}
void exit(int code)
{
    syscall(SYS_exit, code);
}
    1cfe:	8082                	ret

0000000000001d00 <waitpid>:
    register long a7 __asm__("a7") = n;
    1d00:	10400893          	li	a7,260
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1d04:	00000073          	ecall

int waitpid(int pid, int *code, int options)
{
    return syscall(SYS_wait4, pid, code, options);
}
    1d08:	2501                	sext.w	a0,a0
    1d0a:	8082                	ret

0000000000001d0c <exec>:
    register long a7 __asm__("a7") = n;
    1d0c:	0dd00893          	li	a7,221
    __asm_syscall("r"(a7), "0"(a0))
    1d10:	00000073          	ecall

int exec(char *name)
{
    return syscall(SYS_execve, name);
}
    1d14:	2501                	sext.w	a0,a0
    1d16:	8082                	ret

0000000000001d18 <execve>:
    register long a7 __asm__("a7") = n;
    1d18:	0dd00893          	li	a7,221
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1d1c:	00000073          	ecall

int execve(const char *name, char *const argv[], char *const argp[])
{
    return syscall(SYS_execve, name, argv, argp);
}
    1d20:	2501                	sext.w	a0,a0
    1d22:	8082                	ret

0000000000001d24 <times>:
    register long a7 __asm__("a7") = n;
    1d24:	09900893          	li	a7,153
    __asm_syscall("r"(a7), "0"(a0))
    1d28:	00000073          	ecall

int times(void *mytimes)
{
	return syscall(SYS_times, mytimes);
}
    1d2c:	2501                	sext.w	a0,a0
    1d2e:	8082                	ret

0000000000001d30 <get_time>:

int64 get_time()
{
    1d30:	1141                	addi	sp,sp,-16
    register long a7 __asm__("a7") = n;
    1d32:	0a900893          	li	a7,169
    register long a0 __asm__("a0") = a;
    1d36:	850a                	mv	a0,sp
    register long a1 __asm__("a1") = b;
    1d38:	4581                	li	a1,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1d3a:	00000073          	ecall
    TimeVal time;
    int err = sys_get_time(&time, 0);
    if (err == 0)
    1d3e:	2501                	sext.w	a0,a0
    1d40:	ed09                	bnez	a0,1d5a <get_time+0x2a>
    {
        return ((time.sec & 0xffff) * 1000 + time.usec / 1000);
    1d42:	67a2                	ld	a5,8(sp)
    1d44:	3e800713          	li	a4,1000
    1d48:	00015503          	lhu	a0,0(sp)
    1d4c:	02e7d7b3          	divu	a5,a5,a4
    1d50:	02e50533          	mul	a0,a0,a4
    1d54:	953e                	add	a0,a0,a5
    }
    else
    {
        return -1;
    }
}
    1d56:	0141                	addi	sp,sp,16
    1d58:	8082                	ret
        return -1;
    1d5a:	557d                	li	a0,-1
    1d5c:	bfed                	j	1d56 <get_time+0x26>

0000000000001d5e <sys_get_time>:
    register long a7 __asm__("a7") = n;
    1d5e:	0a900893          	li	a7,169
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1d62:	00000073          	ecall

int sys_get_time(TimeVal *ts, int tz)
{
    return syscall(SYS_gettimeofday, ts, tz);
}
    1d66:	2501                	sext.w	a0,a0
    1d68:	8082                	ret

0000000000001d6a <time>:
    register long a7 __asm__("a7") = n;
    1d6a:	42600893          	li	a7,1062
    __asm_syscall("r"(a7), "0"(a0))
    1d6e:	00000073          	ecall

int time(unsigned long *tloc)
{
    return syscall(SYS_time, tloc);
}
    1d72:	2501                	sext.w	a0,a0
    1d74:	8082                	ret

0000000000001d76 <sleep>:

int sleep(unsigned long long time)
{
    1d76:	1141                	addi	sp,sp,-16
    TimeVal tv = {.sec = time, .usec = 0};
    1d78:	e02a                	sd	a0,0(sp)
    register long a0 __asm__("a0") = a;
    1d7a:	850a                	mv	a0,sp
    1d7c:	e402                	sd	zero,8(sp)
    register long a7 __asm__("a7") = n;
    1d7e:	06500893          	li	a7,101
    register long a1 __asm__("a1") = b;
    1d82:	85aa                	mv	a1,a0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1d84:	00000073          	ecall
    if (syscall(SYS_nanosleep, &tv, &tv)) return tv.sec;
    1d88:	e501                	bnez	a0,1d90 <sleep+0x1a>
    return 0;
    1d8a:	4501                	li	a0,0
}
    1d8c:	0141                	addi	sp,sp,16
    1d8e:	8082                	ret
    if (syscall(SYS_nanosleep, &tv, &tv)) return tv.sec;
    1d90:	4502                	lw	a0,0(sp)
}
    1d92:	0141                	addi	sp,sp,16
    1d94:	8082                	ret

0000000000001d96 <set_priority>:
    register long a7 __asm__("a7") = n;
    1d96:	08c00893          	li	a7,140
    __asm_syscall("r"(a7), "0"(a0))
    1d9a:	00000073          	ecall

int set_priority(int prio)
{
    return syscall(SYS_setpriority, prio);
}
    1d9e:	2501                	sext.w	a0,a0
    1da0:	8082                	ret

0000000000001da2 <mmap>:
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4))
}

static inline long __syscall6(long n, long a, long b, long c, long d, long e, long f)
{
    register long a7 __asm__("a7") = n;
    1da2:	0de00893          	li	a7,222
    register long a1 __asm__("a1") = b;
    register long a2 __asm__("a2") = c;
    register long a3 __asm__("a3") = d;
    register long a4 __asm__("a4") = e;
    register long a5 __asm__("a5") = f;
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4), "r"(a5))
    1da6:	00000073          	ecall

void *mmap(void *start, size_t len, int prot, int flags, int fd, off_t off)
{
    return syscall(SYS_mmap, start, len, prot, flags, fd, off);
}
    1daa:	8082                	ret

0000000000001dac <munmap>:
    register long a7 __asm__("a7") = n;
    1dac:	0d700893          	li	a7,215
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1db0:	00000073          	ecall

int munmap(void *start, size_t len)
{
    return syscall(SYS_munmap, start, len);
}
    1db4:	2501                	sext.w	a0,a0
    1db6:	8082                	ret

0000000000001db8 <wait>:

int wait(int *code)
{
    1db8:	85aa                	mv	a1,a0
    register long a7 __asm__("a7") = n;
    1dba:	10400893          	li	a7,260
    register long a0 __asm__("a0") = a;
    1dbe:	557d                	li	a0,-1
    register long a2 __asm__("a2") = c;
    1dc0:	4601                	li	a2,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1dc2:	00000073          	ecall
    return waitpid(-1, code, 0);
}
    1dc6:	2501                	sext.w	a0,a0
    1dc8:	8082                	ret

0000000000001dca <spawn>:
    register long a7 __asm__("a7") = n;
    1dca:	19000893          	li	a7,400
    __asm_syscall("r"(a7), "0"(a0))
    1dce:	00000073          	ecall

int spawn(char *file)
{
    return syscall(SYS_spawn, file);
}
    1dd2:	2501                	sext.w	a0,a0
    1dd4:	8082                	ret

0000000000001dd6 <mailread>:
    register long a7 __asm__("a7") = n;
    1dd6:	19100893          	li	a7,401
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1dda:	00000073          	ecall

int mailread(void *buf, int len)
{
    return syscall(SYS_mailread, buf, len);
}
    1dde:	2501                	sext.w	a0,a0
    1de0:	8082                	ret

0000000000001de2 <mailwrite>:
    register long a7 __asm__("a7") = n;
    1de2:	19200893          	li	a7,402
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1de6:	00000073          	ecall

int mailwrite(int pid, void *buf, int len)
{
    return syscall(SYS_mailwrite, pid, buf, len);
}
    1dea:	2501                	sext.w	a0,a0
    1dec:	8082                	ret

0000000000001dee <fstat>:
    register long a7 __asm__("a7") = n;
    1dee:	05000893          	li	a7,80
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1df2:	00000073          	ecall

int fstat(int fd, struct kstat *st)
{
    return syscall(SYS_fstat, fd, st);
}
    1df6:	2501                	sext.w	a0,a0
    1df8:	8082                	ret

0000000000001dfa <sys_linkat>:
    register long a4 __asm__("a4") = e;
    1dfa:	1702                	slli	a4,a4,0x20
    register long a7 __asm__("a7") = n;
    1dfc:	02500893          	li	a7,37
    register long a4 __asm__("a4") = e;
    1e00:	9301                	srli	a4,a4,0x20
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4))
    1e02:	00000073          	ecall

int sys_linkat(int olddirfd, char *oldpath, int newdirfd, char *newpath, unsigned int flags)
{
    return syscall(SYS_linkat, olddirfd, oldpath, newdirfd, newpath, flags);
}
    1e06:	2501                	sext.w	a0,a0
    1e08:	8082                	ret

0000000000001e0a <sys_unlinkat>:
    register long a2 __asm__("a2") = c;
    1e0a:	1602                	slli	a2,a2,0x20
    register long a7 __asm__("a7") = n;
    1e0c:	02300893          	li	a7,35
    register long a2 __asm__("a2") = c;
    1e10:	9201                	srli	a2,a2,0x20
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1e12:	00000073          	ecall

int sys_unlinkat(int dirfd, char *path, unsigned int flags)
{
    return syscall(SYS_unlinkat, dirfd, path, flags);
}
    1e16:	2501                	sext.w	a0,a0
    1e18:	8082                	ret

0000000000001e1a <link>:

int link(char *old_path, char *new_path)
{
    1e1a:	87aa                	mv	a5,a0
    1e1c:	86ae                	mv	a3,a1
    register long a7 __asm__("a7") = n;
    1e1e:	02500893          	li	a7,37
    register long a0 __asm__("a0") = a;
    1e22:	f9c00513          	li	a0,-100
    register long a1 __asm__("a1") = b;
    1e26:	85be                	mv	a1,a5
    register long a2 __asm__("a2") = c;
    1e28:	f9c00613          	li	a2,-100
    register long a4 __asm__("a4") = e;
    1e2c:	4701                	li	a4,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4))
    1e2e:	00000073          	ecall
    return sys_linkat(AT_FDCWD, old_path, AT_FDCWD, new_path, 0);
}
    1e32:	2501                	sext.w	a0,a0
    1e34:	8082                	ret

0000000000001e36 <unlink>:

int unlink(char *path)
{
    1e36:	85aa                	mv	a1,a0
    register long a7 __asm__("a7") = n;
    1e38:	02300893          	li	a7,35
    register long a0 __asm__("a0") = a;
    1e3c:	f9c00513          	li	a0,-100
    register long a2 __asm__("a2") = c;
    1e40:	4601                	li	a2,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1e42:	00000073          	ecall
    return sys_unlinkat(AT_FDCWD, path, 0);
}
    1e46:	2501                	sext.w	a0,a0
    1e48:	8082                	ret

0000000000001e4a <uname>:
    register long a7 __asm__("a7") = n;
    1e4a:	0a000893          	li	a7,160
    __asm_syscall("r"(a7), "0"(a0))
    1e4e:	00000073          	ecall

int uname(void *buf)
{
    return syscall(SYS_uname, buf);
}
    1e52:	2501                	sext.w	a0,a0
    1e54:	8082                	ret

0000000000001e56 <brk>:
    register long a7 __asm__("a7") = n;
    1e56:	0d600893          	li	a7,214
    __asm_syscall("r"(a7), "0"(a0))
    1e5a:	00000073          	ecall

int brk(void *addr)
{
    return syscall(SYS_brk, addr);
}
    1e5e:	2501                	sext.w	a0,a0
    1e60:	8082                	ret

0000000000001e62 <getcwd>:
    register long a7 __asm__("a7") = n;
    1e62:	48c5                	li	a7,17
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1e64:	00000073          	ecall

char *getcwd(char *buf, size_t size){
    return syscall(SYS_getcwd, buf, size);
}
    1e68:	8082                	ret

0000000000001e6a <chdir>:
    register long a7 __asm__("a7") = n;
    1e6a:	03100893          	li	a7,49
    __asm_syscall("r"(a7), "0"(a0))
    1e6e:	00000073          	ecall

int chdir(const char *path){
    return syscall(SYS_chdir, path);
}
    1e72:	2501                	sext.w	a0,a0
    1e74:	8082                	ret

0000000000001e76 <mkdir>:

int mkdir(const char *path, mode_t mode){
    1e76:	862e                	mv	a2,a1
    1e78:	87aa                	mv	a5,a0
    register long a2 __asm__("a2") = c;
    1e7a:	1602                	slli	a2,a2,0x20
    register long a7 __asm__("a7") = n;
    1e7c:	02200893          	li	a7,34
    register long a0 __asm__("a0") = a;
    1e80:	f9c00513          	li	a0,-100
    register long a1 __asm__("a1") = b;
    1e84:	85be                	mv	a1,a5
    register long a2 __asm__("a2") = c;
    1e86:	9201                	srli	a2,a2,0x20
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1e88:	00000073          	ecall
    return syscall(SYS_mkdirat, AT_FDCWD, path, mode);
}
    1e8c:	2501                	sext.w	a0,a0
    1e8e:	8082                	ret

0000000000001e90 <getdents>:
    register long a7 __asm__("a7") = n;
    1e90:	03d00893          	li	a7,61
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1e94:	00000073          	ecall

int getdents(int fd, struct linux_dirent64 *dirp64, unsigned long len){
    //return syscall(SYS_getdents64, fd, dirp64, len);
    return syscall(SYS_getdents64, fd, dirp64, len);
}
    1e98:	2501                	sext.w	a0,a0
    1e9a:	8082                	ret

0000000000001e9c <pipe>:
    register long a7 __asm__("a7") = n;
    1e9c:	03b00893          	li	a7,59
    register long a1 __asm__("a1") = b;
    1ea0:	4581                	li	a1,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1ea2:	00000073          	ecall

int pipe(int fd[2]){
    return syscall(SYS_pipe2, fd, 0);
}
    1ea6:	2501                	sext.w	a0,a0
    1ea8:	8082                	ret

0000000000001eaa <dup>:
    register long a7 __asm__("a7") = n;
    1eaa:	48dd                	li	a7,23
    __asm_syscall("r"(a7), "0"(a0))
    1eac:	00000073          	ecall

int dup(int fd){
    return syscall(SYS_dup, fd);
}
    1eb0:	2501                	sext.w	a0,a0
    1eb2:	8082                	ret

0000000000001eb4 <dup2>:
    register long a7 __asm__("a7") = n;
    1eb4:	48e1                	li	a7,24
    register long a2 __asm__("a2") = c;
    1eb6:	4601                	li	a2,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1eb8:	00000073          	ecall

int dup2(int old, int new){
    return syscall(SYS_dup3, old, new, 0);
}
    1ebc:	2501                	sext.w	a0,a0
    1ebe:	8082                	ret

0000000000001ec0 <mount>:
    register long a7 __asm__("a7") = n;
    1ec0:	02800893          	li	a7,40
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4))
    1ec4:	00000073          	ecall

int mount(const char *special, const char *dir, const char *fstype, unsigned long flags, const void *data)
{
        return syscall(SYS_mount, special, dir, fstype, flags, data);
}
    1ec8:	2501                	sext.w	a0,a0
    1eca:	8082                	ret

0000000000001ecc <umount>:
    register long a7 __asm__("a7") = n;
    1ecc:	02700893          	li	a7,39
    register long a1 __asm__("a1") = b;
    1ed0:	4581                	li	a1,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1ed2:	00000073          	ecall

int umount(const char *special)
{
        return syscall(SYS_umount2, special, 0);
}
    1ed6:	2501                	sext.w	a0,a0
    1ed8:	8082                	ret

0000000000001eda <__clone>:

.global __clone
.type  __clone, %function
__clone:
	# Save func and arg to stack
	addi a1, a1, -16
    1eda:	15c1                	addi	a1,a1,-16
	sd a0, 0(a1)
    1edc:	e188                	sd	a0,0(a1)
	sd a3, 8(a1)
    1ede:	e594                	sd	a3,8(a1)

	# Call SYS_clone
	mv a0, a2
    1ee0:	8532                	mv	a0,a2
	mv a2, a4
    1ee2:	863a                	mv	a2,a4
	mv a3, a5
    1ee4:	86be                	mv	a3,a5
	mv a4, a6
    1ee6:	8742                	mv	a4,a6
	li a7, 220 # SYS_clone
    1ee8:	0dc00893          	li	a7,220
	ecall
    1eec:	00000073          	ecall

	beqz a0, 1f
    1ef0:	c111                	beqz	a0,1ef4 <__clone+0x1a>
	# Parent
	ret
    1ef2:	8082                	ret

	# Child
1:      ld a1, 0(sp)
    1ef4:	6582                	ld	a1,0(sp)
	ld a0, 8(sp)
    1ef6:	6522                	ld	a0,8(sp)
	jalr a1
    1ef8:	9582                	jalr	a1

	# Exit
	li a7, 93 # SYS_exit
    1efa:	05d00893          	li	a7,93
	ecall
    1efe:	00000073          	ecall
