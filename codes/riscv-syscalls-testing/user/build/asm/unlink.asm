
/home/ubuntu/rust/ultra-os/codes/riscv-syscalls-testing/user/build/riscv64/unlink:     file format elf64-littleriscv


Disassembly of section .text:

0000000000001000 <_start>:
.section .text.entry
.globl _start
_start:
    mv a0, sp
    1000:	850a                	mv	a0,sp
    tail __start_main
    1002:	a8f9                	j	10e0 <__start_main>

0000000000001004 <test_unlink>:
 * 测试失败，应有如下输出：
 * "  unlink error!"
 */

int test_unlink()
{
    1004:	1141                	addi	sp,sp,-16
    TEST_START(__func__);
    1006:	00001517          	auipc	a0,0x1
    100a:	f1250513          	addi	a0,a0,-238 # 1f18 <__clone+0x2a>
{
    100e:	e406                	sd	ra,8(sp)
    1010:	e022                	sd	s0,0(sp)
    TEST_START(__func__);
    1012:	340000ef          	jal	ra,1352 <puts>
    1016:	00001517          	auipc	a0,0x1
    101a:	fba50513          	addi	a0,a0,-70 # 1fd0 <__func__.0>
    101e:	334000ef          	jal	ra,1352 <puts>
    1022:	00001517          	auipc	a0,0x1
    1026:	f0e50513          	addi	a0,a0,-242 # 1f30 <__clone+0x42>
    102a:	328000ef          	jal	ra,1352 <puts>

    char *fname = "./test_unlink";
    int fd, ret;

    fd = open(fname, O_CREATE | O_WRONLY);
    102e:	04100593          	li	a1,65
    1032:	00001517          	auipc	a0,0x1
    1036:	f0e50513          	addi	a0,a0,-242 # 1f40 <__clone+0x52>
    103a:	443000ef          	jal	ra,1c7c <open>
    103e:	842a                	mv	s0,a0
    assert(fd > 0);
    1040:	08a05163          	blez	a0,10c2 <test_unlink+0xbe>
    close(fd);
    1044:	8522                	mv	a0,s0
    1046:	45f000ef          	jal	ra,1ca4 <close>

    // unlink test
    ret = unlink(fname);
    104a:	00001517          	auipc	a0,0x1
    104e:	ef650513          	addi	a0,a0,-266 # 1f40 <__clone+0x52>
    1052:	5f9000ef          	jal	ra,1e4a <unlink>
    assert(ret == 0);
    1056:	ed39                	bnez	a0,10b4 <test_unlink+0xb0>
    fd = open(fname, O_RDONLY);
    1058:	4581                	li	a1,0
    105a:	00001517          	auipc	a0,0x1
    105e:	ee650513          	addi	a0,a0,-282 # 1f40 <__clone+0x52>
    1062:	41b000ef          	jal	ra,1c7c <open>
    1066:	842a                	mv	s0,a0
    if(fd < 0){
    1068:	02054f63          	bltz	a0,10a6 <test_unlink+0xa2>
        printf("  unlink success!\n");
    }else{
	printf("  unlink error!\n");
    106c:	00001517          	auipc	a0,0x1
    1070:	f1c50513          	addi	a0,a0,-228 # 1f88 <__clone+0x9a>
    1074:	300000ef          	jal	ra,1374 <printf>
        close(fd);
    1078:	8522                	mv	a0,s0
    107a:	42b000ef          	jal	ra,1ca4 <close>
    }
    // It's Ok if you don't delete the inode and data blocks.

    TEST_END(__func__);
    107e:	00001517          	auipc	a0,0x1
    1082:	f2250513          	addi	a0,a0,-222 # 1fa0 <__clone+0xb2>
    1086:	2cc000ef          	jal	ra,1352 <puts>
    108a:	00001517          	auipc	a0,0x1
    108e:	f4650513          	addi	a0,a0,-186 # 1fd0 <__func__.0>
    1092:	2c0000ef          	jal	ra,1352 <puts>
}
    1096:	6402                	ld	s0,0(sp)
    1098:	60a2                	ld	ra,8(sp)
    TEST_END(__func__);
    109a:	00001517          	auipc	a0,0x1
    109e:	e9650513          	addi	a0,a0,-362 # 1f30 <__clone+0x42>
}
    10a2:	0141                	addi	sp,sp,16
    TEST_END(__func__);
    10a4:	a47d                	j	1352 <puts>
        printf("  unlink success!\n");
    10a6:	00001517          	auipc	a0,0x1
    10aa:	eca50513          	addi	a0,a0,-310 # 1f70 <__clone+0x82>
    10ae:	2c6000ef          	jal	ra,1374 <printf>
    10b2:	b7f1                	j	107e <test_unlink+0x7a>
    assert(ret == 0);
    10b4:	00001517          	auipc	a0,0x1
    10b8:	e9c50513          	addi	a0,a0,-356 # 1f50 <__clone+0x62>
    10bc:	53c000ef          	jal	ra,15f8 <panic>
    10c0:	bf61                	j	1058 <test_unlink+0x54>
    assert(fd > 0);
    10c2:	00001517          	auipc	a0,0x1
    10c6:	e8e50513          	addi	a0,a0,-370 # 1f50 <__clone+0x62>
    10ca:	52e000ef          	jal	ra,15f8 <panic>
    10ce:	bf9d                	j	1044 <test_unlink+0x40>

00000000000010d0 <main>:

int main(void) {
    10d0:	1141                	addi	sp,sp,-16
    10d2:	e406                	sd	ra,8(sp)
	test_unlink();
    10d4:	f31ff0ef          	jal	ra,1004 <test_unlink>
	return 0;
}
    10d8:	60a2                	ld	ra,8(sp)
    10da:	4501                	li	a0,0
    10dc:	0141                	addi	sp,sp,16
    10de:	8082                	ret

00000000000010e0 <__start_main>:
#include <unistd.h>

extern int main();

