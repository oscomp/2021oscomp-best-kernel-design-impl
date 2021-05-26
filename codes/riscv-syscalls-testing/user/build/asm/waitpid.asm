
/home/ubuntu/rust/ultra-os/codes/riscv-syscalls-testing/user/build/riscv64/waitpid:     file format elf64-littleriscv


Disassembly of section .text:

0000000000001000 <_start>:
.section .text.entry
.globl _start
_start:
    mv a0, sp
    1000:	850a                	mv	a0,sp
    tail __start_main
    1002:	a8c1                	j	10d2 <__start_main>

0000000000001004 <test_waitpid>:
#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"

int i = 1000;
void test_waitpid(void){
    1004:	1101                	addi	sp,sp,-32
    TEST_START(__func__);
    1006:	00001517          	auipc	a0,0x1
    100a:	f0250513          	addi	a0,a0,-254 # 1f08 <__clone+0x28>
void test_waitpid(void){
    100e:	ec06                	sd	ra,24(sp)
    1010:	e822                	sd	s0,16(sp)
    TEST_START(__func__);
    1012:	332000ef          	jal	ra,1344 <puts>
    1016:	00001517          	auipc	a0,0x1
    101a:	faa50513          	addi	a0,a0,-86 # 1fc0 <__func__.0>
    101e:	326000ef          	jal	ra,1344 <puts>
    1022:	00001517          	auipc	a0,0x1
    1026:	efe50513          	addi	a0,a0,-258 # 1f20 <__clone+0x40>
    102a:	31a000ef          	jal	ra,1344 <puts>
    int cpid, wstatus;
    cpid = fork();
    102e:	4ad000ef          	jal	ra,1cda <fork>
    assert(cpid != -1);
    1032:	57fd                	li	a5,-1
    cpid = fork();
    1034:	842a                	mv	s0,a0
    assert(cpid != -1);
    1036:	04f50863          	beq	a0,a5,1086 <test_waitpid+0x82>
    if(cpid == 0){
    103a:	ed21                	bnez	a0,1092 <test_waitpid+0x8e>
	while(i--);
    103c:	00001717          	auipc	a4,0x1
    1040:	faf72423          	sw	a5,-88(a4) # 1fe4 <i>
	sched_yield();
    1044:	48b000ef          	jal	ra,1cce <sched_yield>
	printf("This is child process\n");
    1048:	00001517          	auipc	a0,0x1
    104c:	f0850513          	addi	a0,a0,-248 # 1f50 <__clone+0x70>
    1050:	316000ef          	jal	ra,1366 <printf>
        exit(3);
    1054:	450d                	li	a0,3
    1056:	4a7000ef          	jal	ra,1cfc <exit>
	if(WEXITSTATUS(wstatus) == 3){
	    printf("  waitpid success.\n");
	}else printf("  waitpid error.\n");
	*/
    }
    TEST_END(__func__);
    105a:	00001517          	auipc	a0,0x1
    105e:	f3650513          	addi	a0,a0,-202 # 1f90 <__clone+0xb0>
    1062:	2e2000ef          	jal	ra,1344 <puts>
    1066:	00001517          	auipc	a0,0x1
    106a:	f5a50513          	addi	a0,a0,-166 # 1fc0 <__func__.0>
    106e:	2d6000ef          	jal	ra,1344 <puts>
    1072:	00001517          	auipc	a0,0x1
    1076:	eae50513          	addi	a0,a0,-338 # 1f20 <__clone+0x40>
    107a:	2ca000ef          	jal	ra,1344 <puts>
}
    107e:	60e2                	ld	ra,24(sp)
    1080:	6442                	ld	s0,16(sp)
    1082:	6105                	addi	sp,sp,32
    1084:	8082                	ret
    assert(cpid != -1);
    1086:	00001517          	auipc	a0,0x1
    108a:	eaa50513          	addi	a0,a0,-342 # 1f30 <__clone+0x50>
    108e:	55c000ef          	jal	ra,15ea <panic>
	pid_t ret = waitpid(cpid, &wstatus, 0);
    1092:	4601                	li	a2,0
    1094:	006c                	addi	a1,sp,12
    1096:	8522                	mv	a0,s0
    1098:	46f000ef          	jal	ra,1d06 <waitpid>
	assert(ret != -1);
    109c:	57fd                	li	a5,-1
    109e:	00f50b63          	beq	a0,a5,10b4 <test_waitpid+0xb0>
	printf("waitpid successfully.\nwstatus: %x\n", WEXITSTATUS(wstatus));
    10a2:	00d14583          	lbu	a1,13(sp)
    10a6:	00001517          	auipc	a0,0x1
    10aa:	ec250513          	addi	a0,a0,-318 # 1f68 <__clone+0x88>
    10ae:	2b8000ef          	jal	ra,1366 <printf>
    10b2:	b765                	j	105a <test_waitpid+0x56>
	assert(ret != -1);
    10b4:	00001517          	auipc	a0,0x1
    10b8:	e7c50513          	addi	a0,a0,-388 # 1f30 <__clone+0x50>
    10bc:	52e000ef          	jal	ra,15ea <panic>
    10c0:	b7cd                	j	10a2 <test_waitpid+0x9e>

00000000000010c2 <main>:

int main(void){
    10c2:	1141                	addi	sp,sp,-16
    10c4:	e406                	sd	ra,8(sp)
    test_waitpid();
    10c6:	f3fff0ef          	jal	ra,1004 <test_waitpid>
    return 0;
}
    10ca:	60a2                	ld	ra,8(sp)
    10cc:	4501                	li	a0,0
    10ce:	0141                	addi	sp,sp,16
    10d0:	8082                	ret

00000000000010d2 <__start_main>:
#include <unistd.h>

extern int main();

int __start_main(long *p)
{
    10d2:	85aa                	mv	a1,a0
	int argc = p[0];
	char **argv = (void *)(p+1);

	exit(main(argc, argv));
    10d4:	4108                	lw	a0,0(a0)
{
    10d6:	1141                	addi	sp,sp,-16
	exit(main(argc, argv));
    10d8:	05a1                	addi	a1,a1,8
{
    10da:	e406                	sd	ra,8(sp)
	exit(main(argc, argv));
    10dc:	fe7ff0ef          	jal	ra,10c2 <main>
    10e0:	41d000ef          	jal	ra,1cfc <exit>
	return 0;
}
    10e4:	60a2                	ld	ra,8(sp)
    10e6:	4501                	li	a0,0
    10e8:	0141                	addi	sp,sp,16
    10ea:	8082                	ret

00000000000010ec <printint.constprop.0>:
    write(f, s, l);
}

static char digits[] = "0123456789abcdef";

static void printint(int xx, int base, int sign)
    10ec:	7179                	addi	sp,sp,-48
    10ee:	f406                	sd	ra,40(sp)
{
    char buf[16 + 1];
    int i;
    uint x;

    if (sign && (sign = xx < 0))
    10f0:	12054b63          	bltz	a0,1226 <printint.constprop.0+0x13a>

    buf[16] = 0;
    i = 15;
    do
    {
        buf[i--] = digits[x % base];
    10f4:	02b577bb          	remuw	a5,a0,a1
    10f8:	00001617          	auipc	a2,0x1
    10fc:	ed860613          	addi	a2,a2,-296 # 1fd0 <digits>
    buf[16] = 0;
    1100:	00010c23          	sb	zero,24(sp)
        buf[i--] = digits[x % base];
    1104:	0005871b          	sext.w	a4,a1
    1108:	1782                	slli	a5,a5,0x20
    110a:	9381                	srli	a5,a5,0x20
    110c:	97b2                	add	a5,a5,a2
    110e:	0007c783          	lbu	a5,0(a5)
    } while ((x /= base) != 0);
    1112:	02b5583b          	divuw	a6,a0,a1
        buf[i--] = digits[x % base];
    1116:	00f10ba3          	sb	a5,23(sp)
    } while ((x /= base) != 0);
    111a:	1cb56363          	bltu	a0,a1,12e0 <printint.constprop.0+0x1f4>
        buf[i--] = digits[x % base];
    111e:	45b9                	li	a1,14
    1120:	02e877bb          	remuw	a5,a6,a4
    1124:	1782                	slli	a5,a5,0x20
    1126:	9381                	srli	a5,a5,0x20
    1128:	97b2                	add	a5,a5,a2
    112a:	0007c783          	lbu	a5,0(a5)
    } while ((x /= base) != 0);
    112e:	02e856bb          	divuw	a3,a6,a4
        buf[i--] = digits[x % base];
    1132:	00f10b23          	sb	a5,22(sp)
    } while ((x /= base) != 0);
    1136:	0ce86e63          	bltu	a6,a4,1212 <printint.constprop.0+0x126>
        buf[i--] = digits[x % base];
    113a:	02e6f5bb          	remuw	a1,a3,a4
    } while ((x /= base) != 0);
    113e:	02e6d7bb          	divuw	a5,a3,a4
        buf[i--] = digits[x % base];
    1142:	1582                	slli	a1,a1,0x20
    1144:	9181                	srli	a1,a1,0x20
    1146:	95b2                	add	a1,a1,a2
    1148:	0005c583          	lbu	a1,0(a1)
    114c:	00b10aa3          	sb	a1,21(sp)
    } while ((x /= base) != 0);
    1150:	0007859b          	sext.w	a1,a5
    1154:	12e6ec63          	bltu	a3,a4,128c <printint.constprop.0+0x1a0>
        buf[i--] = digits[x % base];
    1158:	02e7f6bb          	remuw	a3,a5,a4
    115c:	1682                	slli	a3,a3,0x20
    115e:	9281                	srli	a3,a3,0x20
    1160:	96b2                	add	a3,a3,a2
    1162:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    1166:	02e7d83b          	divuw	a6,a5,a4
        buf[i--] = digits[x % base];
    116a:	00d10a23          	sb	a3,20(sp)
    } while ((x /= base) != 0);
    116e:	12e5e863          	bltu	a1,a4,129e <printint.constprop.0+0x1b2>
        buf[i--] = digits[x % base];
    1172:	02e876bb          	remuw	a3,a6,a4
    1176:	1682                	slli	a3,a3,0x20
    1178:	9281                	srli	a3,a3,0x20
    117a:	96b2                	add	a3,a3,a2
    117c:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    1180:	02e855bb          	divuw	a1,a6,a4
        buf[i--] = digits[x % base];
    1184:	00d109a3          	sb	a3,19(sp)
    } while ((x /= base) != 0);
    1188:	12e86463          	bltu	a6,a4,12b0 <printint.constprop.0+0x1c4>
        buf[i--] = digits[x % base];
    118c:	02e5f6bb          	remuw	a3,a1,a4
    1190:	1682                	slli	a3,a3,0x20
    1192:	9281                	srli	a3,a3,0x20
    1194:	96b2                	add	a3,a3,a2
    1196:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    119a:	02e5d83b          	divuw	a6,a1,a4
        buf[i--] = digits[x % base];
    119e:	00d10923          	sb	a3,18(sp)
    } while ((x /= base) != 0);
    11a2:	0ce5ec63          	bltu	a1,a4,127a <printint.constprop.0+0x18e>
        buf[i--] = digits[x % base];
    11a6:	02e876bb          	remuw	a3,a6,a4
    11aa:	1682                	slli	a3,a3,0x20
    11ac:	9281                	srli	a3,a3,0x20
    11ae:	96b2                	add	a3,a3,a2
    11b0:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    11b4:	02e855bb          	divuw	a1,a6,a4
        buf[i--] = digits[x % base];
    11b8:	00d108a3          	sb	a3,17(sp)
    } while ((x /= base) != 0);
    11bc:	10e86963          	bltu	a6,a4,12ce <printint.constprop.0+0x1e2>
        buf[i--] = digits[x % base];
    11c0:	02e5f6bb          	remuw	a3,a1,a4
    11c4:	1682                	slli	a3,a3,0x20
    11c6:	9281                	srli	a3,a3,0x20
    11c8:	96b2                	add	a3,a3,a2
    11ca:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    11ce:	02e5d83b          	divuw	a6,a1,a4
        buf[i--] = digits[x % base];
    11d2:	00d10823          	sb	a3,16(sp)
    } while ((x /= base) != 0);
    11d6:	10e5e763          	bltu	a1,a4,12e4 <printint.constprop.0+0x1f8>
        buf[i--] = digits[x % base];
    11da:	02e876bb          	remuw	a3,a6,a4
    11de:	1682                	slli	a3,a3,0x20
    11e0:	9281                	srli	a3,a3,0x20
    11e2:	96b2                	add	a3,a3,a2
    11e4:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    11e8:	02e857bb          	divuw	a5,a6,a4
        buf[i--] = digits[x % base];
    11ec:	00d107a3          	sb	a3,15(sp)
    } while ((x /= base) != 0);
    11f0:	10e86363          	bltu	a6,a4,12f6 <printint.constprop.0+0x20a>
        buf[i--] = digits[x % base];
    11f4:	1782                	slli	a5,a5,0x20
    11f6:	9381                	srli	a5,a5,0x20
    11f8:	97b2                	add	a5,a5,a2
    11fa:	0007c783          	lbu	a5,0(a5)
    11fe:	4599                	li	a1,6
    1200:	00f10723          	sb	a5,14(sp)

    if (sign)
    1204:	00055763          	bgez	a0,1212 <printint.constprop.0+0x126>
        buf[i--] = '-';
    1208:	02d00793          	li	a5,45
    120c:	00f106a3          	sb	a5,13(sp)
        buf[i--] = digits[x % base];
    1210:	4595                	li	a1,5
    write(f, s, l);
    1212:	003c                	addi	a5,sp,8
    1214:	4641                	li	a2,16
    1216:	9e0d                	subw	a2,a2,a1
    1218:	4505                	li	a0,1
    121a:	95be                	add	a1,a1,a5
    121c:	291000ef          	jal	ra,1cac <write>
    i++;
    if (i < 0)
        puts("printint error");
    out(stdout, buf + i, 16 - i);
}
    1220:	70a2                	ld	ra,40(sp)
    1222:	6145                	addi	sp,sp,48
    1224:	8082                	ret
        x = -xx;
    1226:	40a0083b          	negw	a6,a0
        buf[i--] = digits[x % base];
    122a:	02b877bb          	remuw	a5,a6,a1
    122e:	00001617          	auipc	a2,0x1
    1232:	da260613          	addi	a2,a2,-606 # 1fd0 <digits>
    buf[16] = 0;
    1236:	00010c23          	sb	zero,24(sp)
        buf[i--] = digits[x % base];
    123a:	0005871b          	sext.w	a4,a1
    123e:	1782                	slli	a5,a5,0x20
    1240:	9381                	srli	a5,a5,0x20
    1242:	97b2                	add	a5,a5,a2
    1244:	0007c783          	lbu	a5,0(a5)
    } while ((x /= base) != 0);
    1248:	02b858bb          	divuw	a7,a6,a1
        buf[i--] = digits[x % base];
    124c:	00f10ba3          	sb	a5,23(sp)
    } while ((x /= base) != 0);
    1250:	06b86963          	bltu	a6,a1,12c2 <printint.constprop.0+0x1d6>
        buf[i--] = digits[x % base];
    1254:	02e8f7bb          	remuw	a5,a7,a4
    1258:	1782                	slli	a5,a5,0x20
    125a:	9381                	srli	a5,a5,0x20
    125c:	97b2                	add	a5,a5,a2
    125e:	0007c783          	lbu	a5,0(a5)
    } while ((x /= base) != 0);
    1262:	02e8d6bb          	divuw	a3,a7,a4
        buf[i--] = digits[x % base];
    1266:	00f10b23          	sb	a5,22(sp)
    } while ((x /= base) != 0);
    126a:	ece8f8e3          	bgeu	a7,a4,113a <printint.constprop.0+0x4e>
        buf[i--] = '-';
    126e:	02d00793          	li	a5,45
    1272:	00f10aa3          	sb	a5,21(sp)
        buf[i--] = digits[x % base];
    1276:	45b5                	li	a1,13
    1278:	bf69                	j	1212 <printint.constprop.0+0x126>
    127a:	45a9                	li	a1,10
    if (sign)
    127c:	f8055be3          	bgez	a0,1212 <printint.constprop.0+0x126>
        buf[i--] = '-';
    1280:	02d00793          	li	a5,45
    1284:	00f108a3          	sb	a5,17(sp)
        buf[i--] = digits[x % base];
    1288:	45a5                	li	a1,9
    128a:	b761                	j	1212 <printint.constprop.0+0x126>
    128c:	45b5                	li	a1,13
    if (sign)
    128e:	f80552e3          	bgez	a0,1212 <printint.constprop.0+0x126>
        buf[i--] = '-';
    1292:	02d00793          	li	a5,45
    1296:	00f10a23          	sb	a5,20(sp)
        buf[i--] = digits[x % base];
    129a:	45b1                	li	a1,12
    129c:	bf9d                	j	1212 <printint.constprop.0+0x126>
    129e:	45b1                	li	a1,12
    if (sign)
    12a0:	f60559e3          	bgez	a0,1212 <printint.constprop.0+0x126>
        buf[i--] = '-';
    12a4:	02d00793          	li	a5,45
    12a8:	00f109a3          	sb	a5,19(sp)
        buf[i--] = digits[x % base];
    12ac:	45ad                	li	a1,11
    12ae:	b795                	j	1212 <printint.constprop.0+0x126>
    12b0:	45ad                	li	a1,11
    if (sign)
    12b2:	f60550e3          	bgez	a0,1212 <printint.constprop.0+0x126>
        buf[i--] = '-';
    12b6:	02d00793          	li	a5,45
    12ba:	00f10923          	sb	a5,18(sp)
        buf[i--] = digits[x % base];
    12be:	45a9                	li	a1,10
    12c0:	bf89                	j	1212 <printint.constprop.0+0x126>
        buf[i--] = '-';
    12c2:	02d00793          	li	a5,45
    12c6:	00f10b23          	sb	a5,22(sp)
        buf[i--] = digits[x % base];
    12ca:	45b9                	li	a1,14
    12cc:	b799                	j	1212 <printint.constprop.0+0x126>
    12ce:	45a5                	li	a1,9
    if (sign)
    12d0:	f40551e3          	bgez	a0,1212 <printint.constprop.0+0x126>
        buf[i--] = '-';
    12d4:	02d00793          	li	a5,45
    12d8:	00f10823          	sb	a5,16(sp)
        buf[i--] = digits[x % base];
    12dc:	45a1                	li	a1,8
    12de:	bf15                	j	1212 <printint.constprop.0+0x126>
    i = 15;
    12e0:	45bd                	li	a1,15
    12e2:	bf05                	j	1212 <printint.constprop.0+0x126>
        buf[i--] = digits[x % base];
    12e4:	45a1                	li	a1,8
    if (sign)
    12e6:	f20556e3          	bgez	a0,1212 <printint.constprop.0+0x126>
        buf[i--] = '-';
    12ea:	02d00793          	li	a5,45
    12ee:	00f107a3          	sb	a5,15(sp)
        buf[i--] = digits[x % base];
    12f2:	459d                	li	a1,7
    12f4:	bf39                	j	1212 <printint.constprop.0+0x126>
    12f6:	459d                	li	a1,7
    if (sign)
    12f8:	f0055de3          	bgez	a0,1212 <printint.constprop.0+0x126>
        buf[i--] = '-';
    12fc:	02d00793          	li	a5,45
    1300:	00f10723          	sb	a5,14(sp)
        buf[i--] = digits[x % base];
    1304:	4599                	li	a1,6
    1306:	b731                	j	1212 <printint.constprop.0+0x126>

