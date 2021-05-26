
/home/ubuntu/rust/ultra-os/codes/riscv-syscalls-testing/user/build/riscv64/mmap:     file format elf64-littleriscv


Disassembly of section .text:

0000000000001000 <_start>:
.section .text.entry
.globl _start
_start:
    mv a0, sp
    1000:	850a                	mv	a0,sp
    tail __start_main
    1002:	a201                	j	1102 <__start_main>

0000000000001004 <test_mmap>:
 * "  Hello, mmap success"
 * 测试失败时输出：
 * "mmap error."
 */
static struct kstat kst;
void test_mmap(void){
    1004:	1101                	addi	sp,sp,-32
    TEST_START(__func__);
    1006:	00001517          	auipc	a0,0x1
    100a:	f2250513          	addi	a0,a0,-222 # 1f28 <__clone+0x2a>
void test_mmap(void){
    100e:	ec06                	sd	ra,24(sp)
    1010:	e822                	sd	s0,16(sp)
    1012:	e04a                	sd	s2,0(sp)
    1014:	e426                	sd	s1,8(sp)
    TEST_START(__func__);
    1016:	35e000ef          	jal	ra,1374 <puts>
    101a:	00001517          	auipc	a0,0x1
    101e:	04e50513          	addi	a0,a0,78 # 2068 <__func__.0>
    1022:	352000ef          	jal	ra,1374 <puts>
    1026:	00001517          	auipc	a0,0x1
    102a:	f1a50513          	addi	a0,a0,-230 # 1f40 <__clone+0x42>
    102e:	346000ef          	jal	ra,1374 <puts>
    char *array;
    const char *str = "  Hello, mmap successfully!";
    int fd;

    fd = open("test_mmap.txt", O_RDWR | O_CREATE);
    1032:	04200593          	li	a1,66
    1036:	00001517          	auipc	a0,0x1
    103a:	f1a50513          	addi	a0,a0,-230 # 1f50 <__clone+0x52>
    103e:	44f000ef          	jal	ra,1c8c <open>
    1042:	842a                	mv	s0,a0
    write(fd, str, strlen(str));
    1044:	00001517          	auipc	a0,0x1
    1048:	f1c50513          	addi	a0,a0,-228 # 1f60 <__clone+0x62>
    104c:	09b000ef          	jal	ra,18e6 <strlen>
    1050:	862a                	mv	a2,a0
    1052:	00001597          	auipc	a1,0x1
    1056:	f0e58593          	addi	a1,a1,-242 # 1f60 <__clone+0x62>
    105a:	8522                	mv	a0,s0
    105c:	46f000ef          	jal	ra,1cca <write>
    fstat(fd, &kst);
    1060:	00001917          	auipc	s2,0x1
    1064:	f8890913          	addi	s2,s2,-120 # 1fe8 <kst>
    1068:	85ca                	mv	a1,s2
    106a:	8522                	mv	a0,s0
    106c:	5a7000ef          	jal	ra,1e12 <fstat>
    printf("file len: %d\n", kst.st_size);
    1070:	03093583          	ld	a1,48(s2)
    1074:	00001517          	auipc	a0,0x1
    1078:	f0c50513          	addi	a0,a0,-244 # 1f80 <__clone+0x82>
    107c:	31a000ef          	jal	ra,1396 <printf>
    array = mmap(NULL, kst.st_size, PROT_WRITE | PROT_READ, MAP_FILE | MAP_SHARED, fd, 0);
    1080:	03093583          	ld	a1,48(s2)
    1084:	4781                	li	a5,0
    1086:	8722                	mv	a4,s0
    1088:	4685                	li	a3,1
    108a:	460d                	li	a2,3
    108c:	4501                	li	a0,0
    108e:	539000ef          	jal	ra,1dc6 <mmap>
    //printf("return array: %x\n", array);

    if (array == MAP_FAILED) {
    1092:	57fd                	li	a5,-1
    1094:	04f50863          	beq	a0,a5,10e4 <test_mmap+0xe0>
	printf("mmap error.\n");
    }else{
	printf("mmap content: %s\n", array);
    1098:	85aa                	mv	a1,a0
    109a:	84aa                	mv	s1,a0
    109c:	00001517          	auipc	a0,0x1
    10a0:	f0450513          	addi	a0,a0,-252 # 1fa0 <__clone+0xa2>
    10a4:	2f2000ef          	jal	ra,1396 <printf>
	//printf("%s\n", str);

	munmap(array, kst.st_size);
    10a8:	03093583          	ld	a1,48(s2)
    10ac:	8526                	mv	a0,s1
    10ae:	523000ef          	jal	ra,1dd0 <munmap>
    }

    close(fd);
    10b2:	8522                	mv	a0,s0
    10b4:	401000ef          	jal	ra,1cb4 <close>

    TEST_END(__func__);
    10b8:	00001517          	auipc	a0,0x1
    10bc:	f0050513          	addi	a0,a0,-256 # 1fb8 <__clone+0xba>
    10c0:	2b4000ef          	jal	ra,1374 <puts>
    10c4:	00001517          	auipc	a0,0x1
    10c8:	fa450513          	addi	a0,a0,-92 # 2068 <__func__.0>
    10cc:	2a8000ef          	jal	ra,1374 <puts>
}
    10d0:	6442                	ld	s0,16(sp)
    10d2:	60e2                	ld	ra,24(sp)
    10d4:	64a2                	ld	s1,8(sp)
    10d6:	6902                	ld	s2,0(sp)
    TEST_END(__func__);
    10d8:	00001517          	auipc	a0,0x1
    10dc:	e6850513          	addi	a0,a0,-408 # 1f40 <__clone+0x42>
}
    10e0:	6105                	addi	sp,sp,32
    TEST_END(__func__);
    10e2:	ac49                	j	1374 <puts>
	printf("mmap error.\n");
    10e4:	00001517          	auipc	a0,0x1
    10e8:	eac50513          	addi	a0,a0,-340 # 1f90 <__clone+0x92>
    10ec:	2aa000ef          	jal	ra,1396 <printf>
    10f0:	b7c9                	j	10b2 <test_mmap+0xae>

00000000000010f2 <main>:

int main(void){
    10f2:	1141                	addi	sp,sp,-16
    10f4:	e406                	sd	ra,8(sp)
    test_mmap();
    10f6:	f0fff0ef          	jal	ra,1004 <test_mmap>
    return 0;
}
    10fa:	60a2                	ld	ra,8(sp)
    10fc:	4501                	li	a0,0
    10fe:	0141                	addi	sp,sp,16
    1100:	8082                	ret

0000000000001102 <__start_main>:
#include <unistd.h>

extern int main();

