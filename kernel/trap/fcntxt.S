# 
# The floating-point registers context.
# 
# When switching process, call here to save/restore the floating-point registers.
# We don't bother to save those register during trap, since the kernel doesn't
# touch any of them. But technically speaking, this should be trap's work.
# 
# Those registers are stored in trapframes.
# 

# void floatstore(struct trapframe *tf);
.globl floatstore
floatstore:

	# Save them on trapframe, where is actully a page.
	addi sp, sp, -16
	sd s0, 8(sp)
	sd s1, 0(sp)
	addi s0, sp, 16

	fsd ft0, 288(a0)
	fsd ft1, 296(a0)
	fsd ft2, 304(a0)
	fsd ft3, 312(a0)
	fsd ft4, 320(a0)
	fsd ft5, 328(a0)
	fsd ft6, 336(a0)
	fsd ft7, 344(a0)
	fsd fs0, 352(a0)
	fsd fs1, 360(a0)
	fsd fa0, 368(a0)
	fsd fa1, 376(a0)
	fsd fa2, 384(a0)
	fsd fa3, 392(a0)
	fsd fa4, 400(a0)
	fsd fa5, 408(a0)
	fsd fa6, 416(a0)
	fsd fa7, 424(a0)
	fsd fs2, 432(a0)
	fsd fs3, 440(a0)
	fsd fs4, 448(a0)
	fsd fs5, 456(a0)
	fsd fs6, 464(a0)
	fsd fs7, 472(a0)
	fsd fs8, 480(a0)
	fsd fs9, 488(a0)
	fsd fs10, 496(a0)
	fsd fs11, 504(a0)
	fsd ft8, 512(a0)
	fsd ft9, 520(a0)
	fsd ft10, 528(a0)
	fsd ft11, 536(a0)

	frcsr s1
	sd s1, 544(a0)

	ld s0, 8(sp)
	ld s1, 0(sp)
	addi sp, sp, 16
	ret

# void floatload(struct trapframe *tf);
.globl floatload
floatload:

	addi sp, sp, -16
	sd s0, 8(sp)
	addi s0, sp, 16

	fld ft0, 288(a0)
	fld ft1, 296(a0)
	fld ft2, 304(a0)
	fld ft3, 312(a0)
	fld ft4, 320(a0)
	fld ft5, 328(a0)
	fld ft6, 336(a0)
	fld ft7, 344(a0)
	fld fs0, 352(a0)
	fld fs1, 360(a0)
	fld fa0, 368(a0)
	fld fa1, 376(a0)
	fld fa2, 384(a0)
	fld fa3, 392(a0)
	fld fa4, 400(a0)
	fld fa5, 408(a0)
	fld fa6, 416(a0)
	fld fa7, 424(a0)
	fld fs2, 432(a0)
	fld fs3, 440(a0)
	fld fs4, 448(a0)
	fld fs5, 456(a0)
	fld fs6, 464(a0)
	fld fs7, 472(a0)
	fld fs8, 480(a0)
	fld fs9, 488(a0)
	fld fs10, 496(a0)
	fld fs11, 504(a0)
	fld ft8, 512(a0)
	fld ft9, 520(a0)
	fld ft10, 528(a0)
	fld ft11, 536(a0)

	ld a0, 544(a0)
	fscsr a0

	ld s0, 8(sp)
	addi sp, sp, 16
	ret
