
/home/ubuntu/rust/ultra-os/codes/riscv-syscalls-testing/user/build/riscv64/brk:     file format elf64-littleriscv


Disassembly of section .text:

0000000000001000 <_start>:
.section .text.entry
.globl _start
_start:
    mv a0, sp
    1000:	850a                	mv	a0,sp
    tail __start_main
    1002:	a855                	j	10b6 <__start_main>

0000000000001004 <test_brk>:
 * "After alloc,heap pos: [num+64]"
 * "Alloc again,heap pos: [num+128]"
 * 
 * Linux 中brk(0)只返回0，此处与Linux表现不同，应特殊说明。
 */
void test_brk(){
    1004:	1101                	addi	sp,sp,-32
    TEST_START(__func__);
    1006:	00001517          	auipc	a0,0x1
    100a:	eda50513          	addi	a0,a0,-294 # 1ee0 <__clone+0x2e>
void test_brk(){
    100e:	ec06                	sd	ra,24(sp)
    1010:	e822                	sd	s0,16(sp)
    1012:	e426                	sd	s1,8(sp)
    TEST_START(__func__);
    1014:	314000ef          	jal	ra,1328 <puts>
    1018:	00001497          	auipc	s1,0x1
    101c:	f8048493          	addi	s1,s1,-128 # 1f98 <__func__.0>
    1020:	8526                	mv	a0,s1
    1022:	306000ef          	jal	ra,1328 <puts>
    1026:	00001517          	auipc	a0,0x1
    102a:	ed250513          	addi	a0,a0,-302 # 1ef8 <__clone+0x46>
    102e:	2fa000ef          	jal	ra,1328 <puts>
    intptr_t cur_pos, alloc_pos, alloc_pos_1;

    cur_pos = brk(0);
    1032:	4501                	li	a0,0
    1034:	5fb000ef          	jal	ra,1e2e <brk>
    printf("Before alloc,heap pos: %d\n", cur_pos);
    1038:	85aa                	mv	a1,a0
    cur_pos = brk(0);
    103a:	842a                	mv	s0,a0
    printf("Before alloc,heap pos: %d\n", cur_pos);
    103c:	00001517          	auipc	a0,0x1
    1040:	ecc50513          	addi	a0,a0,-308 # 1f08 <__clone+0x56>
    1044:	306000ef          	jal	ra,134a <printf>
    brk(cur_pos + 64);
    1048:	04040513          	addi	a0,s0,64
    104c:	5e3000ef          	jal	ra,1e2e <brk>
    alloc_pos = brk(0);
    1050:	4501                	li	a0,0
    1052:	5dd000ef          	jal	ra,1e2e <brk>
    printf("After alloc,heap pos: %d\n",alloc_pos);
    1056:	85aa                	mv	a1,a0
    alloc_pos = brk(0);
    1058:	842a                	mv	s0,a0
    printf("After alloc,heap pos: %d\n",alloc_pos);
    105a:	00001517          	auipc	a0,0x1
    105e:	ece50513          	addi	a0,a0,-306 # 1f28 <__clone+0x76>
    1062:	2e8000ef          	jal	ra,134a <printf>
    brk(alloc_pos + 64);
    1066:	04040513          	addi	a0,s0,64
    106a:	5c5000ef          	jal	ra,1e2e <brk>
    alloc_pos_1 = brk(0);
    106e:	4501                	li	a0,0
    1070:	5bf000ef          	jal	ra,1e2e <brk>
    1074:	85aa                	mv	a1,a0
    printf("Alloc again,heap pos: %d\n",alloc_pos_1);
    1076:	00001517          	auipc	a0,0x1
    107a:	ed250513          	addi	a0,a0,-302 # 1f48 <__clone+0x96>
    107e:	2cc000ef          	jal	ra,134a <printf>
    TEST_END(__func__);
    1082:	00001517          	auipc	a0,0x1
    1086:	ee650513          	addi	a0,a0,-282 # 1f68 <__clone+0xb6>
    108a:	29e000ef          	jal	ra,1328 <puts>
    108e:	8526                	mv	a0,s1
    1090:	298000ef          	jal	ra,1328 <puts>
}
    1094:	6442                	ld	s0,16(sp)
    1096:	60e2                	ld	ra,24(sp)
    1098:	64a2                	ld	s1,8(sp)
    TEST_END(__func__);
    109a:	00001517          	auipc	a0,0x1
    109e:	e5e50513          	addi	a0,a0,-418 # 1ef8 <__clone+0x46>
}
    10a2:	6105                	addi	sp,sp,32
    TEST_END(__func__);
    10a4:	a451                	j	1328 <puts>

00000000000010a6 <main>:

int main(void) {
    10a6:	1141                	addi	sp,sp,-16
    10a8:	e406                	sd	ra,8(sp)
    test_brk();
    10aa:	f5bff0ef          	jal	ra,1004 <test_brk>
    return 0;
}
    10ae:	60a2                	ld	ra,8(sp)
    10b0:	4501                	li	a0,0
    10b2:	0141                	addi	sp,sp,16
    10b4:	8082                	ret

00000000000010b6 <__start_main>:
#include <unistd.h>

extern int main();

int __start_main(long *p)
{
    10b6:	85aa                	mv	a1,a0
	int argc = p[0];
	char **argv = (void *)(p+1);

	exit(main(argc, argv));
    10b8:	4108                	lw	a0,0(a0)
{
    10ba:	1141                	addi	sp,sp,-16
	exit(main(argc, argv));
    10bc:	05a1                	addi	a1,a1,8
{
    10be:	e406                	sd	ra,8(sp)
	exit(main(argc, argv));
    10c0:	fe7ff0ef          	jal	ra,10a6 <main>
    10c4:	40b000ef          	jal	ra,1cce <exit>
	return 0;
}
    10c8:	60a2                	ld	ra,8(sp)
    10ca:	4501                	li	a0,0
    10cc:	0141                	addi	sp,sp,16
    10ce:	8082                	ret

00000000000010d0 <printint.constprop.0>:
    write(f, s, l);
}

static char digits[] = "0123456789abcdef";

static void printint(int xx, int base, int sign)
    10d0:	7179                	addi	sp,sp,-48
    10d2:	f406                	sd	ra,40(sp)
{
    char buf[16 + 1];
    int i;
    uint x;

    if (sign && (sign = xx < 0))
    10d4:	12054b63          	bltz	a0,120a <printint.constprop.0+0x13a>

    buf[16] = 0;
    i = 15;
    do
    {
        buf[i--] = digits[x % base];
    10d8:	02b577bb          	remuw	a5,a0,a1
    10dc:	00001617          	auipc	a2,0x1
    10e0:	ecc60613          	addi	a2,a2,-308 # 1fa8 <digits>
    buf[16] = 0;
    10e4:	00010c23          	sb	zero,24(sp)
        buf[i--] = digits[x % base];
    10e8:	0005871b          	sext.w	a4,a1
    10ec:	1782                	slli	a5,a5,0x20
    10ee:	9381                	srli	a5,a5,0x20
    10f0:	97b2                	add	a5,a5,a2
    10f2:	0007c783          	lbu	a5,0(a5)
    } while ((x /= base) != 0);
    10f6:	02b5583b          	divuw	a6,a0,a1
        buf[i--] = digits[x % base];
    10fa:	00f10ba3          	sb	a5,23(sp)
    } while ((x /= base) != 0);
    10fe:	1cb56363          	bltu	a0,a1,12c4 <printint.constprop.0+0x1f4>
        buf[i--] = digits[x % base];
    1102:	45b9                	li	a1,14
    1104:	02e877bb          	remuw	a5,a6,a4
    1108:	1782                	slli	a5,a5,0x20
    110a:	9381                	srli	a5,a5,0x20
    110c:	97b2                	add	a5,a5,a2
    110e:	0007c783          	lbu	a5,0(a5)
    } while ((x /= base) != 0);
    1112:	02e856bb          	divuw	a3,a6,a4
        buf[i--] = digits[x % base];
    1116:	00f10b23          	sb	a5,22(sp)
    } while ((x /= base) != 0);
    111a:	0ce86e63          	bltu	a6,a4,11f6 <printint.constprop.0+0x126>
        buf[i--] = digits[x % base];
    111e:	02e6f5bb          	remuw	a1,a3,a4
    } while ((x /= base) != 0);
    1122:	02e6d7bb          	divuw	a5,a3,a4
        buf[i--] = digits[x % base];
    1126:	1582                	slli	a1,a1,0x20
    1128:	9181                	srli	a1,a1,0x20
    112a:	95b2                	add	a1,a1,a2
    112c:	0005c583          	lbu	a1,0(a1)
    1130:	00b10aa3          	sb	a1,21(sp)
    } while ((x /= base) != 0);
    1134:	0007859b          	sext.w	a1,a5
    1138:	12e6ec63          	bltu	a3,a4,1270 <printint.constprop.0+0x1a0>
        buf[i--] = digits[x % base];
    113c:	02e7f6bb          	remuw	a3,a5,a4
    1140:	1682                	slli	a3,a3,0x20
    1142:	9281                	srli	a3,a3,0x20
    1144:	96b2                	add	a3,a3,a2
    1146:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    114a:	02e7d83b          	divuw	a6,a5,a4
        buf[i--] = digits[x % base];
    114e:	00d10a23          	sb	a3,20(sp)
    } while ((x /= base) != 0);
    1152:	12e5e863          	bltu	a1,a4,1282 <printint.constprop.0+0x1b2>
        buf[i--] = digits[x % base];
    1156:	02e876bb          	remuw	a3,a6,a4
    115a:	1682                	slli	a3,a3,0x20
    115c:	9281                	srli	a3,a3,0x20
    115e:	96b2                	add	a3,a3,a2
    1160:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    1164:	02e855bb          	divuw	a1,a6,a4
        buf[i--] = digits[x % base];
    1168:	00d109a3          	sb	a3,19(sp)
    } while ((x /= base) != 0);
    116c:	12e86463          	bltu	a6,a4,1294 <printint.constprop.0+0x1c4>
        buf[i--] = digits[x % base];
    1170:	02e5f6bb          	remuw	a3,a1,a4
    1174:	1682                	slli	a3,a3,0x20
    1176:	9281                	srli	a3,a3,0x20
    1178:	96b2                	add	a3,a3,a2
    117a:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    117e:	02e5d83b          	divuw	a6,a1,a4
        buf[i--] = digits[x % base];
    1182:	00d10923          	sb	a3,18(sp)
    } while ((x /= base) != 0);
    1186:	0ce5ec63          	bltu	a1,a4,125e <printint.constprop.0+0x18e>
        buf[i--] = digits[x % base];
    118a:	02e876bb          	remuw	a3,a6,a4
    118e:	1682                	slli	a3,a3,0x20
    1190:	9281                	srli	a3,a3,0x20
    1192:	96b2                	add	a3,a3,a2
    1194:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    1198:	02e855bb          	divuw	a1,a6,a4
        buf[i--] = digits[x % base];
    119c:	00d108a3          	sb	a3,17(sp)
    } while ((x /= base) != 0);
    11a0:	10e86963          	bltu	a6,a4,12b2 <printint.constprop.0+0x1e2>
        buf[i--] = digits[x % base];
    11a4:	02e5f6bb          	remuw	a3,a1,a4
    11a8:	1682                	slli	a3,a3,0x20
    11aa:	9281                	srli	a3,a3,0x20
    11ac:	96b2                	add	a3,a3,a2
    11ae:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    11b2:	02e5d83b          	divuw	a6,a1,a4
        buf[i--] = digits[x % base];
    11b6:	00d10823          	sb	a3,16(sp)
    } while ((x /= base) != 0);
    11ba:	10e5e763          	bltu	a1,a4,12c8 <printint.constprop.0+0x1f8>
        buf[i--] = digits[x % base];
    11be:	02e876bb          	remuw	a3,a6,a4
    11c2:	1682                	slli	a3,a3,0x20
    11c4:	9281                	srli	a3,a3,0x20
    11c6:	96b2                	add	a3,a3,a2
    11c8:	0006c683          	lbu	a3,0(a3)
    } while ((x /= base) != 0);
    11cc:	02e857bb          	divuw	a5,a6,a4
        buf[i--] = digits[x % base];
    11d0:	00d107a3          	sb	a3,15(sp)
    } while ((x /= base) != 0);
    11d4:	10e86363          	bltu	a6,a4,12da <printint.constprop.0+0x20a>
        buf[i--] = digits[x % base];
    11d8:	1782                	slli	a5,a5,0x20
    11da:	9381                	srli	a5,a5,0x20
    11dc:	97b2                	add	a5,a5,a2
    11de:	0007c783          	lbu	a5,0(a5)
    11e2:	4599                	li	a1,6
    11e4:	00f10723          	sb	a5,14(sp)

    if (sign)
    11e8:	00055763          	bgez	a0,11f6 <printint.constprop.0+0x126>
        buf[i--] = '-';
    11ec:	02d00793          	li	a5,45
    11f0:	00f106a3          	sb	a5,13(sp)
        buf[i--] = digits[x % base];
    11f4:	4595                	li	a1,5
    write(f, s, l);
    11f6:	003c                	addi	a5,sp,8
    11f8:	4641                	li	a2,16
    11fa:	9e0d                	subw	a2,a2,a1
    11fc:	4505                	li	a0,1
    11fe:	95be                	add	a1,a1,a5
    1200:	27f000ef          	jal	ra,1c7e <write>
    i++;
    if (i < 0)
        puts("printint error");
    out(stdout, buf + i, 16 - i);
}
    1204:	70a2                	ld	ra,40(sp)
    1206:	6145                	addi	sp,sp,48
    1208:	8082                	ret
        x = -xx;
    120a:	40a0083b          	negw	a6,a0
        buf[i--] = digits[x % base];
    120e:	02b877bb          	remuw	a5,a6,a1
    1212:	00001617          	auipc	a2,0x1
    1216:	d9660613          	addi	a2,a2,-618 # 1fa8 <digits>
    buf[16] = 0;
    121a:	00010c23          	sb	zero,24(sp)
        buf[i--] = digits[x % base];
    121e:	0005871b          	sext.w	a4,a1
    1222:	1782                	slli	a5,a5,0x20
    1224:	9381                	srli	a5,a5,0x20
    1226:	97b2                	add	a5,a5,a2
    1228:	0007c783          	lbu	a5,0(a5)
    } while ((x /= base) != 0);
    122c:	02b858bb          	divuw	a7,a6,a1
        buf[i--] = digits[x % base];
    1230:	00f10ba3          	sb	a5,23(sp)
    } while ((x /= base) != 0);
    1234:	06b86963          	bltu	a6,a1,12a6 <printint.constprop.0+0x1d6>
        buf[i--] = digits[x % base];
    1238:	02e8f7bb          	remuw	a5,a7,a4
    123c:	1782                	slli	a5,a5,0x20
    123e:	9381                	srli	a5,a5,0x20
    1240:	97b2                	add	a5,a5,a2
    1242:	0007c783          	lbu	a5,0(a5)
    } while ((x /= base) != 0);
    1246:	02e8d6bb          	divuw	a3,a7,a4
        buf[i--] = digits[x % base];
    124a:	00f10b23          	sb	a5,22(sp)
    } while ((x /= base) != 0);
    124e:	ece8f8e3          	bgeu	a7,a4,111e <printint.constprop.0+0x4e>
        buf[i--] = '-';
    1252:	02d00793          	li	a5,45
    1256:	00f10aa3          	sb	a5,21(sp)
        buf[i--] = digits[x % base];
    125a:	45b5                	li	a1,13
    125c:	bf69                	j	11f6 <printint.constprop.0+0x126>
    125e:	45a9                	li	a1,10
    if (sign)
    1260:	f8055be3          	bgez	a0,11f6 <printint.constprop.0+0x126>
        buf[i--] = '-';
    1264:	02d00793          	li	a5,45
    1268:	00f108a3          	sb	a5,17(sp)
        buf[i--] = digits[x % base];
    126c:	45a5                	li	a1,9
    126e:	b761                	j	11f6 <printint.constprop.0+0x126>
    1270:	45b5                	li	a1,13
    if (sign)
    1272:	f80552e3          	bgez	a0,11f6 <printint.constprop.0+0x126>
        buf[i--] = '-';
    1276:	02d00793          	li	a5,45
    127a:	00f10a23          	sb	a5,20(sp)
        buf[i--] = digits[x % base];
    127e:	45b1                	li	a1,12
    1280:	bf9d                	j	11f6 <printint.constprop.0+0x126>
    1282:	45b1                	li	a1,12
    if (sign)
    1284:	f60559e3          	bgez	a0,11f6 <printint.constprop.0+0x126>
        buf[i--] = '-';
    1288:	02d00793          	li	a5,45
    128c:	00f109a3          	sb	a5,19(sp)
        buf[i--] = digits[x % base];
    1290:	45ad                	li	a1,11
    1292:	b795                	j	11f6 <printint.constprop.0+0x126>
    1294:	45ad                	li	a1,11
    if (sign)
    1296:	f60550e3          	bgez	a0,11f6 <printint.constprop.0+0x126>
        buf[i--] = '-';
    129a:	02d00793          	li	a5,45
    129e:	00f10923          	sb	a5,18(sp)
        buf[i--] = digits[x % base];
    12a2:	45a9                	li	a1,10
    12a4:	bf89                	j	11f6 <printint.constprop.0+0x126>
        buf[i--] = '-';
    12a6:	02d00793          	li	a5,45
    12aa:	00f10b23          	sb	a5,22(sp)
        buf[i--] = digits[x % base];
    12ae:	45b9                	li	a1,14
    12b0:	b799                	j	11f6 <printint.constprop.0+0x126>
    12b2:	45a5                	li	a1,9
    if (sign)
    12b4:	f40551e3          	bgez	a0,11f6 <printint.constprop.0+0x126>
        buf[i--] = '-';
    12b8:	02d00793          	li	a5,45
    12bc:	00f10823          	sb	a5,16(sp)
        buf[i--] = digits[x % base];
    12c0:	45a1                	li	a1,8
    12c2:	bf15                	j	11f6 <printint.constprop.0+0x126>
    i = 15;
    12c4:	45bd                	li	a1,15
    12c6:	bf05                	j	11f6 <printint.constprop.0+0x126>
        buf[i--] = digits[x % base];
    12c8:	45a1                	li	a1,8
    if (sign)
    12ca:	f20556e3          	bgez	a0,11f6 <printint.constprop.0+0x126>
        buf[i--] = '-';
    12ce:	02d00793          	li	a5,45
    12d2:	00f107a3          	sb	a5,15(sp)
        buf[i--] = digits[x % base];
    12d6:	459d                	li	a1,7
    12d8:	bf39                	j	11f6 <printint.constprop.0+0x126>
    12da:	459d                	li	a1,7
    if (sign)
    12dc:	f0055de3          	bgez	a0,11f6 <printint.constprop.0+0x126>
        buf[i--] = '-';
    12e0:	02d00793          	li	a5,45
    12e4:	00f10723          	sb	a5,14(sp)
        buf[i--] = digits[x % base];
    12e8:	4599                	li	a1,6
    12ea:	b731                	j	11f6 <printint.constprop.0+0x126>

