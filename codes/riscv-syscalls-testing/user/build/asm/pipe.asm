
/home/ubuntu/rust/ultra-os/codes/riscv-syscalls-testing/user/build/riscv64/pipe:     file format elf64-littleriscv


Disassembly of section .text:

0000000000001000 <_start>:
.section .text.entry
.globl _start
_start:
    mv a0, sp
    1000:	850a                	mv	a0,sp
    tail __start_main
    1002:	aa85                	j	1172 <__start_main>

0000000000001004 <test_pipe>:
 * 成功测试时的输出：
 * "  Write to pipe successfully."
 */
static int fd[2];

void test_pipe(void){
    1004:	7135                	addi	sp,sp,-160
    TEST_START(__func__);
    1006:	00001517          	auipc	a0,0x1
    100a:	fa250513          	addi	a0,a0,-94 # 1fa8 <__clone+0x28>
void test_pipe(void){
    100e:	ed06                	sd	ra,152(sp)
    1010:	e922                	sd	s0,144(sp)
    1012:	e526                	sd	s1,136(sp)
    TEST_START(__func__);
    1014:	3d0000ef          	jal	ra,13e4 <puts>
    1018:	00001517          	auipc	a0,0x1
    101c:	0a050513          	addi	a0,a0,160 # 20b8 <__func__.0>
    1020:	3c4000ef          	jal	ra,13e4 <puts>
    1024:	00001517          	auipc	a0,0x1
    1028:	f9c50513          	addi	a0,a0,-100 # 1fc0 <__clone+0x40>
    102c:	3b8000ef          	jal	ra,13e4 <puts>
    int cpid;
    char buf[128] = {0};
    int ret = pipe(fd);
    1030:	00001517          	auipc	a0,0x1
    1034:	08050513          	addi	a0,a0,128 # 20b0 <fd>
    char buf[128] = {0};
    1038:	e002                	sd	zero,0(sp)
    103a:	e402                	sd	zero,8(sp)
    103c:	e802                	sd	zero,16(sp)
    103e:	ec02                	sd	zero,24(sp)
    1040:	f002                	sd	zero,32(sp)
    1042:	f402                	sd	zero,40(sp)
    1044:	f802                	sd	zero,48(sp)
    1046:	fc02                	sd	zero,56(sp)
    1048:	e082                	sd	zero,64(sp)
    104a:	e482                	sd	zero,72(sp)
    104c:	e882                	sd	zero,80(sp)
    104e:	ec82                	sd	zero,88(sp)
    1050:	f082                	sd	zero,96(sp)
    1052:	f482                	sd	zero,104(sp)
    1054:	f882                	sd	zero,112(sp)
    1056:	fc82                	sd	zero,120(sp)
    int ret = pipe(fd);
    1058:	6eb000ef          	jal	ra,1f42 <pipe>
    assert(ret != -1);
    105c:	57fd                	li	a5,-1
    105e:	0ef50b63          	beq	a0,a5,1154 <test_pipe+0x150>
    const char *data = "  Write to pipe successfully.\n";
    cpid = fork();
    1062:	519000ef          	jal	ra,1d7a <fork>
    1066:	842a                	mv	s0,a0
    printf("cpid: %d\n", cpid);
    1068:	85aa                	mv	a1,a0
    106a:	00001517          	auipc	a0,0x1
    106e:	f8650513          	addi	a0,a0,-122 # 1ff0 <__clone+0x70>
    1072:	394000ef          	jal	ra,1406 <printf>
    if(cpid > 0){
    1076:	08805663          	blez	s0,1102 <test_pipe+0xfe>
	close(fd[1]);
    107a:	00001417          	auipc	s0,0x1
    107e:	03640413          	addi	s0,s0,54 # 20b0 <fd>
    1082:	4048                	lw	a0,4(s0)
    1084:	4b3000ef          	jal	ra,1d36 <close>
    printf("Start read data \n");
    1088:	00001517          	auipc	a0,0x1
    108c:	f7850513          	addi	a0,a0,-136 # 2000 <__clone+0x80>
    1090:	376000ef          	jal	ra,1406 <printf>

	while(read(fd[0], buf, 1) > 0)
    1094:	a019                	j	109a <test_pipe+0x96>
            write(STDOUT, buf, 1);
    1096:	4b7000ef          	jal	ra,1d4c <write>
	while(read(fd[0], buf, 1) > 0)
    109a:	4008                	lw	a0,0(s0)
    109c:	4605                	li	a2,1
    109e:	858a                	mv	a1,sp
    10a0:	4a3000ef          	jal	ra,1d42 <read>
    10a4:	87aa                	mv	a5,a0
            write(STDOUT, buf, 1);
    10a6:	4605                	li	a2,1
    10a8:	858a                	mv	a1,sp
    10aa:	4505                	li	a0,1
	while(read(fd[0], buf, 1) > 0)
    10ac:	fef045e3          	bgtz	a5,1096 <test_pipe+0x92>
	write(STDOUT, "\n", 1);
    10b0:	00001597          	auipc	a1,0x1
    10b4:	f6058593          	addi	a1,a1,-160 # 2010 <__clone+0x90>
    10b8:	495000ef          	jal	ra,1d4c <write>
    printf("Read data finished\n");
    10bc:	00001517          	auipc	a0,0x1
    10c0:	f5c50513          	addi	a0,a0,-164 # 2018 <__clone+0x98>
    10c4:	342000ef          	jal	ra,1406 <printf>

	close(fd[0]);
    10c8:	4008                	lw	a0,0(s0)
    10ca:	46d000ef          	jal	ra,1d36 <close>
	wait(NULL);
    10ce:	4501                	li	a0,0
    10d0:	58f000ef          	jal	ra,1e5e <wait>
    printf("Write data finished\n");

	close(fd[1]);
	exit(0);
    }
    TEST_END(__func__);
    10d4:	00001517          	auipc	a0,0x1
    10d8:	fac50513          	addi	a0,a0,-84 # 2080 <__clone+0x100>
    10dc:	308000ef          	jal	ra,13e4 <puts>
    10e0:	00001517          	auipc	a0,0x1
    10e4:	fd850513          	addi	a0,a0,-40 # 20b8 <__func__.0>
    10e8:	2fc000ef          	jal	ra,13e4 <puts>
    10ec:	00001517          	auipc	a0,0x1
    10f0:	ed450513          	addi	a0,a0,-300 # 1fc0 <__clone+0x40>
    10f4:	2f0000ef          	jal	ra,13e4 <puts>
}
    10f8:	60ea                	ld	ra,152(sp)
    10fa:	644a                	ld	s0,144(sp)
    10fc:	64aa                	ld	s1,136(sp)
    10fe:	610d                	addi	sp,sp,160
    1100:	8082                	ret
	close(fd[0]);
    1102:	00001417          	auipc	s0,0x1
    1106:	fae40413          	addi	s0,s0,-82 # 20b0 <fd>
    110a:	4008                	lw	a0,0(s0)
    110c:	42b000ef          	jal	ra,1d36 <close>
    printf("Start write data \n");
    1110:	00001517          	auipc	a0,0x1
    1114:	f2050513          	addi	a0,a0,-224 # 2030 <__clone+0xb0>
    1118:	2ee000ef          	jal	ra,1406 <printf>
	write(fd[1], data, strlen(data));
    111c:	4044                	lw	s1,4(s0)
    111e:	00001517          	auipc	a0,0x1
    1122:	f2a50513          	addi	a0,a0,-214 # 2048 <__clone+0xc8>
    1126:	043000ef          	jal	ra,1968 <strlen>
    112a:	862a                	mv	a2,a0
    112c:	00001597          	auipc	a1,0x1
    1130:	f1c58593          	addi	a1,a1,-228 # 2048 <__clone+0xc8>
    1134:	8526                	mv	a0,s1
    1136:	417000ef          	jal	ra,1d4c <write>
    printf("Write data finished\n");
    113a:	00001517          	auipc	a0,0x1
    113e:	f2e50513          	addi	a0,a0,-210 # 2068 <__clone+0xe8>
    1142:	2c4000ef          	jal	ra,1406 <printf>
	close(fd[1]);
    1146:	4048                	lw	a0,4(s0)
    1148:	3ef000ef          	jal	ra,1d36 <close>
	exit(0);
    114c:	4501                	li	a0,0
    114e:	44f000ef          	jal	ra,1d9c <exit>
    1152:	b749                	j	10d4 <test_pipe+0xd0>
    assert(ret != -1);
    1154:	00001517          	auipc	a0,0x1
    1158:	e7c50513          	addi	a0,a0,-388 # 1fd0 <__clone+0x50>
    115c:	52e000ef          	jal	ra,168a <panic>
    1160:	b709                	j	1062 <test_pipe+0x5e>

0000000000001162 <main>:

int main(void){
    1162:	1141                	addi	sp,sp,-16
    1164:	e406                	sd	ra,8(sp)
    test_pipe();
    1166:	e9fff0ef          	jal	ra,1004 <test_pipe>
    return 0;
}
    116a:	60a2                	ld	ra,8(sp)
    116c:	4501                	li	a0,0
    116e:	0141                	addi	sp,sp,16
    1170:	8082                	ret

0000000000001172 <__start_main>:
#include <unistd.h>

extern int main();

int __start_main(long *p)
{
    1172:	85aa                	mv	a1,a0
	int argc = p[0];
	char **argv = (void *)(p+1);

	exit(main(argc, argv));
    1174:	4108                	lw	a0,0(a0)
{
    1176:	1141                	addi	sp,sp,-16
	exit(main(argc, argv));
    1178:	05a1                	addi	a1,a1,8
{
    117a:	e406                	sd	ra,8(sp)
	exit(main(argc, argv));
    117c:	fe7ff0ef          	jal	ra,1162 <main>
    1180:	41d000ef          	jal	ra,1d9c <exit>
	return 0;
}
    1184:	60a2                	ld	ra,8(sp)
    1186:	4501                	li	a0,0
    1188:	0141                	addi	sp,sp,16
    118a:	8082                	ret

000000000000118c <printint.constprop.0>:
    write(f, s, l);
}

static char digits[] = "0123456789abcdef";

static void printint(int xx, int base, int sign)
    118c:	7179                	addi	sp,sp,-48
    118e:	f406                	sd	ra,40(sp)
{
    char buf[16 + 1];
    int i;
    uint x;

    if (sign && (sign = xx < 0))
    1190:	12054b63          	bltz	a0,12c6 <printint.constprop.0+0x13a>

    buf[16] = 0;
    i = 15;
    do
    {
        buf[i--] = digits[x % base];
    1194:	02b577bb          	remuw	a5,a0,a1
    1198:	00001617          	auipc	a2,0x1
    119c:	f3060613          	addi	a2,a2,-208 # 20c8 <digits>
    buf[16] = 0;
    11a0:	00010c23          	sb	zero,24(sp)
        buf[i--] = digits[x % base];
    11a4:	0005871b          	sext.w	a4,a1
    11a8:	1782                	slli	a5,a5,0x20
    11aa:	9381                	srli	a5,a5,0x20
    11ac:	97b2                	add	a5,a5,a2
    11ae:	0007c783          	lbu	a5,0(a5)
    } while ((x /= base) != 0);
    11b2:	02b5583b          	divuw	a6,a0,a1
        buf[i--] = digits[x % base];
    11b6:	00f10ba3          	sb	a5,23(sp)
    } while ((x /= base) != 0);
    11ba:	1cb56363          	bltu	a0,a1,1380 <printint.constprop.0+0x1f4>
        buf[i--] = digits[x % base];
    11be:	45b9                	li	a1,14
    11c0:	02e877bb          	remuw	a5,a6,a4
    11c4:	1782                	slli	a5,a5,0x20
    11c6:	9381                	srli	a5,a5,0x20
    11c8:	97b2                	add	a5,a5,a2
    11ca:	0007c783          	lbu	a5,0(a5)
    } while ((x /= base) != 0);
    11ce:	02e856bb          	divuw	a3,a6,a4
        buf[i--] = digits[x % base];
    11d2:	00f10b23          	sb	a5,22(sp)
    } while ((x /= base) != 0);
    11d6:	0ce86e63          	bltu	a6,a4,12b2 <printint.constprop.0+0x126>
        buf[i--] = digits[x % base];
    11da:	02e6f5bb          	remuw	a1,a3,a4
    } while ((x /= base) != 0);
    11de:	02e6d7bb          	divuw	a5,a3,a4
        buf[i--] = digits[x % base];
    11e2:	1582                	slli	a1,a1,0x20
    11e4:	9181                	srli	a1,a1,0x20
    11e6:	95b2                	add	a1,a1,a2
    11e8:	0005c583          	lbu	a1,0(a1)
    11ec:	00b10aa3          	sb	a1,21(sp)
    } while ((x /= base) != 0);
    11f0:	0007859b          	sext.w	a1,a5
    11f4:	12e6ec63          	bltu	a3,a4,132c <printint.constprop.0+0x1a0>
        buf[i--] = digits[x % base];
    11f8:	02e7f6bb          	remuw	a3,a5,a4
    11fc:	1682                	slli	a3,a3,0x20
    11fe:	9281                	srli	a3,a3,0x20
    1200:	96b2                	add	a3,a3,a2
    1202:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    1206:	02e7d83b          	divuw	a6,a5,a4
        buf[i--] = digits[x % base];
    120a:	00d10a23          	sb	a3,20(sp)
    } while ((x /= base) != 0);
    120e:	12e5e863          	bltu	a1,a4,133e <printint.constprop.0+0x1b2>
        buf[i--] = digits[x % base];
    1212:	02e876bb          	remuw	a3,a6,a4
    1216:	1682                	slli	a3,a3,0x20
    1218:	9281                	srli	a3,a3,0x20
    121a:	96b2                	add	a3,a3,a2
    121c:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    1220:	02e855bb          	divuw	a1,a6,a4
        buf[i--] = digits[x % base];
    1224:	00d109a3          	sb	a3,19(sp)
    } while ((x /= base) != 0);
    1228:	12e86463          	bltu	a6,a4,1350 <printint.constprop.0+0x1c4>
        buf[i--] = digits[x % base];
    122c:	02e5f6bb          	remuw	a3,a1,a4
    1230:	1682                	slli	a3,a3,0x20
    1232:	9281                	srli	a3,a3,0x20
    1234:	96b2                	add	a3,a3,a2
    1236:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    123a:	02e5d83b          	divuw	a6,a1,a4
        buf[i--] = digits[x % base];
    123e:	00d10923          	sb	a3,18(sp)
    } while ((x /= base) != 0);
    1242:	0ce5ec63          	bltu	a1,a4,131a <printint.constprop.0+0x18e>
        buf[i--] = digits[x % base];
    1246:	02e876bb          	remuw	a3,a6,a4
    124a:	1682                	slli	a3,a3,0x20
    124c:	9281                	srli	a3,a3,0x20
    124e:	96b2                	add	a3,a3,a2
    1250:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    1254:	02e855bb          	divuw	a1,a6,a4
        buf[i--] = digits[x % base];
    1258:	00d108a3          	sb	a3,17(sp)
    } while ((x /= base) != 0);
    125c:	10e86963          	bltu	a6,a4,136e <printint.constprop.0+0x1e2>
        buf[i--] = digits[x % base];
    1260:	02e5f6bb          	remuw	a3,a1,a4
    1264:	1682                	slli	a3,a3,0x20
    1266:	9281                	srli	a3,a3,0x20
    1268:	96b2                	add	a3,a3,a2
    126a:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    126e:	02e5d83b          	divuw	a6,a1,a4
        buf[i--] = digits[x % base];
    1272:	00d10823          	sb	a3,16(sp)
    } while ((x /= base) != 0);
    1276:	10e5e763          	bltu	a1,a4,1384 <printint.constprop.0+0x1f8>
        buf[i--] = digits[x % base];
    127a:	02e876bb          	remuw	a3,a6,a4
    127e:	1682                	slli	a3,a3,0x20
    1280:	9281                	srli	a3,a3,0x20
    1282:	96b2                	add	a3,a3,a2
    1284:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    1288:	02e857bb          	divuw	a5,a6,a4
        buf[i--] = digits[x % base];
    128c:	00d107a3          	sb	a3,15(sp)
    } while ((x /= base) != 0);
    1290:	10e86363          	bltu	a6,a4,1396 <printint.constprop.0+0x20a>
        buf[i--] = digits[x % base];
    1294:	1782                	slli	a5,a5,0x20
    1296:	9381                	srli	a5,a5,0x20
    1298:	97b2                	add	a5,a5,a2
    129a:	0007c783          	lbu	a5,0(a5)
    129e:	4599                	li	a1,6
    12a0:	00f10723          	sb	a5,14(sp)

    if (sign)
    12a4:	00055763          	bgez	a0,12b2 <printint.constprop.0+0x126>
        buf[i--] = '-';
    12a8:	02d00793          	li	a5,45
    12ac:	00f106a3          	sb	a5,13(sp)
        buf[i--] = digits[x % base];
    12b0:	4595                	li	a1,5
    write(f, s, l);
    12b2:	003c                	addi	a5,sp,8
    12b4:	4641                	li	a2,16
    12b6:	9e0d                	subw	a2,a2,a1
    12b8:	4505                	li	a0,1
    12ba:	95be                	add	a1,a1,a5
    12bc:	291000ef          	jal	ra,1d4c <write>
    i++;
    if (i < 0)
        puts("printint error");
    out(stdout, buf + i, 16 - i);
}
    12c0:	70a2                	ld	ra,40(sp)
    12c2:	6145                	addi	sp,sp,48
    12c4:	8082                	ret
        x = -xx;
    12c6:	40a0083b          	negw	a6,a0
        buf[i--] = digits[x % base];
    12ca:	02b877bb          	remuw	a5,a6,a1
    12ce:	00001617          	auipc	a2,0x1
    12d2:	dfa60613          	addi	a2,a2,-518 # 20c8 <digits>
    buf[16] = 0;
    12d6:	00010c23          	sb	zero,24(sp)
        buf[i--] = digits[x % base];
    12da:	0005871b          	sext.w	a4,a1
    12de:	1782                	slli	a5,a5,0x20
    12e0:	9381                	srli	a5,a5,0x20
    12e2:	97b2                	add	a5,a5,a2
    12e4:	0007c783          	lbu	a5,0(a5)
    } while ((x /= base) != 0);
    12e8:	02b858bb          	divuw	a7,a6,a1
        buf[i--] = digits[x % base];
    12ec:	00f10ba3          	sb	a5,23(sp)
    } while ((x /= base) != 0);
    12f0:	06b86963          	bltu	a6,a1,1362 <printint.constprop.0+0x1d6>
        buf[i--] = digits[x % base];
    12f4:	02e8f7bb          	remuw	a5,a7,a4
    12f8:	1782                	slli	a5,a5,0x20
    12fa:	9381                	srli	a5,a5,0x20
    12fc:	97b2                	add	a5,a5,a2
    12fe:	0007c783          	lbu	a5,0(a5)
    } while ((x /= base) != 0);
    1302:	02e8d6bb          	divuw	a3,a7,a4
        buf[i--] = digits[x % base];
    1306:	00f10b23          	sb	a5,22(sp)
    } while ((x /= base) != 0);
    130a:	ece8f8e3          	bgeu	a7,a4,11da <printint.constprop.0+0x4e>
        buf[i--] = '-';
    130e:	02d00793          	li	a5,45
    1312:	00f10aa3          	sb	a5,21(sp)
        buf[i--] = digits[x % base];
    1316:	45b5                	li	a1,13
    1318:	bf69                	j	12b2 <printint.constprop.0+0x126>
    131a:	45a9                	li	a1,10
    if (sign)
    131c:	f8055be3          	bgez	a0,12b2 <printint.constprop.0+0x126>
        buf[i--] = '-';
    1320:	02d00793          	li	a5,45
    1324:	00f108a3          	sb	a5,17(sp)
        buf[i--] = digits[x % base];
    1328:	45a5                	li	a1,9
    132a:	b761                	j	12b2 <printint.constprop.0+0x126>
    132c:	45b5                	li	a1,13
    if (sign)
    132e:	f80552e3          	bgez	a0,12b2 <printint.constprop.0+0x126>
        buf[i--] = '-';
    1332:	02d00793          	li	a5,45
    1336:	00f10a23          	sb	a5,20(sp)
        buf[i--] = digits[x % base];
    133a:	45b1                	li	a1,12
    133c:	bf9d                	j	12b2 <printint.constprop.0+0x126>
    133e:	45b1                	li	a1,12
    if (sign)
    1340:	f60559e3          	bgez	a0,12b2 <printint.constprop.0+0x126>
        buf[i--] = '-';
    1344:	02d00793          	li	a5,45
    1348:	00f109a3          	sb	a5,19(sp)
        buf[i--] = digits[x % base];
    134c:	45ad                	li	a1,11
    134e:	b795                	j	12b2 <printint.constprop.0+0x126>
    1350:	45ad                	li	a1,11
    if (sign)
    1352:	f60550e3          	bgez	a0,12b2 <printint.constprop.0+0x126>
        buf[i--] = '-';
    1356:	02d00793          	li	a5,45
    135a:	00f10923          	sb	a5,18(sp)
        buf[i--] = digits[x % base];
    135e:	45a9                	li	a1,10
    1360:	bf89                	j	12b2 <printint.constprop.0+0x126>
        buf[i--] = '-';
    1362:	02d00793          	li	a5,45
    1366:	00f10b23          	sb	a5,22(sp)
        buf[i--] = digits[x % base];
    136a:	45b9                	li	a1,14
    136c:	b799                	j	12b2 <printint.constprop.0+0x126>
    136e:	45a5                	li	a1,9
    if (sign)
    1370:	f40551e3          	bgez	a0,12b2 <printint.constprop.0+0x126>
        buf[i--] = '-';
    1374:	02d00793          	li	a5,45
    1378:	00f10823          	sb	a5,16(sp)
        buf[i--] = digits[x % base];
    137c:	45a1                	li	a1,8
    137e:	bf15                	j	12b2 <printint.constprop.0+0x126>
    i = 15;
    1380:	45bd                	li	a1,15
    1382:	bf05                	j	12b2 <printint.constprop.0+0x126>
        buf[i--] = digits[x % base];
    1384:	45a1                	li	a1,8
    if (sign)
    1386:	f20556e3          	bgez	a0,12b2 <printint.constprop.0+0x126>
        buf[i--] = '-';
    138a:	02d00793          	li	a5,45
    138e:	00f107a3          	sb	a5,15(sp)
        buf[i--] = digits[x % base];
    1392:	459d                	li	a1,7
    1394:	bf39                	j	12b2 <printint.constprop.0+0x126>
    1396:	459d                	li	a1,7
    if (sign)
    1398:	f0055de3          	bgez	a0,12b2 <printint.constprop.0+0x126>
        buf[i--] = '-';
    139c:	02d00793          	li	a5,45
    13a0:	00f10723          	sb	a5,14(sp)
        buf[i--] = digits[x % base];
    13a4:	4599                	li	a1,6
    13a6:	b731                	j	12b2 <printint.constprop.0+0x126>