0000000000001308 <getchar>:
{
    1308:	1101                	addi	sp,sp,-32
    read(stdin, &byte, 1);
    130a:	00f10593          	addi	a1,sp,15
    130e:	4605                	li	a2,1
    1310:	4501                	li	a0,0
{
    1312:	ec06                	sd	ra,24(sp)
    char byte = 0;
    1314:	000107a3          	sb	zero,15(sp)
    read(stdin, &byte, 1);
    1318:	18b000ef          	jal	ra,1ca2 <read>
}
    131c:	60e2                	ld	ra,24(sp)
    131e:	00f14503          	lbu	a0,15(sp)
    1322:	6105                	addi	sp,sp,32
    1324:	8082                	ret

0000000000001326 <putchar>:
{
    1326:	1101                	addi	sp,sp,-32
    1328:	87aa                	mv	a5,a0
    return write(stdout, &byte, 1);
    132a:	00f10593          	addi	a1,sp,15
    132e:	4605                	li	a2,1
    1330:	4505                	li	a0,1
{
    1332:	ec06                	sd	ra,24(sp)
    char byte = c;
    1334:	00f107a3          	sb	a5,15(sp)
    return write(stdout, &byte, 1);
    1338:	175000ef          	jal	ra,1cac <write>
}
    133c:	60e2                	ld	ra,24(sp)
    133e:	2501                	sext.w	a0,a0
    1340:	6105                	addi	sp,sp,32
    1342:	8082                	ret

0000000000001344 <puts>:
{
    1344:	1141                	addi	sp,sp,-16
    1346:	e406                	sd	ra,8(sp)
    1348:	e022                	sd	s0,0(sp)
    134a:	842a                	mv	s0,a0
    r = -(write(stdout, s, strlen(s)) < 0);
    134c:	57c000ef          	jal	ra,18c8 <strlen>
    1350:	862a                	mv	a2,a0
    1352:	85a2                	mv	a1,s0
    1354:	4505                	li	a0,1
    1356:	157000ef          	jal	ra,1cac <write>
}
    135a:	60a2                	ld	ra,8(sp)
    135c:	6402                	ld	s0,0(sp)
    r = -(write(stdout, s, strlen(s)) < 0);
    135e:	957d                	srai	a0,a0,0x3f
    return r;
    1360:	2501                	sext.w	a0,a0
}
    1362:	0141                	addi	sp,sp,16
    1364:	8082                	ret

0000000000001366 <printf>:
    out(stdout, buf, i);
}

