
/home/ubuntu/rust/ultra-os/codes/riscv-syscalls-testing/user/build/riscv64/umount:     file format elf64-littleriscv


Disassembly of section .text:

0000000000001000 <_start>:
.section .text.entry
.globl _start
_start:
    mv a0, sp
    1000:	850a                	mv	a0,sp
    tail __start_main
    1002:	aa21                	j	111a <__start_main>

0000000000001004 <test_umount>:

static char mntpoint[64] = "./mnt";
static char device[64] = "/dev/vda2";
static const char *fs_type = "vfat";

void test_umount() {
    1004:	1101                	addi	sp,sp,-32
	TEST_START(__func__);
    1006:	00001517          	auipc	a0,0x1
    100a:	f4a50513          	addi	a0,a0,-182 # 1f50 <__clone+0x28>
void test_umount() {
    100e:	ec06                	sd	ra,24(sp)
    1010:	e822                	sd	s0,16(sp)
	TEST_START(__func__);
    1012:	37a000ef          	jal	ra,138c <puts>
    1016:	00001517          	auipc	a0,0x1
    101a:	08a50513          	addi	a0,a0,138 # 20a0 <__func__.0>
    101e:	36e000ef          	jal	ra,138c <puts>
    1022:	00001517          	auipc	a0,0x1
    1026:	f4650513          	addi	a0,a0,-186 # 1f68 <__clone+0x40>
    102a:	362000ef          	jal	ra,138c <puts>

	printf("Mounting dev:%s to %s\n", device, mntpoint);
    102e:	00001617          	auipc	a2,0x1
    1032:	ff260613          	addi	a2,a2,-14 # 2020 <mntpoint>
    1036:	00001597          	auipc	a1,0x1
    103a:	02a58593          	addi	a1,a1,42 # 2060 <device>
    103e:	00001517          	auipc	a0,0x1
    1042:	f3a50513          	addi	a0,a0,-198 # 1f78 <__clone+0x50>
    1046:	368000ef          	jal	ra,13ae <printf>
	int ret = mount(device, mntpoint, fs_type, 0, NULL);
    104a:	00001597          	auipc	a1,0x1
    104e:	fd658593          	addi	a1,a1,-42 # 2020 <mntpoint>
    1052:	4701                	li	a4,0
    1054:	4681                	li	a3,0
    1056:	00001617          	auipc	a2,0x1
    105a:	f3a60613          	addi	a2,a2,-198 # 1f90 <__clone+0x68>
    105e:	00001517          	auipc	a0,0x1
    1062:	00250513          	addi	a0,a0,2 # 2060 <device>
    1066:	6a9000ef          	jal	ra,1f0e <mount>
    106a:	842a                	mv	s0,a0
	printf("mount return: %d\n", ret);
    106c:	85aa                	mv	a1,a0
    106e:	00001517          	auipc	a0,0x1
    1072:	f2a50513          	addi	a0,a0,-214 # 1f98 <__clone+0x70>
    1076:	338000ef          	jal	ra,13ae <printf>

	if (ret == 0) {
    107a:	c40d                	beqz	s0,10a4 <test_umount+0xa0>
		ret = umount(mntpoint);
		assert(ret == 0);
		printf("umount success.\nreturn: %d\n", ret);
	}

	TEST_END(__func__);
    107c:	00001517          	auipc	a0,0x1
    1080:	f7450513          	addi	a0,a0,-140 # 1ff0 <__clone+0xc8>
    1084:	308000ef          	jal	ra,138c <puts>
    1088:	00001517          	auipc	a0,0x1
    108c:	01850513          	addi	a0,a0,24 # 20a0 <__func__.0>
    1090:	2fc000ef          	jal	ra,138c <puts>
}
    1094:	6442                	ld	s0,16(sp)
    1096:	60e2                	ld	ra,24(sp)
	TEST_END(__func__);
    1098:	00001517          	auipc	a0,0x1
    109c:	ed050513          	addi	a0,a0,-304 # 1f68 <__clone+0x40>
}
    10a0:	6105                	addi	sp,sp,32
	TEST_END(__func__);
    10a2:	a4ed                	j	138c <puts>
		ret = umount(mntpoint);
    10a4:	00001517          	auipc	a0,0x1
    10a8:	f7c50513          	addi	a0,a0,-132 # 2020 <mntpoint>
    10ac:	66f000ef          	jal	ra,1f1a <umount>
    10b0:	85aa                	mv	a1,a0
		assert(ret == 0);
    10b2:	e901                	bnez	a0,10c2 <test_umount+0xbe>
		printf("umount success.\nreturn: %d\n", ret);
    10b4:	00001517          	auipc	a0,0x1
    10b8:	f1c50513          	addi	a0,a0,-228 # 1fd0 <__clone+0xa8>
    10bc:	2f2000ef          	jal	ra,13ae <printf>
    10c0:	bf75                	j	107c <test_umount+0x78>
    10c2:	e42a                	sd	a0,8(sp)
		assert(ret == 0);
    10c4:	00001517          	auipc	a0,0x1
    10c8:	eec50513          	addi	a0,a0,-276 # 1fb0 <__clone+0x88>
    10cc:	566000ef          	jal	ra,1632 <panic>
    10d0:	65a2                	ld	a1,8(sp)
    10d2:	b7cd                	j	10b4 <test_umount+0xb0>

00000000000010d4 <main>:

int main(int argc,char *argv[]) {
    10d4:	1101                	addi	sp,sp,-32
    10d6:	ec06                	sd	ra,24(sp)
    10d8:	e822                	sd	s0,16(sp)
    10da:	e426                	sd	s1,8(sp)
	if(argc >= 2){
    10dc:	4785                	li	a5,1
    10de:	00a7ca63          	blt	a5,a0,10f2 <main+0x1e>

	if(argc >= 3){
		strcpy(mntpoint, argv[2]);
	}

	test_umount();
    10e2:	f23ff0ef          	jal	ra,1004 <test_umount>
	return 0;
}
    10e6:	60e2                	ld	ra,24(sp)
    10e8:	6442                	ld	s0,16(sp)
    10ea:	64a2                	ld	s1,8(sp)
    10ec:	4501                	li	a0,0
    10ee:	6105                	addi	sp,sp,32
    10f0:	8082                	ret
		strcpy(device, argv[1]);
    10f2:	84ae                	mv	s1,a1
    10f4:	658c                	ld	a1,8(a1)
    10f6:	842a                	mv	s0,a0
    10f8:	00001517          	auipc	a0,0x1
    10fc:	f6850513          	addi	a0,a0,-152 # 2060 <device>
    1100:	12b000ef          	jal	ra,1a2a <strcpy>
	if(argc >= 3){
    1104:	4789                	li	a5,2
    1106:	fcf40ee3          	beq	s0,a5,10e2 <main+0xe>
		strcpy(mntpoint, argv[2]);
    110a:	688c                	ld	a1,16(s1)
    110c:	00001517          	auipc	a0,0x1
    1110:	f1450513          	addi	a0,a0,-236 # 2020 <mntpoint>
    1114:	117000ef          	jal	ra,1a2a <strcpy>
    1118:	b7e9                	j	10e2 <main+0xe>

000000000000111a <__start_main>:
#include <unistd.h>

extern int main();

int __start_main(long *p)
{
    111a:	85aa                	mv	a1,a0
	int argc = p[0];
	char **argv = (void *)(p+1);

	exit(main(argc, argv));
    111c:	4108                	lw	a0,0(a0)
{
    111e:	1141                	addi	sp,sp,-16
	exit(main(argc, argv));
    1120:	05a1                	addi	a1,a1,8
{
    1122:	e406                	sd	ra,8(sp)
	exit(main(argc, argv));
    1124:	fb1ff0ef          	jal	ra,10d4 <main>
    1128:	41d000ef          	jal	ra,1d44 <exit>
	return 0;
}
    112c:	60a2                	ld	ra,8(sp)
    112e:	4501                	li	a0,0
    1130:	0141                	addi	sp,sp,16
    1132:	8082                	ret

0000000000001134 <printint.constprop.0>:
    write(f, s, l);
}

static char digits[] = "0123456789abcdef";

static void printint(int xx, int base, int sign)
    1134:	7179                	addi	sp,sp,-48
    1136:	f406                	sd	ra,40(sp)
{
    char buf[16 + 1];
    int i;
    uint x;

    if (sign && (sign = xx < 0))
    1138:	12054b63          	bltz	a0,126e <printint.constprop.0+0x13a>

    buf[16] = 0;
    i = 15;
    do
    {
        buf[i--] = digits[x % base];
    113c:	02b577bb          	remuw	a5,a0,a1
    1140:	00001617          	auipc	a2,0x1
    1144:	f7060613          	addi	a2,a2,-144 # 20b0 <digits>
    buf[16] = 0;
    1148:	00010c23          	sb	zero,24(sp)
        buf[i--] = digits[x % base];
    114c:	0005871b          	sext.w	a4,a1
    1150:	1782                	slli	a5,a5,0x20
    1152:	9381                	srli	a5,a5,0x20
    1154:	97b2                	add	a5,a5,a2
    1156:	0007c783          	lbu	a5,0(a5)
    } while ((x /= base) != 0);
    115a:	02b5583b          	divuw	a6,a0,a1
        buf[i--] = digits[x % base];
    115e:	00f10ba3          	sb	a5,23(sp)
    } while ((x /= base) != 0);
    1162:	1cb56363          	bltu	a0,a1,1328 <printint.constprop.0+0x1f4>
        buf[i--] = digits[x % base];
    1166:	45b9                	li	a1,14
    1168:	02e877bb          	remuw	a5,a6,a4
    116c:	1782                	slli	a5,a5,0x20
    116e:	9381                	srli	a5,a5,0x20
    1170:	97b2                	add	a5,a5,a2
    1172:	0007c783          	lbu	a5,0(a5)
    } while ((x /= base) != 0);
    1176:	02e856bb          	divuw	a3,a6,a4
        buf[i--] = digits[x % base];
    117a:	00f10b23          	sb	a5,22(sp)
    } while ((x /= base) != 0);
    117e:	0ce86e63          	bltu	a6,a4,125a <printint.constprop.0+0x126>
        buf[i--] = digits[x % base];
    1182:	02e6f5bb          	remuw	a1,a3,a4
    } while ((x /= base) != 0);
    1186:	02e6d7bb          	divuw	a5,a3,a4
        buf[i--] = digits[x % base];
    118a:	1582                	slli	a1,a1,0x20
    118c:	9181                	srli	a1,a1,0x20
    118e:	95b2                	add	a1,a1,a2
    1190:	0005c583          	lbu	a1,0(a1)
    1194:	00b10aa3          	sb	a1,21(sp)
    } while ((x /= base) != 0);
    1198:	0007859b          	sext.w	a1,a5
    119c:	12e6ec63          	bltu	a3,a4,12d4 <printint.constprop.0+0x1a0>
        buf[i--] = digits[x % base];
    11a0:	02e7f6bb          	remuw	a3,a5,a4
    11a4:	1682                	slli	a3,a3,0x20
    11a6:	9281                	srli	a3,a3,0x20
    11a8:	96b2                	add	a3,a3,a2
    11aa:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    11ae:	02e7d83b          	divuw	a6,a5,a4
        buf[i--] = digits[x % base];
    11b2:	00d10a23          	sb	a3,20(sp)
    } while ((x /= base) != 0);
    11b6:	12e5e863          	bltu	a1,a4,12e6 <printint.constprop.0+0x1b2>
        buf[i--] = digits[x % base];
    11ba:	02e876bb          	remuw	a3,a6,a4
    11be:	1682                	slli	a3,a3,0x20
    11c0:	9281                	srli	a3,a3,0x20
    11c2:	96b2                	add	a3,a3,a2
    11c4:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    11c8:	02e855bb          	divuw	a1,a6,a4
        buf[i--] = digits[x % base];
    11cc:	00d109a3          	sb	a3,19(sp)
    } while ((x /= base) != 0);
    11d0:	12e86463          	bltu	a6,a4,12f8 <printint.constprop.0+0x1c4>
        buf[i--] = digits[x % base];
    11d4:	02e5f6bb          	remuw	a3,a1,a4
    11d8:	1682                	slli	a3,a3,0x20
    11da:	9281                	srli	a3,a3,0x20
    11dc:	96b2                	add	a3,a3,a2
    11de:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    11e2:	02e5d83b          	divuw	a6,a1,a4
        buf[i--] = digits[x % base];
    11e6:	00d10923          	sb	a3,18(sp)
    } while ((x /= base) != 0);
    11ea:	0ce5ec63          	bltu	a1,a4,12c2 <printint.constprop.0+0x18e>
        buf[i--] = digits[x % base];
    11ee:	02e876bb          	remuw	a3,a6,a4
    11f2:	1682                	slli	a3,a3,0x20
    11f4:	9281                	srli	a3,a3,0x20
    11f6:	96b2                	add	a3,a3,a2
    11f8:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    11fc:	02e855bb          	divuw	a1,a6,a4
        buf[i--] = digits[x % base];
    1200:	00d108a3          	sb	a3,17(sp)
    } while ((x /= base) != 0);
    1204:	10e86963          	bltu	a6,a4,1316 <printint.constprop.0+0x1e2>
        buf[i--] = digits[x % base];
    1208:	02e5f6bb          	remuw	a3,a1,a4
    120c:	1682                	slli	a3,a3,0x20
    120e:	9281                	srli	a3,a3,0x20
    1210:	96b2                	add	a3,a3,a2
    1212:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    1216:	02e5d83b          	divuw	a6,a1,a4
        buf[i--] = digits[x % base];
    121a:	00d10823          	sb	a3,16(sp)
    } while ((x /= base) != 0);
    121e:	10e5e763          	bltu	a1,a4,132c <printint.constprop.0+0x1f8>
        buf[i--] = digits[x % base];
    1222:	02e876bb          	remuw	a3,a6,a4
    1226:	1682                	slli	a3,a3,0x20
    1228:	9281                	srli	a3,a3,0x20
    122a:	96b2                	add	a3,a3,a2
    122c:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    1230:	02e857bb          	divuw	a5,a6,a4
        buf[i--] = digits[x % base];
    1234:	00d107a3          	sb	a3,15(sp)
    } while ((x /= base) != 0);
    1238:	10e86363          	bltu	a6,a4,133e <printint.constprop.0+0x20a>
        buf[i--] = digits[x % base];
    123c:	1782                	slli	a5,a5,0x20
    123e:	9381                	srli	a5,a5,0x20
    1240:	97b2                	add	a5,a5,a2
    1242:	0007c783          	lbu	a5,0(a5)
    1246:	4599                	li	a1,6
    1248:	00f10723          	sb	a5,14(sp)

    if (sign)
    124c:	00055763          	bgez	a0,125a <printint.constprop.0+0x126>
        buf[i--] = '-';
    1250:	02d00793          	li	a5,45
    1254:	00f106a3          	sb	a5,13(sp)
        buf[i--] = digits[x % base];
    1258:	4595                	li	a1,5
    write(f, s, l);
    125a:	003c                	addi	a5,sp,8
    125c:	4641                	li	a2,16
    125e:	9e0d                	subw	a2,a2,a1
    1260:	4505                	li	a0,1
    1262:	95be                	add	a1,a1,a5
    1264:	291000ef          	jal	ra,1cf4 <write>
    i++;
    if (i < 0)
        puts("printint error");
    out(stdout, buf + i, 16 - i);
}
    1268:	70a2                	ld	ra,40(sp)
    126a:	6145                	addi	sp,sp,48
    126c:	8082                	ret
        x = -xx;
    126e:	40a0083b          	negw	a6,a0
        buf[i--] = digits[x % base];
    1272:	02b877bb          	remuw	a5,a6,a1
    1276:	00001617          	auipc	a2,0x1
    127a:	e3a60613          	addi	a2,a2,-454 # 20b0 <digits>
    buf[16] = 0;
    127e:	00010c23          	sb	zero,24(sp)
        buf[i--] = digits[x % base];
    1282:	0005871b          	sext.w	a4,a1
    1286:	1782                	slli	a5,a5,0x20
    1288:	9381                	srli	a5,a5,0x20
    128a:	97b2                	add	a5,a5,a2
    128c:	0007c783          	lbu	a5,0(a5)
    } while ((x /= base) != 0);
    1290:	02b858bb          	divuw	a7,a6,a1
        buf[i--] = digits[x % base];
    1294:	00f10ba3          	sb	a5,23(sp)
    } while ((x /= base) != 0);
    1298:	06b86963          	bltu	a6,a1,130a <printint.constprop.0+0x1d6>
        buf[i--] = digits[x % base];
    129c:	02e8f7bb          	remuw	a5,a7,a4
    12a0:	1782                	slli	a5,a5,0x20
    12a2:	9381                	srli	a5,a5,0x20
    12a4:	97b2                	add	a5,a5,a2
    12a6:	0007c783          	lbu	a5,0(a5)
    } while ((x /= base) != 0);
    12aa:	02e8d6bb          	divuw	a3,a7,a4
        buf[i--] = digits[x % base];
    12ae:	00f10b23          	sb	a5,22(sp)
    } while ((x /= base) != 0);
    12b2:	ece8f8e3          	bgeu	a7,a4,1182 <printint.constprop.0+0x4e>
        buf[i--] = '-';
    12b6:	02d00793          	li	a5,45
    12ba:	00f10aa3          	sb	a5,21(sp)
        buf[i--] = digits[x % base];
    12be:	45b5                	li	a1,13
    12c0:	bf69                	j	125a <printint.constprop.0+0x126>
    12c2:	45a9                	li	a1,10
    if (sign)
    12c4:	f8055be3          	bgez	a0,125a <printint.constprop.0+0x126>
        buf[i--] = '-';
    12c8:	02d00793          	li	a5,45
    12cc:	00f108a3          	sb	a5,17(sp)
        buf[i--] = digits[x % base];
    12d0:	45a5                	li	a1,9
    12d2:	b761                	j	125a <printint.constprop.0+0x126>
    12d4:	45b5                	li	a1,13
    if (sign)
    12d6:	f80552e3          	bgez	a0,125a <printint.constprop.0+0x126>
        buf[i--] = '-';
    12da:	02d00793          	li	a5,45
    12de:	00f10a23          	sb	a5,20(sp)
        buf[i--] = digits[x % base];
    12e2:	45b1                	li	a1,12
    12e4:	bf9d                	j	125a <printint.constprop.0+0x126>
    12e6:	45b1                	li	a1,12
    if (sign)
    12e8:	f60559e3          	bgez	a0,125a <printint.constprop.0+0x126>
        buf[i--] = '-';
    12ec:	02d00793          	li	a5,45
    12f0:	00f109a3          	sb	a5,19(sp)
        buf[i--] = digits[x % base];
    12f4:	45ad                	li	a1,11
    12f6:	b795                	j	125a <printint.constprop.0+0x126>
    12f8:	45ad                	li	a1,11
    if (sign)
    12fa:	f60550e3          	bgez	a0,125a <printint.constprop.0+0x126>
        buf[i--] = '-';
    12fe:	02d00793          	li	a5,45
    1302:	00f10923          	sb	a5,18(sp)
        buf[i--] = digits[x % base];
    1306:	45a9                	li	a1,10
    1308:	bf89                	j	125a <printint.constprop.0+0x126>
        buf[i--] = '-';
    130a:	02d00793          	li	a5,45
    130e:	00f10b23          	sb	a5,22(sp)
        buf[i--] = digits[x % base];
    1312:	45b9                	li	a1,14
    1314:	b799                	j	125a <printint.constprop.0+0x126>
    1316:	45a5                	li	a1,9
    if (sign)
    1318:	f40551e3          	bgez	a0,125a <printint.constprop.0+0x126>
        buf[i--] = '-';
    131c:	02d00793          	li	a5,45
    1320:	00f10823          	sb	a5,16(sp)
        buf[i--] = digits[x % base];
    1324:	45a1                	li	a1,8
    1326:	bf15                	j	125a <printint.constprop.0+0x126>
    i = 15;
    1328:	45bd                	li	a1,15
    132a:	bf05                	j	125a <printint.constprop.0+0x126>
        buf[i--] = digits[x % base];
    132c:	45a1                	li	a1,8
    if (sign)
    132e:	f20556e3          	bgez	a0,125a <printint.constprop.0+0x126>
        buf[i--] = '-';
    1332:	02d00793          	li	a5,45
    1336:	00f107a3          	sb	a5,15(sp)
        buf[i--] = digits[x % base];
    133a:	459d                	li	a1,7
    133c:	bf39                	j	125a <printint.constprop.0+0x126>
    133e:	459d                	li	a1,7
    if (sign)
    1340:	f0055de3          	bgez	a0,125a <printint.constprop.0+0x126>
        buf[i--] = '-';
    1344:	02d00793          	li	a5,45
    1348:	00f10723          	sb	a5,14(sp)
        buf[i--] = digits[x % base];
    134c:	4599                	li	a1,6
    134e:	b731                	j	125a <printint.constprop.0+0x126>

0000000000001350 <getchar>:
{
    1350:	1101                	addi	sp,sp,-32
    read(stdin, &byte, 1);
    1352:	00f10593          	addi	a1,sp,15
    1356:	4605                	li	a2,1
    1358:	4501                	li	a0,0
{
    135a:	ec06                	sd	ra,24(sp)
    char byte = 0;
    135c:	000107a3          	sb	zero,15(sp)
    read(stdin, &byte, 1);
    1360:	18b000ef          	jal	ra,1cea <read>
}
    1364:	60e2                	ld	ra,24(sp)
    1366:	00f14503          	lbu	a0,15(sp)
    136a:	6105                	addi	sp,sp,32
    136c:	8082                	ret

000000000000136e <putchar>:
{
    136e:	1101                	addi	sp,sp,-32
    1370:	87aa                	mv	a5,a0
    return write(stdout, &byte, 1);
    1372:	00f10593          	addi	a1,sp,15
    1376:	4605                	li	a2,1
    1378:	4505                	li	a0,1
{
    137a:	ec06                	sd	ra,24(sp)
    char byte = c;
    137c:	00f107a3          	sb	a5,15(sp)
    return write(stdout, &byte, 1);
    1380:	175000ef          	jal	ra,1cf4 <write>
}
    1384:	60e2                	ld	ra,24(sp)
    1386:	2501                	sext.w	a0,a0
    1388:	6105                	addi	sp,sp,32
    138a:	8082                	ret

000000000000138c <puts>:
{
    138c:	1141                	addi	sp,sp,-16
    138e:	e406                	sd	ra,8(sp)
    1390:	e022                	sd	s0,0(sp)
    1392:	842a                	mv	s0,a0
    r = -(write(stdout, s, strlen(s)) < 0);
    1394:	57c000ef          	jal	ra,1910 <strlen>
    1398:	862a                	mv	a2,a0
    139a:	85a2                	mv	a1,s0
    139c:	4505                	li	a0,1
    139e:	157000ef          	jal	ra,1cf4 <write>
}
    13a2:	60a2                	ld	ra,8(sp)
    13a4:	6402                	ld	s0,0(sp)
    r = -(write(stdout, s, strlen(s)) < 0);
    13a6:	957d                	srai	a0,a0,0x3f
    return r;
    13a8:	2501                	sext.w	a0,a0
}
    13aa:	0141                	addi	sp,sp,16
    13ac:	8082                	ret

00000000000013ae <printf>:
    out(stdout, buf, i);
}