00000000000013a8 <getchar>:
{
    13a8:	1101                	addi	sp,sp,-32
    read(stdin, &byte, 1);
    13aa:	00f10593          	addi	a1,sp,15
    13ae:	4605                	li	a2,1
    13b0:	4501                	li	a0,0
{
    13b2:	ec06                	sd	ra,24(sp)
    char byte = 0;
    13b4:	000107a3          	sb	zero,15(sp)
    read(stdin, &byte, 1);
    13b8:	18b000ef          	jal	ra,1d42 <read>
}
    13bc:	60e2                	ld	ra,24(sp)
    13be:	00f14503          	lbu	a0,15(sp)
    13c2:	6105                	addi	sp,sp,32
    13c4:	8082                	ret

00000000000013c6 <putchar>:
{
    13c6:	1101                	addi	sp,sp,-32
    13c8:	87aa                	mv	a5,a0
    return write(stdout, &byte, 1);
    13ca:	00f10593          	addi	a1,sp,15
    13ce:	4605                	li	a2,1
    13d0:	4505                	li	a0,1
{
    13d2:	ec06                	sd	ra,24(sp)
    char byte = c;
    13d4:	00f107a3          	sb	a5,15(sp)
    return write(stdout, &byte, 1);
    13d8:	175000ef          	jal	ra,1d4c <write>
}
    13dc:	60e2                	ld	ra,24(sp)
    13de:	2501                	sext.w	a0,a0
    13e0:	6105                	addi	sp,sp,32
    13e2:	8082                	ret

00000000000013e4 <puts>:
{
    13e4:	1141                	addi	sp,sp,-16
    13e6:	e406                	sd	ra,8(sp)
    13e8:	e022                	sd	s0,0(sp)
    13ea:	842a                	mv	s0,a0
    r = -(write(stdout, s, strlen(s)) < 0);
    13ec:	57c000ef          	jal	ra,1968 <strlen>
    13f0:	862a                	mv	a2,a0
    13f2:	85a2                	mv	a1,s0
    13f4:	4505                	li	a0,1
    13f6:	157000ef          	jal	ra,1d4c <write>
}
    13fa:	60a2                	ld	ra,8(sp)
    13fc:	6402                	ld	s0,0(sp)
    r = -(write(stdout, s, strlen(s)) < 0);
    13fe:	957d                	srai	a0,a0,0x3f
    return r;
    1400:	2501                	sext.w	a0,a0
}
    1402:	0141                	addi	sp,sp,16
    1404:	8082                	ret

0000000000001406 <printf>:
    out(stdout, buf, i);
}