// Print to the console. only understands %d, %x, %p, %s.
void printf(const char *fmt, ...)
{
    1366:	7171                	addi	sp,sp,-176
    1368:	fc56                	sd	s5,56(sp)
    136a:	ed3e                	sd	a5,152(sp)
    buf[i++] = '0';
    136c:	7ae1                	lui	s5,0xffff8
    va_list ap;
    int cnt = 0, l = 0;
    char *a, *z, *s = (char *)fmt, str;
    int f = stdout;

    va_start(ap, fmt);
    136e:	18bc                	addi	a5,sp,120
{
    1370:	e8ca                	sd	s2,80(sp)
    1372:	e4ce                	sd	s3,72(sp)
    1374:	e0d2                	sd	s4,64(sp)
    1376:	f85a                	sd	s6,48(sp)
    1378:	f486                	sd	ra,104(sp)
    137a:	f0a2                	sd	s0,96(sp)
    137c:	eca6                	sd	s1,88(sp)
    137e:	fcae                	sd	a1,120(sp)
    1380:	e132                	sd	a2,128(sp)
    1382:	e536                	sd	a3,136(sp)
    1384:	e93a                	sd	a4,144(sp)
    1386:	f142                	sd	a6,160(sp)
    1388:	f546                	sd	a7,168(sp)
    va_start(ap, fmt);
    138a:	e03e                	sd	a5,0(sp)
    for (;;)
    {
        if (!*s)
            break;
        for (a = s; *s && *s != '%'; s++)
    138c:	02500913          	li	s2,37
        out(f, a, l);
        if (l)
            continue;
        if (s[1] == 0)
            break;
        switch (s[1])
    1390:	07300a13          	li	s4,115
        case 'p':
            printptr(va_arg(ap, uint64));
            break;
        case 's':
            if ((a = va_arg(ap, char *)) == 0)
                a = "(null)";
    1394:	00001b17          	auipc	s6,0x1
    1398:	c0cb0b13          	addi	s6,s6,-1012 # 1fa0 <__clone+0xc0>
    buf[i++] = '0';
    139c:	830aca93          	xori	s5,s5,-2000
        buf[i++] = digits[x >> (sizeof(uint64) * 8 - 4)];
    13a0:	00001997          	auipc	s3,0x1
    13a4:	c3098993          	addi	s3,s3,-976 # 1fd0 <digits>
        if (!*s)
    13a8:	00054783          	lbu	a5,0(a0)
    13ac:	16078a63          	beqz	a5,1520 <printf+0x1ba>
    13b0:	862a                	mv	a2,a0
        for (a = s; *s && *s != '%'; s++)
    13b2:	19278163          	beq	a5,s2,1534 <printf+0x1ce>
    13b6:	00164783          	lbu	a5,1(a2)
    13ba:	0605                	addi	a2,a2,1
    13bc:	fbfd                	bnez	a5,13b2 <printf+0x4c>
    13be:	84b2                	mv	s1,a2
        l = z - a;
    13c0:	40a6043b          	subw	s0,a2,a0
    write(f, s, l);
    13c4:	85aa                	mv	a1,a0
    13c6:	8622                	mv	a2,s0
    13c8:	4505                	li	a0,1
    13ca:	0e3000ef          	jal	ra,1cac <write>
        if (l)
    13ce:	18041c63          	bnez	s0,1566 <printf+0x200>
        if (s[1] == 0)
    13d2:	0014c783          	lbu	a5,1(s1)
    13d6:	14078563          	beqz	a5,1520 <printf+0x1ba>
        switch (s[1])
    13da:	1d478063          	beq	a5,s4,159a <printf+0x234>
    13de:	18fa6663          	bltu	s4,a5,156a <printf+0x204>
    13e2:	06400713          	li	a4,100
    13e6:	1ae78063          	beq	a5,a4,1586 <printf+0x220>
    13ea:	07000713          	li	a4,112
    13ee:	1ce79963          	bne	a5,a4,15c0 <printf+0x25a>
            printptr(va_arg(ap, uint64));
    13f2:	6702                	ld	a4,0(sp)
    buf[i++] = '0';
    13f4:	01511423          	sh	s5,8(sp)
    write(f, s, l);
    13f8:	4649                	li	a2,18
            printptr(va_arg(ap, uint64));
    13fa:	631c                	ld	a5,0(a4)
    13fc:	0721                	addi	a4,a4,8
    13fe:	e03a                	sd	a4,0(sp)
    for (j = 0; j < (sizeof(uint64) * 2); j++, x <<= 4)
    1400:	00479293          	slli	t0,a5,0x4
    1404:	00879f93          	slli	t6,a5,0x8
    1408:	00c79f13          	slli	t5,a5,0xc
    140c:	01079e93          	slli	t4,a5,0x10
    1410:	01479e13          	slli	t3,a5,0x14
    1414:	01879313          	slli	t1,a5,0x18
    1418:	01c79893          	slli	a7,a5,0x1c
    141c:	02479813          	slli	a6,a5,0x24
    1420:	02879513          	slli	a0,a5,0x28
    1424:	02c79593          	slli	a1,a5,0x2c
    1428:	03079693          	slli	a3,a5,0x30
    142c:	03479713          	slli	a4,a5,0x34
        buf[i++] = digits[x >> (sizeof(uint64) * 8 - 4)];
    1430:	03c7d413          	srli	s0,a5,0x3c
    1434:	01c7d39b          	srliw	t2,a5,0x1c
    1438:	03c2d293          	srli	t0,t0,0x3c
    143c:	03cfdf93          	srli	t6,t6,0x3c
    1440:	03cf5f13          	srli	t5,t5,0x3c
    1444:	03cede93          	srli	t4,t4,0x3c
    1448:	03ce5e13          	srli	t3,t3,0x3c
    144c:	03c35313          	srli	t1,t1,0x3c
    1450:	03c8d893          	srli	a7,a7,0x3c
    1454:	03c85813          	srli	a6,a6,0x3c
    1458:	9171                	srli	a0,a0,0x3c
    145a:	91f1                	srli	a1,a1,0x3c
    145c:	92f1                	srli	a3,a3,0x3c
    145e:	9371                	srli	a4,a4,0x3c
    1460:	96ce                	add	a3,a3,s3
    1462:	974e                	add	a4,a4,s3
    1464:	944e                	add	s0,s0,s3
    1466:	92ce                	add	t0,t0,s3
    1468:	9fce                	add	t6,t6,s3
    146a:	9f4e                	add	t5,t5,s3
    146c:	9ece                	add	t4,t4,s3
    146e:	9e4e                	add	t3,t3,s3
    1470:	934e                	add	t1,t1,s3
    1472:	98ce                	add	a7,a7,s3
    1474:	93ce                	add	t2,t2,s3
    1476:	984e                	add	a6,a6,s3
    1478:	954e                	add	a0,a0,s3
    147a:	95ce                	add	a1,a1,s3
    147c:	0006c083          	lbu	ra,0(a3)
    1480:	0002c283          	lbu	t0,0(t0)
    1484:	00074683          	lbu	a3,0(a4)
    1488:	000fcf83          	lbu	t6,0(t6)
    148c:	000f4f03          	lbu	t5,0(t5)
    1490:	000ece83          	lbu	t4,0(t4)
    1494:	000e4e03          	lbu	t3,0(t3)
    1498:	00034303          	lbu	t1,0(t1)
    149c:	0008c883          	lbu	a7,0(a7)
    14a0:	0003c383          	lbu	t2,0(t2)
    14a4:	00084803          	lbu	a6,0(a6)
    14a8:	00054503          	lbu	a0,0(a0)
    14ac:	0005c583          	lbu	a1,0(a1)
    14b0:	00044403          	lbu	s0,0(s0)
    for (j = 0; j < (sizeof(uint64) * 2); j++, x <<= 4)
    14b4:	03879713          	slli	a4,a5,0x38
        buf[i++] = digits[x >> (sizeof(uint64) * 8 - 4)];
    14b8:	9371                	srli	a4,a4,0x3c
    14ba:	8bbd                	andi	a5,a5,15
    14bc:	974e                	add	a4,a4,s3
    14be:	97ce                	add	a5,a5,s3
    14c0:	005105a3          	sb	t0,11(sp)
    14c4:	01f10623          	sb	t6,12(sp)
    14c8:	01e106a3          	sb	t5,13(sp)
    14cc:	01d10723          	sb	t4,14(sp)
    14d0:	01c107a3          	sb	t3,15(sp)
    14d4:	00610823          	sb	t1,16(sp)
    14d8:	011108a3          	sb	a7,17(sp)
    14dc:	00710923          	sb	t2,18(sp)
    14e0:	010109a3          	sb	a6,19(sp)
    14e4:	00a10a23          	sb	a0,20(sp)
    14e8:	00b10aa3          	sb	a1,21(sp)
    14ec:	00110b23          	sb	ra,22(sp)
    14f0:	00d10ba3          	sb	a3,23(sp)
    14f4:	00810523          	sb	s0,10(sp)
    14f8:	00074703          	lbu	a4,0(a4)
    14fc:	0007c783          	lbu	a5,0(a5)
    write(f, s, l);
    1500:	002c                	addi	a1,sp,8
    1502:	4505                	li	a0,1
        buf[i++] = digits[x >> (sizeof(uint64) * 8 - 4)];
    1504:	00e10c23          	sb	a4,24(sp)
    1508:	00f10ca3          	sb	a5,25(sp)
    buf[i] = 0;
    150c:	00010d23          	sb	zero,26(sp)
    write(f, s, l);
    1510:	79c000ef          	jal	ra,1cac <write>
            // Print unknown % sequence to draw attention.
            putchar('%');
            putchar(s[1]);
            break;
        }
        s += 2;
    1514:	00248513          	addi	a0,s1,2
        if (!*s)
    1518:	00054783          	lbu	a5,0(a0)
    151c:	e8079ae3          	bnez	a5,13b0 <printf+0x4a>
    }
    va_end(ap);
}
    1520:	70a6                	ld	ra,104(sp)
    1522:	7406                	ld	s0,96(sp)
    1524:	64e6                	ld	s1,88(sp)
    1526:	6946                	ld	s2,80(sp)
    1528:	69a6                	ld	s3,72(sp)
    152a:	6a06                	ld	s4,64(sp)
    152c:	7ae2                	ld	s5,56(sp)
    152e:	7b42                	ld	s6,48(sp)
    1530:	614d                	addi	sp,sp,176
    1532:	8082                	ret
        for (z = s; s[0] == '%' && s[1] == '%'; z++, s += 2)
    1534:	00064783          	lbu	a5,0(a2)
    1538:	84b2                	mv	s1,a2
    153a:	01278963          	beq	a5,s2,154c <printf+0x1e6>
    153e:	b549                	j	13c0 <printf+0x5a>
    1540:	0024c783          	lbu	a5,2(s1)
    1544:	0605                	addi	a2,a2,1
    1546:	0489                	addi	s1,s1,2
    1548:	e7279ce3          	bne	a5,s2,13c0 <printf+0x5a>
    154c:	0014c783          	lbu	a5,1(s1)
    1550:	ff2788e3          	beq	a5,s2,1540 <printf+0x1da>
        l = z - a;
    1554:	40a6043b          	subw	s0,a2,a0
    write(f, s, l);
    1558:	85aa                	mv	a1,a0
    155a:	8622                	mv	a2,s0
    155c:	4505                	li	a0,1
    155e:	74e000ef          	jal	ra,1cac <write>
        if (l)
    1562:	e60408e3          	beqz	s0,13d2 <printf+0x6c>
    1566:	8526                	mv	a0,s1
    1568:	b581                	j	13a8 <printf+0x42>
        switch (s[1])
    156a:	07800713          	li	a4,120
    156e:	04e79963          	bne	a5,a4,15c0 <printf+0x25a>
            printint(va_arg(ap, int), 16, 1);
    1572:	6782                	ld	a5,0(sp)
    1574:	45c1                	li	a1,16
    1576:	4388                	lw	a0,0(a5)
    1578:	07a1                	addi	a5,a5,8
    157a:	e03e                	sd	a5,0(sp)
    157c:	b71ff0ef          	jal	ra,10ec <printint.constprop.0>
        s += 2;
    1580:	00248513          	addi	a0,s1,2
    1584:	bf51                	j	1518 <printf+0x1b2>
            printint(va_arg(ap, int), 10, 1);
    1586:	6782                	ld	a5,0(sp)
    1588:	45a9                	li	a1,10
    158a:	4388                	lw	a0,0(a5)
    158c:	07a1                	addi	a5,a5,8
    158e:	e03e                	sd	a5,0(sp)
    1590:	b5dff0ef          	jal	ra,10ec <printint.constprop.0>
        s += 2;
    1594:	00248513          	addi	a0,s1,2
    1598:	b741                	j	1518 <printf+0x1b2>
            if ((a = va_arg(ap, char *)) == 0)
    159a:	6782                	ld	a5,0(sp)
    159c:	6380                	ld	s0,0(a5)
    159e:	07a1                	addi	a5,a5,8
    15a0:	e03e                	sd	a5,0(sp)
    15a2:	c031                	beqz	s0,15e6 <printf+0x280>
            l = strnlen(a, 200);
    15a4:	0c800593          	li	a1,200
    15a8:	8522                	mv	a0,s0
    15aa:	40a000ef          	jal	ra,19b4 <strnlen>
    write(f, s, l);
    15ae:	0005061b          	sext.w	a2,a0
    15b2:	85a2                	mv	a1,s0
    15b4:	4505                	li	a0,1
    15b6:	6f6000ef          	jal	ra,1cac <write>
        s += 2;
    15ba:	00248513          	addi	a0,s1,2
    15be:	bfa9                	j	1518 <printf+0x1b2>
    return write(stdout, &byte, 1);
    15c0:	4605                	li	a2,1
    15c2:	002c                	addi	a1,sp,8
    15c4:	4505                	li	a0,1
    char byte = c;
    15c6:	01210423          	sb	s2,8(sp)
    return write(stdout, &byte, 1);
    15ca:	6e2000ef          	jal	ra,1cac <write>
    char byte = c;
    15ce:	0014c783          	lbu	a5,1(s1)
    return write(stdout, &byte, 1);
    15d2:	4605                	li	a2,1
    15d4:	002c                	addi	a1,sp,8
    15d6:	4505                	li	a0,1
    char byte = c;
    15d8:	00f10423          	sb	a5,8(sp)
    return write(stdout, &byte, 1);
    15dc:	6d0000ef          	jal	ra,1cac <write>
        s += 2;
    15e0:	00248513          	addi	a0,s1,2
    15e4:	bf15                	j	1518 <printf+0x1b2>
                a = "(null)";
    15e6:	845a                	mv	s0,s6
    15e8:	bf75                	j	15a4 <printf+0x23e>

00000000000015ea <panic>:
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