int __start_main(long *p)
{
    10e0:	85aa                	mv	a1,a0
	int argc = p[0];
	char **argv = (void *)(p+1);

	exit(main(argc, argv));
    10e2:	4108                	lw	a0,0(a0)
{
    10e4:	1141                	addi	sp,sp,-16
	exit(main(argc, argv));
    10e6:	05a1                	addi	a1,a1,8
{
    10e8:	e406                	sd	ra,8(sp)
	exit(main(argc, argv));
    10ea:	fe7ff0ef          	jal	ra,10d0 <main>
    10ee:	41d000ef          	jal	ra,1d0a <exit>
	return 0;
}
    10f2:	60a2                	ld	ra,8(sp)
    10f4:	4501                	li	a0,0
    10f6:	0141                	addi	sp,sp,16
    10f8:	8082                	ret

00000000000010fa <printint.constprop.0>:
    write(f, s, l);
}

static char digits[] = "0123456789abcdef";

static void printint(int xx, int base, int sign)
    10fa:	7179                	addi	sp,sp,-48
    10fc:	f406                	sd	ra,40(sp)
{
    char buf[16 + 1];
    int i;
    uint x;

    if (sign && (sign = xx < 0))
    10fe:	12054b63          	bltz	a0,1234 <printint.constprop.0+0x13a>

    buf[16] = 0;
    i = 15;
    do
    {
        buf[i--] = digits[x % base];
    1102:	02b577bb          	remuw	a5,a0,a1
    1106:	00001617          	auipc	a2,0x1
    110a:	eda60613          	addi	a2,a2,-294 # 1fe0 <digits>
    buf[16] = 0;
    110e:	00010c23          	sb	zero,24(sp)
        buf[i--] = digits[x % base];
    1112:	0005871b          	sext.w	a4,a1
    1116:	1782                	slli	a5,a5,0x20
    1118:	9381                	srli	a5,a5,0x20
    111a:	97b2                	add	a5,a5,a2
    111c:	0007c783          	lbu	a5,0(a5)
    } while ((x /= base) != 0);
    1120:	02b5583b          	divuw	a6,a0,a1
        buf[i--] = digits[x % base];
    1124:	00f10ba3          	sb	a5,23(sp)
    } while ((x /= base) != 0);
    1128:	1cb56363          	bltu	a0,a1,12ee <printint.constprop.0+0x1f4>
        buf[i--] = digits[x % base];
    112c:	45b9                	li	a1,14
    112e:	02e877bb          	remuw	a5,a6,a4
    1132:	1782                	slli	a5,a5,0x20
    1134:	9381                	srli	a5,a5,0x20
    1136:	97b2                	add	a5,a5,a2
    1138:	0007c783          	lbu	a5,0(a5)
    } while ((x /= base) != 0);
    113c:	02e856bb          	divuw	a3,a6,a4
        buf[i--] = digits[x % base];
    1140:	00f10b23          	sb	a5,22(sp)
    } while ((x /= base) != 0);
    1144:	0ce86e63          	bltu	a6,a4,1220 <printint.constprop.0+0x126>
        buf[i--] = digits[x % base];
    1148:	02e6f5bb          	remuw	a1,a3,a4
    } while ((x /= base) != 0);
    114c:	02e6d7bb          	divuw	a5,a3,a4
        buf[i--] = digits[x % base];
    1150:	1582                	slli	a1,a1,0x20
    1152:	9181                	srli	a1,a1,0x20
    1154:	95b2                	add	a1,a1,a2
    1156:	0005c583          	lbu	a1,0(a1)
    115a:	00b10aa3          	sb	a1,21(sp)
    } while ((x /= base) != 0);
    115e:	0007859b          	sext.w	a1,a5
    1162:	12e6ec63          	bltu	a3,a4,129a <printint.constprop.0+0x1a0>
        buf[i--] = digits[x % base];
    1166:	02e7f6bb          	remuw	a3,a5,a4
    116a:	1682                	slli	a3,a3,0x20
    116c:	9281                	srli	a3,a3,0x20
    116e:	96b2                	add	a3,a3,a2
    1170:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    1174:	02e7d83b          	divuw	a6,a5,a4
        buf[i--] = digits[x % base];
    1178:	00d10a23          	sb	a3,20(sp)
    } while ((x /= base) != 0);
    117c:	12e5e863          	bltu	a1,a4,12ac <printint.constprop.0+0x1b2>
        buf[i--] = digits[x % base];
    1180:	02e876bb          	remuw	a3,a6,a4
    1184:	1682                	slli	a3,a3,0x20
    1186:	9281                	srli	a3,a3,0x20
    1188:	96b2                	add	a3,a3,a2
    118a:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    118e:	02e855bb          	divuw	a1,a6,a4
        buf[i--] = digits[x % base];
    1192:	00d109a3          	sb	a3,19(sp)
    } while ((x /= base) != 0);
    1196:	12e86463          	bltu	a6,a4,12be <printint.constprop.0+0x1c4>
        buf[i--] = digits[x % base];
    119a:	02e5f6bb          	remuw	a3,a1,a4
    119e:	1682                	slli	a3,a3,0x20
    11a0:	9281                	srli	a3,a3,0x20
    11a2:	96b2                	add	a3,a3,a2
    11a4:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    11a8:	02e5d83b          	divuw	a6,a1,a4
        buf[i--] = digits[x % base];
    11ac:	00d10923          	sb	a3,18(sp)
    } while ((x /= base) != 0);
    11b0:	0ce5ec63          	bltu	a1,a4,1288 <printint.constprop.0+0x18e>
        buf[i--] = digits[x % base];
    11b4:	02e876bb          	remuw	a3,a6,a4
    11b8:	1682                	slli	a3,a3,0x20
    11ba:	9281                	srli	a3,a3,0x20
    11bc:	96b2                	add	a3,a3,a2
    11be:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    11c2:	02e855bb          	divuw	a1,a6,a4
        buf[i--] = digits[x % base];
    11c6:	00d108a3          	sb	a3,17(sp)
    } while ((x /= base) != 0);
    11ca:	10e86963          	bltu	a6,a4,12dc <printint.constprop.0+0x1e2>
        buf[i--] = digits[x % base];
    11ce:	02e5f6bb          	remuw	a3,a1,a4
    11d2:	1682                	slli	a3,a3,0x20
    11d4:	9281                	srli	a3,a3,0x20
    11d6:	96b2                	add	a3,a3,a2
    11d8:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    11dc:	02e5d83b          	divuw	a6,a1,a4
        buf[i--] = digits[x % base];
    11e0:	00d10823          	sb	a3,16(sp)
    } while ((x /= base) != 0);
    11e4:	10e5e763          	bltu	a1,a4,12f2 <printint.constprop.0+0x1f8>
        buf[i--] = digits[x % base];
    11e8:	02e876bb          	remuw	a3,a6,a4
    11ec:	1682                	slli	a3,a3,0x20
    11ee:	9281                	srli	a3,a3,0x20
    11f0:	96b2                	add	a3,a3,a2
    11f2:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    11f6:	02e857bb          	divuw	a5,a6,a4
        buf[i--] = digits[x % base];
    11fa:	00d107a3          	sb	a3,15(sp)
    } while ((x /= base) != 0);
    11fe:	10e86363          	bltu	a6,a4,1304 <printint.constprop.0+0x20a>
        buf[i--] = digits[x % base];
    1202:	1782                	slli	a5,a5,0x20
    1204:	9381                	srli	a5,a5,0x20
    1206:	97b2                	add	a5,a5,a2
    1208:	0007c783          	lbu	a5,0(a5)
    120c:	4599                	li	a1,6
    120e:	00f10723          	sb	a5,14(sp)

    if (sign)
    1212:	00055763          	bgez	a0,1220 <printint.constprop.0+0x126>
        buf[i--] = '-';
    1216:	02d00793          	li	a5,45
    121a:	00f106a3          	sb	a5,13(sp)
        buf[i--] = digits[x % base];
    121e:	4595                	li	a1,5
    write(f, s, l);
    1220:	003c                	addi	a5,sp,8
    1222:	4641                	li	a2,16
    1224:	9e0d                	subw	a2,a2,a1
    1226:	4505                	li	a0,1
    1228:	95be                	add	a1,a1,a5
    122a:	291000ef          	jal	ra,1cba <write>
    i++;
    if (i < 0)
        puts("printint error");
    out(stdout, buf + i, 16 - i);
}
    122e:	70a2                	ld	ra,40(sp)
    1230:	6145                	addi	sp,sp,48
    1232:	8082                	ret
        x = -xx;
    1234:	40a0083b          	negw	a6,a0
        buf[i--] = digits[x % base];
    1238:	02b877bb          	remuw	a5,a6,a1
    123c:	00001617          	auipc	a2,0x1
    1240:	da460613          	addi	a2,a2,-604 # 1fe0 <digits>
    buf[16] = 0;
    1244:	00010c23          	sb	zero,24(sp)
        buf[i--] = digits[x % base];
    1248:	0005871b          	sext.w	a4,a1
    124c:	1782                	slli	a5,a5,0x20
    124e:	9381                	srli	a5,a5,0x20
    1250:	97b2                	add	a5,a5,a2
    1252:	0007c783          	lbu	a5,0(a5)
    } while ((x /= base) != 0);
    1256:	02b858bb          	divuw	a7,a6,a1
        buf[i--] = digits[x % base];
    125a:	00f10ba3          	sb	a5,23(sp)
    } while ((x /= base) != 0);
    125e:	06b86963          	bltu	a6,a1,12d0 <printint.constprop.0+0x1d6>
        buf[i--] = digits[x % base];
    1262:	02e8f7bb          	remuw	a5,a7,a4
    1266:	1782                	slli	a5,a5,0x20
    1268:	9381                	srli	a5,a5,0x20
    126a:	97b2                	add	a5,a5,a2
    126c:	0007c783          	lbu	a5,0(a5)
    } while ((x /= base) != 0);
    1270:	02e8d6bb          	divuw	a3,a7,a4
        buf[i--] = digits[x % base];
    1274:	00f10b23          	sb	a5,22(sp)
    } while ((x /= base) != 0);
    1278:	ece8f8e3          	bgeu	a7,a4,1148 <printint.constprop.0+0x4e>
        buf[i--] = '-';
    127c:	02d00793          	li	a5,45
    1280:	00f10aa3          	sb	a5,21(sp)
        buf[i--] = digits[x % base];
    1284:	45b5                	li	a1,13
    1286:	bf69                	j	1220 <printint.constprop.0+0x126>
    1288:	45a9                	li	a1,10
    if (sign)
    128a:	f8055be3          	bgez	a0,1220 <printint.constprop.0+0x126>
        buf[i--] = '-';
    128e:	02d00793          	li	a5,45
    1292:	00f108a3          	sb	a5,17(sp)
        buf[i--] = digits[x % base];
    1296:	45a5                	li	a1,9
    1298:	b761                	j	1220 <printint.constprop.0+0x126>
    129a:	45b5                	li	a1,13
    if (sign)
    129c:	f80552e3          	bgez	a0,1220 <printint.constprop.0+0x126>
        buf[i--] = '-';
    12a0:	02d00793          	li	a5,45
    12a4:	00f10a23          	sb	a5,20(sp)
        buf[i--] = digits[x % base];
    12a8:	45b1                	li	a1,12
    12aa:	bf9d                	j	1220 <printint.constprop.0+0x126>
    12ac:	45b1                	li	a1,12
    if (sign)
    12ae:	f60559e3          	bgez	a0,1220 <printint.constprop.0+0x126>
        buf[i--] = '-';
    12b2:	02d00793          	li	a5,45
    12b6:	00f109a3          	sb	a5,19(sp)
        buf[i--] = digits[x % base];
    12ba:	45ad                	li	a1,11
    12bc:	b795                	j	1220 <printint.constprop.0+0x126>
    12be:	45ad                	li	a1,11
    if (sign)
    12c0:	f60550e3          	bgez	a0,1220 <printint.constprop.0+0x126>
        buf[i--] = '-';
    12c4:	02d00793          	li	a5,45
    12c8:	00f10923          	sb	a5,18(sp)
        buf[i--] = digits[x % base];
    12cc:	45a9                	li	a1,10
    12ce:	bf89                	j	1220 <printint.constprop.0+0x126>
        buf[i--] = '-';
    12d0:	02d00793          	li	a5,45
    12d4:	00f10b23          	sb	a5,22(sp)
        buf[i--] = digits[x % base];
    12d8:	45b9                	li	a1,14
    12da:	b799                	j	1220 <printint.constprop.0+0x126>
    12dc:	45a5                	li	a1,9
    if (sign)
    12de:	f40551e3          	bgez	a0,1220 <printint.constprop.0+0x126>
        buf[i--] = '-';
    12e2:	02d00793          	li	a5,45
    12e6:	00f10823          	sb	a5,16(sp)
        buf[i--] = digits[x % base];
    12ea:	45a1                	li	a1,8
    12ec:	bf15                	j	1220 <printint.constprop.0+0x126>
    i = 15;
    12ee:	45bd                	li	a1,15
    12f0:	bf05                	j	1220 <printint.constprop.0+0x126>
        buf[i--] = digits[x % base];
    12f2:	45a1                	li	a1,8
    if (sign)
    12f4:	f20556e3          	bgez	a0,1220 <printint.constprop.0+0x126>
        buf[i--] = '-';
    12f8:	02d00793          	li	a5,45
    12fc:	00f107a3          	sb	a5,15(sp)
        buf[i--] = digits[x % base];
    1300:	459d                	li	a1,7
    1302:	bf39                	j	1220 <printint.constprop.0+0x126>
    1304:	459d                	li	a1,7
    if (sign)
    1306:	f0055de3          	bgez	a0,1220 <printint.constprop.0+0x126>
        buf[i--] = '-';
    130a:	02d00793          	li	a5,45
    130e:	00f10723          	sb	a5,14(sp)
        buf[i--] = digits[x % base];
    1312:	4599                	li	a1,6
    1314:	b731                	j	1220 <printint.constprop.0+0x126>

0000000000001316 <getchar>:
{
    1316:	1101                	addi	sp,sp,-32
    read(stdin, &byte, 1);
    1318:	00f10593          	addi	a1,sp,15
    131c:	4605                	li	a2,1
    131e:	4501                	li	a0,0
{
    1320:	ec06                	sd	ra,24(sp)
    char byte = 0;
    1322:	000107a3          	sb	zero,15(sp)
    read(stdin, &byte, 1);
    1326:	18b000ef          	jal	ra,1cb0 <read>
}
    132a:	60e2                	ld	ra,24(sp)
    132c:	00f14503          	lbu	a0,15(sp)
    1330:	6105                	addi	sp,sp,32
    1332:	8082                	ret

0000000000001334 <putchar>:
{
    1334:	1101                	addi	sp,sp,-32
    1336:	87aa                	mv	a5,a0
    return write(stdout, &byte, 1);
    1338:	00f10593          	addi	a1,sp,15
    133c:	4605                	li	a2,1
    133e:	4505                	li	a0,1
{
    1340:	ec06                	sd	ra,24(sp)
    char byte = c;
    1342:	00f107a3          	sb	a5,15(sp)
    return write(stdout, &byte, 1);
    1346:	175000ef          	jal	ra,1cba <write>
}
    134a:	60e2                	ld	ra,24(sp)
    134c:	2501                	sext.w	a0,a0
    134e:	6105                	addi	sp,sp,32
    1350:	8082                	ret

0000000000001352 <puts>:
{
    1352:	1141                	addi	sp,sp,-16
    1354:	e406                	sd	ra,8(sp)
    1356:	e022                	sd	s0,0(sp)
    1358:	842a                	mv	s0,a0
    r = -(write(stdout, s, strlen(s)) < 0);
    135a:	57c000ef          	jal	ra,18d6 <strlen>
    135e:	862a                	mv	a2,a0
    1360:	85a2                	mv	a1,s0
    1362:	4505                	li	a0,1
    1364:	157000ef          	jal	ra,1cba <write>
}
    1368:	60a2                	ld	ra,8(sp)
    136a:	6402                	ld	s0,0(sp)
    r = -(write(stdout, s, strlen(s)) < 0);
    136c:	957d                	srai	a0,a0,0x3f
    return r;
    136e:	2501                	sext.w	a0,a0
}
    1370:	0141                	addi	sp,sp,16
    1372:	8082                	ret

0000000000001374 <printf>:
    out(stdout, buf, i);
}

