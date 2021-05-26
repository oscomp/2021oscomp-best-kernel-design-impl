
/home/ubuntu/rust/ultra-os/codes/riscv-syscalls-testing/user/build/riscv64/fork:     file format elf64-littleriscv


Disassembly of section .text:

0000000000001000 <_start>:
.section .text.entry
.globl _start
_start:
    mv a0, sp
    1000:	850a                	mv	a0,sp
    tail __start_main
    1002:	a05d                	j	10a8 <__start_main>

0000000000001004 <test_fork>:
 * 成功测试时子进程的输出：
 * "  child process."
 */
static int fd[2];

void test_fork(void){
    1004:	1101                	addi	sp,sp,-32
    TEST_START(__func__);
    1006:	00001517          	auipc	a0,0x1
    100a:	eda50513          	addi	a0,a0,-294 # 1ee0 <__clone+0x2a>
void test_fork(void){
    100e:	ec06                	sd	ra,24(sp)
    TEST_START(__func__);
    1010:	30a000ef          	jal	ra,131a <puts>
    1014:	00001517          	auipc	a0,0x1
    1018:	f7c50513          	addi	a0,a0,-132 # 1f90 <__func__.0>
    101c:	2fe000ef          	jal	ra,131a <puts>
    1020:	00001517          	auipc	a0,0x1
    1024:	ed850513          	addi	a0,a0,-296 # 1ef8 <__clone+0x42>
    1028:	2f2000ef          	jal	ra,131a <puts>
    int cpid, wstatus;
    cpid = fork();
    102c:	485000ef          	jal	ra,1cb0 <fork>
    assert(cpid != -1);
    1030:	57fd                	li	a5,-1
    1032:	04f50363          	beq	a0,a5,1078 <test_fork+0x74>

    if(cpid > 0){
    1036:	04a05763          	blez	a0,1084 <test_fork+0x80>
	wait(&wstatus);
    103a:	0068                	addi	a0,sp,12
    103c:	559000ef          	jal	ra,1d94 <wait>
	printf("  parent process. wstatus:%d\n", wstatus);
    1040:	45b2                	lw	a1,12(sp)
    1042:	00001517          	auipc	a0,0x1
    1046:	ee650513          	addi	a0,a0,-282 # 1f28 <__clone+0x72>
    104a:	2f2000ef          	jal	ra,133c <printf>
    }else{
	printf("  child process.\n");
	exit(0);
    }
    TEST_END(__func__);
    104e:	00001517          	auipc	a0,0x1
    1052:	f1250513          	addi	a0,a0,-238 # 1f60 <__clone+0xaa>
    1056:	2c4000ef          	jal	ra,131a <puts>
    105a:	00001517          	auipc	a0,0x1
    105e:	f3650513          	addi	a0,a0,-202 # 1f90 <__func__.0>
    1062:	2b8000ef          	jal	ra,131a <puts>
    1066:	00001517          	auipc	a0,0x1
    106a:	e9250513          	addi	a0,a0,-366 # 1ef8 <__clone+0x42>
    106e:	2ac000ef          	jal	ra,131a <puts>
}
    1072:	60e2                	ld	ra,24(sp)
    1074:	6105                	addi	sp,sp,32
    1076:	8082                	ret
    assert(cpid != -1);
    1078:	00001517          	auipc	a0,0x1
    107c:	e9050513          	addi	a0,a0,-368 # 1f08 <__clone+0x52>
    1080:	540000ef          	jal	ra,15c0 <panic>
	printf("  child process.\n");
    1084:	00001517          	auipc	a0,0x1
    1088:	ec450513          	addi	a0,a0,-316 # 1f48 <__clone+0x92>
    108c:	2b0000ef          	jal	ra,133c <printf>
	exit(0);
    1090:	4501                	li	a0,0
    1092:	441000ef          	jal	ra,1cd2 <exit>
    1096:	bf65                	j	104e <test_fork+0x4a>

0000000000001098 <main>:

int main(void){
    1098:	1141                	addi	sp,sp,-16
    109a:	e406                	sd	ra,8(sp)
    test_fork();
    109c:	f69ff0ef          	jal	ra,1004 <test_fork>
    return 0;
}
    10a0:	60a2                	ld	ra,8(sp)
    10a2:	4501                	li	a0,0
    10a4:	0141                	addi	sp,sp,16
    10a6:	8082                	ret

00000000000010a8 <__start_main>:
#include <unistd.h>

extern int main();

int __start_main(long *p)
{
    10a8:	85aa                	mv	a1,a0
	int argc = p[0];
	char **argv = (void *)(p+1);

	exit(main(argc, argv));
    10aa:	4108                	lw	a0,0(a0)
{
    10ac:	1141                	addi	sp,sp,-16
	exit(main(argc, argv));
    10ae:	05a1                	addi	a1,a1,8
{
    10b0:	e406                	sd	ra,8(sp)
	exit(main(argc, argv));
    10b2:	fe7ff0ef          	jal	ra,1098 <main>
    10b6:	41d000ef          	jal	ra,1cd2 <exit>
	return 0;
}
    10ba:	60a2                	ld	ra,8(sp)
    10bc:	4501                	li	a0,0
    10be:	0141                	addi	sp,sp,16
    10c0:	8082                	ret

00000000000010c2 <printint.constprop.0>:
    write(f, s, l);
}

static char digits[] = "0123456789abcdef";

static void printint(int xx, int base, int sign)
    10c2:	7179                	addi	sp,sp,-48
    10c4:	f406                	sd	ra,40(sp)
{
    char buf[16 + 1];
    int i;
    uint x;

    if (sign && (sign = xx < 0))
    10c6:	12054b63          	bltz	a0,11fc <printint.constprop.0+0x13a>

    buf[16] = 0;
    i = 15;
    do
    {
        buf[i--] = digits[x % base];
    10ca:	02b577bb          	remuw	a5,a0,a1
    10ce:	00001617          	auipc	a2,0x1
    10d2:	ed260613          	addi	a2,a2,-302 # 1fa0 <digits>
    buf[16] = 0;
    10d6:	00010c23          	sb	zero,24(sp)
        buf[i--] = digits[x % base];
    10da:	0005871b          	sext.w	a4,a1
    10de:	1782                	slli	a5,a5,0x20
    10e0:	9381                	srli	a5,a5,0x20
    10e2:	97b2                	add	a5,a5,a2
    10e4:	0007c783          	lbu	a5,0(a5)
    } while ((x /= base) != 0);
    10e8:	02b5583b          	divuw	a6,a0,a1
        buf[i--] = digits[x % base];
    10ec:	00f10ba3          	sb	a5,23(sp)
    } while ((x /= base) != 0);
    10f0:	1cb56363          	bltu	a0,a1,12b6 <printint.constprop.0+0x1f4>
        buf[i--] = digits[x % base];
    10f4:	45b9                	li	a1,14
    10f6:	02e877bb          	remuw	a5,a6,a4
    10fa:	1782                	slli	a5,a5,0x20
    10fc:	9381                	srli	a5,a5,0x20
    10fe:	97b2                	add	a5,a5,a2
    1100:	0007c783          	lbu	a5,0(a5)
    } while ((x /= base) != 0);
    1104:	02e856bb          	divuw	a3,a6,a4
        buf[i--] = digits[x % base];
    1108:	00f10b23          	sb	a5,22(sp)
    } while ((x /= base) != 0);
    110c:	0ce86e63          	bltu	a6,a4,11e8 <printint.constprop.0+0x126>
        buf[i--] = digits[x % base];
    1110:	02e6f5bb          	remuw	a1,a3,a4
    } while ((x /= base) != 0);
    1114:	02e6d7bb          	divuw	a5,a3,a4
        buf[i--] = digits[x % base];
    1118:	1582                	slli	a1,a1,0x20
    111a:	9181                	srli	a1,a1,0x20
    111c:	95b2                	add	a1,a1,a2
    111e:	0005c583          	lbu	a1,0(a1)
    1122:	00b10aa3          	sb	a1,21(sp)
    } while ((x /= base) != 0);
    1126:	0007859b          	sext.w	a1,a5
    112a:	12e6ec63          	bltu	a3,a4,1262 <printint.constprop.0+0x1a0>
        buf[i--] = digits[x % base];
    112e:	02e7f6bb          	remuw	a3,a5,a4
    1132:	1682                	slli	a3,a3,0x20
    1134:	9281                	srli	a3,a3,0x20
    1136:	96b2                	add	a3,a3,a2
    1138:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    113c:	02e7d83b          	divuw	a6,a5,a4
        buf[i--] = digits[x % base];
    1140:	00d10a23          	sb	a3,20(sp)
    } while ((x /= base) != 0);
    1144:	12e5e863          	bltu	a1,a4,1274 <printint.constprop.0+0x1b2>
        buf[i--] = digits[x % base];
    1148:	02e876bb          	remuw	a3,a6,a4
    114c:	1682                	slli	a3,a3,0x20
    114e:	9281                	srli	a3,a3,0x20
    1150:	96b2                	add	a3,a3,a2
    1152:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    1156:	02e855bb          	divuw	a1,a6,a4
        buf[i--] = digits[x % base];
    115a:	00d109a3          	sb	a3,19(sp)
    } while ((x /= base) != 0);
    115e:	12e86463          	bltu	a6,a4,1286 <printint.constprop.0+0x1c4>
        buf[i--] = digits[x % base];
    1162:	02e5f6bb          	remuw	a3,a1,a4
    1166:	1682                	slli	a3,a3,0x20
    1168:	9281                	srli	a3,a3,0x20
    116a:	96b2                	add	a3,a3,a2
    116c:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    1170:	02e5d83b          	divuw	a6,a1,a4
        buf[i--] = digits[x % base];
    1174:	00d10923          	sb	a3,18(sp)
    } while ((x /= base) != 0);
    1178:	0ce5ec63          	bltu	a1,a4,1250 <printint.constprop.0+0x18e>
        buf[i--] = digits[x % base];
    117c:	02e876bb          	remuw	a3,a6,a4
    1180:	1682                	slli	a3,a3,0x20
    1182:	9281                	srli	a3,a3,0x20
    1184:	96b2                	add	a3,a3,a2
    1186:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    118a:	02e855bb          	divuw	a1,a6,a4
        buf[i--] = digits[x % base];
    118e:	00d108a3          	sb	a3,17(sp)
    } while ((x /= base) != 0);
    1192:	10e86963          	bltu	a6,a4,12a4 <printint.constprop.0+0x1e2>
        buf[i--] = digits[x % base];
    1196:	02e5f6bb          	remuw	a3,a1,a4
    119a:	1682                	slli	a3,a3,0x20
    119c:	9281                	srli	a3,a3,0x20
    119e:	96b2                	add	a3,a3,a2
    11a0:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    11a4:	02e5d83b          	divuw	a6,a1,a4
        buf[i--] = digits[x % base];
    11a8:	00d10823          	sb	a3,16(sp)
    } while ((x /= base) != 0);
    11ac:	10e5e763          	bltu	a1,a4,12ba <printint.constprop.0+0x1f8>
        buf[i--] = digits[x % base];
    11b0:	02e876bb          	remuw	a3,a6,a4
    11b4:	1682                	slli	a3,a3,0x20
    11b6:	9281                	srli	a3,a3,0x20
    11b8:	96b2                	add	a3,a3,a2
    11ba:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    11be:	02e857bb          	divuw	a5,a6,a4
        buf[i--] = digits[x % base];
    11c2:	00d107a3          	sb	a3,15(sp)
    } while ((x /= base) != 0);
    11c6:	10e86363          	bltu	a6,a4,12cc <printint.constprop.0+0x20a>
        buf[i--] = digits[x % base];
    11ca:	1782                	slli	a5,a5,0x20
    11cc:	9381                	srli	a5,a5,0x20
    11ce:	97b2                	add	a5,a5,a2
    11d0:	0007c783          	lbu	a5,0(a5)
    11d4:	4599                	li	a1,6
    11d6:	00f10723          	sb	a5,14(sp)

    if (sign)
    11da:	00055763          	bgez	a0,11e8 <printint.constprop.0+0x126>
        buf[i--] = '-';
    11de:	02d00793          	li	a5,45
    11e2:	00f106a3          	sb	a5,13(sp)
        buf[i--] = digits[x % base];
    11e6:	4595                	li	a1,5
    write(f, s, l);
    11e8:	003c                	addi	a5,sp,8
    11ea:	4641                	li	a2,16
    11ec:	9e0d                	subw	a2,a2,a1
    11ee:	4505                	li	a0,1
    11f0:	95be                	add	a1,a1,a5
    11f2:	291000ef          	jal	ra,1c82 <write>
    i++;
    if (i < 0)
        puts("printint error");
    out(stdout, buf + i, 16 - i);
}
    11f6:	70a2                	ld	ra,40(sp)
    11f8:	6145                	addi	sp,sp,48
    11fa:	8082                	ret
        x = -xx;
    11fc:	40a0083b          	negw	a6,a0
        buf[i--] = digits[x % base];
    1200:	02b877bb          	remuw	a5,a6,a1
    1204:	00001617          	auipc	a2,0x1
    1208:	d9c60613          	addi	a2,a2,-612 # 1fa0 <digits>
    buf[16] = 0;
    120c:	00010c23          	sb	zero,24(sp)
        buf[i--] = digits[x % base];
    1210:	0005871b          	sext.w	a4,a1
    1214:	1782                	slli	a5,a5,0x20
    1216:	9381                	srli	a5,a5,0x20
    1218:	97b2                	add	a5,a5,a2
    121a:	0007c783          	lbu	a5,0(a5)
    } while ((x /= base) != 0);
    121e:	02b858bb          	divuw	a7,a6,a1
        buf[i--] = digits[x % base];
    1222:	00f10ba3          	sb	a5,23(sp)
    } while ((x /= base) != 0);
    1226:	06b86963          	bltu	a6,a1,1298 <printint.constprop.0+0x1d6>
        buf[i--] = digits[x % base];
    122a:	02e8f7bb          	remuw	a5,a7,a4
    122e:	1782                	slli	a5,a5,0x20
    1230:	9381                	srli	a5,a5,0x20
    1232:	97b2                	add	a5,a5,a2
    1234:	0007c783          	lbu	a5,0(a5)
    } while ((x /= base) != 0);
    1238:	02e8d6bb          	divuw	a3,a7,a4
        buf[i--] = digits[x % base];
    123c:	00f10b23          	sb	a5,22(sp)
    } while ((x /= base) != 0);
    1240:	ece8f8e3          	bgeu	a7,a4,1110 <printint.constprop.0+0x4e>
        buf[i--] = '-';
    1244:	02d00793          	li	a5,45
    1248:	00f10aa3          	sb	a5,21(sp)
        buf[i--] = digits[x % base];
    124c:	45b5                	li	a1,13
    124e:	bf69                	j	11e8 <printint.constprop.0+0x126>
    1250:	45a9                	li	a1,10
    if (sign)
    1252:	f8055be3          	bgez	a0,11e8 <printint.constprop.0+0x126>
        buf[i--] = '-';
    1256:	02d00793          	li	a5,45
    125a:	00f108a3          	sb	a5,17(sp)
        buf[i--] = digits[x % base];
    125e:	45a5                	li	a1,9
    1260:	b761                	j	11e8 <printint.constprop.0+0x126>
    1262:	45b5                	li	a1,13
    if (sign)
    1264:	f80552e3          	bgez	a0,11e8 <printint.constprop.0+0x126>
        buf[i--] = '-';
    1268:	02d00793          	li	a5,45
    126c:	00f10a23          	sb	a5,20(sp)
        buf[i--] = digits[x % base];
    1270:	45b1                	li	a1,12
    1272:	bf9d                	j	11e8 <printint.constprop.0+0x126>
    1274:	45b1                	li	a1,12
    if (sign)
    1276:	f60559e3          	bgez	a0,11e8 <printint.constprop.0+0x126>
        buf[i--] = '-';
    127a:	02d00793          	li	a5,45
    127e:	00f109a3          	sb	a5,19(sp)
        buf[i--] = digits[x % base];
    1282:	45ad                	li	a1,11
    1284:	b795                	j	11e8 <printint.constprop.0+0x126>
    1286:	45ad                	li	a1,11
    if (sign)
    1288:	f60550e3          	bgez	a0,11e8 <printint.constprop.0+0x126>
        buf[i--] = '-';
    128c:	02d00793          	li	a5,45
    1290:	00f10923          	sb	a5,18(sp)
        buf[i--] = digits[x % base];
    1294:	45a9                	li	a1,10
    1296:	bf89                	j	11e8 <printint.constprop.0+0x126>
        buf[i--] = '-';
    1298:	02d00793          	li	a5,45
    129c:	00f10b23          	sb	a5,22(sp)
        buf[i--] = digits[x % base];
    12a0:	45b9                	li	a1,14
    12a2:	b799                	j	11e8 <printint.constprop.0+0x126>
    12a4:	45a5                	li	a1,9
    if (sign)
    12a6:	f40551e3          	bgez	a0,11e8 <printint.constprop.0+0x126>
        buf[i--] = '-';
    12aa:	02d00793          	li	a5,45
    12ae:	00f10823          	sb	a5,16(sp)
        buf[i--] = digits[x % base];
    12b2:	45a1                	li	a1,8
    12b4:	bf15                	j	11e8 <printint.constprop.0+0x126>
    i = 15;
    12b6:	45bd                	li	a1,15
    12b8:	bf05                	j	11e8 <printint.constprop.0+0x126>
        buf[i--] = digits[x % base];
    12ba:	45a1                	li	a1,8
    if (sign)
    12bc:	f20556e3          	bgez	a0,11e8 <printint.constprop.0+0x126>
        buf[i--] = '-';
    12c0:	02d00793          	li	a5,45
    12c4:	00f107a3          	sb	a5,15(sp)
        buf[i--] = digits[x % base];
    12c8:	459d                	li	a1,7
    12ca:	bf39                	j	11e8 <printint.constprop.0+0x126>
    12cc:	459d                	li	a1,7
    if (sign)
    12ce:	f0055de3          	bgez	a0,11e8 <printint.constprop.0+0x126>
        buf[i--] = '-';
    12d2:	02d00793          	li	a5,45
    12d6:	00f10723          	sb	a5,14(sp)
        buf[i--] = digits[x % base];
    12da:	4599                	li	a1,6
    12dc:	b731                	j	11e8 <printint.constprop.0+0x126>