void panic(char *m)
{
    15ea:	1141                	addi	sp,sp,-16
    15ec:	e406                	sd	ra,8(sp)
    puts(m);
    15ee:	d57ff0ef          	jal	ra,1344 <puts>
    exit(-100);
}
    15f2:	60a2                	ld	ra,8(sp)
    exit(-100);
    15f4:	f9c00513          	li	a0,-100
}
    15f8:	0141                	addi	sp,sp,16
    exit(-100);
    15fa:	a709                	j	1cfc <exit>

00000000000015fc <isspace>:
#define HIGHS (ONES * (UCHAR_MAX / 2 + 1))
#define HASZERO(x) (((x)-ONES) & ~(x)&HIGHS)

int isspace(int c)
{
    return c == ' ' || (unsigned)c - '\t' < 5;
    15fc:	02000793          	li	a5,32
    1600:	00f50663          	beq	a0,a5,160c <isspace+0x10>
    1604:	355d                	addiw	a0,a0,-9
    1606:	00553513          	sltiu	a0,a0,5
    160a:	8082                	ret
    160c:	4505                	li	a0,1
}
    160e:	8082                	ret

0000000000001610 <isdigit>:

int isdigit(int c)
{
    return (unsigned)c - '0' < 10;
    1610:	fd05051b          	addiw	a0,a0,-48
}
    1614:	00a53513          	sltiu	a0,a0,10
    1618:	8082                	ret

000000000000161a <atoi>:
    return c == ' ' || (unsigned)c - '\t' < 5;
    161a:	02000613          	li	a2,32
    161e:	4591                	li	a1,4

int atoi(const char *s)
{
    int n = 0, neg = 0;
    while (isspace(*s))
    1620:	00054703          	lbu	a4,0(a0)
    return c == ' ' || (unsigned)c - '\t' < 5;
    1624:	ff77069b          	addiw	a3,a4,-9
    1628:	04c70d63          	beq	a4,a2,1682 <atoi+0x68>
    162c:	0007079b          	sext.w	a5,a4
    1630:	04d5f963          	bgeu	a1,a3,1682 <atoi+0x68>
        s++;
    switch (*s)
    1634:	02b00693          	li	a3,43
    1638:	04d70a63          	beq	a4,a3,168c <atoi+0x72>
    163c:	02d00693          	li	a3,45
    1640:	06d70463          	beq	a4,a3,16a8 <atoi+0x8e>
        neg = 1;
    case '+':
        s++;
    }
    /* Compute n as a negative number to avoid overflow on INT_MIN */
    while (isdigit(*s))
    1644:	fd07859b          	addiw	a1,a5,-48
    1648:	4625                	li	a2,9
    164a:	873e                	mv	a4,a5
    164c:	86aa                	mv	a3,a0
    int n = 0, neg = 0;
    164e:	4e01                	li	t3,0
    while (isdigit(*s))
    1650:	04b66a63          	bltu	a2,a1,16a4 <atoi+0x8a>
    int n = 0, neg = 0;
    1654:	4501                	li	a0,0
    while (isdigit(*s))
    1656:	4825                	li	a6,9
    1658:	0016c603          	lbu	a2,1(a3)
        n = 10 * n - (*s++ - '0');
    165c:	0025179b          	slliw	a5,a0,0x2
    1660:	9d3d                	addw	a0,a0,a5
    1662:	fd07031b          	addiw	t1,a4,-48
    1666:	0015189b          	slliw	a7,a0,0x1
    while (isdigit(*s))
    166a:	fd06059b          	addiw	a1,a2,-48
        n = 10 * n - (*s++ - '0');
    166e:	0685                	addi	a3,a3,1
    1670:	4068853b          	subw	a0,a7,t1
    while (isdigit(*s))
    1674:	0006071b          	sext.w	a4,a2
    1678:	feb870e3          	bgeu	a6,a1,1658 <atoi+0x3e>
    return neg ? n : -n;
    167c:	000e0563          	beqz	t3,1686 <atoi+0x6c>
}
    1680:	8082                	ret
        s++;
    1682:	0505                	addi	a0,a0,1
    1684:	bf71                	j	1620 <atoi+0x6>
    return neg ? n : -n;
    1686:	4113053b          	subw	a0,t1,a7
    168a:	8082                	ret
    while (isdigit(*s))
    168c:	00154783          	lbu	a5,1(a0)
    1690:	4625                	li	a2,9
        s++;
    1692:	00150693          	addi	a3,a0,1
    while (isdigit(*s))
    1696:	fd07859b          	addiw	a1,a5,-48
    169a:	0007871b          	sext.w	a4,a5
    int n = 0, neg = 0;
    169e:	4e01                	li	t3,0
    while (isdigit(*s))
    16a0:	fab67ae3          	bgeu	a2,a1,1654 <atoi+0x3a>
    16a4:	4501                	li	a0,0
}
    16a6:	8082                	ret
    while (isdigit(*s))
    16a8:	00154783          	lbu	a5,1(a0)
    16ac:	4625                	li	a2,9
        s++;
    16ae:	00150693          	addi	a3,a0,1
    while (isdigit(*s))
    16b2:	fd07859b          	addiw	a1,a5,-48
    16b6:	0007871b          	sext.w	a4,a5
    16ba:	feb665e3          	bltu	a2,a1,16a4 <atoi+0x8a>
        neg = 1;
    16be:	4e05                	li	t3,1
    16c0:	bf51                	j	1654 <atoi+0x3a>

00000000000016c2 <memset>:

void *memset(void *dest, int c, size_t n)
{
    char *p = dest;
    for (int i = 0; i < n; ++i, *(p++) = c)
    16c2:	16060d63          	beqz	a2,183c <memset+0x17a>
    16c6:	40a007b3          	neg	a5,a0
    16ca:	8b9d                	andi	a5,a5,7
    16cc:	00778713          	addi	a4,a5,7
    16d0:	482d                	li	a6,11
    16d2:	0ff5f593          	andi	a1,a1,255
    16d6:	fff60693          	addi	a3,a2,-1
    16da:	17076263          	bltu	a4,a6,183e <memset+0x17c>
    16de:	16e6ea63          	bltu	a3,a4,1852 <memset+0x190>
    16e2:	16078563          	beqz	a5,184c <memset+0x18a>
    16e6:	00b50023          	sb	a1,0(a0)
    16ea:	4705                	li	a4,1
    16ec:	00150e93          	addi	t4,a0,1
    16f0:	14e78c63          	beq	a5,a4,1848 <memset+0x186>
    16f4:	00b500a3          	sb	a1,1(a0)
    16f8:	4709                	li	a4,2
    16fa:	00250e93          	addi	t4,a0,2
    16fe:	14e78d63          	beq	a5,a4,1858 <memset+0x196>
    1702:	00b50123          	sb	a1,2(a0)
    1706:	470d                	li	a4,3
    1708:	00350e93          	addi	t4,a0,3
    170c:	12e78b63          	beq	a5,a4,1842 <memset+0x180>
    1710:	00b501a3          	sb	a1,3(a0)
    1714:	4711                	li	a4,4
    1716:	00450e93          	addi	t4,a0,4
    171a:	14e78163          	beq	a5,a4,185c <memset+0x19a>
    171e:	00b50223          	sb	a1,4(a0)
    1722:	4715                	li	a4,5
    1724:	00550e93          	addi	t4,a0,5
    1728:	12e78c63          	beq	a5,a4,1860 <memset+0x19e>
    172c:	00b502a3          	sb	a1,5(a0)
    1730:	471d                	li	a4,7
    1732:	00650e93          	addi	t4,a0,6
    1736:	12e79763          	bne	a5,a4,1864 <memset+0x1a2>
    173a:	00750e93          	addi	t4,a0,7
    173e:	00b50323          	sb	a1,6(a0)
    1742:	4f1d                	li	t5,7
    1744:	00859713          	slli	a4,a1,0x8
    1748:	8f4d                	or	a4,a4,a1
    174a:	01059e13          	slli	t3,a1,0x10
    174e:	01c76e33          	or	t3,a4,t3
    1752:	01859313          	slli	t1,a1,0x18
    1756:	006e6333          	or	t1,t3,t1
    175a:	02059893          	slli	a7,a1,0x20
    175e:	011368b3          	or	a7,t1,a7
    1762:	02859813          	slli	a6,a1,0x28
    1766:	40f60333          	sub	t1,a2,a5
    176a:	0108e833          	or	a6,a7,a6
    176e:	03059693          	slli	a3,a1,0x30
    1772:	00d866b3          	or	a3,a6,a3
    1776:	03859713          	slli	a4,a1,0x38
    177a:	97aa                	add	a5,a5,a0
    177c:	ff837813          	andi	a6,t1,-8
    1780:	8f55                	or	a4,a4,a3
    1782:	00f806b3          	add	a3,a6,a5
    1786:	e398                	sd	a4,0(a5)
    1788:	07a1                	addi	a5,a5,8
    178a:	fed79ee3          	bne	a5,a3,1786 <memset+0xc4>
    178e:	ff837693          	andi	a3,t1,-8
    1792:	00de87b3          	add	a5,t4,a3
    1796:	01e6873b          	addw	a4,a3,t5
    179a:	0ad30663          	beq	t1,a3,1846 <memset+0x184>
    179e:	00b78023          	sb	a1,0(a5)
    17a2:	0017069b          	addiw	a3,a4,1
    17a6:	08c6fb63          	bgeu	a3,a2,183c <memset+0x17a>
    17aa:	00b780a3          	sb	a1,1(a5)
    17ae:	0027069b          	addiw	a3,a4,2
    17b2:	08c6f563          	bgeu	a3,a2,183c <memset+0x17a>
    17b6:	00b78123          	sb	a1,2(a5)
    17ba:	0037069b          	addiw	a3,a4,3
    17be:	06c6ff63          	bgeu	a3,a2,183c <memset+0x17a>
    17c2:	00b781a3          	sb	a1,3(a5)
    17c6:	0047069b          	addiw	a3,a4,4
    17ca:	06c6f963          	bgeu	a3,a2,183c <memset+0x17a>
    17ce:	00b78223          	sb	a1,4(a5)
    17d2:	0057069b          	addiw	a3,a4,5
    17d6:	06c6f363          	bgeu	a3,a2,183c <memset+0x17a>
    17da:	00b782a3          	sb	a1,5(a5)
    17de:	0067069b          	addiw	a3,a4,6
    17e2:	04c6fd63          	bgeu	a3,a2,183c <memset+0x17a>
    17e6:	00b78323          	sb	a1,6(a5)
    17ea:	0077069b          	addiw	a3,a4,7
    17ee:	04c6f763          	bgeu	a3,a2,183c <memset+0x17a>
    17f2:	00b783a3          	sb	a1,7(a5)
    17f6:	0087069b          	addiw	a3,a4,8
    17fa:	04c6f163          	bgeu	a3,a2,183c <memset+0x17a>
    17fe:	00b78423          	sb	a1,8(a5)
    1802:	0097069b          	addiw	a3,a4,9
    1806:	02c6fb63          	bgeu	a3,a2,183c <memset+0x17a>
    180a:	00b784a3          	sb	a1,9(a5)
    180e:	00a7069b          	addiw	a3,a4,10
    1812:	02c6f563          	bgeu	a3,a2,183c <memset+0x17a>
    1816:	00b78523          	sb	a1,10(a5)
    181a:	00b7069b          	addiw	a3,a4,11
    181e:	00c6ff63          	bgeu	a3,a2,183c <memset+0x17a>
    1822:	00b785a3          	sb	a1,11(a5)
    1826:	00c7069b          	addiw	a3,a4,12
    182a:	00c6f963          	bgeu	a3,a2,183c <memset+0x17a>
    182e:	00b78623          	sb	a1,12(a5)
    1832:	2735                	addiw	a4,a4,13
    1834:	00c77463          	bgeu	a4,a2,183c <memset+0x17a>
    1838:	00b786a3          	sb	a1,13(a5)
        ;
    return dest;
}
    183c:	8082                	ret
    183e:	472d                	li	a4,11
    1840:	bd79                	j	16de <memset+0x1c>
    for (int i = 0; i < n; ++i, *(p++) = c)
    1842:	4f0d                	li	t5,3
    1844:	b701                	j	1744 <memset+0x82>
    1846:	8082                	ret
    1848:	4f05                	li	t5,1
    184a:	bded                	j	1744 <memset+0x82>
    184c:	8eaa                	mv	t4,a0
    184e:	4f01                	li	t5,0
    1850:	bdd5                	j	1744 <memset+0x82>
    1852:	87aa                	mv	a5,a0
    1854:	4701                	li	a4,0
    1856:	b7a1                	j	179e <memset+0xdc>
    1858:	4f09                	li	t5,2
    185a:	b5ed                	j	1744 <memset+0x82>
    185c:	4f11                	li	t5,4
    185e:	b5dd                	j	1744 <memset+0x82>
    1860:	4f15                	li	t5,5
    1862:	b5cd                	j	1744 <memset+0x82>
    1864:	4f19                	li	t5,6
    1866:	bdf9                	j	1744 <memset+0x82>