// Print to the console. only understands %d, %x, %p, %s.
void printf(const char *fmt, ...)
{
    13ae:	7171                	addi	sp,sp,-176
    13b0:	fc56                	sd	s5,56(sp)
    13b2:	ed3e                	sd	a5,152(sp)
    buf[i++] = '0';
    13b4:	7ae1                	lui	s5,0xffff8
    va_list ap;
    int cnt = 0, l = 0;
    char *a, *z, *s = (char *)fmt, str;
    int f = stdout;

    va_start(ap, fmt);
    13b6:	18bc                	addi	a5,sp,120
{
    13b8:	e8ca                	sd	s2,80(sp)
    13ba:	e4ce                	sd	s3,72(sp)
    13bc:	e0d2                	sd	s4,64(sp)
    13be:	f85a                	sd	s6,48(sp)
    13c0:	f486                	sd	ra,104(sp)
    13c2:	f0a2                	sd	s0,96(sp)
    13c4:	eca6                	sd	s1,88(sp)
    13c6:	fcae                	sd	a1,120(sp)
    13c8:	e132                	sd	a2,128(sp)
    13ca:	e536                	sd	a3,136(sp)
    13cc:	e93a                	sd	a4,144(sp)
    13ce:	f142                	sd	a6,160(sp)
    13d0:	f546                	sd	a7,168(sp)
    va_start(ap, fmt);
    13d2:	e03e                	sd	a5,0(sp)
    for (;;)
    {
        if (!*s)
            break;
        for (a = s; *s && *s != '%'; s++)
    13d4:	02500913          	li	s2,37
        out(f, a, l);
        if (l)
            continue;
        if (s[1] == 0)
            break;
        switch (s[1])
    13d8:	07300a13          	li	s4,115
        case 'p':
            printptr(va_arg(ap, uint64));
            break;
        case 's':
            if ((a = va_arg(ap, char *)) == 0)
                a = "(null)";
    13dc:	00001b17          	auipc	s6,0x1
    13e0:	c24b0b13          	addi	s6,s6,-988 # 2000 <__clone+0xd8>
    buf[i++] = '0';
    13e4:	830aca93          	xori	s5,s5,-2000
        buf[i++] = digits[x >> (sizeof(uint64) * 8 - 4)];
    13e8:	00001997          	auipc	s3,0x1
    13ec:	cc898993          	addi	s3,s3,-824 # 20b0 <digits>
        if (!*s)
    13f0:	00054783          	lbu	a5,0(a0)
    13f4:	16078a63          	beqz	a5,1568 <printf+0x1ba>
    13f8:	862a                	mv	a2,a0
        for (a = s; *s && *s != '%'; s++)
    13fa:	19278163          	beq	a5,s2,157c <printf+0x1ce>
    13fe:	00164783          	lbu	a5,1(a2)
    1402:	0605                	addi	a2,a2,1
    1404:	fbfd                	bnez	a5,13fa <printf+0x4c>
    1406:	84b2                	mv	s1,a2
        l = z - a;
    1408:	40a6043b          	subw	s0,a2,a0
    write(f, s, l);
    140c:	85aa                	mv	a1,a0
    140e:	8622                	mv	a2,s0
    1410:	4505                	li	a0,1
    1412:	0e3000ef          	jal	ra,1cf4 <write>
        if (l)
    1416:	18041c63          	bnez	s0,15ae <printf+0x200>
        if (s[1] == 0)
    141a:	0014c783          	lbu	a5,1(s1)
    141e:	14078563          	beqz	a5,1568 <printf+0x1ba>
        switch (s[1])
    1422:	1d478063          	beq	a5,s4,15e2 <printf+0x234>
    1426:	18fa6663          	bltu	s4,a5,15b2 <printf+0x204>
    142a:	06400713          	li	a4,100
    142e:	1ae78063          	beq	a5,a4,15ce <printf+0x220>
    1432:	07000713          	li	a4,112
    1436:	1ce79963          	bne	a5,a4,1608 <printf+0x25a>
            printptr(va_arg(ap, uint64));
    143a:	6702                	ld	a4,0(sp)
    buf[i++] = '0';
    143c:	01511423          	sh	s5,8(sp)
    write(f, s, l);
    1440:	4649                	li	a2,18
            printptr(va_arg(ap, uint64));
    1442:	631c                	ld	a5,0(a4)
    1444:	0721                	addi	a4,a4,8
    1446:	e03a                	sd	a4,0(sp)
    for (j = 0; j < (sizeof(uint64) * 2); j++, x <<= 4)
    1448:	00479293          	slli	t0,a5,0x4
    144c:	00879f93          	slli	t6,a5,0x8
    1450:	00c79f13          	slli	t5,a5,0xc
    1454:	01079e93          	slli	t4,a5,0x10
    1458:	01479e13          	slli	t3,a5,0x14
    145c:	01879313          	slli	t1,a5,0x18
    1460:	01c79893          	slli	a7,a5,0x1c
    1464:	02479813          	slli	a6,a5,0x24
    1468:	02879513          	slli	a0,a5,0x28
    146c:	02c79593          	slli	a1,a5,0x2c
    1470:	03079693          	slli	a3,a5,0x30
    1474:	03479713          	slli	a4,a5,0x34
        buf[i++] = digits[x >> (sizeof(uint64) * 8 - 4)];
    1478:	03c7d413          	srli	s0,a5,0x3c
    147c:	01c7d39b          	srliw	t2,a5,0x1c
    1480:	03c2d293          	srli	t0,t0,0x3c
    1484:	03cfdf93          	srli	t6,t6,0x3c
    1488:	03cf5f13          	srli	t5,t5,0x3c
    148c:	03cede93          	srli	t4,t4,0x3c
    1490:	03ce5e13          	srli	t3,t3,0x3c
    1494:	03c35313          	srli	t1,t1,0x3c
    1498:	03c8d893          	srli	a7,a7,0x3c
    149c:	03c85813          	srli	a6,a6,0x3c
    14a0:	9171                	srli	a0,a0,0x3c
    14a2:	91f1                	srli	a1,a1,0x3c
    14a4:	92f1                	srli	a3,a3,0x3c
    14a6:	9371                	srli	a4,a4,0x3c
    14a8:	96ce                	add	a3,a3,s3
    14aa:	974e                	add	a4,a4,s3
    14ac:	944e                	add	s0,s0,s3
    14ae:	92ce                	add	t0,t0,s3
    14b0:	9fce                	add	t6,t6,s3
    14b2:	9f4e                	add	t5,t5,s3
    14b4:	9ece                	add	t4,t4,s3
    14b6:	9e4e                	add	t3,t3,s3
    14b8:	934e                	add	t1,t1,s3
    14ba:	98ce                	add	a7,a7,s3
    14bc:	93ce                	add	t2,t2,s3
    14be:	984e                	add	a6,a6,s3
    14c0:	954e                	add	a0,a0,s3
    14c2:	95ce                	add	a1,a1,s3
    14c4:	0006c083          	lbu	ra,0(a3)
    14c8:	0002c283          	lbu	t0,0(t0)
    14cc:	00074683          	lbu	a3,0(a4)
    14d0:	000fcf83          	lbu	t6,0(t6)
    14d4:	000f4f03          	lbu	t5,0(t5)
    14d8:	000ece83          	lbu	t4,0(t4)
    14dc:	000e4e03          	lbu	t3,0(t3)
    14e0:	00034303          	lbu	t1,0(t1)
    14e4:	0008c883          	lbu	a7,0(a7)
    14e8:	0003c383          	lbu	t2,0(t2)
    14ec:	00084803          	lbu	a6,0(a6)
    14f0:	00054503          	lbu	a0,0(a0)
    14f4:	0005c583          	lbu	a1,0(a1)
    14f8:	00044403          	lbu	s0,0(s0)
    for (j = 0; j < (sizeof(uint64) * 2); j++, x <<= 4)
    14fc:	03879713          	slli	a4,a5,0x38
        buf[i++] = digits[x >> (sizeof(uint64) * 8 - 4)];
    1500:	9371                	srli	a4,a4,0x3c
    1502:	8bbd                	andi	a5,a5,15
    1504:	974e                	add	a4,a4,s3
    1506:	97ce                	add	a5,a5,s3
    1508:	005105a3          	sb	t0,11(sp)
    150c:	01f10623          	sb	t6,12(sp)
    1510:	01e106a3          	sb	t5,13(sp)
    1514:	01d10723          	sb	t4,14(sp)
    1518:	01c107a3          	sb	t3,15(sp)
    151c:	00610823          	sb	t1,16(sp)
    1520:	011108a3          	sb	a7,17(sp)
    1524:	00710923          	sb	t2,18(sp)
    1528:	010109a3          	sb	a6,19(sp)
    152c:	00a10a23          	sb	a0,20(sp)
    1530:	00b10aa3          	sb	a1,21(sp)
    1534:	00110b23          	sb	ra,22(sp)
    1538:	00d10ba3          	sb	a3,23(sp)
    153c:	00810523          	sb	s0,10(sp)
    1540:	00074703          	lbu	a4,0(a4)
    1544:	0007c783          	lbu	a5,0(a5)
    write(f, s, l);
    1548:	002c                	addi	a1,sp,8
    154a:	4505                	li	a0,1
        buf[i++] = digits[x >> (sizeof(uint64) * 8 - 4)];
    154c:	00e10c23          	sb	a4,24(sp)
    1550:	00f10ca3          	sb	a5,25(sp)
    buf[i] = 0;
    1554:	00010d23          	sb	zero,26(sp)
    write(f, s, l);
    1558:	79c000ef          	jal	ra,1cf4 <write>
            // Print unknown % sequence to draw attention.
            putchar('%');
            putchar(s[1]);
            break;
        }
        s += 2;
    155c:	00248513          	addi	a0,s1,2
        if (!*s)
    1560:	00054783          	lbu	a5,0(a0)
    1564:	e8079ae3          	bnez	a5,13f8 <printf+0x4a>
    }
    va_end(ap);
}
    1568:	70a6                	ld	ra,104(sp)
    156a:	7406                	ld	s0,96(sp)
    156c:	64e6                	ld	s1,88(sp)
    156e:	6946                	ld	s2,80(sp)
    1570:	69a6                	ld	s3,72(sp)
    1572:	6a06                	ld	s4,64(sp)
    1574:	7ae2                	ld	s5,56(sp)
    1576:	7b42                	ld	s6,48(sp)
    1578:	614d                	addi	sp,sp,176
    157a:	8082                	ret
        for (z = s; s[0] == '%' && s[1] == '%'; z++, s += 2)
    157c:	00064783          	lbu	a5,0(a2)
    1580:	84b2                	mv	s1,a2
    1582:	01278963          	beq	a5,s2,1594 <printf+0x1e6>
    1586:	b549                	j	1408 <printf+0x5a>
    1588:	0024c783          	lbu	a5,2(s1)
    158c:	0605                	addi	a2,a2,1
    158e:	0489                	addi	s1,s1,2
    1590:	e7279ce3          	bne	a5,s2,1408 <printf+0x5a>
    1594:	0014c783          	lbu	a5,1(s1)
    1598:	ff2788e3          	beq	a5,s2,1588 <printf+0x1da>
        l = z - a;
    159c:	40a6043b          	subw	s0,a2,a0
    write(f, s, l);
    15a0:	85aa                	mv	a1,a0
    15a2:	8622                	mv	a2,s0
    15a4:	4505                	li	a0,1
    15a6:	74e000ef          	jal	ra,1cf4 <write>
        if (l)
    15aa:	e60408e3          	beqz	s0,141a <printf+0x6c>
    15ae:	8526                	mv	a0,s1
    15b0:	b581                	j	13f0 <printf+0x42>
        switch (s[1])
    15b2:	07800713          	li	a4,120
    15b6:	04e79963          	bne	a5,a4,1608 <printf+0x25a>
            printint(va_arg(ap, int), 16, 1);
    15ba:	6782                	ld	a5,0(sp)
    15bc:	45c1                	li	a1,16
    15be:	4388                	lw	a0,0(a5)
    15c0:	07a1                	addi	a5,a5,8
    15c2:	e03e                	sd	a5,0(sp)
    15c4:	b71ff0ef          	jal	ra,1134 <printint.constprop.0>
        s += 2;
    15c8:	00248513          	addi	a0,s1,2
    15cc:	bf51                	j	1560 <printf+0x1b2>
            printint(va_arg(ap, int), 10, 1);
    15ce:	6782                	ld	a5,0(sp)
    15d0:	45a9                	li	a1,10
    15d2:	4388                	lw	a0,0(a5)
    15d4:	07a1                	addi	a5,a5,8
    15d6:	e03e                	sd	a5,0(sp)
    15d8:	b5dff0ef          	jal	ra,1134 <printint.constprop.0>
        s += 2;
    15dc:	00248513          	addi	a0,s1,2
    15e0:	b741                	j	1560 <printf+0x1b2>
            if ((a = va_arg(ap, char *)) == 0)
    15e2:	6782                	ld	a5,0(sp)
    15e4:	6380                	ld	s0,0(a5)
    15e6:	07a1                	addi	a5,a5,8
    15e8:	e03e                	sd	a5,0(sp)
    15ea:	c031                	beqz	s0,162e <printf+0x280>
            l = strnlen(a, 200);
    15ec:	0c800593          	li	a1,200
    15f0:	8522                	mv	a0,s0
    15f2:	40a000ef          	jal	ra,19fc <strnlen>
    write(f, s, l);
    15f6:	0005061b          	sext.w	a2,a0
    15fa:	85a2                	mv	a1,s0
    15fc:	4505                	li	a0,1
    15fe:	6f6000ef          	jal	ra,1cf4 <write>
        s += 2;
    1602:	00248513          	addi	a0,s1,2
    1606:	bfa9                	j	1560 <printf+0x1b2>
    return write(stdout, &byte, 1);
    1608:	4605                	li	a2,1
    160a:	002c                	addi	a1,sp,8
    160c:	4505                	li	a0,1
    char byte = c;
    160e:	01210423          	sb	s2,8(sp)
    return write(stdout, &byte, 1);
    1612:	6e2000ef          	jal	ra,1cf4 <write>
    char byte = c;
    1616:	0014c783          	lbu	a5,1(s1)
    return write(stdout, &byte, 1);
    161a:	4605                	li	a2,1
    161c:	002c                	addi	a1,sp,8
    161e:	4505                	li	a0,1
    char byte = c;
    1620:	00f10423          	sb	a5,8(sp)
    return write(stdout, &byte, 1);
    1624:	6d0000ef          	jal	ra,1cf4 <write>
        s += 2;
    1628:	00248513          	addi	a0,s1,2
    162c:	bf15                	j	1560 <printf+0x1b2>
                a = "(null)";
    162e:	845a                	mv	s0,s6
    1630:	bf75                	j	15ec <printf+0x23e>