// Print to the console. only understands %d, %x, %p, %s.
void printf(const char *fmt, ...)
{
    1406:	7171                	addi	sp,sp,-176
    1408:	fc56                	sd	s5,56(sp)
    140a:	ed3e                	sd	a5,152(sp)
    buf[i++] = '0';
    140c:	7ae1                	lui	s5,0xffff8
    va_list ap;
    int cnt = 0, l = 0;
    char *a, *z, *s = (char *)fmt, str;
    int f = stdout;

    va_start(ap, fmt);
    140e:	18bc                	addi	a5,sp,120
{
    1410:	e8ca                	sd	s2,80(sp)
    1412:	e4ce                	sd	s3,72(sp)
    1414:	e0d2                	sd	s4,64(sp)
    1416:	f85a                	sd	s6,48(sp)
    1418:	f486                	sd	ra,104(sp)
    141a:	f0a2                	sd	s0,96(sp)
    141c:	eca6                	sd	s1,88(sp)
    141e:	fcae                	sd	a1,120(sp)
    1420:	e132                	sd	a2,128(sp)
    1422:	e536                	sd	a3,136(sp)
    1424:	e93a                	sd	a4,144(sp)
    1426:	f142                	sd	a6,160(sp)
    1428:	f546                	sd	a7,168(sp)
    va_start(ap, fmt);
    142a:	e03e                	sd	a5,0(sp)
    for (;;)
    {
        if (!*s)
            break;
        for (a = s; *s && *s != '%'; s++)
    142c:	02500913          	li	s2,37
        out(f, a, l);
        if (l)
            continue;
        if (s[1] == 0)
            break;
        switch (s[1])
    1430:	07300a13          	li	s4,115
        case 'p':
            printptr(va_arg(ap, uint64));
            break;
        case 's':
            if ((a = va_arg(ap, char *)) == 0)
                a = "(null)";
    1434:	00001b17          	auipc	s6,0x1
    1438:	c5cb0b13          	addi	s6,s6,-932 # 2090 <__clone+0x110>
    buf[i++] = '0';
    143c:	830aca93          	xori	s5,s5,-2000
        buf[i++] = digits[x >> (sizeof(uint64) * 8 - 4)];
    1440:	00001997          	auipc	s3,0x1
    1444:	c8898993          	addi	s3,s3,-888 # 20c8 <digits>
        if (!*s)
    1448:	00054783          	lbu	a5,0(a0)
    144c:	16078a63          	beqz	a5,15c0 <printf+0x1ba>
    1450:	862a                	mv	a2,a0
        for (a = s; *s && *s != '%'; s++)
    1452:	19278163          	beq	a5,s2,15d4 <printf+0x1ce>
    1456:	00164783          	lbu	a5,1(a2)
    145a:	0605                	addi	a2,a2,1
    145c:	fbfd                	bnez	a5,1452 <printf+0x4c>
    145e:	84b2                	mv	s1,a2
        l = z - a;
    1460:	40a6043b          	subw	s0,a2,a0
    write(f, s, l);
    1464:	85aa                	mv	a1,a0
    1466:	8622                	mv	a2,s0
    1468:	4505                	li	a0,1
    146a:	0e3000ef          	jal	ra,1d4c <write>
        if (l)
    146e:	18041c63          	bnez	s0,1606 <printf+0x200>
        if (s[1] == 0)
    1472:	0014c783          	lbu	a5,1(s1)
    1476:	14078563          	beqz	a5,15c0 <printf+0x1ba>
        switch (s[1])
    147a:	1d478063          	beq	a5,s4,163a <printf+0x234>
    147e:	18fa6663          	bltu	s4,a5,160a <printf+0x204>
    1482:	06400713          	li	a4,100
    1486:	1ae78063          	beq	a5,a4,1626 <printf+0x220>
    148a:	07000713          	li	a4,112
    148e:	1ce79963          	bne	a5,a4,1660 <printf+0x25a>
            printptr(va_arg(ap, uint64));
    1492:	6702                	ld	a4,0(sp)
    buf[i++] = '0';
    1494:	01511423          	sh	s5,8(sp)
    write(f, s, l);
    1498:	4649                	li	a2,18
            printptr(va_arg(ap, uint64));
    149a:	631c                	ld	a5,0(a4)
    149c:	0721                	addi	a4,a4,8
    149e:	e03a                	sd	a4,0(sp)
    for (j = 0; j < (sizeof(uint64) * 2); j++, x <<= 4)
    14a0:	00479293          	slli	t0,a5,0x4
    14a4:	00879f93          	slli	t6,a5,0x8
    14a8:	00c79f13          	slli	t5,a5,0xc
    14ac:	01079e93          	slli	t4,a5,0x10
    14b0:	01479e13          	slli	t3,a5,0x14
    14b4:	01879313          	slli	t1,a5,0x18
    14b8:	01c79893          	slli	a7,a5,0x1c
    14bc:	02479813          	slli	a6,a5,0x24
    14c0:	02879513          	slli	a0,a5,0x28
    14c4:	02c79593          	slli	a1,a5,0x2c
    14c8:	03079693          	slli	a3,a5,0x30
    14cc:	03479713          	slli	a4,a5,0x34
        buf[i++] = digits[x >> (sizeof(uint64) * 8 - 4)];
    14d0:	03c7d413          	srli	s0,a5,0x3c
    14d4:	01c7d39b          	srliw	t2,a5,0x1c
    14d8:	03c2d293          	srli	t0,t0,0x3c
    14dc:	03cfdf93          	srli	t6,t6,0x3c
    14e0:	03cf5f13          	srli	t5,t5,0x3c
    14e4:	03cede93          	srli	t4,t4,0x3c
    14e8:	03ce5e13          	srli	t3,t3,0x3c
    14ec:	03c35313          	srli	t1,t1,0x3c
    14f0:	03c8d893          	srli	a7,a7,0x3c
    14f4:	03c85813          	srli	a6,a6,0x3c
    14f8:	9171                	srli	a0,a0,0x3c
    14fa:	91f1                	srli	a1,a1,0x3c
    14fc:	92f1                	srli	a3,a3,0x3c
    14fe:	9371                	srli	a4,a4,0x3c
    1500:	96ce                	add	a3,a3,s3
    1502:	974e                	add	a4,a4,s3
    1504:	944e                	add	s0,s0,s3
    1506:	92ce                	add	t0,t0,s3
    1508:	9fce                	add	t6,t6,s3
    150a:	9f4e                	add	t5,t5,s3
    150c:	9ece                	add	t4,t4,s3
    150e:	9e4e                	add	t3,t3,s3
    1510:	934e                	add	t1,t1,s3
    1512:	98ce                	add	a7,a7,s3
    1514:	93ce                	add	t2,t2,s3
    1516:	984e                	add	a6,a6,s3
    1518:	954e                	add	a0,a0,s3
    151a:	95ce                	add	a1,a1,s3
    151c:	0006c083          	lbu	ra,0(a3)
    1520:	0002c283          	lbu	t0,0(t0)
    1524:	00074683          	lbu	a3,0(a4)
    1528:	000fcf83          	lbu	t6,0(t6)
    152c:	000f4f03          	lbu	t5,0(t5)
    1530:	000ece83          	lbu	t4,0(t4)
    1534:	000e4e03          	lbu	t3,0(t3)
    1538:	00034303          	lbu	t1,0(t1)
    153c:	0008c883          	lbu	a7,0(a7)
    1540:	0003c383          	lbu	t2,0(t2)
    1544:	00084803          	lbu	a6,0(a6)
    1548:	00054503          	lbu	a0,0(a0)
    154c:	0005c583          	lbu	a1,0(a1)
    1550:	00044403          	lbu	s0,0(s0)
    for (j = 0; j < (sizeof(uint64) * 2); j++, x <<= 4)
    1554:	03879713          	slli	a4,a5,0x38
        buf[i++] = digits[x >> (sizeof(uint64) * 8 - 4)];
    1558:	9371                	srli	a4,a4,0x3c
    155a:	8bbd                	andi	a5,a5,15
    155c:	974e                	add	a4,a4,s3
    155e:	97ce                	add	a5,a5,s3
    1560:	005105a3          	sb	t0,11(sp)
    1564:	01f10623          	sb	t6,12(sp)
    1568:	01e106a3          	sb	t5,13(sp)
    156c:	01d10723          	sb	t4,14(sp)
    1570:	01c107a3          	sb	t3,15(sp)
    1574:	00610823          	sb	t1,16(sp)
    1578:	011108a3          	sb	a7,17(sp)
    157c:	00710923          	sb	t2,18(sp)
    1580:	010109a3          	sb	a6,19(sp)
    1584:	00a10a23          	sb	a0,20(sp)
    1588:	00b10aa3          	sb	a1,21(sp)
    158c:	00110b23          	sb	ra,22(sp)
    1590:	00d10ba3          	sb	a3,23(sp)
    1594:	00810523          	sb	s0,10(sp)
    1598:	00074703          	lbu	a4,0(a4)
    159c:	0007c783          	lbu	a5,0(a5)
    write(f, s, l);
    15a0:	002c                	addi	a1,sp,8
    15a2:	4505                	li	a0,1
        buf[i++] = digits[x >> (sizeof(uint64) * 8 - 4)];
    15a4:	00e10c23          	sb	a4,24(sp)
    15a8:	00f10ca3          	sb	a5,25(sp)
    buf[i] = 0;
    15ac:	00010d23          	sb	zero,26(sp)
    write(f, s, l);
    15b0:	79c000ef          	jal	ra,1d4c <write>
            // Print unknown % sequence to draw attention.
            putchar('%');
            putchar(s[1]);
            break;
        }
        s += 2;
    15b4:	00248513          	addi	a0,s1,2
        if (!*s)
    15b8:	00054783          	lbu	a5,0(a0)
    15bc:	e8079ae3          	bnez	a5,1450 <printf+0x4a>
    }
    va_end(ap);
}
    15c0:	70a6                	ld	ra,104(sp)
    15c2:	7406                	ld	s0,96(sp)
    15c4:	64e6                	ld	s1,88(sp)
    15c6:	6946                	ld	s2,80(sp)
    15c8:	69a6                	ld	s3,72(sp)
    15ca:	6a06                	ld	s4,64(sp)
    15cc:	7ae2                	ld	s5,56(sp)
    15ce:	7b42                	ld	s6,48(sp)
    15d0:	614d                	addi	sp,sp,176
    15d2:	8082                	ret
        for (z = s; s[0] == '%' && s[1] == '%'; z++, s += 2)
    15d4:	00064783          	lbu	a5,0(a2)
    15d8:	84b2                	mv	s1,a2
    15da:	01278963          	beq	a5,s2,15ec <printf+0x1e6>
    15de:	b549                	j	1460 <printf+0x5a>
    15e0:	0024c783          	lbu	a5,2(s1)
    15e4:	0605                	addi	a2,a2,1
    15e6:	0489                	addi	s1,s1,2
    15e8:	e7279ce3          	bne	a5,s2,1460 <printf+0x5a>
    15ec:	0014c783          	lbu	a5,1(s1)
    15f0:	ff2788e3          	beq	a5,s2,15e0 <printf+0x1da>
        l = z - a;
    15f4:	40a6043b          	subw	s0,a2,a0
    write(f, s, l);
    15f8:	85aa                	mv	a1,a0
    15fa:	8622                	mv	a2,s0
    15fc:	4505                	li	a0,1
    15fe:	74e000ef          	jal	ra,1d4c <write>
        if (l)
    1602:	e60408e3          	beqz	s0,1472 <printf+0x6c>
    1606:	8526                	mv	a0,s1
    1608:	b581                	j	1448 <printf+0x42>
        switch (s[1])
    160a:	07800713          	li	a4,120
    160e:	04e79963          	bne	a5,a4,1660 <printf+0x25a>
            printint(va_arg(ap, int), 16, 1);
    1612:	6782                	ld	a5,0(sp)
    1614:	45c1                	li	a1,16
    1616:	4388                	lw	a0,0(a5)
    1618:	07a1                	addi	a5,a5,8
    161a:	e03e                	sd	a5,0(sp)
    161c:	b71ff0ef          	jal	ra,118c <printint.constprop.0>
        s += 2;
    1620:	00248513          	addi	a0,s1,2
    1624:	bf51                	j	15b8 <printf+0x1b2>
            printint(va_arg(ap, int), 10, 1);
    1626:	6782                	ld	a5,0(sp)
    1628:	45a9                	li	a1,10
    162a:	4388                	lw	a0,0(a5)
    162c:	07a1                	addi	a5,a5,8
    162e:	e03e                	sd	a5,0(sp)
    1630:	b5dff0ef          	jal	ra,118c <printint.constprop.0>
        s += 2;
    1634:	00248513          	addi	a0,s1,2
    1638:	b741                	j	15b8 <printf+0x1b2>
            if ((a = va_arg(ap, char *)) == 0)
    163a:	6782                	ld	a5,0(sp)
    163c:	6380                	ld	s0,0(a5)
    163e:	07a1                	addi	a5,a5,8
    1640:	e03e                	sd	a5,0(sp)
    1642:	c031                	beqz	s0,1686 <printf+0x280>
            l = strnlen(a, 200);
    1644:	0c800593          	li	a1,200
    1648:	8522                	mv	a0,s0
    164a:	40a000ef          	jal	ra,1a54 <strnlen>
    write(f, s, l);
    164e:	0005061b          	sext.w	a2,a0
    1652:	85a2                	mv	a1,s0
    1654:	4505                	li	a0,1
    1656:	6f6000ef          	jal	ra,1d4c <write>
        s += 2;
    165a:	00248513          	addi	a0,s1,2
    165e:	bfa9                	j	15b8 <printf+0x1b2>
    return write(stdout, &byte, 1);
    1660:	4605                	li	a2,1
    1662:	002c                	addi	a1,sp,8
    1664:	4505                	li	a0,1
    char byte = c;
    1666:	01210423          	sb	s2,8(sp)
    return write(stdout, &byte, 1);
    166a:	6e2000ef          	jal	ra,1d4c <write>
    char byte = c;
    166e:	0014c783          	lbu	a5,1(s1)
    return write(stdout, &byte, 1);
    1672:	4605                	li	a2,1
    1674:	002c                	addi	a1,sp,8
    1676:	4505                	li	a0,1
    char byte = c;
    1678:	00f10423          	sb	a5,8(sp)
    return write(stdout, &byte, 1);
    167c:	6d0000ef          	jal	ra,1d4c <write>
        s += 2;
    1680:	00248513          	addi	a0,s1,2
    1684:	bf15                	j	15b8 <printf+0x1b2>
                a = "(null)";
    1686:	845a                	mv	s0,s6
    1688:	bf75                	j	1644 <printf+0x23e>