0000000000001868 <strcmp>:

int strcmp(const char *l, const char *r)
{
    for (; *l == *r && *l; l++, r++)
    1868:	00054783          	lbu	a5,0(a0)
    186c:	0005c703          	lbu	a4,0(a1)
    1870:	00e79863          	bne	a5,a4,1880 <strcmp+0x18>
    1874:	0505                	addi	a0,a0,1
    1876:	0585                	addi	a1,a1,1
    1878:	fbe5                	bnez	a5,1868 <strcmp>
    187a:	4501                	li	a0,0
        ;
    return *(unsigned char *)l - *(unsigned char *)r;
}
    187c:	9d19                	subw	a0,a0,a4
    187e:	8082                	ret
    return *(unsigned char *)l - *(unsigned char *)r;
    1880:	0007851b          	sext.w	a0,a5
    1884:	bfe5                	j	187c <strcmp+0x14>

0000000000001886 <strncmp>:

int strncmp(const char *_l, const char *_r, size_t n)
{
    const unsigned char *l = (void *)_l, *r = (void *)_r;
    if (!n--)
    1886:	ce05                	beqz	a2,18be <strncmp+0x38>
        return 0;
    for (; *l && *r && n && *l == *r; l++, r++, n--)
    1888:	00054703          	lbu	a4,0(a0)
    188c:	0005c783          	lbu	a5,0(a1)
    1890:	cb0d                	beqz	a4,18c2 <strncmp+0x3c>
    if (!n--)
    1892:	167d                	addi	a2,a2,-1
    1894:	00c506b3          	add	a3,a0,a2
    1898:	a819                	j	18ae <strncmp+0x28>
    for (; *l && *r && n && *l == *r; l++, r++, n--)
    189a:	00a68e63          	beq	a3,a0,18b6 <strncmp+0x30>
    189e:	0505                	addi	a0,a0,1
    18a0:	00e79b63          	bne	a5,a4,18b6 <strncmp+0x30>
    18a4:	00054703          	lbu	a4,0(a0)
        ;
    return *l - *r;
    18a8:	0005c783          	lbu	a5,0(a1)
    for (; *l && *r && n && *l == *r; l++, r++, n--)
    18ac:	cb19                	beqz	a4,18c2 <strncmp+0x3c>
    18ae:	0005c783          	lbu	a5,0(a1)
    18b2:	0585                	addi	a1,a1,1
    18b4:	f3fd                	bnez	a5,189a <strncmp+0x14>
    return *l - *r;
    18b6:	0007051b          	sext.w	a0,a4
    18ba:	9d1d                	subw	a0,a0,a5
    18bc:	8082                	ret
        return 0;
    18be:	4501                	li	a0,0
}
    18c0:	8082                	ret
    18c2:	4501                	li	a0,0
    return *l - *r;
    18c4:	9d1d                	subw	a0,a0,a5
    18c6:	8082                	ret

00000000000018c8 <strlen>:
size_t strlen(const char *s)
{
    const char *a = s;
    typedef size_t __attribute__((__may_alias__)) word;
    const word *w;
    for (; (uintptr_t)s % SS; s++)
    18c8:	00757793          	andi	a5,a0,7
    18cc:	cf89                	beqz	a5,18e6 <strlen+0x1e>
    18ce:	87aa                	mv	a5,a0
    18d0:	a029                	j	18da <strlen+0x12>
    18d2:	0785                	addi	a5,a5,1
    18d4:	0077f713          	andi	a4,a5,7
    18d8:	cb01                	beqz	a4,18e8 <strlen+0x20>
        if (!*s)
    18da:	0007c703          	lbu	a4,0(a5)
    18de:	fb75                	bnez	a4,18d2 <strlen+0xa>
    for (w = (const void *)s; !HASZERO(*w); w++)
        ;
    s = (const void *)w;
    for (; *s; s++)
        ;
    return s - a;
    18e0:	40a78533          	sub	a0,a5,a0
}
    18e4:	8082                	ret
    for (; (uintptr_t)s % SS; s++)
    18e6:	87aa                	mv	a5,a0
    for (w = (const void *)s; !HASZERO(*w); w++)
    18e8:	6394                	ld	a3,0(a5)
    18ea:	00000597          	auipc	a1,0x0
    18ee:	6be5b583          	ld	a1,1726(a1) # 1fa8 <__clone+0xc8>
    18f2:	00000617          	auipc	a2,0x0
    18f6:	6be63603          	ld	a2,1726(a2) # 1fb0 <__clone+0xd0>
    18fa:	a019                	j	1900 <strlen+0x38>
    18fc:	6794                	ld	a3,8(a5)
    18fe:	07a1                	addi	a5,a5,8
    1900:	00b68733          	add	a4,a3,a1
    1904:	fff6c693          	not	a3,a3
    1908:	8f75                	and	a4,a4,a3
    190a:	8f71                	and	a4,a4,a2
    190c:	db65                	beqz	a4,18fc <strlen+0x34>
    for (; *s; s++)
    190e:	0007c703          	lbu	a4,0(a5)
    1912:	d779                	beqz	a4,18e0 <strlen+0x18>
    1914:	0017c703          	lbu	a4,1(a5)
    1918:	0785                	addi	a5,a5,1
    191a:	d379                	beqz	a4,18e0 <strlen+0x18>
    191c:	0017c703          	lbu	a4,1(a5)
    1920:	0785                	addi	a5,a5,1
    1922:	fb6d                	bnez	a4,1914 <strlen+0x4c>
    1924:	bf75                	j	18e0 <strlen+0x18>

0000000000001926 <memchr>:

void *memchr(const void *src, int c, size_t n)
{
    const unsigned char *s = src;
    c = (unsigned char)c;
    for (; ((uintptr_t)s & ALIGN) && n && *s != c; s++, n--)
    1926:	00757713          	andi	a4,a0,7
{
    192a:	87aa                	mv	a5,a0
    c = (unsigned char)c;
    192c:	0ff5f593          	andi	a1,a1,255
    for (; ((uintptr_t)s & ALIGN) && n && *s != c; s++, n--)
    1930:	cb19                	beqz	a4,1946 <memchr+0x20>
    1932:	ce25                	beqz	a2,19aa <memchr+0x84>
    1934:	0007c703          	lbu	a4,0(a5)
    1938:	04b70e63          	beq	a4,a1,1994 <memchr+0x6e>
    193c:	0785                	addi	a5,a5,1
    193e:	0077f713          	andi	a4,a5,7
    1942:	167d                	addi	a2,a2,-1
    1944:	f77d                	bnez	a4,1932 <memchr+0xc>
            ;
        s = (const void *)w;
    }
    for (; n && *s != c; s++, n--)
        ;
    return n ? (void *)s : 0;
    1946:	4501                	li	a0,0
    if (n && *s != c)
    1948:	c235                	beqz	a2,19ac <memchr+0x86>
    194a:	0007c703          	lbu	a4,0(a5)
    194e:	04b70363          	beq	a4,a1,1994 <memchr+0x6e>
        size_t k = ONES * c;
    1952:	00000517          	auipc	a0,0x0
    1956:	66653503          	ld	a0,1638(a0) # 1fb8 <__clone+0xd8>
        for (w = (const void *)s; n >= SS && !HASZERO(*w ^ k); w++, n -= SS)
    195a:	471d                	li	a4,7
        size_t k = ONES * c;
    195c:	02a58533          	mul	a0,a1,a0
        for (w = (const void *)s; n >= SS && !HASZERO(*w ^ k); w++, n -= SS)
    1960:	02c77a63          	bgeu	a4,a2,1994 <memchr+0x6e>
    1964:	00000897          	auipc	a7,0x0
    1968:	6448b883          	ld	a7,1604(a7) # 1fa8 <__clone+0xc8>
    196c:	00000817          	auipc	a6,0x0
    1970:	64483803          	ld	a6,1604(a6) # 1fb0 <__clone+0xd0>
    1974:	431d                	li	t1,7
    1976:	a029                	j	1980 <memchr+0x5a>
    1978:	1661                	addi	a2,a2,-8
    197a:	07a1                	addi	a5,a5,8
    197c:	02c37963          	bgeu	t1,a2,19ae <memchr+0x88>
    1980:	6398                	ld	a4,0(a5)
    1982:	8f29                	xor	a4,a4,a0
    1984:	011706b3          	add	a3,a4,a7
    1988:	fff74713          	not	a4,a4
    198c:	8f75                	and	a4,a4,a3
    198e:	01077733          	and	a4,a4,a6
    1992:	d37d                	beqz	a4,1978 <memchr+0x52>
    1994:	853e                	mv	a0,a5
    1996:	97b2                	add	a5,a5,a2
    1998:	a021                	j	19a0 <memchr+0x7a>
    for (; n && *s != c; s++, n--)
    199a:	0505                	addi	a0,a0,1
    199c:	00f50763          	beq	a0,a5,19aa <memchr+0x84>
    19a0:	00054703          	lbu	a4,0(a0)
    19a4:	feb71be3          	bne	a4,a1,199a <memchr+0x74>
    19a8:	8082                	ret
    return n ? (void *)s : 0;
    19aa:	4501                	li	a0,0
}
    19ac:	8082                	ret
    return n ? (void *)s : 0;
    19ae:	4501                	li	a0,0
    for (; n && *s != c; s++, n--)
    19b0:	f275                	bnez	a2,1994 <memchr+0x6e>
}
    19b2:	8082                	ret

00000000000019b4 <strnlen>:

size_t strnlen(const char *s, size_t n)
{
    19b4:	1101                	addi	sp,sp,-32
    19b6:	e822                	sd	s0,16(sp)
    const char *p = memchr(s, 0, n);
    19b8:	862e                	mv	a2,a1
{
    19ba:	842e                	mv	s0,a1
    const char *p = memchr(s, 0, n);
    19bc:	4581                	li	a1,0
{
    19be:	e426                	sd	s1,8(sp)
    19c0:	ec06                	sd	ra,24(sp)
    19c2:	84aa                	mv	s1,a0
    const char *p = memchr(s, 0, n);
    19c4:	f63ff0ef          	jal	ra,1926 <memchr>
    return p ? p - s : n;
    19c8:	c519                	beqz	a0,19d6 <strnlen+0x22>
}
    19ca:	60e2                	ld	ra,24(sp)
    19cc:	6442                	ld	s0,16(sp)
    return p ? p - s : n;
    19ce:	8d05                	sub	a0,a0,s1
}
    19d0:	64a2                	ld	s1,8(sp)
    19d2:	6105                	addi	sp,sp,32
    19d4:	8082                	ret
    19d6:	60e2                	ld	ra,24(sp)
    return p ? p - s : n;
    19d8:	8522                	mv	a0,s0
}
    19da:	6442                	ld	s0,16(sp)
    19dc:	64a2                	ld	s1,8(sp)
    19de:	6105                	addi	sp,sp,32
    19e0:	8082                	ret