// Print to the console. only understands %d, %x, %p, %s.
void printf(const char *fmt, ...)
{
    1374:	7171                	addi	sp,sp,-176
    1376:	fc56                	sd	s5,56(sp)
    1378:	ed3e                	sd	a5,152(sp)
    buf[i++] = '0';
    137a:	7ae1                	lui	s5,0xffff8
    va_list ap;
    int cnt = 0, l = 0;
    char *a, *z, *s = (char *)fmt, str;
    int f = stdout;

    va_start(ap, fmt);
    137c:	18bc                	addi	a5,sp,120
{
    137e:	e8ca                	sd	s2,80(sp)
    1380:	e4ce                	sd	s3,72(sp)
    1382:	e0d2                	sd	s4,64(sp)
    1384:	f85a                	sd	s6,48(sp)
    1386:	f486                	sd	ra,104(sp)
    1388:	f0a2                	sd	s0,96(sp)
    138a:	eca6                	sd	s1,88(sp)
    138c:	fcae                	sd	a1,120(sp)
    138e:	e132                	sd	a2,128(sp)
    1390:	e536                	sd	a3,136(sp)
    1392:	e93a                	sd	a4,144(sp)
    1394:	f142                	sd	a6,160(sp)
    1396:	f546                	sd	a7,168(sp)
    va_start(ap, fmt);
    1398:	e03e                	sd	a5,0(sp)
    for (;;)
    {
        if (!*s)
            break;
        for (a = s; *s && *s != '%'; s++)
    139a:	02500913          	li	s2,37
        out(f, a, l);
        if (l)
            continue;
        if (s[1] == 0)
            break;
        switch (s[1])
    139e:	07300a13          	li	s4,115
        case 'p':
            printptr(va_arg(ap, uint64));
            break;
        case 's':
            if ((a = va_arg(ap, char *)) == 0)
                a = "(null)";
    13a2:	00001b17          	auipc	s6,0x1
    13a6:	c0eb0b13          	addi	s6,s6,-1010 # 1fb0 <__clone+0xc2>
    buf[i++] = '0';
    13aa:	830aca93          	xori	s5,s5,-2000
        buf[i++] = digits[x >> (sizeof(uint64) * 8 - 4)];
    13ae:	00001997          	auipc	s3,0x1
    13b2:	c3298993          	addi	s3,s3,-974 # 1fe0 <digits>
        if (!*s)
    13b6:	00054783          	lbu	a5,0(a0)
    13ba:	16078a63          	beqz	a5,152e <printf+0x1ba>
    13be:	862a                	mv	a2,a0
        for (a = s; *s && *s != '%'; s++)
    13c0:	19278163          	beq	a5,s2,1542 <printf+0x1ce>
    13c4:	00164783          	lbu	a5,1(a2)
    13c8:	0605                	addi	a2,a2,1
    13ca:	fbfd                	bnez	a5,13c0 <printf+0x4c>
    13cc:	84b2                	mv	s1,a2
        l = z - a;
    13ce:	40a6043b          	subw	s0,a2,a0
    write(f, s, l);
    13d2:	85aa                	mv	a1,a0
    13d4:	8622                	mv	a2,s0
    13d6:	4505                	li	a0,1
    13d8:	0e3000ef          	jal	ra,1cba <write>
        if (l)
    13dc:	18041c63          	bnez	s0,1574 <printf+0x200>
        if (s[1] == 0)
    13e0:	0014c783          	lbu	a5,1(s1)
    13e4:	14078563          	beqz	a5,152e <printf+0x1ba>
        switch (s[1])
    13e8:	1d478063          	beq	a5,s4,15a8 <printf+0x234>
    13ec:	18fa6663          	bltu	s4,a5,1578 <printf+0x204>
    13f0:	06400713          	li	a4,100
    13f4:	1ae78063          	beq	a5,a4,1594 <printf+0x220>
    13f8:	07000713          	li	a4,112
    13fc:	1ce79963          	bne	a5,a4,15ce <printf+0x25a>
            printptr(va_arg(ap, uint64));
    1400:	6702                	ld	a4,0(sp)
    buf[i++] = '0';
    1402:	01511423          	sh	s5,8(sp)
    write(f, s, l);
    1406:	4649                	li	a2,18
            printptr(va_arg(ap, uint64));
    1408:	631c                	ld	a5,0(a4)
    140a:	0721                	addi	a4,a4,8
    140c:	e03a                	sd	a4,0(sp)
    for (j = 0; j < (sizeof(uint64) * 2); j++, x <<= 4)
    140e:	00479293          	slli	t0,a5,0x4
    1412:	00879f93          	slli	t6,a5,0x8
    1416:	00c79f13          	slli	t5,a5,0xc
    141a:	01079e93          	slli	t4,a5,0x10
    141e:	01479e13          	slli	t3,a5,0x14
    1422:	01879313          	slli	t1,a5,0x18
    1426:	01c79893          	slli	a7,a5,0x1c
    142a:	02479813          	slli	a6,a5,0x24
    142e:	02879513          	slli	a0,a5,0x28
    1432:	02c79593          	slli	a1,a5,0x2c
    1436:	03079693          	slli	a3,a5,0x30
    143a:	03479713          	slli	a4,a5,0x34
        buf[i++] = digits[x >> (sizeof(uint64) * 8 - 4)];
    143e:	03c7d413          	srli	s0,a5,0x3c
    1442:	01c7d39b          	srliw	t2,a5,0x1c
    1446:	03c2d293          	srli	t0,t0,0x3c
    144a:	03cfdf93          	srli	t6,t6,0x3c
    144e:	03cf5f13          	srli	t5,t5,0x3c
    1452:	03cede93          	srli	t4,t4,0x3c
    1456:	03ce5e13          	srli	t3,t3,0x3c
    145a:	03c35313          	srli	t1,t1,0x3c
    145e:	03c8d893          	srli	a7,a7,0x3c
    1462:	03c85813          	srli	a6,a6,0x3c
    1466:	9171                	srli	a0,a0,0x3c
    1468:	91f1                	srli	a1,a1,0x3c
    146a:	92f1                	srli	a3,a3,0x3c
    146c:	9371                	srli	a4,a4,0x3c
    146e:	96ce                	add	a3,a3,s3
    1470:	974e                	add	a4,a4,s3
    1472:	944e                	add	s0,s0,s3
    1474:	92ce                	add	t0,t0,s3
    1476:	9fce                	add	t6,t6,s3
    1478:	9f4e                	add	t5,t5,s3
    147a:	9ece                	add	t4,t4,s3
    147c:	9e4e                	add	t3,t3,s3
    147e:	934e                	add	t1,t1,s3
    1480:	98ce                	add	a7,a7,s3
    1482:	93ce                	add	t2,t2,s3
    1484:	984e                	add	a6,a6,s3
    1486:	954e                	add	a0,a0,s3
    1488:	95ce                	add	a1,a1,s3
    148a:	0006c083          	lbu	ra,0(a3)
    148e:	0002c283          	lbu	t0,0(t0)
    1492:	00074683          	lbu	a3,0(a4)
    1496:	000fcf83          	lbu	t6,0(t6)
    149a:	000f4f03          	lbu	t5,0(t5)
    149e:	000ece83          	lbu	t4,0(t4)
    14a2:	000e4e03          	lbu	t3,0(t3)
    14a6:	00034303          	lbu	t1,0(t1)
    14aa:	0008c883          	lbu	a7,0(a7)
    14ae:	0003c383          	lbu	t2,0(t2)
    14b2:	00084803          	lbu	a6,0(a6)
    14b6:	00054503          	lbu	a0,0(a0)
    14ba:	0005c583          	lbu	a1,0(a1)
    14be:	00044403          	lbu	s0,0(s0)
    for (j = 0; j < (sizeof(uint64) * 2); j++, x <<= 4)
    14c2:	03879713          	slli	a4,a5,0x38
        buf[i++] = digits[x >> (sizeof(uint64) * 8 - 4)];
    14c6:	9371                	srli	a4,a4,0x3c
    14c8:	8bbd                	andi	a5,a5,15
    14ca:	974e                	add	a4,a4,s3
    14cc:	97ce                	add	a5,a5,s3
    14ce:	005105a3          	sb	t0,11(sp)
    14d2:	01f10623          	sb	t6,12(sp)
    14d6:	01e106a3          	sb	t5,13(sp)
    14da:	01d10723          	sb	t4,14(sp)
    14de:	01c107a3          	sb	t3,15(sp)
    14e2:	00610823          	sb	t1,16(sp)
    14e6:	011108a3          	sb	a7,17(sp)
    14ea:	00710923          	sb	t2,18(sp)
    14ee:	010109a3          	sb	a6,19(sp)
    14f2:	00a10a23          	sb	a0,20(sp)
    14f6:	00b10aa3          	sb	a1,21(sp)
    14fa:	00110b23          	sb	ra,22(sp)
    14fe:	00d10ba3          	sb	a3,23(sp)
    1502:	00810523          	sb	s0,10(sp)
    1506:	00074703          	lbu	a4,0(a4)
    150a:	0007c783          	lbu	a5,0(a5)
    write(f, s, l);
    150e:	002c                	addi	a1,sp,8
    1510:	4505                	li	a0,1
        buf[i++] = digits[x >> (sizeof(uint64) * 8 - 4)];
    1512:	00e10c23          	sb	a4,24(sp)
    1516:	00f10ca3          	sb	a5,25(sp)
    buf[i] = 0;
    151a:	00010d23          	sb	zero,26(sp)
    write(f, s, l);
    151e:	79c000ef          	jal	ra,1cba <write>
            // Print unknown % sequence to draw attention.
            putchar('%');
            putchar(s[1]);
            break;
        }
        s += 2;
    1522:	00248513          	addi	a0,s1,2
        if (!*s)
    1526:	00054783          	lbu	a5,0(a0)
    152a:	e8079ae3          	bnez	a5,13be <printf+0x4a>
    }
    va_end(ap);
}
    152e:	70a6                	ld	ra,104(sp)
    1530:	7406                	ld	s0,96(sp)
    1532:	64e6                	ld	s1,88(sp)
    1534:	6946                	ld	s2,80(sp)
    1536:	69a6                	ld	s3,72(sp)
    1538:	6a06                	ld	s4,64(sp)
    153a:	7ae2                	ld	s5,56(sp)
    153c:	7b42                	ld	s6,48(sp)
    153e:	614d                	addi	sp,sp,176
    1540:	8082                	ret
        for (z = s; s[0] == '%' && s[1] == '%'; z++, s += 2)
    1542:	00064783          	lbu	a5,0(a2)
    1546:	84b2                	mv	s1,a2
    1548:	01278963          	beq	a5,s2,155a <printf+0x1e6>
    154c:	b549                	j	13ce <printf+0x5a>
    154e:	0024c783          	lbu	a5,2(s1)
    1552:	0605                	addi	a2,a2,1
    1554:	0489                	addi	s1,s1,2
    1556:	e7279ce3          	bne	a5,s2,13ce <printf+0x5a>
    155a:	0014c783          	lbu	a5,1(s1)
    155e:	ff2788e3          	beq	a5,s2,154e <printf+0x1da>
        l = z - a;
    1562:	40a6043b          	subw	s0,a2,a0
    write(f, s, l);
    1566:	85aa                	mv	a1,a0
    1568:	8622                	mv	a2,s0
    156a:	4505                	li	a0,1
    156c:	74e000ef          	jal	ra,1cba <write>
        if (l)
    1570:	e60408e3          	beqz	s0,13e0 <printf+0x6c>
    1574:	8526                	mv	a0,s1
    1576:	b581                	j	13b6 <printf+0x42>
        switch (s[1])
    1578:	07800713          	li	a4,120
    157c:	04e79963          	bne	a5,a4,15ce <printf+0x25a>
            printint(va_arg(ap, int), 16, 1);
    1580:	6782                	ld	a5,0(sp)
    1582:	45c1                	li	a1,16
    1584:	4388                	lw	a0,0(a5)
    1586:	07a1                	addi	a5,a5,8
    1588:	e03e                	sd	a5,0(sp)
    158a:	b71ff0ef          	jal	ra,10fa <printint.constprop.0>
        s += 2;
    158e:	00248513          	addi	a0,s1,2
    1592:	bf51                	j	1526 <printf+0x1b2>
            printint(va_arg(ap, int), 10, 1);
    1594:	6782                	ld	a5,0(sp)
    1596:	45a9                	li	a1,10
    1598:	4388                	lw	a0,0(a5)
    159a:	07a1                	addi	a5,a5,8
    159c:	e03e                	sd	a5,0(sp)
    159e:	b5dff0ef          	jal	ra,10fa <printint.constprop.0>
        s += 2;
    15a2:	00248513          	addi	a0,s1,2
    15a6:	b741                	j	1526 <printf+0x1b2>
            if ((a = va_arg(ap, char *)) == 0)
    15a8:	6782                	ld	a5,0(sp)
    15aa:	6380                	ld	s0,0(a5)
    15ac:	07a1                	addi	a5,a5,8
    15ae:	e03e                	sd	a5,0(sp)
    15b0:	c031                	beqz	s0,15f4 <printf+0x280>
            l = strnlen(a, 200);
    15b2:	0c800593          	li	a1,200
    15b6:	8522                	mv	a0,s0
    15b8:	40a000ef          	jal	ra,19c2 <strnlen>
    write(f, s, l);
    15bc:	0005061b          	sext.w	a2,a0
    15c0:	85a2                	mv	a1,s0
    15c2:	4505                	li	a0,1
    15c4:	6f6000ef          	jal	ra,1cba <write>
        s += 2;
    15c8:	00248513          	addi	a0,s1,2
    15cc:	bfa9                	j	1526 <printf+0x1b2>
    return write(stdout, &byte, 1);
    15ce:	4605                	li	a2,1
    15d0:	002c                	addi	a1,sp,8
    15d2:	4505                	li	a0,1
    char byte = c;
    15d4:	01210423          	sb	s2,8(sp)
    return write(stdout, &byte, 1);
    15d8:	6e2000ef          	jal	ra,1cba <write>
    char byte = c;
    15dc:	0014c783          	lbu	a5,1(s1)
    return write(stdout, &byte, 1);
    15e0:	4605                	li	a2,1
    15e2:	002c                	addi	a1,sp,8
    15e4:	4505                	li	a0,1
    char byte = c;
    15e6:	00f10423          	sb	a5,8(sp)
    return write(stdout, &byte, 1);
    15ea:	6d0000ef          	jal	ra,1cba <write>
        s += 2;
    15ee:	00248513          	addi	a0,s1,2
    15f2:	bf15                	j	1526 <printf+0x1b2>
                a = "(null)";
    15f4:	845a                	mv	s0,s6
    15f6:	bf75                	j	15b2 <printf+0x23e>

00000000000015f8 <panic>:
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