00000000000012ec <getchar>:
{
    12ec:	1101                	addi	sp,sp,-32
    read(stdin, &byte, 1);
    12ee:	00f10593          	addi	a1,sp,15
    12f2:	4605                	li	a2,1
    12f4:	4501                	li	a0,0
{
    12f6:	ec06                	sd	ra,24(sp)
    char byte = 0;
    12f8:	000107a3          	sb	zero,15(sp)
    read(stdin, &byte, 1);
    12fc:	179000ef          	jal	ra,1c74 <read>
}
    1300:	60e2                	ld	ra,24(sp)
    1302:	00f14503          	lbu	a0,15(sp)
    1306:	6105                	addi	sp,sp,32
    1308:	8082                	ret

000000000000130a <putchar>:
{
    130a:	1101                	addi	sp,sp,-32
    130c:	87aa                	mv	a5,a0
    return write(stdout, &byte, 1);
    130e:	00f10593          	addi	a1,sp,15
    1312:	4605                	li	a2,1
    1314:	4505                	li	a0,1
{
    1316:	ec06                	sd	ra,24(sp)
    char byte = c;
    1318:	00f107a3          	sb	a5,15(sp)
    return write(stdout, &byte, 1);
    131c:	163000ef          	jal	ra,1c7e <write>
}
    1320:	60e2                	ld	ra,24(sp)
    1322:	2501                	sext.w	a0,a0
    1324:	6105                	addi	sp,sp,32
    1326:	8082                	ret

0000000000001328 <puts>:
{
    1328:	1141                	addi	sp,sp,-16
    132a:	e406                	sd	ra,8(sp)
    132c:	e022                	sd	s0,0(sp)
    132e:	842a                	mv	s0,a0
    r = -(write(stdout, s, strlen(s)) < 0);
    1330:	56a000ef          	jal	ra,189a <strlen>
    1334:	862a                	mv	a2,a0
    1336:	85a2                	mv	a1,s0
    1338:	4505                	li	a0,1
    133a:	145000ef          	jal	ra,1c7e <write>
}
    133e:	60a2                	ld	ra,8(sp)
    1340:	6402                	ld	s0,0(sp)
    r = -(write(stdout, s, strlen(s)) < 0);
    1342:	957d                	srai	a0,a0,0x3f
    return r;
    1344:	2501                	sext.w	a0,a0
}
    1346:	0141                	addi	sp,sp,16
    1348:	8082                	ret

000000000000134a <printf>:
    out(stdout, buf, i);
}