int __start_main(long *p)
{
    1102:	85aa                	mv	a1,a0
	int argc = p[0];
	char **argv = (void *)(p+1);

	exit(main(argc, argv));
    1104:	4108                	lw	a0,0(a0)
{
    1106:	1141                	addi	sp,sp,-16
	exit(main(argc, argv));
    1108:	05a1                	addi	a1,a1,8
{
    110a:	e406                	sd	ra,8(sp)
	exit(main(argc, argv));
    110c:	fe7ff0ef          	jal	ra,10f2 <main>
    1110:	40b000ef          	jal	ra,1d1a <exit>
	return 0;
}
    1114:	60a2                	ld	ra,8(sp)
    1116:	4501                	li	a0,0
    1118:	0141                	addi	sp,sp,16
    111a:	8082                	ret

000000000000111c <printint.constprop.0>:
    write(f, s, l);
}

static char digits[] = "0123456789abcdef";

static void printint(int xx, int base, int sign)
    111c:	7179                	addi	sp,sp,-48
    111e:	f406                	sd	ra,40(sp)
{
    char buf[16 + 1];
    int i;
    uint x;

    if (sign && (sign = xx < 0))
    1120:	12054b63          	bltz	a0,1256 <printint.constprop.0+0x13a>

    buf[16] = 0;
    i = 15;
    do
    {
        buf[i--] = digits[x % base];
    1124:	02b577bb          	remuw	a5,a0,a1
    1128:	00001617          	auipc	a2,0x1
    112c:	f5060613          	addi	a2,a2,-176 # 2078 <digits>
    buf[16] = 0;
    1130:	00010c23          	sb	zero,24(sp)
        buf[i--] = digits[x % base];
    1134:	0005871b          	sext.w	a4,a1
    1138:	1782                	slli	a5,a5,0x20
    113a:	9381                	srli	a5,a5,0x20
    113c:	97b2                	add	a5,a5,a2
    113e:	0007c783          	lbu	a5,0(a5)
    } while ((x /= base) != 0);
    1142:	02b5583b          	divuw	a6,a0,a1
        buf[i--] = digits[x % base];
    1146:	00f10ba3          	sb	a5,23(sp)
    } while ((x /= base) != 0);
    114a:	1cb56363          	bltu	a0,a1,1310 <printint.constprop.0+0x1f4>
        buf[i--] = digits[x % base];
    114e:	45b9                	li	a1,14
    1150:	02e877bb          	remuw	a5,a6,a4
    1154:	1782                	slli	a5,a5,0x20
    1156:	9381                	srli	a5,a5,0x20
    1158:	97b2                	add	a5,a5,a2
    115a:	0007c783          	lbu	a5,0(a5)
    } while ((x /= base) != 0);
    115e:	02e856bb          	divuw	a3,a6,a4
        buf[i--] = digits[x % base];
    1162:	00f10b23          	sb	a5,22(sp)
    } while ((x /= base) != 0);
    1166:	0ce86e63          	bltu	a6,a4,1242 <printint.constprop.0+0x126>
        buf[i--] = digits[x % base];
    116a:	02e6f5bb          	remuw	a1,a3,a4
    } while ((x /= base) != 0);
    116e:	02e6d7bb          	divuw	a5,a3,a4
        buf[i--] = digits[x % base];
    1172:	1582                	slli	a1,a1,0x20
    1174:	9181                	srli	a1,a1,0x20
    1176:	95b2                	add	a1,a1,a2
    1178:	0005c583          	lbu	a1,0(a1)
    117c:	00b10aa3          	sb	a1,21(sp)
    } while ((x /= base) != 0);
    1180:	0007859b          	sext.w	a1,a5
    1184:	12e6ec63          	bltu	a3,a4,12bc <printint.constprop.0+0x1a0>
        buf[i--] = digits[x % base];
    1188:	02e7f6bb          	remuw	a3,a5,a4
    118c:	1682                	slli	a3,a3,0x20
    118e:	9281                	srli	a3,a3,0x20
    1190:	96b2                	add	a3,a3,a2
    1192:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    1196:	02e7d83b          	divuw	a6,a5,a4
        buf[i--] = digits[x % base];
    119a:	00d10a23          	sb	a3,20(sp)
    } while ((x /= base) != 0);
    119e:	12e5e863          	bltu	a1,a4,12ce <printint.constprop.0+0x1b2>
        buf[i--] = digits[x % base];
    11a2:	02e876bb          	remuw	a3,a6,a4
    11a6:	1682                	slli	a3,a3,0x20
    11a8:	9281                	srli	a3,a3,0x20
    11aa:	96b2                	add	a3,a3,a2
    11ac:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    11b0:	02e855bb          	divuw	a1,a6,a4
        buf[i--] = digits[x % base];
    11b4:	00d109a3          	sb	a3,19(sp)
    } while ((x /= base) != 0);
    11b8:	12e86463          	bltu	a6,a4,12e0 <printint.constprop.0+0x1c4>
        buf[i--] = digits[x % base];
    11bc:	02e5f6bb          	remuw	a3,a1,a4
    11c0:	1682                	slli	a3,a3,0x20
    11c2:	9281                	srli	a3,a3,0x20
    11c4:	96b2                	add	a3,a3,a2
    11c6:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    11ca:	02e5d83b          	divuw	a6,a1,a4
        buf[i--] = digits[x % base];
    11ce:	00d10923          	sb	a3,18(sp)
    } while ((x /= base) != 0);
    11d2:	0ce5ec63          	bltu	a1,a4,12aa <printint.constprop.0+0x18e>
        buf[i--] = digits[x % base];
    11d6:	02e876bb          	remuw	a3,a6,a4
    11da:	1682                	slli	a3,a3,0x20
    11dc:	9281                	srli	a3,a3,0x20
    11de:	96b2                	add	a3,a3,a2
    11e0:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    11e4:	02e855bb          	divuw	a1,a6,a4
        buf[i--] = digits[x % base];
    11e8:	00d108a3          	sb	a3,17(sp)
    } while ((x /= base) != 0);
    11ec:	10e86963          	bltu	a6,a4,12fe <printint.constprop.0+0x1e2>
        buf[i--] = digits[x % base];
    11f0:	02e5f6bb          	remuw	a3,a1,a4
    11f4:	1682                	slli	a3,a3,0x20
    11f6:	9281                	srli	a3,a3,0x20
    11f8:	96b2                	add	a3,a3,a2
    11fa:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    11fe:	02e5d83b          	divuw	a6,a1,a4
        buf[i--] = digits[x % base];
    1202:	00d10823          	sb	a3,16(sp)
    } while ((x /= base) != 0);
    1206:	10e5e763          	bltu	a1,a4,1314 <printint.constprop.0+0x1f8>
        buf[i--] = digits[x % base];
    120a:	02e876bb          	remuw	a3,a6,a4
    120e:	1682                	slli	a3,a3,0x20
    1210:	9281                	srli	a3,a3,0x20
    1212:	96b2                	add	a3,a3,a2
    1214:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    1218:	02e857bb          	divuw	a5,a6,a4
        buf[i--] = digits[x % base];
    121c:	00d107a3          	sb	a3,15(sp)
    } while ((x /= base) != 0);
    1220:	10e86363          	bltu	a6,a4,1326 <printint.constprop.0+0x20a>
        buf[i--] = digits[x % base];
    1224:	1782                	slli	a5,a5,0x20
    1226:	9381                	srli	a5,a5,0x20
    1228:	97b2                	add	a5,a5,a2
    122a:	0007c783          	lbu	a5,0(a5)
    122e:	4599                	li	a1,6
    1230:	00f10723          	sb	a5,14(sp)

    if (sign)
    1234:	00055763          	bgez	a0,1242 <printint.constprop.0+0x126>
        buf[i--] = '-';
    1238:	02d00793          	li	a5,45
    123c:	00f106a3          	sb	a5,13(sp)
        buf[i--] = digits[x % base];
    1240:	4595                	li	a1,5
    write(f, s, l);
    1242:	003c                	addi	a5,sp,8
    1244:	4641                	li	a2,16
    1246:	9e0d                	subw	a2,a2,a1
    1248:	4505                	li	a0,1
    124a:	95be                	add	a1,a1,a5
    124c:	27f000ef          	jal	ra,1cca <write>
    i++;
    if (i < 0)
        puts("printint error");
    out(stdout, buf + i, 16 - i);
}
    1250:	70a2                	ld	ra,40(sp)
    1252:	6145                	addi	sp,sp,48
    1254:	8082                	ret
        x = -xx;
    1256:	40a0083b          	negw	a6,a0
        buf[i--] = digits[x % base];
    125a:	02b877bb          	remuw	a5,a6,a1
    125e:	00001617          	auipc	a2,0x1
    1262:	e1a60613          	addi	a2,a2,-486 # 2078 <digits>
    buf[16] = 0;
    1266:	00010c23          	sb	zero,24(sp)
        buf[i--] = digits[x % base];
    126a:	0005871b          	sext.w	a4,a1
    126e:	1782                	slli	a5,a5,0x20
    1270:	9381                	srli	a5,a5,0x20
    1272:	97b2                	add	a5,a5,a2
    1274:	0007c783          	lbu	a5,0(a5)
    } while ((x /= base) != 0);
    1278:	02b858bb          	divuw	a7,a6,a1
        buf[i--] = digits[x % base];
    127c:	00f10ba3          	sb	a5,23(sp)
    } while ((x /= base) != 0);
    1280:	06b86963          	bltu	a6,a1,12f2 <printint.constprop.0+0x1d6>
        buf[i--] = digits[x % base];
    1284:	02e8f7bb          	remuw	a5,a7,a4
    1288:	1782                	slli	a5,a5,0x20
    128a:	9381                	srli	a5,a5,0x20
    128c:	97b2                	add	a5,a5,a2
    128e:	0007c783          	lbu	a5,0(a5)
    } while ((x /= base) != 0);
    1292:	02e8d6bb          	divuw	a3,a7,a4
        buf[i--] = digits[x % base];
    1296:	00f10b23          	sb	a5,22(sp)
    } while ((x /= base) != 0);
    129a:	ece8f8e3          	bgeu	a7,a4,116a <printint.constprop.0+0x4e>
        buf[i--] = '-';
    129e:	02d00793          	li	a5,45
    12a2:	00f10aa3          	sb	a5,21(sp)
        buf[i--] = digits[x % base];
    12a6:	45b5                	li	a1,13
    12a8:	bf69                	j	1242 <printint.constprop.0+0x126>
    12aa:	45a9                	li	a1,10
    if (sign)
    12ac:	f8055be3          	bgez	a0,1242 <printint.constprop.0+0x126>
        buf[i--] = '-';
    12b0:	02d00793          	li	a5,45
    12b4:	00f108a3          	sb	a5,17(sp)
        buf[i--] = digits[x % base];
    12b8:	45a5                	li	a1,9
    12ba:	b761                	j	1242 <printint.constprop.0+0x126>
    12bc:	45b5                	li	a1,13
    if (sign)
    12be:	f80552e3          	bgez	a0,1242 <printint.constprop.0+0x126>
        buf[i--] = '-';
    12c2:	02d00793          	li	a5,45
    12c6:	00f10a23          	sb	a5,20(sp)
        buf[i--] = digits[x % base];
    12ca:	45b1                	li	a1,12
    12cc:	bf9d                	j	1242 <printint.constprop.0+0x126>
    12ce:	45b1                	li	a1,12
    if (sign)
    12d0:	f60559e3          	bgez	a0,1242 <printint.constprop.0+0x126>
        buf[i--] = '-';
    12d4:	02d00793          	li	a5,45
    12d8:	00f109a3          	sb	a5,19(sp)
        buf[i--] = digits[x % base];
    12dc:	45ad                	li	a1,11
    12de:	b795                	j	1242 <printint.constprop.0+0x126>
    12e0:	45ad                	li	a1,11
    if (sign)
    12e2:	f60550e3          	bgez	a0,1242 <printint.constprop.0+0x126>
        buf[i--] = '-';
    12e6:	02d00793          	li	a5,45
    12ea:	00f10923          	sb	a5,18(sp)
        buf[i--] = digits[x % base];
    12ee:	45a9                	li	a1,10
    12f0:	bf89                	j	1242 <printint.constprop.0+0x126>
        buf[i--] = '-';
    12f2:	02d00793          	li	a5,45
    12f6:	00f10b23          	sb	a5,22(sp)
        buf[i--] = digits[x % base];
    12fa:	45b9                	li	a1,14
    12fc:	b799                	j	1242 <printint.constprop.0+0x126>
    12fe:	45a5                	li	a1,9
    if (sign)
    1300:	f40551e3          	bgez	a0,1242 <printint.constprop.0+0x126>
        buf[i--] = '-';
    1304:	02d00793          	li	a5,45
    1308:	00f10823          	sb	a5,16(sp)
        buf[i--] = digits[x % base];
    130c:	45a1                	li	a1,8
    130e:	bf15                	j	1242 <printint.constprop.0+0x126>
    i = 15;
    1310:	45bd                	li	a1,15
    1312:	bf05                	j	1242 <printint.constprop.0+0x126>
        buf[i--] = digits[x % base];
    1314:	45a1                	li	a1,8
    if (sign)
    1316:	f20556e3          	bgez	a0,1242 <printint.constprop.0+0x126>
        buf[i--] = '-';
    131a:	02d00793          	li	a5,45
    131e:	00f107a3          	sb	a5,15(sp)
        buf[i--] = digits[x % base];
    1322:	459d                	li	a1,7
    1324:	bf39                	j	1242 <printint.constprop.0+0x126>
    1326:	459d                	li	a1,7
    if (sign)
    1328:	f0055de3          	bgez	a0,1242 <printint.constprop.0+0x126>
        buf[i--] = '-';
    132c:	02d00793          	li	a5,45
    1330:	00f10723          	sb	a5,14(sp)
        buf[i--] = digits[x % base];
    1334:	4599                	li	a1,6
    1336:	b731                	j	1242 <printint.constprop.0+0x126>

0000000000001338 <getchar>:
{
    1338:	1101                	addi	sp,sp,-32
    read(stdin, &byte, 1);
    133a:	00f10593          	addi	a1,sp,15
    133e:	4605                	li	a2,1
    1340:	4501                	li	a0,0
{
    1342:	ec06                	sd	ra,24(sp)
    char byte = 0;
    1344:	000107a3          	sb	zero,15(sp)
    read(stdin, &byte, 1);
    1348:	179000ef          	jal	ra,1cc0 <read>
}
    134c:	60e2                	ld	ra,24(sp)
    134e:	00f14503          	lbu	a0,15(sp)
    1352:	6105                	addi	sp,sp,32
    1354:	8082                	ret

0000000000001356 <putchar>:
{
    1356:	1101                	addi	sp,sp,-32
    1358:	87aa                	mv	a5,a0
    return write(stdout, &byte, 1);
    135a:	00f10593          	addi	a1,sp,15
    135e:	4605                	li	a2,1
    1360:	4505                	li	a0,1
{
    1362:	ec06                	sd	ra,24(sp)
    char byte = c;
    1364:	00f107a3          	sb	a5,15(sp)
    return write(stdout, &byte, 1);
    1368:	163000ef          	jal	ra,1cca <write>
}
    136c:	60e2                	ld	ra,24(sp)
    136e:	2501                	sext.w	a0,a0
    1370:	6105                	addi	sp,sp,32
    1372:	8082                	ret

0000000000001374 <puts>:
{
    1374:	1141                	addi	sp,sp,-16
    1376:	e406                	sd	ra,8(sp)
    1378:	e022                	sd	s0,0(sp)
    137a:	842a                	mv	s0,a0
    r = -(write(stdout, s, strlen(s)) < 0);
    137c:	56a000ef          	jal	ra,18e6 <strlen>
    1380:	862a                	mv	a2,a0
    1382:	85a2                	mv	a1,s0
    1384:	4505                	li	a0,1
    1386:	145000ef          	jal	ra,1cca <write>
}
    138a:	60a2                	ld	ra,8(sp)
    138c:	6402                	ld	s0,0(sp)
    r = -(write(stdout, s, strlen(s)) < 0);
    138e:	957d                	srai	a0,a0,0x3f
    return r;
    1390:	2501                	sext.w	a0,a0
}
    1392:	0141                	addi	sp,sp,16
    1394:	8082                	ret

0000000000001396 <printf>:
    out(stdout, buf, i);
}