00000000000019e2 <strcpy>:
char *strcpy(char *restrict d, const char *s)
{
    typedef size_t __attribute__((__may_alias__)) word;
    word *wd;
    const word *ws;
    if ((uintptr_t)s % SS == (uintptr_t)d % SS)
    19e2:	00b547b3          	xor	a5,a0,a1
    19e6:	8b9d                	andi	a5,a5,7
    19e8:	eb95                	bnez	a5,1a1c <strcpy+0x3a>
    {
        for (; (uintptr_t)s % SS; s++, d++)
    19ea:	0075f793          	andi	a5,a1,7
    19ee:	e7b1                	bnez	a5,1a3a <strcpy+0x58>
            if (!(*d = *s))
                return d;
        wd = (void *)d;
        ws = (const void *)s;
        for (; !HASZERO(*ws); *wd++ = *ws++)
    19f0:	6198                	ld	a4,0(a1)
    19f2:	00000617          	auipc	a2,0x0
    19f6:	5b663603          	ld	a2,1462(a2) # 1fa8 <__clone+0xc8>
    19fa:	00000817          	auipc	a6,0x0
    19fe:	5b683803          	ld	a6,1462(a6) # 1fb0 <__clone+0xd0>
    1a02:	a029                	j	1a0c <strcpy+0x2a>
    1a04:	e118                	sd	a4,0(a0)
    1a06:	6598                	ld	a4,8(a1)
    1a08:	05a1                	addi	a1,a1,8
    1a0a:	0521                	addi	a0,a0,8
    1a0c:	00c707b3          	add	a5,a4,a2
    1a10:	fff74693          	not	a3,a4
    1a14:	8ff5                	and	a5,a5,a3
    1a16:	0107f7b3          	and	a5,a5,a6
    1a1a:	d7ed                	beqz	a5,1a04 <strcpy+0x22>
            ;
        d = (void *)wd;
        s = (const void *)ws;
    }
    for (; (*d = *s); s++, d++)
    1a1c:	0005c783          	lbu	a5,0(a1)
    1a20:	00f50023          	sb	a5,0(a0)
    1a24:	c785                	beqz	a5,1a4c <strcpy+0x6a>
    1a26:	0015c783          	lbu	a5,1(a1)
    1a2a:	0505                	addi	a0,a0,1
    1a2c:	0585                	addi	a1,a1,1
    1a2e:	00f50023          	sb	a5,0(a0)
    1a32:	fbf5                	bnez	a5,1a26 <strcpy+0x44>
        ;
    return d;
}
    1a34:	8082                	ret
        for (; (uintptr_t)s % SS; s++, d++)
    1a36:	0505                	addi	a0,a0,1
    1a38:	df45                	beqz	a4,19f0 <strcpy+0xe>
            if (!(*d = *s))
    1a3a:	0005c783          	lbu	a5,0(a1)
        for (; (uintptr_t)s % SS; s++, d++)
    1a3e:	0585                	addi	a1,a1,1
    1a40:	0075f713          	andi	a4,a1,7
            if (!(*d = *s))
    1a44:	00f50023          	sb	a5,0(a0)
    1a48:	f7fd                	bnez	a5,1a36 <strcpy+0x54>
}
    1a4a:	8082                	ret
    1a4c:	8082                	ret

0000000000001a4e <strncpy>:
char *strncpy(char *restrict d, const char *s, size_t n)
{
    typedef size_t __attribute__((__may_alias__)) word;
    word *wd;
    const word *ws;
    if (((uintptr_t)s & ALIGN) == ((uintptr_t)d & ALIGN))
    1a4e:	00b547b3          	xor	a5,a0,a1
    1a52:	8b9d                	andi	a5,a5,7
    1a54:	1a079863          	bnez	a5,1c04 <strncpy+0x1b6>
    {
        for (; ((uintptr_t)s & ALIGN) && n && (*d = *s); n--, s++, d++)
    1a58:	0075f793          	andi	a5,a1,7
    1a5c:	16078463          	beqz	a5,1bc4 <strncpy+0x176>
    1a60:	ea01                	bnez	a2,1a70 <strncpy+0x22>
    1a62:	a421                	j	1c6a <strncpy+0x21c>
    1a64:	167d                	addi	a2,a2,-1
    1a66:	0505                	addi	a0,a0,1
    1a68:	14070e63          	beqz	a4,1bc4 <strncpy+0x176>
    1a6c:	1a060863          	beqz	a2,1c1c <strncpy+0x1ce>
    1a70:	0005c783          	lbu	a5,0(a1)
    1a74:	0585                	addi	a1,a1,1
    1a76:	0075f713          	andi	a4,a1,7
    1a7a:	00f50023          	sb	a5,0(a0)
    1a7e:	f3fd                	bnez	a5,1a64 <strncpy+0x16>
    1a80:	4805                	li	a6,1
    1a82:	1a061863          	bnez	a2,1c32 <strncpy+0x1e4>
    1a86:	40a007b3          	neg	a5,a0
    1a8a:	8b9d                	andi	a5,a5,7
    1a8c:	4681                	li	a3,0
    1a8e:	18061a63          	bnez	a2,1c22 <strncpy+0x1d4>
    1a92:	00778713          	addi	a4,a5,7
    1a96:	45ad                	li	a1,11
    1a98:	18b76363          	bltu	a4,a1,1c1e <strncpy+0x1d0>
    1a9c:	1ae6eb63          	bltu	a3,a4,1c52 <strncpy+0x204>
    1aa0:	1a078363          	beqz	a5,1c46 <strncpy+0x1f8>
    for (int i = 0; i < n; ++i, *(p++) = c)
    1aa4:	00050023          	sb	zero,0(a0)
    1aa8:	4685                	li	a3,1
    1aaa:	00150713          	addi	a4,a0,1
    1aae:	18d78f63          	beq	a5,a3,1c4c <strncpy+0x1fe>
    1ab2:	000500a3          	sb	zero,1(a0)
    1ab6:	4689                	li	a3,2
    1ab8:	00250713          	addi	a4,a0,2
    1abc:	18d78e63          	beq	a5,a3,1c58 <strncpy+0x20a>
    1ac0:	00050123          	sb	zero,2(a0)
    1ac4:	468d                	li	a3,3
    1ac6:	00350713          	addi	a4,a0,3
    1aca:	16d78c63          	beq	a5,a3,1c42 <strncpy+0x1f4>
    1ace:	000501a3          	sb	zero,3(a0)
    1ad2:	4691                	li	a3,4
    1ad4:	00450713          	addi	a4,a0,4
    1ad8:	18d78263          	beq	a5,a3,1c5c <strncpy+0x20e>
    1adc:	00050223          	sb	zero,4(a0)
    1ae0:	4695                	li	a3,5
    1ae2:	00550713          	addi	a4,a0,5
    1ae6:	16d78d63          	beq	a5,a3,1c60 <strncpy+0x212>
    1aea:	000502a3          	sb	zero,5(a0)
    1aee:	469d                	li	a3,7
    1af0:	00650713          	addi	a4,a0,6
    1af4:	16d79863          	bne	a5,a3,1c64 <strncpy+0x216>
    1af8:	00750713          	addi	a4,a0,7
    1afc:	00050323          	sb	zero,6(a0)
    1b00:	40f80833          	sub	a6,a6,a5
    1b04:	ff887593          	andi	a1,a6,-8
    1b08:	97aa                	add	a5,a5,a0
    1b0a:	95be                	add	a1,a1,a5
    1b0c:	0007b023          	sd	zero,0(a5)
    1b10:	07a1                	addi	a5,a5,8
    1b12:	feb79de3          	bne	a5,a1,1b0c <strncpy+0xbe>
    1b16:	ff887593          	andi	a1,a6,-8
    1b1a:	9ead                	addw	a3,a3,a1
    1b1c:	00b707b3          	add	a5,a4,a1
    1b20:	12b80863          	beq	a6,a1,1c50 <strncpy+0x202>
    1b24:	00078023          	sb	zero,0(a5)
    1b28:	0016871b          	addiw	a4,a3,1
    1b2c:	0ec77863          	bgeu	a4,a2,1c1c <strncpy+0x1ce>
    1b30:	000780a3          	sb	zero,1(a5)
    1b34:	0026871b          	addiw	a4,a3,2
    1b38:	0ec77263          	bgeu	a4,a2,1c1c <strncpy+0x1ce>
    1b3c:	00078123          	sb	zero,2(a5)
    1b40:	0036871b          	addiw	a4,a3,3
    1b44:	0cc77c63          	bgeu	a4,a2,1c1c <strncpy+0x1ce>
    1b48:	000781a3          	sb	zero,3(a5)
    1b4c:	0046871b          	addiw	a4,a3,4
    1b50:	0cc77663          	bgeu	a4,a2,1c1c <strncpy+0x1ce>
    1b54:	00078223          	sb	zero,4(a5)
    1b58:	0056871b          	addiw	a4,a3,5
    1b5c:	0cc77063          	bgeu	a4,a2,1c1c <strncpy+0x1ce>
    1b60:	000782a3          	sb	zero,5(a5)
    1b64:	0066871b          	addiw	a4,a3,6
    1b68:	0ac77a63          	bgeu	a4,a2,1c1c <strncpy+0x1ce>
    1b6c:	00078323          	sb	zero,6(a5)
    1b70:	0076871b          	addiw	a4,a3,7
    1b74:	0ac77463          	bgeu	a4,a2,1c1c <strncpy+0x1ce>
    1b78:	000783a3          	sb	zero,7(a5)
    1b7c:	0086871b          	addiw	a4,a3,8
    1b80:	08c77e63          	bgeu	a4,a2,1c1c <strncpy+0x1ce>
    1b84:	00078423          	sb	zero,8(a5)
    1b88:	0096871b          	addiw	a4,a3,9
    1b8c:	08c77863          	bgeu	a4,a2,1c1c <strncpy+0x1ce>
    1b90:	000784a3          	sb	zero,9(a5)
    1b94:	00a6871b          	addiw	a4,a3,10
    1b98:	08c77263          	bgeu	a4,a2,1c1c <strncpy+0x1ce>
    1b9c:	00078523          	sb	zero,10(a5)
    1ba0:	00b6871b          	addiw	a4,a3,11
    1ba4:	06c77c63          	bgeu	a4,a2,1c1c <strncpy+0x1ce>
    1ba8:	000785a3          	sb	zero,11(a5)
    1bac:	00c6871b          	addiw	a4,a3,12
    1bb0:	06c77663          	bgeu	a4,a2,1c1c <strncpy+0x1ce>
    1bb4:	00078623          	sb	zero,12(a5)
    1bb8:	26b5                	addiw	a3,a3,13
    1bba:	06c6f163          	bgeu	a3,a2,1c1c <strncpy+0x1ce>
    1bbe:	000786a3          	sb	zero,13(a5)
    1bc2:	8082                	ret
            ;
        if (!n || !*s)
    1bc4:	c645                	beqz	a2,1c6c <strncpy+0x21e>
    1bc6:	0005c783          	lbu	a5,0(a1)
    1bca:	ea078be3          	beqz	a5,1a80 <strncpy+0x32>
            goto tail;
        wd = (void *)d;
        ws = (const void *)s;
        for (; n >= sizeof(size_t) && !HASZERO(*ws); n -= sizeof(size_t), ws++, wd++)
    1bce:	479d                	li	a5,7
    1bd0:	02c7ff63          	bgeu	a5,a2,1c0e <strncpy+0x1c0>
    1bd4:	00000897          	auipc	a7,0x0
    1bd8:	3d48b883          	ld	a7,980(a7) # 1fa8 <__clone+0xc8>
    1bdc:	00000817          	auipc	a6,0x0
    1be0:	3d483803          	ld	a6,980(a6) # 1fb0 <__clone+0xd0>
    1be4:	431d                	li	t1,7
    1be6:	6198                	ld	a4,0(a1)
    1be8:	011707b3          	add	a5,a4,a7
    1bec:	fff74693          	not	a3,a4
    1bf0:	8ff5                	and	a5,a5,a3
    1bf2:	0107f7b3          	and	a5,a5,a6
    1bf6:	ef81                	bnez	a5,1c0e <strncpy+0x1c0>
            *wd = *ws;
    1bf8:	e118                	sd	a4,0(a0)
        for (; n >= sizeof(size_t) && !HASZERO(*ws); n -= sizeof(size_t), ws++, wd++)
    1bfa:	1661                	addi	a2,a2,-8
    1bfc:	05a1                	addi	a1,a1,8
    1bfe:	0521                	addi	a0,a0,8
    1c00:	fec363e3          	bltu	t1,a2,1be6 <strncpy+0x198>
        d = (void *)wd;
        s = (const void *)ws;
    }
    for (; n && (*d = *s); n--, s++, d++)
    1c04:	e609                	bnez	a2,1c0e <strncpy+0x1c0>
    1c06:	a08d                	j	1c68 <strncpy+0x21a>
    1c08:	167d                	addi	a2,a2,-1
    1c0a:	0505                	addi	a0,a0,1
    1c0c:	ca01                	beqz	a2,1c1c <strncpy+0x1ce>
    1c0e:	0005c783          	lbu	a5,0(a1)
    1c12:	0585                	addi	a1,a1,1
    1c14:	00f50023          	sb	a5,0(a0)
    1c18:	fbe5                	bnez	a5,1c08 <strncpy+0x1ba>
        ;
tail:
    1c1a:	b59d                	j	1a80 <strncpy+0x32>
    memset(d, 0, n);
    return d;
}
    1c1c:	8082                	ret
    1c1e:	472d                	li	a4,11
    1c20:	bdb5                	j	1a9c <strncpy+0x4e>
    1c22:	00778713          	addi	a4,a5,7
    1c26:	45ad                	li	a1,11
    1c28:	fff60693          	addi	a3,a2,-1
    1c2c:	e6b778e3          	bgeu	a4,a1,1a9c <strncpy+0x4e>
    1c30:	b7fd                	j	1c1e <strncpy+0x1d0>
    1c32:	40a007b3          	neg	a5,a0
    1c36:	8832                	mv	a6,a2
    1c38:	8b9d                	andi	a5,a5,7
    1c3a:	4681                	li	a3,0
    1c3c:	e4060be3          	beqz	a2,1a92 <strncpy+0x44>
    1c40:	b7cd                	j	1c22 <strncpy+0x1d4>
    for (int i = 0; i < n; ++i, *(p++) = c)
    1c42:	468d                	li	a3,3
    1c44:	bd75                	j	1b00 <strncpy+0xb2>
    1c46:	872a                	mv	a4,a0
    1c48:	4681                	li	a3,0
    1c4a:	bd5d                	j	1b00 <strncpy+0xb2>
    1c4c:	4685                	li	a3,1
    1c4e:	bd4d                	j	1b00 <strncpy+0xb2>
    1c50:	8082                	ret
    1c52:	87aa                	mv	a5,a0
    1c54:	4681                	li	a3,0
    1c56:	b5f9                	j	1b24 <strncpy+0xd6>
    1c58:	4689                	li	a3,2
    1c5a:	b55d                	j	1b00 <strncpy+0xb2>
    1c5c:	4691                	li	a3,4
    1c5e:	b54d                	j	1b00 <strncpy+0xb2>
    1c60:	4695                	li	a3,5
    1c62:	bd79                	j	1b00 <strncpy+0xb2>
    1c64:	4699                	li	a3,6
    1c66:	bd69                	j	1b00 <strncpy+0xb2>
    1c68:	8082                	ret
    1c6a:	8082                	ret
    1c6c:	8082                	ret