void panic(char *m)
{
    15f8:	1141                	addi	sp,sp,-16
    15fa:	e406                	sd	ra,8(sp)
    puts(m);
    15fc:	d57ff0ef          	jal	ra,1352 <puts>
    exit(-100);
}
    1600:	60a2                	ld	ra,8(sp)
    exit(-100);
    1602:	f9c00513          	li	a0,-100
}
    1606:	0141                	addi	sp,sp,16
    exit(-100);
    1608:	a709                	j	1d0a <exit>

000000000000160a <isspace>:
#define HIGHS (ONES * (UCHAR_MAX / 2 + 1))
#define HASZERO(x) (((x)-ONES) & ~(x)&HIGHS)

int isspace(int c)
{
    return c == ' ' || (unsigned)c - '\t' < 5;
    160a:	02000793          	li	a5,32
    160e:	00f50663          	beq	a0,a5,161a <isspace+0x10>
    1612:	355d                	addiw	a0,a0,-9
    1614:	00553513          	sltiu	a0,a0,5
    1618:	8082                	ret
    161a:	4505                	li	a0,1
}
    161c:	8082                	ret

000000000000161e <isdigit>:

int isdigit(int c)
{
    return (unsigned)c - '0' < 10;
    161e:	fd05051b          	addiw	a0,a0,-48
}
    1622:	00a53513          	sltiu	a0,a0,10
    1626:	8082                	ret

0000000000001628 <atoi>:
    return c == ' ' || (unsigned)c - '\t' < 5;
    1628:	02000613          	li	a2,32
    162c:	4591                	li	a1,4

int atoi(const char *s)
{
    int n = 0, neg = 0;
    while (isspace(*s))
    162e:	00054703          	lbu	a4,0(a0)
    return c == ' ' || (unsigned)c - '\t' < 5;
    1632:	ff77069b          	addiw	a3,a4,-9
    1636:	04c70d63          	beq	a4,a2,1690 <atoi+0x68>
    163a:	0007079b          	sext.w	a5,a4
    163e:	04d5f963          	bgeu	a1,a3,1690 <atoi+0x68>
        s++;
    switch (*s)
    1642:	02b00693          	li	a3,43
    1646:	04d70a63          	beq	a4,a3,169a <atoi+0x72>
    164a:	02d00693          	li	a3,45
    164e:	06d70463          	beq	a4,a3,16b6 <atoi+0x8e>
        neg = 1;
    case '+':
        s++;
    }
    /* Compute n as a negative number to avoid overflow on INT_MIN */
    while (isdigit(*s))
    1652:	fd07859b          	addiw	a1,a5,-48
    1656:	4625                	li	a2,9
    1658:	873e                	mv	a4,a5
    165a:	86aa                	mv	a3,a0
    int n = 0, neg = 0;
    165c:	4e01                	li	t3,0
    while (isdigit(*s))
    165e:	04b66a63          	bltu	a2,a1,16b2 <atoi+0x8a>
    int n = 0, neg = 0;
    1662:	4501                	li	a0,0
    while (isdigit(*s))
    1664:	4825                	li	a6,9
    1666:	0016c603          	lbu	a2,1(a3)
        n = 10 * n - (*s++ - '0');
    166a:	0025179b          	slliw	a5,a0,0x2
    166e:	9d3d                	addw	a0,a0,a5
    1670:	fd07031b          	addiw	t1,a4,-48
    1674:	0015189b          	slliw	a7,a0,0x1
    while (isdigit(*s))
    1678:	fd06059b          	addiw	a1,a2,-48
        n = 10 * n - (*s++ - '0');
    167c:	0685                	addi	a3,a3,1
    167e:	4068853b          	subw	a0,a7,t1
    while (isdigit(*s))
    1682:	0006071b          	sext.w	a4,a2
    1686:	feb870e3          	bgeu	a6,a1,1666 <atoi+0x3e>
    return neg ? n : -n;
    168a:	000e0563          	beqz	t3,1694 <atoi+0x6c>
}
    168e:	8082                	ret
        s++;
    1690:	0505                	addi	a0,a0,1
    1692:	bf71                	j	162e <atoi+0x6>
    return neg ? n : -n;
    1694:	4113053b          	subw	a0,t1,a7
    1698:	8082                	ret
    while (isdigit(*s))
    169a:	00154783          	lbu	a5,1(a0)
    169e:	4625                	li	a2,9
        s++;
    16a0:	00150693          	addi	a3,a0,1
    while (isdigit(*s))
    16a4:	fd07859b          	addiw	a1,a5,-48
    16a8:	0007871b          	sext.w	a4,a5
    int n = 0, neg = 0;
    16ac:	4e01                	li	t3,0
    while (isdigit(*s))
    16ae:	fab67ae3          	bgeu	a2,a1,1662 <atoi+0x3a>
    16b2:	4501                	li	a0,0
}
    16b4:	8082                	ret
    while (isdigit(*s))
    16b6:	00154783          	lbu	a5,1(a0)
    16ba:	4625                	li	a2,9
        s++;
    16bc:	00150693          	addi	a3,a0,1
    while (isdigit(*s))
    16c0:	fd07859b          	addiw	a1,a5,-48
    16c4:	0007871b          	sext.w	a4,a5
    16c8:	feb665e3          	bltu	a2,a1,16b2 <atoi+0x8a>
        neg = 1;
    16cc:	4e05                	li	t3,1
    16ce:	bf51                	j	1662 <atoi+0x3a>

00000000000016d0 <memset>:

void *memset(void *dest, int c, size_t n)
{
    char *p = dest;
    for (int i = 0; i < n; ++i, *(p++) = c)
    16d0:	16060d63          	beqz	a2,184a <memset+0x17a>
    16d4:	40a007b3          	neg	a5,a0
    16d8:	8b9d                	andi	a5,a5,7
    16da:	00778713          	addi	a4,a5,7
    16de:	482d                	li	a6,11
    16e0:	0ff5f593          	andi	a1,a1,255
    16e4:	fff60693          	addi	a3,a2,-1
    16e8:	17076263          	bltu	a4,a6,184c <memset+0x17c>
    16ec:	16e6ea63          	bltu	a3,a4,1860 <memset+0x190>
    16f0:	16078563          	beqz	a5,185a <memset+0x18a>
    16f4:	00b50023          	sb	a1,0(a0)
    16f8:	4705                	li	a4,1
    16fa:	00150e93          	addi	t4,a0,1
    16fe:	14e78c63          	beq	a5,a4,1856 <memset+0x186>
    1702:	00b500a3          	sb	a1,1(a0)
    1706:	4709                	li	a4,2
    1708:	00250e93          	addi	t4,a0,2
    170c:	14e78d63          	beq	a5,a4,1866 <memset+0x196>
    1710:	00b50123          	sb	a1,2(a0)
    1714:	470d                	li	a4,3
    1716:	00350e93          	addi	t4,a0,3
    171a:	12e78b63          	beq	a5,a4,1850 <memset+0x180>
    171e:	00b501a3          	sb	a1,3(a0)
    1722:	4711                	li	a4,4
    1724:	00450e93          	addi	t4,a0,4
    1728:	14e78163          	beq	a5,a4,186a <memset+0x19a>
    172c:	00b50223          	sb	a1,4(a0)
    1730:	4715                	li	a4,5
    1732:	00550e93          	addi	t4,a0,5
    1736:	12e78c63          	beq	a5,a4,186e <memset+0x19e>
    173a:	00b502a3          	sb	a1,5(a0)
    173e:	471d                	li	a4,7
    1740:	00650e93          	addi	t4,a0,6
    1744:	12e79763          	bne	a5,a4,1872 <memset+0x1a2>
    1748:	00750e93          	addi	t4,a0,7
    174c:	00b50323          	sb	a1,6(a0)
    1750:	4f1d                	li	t5,7
    1752:	00859713          	slli	a4,a1,0x8
    1756:	8f4d                	or	a4,a4,a1
    1758:	01059e13          	slli	t3,a1,0x10
    175c:	01c76e33          	or	t3,a4,t3
    1760:	01859313          	slli	t1,a1,0x18
    1764:	006e6333          	or	t1,t3,t1
    1768:	02059893          	slli	a7,a1,0x20
    176c:	011368b3          	or	a7,t1,a7
    1770:	02859813          	slli	a6,a1,0x28
    1774:	40f60333          	sub	t1,a2,a5
    1778:	0108e833          	or	a6,a7,a6
    177c:	03059693          	slli	a3,a1,0x30
    1780:	00d866b3          	or	a3,a6,a3
    1784:	03859713          	slli	a4,a1,0x38
    1788:	97aa                	add	a5,a5,a0
    178a:	ff837813          	andi	a6,t1,-8
    178e:	8f55                	or	a4,a4,a3
    1790:	00f806b3          	add	a3,a6,a5
    1794:	e398                	sd	a4,0(a5)
    1796:	07a1                	addi	a5,a5,8
    1798:	fed79ee3          	bne	a5,a3,1794 <memset+0xc4>
    179c:	ff837693          	andi	a3,t1,-8
    17a0:	00de87b3          	add	a5,t4,a3
    17a4:	01e6873b          	addw	a4,a3,t5
    17a8:	0ad30663          	beq	t1,a3,1854 <memset+0x184>
    17ac:	00b78023          	sb	a1,0(a5)
    17b0:	0017069b          	addiw	a3,a4,1
    17b4:	08c6fb63          	bgeu	a3,a2,184a <memset+0x17a>
    17b8:	00b780a3          	sb	a1,1(a5)
    17bc:	0027069b          	addiw	a3,a4,2
    17c0:	08c6f563          	bgeu	a3,a2,184a <memset+0x17a>
    17c4:	00b78123          	sb	a1,2(a5)
    17c8:	0037069b          	addiw	a3,a4,3
    17cc:	06c6ff63          	bgeu	a3,a2,184a <memset+0x17a>
    17d0:	00b781a3          	sb	a1,3(a5)
    17d4:	0047069b          	addiw	a3,a4,4
    17d8:	06c6f963          	bgeu	a3,a2,184a <memset+0x17a>
    17dc:	00b78223          	sb	a1,4(a5)
    17e0:	0057069b          	addiw	a3,a4,5
    17e4:	06c6f363          	bgeu	a3,a2,184a <memset+0x17a>
    17e8:	00b782a3          	sb	a1,5(a5)
    17ec:	0067069b          	addiw	a3,a4,6
    17f0:	04c6fd63          	bgeu	a3,a2,184a <memset+0x17a>
    17f4:	00b78323          	sb	a1,6(a5)
    17f8:	0077069b          	addiw	a3,a4,7
    17fc:	04c6f763          	bgeu	a3,a2,184a <memset+0x17a>
    1800:	00b783a3          	sb	a1,7(a5)
    1804:	0087069b          	addiw	a3,a4,8
    1808:	04c6f163          	bgeu	a3,a2,184a <memset+0x17a>
    180c:	00b78423          	sb	a1,8(a5)
    1810:	0097069b          	addiw	a3,a4,9
    1814:	02c6fb63          	bgeu	a3,a2,184a <memset+0x17a>
    1818:	00b784a3          	sb	a1,9(a5)
    181c:	00a7069b          	addiw	a3,a4,10
    1820:	02c6f563          	bgeu	a3,a2,184a <memset+0x17a>
    1824:	00b78523          	sb	a1,10(a5)
    1828:	00b7069b          	addiw	a3,a4,11
    182c:	00c6ff63          	bgeu	a3,a2,184a <memset+0x17a>
    1830:	00b785a3          	sb	a1,11(a5)
    1834:	00c7069b          	addiw	a3,a4,12
    1838:	00c6f963          	bgeu	a3,a2,184a <memset+0x17a>
    183c:	00b78623          	sb	a1,12(a5)
    1840:	2735                	addiw	a4,a4,13
    1842:	00c77463          	bgeu	a4,a2,184a <memset+0x17a>
    1846:	00b786a3          	sb	a1,13(a5)
        ;
    return dest;
}
    184a:	8082                	ret
    184c:	472d                	li	a4,11
    184e:	bd79                	j	16ec <memset+0x1c>
    for (int i = 0; i < n; ++i, *(p++) = c)
    1850:	4f0d                	li	t5,3
    1852:	b701                	j	1752 <memset+0x82>
    1854:	8082                	ret
    1856:	4f05                	li	t5,1
    1858:	bded                	j	1752 <memset+0x82>
    185a:	8eaa                	mv	t4,a0
    185c:	4f01                	li	t5,0
    185e:	bdd5                	j	1752 <memset+0x82>
    1860:	87aa                	mv	a5,a0
    1862:	4701                	li	a4,0
    1864:	b7a1                	j	17ac <memset+0xdc>
    1866:	4f09                	li	t5,2
    1868:	b5ed                	j	1752 <memset+0x82>
    186a:	4f11                	li	t5,4
    186c:	b5dd                	j	1752 <memset+0x82>
    186e:	4f15                	li	t5,5
    1870:	b5cd                	j	1752 <memset+0x82>
    1872:	4f19                	li	t5,6
    1874:	bdf9                	j	1752 <memset+0x82>