00000000000012de <getchar>:
{
    12de:	1101                	addi	sp,sp,-32
    read(stdin, &byte, 1);
    12e0:	00f10593          	addi	a1,sp,15
    12e4:	4605                	li	a2,1
    12e6:	4501                	li	a0,0
{
    12e8:	ec06                	sd	ra,24(sp)
    char byte = 0;
    12ea:	000107a3          	sb	zero,15(sp)
    read(stdin, &byte, 1);
    12ee:	18b000ef          	jal	ra,1c78 <read>
}
    12f2:	60e2                	ld	ra,24(sp)
    12f4:	00f14503          	lbu	a0,15(sp)
    12f8:	6105                	addi	sp,sp,32
    12fa:	8082                	ret

00000000000012fc <putchar>:
{
    12fc:	1101                	addi	sp,sp,-32
    12fe:	87aa                	mv	a5,a0
    return write(stdout, &byte, 1);
    1300:	00f10593          	addi	a1,sp,15
    1304:	4605                	li	a2,1
    1306:	4505                	li	a0,1
{
    1308:	ec06                	sd	ra,24(sp)
    char byte = c;
    130a:	00f107a3          	sb	a5,15(sp)
    return write(stdout, &byte, 1);
    130e:	175000ef          	jal	ra,1c82 <write>
}
    1312:	60e2                	ld	ra,24(sp)
    1314:	2501                	sext.w	a0,a0
    1316:	6105                	addi	sp,sp,32
    1318:	8082                	ret

000000000000131a <puts>:
{
    131a:	1141                	addi	sp,sp,-16
    131c:	e406                	sd	ra,8(sp)
    131e:	e022                	sd	s0,0(sp)
    1320:	842a                	mv	s0,a0
    r = -(write(stdout, s, strlen(s)) < 0);
    1322:	57c000ef          	jal	ra,189e <strlen>
    1326:	862a                	mv	a2,a0
    1328:	85a2                	mv	a1,s0
    132a:	4505                	li	a0,1
    132c:	157000ef          	jal	ra,1c82 <write>
}
    1330:	60a2                	ld	ra,8(sp)
    1332:	6402                	ld	s0,0(sp)
    r = -(write(stdout, s, strlen(s)) < 0);
    1334:	957d                	srai	a0,a0,0x3f
    return r;
    1336:	2501                	sext.w	a0,a0
}
    1338:	0141                	addi	sp,sp,16
    133a:	8082                	ret

000000000000133c <printf>:
    out(stdout, buf, i);
}