// Print to the console. only understands %d, %x, %p, %s.
void printf(const char *fmt, ...)
{
    134a:	7171                	addi	sp,sp,-176
    134c:	fc56                	sd	s5,56(sp)
    134e:	ed3e                	sd	a5,152(sp)
    buf[i++] = '0';
    1350:	7ae1                	lui	s5,0xffff8
    va_list ap;
    int cnt = 0, l = 0;
    char *a, *z, *s = (char *)fmt, str;
    int f = stdout;

    va_start(ap, fmt);
    1352:	18bc                	addi	a5,sp,120
{
    1354:	e8ca                	sd	s2,80(sp)
    1356:	e4ce                	sd	s3,72(sp)
    1358:	e0d2                	sd	s4,64(sp)
    135a:	f85a                	sd	s6,48(sp)
    135c:	f486                	sd	ra,104(sp)
    135e:	f0a2                	sd	s0,96(sp)
    1360:	eca6                	sd	s1,88(sp)
    1362:	fcae                	sd	a1,120(sp)
    1364:	e132                	sd	a2,128(sp)
    1366:	e536                	sd	a3,136(sp)
    1368:	e93a                	sd	a4,144(sp)
    136a:	f142                	sd	a6,160(sp)
    136c:	f546                	sd	a7,168(sp)
    va_start(ap, fmt);
    136e:	e03e                	sd	a5,0(sp)
    for (;;)
    {
        if (!*s)
            break;
        for (a = s; *s && *s != '%'; s++)
    1370:	02500913          	li	s2,37
        out(f, a, l);
        if (l)
            continue;
        if (s[1] == 0)
            break;
        switch (s[1])
    1374:	07300a13          	li	s4,115
        case 'p':
            printptr(va_arg(ap, uint64));
            break;
        case 's':
            if ((a = va_arg(ap, char *)) == 0)
                a = "(null)";
    1378:	00001b17          	auipc	s6,0x1
    137c:	c00b0b13          	addi	s6,s6,-1024 # 1f78 <__clone+0xc6>
    buf[i++] = '0';
    1380:	830aca93          	xori	s5,s5,-2000
        buf[i++] = digits[x >> (sizeof(uint64) * 8 - 4)];
    1384:	00001997          	auipc	s3,0x1
    1388:	c2498993          	addi	s3,s3,-988 # 1fa8 <digits>
        if (!*s)
    138c:	00054783          	lbu	a5,0(a0)
    1390:	16078a63          	beqz	a5,1504 <printf+0x1ba>
    1394:	862a                	mv	a2,a0
        for (a = s; *s && *s != '%'; s++)
    1396:	19278163          	beq	a5,s2,1518 <printf+0x1ce>
    139a:	00164783          	lbu	a5,1(a2)
    139e:	0605                	addi	a2,a2,1
    13a0:	fbfd                	bnez	a5,1396 <printf+0x4c>
    13a2:	84b2                	mv	s1,a2
        l = z - a;
    13a4:	40a6043b          	subw	s0,a2,a0
    write(f, s, l);
    13a8:	85aa                	mv	a1,a0
    13aa:	8622                	mv	a2,s0
    13ac:	4505                	li	a0,1
    13ae:	0d1000ef          	jal	ra,1c7e <write>
        if (l)
    13b2:	18041c63          	bnez	s0,154a <printf+0x200>
        if (s[1] == 0)
    13b6:	0014c783          	lbu	a5,1(s1)
    13ba:	14078563          	beqz	a5,1504 <printf+0x1ba>
        switch (s[1])
    13be:	1d478063          	beq	a5,s4,157e <printf+0x234>
    13c2:	18fa6663          	bltu	s4,a5,154e <printf+0x204>
    13c6:	06400713          	li	a4,100
    13ca:	1ae78063          	beq	a5,a4,156a <printf+0x220>
    13ce:	07000713          	li	a4,112
    13d2:	1ce79963          	bne	a5,a4,15a4 <printf+0x25a>
            printptr(va_arg(ap, uint64));
    13d6:	6702                	ld	a4,0(sp)
    buf[i++] = '0';
    13d8:	01511423          	sh	s5,8(sp)
    write(f, s, l);
    13dc:	4649                	li	a2,18
            printptr(va_arg(ap, uint64));
    13de:	631c                	ld	a5,0(a4)
    13e0:	0721                	addi	a4,a4,8
    13e2:	e03a                	sd	a4,0(sp)
    for (j = 0; j < (sizeof(uint64) * 2); j++, x <<= 4)
    13e4:	00479293          	slli	t0,a5,0x4
    13e8:	00879f93          	slli	t6,a5,0x8
    13ec:	00c79f13          	slli	t5,a5,0xc
    13f0:	01079e93          	slli	t4,a5,0x10
    13f4:	01479e13          	slli	t3,a5,0x14
    13f8:	01879313          	slli	t1,a5,0x18
    13fc:	01c79893          	slli	a7,a5,0x1c
    1400:	02479813          	slli	a6,a5,0x24
    1404:	02879513          	slli	a0,a5,0x28
    1408:	02c79593          	slli	a1,a5,0x2c
    140c:	03079693          	slli	a3,a5,0x30
    1410:	03479713          	slli	a4,a5,0x34
        buf[i++] = digits[x >> (sizeof(uint64) * 8 - 4)];
    1414:	03c7d413          	srli	s0,a5,0x3c
    1418:	01c7d39b          	srliw	t2,a5,0x1c
    141c:	03c2d293          	srli	t0,t0,0x3c
    1420:	03cfdf93          	srli	t6,t6,0x3c
    1424:	03cf5f13          	srli	t5,t5,0x3c
    1428:	03cede93          	srli	t4,t4,0x3c
    142c:	03ce5e13          	srli	t3,t3,0x3c
    1430:	03c35313          	srli	t1,t1,0x3c
    1434:	03c8d893          	srli	a7,a7,0x3c
    1438:	03c85813          	srli	a6,a6,0x3c
    143c:	9171                	srli	a0,a0,0x3c
    143e:	91f1                	srli	a1,a1,0x3c
    1440:	92f1                	srli	a3,a3,0x3c
    1442:	9371                	srli	a4,a4,0x3c
    1444:	96ce                	add	a3,a3,s3
    1446:	974e                	add	a4,a4,s3
    1448:	944e                	add	s0,s0,s3
    144a:	92ce                	add	t0,t0,s3
    144c:	9fce                	add	t6,t6,s3
    144e:	9f4e                	add	t5,t5,s3
    1450:	9ece                	add	t4,t4,s3
    1452:	9e4e                	add	t3,t3,s3
    1454:	934e                	add	t1,t1,s3
    1456:	98ce                	add	a7,a7,s3
    1458:	93ce                	add	t2,t2,s3
    145a:	984e                	add	a6,a6,s3
    145c:	954e                	add	a0,a0,s3
    145e:	95ce                	add	a1,a1,s3
    1460:	0006c083          	lbu	ra,0(a3)
    1464:	0002c283          	lbu	t0,0(t0)
    1468:	00074683          	lbu	a3,0(a4)
    146c:	000fcf83          	lbu	t6,0(t6)
    1470:	000f4f03          	lbu	t5,0(t5)
    1474:	000ece83          	lbu	t4,0(t4)
    1478:	000e4e03          	lbu	t3,0(t3)
    147c:	00034303          	lbu	t1,0(t1)
    1480:	0008c883          	lbu	a7,0(a7)
    1484:	0003c383          	lbu	t2,0(t2)
    1488:	00084803          	lbu	a6,0(a6)
    148c:	00054503          	lbu	a0,0(a0)
    1490:	0005c583          	lbu	a1,0(a1)
    1494:	00044403          	lbu	s0,0(s0)
    for (j = 0; j < (sizeof(uint64) * 2); j++, x <<= 4)
    1498:	03879713          	slli	a4,a5,0x38
        buf[i++] = digits[x >> (sizeof(uint64) * 8 - 4)];
    149c:	9371                	srli	a4,a4,0x3c
    149e:	8bbd                	andi	a5,a5,15
    14a0:	974e                	add	a4,a4,s3
    14a2:	97ce                	add	a5,a5,s3
    14a4:	005105a3          	sb	t0,11(sp)
    14a8:	01f10623          	sb	t6,12(sp)
    14ac:	01e106a3          	sb	t5,13(sp)
    14b0:	01d10723          	sb	t4,14(sp)
    14b4:	01c107a3          	sb	t3,15(sp)
    14b8:	00610823          	sb	t1,16(sp)
    14bc:	011108a3          	sb	a7,17(sp)
    14c0:	00710923          	sb	t2,18(sp)
    14c4:	010109a3          	sb	a6,19(sp)
    14c8:	00a10a23          	sb	a0,20(sp)
    14cc:	00b10aa3          	sb	a1,21(sp)
    14d0:	00110b23          	sb	ra,22(sp)
    14d4:	00d10ba3          	sb	a3,23(sp)
    14d8:	00810523          	sb	s0,10(sp)
    14dc:	00074703          	lbu	a4,0(a4)
    14e0:	0007c783          	lbu	a5,0(a5)
    write(f, s, l);
    14e4:	002c                	addi	a1,sp,8
    14e6:	4505                	li	a0,1
        buf[i++] = digits[x >> (sizeof(uint64) * 8 - 4)];
    14e8:	00e10c23          	sb	a4,24(sp)
    14ec:	00f10ca3          	sb	a5,25(sp)
    buf[i] = 0;
    14f0:	00010d23          	sb	zero,26(sp)
    write(f, s, l);
    14f4:	78a000ef          	jal	ra,1c7e <write>
            // Print unknown % sequence to draw attention.
            putchar('%');
            putchar(s[1]);
            break;
        }
        s += 2;
    14f8:	00248513          	addi	a0,s1,2
        if (!*s)
    14fc:	00054783          	lbu	a5,0(a0)
    1500:	e8079ae3          	bnez	a5,1394 <printf+0x4a>
    }
    va_end(ap);
}
    1504:	70a6                	ld	ra,104(sp)
    1506:	7406                	ld	s0,96(sp)
    1508:	64e6                	ld	s1,88(sp)
    150a:	6946                	ld	s2,80(sp)
    150c:	69a6                	ld	s3,72(sp)
    150e:	6a06                	ld	s4,64(sp)
    1510:	7ae2                	ld	s5,56(sp)
    1512:	7b42                	ld	s6,48(sp)
    1514:	614d                	addi	sp,sp,176
    1516:	8082                	ret
        for (z = s; s[0] == '%' && s[1] == '%'; z++, s += 2)
    1518:	00064783          	lbu	a5,0(a2)
    151c:	84b2                	mv	s1,a2
    151e:	01278963          	beq	a5,s2,1530 <printf+0x1e6>
    1522:	b549                	j	13a4 <printf+0x5a>
    1524:	0024c783          	lbu	a5,2(s1)
    1528:	0605                	addi	a2,a2,1
    152a:	0489                	addi	s1,s1,2
    152c:	e7279ce3          	bne	a5,s2,13a4 <printf+0x5a>
    1530:	0014c783          	lbu	a5,1(s1)
    1534:	ff2788e3          	beq	a5,s2,1524 <printf+0x1da>
        l = z - a;
    1538:	40a6043b          	subw	s0,a2,a0
    write(f, s, l);
    153c:	85aa                	mv	a1,a0
    153e:	8622                	mv	a2,s0
    1540:	4505                	li	a0,1
    1542:	73c000ef          	jal	ra,1c7e <write>
        if (l)
    1546:	e60408e3          	beqz	s0,13b6 <printf+0x6c>
    154a:	8526                	mv	a0,s1
    154c:	b581                	j	138c <printf+0x42>
        switch (s[1])
    154e:	07800713          	li	a4,120
    1552:	04e79963          	bne	a5,a4,15a4 <printf+0x25a>
            printint(va_arg(ap, int), 16, 1);
    1556:	6782                	ld	a5,0(sp)
    1558:	45c1                	li	a1,16
    155a:	4388                	lw	a0,0(a5)
    155c:	07a1                	addi	a5,a5,8
    155e:	e03e                	sd	a5,0(sp)
    1560:	b71ff0ef          	jal	ra,10d0 <printint.constprop.0>
        s += 2;
    1564:	00248513          	addi	a0,s1,2
    1568:	bf51                	j	14fc <printf+0x1b2>
            printint(va_arg(ap, int), 10, 1);
    156a:	6782                	ld	a5,0(sp)
    156c:	45a9                	li	a1,10
    156e:	4388                	lw	a0,0(a5)
    1570:	07a1                	addi	a5,a5,8
    1572:	e03e                	sd	a5,0(sp)
    1574:	b5dff0ef          	jal	ra,10d0 <printint.constprop.0>
        s += 2;
    1578:	00248513          	addi	a0,s1,2
    157c:	b741                	j	14fc <printf+0x1b2>
            if ((a = va_arg(ap, char *)) == 0)
    157e:	6782                	ld	a5,0(sp)
    1580:	6380                	ld	s0,0(a5)
    1582:	07a1                	addi	a5,a5,8
    1584:	e03e                	sd	a5,0(sp)
    1586:	c031                	beqz	s0,15ca <printf+0x280>
            l = strnlen(a, 200);
    1588:	0c800593          	li	a1,200
    158c:	8522                	mv	a0,s0
    158e:	3f8000ef          	jal	ra,1986 <strnlen>
    write(f, s, l);
    1592:	0005061b          	sext.w	a2,a0
    1596:	85a2                	mv	a1,s0
    1598:	4505                	li	a0,1
    159a:	6e4000ef          	jal	ra,1c7e <write>
        s += 2;
    159e:	00248513          	addi	a0,s1,2
    15a2:	bfa9                	j	14fc <printf+0x1b2>
    return write(stdout, &byte, 1);
    15a4:	4605                	li	a2,1
    15a6:	002c                	addi	a1,sp,8
    15a8:	4505                	li	a0,1
    char byte = c;
    15aa:	01210423          	sb	s2,8(sp)
    return write(stdout, &byte, 1);
    15ae:	6d0000ef          	jal	ra,1c7e <write>
    char byte = c;
    15b2:	0014c783          	lbu	a5,1(s1)
    return write(stdout, &byte, 1);
    15b6:	4605                	li	a2,1
    15b8:	002c                	addi	a1,sp,8
    15ba:	4505                	li	a0,1
    char byte = c;
    15bc:	00f10423          	sb	a5,8(sp)
    return write(stdout, &byte, 1);
    15c0:	6be000ef          	jal	ra,1c7e <write>
        s += 2;
    15c4:	00248513          	addi	a0,s1,2
    15c8:	bf15                	j	14fc <printf+0x1b2>
                a = "(null)";
    15ca:	845a                	mv	s0,s6
    15cc:	bf75                	j	1588 <printf+0x23e>

00000000000015ce <isspace>:
#define HIGHS (ONES * (UCHAR_MAX / 2 + 1))
#define HASZERO(x) (((x)-ONES) & ~(x)&HIGHS)