0000000000001876 <strcmp>:

int strcmp(const char *l, const char *r)
{
    for (; *l == *r && *l; l++, r++)
    1876:	00054783          	lbu	a5,0(a0)
    187a:	0005c703          	lbu	a4,0(a1)
    187e:	00e79863          	bne	a5,a4,188e <strcmp+0x18>
    1882:	0505                	addi	a0,a0,1
    1884:	0585                	addi	a1,a1,1
    1886:	fbe5                	bnez	a5,1876 <strcmp>
    1888:	4501                	li	a0,0
        ;
    return *(unsigned char *)l - *(unsigned char *)r;
}
    188a:	9d19                	subw	a0,a0,a4
    188c:	8082                	ret
    return *(unsigned char *)l - *(unsigned char *)r;
    188e:	0007851b          	sext.w	a0,a5
    1892:	bfe5                	j	188a <strcmp+0x14>

0000000000001894 <strncmp>:

int strncmp(const char *_l, const char *_r, size_t n)
{
    const unsigned char *l = (void *)_l, *r = (void *)_r;
    if (!n--)
    1894:	ce05                	beqz	a2,18cc <strncmp+0x38>
        return 0;
    for (; *l && *r && n && *l == *r; l++, r++, n--)
    1896:	00054703          	lbu	a4,0(a0)
    189a:	0005c783          	lbu	a5,0(a1)
    189e:	cb0d                	beqz	a4,18d0 <strncmp+0x3c>
    if (!n--)
    18a0:	167d                	addi	a2,a2,-1
    18a2:	00c506b3          	add	a3,a0,a2
    18a6:	a819                	j	18bc <strncmp+0x28>
    for (; *l && *r && n && *l == *r; l++, r++, n--)
    18a8:	00a68e63          	beq	a3,a0,18c4 <strncmp+0x30>
    18ac:	0505                	addi	a0,a0,1
    18ae:	00e79b63          	bne	a5,a4,18c4 <strncmp+0x30>
    18b2:	00054703          	lbu	a4,0(a0)
        ;
    return *l - *r;
    18b6:	0005c783          	lbu	a5,0(a1)
    for (; *l && *r && n && *l == *r; l++, r++, n--)
    18ba:	cb19                	beqz	a4,18d0 <strncmp+0x3c>
    18bc:	0005c783          	lbu	a5,0(a1)
    18c0:	0585                	addi	a1,a1,1
    18c2:	f3fd                	bnez	a5,18a8 <strncmp+0x14>
    return *l - *r;
    18c4:	0007051b          	sext.w	a0,a4
    18c8:	9d1d                	subw	a0,a0,a5
    18ca:	8082                	ret
        return 0;
    18cc:	4501                	li	a0,0
}
    18ce:	8082                	ret
    18d0:	4501                	li	a0,0
    return *l - *r;
    18d2:	9d1d                	subw	a0,a0,a5
    18d4:	8082                	ret

00000000000018d6 <strlen>:
size_t strlen(const char *s)
{
    const char *a = s;
    typedef size_t __attribute__((__may_alias__)) word;
    const word *w;
    for (; (uintptr_t)s % SS; s++)
    18d6:	00757793          	andi	a5,a0,7
    18da:	cf89                	beqz	a5,18f4 <strlen+0x1e>
    18dc:	87aa                	mv	a5,a0
    18de:	a029                	j	18e8 <strlen+0x12>
    18e0:	0785                	addi	a5,a5,1
    18e2:	0077f713          	andi	a4,a5,7
    18e6:	cb01                	beqz	a4,18f6 <strlen+0x20>
        if (!*s)
    18e8:	0007c703          	lbu	a4,0(a5)
    18ec:	fb75                	bnez	a4,18e0 <strlen+0xa>
    for (w = (const void *)s; !HASZERO(*w); w++)
        ;
    s = (const void *)w;
    for (; *s; s++)
        ;
    return s - a;
    18ee:	40a78533          	sub	a0,a5,a0
}
    18f2:	8082                	ret
    for (; (uintptr_t)s % SS; s++)
    18f4:	87aa                	mv	a5,a0
    for (w = (const void *)s; !HASZERO(*w); w++)
    18f6:	6394                	ld	a3,0(a5)
    18f8:	00000597          	auipc	a1,0x0
    18fc:	6c05b583          	ld	a1,1728(a1) # 1fb8 <__clone+0xca>
    1900:	00000617          	auipc	a2,0x0
    1904:	6c063603          	ld	a2,1728(a2) # 1fc0 <__clone+0xd2>
    1908:	a019                	j	190e <strlen+0x38>
    190a:	6794                	ld	a3,8(a5)
    190c:	07a1                	addi	a5,a5,8
    190e:	00b68733          	add	a4,a3,a1
    1912:	fff6c693          	not	a3,a3
    1916:	8f75                	and	a4,a4,a3
    1918:	8f71                	and	a4,a4,a2
    191a:	db65                	beqz	a4,190a <strlen+0x34>
    for (; *s; s++)
    191c:	0007c703          	lbu	a4,0(a5)
    1920:	d779                	beqz	a4,18ee <strlen+0x18>
    1922:	0017c703          	lbu	a4,1(a5)
    1926:	0785                	addi	a5,a5,1
    1928:	d379                	beqz	a4,18ee <strlen+0x18>
    192a:	0017c703          	lbu	a4,1(a5)
    192e:	0785                	addi	a5,a5,1
    1930:	fb6d                	bnez	a4,1922 <strlen+0x4c>
    1932:	bf75                	j	18ee <strlen+0x18>

0000000000001934 <memchr>:

void *memchr(const void *src, int c, size_t n)
{
    const unsigned char *s = src;
    c = (unsigned char)c;
    for (; ((uintptr_t)s & ALIGN) && n && *s != c; s++, n--)
    1934:	00757713          	andi	a4,a0,7
{
    1938:	87aa                	mv	a5,a0
    c = (unsigned char)c;
    193a:	0ff5f593          	andi	a1,a1,255
    for (; ((uintptr_t)s & ALIGN) && n && *s != c; s++, n--)
    193e:	cb19                	beqz	a4,1954 <memchr+0x20>
    1940:	ce25                	beqz	a2,19b8 <memchr+0x84>
    1942:	0007c703          	lbu	a4,0(a5)
    1946:	04b70e63          	beq	a4,a1,19a2 <memchr+0x6e>
    194a:	0785                	addi	a5,a5,1
    194c:	0077f713          	andi	a4,a5,7
    1950:	167d                	addi	a2,a2,-1
    1952:	f77d                	bnez	a4,1940 <memchr+0xc>
            ;
        s = (const void *)w;
    }
    for (; n && *s != c; s++, n--)
        ;
    return n ? (void *)s : 0;
    1954:	4501                	li	a0,0
    if (n && *s != c)
    1956:	c235                	beqz	a2,19ba <memchr+0x86>
    1958:	0007c703          	lbu	a4,0(a5)
    195c:	04b70363          	beq	a4,a1,19a2 <memchr+0x6e>
        size_t k = ONES * c;
    1960:	00000517          	auipc	a0,0x0
    1964:	66853503          	ld	a0,1640(a0) # 1fc8 <__clone+0xda>
        for (w = (const void *)s; n >= SS && !HASZERO(*w ^ k); w++, n -= SS)
    1968:	471d                	li	a4,7
        size_t k = ONES * c;
    196a:	02a58533          	mul	a0,a1,a0
        for (w = (const void *)s; n >= SS && !HASZERO(*w ^ k); w++, n -= SS)
    196e:	02c77a63          	bgeu	a4,a2,19a2 <memchr+0x6e>
    1972:	00000897          	auipc	a7,0x0
    1976:	6468b883          	ld	a7,1606(a7) # 1fb8 <__clone+0xca>
    197a:	00000817          	auipc	a6,0x0
    197e:	64683803          	ld	a6,1606(a6) # 1fc0 <__clone+0xd2>
    1982:	431d                	li	t1,7
    1984:	a029                	j	198e <memchr+0x5a>
    1986:	1661                	addi	a2,a2,-8
    1988:	07a1                	addi	a5,a5,8
    198a:	02c37963          	bgeu	t1,a2,19bc <memchr+0x88>
    198e:	6398                	ld	a4,0(a5)
    1990:	8f29                	xor	a4,a4,a0
    1992:	011706b3          	add	a3,a4,a7
    1996:	fff74713          	not	a4,a4
    199a:	8f75                	and	a4,a4,a3
    199c:	01077733          	and	a4,a4,a6
    19a0:	d37d                	beqz	a4,1986 <memchr+0x52>
    19a2:	853e                	mv	a0,a5
    19a4:	97b2                	add	a5,a5,a2
    19a6:	a021                	j	19ae <memchr+0x7a>
    for (; n && *s != c; s++, n--)
    19a8:	0505                	addi	a0,a0,1
    19aa:	00f50763          	beq	a0,a5,19b8 <memchr+0x84>
    19ae:	00054703          	lbu	a4,0(a0)
    19b2:	feb71be3          	bne	a4,a1,19a8 <memchr+0x74>
    19b6:	8082                	ret
    return n ? (void *)s : 0;
    19b8:	4501                	li	a0,0
}
    19ba:	8082                	ret
    return n ? (void *)s : 0;
    19bc:	4501                	li	a0,0
    for (; n && *s != c; s++, n--)
    19be:	f275                	bnez	a2,19a2 <memchr+0x6e>
}
    19c0:	8082                	ret

00000000000019c2 <strnlen>:

size_t strnlen(const char *s, size_t n)
{
    19c2:	1101                	addi	sp,sp,-32
    19c4:	e822                	sd	s0,16(sp)
    const char *p = memchr(s, 0, n);
    19c6:	862e                	mv	a2,a1
{
    19c8:	842e                	mv	s0,a1
    const char *p = memchr(s, 0, n);
    19ca:	4581                	li	a1,0
{
    19cc:	e426                	sd	s1,8(sp)
    19ce:	ec06                	sd	ra,24(sp)
    19d0:	84aa                	mv	s1,a0
    const char *p = memchr(s, 0, n);
    19d2:	f63ff0ef          	jal	ra,1934 <memchr>
    return p ? p - s : n;
    19d6:	c519                	beqz	a0,19e4 <strnlen+0x22>
}
    19d8:	60e2                	ld	ra,24(sp)
    19da:	6442                	ld	s0,16(sp)
    return p ? p - s : n;
    19dc:	8d05                	sub	a0,a0,s1
}
    19de:	64a2                	ld	s1,8(sp)
    19e0:	6105                	addi	sp,sp,32
    19e2:	8082                	ret
    19e4:	60e2                	ld	ra,24(sp)
    return p ? p - s : n;
    19e6:	8522                	mv	a0,s0
}
    19e8:	6442                	ld	s0,16(sp)
    19ea:	64a2                	ld	s1,8(sp)
    19ec:	6105                	addi	sp,sp,32
    19ee:	8082                	ret