0000000000001632 <panic>:
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

void panic(char *m)
{
    1632:	1141                	addi	sp,sp,-16
    1634:	e406                	sd	ra,8(sp)
    puts(m);
    1636:	d57ff0ef          	jal	ra,138c <puts>
    exit(-100);
}
    163a:	60a2                	ld	ra,8(sp)
    exit(-100);
    163c:	f9c00513          	li	a0,-100
}
    1640:	0141                	addi	sp,sp,16
    exit(-100);
    1642:	a709                	j	1d44 <exit>

0000000000001644 <isspace>:
#define HIGHS (ONES * (UCHAR_MAX / 2 + 1))
#define HASZERO(x) (((x)-ONES) & ~(x)&HIGHS)

int isspace(int c)
{
    return c == ' ' || (unsigned)c - '\t' < 5;
    1644:	02000793          	li	a5,32
    1648:	00f50663          	beq	a0,a5,1654 <isspace+0x10>
    164c:	355d                	addiw	a0,a0,-9
    164e:	00553513          	sltiu	a0,a0,5
    1652:	8082                	ret
    1654:	4505                	li	a0,1
}
    1656:	8082                	ret

0000000000001658 <isdigit>:

int isdigit(int c)
{
    return (unsigned)c - '0' < 10;
    1658:	fd05051b          	addiw	a0,a0,-48
}
    165c:	00a53513          	sltiu	a0,a0,10
    1660:	8082                	ret

0000000000001662 <atoi>:
    return c == ' ' || (unsigned)c - '\t' < 5;
    1662:	02000613          	li	a2,32
    1666:	4591                	li	a1,4

int atoi(const char *s)
{
    int n = 0, neg = 0;
    while (isspace(*s))
    1668:	00054703          	lbu	a4,0(a0)
    return c == ' ' || (unsigned)c - '\t' < 5;
    166c:	ff77069b          	addiw	a3,a4,-9
    1670:	04c70d63          	beq	a4,a2,16ca <atoi+0x68>
    1674:	0007079b          	sext.w	a5,a4
    1678:	04d5f963          	bgeu	a1,a3,16ca <atoi+0x68>
        s++;
    switch (*s)
    167c:	02b00693          	li	a3,43
    1680:	04d70a63          	beq	a4,a3,16d4 <atoi+0x72>
    1684:	02d00693          	li	a3,45
    1688:	06d70463          	beq	a4,a3,16f0 <atoi+0x8e>
        neg = 1;
    case '+':
        s++;
    }
    /* Compute n as a negative number to avoid overflow on INT_MIN */
    while (isdigit(*s))
    168c:	fd07859b          	addiw	a1,a5,-48
    1690:	4625                	li	a2,9
    1692:	873e                	mv	a4,a5
    1694:	86aa                	mv	a3,a0
    int n = 0, neg = 0;
    1696:	4e01                	li	t3,0
    while (isdigit(*s))
    1698:	04b66a63          	bltu	a2,a1,16ec <atoi+0x8a>
    int n = 0, neg = 0;
    169c:	4501                	li	a0,0
    while (isdigit(*s))
    169e:	4825                	li	a6,9
    16a0:	0016c603          	lbu	a2,1(a3)
        n = 10 * n - (*s++ - '0');
    16a4:	0025179b          	slliw	a5,a0,0x2
    16a8:	9d3d                	addw	a0,a0,a5
    16aa:	fd07031b          	addiw	t1,a4,-48
    16ae:	0015189b          	slliw	a7,a0,0x1
    while (isdigit(*s))
    16b2:	fd06059b          	addiw	a1,a2,-48
        n = 10 * n - (*s++ - '0');
    16b6:	0685                	addi	a3,a3,1
    16b8:	4068853b          	subw	a0,a7,t1
    while (isdigit(*s))
    16bc:	0006071b          	sext.w	a4,a2
    16c0:	feb870e3          	bgeu	a6,a1,16a0 <atoi+0x3e>
    return neg ? n : -n;
    16c4:	000e0563          	beqz	t3,16ce <atoi+0x6c>
}
    16c8:	8082                	ret
        s++;
    16ca:	0505                	addi	a0,a0,1
    16cc:	bf71                	j	1668 <atoi+0x6>
    return neg ? n : -n;
    16ce:	4113053b          	subw	a0,t1,a7
    16d2:	8082                	ret
    while (isdigit(*s))
    16d4:	00154783          	lbu	a5,1(a0)
    16d8:	4625                	li	a2,9
        s++;
    16da:	00150693          	addi	a3,a0,1
    while (isdigit(*s))
    16de:	fd07859b          	addiw	a1,a5,-48
    16e2:	0007871b          	sext.w	a4,a5
    int n = 0, neg = 0;
    16e6:	4e01                	li	t3,0
    while (isdigit(*s))
    16e8:	fab67ae3          	bgeu	a2,a1,169c <atoi+0x3a>
    16ec:	4501                	li	a0,0
}
    16ee:	8082                	ret
    while (isdigit(*s))
    16f0:	00154783          	lbu	a5,1(a0)
    16f4:	4625                	li	a2,9
        s++;
    16f6:	00150693          	addi	a3,a0,1
    while (isdigit(*s))
    16fa:	fd07859b          	addiw	a1,a5,-48
    16fe:	0007871b          	sext.w	a4,a5
    1702:	feb665e3          	bltu	a2,a1,16ec <atoi+0x8a>
        neg = 1;
    1706:	4e05                	li	t3,1
    1708:	bf51                	j	169c <atoi+0x3a>

000000000000170a <memset>:

void *memset(void *dest, int c, size_t n)
{
    char *p = dest;
    for (int i = 0; i < n; ++i, *(p++) = c)
    170a:	16060d63          	beqz	a2,1884 <memset+0x17a>
    170e:	40a007b3          	neg	a5,a0
    1712:	8b9d                	andi	a5,a5,7
    1714:	00778713          	addi	a4,a5,7
    1718:	482d                	li	a6,11
    171a:	0ff5f593          	andi	a1,a1,255
    171e:	fff60693          	addi	a3,a2,-1
    1722:	17076263          	bltu	a4,a6,1886 <memset+0x17c>
    1726:	16e6ea63          	bltu	a3,a4,189a <memset+0x190>
    172a:	16078563          	beqz	a5,1894 <memset+0x18a>
    172e:	00b50023          	sb	a1,0(a0)
    1732:	4705                	li	a4,1
    1734:	00150e93          	addi	t4,a0,1
    1738:	14e78c63          	beq	a5,a4,1890 <memset+0x186>
    173c:	00b500a3          	sb	a1,1(a0)
    1740:	4709                	li	a4,2
    1742:	00250e93          	addi	t4,a0,2
    1746:	14e78d63          	beq	a5,a4,18a0 <memset+0x196>
    174a:	00b50123          	sb	a1,2(a0)
    174e:	470d                	li	a4,3
    1750:	00350e93          	addi	t4,a0,3
    1754:	12e78b63          	beq	a5,a4,188a <memset+0x180>
    1758:	00b501a3          	sb	a1,3(a0)
    175c:	4711                	li	a4,4
    175e:	00450e93          	addi	t4,a0,4
    1762:	14e78163          	beq	a5,a4,18a4 <memset+0x19a>
    1766:	00b50223          	sb	a1,4(a0)
    176a:	4715                	li	a4,5
    176c:	00550e93          	addi	t4,a0,5
    1770:	12e78c63          	beq	a5,a4,18a8 <memset+0x19e>
    1774:	00b502a3          	sb	a1,5(a0)
    1778:	471d                	li	a4,7
    177a:	00650e93          	addi	t4,a0,6
    177e:	12e79763          	bne	a5,a4,18ac <memset+0x1a2>
    1782:	00750e93          	addi	t4,a0,7
    1786:	00b50323          	sb	a1,6(a0)
    178a:	4f1d                	li	t5,7
    178c:	00859713          	slli	a4,a1,0x8
    1790:	8f4d                	or	a4,a4,a1
    1792:	01059e13          	slli	t3,a1,0x10
    1796:	01c76e33          	or	t3,a4,t3
    179a:	01859313          	slli	t1,a1,0x18
    179e:	006e6333          	or	t1,t3,t1
    17a2:	02059893          	slli	a7,a1,0x20
    17a6:	011368b3          	or	a7,t1,a7
    17aa:	02859813          	slli	a6,a1,0x28
    17ae:	40f60333          	sub	t1,a2,a5
    17b2:	0108e833          	or	a6,a7,a6
    17b6:	03059693          	slli	a3,a1,0x30
    17ba:	00d866b3          	or	a3,a6,a3
    17be:	03859713          	slli	a4,a1,0x38
    17c2:	97aa                	add	a5,a5,a0
    17c4:	ff837813          	andi	a6,t1,-8
    17c8:	8f55                	or	a4,a4,a3
    17ca:	00f806b3          	add	a3,a6,a5
    17ce:	e398                	sd	a4,0(a5)
    17d0:	07a1                	addi	a5,a5,8
    17d2:	fed79ee3          	bne	a5,a3,17ce <memset+0xc4>
    17d6:	ff837693          	andi	a3,t1,-8
    17da:	00de87b3          	add	a5,t4,a3
    17de:	01e6873b          	addw	a4,a3,t5
    17e2:	0ad30663          	beq	t1,a3,188e <memset+0x184>
    17e6:	00b78023          	sb	a1,0(a5)
    17ea:	0017069b          	addiw	a3,a4,1
    17ee:	08c6fb63          	bgeu	a3,a2,1884 <memset+0x17a>
    17f2:	00b780a3          	sb	a1,1(a5)
    17f6:	0027069b          	addiw	a3,a4,2
    17fa:	08c6f563          	bgeu	a3,a2,1884 <memset+0x17a>
    17fe:	00b78123          	sb	a1,2(a5)
    1802:	0037069b          	addiw	a3,a4,3
    1806:	06c6ff63          	bgeu	a3,a2,1884 <memset+0x17a>
    180a:	00b781a3          	sb	a1,3(a5)
    180e:	0047069b          	addiw	a3,a4,4
    1812:	06c6f963          	bgeu	a3,a2,1884 <memset+0x17a>
    1816:	00b78223          	sb	a1,4(a5)
    181a:	0057069b          	addiw	a3,a4,5
    181e:	06c6f363          	bgeu	a3,a2,1884 <memset+0x17a>
    1822:	00b782a3          	sb	a1,5(a5)
    1826:	0067069b          	addiw	a3,a4,6
    182a:	04c6fd63          	bgeu	a3,a2,1884 <memset+0x17a>
    182e:	00b78323          	sb	a1,6(a5)
    1832:	0077069b          	addiw	a3,a4,7
    1836:	04c6f763          	bgeu	a3,a2,1884 <memset+0x17a>
    183a:	00b783a3          	sb	a1,7(a5)
    183e:	0087069b          	addiw	a3,a4,8
    1842:	04c6f163          	bgeu	a3,a2,1884 <memset+0x17a>
    1846:	00b78423          	sb	a1,8(a5)
    184a:	0097069b          	addiw	a3,a4,9
    184e:	02c6fb63          	bgeu	a3,a2,1884 <memset+0x17a>
    1852:	00b784a3          	sb	a1,9(a5)
    1856:	00a7069b          	addiw	a3,a4,10
    185a:	02c6f563          	bgeu	a3,a2,1884 <memset+0x17a>
    185e:	00b78523          	sb	a1,10(a5)
    1862:	00b7069b          	addiw	a3,a4,11
    1866:	00c6ff63          	bgeu	a3,a2,1884 <memset+0x17a>
    186a:	00b785a3          	sb	a1,11(a5)
    186e:	00c7069b          	addiw	a3,a4,12
    1872:	00c6f963          	bgeu	a3,a2,1884 <memset+0x17a>
    1876:	00b78623          	sb	a1,12(a5)
    187a:	2735                	addiw	a4,a4,13
    187c:	00c77463          	bgeu	a4,a2,1884 <memset+0x17a>
    1880:	00b786a3          	sb	a1,13(a5)
        ;
    return dest;
}
    1884:	8082                	ret
    1886:	472d                	li	a4,11
    1888:	bd79                	j	1726 <memset+0x1c>
    for (int i = 0; i < n; ++i, *(p++) = c)
    188a:	4f0d                	li	t5,3
    188c:	b701                	j	178c <memset+0x82>
    188e:	8082                	ret
    1890:	4f05                	li	t5,1
    1892:	bded                	j	178c <memset+0x82>
    1894:	8eaa                	mv	t4,a0
    1896:	4f01                	li	t5,0
    1898:	bdd5                	j	178c <memset+0x82>
    189a:	87aa                	mv	a5,a0
    189c:	4701                	li	a4,0
    189e:	b7a1                	j	17e6 <memset+0xdc>
    18a0:	4f09                	li	t5,2
    18a2:	b5ed                	j	178c <memset+0x82>
    18a4:	4f11                	li	t5,4
    18a6:	b5dd                	j	178c <memset+0x82>
    18a8:	4f15                	li	t5,5
    18aa:	b5cd                	j	178c <memset+0x82>
    18ac:	4f19                	li	t5,6
    18ae:	bdf9                	j	178c <memset+0x82>