0000000000001c6e <open>:
#include <unistd.h>

#include "syscall.h"

int open(const char *path, int flags)
{
    1c6e:	87aa                	mv	a5,a0
    1c70:	862e                	mv	a2,a1
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
}

static inline long __syscall4(long n, long a, long b, long c, long d)
{
    register long a7 __asm__("a7") = n;
    1c72:	03800893          	li	a7,56
    register long a0 __asm__("a0") = a;
    1c76:	f9c00513          	li	a0,-100
    register long a1 __asm__("a1") = b;
    1c7a:	85be                	mv	a1,a5
    register long a2 __asm__("a2") = c;
    register long a3 __asm__("a3") = d;
    1c7c:	4689                	li	a3,2
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3))
    1c7e:	00000073          	ecall
    return syscall(SYS_openat, AT_FDCWD, path, flags, O_RDWR);
}
    1c82:	2501                	sext.w	a0,a0
    1c84:	8082                	ret

0000000000001c86 <openat>:
    register long a7 __asm__("a7") = n;
    1c86:	03800893          	li	a7,56
    register long a3 __asm__("a3") = d;
    1c8a:	18000693          	li	a3,384
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3))
    1c8e:	00000073          	ecall

int openat(int dirfd,const char *path, int flags)
{
    return syscall(SYS_openat, dirfd, path, flags, 0600);
}
    1c92:	2501                	sext.w	a0,a0
    1c94:	8082                	ret

0000000000001c96 <close>:
    register long a7 __asm__("a7") = n;
    1c96:	03900893          	li	a7,57
    __asm_syscall("r"(a7), "0"(a0))
    1c9a:	00000073          	ecall

int close(int fd)
{
    return syscall(SYS_close, fd);
}
    1c9e:	2501                	sext.w	a0,a0
    1ca0:	8082                	ret

0000000000001ca2 <read>:
    register long a7 __asm__("a7") = n;
    1ca2:	03f00893          	li	a7,63
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1ca6:	00000073          	ecall

ssize_t read(int fd, void *buf, size_t len)
{
    return syscall(SYS_read, fd, buf, len);
}
    1caa:	8082                	ret

0000000000001cac <write>:
    register long a7 __asm__("a7") = n;
    1cac:	04000893          	li	a7,64
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1cb0:	00000073          	ecall

ssize_t write(int fd, const void *buf, size_t len)
{
    return syscall(SYS_write, fd, buf, len);
}
    1cb4:	8082                	ret

0000000000001cb6 <getpid>:
    register long a7 __asm__("a7") = n;
    1cb6:	0ac00893          	li	a7,172
    __asm_syscall("r"(a7))
    1cba:	00000073          	ecall

pid_t getpid(void)
{
    return syscall(SYS_getpid);
}
    1cbe:	2501                	sext.w	a0,a0
    1cc0:	8082                	ret

0000000000001cc2 <getppid>:
    register long a7 __asm__("a7") = n;
    1cc2:	0ad00893          	li	a7,173
    __asm_syscall("r"(a7))
    1cc6:	00000073          	ecall

pid_t getppid(void)
{
    return syscall(SYS_getppid);
}
    1cca:	2501                	sext.w	a0,a0
    1ccc:	8082                	ret

0000000000001cce <sched_yield>:
    register long a7 __asm__("a7") = n;
    1cce:	07c00893          	li	a7,124
    __asm_syscall("r"(a7))
    1cd2:	00000073          	ecall

int sched_yield(void)
{
    return syscall(SYS_sched_yield);
}
    1cd6:	2501                	sext.w	a0,a0
    1cd8:	8082                	ret

0000000000001cda <fork>:
    register long a7 __asm__("a7") = n;
    1cda:	0dc00893          	li	a7,220
    register long a0 __asm__("a0") = a;
    1cde:	4545                	li	a0,17
    register long a1 __asm__("a1") = b;
    1ce0:	4581                	li	a1,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1ce2:	00000073          	ecall

pid_t fork(void)
{
    return syscall(SYS_clone, SIGCHLD, 0);
}
    1ce6:	2501                	sext.w	a0,a0
    1ce8:	8082                	ret

0000000000001cea <clone>:

pid_t clone(int (*fn)(void *arg), void *arg, void *stack, size_t stack_size, unsigned long flags)
{
    1cea:	85b2                	mv	a1,a2
    1cec:	863a                	mv	a2,a4
    if (stack)
    1cee:	c191                	beqz	a1,1cf2 <clone+0x8>
	stack += stack_size;
    1cf0:	95b6                	add	a1,a1,a3

    return __clone(fn, stack, flags, NULL, NULL, NULL);
    1cf2:	4781                	li	a5,0
    1cf4:	4701                	li	a4,0
    1cf6:	4681                	li	a3,0
    1cf8:	2601                	sext.w	a2,a2
    1cfa:	a2dd                	j	1ee0 <__clone>

0000000000001cfc <exit>:
    register long a7 __asm__("a7") = n;
    1cfc:	05d00893          	li	a7,93
    __asm_syscall("r"(a7), "0"(a0))
    1d00:	00000073          	ecall
    //return syscall(SYS_clone, fn, stack, flags, NULL, NULL, NULL);
}
void exit(int code)
{
    syscall(SYS_exit, code);
}
    1d04:	8082                	ret

0000000000001d06 <waitpid>:
    register long a7 __asm__("a7") = n;
    1d06:	10400893          	li	a7,260
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1d0a:	00000073          	ecall

int waitpid(int pid, int *code, int options)
{
    return syscall(SYS_wait4, pid, code, options);
}
    1d0e:	2501                	sext.w	a0,a0
    1d10:	8082                	ret

0000000000001d12 <exec>:
    register long a7 __asm__("a7") = n;
    1d12:	0dd00893          	li	a7,221
    __asm_syscall("r"(a7), "0"(a0))
    1d16:	00000073          	ecall

int exec(char *name)
{
    return syscall(SYS_execve, name);
}
    1d1a:	2501                	sext.w	a0,a0
    1d1c:	8082                	ret

0000000000001d1e <execve>:
    register long a7 __asm__("a7") = n;
    1d1e:	0dd00893          	li	a7,221
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1d22:	00000073          	ecall

int execve(const char *name, char *const argv[], char *const argp[])
{
    return syscall(SYS_execve, name, argv, argp);
}
    1d26:	2501                	sext.w	a0,a0
    1d28:	8082                	ret

0000000000001d2a <times>:
    register long a7 __asm__("a7") = n;
    1d2a:	09900893          	li	a7,153
    __asm_syscall("r"(a7), "0"(a0))
    1d2e:	00000073          	ecall

int times(void *mytimes)
{
	return syscall(SYS_times, mytimes);
}
    1d32:	2501                	sext.w	a0,a0
    1d34:	8082                	ret

0000000000001d36 <get_time>:

int64 get_time()
{
    1d36:	1141                	addi	sp,sp,-16
    register long a7 __asm__("a7") = n;
    1d38:	0a900893          	li	a7,169
    register long a0 __asm__("a0") = a;
    1d3c:	850a                	mv	a0,sp
    register long a1 __asm__("a1") = b;
    1d3e:	4581                	li	a1,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1d40:	00000073          	ecall
    TimeVal time;
    int err = sys_get_time(&time, 0);
    if (err == 0)
    1d44:	2501                	sext.w	a0,a0
    1d46:	ed09                	bnez	a0,1d60 <get_time+0x2a>
    {
        return ((time.sec & 0xffff) * 1000 + time.usec / 1000);
    1d48:	67a2                	ld	a5,8(sp)
    1d4a:	3e800713          	li	a4,1000
    1d4e:	00015503          	lhu	a0,0(sp)
    1d52:	02e7d7b3          	divu	a5,a5,a4
    1d56:	02e50533          	mul	a0,a0,a4
    1d5a:	953e                	add	a0,a0,a5
    }
    else
    {
        return -1;
    }
}
    1d5c:	0141                	addi	sp,sp,16
    1d5e:	8082                	ret
        return -1;
    1d60:	557d                	li	a0,-1
    1d62:	bfed                	j	1d5c <get_time+0x26>

0000000000001d64 <sys_get_time>:
    register long a7 __asm__("a7") = n;
    1d64:	0a900893          	li	a7,169
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1d68:	00000073          	ecall

int sys_get_time(TimeVal *ts, int tz)
{
    return syscall(SYS_gettimeofday, ts, tz);
}
    1d6c:	2501                	sext.w	a0,a0
    1d6e:	8082                	ret

0000000000001d70 <time>:
    register long a7 __asm__("a7") = n;
    1d70:	42600893          	li	a7,1062
    __asm_syscall("r"(a7), "0"(a0))
    1d74:	00000073          	ecall

int time(unsigned long *tloc)
{
    return syscall(SYS_time, tloc);
}
    1d78:	2501                	sext.w	a0,a0
    1d7a:	8082                	ret

0000000000001d7c <sleep>:

int sleep(unsigned long long time)
{
    1d7c:	1141                	addi	sp,sp,-16
    TimeVal tv = {.sec = time, .usec = 0};
    1d7e:	e02a                	sd	a0,0(sp)
    register long a0 __asm__("a0") = a;
    1d80:	850a                	mv	a0,sp
    1d82:	e402                	sd	zero,8(sp)
    register long a7 __asm__("a7") = n;
    1d84:	06500893          	li	a7,101
    register long a1 __asm__("a1") = b;
    1d88:	85aa                	mv	a1,a0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1d8a:	00000073          	ecall
    if (syscall(SYS_nanosleep, &tv, &tv)) return tv.sec;
    1d8e:	e501                	bnez	a0,1d96 <sleep+0x1a>
    return 0;
    1d90:	4501                	li	a0,0
}
    1d92:	0141                	addi	sp,sp,16
    1d94:	8082                	ret
    if (syscall(SYS_nanosleep, &tv, &tv)) return tv.sec;
    1d96:	4502                	lw	a0,0(sp)
}
    1d98:	0141                	addi	sp,sp,16
    1d9a:	8082                	ret