000000000000168a <panic>:
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

void panic(char *m)
{
    168a:	1141                	addi	sp,sp,-16
    168c:	e406                	sd	ra,8(sp)
    puts(m);
    168e:	d57ff0ef          	jal	ra,13e4 <puts>
    exit(-100);
}
    1692:	60a2                	ld	ra,8(sp)
    exit(-100);
    1694:	f9c00513          	li	a0,-100
}
    1698:	0141                	addi	sp,sp,16
    exit(-100);
    169a:	a709                	j	1d9c <exit>

000000000000169c <isspace>:
#define HIGHS (ONES * (UCHAR_MAX / 2 + 1))
#define HASZERO(x) (((x)-ONES) & ~(x)&HIGHS)

int isspace(int c)
{
    return c == ' ' || (unsigned)c - '\t' < 5;
    169c:	02000793          	li	a5,32
    16a0:	00f50663          	beq	a0,a5,16ac <isspace+0x10>
    16a4:	355d                	addiw	a0,a0,-9
    16a6:	00553513          	sltiu	a0,a0,5
    16aa:	8082                	ret
    16ac:	4505                	li	a0,1
}
    16ae:	8082                	ret

00000000000016b0 <isdigit>:

int isdigit(int c)
{
    return (unsigned)c - '0' < 10;
    16b0:	fd05051b          	addiw	a0,a0,-48
}
    16b4:	00a53513          	sltiu	a0,a0,10
    16b8:	8082                	ret

00000000000016ba <atoi>:
    return c == ' ' || (unsigned)c - '\t' < 5;
    16ba:	02000613          	li	a2,32
    16be:	4591                	li	a1,4

int atoi(const char *s)
{
    int n = 0, neg = 0;
    while (isspace(*s))
    16c0:	00054703          	lbu	a4,0(a0)
    return c == ' ' || (unsigned)c - '\t' < 5;
    16c4:	ff77069b          	addiw	a3,a4,-9
    16c8:	04c70d63          	beq	a4,a2,1722 <atoi+0x68>
    16cc:	0007079b          	sext.w	a5,a4
    16d0:	04d5f963          	bgeu	a1,a3,1722 <atoi+0x68>
        s++;
    switch (*s)
    16d4:	02b00693          	li	a3,43
    16d8:	04d70a63          	beq	a4,a3,172c <atoi+0x72>
    16dc:	02d00693          	li	a3,45
    16e0:	06d70463          	beq	a4,a3,1748 <atoi+0x8e>
        neg = 1;
    case '+':
        s++;
    }
    /* Compute n as a negative number to avoid overflow on INT_MIN */
    while (isdigit(*s))
    16e4:	fd07859b          	addiw	a1,a5,-48
    16e8:	4625                	li	a2,9
    16ea:	873e                	mv	a4,a5
    16ec:	86aa                	mv	a3,a0
    int n = 0, neg = 0;
    16ee:	4e01                	li	t3,0
    while (isdigit(*s))
    16f0:	04b66a63          	bltu	a2,a1,1744 <atoi+0x8a>
    int n = 0, neg = 0;
    16f4:	4501                	li	a0,0
    while (isdigit(*s))
    16f6:	4825                	li	a6,9
    16f8:	0016c603          	lbu	a2,1(a3)
        n = 10 * n - (*s++ - '0');
    16fc:	0025179b          	slliw	a5,a0,0x2
    1700:	9d3d                	addw	a0,a0,a5
    1702:	fd07031b          	addiw	t1,a4,-48
    1706:	0015189b          	slliw	a7,a0,0x1
    while (isdigit(*s))
    170a:	fd06059b          	addiw	a1,a2,-48
        n = 10 * n - (*s++ - '0');
    170e:	0685                	addi	a3,a3,1
    1710:	4068853b          	subw	a0,a7,t1
    while (isdigit(*s))
    1714:	0006071b          	sext.w	a4,a2
    1718:	feb870e3          	bgeu	a6,a1,16f8 <atoi+0x3e>
    return neg ? n : -n;
    171c:	000e0563          	beqz	t3,1726 <atoi+0x6c>
}
    1720:	8082                	ret
        s++;
    1722:	0505                	addi	a0,a0,1
    1724:	bf71                	j	16c0 <atoi+0x6>
    return neg ? n : -n;
    1726:	4113053b          	subw	a0,t1,a7
    172a:	8082                	ret
    while (isdigit(*s))
    172c:	00154783          	lbu	a5,1(a0)
    1730:	4625                	li	a2,9
        s++;
    1732:	00150693          	addi	a3,a0,1
    while (isdigit(*s))
    1736:	fd07859b          	addiw	a1,a5,-48
    173a:	0007871b          	sext.w	a4,a5
    int n = 0, neg = 0;
    173e:	4e01                	li	t3,0
    while (isdigit(*s))
    1740:	fab67ae3          	bgeu	a2,a1,16f4 <atoi+0x3a>
    1744:	4501                	li	a0,0
}
    1746:	8082                	ret
    while (isdigit(*s))
    1748:	00154783          	lbu	a5,1(a0)
    174c:	4625                	li	a2,9
        s++;
    174e:	00150693          	addi	a3,a0,1
    while (isdigit(*s))
    1752:	fd07859b          	addiw	a1,a5,-48
    1756:	0007871b          	sext.w	a4,a5
    175a:	feb665e3          	bltu	a2,a1,1744 <atoi+0x8a>
        neg = 1;
    175e:	4e05                	li	t3,1
    1760:	bf51                	j	16f4 <atoi+0x3a>

0000000000001762 <memset>:

void *memset(void *dest, int c, size_t n)
{
    char *p = dest;
    for (int i = 0; i < n; ++i, *(p++) = c)
    1762:	16060d63          	beqz	a2,18dc <memset+0x17a>
    1766:	40a007b3          	neg	a5,a0
    176a:	8b9d                	andi	a5,a5,7
    176c:	00778713          	addi	a4,a5,7
    1770:	482d                	li	a6,11
    1772:	0ff5f593          	andi	a1,a1,255
    1776:	fff60693          	addi	a3,a2,-1
    177a:	17076263          	bltu	a4,a6,18de <memset+0x17c>
    177e:	16e6ea63          	bltu	a3,a4,18f2 <memset+0x190>
    1782:	16078563          	beqz	a5,18ec <memset+0x18a>
    1786:	00b50023          	sb	a1,0(a0)
    178a:	4705                	li	a4,1
    178c:	00150e93          	addi	t4,a0,1
    1790:	14e78c63          	beq	a5,a4,18e8 <memset+0x186>
    1794:	00b500a3          	sb	a1,1(a0)
    1798:	4709                	li	a4,2
    179a:	00250e93          	addi	t4,a0,2
    179e:	14e78d63          	beq	a5,a4,18f8 <memset+0x196>
    17a2:	00b50123          	sb	a1,2(a0)
    17a6:	470d                	li	a4,3
    17a8:	00350e93          	addi	t4,a0,3
    17ac:	12e78b63          	beq	a5,a4,18e2 <memset+0x180>
    17b0:	00b501a3          	sb	a1,3(a0)
    17b4:	4711                	li	a4,4
    17b6:	00450e93          	addi	t4,a0,4
    17ba:	14e78163          	beq	a5,a4,18fc <memset+0x19a>
    17be:	00b50223          	sb	a1,4(a0)
    17c2:	4715                	li	a4,5
    17c4:	00550e93          	addi	t4,a0,5
    17c8:	12e78c63          	beq	a5,a4,1900 <memset+0x19e>
    17cc:	00b502a3          	sb	a1,5(a0)
    17d0:	471d                	li	a4,7
    17d2:	00650e93          	addi	t4,a0,6
    17d6:	12e79763          	bne	a5,a4,1904 <memset+0x1a2>
    17da:	00750e93          	addi	t4,a0,7
    17de:	00b50323          	sb	a1,6(a0)
    17e2:	4f1d                	li	t5,7
    17e4:	00859713          	slli	a4,a1,0x8
    17e8:	8f4d                	or	a4,a4,a1
    17ea:	01059e13          	slli	t3,a1,0x10
    17ee:	01c76e33          	or	t3,a4,t3
    17f2:	01859313          	slli	t1,a1,0x18
    17f6:	006e6333          	or	t1,t3,t1
    17fa:	02059893          	slli	a7,a1,0x20
    17fe:	011368b3          	or	a7,t1,a7
    1802:	02859813          	slli	a6,a1,0x28
    1806:	40f60333          	sub	t1,a2,a5
    180a:	0108e833          	or	a6,a7,a6
    180e:	03059693          	slli	a3,a1,0x30
    1812:	00d866b3          	or	a3,a6,a3
    1816:	03859713          	slli	a4,a1,0x38
    181a:	97aa                	add	a5,a5,a0
    181c:	ff837813          	andi	a6,t1,-8
    1820:	8f55                	or	a4,a4,a3
    1822:	00f806b3          	add	a3,a6,a5
    1826:	e398                	sd	a4,0(a5)
    1828:	07a1                	addi	a5,a5,8
    182a:	fed79ee3          	bne	a5,a3,1826 <memset+0xc4>
    182e:	ff837693          	andi	a3,t1,-8
    1832:	00de87b3          	add	a5,t4,a3
    1836:	01e6873b          	addw	a4,a3,t5
    183a:	0ad30663          	beq	t1,a3,18e6 <memset+0x184>
    183e:	00b78023          	sb	a1,0(a5)
    1842:	0017069b          	addiw	a3,a4,1
    1846:	08c6fb63          	bgeu	a3,a2,18dc <memset+0x17a>
    184a:	00b780a3          	sb	a1,1(a5)
    184e:	0027069b          	addiw	a3,a4,2
    1852:	08c6f563          	bgeu	a3,a2,18dc <memset+0x17a>
    1856:	00b78123          	sb	a1,2(a5)
    185a:	0037069b          	addiw	a3,a4,3
    185e:	06c6ff63          	bgeu	a3,a2,18dc <memset+0x17a>
    1862:	00b781a3          	sb	a1,3(a5)
    1866:	0047069b          	addiw	a3,a4,4
    186a:	06c6f963          	bgeu	a3,a2,18dc <memset+0x17a>
    186e:	00b78223          	sb	a1,4(a5)
    1872:	0057069b          	addiw	a3,a4,5
    1876:	06c6f363          	bgeu	a3,a2,18dc <memset+0x17a>
    187a:	00b782a3          	sb	a1,5(a5)
    187e:	0067069b          	addiw	a3,a4,6
    1882:	04c6fd63          	bgeu	a3,a2,18dc <memset+0x17a>
    1886:	00b78323          	sb	a1,6(a5)
    188a:	0077069b          	addiw	a3,a4,7
    188e:	04c6f763          	bgeu	a3,a2,18dc <memset+0x17a>
    1892:	00b783a3          	sb	a1,7(a5)
    1896:	0087069b          	addiw	a3,a4,8
    189a:	04c6f163          	bgeu	a3,a2,18dc <memset+0x17a>
    189e:	00b78423          	sb	a1,8(a5)
    18a2:	0097069b          	addiw	a3,a4,9
    18a6:	02c6fb63          	bgeu	a3,a2,18dc <memset+0x17a>
    18aa:	00b784a3          	sb	a1,9(a5)
    18ae:	00a7069b          	addiw	a3,a4,10
    18b2:	02c6f563          	bgeu	a3,a2,18dc <memset+0x17a>
    18b6:	00b78523          	sb	a1,10(a5)
    18ba:	00b7069b          	addiw	a3,a4,11
    18be:	00c6ff63          	bgeu	a3,a2,18dc <memset+0x17a>
    18c2:	00b785a3          	sb	a1,11(a5)
    18c6:	00c7069b          	addiw	a3,a4,12
    18ca:	00c6f963          	bgeu	a3,a2,18dc <memset+0x17a>
    18ce:	00b78623          	sb	a1,12(a5)
    18d2:	2735                	addiw	a4,a4,13
    18d4:	00c77463          	bgeu	a4,a2,18dc <memset+0x17a>
    18d8:	00b786a3          	sb	a1,13(a5)
        ;
    return dest;
}
    18dc:	8082                	ret
    18de:	472d                	li	a4,11
    18e0:	bd79                	j	177e <memset+0x1c>
    for (int i = 0; i < n; ++i, *(p++) = c)
    18e2:	4f0d                	li	t5,3
    18e4:	b701                	j	17e4 <memset+0x82>
    18e6:	8082                	ret
    18e8:	4f05                	li	t5,1
    18ea:	bded                	j	17e4 <memset+0x82>
    18ec:	8eaa                	mv	t4,a0
    18ee:	4f01                	li	t5,0
    18f0:	bdd5                	j	17e4 <memset+0x82>
    18f2:	87aa                	mv	a5,a0
    18f4:	4701                	li	a4,0
    18f6:	b7a1                	j	183e <memset+0xdc>
    18f8:	4f09                	li	t5,2
    18fa:	b5ed                	j	17e4 <memset+0x82>
    18fc:	4f11                	li	t5,4
    18fe:	b5dd                	j	17e4 <memset+0x82>
    1900:	4f15                	li	t5,5
    1902:	b5cd                	j	17e4 <memset+0x82>
    1904:	4f19                	li	t5,6
    1906:	bdf9                	j	17e4 <memset+0x82>