// Print to the console. only understands %d, %x, %p, %s.
void printf(const char *fmt, ...)
{
    133c:	7171                	addi	sp,sp,-176
    133e:	fc56                	sd	s5,56(sp)
    1340:	ed3e                	sd	a5,152(sp)
    buf[i++] = '0';
    1342:	7ae1                	lui	s5,0xffff8
    va_list ap;
    int cnt = 0, l = 0;
    char *a, *z, *s = (char *)fmt, str;
    int f = stdout;

    va_start(ap, fmt);
    1344:	18bc                	addi	a5,sp,120
{
    1346:	e8ca                	sd	s2,80(sp)
    1348:	e4ce                	sd	s3,72(sp)
    134a:	e0d2                	sd	s4,64(sp)
    134c:	f85a                	sd	s6,48(sp)
    134e:	f486                	sd	ra,104(sp)
    1350:	f0a2                	sd	s0,96(sp)
    1352:	eca6                	sd	s1,88(sp)
    1354:	fcae                	sd	a1,120(sp)
    1356:	e132                	sd	a2,128(sp)
    1358:	e536                	sd	a3,136(sp)
    135a:	e93a                	sd	a4,144(sp)
    135c:	f142                	sd	a6,160(sp)
    135e:	f546                	sd	a7,168(sp)
    va_start(ap, fmt);
    1360:	e03e                	sd	a5,0(sp)
    for (;;)
    {
        if (!*s)
            break;
        for (a = s; *s && *s != '%'; s++)
    1362:	02500913          	li	s2,37
        out(f, a, l);
        if (l)
            continue;
        if (s[1] == 0)
            break;
        switch (s[1])
    1366:	07300a13          	li	s4,115
        case 'p':
            printptr(va_arg(ap, uint64));
            break;
        case 's':
            if ((a = va_arg(ap, char *)) == 0)
                a = "(null)";
    136a:	00001b17          	auipc	s6,0x1
    136e:	c06b0b13          	addi	s6,s6,-1018 # 1f70 <__clone+0xba>
    buf[i++] = '0';
    1372:	830aca93          	xori	s5,s5,-2000
        buf[i++] = digits[x >> (sizeof(uint64) * 8 - 4)];
    1376:	00001997          	auipc	s3,0x1
    137a:	c2a98993          	addi	s3,s3,-982 # 1fa0 <digits>
        if (!*s)
    137e:	00054783          	lbu	a5,0(a0)
    1382:	16078a63          	beqz	a5,14f6 <printf+0x1ba>
    1386:	862a                	mv	a2,a0
        for (a = s; *s && *s != '%'; s++)
    1388:	19278163          	beq	a5,s2,150a <printf+0x1ce>
    138c:	00164783          	lbu	a5,1(a2)
    1390:	0605                	addi	a2,a2,1
    1392:	fbfd                	bnez	a5,1388 <printf+0x4c>
    1394:	84b2                	mv	s1,a2
        l = z - a;
    1396:	40a6043b          	subw	s0,a2,a0
    write(f, s, l);
    139a:	85aa                	mv	a1,a0
    139c:	8622                	mv	a2,s0
    139e:	4505                	li	a0,1
    13a0:	0e3000ef          	jal	ra,1c82 <write>
        if (l)
    13a4:	18041c63          	bnez	s0,153c <printf+0x200>
        if (s[1] == 0)
    13a8:	0014c783          	lbu	a5,1(s1)
    13ac:	14078563          	beqz	a5,14f6 <printf+0x1ba>
        switch (s[1])
    13b0:	1d478063          	beq	a5,s4,1570 <printf+0x234>
    13b4:	18fa6663          	bltu	s4,a5,1540 <printf+0x204>
    13b8:	06400713          	li	a4,100
    13bc:	1ae78063          	beq	a5,a4,155c <printf+0x220>
    13c0:	07000713          	li	a4,112
    13c4:	1ce79963          	bne	a5,a4,1596 <printf+0x25a>
            printptr(va_arg(ap, uint64));
    13c8:	6702                	ld	a4,0(sp)
    buf[i++] = '0';
    13ca:	01511423          	sh	s5,8(sp)
    write(f, s, l);
    13ce:	4649                	li	a2,18
            printptr(va_arg(ap, uint64));
    13d0:	631c                	ld	a5,0(a4)
    13d2:	0721                	addi	a4,a4,8
    13d4:	e03a                	sd	a4,0(sp)
    for (j = 0; j < (sizeof(uint64) * 2); j++, x <<= 4)
    13d6:	00479293          	slli	t0,a5,0x4
    13da:	00879f93          	slli	t6,a5,0x8
    13de:	00c79f13          	slli	t5,a5,0xc
    13e2:	01079e93          	slli	t4,a5,0x10
    13e6:	01479e13          	slli	t3,a5,0x14
    13ea:	01879313          	slli	t1,a5,0x18
    13ee:	01c79893          	slli	a7,a5,0x1c
    13f2:	02479813          	slli	a6,a5,0x24
    13f6:	02879513          	slli	a0,a5,0x28
    13fa:	02c79593          	slli	a1,a5,0x2c
    13fe:	03079693          	slli	a3,a5,0x30
    1402:	03479713          	slli	a4,a5,0x34
        buf[i++] = digits[x >> (sizeof(uint64) * 8 - 4)];
    1406:	03c7d413          	srli	s0,a5,0x3c
    140a:	01c7d39b          	srliw	t2,a5,0x1c
    140e:	03c2d293          	srli	t0,t0,0x3c
    1412:	03cfdf93          	srli	t6,t6,0x3c
    1416:	03cf5f13          	srli	t5,t5,0x3c
    141a:	03cede93          	srli	t4,t4,0x3c
    141e:	03ce5e13          	srli	t3,t3,0x3c
    1422:	03c35313          	srli	t1,t1,0x3c
    1426:	03c8d893          	srli	a7,a7,0x3c
    142a:	03c85813          	srli	a6,a6,0x3c
    142e:	9171                	srli	a0,a0,0x3c
    1430:	91f1                	srli	a1,a1,0x3c
    1432:	92f1                	srli	a3,a3,0x3c
    1434:	9371                	srli	a4,a4,0x3c
    1436:	96ce                	add	a3,a3,s3
    1438:	974e                	add	a4,a4,s3
    143a:	944e                	add	s0,s0,s3
    143c:	92ce                	add	t0,t0,s3
    143e:	9fce                	add	t6,t6,s3
    1440:	9f4e                	add	t5,t5,s3
    1442:	9ece                	add	t4,t4,s3
    1444:	9e4e                	add	t3,t3,s3
    1446:	934e                	add	t1,t1,s3
    1448:	98ce                	add	a7,a7,s3
    144a:	93ce                	add	t2,t2,s3
    144c:	984e                	add	a6,a6,s3
    144e:	954e                	add	a0,a0,s3
    1450:	95ce                	add	a1,a1,s3
    1452:	0006c083          	lbu	ra,0(a3)
    1456:	0002c283          	lbu	t0,0(t0)
    145a:	00074683          	lbu	a3,0(a4)
    145e:	000fcf83          	lbu	t6,0(t6)
    1462:	000f4f03          	lbu	t5,0(t5)
    1466:	000ece83          	lbu	t4,0(t4)
    146a:	000e4e03          	lbu	t3,0(t3)
    146e:	00034303          	lbu	t1,0(t1)
    1472:	0008c883          	lbu	a7,0(a7)
    1476:	0003c383          	lbu	t2,0(t2)
    147a:	00084803          	lbu	a6,0(a6)
    147e:	00054503          	lbu	a0,0(a0)
    1482:	0005c583          	lbu	a1,0(a1)
    1486:	00044403          	lbu	s0,0(s0)
    for (j = 0; j < (sizeof(uint64) * 2); j++, x <<= 4)
    148a:	03879713          	slli	a4,a5,0x38
        buf[i++] = digits[x >> (sizeof(uint64) * 8 - 4)];
    148e:	9371                	srli	a4,a4,0x3c
    1490:	8bbd                	andi	a5,a5,15
    1492:	974e                	add	a4,a4,s3
    1494:	97ce                	add	a5,a5,s3
    1496:	005105a3          	sb	t0,11(sp)
    149a:	01f10623          	sb	t6,12(sp)
    149e:	01e106a3          	sb	t5,13(sp)
    14a2:	01d10723          	sb	t4,14(sp)
    14a6:	01c107a3          	sb	t3,15(sp)
    14aa:	00610823          	sb	t1,16(sp)
    14ae:	011108a3          	sb	a7,17(sp)
    14b2:	00710923          	sb	t2,18(sp)
    14b6:	010109a3          	sb	a6,19(sp)
    14ba:	00a10a23          	sb	a0,20(sp)
    14be:	00b10aa3          	sb	a1,21(sp)
    14c2:	00110b23          	sb	ra,22(sp)
    14c6:	00d10ba3          	sb	a3,23(sp)
    14ca:	00810523          	sb	s0,10(sp)
    14ce:	00074703          	lbu	a4,0(a4)
    14d2:	0007c783          	lbu	a5,0(a5)
    write(f, s, l);
    14d6:	002c                	addi	a1,sp,8
    14d8:	4505                	li	a0,1
        buf[i++] = digits[x >> (sizeof(uint64) * 8 - 4)];
    14da:	00e10c23          	sb	a4,24(sp)
    14de:	00f10ca3          	sb	a5,25(sp)
    buf[i] = 0;
    14e2:	00010d23          	sb	zero,26(sp)
    write(f, s, l);
    14e6:	79c000ef          	jal	ra,1c82 <write>
            // Print unknown % sequence to draw attention.
            putchar('%');
            putchar(s[1]);
            break;
        }
        s += 2;
    14ea:	00248513          	addi	a0,s1,2
        if (!*s)
    14ee:	00054783          	lbu	a5,0(a0)
    14f2:	e8079ae3          	bnez	a5,1386 <printf+0x4a>
    }
    va_end(ap);
}
    14f6:	70a6                	ld	ra,104(sp)
    14f8:	7406                	ld	s0,96(sp)
    14fa:	64e6                	ld	s1,88(sp)
    14fc:	6946                	ld	s2,80(sp)
    14fe:	69a6                	ld	s3,72(sp)
    1500:	6a06                	ld	s4,64(sp)
    1502:	7ae2                	ld	s5,56(sp)
    1504:	7b42                	ld	s6,48(sp)
    1506:	614d                	addi	sp,sp,176
    1508:	8082                	ret
        for (z = s; s[0] == '%' && s[1] == '%'; z++, s += 2)
    150a:	00064783          	lbu	a5,0(a2)
    150e:	84b2                	mv	s1,a2
    1510:	01278963          	beq	a5,s2,1522 <printf+0x1e6>
    1514:	b549                	j	1396 <printf+0x5a>
    1516:	0024c783          	lbu	a5,2(s1)
    151a:	0605                	addi	a2,a2,1
    151c:	0489                	addi	s1,s1,2
    151e:	e7279ce3          	bne	a5,s2,1396 <printf+0x5a>
    1522:	0014c783          	lbu	a5,1(s1)
    1526:	ff2788e3          	beq	a5,s2,1516 <printf+0x1da>
        l = z - a;
    152a:	40a6043b          	subw	s0,a2,a0
    write(f, s, l);
    152e:	85aa                	mv	a1,a0
    1530:	8622                	mv	a2,s0
    1532:	4505                	li	a0,1
    1534:	74e000ef          	jal	ra,1c82 <write>
        if (l)
    1538:	e60408e3          	beqz	s0,13a8 <printf+0x6c>
    153c:	8526                	mv	a0,s1
    153e:	b581                	j	137e <printf+0x42>
        switch (s[1])
    1540:	07800713          	li	a4,120
    1544:	04e79963          	bne	a5,a4,1596 <printf+0x25a>
            printint(va_arg(ap, int), 16, 1);
    1548:	6782                	ld	a5,0(sp)
    154a:	45c1                	li	a1,16
    154c:	4388                	lw	a0,0(a5)
    154e:	07a1                	addi	a5,a5,8
    1550:	e03e                	sd	a5,0(sp)
    1552:	b71ff0ef          	jal	ra,10c2 <printint.constprop.0>
        s += 2;
    1556:	00248513          	addi	a0,s1,2
    155a:	bf51                	j	14ee <printf+0x1b2>
            printint(va_arg(ap, int), 10, 1);
    155c:	6782                	ld	a5,0(sp)
    155e:	45a9                	li	a1,10
    1560:	4388                	lw	a0,0(a5)
    1562:	07a1                	addi	a5,a5,8
    1564:	e03e                	sd	a5,0(sp)
    1566:	b5dff0ef          	jal	ra,10c2 <printint.constprop.0>
        s += 2;
    156a:	00248513          	addi	a0,s1,2
    156e:	b741                	j	14ee <printf+0x1b2>
            if ((a = va_arg(ap, char *)) == 0)
    1570:	6782                	ld	a5,0(sp)
    1572:	6380                	ld	s0,0(a5)
    1574:	07a1                	addi	a5,a5,8
    1576:	e03e                	sd	a5,0(sp)
    1578:	c031                	beqz	s0,15bc <printf+0x280>
            l = strnlen(a, 200);
    157a:	0c800593          	li	a1,200
    157e:	8522                	mv	a0,s0
    1580:	40a000ef          	jal	ra,198a <strnlen>
    write(f, s, l);
    1584:	0005061b          	sext.w	a2,a0
    1588:	85a2                	mv	a1,s0
    158a:	4505                	li	a0,1
    158c:	6f6000ef          	jal	ra,1c82 <write>
        s += 2;
    1590:	00248513          	addi	a0,s1,2
    1594:	bfa9                	j	14ee <printf+0x1b2>
    return write(stdout, &byte, 1);
    1596:	4605                	li	a2,1
    1598:	002c                	addi	a1,sp,8
    159a:	4505                	li	a0,1
    char byte = c;
    159c:	01210423          	sb	s2,8(sp)
    return write(stdout, &byte, 1);
    15a0:	6e2000ef          	jal	ra,1c82 <write>
    char byte = c;
    15a4:	0014c783          	lbu	a5,1(s1)
    return write(stdout, &byte, 1);
    15a8:	4605                	li	a2,1
    15aa:	002c                	addi	a1,sp,8
    15ac:	4505                	li	a0,1
    char byte = c;
    15ae:	00f10423          	sb	a5,8(sp)
    return write(stdout, &byte, 1);
    15b2:	6d0000ef          	jal	ra,1c82 <write>
        s += 2;
    15b6:	00248513          	addi	a0,s1,2
    15ba:	bf15                	j	14ee <printf+0x1b2>
                a = "(null)";
    15bc:	845a                	mv	s0,s6
    15be:	bf75                	j	157a <printf+0x23e>

00000000000015c0 <panic>:
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

void panic(char *m)
{
    15c0:	1141                	addi	sp,sp,-16
    15c2:	e406                	sd	ra,8(sp)
    puts(m);
    15c4:	d57ff0ef          	jal	ra,131a <puts>
    exit(-100);
}
    15c8:	60a2                	ld	ra,8(sp)
    exit(-100);
    15ca:	f9c00513          	li	a0,-100
}
    15ce:	0141                	addi	sp,sp,16
    exit(-100);
    15d0:	a709                	j	1cd2 <exit>

00000000000015d2 <isspace>:
#define HIGHS (ONES * (UCHAR_MAX / 2 + 1))
#define HASZERO(x) (((x)-ONES) & ~(x)&HIGHS)