int isspace(int c)
{
    return c == ' ' || (unsigned)c - '\t' < 5;
    15ce:	02000793          	li	a5,32
    15d2:	00f50663          	beq	a0,a5,15de <isspace+0x10>
    15d6:	355d                	addiw	a0,a0,-9
    15d8:	00553513          	sltiu	a0,a0,5
    15dc:	8082                	ret
    15de:	4505                	li	a0,1
}
    15e0:	8082                	ret

00000000000015e2 <isdigit>:

int isdigit(int c)
{
    return (unsigned)c - '0' < 10;
    15e2:	fd05051b          	addiw	a0,a0,-48
}
    15e6:	00a53513          	sltiu	a0,a0,10
    15ea:	8082                	ret

00000000000015ec <atoi>:
    return c == ' ' || (unsigned)c - '\t' < 5;
    15ec:	02000613          	li	a2,32
    15f0:	4591                	li	a1,4

int atoi(const char *s)
{
    int n = 0, neg = 0;
    while (isspace(*s))
    15f2:	00054703          	lbu	a4,0(a0)
    return c == ' ' || (unsigned)c - '\t' < 5;
    15f6:	ff77069b          	addiw	a3,a4,-9
    15fa:	04c70d63          	beq	a4,a2,1654 <atoi+0x68>
    15fe:	0007079b          	sext.w	a5,a4
    1602:	04d5f963          	bgeu	a1,a3,1654 <atoi+0x68>
        s++;
    switch (*s)
    1606:	02b00693          	li	a3,43
    160a:	04d70a63          	beq	a4,a3,165e <atoi+0x72>
    160e:	02d00693          	li	a3,45
    1612:	06d70463          	beq	a4,a3,167a <atoi+0x8e>
        neg = 1;
    case '+':
        s++;
    }
    /* Compute n as a negative number to avoid overflow on INT_MIN */
    while (isdigit(*s))
    1616:	fd07859b          	addiw	a1,a5,-48
    161a:	4625                	li	a2,9
    161c:	873e                	mv	a4,a5
    161e:	86aa                	mv	a3,a0
    int n = 0, neg = 0;
    1620:	4e01                	li	t3,0
    while (isdigit(*s))
    1622:	04b66a63          	bltu	a2,a1,1676 <atoi+0x8a>
    int n = 0, neg = 0;
    1626:	4501                	li	a0,0
    while (isdigit(*s))
    1628:	4825                	li	a6,9
    162a:	0016c603          	lbu	a2,1(a3)
        n = 10 * n - (*s++ - '0');
    162e:	0025179b          	slliw	a5,a0,0x2
    1632:	9d3d                	addw	a0,a0,a5
    1634:	fd07031b          	addiw	t1,a4,-48
    1638:	0015189b          	slliw	a7,a0,0x1
    while (isdigit(*s))
    163c:	fd06059b          	addiw	a1,a2,-48
        n = 10 * n - (*s++ - '0');
    1640:	0685                	addi	a3,a3,1
    1642:	4068853b          	subw	a0,a7,t1
    while (isdigit(*s))
    1646:	0006071b          	sext.w	a4,a2
    164a:	feb870e3          	bgeu	a6,a1,162a <atoi+0x3e>
    return neg ? n : -n;
    164e:	000e0563          	beqz	t3,1658 <atoi+0x6c>
}
    1652:	8082                	ret
        s++;
    1654:	0505                	addi	a0,a0,1
    1656:	bf71                	j	15f2 <atoi+0x6>
    return neg ? n : -n;
    1658:	4113053b          	subw	a0,t1,a7
    165c:	8082                	ret
    while (isdigit(*s))
    165e:	00154783          	lbu	a5,1(a0)
    1662:	4625                	li	a2,9
        s++;
    1664:	00150693          	addi	a3,a0,1
    while (isdigit(*s))
    1668:	fd07859b          	addiw	a1,a5,-48
    166c:	0007871b          	sext.w	a4,a5
    int n = 0, neg = 0;
    1670:	4e01                	li	t3,0
    while (isdigit(*s))
    1672:	fab67ae3          	bgeu	a2,a1,1626 <atoi+0x3a>
    1676:	4501                	li	a0,0
}
    1678:	8082                	ret
    while (isdigit(*s))
    167a:	00154783          	lbu	a5,1(a0)
    167e:	4625                	li	a2,9
        s++;
    1680:	00150693          	addi	a3,a0,1
    while (isdigit(*s))
    1684:	fd07859b          	addiw	a1,a5,-48
    1688:	0007871b          	sext.w	a4,a5
    168c:	feb665e3          	bltu	a2,a1,1676 <atoi+0x8a>
        neg = 1;
    1690:	4e05                	li	t3,1
    1692:	bf51                	j	1626 <atoi+0x3a>

0000000000001694 <memset>:

void *memset(void *dest, int c, size_t n)
{
    char *p = dest;
    for (int i = 0; i < n; ++i, *(p++) = c)
    1694:	16060d63          	beqz	a2,180e <memset+0x17a>
    1698:	40a007b3          	neg	a5,a0
    169c:	8b9d                	andi	a5,a5,7
    169e:	00778713          	addi	a4,a5,7
    16a2:	482d                	li	a6,11
    16a4:	0ff5f593          	andi	a1,a1,255
    16a8:	fff60693          	addi	a3,a2,-1
    16ac:	17076263          	bltu	a4,a6,1810 <memset+0x17c>
    16b0:	16e6ea63          	bltu	a3,a4,1824 <memset+0x190>
    16b4:	16078563          	beqz	a5,181e <memset+0x18a>
    16b8:	00b50023          	sb	a1,0(a0)
    16bc:	4705                	li	a4,1
    16be:	00150e93          	addi	t4,a0,1
    16c2:	14e78c63          	beq	a5,a4,181a <memset+0x186>
    16c6:	00b500a3          	sb	a1,1(a0)
    16ca:	4709                	li	a4,2
    16cc:	00250e93          	addi	t4,a0,2
    16d0:	14e78d63          	beq	a5,a4,182a <memset+0x196>
    16d4:	00b50123          	sb	a1,2(a0)
    16d8:	470d                	li	a4,3
    16da:	00350e93          	addi	t4,a0,3
    16de:	12e78b63          	beq	a5,a4,1814 <memset+0x180>
    16e2:	00b501a3          	sb	a1,3(a0)
    16e6:	4711                	li	a4,4
    16e8:	00450e93          	addi	t4,a0,4
    16ec:	14e78163          	beq	a5,a4,182e <memset+0x19a>
    16f0:	00b50223          	sb	a1,4(a0)
    16f4:	4715                	li	a4,5
    16f6:	00550e93          	addi	t4,a0,5
    16fa:	12e78c63          	beq	a5,a4,1832 <memset+0x19e>
    16fe:	00b502a3          	sb	a1,5(a0)
    1702:	471d                	li	a4,7
    1704:	00650e93          	addi	t4,a0,6
    1708:	12e79763          	bne	a5,a4,1836 <memset+0x1a2>
    170c:	00750e93          	addi	t4,a0,7
    1710:	00b50323          	sb	a1,6(a0)
    1714:	4f1d                	li	t5,7
    1716:	00859713          	slli	a4,a1,0x8
    171a:	8f4d                	or	a4,a4,a1
    171c:	01059e13          	slli	t3,a1,0x10
    1720:	01c76e33          	or	t3,a4,t3
    1724:	01859313          	slli	t1,a1,0x18
    1728:	006e6333          	or	t1,t3,t1
    172c:	02059893          	slli	a7,a1,0x20
    1730:	011368b3          	or	a7,t1,a7
    1734:	02859813          	slli	a6,a1,0x28
    1738:	40f60333          	sub	t1,a2,a5
    173c:	0108e833          	or	a6,a7,a6
    1740:	03059693          	slli	a3,a1,0x30
    1744:	00d866b3          	or	a3,a6,a3
    1748:	03859713          	slli	a4,a1,0x38
    174c:	97aa                	add	a5,a5,a0
    174e:	ff837813          	andi	a6,t1,-8
    1752:	8f55                	or	a4,a4,a3
    1754:	00f806b3          	add	a3,a6,a5
    1758:	e398                	sd	a4,0(a5)
    175a:	07a1                	addi	a5,a5,8
    175c:	fed79ee3          	bne	a5,a3,1758 <memset+0xc4>
    1760:	ff837693          	andi	a3,t1,-8
    1764:	00de87b3          	add	a5,t4,a3
    1768:	01e6873b          	addw	a4,a3,t5
    176c:	0ad30663          	beq	t1,a3,1818 <memset+0x184>
    1770:	00b78023          	sb	a1,0(a5)
    1774:	0017069b          	addiw	a3,a4,1
    1778:	08c6fb63          	bgeu	a3,a2,180e <memset+0x17a>
    177c:	00b780a3          	sb	a1,1(a5)
    1780:	0027069b          	addiw	a3,a4,2
    1784:	08c6f563          	bgeu	a3,a2,180e <memset+0x17a>
    1788:	00b78123          	sb	a1,2(a5)
    178c:	0037069b          	addiw	a3,a4,3
    1790:	06c6ff63          	bgeu	a3,a2,180e <memset+0x17a>
    1794:	00b781a3          	sb	a1,3(a5)
    1798:	0047069b          	addiw	a3,a4,4
    179c:	06c6f963          	bgeu	a3,a2,180e <memset+0x17a>
    17a0:	00b78223          	sb	a1,4(a5)
    17a4:	0057069b          	addiw	a3,a4,5
    17a8:	06c6f363          	bgeu	a3,a2,180e <memset+0x17a>
    17ac:	00b782a3          	sb	a1,5(a5)
    17b0:	0067069b          	addiw	a3,a4,6
    17b4:	04c6fd63          	bgeu	a3,a2,180e <memset+0x17a>
    17b8:	00b78323          	sb	a1,6(a5)
    17bc:	0077069b          	addiw	a3,a4,7
    17c0:	04c6f763          	bgeu	a3,a2,180e <memset+0x17a>
    17c4:	00b783a3          	sb	a1,7(a5)
    17c8:	0087069b          	addiw	a3,a4,8
    17cc:	04c6f163          	bgeu	a3,a2,180e <memset+0x17a>
    17d0:	00b78423          	sb	a1,8(a5)
    17d4:	0097069b          	addiw	a3,a4,9
    17d8:	02c6fb63          	bgeu	a3,a2,180e <memset+0x17a>
    17dc:	00b784a3          	sb	a1,9(a5)
    17e0:	00a7069b          	addiw	a3,a4,10
    17e4:	02c6f563          	bgeu	a3,a2,180e <memset+0x17a>
    17e8:	00b78523          	sb	a1,10(a5)
    17ec:	00b7069b          	addiw	a3,a4,11
    17f0:	00c6ff63          	bgeu	a3,a2,180e <memset+0x17a>
    17f4:	00b785a3          	sb	a1,11(a5)
    17f8:	00c7069b          	addiw	a3,a4,12
    17fc:	00c6f963          	bgeu	a3,a2,180e <memset+0x17a>
    1800:	00b78623          	sb	a1,12(a5)
    1804:	2735                	addiw	a4,a4,13
    1806:	00c77463          	bgeu	a4,a2,180e <memset+0x17a>
    180a:	00b786a3          	sb	a1,13(a5)
        ;
    return dest;
}
    180e:	8082                	ret
    1810:	472d                	li	a4,11
    1812:	bd79                	j	16b0 <memset+0x1c>
    for (int i = 0; i < n; ++i, *(p++) = c)
    1814:	4f0d                	li	t5,3
    1816:	b701                	j	1716 <memset+0x82>
    1818:	8082                	ret
    181a:	4f05                	li	t5,1
    181c:	bded                	j	1716 <memset+0x82>
    181e:	8eaa                	mv	t4,a0
    1820:	4f01                	li	t5,0
    1822:	bdd5                	j	1716 <memset+0x82>
    1824:	87aa                	mv	a5,a0
    1826:	4701                	li	a4,0
    1828:	b7a1                	j	1770 <memset+0xdc>
    182a:	4f09                	li	t5,2
    182c:	b5ed                	j	1716 <memset+0x82>
    182e:	4f11                	li	t5,4
    1830:	b5dd                	j	1716 <memset+0x82>
    1832:	4f15                	li	t5,5
    1834:	b5cd                	j	1716 <memset+0x82>
    1836:	4f19                	li	t5,6
    1838:	bdf9                	j	1716 <memset+0x82>