00000000000018b0 <strcmp>:

int strcmp(const char *l, const char *r)
{
    for (; *l == *r && *l; l++, r++)
    18b0:	00054783          	lbu	a5,0(a0)
    18b4:	0005c703          	lbu	a4,0(a1)
    18b8:	00e79863          	bne	a5,a4,18c8 <strcmp+0x18>
    18bc:	0505                	addi	a0,a0,1
    18be:	0585                	addi	a1,a1,1
    18c0:	fbe5                	bnez	a5,18b0 <strcmp>
    18c2:	4501                	li	a0,0
        ;
    return *(unsigned char *)l - *(unsigned char *)r;
}
    18c4:	9d19                	subw	a0,a0,a4
    18c6:	8082                	ret
    return *(unsigned char *)l - *(unsigned char *)r;
    18c8:	0007851b          	sext.w	a0,a5
    18cc:	bfe5                	j	18c4 <strcmp+0x14>

00000000000018ce <strncmp>:

int strncmp(const char *_l, const char *_r, size_t n)
{
    const unsigned char *l = (void *)_l, *r = (void *)_r;
    if (!n--)
    18ce:	ce05                	beqz	a2,1906 <strncmp+0x38>
        return 0;
    for (; *l && *r && n && *l == *r; l++, r++, n--)
    18d0:	00054703          	lbu	a4,0(a0)
    18d4:	0005c783          	lbu	a5,0(a1)
    18d8:	cb0d                	beqz	a4,190a <strncmp+0x3c>
    if (!n--)
    18da:	167d                	addi	a2,a2,-1
    18dc:	00c506b3          	add	a3,a0,a2
    18e0:	a819                	j	18f6 <strncmp+0x28>
    for (; *l && *r && n && *l == *r; l++, r++, n--)
    18e2:	00a68e63          	beq	a3,a0,18fe <strncmp+0x30>
    18e6:	0505                	addi	a0,a0,1
    18e8:	00e79b63          	bne	a5,a4,18fe <strncmp+0x30>
    18ec:	00054703          	lbu	a4,0(a0)
        ;
    return *l - *r;
    18f0:	0005c783          	lbu	a5,0(a1)
    for (; *l && *r && n && *l == *r; l++, r++, n--)
    18f4:	cb19                	beqz	a4,190a <strncmp+0x3c>
    18f6:	0005c783          	lbu	a5,0(a1)
    18fa:	0585                	addi	a1,a1,1
    18fc:	f3fd                	bnez	a5,18e2 <strncmp+0x14>
    return *l - *r;
    18fe:	0007051b          	sext.w	a0,a4
    1902:	9d1d                	subw	a0,a0,a5
    1904:	8082                	ret
        return 0;
    1906:	4501                	li	a0,0
}
    1908:	8082                	ret
    190a:	4501                	li	a0,0
    return *l - *r;
    190c:	9d1d                	subw	a0,a0,a5
    190e:	8082                	ret

0000000000001910 <strlen>:
size_t strlen(const char *s)
{
    const char *a = s;
    typedef size_t __attribute__((__may_alias__)) word;
    const word *w;
    for (; (uintptr_t)s % SS; s++)
    1910:	00757793          	andi	a5,a0,7
    1914:	cf89                	beqz	a5,192e <strlen+0x1e>
    1916:	87aa                	mv	a5,a0
    1918:	a029                	j	1922 <strlen+0x12>
    191a:	0785                	addi	a5,a5,1
    191c:	0077f713          	andi	a4,a5,7
    1920:	cb01                	beqz	a4,1930 <strlen+0x20>
        if (!*s)
    1922:	0007c703          	lbu	a4,0(a5)
    1926:	fb75                	bnez	a4,191a <strlen+0xa>
    for (w = (const void *)s; !HASZERO(*w); w++)
        ;
    s = (const void *)w;
    for (; *s; s++)
        ;
    return s - a;
    1928:	40a78533          	sub	a0,a5,a0
}
    192c:	8082                	ret
    for (; (uintptr_t)s % SS; s++)
    192e:	87aa                	mv	a5,a0
    for (w = (const void *)s; !HASZERO(*w); w++)
    1930:	6394                	ld	a3,0(a5)
    1932:	00000597          	auipc	a1,0x0
    1936:	6d65b583          	ld	a1,1750(a1) # 2008 <__clone+0xe0>
    193a:	00000617          	auipc	a2,0x0
    193e:	6d663603          	ld	a2,1750(a2) # 2010 <__clone+0xe8>
    1942:	a019                	j	1948 <strlen+0x38>
    1944:	6794                	ld	a3,8(a5)
    1946:	07a1                	addi	a5,a5,8
    1948:	00b68733          	add	a4,a3,a1
    194c:	fff6c693          	not	a3,a3
    1950:	8f75                	and	a4,a4,a3
    1952:	8f71                	and	a4,a4,a2
    1954:	db65                	beqz	a4,1944 <strlen+0x34>
    for (; *s; s++)
    1956:	0007c703          	lbu	a4,0(a5)
    195a:	d779                	beqz	a4,1928 <strlen+0x18>
    195c:	0017c703          	lbu	a4,1(a5)
    1960:	0785                	addi	a5,a5,1
    1962:	d379                	beqz	a4,1928 <strlen+0x18>
    1964:	0017c703          	lbu	a4,1(a5)
    1968:	0785                	addi	a5,a5,1
    196a:	fb6d                	bnez	a4,195c <strlen+0x4c>
    196c:	bf75                	j	1928 <strlen+0x18>

000000000000196e <memchr>:

void *memchr(const void *src, int c, size_t n)
{
    const unsigned char *s = src;
    c = (unsigned char)c;
    for (; ((uintptr_t)s & ALIGN) && n && *s != c; s++, n--)
    196e:	00757713          	andi	a4,a0,7
{
    1972:	87aa                	mv	a5,a0
    c = (unsigned char)c;
    1974:	0ff5f593          	andi	a1,a1,255
    for (; ((uintptr_t)s & ALIGN) && n && *s != c; s++, n--)
    1978:	cb19                	beqz	a4,198e <memchr+0x20>
    197a:	ce25                	beqz	a2,19f2 <memchr+0x84>
    197c:	0007c703          	lbu	a4,0(a5)
    1980:	04b70e63          	beq	a4,a1,19dc <memchr+0x6e>
    1984:	0785                	addi	a5,a5,1
    1986:	0077f713          	andi	a4,a5,7
    198a:	167d                	addi	a2,a2,-1
    198c:	f77d                	bnez	a4,197a <memchr+0xc>
            ;
        s = (const void *)w;
    }
    for (; n && *s != c; s++, n--)
        ;
    return n ? (void *)s : 0;
    198e:	4501                	li	a0,0
    if (n && *s != c)
    1990:	c235                	beqz	a2,19f4 <memchr+0x86>
    1992:	0007c703          	lbu	a4,0(a5)
    1996:	04b70363          	beq	a4,a1,19dc <memchr+0x6e>
        size_t k = ONES * c;
    199a:	00000517          	auipc	a0,0x0
    199e:	67e53503          	ld	a0,1662(a0) # 2018 <__clone+0xf0>
        for (w = (const void *)s; n >= SS && !HASZERO(*w ^ k); w++, n -= SS)
    19a2:	471d                	li	a4,7
        size_t k = ONES * c;
    19a4:	02a58533          	mul	a0,a1,a0
        for (w = (const void *)s; n >= SS && !HASZERO(*w ^ k); w++, n -= SS)
    19a8:	02c77a63          	bgeu	a4,a2,19dc <memchr+0x6e>
    19ac:	00000897          	auipc	a7,0x0
    19b0:	65c8b883          	ld	a7,1628(a7) # 2008 <__clone+0xe0>
    19b4:	00000817          	auipc	a6,0x0
    19b8:	65c83803          	ld	a6,1628(a6) # 2010 <__clone+0xe8>
    19bc:	431d                	li	t1,7
    19be:	a029                	j	19c8 <memchr+0x5a>
    19c0:	1661                	addi	a2,a2,-8
    19c2:	07a1                	addi	a5,a5,8
    19c4:	02c37963          	bgeu	t1,a2,19f6 <memchr+0x88>
    19c8:	6398                	ld	a4,0(a5)
    19ca:	8f29                	xor	a4,a4,a0
    19cc:	011706b3          	add	a3,a4,a7
    19d0:	fff74713          	not	a4,a4
    19d4:	8f75                	and	a4,a4,a3
    19d6:	01077733          	and	a4,a4,a6
    19da:	d37d                	beqz	a4,19c0 <memchr+0x52>
    19dc:	853e                	mv	a0,a5
    19de:	97b2                	add	a5,a5,a2
    19e0:	a021                	j	19e8 <memchr+0x7a>
    for (; n && *s != c; s++, n--)
    19e2:	0505                	addi	a0,a0,1
    19e4:	00f50763          	beq	a0,a5,19f2 <memchr+0x84>
    19e8:	00054703          	lbu	a4,0(a0)
    19ec:	feb71be3          	bne	a4,a1,19e2 <memchr+0x74>
    19f0:	8082                	ret
    return n ? (void *)s : 0;
    19f2:	4501                	li	a0,0
}
    19f4:	8082                	ret
    return n ? (void *)s : 0;
    19f6:	4501                	li	a0,0
    for (; n && *s != c; s++, n--)
    19f8:	f275                	bnez	a2,19dc <memchr+0x6e>
}
    19fa:	8082                	ret

00000000000019fc <strnlen>:

size_t strnlen(const char *s, size_t n)
{
    19fc:	1101                	addi	sp,sp,-32
    19fe:	e822                	sd	s0,16(sp)
    const char *p = memchr(s, 0, n);
    1a00:	862e                	mv	a2,a1
{
    1a02:	842e                	mv	s0,a1
    const char *p = memchr(s, 0, n);
    1a04:	4581                	li	a1,0
{
    1a06:	e426                	sd	s1,8(sp)
    1a08:	ec06                	sd	ra,24(sp)
    1a0a:	84aa                	mv	s1,a0
    const char *p = memchr(s, 0, n);
    1a0c:	f63ff0ef          	jal	ra,196e <memchr>
    return p ? p - s : n;
    1a10:	c519                	beqz	a0,1a1e <strnlen+0x22>
}
    1a12:	60e2                	ld	ra,24(sp)
    1a14:	6442                	ld	s0,16(sp)
    return p ? p - s : n;
    1a16:	8d05                	sub	a0,a0,s1
}
    1a18:	64a2                	ld	s1,8(sp)
    1a1a:	6105                	addi	sp,sp,32
    1a1c:	8082                	ret
    1a1e:	60e2                	ld	ra,24(sp)
    return p ? p - s : n;
    1a20:	8522                	mv	a0,s0
}
    1a22:	6442                	ld	s0,16(sp)
    1a24:	64a2                	ld	s1,8(sp)
    1a26:	6105                	addi	sp,sp,32
    1a28:	8082                	ret