0000000000001908 <strcmp>:

int strcmp(const char *l, const char *r)
{
    for (; *l == *r && *l; l++, r++)
    1908:	00054783          	lbu	a5,0(a0)
    190c:	0005c703          	lbu	a4,0(a1)
    1910:	00e79863          	bne	a5,a4,1920 <strcmp+0x18>
    1914:	0505                	addi	a0,a0,1
    1916:	0585                	addi	a1,a1,1
    1918:	fbe5                	bnez	a5,1908 <strcmp>
    191a:	4501                	li	a0,0
        ;
    return *(unsigned char *)l - *(unsigned char *)r;
}
    191c:	9d19                	subw	a0,a0,a4
    191e:	8082                	ret
    return *(unsigned char *)l - *(unsigned char *)r;
    1920:	0007851b          	sext.w	a0,a5
    1924:	bfe5                	j	191c <strcmp+0x14>

0000000000001926 <strncmp>:

int strncmp(const char *_l, const char *_r, size_t n)
{
    const unsigned char *l = (void *)_l, *r = (void *)_r;
    if (!n--)
    1926:	ce05                	beqz	a2,195e <strncmp+0x38>
        return 0;
    for (; *l && *r && n && *l == *r; l++, r++, n--)
    1928:	00054703          	lbu	a4,0(a0)
    192c:	0005c783          	lbu	a5,0(a1)
    1930:	cb0d                	beqz	a4,1962 <strncmp+0x3c>
    if (!n--)
    1932:	167d                	addi	a2,a2,-1
    1934:	00c506b3          	add	a3,a0,a2
    1938:	a819                	j	194e <strncmp+0x28>
    for (; *l && *r && n && *l == *r; l++, r++, n--)
    193a:	00a68e63          	beq	a3,a0,1956 <strncmp+0x30>
    193e:	0505                	addi	a0,a0,1
    1940:	00e79b63          	bne	a5,a4,1956 <strncmp+0x30>
    1944:	00054703          	lbu	a4,0(a0)
        ;
    return *l - *r;
    1948:	0005c783          	lbu	a5,0(a1)
    for (; *l && *r && n && *l == *r; l++, r++, n--)
    194c:	cb19                	beqz	a4,1962 <strncmp+0x3c>
    194e:	0005c783          	lbu	a5,0(a1)
    1952:	0585                	addi	a1,a1,1
    1954:	f3fd                	bnez	a5,193a <strncmp+0x14>
    return *l - *r;
    1956:	0007051b          	sext.w	a0,a4
    195a:	9d1d                	subw	a0,a0,a5
    195c:	8082                	ret
        return 0;
    195e:	4501                	li	a0,0
}
    1960:	8082                	ret
    1962:	4501                	li	a0,0
    return *l - *r;
    1964:	9d1d                	subw	a0,a0,a5
    1966:	8082                	ret

0000000000001968 <strlen>:
size_t strlen(const char *s)
{
    const char *a = s;
    typedef size_t __attribute__((__may_alias__)) word;
    const word *w;
    for (; (uintptr_t)s % SS; s++)
    1968:	00757793          	andi	a5,a0,7
    196c:	cf89                	beqz	a5,1986 <strlen+0x1e>
    196e:	87aa                	mv	a5,a0
    1970:	a029                	j	197a <strlen+0x12>
    1972:	0785                	addi	a5,a5,1
    1974:	0077f713          	andi	a4,a5,7
    1978:	cb01                	beqz	a4,1988 <strlen+0x20>
        if (!*s)
    197a:	0007c703          	lbu	a4,0(a5)
    197e:	fb75                	bnez	a4,1972 <strlen+0xa>
    for (w = (const void *)s; !HASZERO(*w); w++)
        ;
    s = (const void *)w;
    for (; *s; s++)
        ;
    return s - a;
    1980:	40a78533          	sub	a0,a5,a0
}
    1984:	8082                	ret
    for (; (uintptr_t)s % SS; s++)
    1986:	87aa                	mv	a5,a0
    for (w = (const void *)s; !HASZERO(*w); w++)
    1988:	6394                	ld	a3,0(a5)
    198a:	00000597          	auipc	a1,0x0
    198e:	70e5b583          	ld	a1,1806(a1) # 2098 <__clone+0x118>
    1992:	00000617          	auipc	a2,0x0
    1996:	70e63603          	ld	a2,1806(a2) # 20a0 <__clone+0x120>
    199a:	a019                	j	19a0 <strlen+0x38>
    199c:	6794                	ld	a3,8(a5)
    199e:	07a1                	addi	a5,a5,8
    19a0:	00b68733          	add	a4,a3,a1
    19a4:	fff6c693          	not	a3,a3
    19a8:	8f75                	and	a4,a4,a3
    19aa:	8f71                	and	a4,a4,a2
    19ac:	db65                	beqz	a4,199c <strlen+0x34>
    for (; *s; s++)
    19ae:	0007c703          	lbu	a4,0(a5)
    19b2:	d779                	beqz	a4,1980 <strlen+0x18>
    19b4:	0017c703          	lbu	a4,1(a5)
    19b8:	0785                	addi	a5,a5,1
    19ba:	d379                	beqz	a4,1980 <strlen+0x18>
    19bc:	0017c703          	lbu	a4,1(a5)
    19c0:	0785                	addi	a5,a5,1
    19c2:	fb6d                	bnez	a4,19b4 <strlen+0x4c>
    19c4:	bf75                	j	1980 <strlen+0x18>

00000000000019c6 <memchr>:

void *memchr(const void *src, int c, size_t n)
{
    const unsigned char *s = src;
    c = (unsigned char)c;
    for (; ((uintptr_t)s & ALIGN) && n && *s != c; s++, n--)
    19c6:	00757713          	andi	a4,a0,7
{
    19ca:	87aa                	mv	a5,a0
    c = (unsigned char)c;
    19cc:	0ff5f593          	andi	a1,a1,255
    for (; ((uintptr_t)s & ALIGN) && n && *s != c; s++, n--)
    19d0:	cb19                	beqz	a4,19e6 <memchr+0x20>
    19d2:	ce25                	beqz	a2,1a4a <memchr+0x84>
    19d4:	0007c703          	lbu	a4,0(a5)
    19d8:	04b70e63          	beq	a4,a1,1a34 <memchr+0x6e>
    19dc:	0785                	addi	a5,a5,1
    19de:	0077f713          	andi	a4,a5,7
    19e2:	167d                	addi	a2,a2,-1
    19e4:	f77d                	bnez	a4,19d2 <memchr+0xc>
            ;
        s = (const void *)w;
    }
    for (; n && *s != c; s++, n--)
        ;
    return n ? (void *)s : 0;
    19e6:	4501                	li	a0,0
    if (n && *s != c)
    19e8:	c235                	beqz	a2,1a4c <memchr+0x86>
    19ea:	0007c703          	lbu	a4,0(a5)
    19ee:	04b70363          	beq	a4,a1,1a34 <memchr+0x6e>
        size_t k = ONES * c;
    19f2:	00000517          	auipc	a0,0x0
    19f6:	6b653503          	ld	a0,1718(a0) # 20a8 <__clone+0x128>
        for (w = (const void *)s; n >= SS && !HASZERO(*w ^ k); w++, n -= SS)
    19fa:	471d                	li	a4,7
        size_t k = ONES * c;
    19fc:	02a58533          	mul	a0,a1,a0
        for (w = (const void *)s; n >= SS && !HASZERO(*w ^ k); w++, n -= SS)
    1a00:	02c77a63          	bgeu	a4,a2,1a34 <memchr+0x6e>
    1a04:	00000897          	auipc	a7,0x0
    1a08:	6948b883          	ld	a7,1684(a7) # 2098 <__clone+0x118>
    1a0c:	00000817          	auipc	a6,0x0
    1a10:	69483803          	ld	a6,1684(a6) # 20a0 <__clone+0x120>
    1a14:	431d                	li	t1,7
    1a16:	a029                	j	1a20 <memchr+0x5a>
    1a18:	1661                	addi	a2,a2,-8
    1a1a:	07a1                	addi	a5,a5,8
    1a1c:	02c37963          	bgeu	t1,a2,1a4e <memchr+0x88>
    1a20:	6398                	ld	a4,0(a5)
    1a22:	8f29                	xor	a4,a4,a0
    1a24:	011706b3          	add	a3,a4,a7
    1a28:	fff74713          	not	a4,a4
    1a2c:	8f75                	and	a4,a4,a3
    1a2e:	01077733          	and	a4,a4,a6
    1a32:	d37d                	beqz	a4,1a18 <memchr+0x52>
    1a34:	853e                	mv	a0,a5
    1a36:	97b2                	add	a5,a5,a2
    1a38:	a021                	j	1a40 <memchr+0x7a>
    for (; n && *s != c; s++, n--)
    1a3a:	0505                	addi	a0,a0,1
    1a3c:	00f50763          	beq	a0,a5,1a4a <memchr+0x84>
    1a40:	00054703          	lbu	a4,0(a0)
    1a44:	feb71be3          	bne	a4,a1,1a3a <memchr+0x74>
    1a48:	8082                	ret
    return n ? (void *)s : 0;
    1a4a:	4501                	li	a0,0
}
    1a4c:	8082                	ret
    return n ? (void *)s : 0;
    1a4e:	4501                	li	a0,0
    for (; n && *s != c; s++, n--)
    1a50:	f275                	bnez	a2,1a34 <memchr+0x6e>
}
    1a52:	8082                	ret

0000000000001a54 <strnlen>:

size_t strnlen(const char *s, size_t n)
{
    1a54:	1101                	addi	sp,sp,-32
    1a56:	e822                	sd	s0,16(sp)
    const char *p = memchr(s, 0, n);
    1a58:	862e                	mv	a2,a1
{
    1a5a:	842e                	mv	s0,a1
    const char *p = memchr(s, 0, n);
    1a5c:	4581                	li	a1,0
{
    1a5e:	e426                	sd	s1,8(sp)
    1a60:	ec06                	sd	ra,24(sp)
    1a62:	84aa                	mv	s1,a0
    const char *p = memchr(s, 0, n);
    1a64:	f63ff0ef          	jal	ra,19c6 <memchr>
    return p ? p - s : n;
    1a68:	c519                	beqz	a0,1a76 <strnlen+0x22>
}
    1a6a:	60e2                	ld	ra,24(sp)
    1a6c:	6442                	ld	s0,16(sp)
    return p ? p - s : n;
    1a6e:	8d05                	sub	a0,a0,s1
}
    1a70:	64a2                	ld	s1,8(sp)
    1a72:	6105                	addi	sp,sp,32
    1a74:	8082                	ret
    1a76:	60e2                	ld	ra,24(sp)
    return p ? p - s : n;
    1a78:	8522                	mv	a0,s0
}
    1a7a:	6442                	ld	s0,16(sp)
    1a7c:	64a2                	ld	s1,8(sp)
    1a7e:	6105                	addi	sp,sp,32
    1a80:	8082                	ret