// Print to the console. only understands %d, %x, %p, %s.
void printf(const char *fmt, ...)
{
    1396:	7171                	addi	sp,sp,-176
    1398:	fc56                	sd	s5,56(sp)
    139a:	ed3e                	sd	a5,152(sp)
    buf[i++] = '0';
    139c:	7ae1                	lui	s5,0xffff8
    va_list ap;
    int cnt = 0, l = 0;
    char *a, *z, *s = (char *)fmt, str;
    int f = stdout;

    va_start(ap, fmt);
    139e:	18bc                	addi	a5,sp,120
{
    13a0:	e8ca                	sd	s2,80(sp)
    13a2:	e4ce                	sd	s3,72(sp)
    13a4:	e0d2                	sd	s4,64(sp)
    13a6:	f85a                	sd	s6,48(sp)
    13a8:	f486                	sd	ra,104(sp)
    13aa:	f0a2                	sd	s0,96(sp)
    13ac:	eca6                	sd	s1,88(sp)
    13ae:	fcae                	sd	a1,120(sp)
    13b0:	e132                	sd	a2,128(sp)
    13b2:	e536                	sd	a3,136(sp)
    13b4:	e93a                	sd	a4,144(sp)
    13b6:	f142                	sd	a6,160(sp)
    13b8:	f546                	sd	a7,168(sp)
    va_start(ap, fmt);
    13ba:	e03e                	sd	a5,0(sp)
    for (;;)
    {
        if (!*s)
            break;
        for (a = s; *s && *s != '%'; s++)
    13bc:	02500913          	li	s2,37
        out(f, a, l);
        if (l)
            continue;
        if (s[1] == 0)
            break;
        switch (s[1])
    13c0:	07300a13          	li	s4,115
        case 'p':
            printptr(va_arg(ap, uint64));
            break;
        case 's':
            if ((a = va_arg(ap, char *)) == 0)
                a = "(null)";
    13c4:	00001b17          	auipc	s6,0x1
    13c8:	c04b0b13          	addi	s6,s6,-1020 # 1fc8 <__clone+0xca>
    buf[i++] = '0';
    13cc:	830aca93          	xori	s5,s5,-2000
        buf[i++] = digits[x >> (sizeof(uint64) * 8 - 4)];
    13d0:	00001997          	auipc	s3,0x1
    13d4:	ca898993          	addi	s3,s3,-856 # 2078 <digits>
        if (!*s)
    13d8:	00054783          	lbu	a5,0(a0)
    13dc:	16078a63          	beqz	a5,1550 <printf+0x1ba>
    13e0:	862a                	mv	a2,a0
        for (a = s; *s && *s != '%'; s++)
    13e2:	19278163          	beq	a5,s2,1564 <printf+0x1ce>
    13e6:	00164783          	lbu	a5,1(a2)
    13ea:	0605                	addi	a2,a2,1
    13ec:	fbfd                	bnez	a5,13e2 <printf+0x4c>
    13ee:	84b2                	mv	s1,a2
        l = z - a;
    13f0:	40a6043b          	subw	s0,a2,a0
    write(f, s, l);
    13f4:	85aa                	mv	a1,a0
    13f6:	8622                	mv	a2,s0
    13f8:	4505                	li	a0,1
    13fa:	0d1000ef          	jal	ra,1cca <write>
        if (l)
    13fe:	18041c63          	bnez	s0,1596 <printf+0x200>
        if (s[1] == 0)
    1402:	0014c783          	lbu	a5,1(s1)
    1406:	14078563          	beqz	a5,1550 <printf+0x1ba>
        switch (s[1])
    140a:	1d478063          	beq	a5,s4,15ca <printf+0x234>
    140e:	18fa6663          	bltu	s4,a5,159a <printf+0x204>
    1412:	06400713          	li	a4,100
    1416:	1ae78063          	beq	a5,a4,15b6 <printf+0x220>
    141a:	07000713          	li	a4,112
    141e:	1ce79963          	bne	a5,a4,15f0 <printf+0x25a>
            printptr(va_arg(ap, uint64));
    1422:	6702                	ld	a4,0(sp)
    buf[i++] = '0';
    1424:	01511423          	sh	s5,8(sp)
    write(f, s, l);
    1428:	4649                	li	a2,18
            printptr(va_arg(ap, uint64));
    142a:	631c                	ld	a5,0(a4)
    142c:	0721                	addi	a4,a4,8
    142e:	e03a                	sd	a4,0(sp)
    for (j = 0; j < (sizeof(uint64) * 2); j++, x <<= 4)
    1430:	00479293          	slli	t0,a5,0x4
    1434:	00879f93          	slli	t6,a5,0x8
    1438:	00c79f13          	slli	t5,a5,0xc
    143c:	01079e93          	slli	t4,a5,0x10
    1440:	01479e13          	slli	t3,a5,0x14
    1444:	01879313          	slli	t1,a5,0x18
    1448:	01c79893          	slli	a7,a5,0x1c
    144c:	02479813          	slli	a6,a5,0x24
    1450:	02879513          	slli	a0,a5,0x28
    1454:	02c79593          	slli	a1,a5,0x2c
    1458:	03079693          	slli	a3,a5,0x30
    145c:	03479713          	slli	a4,a5,0x34
        buf[i++] = digits[x >> (sizeof(uint64) * 8 - 4)];
    1460:	03c7d413          	srli	s0,a5,0x3c
    1464:	01c7d39b          	srliw	t2,a5,0x1c
    1468:	03c2d293          	srli	t0,t0,0x3c
    146c:	03cfdf93          	srli	t6,t6,0x3c
    1470:	03cf5f13          	srli	t5,t5,0x3c
    1474:	03cede93          	srli	t4,t4,0x3c
    1478:	03ce5e13          	srli	t3,t3,0x3c
    147c:	03c35313          	srli	t1,t1,0x3c
    1480:	03c8d893          	srli	a7,a7,0x3c
    1484:	03c85813          	srli	a6,a6,0x3c
    1488:	9171                	srli	a0,a0,0x3c
    148a:	91f1                	srli	a1,a1,0x3c
    148c:	92f1                	srli	a3,a3,0x3c
    148e:	9371                	srli	a4,a4,0x3c
    1490:	96ce                	add	a3,a3,s3
    1492:	974e                	add	a4,a4,s3
    1494:	944e                	add	s0,s0,s3
    1496:	92ce                	add	t0,t0,s3
    1498:	9fce                	add	t6,t6,s3
    149a:	9f4e                	add	t5,t5,s3
    149c:	9ece                	add	t4,t4,s3
    149e:	9e4e                	add	t3,t3,s3
    14a0:	934e                	add	t1,t1,s3
    14a2:	98ce                	add	a7,a7,s3
    14a4:	93ce                	add	t2,t2,s3
    14a6:	984e                	add	a6,a6,s3
    14a8:	954e                	add	a0,a0,s3
    14aa:	95ce                	add	a1,a1,s3
    14ac:	0006c083          	lbu	ra,0(a3)
    14b0:	0002c283          	lbu	t0,0(t0)
    14b4:	00074683          	lbu	a3,0(a4)
    14b8:	000fcf83          	lbu	t6,0(t6)
    14bc:	000f4f03          	lbu	t5,0(t5)
    14c0:	000ece83          	lbu	t4,0(t4)
    14c4:	000e4e03          	lbu	t3,0(t3)
    14c8:	00034303          	lbu	t1,0(t1)
    14cc:	0008c883          	lbu	a7,0(a7)
    14d0:	0003c383          	lbu	t2,0(t2)
    14d4:	00084803          	lbu	a6,0(a6)
    14d8:	00054503          	lbu	a0,0(a0)
    14dc:	0005c583          	lbu	a1,0(a1)
    14e0:	00044403          	lbu	s0,0(s0)
    for (j = 0; j < (sizeof(uint64) * 2); j++, x <<= 4)
    14e4:	03879713          	slli	a4,a5,0x38
        buf[i++] = digits[x >> (sizeof(uint64) * 8 - 4)];
    14e8:	9371                	srli	a4,a4,0x3c
    14ea:	8bbd                	andi	a5,a5,15
    14ec:	974e                	add	a4,a4,s3
    14ee:	97ce                	add	a5,a5,s3
    14f0:	005105a3          	sb	t0,11(sp)
    14f4:	01f10623          	sb	t6,12(sp)
    14f8:	01e106a3          	sb	t5,13(sp)
    14fc:	01d10723          	sb	t4,14(sp)
    1500:	01c107a3          	sb	t3,15(sp)
    1504:	00610823          	sb	t1,16(sp)
    1508:	011108a3          	sb	a7,17(sp)
    150c:	00710923          	sb	t2,18(sp)
    1510:	010109a3          	sb	a6,19(sp)
    1514:	00a10a23          	sb	a0,20(sp)
    1518:	00b10aa3          	sb	a1,21(sp)
    151c:	00110b23          	sb	ra,22(sp)
    1520:	00d10ba3          	sb	a3,23(sp)
    1524:	00810523          	sb	s0,10(sp)
    1528:	00074703          	lbu	a4,0(a4)
    152c:	0007c783          	lbu	a5,0(a5)
    write(f, s, l);
    1530:	002c                	addi	a1,sp,8
    1532:	4505                	li	a0,1
        buf[i++] = digits[x >> (sizeof(uint64) * 8 - 4)];
    1534:	00e10c23          	sb	a4,24(sp)
    1538:	00f10ca3          	sb	a5,25(sp)
    buf[i] = 0;
    153c:	00010d23          	sb	zero,26(sp)
    write(f, s, l);
    1540:	78a000ef          	jal	ra,1cca <write>
            // Print unknown % sequence to draw attention.
            putchar('%');
            putchar(s[1]);
            break;
        }
        s += 2;
    1544:	00248513          	addi	a0,s1,2
        if (!*s)
    1548:	00054783          	lbu	a5,0(a0)
    154c:	e8079ae3          	bnez	a5,13e0 <printf+0x4a>
    }
    va_end(ap);
}
    1550:	70a6                	ld	ra,104(sp)
    1552:	7406                	ld	s0,96(sp)
    1554:	64e6                	ld	s1,88(sp)
    1556:	6946                	ld	s2,80(sp)
    1558:	69a6                	ld	s3,72(sp)
    155a:	6a06                	ld	s4,64(sp)
    155c:	7ae2                	ld	s5,56(sp)
    155e:	7b42                	ld	s6,48(sp)
    1560:	614d                	addi	sp,sp,176
    1562:	8082                	ret
        for (z = s; s[0] == '%' && s[1] == '%'; z++, s += 2)
    1564:	00064783          	lbu	a5,0(a2)
    1568:	84b2                	mv	s1,a2
    156a:	01278963          	beq	a5,s2,157c <printf+0x1e6>
    156e:	b549                	j	13f0 <printf+0x5a>
    1570:	0024c783          	lbu	a5,2(s1)
    1574:	0605                	addi	a2,a2,1
    1576:	0489                	addi	s1,s1,2
    1578:	e7279ce3          	bne	a5,s2,13f0 <printf+0x5a>
    157c:	0014c783          	lbu	a5,1(s1)
    1580:	ff2788e3          	beq	a5,s2,1570 <printf+0x1da>
        l = z - a;
    1584:	40a6043b          	subw	s0,a2,a0
    write(f, s, l);
    1588:	85aa                	mv	a1,a0
    158a:	8622                	mv	a2,s0
    158c:	4505                	li	a0,1
    158e:	73c000ef          	jal	ra,1cca <write>
        if (l)
    1592:	e60408e3          	beqz	s0,1402 <printf+0x6c>
    1596:	8526                	mv	a0,s1
    1598:	b581                	j	13d8 <printf+0x42>
        switch (s[1])
    159a:	07800713          	li	a4,120
    159e:	04e79963          	bne	a5,a4,15f0 <printf+0x25a>
            printint(va_arg(ap, int), 16, 1);
    15a2:	6782                	ld	a5,0(sp)
    15a4:	45c1                	li	a1,16
    15a6:	4388                	lw	a0,0(a5)
    15a8:	07a1                	addi	a5,a5,8
    15aa:	e03e                	sd	a5,0(sp)
    15ac:	b71ff0ef          	jal	ra,111c <printint.constprop.0>
        s += 2;
    15b0:	00248513          	addi	a0,s1,2
    15b4:	bf51                	j	1548 <printf+0x1b2>
            printint(va_arg(ap, int), 10, 1);
    15b6:	6782                	ld	a5,0(sp)
    15b8:	45a9                	li	a1,10
    15ba:	4388                	lw	a0,0(a5)
    15bc:	07a1                	addi	a5,a5,8
    15be:	e03e                	sd	a5,0(sp)
    15c0:	b5dff0ef          	jal	ra,111c <printint.constprop.0>
        s += 2;
    15c4:	00248513          	addi	a0,s1,2
    15c8:	b741                	j	1548 <printf+0x1b2>
            if ((a = va_arg(ap, char *)) == 0)
    15ca:	6782                	ld	a5,0(sp)
    15cc:	6380                	ld	s0,0(a5)
    15ce:	07a1                	addi	a5,a5,8
    15d0:	e03e                	sd	a5,0(sp)
    15d2:	c031                	beqz	s0,1616 <printf+0x280>
            l = strnlen(a, 200);
    15d4:	0c800593          	li	a1,200
    15d8:	8522                	mv	a0,s0
    15da:	3f8000ef          	jal	ra,19d2 <strnlen>
    write(f, s, l);
    15de:	0005061b          	sext.w	a2,a0
    15e2:	85a2                	mv	a1,s0
    15e4:	4505                	li	a0,1
    15e6:	6e4000ef          	jal	ra,1cca <write>
        s += 2;
    15ea:	00248513          	addi	a0,s1,2
    15ee:	bfa9                	j	1548 <printf+0x1b2>
    return write(stdout, &byte, 1);
    15f0:	4605                	li	a2,1
    15f2:	002c                	addi	a1,sp,8
    15f4:	4505                	li	a0,1
    char byte = c;
    15f6:	01210423          	sb	s2,8(sp)
    return write(stdout, &byte, 1);
    15fa:	6d0000ef          	jal	ra,1cca <write>
    char byte = c;
    15fe:	0014c783          	lbu	a5,1(s1)
    return write(stdout, &byte, 1);
    1602:	4605                	li	a2,1
    1604:	002c                	addi	a1,sp,8
    1606:	4505                	li	a0,1
    char byte = c;
    1608:	00f10423          	sb	a5,8(sp)
    return write(stdout, &byte, 1);
    160c:	6be000ef          	jal	ra,1cca <write>
        s += 2;
    1610:	00248513          	addi	a0,s1,2
    1614:	bf15                	j	1548 <printf+0x1b2>
                a = "(null)";
    1616:	845a                	mv	s0,s6
    1618:	bf75                	j	15d4 <printf+0x23e>