0000000000001a2a <strcpy>:
char *strcpy(char *restrict d, const char *s)
{
    typedef size_t __attribute__((__may_alias__)) word;
    word *wd;
    const word *ws;
    if ((uintptr_t)s % SS == (uintptr_t)d % SS)
    1a2a:	00b547b3          	xor	a5,a0,a1
    1a2e:	8b9d                	andi	a5,a5,7
    1a30:	eb95                	bnez	a5,1a64 <strcpy+0x3a>
    {
        for (; (uintptr_t)s % SS; s++, d++)
    1a32:	0075f793          	andi	a5,a1,7
    1a36:	e7b1                	bnez	a5,1a82 <strcpy+0x58>
            if (!(*d = *s))
                return d;
        wd = (void *)d;
        ws = (const void *)s;
        for (; !HASZERO(*ws); *wd++ = *ws++)
    1a38:	6198                	ld	a4,0(a1)
    1a3a:	00000617          	auipc	a2,0x0
    1a3e:	5ce63603          	ld	a2,1486(a2) # 2008 <__clone+0xe0>
    1a42:	00000817          	auipc	a6,0x0
    1a46:	5ce83803          	ld	a6,1486(a6) # 2010 <__clone+0xe8>
    1a4a:	a029                	j	1a54 <strcpy+0x2a>
    1a4c:	e118                	sd	a4,0(a0)
    1a4e:	6598                	ld	a4,8(a1)
    1a50:	05a1                	addi	a1,a1,8
    1a52:	0521                	addi	a0,a0,8
    1a54:	00c707b3          	add	a5,a4,a2
    1a58:	fff74693          	not	a3,a4
    1a5c:	8ff5                	and	a5,a5,a3
    1a5e:	0107f7b3          	and	a5,a5,a6
    1a62:	d7ed                	beqz	a5,1a4c <strcpy+0x22>
            ;
        d = (void *)wd;
        s = (const void *)ws;
    }
    for (; (*d = *s); s++, d++)
    1a64:	0005c783          	lbu	a5,0(a1)
    1a68:	00f50023          	sb	a5,0(a0)
    1a6c:	c785                	beqz	a5,1a94 <strcpy+0x6a>
    1a6e:	0015c783          	lbu	a5,1(a1)
    1a72:	0505                	addi	a0,a0,1
    1a74:	0585                	addi	a1,a1,1
    1a76:	00f50023          	sb	a5,0(a0)
    1a7a:	fbf5                	bnez	a5,1a6e <strcpy+0x44>
        ;
    return d;
}
    1a7c:	8082                	ret
        for (; (uintptr_t)s % SS; s++, d++)
    1a7e:	0505                	addi	a0,a0,1
    1a80:	df45                	beqz	a4,1a38 <strcpy+0xe>
            if (!(*d = *s))
    1a82:	0005c783          	lbu	a5,0(a1)
        for (; (uintptr_t)s % SS; s++, d++)
    1a86:	0585                	addi	a1,a1,1
    1a88:	0075f713          	andi	a4,a1,7
            if (!(*d = *s))
    1a8c:	00f50023          	sb	a5,0(a0)
    1a90:	f7fd                	bnez	a5,1a7e <strcpy+0x54>
}
    1a92:	8082                	ret
    1a94:	8082                	ret

0000000000001a96 <strncpy>:
char *strncpy(char *restrict d, const char *s, size_t n)
{
    typedef size_t __attribute__((__may_alias__)) word;
    word *wd;
    const word *ws;
    if (((uintptr_t)s & ALIGN) == ((uintptr_t)d & ALIGN))
    1a96:	00b547b3          	xor	a5,a0,a1
    1a9a:	8b9d                	andi	a5,a5,7
    1a9c:	1a079863          	bnez	a5,1c4c <strncpy+0x1b6>
    {
        for (; ((uintptr_t)s & ALIGN) && n && (*d = *s); n--, s++, d++)
    1aa0:	0075f793          	andi	a5,a1,7
    1aa4:	16078463          	beqz	a5,1c0c <strncpy+0x176>
    1aa8:	ea01                	bnez	a2,1ab8 <strncpy+0x22>
    1aaa:	a421                	j	1cb2 <strncpy+0x21c>
    1aac:	167d                	addi	a2,a2,-1
    1aae:	0505                	addi	a0,a0,1
    1ab0:	14070e63          	beqz	a4,1c0c <strncpy+0x176>
    1ab4:	1a060863          	beqz	a2,1c64 <strncpy+0x1ce>
    1ab8:	0005c783          	lbu	a5,0(a1)
    1abc:	0585                	addi	a1,a1,1
    1abe:	0075f713          	andi	a4,a1,7
    1ac2:	00f50023          	sb	a5,0(a0)
    1ac6:	f3fd                	bnez	a5,1aac <strncpy+0x16>
    1ac8:	4805                	li	a6,1
    1aca:	1a061863          	bnez	a2,1c7a <strncpy+0x1e4>
    1ace:	40a007b3          	neg	a5,a0
    1ad2:	8b9d                	andi	a5,a5,7
    1ad4:	4681                	li	a3,0
    1ad6:	18061a63          	bnez	a2,1c6a <strncpy+0x1d4>
    1ada:	00778713          	addi	a4,a5,7
    1ade:	45ad                	li	a1,11
    1ae0:	18b76363          	bltu	a4,a1,1c66 <strncpy+0x1d0>
    1ae4:	1ae6eb63          	bltu	a3,a4,1c9a <strncpy+0x204>
    1ae8:	1a078363          	beqz	a5,1c8e <strncpy+0x1f8>
    for (int i = 0; i < n; ++i, *(p++) = c)
    1aec:	00050023          	sb	zero,0(a0)
    1af0:	4685                	li	a3,1
    1af2:	00150713          	addi	a4,a0,1
    1af6:	18d78f63          	beq	a5,a3,1c94 <strncpy+0x1fe>
    1afa:	000500a3          	sb	zero,1(a0)
    1afe:	4689                	li	a3,2
    1b00:	00250713          	addi	a4,a0,2
    1b04:	18d78e63          	beq	a5,a3,1ca0 <strncpy+0x20a>
    1b08:	00050123          	sb	zero,2(a0)
    1b0c:	468d                	li	a3,3
    1b0e:	00350713          	addi	a4,a0,3
    1b12:	16d78c63          	beq	a5,a3,1c8a <strncpy+0x1f4>
    1b16:	000501a3          	sb	zero,3(a0)
    1b1a:	4691                	li	a3,4
    1b1c:	00450713          	addi	a4,a0,4
    1b20:	18d78263          	beq	a5,a3,1ca4 <strncpy+0x20e>
    1b24:	00050223          	sb	zero,4(a0)
    1b28:	4695                	li	a3,5
    1b2a:	00550713          	addi	a4,a0,5
    1b2e:	16d78d63          	beq	a5,a3,1ca8 <strncpy+0x212>
    1b32:	000502a3          	sb	zero,5(a0)
    1b36:	469d                	li	a3,7
    1b38:	00650713          	addi	a4,a0,6
    1b3c:	16d79863          	bne	a5,a3,1cac <strncpy+0x216>
    1b40:	00750713          	addi	a4,a0,7
    1b44:	00050323          	sb	zero,6(a0)
    1b48:	40f80833          	sub	a6,a6,a5
    1b4c:	ff887593          	andi	a1,a6,-8
    1b50:	97aa                	add	a5,a5,a0
    1b52:	95be                	add	a1,a1,a5
    1b54:	0007b023          	sd	zero,0(a5)
    1b58:	07a1                	addi	a5,a5,8
    1b5a:	feb79de3          	bne	a5,a1,1b54 <strncpy+0xbe>
    1b5e:	ff887593          	andi	a1,a6,-8
    1b62:	9ead                	addw	a3,a3,a1
    1b64:	00b707b3          	add	a5,a4,a1
    1b68:	12b80863          	beq	a6,a1,1c98 <strncpy+0x202>
    1b6c:	00078023          	sb	zero,0(a5)
    1b70:	0016871b          	addiw	a4,a3,1
    1b74:	0ec77863          	bgeu	a4,a2,1c64 <strncpy+0x1ce>
    1b78:	000780a3          	sb	zero,1(a5)
    1b7c:	0026871b          	addiw	a4,a3,2
    1b80:	0ec77263          	bgeu	a4,a2,1c64 <strncpy+0x1ce>
    1b84:	00078123          	sb	zero,2(a5)
    1b88:	0036871b          	addiw	a4,a3,3
    1b8c:	0cc77c63          	bgeu	a4,a2,1c64 <strncpy+0x1ce>
    1b90:	000781a3          	sb	zero,3(a5)
    1b94:	0046871b          	addiw	a4,a3,4
    1b98:	0cc77663          	bgeu	a4,a2,1c64 <strncpy+0x1ce>
    1b9c:	00078223          	sb	zero,4(a5)
    1ba0:	0056871b          	addiw	a4,a3,5
    1ba4:	0cc77063          	bgeu	a4,a2,1c64 <strncpy+0x1ce>
    1ba8:	000782a3          	sb	zero,5(a5)
    1bac:	0066871b          	addiw	a4,a3,6
    1bb0:	0ac77a63          	bgeu	a4,a2,1c64 <strncpy+0x1ce>
    1bb4:	00078323          	sb	zero,6(a5)
    1bb8:	0076871b          	addiw	a4,a3,7
    1bbc:	0ac77463          	bgeu	a4,a2,1c64 <strncpy+0x1ce>
    1bc0:	000783a3          	sb	zero,7(a5)
    1bc4:	0086871b          	addiw	a4,a3,8
    1bc8:	08c77e63          	bgeu	a4,a2,1c64 <strncpy+0x1ce>
    1bcc:	00078423          	sb	zero,8(a5)
    1bd0:	0096871b          	addiw	a4,a3,9
    1bd4:	08c77863          	bgeu	a4,a2,1c64 <strncpy+0x1ce>
    1bd8:	000784a3          	sb	zero,9(a5)
    1bdc:	00a6871b          	addiw	a4,a3,10
    1be0:	08c77263          	bgeu	a4,a2,1c64 <strncpy+0x1ce>
    1be4:	00078523          	sb	zero,10(a5)
    1be8:	00b6871b          	addiw	a4,a3,11
    1bec:	06c77c63          	bgeu	a4,a2,1c64 <strncpy+0x1ce>
    1bf0:	000785a3          	sb	zero,11(a5)
    1bf4:	00c6871b          	addiw	a4,a3,12
    1bf8:	06c77663          	bgeu	a4,a2,1c64 <strncpy+0x1ce>
    1bfc:	00078623          	sb	zero,12(a5)
    1c00:	26b5                	addiw	a3,a3,13
    1c02:	06c6f163          	bgeu	a3,a2,1c64 <strncpy+0x1ce>
    1c06:	000786a3          	sb	zero,13(a5)
    1c0a:	8082                	ret
            ;
        if (!n || !*s)
    1c0c:	c645                	beqz	a2,1cb4 <strncpy+0x21e>
    1c0e:	0005c783          	lbu	a5,0(a1)
    1c12:	ea078be3          	beqz	a5,1ac8 <strncpy+0x32>
            goto tail;
        wd = (void *)d;
        ws = (const void *)s;
        for (; n >= sizeof(size_t) && !HASZERO(*ws); n -= sizeof(size_t), ws++, wd++)
    1c16:	479d                	li	a5,7
    1c18:	02c7ff63          	bgeu	a5,a2,1c56 <strncpy+0x1c0>
    1c1c:	00000897          	auipc	a7,0x0
    1c20:	3ec8b883          	ld	a7,1004(a7) # 2008 <__clone+0xe0>
    1c24:	00000817          	auipc	a6,0x0
    1c28:	3ec83803          	ld	a6,1004(a6) # 2010 <__clone+0xe8>
    1c2c:	431d                	li	t1,7
    1c2e:	6198                	ld	a4,0(a1)
    1c30:	011707b3          	add	a5,a4,a7
    1c34:	fff74693          	not	a3,a4
    1c38:	8ff5                	and	a5,a5,a3
    1c3a:	0107f7b3          	and	a5,a5,a6
    1c3e:	ef81                	bnez	a5,1c56 <strncpy+0x1c0>
            *wd = *ws;
    1c40:	e118                	sd	a4,0(a0)
        for (; n >= sizeof(size_t) && !HASZERO(*ws); n -= sizeof(size_t), ws++, wd++)
    1c42:	1661                	addi	a2,a2,-8
    1c44:	05a1                	addi	a1,a1,8
    1c46:	0521                	addi	a0,a0,8
    1c48:	fec363e3          	bltu	t1,a2,1c2e <strncpy+0x198>
        d = (void *)wd;
        s = (const void *)ws;
    }
    for (; n && (*d = *s); n--, s++, d++)
    1c4c:	e609                	bnez	a2,1c56 <strncpy+0x1c0>
    1c4e:	a08d                	j	1cb0 <strncpy+0x21a>
    1c50:	167d                	addi	a2,a2,-1
    1c52:	0505                	addi	a0,a0,1
    1c54:	ca01                	beqz	a2,1c64 <strncpy+0x1ce>
    1c56:	0005c783          	lbu	a5,0(a1)
    1c5a:	0585                	addi	a1,a1,1
    1c5c:	00f50023          	sb	a5,0(a0)
    1c60:	fbe5                	bnez	a5,1c50 <strncpy+0x1ba>
        ;
tail:
    1c62:	b59d                	j	1ac8 <strncpy+0x32>
    memset(d, 0, n);
    return d;
}
    1c64:	8082                	ret
    1c66:	472d                	li	a4,11
    1c68:	bdb5                	j	1ae4 <strncpy+0x4e>
    1c6a:	00778713          	addi	a4,a5,7
    1c6e:	45ad                	li	a1,11
    1c70:	fff60693          	addi	a3,a2,-1
    1c74:	e6b778e3          	bgeu	a4,a1,1ae4 <strncpy+0x4e>
    1c78:	b7fd                	j	1c66 <strncpy+0x1d0>
    1c7a:	40a007b3          	neg	a5,a0
    1c7e:	8832                	mv	a6,a2
    1c80:	8b9d                	andi	a5,a5,7
    1c82:	4681                	li	a3,0
    1c84:	e4060be3          	beqz	a2,1ada <strncpy+0x44>
    1c88:	b7cd                	j	1c6a <strncpy+0x1d4>
    for (int i = 0; i < n; ++i, *(p++) = c)
    1c8a:	468d                	li	a3,3
    1c8c:	bd75                	j	1b48 <strncpy+0xb2>
    1c8e:	872a                	mv	a4,a0
    1c90:	4681                	li	a3,0
    1c92:	bd5d                	j	1b48 <strncpy+0xb2>
    1c94:	4685                	li	a3,1
    1c96:	bd4d                	j	1b48 <strncpy+0xb2>
    1c98:	8082                	ret
    1c9a:	87aa                	mv	a5,a0
    1c9c:	4681                	li	a3,0
    1c9e:	b5f9                	j	1b6c <strncpy+0xd6>
    1ca0:	4689                	li	a3,2
    1ca2:	b55d                	j	1b48 <strncpy+0xb2>
    1ca4:	4691                	li	a3,4
    1ca6:	b54d                	j	1b48 <strncpy+0xb2>
    1ca8:	4695                	li	a3,5
    1caa:	bd79                	j	1b48 <strncpy+0xb2>
    1cac:	4699                	li	a3,6
    1cae:	bd69                	j	1b48 <strncpy+0xb2>
    1cb0:	8082                	ret
    1cb2:	8082                	ret
    1cb4:	8082                	ret