0000000000001d9c <set_priority>:
    register long a7 __asm__("a7") = n;
    1d9c:	08c00893          	li	a7,140
    __asm_syscall("r"(a7), "0"(a0))
    1da0:	00000073          	ecall

int set_priority(int prio)
{
    return syscall(SYS_setpriority, prio);
}
    1da4:	2501                	sext.w	a0,a0
    1da6:	8082                	ret

0000000000001da8 <mmap>:
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4))
}

static inline long __syscall6(long n, long a, long b, long c, long d, long e, long f)
{
    register long a7 __asm__("a7") = n;
    1da8:	0de00893          	li	a7,222
    register long a1 __asm__("a1") = b;
    register long a2 __asm__("a2") = c;
    register long a3 __asm__("a3") = d;
    register long a4 __asm__("a4") = e;
    register long a5 __asm__("a5") = f;
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4), "r"(a5))
    1dac:	00000073          	ecall

void *mmap(void *start, size_t len, int prot, int flags, int fd, off_t off)
{
    return syscall(SYS_mmap, start, len, prot, flags, fd, off);
}
    1db0:	8082                	ret

0000000000001db2 <munmap>:
    register long a7 __asm__("a7") = n;
    1db2:	0d700893          	li	a7,215
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1db6:	00000073          	ecall

int munmap(void *start, size_t len)
{
    return syscall(SYS_munmap, start, len);
}
    1dba:	2501                	sext.w	a0,a0
    1dbc:	8082                	ret

0000000000001dbe <wait>:

int wait(int *code)
{
    1dbe:	85aa                	mv	a1,a0
    register long a7 __asm__("a7") = n;
    1dc0:	10400893          	li	a7,260
    register long a0 __asm__("a0") = a;
    1dc4:	557d                	li	a0,-1
    register long a2 __asm__("a2") = c;
    1dc6:	4601                	li	a2,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1dc8:	00000073          	ecall
    return waitpid(-1, code, 0);
}
    1dcc:	2501                	sext.w	a0,a0
    1dce:	8082                	ret

0000000000001dd0 <spawn>:
    register long a7 __asm__("a7") = n;
    1dd0:	19000893          	li	a7,400
    __asm_syscall("r"(a7), "0"(a0))
    1dd4:	00000073          	ecall

int spawn(char *file)
{
    return syscall(SYS_spawn, file);
}
    1dd8:	2501                	sext.w	a0,a0
    1dda:	8082                	ret

0000000000001ddc <mailread>:
    register long a7 __asm__("a7") = n;
    1ddc:	19100893          	li	a7,401
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1de0:	00000073          	ecall

int mailread(void *buf, int len)
{
    return syscall(SYS_mailread, buf, len);
}
    1de4:	2501                	sext.w	a0,a0
    1de6:	8082                	ret

0000000000001de8 <mailwrite>:
    register long a7 __asm__("a7") = n;
    1de8:	19200893          	li	a7,402
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1dec:	00000073          	ecall

int mailwrite(int pid, void *buf, int len)
{
    return syscall(SYS_mailwrite, pid, buf, len);
}
    1df0:	2501                	sext.w	a0,a0
    1df2:	8082                	ret

0000000000001df4 <fstat>:
    register long a7 __asm__("a7") = n;
    1df4:	05000893          	li	a7,80
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1df8:	00000073          	ecall

int fstat(int fd, struct kstat *st)
{
    return syscall(SYS_fstat, fd, st);
}
    1dfc:	2501                	sext.w	a0,a0
    1dfe:	8082                	ret

0000000000001e00 <sys_linkat>:
    register long a4 __asm__("a4") = e;
    1e00:	1702                	slli	a4,a4,0x20
    register long a7 __asm__("a7") = n;
    1e02:	02500893          	li	a7,37
    register long a4 __asm__("a4") = e;
    1e06:	9301                	srli	a4,a4,0x20
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4))
    1e08:	00000073          	ecall

int sys_linkat(int olddirfd, char *oldpath, int newdirfd, char *newpath, unsigned int flags)
{
    return syscall(SYS_linkat, olddirfd, oldpath, newdirfd, newpath, flags);
}
    1e0c:	2501                	sext.w	a0,a0
    1e0e:	8082                	ret

0000000000001e10 <sys_unlinkat>:
    register long a2 __asm__("a2") = c;
    1e10:	1602                	slli	a2,a2,0x20
    register long a7 __asm__("a7") = n;
    1e12:	02300893          	li	a7,35
    register long a2 __asm__("a2") = c;
    1e16:	9201                	srli	a2,a2,0x20
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1e18:	00000073          	ecall

int sys_unlinkat(int dirfd, char *path, unsigned int flags)
{
    return syscall(SYS_unlinkat, dirfd, path, flags);
}
    1e1c:	2501                	sext.w	a0,a0
    1e1e:	8082                	ret

0000000000001e20 <link>:

int link(char *old_path, char *new_path)
{
    1e20:	87aa                	mv	a5,a0
    1e22:	86ae                	mv	a3,a1
    register long a7 __asm__("a7") = n;
    1e24:	02500893          	li	a7,37
    register long a0 __asm__("a0") = a;
    1e28:	f9c00513          	li	a0,-100
    register long a1 __asm__("a1") = b;
    1e2c:	85be                	mv	a1,a5
    register long a2 __asm__("a2") = c;
    1e2e:	f9c00613          	li	a2,-100
    register long a4 __asm__("a4") = e;
    1e32:	4701                	li	a4,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4))
    1e34:	00000073          	ecall
    return sys_linkat(AT_FDCWD, old_path, AT_FDCWD, new_path, 0);
}
    1e38:	2501                	sext.w	a0,a0
    1e3a:	8082                	ret

0000000000001e3c <unlink>:

int unlink(char *path)
{
    1e3c:	85aa                	mv	a1,a0
    register long a7 __asm__("a7") = n;
    1e3e:	02300893          	li	a7,35
    register long a0 __asm__("a0") = a;
    1e42:	f9c00513          	li	a0,-100
    register long a2 __asm__("a2") = c;
    1e46:	4601                	li	a2,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1e48:	00000073          	ecall
    return sys_unlinkat(AT_FDCWD, path, 0);
}
    1e4c:	2501                	sext.w	a0,a0
    1e4e:	8082                	ret

0000000000001e50 <uname>:
    register long a7 __asm__("a7") = n;
    1e50:	0a000893          	li	a7,160
    __asm_syscall("r"(a7), "0"(a0))
    1e54:	00000073          	ecall

int uname(void *buf)
{
    return syscall(SYS_uname, buf);
}
    1e58:	2501                	sext.w	a0,a0
    1e5a:	8082                	ret

0000000000001e5c <brk>:
    register long a7 __asm__("a7") = n;
    1e5c:	0d600893          	li	a7,214
    __asm_syscall("r"(a7), "0"(a0))
    1e60:	00000073          	ecall

int brk(void *addr)
{
    return syscall(SYS_brk, addr);
}
    1e64:	2501                	sext.w	a0,a0
    1e66:	8082                	ret

0000000000001e68 <getcwd>:
    register long a7 __asm__("a7") = n;
    1e68:	48c5                	li	a7,17
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1e6a:	00000073          	ecall

char *getcwd(char *buf, size_t size){
    return syscall(SYS_getcwd, buf, size);
}
    1e6e:	8082                	ret

0000000000001e70 <chdir>:
    register long a7 __asm__("a7") = n;
    1e70:	03100893          	li	a7,49
    __asm_syscall("r"(a7), "0"(a0))
    1e74:	00000073          	ecall

int chdir(const char *path){
    return syscall(SYS_chdir, path);
}
    1e78:	2501                	sext.w	a0,a0
    1e7a:	8082                	ret

0000000000001e7c <mkdir>:

int mkdir(const char *path, mode_t mode){
    1e7c:	862e                	mv	a2,a1
    1e7e:	87aa                	mv	a5,a0
    register long a2 __asm__("a2") = c;
    1e80:	1602                	slli	a2,a2,0x20
    register long a7 __asm__("a7") = n;
    1e82:	02200893          	li	a7,34
    register long a0 __asm__("a0") = a;
    1e86:	f9c00513          	li	a0,-100
    register long a1 __asm__("a1") = b;
    1e8a:	85be                	mv	a1,a5
    register long a2 __asm__("a2") = c;
    1e8c:	9201                	srli	a2,a2,0x20
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1e8e:	00000073          	ecall
    return syscall(SYS_mkdirat, AT_FDCWD, path, mode);
}
    1e92:	2501                	sext.w	a0,a0
    1e94:	8082                	ret

0000000000001e96 <getdents>:
    register long a7 __asm__("a7") = n;
    1e96:	03d00893          	li	a7,61
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1e9a:	00000073          	ecall

int getdents(int fd, struct linux_dirent64 *dirp64, unsigned long len){
    //return syscall(SYS_getdents64, fd, dirp64, len);
    return syscall(SYS_getdents64, fd, dirp64, len);
}
    1e9e:	2501                	sext.w	a0,a0
    1ea0:	8082                	ret

0000000000001ea2 <pipe>:
    register long a7 __asm__("a7") = n;
    1ea2:	03b00893          	li	a7,59
    register long a1 __asm__("a1") = b;
    1ea6:	4581                	li	a1,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1ea8:	00000073          	ecall

int pipe(int fd[2]){
    return syscall(SYS_pipe2, fd, 0);
}
    1eac:	2501                	sext.w	a0,a0
    1eae:	8082                	ret

0000000000001eb0 <dup>:
    register long a7 __asm__("a7") = n;
    1eb0:	48dd                	li	a7,23
    __asm_syscall("r"(a7), "0"(a0))
    1eb2:	00000073          	ecall

int dup(int fd){
    return syscall(SYS_dup, fd);
}
    1eb6:	2501                	sext.w	a0,a0
    1eb8:	8082                	ret

0000000000001eba <dup2>:
    register long a7 __asm__("a7") = n;
    1eba:	48e1                	li	a7,24
    register long a2 __asm__("a2") = c;
    1ebc:	4601                	li	a2,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1ebe:	00000073          	ecall

int dup2(int old, int new){
    return syscall(SYS_dup3, old, new, 0);
}
    1ec2:	2501                	sext.w	a0,a0
    1ec4:	8082                	ret

0000000000001ec6 <mount>:
    register long a7 __asm__("a7") = n;
    1ec6:	02800893          	li	a7,40
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4))
    1eca:	00000073          	ecall

int mount(const char *special, const char *dir, const char *fstype, unsigned long flags, const void *data)
{
        return syscall(SYS_mount, special, dir, fstype, flags, data);
}
    1ece:	2501                	sext.w	a0,a0
    1ed0:	8082                	ret

0000000000001ed2 <umount>:
    register long a7 __asm__("a7") = n;
    1ed2:	02700893          	li	a7,39
    register long a1 __asm__("a1") = b;
    1ed6:	4581                	li	a1,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1ed8:	00000073          	ecall

int umount(const char *special)
{
        return syscall(SYS_umount2, special, 0);
}
    1edc:	2501                	sext.w	a0,a0
    1ede:	8082                	ret

0000000000001ee0 <__clone>:

.global __clone
.type  __clone, %function
__clone:
	# Save func and arg to stack
	addi a1, a1, -16
    1ee0:	15c1                	addi	a1,a1,-16
	sd a0, 0(a1)
    1ee2:	e188                	sd	a0,0(a1)
	sd a3, 8(a1)
    1ee4:	e594                	sd	a3,8(a1)

	# Call SYS_clone
	mv a0, a2
    1ee6:	8532                	mv	a0,a2
	mv a2, a4
    1ee8:	863a                	mv	a2,a4
	mv a3, a5
    1eea:	86be                	mv	a3,a5
	mv a4, a6
    1eec:	8742                	mv	a4,a6
	li a7, 220 # SYS_clone
    1eee:	0dc00893          	li	a7,220
	ecall
    1ef2:	00000073          	ecall

	beqz a0, 1f
    1ef6:	c111                	beqz	a0,1efa <__clone+0x1a>
	# Parent
	ret
    1ef8:	8082                	ret

	# Child
1:      ld a1, 0(sp)
    1efa:	6582                	ld	a1,0(sp)
	ld a0, 8(sp)
    1efc:	6522                	ld	a0,8(sp)
	jalr a1
    1efe:	9582                	jalr	a1

	# Exit
	li a7, 93 # SYS_exit
    1f00:	05d00893          	li	a7,93
	ecall
    1f04:	00000073          	ecall