000000000000183a <strcmp>:

int strcmp(const char *l, const char *r)
{
    for (; *l == *r && *l; l++, r++)
    183a:	00054783          	lbu	a5,0(a0)
    183e:	0005c703          	lbu	a4,0(a1)
    1842:	00e79863          	bne	a5,a4,1852 <strcmp+0x18>
    1846:	0505                	addi	a0,a0,1
    1848:	0585                	addi	a1,a1,1
    184a:	fbe5                	bnez	a5,183a <strcmp>
    184c:	4501                	li	a0,0
        ;
    return *(unsigned char *)l - *(unsigned char *)r;
}
    184e:	9d19                	subw	a0,a0,a4
    1850:	8082                	ret
    return *(unsigned char *)l - *(unsigned char *)r;
    1852:	0007851b          	sext.w	a0,a5
    1856:	bfe5                	j	184e <strcmp+0x14>

0000000000001858 <strncmp>:

int strncmp(const char *_l, const char *_r, size_t n)
{
    const unsigned char *l = (void *)_l, *r = (void *)_r;
    if (!n--)
    1858:	ce05                	beqz	a2,1890 <strncmp+0x38>
        return 0;
    for (; *l && *r && n && *l == *r; l++, r++, n--)
    185a:	00054703          	lbu	a4,0(a0)
    185e:	0005c783          	lbu	a5,0(a1)
    1862:	cb0d                	beqz	a4,1894 <strncmp+0x3c>
    if (!n--)
    1864:	167d                	addi	a2,a2,-1
    1866:	00c506b3          	add	a3,a0,a2
    186a:	a819                	j	1880 <strncmp+0x28>
    for (; *l && *r && n && *l == *r; l++, r++, n--)
    186c:	00a68e63          	beq	a3,a0,1888 <strncmp+0x30>
    1870:	0505                	addi	a0,a0,1
    1872:	00e79b63          	bne	a5,a4,1888 <strncmp+0x30>
    1876:	00054703          	lbu	a4,0(a0)
        ;
    return *l - *r;
    187a:	0005c783          	lbu	a5,0(a1)
    for (; *l && *r && n && *l == *r; l++, r++, n--)
    187e:	cb19                	beqz	a4,1894 <strncmp+0x3c>
    1880:	0005c783          	lbu	a5,0(a1)
    1884:	0585                	addi	a1,a1,1
    1886:	f3fd                	bnez	a5,186c <strncmp+0x14>
    return *l - *r;
    1888:	0007051b          	sext.w	a0,a4
    188c:	9d1d                	subw	a0,a0,a5
    188e:	8082                	ret
        return 0;
    1890:	4501                	li	a0,0
}
    1892:	8082                	ret
    1894:	4501                	li	a0,0
    return *l - *r;
    1896:	9d1d                	subw	a0,a0,a5
    1898:	8082                	ret

000000000000189a <strlen>:
size_t strlen(const char *s)
{
    const char *a = s;
    typedef size_t __attribute__((__may_alias__)) word;
    const word *w;
    for (; (uintptr_t)s % SS; s++)
    189a:	00757793          	andi	a5,a0,7
    189e:	cf89                	beqz	a5,18b8 <strlen+0x1e>
    18a0:	87aa                	mv	a5,a0
    18a2:	a029                	j	18ac <strlen+0x12>
    18a4:	0785                	addi	a5,a5,1
    18a6:	0077f713          	andi	a4,a5,7
    18aa:	cb01                	beqz	a4,18ba <strlen+0x20>
        if (!*s)
    18ac:	0007c703          	lbu	a4,0(a5)
    18b0:	fb75                	bnez	a4,18a4 <strlen+0xa>
    for (w = (const void *)s; !HASZERO(*w); w++)
        ;
    s = (const void *)w;
    for (; *s; s++)
        ;
    return s - a;
    18b2:	40a78533          	sub	a0,a5,a0
}
    18b6:	8082                	ret
    for (; (uintptr_t)s % SS; s++)
    18b8:	87aa                	mv	a5,a0
    for (w = (const void *)s; !HASZERO(*w); w++)
    18ba:	6394                	ld	a3,0(a5)
    18bc:	00000597          	auipc	a1,0x0
    18c0:	6c45b583          	ld	a1,1732(a1) # 1f80 <__clone+0xce>
    18c4:	00000617          	auipc	a2,0x0
    18c8:	6c463603          	ld	a2,1732(a2) # 1f88 <__clone+0xd6>
    18cc:	a019                	j	18d2 <strlen+0x38>
    18ce:	6794                	ld	a3,8(a5)
    18d0:	07a1                	addi	a5,a5,8
    18d2:	00b68733          	add	a4,a3,a1
    18d6:	fff6c693          	not	a3,a3
    18da:	8f75                	and	a4,a4,a3
    18dc:	8f71                	and	a4,a4,a2
    18de:	db65                	beqz	a4,18ce <strlen+0x34>
    for (; *s; s++)
    18e0:	0007c703          	lbu	a4,0(a5)
    18e4:	d779                	beqz	a4,18b2 <strlen+0x18>
    18e6:	0017c703          	lbu	a4,1(a5)
    18ea:	0785                	addi	a5,a5,1
    18ec:	d379                	beqz	a4,18b2 <strlen+0x18>
    18ee:	0017c703          	lbu	a4,1(a5)
    18f2:	0785                	addi	a5,a5,1
    18f4:	fb6d                	bnez	a4,18e6 <strlen+0x4c>
    18f6:	bf75                	j	18b2 <strlen+0x18>

00000000000018f8 <memchr>:

void *memchr(const void *src, int c, size_t n)
{
    const unsigned char *s = src;
    c = (unsigned char)c;
    for (; ((uintptr_t)s & ALIGN) && n && *s != c; s++, n--)
    18f8:	00757713          	andi	a4,a0,7
{
    18fc:	87aa                	mv	a5,a0
    c = (unsigned char)c;
    18fe:	0ff5f593          	andi	a1,a1,255
    for (; ((uintptr_t)s & ALIGN) && n && *s != c; s++, n--)
    1902:	cb19                	beqz	a4,1918 <memchr+0x20>
    1904:	ce25                	beqz	a2,197c <memchr+0x84>
    1906:	0007c703          	lbu	a4,0(a5)
    190a:	04b70e63          	beq	a4,a1,1966 <memchr+0x6e>
    190e:	0785                	addi	a5,a5,1
    1910:	0077f713          	andi	a4,a5,7
    1914:	167d                	addi	a2,a2,-1
    1916:	f77d                	bnez	a4,1904 <memchr+0xc>
            ;
        s = (const void *)w;
    }
    for (; n && *s != c; s++, n--)
        ;
    return n ? (void *)s : 0;
    1918:	4501                	li	a0,0
    if (n && *s != c)
    191a:	c235                	beqz	a2,197e <memchr+0x86>
    191c:	0007c703          	lbu	a4,0(a5)
    1920:	04b70363          	beq	a4,a1,1966 <memchr+0x6e>
        size_t k = ONES * c;
    1924:	00000517          	auipc	a0,0x0
    1928:	66c53503          	ld	a0,1644(a0) # 1f90 <__clone+0xde>
        for (w = (const void *)s; n >= SS && !HASZERO(*w ^ k); w++, n -= SS)
    192c:	471d                	li	a4,7
        size_t k = ONES * c;
    192e:	02a58533          	mul	a0,a1,a0
        for (w = (const void *)s; n >= SS && !HASZERO(*w ^ k); w++, n -= SS)
    1932:	02c77a63          	bgeu	a4,a2,1966 <memchr+0x6e>
    1936:	00000897          	auipc	a7,0x0
    193a:	64a8b883          	ld	a7,1610(a7) # 1f80 <__clone+0xce>
    193e:	00000817          	auipc	a6,0x0
    1942:	64a83803          	ld	a6,1610(a6) # 1f88 <__clone+0xd6>
    1946:	431d                	li	t1,7
    1948:	a029                	j	1952 <memchr+0x5a>
    194a:	1661                	addi	a2,a2,-8
    194c:	07a1                	addi	a5,a5,8
    194e:	02c37963          	bgeu	t1,a2,1980 <memchr+0x88>
    1952:	6398                	ld	a4,0(a5)
    1954:	8f29                	xor	a4,a4,a0
    1956:	011706b3          	add	a3,a4,a7
    195a:	fff74713          	not	a4,a4
    195e:	8f75                	and	a4,a4,a3
    1960:	01077733          	and	a4,a4,a6
    1964:	d37d                	beqz	a4,194a <memchr+0x52>
    1966:	853e                	mv	a0,a5
    1968:	97b2                	add	a5,a5,a2
    196a:	a021                	j	1972 <memchr+0x7a>
    for (; n && *s != c; s++, n--)
    196c:	0505                	addi	a0,a0,1
    196e:	00f50763          	beq	a0,a5,197c <memchr+0x84>
    1972:	00054703          	lbu	a4,0(a0)
    1976:	feb71be3          	bne	a4,a1,196c <memchr+0x74>
    197a:	8082                	ret
    return n ? (void *)s : 0;
    197c:	4501                	li	a0,0
}
    197e:	8082                	ret
    return n ? (void *)s : 0;
    1980:	4501                	li	a0,0
    for (; n && *s != c; s++, n--)
    1982:	f275                	bnez	a2,1966 <memchr+0x6e>
}
    1984:	8082                	ret

0000000000001986 <strnlen>:

size_t strnlen(const char *s, size_t n)
{
    1986:	1101                	addi	sp,sp,-32
    1988:	e822                	sd	s0,16(sp)
    const char *p = memchr(s, 0, n);
    198a:	862e                	mv	a2,a1
{
    198c:	842e                	mv	s0,a1
    const char *p = memchr(s, 0, n);
    198e:	4581                	li	a1,0
{
    1990:	e426                	sd	s1,8(sp)
    1992:	ec06                	sd	ra,24(sp)
    1994:	84aa                	mv	s1,a0
    const char *p = memchr(s, 0, n);
    1996:	f63ff0ef          	jal	ra,18f8 <memchr>
    return p ? p - s : n;
    199a:	c519                	beqz	a0,19a8 <strnlen+0x22>
}
    199c:	60e2                	ld	ra,24(sp)
    199e:	6442                	ld	s0,16(sp)
    return p ? p - s : n;
    19a0:	8d05                	sub	a0,a0,s1
}
    19a2:	64a2                	ld	s1,8(sp)
    19a4:	6105                	addi	sp,sp,32
    19a6:	8082                	ret
    19a8:	60e2                	ld	ra,24(sp)
    return p ? p - s : n;
    19aa:	8522                	mv	a0,s0
}
    19ac:	6442                	ld	s0,16(sp)
    19ae:	64a2                	ld	s1,8(sp)
    19b0:	6105                	addi	sp,sp,32
    19b2:	8082                	ret