0000000000001cb6 <open>:
#include <unistd.h>

#include "syscall.h"

int open(const char *path, int flags)
{
    1cb6:	87aa                	mv	a5,a0
    1cb8:	862e                	mv	a2,a1
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
}

static inline long __syscall4(long n, long a, long b, long c, long d)
{
    register long a7 __asm__("a7") = n;
    1cba:	03800893          	li	a7,56
    register long a0 __asm__("a0") = a;
    1cbe:	f9c00513          	li	a0,-100
    register long a1 __asm__("a1") = b;
    1cc2:	85be                	mv	a1,a5
    register long a2 __asm__("a2") = c;
    register long a3 __asm__("a3") = d;
    1cc4:	4689                	li	a3,2
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3))
    1cc6:	00000073          	ecall
    return syscall(SYS_openat, AT_FDCWD, path, flags, O_RDWR);
}
    1cca:	2501                	sext.w	a0,a0
    1ccc:	8082                	ret

0000000000001cce <openat>:
    register long a7 __asm__("a7") = n;
    1cce:	03800893          	li	a7,56
    register long a3 __asm__("a3") = d;
    1cd2:	18000693          	li	a3,384
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3))
    1cd6:	00000073          	ecall

int openat(int dirfd,const char *path, int flags)
{
    return syscall(SYS_openat, dirfd, path, flags, 0600);
}
    1cda:	2501                	sext.w	a0,a0
    1cdc:	8082                	ret

0000000000001cde <close>:
    register long a7 __asm__("a7") = n;
    1cde:	03900893          	li	a7,57
    __asm_syscall("r"(a7), "0"(a0))
    1ce2:	00000073          	ecall

int close(int fd)
{
    return syscall(SYS_close, fd);
}
    1ce6:	2501                	sext.w	a0,a0
    1ce8:	8082                	ret

0000000000001cea <read>:
    register long a7 __asm__("a7") = n;
    1cea:	03f00893          	li	a7,63
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1cee:	00000073          	ecall

ssize_t read(int fd, void *buf, size_t len)
{
    return syscall(SYS_read, fd, buf, len);
}
    1cf2:	8082                	ret

0000000000001cf4 <write>:
    register long a7 __asm__("a7") = n;
    1cf4:	04000893          	li	a7,64
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1cf8:	00000073          	ecall

ssize_t write(int fd, const void *buf, size_t len)
{
    return syscall(SYS_write, fd, buf, len);
}
    1cfc:	8082                	ret

0000000000001cfe <getpid>:
    register long a7 __asm__("a7") = n;
    1cfe:	0ac00893          	li	a7,172
    __asm_syscall("r"(a7))
    1d02:	00000073          	ecall

pid_t getpid(void)
{
    return syscall(SYS_getpid);
}
    1d06:	2501                	sext.w	a0,a0
    1d08:	8082                	ret

0000000000001d0a <getppid>:
    register long a7 __asm__("a7") = n;
    1d0a:	0ad00893          	li	a7,173
    __asm_syscall("r"(a7))
    1d0e:	00000073          	ecall

pid_t getppid(void)
{
    return syscall(SYS_getppid);
}
    1d12:	2501                	sext.w	a0,a0
    1d14:	8082                	ret

0000000000001d16 <sched_yield>:
    register long a7 __asm__("a7") = n;
    1d16:	07c00893          	li	a7,124
    __asm_syscall("r"(a7))
    1d1a:	00000073          	ecall

int sched_yield(void)
{
    return syscall(SYS_sched_yield);
}
    1d1e:	2501                	sext.w	a0,a0
    1d20:	8082                	ret

0000000000001d22 <fork>:
    register long a7 __asm__("a7") = n;
    1d22:	0dc00893          	li	a7,220
    register long a0 __asm__("a0") = a;
    1d26:	4545                	li	a0,17
    register long a1 __asm__("a1") = b;
    1d28:	4581                	li	a1,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1d2a:	00000073          	ecall

pid_t fork(void)
{
    return syscall(SYS_clone, SIGCHLD, 0);
}
    1d2e:	2501                	sext.w	a0,a0
    1d30:	8082                	ret

0000000000001d32 <clone>:

pid_t clone(int (*fn)(void *arg), void *arg, void *stack, size_t stack_size, unsigned long flags)
{
    1d32:	85b2                	mv	a1,a2
    1d34:	863a                	mv	a2,a4
    if (stack)
    1d36:	c191                	beqz	a1,1d3a <clone+0x8>
	stack += stack_size;
    1d38:	95b6                	add	a1,a1,a3

    return __clone(fn, stack, flags, NULL, NULL, NULL);
    1d3a:	4781                	li	a5,0
    1d3c:	4701                	li	a4,0
    1d3e:	4681                	li	a3,0
    1d40:	2601                	sext.w	a2,a2
    1d42:	a2dd                	j	1f28 <__clone>

0000000000001d44 <exit>:
    register long a7 __asm__("a7") = n;
    1d44:	05d00893          	li	a7,93
    __asm_syscall("r"(a7), "0"(a0))
    1d48:	00000073          	ecall
    //return syscall(SYS_clone, fn, stack, flags, NULL, NULL, NULL);
}
void exit(int code)
{
    syscall(SYS_exit, code);
}
    1d4c:	8082                	ret

0000000000001d4e <waitpid>:
    register long a7 __asm__("a7") = n;
    1d4e:	10400893          	li	a7,260
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1d52:	00000073          	ecall

int waitpid(int pid, int *code, int options)
{
    return syscall(SYS_wait4, pid, code, options);
}
    1d56:	2501                	sext.w	a0,a0
    1d58:	8082                	ret

0000000000001d5a <exec>:
    register long a7 __asm__("a7") = n;
    1d5a:	0dd00893          	li	a7,221
    __asm_syscall("r"(a7), "0"(a0))
    1d5e:	00000073          	ecall

int exec(char *name)
{
    return syscall(SYS_execve, name);
}
    1d62:	2501                	sext.w	a0,a0
    1d64:	8082                	ret

0000000000001d66 <execve>:
    register long a7 __asm__("a7") = n;
    1d66:	0dd00893          	li	a7,221
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1d6a:	00000073          	ecall

int execve(const char *name, char *const argv[], char *const argp[])
{
    return syscall(SYS_execve, name, argv, argp);
}
    1d6e:	2501                	sext.w	a0,a0
    1d70:	8082                	ret

0000000000001d72 <times>:
    register long a7 __asm__("a7") = n;
    1d72:	09900893          	li	a7,153
    __asm_syscall("r"(a7), "0"(a0))
    1d76:	00000073          	ecall

int times(void *mytimes)
{
	return syscall(SYS_times, mytimes);
}
    1d7a:	2501                	sext.w	a0,a0
    1d7c:	8082                	ret

0000000000001d7e <get_time>:

int64 get_time()
{
    1d7e:	1141                	addi	sp,sp,-16
    register long a7 __asm__("a7") = n;
    1d80:	0a900893          	li	a7,169
    register long a0 __asm__("a0") = a;
    1d84:	850a                	mv	a0,sp
    register long a1 __asm__("a1") = b;
    1d86:	4581                	li	a1,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1d88:	00000073          	ecall
    TimeVal time;
    int err = sys_get_time(&time, 0);
    if (err == 0)
    1d8c:	2501                	sext.w	a0,a0
    1d8e:	ed09                	bnez	a0,1da8 <get_time+0x2a>
    {
        return ((time.sec & 0xffff) * 1000 + time.usec / 1000);
    1d90:	67a2                	ld	a5,8(sp)
    1d92:	3e800713          	li	a4,1000
    1d96:	00015503          	lhu	a0,0(sp)
    1d9a:	02e7d7b3          	divu	a5,a5,a4
    1d9e:	02e50533          	mul	a0,a0,a4
    1da2:	953e                	add	a0,a0,a5
    }
    else
    {
        return -1;
    }
}
    1da4:	0141                	addi	sp,sp,16
    1da6:	8082                	ret
        return -1;
    1da8:	557d                	li	a0,-1
    1daa:	bfed                	j	1da4 <get_time+0x26>

0000000000001dac <sys_get_time>:
    register long a7 __asm__("a7") = n;
    1dac:	0a900893          	li	a7,169
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1db0:	00000073          	ecall

int sys_get_time(TimeVal *ts, int tz)
{
    return syscall(SYS_gettimeofday, ts, tz);
}
    1db4:	2501                	sext.w	a0,a0
    1db6:	8082                	ret

0000000000001db8 <time>:
    register long a7 __asm__("a7") = n;
    1db8:	42600893          	li	a7,1062
    __asm_syscall("r"(a7), "0"(a0))
    1dbc:	00000073          	ecall

int time(unsigned long *tloc)
{
    return syscall(SYS_time, tloc);
}
    1dc0:	2501                	sext.w	a0,a0
    1dc2:	8082                	ret

0000000000001dc4 <sleep>:

int sleep(unsigned long long time)
{
    1dc4:	1141                	addi	sp,sp,-16
    TimeVal tv = {.sec = time, .usec = 0};
    1dc6:	e02a                	sd	a0,0(sp)
    register long a0 __asm__("a0") = a;
    1dc8:	850a                	mv	a0,sp
    1dca:	e402                	sd	zero,8(sp)
    register long a7 __asm__("a7") = n;
    1dcc:	06500893          	li	a7,101
    register long a1 __asm__("a1") = b;
    1dd0:	85aa                	mv	a1,a0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1dd2:	00000073          	ecall
    if (syscall(SYS_nanosleep, &tv, &tv)) return tv.sec;
    1dd6:	e501                	bnez	a0,1dde <sleep+0x1a>
    return 0;
    1dd8:	4501                	li	a0,0
}
    1dda:	0141                	addi	sp,sp,16
    1ddc:	8082                	ret
    if (syscall(SYS_nanosleep, &tv, &tv)) return tv.sec;
    1dde:	4502                	lw	a0,0(sp)
}
    1de0:	0141                	addi	sp,sp,16
    1de2:	8082                	ret