int isspace(int c)
{
    return c == ' ' || (unsigned)c - '\t' < 5;
    15d2:	02000793          	li	a5,32
    15d6:	00f50663          	beq	a0,a5,15e2 <isspace+0x10>
    15da:	355d                	addiw	a0,a0,-9
    15dc:	00553513          	sltiu	a0,a0,5
    15e0:	8082                	ret
    15e2:	4505                	li	a0,1
}
    15e4:	8082                	ret

00000000000015e6 <isdigit>:

int isdigit(int c)
{
    return (unsigned)c - '0' < 10;
    15e6:	fd05051b          	addiw	a0,a0,-48
}
    15ea:	00a53513          	sltiu	a0,a0,10
    15ee:	8082                	ret

00000000000015f0 <atoi>:
    return c == ' ' || (unsigned)c - '\t' < 5;
    15f0:	02000613          	li	a2,32
    15f4:	4591                	li	a1,4

int atoi(const char *s)
{
    int n = 0, neg = 0;
    while (isspace(*s))
    15f6:	00054703          	lbu	a4,0(a0)
    return c == ' ' || (unsigned)c - '\t' < 5;
    15fa:	ff77069b          	addiw	a3,a4,-9
    15fe:	04c70d63          	beq	a4,a2,1658 <atoi+0x68>
    1602:	0007079b          	sext.w	a5,a4
    1606:	04d5f963          	bgeu	a1,a3,1658 <atoi+0x68>
        s++;
    switch (*s)
    160a:	02b00693          	li	a3,43
    160e:	04d70a63          	beq	a4,a3,1662 <atoi+0x72>
    1612:	02d00693          	li	a3,45
    1616:	06d70463          	beq	a4,a3,167e <atoi+0x8e>
        neg = 1;
    case '+':
        s++;
    }
    /* Compute n as a negative number to avoid overflow on INT_MIN */
    while (isdigit(*s))
    161a:	fd07859b          	addiw	a1,a5,-48
    161e:	4625                	li	a2,9
    1620:	873e                	mv	a4,a5
    1622:	86aa                	mv	a3,a0
    int n = 0, neg = 0;
    1624:	4e01                	li	t3,0
    while (isdigit(*s))
    1626:	04b66a63          	bltu	a2,a1,167a <atoi+0x8a>
    int n = 0, neg = 0;
    162a:	4501                	li	a0,0
    while (isdigit(*s))
    162c:	4825                	li	a6,9
    162e:	0016c603          	lbu	a2,1(a3)
        n = 10 * n - (*s++ - '0');
    1632:	0025179b          	slliw	a5,a0,0x2
    1636:	9d3d                	addw	a0,a0,a5
    1638:	fd07031b          	addiw	t1,a4,-48
    163c:	0015189b          	slliw	a7,a0,0x1
    while (isdigit(*s))
    1640:	fd06059b          	addiw	a1,a2,-48
        n = 10 * n - (*s++ - '0');
    1644:	0685                	addi	a3,a3,1
    1646:	4068853b          	subw	a0,a7,t1
    while (isdigit(*s))
    164a:	0006071b          	sext.w	a4,a2
    164e:	feb870e3          	bgeu	a6,a1,162e <atoi+0x3e>
    return neg ? n : -n;
    1652:	000e0563          	beqz	t3,165c <atoi+0x6c>
}
    1656:	8082                	ret
        s++;
    1658:	0505                	addi	a0,a0,1
    165a:	bf71                	j	15f6 <atoi+0x6>
    return neg ? n : -n;
    165c:	4113053b          	subw	a0,t1,a7
    1660:	8082                	ret
    while (isdigit(*s))
    1662:	00154783          	lbu	a5,1(a0)
    1666:	4625                	li	a2,9
        s++;
    1668:	00150693          	addi	a3,a0,1
    while (isdigit(*s))
    166c:	fd07859b          	addiw	a1,a5,-48
    1670:	0007871b          	sext.w	a4,a5
    int n = 0, neg = 0;
    1674:	4e01                	li	t3,0
    while (isdigit(*s))
    1676:	fab67ae3          	bgeu	a2,a1,162a <atoi+0x3a>
    167a:	4501                	li	a0,0
}
    167c:	8082                	ret
    while (isdigit(*s))
    167e:	00154783          	lbu	a5,1(a0)
    1682:	4625                	li	a2,9
        s++;
    1684:	00150693          	addi	a3,a0,1
    while (isdigit(*s))
    1688:	fd07859b          	addiw	a1,a5,-48
    168c:	0007871b          	sext.w	a4,a5
    1690:	feb665e3          	bltu	a2,a1,167a <atoi+0x8a>
        neg = 1;
    1694:	4e05                	li	t3,1
    1696:	bf51                	j	162a <atoi+0x3a>

0000000000001698 <memset>:

void *memset(void *dest, int c, size_t n)
{
    char *p = dest;
    for (int i = 0; i < n; ++i, *(p++) = c)
    1698:	16060d63          	beqz	a2,1812 <memset+0x17a>
    169c:	40a007b3          	neg	a5,a0
    16a0:	8b9d                	andi	a5,a5,7
    16a2:	00778713          	addi	a4,a5,7
    16a6:	482d                	li	a6,11
    16a8:	0ff5f593          	andi	a1,a1,255
    16ac:	fff60693          	addi	a3,a2,-1
    16b0:	17076263          	bltu	a4,a6,1814 <memset+0x17c>
    16b4:	16e6ea63          	bltu	a3,a4,1828 <memset+0x190>
    16b8:	16078563          	beqz	a5,1822 <memset+0x18a>
    16bc:	00b50023          	sb	a1,0(a0)
    16c0:	4705                	li	a4,1
    16c2:	00150e93          	addi	t4,a0,1
    16c6:	14e78c63          	beq	a5,a4,181e <memset+0x186>
    16ca:	00b500a3          	sb	a1,1(a0)
    16ce:	4709                	li	a4,2
    16d0:	00250e93          	addi	t4,a0,2
    16d4:	14e78d63          	beq	a5,a4,182e <memset+0x196>
    16d8:	00b50123          	sb	a1,2(a0)
    16dc:	470d                	li	a4,3
    16de:	00350e93          	addi	t4,a0,3
    16e2:	12e78b63          	beq	a5,a4,1818 <memset+0x180>
    16e6:	00b501a3          	sb	a1,3(a0)
    16ea:	4711                	li	a4,4
    16ec:	00450e93          	addi	t4,a0,4
    16f0:	14e78163          	beq	a5,a4,1832 <memset+0x19a>
    16f4:	00b50223          	sb	a1,4(a0)
    16f8:	4715                	li	a4,5
    16fa:	00550e93          	addi	t4,a0,5
    16fe:	12e78c63          	beq	a5,a4,1836 <memset+0x19e>
    1702:	00b502a3          	sb	a1,5(a0)
    1706:	471d                	li	a4,7
    1708:	00650e93          	addi	t4,a0,6
    170c:	12e79763          	bne	a5,a4,183a <memset+0x1a2>
    1710:	00750e93          	addi	t4,a0,7
    1714:	00b50323          	sb	a1,6(a0)
    1718:	4f1d                	li	t5,7
    171a:	00859713          	slli	a4,a1,0x8
    171e:	8f4d                	or	a4,a4,a1
    1720:	01059e13          	slli	t3,a1,0x10
    1724:	01c76e33          	or	t3,a4,t3
    1728:	01859313          	slli	t1,a1,0x18
    172c:	006e6333          	or	t1,t3,t1
    1730:	02059893          	slli	a7,a1,0x20
    1734:	011368b3          	or	a7,t1,a7
    1738:	02859813          	slli	a6,a1,0x28
    173c:	40f60333          	sub	t1,a2,a5
    1740:	0108e833          	or	a6,a7,a6
    1744:	03059693          	slli	a3,a1,0x30
    1748:	00d866b3          	or	a3,a6,a3
    174c:	03859713          	slli	a4,a1,0x38
    1750:	97aa                	add	a5,a5,a0
    1752:	ff837813          	andi	a6,t1,-8
    1756:	8f55                	or	a4,a4,a3
    1758:	00f806b3          	add	a3,a6,a5
    175c:	e398                	sd	a4,0(a5)
    175e:	07a1                	addi	a5,a5,8
    1760:	fed79ee3          	bne	a5,a3,175c <memset+0xc4>
    1764:	ff837693          	andi	a3,t1,-8
    1768:	00de87b3          	add	a5,t4,a3
    176c:	01e6873b          	addw	a4,a3,t5
    1770:	0ad30663          	beq	t1,a3,181c <memset+0x184>
    1774:	00b78023          	sb	a1,0(a5)
    1778:	0017069b          	addiw	a3,a4,1
    177c:	08c6fb63          	bgeu	a3,a2,1812 <memset+0x17a>
    1780:	00b780a3          	sb	a1,1(a5)
    1784:	0027069b          	addiw	a3,a4,2
    1788:	08c6f563          	bgeu	a3,a2,1812 <memset+0x17a>
    178c:	00b78123          	sb	a1,2(a5)
    1790:	0037069b          	addiw	a3,a4,3
    1794:	06c6ff63          	bgeu	a3,a2,1812 <memset+0x17a>
    1798:	00b781a3          	sb	a1,3(a5)
    179c:	0047069b          	addiw	a3,a4,4
    17a0:	06c6f963          	bgeu	a3,a2,1812 <memset+0x17a>
    17a4:	00b78223          	sb	a1,4(a5)
    17a8:	0057069b          	addiw	a3,a4,5
    17ac:	06c6f363          	bgeu	a3,a2,1812 <memset+0x17a>
    17b0:	00b782a3          	sb	a1,5(a5)
    17b4:	0067069b          	addiw	a3,a4,6
    17b8:	04c6fd63          	bgeu	a3,a2,1812 <memset+0x17a>
    17bc:	00b78323          	sb	a1,6(a5)
    17c0:	0077069b          	addiw	a3,a4,7
    17c4:	04c6f763          	bgeu	a3,a2,1812 <memset+0x17a>
    17c8:	00b783a3          	sb	a1,7(a5)
    17cc:	0087069b          	addiw	a3,a4,8
    17d0:	04c6f163          	bgeu	a3,a2,1812 <memset+0x17a>
    17d4:	00b78423          	sb	a1,8(a5)
    17d8:	0097069b          	addiw	a3,a4,9
    17dc:	02c6fb63          	bgeu	a3,a2,1812 <memset+0x17a>
    17e0:	00b784a3          	sb	a1,9(a5)
    17e4:	00a7069b          	addiw	a3,a4,10
    17e8:	02c6f563          	bgeu	a3,a2,1812 <memset+0x17a>
    17ec:	00b78523          	sb	a1,10(a5)
    17f0:	00b7069b          	addiw	a3,a4,11
    17f4:	00c6ff63          	bgeu	a3,a2,1812 <memset+0x17a>
    17f8:	00b785a3          	sb	a1,11(a5)
    17fc:	00c7069b          	addiw	a3,a4,12
    1800:	00c6f963          	bgeu	a3,a2,1812 <memset+0x17a>
    1804:	00b78623          	sb	a1,12(a5)
    1808:	2735                	addiw	a4,a4,13
    180a:	00c77463          	bgeu	a4,a2,1812 <memset+0x17a>
    180e:	00b786a3          	sb	a1,13(a5)
        ;
    return dest;
}
    1812:	8082                	ret
    1814:	472d                	li	a4,11
    1816:	bd79                	j	16b4 <memset+0x1c>
    for (int i = 0; i < n; ++i, *(p++) = c)
    1818:	4f0d                	li	t5,3
    181a:	b701                	j	171a <memset+0x82>
    181c:	8082                	ret
    181e:	4f05                	li	t5,1
    1820:	bded                	j	171a <memset+0x82>
    1822:	8eaa                	mv	t4,a0
    1824:	4f01                	li	t5,0
    1826:	bdd5                	j	171a <memset+0x82>
    1828:	87aa                	mv	a5,a0
    182a:	4701                	li	a4,0
    182c:	b7a1                	j	1774 <memset+0xdc>
    182e:	4f09                	li	t5,2
    1830:	b5ed                	j	171a <memset+0x82>
    1832:	4f11                	li	t5,4
    1834:	b5dd                	j	171a <memset+0x82>
    1836:	4f15                	li	t5,5
    1838:	b5cd                	j	171a <memset+0x82>
    183a:	4f19                	li	t5,6
    183c:	bdf9                	j	171a <memset+0x82>