0000000000001a82 <strcpy>:
char *strcpy(char *restrict d, const char *s)
{
    typedef size_t __attribute__((__may_alias__)) word;
    word *wd;
    const word *ws;
    if ((uintptr_t)s % SS == (uintptr_t)d % SS)
    1a82:	00b547b3          	xor	a5,a0,a1
    1a86:	8b9d                	andi	a5,a5,7
    1a88:	eb95                	bnez	a5,1abc <strcpy+0x3a>
    {
        for (; (uintptr_t)s % SS; s++, d++)
    1a8a:	0075f793          	andi	a5,a1,7
    1a8e:	e7b1                	bnez	a5,1ada <strcpy+0x58>
            if (!(*d = *s))
                return d;
        wd = (void *)d;
        ws = (const void *)s;
        for (; !HASZERO(*ws); *wd++ = *ws++)
    1a90:	6198                	ld	a4,0(a1)
    1a92:	00000617          	auipc	a2,0x0
    1a96:	60663603          	ld	a2,1542(a2) # 2098 <__clone+0x118>
    1a9a:	00000817          	auipc	a6,0x0
    1a9e:	60683803          	ld	a6,1542(a6) # 20a0 <__clone+0x120>
    1aa2:	a029                	j	1aac <strcpy+0x2a>
    1aa4:	e118                	sd	a4,0(a0)
    1aa6:	6598                	ld	a4,8(a1)
    1aa8:	05a1                	addi	a1,a1,8
    1aaa:	0521                	addi	a0,a0,8
    1aac:	00c707b3          	add	a5,a4,a2
    1ab0:	fff74693          	not	a3,a4
    1ab4:	8ff5                	and	a5,a5,a3
    1ab6:	0107f7b3          	and	a5,a5,a6
    1aba:	d7ed                	beqz	a5,1aa4 <strcpy+0x22>
            ;
        d = (void *)wd;
        s = (const void *)ws;
    }
    for (; (*d = *s); s++, d++)
    1abc:	0005c783          	lbu	a5,0(a1)
    1ac0:	00f50023          	sb	a5,0(a0)
    1ac4:	c785                	beqz	a5,1aec <strcpy+0x6a>
    1ac6:	0015c783          	lbu	a5,1(a1)
    1aca:	0505                	addi	a0,a0,1
    1acc:	0585                	addi	a1,a1,1
    1ace:	00f50023          	sb	a5,0(a0)
    1ad2:	fbf5                	bnez	a5,1ac6 <strcpy+0x44>
        ;
    return d;
}
    1ad4:	8082                	ret
        for (; (uintptr_t)s % SS; s++, d++)
    1ad6:	0505                	addi	a0,a0,1
    1ad8:	df45                	beqz	a4,1a90 <strcpy+0xe>
            if (!(*d = *s))
    1ada:	0005c783          	lbu	a5,0(a1)
        for (; (uintptr_t)s % SS; s++, d++)
    1ade:	0585                	addi	a1,a1,1
    1ae0:	0075f713          	andi	a4,a1,7
            if (!(*d = *s))
    1ae4:	00f50023          	sb	a5,0(a0)
    1ae8:	f7fd                	bnez	a5,1ad6 <strcpy+0x54>
}
    1aea:	8082                	ret
    1aec:	8082                	ret

0000000000001aee <strncpy>:
char *strncpy(char *restrict d, const char *s, size_t n)
{
    typedef size_t __attribute__((__may_alias__)) word;
    word *wd;
    const word *ws;
    if (((uintptr_t)s & ALIGN) == ((uintptr_t)d & ALIGN))
    1aee:	00b547b3          	xor	a5,a0,a1
    1af2:	8b9d                	andi	a5,a5,7
    1af4:	1a079863          	bnez	a5,1ca4 <strncpy+0x1b6>
    {
        for (; ((uintptr_t)s & ALIGN) && n && (*d = *s); n--, s++, d++)
    1af8:	0075f793          	andi	a5,a1,7
    1afc:	16078463          	beqz	a5,1c64 <strncpy+0x176>
    1b00:	ea01                	bnez	a2,1b10 <strncpy+0x22>
    1b02:	a421                	j	1d0a <strncpy+0x21c>
    1b04:	167d                	addi	a2,a2,-1
    1b06:	0505                	addi	a0,a0,1
    1b08:	14070e63          	beqz	a4,1c64 <strncpy+0x176>
    1b0c:	1a060863          	beqz	a2,1cbc <strncpy+0x1ce>
    1b10:	0005c783          	lbu	a5,0(a1)
    1b14:	0585                	addi	a1,a1,1
    1b16:	0075f713          	andi	a4,a1,7
    1b1a:	00f50023          	sb	a5,0(a0)
    1b1e:	f3fd                	bnez	a5,1b04 <strncpy+0x16>
    1b20:	4805                	li	a6,1
    1b22:	1a061863          	bnez	a2,1cd2 <strncpy+0x1e4>
    1b26:	40a007b3          	neg	a5,a0
    1b2a:	8b9d                	andi	a5,a5,7
    1b2c:	4681                	li	a3,0
    1b2e:	18061a63          	bnez	a2,1cc2 <strncpy+0x1d4>
    1b32:	00778713          	addi	a4,a5,7
    1b36:	45ad                	li	a1,11
    1b38:	18b76363          	bltu	a4,a1,1cbe <strncpy+0x1d0>
    1b3c:	1ae6eb63          	bltu	a3,a4,1cf2 <strncpy+0x204>
    1b40:	1a078363          	beqz	a5,1ce6 <strncpy+0x1f8>
    for (int i = 0; i < n; ++i, *(p++) = c)
    1b44:	00050023          	sb	zero,0(a0)
    1b48:	4685                	li	a3,1
    1b4a:	00150713          	addi	a4,a0,1
    1b4e:	18d78f63          	beq	a5,a3,1cec <strncpy+0x1fe>
    1b52:	000500a3          	sb	zero,1(a0)
    1b56:	4689                	li	a3,2
    1b58:	00250713          	addi	a4,a0,2
    1b5c:	18d78e63          	beq	a5,a3,1cf8 <strncpy+0x20a>
    1b60:	00050123          	sb	zero,2(a0)
    1b64:	468d                	li	a3,3
    1b66:	00350713          	addi	a4,a0,3
    1b6a:	16d78c63          	beq	a5,a3,1ce2 <strncpy+0x1f4>
    1b6e:	000501a3          	sb	zero,3(a0)
    1b72:	4691                	li	a3,4
    1b74:	00450713          	addi	a4,a0,4
    1b78:	18d78263          	beq	a5,a3,1cfc <strncpy+0x20e>
    1b7c:	00050223          	sb	zero,4(a0)
    1b80:	4695                	li	a3,5
    1b82:	00550713          	addi	a4,a0,5
    1b86:	16d78d63          	beq	a5,a3,1d00 <strncpy+0x212>
    1b8a:	000502a3          	sb	zero,5(a0)
    1b8e:	469d                	li	a3,7
    1b90:	00650713          	addi	a4,a0,6
    1b94:	16d79863          	bne	a5,a3,1d04 <strncpy+0x216>
    1b98:	00750713          	addi	a4,a0,7
    1b9c:	00050323          	sb	zero,6(a0)
    1ba0:	40f80833          	sub	a6,a6,a5
    1ba4:	ff887593          	andi	a1,a6,-8
    1ba8:	97aa                	add	a5,a5,a0
    1baa:	95be                	add	a1,a1,a5
    1bac:	0007b023          	sd	zero,0(a5)
    1bb0:	07a1                	addi	a5,a5,8
    1bb2:	feb79de3          	bne	a5,a1,1bac <strncpy+0xbe>
    1bb6:	ff887593          	andi	a1,a6,-8
    1bba:	9ead                	addw	a3,a3,a1
    1bbc:	00b707b3          	add	a5,a4,a1
    1bc0:	12b80863          	beq	a6,a1,1cf0 <strncpy+0x202>
    1bc4:	00078023          	sb	zero,0(a5)
    1bc8:	0016871b          	addiw	a4,a3,1
    1bcc:	0ec77863          	bgeu	a4,a2,1cbc <strncpy+0x1ce>
    1bd0:	000780a3          	sb	zero,1(a5)
    1bd4:	0026871b          	addiw	a4,a3,2
    1bd8:	0ec77263          	bgeu	a4,a2,1cbc <strncpy+0x1ce>
    1bdc:	00078123          	sb	zero,2(a5)
    1be0:	0036871b          	addiw	a4,a3,3
    1be4:	0cc77c63          	bgeu	a4,a2,1cbc <strncpy+0x1ce>
    1be8:	000781a3          	sb	zero,3(a5)
    1bec:	0046871b          	addiw	a4,a3,4
    1bf0:	0cc77663          	bgeu	a4,a2,1cbc <strncpy+0x1ce>
    1bf4:	00078223          	sb	zero,4(a5)
    1bf8:	0056871b          	addiw	a4,a3,5
    1bfc:	0cc77063          	bgeu	a4,a2,1cbc <strncpy+0x1ce>
    1c00:	000782a3          	sb	zero,5(a5)
    1c04:	0066871b          	addiw	a4,a3,6
    1c08:	0ac77a63          	bgeu	a4,a2,1cbc <strncpy+0x1ce>
    1c0c:	00078323          	sb	zero,6(a5)
    1c10:	0076871b          	addiw	a4,a3,7
    1c14:	0ac77463          	bgeu	a4,a2,1cbc <strncpy+0x1ce>
    1c18:	000783a3          	sb	zero,7(a5)
    1c1c:	0086871b          	addiw	a4,a3,8
    1c20:	08c77e63          	bgeu	a4,a2,1cbc <strncpy+0x1ce>
    1c24:	00078423          	sb	zero,8(a5)
    1c28:	0096871b          	addiw	a4,a3,9
    1c2c:	08c77863          	bgeu	a4,a2,1cbc <strncpy+0x1ce>
    1c30:	000784a3          	sb	zero,9(a5)
    1c34:	00a6871b          	addiw	a4,a3,10
    1c38:	08c77263          	bgeu	a4,a2,1cbc <strncpy+0x1ce>
    1c3c:	00078523          	sb	zero,10(a5)
    1c40:	00b6871b          	addiw	a4,a3,11
    1c44:	06c77c63          	bgeu	a4,a2,1cbc <strncpy+0x1ce>
    1c48:	000785a3          	sb	zero,11(a5)
    1c4c:	00c6871b          	addiw	a4,a3,12
    1c50:	06c77663          	bgeu	a4,a2,1cbc <strncpy+0x1ce>
    1c54:	00078623          	sb	zero,12(a5)
    1c58:	26b5                	addiw	a3,a3,13
    1c5a:	06c6f163          	bgeu	a3,a2,1cbc <strncpy+0x1ce>
    1c5e:	000786a3          	sb	zero,13(a5)
    1c62:	8082                	ret
            ;
        if (!n || !*s)
    1c64:	c645                	beqz	a2,1d0c <strncpy+0x21e>
    1c66:	0005c783          	lbu	a5,0(a1)
    1c6a:	ea078be3          	beqz	a5,1b20 <strncpy+0x32>
            goto tail;
        wd = (void *)d;
        ws = (const void *)s;
        for (; n >= sizeof(size_t) && !HASZERO(*ws); n -= sizeof(size_t), ws++, wd++)
    1c6e:	479d                	li	a5,7
    1c70:	02c7ff63          	bgeu	a5,a2,1cae <strncpy+0x1c0>
    1c74:	00000897          	auipc	a7,0x0
    1c78:	4248b883          	ld	a7,1060(a7) # 2098 <__clone+0x118>
    1c7c:	00000817          	auipc	a6,0x0
    1c80:	42483803          	ld	a6,1060(a6) # 20a0 <__clone+0x120>
    1c84:	431d                	li	t1,7
    1c86:	6198                	ld	a4,0(a1)
    1c88:	011707b3          	add	a5,a4,a7
    1c8c:	fff74693          	not	a3,a4
    1c90:	8ff5                	and	a5,a5,a3
    1c92:	0107f7b3          	and	a5,a5,a6
    1c96:	ef81                	bnez	a5,1cae <strncpy+0x1c0>
            *wd = *ws;
    1c98:	e118                	sd	a4,0(a0)
        for (; n >= sizeof(size_t) && !HASZERO(*ws); n -= sizeof(size_t), ws++, wd++)
    1c9a:	1661                	addi	a2,a2,-8
    1c9c:	05a1                	addi	a1,a1,8
    1c9e:	0521                	addi	a0,a0,8
    1ca0:	fec363e3          	bltu	t1,a2,1c86 <strncpy+0x198>
        d = (void *)wd;
        s = (const void *)ws;
    }
    for (; n && (*d = *s); n--, s++, d++)
    1ca4:	e609                	bnez	a2,1cae <strncpy+0x1c0>
    1ca6:	a08d                	j	1d08 <strncpy+0x21a>
    1ca8:	167d                	addi	a2,a2,-1
    1caa:	0505                	addi	a0,a0,1
    1cac:	ca01                	beqz	a2,1cbc <strncpy+0x1ce>
    1cae:	0005c783          	lbu	a5,0(a1)
    1cb2:	0585                	addi	a1,a1,1
    1cb4:	00f50023          	sb	a5,0(a0)
    1cb8:	fbe5                	bnez	a5,1ca8 <strncpy+0x1ba>
        ;
tail:
    1cba:	b59d                	j	1b20 <strncpy+0x32>
    memset(d, 0, n);
    return d;
}
    1cbc:	8082                	ret
    1cbe:	472d                	li	a4,11
    1cc0:	bdb5                	j	1b3c <strncpy+0x4e>
    1cc2:	00778713          	addi	a4,a5,7
    1cc6:	45ad                	li	a1,11
    1cc8:	fff60693          	addi	a3,a2,-1
    1ccc:	e6b778e3          	bgeu	a4,a1,1b3c <strncpy+0x4e>
    1cd0:	b7fd                	j	1cbe <strncpy+0x1d0>
    1cd2:	40a007b3          	neg	a5,a0
    1cd6:	8832                	mv	a6,a2
    1cd8:	8b9d                	andi	a5,a5,7
    1cda:	4681                	li	a3,0
    1cdc:	e4060be3          	beqz	a2,1b32 <strncpy+0x44>
    1ce0:	b7cd                	j	1cc2 <strncpy+0x1d4>
    for (int i = 0; i < n; ++i, *(p++) = c)
    1ce2:	468d                	li	a3,3
    1ce4:	bd75                	j	1ba0 <strncpy+0xb2>
    1ce6:	872a                	mv	a4,a0
    1ce8:	4681                	li	a3,0
    1cea:	bd5d                	j	1ba0 <strncpy+0xb2>
    1cec:	4685                	li	a3,1
    1cee:	bd4d                	j	1ba0 <strncpy+0xb2>
    1cf0:	8082                	ret
    1cf2:	87aa                	mv	a5,a0
    1cf4:	4681                	li	a3,0
    1cf6:	b5f9                	j	1bc4 <strncpy+0xd6>
    1cf8:	4689                	li	a3,2
    1cfa:	b55d                	j	1ba0 <strncpy+0xb2>
    1cfc:	4691                	li	a3,4
    1cfe:	b54d                	j	1ba0 <strncpy+0xb2>
    1d00:	4695                	li	a3,5
    1d02:	bd79                	j	1ba0 <strncpy+0xb2>
    1d04:	4699                	li	a3,6
    1d06:	bd69                	j	1ba0 <strncpy+0xb2>
    1d08:	8082                	ret
    1d0a:	8082                	ret
    1d0c:	8082                	ret