00000000000019f0 <strcpy>:
char *strcpy(char *restrict d, const char *s)
{
    typedef size_t __attribute__((__may_alias__)) word;
    word *wd;
    const word *ws;
    if ((uintptr_t)s % SS == (uintptr_t)d % SS)
    19f0:	00b547b3          	xor	a5,a0,a1
    19f4:	8b9d                	andi	a5,a5,7
    19f6:	eb95                	bnez	a5,1a2a <strcpy+0x3a>
    {
        for (; (uintptr_t)s % SS; s++, d++)
    19f8:	0075f793          	andi	a5,a1,7
    19fc:	e7b1                	bnez	a5,1a48 <strcpy+0x58>
            if (!(*d = *s))
                return d;
        wd = (void *)d;
        ws = (const void *)s;
        for (; !HASZERO(*ws); *wd++ = *ws++)
    19fe:	6198                	ld	a4,0(a1)
    1a00:	00000617          	auipc	a2,0x0
    1a04:	5b863603          	ld	a2,1464(a2) # 1fb8 <__clone+0xca>
    1a08:	00000817          	auipc	a6,0x0
    1a0c:	5b883803          	ld	a6,1464(a6) # 1fc0 <__clone+0xd2>
    1a10:	a029                	j	1a1a <strcpy+0x2a>
    1a12:	e118                	sd	a4,0(a0)
    1a14:	6598                	ld	a4,8(a1)
    1a16:	05a1                	addi	a1,a1,8
    1a18:	0521                	addi	a0,a0,8
    1a1a:	00c707b3          	add	a5,a4,a2
    1a1e:	fff74693          	not	a3,a4
    1a22:	8ff5                	and	a5,a5,a3
    1a24:	0107f7b3          	and	a5,a5,a6
    1a28:	d7ed                	beqz	a5,1a12 <strcpy+0x22>
            ;
        d = (void *)wd;
        s = (const void *)ws;
    }
    for (; (*d = *s); s++, d++)
    1a2a:	0005c783          	lbu	a5,0(a1)
    1a2e:	00f50023          	sb	a5,0(a0)
    1a32:	c785                	beqz	a5,1a5a <strcpy+0x6a>
    1a34:	0015c783          	lbu	a5,1(a1)
    1a38:	0505                	addi	a0,a0,1
    1a3a:	0585                	addi	a1,a1,1
    1a3c:	00f50023          	sb	a5,0(a0)
    1a40:	fbf5                	bnez	a5,1a34 <strcpy+0x44>
        ;
    return d;
}
    1a42:	8082                	ret
        for (; (uintptr_t)s % SS; s++, d++)
    1a44:	0505                	addi	a0,a0,1
    1a46:	df45                	beqz	a4,19fe <strcpy+0xe>
            if (!(*d = *s))
    1a48:	0005c783          	lbu	a5,0(a1)
        for (; (uintptr_t)s % SS; s++, d++)
    1a4c:	0585                	addi	a1,a1,1
    1a4e:	0075f713          	andi	a4,a1,7
            if (!(*d = *s))
    1a52:	00f50023          	sb	a5,0(a0)
    1a56:	f7fd                	bnez	a5,1a44 <strcpy+0x54>
}
    1a58:	8082                	ret
    1a5a:	8082                	ret

0000000000001a5c <strncpy>:
char *strncpy(char *restrict d, const char *s, size_t n)
{
    typedef size_t __attribute__((__may_alias__)) word;
    word *wd;
    const word *ws;
    if (((uintptr_t)s & ALIGN) == ((uintptr_t)d & ALIGN))
    1a5c:	00b547b3          	xor	a5,a0,a1
    1a60:	8b9d                	andi	a5,a5,7
    1a62:	1a079863          	bnez	a5,1c12 <strncpy+0x1b6>
    {
        for (; ((uintptr_t)s & ALIGN) && n && (*d = *s); n--, s++, d++)
    1a66:	0075f793          	andi	a5,a1,7
    1a6a:	16078463          	beqz	a5,1bd2 <strncpy+0x176>
    1a6e:	ea01                	bnez	a2,1a7e <strncpy+0x22>
    1a70:	a421                	j	1c78 <strncpy+0x21c>
    1a72:	167d                	addi	a2,a2,-1
    1a74:	0505                	addi	a0,a0,1
    1a76:	14070e63          	beqz	a4,1bd2 <strncpy+0x176>
    1a7a:	1a060863          	beqz	a2,1c2a <strncpy+0x1ce>
    1a7e:	0005c783          	lbu	a5,0(a1)
    1a82:	0585                	addi	a1,a1,1
    1a84:	0075f713          	andi	a4,a1,7
    1a88:	00f50023          	sb	a5,0(a0)
    1a8c:	f3fd                	bnez	a5,1a72 <strncpy+0x16>
    1a8e:	4805                	li	a6,1
    1a90:	1a061863          	bnez	a2,1c40 <strncpy+0x1e4>
    1a94:	40a007b3          	neg	a5,a0
    1a98:	8b9d                	andi	a5,a5,7
    1a9a:	4681                	li	a3,0
    1a9c:	18061a63          	bnez	a2,1c30 <strncpy+0x1d4>
    1aa0:	00778713          	addi	a4,a5,7
    1aa4:	45ad                	li	a1,11
    1aa6:	18b76363          	bltu	a4,a1,1c2c <strncpy+0x1d0>
    1aaa:	1ae6eb63          	bltu	a3,a4,1c60 <strncpy+0x204>
    1aae:	1a078363          	beqz	a5,1c54 <strncpy+0x1f8>
    for (int i = 0; i < n; ++i, *(p++) = c)
    1ab2:	00050023          	sb	zero,0(a0)
    1ab6:	4685                	li	a3,1
    1ab8:	00150713          	addi	a4,a0,1
    1abc:	18d78f63          	beq	a5,a3,1c5a <strncpy+0x1fe>
    1ac0:	000500a3          	sb	zero,1(a0)
    1ac4:	4689                	li	a3,2
    1ac6:	00250713          	addi	a4,a0,2
    1aca:	18d78e63          	beq	a5,a3,1c66 <strncpy+0x20a>
    1ace:	00050123          	sb	zero,2(a0)
    1ad2:	468d                	li	a3,3
    1ad4:	00350713          	addi	a4,a0,3
    1ad8:	16d78c63          	beq	a5,a3,1c50 <strncpy+0x1f4>
    1adc:	000501a3          	sb	zero,3(a0)
    1ae0:	4691                	li	a3,4
    1ae2:	00450713          	addi	a4,a0,4
    1ae6:	18d78263          	beq	a5,a3,1c6a <strncpy+0x20e>
    1aea:	00050223          	sb	zero,4(a0)
    1aee:	4695                	li	a3,5
    1af0:	00550713          	addi	a4,a0,5
    1af4:	16d78d63          	beq	a5,a3,1c6e <strncpy+0x212>
    1af8:	000502a3          	sb	zero,5(a0)
    1afc:	469d                	li	a3,7
    1afe:	00650713          	addi	a4,a0,6
    1b02:	16d79863          	bne	a5,a3,1c72 <strncpy+0x216>
    1b06:	00750713          	addi	a4,a0,7
    1b0a:	00050323          	sb	zero,6(a0)
    1b0e:	40f80833          	sub	a6,a6,a5
    1b12:	ff887593          	andi	a1,a6,-8
    1b16:	97aa                	add	a5,a5,a0
    1b18:	95be                	add	a1,a1,a5
    1b1a:	0007b023          	sd	zero,0(a5)
    1b1e:	07a1                	addi	a5,a5,8
    1b20:	feb79de3          	bne	a5,a1,1b1a <strncpy+0xbe>
    1b24:	ff887593          	andi	a1,a6,-8
    1b28:	9ead                	addw	a3,a3,a1
    1b2a:	00b707b3          	add	a5,a4,a1
    1b2e:	12b80863          	beq	a6,a1,1c5e <strncpy+0x202>
    1b32:	00078023          	sb	zero,0(a5)
    1b36:	0016871b          	addiw	a4,a3,1
    1b3a:	0ec77863          	bgeu	a4,a2,1c2a <strncpy+0x1ce>
    1b3e:	000780a3          	sb	zero,1(a5)
    1b42:	0026871b          	addiw	a4,a3,2
    1b46:	0ec77263          	bgeu	a4,a2,1c2a <strncpy+0x1ce>
    1b4a:	00078123          	sb	zero,2(a5)
    1b4e:	0036871b          	addiw	a4,a3,3
    1b52:	0cc77c63          	bgeu	a4,a2,1c2a <strncpy+0x1ce>
    1b56:	000781a3          	sb	zero,3(a5)
    1b5a:	0046871b          	addiw	a4,a3,4
    1b5e:	0cc77663          	bgeu	a4,a2,1c2a <strncpy+0x1ce>
    1b62:	00078223          	sb	zero,4(a5)
    1b66:	0056871b          	addiw	a4,a3,5
    1b6a:	0cc77063          	bgeu	a4,a2,1c2a <strncpy+0x1ce>
    1b6e:	000782a3          	sb	zero,5(a5)
    1b72:	0066871b          	addiw	a4,a3,6
    1b76:	0ac77a63          	bgeu	a4,a2,1c2a <strncpy+0x1ce>
    1b7a:	00078323          	sb	zero,6(a5)
    1b7e:	0076871b          	addiw	a4,a3,7
    1b82:	0ac77463          	bgeu	a4,a2,1c2a <strncpy+0x1ce>
    1b86:	000783a3          	sb	zero,7(a5)
    1b8a:	0086871b          	addiw	a4,a3,8
    1b8e:	08c77e63          	bgeu	a4,a2,1c2a <strncpy+0x1ce>
    1b92:	00078423          	sb	zero,8(a5)
    1b96:	0096871b          	addiw	a4,a3,9
    1b9a:	08c77863          	bgeu	a4,a2,1c2a <strncpy+0x1ce>
    1b9e:	000784a3          	sb	zero,9(a5)
    1ba2:	00a6871b          	addiw	a4,a3,10
    1ba6:	08c77263          	bgeu	a4,a2,1c2a <strncpy+0x1ce>
    1baa:	00078523          	sb	zero,10(a5)
    1bae:	00b6871b          	addiw	a4,a3,11
    1bb2:	06c77c63          	bgeu	a4,a2,1c2a <strncpy+0x1ce>
    1bb6:	000785a3          	sb	zero,11(a5)
    1bba:	00c6871b          	addiw	a4,a3,12
    1bbe:	06c77663          	bgeu	a4,a2,1c2a <strncpy+0x1ce>
    1bc2:	00078623          	sb	zero,12(a5)
    1bc6:	26b5                	addiw	a3,a3,13
    1bc8:	06c6f163          	bgeu	a3,a2,1c2a <strncpy+0x1ce>
    1bcc:	000786a3          	sb	zero,13(a5)
    1bd0:	8082                	ret
            ;
        if (!n || !*s)
    1bd2:	c645                	beqz	a2,1c7a <strncpy+0x21e>
    1bd4:	0005c783          	lbu	a5,0(a1)
    1bd8:	ea078be3          	beqz	a5,1a8e <strncpy+0x32>
            goto tail;
        wd = (void *)d;
        ws = (const void *)s;
        for (; n >= sizeof(size_t) && !HASZERO(*ws); n -= sizeof(size_t), ws++, wd++)
    1bdc:	479d                	li	a5,7
    1bde:	02c7ff63          	bgeu	a5,a2,1c1c <strncpy+0x1c0>
    1be2:	00000897          	auipc	a7,0x0
    1be6:	3d68b883          	ld	a7,982(a7) # 1fb8 <__clone+0xca>
    1bea:	00000817          	auipc	a6,0x0
    1bee:	3d683803          	ld	a6,982(a6) # 1fc0 <__clone+0xd2>
    1bf2:	431d                	li	t1,7
    1bf4:	6198                	ld	a4,0(a1)
    1bf6:	011707b3          	add	a5,a4,a7
    1bfa:	fff74693          	not	a3,a4
    1bfe:	8ff5                	and	a5,a5,a3
    1c00:	0107f7b3          	and	a5,a5,a6
    1c04:	ef81                	bnez	a5,1c1c <strncpy+0x1c0>
            *wd = *ws;
    1c06:	e118                	sd	a4,0(a0)
        for (; n >= sizeof(size_t) && !HASZERO(*ws); n -= sizeof(size_t), ws++, wd++)
    1c08:	1661                	addi	a2,a2,-8
    1c0a:	05a1                	addi	a1,a1,8
    1c0c:	0521                	addi	a0,a0,8
    1c0e:	fec363e3          	bltu	t1,a2,1bf4 <strncpy+0x198>
        d = (void *)wd;
        s = (const void *)ws;
    }
    for (; n && (*d = *s); n--, s++, d++)
    1c12:	e609                	bnez	a2,1c1c <strncpy+0x1c0>
    1c14:	a08d                	j	1c76 <strncpy+0x21a>
    1c16:	167d                	addi	a2,a2,-1
    1c18:	0505                	addi	a0,a0,1
    1c1a:	ca01                	beqz	a2,1c2a <strncpy+0x1ce>
    1c1c:	0005c783          	lbu	a5,0(a1)
    1c20:	0585                	addi	a1,a1,1
    1c22:	00f50023          	sb	a5,0(a0)
    1c26:	fbe5                	bnez	a5,1c16 <strncpy+0x1ba>
        ;
tail:
    1c28:	b59d                	j	1a8e <strncpy+0x32>
    memset(d, 0, n);
    return d;
}
    1c2a:	8082                	ret
    1c2c:	472d                	li	a4,11
    1c2e:	bdb5                	j	1aaa <strncpy+0x4e>
    1c30:	00778713          	addi	a4,a5,7
    1c34:	45ad                	li	a1,11
    1c36:	fff60693          	addi	a3,a2,-1
    1c3a:	e6b778e3          	bgeu	a4,a1,1aaa <strncpy+0x4e>
    1c3e:	b7fd                	j	1c2c <strncpy+0x1d0>
    1c40:	40a007b3          	neg	a5,a0
    1c44:	8832                	mv	a6,a2
    1c46:	8b9d                	andi	a5,a5,7
    1c48:	4681                	li	a3,0
    1c4a:	e4060be3          	beqz	a2,1aa0 <strncpy+0x44>
    1c4e:	b7cd                	j	1c30 <strncpy+0x1d4>
    for (int i = 0; i < n; ++i, *(p++) = c)
    1c50:	468d                	li	a3,3
    1c52:	bd75                	j	1b0e <strncpy+0xb2>
    1c54:	872a                	mv	a4,a0
    1c56:	4681                	li	a3,0
    1c58:	bd5d                	j	1b0e <strncpy+0xb2>
    1c5a:	4685                	li	a3,1
    1c5c:	bd4d                	j	1b0e <strncpy+0xb2>
    1c5e:	8082                	ret
    1c60:	87aa                	mv	a5,a0
    1c62:	4681                	li	a3,0
    1c64:	b5f9                	j	1b32 <strncpy+0xd6>
    1c66:	4689                	li	a3,2
    1c68:	b55d                	j	1b0e <strncpy+0xb2>
    1c6a:	4691                	li	a3,4
    1c6c:	b54d                	j	1b0e <strncpy+0xb2>
    1c6e:	4695                	li	a3,5
    1c70:	bd79                	j	1b0e <strncpy+0xb2>
    1c72:	4699                	li	a3,6
    1c74:	bd69                	j	1b0e <strncpy+0xb2>
    1c76:	8082                	ret
    1c78:	8082                	ret
    1c7a:	8082                	ret