00000000000019b4 <strcpy>:
char *strcpy(char *restrict d, const char *s)
{
    typedef size_t __attribute__((__may_alias__)) word;
    word *wd;
    const word *ws;
    if ((uintptr_t)s % SS == (uintptr_t)d % SS)
    19b4:	00b547b3          	xor	a5,a0,a1
    19b8:	8b9d                	andi	a5,a5,7
    19ba:	eb95                	bnez	a5,19ee <strcpy+0x3a>
    {
        for (; (uintptr_t)s % SS; s++, d++)
    19bc:	0075f793          	andi	a5,a1,7
    19c0:	e7b1                	bnez	a5,1a0c <strcpy+0x58>
            if (!(*d = *s))
                return d;
        wd = (void *)d;
        ws = (const void *)s;
        for (; !HASZERO(*ws); *wd++ = *ws++)
    19c2:	6198                	ld	a4,0(a1)
    19c4:	00000617          	auipc	a2,0x0
    19c8:	5bc63603          	ld	a2,1468(a2) # 1f80 <__clone+0xce>
    19cc:	00000817          	auipc	a6,0x0
    19d0:	5bc83803          	ld	a6,1468(a6) # 1f88 <__clone+0xd6>
    19d4:	a029                	j	19de <strcpy+0x2a>
    19d6:	e118                	sd	a4,0(a0)
    19d8:	6598                	ld	a4,8(a1)
    19da:	05a1                	addi	a1,a1,8
    19dc:	0521                	addi	a0,a0,8
    19de:	00c707b3          	add	a5,a4,a2
    19e2:	fff74693          	not	a3,a4
    19e6:	8ff5                	and	a5,a5,a3
    19e8:	0107f7b3          	and	a5,a5,a6
    19ec:	d7ed                	beqz	a5,19d6 <strcpy+0x22>
            ;
        d = (void *)wd;
        s = (const void *)ws;
    }
    for (; (*d = *s); s++, d++)
    19ee:	0005c783          	lbu	a5,0(a1)
    19f2:	00f50023          	sb	a5,0(a0)
    19f6:	c785                	beqz	a5,1a1e <strcpy+0x6a>
    19f8:	0015c783          	lbu	a5,1(a1)
    19fc:	0505                	addi	a0,a0,1
    19fe:	0585                	addi	a1,a1,1
    1a00:	00f50023          	sb	a5,0(a0)
    1a04:	fbf5                	bnez	a5,19f8 <strcpy+0x44>
        ;
    return d;
}
    1a06:	8082                	ret
        for (; (uintptr_t)s % SS; s++, d++)
    1a08:	0505                	addi	a0,a0,1
    1a0a:	df45                	beqz	a4,19c2 <strcpy+0xe>
            if (!(*d = *s))
    1a0c:	0005c783          	lbu	a5,0(a1)
        for (; (uintptr_t)s % SS; s++, d++)
    1a10:	0585                	addi	a1,a1,1
    1a12:	0075f713          	andi	a4,a1,7
            if (!(*d = *s))
    1a16:	00f50023          	sb	a5,0(a0)
    1a1a:	f7fd                	bnez	a5,1a08 <strcpy+0x54>
}
    1a1c:	8082                	ret
    1a1e:	8082                	ret

0000000000001a20 <strncpy>:
char *strncpy(char *restrict d, const char *s, size_t n)
{
    typedef size_t __attribute__((__may_alias__)) word;
    word *wd;
    const word *ws;
    if (((uintptr_t)s & ALIGN) == ((uintptr_t)d & ALIGN))
    1a20:	00b547b3          	xor	a5,a0,a1
    1a24:	8b9d                	andi	a5,a5,7
    1a26:	1a079863          	bnez	a5,1bd6 <strncpy+0x1b6>
    {
        for (; ((uintptr_t)s & ALIGN) && n && (*d = *s); n--, s++, d++)
    1a2a:	0075f793          	andi	a5,a1,7
    1a2e:	16078463          	beqz	a5,1b96 <strncpy+0x176>
    1a32:	ea01                	bnez	a2,1a42 <strncpy+0x22>
    1a34:	a421                	j	1c3c <strncpy+0x21c>
    1a36:	167d                	addi	a2,a2,-1
    1a38:	0505                	addi	a0,a0,1
    1a3a:	14070e63          	beqz	a4,1b96 <strncpy+0x176>
    1a3e:	1a060863          	beqz	a2,1bee <strncpy+0x1ce>
    1a42:	0005c783          	lbu	a5,0(a1)
    1a46:	0585                	addi	a1,a1,1
    1a48:	0075f713          	andi	a4,a1,7
    1a4c:	00f50023          	sb	a5,0(a0)
    1a50:	f3fd                	bnez	a5,1a36 <strncpy+0x16>
    1a52:	4805                	li	a6,1
    1a54:	1a061863          	bnez	a2,1c04 <strncpy+0x1e4>
    1a58:	40a007b3          	neg	a5,a0
    1a5c:	8b9d                	andi	a5,a5,7
    1a5e:	4681                	li	a3,0
    1a60:	18061a63          	bnez	a2,1bf4 <strncpy+0x1d4>
    1a64:	00778713          	addi	a4,a5,7
    1a68:	45ad                	li	a1,11
    1a6a:	18b76363          	bltu	a4,a1,1bf0 <strncpy+0x1d0>
    1a6e:	1ae6eb63          	bltu	a3,a4,1c24 <strncpy+0x204>
    1a72:	1a078363          	beqz	a5,1c18 <strncpy+0x1f8>
    for (int i = 0; i < n; ++i, *(p++) = c)
    1a76:	00050023          	sb	zero,0(a0)
    1a7a:	4685                	li	a3,1
    1a7c:	00150713          	addi	a4,a0,1
    1a80:	18d78f63          	beq	a5,a3,1c1e <strncpy+0x1fe>
    1a84:	000500a3          	sb	zero,1(a0)
    1a88:	4689                	li	a3,2
    1a8a:	00250713          	addi	a4,a0,2
    1a8e:	18d78e63          	beq	a5,a3,1c2a <strncpy+0x20a>
    1a92:	00050123          	sb	zero,2(a0)
    1a96:	468d                	li	a3,3
    1a98:	00350713          	addi	a4,a0,3
    1a9c:	16d78c63          	beq	a5,a3,1c14 <strncpy+0x1f4>
    1aa0:	000501a3          	sb	zero,3(a0)
    1aa4:	4691                	li	a3,4
    1aa6:	00450713          	addi	a4,a0,4
    1aaa:	18d78263          	beq	a5,a3,1c2e <strncpy+0x20e>
    1aae:	00050223          	sb	zero,4(a0)
    1ab2:	4695                	li	a3,5
    1ab4:	00550713          	addi	a4,a0,5
    1ab8:	16d78d63          	beq	a5,a3,1c32 <strncpy+0x212>
    1abc:	000502a3          	sb	zero,5(a0)
    1ac0:	469d                	li	a3,7
    1ac2:	00650713          	addi	a4,a0,6
    1ac6:	16d79863          	bne	a5,a3,1c36 <strncpy+0x216>
    1aca:	00750713          	addi	a4,a0,7
    1ace:	00050323          	sb	zero,6(a0)
    1ad2:	40f80833          	sub	a6,a6,a5
    1ad6:	ff887593          	andi	a1,a6,-8
    1ada:	97aa                	add	a5,a5,a0
    1adc:	95be                	add	a1,a1,a5
    1ade:	0007b023          	sd	zero,0(a5)
    1ae2:	07a1                	addi	a5,a5,8
    1ae4:	feb79de3          	bne	a5,a1,1ade <strncpy+0xbe>
    1ae8:	ff887593          	andi	a1,a6,-8
    1aec:	9ead                	addw	a3,a3,a1
    1aee:	00b707b3          	add	a5,a4,a1
    1af2:	12b80863          	beq	a6,a1,1c22 <strncpy+0x202>
    1af6:	00078023          	sb	zero,0(a5)
    1afa:	0016871b          	addiw	a4,a3,1
    1afe:	0ec77863          	bgeu	a4,a2,1bee <strncpy+0x1ce>
    1b02:	000780a3          	sb	zero,1(a5)
    1b06:	0026871b          	addiw	a4,a3,2
    1b0a:	0ec77263          	bgeu	a4,a2,1bee <strncpy+0x1ce>
    1b0e:	00078123          	sb	zero,2(a5)
    1b12:	0036871b          	addiw	a4,a3,3
    1b16:	0cc77c63          	bgeu	a4,a2,1bee <strncpy+0x1ce>
    1b1a:	000781a3          	sb	zero,3(a5)
    1b1e:	0046871b          	addiw	a4,a3,4
    1b22:	0cc77663          	bgeu	a4,a2,1bee <strncpy+0x1ce>
    1b26:	00078223          	sb	zero,4(a5)
    1b2a:	0056871b          	addiw	a4,a3,5
    1b2e:	0cc77063          	bgeu	a4,a2,1bee <strncpy+0x1ce>
    1b32:	000782a3          	sb	zero,5(a5)
    1b36:	0066871b          	addiw	a4,a3,6
    1b3a:	0ac77a63          	bgeu	a4,a2,1bee <strncpy+0x1ce>
    1b3e:	00078323          	sb	zero,6(a5)
    1b42:	0076871b          	addiw	a4,a3,7
    1b46:	0ac77463          	bgeu	a4,a2,1bee <strncpy+0x1ce>
    1b4a:	000783a3          	sb	zero,7(a5)
    1b4e:	0086871b          	addiw	a4,a3,8
    1b52:	08c77e63          	bgeu	a4,a2,1bee <strncpy+0x1ce>
    1b56:	00078423          	sb	zero,8(a5)
    1b5a:	0096871b          	addiw	a4,a3,9
    1b5e:	08c77863          	bgeu	a4,a2,1bee <strncpy+0x1ce>
    1b62:	000784a3          	sb	zero,9(a5)
    1b66:	00a6871b          	addiw	a4,a3,10
    1b6a:	08c77263          	bgeu	a4,a2,1bee <strncpy+0x1ce>
    1b6e:	00078523          	sb	zero,10(a5)
    1b72:	00b6871b          	addiw	a4,a3,11
    1b76:	06c77c63          	bgeu	a4,a2,1bee <strncpy+0x1ce>
    1b7a:	000785a3          	sb	zero,11(a5)
    1b7e:	00c6871b          	addiw	a4,a3,12
    1b82:	06c77663          	bgeu	a4,a2,1bee <strncpy+0x1ce>
    1b86:	00078623          	sb	zero,12(a5)
    1b8a:	26b5                	addiw	a3,a3,13
    1b8c:	06c6f163          	bgeu	a3,a2,1bee <strncpy+0x1ce>
    1b90:	000786a3          	sb	zero,13(a5)
    1b94:	8082                	ret
            ;
        if (!n || !*s)
    1b96:	c645                	beqz	a2,1c3e <strncpy+0x21e>
    1b98:	0005c783          	lbu	a5,0(a1)
    1b9c:	ea078be3          	beqz	a5,1a52 <strncpy+0x32>
            goto tail;
        wd = (void *)d;
        ws = (const void *)s;
        for (; n >= sizeof(size_t) && !HASZERO(*ws); n -= sizeof(size_t), ws++, wd++)
    1ba0:	479d                	li	a5,7
    1ba2:	02c7ff63          	bgeu	a5,a2,1be0 <strncpy+0x1c0>
    1ba6:	00000897          	auipc	a7,0x0
    1baa:	3da8b883          	ld	a7,986(a7) # 1f80 <__clone+0xce>
    1bae:	00000817          	auipc	a6,0x0
    1bb2:	3da83803          	ld	a6,986(a6) # 1f88 <__clone+0xd6>
    1bb6:	431d                	li	t1,7
    1bb8:	6198                	ld	a4,0(a1)
    1bba:	011707b3          	add	a5,a4,a7
    1bbe:	fff74693          	not	a3,a4
    1bc2:	8ff5                	and	a5,a5,a3
    1bc4:	0107f7b3          	and	a5,a5,a6
    1bc8:	ef81                	bnez	a5,1be0 <strncpy+0x1c0>
            *wd = *ws;
    1bca:	e118                	sd	a4,0(a0)
        for (; n >= sizeof(size_t) && !HASZERO(*ws); n -= sizeof(size_t), ws++, wd++)
    1bcc:	1661                	addi	a2,a2,-8
    1bce:	05a1                	addi	a1,a1,8
    1bd0:	0521                	addi	a0,a0,8
    1bd2:	fec363e3          	bltu	t1,a2,1bb8 <strncpy+0x198>
        d = (void *)wd;
        s = (const void *)ws;
    }
    for (; n && (*d = *s); n--, s++, d++)
    1bd6:	e609                	bnez	a2,1be0 <strncpy+0x1c0>
    1bd8:	a08d                	j	1c3a <strncpy+0x21a>
    1bda:	167d                	addi	a2,a2,-1
    1bdc:	0505                	addi	a0,a0,1
    1bde:	ca01                	beqz	a2,1bee <strncpy+0x1ce>
    1be0:	0005c783          	lbu	a5,0(a1)
    1be4:	0585                	addi	a1,a1,1
    1be6:	00f50023          	sb	a5,0(a0)
    1bea:	fbe5                	bnez	a5,1bda <strncpy+0x1ba>
        ;
tail:
    1bec:	b59d                	j	1a52 <strncpy+0x32>
    memset(d, 0, n);
    return d;
}
    1bee:	8082                	ret
    1bf0:	472d                	li	a4,11
    1bf2:	bdb5                	j	1a6e <strncpy+0x4e>
    1bf4:	00778713          	addi	a4,a5,7
    1bf8:	45ad                	li	a1,11
    1bfa:	fff60693          	addi	a3,a2,-1
    1bfe:	e6b778e3          	bgeu	a4,a1,1a6e <strncpy+0x4e>
    1c02:	b7fd                	j	1bf0 <strncpy+0x1d0>
    1c04:	40a007b3          	neg	a5,a0
    1c08:	8832                	mv	a6,a2
    1c0a:	8b9d                	andi	a5,a5,7
    1c0c:	4681                	li	a3,0
    1c0e:	e4060be3          	beqz	a2,1a64 <strncpy+0x44>
    1c12:	b7cd                	j	1bf4 <strncpy+0x1d4>
    for (int i = 0; i < n; ++i, *(p++) = c)
    1c14:	468d                	li	a3,3
    1c16:	bd75                	j	1ad2 <strncpy+0xb2>
    1c18:	872a                	mv	a4,a0
    1c1a:	4681                	li	a3,0
    1c1c:	bd5d                	j	1ad2 <strncpy+0xb2>
    1c1e:	4685                	li	a3,1
    1c20:	bd4d                	j	1ad2 <strncpy+0xb2>
    1c22:	8082                	ret
    1c24:	87aa                	mv	a5,a0
    1c26:	4681                	li	a3,0
    1c28:	b5f9                	j	1af6 <strncpy+0xd6>
    1c2a:	4689                	li	a3,2
    1c2c:	b55d                	j	1ad2 <strncpy+0xb2>
    1c2e:	4691                	li	a3,4
    1c30:	b54d                	j	1ad2 <strncpy+0xb2>
    1c32:	4695                	li	a3,5
    1c34:	bd79                	j	1ad2 <strncpy+0xb2>
    1c36:	4699                	li	a3,6
    1c38:	bd69                	j	1ad2 <strncpy+0xb2>
    1c3a:	8082                	ret
    1c3c:	8082                	ret
    1c3e:	8082                	ret