000000000000161a <isspace>:
#define HIGHS (ONES * (UCHAR_MAX / 2 + 1))
#define HASZERO(x) (((x)-ONES) & ~(x)&HIGHS)

int isspace(int c)
{
    return c == ' ' || (unsigned)c - '\t' < 5;
    161a:	02000793          	li	a5,32
    161e:	00f50663          	beq	a0,a5,162a <isspace+0x10>
    1622:	355d                	addiw	a0,a0,-9
    1624:	00553513          	sltiu	a0,a0,5
    1628:	8082                	ret
    162a:	4505                	li	a0,1
}
    162c:	8082                	ret

000000000000162e <isdigit>:

int isdigit(int c)
{
    return (unsigned)c - '0' < 10;
    162e:	fd05051b          	addiw	a0,a0,-48
}
    1632:	00a53513          	sltiu	a0,a0,10
    1636:	8082                	ret

0000000000001638 <atoi>:
    return c == ' ' || (unsigned)c - '\t' < 5;
    1638:	02000613          	li	a2,32
    163c:	4591                	li	a1,4

int atoi(const char *s)
{
    int n = 0, neg = 0;
    while (isspace(*s))
    163e:	00054703          	lbu	a4,0(a0)
    return c == ' ' || (unsigned)c - '\t' < 5;
    1642:	ff77069b          	addiw	a3,a4,-9
    1646:	04c70d63          	beq	a4,a2,16a0 <atoi+0x68>
    164a:	0007079b          	sext.w	a5,a4
    164e:	04d5f963          	bgeu	a1,a3,16a0 <atoi+0x68>
        s++;
    switch (*s)
    1652:	02b00693          	li	a3,43
    1656:	04d70a63          	beq	a4,a3,16aa <atoi+0x72>
    165a:	02d00693          	li	a3,45
    165e:	06d70463          	beq	a4,a3,16c6 <atoi+0x8e>
        neg = 1;
    case '+':
        s++;
    }
    /* Compute n as a negative number to avoid overflow on INT_MIN */
    while (isdigit(*s))
    1662:	fd07859b          	addiw	a1,a5,-48
    1666:	4625                	li	a2,9
    1668:	873e                	mv	a4,a5
    166a:	86aa                	mv	a3,a0
    int n = 0, neg = 0;
    166c:	4e01                	li	t3,0
    while (isdigit(*s))
    166e:	04b66a63          	bltu	a2,a1,16c2 <atoi+0x8a>
    int n = 0, neg = 0;
    1672:	4501                	li	a0,0
    while (isdigit(*s))
    1674:	4825                	li	a6,9
    1676:	0016c603          	lbu	a2,1(a3)
        n = 10 * n - (*s++ - '0');
    167a:	0025179b          	slliw	a5,a0,0x2
    167e:	9d3d                	addw	a0,a0,a5
    1680:	fd07031b          	addiw	t1,a4,-48
    1684:	0015189b          	slliw	a7,a0,0x1
    while (isdigit(*s))
    1688:	fd06059b          	addiw	a1,a2,-48
        n = 10 * n - (*s++ - '0');
    168c:	0685                	addi	a3,a3,1
    168e:	4068853b          	subw	a0,a7,t1
    while (isdigit(*s))
    1692:	0006071b          	sext.w	a4,a2
    1696:	feb870e3          	bgeu	a6,a1,1676 <atoi+0x3e>
    return neg ? n : -n;
    169a:	000e0563          	beqz	t3,16a4 <atoi+0x6c>
}
    169e:	8082                	ret
        s++;
    16a0:	0505                	addi	a0,a0,1
    16a2:	bf71                	j	163e <atoi+0x6>
    return neg ? n : -n;
    16a4:	4113053b          	subw	a0,t1,a7
    16a8:	8082                	ret
    while (isdigit(*s))
    16aa:	00154783          	lbu	a5,1(a0)
    16ae:	4625                	li	a2,9
        s++;
    16b0:	00150693          	addi	a3,a0,1
    while (isdigit(*s))
    16b4:	fd07859b          	addiw	a1,a5,-48
    16b8:	0007871b          	sext.w	a4,a5
    int n = 0, neg = 0;
    16bc:	4e01                	li	t3,0
    while (isdigit(*s))
    16be:	fab67ae3          	bgeu	a2,a1,1672 <atoi+0x3a>
    16c2:	4501                	li	a0,0
}
    16c4:	8082                	ret
    while (isdigit(*s))
    16c6:	00154783          	lbu	a5,1(a0)
    16ca:	4625                	li	a2,9
        s++;
    16cc:	00150693          	addi	a3,a0,1
    while (isdigit(*s))
    16d0:	fd07859b          	addiw	a1,a5,-48
    16d4:	0007871b          	sext.w	a4,a5
    16d8:	feb665e3          	bltu	a2,a1,16c2 <atoi+0x8a>
        neg = 1;
    16dc:	4e05                	li	t3,1
    16de:	bf51                	j	1672 <atoi+0x3a>

00000000000016e0 <memset>:

void *memset(void *dest, int c, size_t n)
{
    char *p = dest;
    for (int i = 0; i < n; ++i, *(p++) = c)
    16e0:	16060d63          	beqz	a2,185a <memset+0x17a>
    16e4:	40a007b3          	neg	a5,a0
    16e8:	8b9d                	andi	a5,a5,7
    16ea:	00778713          	addi	a4,a5,7
    16ee:	482d                	li	a6,11
    16f0:	0ff5f593          	andi	a1,a1,255
    16f4:	fff60693          	addi	a3,a2,-1
    16f8:	17076263          	bltu	a4,a6,185c <memset+0x17c>
    16fc:	16e6ea63          	bltu	a3,a4,1870 <memset+0x190>
    1700:	16078563          	beqz	a5,186a <memset+0x18a>
    1704:	00b50023          	sb	a1,0(a0)
    1708:	4705                	li	a4,1
    170a:	00150e93          	addi	t4,a0,1
    170e:	14e78c63          	beq	a5,a4,1866 <memset+0x186>
    1712:	00b500a3          	sb	a1,1(a0)
    1716:	4709                	li	a4,2
    1718:	00250e93          	addi	t4,a0,2
    171c:	14e78d63          	beq	a5,a4,1876 <memset+0x196>
    1720:	00b50123          	sb	a1,2(a0)
    1724:	470d                	li	a4,3
    1726:	00350e93          	addi	t4,a0,3
    172a:	12e78b63          	beq	a5,a4,1860 <memset+0x180>
    172e:	00b501a3          	sb	a1,3(a0)
    1732:	4711                	li	a4,4
    1734:	00450e93          	addi	t4,a0,4
    1738:	14e78163          	beq	a5,a4,187a <memset+0x19a>
    173c:	00b50223          	sb	a1,4(a0)
    1740:	4715                	li	a4,5
    1742:	00550e93          	addi	t4,a0,5
    1746:	12e78c63          	beq	a5,a4,187e <memset+0x19e>
    174a:	00b502a3          	sb	a1,5(a0)
    174e:	471d                	li	a4,7
    1750:	00650e93          	addi	t4,a0,6
    1754:	12e79763          	bne	a5,a4,1882 <memset+0x1a2>
    1758:	00750e93          	addi	t4,a0,7
    175c:	00b50323          	sb	a1,6(a0)
    1760:	4f1d                	li	t5,7
    1762:	00859713          	slli	a4,a1,0x8
    1766:	8f4d                	or	a4,a4,a1
    1768:	01059e13          	slli	t3,a1,0x10
    176c:	01c76e33          	or	t3,a4,t3
    1770:	01859313          	slli	t1,a1,0x18
    1774:	006e6333          	or	t1,t3,t1
    1778:	02059893          	slli	a7,a1,0x20
    177c:	011368b3          	or	a7,t1,a7
    1780:	02859813          	slli	a6,a1,0x28
    1784:	40f60333          	sub	t1,a2,a5
    1788:	0108e833          	or	a6,a7,a6
    178c:	03059693          	slli	a3,a1,0x30
    1790:	00d866b3          	or	a3,a6,a3
    1794:	03859713          	slli	a4,a1,0x38
    1798:	97aa                	add	a5,a5,a0
    179a:	ff837813          	andi	a6,t1,-8
    179e:	8f55                	or	a4,a4,a3
    17a0:	00f806b3          	add	a3,a6,a5
    17a4:	e398                	sd	a4,0(a5)
    17a6:	07a1                	addi	a5,a5,8
    17a8:	fed79ee3          	bne	a5,a3,17a4 <memset+0xc4>
    17ac:	ff837693          	andi	a3,t1,-8
    17b0:	00de87b3          	add	a5,t4,a3
    17b4:	01e6873b          	addw	a4,a3,t5
    17b8:	0ad30663          	beq	t1,a3,1864 <memset+0x184>
    17bc:	00b78023          	sb	a1,0(a5)
    17c0:	0017069b          	addiw	a3,a4,1
    17c4:	08c6fb63          	bgeu	a3,a2,185a <memset+0x17a>
    17c8:	00b780a3          	sb	a1,1(a5)
    17cc:	0027069b          	addiw	a3,a4,2
    17d0:	08c6f563          	bgeu	a3,a2,185a <memset+0x17a>
    17d4:	00b78123          	sb	a1,2(a5)
    17d8:	0037069b          	addiw	a3,a4,3
    17dc:	06c6ff63          	bgeu	a3,a2,185a <memset+0x17a>
    17e0:	00b781a3          	sb	a1,3(a5)
    17e4:	0047069b          	addiw	a3,a4,4
    17e8:	06c6f963          	bgeu	a3,a2,185a <memset+0x17a>
    17ec:	00b78223          	sb	a1,4(a5)
    17f0:	0057069b          	addiw	a3,a4,5
    17f4:	06c6f363          	bgeu	a3,a2,185a <memset+0x17a>
    17f8:	00b782a3          	sb	a1,5(a5)
    17fc:	0067069b          	addiw	a3,a4,6
    1800:	04c6fd63          	bgeu	a3,a2,185a <memset+0x17a>
    1804:	00b78323          	sb	a1,6(a5)
    1808:	0077069b          	addiw	a3,a4,7
    180c:	04c6f763          	bgeu	a3,a2,185a <memset+0x17a>
    1810:	00b783a3          	sb	a1,7(a5)
    1814:	0087069b          	addiw	a3,a4,8
    1818:	04c6f163          	bgeu	a3,a2,185a <memset+0x17a>
    181c:	00b78423          	sb	a1,8(a5)
    1820:	0097069b          	addiw	a3,a4,9
    1824:	02c6fb63          	bgeu	a3,a2,185a <memset+0x17a>
    1828:	00b784a3          	sb	a1,9(a5)
    182c:	00a7069b          	addiw	a3,a4,10
    1830:	02c6f563          	bgeu	a3,a2,185a <memset+0x17a>
    1834:	00b78523          	sb	a1,10(a5)
    1838:	00b7069b          	addiw	a3,a4,11
    183c:	00c6ff63          	bgeu	a3,a2,185a <memset+0x17a>
    1840:	00b785a3          	sb	a1,11(a5)
    1844:	00c7069b          	addiw	a3,a4,12
    1848:	00c6f963          	bgeu	a3,a2,185a <memset+0x17a>
    184c:	00b78623          	sb	a1,12(a5)
    1850:	2735                	addiw	a4,a4,13
    1852:	00c77463          	bgeu	a4,a2,185a <memset+0x17a>
    1856:	00b786a3          	sb	a1,13(a5)
        ;
    return dest;
}
    185a:	8082                	ret
    185c:	472d                	li	a4,11
    185e:	bd79                	j	16fc <memset+0x1c>
    for (int i = 0; i < n; ++i, *(p++) = c)
    1860:	4f0d                	li	t5,3
    1862:	b701                	j	1762 <memset+0x82>
    1864:	8082                	ret
    1866:	4f05                	li	t5,1
    1868:	bded                	j	1762 <memset+0x82>
    186a:	8eaa                	mv	t4,a0
    186c:	4f01                	li	t5,0
    186e:	bdd5                	j	1762 <memset+0x82>
    1870:	87aa                	mv	a5,a0
    1872:	4701                	li	a4,0
    1874:	b7a1                	j	17bc <memset+0xdc>
    1876:	4f09                	li	t5,2
    1878:	b5ed                	j	1762 <memset+0x82>
    187a:	4f11                	li	t5,4
    187c:	b5dd                	j	1762 <memset+0x82>
    187e:	4f15                	li	t5,5
    1880:	b5cd                	j	1762 <memset+0x82>
    1882:	4f19                	li	t5,6
    1884:	bdf9                	j	1762 <memset+0x82>