000000000000183e <strcmp>:

int strcmp(const char *l, const char *r)
{
    for (; *l == *r && *l; l++, r++)
    183e:	00054783          	lbu	a5,0(a0)
    1842:	0005c703          	lbu	a4,0(a1)
    1846:	00e79863          	bne	a5,a4,1856 <strcmp+0x18>
    184a:	0505                	addi	a0,a0,1
    184c:	0585                	addi	a1,a1,1
    184e:	fbe5                	bnez	a5,183e <strcmp>
    1850:	4501                	li	a0,0
        ;
    return *(unsigned char *)l - *(unsigned char *)r;
}
    1852:	9d19                	subw	a0,a0,a4
    1854:	8082                	ret
    return *(unsigned char *)l - *(unsigned char *)r;
    1856:	0007851b          	sext.w	a0,a5
    185a:	bfe5                	j	1852 <strcmp+0x14>

000000000000185c <strncmp>:

int strncmp(const char *_l, const char *_r, size_t n)
{
    const unsigned char *l = (void *)_l, *r = (void *)_r;
    if (!n--)
    185c:	ce05                	beqz	a2,1894 <strncmp+0x38>
        return 0;
    for (; *l && *r && n && *l == *r; l++, r++, n--)
    185e:	00054703          	lbu	a4,0(a0)
    1862:	0005c783          	lbu	a5,0(a1)
    1866:	cb0d                	beqz	a4,1898 <strncmp+0x3c>
    if (!n--)
    1868:	167d                	addi	a2,a2,-1
    186a:	00c506b3          	add	a3,a0,a2
    186e:	a819                	j	1884 <strncmp+0x28>
    for (; *l && *r && n && *l == *r; l++, r++, n--)
    1870:	00a68e63          	beq	a3,a0,188c <strncmp+0x30>
    1874:	0505                	addi	a0,a0,1
    1876:	00e79b63          	bne	a5,a4,188c <strncmp+0x30>
    187a:	00054703          	lbu	a4,0(a0)
        ;
    return *l - *r;
    187e:	0005c783          	lbu	a5,0(a1)
    for (; *l && *r && n && *l == *r; l++, r++, n--)
    1882:	cb19                	beqz	a4,1898 <strncmp+0x3c>
    1884:	0005c783          	lbu	a5,0(a1)
    1888:	0585                	addi	a1,a1,1
    188a:	f3fd                	bnez	a5,1870 <strncmp+0x14>
    return *l - *r;
    188c:	0007051b          	sext.w	a0,a4
    1890:	9d1d                	subw	a0,a0,a5
    1892:	8082                	ret
        return 0;
    1894:	4501                	li	a0,0
}
    1896:	8082                	ret
    1898:	4501                	li	a0,0
    return *l - *r;
    189a:	9d1d                	subw	a0,a0,a5
    189c:	8082                	ret

000000000000189e <strlen>:
size_t strlen(const char *s)
{
    const char *a = s;
    typedef size_t __attribute__((__may_alias__)) word;
    const word *w;
    for (; (uintptr_t)s % SS; s++)
    189e:	00757793          	andi	a5,a0,7
    18a2:	cf89                	beqz	a5,18bc <strlen+0x1e>
    18a4:	87aa                	mv	a5,a0
    18a6:	a029                	j	18b0 <strlen+0x12>
    18a8:	0785                	addi	a5,a5,1
    18aa:	0077f713          	andi	a4,a5,7
    18ae:	cb01                	beqz	a4,18be <strlen+0x20>
        if (!*s)
    18b0:	0007c703          	lbu	a4,0(a5)
    18b4:	fb75                	bnez	a4,18a8 <strlen+0xa>
    for (w = (const void *)s; !HASZERO(*w); w++)
        ;
    s = (const void *)w;
    for (; *s; s++)
        ;
    return s - a;
    18b6:	40a78533          	sub	a0,a5,a0
}
    18ba:	8082                	ret
    for (; (uintptr_t)s % SS; s++)
    18bc:	87aa                	mv	a5,a0
    for (w = (const void *)s; !HASZERO(*w); w++)
    18be:	6394                	ld	a3,0(a5)
    18c0:	00000597          	auipc	a1,0x0
    18c4:	6b85b583          	ld	a1,1720(a1) # 1f78 <__clone+0xc2>
    18c8:	00000617          	auipc	a2,0x0
    18cc:	6b863603          	ld	a2,1720(a2) # 1f80 <__clone+0xca>
    18d0:	a019                	j	18d6 <strlen+0x38>
    18d2:	6794                	ld	a3,8(a5)
    18d4:	07a1                	addi	a5,a5,8
    18d6:	00b68733          	add	a4,a3,a1
    18da:	fff6c693          	not	a3,a3
    18de:	8f75                	and	a4,a4,a3
    18e0:	8f71                	and	a4,a4,a2
    18e2:	db65                	beqz	a4,18d2 <strlen+0x34>
    for (; *s; s++)
    18e4:	0007c703          	lbu	a4,0(a5)
    18e8:	d779                	beqz	a4,18b6 <strlen+0x18>
    18ea:	0017c703          	lbu	a4,1(a5)
    18ee:	0785                	addi	a5,a5,1
    18f0:	d379                	beqz	a4,18b6 <strlen+0x18>
    18f2:	0017c703          	lbu	a4,1(a5)
    18f6:	0785                	addi	a5,a5,1
    18f8:	fb6d                	bnez	a4,18ea <strlen+0x4c>
    18fa:	bf75                	j	18b6 <strlen+0x18>

00000000000018fc <memchr>:

void *memchr(const void *src, int c, size_t n)
{
    const unsigned char *s = src;
    c = (unsigned char)c;
    for (; ((uintptr_t)s & ALIGN) && n && *s != c; s++, n--)
    18fc:	00757713          	andi	a4,a0,7
{
    1900:	87aa                	mv	a5,a0
    c = (unsigned char)c;
    1902:	0ff5f593          	andi	a1,a1,255
    for (; ((uintptr_t)s & ALIGN) && n && *s != c; s++, n--)
    1906:	cb19                	beqz	a4,191c <memchr+0x20>
    1908:	ce25                	beqz	a2,1980 <memchr+0x84>
    190a:	0007c703          	lbu	a4,0(a5)
    190e:	04b70e63          	beq	a4,a1,196a <memchr+0x6e>
    1912:	0785                	addi	a5,a5,1
    1914:	0077f713          	andi	a4,a5,7
    1918:	167d                	addi	a2,a2,-1
    191a:	f77d                	bnez	a4,1908 <memchr+0xc>
            ;
        s = (const void *)w;
    }
    for (; n && *s != c; s++, n--)
        ;
    return n ? (void *)s : 0;
    191c:	4501                	li	a0,0
    if (n && *s != c)
    191e:	c235                	beqz	a2,1982 <memchr+0x86>
    1920:	0007c703          	lbu	a4,0(a5)
    1924:	04b70363          	beq	a4,a1,196a <memchr+0x6e>
        size_t k = ONES * c;
    1928:	00000517          	auipc	a0,0x0
    192c:	66053503          	ld	a0,1632(a0) # 1f88 <__clone+0xd2>
        for (w = (const void *)s; n >= SS && !HASZERO(*w ^ k); w++, n -= SS)
    1930:	471d                	li	a4,7
        size_t k = ONES * c;
    1932:	02a58533          	mul	a0,a1,a0
        for (w = (const void *)s; n >= SS && !HASZERO(*w ^ k); w++, n -= SS)
    1936:	02c77a63          	bgeu	a4,a2,196a <memchr+0x6e>
    193a:	00000897          	auipc	a7,0x0
    193e:	63e8b883          	ld	a7,1598(a7) # 1f78 <__clone+0xc2>
    1942:	00000817          	auipc	a6,0x0
    1946:	63e83803          	ld	a6,1598(a6) # 1f80 <__clone+0xca>
    194a:	431d                	li	t1,7
    194c:	a029                	j	1956 <memchr+0x5a>
    194e:	1661                	addi	a2,a2,-8
    1950:	07a1                	addi	a5,a5,8
    1952:	02c37963          	bgeu	t1,a2,1984 <memchr+0x88>
    1956:	6398                	ld	a4,0(a5)
    1958:	8f29                	xor	a4,a4,a0
    195a:	011706b3          	add	a3,a4,a7
    195e:	fff74713          	not	a4,a4
    1962:	8f75                	and	a4,a4,a3
    1964:	01077733          	and	a4,a4,a6
    1968:	d37d                	beqz	a4,194e <memchr+0x52>
    196a:	853e                	mv	a0,a5
    196c:	97b2                	add	a5,a5,a2
    196e:	a021                	j	1976 <memchr+0x7a>
    for (; n && *s != c; s++, n--)
    1970:	0505                	addi	a0,a0,1
    1972:	00f50763          	beq	a0,a5,1980 <memchr+0x84>
    1976:	00054703          	lbu	a4,0(a0)
    197a:	feb71be3          	bne	a4,a1,1970 <memchr+0x74>
    197e:	8082                	ret
    return n ? (void *)s : 0;
    1980:	4501                	li	a0,0
}
    1982:	8082                	ret
    return n ? (void *)s : 0;
    1984:	4501                	li	a0,0
    for (; n && *s != c; s++, n--)
    1986:	f275                	bnez	a2,196a <memchr+0x6e>
}
    1988:	8082                	ret

000000000000198a <strnlen>:

size_t strnlen(const char *s, size_t n)
{
    198a:	1101                	addi	sp,sp,-32
    198c:	e822                	sd	s0,16(sp)
    const char *p = memchr(s, 0, n);
    198e:	862e                	mv	a2,a1
{
    1990:	842e                	mv	s0,a1
    const char *p = memchr(s, 0, n);
    1992:	4581                	li	a1,0
{
    1994:	e426                	sd	s1,8(sp)
    1996:	ec06                	sd	ra,24(sp)
    1998:	84aa                	mv	s1,a0
    const char *p = memchr(s, 0, n);
    199a:	f63ff0ef          	jal	ra,18fc <memchr>
    return p ? p - s : n;
    199e:	c519                	beqz	a0,19ac <strnlen+0x22>
}
    19a0:	60e2                	ld	ra,24(sp)
    19a2:	6442                	ld	s0,16(sp)
    return p ? p - s : n;
    19a4:	8d05                	sub	a0,a0,s1
}
    19a6:	64a2                	ld	s1,8(sp)
    19a8:	6105                	addi	sp,sp,32
    19aa:	8082                	ret
    19ac:	60e2                	ld	ra,24(sp)
    return p ? p - s : n;
    19ae:	8522                	mv	a0,s0
}
    19b0:	6442                	ld	s0,16(sp)
    19b2:	64a2                	ld	s1,8(sp)
    19b4:	6105                	addi	sp,sp,32
    19b6:	8082                	ret