0000000000001c40 <open>:
#include <unistd.h>

#include "syscall.h"

int open(const char *path, int flags)
{
    1c40:	87aa                	mv	a5,a0
    1c42:	862e                	mv	a2,a1
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
}

static inline long __syscall4(long n, long a, long b, long c, long d)
{
    register long a7 __asm__("a7") = n;
    1c44:	03800893          	li	a7,56
    register long a0 __asm__("a0") = a;
    1c48:	f9c00513          	li	a0,-100
    register long a1 __asm__("a1") = b;
    1c4c:	85be                	mv	a1,a5
    register long a2 __asm__("a2") = c;
    register long a3 __asm__("a3") = d;
    1c4e:	4689                	li	a3,2
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3))
    1c50:	00000073          	ecall
    return syscall(SYS_openat, AT_FDCWD, path, flags, O_RDWR);
}
    1c54:	2501                	sext.w	a0,a0
    1c56:	8082                	ret

0000000000001c58 <openat>:
    register long a7 __asm__("a7") = n;
    1c58:	03800893          	li	a7,56
    register long a3 __asm__("a3") = d;
    1c5c:	18000693          	li	a3,384
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3))
    1c60:	00000073          	ecall

int openat(int dirfd,const char *path, int flags)
{
    return syscall(SYS_openat, dirfd, path, flags, 0600);
}
    1c64:	2501                	sext.w	a0,a0
    1c66:	8082                	ret

0000000000001c68 <close>:
    register long a7 __asm__("a7") = n;
    1c68:	03900893          	li	a7,57
    __asm_syscall("r"(a7), "0"(a0))
    1c6c:	00000073          	ecall

int close(int fd)
{
    return syscall(SYS_close, fd);
}
    1c70:	2501                	sext.w	a0,a0
    1c72:	8082                	ret

0000000000001c74 <read>:
    register long a7 __asm__("a7") = n;
    1c74:	03f00893          	li	a7,63
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1c78:	00000073          	ecall

ssize_t read(int fd, void *buf, size_t len)
{
    return syscall(SYS_read, fd, buf, len);
}
    1c7c:	8082                	ret

0000000000001c7e <write>:
    register long a7 __asm__("a7") = n;
    1c7e:	04000893          	li	a7,64
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1c82:	00000073          	ecall

ssize_t write(int fd, const void *buf, size_t len)
{
    return syscall(SYS_write, fd, buf, len);
}
    1c86:	8082                	ret

0000000000001c88 <getpid>:
    register long a7 __asm__("a7") = n;
    1c88:	0ac00893          	li	a7,172
    __asm_syscall("r"(a7))
    1c8c:	00000073          	ecall

pid_t getpid(void)
{
    return syscall(SYS_getpid);
}
    1c90:	2501                	sext.w	a0,a0
    1c92:	8082                	ret

0000000000001c94 <getppid>:
    register long a7 __asm__("a7") = n;
    1c94:	0ad00893          	li	a7,173
    __asm_syscall("r"(a7))
    1c98:	00000073          	ecall

pid_t getppid(void)
{
    return syscall(SYS_getppid);
}
    1c9c:	2501                	sext.w	a0,a0
    1c9e:	8082                	ret

0000000000001ca0 <sched_yield>:
    register long a7 __asm__("a7") = n;
    1ca0:	07c00893          	li	a7,124
    __asm_syscall("r"(a7))
    1ca4:	00000073          	ecall

int sched_yield(void)
{
    return syscall(SYS_sched_yield);
}
    1ca8:	2501                	sext.w	a0,a0
    1caa:	8082                	ret

0000000000001cac <fork>:
    register long a7 __asm__("a7") = n;
    1cac:	0dc00893          	li	a7,220
    register long a0 __asm__("a0") = a;
    1cb0:	4545                	li	a0,17
    register long a1 __asm__("a1") = b;
    1cb2:	4581                	li	a1,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1cb4:	00000073          	ecall

pid_t fork(void)
{
    return syscall(SYS_clone, SIGCHLD, 0);
}
    1cb8:	2501                	sext.w	a0,a0
    1cba:	8082                	ret

0000000000001cbc <clone>:

pid_t clone(int (*fn)(void *arg), void *arg, void *stack, size_t stack_size, unsigned long flags)
{
    1cbc:	85b2                	mv	a1,a2
    1cbe:	863a                	mv	a2,a4
    if (stack)
    1cc0:	c191                	beqz	a1,1cc4 <clone+0x8>
	stack += stack_size;
    1cc2:	95b6                	add	a1,a1,a3

    return __clone(fn, stack, flags, NULL, NULL, NULL);
    1cc4:	4781                	li	a5,0
    1cc6:	4701                	li	a4,0
    1cc8:	4681                	li	a3,0
    1cca:	2601                	sext.w	a2,a2
    1ccc:	a2dd                	j	1eb2 <__clone>

0000000000001cce <exit>:
    register long a7 __asm__("a7") = n;
    1cce:	05d00893          	li	a7,93
    __asm_syscall("r"(a7), "0"(a0))
    1cd2:	00000073          	ecall
    //return syscall(SYS_clone, fn, stack, flags, NULL, NULL, NULL);
}
void exit(int code)
{
    syscall(SYS_exit, code);
}
    1cd6:	8082                	ret

0000000000001cd8 <waitpid>:
    register long a7 __asm__("a7") = n;
    1cd8:	10400893          	li	a7,260
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1cdc:	00000073          	ecall

int waitpid(int pid, int *code, int options)
{
    return syscall(SYS_wait4, pid, code, options);
}
    1ce0:	2501                	sext.w	a0,a0
    1ce2:	8082                	ret

0000000000001ce4 <exec>:
    register long a7 __asm__("a7") = n;
    1ce4:	0dd00893          	li	a7,221
    __asm_syscall("r"(a7), "0"(a0))
    1ce8:	00000073          	ecall

int exec(char *name)
{
    return syscall(SYS_execve, name);
}
    1cec:	2501                	sext.w	a0,a0
    1cee:	8082                	ret

0000000000001cf0 <execve>:
    register long a7 __asm__("a7") = n;
    1cf0:	0dd00893          	li	a7,221
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1cf4:	00000073          	ecall

int execve(const char *name, char *const argv[], char *const argp[])
{
    return syscall(SYS_execve, name, argv, argp);
}
    1cf8:	2501                	sext.w	a0,a0
    1cfa:	8082                	ret

0000000000001cfc <times>:
    register long a7 __asm__("a7") = n;
    1cfc:	09900893          	li	a7,153
    __asm_syscall("r"(a7), "0"(a0))
    1d00:	00000073          	ecall

int times(void *mytimes)
{
	return syscall(SYS_times, mytimes);
}
    1d04:	2501                	sext.w	a0,a0
    1d06:	8082                	ret

0000000000001d08 <get_time>:

int64 get_time()
{
    1d08:	1141                	addi	sp,sp,-16
    register long a7 __asm__("a7") = n;
    1d0a:	0a900893          	li	a7,169
    register long a0 __asm__("a0") = a;
    1d0e:	850a                	mv	a0,sp
    register long a1 __asm__("a1") = b;
    1d10:	4581                	li	a1,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1d12:	00000073          	ecall
    TimeVal time;
    int err = sys_get_time(&time, 0);
    if (err == 0)
    1d16:	2501                	sext.w	a0,a0
    1d18:	ed09                	bnez	a0,1d32 <get_time+0x2a>
    {
        return ((time.sec & 0xffff) * 1000 + time.usec / 1000);
    1d1a:	67a2                	ld	a5,8(sp)
    1d1c:	3e800713          	li	a4,1000
    1d20:	00015503          	lhu	a0,0(sp)
    1d24:	02e7d7b3          	divu	a5,a5,a4
    1d28:	02e50533          	mul	a0,a0,a4
    1d2c:	953e                	add	a0,a0,a5
    }
    else
    {
        return -1;
    }
}
    1d2e:	0141                	addi	sp,sp,16
    1d30:	8082                	ret
        return -1;
    1d32:	557d                	li	a0,-1
    1d34:	bfed                	j	1d2e <get_time+0x26>

0000000000001d36 <sys_get_time>:
    register long a7 __asm__("a7") = n;
    1d36:	0a900893          	li	a7,169
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1d3a:	00000073          	ecall

int sys_get_time(TimeVal *ts, int tz)
{
    return syscall(SYS_gettimeofday, ts, tz);
}
    1d3e:	2501                	sext.w	a0,a0
    1d40:	8082                	ret

0000000000001d42 <time>:
    register long a7 __asm__("a7") = n;
    1d42:	42600893          	li	a7,1062
    __asm_syscall("r"(a7), "0"(a0))
    1d46:	00000073          	ecall

int time(unsigned long *tloc)
{
    return syscall(SYS_time, tloc);
}
    1d4a:	2501                	sext.w	a0,a0
    1d4c:	8082                	ret

0000000000001d4e <sleep>:

int sleep(unsigned long long time)
{
    1d4e:	1141                	addi	sp,sp,-16
    TimeVal tv = {.sec = time, .usec = 0};
    1d50:	e02a                	sd	a0,0(sp)
    register long a0 __asm__("a0") = a;
    1d52:	850a                	mv	a0,sp
    1d54:	e402                	sd	zero,8(sp)
    register long a7 __asm__("a7") = n;
    1d56:	06500893          	li	a7,101
    register long a1 __asm__("a1") = b;
    1d5a:	85aa                	mv	a1,a0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1d5c:	00000073          	ecall
    if (syscall(SYS_nanosleep, &tv, &tv)) return tv.sec;
    1d60:	e501                	bnez	a0,1d68 <sleep+0x1a>
    return 0;
    1d62:	4501                	li	a0,0
}
    1d64:	0141                	addi	sp,sp,16
    1d66:	8082                	ret
    if (syscall(SYS_nanosleep, &tv, &tv)) return tv.sec;
    1d68:	4502                	lw	a0,0(sp)
}
    1d6a:	0141                	addi	sp,sp,16
    1d6c:	8082                	ret

0000000000001d6e <set_priority>:
    register long a7 __asm__("a7") = n;
    1d6e:	08c00893          	li	a7,140
    __asm_syscall("r"(a7), "0"(a0))
    1d72:	00000073          	ecall

int set_priority(int prio)
{
    return syscall(SYS_setpriority, prio);
}
    1d76:	2501                	sext.w	a0,a0
    1d78:	8082                	ret