0000000000001d0e <open>:
#include <unistd.h>

#include "syscall.h"

int open(const char *path, int flags)
{
    1d0e:	87aa                	mv	a5,a0
    1d10:	862e                	mv	a2,a1
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
}

static inline long __syscall4(long n, long a, long b, long c, long d)
{
    register long a7 __asm__("a7") = n;
    1d12:	03800893          	li	a7,56
    register long a0 __asm__("a0") = a;
    1d16:	f9c00513          	li	a0,-100
    register long a1 __asm__("a1") = b;
    1d1a:	85be                	mv	a1,a5
    register long a2 __asm__("a2") = c;
    register long a3 __asm__("a3") = d;
    1d1c:	4689                	li	a3,2
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3))
    1d1e:	00000073          	ecall
    return syscall(SYS_openat, AT_FDCWD, path, flags, O_RDWR);
}
    1d22:	2501                	sext.w	a0,a0
    1d24:	8082                	ret

0000000000001d26 <openat>:
    register long a7 __asm__("a7") = n;
    1d26:	03800893          	li	a7,56
    register long a3 __asm__("a3") = d;
    1d2a:	18000693          	li	a3,384
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3))
    1d2e:	00000073          	ecall

int openat(int dirfd,const char *path, int flags)
{
    return syscall(SYS_openat, dirfd, path, flags, 0600);
}
    1d32:	2501                	sext.w	a0,a0
    1d34:	8082                	ret

0000000000001d36 <close>:
    register long a7 __asm__("a7") = n;
    1d36:	03900893          	li	a7,57
    __asm_syscall("r"(a7), "0"(a0))
    1d3a:	00000073          	ecall

int close(int fd)
{
    return syscall(SYS_close, fd);
}
    1d3e:	2501                	sext.w	a0,a0
    1d40:	8082                	ret

0000000000001d42 <read>:
    register long a7 __asm__("a7") = n;
    1d42:	03f00893          	li	a7,63
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1d46:	00000073          	ecall

ssize_t read(int fd, void *buf, size_t len)
{
    return syscall(SYS_read, fd, buf, len);
}
    1d4a:	8082                	ret

0000000000001d4c <write>:
    register long a7 __asm__("a7") = n;
    1d4c:	04000893          	li	a7,64
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1d50:	00000073          	ecall

ssize_t write(int fd, const void *buf, size_t len)
{
    return syscall(SYS_write, fd, buf, len);
}
    1d54:	8082                	ret

0000000000001d56 <getpid>:
    register long a7 __asm__("a7") = n;
    1d56:	0ac00893          	li	a7,172
    __asm_syscall("r"(a7))
    1d5a:	00000073          	ecall

pid_t getpid(void)
{
    return syscall(SYS_getpid);
}
    1d5e:	2501                	sext.w	a0,a0
    1d60:	8082                	ret

0000000000001d62 <getppid>:
    register long a7 __asm__("a7") = n;
    1d62:	0ad00893          	li	a7,173
    __asm_syscall("r"(a7))
    1d66:	00000073          	ecall

pid_t getppid(void)
{
    return syscall(SYS_getppid);
}
    1d6a:	2501                	sext.w	a0,a0
    1d6c:	8082                	ret

0000000000001d6e <sched_yield>:
    register long a7 __asm__("a7") = n;
    1d6e:	07c00893          	li	a7,124
    __asm_syscall("r"(a7))
    1d72:	00000073          	ecall

int sched_yield(void)
{
    return syscall(SYS_sched_yield);
}
    1d76:	2501                	sext.w	a0,a0
    1d78:	8082                	ret

0000000000001d7a <fork>:
    register long a7 __asm__("a7") = n;
    1d7a:	0dc00893          	li	a7,220
    register long a0 __asm__("a0") = a;
    1d7e:	4545                	li	a0,17
    register long a1 __asm__("a1") = b;
    1d80:	4581                	li	a1,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1d82:	00000073          	ecall

pid_t fork(void)
{
    return syscall(SYS_clone, SIGCHLD, 0);
}
    1d86:	2501                	sext.w	a0,a0
    1d88:	8082                	ret

0000000000001d8a <clone>:

pid_t clone(int (*fn)(void *arg), void *arg, void *stack, size_t stack_size, unsigned long flags)
{
    1d8a:	85b2                	mv	a1,a2
    1d8c:	863a                	mv	a2,a4
    if (stack)
    1d8e:	c191                	beqz	a1,1d92 <clone+0x8>
	stack += stack_size;
    1d90:	95b6                	add	a1,a1,a3

    return __clone(fn, stack, flags, NULL, NULL, NULL);
    1d92:	4781                	li	a5,0
    1d94:	4701                	li	a4,0
    1d96:	4681                	li	a3,0
    1d98:	2601                	sext.w	a2,a2
    1d9a:	a2dd                	j	1f80 <__clone>

0000000000001d9c <exit>:
    register long a7 __asm__("a7") = n;
    1d9c:	05d00893          	li	a7,93
    __asm_syscall("r"(a7), "0"(a0))
    1da0:	00000073          	ecall
    //return syscall(SYS_clone, fn, stack, flags, NULL, NULL, NULL);
}
void exit(int code)
{
    syscall(SYS_exit, code);
}
    1da4:	8082                	ret

0000000000001da6 <waitpid>:
    register long a7 __asm__("a7") = n;
    1da6:	10400893          	li	a7,260
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1daa:	00000073          	ecall

int waitpid(int pid, int *code, int options)
{
    return syscall(SYS_wait4, pid, code, options);
}
    1dae:	2501                	sext.w	a0,a0
    1db0:	8082                	ret

0000000000001db2 <exec>:
    register long a7 __asm__("a7") = n;
    1db2:	0dd00893          	li	a7,221
    __asm_syscall("r"(a7), "0"(a0))
    1db6:	00000073          	ecall

int exec(char *name)
{
    return syscall(SYS_execve, name);
}
    1dba:	2501                	sext.w	a0,a0
    1dbc:	8082                	ret

0000000000001dbe <execve>:
    register long a7 __asm__("a7") = n;
    1dbe:	0dd00893          	li	a7,221
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1dc2:	00000073          	ecall

int execve(const char *name, char *const argv[], char *const argp[])
{
    return syscall(SYS_execve, name, argv, argp);
}
    1dc6:	2501                	sext.w	a0,a0
    1dc8:	8082                	ret

0000000000001dca <times>:
    register long a7 __asm__("a7") = n;
    1dca:	09900893          	li	a7,153
    __asm_syscall("r"(a7), "0"(a0))
    1dce:	00000073          	ecall

int times(void *mytimes)
{
	return syscall(SYS_times, mytimes);
}
    1dd2:	2501                	sext.w	a0,a0
    1dd4:	8082                	ret

0000000000001dd6 <get_time>:

int64 get_time()
{
    1dd6:	1141                	addi	sp,sp,-16
    register long a7 __asm__("a7") = n;
    1dd8:	0a900893          	li	a7,169
    register long a0 __asm__("a0") = a;
    1ddc:	850a                	mv	a0,sp
    register long a1 __asm__("a1") = b;
    1dde:	4581                	li	a1,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1de0:	00000073          	ecall
    TimeVal time;
    int err = sys_get_time(&time, 0);
    if (err == 0)
    1de4:	2501                	sext.w	a0,a0
    1de6:	ed09                	bnez	a0,1e00 <get_time+0x2a>
    {
        return ((time.sec & 0xffff) * 1000 + time.usec / 1000);
    1de8:	67a2                	ld	a5,8(sp)
    1dea:	3e800713          	li	a4,1000
    1dee:	00015503          	lhu	a0,0(sp)
    1df2:	02e7d7b3          	divu	a5,a5,a4
    1df6:	02e50533          	mul	a0,a0,a4
    1dfa:	953e                	add	a0,a0,a5
    }
    else
    {
        return -1;
    }
}
    1dfc:	0141                	addi	sp,sp,16
    1dfe:	8082                	ret
        return -1;
    1e00:	557d                	li	a0,-1
    1e02:	bfed                	j	1dfc <get_time+0x26>

0000000000001e04 <sys_get_time>:
    register long a7 __asm__("a7") = n;
    1e04:	0a900893          	li	a7,169
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1e08:	00000073          	ecall

int sys_get_time(TimeVal *ts, int tz)
{
    return syscall(SYS_gettimeofday, ts, tz);
}
    1e0c:	2501                	sext.w	a0,a0
    1e0e:	8082                	ret

0000000000001e10 <time>:
    register long a7 __asm__("a7") = n;
    1e10:	42600893          	li	a7,1062
    __asm_syscall("r"(a7), "0"(a0))
    1e14:	00000073          	ecall

int time(unsigned long *tloc)
{
    return syscall(SYS_time, tloc);
}
    1e18:	2501                	sext.w	a0,a0
    1e1a:	8082                	ret

0000000000001e1c <sleep>:

int sleep(unsigned long long time)
{
    1e1c:	1141                	addi	sp,sp,-16
    TimeVal tv = {.sec = time, .usec = 0};
    1e1e:	e02a                	sd	a0,0(sp)
    register long a0 __asm__("a0") = a;
    1e20:	850a                	mv	a0,sp
    1e22:	e402                	sd	zero,8(sp)
    register long a7 __asm__("a7") = n;
    1e24:	06500893          	li	a7,101
    register long a1 __asm__("a1") = b;
    1e28:	85aa                	mv	a1,a0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1e2a:	00000073          	ecall
    if (syscall(SYS_nanosleep, &tv, &tv)) return tv.sec;
    1e2e:	e501                	bnez	a0,1e36 <sleep+0x1a>
    return 0;
    1e30:	4501                	li	a0,0
}
    1e32:	0141                	addi	sp,sp,16
    1e34:	8082                	ret
    if (syscall(SYS_nanosleep, &tv, &tv)) return tv.sec;
    1e36:	4502                	lw	a0,0(sp)
}
    1e38:	0141                	addi	sp,sp,16
    1e3a:	8082                	ret