00000000000019b8 <strcpy>:
char *strcpy(char *restrict d, const char *s)
{
    typedef size_t __attribute__((__may_alias__)) word;
    word *wd;
    const word *ws;
    if ((uintptr_t)s % SS == (uintptr_t)d % SS)
    19b8:	00b547b3          	xor	a5,a0,a1
    19bc:	8b9d                	andi	a5,a5,7
    19be:	eb95                	bnez	a5,19f2 <strcpy+0x3a>
    {
        for (; (uintptr_t)s % SS; s++, d++)
    19c0:	0075f793          	andi	a5,a1,7
    19c4:	e7b1                	bnez	a5,1a10 <strcpy+0x58>
            if (!(*d = *s))
                return d;
        wd = (void *)d;
        ws = (const void *)s;
        for (; !HASZERO(*ws); *wd++ = *ws++)
    19c6:	6198                	ld	a4,0(a1)
    19c8:	00000617          	auipc	a2,0x0
    19cc:	5b063603          	ld	a2,1456(a2) # 1f78 <__clone+0xc2>
    19d0:	00000817          	auipc	a6,0x0
    19d4:	5b083803          	ld	a6,1456(a6) # 1f80 <__clone+0xca>
    19d8:	a029                	j	19e2 <strcpy+0x2a>
    19da:	e118                	sd	a4,0(a0)
    19dc:	6598                	ld	a4,8(a1)
    19de:	05a1                	addi	a1,a1,8
    19e0:	0521                	addi	a0,a0,8
    19e2:	00c707b3          	add	a5,a4,a2
    19e6:	fff74693          	not	a3,a4
    19ea:	8ff5                	and	a5,a5,a3
    19ec:	0107f7b3          	and	a5,a5,a6
    19f0:	d7ed                	beqz	a5,19da <strcpy+0x22>
            ;
        d = (void *)wd;
        s = (const void *)ws;
    }
    for (; (*d = *s); s++, d++)
    19f2:	0005c783          	lbu	a5,0(a1)
    19f6:	00f50023          	sb	a5,0(a0)
    19fa:	c785                	beqz	a5,1a22 <strcpy+0x6a>
    19fc:	0015c783          	lbu	a5,1(a1)
    1a00:	0505                	addi	a0,a0,1
    1a02:	0585                	addi	a1,a1,1
    1a04:	00f50023          	sb	a5,0(a0)
    1a08:	fbf5                	bnez	a5,19fc <strcpy+0x44>
        ;
    return d;
}
    1a0a:	8082                	ret
        for (; (uintptr_t)s % SS; s++, d++)
    1a0c:	0505                	addi	a0,a0,1
    1a0e:	df45                	beqz	a4,19c6 <strcpy+0xe>
            if (!(*d = *s))
    1a10:	0005c783          	lbu	a5,0(a1)
        for (; (uintptr_t)s % SS; s++, d++)
    1a14:	0585                	addi	a1,a1,1
    1a16:	0075f713          	andi	a4,a1,7
            if (!(*d = *s))
    1a1a:	00f50023          	sb	a5,0(a0)
    1a1e:	f7fd                	bnez	a5,1a0c <strcpy+0x54>
}
    1a20:	8082                	ret
    1a22:	8082                	ret

0000000000001a24 <strncpy>:
char *strncpy(char *restrict d, const char *s, size_t n)
{
    typedef size_t __attribute__((__may_alias__)) word;
    word *wd;
    const word *ws;
    if (((uintptr_t)s & ALIGN) == ((uintptr_t)d & ALIGN))
    1a24:	00b547b3          	xor	a5,a0,a1
    1a28:	8b9d                	andi	a5,a5,7
    1a2a:	1a079863          	bnez	a5,1bda <strncpy+0x1b6>
    {
        for (; ((uintptr_t)s & ALIGN) && n && (*d = *s); n--, s++, d++)
    1a2e:	0075f793          	andi	a5,a1,7
    1a32:	16078463          	beqz	a5,1b9a <strncpy+0x176>
    1a36:	ea01                	bnez	a2,1a46 <strncpy+0x22>
    1a38:	a421                	j	1c40 <strncpy+0x21c>
    1a3a:	167d                	addi	a2,a2,-1
    1a3c:	0505                	addi	a0,a0,1
    1a3e:	14070e63          	beqz	a4,1b9a <strncpy+0x176>
    1a42:	1a060863          	beqz	a2,1bf2 <strncpy+0x1ce>
    1a46:	0005c783          	lbu	a5,0(a1)
    1a4a:	0585                	addi	a1,a1,1
    1a4c:	0075f713          	andi	a4,a1,7
    1a50:	00f50023          	sb	a5,0(a0)
    1a54:	f3fd                	bnez	a5,1a3a <strncpy+0x16>
    1a56:	4805                	li	a6,1
    1a58:	1a061863          	bnez	a2,1c08 <strncpy+0x1e4>
    1a5c:	40a007b3          	neg	a5,a0
    1a60:	8b9d                	andi	a5,a5,7
    1a62:	4681                	li	a3,0
    1a64:	18061a63          	bnez	a2,1bf8 <strncpy+0x1d4>
    1a68:	00778713          	addi	a4,a5,7
    1a6c:	45ad                	li	a1,11
    1a6e:	18b76363          	bltu	a4,a1,1bf4 <strncpy+0x1d0>
    1a72:	1ae6eb63          	bltu	a3,a4,1c28 <strncpy+0x204>
    1a76:	1a078363          	beqz	a5,1c1c <strncpy+0x1f8>
    for (int i = 0; i < n; ++i, *(p++) = c)
    1a7a:	00050023          	sb	zero,0(a0)
    1a7e:	4685                	li	a3,1
    1a80:	00150713          	addi	a4,a0,1
    1a84:	18d78f63          	beq	a5,a3,1c22 <strncpy+0x1fe>
    1a88:	000500a3          	sb	zero,1(a0)
    1a8c:	4689                	li	a3,2
    1a8e:	00250713          	addi	a4,a0,2
    1a92:	18d78e63          	beq	a5,a3,1c2e <strncpy+0x20a>
    1a96:	00050123          	sb	zero,2(a0)
    1a9a:	468d                	li	a3,3
    1a9c:	00350713          	addi	a4,a0,3
    1aa0:	16d78c63          	beq	a5,a3,1c18 <strncpy+0x1f4>
    1aa4:	000501a3          	sb	zero,3(a0)
    1aa8:	4691                	li	a3,4
    1aaa:	00450713          	addi	a4,a0,4
    1aae:	18d78263          	beq	a5,a3,1c32 <strncpy+0x20e>
    1ab2:	00050223          	sb	zero,4(a0)
    1ab6:	4695                	li	a3,5
    1ab8:	00550713          	addi	a4,a0,5
    1abc:	16d78d63          	beq	a5,a3,1c36 <strncpy+0x212>
    1ac0:	000502a3          	sb	zero,5(a0)
    1ac4:	469d                	li	a3,7
    1ac6:	00650713          	addi	a4,a0,6
    1aca:	16d79863          	bne	a5,a3,1c3a <strncpy+0x216>
    1ace:	00750713          	addi	a4,a0,7
    1ad2:	00050323          	sb	zero,6(a0)
    1ad6:	40f80833          	sub	a6,a6,a5
    1ada:	ff887593          	andi	a1,a6,-8
    1ade:	97aa                	add	a5,a5,a0
    1ae0:	95be                	add	a1,a1,a5
    1ae2:	0007b023          	sd	zero,0(a5)
    1ae6:	07a1                	addi	a5,a5,8
    1ae8:	feb79de3          	bne	a5,a1,1ae2 <strncpy+0xbe>
    1aec:	ff887593          	andi	a1,a6,-8
    1af0:	9ead                	addw	a3,a3,a1
    1af2:	00b707b3          	add	a5,a4,a1
    1af6:	12b80863          	beq	a6,a1,1c26 <strncpy+0x202>
    1afa:	00078023          	sb	zero,0(a5)
    1afe:	0016871b          	addiw	a4,a3,1
    1b02:	0ec77863          	bgeu	a4,a2,1bf2 <strncpy+0x1ce>
    1b06:	000780a3          	sb	zero,1(a5)
    1b0a:	0026871b          	addiw	a4,a3,2
    1b0e:	0ec77263          	bgeu	a4,a2,1bf2 <strncpy+0x1ce>
    1b12:	00078123          	sb	zero,2(a5)
    1b16:	0036871b          	addiw	a4,a3,3
    1b1a:	0cc77c63          	bgeu	a4,a2,1bf2 <strncpy+0x1ce>
    1b1e:	000781a3          	sb	zero,3(a5)
    1b22:	0046871b          	addiw	a4,a3,4
    1b26:	0cc77663          	bgeu	a4,a2,1bf2 <strncpy+0x1ce>
    1b2a:	00078223          	sb	zero,4(a5)
    1b2e:	0056871b          	addiw	a4,a3,5
    1b32:	0cc77063          	bgeu	a4,a2,1bf2 <strncpy+0x1ce>
    1b36:	000782a3          	sb	zero,5(a5)
    1b3a:	0066871b          	addiw	a4,a3,6
    1b3e:	0ac77a63          	bgeu	a4,a2,1bf2 <strncpy+0x1ce>
    1b42:	00078323          	sb	zero,6(a5)
    1b46:	0076871b          	addiw	a4,a3,7
    1b4a:	0ac77463          	bgeu	a4,a2,1bf2 <strncpy+0x1ce>
    1b4e:	000783a3          	sb	zero,7(a5)
    1b52:	0086871b          	addiw	a4,a3,8
    1b56:	08c77e63          	bgeu	a4,a2,1bf2 <strncpy+0x1ce>
    1b5a:	00078423          	sb	zero,8(a5)
    1b5e:	0096871b          	addiw	a4,a3,9
    1b62:	08c77863          	bgeu	a4,a2,1bf2 <strncpy+0x1ce>
    1b66:	000784a3          	sb	zero,9(a5)
    1b6a:	00a6871b          	addiw	a4,a3,10
    1b6e:	08c77263          	bgeu	a4,a2,1bf2 <strncpy+0x1ce>
    1b72:	00078523          	sb	zero,10(a5)
    1b76:	00b6871b          	addiw	a4,a3,11
    1b7a:	06c77c63          	bgeu	a4,a2,1bf2 <strncpy+0x1ce>
    1b7e:	000785a3          	sb	zero,11(a5)
    1b82:	00c6871b          	addiw	a4,a3,12
    1b86:	06c77663          	bgeu	a4,a2,1bf2 <strncpy+0x1ce>
    1b8a:	00078623          	sb	zero,12(a5)
    1b8e:	26b5                	addiw	a3,a3,13
    1b90:	06c6f163          	bgeu	a3,a2,1bf2 <strncpy+0x1ce>
    1b94:	000786a3          	sb	zero,13(a5)
    1b98:	8082                	ret
            ;
        if (!n || !*s)
    1b9a:	c645                	beqz	a2,1c42 <strncpy+0x21e>
    1b9c:	0005c783          	lbu	a5,0(a1)
    1ba0:	ea078be3          	beqz	a5,1a56 <strncpy+0x32>
            goto tail;
        wd = (void *)d;
        ws = (const void *)s;
        for (; n >= sizeof(size_t) && !HASZERO(*ws); n -= sizeof(size_t), ws++, wd++)
    1ba4:	479d                	li	a5,7
    1ba6:	02c7ff63          	bgeu	a5,a2,1be4 <strncpy+0x1c0>
    1baa:	00000897          	auipc	a7,0x0
    1bae:	3ce8b883          	ld	a7,974(a7) # 1f78 <__clone+0xc2>
    1bb2:	00000817          	auipc	a6,0x0
    1bb6:	3ce83803          	ld	a6,974(a6) # 1f80 <__clone+0xca>
    1bba:	431d                	li	t1,7
    1bbc:	6198                	ld	a4,0(a1)
    1bbe:	011707b3          	add	a5,a4,a7
    1bc2:	fff74693          	not	a3,a4
    1bc6:	8ff5                	and	a5,a5,a3
    1bc8:	0107f7b3          	and	a5,a5,a6
    1bcc:	ef81                	bnez	a5,1be4 <strncpy+0x1c0>
            *wd = *ws;
    1bce:	e118                	sd	a4,0(a0)
        for (; n >= sizeof(size_t) && !HASZERO(*ws); n -= sizeof(size_t), ws++, wd++)
    1bd0:	1661                	addi	a2,a2,-8
    1bd2:	05a1                	addi	a1,a1,8
    1bd4:	0521                	addi	a0,a0,8
    1bd6:	fec363e3          	bltu	t1,a2,1bbc <strncpy+0x198>
        d = (void *)wd;
        s = (const void *)ws;
    }
    for (; n && (*d = *s); n--, s++, d++)
    1bda:	e609                	bnez	a2,1be4 <strncpy+0x1c0>
    1bdc:	a08d                	j	1c3e <strncpy+0x21a>
    1bde:	167d                	addi	a2,a2,-1
    1be0:	0505                	addi	a0,a0,1
    1be2:	ca01                	beqz	a2,1bf2 <strncpy+0x1ce>
    1be4:	0005c783          	lbu	a5,0(a1)
    1be8:	0585                	addi	a1,a1,1
    1bea:	00f50023          	sb	a5,0(a0)
    1bee:	fbe5                	bnez	a5,1bde <strncpy+0x1ba>
        ;
tail:
    1bf0:	b59d                	j	1a56 <strncpy+0x32>
    memset(d, 0, n);
    return d;
}
    1bf2:	8082                	ret
    1bf4:	472d                	li	a4,11
    1bf6:	bdb5                	j	1a72 <strncpy+0x4e>
    1bf8:	00778713          	addi	a4,a5,7
    1bfc:	45ad                	li	a1,11
    1bfe:	fff60693          	addi	a3,a2,-1
    1c02:	e6b778e3          	bgeu	a4,a1,1a72 <strncpy+0x4e>
    1c06:	b7fd                	j	1bf4 <strncpy+0x1d0>
    1c08:	40a007b3          	neg	a5,a0
    1c0c:	8832                	mv	a6,a2
    1c0e:	8b9d                	andi	a5,a5,7
    1c10:	4681                	li	a3,0
    1c12:	e4060be3          	beqz	a2,1a68 <strncpy+0x44>
    1c16:	b7cd                	j	1bf8 <strncpy+0x1d4>
    for (int i = 0; i < n; ++i, *(p++) = c)
    1c18:	468d                	li	a3,3
    1c1a:	bd75                	j	1ad6 <strncpy+0xb2>
    1c1c:	872a                	mv	a4,a0
    1c1e:	4681                	li	a3,0
    1c20:	bd5d                	j	1ad6 <strncpy+0xb2>
    1c22:	4685                	li	a3,1
    1c24:	bd4d                	j	1ad6 <strncpy+0xb2>
    1c26:	8082                	ret
    1c28:	87aa                	mv	a5,a0
    1c2a:	4681                	li	a3,0
    1c2c:	b5f9                	j	1afa <strncpy+0xd6>
    1c2e:	4689                	li	a3,2
    1c30:	b55d                	j	1ad6 <strncpy+0xb2>
    1c32:	4691                	li	a3,4
    1c34:	b54d                	j	1ad6 <strncpy+0xb2>
    1c36:	4695                	li	a3,5
    1c38:	bd79                	j	1ad6 <strncpy+0xb2>
    1c3a:	4699                	li	a3,6
    1c3c:	bd69                	j	1ad6 <strncpy+0xb2>
    1c3e:	8082                	ret
    1c40:	8082                	ret
    1c42:	8082                	ret