0000000000001de4 <set_priority>:
    register long a7 __asm__("a7") = n;
    1de4:	08c00893          	li	a7,140
    __asm_syscall("r"(a7), "0"(a0))
    1de8:	00000073          	ecall

int set_priority(int prio)
{
    return syscall(SYS_setpriority, prio);
}
    1dec:	2501                	sext.w	a0,a0
    1dee:	8082                	ret

0000000000001df0 <mmap>:
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4))
}

static inline long __syscall6(long n, long a, long b, long c, long d, long e, long f)
{
    register long a7 __asm__("a7") = n;
    1df0:	0de00893          	li	a7,222
    register long a1 __asm__("a1") = b;
    register long a2 __asm__("a2") = c;
    register long a3 __asm__("a3") = d;
    register long a4 __asm__("a4") = e;
    register long a5 __asm__("a5") = f;
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4), "r"(a5))
    1df4:	00000073          	ecall

void *mmap(void *start, size_t len, int prot, int flags, int fd, off_t off)
{
    return syscall(SYS_mmap, start, len, prot, flags, fd, off);
}
    1df8:	8082                	ret

0000000000001dfa <munmap>:
    register long a7 __asm__("a7") = n;
    1dfa:	0d700893          	li	a7,215
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1dfe:	00000073          	ecall

int munmap(void *start, size_t len)
{
    return syscall(SYS_munmap, start, len);
}
    1e02:	2501                	sext.w	a0,a0
    1e04:	8082                	ret

0000000000001e06 <wait>:

int wait(int *code)
{
    1e06:	85aa                	mv	a1,a0
    register long a7 __asm__("a7") = n;
    1e08:	10400893          	li	a7,260
    register long a0 __asm__("a0") = a;
    1e0c:	557d                	li	a0,-1
    register long a2 __asm__("a2") = c;
    1e0e:	4601                	li	a2,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1e10:	00000073          	ecall
    return waitpid(-1, code, 0);
}
    1e14:	2501                	sext.w	a0,a0
    1e16:	8082                	ret

0000000000001e18 <spawn>:
    register long a7 __asm__("a7") = n;
    1e18:	19000893          	li	a7,400
    __asm_syscall("r"(a7), "0"(a0))
    1e1c:	00000073          	ecall

int spawn(char *file)
{
    return syscall(SYS_spawn, file);
}
    1e20:	2501                	sext.w	a0,a0
    1e22:	8082                	ret

0000000000001e24 <mailread>:
    register long a7 __asm__("a7") = n;
    1e24:	19100893          	li	a7,401
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1e28:	00000073          	ecall

int mailread(void *buf, int len)
{
    return syscall(SYS_mailread, buf, len);
}
    1e2c:	2501                	sext.w	a0,a0
    1e2e:	8082                	ret

0000000000001e30 <mailwrite>:
    register long a7 __asm__("a7") = n;
    1e30:	19200893          	li	a7,402
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1e34:	00000073          	ecall

int mailwrite(int pid, void *buf, int len)
{
    return syscall(SYS_mailwrite, pid, buf, len);
}
    1e38:	2501                	sext.w	a0,a0
    1e3a:	8082                	ret

0000000000001e3c <fstat>:
    register long a7 __asm__("a7") = n;
    1e3c:	05000893          	li	a7,80
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1e40:	00000073          	ecall

int fstat(int fd, struct kstat *st)
{
    return syscall(SYS_fstat, fd, st);
}
    1e44:	2501                	sext.w	a0,a0
    1e46:	8082                	ret

0000000000001e48 <sys_linkat>:
    register long a4 __asm__("a4") = e;
    1e48:	1702                	slli	a4,a4,0x20
    register long a7 __asm__("a7") = n;
    1e4a:	02500893          	li	a7,37
    register long a4 __asm__("a4") = e;
    1e4e:	9301                	srli	a4,a4,0x20
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4))
    1e50:	00000073          	ecall

int sys_linkat(int olddirfd, char *oldpath, int newdirfd, char *newpath, unsigned int flags)
{
    return syscall(SYS_linkat, olddirfd, oldpath, newdirfd, newpath, flags);
}
    1e54:	2501                	sext.w	a0,a0
    1e56:	8082                	ret

0000000000001e58 <sys_unlinkat>:
    register long a2 __asm__("a2") = c;
    1e58:	1602                	slli	a2,a2,0x20
    register long a7 __asm__("a7") = n;
    1e5a:	02300893          	li	a7,35
    register long a2 __asm__("a2") = c;
    1e5e:	9201                	srli	a2,a2,0x20
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1e60:	00000073          	ecall

int sys_unlinkat(int dirfd, char *path, unsigned int flags)
{
    return syscall(SYS_unlinkat, dirfd, path, flags);
}
    1e64:	2501                	sext.w	a0,a0
    1e66:	8082                	ret

0000000000001e68 <link>:

int link(char *old_path, char *new_path)
{
    1e68:	87aa                	mv	a5,a0
    1e6a:	86ae                	mv	a3,a1
    register long a7 __asm__("a7") = n;
    1e6c:	02500893          	li	a7,37
    register long a0 __asm__("a0") = a;
    1e70:	f9c00513          	li	a0,-100
    register long a1 __asm__("a1") = b;
    1e74:	85be                	mv	a1,a5
    register long a2 __asm__("a2") = c;
    1e76:	f9c00613          	li	a2,-100
    register long a4 __asm__("a4") = e;
    1e7a:	4701                	li	a4,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4))
    1e7c:	00000073          	ecall
    return sys_linkat(AT_FDCWD, old_path, AT_FDCWD, new_path, 0);
}
    1e80:	2501                	sext.w	a0,a0
    1e82:	8082                	ret

0000000000001e84 <unlink>:

int unlink(char *path)
{
    1e84:	85aa                	mv	a1,a0
    register long a7 __asm__("a7") = n;
    1e86:	02300893          	li	a7,35
    register long a0 __asm__("a0") = a;
    1e8a:	f9c00513          	li	a0,-100
    register long a2 __asm__("a2") = c;
    1e8e:	4601                	li	a2,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1e90:	00000073          	ecall
    return sys_unlinkat(AT_FDCWD, path, 0);
}
    1e94:	2501                	sext.w	a0,a0
    1e96:	8082                	ret

0000000000001e98 <uname>:
    register long a7 __asm__("a7") = n;
    1e98:	0a000893          	li	a7,160
    __asm_syscall("r"(a7), "0"(a0))
    1e9c:	00000073          	ecall

int uname(void *buf)
{
    return syscall(SYS_uname, buf);
}
    1ea0:	2501                	sext.w	a0,a0
    1ea2:	8082                	ret

0000000000001ea4 <brk>:
    register long a7 __asm__("a7") = n;
    1ea4:	0d600893          	li	a7,214
    __asm_syscall("r"(a7), "0"(a0))
    1ea8:	00000073          	ecall

int brk(void *addr)
{
    return syscall(SYS_brk, addr);
}
    1eac:	2501                	sext.w	a0,a0
    1eae:	8082                	ret

0000000000001eb0 <getcwd>:
    register long a7 __asm__("a7") = n;
    1eb0:	48c5                	li	a7,17
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1eb2:	00000073          	ecall

char *getcwd(char *buf, size_t size){
    return syscall(SYS_getcwd, buf, size);
}
    1eb6:	8082                	ret

0000000000001eb8 <chdir>:
    register long a7 __asm__("a7") = n;
    1eb8:	03100893          	li	a7,49
    __asm_syscall("r"(a7), "0"(a0))
    1ebc:	00000073          	ecall

int chdir(const char *path){
    return syscall(SYS_chdir, path);
}
    1ec0:	2501                	sext.w	a0,a0
    1ec2:	8082                	ret

0000000000001ec4 <mkdir>:

int mkdir(const char *path, mode_t mode){
    1ec4:	862e                	mv	a2,a1
    1ec6:	87aa                	mv	a5,a0
    register long a2 __asm__("a2") = c;
    1ec8:	1602                	slli	a2,a2,0x20
    register long a7 __asm__("a7") = n;
    1eca:	02200893          	li	a7,34
    register long a0 __asm__("a0") = a;
    1ece:	f9c00513          	li	a0,-100
    register long a1 __asm__("a1") = b;
    1ed2:	85be                	mv	a1,a5
    register long a2 __asm__("a2") = c;
    1ed4:	9201                	srli	a2,a2,0x20
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1ed6:	00000073          	ecall
    return syscall(SYS_mkdirat, AT_FDCWD, path, mode);
}
    1eda:	2501                	sext.w	a0,a0
    1edc:	8082                	ret

0000000000001ede <getdents>:
    register long a7 __asm__("a7") = n;
    1ede:	03d00893          	li	a7,61
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1ee2:	00000073          	ecall

int getdents(int fd, struct linux_dirent64 *dirp64, unsigned long len){
    //return syscall(SYS_getdents64, fd, dirp64, len);
    return syscall(SYS_getdents64, fd, dirp64, len);
}
    1ee6:	2501                	sext.w	a0,a0
    1ee8:	8082                	ret

0000000000001eea <pipe>:
    register long a7 __asm__("a7") = n;
    1eea:	03b00893          	li	a7,59
    register long a1 __asm__("a1") = b;
    1eee:	4581                	li	a1,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1ef0:	00000073          	ecall

int pipe(int fd[2]){
    return syscall(SYS_pipe2, fd, 0);
}
    1ef4:	2501                	sext.w	a0,a0
    1ef6:	8082                	ret

0000000000001ef8 <dup>:
    register long a7 __asm__("a7") = n;
    1ef8:	48dd                	li	a7,23
    __asm_syscall("r"(a7), "0"(a0))
    1efa:	00000073          	ecall

int dup(int fd){
    return syscall(SYS_dup, fd);
}
    1efe:	2501                	sext.w	a0,a0
    1f00:	8082                	ret

0000000000001f02 <dup2>:
    register long a7 __asm__("a7") = n;
    1f02:	48e1                	li	a7,24
    register long a2 __asm__("a2") = c;
    1f04:	4601                	li	a2,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1f06:	00000073          	ecall

int dup2(int old, int new){
    return syscall(SYS_dup3, old, new, 0);
}
    1f0a:	2501                	sext.w	a0,a0
    1f0c:	8082                	ret

0000000000001f0e <mount>:
    register long a7 __asm__("a7") = n;
    1f0e:	02800893          	li	a7,40
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4))
    1f12:	00000073          	ecall

int mount(const char *special, const char *dir, const char *fstype, unsigned long flags, const void *data)
{
        return syscall(SYS_mount, special, dir, fstype, flags, data);
}
    1f16:	2501                	sext.w	a0,a0
    1f18:	8082                	ret

0000000000001f1a <umount>:
    register long a7 __asm__("a7") = n;
    1f1a:	02700893          	li	a7,39
    register long a1 __asm__("a1") = b;
    1f1e:	4581                	li	a1,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1f20:	00000073          	ecall

int umount(const char *special)
{
        return syscall(SYS_umount2, special, 0);
}
    1f24:	2501                	sext.w	a0,a0
    1f26:	8082                	ret

0000000000001f28 <__clone>:

.global __clone
.type  __clone, %function
__clone:
	# Save func and arg to stack
	addi a1, a1, -16
    1f28:	15c1                	addi	a1,a1,-16
	sd a0, 0(a1)
    1f2a:	e188                	sd	a0,0(a1)
	sd a3, 8(a1)
    1f2c:	e594                	sd	a3,8(a1)

	# Call SYS_clone
	mv a0, a2
    1f2e:	8532                	mv	a0,a2
	mv a2, a4
    1f30:	863a                	mv	a2,a4
	mv a3, a5
    1f32:	86be                	mv	a3,a5
	mv a4, a6
    1f34:	8742                	mv	a4,a6
	li a7, 220 # SYS_clone
    1f36:	0dc00893          	li	a7,220
	ecall
    1f3a:	00000073          	ecall

	beqz a0, 1f
    1f3e:	c111                	beqz	a0,1f42 <__clone+0x1a>
	# Parent
	ret
    1f40:	8082                	ret

	# Child
1:      ld a1, 0(sp)
    1f42:	6582                	ld	a1,0(sp)
	ld a0, 8(sp)
    1f44:	6522                	ld	a0,8(sp)
	jalr a1
    1f46:	9582                	jalr	a1

	# Exit
	li a7, 93 # SYS_exit
    1f48:	05d00893          	li	a7,93
	ecall
    1f4c:	00000073          	ecall