0000000000001c7c <open>:
#include <unistd.h>

#include "syscall.h"

int open(const char *path, int flags)
{
    1c7c:	87aa                	mv	a5,a0
    1c7e:	862e                	mv	a2,a1
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
}

static inline long __syscall4(long n, long a, long b, long c, long d)
{
    register long a7 __asm__("a7") = n;
    1c80:	03800893          	li	a7,56
    register long a0 __asm__("a0") = a;
    1c84:	f9c00513          	li	a0,-100
    register long a1 __asm__("a1") = b;
    1c88:	85be                	mv	a1,a5
    register long a2 __asm__("a2") = c;
    register long a3 __asm__("a3") = d;
    1c8a:	4689                	li	a3,2
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3))
    1c8c:	00000073          	ecall
    return syscall(SYS_openat, AT_FDCWD, path, flags, O_RDWR);
}
    1c90:	2501                	sext.w	a0,a0
    1c92:	8082                	ret

0000000000001c94 <openat>:
    register long a7 __asm__("a7") = n;
    1c94:	03800893          	li	a7,56
    register long a3 __asm__("a3") = d;
    1c98:	18000693          	li	a3,384
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3))
    1c9c:	00000073          	ecall

int openat(int dirfd,const char *path, int flags)
{
    return syscall(SYS_openat, dirfd, path, flags, 0600);
}
    1ca0:	2501                	sext.w	a0,a0
    1ca2:	8082                	ret

0000000000001ca4 <close>:
    register long a7 __asm__("a7") = n;
    1ca4:	03900893          	li	a7,57
    __asm_syscall("r"(a7), "0"(a0))
    1ca8:	00000073          	ecall

int close(int fd)
{
    return syscall(SYS_close, fd);
}
    1cac:	2501                	sext.w	a0,a0
    1cae:	8082                	ret

0000000000001cb0 <read>:
    register long a7 __asm__("a7") = n;
    1cb0:	03f00893          	li	a7,63
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1cb4:	00000073          	ecall

ssize_t read(int fd, void *buf, size_t len)
{
    return syscall(SYS_read, fd, buf, len);
}
    1cb8:	8082                	ret

0000000000001cba <write>:
    register long a7 __asm__("a7") = n;
    1cba:	04000893          	li	a7,64
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1cbe:	00000073          	ecall

ssize_t write(int fd, const void *buf, size_t len)
{
    return syscall(SYS_write, fd, buf, len);
}
    1cc2:	8082                	ret

0000000000001cc4 <getpid>:
    register long a7 __asm__("a7") = n;
    1cc4:	0ac00893          	li	a7,172
    __asm_syscall("r"(a7))
    1cc8:	00000073          	ecall

pid_t getpid(void)
{
    return syscall(SYS_getpid);
}
    1ccc:	2501                	sext.w	a0,a0
    1cce:	8082                	ret

0000000000001cd0 <getppid>:
    register long a7 __asm__("a7") = n;
    1cd0:	0ad00893          	li	a7,173
    __asm_syscall("r"(a7))
    1cd4:	00000073          	ecall

pid_t getppid(void)
{
    return syscall(SYS_getppid);
}
    1cd8:	2501                	sext.w	a0,a0
    1cda:	8082                	ret

0000000000001cdc <sched_yield>:
    register long a7 __asm__("a7") = n;
    1cdc:	07c00893          	li	a7,124
    __asm_syscall("r"(a7))
    1ce0:	00000073          	ecall

int sched_yield(void)
{
    return syscall(SYS_sched_yield);
}
    1ce4:	2501                	sext.w	a0,a0
    1ce6:	8082                	ret

0000000000001ce8 <fork>:
    register long a7 __asm__("a7") = n;
    1ce8:	0dc00893          	li	a7,220
    register long a0 __asm__("a0") = a;
    1cec:	4545                	li	a0,17
    register long a1 __asm__("a1") = b;
    1cee:	4581                	li	a1,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1cf0:	00000073          	ecall

pid_t fork(void)
{
    return syscall(SYS_clone, SIGCHLD, 0);
}
    1cf4:	2501                	sext.w	a0,a0
    1cf6:	8082                	ret

0000000000001cf8 <clone>:

pid_t clone(int (*fn)(void *arg), void *arg, void *stack, size_t stack_size, unsigned long flags)
{
    1cf8:	85b2                	mv	a1,a2
    1cfa:	863a                	mv	a2,a4
    if (stack)
    1cfc:	c191                	beqz	a1,1d00 <clone+0x8>
	stack += stack_size;
    1cfe:	95b6                	add	a1,a1,a3

    return __clone(fn, stack, flags, NULL, NULL, NULL);
    1d00:	4781                	li	a5,0
    1d02:	4701                	li	a4,0
    1d04:	4681                	li	a3,0
    1d06:	2601                	sext.w	a2,a2
    1d08:	a2dd                	j	1eee <__clone>

0000000000001d0a <exit>:
    register long a7 __asm__("a7") = n;
    1d0a:	05d00893          	li	a7,93
    __asm_syscall("r"(a7), "0"(a0))
    1d0e:	00000073          	ecall
    //return syscall(SYS_clone, fn, stack, flags, NULL, NULL, NULL);
}
void exit(int code)
{
    syscall(SYS_exit, code);
}
    1d12:	8082                	ret

0000000000001d14 <waitpid>:
    register long a7 __asm__("a7") = n;
    1d14:	10400893          	li	a7,260
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1d18:	00000073          	ecall

int waitpid(int pid, int *code, int options)
{
    return syscall(SYS_wait4, pid, code, options);
}
    1d1c:	2501                	sext.w	a0,a0
    1d1e:	8082                	ret

0000000000001d20 <exec>:
    register long a7 __asm__("a7") = n;
    1d20:	0dd00893          	li	a7,221
    __asm_syscall("r"(a7), "0"(a0))
    1d24:	00000073          	ecall

int exec(char *name)
{
    return syscall(SYS_execve, name);
}
    1d28:	2501                	sext.w	a0,a0
    1d2a:	8082                	ret

0000000000001d2c <execve>:
    register long a7 __asm__("a7") = n;
    1d2c:	0dd00893          	li	a7,221
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1d30:	00000073          	ecall

int execve(const char *name, char *const argv[], char *const argp[])
{
    return syscall(SYS_execve, name, argv, argp);
}
    1d34:	2501                	sext.w	a0,a0
    1d36:	8082                	ret

0000000000001d38 <times>:
    register long a7 __asm__("a7") = n;
    1d38:	09900893          	li	a7,153
    __asm_syscall("r"(a7), "0"(a0))
    1d3c:	00000073          	ecall

int times(void *mytimes)
{
	return syscall(SYS_times, mytimes);
}
    1d40:	2501                	sext.w	a0,a0
    1d42:	8082                	ret

0000000000001d44 <get_time>:

int64 get_time()
{
    1d44:	1141                	addi	sp,sp,-16
    register long a7 __asm__("a7") = n;
    1d46:	0a900893          	li	a7,169
    register long a0 __asm__("a0") = a;
    1d4a:	850a                	mv	a0,sp
    register long a1 __asm__("a1") = b;
    1d4c:	4581                	li	a1,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1d4e:	00000073          	ecall
    TimeVal time;
    int err = sys_get_time(&time, 0);
    if (err == 0)
    1d52:	2501                	sext.w	a0,a0
    1d54:	ed09                	bnez	a0,1d6e <get_time+0x2a>
    {
        return ((time.sec & 0xffff) * 1000 + time.usec / 1000);
    1d56:	67a2                	ld	a5,8(sp)
    1d58:	3e800713          	li	a4,1000
    1d5c:	00015503          	lhu	a0,0(sp)
    1d60:	02e7d7b3          	divu	a5,a5,a4
    1d64:	02e50533          	mul	a0,a0,a4
    1d68:	953e                	add	a0,a0,a5
    }
    else
    {
        return -1;
    }
}
    1d6a:	0141                	addi	sp,sp,16
    1d6c:	8082                	ret
        return -1;
    1d6e:	557d                	li	a0,-1
    1d70:	bfed                	j	1d6a <get_time+0x26>

0000000000001d72 <sys_get_time>:
    register long a7 __asm__("a7") = n;
    1d72:	0a900893          	li	a7,169
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1d76:	00000073          	ecall

int sys_get_time(TimeVal *ts, int tz)
{
    return syscall(SYS_gettimeofday, ts, tz);
}
    1d7a:	2501                	sext.w	a0,a0
    1d7c:	8082                	ret

0000000000001d7e <time>:
    register long a7 __asm__("a7") = n;
    1d7e:	42600893          	li	a7,1062
    __asm_syscall("r"(a7), "0"(a0))
    1d82:	00000073          	ecall

int time(unsigned long *tloc)
{
    return syscall(SYS_time, tloc);
}
    1d86:	2501                	sext.w	a0,a0
    1d88:	8082                	ret

0000000000001d8a <sleep>:

int sleep(unsigned long long time)
{
    1d8a:	1141                	addi	sp,sp,-16
    TimeVal tv = {.sec = time, .usec = 0};
    1d8c:	e02a                	sd	a0,0(sp)
    register long a0 __asm__("a0") = a;
    1d8e:	850a                	mv	a0,sp
    1d90:	e402                	sd	zero,8(sp)
    register long a7 __asm__("a7") = n;
    1d92:	06500893          	li	a7,101
    register long a1 __asm__("a1") = b;
    1d96:	85aa                	mv	a1,a0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1d98:	00000073          	ecall
    if (syscall(SYS_nanosleep, &tv, &tv)) return tv.sec;
    1d9c:	e501                	bnez	a0,1da4 <sleep+0x1a>
    return 0;
    1d9e:	4501                	li	a0,0
}
    1da0:	0141                	addi	sp,sp,16
    1da2:	8082                	ret
    if (syscall(SYS_nanosleep, &tv, &tv)) return tv.sec;
    1da4:	4502                	lw	a0,0(sp)
}
    1da6:	0141                	addi	sp,sp,16
    1da8:	8082                	ret