0000000000001c44 <open>:
#include <unistd.h>

#include "syscall.h"

int open(const char *path, int flags)
{
    1c44:	87aa                	mv	a5,a0
    1c46:	862e                	mv	a2,a1
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
}

static inline long __syscall4(long n, long a, long b, long c, long d)
{
    register long a7 __asm__("a7") = n;
    1c48:	03800893          	li	a7,56
    register long a0 __asm__("a0") = a;
    1c4c:	f9c00513          	li	a0,-100
    register long a1 __asm__("a1") = b;
    1c50:	85be                	mv	a1,a5
    register long a2 __asm__("a2") = c;
    register long a3 __asm__("a3") = d;
    1c52:	4689                	li	a3,2
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3))
    1c54:	00000073          	ecall
    return syscall(SYS_openat, AT_FDCWD, path, flags, O_RDWR);
}
    1c58:	2501                	sext.w	a0,a0
    1c5a:	8082                	ret

0000000000001c5c <openat>:
    register long a7 __asm__("a7") = n;
    1c5c:	03800893          	li	a7,56
    register long a3 __asm__("a3") = d;
    1c60:	18000693          	li	a3,384
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3))
    1c64:	00000073          	ecall

int openat(int dirfd,const char *path, int flags)
{
    return syscall(SYS_openat, dirfd, path, flags, 0600);
}
    1c68:	2501                	sext.w	a0,a0
    1c6a:	8082                	ret

0000000000001c6c <close>:
    register long a7 __asm__("a7") = n;
    1c6c:	03900893          	li	a7,57
    __asm_syscall("r"(a7), "0"(a0))
    1c70:	00000073          	ecall

int close(int fd)
{
    return syscall(SYS_close, fd);
}
    1c74:	2501                	sext.w	a0,a0
    1c76:	8082                	ret

0000000000001c78 <read>:
    register long a7 __asm__("a7") = n;
    1c78:	03f00893          	li	a7,63
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1c7c:	00000073          	ecall

ssize_t read(int fd, void *buf, size_t len)
{
    return syscall(SYS_read, fd, buf, len);
}
    1c80:	8082                	ret

0000000000001c82 <write>:
    register long a7 __asm__("a7") = n;
    1c82:	04000893          	li	a7,64
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1c86:	00000073          	ecall

ssize_t write(int fd, const void *buf, size_t len)
{
    return syscall(SYS_write, fd, buf, len);
}
    1c8a:	8082                	ret

0000000000001c8c <getpid>:
    register long a7 __asm__("a7") = n;
    1c8c:	0ac00893          	li	a7,172
    __asm_syscall("r"(a7))
    1c90:	00000073          	ecall

pid_t getpid(void)
{
    return syscall(SYS_getpid);
}
    1c94:	2501                	sext.w	a0,a0
    1c96:	8082                	ret

0000000000001c98 <getppid>:
    register long a7 __asm__("a7") = n;
    1c98:	0ad00893          	li	a7,173
    __asm_syscall("r"(a7))
    1c9c:	00000073          	ecall

pid_t getppid(void)
{
    return syscall(SYS_getppid);
}
    1ca0:	2501                	sext.w	a0,a0
    1ca2:	8082                	ret

0000000000001ca4 <sched_yield>:
    register long a7 __asm__("a7") = n;
    1ca4:	07c00893          	li	a7,124
    __asm_syscall("r"(a7))
    1ca8:	00000073          	ecall

int sched_yield(void)
{
    return syscall(SYS_sched_yield);
}
    1cac:	2501                	sext.w	a0,a0
    1cae:	8082                	ret

0000000000001cb0 <fork>:
    register long a7 __asm__("a7") = n;
    1cb0:	0dc00893          	li	a7,220
    register long a0 __asm__("a0") = a;
    1cb4:	4545                	li	a0,17
    register long a1 __asm__("a1") = b;
    1cb6:	4581                	li	a1,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1cb8:	00000073          	ecall

pid_t fork(void)
{
    return syscall(SYS_clone, SIGCHLD, 0);
}
    1cbc:	2501                	sext.w	a0,a0
    1cbe:	8082                	ret

0000000000001cc0 <clone>:

pid_t clone(int (*fn)(void *arg), void *arg, void *stack, size_t stack_size, unsigned long flags)
{
    1cc0:	85b2                	mv	a1,a2
    1cc2:	863a                	mv	a2,a4
    if (stack)
    1cc4:	c191                	beqz	a1,1cc8 <clone+0x8>
	stack += stack_size;
    1cc6:	95b6                	add	a1,a1,a3

    return __clone(fn, stack, flags, NULL, NULL, NULL);
    1cc8:	4781                	li	a5,0
    1cca:	4701                	li	a4,0
    1ccc:	4681                	li	a3,0
    1cce:	2601                	sext.w	a2,a2
    1cd0:	a2dd                	j	1eb6 <__clone>

0000000000001cd2 <exit>:
    register long a7 __asm__("a7") = n;
    1cd2:	05d00893          	li	a7,93
    __asm_syscall("r"(a7), "0"(a0))
    1cd6:	00000073          	ecall
    //return syscall(SYS_clone, fn, stack, flags, NULL, NULL, NULL);
}
void exit(int code)
{
    syscall(SYS_exit, code);
}
    1cda:	8082                	ret

0000000000001cdc <waitpid>:
    register long a7 __asm__("a7") = n;
    1cdc:	10400893          	li	a7,260
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1ce0:	00000073          	ecall

int waitpid(int pid, int *code, int options)
{
    return syscall(SYS_wait4, pid, code, options);
}
    1ce4:	2501                	sext.w	a0,a0
    1ce6:	8082                	ret

0000000000001ce8 <exec>:
    register long a7 __asm__("a7") = n;
    1ce8:	0dd00893          	li	a7,221
    __asm_syscall("r"(a7), "0"(a0))
    1cec:	00000073          	ecall

int exec(char *name)
{
    return syscall(SYS_execve, name);
}
    1cf0:	2501                	sext.w	a0,a0
    1cf2:	8082                	ret

0000000000001cf4 <execve>:
    register long a7 __asm__("a7") = n;
    1cf4:	0dd00893          	li	a7,221
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1cf8:	00000073          	ecall

int execve(const char *name, char *const argv[], char *const argp[])
{
    return syscall(SYS_execve, name, argv, argp);
}
    1cfc:	2501                	sext.w	a0,a0
    1cfe:	8082                	ret

0000000000001d00 <times>:
    register long a7 __asm__("a7") = n;
    1d00:	09900893          	li	a7,153
    __asm_syscall("r"(a7), "0"(a0))
    1d04:	00000073          	ecall

int times(void *mytimes)
{
	return syscall(SYS_times, mytimes);
}
    1d08:	2501                	sext.w	a0,a0
    1d0a:	8082                	ret

0000000000001d0c <get_time>:

int64 get_time()
{
    1d0c:	1141                	addi	sp,sp,-16
    register long a7 __asm__("a7") = n;
    1d0e:	0a900893          	li	a7,169
    register long a0 __asm__("a0") = a;
    1d12:	850a                	mv	a0,sp
    register long a1 __asm__("a1") = b;
    1d14:	4581                	li	a1,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1d16:	00000073          	ecall
    TimeVal time;
    int err = sys_get_time(&time, 0);
    if (err == 0)
    1d1a:	2501                	sext.w	a0,a0
    1d1c:	ed09                	bnez	a0,1d36 <get_time+0x2a>
    {
        return ((time.sec & 0xffff) * 1000 + time.usec / 1000);
    1d1e:	67a2                	ld	a5,8(sp)
    1d20:	3e800713          	li	a4,1000
    1d24:	00015503          	lhu	a0,0(sp)
    1d28:	02e7d7b3          	divu	a5,a5,a4
    1d2c:	02e50533          	mul	a0,a0,a4
    1d30:	953e                	add	a0,a0,a5
    }
    else
    {
        return -1;
    }
}
    1d32:	0141                	addi	sp,sp,16
    1d34:	8082                	ret
        return -1;
    1d36:	557d                	li	a0,-1
    1d38:	bfed                	j	1d32 <get_time+0x26>

0000000000001d3a <sys_get_time>:
    register long a7 __asm__("a7") = n;
    1d3a:	0a900893          	li	a7,169
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1d3e:	00000073          	ecall

int sys_get_time(TimeVal *ts, int tz)
{
    return syscall(SYS_gettimeofday, ts, tz);
}
    1d42:	2501                	sext.w	a0,a0
    1d44:	8082                	ret

0000000000001d46 <time>:
    register long a7 __asm__("a7") = n;
    1d46:	42600893          	li	a7,1062
    __asm_syscall("r"(a7), "0"(a0))
    1d4a:	00000073          	ecall

int time(unsigned long *tloc)
{
    return syscall(SYS_time, tloc);
}
    1d4e:	2501                	sext.w	a0,a0
    1d50:	8082                	ret

0000000000001d52 <sleep>:

int sleep(unsigned long long time)
{
    1d52:	1141                	addi	sp,sp,-16
    TimeVal tv = {.sec = time, .usec = 0};
    1d54:	e02a                	sd	a0,0(sp)
    register long a0 __asm__("a0") = a;
    1d56:	850a                	mv	a0,sp
    1d58:	e402                	sd	zero,8(sp)
    register long a7 __asm__("a7") = n;
    1d5a:	06500893          	li	a7,101
    register long a1 __asm__("a1") = b;
    1d5e:	85aa                	mv	a1,a0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1d60:	00000073          	ecall
    if (syscall(SYS_nanosleep, &tv, &tv)) return tv.sec;
    1d64:	e501                	bnez	a0,1d6c <sleep+0x1a>
    return 0;
    1d66:	4501                	li	a0,0
}
    1d68:	0141                	addi	sp,sp,16
    1d6a:	8082                	ret
    if (syscall(SYS_nanosleep, &tv, &tv)) return tv.sec;
    1d6c:	4502                	lw	a0,0(sp)
}
    1d6e:	0141                	addi	sp,sp,16
    1d70:	8082                	ret

0000000000001d72 <set_priority>:
    register long a7 __asm__("a7") = n;
    1d72:	08c00893          	li	a7,140
    __asm_syscall("r"(a7), "0"(a0))
    1d76:	00000073          	ecall