0000000000001886 <strcmp>:

int strcmp(const char *l, const char *r)
{
    for (; *l == *r && *l; l++, r++)
    1886:	00054783          	lbu	a5,0(a0)
    188a:	0005c703          	lbu	a4,0(a1)
    188e:	00e79863          	bne	a5,a4,189e <strcmp+0x18>
    1892:	0505                	addi	a0,a0,1
    1894:	0585                	addi	a1,a1,1
    1896:	fbe5                	bnez	a5,1886 <strcmp>
    1898:	4501                	li	a0,0
        ;
    return *(unsigned char *)l - *(unsigned char *)r;
}
    189a:	9d19                	subw	a0,a0,a4
    189c:	8082                	ret
    return *(unsigned char *)l - *(unsigned char *)r;
    189e:	0007851b          	sext.w	a0,a5
    18a2:	bfe5                	j	189a <strcmp+0x14>

00000000000018a4 <strncmp>:

int strncmp(const char *_l, const char *_r, size_t n)
{
    const unsigned char *l = (void *)_l, *r = (void *)_r;
    if (!n--)
    18a4:	ce05                	beqz	a2,18dc <strncmp+0x38>
        return 0;
    for (; *l && *r && n && *l == *r; l++, r++, n--)
    18a6:	00054703          	lbu	a4,0(a0)
    18aa:	0005c783          	lbu	a5,0(a1)
    18ae:	cb0d                	beqz	a4,18e0 <strncmp+0x3c>
    if (!n--)
    18b0:	167d                	addi	a2,a2,-1
    18b2:	00c506b3          	add	a3,a0,a2
    18b6:	a819                	j	18cc <strncmp+0x28>
    for (; *l && *r && n && *l == *r; l++, r++, n--)
    18b8:	00a68e63          	beq	a3,a0,18d4 <strncmp+0x30>
    18bc:	0505                	addi	a0,a0,1
    18be:	00e79b63          	bne	a5,a4,18d4 <strncmp+0x30>
    18c2:	00054703          	lbu	a4,0(a0)
        ;
    return *l - *r;
    18c6:	0005c783          	lbu	a5,0(a1)
    for (; *l && *r && n && *l == *r; l++, r++, n--)
    18ca:	cb19                	beqz	a4,18e0 <strncmp+0x3c>
    18cc:	0005c783          	lbu	a5,0(a1)
    18d0:	0585                	addi	a1,a1,1
    18d2:	f3fd                	bnez	a5,18b8 <strncmp+0x14>
    return *l - *r;
    18d4:	0007051b          	sext.w	a0,a4
    18d8:	9d1d                	subw	a0,a0,a5
    18da:	8082                	ret
        return 0;
    18dc:	4501                	li	a0,0
}
    18de:	8082                	ret
    18e0:	4501                	li	a0,0
    return *l - *r;
    18e2:	9d1d                	subw	a0,a0,a5
    18e4:	8082                	ret

00000000000018e6 <strlen>:
size_t strlen(const char *s)
{
    const char *a = s;
    typedef size_t __attribute__((__may_alias__)) word;
    const word *w;
    for (; (uintptr_t)s % SS; s++)
    18e6:	00757793          	andi	a5,a0,7
    18ea:	cf89                	beqz	a5,1904 <strlen+0x1e>
    18ec:	87aa                	mv	a5,a0
    18ee:	a029                	j	18f8 <strlen+0x12>
    18f0:	0785                	addi	a5,a5,1
    18f2:	0077f713          	andi	a4,a5,7
    18f6:	cb01                	beqz	a4,1906 <strlen+0x20>
        if (!*s)
    18f8:	0007c703          	lbu	a4,0(a5)
    18fc:	fb75                	bnez	a4,18f0 <strlen+0xa>
    for (w = (const void *)s; !HASZERO(*w); w++)
        ;
    s = (const void *)w;
    for (; *s; s++)
        ;
    return s - a;
    18fe:	40a78533          	sub	a0,a5,a0
}
    1902:	8082                	ret
    for (; (uintptr_t)s % SS; s++)
    1904:	87aa                	mv	a5,a0
    for (w = (const void *)s; !HASZERO(*w); w++)
    1906:	6394                	ld	a3,0(a5)
    1908:	00000597          	auipc	a1,0x0
    190c:	6c85b583          	ld	a1,1736(a1) # 1fd0 <__clone+0xd2>
    1910:	00000617          	auipc	a2,0x0
    1914:	6c863603          	ld	a2,1736(a2) # 1fd8 <__clone+0xda>
    1918:	a019                	j	191e <strlen+0x38>
    191a:	6794                	ld	a3,8(a5)
    191c:	07a1                	addi	a5,a5,8
    191e:	00b68733          	add	a4,a3,a1
    1922:	fff6c693          	not	a3,a3
    1926:	8f75                	and	a4,a4,a3
    1928:	8f71                	and	a4,a4,a2
    192a:	db65                	beqz	a4,191a <strlen+0x34>
    for (; *s; s++)
    192c:	0007c703          	lbu	a4,0(a5)
    1930:	d779                	beqz	a4,18fe <strlen+0x18>
    1932:	0017c703          	lbu	a4,1(a5)
    1936:	0785                	addi	a5,a5,1
    1938:	d379                	beqz	a4,18fe <strlen+0x18>
    193a:	0017c703          	lbu	a4,1(a5)
    193e:	0785                	addi	a5,a5,1
    1940:	fb6d                	bnez	a4,1932 <strlen+0x4c>
    1942:	bf75                	j	18fe <strlen+0x18>

0000000000001944 <memchr>:

void *memchr(const void *src, int c, size_t n)
{
    const unsigned char *s = src;
    c = (unsigned char)c;
    for (; ((uintptr_t)s & ALIGN) && n && *s != c; s++, n--)
    1944:	00757713          	andi	a4,a0,7
{
    1948:	87aa                	mv	a5,a0
    c = (unsigned char)c;
    194a:	0ff5f593          	andi	a1,a1,255
    for (; ((uintptr_t)s & ALIGN) && n && *s != c; s++, n--)
    194e:	cb19                	beqz	a4,1964 <memchr+0x20>
    1950:	ce25                	beqz	a2,19c8 <memchr+0x84>
    1952:	0007c703          	lbu	a4,0(a5)
    1956:	04b70e63          	beq	a4,a1,19b2 <memchr+0x6e>
    195a:	0785                	addi	a5,a5,1
    195c:	0077f713          	andi	a4,a5,7
    1960:	167d                	addi	a2,a2,-1
    1962:	f77d                	bnez	a4,1950 <memchr+0xc>
            ;
        s = (const void *)w;
    }
    for (; n && *s != c; s++, n--)
        ;
    return n ? (void *)s : 0;
    1964:	4501                	li	a0,0
    if (n && *s != c)
    1966:	c235                	beqz	a2,19ca <memchr+0x86>
    1968:	0007c703          	lbu	a4,0(a5)
    196c:	04b70363          	beq	a4,a1,19b2 <memchr+0x6e>
        size_t k = ONES * c;
    1970:	00000517          	auipc	a0,0x0
    1974:	67053503          	ld	a0,1648(a0) # 1fe0 <__clone+0xe2>
        for (w = (const void *)s; n >= SS && !HASZERO(*w ^ k); w++, n -= SS)
    1978:	471d                	li	a4,7
        size_t k = ONES * c;
    197a:	02a58533          	mul	a0,a1,a0
        for (w = (const void *)s; n >= SS && !HASZERO(*w ^ k); w++, n -= SS)
    197e:	02c77a63          	bgeu	a4,a2,19b2 <memchr+0x6e>
    1982:	00000897          	auipc	a7,0x0
    1986:	64e8b883          	ld	a7,1614(a7) # 1fd0 <__clone+0xd2>
    198a:	00000817          	auipc	a6,0x0
    198e:	64e83803          	ld	a6,1614(a6) # 1fd8 <__clone+0xda>
    1992:	431d                	li	t1,7
    1994:	a029                	j	199e <memchr+0x5a>
    1996:	1661                	addi	a2,a2,-8
    1998:	07a1                	addi	a5,a5,8
    199a:	02c37963          	bgeu	t1,a2,19cc <memchr+0x88>
    199e:	6398                	ld	a4,0(a5)
    19a0:	8f29                	xor	a4,a4,a0
    19a2:	011706b3          	add	a3,a4,a7
    19a6:	fff74713          	not	a4,a4
    19aa:	8f75                	and	a4,a4,a3
    19ac:	01077733          	and	a4,a4,a6
    19b0:	d37d                	beqz	a4,1996 <memchr+0x52>
    19b2:	853e                	mv	a0,a5
    19b4:	97b2                	add	a5,a5,a2
    19b6:	a021                	j	19be <memchr+0x7a>
    for (; n && *s != c; s++, n--)
    19b8:	0505                	addi	a0,a0,1
    19ba:	00f50763          	beq	a0,a5,19c8 <memchr+0x84>
    19be:	00054703          	lbu	a4,0(a0)
    19c2:	feb71be3          	bne	a4,a1,19b8 <memchr+0x74>
    19c6:	8082                	ret
    return n ? (void *)s : 0;
    19c8:	4501                	li	a0,0
}
    19ca:	8082                	ret
    return n ? (void *)s : 0;
    19cc:	4501                	li	a0,0
    for (; n && *s != c; s++, n--)
    19ce:	f275                	bnez	a2,19b2 <memchr+0x6e>
}
    19d0:	8082                	ret

00000000000019d2 <strnlen>:

size_t strnlen(const char *s, size_t n)
{
    19d2:	1101                	addi	sp,sp,-32
    19d4:	e822                	sd	s0,16(sp)
    const char *p = memchr(s, 0, n);
    19d6:	862e                	mv	a2,a1
{
    19d8:	842e                	mv	s0,a1
    const char *p = memchr(s, 0, n);
    19da:	4581                	li	a1,0
{
    19dc:	e426                	sd	s1,8(sp)
    19de:	ec06                	sd	ra,24(sp)
    19e0:	84aa                	mv	s1,a0
    const char *p = memchr(s, 0, n);
    19e2:	f63ff0ef          	jal	ra,1944 <memchr>
    return p ? p - s : n;
    19e6:	c519                	beqz	a0,19f4 <strnlen+0x22>
}
    19e8:	60e2                	ld	ra,24(sp)
    19ea:	6442                	ld	s0,16(sp)
    return p ? p - s : n;
    19ec:	8d05                	sub	a0,a0,s1
}
    19ee:	64a2                	ld	s1,8(sp)
    19f0:	6105                	addi	sp,sp,32
    19f2:	8082                	ret
    19f4:	60e2                	ld	ra,24(sp)
    return p ? p - s : n;
    19f6:	8522                	mv	a0,s0
}
    19f8:	6442                	ld	s0,16(sp)
    19fa:	64a2                	ld	s1,8(sp)
    19fc:	6105                	addi	sp,sp,32
    19fe:	8082                	ret