0000000000001daa <set_priority>:
    register long a7 __asm__("a7") = n;
    1daa:	08c00893          	li	a7,140
    __asm_syscall("r"(a7), "0"(a0))
    1dae:	00000073          	ecall

int set_priority(int prio)
{
    return syscall(SYS_setpriority, prio);
}
    1db2:	2501                	sext.w	a0,a0
    1db4:	8082                	ret

0000000000001db6 <mmap>:
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4))
}

static inline long __syscall6(long n, long a, long b, long c, long d, long e, long f)
{
    register long a7 __asm__("a7") = n;
    1db6:	0de00893          	li	a7,222
    register long a1 __asm__("a1") = b;
    register long a2 __asm__("a2") = c;
    register long a3 __asm__("a3") = d;
    register long a4 __asm__("a4") = e;
    register long a5 __asm__("a5") = f;
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4), "r"(a5))
    1dba:	00000073          	ecall

void *mmap(void *start, size_t len, int prot, int flags, int fd, off_t off)
{
    return syscall(SYS_mmap, start, len, prot, flags, fd, off);
}
    1dbe:	8082                	ret

0000000000001dc0 <munmap>:
    register long a7 __asm__("a7") = n;
    1dc0:	0d700893          	li	a7,215
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1dc4:	00000073          	ecall

int munmap(void *start, size_t len)
{
    return syscall(SYS_munmap, start, len);
}
    1dc8:	2501                	sext.w	a0,a0
    1dca:	8082                	ret

0000000000001dcc <wait>:

int wait(int *code)
{
    1dcc:	85aa                	mv	a1,a0
    register long a7 __asm__("a7") = n;
    1dce:	10400893          	li	a7,260
    register long a0 __asm__("a0") = a;
    1dd2:	557d                	li	a0,-1
    register long a2 __asm__("a2") = c;
    1dd4:	4601                	li	a2,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1dd6:	00000073          	ecall
    return waitpid(-1, code, 0);
}
    1dda:	2501                	sext.w	a0,a0
    1ddc:	8082                	ret

0000000000001dde <spawn>:
    register long a7 __asm__("a7") = n;
    1dde:	19000893          	li	a7,400
    __asm_syscall("r"(a7), "0"(a0))
    1de2:	00000073          	ecall

int spawn(char *file)
{
    return syscall(SYS_spawn, file);
}
    1de6:	2501                	sext.w	a0,a0
    1de8:	8082                	ret

0000000000001dea <mailread>:
    register long a7 __asm__("a7") = n;
    1dea:	19100893          	li	a7,401
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1dee:	00000073          	ecall

int mailread(void *buf, int len)
{
    return syscall(SYS_mailread, buf, len);
}
    1df2:	2501                	sext.w	a0,a0
    1df4:	8082                	ret

0000000000001df6 <mailwrite>:
    register long a7 __asm__("a7") = n;
    1df6:	19200893          	li	a7,402
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1dfa:	00000073          	ecall

int mailwrite(int pid, void *buf, int len)
{
    return syscall(SYS_mailwrite, pid, buf, len);
}
    1dfe:	2501                	sext.w	a0,a0
    1e00:	8082                	ret

0000000000001e02 <fstat>:
    register long a7 __asm__("a7") = n;
    1e02:	05000893          	li	a7,80
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1e06:	00000073          	ecall

int fstat(int fd, struct kstat *st)
{
    return syscall(SYS_fstat, fd, st);
}
    1e0a:	2501                	sext.w	a0,a0
    1e0c:	8082                	ret

0000000000001e0e <sys_linkat>:
    register long a4 __asm__("a4") = e;
    1e0e:	1702                	slli	a4,a4,0x20
    register long a7 __asm__("a7") = n;
    1e10:	02500893          	li	a7,37
    register long a4 __asm__("a4") = e;
    1e14:	9301                	srli	a4,a4,0x20
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4))
    1e16:	00000073          	ecall

int sys_linkat(int olddirfd, char *oldpath, int newdirfd, char *newpath, unsigned int flags)
{
    return syscall(SYS_linkat, olddirfd, oldpath, newdirfd, newpath, flags);
}
    1e1a:	2501                	sext.w	a0,a0
    1e1c:	8082                	ret

0000000000001e1e <sys_unlinkat>:
    register long a2 __asm__("a2") = c;
    1e1e:	1602                	slli	a2,a2,0x20
    register long a7 __asm__("a7") = n;
    1e20:	02300893          	li	a7,35
    register long a2 __asm__("a2") = c;
    1e24:	9201                	srli	a2,a2,0x20
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1e26:	00000073          	ecall

int sys_unlinkat(int dirfd, char *path, unsigned int flags)
{
    return syscall(SYS_unlinkat, dirfd, path, flags);
}
    1e2a:	2501                	sext.w	a0,a0
    1e2c:	8082                	ret

0000000000001e2e <link>:

int link(char *old_path, char *new_path)
{
    1e2e:	87aa                	mv	a5,a0
    1e30:	86ae                	mv	a3,a1
    register long a7 __asm__("a7") = n;
    1e32:	02500893          	li	a7,37
    register long a0 __asm__("a0") = a;
    1e36:	f9c00513          	li	a0,-100
    register long a1 __asm__("a1") = b;
    1e3a:	85be                	mv	a1,a5
    register long a2 __asm__("a2") = c;
    1e3c:	f9c00613          	li	a2,-100
    register long a4 __asm__("a4") = e;
    1e40:	4701                	li	a4,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4))
    1e42:	00000073          	ecall
    return sys_linkat(AT_FDCWD, old_path, AT_FDCWD, new_path, 0);
}
    1e46:	2501                	sext.w	a0,a0
    1e48:	8082                	ret

0000000000001e4a <unlink>:

int unlink(char *path)
{
    1e4a:	85aa                	mv	a1,a0
    register long a7 __asm__("a7") = n;
    1e4c:	02300893          	li	a7,35
    register long a0 __asm__("a0") = a;
    1e50:	f9c00513          	li	a0,-100
    register long a2 __asm__("a2") = c;
    1e54:	4601                	li	a2,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1e56:	00000073          	ecall
    return sys_unlinkat(AT_FDCWD, path, 0);
}
    1e5a:	2501                	sext.w	a0,a0
    1e5c:	8082                	ret

0000000000001e5e <uname>:
    register long a7 __asm__("a7") = n;
    1e5e:	0a000893          	li	a7,160
    __asm_syscall("r"(a7), "0"(a0))
    1e62:	00000073          	ecall

int uname(void *buf)
{
    return syscall(SYS_uname, buf);
}
    1e66:	2501                	sext.w	a0,a0
    1e68:	8082                	ret

0000000000001e6a <brk>:
    register long a7 __asm__("a7") = n;
    1e6a:	0d600893          	li	a7,214
    __asm_syscall("r"(a7), "0"(a0))
    1e6e:	00000073          	ecall

int brk(void *addr)
{
    return syscall(SYS_brk, addr);
}
    1e72:	2501                	sext.w	a0,a0
    1e74:	8082                	ret

0000000000001e76 <getcwd>:
    register long a7 __asm__("a7") = n;
    1e76:	48c5                	li	a7,17
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1e78:	00000073          	ecall

char *getcwd(char *buf, size_t size){
    return syscall(SYS_getcwd, buf, size);
}
    1e7c:	8082                	ret

0000000000001e7e <chdir>:
    register long a7 __asm__("a7") = n;
    1e7e:	03100893          	li	a7,49
    __asm_syscall("r"(a7), "0"(a0))
    1e82:	00000073          	ecall

int chdir(const char *path){
    return syscall(SYS_chdir, path);
}
    1e86:	2501                	sext.w	a0,a0
    1e88:	8082                	ret

0000000000001e8a <mkdir>:

int mkdir(const char *path, mode_t mode){
    1e8a:	862e                	mv	a2,a1
    1e8c:	87aa                	mv	a5,a0
    register long a2 __asm__("a2") = c;
    1e8e:	1602                	slli	a2,a2,0x20
    register long a7 __asm__("a7") = n;
    1e90:	02200893          	li	a7,34
    register long a0 __asm__("a0") = a;
    1e94:	f9c00513          	li	a0,-100
    register long a1 __asm__("a1") = b;
    1e98:	85be                	mv	a1,a5
    register long a2 __asm__("a2") = c;
    1e9a:	9201                	srli	a2,a2,0x20
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1e9c:	00000073          	ecall
    return syscall(SYS_mkdirat, AT_FDCWD, path, mode);
}
    1ea0:	2501                	sext.w	a0,a0
    1ea2:	8082                	ret

0000000000001ea4 <getdents>:
    register long a7 __asm__("a7") = n;
    1ea4:	03d00893          	li	a7,61
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1ea8:	00000073          	ecall

int getdents(int fd, struct linux_dirent64 *dirp64, unsigned long len){
    //return syscall(SYS_getdents64, fd, dirp64, len);
    return syscall(SYS_getdents64, fd, dirp64, len);
}
    1eac:	2501                	sext.w	a0,a0
    1eae:	8082                	ret

0000000000001eb0 <pipe>:
    register long a7 __asm__("a7") = n;
    1eb0:	03b00893          	li	a7,59
    register long a1 __asm__("a1") = b;
    1eb4:	4581                	li	a1,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1eb6:	00000073          	ecall

int pipe(int fd[2]){
    return syscall(SYS_pipe2, fd, 0);
}
    1eba:	2501                	sext.w	a0,a0
    1ebc:	8082                	ret

0000000000001ebe <dup>:
    register long a7 __asm__("a7") = n;
    1ebe:	48dd                	li	a7,23
    __asm_syscall("r"(a7), "0"(a0))
    1ec0:	00000073          	ecall

int dup(int fd){
    return syscall(SYS_dup, fd);
}
    1ec4:	2501                	sext.w	a0,a0
    1ec6:	8082                	ret

0000000000001ec8 <dup2>:
    register long a7 __asm__("a7") = n;
    1ec8:	48e1                	li	a7,24
    register long a2 __asm__("a2") = c;
    1eca:	4601                	li	a2,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1ecc:	00000073          	ecall

int dup2(int old, int new){
    return syscall(SYS_dup3, old, new, 0);
}
    1ed0:	2501                	sext.w	a0,a0
    1ed2:	8082                	ret

0000000000001ed4 <mount>:
    register long a7 __asm__("a7") = n;
    1ed4:	02800893          	li	a7,40
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4))
    1ed8:	00000073          	ecall

int mount(const char *special, const char *dir, const char *fstype, unsigned long flags, const void *data)
{
        return syscall(SYS_mount, special, dir, fstype, flags, data);
}
    1edc:	2501                	sext.w	a0,a0
    1ede:	8082                	ret

0000000000001ee0 <umount>:
    register long a7 __asm__("a7") = n;
    1ee0:	02700893          	li	a7,39
    register long a1 __asm__("a1") = b;
    1ee4:	4581                	li	a1,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1ee6:	00000073          	ecall

int umount(const char *special)
{
        return syscall(SYS_umount2, special, 0);
}
    1eea:	2501                	sext.w	a0,a0
    1eec:	8082                	ret

0000000000001eee <__clone>:

.global __clone
.type  __clone, %function
__clone:
	# Save func and arg to stack
	addi a1, a1, -16
    1eee:	15c1                	addi	a1,a1,-16
	sd a0, 0(a1)
    1ef0:	e188                	sd	a0,0(a1)
	sd a3, 8(a1)
    1ef2:	e594                	sd	a3,8(a1)

	# Call SYS_clone
	mv a0, a2
    1ef4:	8532                	mv	a0,a2
	mv a2, a4
    1ef6:	863a                	mv	a2,a4
	mv a3, a5
    1ef8:	86be                	mv	a3,a5
	mv a4, a6
    1efa:	8742                	mv	a4,a6
	li a7, 220 # SYS_clone
    1efc:	0dc00893          	li	a7,220
	ecall
    1f00:	00000073          	ecall

	beqz a0, 1f
    1f04:	c111                	beqz	a0,1f08 <__clone+0x1a>
	# Parent
	ret
    1f06:	8082                	ret

	# Child
1:      ld a1, 0(sp)
    1f08:	6582                	ld	a1,0(sp)
	ld a0, 8(sp)
    1f0a:	6522                	ld	a0,8(sp)
	jalr a1
    1f0c:	9582                	jalr	a1

	# Exit
	li a7, 93 # SYS_exit
    1f0e:	05d00893          	li	a7,93
	ecall
    1f12:	00000073          	ecall