int set_priority(int prio)
{
    return syscall(SYS_setpriority, prio);
}
    1d7a:	2501                	sext.w	a0,a0
    1d7c:	8082                	ret

0000000000001d7e <mmap>:
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4))
}

static inline long __syscall6(long n, long a, long b, long c, long d, long e, long f)
{
    register long a7 __asm__("a7") = n;
    1d7e:	0de00893          	li	a7,222
    register long a1 __asm__("a1") = b;
    register long a2 __asm__("a2") = c;
    register long a3 __asm__("a3") = d;
    register long a4 __asm__("a4") = e;
    register long a5 __asm__("a5") = f;
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4), "r"(a5))
    1d82:	00000073          	ecall

void *mmap(void *start, size_t len, int prot, int flags, int fd, off_t off)
{
    return syscall(SYS_mmap, start, len, prot, flags, fd, off);
}
    1d86:	8082                	ret

0000000000001d88 <munmap>:
    register long a7 __asm__("a7") = n;
    1d88:	0d700893          	li	a7,215
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1d8c:	00000073          	ecall

int munmap(void *start, size_t len)
{
    return syscall(SYS_munmap, start, len);
}
    1d90:	2501                	sext.w	a0,a0
    1d92:	8082                	ret

0000000000001d94 <wait>:

int wait(int *code)
{
    1d94:	85aa                	mv	a1,a0
    register long a7 __asm__("a7") = n;
    1d96:	10400893          	li	a7,260
    register long a0 __asm__("a0") = a;
    1d9a:	557d                	li	a0,-1
    register long a2 __asm__("a2") = c;
    1d9c:	4601                	li	a2,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1d9e:	00000073          	ecall
    return waitpid(-1, code, 0);
}
    1da2:	2501                	sext.w	a0,a0
    1da4:	8082                	ret

0000000000001da6 <spawn>:
    register long a7 __asm__("a7") = n;
    1da6:	19000893          	li	a7,400
    __asm_syscall("r"(a7), "0"(a0))
    1daa:	00000073          	ecall

int spawn(char *file)
{
    return syscall(SYS_spawn, file);
}
    1dae:	2501                	sext.w	a0,a0
    1db0:	8082                	ret

0000000000001db2 <mailread>:
    register long a7 __asm__("a7") = n;
    1db2:	19100893          	li	a7,401
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1db6:	00000073          	ecall

int mailread(void *buf, int len)
{
    return syscall(SYS_mailread, buf, len);
}
    1dba:	2501                	sext.w	a0,a0
    1dbc:	8082                	ret

0000000000001dbe <mailwrite>:
    register long a7 __asm__("a7") = n;
    1dbe:	19200893          	li	a7,402
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1dc2:	00000073          	ecall

int mailwrite(int pid, void *buf, int len)
{
    return syscall(SYS_mailwrite, pid, buf, len);
}
    1dc6:	2501                	sext.w	a0,a0
    1dc8:	8082                	ret

0000000000001dca <fstat>:
    register long a7 __asm__("a7") = n;
    1dca:	05000893          	li	a7,80
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1dce:	00000073          	ecall

int fstat(int fd, struct kstat *st)
{
    return syscall(SYS_fstat, fd, st);
}
    1dd2:	2501                	sext.w	a0,a0
    1dd4:	8082                	ret

0000000000001dd6 <sys_linkat>:
    register long a4 __asm__("a4") = e;
    1dd6:	1702                	slli	a4,a4,0x20
    register long a7 __asm__("a7") = n;
    1dd8:	02500893          	li	a7,37
    register long a4 __asm__("a4") = e;
    1ddc:	9301                	srli	a4,a4,0x20
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4))
    1dde:	00000073          	ecall

int sys_linkat(int olddirfd, char *oldpath, int newdirfd, char *newpath, unsigned int flags)
{
    return syscall(SYS_linkat, olddirfd, oldpath, newdirfd, newpath, flags);
}
    1de2:	2501                	sext.w	a0,a0
    1de4:	8082                	ret

0000000000001de6 <sys_unlinkat>:
    register long a2 __asm__("a2") = c;
    1de6:	1602                	slli	a2,a2,0x20
    register long a7 __asm__("a7") = n;
    1de8:	02300893          	li	a7,35
    register long a2 __asm__("a2") = c;
    1dec:	9201                	srli	a2,a2,0x20
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1dee:	00000073          	ecall

int sys_unlinkat(int dirfd, char *path, unsigned int flags)
{
    return syscall(SYS_unlinkat, dirfd, path, flags);
}
    1df2:	2501                	sext.w	a0,a0
    1df4:	8082                	ret

0000000000001df6 <link>:

int link(char *old_path, char *new_path)
{
    1df6:	87aa                	mv	a5,a0
    1df8:	86ae                	mv	a3,a1
    register long a7 __asm__("a7") = n;
    1dfa:	02500893          	li	a7,37
    register long a0 __asm__("a0") = a;
    1dfe:	f9c00513          	li	a0,-100
    register long a1 __asm__("a1") = b;
    1e02:	85be                	mv	a1,a5
    register long a2 __asm__("a2") = c;
    1e04:	f9c00613          	li	a2,-100
    register long a4 __asm__("a4") = e;
    1e08:	4701                	li	a4,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4))
    1e0a:	00000073          	ecall
    return sys_linkat(AT_FDCWD, old_path, AT_FDCWD, new_path, 0);
}
    1e0e:	2501                	sext.w	a0,a0
    1e10:	8082                	ret

0000000000001e12 <unlink>:

int unlink(char *path)
{
    1e12:	85aa                	mv	a1,a0
    register long a7 __asm__("a7") = n;
    1e14:	02300893          	li	a7,35
    register long a0 __asm__("a0") = a;
    1e18:	f9c00513          	li	a0,-100
    register long a2 __asm__("a2") = c;
    1e1c:	4601                	li	a2,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1e1e:	00000073          	ecall
    return sys_unlinkat(AT_FDCWD, path, 0);
}
    1e22:	2501                	sext.w	a0,a0
    1e24:	8082                	ret

0000000000001e26 <uname>:
    register long a7 __asm__("a7") = n;
    1e26:	0a000893          	li	a7,160
    __asm_syscall("r"(a7), "0"(a0))
    1e2a:	00000073          	ecall

int uname(void *buf)
{
    return syscall(SYS_uname, buf);
}
    1e2e:	2501                	sext.w	a0,a0
    1e30:	8082                	ret

0000000000001e32 <brk>:
    register long a7 __asm__("a7") = n;
    1e32:	0d600893          	li	a7,214
    __asm_syscall("r"(a7), "0"(a0))
    1e36:	00000073          	ecall

int brk(void *addr)
{
    return syscall(SYS_brk, addr);
}
    1e3a:	2501                	sext.w	a0,a0
    1e3c:	8082                	ret

0000000000001e3e <getcwd>:
    register long a7 __asm__("a7") = n;
    1e3e:	48c5                	li	a7,17
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1e40:	00000073          	ecall

char *getcwd(char *buf, size_t size){
    return syscall(SYS_getcwd, buf, size);
}
    1e44:	8082                	ret

0000000000001e46 <chdir>:
    register long a7 __asm__("a7") = n;
    1e46:	03100893          	li	a7,49
    __asm_syscall("r"(a7), "0"(a0))
    1e4a:	00000073          	ecall

int chdir(const char *path){
    return syscall(SYS_chdir, path);
}
    1e4e:	2501                	sext.w	a0,a0
    1e50:	8082                	ret

0000000000001e52 <mkdir>:

int mkdir(const char *path, mode_t mode){
    1e52:	862e                	mv	a2,a1
    1e54:	87aa                	mv	a5,a0
    register long a2 __asm__("a2") = c;
    1e56:	1602                	slli	a2,a2,0x20
    register long a7 __asm__("a7") = n;
    1e58:	02200893          	li	a7,34
    register long a0 __asm__("a0") = a;
    1e5c:	f9c00513          	li	a0,-100
    register long a1 __asm__("a1") = b;
    1e60:	85be                	mv	a1,a5
    register long a2 __asm__("a2") = c;
    1e62:	9201                	srli	a2,a2,0x20
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1e64:	00000073          	ecall
    return syscall(SYS_mkdirat, AT_FDCWD, path, mode);
}
    1e68:	2501                	sext.w	a0,a0
    1e6a:	8082                	ret

0000000000001e6c <getdents>:
    register long a7 __asm__("a7") = n;
    1e6c:	03d00893          	li	a7,61
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1e70:	00000073          	ecall

int getdents(int fd, struct linux_dirent64 *dirp64, unsigned long len){
    //return syscall(SYS_getdents64, fd, dirp64, len);
    return syscall(SYS_getdents64, fd, dirp64, len);
}
    1e74:	2501                	sext.w	a0,a0
    1e76:	8082                	ret

0000000000001e78 <pipe>:
    register long a7 __asm__("a7") = n;
    1e78:	03b00893          	li	a7,59
    register long a1 __asm__("a1") = b;
    1e7c:	4581                	li	a1,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1e7e:	00000073          	ecall

int pipe(int fd[2]){
    return syscall(SYS_pipe2, fd, 0);
}
    1e82:	2501                	sext.w	a0,a0
    1e84:	8082                	ret

0000000000001e86 <dup>:
    register long a7 __asm__("a7") = n;
    1e86:	48dd                	li	a7,23
    __asm_syscall("r"(a7), "0"(a0))
    1e88:	00000073          	ecall

int dup(int fd){
    return syscall(SYS_dup, fd);
}
    1e8c:	2501                	sext.w	a0,a0
    1e8e:	8082                	ret

0000000000001e90 <dup2>:
    register long a7 __asm__("a7") = n;
    1e90:	48e1                	li	a7,24
    register long a2 __asm__("a2") = c;
    1e92:	4601                	li	a2,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1e94:	00000073          	ecall

int dup2(int old, int new){
    return syscall(SYS_dup3, old, new, 0);
}
    1e98:	2501                	sext.w	a0,a0
    1e9a:	8082                	ret

0000000000001e9c <mount>:
    register long a7 __asm__("a7") = n;
    1e9c:	02800893          	li	a7,40
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4))
    1ea0:	00000073          	ecall

int mount(const char *special, const char *dir, const char *fstype, unsigned long flags, const void *data)
{
        return syscall(SYS_mount, special, dir, fstype, flags, data);
}
    1ea4:	2501                	sext.w	a0,a0
    1ea6:	8082                	ret

0000000000001ea8 <umount>:
    register long a7 __asm__("a7") = n;
    1ea8:	02700893          	li	a7,39
    register long a1 __asm__("a1") = b;
    1eac:	4581                	li	a1,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1eae:	00000073          	ecall

int umount(const char *special)
{
        return syscall(SYS_umount2, special, 0);
}
    1eb2:	2501                	sext.w	a0,a0
    1eb4:	8082                	ret

0000000000001eb6 <__clone>:

.global __clone
.type  __clone, %function
__clone:
	# Save func and arg to stack
	addi a1, a1, -16
    1eb6:	15c1                	addi	a1,a1,-16
	sd a0, 0(a1)
    1eb8:	e188                	sd	a0,0(a1)
	sd a3, 8(a1)
    1eba:	e594                	sd	a3,8(a1)

	# Call SYS_clone
	mv a0, a2
    1ebc:	8532                	mv	a0,a2
	mv a2, a4
    1ebe:	863a                	mv	a2,a4
	mv a3, a5
    1ec0:	86be                	mv	a3,a5
	mv a4, a6
    1ec2:	8742                	mv	a4,a6
	li a7, 220 # SYS_clone
    1ec4:	0dc00893          	li	a7,220
	ecall
    1ec8:	00000073          	ecall

	beqz a0, 1f
    1ecc:	c111                	beqz	a0,1ed0 <__clone+0x1a>
	# Parent
	ret
    1ece:	8082                	ret

	# Child
1:      ld a1, 0(sp)
    1ed0:	6582                	ld	a1,0(sp)
	ld a0, 8(sp)
    1ed2:	6522                	ld	a0,8(sp)
	jalr a1
    1ed4:	9582                	jalr	a1

	# Exit
	li a7, 93 # SYS_exit
    1ed6:	05d00893          	li	a7,93
	ecall
    1eda:	00000073          	ecall