0000000000001a00 <strcpy>:
char *strcpy(char *restrict d, const char *s)
{
    typedef size_t __attribute__((__may_alias__)) word;
    word *wd;
    const word *ws;
    if ((uintptr_t)s % SS == (uintptr_t)d % SS)
    1a00:	00b547b3          	xor	a5,a0,a1
    1a04:	8b9d                	andi	a5,a5,7
    1a06:	eb95                	bnez	a5,1a3a <strcpy+0x3a>
    {
        for (; (uintptr_t)s % SS; s++, d++)
    1a08:	0075f793          	andi	a5,a1,7
    1a0c:	e7b1                	bnez	a5,1a58 <strcpy+0x58>
            if (!(*d = *s))
                return d;
        wd = (void *)d;
        ws = (const void *)s;
        for (; !HASZERO(*ws); *wd++ = *ws++)
    1a0e:	6198                	ld	a4,0(a1)
    1a10:	00000617          	auipc	a2,0x0
    1a14:	5c063603          	ld	a2,1472(a2) # 1fd0 <__clone+0xd2>
    1a18:	00000817          	auipc	a6,0x0
    1a1c:	5c083803          	ld	a6,1472(a6) # 1fd8 <__clone+0xda>
    1a20:	a029                	j	1a2a <strcpy+0x2a>
    1a22:	e118                	sd	a4,0(a0)
    1a24:	6598                	ld	a4,8(a1)
    1a26:	05a1                	addi	a1,a1,8
    1a28:	0521                	addi	a0,a0,8
    1a2a:	00c707b3          	add	a5,a4,a2
    1a2e:	fff74693          	not	a3,a4
    1a32:	8ff5                	and	a5,a5,a3
    1a34:	0107f7b3          	and	a5,a5,a6
    1a38:	d7ed                	beqz	a5,1a22 <strcpy+0x22>
            ;
        d = (void *)wd;
        s = (const void *)ws;
    }
    for (; (*d = *s); s++, d++)
    1a3a:	0005c783          	lbu	a5,0(a1)
    1a3e:	00f50023          	sb	a5,0(a0)
    1a42:	c785                	beqz	a5,1a6a <strcpy+0x6a>
    1a44:	0015c783          	lbu	a5,1(a1)
    1a48:	0505                	addi	a0,a0,1
    1a4a:	0585                	addi	a1,a1,1
    1a4c:	00f50023          	sb	a5,0(a0)
    1a50:	fbf5                	bnez	a5,1a44 <strcpy+0x44>
        ;
    return d;
}
    1a52:	8082                	ret
        for (; (uintptr_t)s % SS; s++, d++)
    1a54:	0505                	addi	a0,a0,1
    1a56:	df45                	beqz	a4,1a0e <strcpy+0xe>
            if (!(*d = *s))
    1a58:	0005c783          	lbu	a5,0(a1)
        for (; (uintptr_t)s % SS; s++, d++)
    1a5c:	0585                	addi	a1,a1,1
    1a5e:	0075f713          	andi	a4,a1,7
            if (!(*d = *s))
    1a62:	00f50023          	sb	a5,0(a0)
    1a66:	f7fd                	bnez	a5,1a54 <strcpy+0x54>
}
    1a68:	8082                	ret
    1a6a:	8082                	ret

0000000000001a6c <strncpy>:
char *strncpy(char *restrict d, const char *s, size_t n)
{
    typedef size_t __attribute__((__may_alias__)) word;
    word *wd;
    const word *ws;
    if (((uintptr_t)s & ALIGN) == ((uintptr_t)d & ALIGN))
    1a6c:	00b547b3          	xor	a5,a0,a1
    1a70:	8b9d                	andi	a5,a5,7
    1a72:	1a079863          	bnez	a5,1c22 <strncpy+0x1b6>
    {
        for (; ((uintptr_t)s & ALIGN) && n && (*d = *s); n--, s++, d++)
    1a76:	0075f793          	andi	a5,a1,7
    1a7a:	16078463          	beqz	a5,1be2 <strncpy+0x176>
    1a7e:	ea01                	bnez	a2,1a8e <strncpy+0x22>
    1a80:	a421                	j	1c88 <strncpy+0x21c>
    1a82:	167d                	addi	a2,a2,-1
    1a84:	0505                	addi	a0,a0,1
    1a86:	14070e63          	beqz	a4,1be2 <strncpy+0x176>
    1a8a:	1a060863          	beqz	a2,1c3a <strncpy+0x1ce>
    1a8e:	0005c783          	lbu	a5,0(a1)
    1a92:	0585                	addi	a1,a1,1
    1a94:	0075f713          	andi	a4,a1,7
    1a98:	00f50023          	sb	a5,0(a0)
    1a9c:	f3fd                	bnez	a5,1a82 <strncpy+0x16>
    1a9e:	4805                	li	a6,1
    1aa0:	1a061863          	bnez	a2,1c50 <strncpy+0x1e4>
    1aa4:	40a007b3          	neg	a5,a0
    1aa8:	8b9d                	andi	a5,a5,7
    1aaa:	4681                	li	a3,0
    1aac:	18061a63          	bnez	a2,1c40 <strncpy+0x1d4>
    1ab0:	00778713          	addi	a4,a5,7
    1ab4:	45ad                	li	a1,11
    1ab6:	18b76363          	bltu	a4,a1,1c3c <strncpy+0x1d0>
    1aba:	1ae6eb63          	bltu	a3,a4,1c70 <strncpy+0x204>
    1abe:	1a078363          	beqz	a5,1c64 <strncpy+0x1f8>
    for (int i = 0; i < n; ++i, *(p++) = c)
    1ac2:	00050023          	sb	zero,0(a0)
    1ac6:	4685                	li	a3,1
    1ac8:	00150713          	addi	a4,a0,1
    1acc:	18d78f63          	beq	a5,a3,1c6a <strncpy+0x1fe>
    1ad0:	000500a3          	sb	zero,1(a0)
    1ad4:	4689                	li	a3,2
    1ad6:	00250713          	addi	a4,a0,2
    1ada:	18d78e63          	beq	a5,a3,1c76 <strncpy+0x20a>
    1ade:	00050123          	sb	zero,2(a0)
    1ae2:	468d                	li	a3,3
    1ae4:	00350713          	addi	a4,a0,3
    1ae8:	16d78c63          	beq	a5,a3,1c60 <strncpy+0x1f4>
    1aec:	000501a3          	sb	zero,3(a0)
    1af0:	4691                	li	a3,4
    1af2:	00450713          	addi	a4,a0,4
    1af6:	18d78263          	beq	a5,a3,1c7a <strncpy+0x20e>
    1afa:	00050223          	sb	zero,4(a0)
    1afe:	4695                	li	a3,5
    1b00:	00550713          	addi	a4,a0,5
    1b04:	16d78d63          	beq	a5,a3,1c7e <strncpy+0x212>
    1b08:	000502a3          	sb	zero,5(a0)
    1b0c:	469d                	li	a3,7
    1b0e:	00650713          	addi	a4,a0,6
    1b12:	16d79863          	bne	a5,a3,1c82 <strncpy+0x216>
    1b16:	00750713          	addi	a4,a0,7
    1b1a:	00050323          	sb	zero,6(a0)
    1b1e:	40f80833          	sub	a6,a6,a5
    1b22:	ff887593          	andi	a1,a6,-8
    1b26:	97aa                	add	a5,a5,a0
    1b28:	95be                	add	a1,a1,a5
    1b2a:	0007b023          	sd	zero,0(a5)
    1b2e:	07a1                	addi	a5,a5,8
    1b30:	feb79de3          	bne	a5,a1,1b2a <strncpy+0xbe>
    1b34:	ff887593          	andi	a1,a6,-8
    1b38:	9ead                	addw	a3,a3,a1
    1b3a:	00b707b3          	add	a5,a4,a1
    1b3e:	12b80863          	beq	a6,a1,1c6e <strncpy+0x202>
    1b42:	00078023          	sb	zero,0(a5)
    1b46:	0016871b          	addiw	a4,a3,1
    1b4a:	0ec77863          	bgeu	a4,a2,1c3a <strncpy+0x1ce>
    1b4e:	000780a3          	sb	zero,1(a5)
    1b52:	0026871b          	addiw	a4,a3,2
    1b56:	0ec77263          	bgeu	a4,a2,1c3a <strncpy+0x1ce>
    1b5a:	00078123          	sb	zero,2(a5)
    1b5e:	0036871b          	addiw	a4,a3,3
    1b62:	0cc77c63          	bgeu	a4,a2,1c3a <strncpy+0x1ce>
    1b66:	000781a3          	sb	zero,3(a5)
    1b6a:	0046871b          	addiw	a4,a3,4
    1b6e:	0cc77663          	bgeu	a4,a2,1c3a <strncpy+0x1ce>
    1b72:	00078223          	sb	zero,4(a5)
    1b76:	0056871b          	addiw	a4,a3,5
    1b7a:	0cc77063          	bgeu	a4,a2,1c3a <strncpy+0x1ce>
    1b7e:	000782a3          	sb	zero,5(a5)
    1b82:	0066871b          	addiw	a4,a3,6
    1b86:	0ac77a63          	bgeu	a4,a2,1c3a <strncpy+0x1ce>
    1b8a:	00078323          	sb	zero,6(a5)
    1b8e:	0076871b          	addiw	a4,a3,7
    1b92:	0ac77463          	bgeu	a4,a2,1c3a <strncpy+0x1ce>
    1b96:	000783a3          	sb	zero,7(a5)
    1b9a:	0086871b          	addiw	a4,a3,8
    1b9e:	08c77e63          	bgeu	a4,a2,1c3a <strncpy+0x1ce>
    1ba2:	00078423          	sb	zero,8(a5)
    1ba6:	0096871b          	addiw	a4,a3,9
    1baa:	08c77863          	bgeu	a4,a2,1c3a <strncpy+0x1ce>
    1bae:	000784a3          	sb	zero,9(a5)
    1bb2:	00a6871b          	addiw	a4,a3,10
    1bb6:	08c77263          	bgeu	a4,a2,1c3a <strncpy+0x1ce>
    1bba:	00078523          	sb	zero,10(a5)
    1bbe:	00b6871b          	addiw	a4,a3,11
    1bc2:	06c77c63          	bgeu	a4,a2,1c3a <strncpy+0x1ce>
    1bc6:	000785a3          	sb	zero,11(a5)
    1bca:	00c6871b          	addiw	a4,a3,12
    1bce:	06c77663          	bgeu	a4,a2,1c3a <strncpy+0x1ce>
    1bd2:	00078623          	sb	zero,12(a5)
    1bd6:	26b5                	addiw	a3,a3,13
    1bd8:	06c6f163          	bgeu	a3,a2,1c3a <strncpy+0x1ce>
    1bdc:	000786a3          	sb	zero,13(a5)
    1be0:	8082                	ret
            ;
        if (!n || !*s)
    1be2:	c645                	beqz	a2,1c8a <strncpy+0x21e>
    1be4:	0005c783          	lbu	a5,0(a1)
    1be8:	ea078be3          	beqz	a5,1a9e <strncpy+0x32>
            goto tail;
        wd = (void *)d;
        ws = (const void *)s;
        for (; n >= sizeof(size_t) && !HASZERO(*ws); n -= sizeof(size_t), ws++, wd++)
    1bec:	479d                	li	a5,7
    1bee:	02c7ff63          	bgeu	a5,a2,1c2c <strncpy+0x1c0>
    1bf2:	00000897          	auipc	a7,0x0
    1bf6:	3de8b883          	ld	a7,990(a7) # 1fd0 <__clone+0xd2>
    1bfa:	00000817          	auipc	a6,0x0
    1bfe:	3de83803          	ld	a6,990(a6) # 1fd8 <__clone+0xda>
    1c02:	431d                	li	t1,7
    1c04:	6198                	ld	a4,0(a1)
    1c06:	011707b3          	add	a5,a4,a7
    1c0a:	fff74693          	not	a3,a4
    1c0e:	8ff5                	and	a5,a5,a3
    1c10:	0107f7b3          	and	a5,a5,a6
    1c14:	ef81                	bnez	a5,1c2c <strncpy+0x1c0>
            *wd = *ws;
    1c16:	e118                	sd	a4,0(a0)
        for (; n >= sizeof(size_t) && !HASZERO(*ws); n -= sizeof(size_t), ws++, wd++)
    1c18:	1661                	addi	a2,a2,-8
    1c1a:	05a1                	addi	a1,a1,8
    1c1c:	0521                	addi	a0,a0,8
    1c1e:	fec363e3          	bltu	t1,a2,1c04 <strncpy+0x198>
        d = (void *)wd;
        s = (const void *)ws;
    }
    for (; n && (*d = *s); n--, s++, d++)
    1c22:	e609                	bnez	a2,1c2c <strncpy+0x1c0>
    1c24:	a08d                	j	1c86 <strncpy+0x21a>
    1c26:	167d                	addi	a2,a2,-1
    1c28:	0505                	addi	a0,a0,1
    1c2a:	ca01                	beqz	a2,1c3a <strncpy+0x1ce>
    1c2c:	0005c783          	lbu	a5,0(a1)
    1c30:	0585                	addi	a1,a1,1
    1c32:	00f50023          	sb	a5,0(a0)
    1c36:	fbe5                	bnez	a5,1c26 <strncpy+0x1ba>
        ;
tail:
    1c38:	b59d                	j	1a9e <strncpy+0x32>
    memset(d, 0, n);
    return d;
}
    1c3a:	8082                	ret
    1c3c:	472d                	li	a4,11
    1c3e:	bdb5                	j	1aba <strncpy+0x4e>
    1c40:	00778713          	addi	a4,a5,7
    1c44:	45ad                	li	a1,11
    1c46:	fff60693          	addi	a3,a2,-1
    1c4a:	e6b778e3          	bgeu	a4,a1,1aba <strncpy+0x4e>
    1c4e:	b7fd                	j	1c3c <strncpy+0x1d0>
    1c50:	40a007b3          	neg	a5,a0
    1c54:	8832                	mv	a6,a2
    1c56:	8b9d                	andi	a5,a5,7
    1c58:	4681                	li	a3,0
    1c5a:	e4060be3          	beqz	a2,1ab0 <strncpy+0x44>
    1c5e:	b7cd                	j	1c40 <strncpy+0x1d4>
    for (int i = 0; i < n; ++i, *(p++) = c)
    1c60:	468d                	li	a3,3
    1c62:	bd75                	j	1b1e <strncpy+0xb2>
    1c64:	872a                	mv	a4,a0
    1c66:	4681                	li	a3,0
    1c68:	bd5d                	j	1b1e <strncpy+0xb2>
    1c6a:	4685                	li	a3,1
    1c6c:	bd4d                	j	1b1e <strncpy+0xb2>
    1c6e:	8082                	ret
    1c70:	87aa                	mv	a5,a0
    1c72:	4681                	li	a3,0
    1c74:	b5f9                	j	1b42 <strncpy+0xd6>
    1c76:	4689                	li	a3,2
    1c78:	b55d                	j	1b1e <strncpy+0xb2>
    1c7a:	4691                	li	a3,4
    1c7c:	b54d                	j	1b1e <strncpy+0xb2>
    1c7e:	4695                	li	a3,5
    1c80:	bd79                	j	1b1e <strncpy+0xb2>
    1c82:	4699                	li	a3,6
    1c84:	bd69                	j	1b1e <strncpy+0xb2>
    1c86:	8082                	ret
    1c88:	8082                	ret
    1c8a:	8082                	ret