0000000000001d7a <mmap>:
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4))
}

static inline long __syscall6(long n, long a, long b, long c, long d, long e, long f)
{
    register long a7 __asm__("a7") = n;
    1d7a:	0de00893          	li	a7,222
    register long a1 __asm__("a1") = b;
    register long a2 __asm__("a2") = c;
    register long a3 __asm__("a3") = d;
    register long a4 __asm__("a4") = e;
    register long a5 __asm__("a5") = f;
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4), "r"(a5))
    1d7e:	00000073          	ecall

void *mmap(void *start, size_t len, int prot, int flags, int fd, off_t off)
{
    return syscall(SYS_mmap, start, len, prot, flags, fd, off);
}
    1d82:	8082                	ret

0000000000001d84 <munmap>:
    register long a7 __asm__("a7") = n;
    1d84:	0d700893          	li	a7,215
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1d88:	00000073          	ecall

int munmap(void *start, size_t len)
{
    return syscall(SYS_munmap, start, len);
}
    1d8c:	2501                	sext.w	a0,a0
    1d8e:	8082                	ret

0000000000001d90 <wait>:

int wait(int *code)
{
    1d90:	85aa                	mv	a1,a0
    register long a7 __asm__("a7") = n;
    1d92:	10400893          	li	a7,260
    register long a0 __asm__("a0") = a;
    1d96:	557d                	li	a0,-1
    register long a2 __asm__("a2") = c;
    1d98:	4601                	li	a2,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1d9a:	00000073          	ecall
    return waitpid(-1, code, 0);
}
    1d9e:	2501                	sext.w	a0,a0
    1da0:	8082                	ret

0000000000001da2 <spawn>:
    register long a7 __asm__("a7") = n;
    1da2:	19000893          	li	a7,400
    __asm_syscall("r"(a7), "0"(a0))
    1da6:	00000073          	ecall

int spawn(char *file)
{
    return syscall(SYS_spawn, file);
}
    1daa:	2501                	sext.w	a0,a0
    1dac:	8082                	ret

0000000000001dae <mailread>:
    register long a7 __asm__("a7") = n;
    1dae:	19100893          	li	a7,401
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1db2:	00000073          	ecall

int mailread(void *buf, int len)
{
    return syscall(SYS_mailread, buf, len);
}
    1db6:	2501                	sext.w	a0,a0
    1db8:	8082                	ret

0000000000001dba <mailwrite>:
    register long a7 __asm__("a7") = n;
    1dba:	19200893          	li	a7,402
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1dbe:	00000073          	ecall

int mailwrite(int pid, void *buf, int len)
{
    return syscall(SYS_mailwrite, pid, buf, len);
}
    1dc2:	2501                	sext.w	a0,a0
    1dc4:	8082                	ret

0000000000001dc6 <fstat>:
    register long a7 __asm__("a7") = n;
    1dc6:	05000893          	li	a7,80
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1dca:	00000073          	ecall

int fstat(int fd, struct kstat *st)
{
    return syscall(SYS_fstat, fd, st);
}
    1dce:	2501                	sext.w	a0,a0
    1dd0:	8082                	ret

0000000000001dd2 <sys_linkat>:
    register long a4 __asm__("a4") = e;
    1dd2:	1702                	slli	a4,a4,0x20
    register long a7 __asm__("a7") = n;
    1dd4:	02500893          	li	a7,37
    register long a4 __asm__("a4") = e;
    1dd8:	9301                	srli	a4,a4,0x20
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4))
    1dda:	00000073          	ecall

int sys_linkat(int olddirfd, char *oldpath, int newdirfd, char *newpath, unsigned int flags)
{
    return syscall(SYS_linkat, olddirfd, oldpath, newdirfd, newpath, flags);
}
    1dde:	2501                	sext.w	a0,a0
    1de0:	8082                	ret

0000000000001de2 <sys_unlinkat>:
    register long a2 __asm__("a2") = c;
    1de2:	1602                	slli	a2,a2,0x20
    register long a7 __asm__("a7") = n;
    1de4:	02300893          	li	a7,35
    register long a2 __asm__("a2") = c;
    1de8:	9201                	srli	a2,a2,0x20
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1dea:	00000073          	ecall

int sys_unlinkat(int dirfd, char *path, unsigned int flags)
{
    return syscall(SYS_unlinkat, dirfd, path, flags);
}
    1dee:	2501                	sext.w	a0,a0
    1df0:	8082                	ret

0000000000001df2 <link>:

int link(char *old_path, char *new_path)
{
    1df2:	87aa                	mv	a5,a0
    1df4:	86ae                	mv	a3,a1
    register long a7 __asm__("a7") = n;
    1df6:	02500893          	li	a7,37
    register long a0 __asm__("a0") = a;
    1dfa:	f9c00513          	li	a0,-100
    register long a1 __asm__("a1") = b;
    1dfe:	85be                	mv	a1,a5
    register long a2 __asm__("a2") = c;
    1e00:	f9c00613          	li	a2,-100
    register long a4 __asm__("a4") = e;
    1e04:	4701                	li	a4,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4))
    1e06:	00000073          	ecall
    return sys_linkat(AT_FDCWD, old_path, AT_FDCWD, new_path, 0);
}
    1e0a:	2501                	sext.w	a0,a0
    1e0c:	8082                	ret

0000000000001e0e <unlink>:

int unlink(char *path)
{
    1e0e:	85aa                	mv	a1,a0
    register long a7 __asm__("a7") = n;
    1e10:	02300893          	li	a7,35
    register long a0 __asm__("a0") = a;
    1e14:	f9c00513          	li	a0,-100
    register long a2 __asm__("a2") = c;
    1e18:	4601                	li	a2,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1e1a:	00000073          	ecall
    return sys_unlinkat(AT_FDCWD, path, 0);
}
    1e1e:	2501                	sext.w	a0,a0
    1e20:	8082                	ret

0000000000001e22 <uname>:
    register long a7 __asm__("a7") = n;
    1e22:	0a000893          	li	a7,160
    __asm_syscall("r"(a7), "0"(a0))
    1e26:	00000073          	ecall

int uname(void *buf)
{
    return syscall(SYS_uname, buf);
}
    1e2a:	2501                	sext.w	a0,a0
    1e2c:	8082                	ret

0000000000001e2e <brk>:
    register long a7 __asm__("a7") = n;
    1e2e:	0d600893          	li	a7,214
    __asm_syscall("r"(a7), "0"(a0))
    1e32:	00000073          	ecall

int brk(void *addr)
{
    return syscall(SYS_brk, addr);
}
    1e36:	2501                	sext.w	a0,a0
    1e38:	8082                	ret

0000000000001e3a <getcwd>:
    register long a7 __asm__("a7") = n;
    1e3a:	48c5                	li	a7,17
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1e3c:	00000073          	ecall

char *getcwd(char *buf, size_t size){
    return syscall(SYS_getcwd, buf, size);
}
    1e40:	8082                	ret

0000000000001e42 <chdir>:
    register long a7 __asm__("a7") = n;
    1e42:	03100893          	li	a7,49
    __asm_syscall("r"(a7), "0"(a0))
    1e46:	00000073          	ecall

int chdir(const char *path){
    return syscall(SYS_chdir, path);
}
    1e4a:	2501                	sext.w	a0,a0
    1e4c:	8082                	ret

0000000000001e4e <mkdir>:

int mkdir(const char *path, mode_t mode){
    1e4e:	862e                	mv	a2,a1
    1e50:	87aa                	mv	a5,a0
    register long a2 __asm__("a2") = c;
    1e52:	1602                	slli	a2,a2,0x20
    register long a7 __asm__("a7") = n;
    1e54:	02200893          	li	a7,34
    register long a0 __asm__("a0") = a;
    1e58:	f9c00513          	li	a0,-100
    register long a1 __asm__("a1") = b;
    1e5c:	85be                	mv	a1,a5
    register long a2 __asm__("a2") = c;
    1e5e:	9201                	srli	a2,a2,0x20
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1e60:	00000073          	ecall
    return syscall(SYS_mkdirat, AT_FDCWD, path, mode);
}
    1e64:	2501                	sext.w	a0,a0
    1e66:	8082                	ret

0000000000001e68 <getdents>:
    register long a7 __asm__("a7") = n;
    1e68:	03d00893          	li	a7,61
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1e6c:	00000073          	ecall

int getdents(int fd, struct linux_dirent64 *dirp64, unsigned long len){
    //return syscall(SYS_getdents64, fd, dirp64, len);
    return syscall(SYS_getdents64, fd, dirp64, len);
}
    1e70:	2501                	sext.w	a0,a0
    1e72:	8082                	ret

0000000000001e74 <pipe>:
    register long a7 __asm__("a7") = n;
    1e74:	03b00893          	li	a7,59
    register long a1 __asm__("a1") = b;
    1e78:	4581                	li	a1,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1e7a:	00000073          	ecall

int pipe(int fd[2]){
    return syscall(SYS_pipe2, fd, 0);
}
    1e7e:	2501                	sext.w	a0,a0
    1e80:	8082                	ret

0000000000001e82 <dup>:
    register long a7 __asm__("a7") = n;
    1e82:	48dd                	li	a7,23
    __asm_syscall("r"(a7), "0"(a0))
    1e84:	00000073          	ecall

int dup(int fd){
    return syscall(SYS_dup, fd);
}
    1e88:	2501                	sext.w	a0,a0
    1e8a:	8082                	ret

0000000000001e8c <dup2>:
    register long a7 __asm__("a7") = n;
    1e8c:	48e1                	li	a7,24
    register long a2 __asm__("a2") = c;
    1e8e:	4601                	li	a2,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2))
    1e90:	00000073          	ecall

int dup2(int old, int new){
    return syscall(SYS_dup3, old, new, 0);
}
    1e94:	2501                	sext.w	a0,a0
    1e96:	8082                	ret

0000000000001e98 <mount>:
    register long a7 __asm__("a7") = n;
    1e98:	02800893          	li	a7,40
    __asm_syscall("r"(a7), "0"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4))
    1e9c:	00000073          	ecall

int mount(const char *special, const char *dir, const char *fstype, unsigned long flags, const void *data)
{
        return syscall(SYS_mount, special, dir, fstype, flags, data);
}
    1ea0:	2501                	sext.w	a0,a0
    1ea2:	8082                	ret

0000000000001ea4 <umount>:
    register long a7 __asm__("a7") = n;
    1ea4:	02700893          	li	a7,39
    register long a1 __asm__("a1") = b;
    1ea8:	4581                	li	a1,0
    __asm_syscall("r"(a7), "0"(a0), "r"(a1))
    1eaa:	00000073          	ecall

int umount(const char *special)
{
        return syscall(SYS_umount2, special, 0);
}
    1eae:	2501                	sext.w	a0,a0
    1eb0:	8082                	ret

0000000000001eb2 <__clone>:

.global __clone
.type  __clone, %function
__clone:
	# Save func and arg to stack
	addi a1, a1, -16
    1eb2:	15c1                	addi	a1,a1,-16
	sd a0, 0(a1)
    1eb4:	e188                	sd	a0,0(a1)
	sd a3, 8(a1)
    1eb6:	e594                	sd	a3,8(a1)

	# Call SYS_clone
	mv a0, a2
    1eb8:	8532                	mv	a0,a2
	mv a2, a4
    1eba:	863a                	mv	a2,a4
	mv a3, a5
    1ebc:	86be                	mv	a3,a5
	mv a4, a6
    1ebe:	8742                	mv	a4,a6
	li a7, 220 # SYS_clone
    1ec0:	0dc00893          	li	a7,220
	ecall
    1ec4:	00000073          	ecall

	beqz a0, 1f
    1ec8:	c111                	beqz	a0,1ecc <__clone+0x1a>
	# Parent
	ret
    1eca:	8082                	ret

	# Child
1:      ld a1, 0(sp)
    1ecc:	6582                	ld	a1,0(sp)
	ld a0, 8(sp)
    1ece:	6522                	ld	a0,8(sp)
	jalr a1
    1ed0:	9582                	jalr	a1

	# Exit
	li a7, 93 # SYS_exit
    1ed2:	05d00893          	li	a7,93
	ecall
    1ed6:	00000073          	ecall
