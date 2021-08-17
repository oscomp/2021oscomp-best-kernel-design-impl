
	.globl floatstore 
floatstore: 
	addi	sp,sp,-16
	sd	s0,8(sp)
	sd	s1,0(sp)
	addi	s0,sp,16

	.word 	0x12053027
	.word 	0x12153427
	.word 	0x12253827
	.word 	0x12353c27
	.word 	0x14453027
	.word 	0x14553427
	.word 	0x14653827
	.word 	0x14753c27
	.word 	0x16853027
	.word 	0x16953427
	.word 	0x16a53827
	.word 	0x16b53c27
	.word 	0x18c53027
	.word 	0x18d53427
	.word 	0x18e53827
	.word 	0x18f53c27
	.word 	0x1b053027
	.word 	0x1b153427
	.word 	0x1b253827
	.word 	0x1b353c27
	.word 	0x1d453027
	.word 	0x1d553427
	.word 	0x1d653827
	.word 	0x1d753c27
	.word 	0x1f853027
	.word 	0x1f953427
	.word 	0x1fa53827
	.word 	0x1fb53c27
	.word 	0x21c53027
	.word 	0x21d53427
	.word 	0x21e53827
	.word 	0x21f53c27
  	
	frcsr	s1
  	sd	s1,544(a0)
	
	ld	s0,8(sp)
	ld	s1,0(sp)
	addi	sp,sp,16
	
	ret

	.globl floatload
floatload: 
	addi	sp,sp,-16
	sd	s0,8(sp)
	addi	s0,sp,16

	.word 	0x12053007
	.word 	0x12853087
	.word 	0x13053107
	.word 	0x13853187
	.word 	0x14053207
	.word 	0x14853287
	.word 	0x15053307
	.word 	0x15853387
	.word 	0x16053407
	.word 	0x16853487
	.word 	0x17053507
	.word 	0x17853587
	.word 	0x18053607
	.word 	0x18853687
	.word 	0x19053707
	.word 	0x19853787
	.word 	0x1a053807
	.word 	0x1a853887
	.word 	0x1b053907
	.word 	0x1b853987
	.word 	0x1c053a07
	.word 	0x1c853a87
	.word 	0x1d053b07
	.word 	0x1d853b87
	.word 	0x1e053c07
	.word 	0x1e853c87
	.word 	0x1f053d07
	.word 	0x1f853d87
	.word 	0x20053e07
	.word 	0x20853e87
	.word 	0x21053f07
	.word 	0x21853f87
	
	ld	a0,544(a0)
	fscsr	a0
	
	ld	s0,8(sp)
	addi	sp,sp,16
	ret