0000000000001c8c <open>:
#include <unistd.h>

#include "syscall.h"

int open(const char *path, int flags)
{
    1c8c:	87aa                	mv	a5,a0
    1c8e:	862e                	mv	a2,a1
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
}

static inline long __syscall4(long n, long a, long b, long c, long d)
{
    register long a7 __asm__("a7") = n;
    1c90:	03800893          	li	a7,56
    register long a0 __asm__("a0") = a;
    1c94:	f9c00513          	li	a0,-100
    register long a1 __asm__("a1") = b;
    1c98:	85be                	mv	a1,a5
    register long a2 __asm__("a2") = c;
    register long a3 __asm__("a3") = d;
    1c9a:	4689                	li	a3,2
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3))
    1c9c:	00000073          	ecall
    return syscall(SYS_openat, AT_FDCWD, path, flags, O_RDWR);
}
    1ca0:	2501                	sext.w	a0,a0
    1ca2:	8082                	ret

0000000000001ca4 <openat>:
    register long a7 __asm__("a7") = n;
    1ca4:	03800893          	li	a7,56
    register long a3 __asm__("a3") = d;
    1ca8:	18000693          	li	a3,384
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3))
    1cac:	00000073          	ecall

int openat(int dirfd,const char *path, int flags)
{
    return syscall(SYS_openat, dirfd, path, flags, 0600);
}
    1cb0:	2501                	sext.w	a0,a0
    1cb2:	8082                	ret

0000000000001cb4 <close>:
    register long a7 __asm__("a7") = n;
    1cb4:	03900893          	li	a7,57
    __asm_syscall("r"(a7), "0"(a0))
    1cb8:	00000073          	ecall

int close(int fd)
{
    return syscall(SYS_close, fd);
}
    1cbc:	2501                	sext.w	a0,a0
    1cbe:	8082                	ret

0000000000001cc0 <read>:
    register long a7 __asm__("a7") = n;
    1cc0:	03f00893          	li	a7,63
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1cc4:	00000073          	ecall

ssize_t read(int fd, void *buf, size_t len)
{
    return syscall(SYS_read, fd, buf, len);
}
    1cc8:	8082                	ret

0000000000001cca <write>:
    register long a7 __asm__("a7") = n;
    1cca:	04000893          	li	a7,64
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1cce:	00000073          	ecall

ssize_t write(int fd, const void *buf, size_t len)
{
    return syscall(SYS_write, fd, buf, len);
}
    1cd2:	8082                	ret

0000000000001cd4 <getpid>:
    register long a7 __asm__("a7") = n;
    1cd4:	0ac00893          	li	a7,172
    __asm_syscall("r"(a7))
    1cd8:	00000073          	ecall

pid_t getpid(void)
{
    return syscall(SYS_getpid);
}
    1cdc:	2501                	sext.w	a0,a0
    1cde:	8082                	ret

0000000000001ce0 <getppid>:
    register long a7 __asm__("a7") = n;
    1ce0:	0ad00893          	li	a7,173
    __asm_syscall("r"(a7))
    1ce4:	00000073          	ecall

pid_t getppid(void)
{
    return syscall(SYS_getppid);
}
    1ce8:	2501                	sext.w	a0,a0
    1cea:	8082                	ret

0000000000001cec <sched_yield>:
    register long a7 __asm__("a7") = n;
    1cec:	07c00893          	li	a7,124
    __asm_syscall("r"(a7))
    1cf0:	00000073          	ecall

int sched_yield(void)
{
    return syscall(SYS_sched_yield);
}
    1cf4:	2501                	sext.w	a0,a0
    1cf6:	8082                	ret

0000000000001cf8 <fork>:
    register long a7 __asm__("a7") = n;
    1cf8:	0dc00893          	li	a7,220
    register long a0 __asm__("a0") = a;
    1cfc:	4545                	li	a0,17
    register long a1 __asm__("a1") = b;
    1cfe:	4581                	li	a1,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1d00:	00000073          	ecall

pid_t fork(void)
{
    return syscall(SYS_clone, SIGCHLD, 0);
}
    1d04:	2501                	sext.w	a0,a0
    1d06:	8082                	ret

0000000000001d08 <clone>:

pid_t clone(int (*fn)(void *arg), void *arg, void *stack, size_t stack_size, unsigned long flags)
{
    1d08:	85b2                	mv	a1,a2
    1d0a:	863a                	mv	a2,a4
    if (stack)
    1d0c:	c191                	beqz	a1,1d10 <clone+0x8>
	stack += stack_size;
    1d0e:	95b6                	add	a1,a1,a3

    return __clone(fn, stack, flags, NULL, NULL, NULL);
    1d10:	4781                	li	a5,0
    1d12:	4701                	li	a4,0
    1d14:	4681                	li	a3,0
    1d16:	2601                	sext.w	a2,a2
    1d18:	a2dd                	j	1efe <__clone>

0000000000001d1a <exit>:
    register long a7 __asm__("a7") = n;
    1d1a:	05d00893          	li	a7,93
    __asm_syscall("r"(a7), "0"(a0))
    1d1e:	00000073          	ecall
    //return syscall(SYS_clone, fn, stack, flags, NULL, NULL, NULL);
}
void exit(int code)
{
    syscall(SYS_exit, code);
}
    1d22:	8082                	ret

0000000000001d24 <waitpid>:
    register long a7 __asm__("a7") = n;
    1d24:	10400893          	li	a7,260
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1d28:	00000073          	ecall

int waitpid(int pid, int *code, int options)
{
    return syscall(SYS_wait4, pid, code, options);
}
    1d2c:	2501                	sext.w	a0,a0
    1d2e:	8082                	ret

0000000000001d30 <exec>:
    register long a7 __asm__("a7") = n;
    1d30:	0dd00893          	li	a7,221
    __asm_syscall("r"(a7), "0"(a0))
    1d34:	00000073          	ecall

int exec(char *name)
{
    return syscall(SYS_execve, name);
}
    1d38:	2501                	sext.w	a0,a0
    1d3a:	8082                	ret

0000000000001d3c <execve>:
    register long a7 __asm__("a7") = n;
    1d3c:	0dd00893          	li	a7,221
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1d40:	00000073          	ecall

int execve(const char *name, char *const argv[], char *const argp[])
{
    return syscall(SYS_execve, name, argv, argp);
}
    1d44:	2501                	sext.w	a0,a0
    1d46:	8082                	ret

0000000000001d48 <times>:
    register long a7 __asm__("a7") = n;
    1d48:	09900893          	li	a7,153
    __asm_syscall("r"(a7), "0"(a0))
    1d4c:	00000073          	ecall

int times(void *mytimes)
{
	return syscall(SYS_times, mytimes);
}
    1d50:	2501                	sext.w	a0,a0
    1d52:	8082                	ret

0000000000001d54 <get_time>:

int64 get_time()
{
    1d54:	1141                	addi	sp,sp,-16
    register long a7 __asm__("a7") = n;
    1d56:	0a900893          	li	a7,169
    register long a0 __asm__("a0") = a;
    1d5a:	850a                	mv	a0,sp
    register long a1 __asm__("a1") = b;
    1d5c:	4581                	li	a1,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1d5e:	00000073          	ecall
    TimeVal time;
    int err = sys_get_time(&time, 0);
    if (err == 0)
    1d62:	2501                	sext.w	a0,a0
    1d64:	ed09                	bnez	a0,1d7e <get_time+0x2a>
    {
        return ((time.sec & 0xffff) * 1000 + time.usec / 1000);
    1d66:	67a2                	ld	a5,8(sp)
    1d68:	3e800713          	li	a4,1000
    1d6c:	00015503          	lhu	a0,0(sp)
    1d70:	02e7d7b3          	divu	a5,a5,a4
    1d74:	02e50533          	mul	a0,a0,a4
    1d78:	953e                	add	a0,a0,a5
    }
    else
    {
        return -1;
    }
}
    1d7a:	0141                	addi	sp,sp,16
    1d7c:	8082                	ret
        return -1;
    1d7e:	557d                	li	a0,-1
    1d80:	bfed                	j	1d7a <get_time+0x26>

0000000000001d82 <sys_get_time>:
    register long a7 __asm__("a7") = n;
    1d82:	0a900893          	li	a7,169
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1d86:	00000073          	ecall

int sys_get_time(TimeVal *ts, int tz)
{
    return syscall(SYS_gettimeofday, ts, tz);
}
    1d8a:	2501                	sext.w	a0,a0
    1d8c:	8082                	ret

0000000000001d8e <time>:
    register long a7 __asm__("a7") = n;
    1d8e:	42600893          	li	a7,1062
    __asm_syscall("r"(a7), "0"(a0))
    1d92:	00000073          	ecall

int time(unsigned long *tloc)
{
    return syscall(SYS_time, tloc);
}
    1d96:	2501                	sext.w	a0,a0
    1d98:	8082                	ret

0000000000001d9a <sleep>:

int sleep(unsigned long long time)
{
    1d9a:	1141                	addi	sp,sp,-16
    TimeVal tv = {.sec = time, .usec = 0};
    1d9c:	e02a                	sd	a0,0(sp)
    register long a0 __asm__("a0") = a;
    1d9e:	850a                	mv	a0,sp
    1da0:	e402                	sd	zero,8(sp)
    register long a7 __asm__("a7") = n;
    1da2:	06500893          	li	a7,101
    register long a1 __asm__("a1") = b;
    1da6:	85aa                	mv	a1,a0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1da8:	00000073          	ecall
    if (syscall(SYS_nanosleep, &tv, &tv)) return tv.sec;
    1dac:	e501                	bnez	a0,1db4 <sleep+0x1a>
    return 0;
    1dae:	4501                	li	a0,0
}
    1db0:	0141                	addi	sp,sp,16
    1db2:	8082                	ret
    if (syscall(SYS_nanosleep, &tv, &tv)) return tv.sec;
    1db4:	4502                	lw	a0,0(sp)
}
    1db6:	0141                	addi	sp,sp,16
    1db8:	8082                	ret