0000000000001e3c <set_priority>:
    register long a7 __asm__("a7") = n;
    1e3c:	08c00893          	li	a7,140
    __asm_syscall("r"(a7), "0"(a0))
    1e40:	00000073          	ecall

int set_priority(int prio)
{
    return syscall(SYS_setpriority, prio);
}
    1e44:	2501                	sext.w	a0,a0
    1e46:	8082                	ret

0000000000001e48 <mmap>:
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4))
}

static inline long __syscall6(long n, long a, long b, long c, long d, long e, long f)
{
    register long a7 __asm__("a7") = n;
    1e48:	0de00893          	li	a7,222
    register long a1 __asm__("a1") = b;
    register long a2 __asm__("a2") = c;
    register long a3 __asm__("a3") = d;
    register long a4 __asm__("a4") = e;
    register long a5 __asm__("a5") = f;
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4), "r"(a5))
    1e4c:	00000073          	ecall

void *mmap(void *start, size_t len, int prot, int flags, int fd, off_t off)
{
    return syscall(SYS_mmap, start, len, prot, flags, fd, off);
}
    1e50:	8082                	ret

0000000000001e52 <munmap>:
    register long a7 __asm__("a7") = n;
    1e52:	0d700893          	li	a7,215
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1e56:	00000073          	ecall

int munmap(void *start, size_t len)
{
    return syscall(SYS_munmap, start, len);
}
    1e5a:	2501                	sext.w	a0,a0
    1e5c:	8082                	ret

0000000000001e5e <wait>:

int wait(int *code)
{
    1e5e:	85aa                	mv	a1,a0
    register long a7 __asm__("a7") = n;
    1e60:	10400893          	li	a7,260
    register long a0 __asm__("a0") = a;
    1e64:	557d                	li	a0,-1
    register long a2 __asm__("a2") = c;
    1e66:	4601                	li	a2,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1e68:	00000073          	ecall
    return waitpid(-1, code, 0);
}
    1e6c:	2501                	sext.w	a0,a0
    1e6e:	8082                	ret

0000000000001e70 <spawn>:
    register long a7 __asm__("a7") = n;
    1e70:	19000893          	li	a7,400
    __asm_syscall("r"(a7), "0"(a0))
    1e74:	00000073          	ecall

int spawn(char *file)
{
    return syscall(SYS_spawn, file);
}
    1e78:	2501                	sext.w	a0,a0
    1e7a:	8082                	ret

0000000000001e7c <mailread>:
    register long a7 __asm__("a7") = n;
    1e7c:	19100893          	li	a7,401
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1e80:	00000073          	ecall

int mailread(void *buf, int len)
{
    return syscall(SYS_mailread, buf, len);
}
    1e84:	2501                	sext.w	a0,a0
    1e86:	8082                	ret

0000000000001e88 <mailwrite>:
    register long a7 __asm__("a7") = n;
    1e88:	19200893          	li	a7,402
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1e8c:	00000073          	ecall

int mailwrite(int pid, void *buf, int len)
{
    return syscall(SYS_mailwrite, pid, buf, len);
}
    1e90:	2501                	sext.w	a0,a0
    1e92:	8082                	ret

0000000000001e94 <fstat>:
    register long a7 __asm__("a7") = n;
    1e94:	05000893          	li	a7,80
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1e98:	00000073          	ecall

int fstat(int fd, struct kstat *st)
{
    return syscall(SYS_fstat, fd, st);
}
    1e9c:	2501                	sext.w	a0,a0
    1e9e:	8082                	ret

0000000000001ea0 <sys_linkat>:
    register long a4 __asm__("a4") = e;
    1ea0:	1702                	slli	a4,a4,0x20
    register long a7 __asm__("a7") = n;
    1ea2:	02500893          	li	a7,37
    register long a4 __asm__("a4") = e;
    1ea6:	9301                	srli	a4,a4,0x20
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4))
    1ea8:	00000073          	ecall

int sys_linkat(int olddirfd, char *oldpath, int newdirfd, char *newpath, unsigned int flags)
{
    return syscall(SYS_linkat, olddirfd, oldpath, newdirfd, newpath, flags);
}
    1eac:	2501                	sext.w	a0,a0
    1eae:	8082                	ret

0000000000001eb0 <sys_unlinkat>:
    register long a2 __asm__("a2") = c;
    1eb0:	1602                	slli	a2,a2,0x20
    register long a7 __asm__("a7") = n;
    1eb2:	02300893          	li	a7,35
    register long a2 __asm__("a2") = c;
    1eb6:	9201                	srli	a2,a2,0x20
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1eb8:	00000073          	ecall

int sys_unlinkat(int dirfd, char *path, unsigned int flags)
{
    return syscall(SYS_unlinkat, dirfd, path, flags);
}
    1ebc:	2501                	sext.w	a0,a0
    1ebe:	8082                	ret

0000000000001ec0 <link>:

int link(char *old_path, char *new_path)
{
    1ec0:	87aa                	mv	a5,a0
    1ec2:	86ae                	mv	a3,a1
    register long a7 __asm__("a7") = n;
    1ec4:	02500893          	li	a7,37
    register long a0 __asm__("a0") = a;
    1ec8:	f9c00513          	li	a0,-100
    register long a1 __asm__("a1") = b;
    1ecc:	85be                	mv	a1,a5
    register long a2 __asm__("a2") = c;
    1ece:	f9c00613          	li	a2,-100
    register long a4 __asm__("a4") = e;
    1ed2:	4701                	li	a4,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4))
    1ed4:	00000073          	ecall
    return sys_linkat(AT_FDCWD, old_path, AT_FDCWD, new_path, 0);
}
    1ed8:	2501                	sext.w	a0,a0
    1eda:	8082                	ret

0000000000001edc <unlink>:

int unlink(char *path)
{
    1edc:	85aa                	mv	a1,a0
    register long a7 __asm__("a7") = n;
    1ede:	02300893          	li	a7,35
    register long a0 __asm__("a0") = a;
    1ee2:	f9c00513          	li	a0,-100
    register long a2 __asm__("a2") = c;
    1ee6:	4601                	li	a2,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1ee8:	00000073          	ecall
    return sys_unlinkat(AT_FDCWD, path, 0);
}
    1eec:	2501                	sext.w	a0,a0
    1eee:	8082                	ret

0000000000001ef0 <uname>:
    register long a7 __asm__("a7") = n;
    1ef0:	0a000893          	li	a7,160
    __asm_syscall("r"(a7), "0"(a0))
    1ef4:	00000073          	ecall

int uname(void *buf)
{
    return syscall(SYS_uname, buf);
}
    1ef8:	2501                	sext.w	a0,a0
    1efa:	8082                	ret

0000000000001efc <brk>:
    register long a7 __asm__("a7") = n;
    1efc:	0d600893          	li	a7,214
    __asm_syscall("r"(a7), "0"(a0))
    1f00:	00000073          	ecall

int brk(void *addr)
{
    return syscall(SYS_brk, addr);
}
    1f04:	2501                	sext.w	a0,a0
    1f06:	8082                	ret

0000000000001f08 <getcwd>:
    register long a7 __asm__("a7") = n;
    1f08:	48c5                	li	a7,17
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1f0a:	00000073          	ecall

char *getcwd(char *buf, size_t size){
    return syscall(SYS_getcwd, buf, size);
}
    1f0e:	8082                	ret

0000000000001f10 <chdir>:
    register long a7 __asm__("a7") = n;
    1f10:	03100893          	li	a7,49
    __asm_syscall("r"(a7), "0"(a0))
    1f14:	00000073          	ecall

int chdir(const char *path){
    return syscall(SYS_chdir, path);
}
    1f18:	2501                	sext.w	a0,a0
    1f1a:	8082                	ret

0000000000001f1c <mkdir>:

int mkdir(const char *path, mode_t mode){
    1f1c:	862e                	mv	a2,a1
    1f1e:	87aa                	mv	a5,a0
    register long a2 __asm__("a2") = c;
    1f20:	1602                	slli	a2,a2,0x20
    register long a7 __asm__("a7") = n;
    1f22:	02200893          	li	a7,34
    register long a0 __asm__("a0") = a;
    1f26:	f9c00513          	li	a0,-100
    register long a1 __asm__("a1") = b;
    1f2a:	85be                	mv	a1,a5
    register long a2 __asm__("a2") = c;
    1f2c:	9201                	srli	a2,a2,0x20
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1f2e:	00000073          	ecall
    return syscall(SYS_mkdirat, AT_FDCWD, path, mode);
}
    1f32:	2501                	sext.w	a0,a0
    1f34:	8082                	ret

0000000000001f36 <getdents>:
    register long a7 __asm__("a7") = n;
    1f36:	03d00893          	li	a7,61
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1f3a:	00000073          	ecall

int getdents(int fd, struct linux_dirent64 *dirp64, unsigned long len){
    //return syscall(SYS_getdents64, fd, dirp64, len);
    return syscall(SYS_getdents64, fd, dirp64, len);
}
    1f3e:	2501                	sext.w	a0,a0
    1f40:	8082                	ret

0000000000001f42 <pipe>:
    register long a7 __asm__("a7") = n;
    1f42:	03b00893          	li	a7,59
    register long a1 __asm__("a1") = b;
    1f46:	4581                	li	a1,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1f48:	00000073          	ecall

int pipe(int fd[2]){
    return syscall(SYS_pipe2, fd, 0);
}
    1f4c:	2501                	sext.w	a0,a0
    1f4e:	8082                	ret

0000000000001f50 <dup>:
    register long a7 __asm__("a7") = n;
    1f50:	48dd                	li	a7,23
    __asm_syscall("r"(a7), "0"(a0))
    1f52:	00000073          	ecall

int dup(int fd){
    return syscall(SYS_dup, fd);
}
    1f56:	2501                	sext.w	a0,a0
    1f58:	8082                	ret

0000000000001f5a <dup2>:
    register long a7 __asm__("a7") = n;
    1f5a:	48e1                	li	a7,24
    register long a2 __asm__("a2") = c;
    1f5c:	4601                	li	a2,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1f5e:	00000073          	ecall

int dup2(int old, int new){
    return syscall(SYS_dup3, old, new, 0);
}
    1f62:	2501                	sext.w	a0,a0
    1f64:	8082                	ret

0000000000001f66 <mount>:
    register long a7 __asm__("a7") = n;
    1f66:	02800893          	li	a7,40
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4))
    1f6a:	00000073          	ecall

int mount(const char *special, const char *dir, const char *fstype, unsigned long flags, const void *data)
{
        return syscall(SYS_mount, special, dir, fstype, flags, data);
}
    1f6e:	2501                	sext.w	a0,a0
    1f70:	8082                	ret

0000000000001f72 <umount>:
    register long a7 __asm__("a7") = n;
    1f72:	02700893          	li	a7,39
    register long a1 __asm__("a1") = b;
    1f76:	4581                	li	a1,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1f78:	00000073          	ecall

int umount(const char *special)
{
        return syscall(SYS_umount2, special, 0);
}
    1f7c:	2501                	sext.w	a0,a0
    1f7e:	8082                	ret

0000000000001f80 <__clone>:

.global __clone
.type  __clone, %function
__clone:
	# Save func and arg to stack
	addi a1, a1, -16
    1f80:	15c1                	addi	a1,a1,-16
	sd a0, 0(a1)
    1f82:	e188                	sd	a0,0(a1)
	sd a3, 8(a1)
    1f84:	e594                	sd	a3,8(a1)

	# Call SYS_clone
	mv a0, a2
    1f86:	8532                	mv	a0,a2
	mv a2, a4
    1f88:	863a                	mv	a2,a4
	mv a3, a5
    1f8a:	86be                	mv	a3,a5
	mv a4, a6
    1f8c:	8742                	mv	a4,a6
	li a7, 220 # SYS_clone
    1f8e:	0dc00893          	li	a7,220
	ecall
    1f92:	00000073          	ecall

	beqz a0, 1f
    1f96:	c111                	beqz	a0,1f9a <__clone+0x1a>
	# Parent
	ret
    1f98:	8082                	ret

	# Child
1:      ld a1, 0(sp)
    1f9a:	6582                	ld	a1,0(sp)
	ld a0, 8(sp)
    1f9c:	6522                	ld	a0,8(sp)
	jalr a1
    1f9e:	9582                	jalr	a1

	# Exit
	li a7, 93 # SYS_exit
    1fa0:	05d00893          	li	a7,93
	ecall
    1fa4:	00000073          	ecall