0000000000001dba <set_priority>:
    register long a7 __asm__("a7") = n;
    1dba:	08c00893          	li	a7,140
    __asm_syscall("r"(a7), "0"(a0))
    1dbe:	00000073          	ecall

int set_priority(int prio)
{
    return syscall(SYS_setpriority, prio);
}
    1dc2:	2501                	sext.w	a0,a0
    1dc4:	8082                	ret

0000000000001dc6 <mmap>:
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4))
}

static inline long __syscall6(long n, long a, long b, long c, long d, long e, long f)
{
    register long a7 __asm__("a7") = n;
    1dc6:	0de00893          	li	a7,222
    register long a1 __asm__("a1") = b;
    register long a2 __asm__("a2") = c;
    register long a3 __asm__("a3") = d;
    register long a4 __asm__("a4") = e;
    register long a5 __asm__("a5") = f;
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4), "r"(a5))
    1dca:	00000073          	ecall

void *mmap(void *start, size_t len, int prot, int flags, int fd, off_t off)
{
    return syscall(SYS_mmap, start, len, prot, flags, fd, off);
}
    1dce:	8082                	ret

0000000000001dd0 <munmap>:
    register long a7 __asm__("a7") = n;
    1dd0:	0d700893          	li	a7,215
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1dd4:	00000073          	ecall

int munmap(void *start, size_t len)
{
    return syscall(SYS_munmap, start, len);
}
    1dd8:	2501                	sext.w	a0,a0
    1dda:	8082                	ret

0000000000001ddc <wait>:

int wait(int *code)
{
    1ddc:	85aa                	mv	a1,a0
    register long a7 __asm__("a7") = n;
    1dde:	10400893          	li	a7,260
    register long a0 __asm__("a0") = a;
    1de2:	557d                	li	a0,-1
    register long a2 __asm__("a2") = c;
    1de4:	4601                	li	a2,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1de6:	00000073          	ecall
    return waitpid(-1, code, 0);
}
    1dea:	2501                	sext.w	a0,a0
    1dec:	8082                	ret

0000000000001dee <spawn>:
    register long a7 __asm__("a7") = n;
    1dee:	19000893          	li	a7,400
    __asm_syscall("r"(a7), "0"(a0))
    1df2:	00000073          	ecall

int spawn(char *file)
{
    return syscall(SYS_spawn, file);
}
    1df6:	2501                	sext.w	a0,a0
    1df8:	8082                	ret

0000000000001dfa <mailread>:
    register long a7 __asm__("a7") = n;
    1dfa:	19100893          	li	a7,401
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1dfe:	00000073          	ecall

int mailread(void *buf, int len)
{
    return syscall(SYS_mailread, buf, len);
}
    1e02:	2501                	sext.w	a0,a0
    1e04:	8082                	ret

0000000000001e06 <mailwrite>:
    register long a7 __asm__("a7") = n;
    1e06:	19200893          	li	a7,402
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1e0a:	00000073          	ecall

int mailwrite(int pid, void *buf, int len)
{
    return syscall(SYS_mailwrite, pid, buf, len);
}
    1e0e:	2501                	sext.w	a0,a0
    1e10:	8082                	ret

0000000000001e12 <fstat>:
    register long a7 __asm__("a7") = n;
    1e12:	05000893          	li	a7,80
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1e16:	00000073          	ecall

int fstat(int fd, struct kstat *st)
{
    return syscall(SYS_fstat, fd, st);
}
    1e1a:	2501                	sext.w	a0,a0
    1e1c:	8082                	ret

0000000000001e1e <sys_linkat>:
    register long a4 __asm__("a4") = e;
    1e1e:	1702                	slli	a4,a4,0x20
    register long a7 __asm__("a7") = n;
    1e20:	02500893          	li	a7,37
    register long a4 __asm__("a4") = e;
    1e24:	9301                	srli	a4,a4,0x20
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4))
    1e26:	00000073          	ecall

int sys_linkat(int olddirfd, char *oldpath, int newdirfd, char *newpath, unsigned int flags)
{
    return syscall(SYS_linkat, olddirfd, oldpath, newdirfd, newpath, flags);
}
    1e2a:	2501                	sext.w	a0,a0
    1e2c:	8082                	ret

0000000000001e2e <sys_unlinkat>:
    register long a2 __asm__("a2") = c;
    1e2e:	1602                	slli	a2,a2,0x20
    register long a7 __asm__("a7") = n;
    1e30:	02300893          	li	a7,35
    register long a2 __asm__("a2") = c;
    1e34:	9201                	srli	a2,a2,0x20
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1e36:	00000073          	ecall

int sys_unlinkat(int dirfd, char *path, unsigned int flags)
{
    return syscall(SYS_unlinkat, dirfd, path, flags);
}
    1e3a:	2501                	sext.w	a0,a0
    1e3c:	8082                	ret

0000000000001e3e <link>:

int link(char *old_path, char *new_path)
{
    1e3e:	87aa                	mv	a5,a0
    1e40:	86ae                	mv	a3,a1
    register long a7 __asm__("a7") = n;
    1e42:	02500893          	li	a7,37
    register long a0 __asm__("a0") = a;
    1e46:	f9c00513          	li	a0,-100
    register long a1 __asm__("a1") = b;
    1e4a:	85be                	mv	a1,a5
    register long a2 __asm__("a2") = c;
    1e4c:	f9c00613          	li	a2,-100
    register long a4 __asm__("a4") = e;
    1e50:	4701                	li	a4,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4))
    1e52:	00000073          	ecall
    return sys_linkat(AT_FDCWD, old_path, AT_FDCWD, new_path, 0);
}
    1e56:	2501                	sext.w	a0,a0
    1e58:	8082                	ret

0000000000001e5a <unlink>:

int unlink(char *path)
{
    1e5a:	85aa                	mv	a1,a0
    register long a7 __asm__("a7") = n;
    1e5c:	02300893          	li	a7,35
    register long a0 __asm__("a0") = a;
    1e60:	f9c00513          	li	a0,-100
    register long a2 __asm__("a2") = c;
    1e64:	4601                	li	a2,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1e66:	00000073          	ecall
    return sys_unlinkat(AT_FDCWD, path, 0);
}
    1e6a:	2501                	sext.w	a0,a0
    1e6c:	8082                	ret

0000000000001e6e <uname>:
    register long a7 __asm__("a7") = n;
    1e6e:	0a000893          	li	a7,160
    __asm_syscall("r"(a7), "0"(a0))
    1e72:	00000073          	ecall

int uname(void *buf)
{
    return syscall(SYS_uname, buf);
}
    1e76:	2501                	sext.w	a0,a0
    1e78:	8082                	ret

0000000000001e7a <brk>:
    register long a7 __asm__("a7") = n;
    1e7a:	0d600893          	li	a7,214
    __asm_syscall("r"(a7), "0"(a0))
    1e7e:	00000073          	ecall

int brk(void *addr)
{
    return syscall(SYS_brk, addr);
}
    1e82:	2501                	sext.w	a0,a0
    1e84:	8082                	ret

0000000000001e86 <getcwd>:
    register long a7 __asm__("a7") = n;
    1e86:	48c5                	li	a7,17
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1e88:	00000073          	ecall

char *getcwd(char *buf, size_t size){
    return syscall(SYS_getcwd, buf, size);
}
    1e8c:	8082                	ret

0000000000001e8e <chdir>:
    register long a7 __asm__("a7") = n;
    1e8e:	03100893          	li	a7,49
    __asm_syscall("r"(a7), "0"(a0))
    1e92:	00000073          	ecall

int chdir(const char *path){
    return syscall(SYS_chdir, path);
}
    1e96:	2501                	sext.w	a0,a0
    1e98:	8082                	ret

0000000000001e9a <mkdir>:

int mkdir(const char *path, mode_t mode){
    1e9a:	862e                	mv	a2,a1
    1e9c:	87aa                	mv	a5,a0
    register long a2 __asm__("a2") = c;
    1e9e:	1602                	slli	a2,a2,0x20
    register long a7 __asm__("a7") = n;
    1ea0:	02200893          	li	a7,34
    register long a0 __asm__("a0") = a;
    1ea4:	f9c00513          	li	a0,-100
    register long a1 __asm__("a1") = b;
    1ea8:	85be                	mv	a1,a5
    register long a2 __asm__("a2") = c;
    1eaa:	9201                	srli	a2,a2,0x20
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1eac:	00000073          	ecall
    return syscall(SYS_mkdirat, AT_FDCWD, path, mode);
}
    1eb0:	2501                	sext.w	a0,a0
    1eb2:	8082                	ret

0000000000001eb4 <getdents>:
    register long a7 __asm__("a7") = n;
    1eb4:	03d00893          	li	a7,61
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1eb8:	00000073          	ecall

int getdents(int fd, struct linux_dirent64 *dirp64, unsigned long len){
    //return syscall(SYS_getdents64, fd, dirp64, len);
    return syscall(SYS_getdents64, fd, dirp64, len);
}
    1ebc:	2501                	sext.w	a0,a0
    1ebe:	8082                	ret

0000000000001ec0 <pipe>:
    register long a7 __asm__("a7") = n;
    1ec0:	03b00893          	li	a7,59
    register long a1 __asm__("a1") = b;
    1ec4:	4581                	li	a1,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1ec6:	00000073          	ecall

int pipe(int fd[2]){
    return syscall(SYS_pipe2, fd, 0);
}
    1eca:	2501                	sext.w	a0,a0
    1ecc:	8082                	ret

0000000000001ece <dup>:
    register long a7 __asm__("a7") = n;
    1ece:	48dd                	li	a7,23
    __asm_syscall("r"(a7), "0"(a0))
    1ed0:	00000073          	ecall

int dup(int fd){
    return syscall(SYS_dup, fd);
}
    1ed4:	2501                	sext.w	a0,a0
    1ed6:	8082                	ret

0000000000001ed8 <dup2>:
    register long a7 __asm__("a7") = n;
    1ed8:	48e1                	li	a7,24
    register long a2 __asm__("a2") = c;
    1eda:	4601                	li	a2,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1edc:	00000073          	ecall

int dup2(int old, int new){
    return syscall(SYS_dup3, old, new, 0);
}
    1ee0:	2501                	sext.w	a0,a0
    1ee2:	8082                	ret

0000000000001ee4 <mount>:
    register long a7 __asm__("a7") = n;
    1ee4:	02800893          	li	a7,40
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4))
    1ee8:	00000073          	ecall

int mount(const char *special, const char *dir, const char *fstype, unsigned long flags, const void *data)
{
        return syscall(SYS_mount, special, dir, fstype, flags, data);
}
    1eec:	2501                	sext.w	a0,a0
    1eee:	8082                	ret

0000000000001ef0 <umount>:
    register long a7 __asm__("a7") = n;
    1ef0:	02700893          	li	a7,39
    register long a1 __asm__("a1") = b;
    1ef4:	4581                	li	a1,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1ef6:	00000073          	ecall

int umount(const char *special)
{
        return syscall(SYS_umount2, special, 0);
}
    1efa:	2501                	sext.w	a0,a0
    1efc:	8082                	ret

0000000000001efe <__clone>:

.global __clone
.type  __clone, %function
__clone:
	# Save func and arg to stack
	addi a1, a1, -16
    1efe:	15c1                	addi	a1,a1,-16
	sd a0, 0(a1)
    1f00:	e188                	sd	a0,0(a1)
	sd a3, 8(a1)
    1f02:	e594                	sd	a3,8(a1)

	# Call SYS_clone
	mv a0, a2
    1f04:	8532                	mv	a0,a2
	mv a2, a4
    1f06:	863a                	mv	a2,a4
	mv a3, a5
    1f08:	86be                	mv	a3,a5
	mv a4, a6
    1f0a:	8742                	mv	a4,a6
	li a7, 220 # SYS_clone
    1f0c:	0dc00893          	li	a7,220
	ecall
    1f10:	00000073          	ecall

	beqz a0, 1f
    1f14:	c111                	beqz	a0,1f18 <__clone+0x1a>
	# Parent
	ret
    1f16:	8082                	ret

	# Child
1:      ld a1, 0(sp)
    1f18:	6582                	ld	a1,0(sp)
	ld a0, 8(sp)
    1f1a:	6522                	ld	a0,8(sp)
	jalr a1
    1f1c:	9582                	jalr	a1

	# Exit
	li a7, 93 # SYS_exit
    1f1e:	05d00893          	li	a7,93
	ecall
    1f22:	00000073          	ecall
