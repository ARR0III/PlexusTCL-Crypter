	.file	"crycon.c"
	.intel_syntax noprefix
	.text
	.globl	PARAM_READ_BYTE
	.section	.rodata
.LC0:
	.string	"rb"
	.section	.data.rel.local,"aw"
	.align 4
	.type	PARAM_READ_BYTE, @object
	.size	PARAM_READ_BYTE, 4
PARAM_READ_BYTE:
	.long	.LC0
	.globl	PARAM_WRITE_BYTE
	.section	.rodata
.LC1:
	.string	"wb"
	.section	.data.rel.local
	.align 4
	.type	PARAM_WRITE_BYTE, @object
	.size	PARAM_WRITE_BYTE, 4
PARAM_WRITE_BYTE:
	.long	.LC1
	.globl	PROGRAMM_NAME
	.section	.rodata
	.align 4
.LC2:
	.string	"PlexusTCL Console Crypter 5.08 03SEP23 [EN]"
	.section	.data.rel.local
	.align 4
	.type	PROGRAMM_NAME, @object
	.size	PROGRAMM_NAME, 4
PROGRAMM_NAME:
	.long	.LC2
	.local	rijndael_ctx
	.comm	rijndael_ctx,4,4
	.local	serpent_ctx
	.comm	serpent_ctx,4,4
	.local	twofish_ctx
	.comm	twofish_ctx,4,4
	.local	blowfish_ctx
	.comm	blowfish_ctx,4,4
	.local	threefish_ctx
	.comm	threefish_ctx,4,4
	.section	.rodata
	.align 4
	.type	INT_SIZE_DATA, @object
	.size	INT_SIZE_DATA, 12
INT_SIZE_DATA:
	.long	1024
	.long	1048576
	.long	1073741824
.LC3:
	.string	"Bt"
.LC4:
	.string	"KiB"
.LC5:
	.string	"MiB"
.LC6:
	.string	"GiB"
	.section	.data.rel.local
	.align 4
	.type	CHAR_SIZE_DATA, @object
	.size	CHAR_SIZE_DATA, 16
CHAR_SIZE_DATA:
	.long	.LC3
	.long	.LC4
	.long	.LC5
	.long	.LC6
	.section	.rodata
.LC7:
	.string	"Encrypt"
.LC8:
	.string	"Decrypt"
	.section	.data.rel.local
	.align 4
	.type	OPERATION_NAME, @object
	.size	OPERATION_NAME, 8
OPERATION_NAME:
	.long	.LC7
	.long	.LC8
	.section	.rodata
.LC9:
	.string	"AES"
.LC10:
	.string	"SERPENT"
.LC11:
	.string	"TWOFISH"
.LC12:
	.string	"BLOWFISH"
.LC13:
	.string	"THREEFISH"
	.section	.data.rel.local
	.align 4
	.type	ALGORITM_NAME, @object
	.size	ALGORITM_NAME, 20
ALGORITM_NAME:
	.long	.LC9
	.long	.LC10
	.long	.LC11
	.long	.LC12
	.long	.LC13
	.text
	.type	free_global_memory, @function
free_global_memory:
.LFB6:
	.cfi_startproc
	push	ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	mov	ebp, esp
	.cfi_def_cfa_register 5
	push	ebx
	sub	esp, 4
	.cfi_offset 3, -12
	call	__x86.get_pc_thunk.bx
	add	ebx, OFFSET FLAT:_GLOBAL_OFFSET_TABLE_
	cmp	DWORD PTR 8[ebp], 0
	je	.L10
	mov	eax, DWORD PTR 8[ebp]
	mov	eax, DWORD PTR 12[eax]
	test	eax, eax
	je	.L4
	mov	eax, DWORD PTR 8[ebp]
	mov	eax, DWORD PTR 16[eax]
	test	eax, eax
	je	.L5
	mov	eax, DWORD PTR 8[ebp]
	mov	edx, DWORD PTR 16[eax]
	mov	eax, DWORD PTR 8[ebp]
	mov	eax, DWORD PTR 12[eax]
	sub	esp, 4
	push	edx
	push	0
	push	eax
	call	meminit@PLT
	add	esp, 16
.L5:
	mov	eax, DWORD PTR 8[ebp]
	mov	eax, DWORD PTR 12[eax]
	sub	esp, 12
	push	eax
	call	free@PLT
	add	esp, 16
.L4:
	mov	eax, DWORD PTR 8[ebp]
	mov	eax, DWORD PTR 20[eax]
	test	eax, eax
	je	.L6
	mov	eax, DWORD PTR 8[ebp]
	mov	eax, DWORD PTR 24[eax]
	test	eax, eax
	je	.L7
	mov	eax, DWORD PTR 8[ebp]
	mov	edx, DWORD PTR 24[eax]
	mov	eax, DWORD PTR 8[ebp]
	mov	eax, DWORD PTR 20[eax]
	sub	esp, 4
	push	edx
	push	0
	push	eax
	call	meminit@PLT
	add	esp, 16
.L7:
	mov	eax, DWORD PTR 8[ebp]
	mov	eax, DWORD PTR 20[eax]
	sub	esp, 12
	push	eax
	call	free@PLT
	add	esp, 16
.L6:
	mov	eax, DWORD PTR 8[ebp]
	mov	eax, DWORD PTR 28[eax]
	test	eax, eax
	je	.L8
	mov	eax, DWORD PTR 8[ebp]
	mov	eax, DWORD PTR 32[eax]
	test	eax, eax
	je	.L9
	mov	eax, DWORD PTR 8[ebp]
	mov	edx, DWORD PTR 32[eax]
	mov	eax, DWORD PTR 8[ebp]
	mov	eax, DWORD PTR 28[eax]
	sub	esp, 4
	push	edx
	push	0
	push	eax
	call	meminit@PLT
	add	esp, 16
.L9:
	mov	eax, DWORD PTR 8[ebp]
	mov	eax, DWORD PTR 28[eax]
	sub	esp, 12
	push	eax
	call	free@PLT
	add	esp, 16
.L8:
	sub	esp, 4
	push	DWORD PTR 12[ebp]
	push	0
	push	DWORD PTR 8[ebp]
	call	meminit@PLT
	add	esp, 16
	sub	esp, 12
	push	DWORD PTR 8[ebp]
	call	free@PLT
	add	esp, 16
	jmp	.L1
.L10:
	nop
.L1:
	mov	ebx, DWORD PTR -4[ebp]
	leave
	.cfi_restore 5
	.cfi_restore 3
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE6:
	.size	free_global_memory, .-free_global_memory
	.type	size_check, @function
size_check:
.LFB7:
	.cfi_startproc
	push	ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	mov	ebp, esp
	.cfi_def_cfa_register 5
	sub	esp, 16
	call	__x86.get_pc_thunk.ax
	add	eax, OFFSET FLAT:_GLOBAL_OFFSET_TABLE_
	mov	DWORD PTR -4[ebp], 0
	mov	eax, 1024
	cmp	DWORD PTR 8[ebp], eax
	jb	.L12
	mov	eax, 1048576
	cmp	DWORD PTR 8[ebp], eax
	jnb	.L12
	mov	DWORD PTR -4[ebp], 1
	jmp	.L13
.L12:
	mov	eax, 1048576
	cmp	DWORD PTR 8[ebp], eax
	jb	.L14
	mov	eax, 1073741824
	cmp	DWORD PTR 8[ebp], eax
	jnb	.L14
	mov	DWORD PTR -4[ebp], 2
	jmp	.L13
.L14:
	mov	eax, 1073741824
	cmp	DWORD PTR 8[ebp], eax
	jb	.L13
	mov	DWORD PTR -4[ebp], 3
.L13:
	mov	eax, DWORD PTR -4[ebp]
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE7:
	.size	size_check, .-size_check
	.type	sizetodoubleprint, @function
sizetodoubleprint:
.LFB8:
	.cfi_startproc
	push	ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	mov	ebp, esp
	.cfi_def_cfa_register 5
	sub	esp, 16
	call	__x86.get_pc_thunk.ax
	add	eax, OFFSET FLAT:_GLOBAL_OFFSET_TABLE_
	mov	edx, DWORD PTR 12[ebp]
	mov	DWORD PTR -8[ebp], edx
	mov	edx, DWORD PTR 16[ebp]
	mov	DWORD PTR -4[ebp], edx
	cmp	DWORD PTR 8[ebp], 0
	je	.L17
	mov	edx, DWORD PTR 8[ebp]
	sub	edx, 1
	mov	eax, DWORD PTR INT_SIZE_DATA@GOTOFF[eax+edx*4]
	mov	DWORD PTR -16[ebp], eax
	mov	DWORD PTR -12[ebp], 0
	fild	QWORD PTR -16[ebp]
	fld	QWORD PTR -8[ebp]
	fdivrp	st(1), st
	jmp	.L19
.L17:
	fld	QWORD PTR -8[ebp]
.L19:
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE8:
	.size	sizetodoubleprint, .-sizetodoubleprint
	.type	KDFCLOMUL, @function
KDFCLOMUL:
.LFB9:
	.cfi_startproc
	push	ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	mov	ebp, esp
	.cfi_def_cfa_register 5
	push	ebx
	sub	esp, 20
	.cfi_offset 3, -12
	call	__x86.get_pc_thunk.bx
	add	ebx, OFFSET FLAT:_GLOBAL_OFFSET_TABLE_
	mov	DWORD PTR -24[ebp], 0
	cmp	DWORD PTR 8[ebp], 0
	je	.L31
	cmp	DWORD PTR 12[ebp], 0
	je	.L31
	cmp	DWORD PTR 20[ebp], 0
	je	.L31
	mov	DWORD PTR -12[ebp], 1
	jmp	.L24
.L25:
	sub	esp, 8
	push	DWORD PTR -12[ebp]
	push	DWORD PTR 12[ebp]
	call	CRC32@PLT
	add	esp, 16
	add	eax, 1
	xor	DWORD PTR -24[ebp], eax
	mov	edx, DWORD PTR 16[ebp]
	mov	eax, DWORD PTR 24[ebp]
	add	edx, eax
	mov	eax, DWORD PTR -12[ebp]
	add	edx, eax
	mov	eax, DWORD PTR -24[ebp]
	sub	eax, edx
	sub	eax, 1
	mov	DWORD PTR -24[ebp], eax
	add	DWORD PTR -12[ebp], 1
.L24:
	mov	eax, DWORD PTR -12[ebp]
	cmp	eax, DWORD PTR 16[ebp]
	jbe	.L25
	sub	esp, 8
	push	DWORD PTR 16[ebp]
	push	DWORD PTR 12[ebp]
	call	CRC32@PLT
	add	esp, 16
	and	DWORD PTR -24[ebp], eax
	shr	DWORD PTR -24[ebp], 18
	or	DWORD PTR -24[ebp], 16384
	mov	eax, DWORD PTR 8[ebp]
	mov	eax, DWORD PTR 12[eax]
	sub	esp, 12
	push	eax
	call	sha256_init@PLT
	add	esp, 16
	mov	DWORD PTR -20[ebp], 0
	mov	eax, DWORD PTR -20[ebp]
	mov	DWORD PTR -12[ebp], eax
	jmp	.L26
.L30:
	mov	DWORD PTR -16[ebp], 0
	jmp	.L27
.L28:
	mov	eax, DWORD PTR 8[ebp]
	mov	eax, DWORD PTR 12[eax]
	sub	esp, 4
	push	DWORD PTR 16[ebp]
	push	DWORD PTR 12[ebp]
	push	eax
	call	sha256_update@PLT
	add	esp, 16
	add	DWORD PTR -16[ebp], 1
.L27:
	mov	eax, DWORD PTR -16[ebp]
	cmp	eax, DWORD PTR -24[ebp]
	jb	.L28
	mov	eax, DWORD PTR 8[ebp]
	mov	eax, DWORD PTR 12[eax]
	sub	esp, 12
	push	eax
	call	sha256_final@PLT
	add	esp, 16
	cmp	DWORD PTR -20[ebp], 31
	jbe	.L29
	mov	DWORD PTR -20[ebp], 0
.L29:
	mov	eax, DWORD PTR 8[ebp]
	mov	ecx, DWORD PTR 12[eax]
	mov	edx, DWORD PTR 20[ebp]
	mov	eax, DWORD PTR -12[ebp]
	add	edx, eax
	mov	eax, DWORD PTR -20[ebp]
	add	eax, ecx
	add	eax, 108
	movzx	eax, BYTE PTR [eax]
	mov	BYTE PTR [edx], al
	add	DWORD PTR -12[ebp], 1
	add	DWORD PTR -20[ebp], 1
.L26:
	mov	eax, DWORD PTR -12[ebp]
	cmp	eax, DWORD PTR 24[ebp]
	jb	.L30
	mov	eax, DWORD PTR 8[ebp]
	mov	edx, DWORD PTR 16[eax]
	mov	eax, DWORD PTR 8[ebp]
	mov	eax, DWORD PTR 12[eax]
	sub	esp, 4
	push	edx
	push	0
	push	eax
	call	meminit@PLT
	add	esp, 16
	jmp	.L20
.L31:
	nop
.L20:
	mov	ebx, DWORD PTR -4[ebp]
	leave
	.cfi_restore 5
	.cfi_restore 3
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE9:
	.size	KDFCLOMUL, .-KDFCLOMUL
	.type	operation_variant, @function
operation_variant:
.LFB10:
	.cfi_startproc
	push	ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	mov	ebp, esp
	.cfi_def_cfa_register 5
	call	__x86.get_pc_thunk.ax
	add	eax, OFFSET FLAT:_GLOBAL_OFFSET_TABLE_
	cmp	DWORD PTR 8[ebp], 0
	setne	al
	movzx	eax, al
	pop	ebp
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE10:
	.size	operation_variant, .-operation_variant
	.type	size_of_file, @function
size_of_file:
.LFB11:
	.cfi_startproc
	push	ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	mov	ebp, esp
	.cfi_def_cfa_register 5
	push	ebx
	sub	esp, 20
	.cfi_offset 3, -12
	call	__x86.get_pc_thunk.bx
	add	ebx, OFFSET FLAT:_GLOBAL_OFFSET_TABLE_
	cmp	DWORD PTR 8[ebp], 0
	jne	.L35
	mov	eax, -1
	jmp	.L36
.L35:
	sub	esp, 4
	push	2
	push	0
	push	DWORD PTR 8[ebp]
	call	fseek@PLT
	add	esp, 16
	test	eax, eax
	je	.L37
	mov	eax, -1
	jmp	.L36
.L37:
	sub	esp, 12
	push	DWORD PTR 8[ebp]
	call	ftell@PLT
	add	esp, 16
	mov	DWORD PTR -12[ebp], eax
	sub	esp, 4
	push	0
	push	0
	push	DWORD PTR 8[ebp]
	call	fseek@PLT
	add	esp, 16
	test	eax, eax
	je	.L38
	mov	eax, -1
	jmp	.L36
.L38:
	mov	eax, DWORD PTR -12[ebp]
.L36:
	mov	ebx, DWORD PTR -4[ebp]
	leave
	.cfi_restore 5
	.cfi_restore 3
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE11:
	.size	size_of_file, .-size_of_file
	.type	cipher_free, @function
cipher_free:
.LFB12:
	.cfi_startproc
	push	ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	mov	ebp, esp
	.cfi_def_cfa_register 5
	push	ebx
	sub	esp, 4
	.cfi_offset 3, -12
	call	__x86.get_pc_thunk.bx
	add	ebx, OFFSET FLAT:_GLOBAL_OFFSET_TABLE_
	cmp	DWORD PTR 8[ebp], 0
	je	.L42
	sub	esp, 4
	push	DWORD PTR 12[ebp]
	push	0
	push	DWORD PTR 8[ebp]
	call	meminit@PLT
	add	esp, 16
	sub	esp, 12
	push	DWORD PTR 8[ebp]
	call	free@PLT
	add	esp, 16
	jmp	.L39
.L42:
	nop
.L39:
	mov	ebx, DWORD PTR -4[ebp]
	leave
	.cfi_restore 5
	.cfi_restore 3
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE12:
	.size	cipher_free, .-cipher_free
	.type	hmac_sha256_uf, @function
hmac_sha256_uf:
.LFB13:
	.cfi_startproc
	push	ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	mov	ebp, esp
	.cfi_def_cfa_register 5
	push	ebx
	sub	esp, 20
	.cfi_offset 3, -12
	call	__x86.get_pc_thunk.bx
	add	ebx, OFFSET FLAT:_GLOBAL_OFFSET_TABLE_
	cmp	DWORD PTR 8[ebp], 0
	je	.L51
	mov	DWORD PTR -16[ebp], 96
	sub	esp, 12
	push	DWORD PTR -16[ebp]
	call	malloc@PLT
	add	esp, 16
	mov	DWORD PTR -20[ebp], eax
	cmp	DWORD PTR -20[ebp], 0
	je	.L52
	mov	eax, DWORD PTR 8[ebp]
	mov	eax, DWORD PTR 32[eax]
	mov	edx, 32
	cmp	eax, 32
	cmova	eax, edx
	mov	DWORD PTR -24[ebp], eax
	mov	eax, DWORD PTR 8[ebp]
	mov	eax, DWORD PTR 12[eax]
	lea	edx, 108[eax]
	mov	eax, DWORD PTR -20[ebp]
	sub	esp, 4
	push	32
	push	edx
	push	eax
	call	memcpy@PLT
	add	esp, 16
	mov	eax, DWORD PTR 8[ebp]
	mov	eax, DWORD PTR 28[eax]
	mov	edx, DWORD PTR -20[ebp]
	add	edx, 32
	sub	esp, 4
	push	DWORD PTR -24[ebp]
	push	eax
	push	edx
	call	memcpy@PLT
	add	esp, 16
	mov	eax, DWORD PTR 8[ebp]
	mov	eax, DWORD PTR 28[eax]
	mov	edx, DWORD PTR -20[ebp]
	add	edx, 64
	sub	esp, 4
	push	DWORD PTR -24[ebp]
	push	eax
	push	edx
	call	memcpy@PLT
	add	esp, 16
	mov	eax, DWORD PTR -24[ebp]
	mov	DWORD PTR -12[ebp], eax
	jmp	.L47
.L48:
	mov	edx, DWORD PTR -20[ebp]
	mov	eax, DWORD PTR -12[ebp]
	add	eax, edx
	add	eax, 32
	mov	BYTE PTR [eax], 0
	mov	edx, DWORD PTR -20[ebp]
	mov	eax, DWORD PTR -12[ebp]
	add	eax, edx
	add	eax, 64
	mov	BYTE PTR [eax], 0
	add	DWORD PTR -12[ebp], 1
.L47:
	cmp	DWORD PTR -12[ebp], 31
	jbe	.L48
	mov	DWORD PTR -12[ebp], 0
	jmp	.L49
.L50:
	mov	edx, DWORD PTR -20[ebp]
	mov	eax, DWORD PTR -12[ebp]
	add	eax, edx
	add	eax, 32
	movzx	eax, BYTE PTR [eax]
	xor	eax, 85
	mov	edx, eax
	mov	ecx, DWORD PTR -20[ebp]
	mov	eax, DWORD PTR -12[ebp]
	add	eax, ecx
	add	eax, 32
	mov	BYTE PTR [eax], dl
	mov	edx, DWORD PTR -20[ebp]
	mov	eax, DWORD PTR -12[ebp]
	add	eax, edx
	add	eax, 64
	movzx	eax, BYTE PTR [eax]
	xor	eax, 102
	mov	edx, eax
	mov	ecx, DWORD PTR -20[ebp]
	mov	eax, DWORD PTR -12[ebp]
	add	eax, ecx
	add	eax, 64
	mov	BYTE PTR [eax], dl
	add	DWORD PTR -12[ebp], 1
.L49:
	cmp	DWORD PTR -12[ebp], 31
	jbe	.L50
	mov	eax, DWORD PTR 8[ebp]
	mov	edx, DWORD PTR 16[eax]
	mov	eax, DWORD PTR 8[ebp]
	mov	eax, DWORD PTR 12[eax]
	sub	esp, 4
	push	edx
	push	0
	push	eax
	call	meminit@PLT
	add	esp, 16
	mov	eax, DWORD PTR 8[ebp]
	mov	eax, DWORD PTR 12[eax]
	sub	esp, 12
	push	eax
	call	sha256_init@PLT
	add	esp, 16
	mov	eax, DWORD PTR -20[ebp]
	lea	edx, 32[eax]
	mov	eax, DWORD PTR 8[ebp]
	mov	eax, DWORD PTR 12[eax]
	sub	esp, 4
	push	32
	push	edx
	push	eax
	call	sha256_update@PLT
	add	esp, 16
	mov	edx, DWORD PTR -20[ebp]
	mov	eax, DWORD PTR 8[ebp]
	mov	eax, DWORD PTR 12[eax]
	sub	esp, 4
	push	32
	push	edx
	push	eax
	call	sha256_update@PLT
	add	esp, 16
	mov	eax, DWORD PTR 8[ebp]
	mov	eax, DWORD PTR 12[eax]
	sub	esp, 12
	push	eax
	call	sha256_final@PLT
	add	esp, 16
	mov	eax, DWORD PTR 8[ebp]
	mov	eax, DWORD PTR 12[eax]
	lea	edx, 108[eax]
	mov	eax, DWORD PTR -20[ebp]
	sub	esp, 4
	push	32
	push	edx
	push	eax
	call	memcpy@PLT
	add	esp, 16
	mov	eax, DWORD PTR 8[ebp]
	mov	edx, DWORD PTR 16[eax]
	mov	eax, DWORD PTR 8[ebp]
	mov	eax, DWORD PTR 12[eax]
	sub	esp, 4
	push	edx
	push	0
	push	eax
	call	meminit@PLT
	add	esp, 16
	mov	eax, DWORD PTR 8[ebp]
	mov	eax, DWORD PTR 12[eax]
	sub	esp, 12
	push	eax
	call	sha256_init@PLT
	add	esp, 16
	mov	eax, DWORD PTR -20[ebp]
	lea	edx, 64[eax]
	mov	eax, DWORD PTR 8[ebp]
	mov	eax, DWORD PTR 12[eax]
	sub	esp, 4
	push	32
	push	edx
	push	eax
	call	sha256_update@PLT
	add	esp, 16
	mov	edx, DWORD PTR -20[ebp]
	mov	eax, DWORD PTR 8[ebp]
	mov	eax, DWORD PTR 12[eax]
	sub	esp, 4
	push	32
	push	edx
	push	eax
	call	sha256_update@PLT
	add	esp, 16
	mov	eax, DWORD PTR 8[ebp]
	mov	eax, DWORD PTR 12[eax]
	sub	esp, 12
	push	eax
	call	sha256_final@PLT
	add	esp, 16
	sub	esp, 4
	push	DWORD PTR -16[ebp]
	push	0
	push	DWORD PTR -20[ebp]
	call	meminit@PLT
	add	esp, 16
	sub	esp, 12
	push	DWORD PTR -20[ebp]
	call	free@PLT
	add	esp, 16
	jmp	.L43
.L51:
	nop
	jmp	.L43
.L52:
	nop
.L43:
	mov	ebx, DWORD PTR -4[ebp]
	leave
	.cfi_restore 5
	.cfi_restore 3
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE13:
	.size	hmac_sha256_uf, .-hmac_sha256_uf
	.type	control_sum_buffer, @function
control_sum_buffer:
.LFB14:
	.cfi_startproc
	push	ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	mov	ebp, esp
	.cfi_def_cfa_register 5
	push	ebx
	sub	esp, 20
	.cfi_offset 3, -12
	call	__x86.get_pc_thunk.bx
	add	ebx, OFFSET FLAT:_GLOBAL_OFFSET_TABLE_
	mov	DWORD PTR -12[ebp], 0
	mov	eax, DWORD PTR 12[ebp]
	mov	DWORD PTR -16[ebp], eax
	cmp	DWORD PTR 8[ebp], 0
	je	.L64
	jmp	.L56
.L63:
	cmp	DWORD PTR -16[ebp], 1023
	ja	.L57
	mov	eax, DWORD PTR 8[ebp]
	mov	eax, DWORD PTR 16777280[eax]
	test	eax, eax
	je	.L58
	mov	eax, DWORD PTR 8[ebp]
	add	eax, 8388644
	jmp	.L59
.L58:
	mov	eax, DWORD PTR 8[ebp]
	add	eax, 36
.L59:
	mov	edx, DWORD PTR -12[ebp]
	add	edx, eax
	mov	eax, DWORD PTR 8[ebp]
	mov	eax, DWORD PTR 12[eax]
	sub	esp, 4
	push	DWORD PTR -16[ebp]
	push	edx
	push	eax
	call	sha256_update@PLT
	add	esp, 16
	jmp	.L60
.L57:
	mov	eax, DWORD PTR 8[ebp]
	mov	eax, DWORD PTR 16777280[eax]
	test	eax, eax
	je	.L61
	mov	eax, DWORD PTR 8[ebp]
	add	eax, 8388644
	jmp	.L62
.L61:
	mov	eax, DWORD PTR 8[ebp]
	add	eax, 36
.L62:
	mov	edx, DWORD PTR -12[ebp]
	add	edx, eax
	mov	eax, DWORD PTR 8[ebp]
	mov	eax, DWORD PTR 12[eax]
	sub	esp, 4
	push	1024
	push	edx
	push	eax
	call	sha256_update@PLT
	add	esp, 16
.L60:
	add	DWORD PTR -12[ebp], 1024
	sub	DWORD PTR -16[ebp], 1024
.L56:
	mov	eax, DWORD PTR -12[ebp]
	cmp	eax, DWORD PTR 12[ebp]
	jb	.L63
	jmp	.L53
.L64:
	nop
.L53:
	mov	ebx, DWORD PTR -4[ebp]
	leave
	.cfi_restore 5
	.cfi_restore 3
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE14:
	.size	control_sum_buffer, .-control_sum_buffer
	.type	close_in_out_files, @function
close_in_out_files:
.LFB15:
	.cfi_startproc
	push	ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	mov	ebp, esp
	.cfi_def_cfa_register 5
	push	ebx
	sub	esp, 4
	.cfi_offset 3, -12
	call	__x86.get_pc_thunk.bx
	add	ebx, OFFSET FLAT:_GLOBAL_OFFSET_TABLE_
	sub	esp, 12
	push	DWORD PTR 8[ebp]
	call	fclose@PLT
	add	esp, 16
	cmp	eax, -1
	jne	.L66
	sub	esp, 12
	push	DWORD PTR 12[ebp]
	call	fclose@PLT
	add	esp, 16
	mov	eax, -6
	jmp	.L67
.L66:
	sub	esp, 12
	push	DWORD PTR 12[ebp]
	call	fclose@PLT
	add	esp, 16
	cmp	eax, -1
	jne	.L68
	mov	eax, -7
	jmp	.L67
.L68:
	mov	eax, DWORD PTR 16[ebp]
.L67:
	mov	ebx, DWORD PTR -4[ebp]
	leave
	.cfi_restore 5
	.cfi_restore 3
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE15:
	.size	close_in_out_files, .-close_in_out_files
	.section	.rodata
	.align 4
.LC16:
	.string	"\r >  %s [%s] (%4.2f %s/%4.2f %s) %3d %%  "
	.align 4
.LC17:
	.string	"[!] WARNING: Control sum file \"%s\" not correct!\n"
	.align 4
.LC18:
	.string	"[#] Control sum file \"%s\" check! :)\n"
	.text
	.type	filecrypt, @function
filecrypt:
.LFB16:
	.cfi_startproc
	push	ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	mov	ebp, esp
	.cfi_def_cfa_register 5
	push	edi
	push	esi
	push	ebx
	sub	esp, 92
	.cfi_offset 7, -12
	.cfi_offset 6, -16
	.cfi_offset 3, -20
	call	__x86.get_pc_thunk.bx
	add	ebx, OFFSET FLAT:_GLOBAL_OFFSET_TABLE_
	mov	DWORD PTR -52[ebp], 0
	mov	DWORD PTR -56[ebp], 0
	mov	DWORD PTR -60[ebp], 0
	mov	DWORD PTR -32[ebp], 0
	mov	DWORD PTR -40[ebp], 0
	mov	DWORD PTR -48[ebp], 0
	mov	edx, DWORD PTR PARAM_READ_BYTE@GOTOFF[ebx]
	mov	eax, DWORD PTR 8[ebp]
	mov	eax, DWORD PTR [eax]
	sub	esp, 8
	push	edx
	push	eax
	call	fopen@PLT
	add	esp, 16
	mov	DWORD PTR -52[ebp], eax
	cmp	DWORD PTR -52[ebp], 0
	jne	.L70
	mov	eax, -1
	jmp	.L71
.L70:
	mov	edx, DWORD PTR PARAM_WRITE_BYTE@GOTOFF[ebx]
	mov	eax, DWORD PTR 8[ebp]
	mov	eax, DWORD PTR 4[eax]
	sub	esp, 8
	push	edx
	push	eax
	call	fopen@PLT
	add	esp, 16
	mov	DWORD PTR -56[ebp], eax
	cmp	DWORD PTR -56[ebp], 0
	jne	.L72
	sub	esp, 12
	push	DWORD PTR -52[ebp]
	call	fclose@PLT
	add	esp, 16
	cmp	eax, -1
	jne	.L73
	mov	eax, -6
	jmp	.L71
.L73:
	mov	eax, -2
	jmp	.L71
.L72:
	sub	esp, 12
	push	DWORD PTR -52[ebp]
	call	size_of_file
	add	esp, 16
	mov	DWORD PTR -44[ebp], eax
	mov	eax, DWORD PTR 8[ebp]
	mov	eax, DWORD PTR 16777280[eax]
	test	eax, eax
	jne	.L74
	mov	eax, DWORD PTR 8[ebp]
	mov	edx, DWORD PTR 24[eax]
	mov	eax, DWORD PTR -44[ebp]
	add	eax, edx
	add	eax, 32
	mov	DWORD PTR -44[ebp], eax
.L74:
	cmp	DWORD PTR -44[ebp], 0
	jg	.L75
	sub	esp, 4
	push	-3
	push	DWORD PTR -56[ebp]
	push	DWORD PTR -52[ebp]
	call	close_in_out_files
	add	esp, 16
	jmp	.L71
.L75:
	mov	eax, DWORD PTR 8[ebp]
	mov	eax, DWORD PTR 16777280[eax]
	test	eax, eax
	jne	.L76
	mov	edx, DWORD PTR -44[ebp]
	mov	eax, DWORD PTR 8[ebp]
	mov	ecx, DWORD PTR 24[eax]
	mov	eax, edx
	sub	eax, ecx
	sub	eax, 32
	mov	DWORD PTR -44[ebp], eax
.L76:
	mov	eax, DWORD PTR 8[ebp]
	mov	eax, DWORD PTR 16777280[eax]
	cmp	eax, 222
	jne	.L77
	mov	eax, DWORD PTR 8[ebp]
	mov	eax, DWORD PTR 24[eax]
	add	eax, 33
	cmp	DWORD PTR -44[ebp], eax
	jge	.L77
	sub	esp, 4
	push	-8
	push	DWORD PTR -56[ebp]
	push	DWORD PTR -52[ebp]
	call	close_in_out_files
	add	esp, 16
	jmp	.L71
.L77:
	fild	DWORD PTR -44[ebp]
	fld	QWORD PTR .LC14@GOTOFF[ebx]
	fdivp	st(1), st
	fstp	QWORD PTR -72[ebp]
	mov	eax, DWORD PTR -44[ebp]
	sub	esp, 12
	push	eax
	call	size_check
	add	esp, 16
	mov	DWORD PTR -76[ebp], eax
	fild	DWORD PTR -44[ebp]
	sub	esp, 4
	lea	esp, -8[esp]
	fstp	QWORD PTR [esp]
	push	DWORD PTR -76[ebp]
	call	sizetodoubleprint
	add	esp, 16
	fstp	QWORD PTR -88[ebp]
	mov	eax, DWORD PTR 8[ebp]
	add	eax, 16777252
	sub	esp, 4
	push	25
	push	46
	push	eax
	call	meminit@PLT
	add	esp, 16
	mov	eax, DWORD PTR 8[ebp]
	mov	eax, DWORD PTR 12[eax]
	sub	esp, 12
	push	eax
	call	sha256_init@PLT
	add	esp, 16
	jmp	.L78
.L104:
	cmp	DWORD PTR -48[ebp], 0
	jne	.L79
	mov	eax, DWORD PTR 8[ebp]
	mov	eax, DWORD PTR 16777280[eax]
	test	eax, eax
	jne	.L80
	mov	eax, DWORD PTR 8[ebp]
	mov	eax, DWORD PTR 16777284[eax]
	cmp	eax, 4
	ja	.L81
	sal	eax, 2
	mov	eax, DWORD PTR .L83@GOTOFF[eax+ebx]
	add	eax, ebx
	jmp	eax
	.section	.rodata
	.align 4
	.align 4
.L83:
	.long	.L87@GOTOFF
	.long	.L86@GOTOFF
	.long	.L85@GOTOFF
	.long	.L84@GOTOFF
	.long	.L82@GOTOFF
	.text
.L87:
	mov	eax, DWORD PTR 8[ebp]
	lea	ecx, 8388644[eax]
	mov	eax, DWORD PTR 8[ebp]
	mov	edx, DWORD PTR 20[eax]
	mov	eax, DWORD PTR rijndael_ctx@GOTOFF[ebx]
	sub	esp, 4
	push	ecx
	push	edx
	push	eax
	call	rijndael_encrypt@PLT
	add	esp, 16
	jmp	.L81
.L86:
	mov	eax, DWORD PTR 8[ebp]
	lea	ecx, 8388644[eax]
	mov	eax, DWORD PTR 8[ebp]
	mov	edx, DWORD PTR 20[eax]
	mov	eax, DWORD PTR serpent_ctx@GOTOFF[ebx]
	sub	esp, 4
	push	ecx
	push	edx
	push	eax
	call	serpent_encrypt@PLT
	add	esp, 16
	jmp	.L81
.L85:
	mov	eax, DWORD PTR 8[ebp]
	lea	ecx, 8388644[eax]
	mov	eax, DWORD PTR 8[ebp]
	mov	edx, DWORD PTR 20[eax]
	mov	eax, DWORD PTR twofish_ctx@GOTOFF[ebx]
	sub	esp, 4
	push	ecx
	push	edx
	push	eax
	call	twofish_encrypt@PLT
	add	esp, 16
	jmp	.L81
.L84:
	mov	eax, DWORD PTR 8[ebp]
	mov	edx, DWORD PTR 24[eax]
	mov	eax, DWORD PTR 8[ebp]
	mov	eax, DWORD PTR 20[eax]
	mov	ecx, DWORD PTR 8[ebp]
	add	ecx, 8388644
	sub	esp, 4
	push	edx
	push	eax
	push	ecx
	call	memcpy@PLT
	add	esp, 16
	mov	eax, DWORD PTR 8[ebp]
	add	eax, 8388644
	lea	ecx, 4[eax]
	mov	eax, DWORD PTR 8[ebp]
	lea	edx, 8388644[eax]
	mov	eax, DWORD PTR blowfish_ctx@GOTOFF[ebx]
	sub	esp, 4
	push	ecx
	push	edx
	push	eax
	call	blowfish_encrypt@PLT
	add	esp, 16
	jmp	.L81
.L82:
	mov	eax, DWORD PTR 8[ebp]
	lea	ecx, 8388644[eax]
	mov	eax, DWORD PTR 8[ebp]
	mov	edx, DWORD PTR 20[eax]
	mov	eax, DWORD PTR threefish_ctx@GOTOFF[ebx]
	sub	esp, 4
	push	ecx
	push	edx
	push	eax
	call	threefish_encrypt@PLT
	add	esp, 16
	nop
.L81:
	mov	eax, DWORD PTR 8[ebp]
	mov	edx, DWORD PTR 24[eax]
	mov	eax, DWORD PTR 8[ebp]
	lea	ecx, 8388644[eax]
	mov	eax, DWORD PTR 8[ebp]
	mov	eax, DWORD PTR 20[eax]
	sub	esp, 4
	push	edx
	push	ecx
	push	eax
	call	memcpy@PLT
	add	esp, 16
	mov	eax, DWORD PTR 8[ebp]
	mov	edx, DWORD PTR 24[eax]
	mov	eax, DWORD PTR 8[ebp]
	mov	eax, DWORD PTR 20[eax]
	push	DWORD PTR -56[ebp]
	push	edx
	push	1
	push	eax
	call	fwrite@PLT
	add	esp, 16
	mov	edx, DWORD PTR 8[ebp]
	mov	edx, DWORD PTR 24[edx]
	cmp	eax, edx
	je	.L88
	sub	esp, 4
	push	-4
	push	DWORD PTR -56[ebp]
	push	DWORD PTR -52[ebp]
	call	close_in_out_files
	add	esp, 16
	jmp	.L71
.L88:
	sub	esp, 12
	push	DWORD PTR -56[ebp]
	call	fflush@PLT
	add	esp, 16
	jmp	.L79
.L80:
	mov	eax, DWORD PTR 8[ebp]
	mov	eax, DWORD PTR 16777280[eax]
	cmp	eax, 222
	jne	.L79
	mov	eax, DWORD PTR 8[ebp]
	mov	edx, DWORD PTR 24[eax]
	mov	eax, DWORD PTR 8[ebp]
	mov	eax, DWORD PTR 20[eax]
	push	DWORD PTR -52[ebp]
	push	edx
	push	1
	push	eax
	call	fread@PLT
	add	esp, 16
	mov	edx, DWORD PTR 8[ebp]
	mov	edx, DWORD PTR 24[edx]
	cmp	eax, edx
	je	.L89
	sub	esp, 4
	push	-5
	push	DWORD PTR -56[ebp]
	push	DWORD PTR -52[ebp]
	call	close_in_out_files
	add	esp, 16
	jmp	.L71
.L89:
	mov	eax, DWORD PTR 8[ebp]
	mov	eax, DWORD PTR 24[eax]
	add	DWORD PTR -48[ebp], eax
.L79:
	mov	eax, DWORD PTR 8[ebp]
	add	eax, 36
	push	DWORD PTR -52[ebp]
	push	8388608
	push	1
	push	eax
	call	fread@PLT
	add	esp, 16
	mov	DWORD PTR -40[ebp], eax
	mov	DWORD PTR -36[ebp], 0
	jmp	.L90
.L100:
	mov	eax, DWORD PTR 8[ebp]
	mov	eax, DWORD PTR 16777284[eax]
	cmp	eax, 4
	ja	.L91
	sal	eax, 2
	mov	eax, DWORD PTR .L93@GOTOFF[eax+ebx]
	add	eax, ebx
	jmp	eax
	.section	.rodata
	.align 4
	.align 4
.L93:
	.long	.L97@GOTOFF
	.long	.L96@GOTOFF
	.long	.L95@GOTOFF
	.long	.L94@GOTOFF
	.long	.L92@GOTOFF
	.text
.L97:
	mov	eax, DWORD PTR 8[ebp]
	lea	edx, 8388644[eax]
	mov	eax, DWORD PTR -36[ebp]
	lea	ecx, [edx+eax]
	mov	eax, DWORD PTR 8[ebp]
	mov	edx, DWORD PTR 20[eax]
	mov	eax, DWORD PTR rijndael_ctx@GOTOFF[ebx]
	sub	esp, 4
	push	ecx
	push	edx
	push	eax
	call	rijndael_encrypt@PLT
	add	esp, 16
	jmp	.L91
.L96:
	mov	eax, DWORD PTR 8[ebp]
	lea	edx, 8388644[eax]
	mov	eax, DWORD PTR -36[ebp]
	lea	ecx, [edx+eax]
	mov	eax, DWORD PTR 8[ebp]
	mov	edx, DWORD PTR 20[eax]
	mov	eax, DWORD PTR serpent_ctx@GOTOFF[ebx]
	sub	esp, 4
	push	ecx
	push	edx
	push	eax
	call	serpent_encrypt@PLT
	add	esp, 16
	jmp	.L91
.L95:
	mov	eax, DWORD PTR 8[ebp]
	lea	edx, 8388644[eax]
	mov	eax, DWORD PTR -36[ebp]
	lea	ecx, [edx+eax]
	mov	eax, DWORD PTR 8[ebp]
	mov	edx, DWORD PTR 20[eax]
	mov	eax, DWORD PTR twofish_ctx@GOTOFF[ebx]
	sub	esp, 4
	push	ecx
	push	edx
	push	eax
	call	twofish_encrypt@PLT
	add	esp, 16
	jmp	.L91
.L94:
	mov	eax, DWORD PTR 8[ebp]
	mov	eax, DWORD PTR 20[eax]
	lea	ecx, 4[eax]
	mov	eax, DWORD PTR 8[ebp]
	mov	edx, DWORD PTR 20[eax]
	mov	eax, DWORD PTR blowfish_ctx@GOTOFF[ebx]
	sub	esp, 4
	push	ecx
	push	edx
	push	eax
	call	blowfish_encrypt@PLT
	add	esp, 16
	mov	eax, DWORD PTR 8[ebp]
	mov	edx, DWORD PTR 24[eax]
	mov	eax, DWORD PTR 8[ebp]
	mov	eax, DWORD PTR 20[eax]
	mov	ecx, DWORD PTR 8[ebp]
	lea	esi, 8388644[ecx]
	mov	ecx, DWORD PTR -36[ebp]
	add	ecx, esi
	sub	esp, 4
	push	edx
	push	eax
	push	ecx
	call	memcpy@PLT
	add	esp, 16
	jmp	.L91
.L92:
	mov	eax, DWORD PTR 8[ebp]
	lea	edx, 8388644[eax]
	mov	eax, DWORD PTR -36[ebp]
	lea	ecx, [edx+eax]
	mov	eax, DWORD PTR 8[ebp]
	mov	edx, DWORD PTR 20[eax]
	mov	eax, DWORD PTR threefish_ctx@GOTOFF[ebx]
	sub	esp, 4
	push	ecx
	push	edx
	push	eax
	call	threefish_encrypt@PLT
	add	esp, 16
	nop
.L91:
	mov	eax, DWORD PTR 8[ebp]
	mov	eax, DWORD PTR 24[eax]
	mov	edx, DWORD PTR 8[ebp]
	lea	ecx, 36[edx]
	mov	edx, DWORD PTR -36[ebp]
	add	ecx, edx
	mov	edx, DWORD PTR 8[ebp]
	lea	esi, 8388644[edx]
	mov	edx, DWORD PTR -36[ebp]
	add	edx, esi
	sub	esp, 4
	push	eax
	push	ecx
	push	edx
	call	memxormove@PLT
	add	esp, 16
	mov	eax, DWORD PTR 8[ebp]
	mov	edx, DWORD PTR 24[eax]
	mov	eax, DWORD PTR 8[ebp]
	mov	eax, DWORD PTR 16777280[eax]
	test	eax, eax
	je	.L98
	mov	eax, DWORD PTR 8[ebp]
	add	eax, 36
	jmp	.L99
.L98:
	mov	eax, DWORD PTR 8[ebp]
	add	eax, 8388644
.L99:
	mov	ecx, DWORD PTR -36[ebp]
	add	ecx, eax
	mov	eax, DWORD PTR 8[ebp]
	mov	eax, DWORD PTR 20[eax]
	sub	esp, 4
	push	edx
	push	ecx
	push	eax
	call	memcpy@PLT
	add	esp, 16
	mov	eax, DWORD PTR 8[ebp]
	mov	eax, DWORD PTR 24[eax]
	add	DWORD PTR -36[ebp], eax
.L90:
	mov	eax, DWORD PTR -36[ebp]
	cmp	eax, DWORD PTR -40[ebp]
	jb	.L100
	mov	eax, DWORD PTR -40[ebp]
	add	DWORD PTR -48[ebp], eax
	fild	DWORD PTR -48[ebp]
	fdiv	QWORD PTR -72[ebp]
	fld	QWORD PTR .LC15@GOTOFF[ebx]
	faddp	st(1), st
	fnstcw	WORD PTR -90[ebp]
	movzx	eax, WORD PTR -90[ebp]
	or	ah, 12
	mov	WORD PTR -92[ebp], ax
	fldcw	WORD PTR -92[ebp]
	fistp	DWORD PTR -28[ebp]
	fldcw	WORD PTR -90[ebp]
	cmp	DWORD PTR -28[ebp], 100
	jle	.L101
	mov	DWORD PTR -28[ebp], 100
.L101:
	mov	eax, DWORD PTR -48[ebp]
	cmp	eax, DWORD PTR -44[ebp]
	jne	.L102
	mov	eax, DWORD PTR 8[ebp]
	mov	eax, DWORD PTR 16777280[eax]
	cmp	eax, 222
	jne	.L102
	sub	DWORD PTR -40[ebp], 32
.L102:
	sub	esp, 8
	push	DWORD PTR -40[ebp]
	push	DWORD PTR 8[ebp]
	call	control_sum_buffer
	add	esp, 16
	mov	eax, DWORD PTR 8[ebp]
	add	eax, 8388644
	push	DWORD PTR -56[ebp]
	push	DWORD PTR -40[ebp]
	push	1
	push	eax
	call	fwrite@PLT
	add	esp, 16
	cmp	DWORD PTR -40[ebp], eax
	je	.L103
	sub	esp, 4
	push	-4
	push	DWORD PTR -56[ebp]
	push	DWORD PTR -52[ebp]
	call	close_in_out_files
	add	esp, 16
	jmp	.L71
.L103:
	sub	esp, 12
	push	DWORD PTR -56[ebp]
	call	fflush@PLT
	add	esp, 16
	mov	eax, DWORD PTR -28[ebp]
	cmp	eax, DWORD PTR -32[ebp]
	jle	.L78
	mov	eax, DWORD PTR -28[ebp]
	lea	edx, 3[eax]
	test	eax, eax
	cmovs	eax, edx
	sar	eax, 2
	mov	edx, eax
	mov	eax, DWORD PTR 8[ebp]
	add	eax, 16777252
	sub	esp, 4
	push	edx
	push	35
	push	eax
	call	meminit@PLT
	add	esp, 16
	mov	eax, DWORD PTR -48[ebp]
	sub	esp, 12
	push	eax
	call	size_check
	add	esp, 16
	mov	DWORD PTR -60[ebp], eax
	mov	eax, DWORD PTR -76[ebp]
	mov	edi, DWORD PTR CHAR_SIZE_DATA@GOTOFF[ebx+eax*4]
	mov	eax, DWORD PTR -60[ebp]
	mov	esi, DWORD PTR CHAR_SIZE_DATA@GOTOFF[ebx+eax*4]
	fild	DWORD PTR -48[ebp]
	sub	esp, 4
	lea	esp, -8[esp]
	fstp	QWORD PTR [esp]
	push	DWORD PTR -60[ebp]
	call	sizetodoubleprint
	add	esp, 16
	fstp	QWORD PTR -104[ebp]
	mov	eax, DWORD PTR 8[ebp]
	add	eax, 16777252
	mov	DWORD PTR -96[ebp], eax
	mov	eax, DWORD PTR 8[ebp]
	mov	eax, DWORD PTR 16777280[eax]
	sub	esp, 12
	push	eax
	call	operation_variant
	add	esp, 16
	mov	eax, DWORD PTR OPERATION_NAME@GOTOFF[ebx+eax*4]
	sub	esp, 8
	push	DWORD PTR -28[ebp]
	push	edi
	push	DWORD PTR -84[ebp]
	push	DWORD PTR -88[ebp]
	push	esi
	push	DWORD PTR -100[ebp]
	push	DWORD PTR -104[ebp]
	push	DWORD PTR -96[ebp]
	push	eax
	lea	eax, .LC16@GOTOFF[ebx]
	push	eax
	call	printf@PLT
	add	esp, 48
	mov	eax, DWORD PTR stdout@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	sub	esp, 12
	push	eax
	call	fflush@PLT
	add	esp, 16
	mov	eax, DWORD PTR -28[ebp]
	mov	DWORD PTR -32[ebp], eax
.L78:
	mov	eax, DWORD PTR -48[ebp]
	cmp	eax, DWORD PTR -44[ebp]
	jl	.L104
	mov	eax, DWORD PTR stdout@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	sub	esp, 8
	push	eax
	push	10
	call	putc@PLT
	add	esp, 16
	mov	eax, DWORD PTR 8[ebp]
	mov	eax, DWORD PTR 12[eax]
	sub	esp, 12
	push	eax
	call	sha256_final@PLT
	add	esp, 16
	sub	esp, 12
	push	DWORD PTR 8[ebp]
	call	hmac_sha256_uf
	add	esp, 16
	mov	eax, DWORD PTR 8[ebp]
	mov	eax, DWORD PTR 16777280[eax]
	test	eax, eax
	jne	.L105
	mov	eax, DWORD PTR 8[ebp]
	mov	eax, DWORD PTR 12[eax]
	add	eax, 108
	push	DWORD PTR -56[ebp]
	push	32
	push	1
	push	eax
	call	fwrite@PLT
	add	esp, 16
	cmp	eax, 32
	je	.L106
	sub	esp, 4
	push	-4
	push	DWORD PTR -56[ebp]
	push	DWORD PTR -52[ebp]
	call	close_in_out_files
	add	esp, 16
	jmp	.L71
.L106:
	sub	esp, 12
	push	DWORD PTR -56[ebp]
	call	fflush@PLT
	add	esp, 16
	jmp	.L107
.L105:
	mov	eax, DWORD PTR 8[ebp]
	mov	eax, DWORD PTR 12[eax]
	lea	edx, 108[eax]
	mov	eax, DWORD PTR 8[ebp]
	lea	ecx, 36[eax]
	mov	eax, DWORD PTR -40[ebp]
	add	eax, ecx
	sub	esp, 4
	push	32
	push	edx
	push	eax
	call	memcmp@PLT
	add	esp, 16
	test	eax, eax
	je	.L108
	mov	eax, DWORD PTR 8[ebp]
	mov	eax, DWORD PTR [eax]
	sub	esp, 8
	push	eax
	lea	eax, .LC17@GOTOFF[ebx]
	push	eax
	call	printf@PLT
	add	esp, 16
	jmp	.L107
.L108:
	mov	eax, DWORD PTR 8[ebp]
	mov	eax, DWORD PTR [eax]
	sub	esp, 8
	push	eax
	lea	eax, .LC18@GOTOFF[ebx]
	push	eax
	call	printf@PLT
	add	esp, 16
.L107:
	sub	esp, 4
	push	0
	push	DWORD PTR -56[ebp]
	push	DWORD PTR -52[ebp]
	call	close_in_out_files
	add	esp, 16
.L71:
	lea	esp, -12[ebp]
	pop	ebx
	.cfi_restore 3
	pop	esi
	.cfi_restore 6
	pop	edi
	.cfi_restore 7
	pop	ebp
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE16:
	.size	filecrypt, .-filecrypt
	.type	random_vector_init, @function
random_vector_init:
.LFB17:
	.cfi_startproc
	push	ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	mov	ebp, esp
	.cfi_def_cfa_register 5
	push	esi
	push	ebx
	sub	esp, 32
	.cfi_offset 6, -12
	.cfi_offset 3, -16
	call	__x86.get_pc_thunk.bx
	add	ebx, OFFSET FLAT:_GLOBAL_OFFSET_TABLE_
	mov	DWORD PTR -16[ebp], 264
	mov	eax, DWORD PTR 12[ebp]
	mov	DWORD PTR -20[ebp], eax
	mov	DWORD PTR -24[ebp], 0
	cmp	DWORD PTR 8[ebp], 0
	je	.L119
	cmp	DWORD PTR 12[ebp], 0
	je	.L119
	sub	esp, 12
	push	DWORD PTR -16[ebp]
	call	malloc@PLT
	add	esp, 16
	mov	DWORD PTR -28[ebp], eax
	sub	esp, 12
	push	DWORD PTR -20[ebp]
	call	malloc@PLT
	add	esp, 16
	mov	DWORD PTR -24[ebp], eax
	cmp	DWORD PTR -28[ebp], 0
	je	.L113
	cmp	DWORD PTR -24[ebp], 0
	jne	.L114
.L113:
	cmp	DWORD PTR -28[ebp], 0
	je	.L115
	sub	esp, 12
	push	DWORD PTR -28[ebp]
	call	free@PLT
	add	esp, 16
.L115:
	cmp	DWORD PTR -24[ebp], 0
	je	.L120
	sub	esp, 12
	push	DWORD PTR -24[ebp]
	call	free@PLT
	add	esp, 16
	jmp	.L120
.L114:
	mov	DWORD PTR -12[ebp], 0
	jmp	.L117
.L118:
	sub	esp, 8
	push	255
	push	0
	call	genrand@PLT
	add	esp, 16
	mov	esi, eax
	mov	edx, DWORD PTR -24[ebp]
	mov	eax, DWORD PTR -12[ebp]
	add	eax, edx
	movzx	edx, BYTE PTR [eax]
	mov	ecx, DWORD PTR -24[ebp]
	mov	eax, DWORD PTR -12[ebp]
	add	eax, ecx
	xor	edx, esi
	mov	BYTE PTR [eax], dl
	add	DWORD PTR -12[ebp], 1
.L117:
	mov	eax, DWORD PTR -12[ebp]
	cmp	eax, DWORD PTR -20[ebp]
	jb	.L118
	sub	esp, 4
	push	DWORD PTR 12[ebp]
	push	DWORD PTR 8[ebp]
	push	DWORD PTR -28[ebp]
	call	arc4_init@PLT
	add	esp, 16
	push	DWORD PTR 12[ebp]
	push	DWORD PTR 8[ebp]
	push	DWORD PTR -24[ebp]
	push	DWORD PTR -28[ebp]
	call	arc4@PLT
	add	esp, 16
	sub	esp, 4
	push	DWORD PTR -20[ebp]
	push	0
	push	DWORD PTR -24[ebp]
	call	meminit@PLT
	add	esp, 16
	sub	esp, 4
	push	DWORD PTR -16[ebp]
	push	0
	push	DWORD PTR -28[ebp]
	call	meminit@PLT
	add	esp, 16
	sub	esp, 12
	push	DWORD PTR -24[ebp]
	call	free@PLT
	add	esp, 16
	sub	esp, 12
	push	DWORD PTR -28[ebp]
	call	free@PLT
	add	esp, 16
	mov	DWORD PTR -24[ebp], 0
	mov	DWORD PTR -28[ebp], 0
	mov	DWORD PTR -20[ebp], 0
	mov	DWORD PTR -16[ebp], 0
	jmp	.L109
.L119:
	nop
	jmp	.L109
.L120:
	nop
.L109:
	lea	esp, -8[ebp]
	pop	ebx
	.cfi_restore 3
	pop	esi
	.cfi_restore 6
	pop	ebp
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE17:
	.size	random_vector_init, .-random_vector_init
	.type	vector_init, @function
vector_init:
.LFB18:
	.cfi_startproc
	push	ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	mov	ebp, esp
	.cfi_def_cfa_register 5
	push	esi
	push	ebx
	sub	esp, 16
	.cfi_offset 6, -12
	.cfi_offset 3, -16
	call	__x86.get_pc_thunk.bx
	add	ebx, OFFSET FLAT:_GLOBAL_OFFSET_TABLE_
	cmp	DWORD PTR 8[ebp], 0
	jne	.L122
	mov	eax, 0
	jmp	.L123
.L122:
	mov	DWORD PTR -12[ebp], 0
	jmp	.L124
.L125:
	mov	eax, DWORD PTR -12[ebp]
	mov	esi, eax
	sub	esp, 8
	push	255
	push	0
	call	genrand@PLT
	add	esp, 16
	mov	ecx, eax
	mov	edx, DWORD PTR 8[ebp]
	mov	eax, DWORD PTR -12[ebp]
	add	eax, edx
	mov	edx, esi
	xor	edx, ecx
	mov	BYTE PTR [eax], dl
	add	DWORD PTR -12[ebp], 1
.L124:
	mov	eax, DWORD PTR -12[ebp]
	cmp	eax, DWORD PTR 12[ebp]
	jb	.L125
	sub	esp, 8
	push	32767
	push	0
	call	genrand@PLT
	add	esp, 16
	xor	eax, DWORD PTR -16[ebp]
	mov	edx, eax
	mov	eax, DWORD PTR 8[ebp]
	mov	eax, DWORD PTR [eax]
	xor	edx, eax
	mov	eax, DWORD PTR 8[ebp]
	mov	DWORD PTR [eax], edx
	sub	esp, 8
	push	DWORD PTR 12[ebp]
	push	DWORD PTR 8[ebp]
	call	random_vector_init
	add	esp, 16
	sub	DWORD PTR 12[ebp], 2
	mov	DWORD PTR -12[ebp], 0
	jmp	.L126
.L129:
	mov	edx, DWORD PTR 8[ebp]
	mov	eax, DWORD PTR -12[ebp]
	add	eax, edx
	movzx	edx, BYTE PTR [eax]
	mov	eax, DWORD PTR -12[ebp]
	lea	ecx, 1[eax]
	mov	eax, DWORD PTR 8[ebp]
	add	eax, ecx
	movzx	eax, BYTE PTR [eax]
	cmp	dl, al
	jne	.L127
	mov	eax, DWORD PTR -12[ebp]
	lea	edx, 1[eax]
	mov	eax, DWORD PTR 8[ebp]
	add	eax, edx
	movzx	edx, BYTE PTR [eax]
	mov	eax, DWORD PTR -12[ebp]
	lea	ecx, 2[eax]
	mov	eax, DWORD PTR 8[ebp]
	add	eax, ecx
	movzx	eax, BYTE PTR [eax]
	cmp	dl, al
	je	.L130
.L127:
	add	DWORD PTR -12[ebp], 1
.L126:
	mov	eax, DWORD PTR -12[ebp]
	cmp	eax, DWORD PTR 12[ebp]
	jb	.L129
	jmp	.L128
.L130:
	nop
.L128:
	mov	eax, DWORD PTR -12[ebp]
.L123:
	lea	esp, -8[ebp]
	pop	ebx
	.cfi_restore 3
	pop	esi
	.cfi_restore 6
	pop	ebp
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE18:
	.size	vector_init, .-vector_init
	.section	.rodata
	.align 4
.LC19:
	.string	"[!] Warning: argument \"%d\" length more \"%d\"!\n"
	.align 4
.LC20:
	.string	"[!] Error: count arguments %d; necessary to 2 do 7 strings!\n"
.LC21:
	.string	"-h"
.LC22:
	.string	"--help"
	.align 4
.LC23:
	.string	"This is software for encrypt/decrypt files.\n"
	.align 4
.LC24:
	.string	"Algoritms:\n\t-r/--aes\n\t-s/--serpent\n\t-w/--twofish\n\t-b/--blowfish\n\t-t/--threefish"
	.align 4
.LC25:
	.string	"Operations:\n\t-e/--encrypt\n\t-d/--decrypt"
	.align 4
.LC26:
	.string	"Lengths key:\n\t-a/--128/--256\n\t-b/--192/--512\n\t-c/--256/--1024\n"
	.align 4
.LC27:
	.string	"Enter: %s [algoritm] [operation] [key length] [input filename] [output filename] [key filename or string key]\n"
	.align 4
.LC28:
	.string	"[!] Incorrect parameter \"%s\"! Only \"-h\" or \"--help\".\n"
.LC29:
	.string	"[!] Cannot allocate memory!\n"
	.align 4
.LC30:
	.string	"[!] Names input and output files equal!\n"
	.align 4
.LC31:
	.string	"[!] Names keyfile and output files equal!\n"
	.align 4
.LC32:
	.string	"[!] Names keyfile and input files equal!\n"
.LC33:
	.string	"-b"
.LC34:
	.string	"--blowfish"
.LC35:
	.string	"-t"
.LC36:
	.string	"--threefish"
.LC37:
	.string	"-r"
.LC38:
	.string	"--aes"
.LC39:
	.string	"-s"
.LC40:
	.string	"--serpent"
.LC41:
	.string	"-w"
.LC42:
	.string	"--twofish"
	.align 4
.LC43:
	.string	"[!] Name cipher \"%s\" incorrect!\n"
.LC44:
	.string	"-e"
.LC45:
	.string	"--encrypt"
.LC46:
	.string	"-d"
.LC47:
	.string	"--decrypt"
	.align 4
.LC48:
	.string	"[!] Operation \"%s\" incorrect!\n"
.LC49:
	.string	"-a"
.LC50:
	.string	"--128"
.LC51:
	.string	"--192"
.LC52:
	.string	"-c"
.LC53:
	.string	"--256"
	.align 4
.LC54:
	.string	"[!] Key length \"%s\" incorrect!\n"
.LC55:
	.string	"--512"
.LC56:
	.string	"--1024"
	.align 4
.LC57:
	.string	"[#] Crypt key read from file \"%s\"!\n"
	.align 4
.LC58:
	.string	"[!] Data in key file %d byte; necessary %d byte!\n"
	.align 4
.LC59:
	.string	"[#] Crypt key read from command line!"
	.align 4
.LC60:
	.string	"[!] Data in string key %d byte; necessary 8..256 byte!\n"
	.align 4
.LC61:
	.string	"[#] Key length %d-bits initialized!\n"
	.align 4
.LC62:
	.string	"[X] Critical error! System time stopped?\n"
.LC63:
	.string	"[#] Algoritm %s initialized!\n"
	.align 4
.LC64:
	.string	"[#] Operation %s file \"%s\" started!\n"
.LC65:
	.string	"[#] %s file \"%s\" complete!\n"
	.align 4
.LC66:
	.string	"[!] File for %s \"%s\" not opened!\n"
	.align 4
.LC67:
	.string	"[!] Output file \"%s\" not opened!\n"
	.align 4
.LC68:
	.string	"[!] Size of input file \"%s\" 0 or more 2 GiB!\n"
	.align 4
.LC69:
	.string	"[!] Error write in file \"%s\" !\n"
	.align 4
.LC70:
	.string	"[!] Error read form file \"%s\" !\n"
	.align 4
.LC71:
	.string	"[!] Error close input stream!\n"
	.align 4
.LC72:
	.string	"[!] Error close output stream!\n"
	.align 4
.LC73:
	.string	"[!] Size of file for decrypt \"%s\" incorrect!\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB19:
	.cfi_startproc
	lea	ecx, 4[esp]
	.cfi_def_cfa 1, 0
	and	esp, -16
	push	DWORD PTR -4[ecx]
	push	ebp
	.cfi_escape 0x10,0x5,0x2,0x75,0
	mov	ebp, esp
	push	esi
	push	ebx
	push	ecx
	.cfi_escape 0xf,0x3,0x75,0x74,0x6
	.cfi_escape 0x10,0x6,0x2,0x75,0x7c
	.cfi_escape 0x10,0x3,0x2,0x75,0x78
	sub	esp, 44
	call	__x86.get_pc_thunk.bx
	add	ebx, OFFSET FLAT:_GLOBAL_OFFSET_TABLE_
	mov	esi, ecx
	mov	DWORD PTR -32[ebp], 0
	mov	DWORD PTR -36[ebp], 0
	cmp	DWORD PTR [esi], 1
	jle	.L132
	cmp	DWORD PTR [esi], 7
	jg	.L132
	mov	DWORD PTR -28[ebp], 1
	jmp	.L133
.L136:
	mov	eax, DWORD PTR -28[ebp]
	lea	edx, 0[0+eax*4]
	mov	eax, DWORD PTR 4[esi]
	add	eax, edx
	mov	eax, DWORD PTR [eax]
	sub	esp, 8
	push	2048
	push	eax
	call	x_strnlen@PLT
	add	esp, 16
	cmp	eax, 2048
	jne	.L134
	mov	eax, DWORD PTR stderr@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	push	2048
	push	DWORD PTR -28[ebp]
	lea	edx, .LC19@GOTOFF[ebx]
	push	edx
	push	eax
	call	fprintf@PLT
	add	esp, 16
	mov	eax, -1
	jmp	.L135
.L134:
	add	DWORD PTR -28[ebp], 1
.L133:
	mov	eax, DWORD PTR [esi]
	sub	eax, 1
	cmp	DWORD PTR -28[ebp], eax
	jl	.L136
	jmp	.L137
.L132:
	cmp	DWORD PTR [esi], 1
	je	.L138
	cmp	DWORD PTR [esi], 7
	jle	.L137
.L138:
	mov	eax, DWORD PTR stderr@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	sub	esp, 4
	push	DWORD PTR [esi]
	lea	edx, .LC20@GOTOFF[ebx]
	push	edx
	push	eax
	call	fprintf@PLT
	add	esp, 16
	mov	eax, 0
	jmp	.L135
.L137:
	cmp	DWORD PTR [esi], 2
	jne	.L139
	mov	eax, DWORD PTR 4[esi]
	add	eax, 4
	mov	eax, DWORD PTR [eax]
	sub	esp, 8
	lea	edx, .LC21@GOTOFF[ebx]
	push	edx
	push	eax
	call	strcmp@PLT
	add	esp, 16
	test	eax, eax
	je	.L140
	mov	eax, DWORD PTR 4[esi]
	add	eax, 4
	mov	eax, DWORD PTR [eax]
	sub	esp, 8
	lea	edx, .LC22@GOTOFF[ebx]
	push	edx
	push	eax
	call	strcmp@PLT
	add	esp, 16
	test	eax, eax
	jne	.L141
.L140:
	mov	eax, DWORD PTR PROGRAMM_NAME@GOTOFF[ebx]
	sub	esp, 12
	push	eax
	call	puts@PLT
	add	esp, 16
	sub	esp, 12
	lea	eax, .LC23@GOTOFF[ebx]
	push	eax
	call	puts@PLT
	add	esp, 16
	sub	esp, 12
	lea	eax, .LC24@GOTOFF[ebx]
	push	eax
	call	puts@PLT
	add	esp, 16
	sub	esp, 12
	lea	eax, .LC25@GOTOFF[ebx]
	push	eax
	call	puts@PLT
	add	esp, 16
	sub	esp, 12
	lea	eax, .LC26@GOTOFF[ebx]
	push	eax
	call	puts@PLT
	add	esp, 16
	mov	eax, DWORD PTR 4[esi]
	mov	eax, DWORD PTR [eax]
	sub	esp, 8
	push	eax
	lea	eax, .LC27@GOTOFF[ebx]
	push	eax
	call	printf@PLT
	add	esp, 16
	mov	eax, 0
	jmp	.L135
.L141:
	mov	eax, DWORD PTR 4[esi]
	add	eax, 4
	mov	edx, DWORD PTR [eax]
	mov	eax, DWORD PTR stderr@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	sub	esp, 4
	push	edx
	lea	edx, .LC28@GOTOFF[ebx]
	push	edx
	push	eax
	call	fprintf@PLT
	add	esp, 16
	mov	eax, -1
	jmp	.L135
.L139:
	cmp	DWORD PTR [esi], 4
	jg	.L142
	mov	eax, DWORD PTR stderr@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	sub	esp, 4
	push	DWORD PTR [esi]
	lea	edx, .LC20@GOTOFF[ebx]
	push	edx
	push	eax
	call	fprintf@PLT
	add	esp, 16
	mov	eax, 0
	jmp	.L135
.L142:
	mov	DWORD PTR -40[ebp], 16777288
	sub	esp, 8
	push	1
	push	DWORD PTR -40[ebp]
	call	calloc@PLT
	add	esp, 16
	mov	DWORD PTR -44[ebp], eax
	cmp	DWORD PTR -44[ebp], 0
	jne	.L143
	mov	eax, DWORD PTR stderr@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	push	eax
	push	28
	push	1
	lea	eax, .LC29@GOTOFF[ebx]
	push	eax
	call	fwrite@PLT
	add	esp, 16
	mov	eax, -1
	jmp	.L135
.L143:
	mov	eax, DWORD PTR [esi]
	add	eax, 1073741823
	lea	edx, 0[0+eax*4]
	mov	eax, DWORD PTR 4[esi]
	add	eax, edx
	mov	edx, DWORD PTR [eax]
	mov	eax, DWORD PTR -44[ebp]
	mov	DWORD PTR 8[eax], edx
	mov	eax, DWORD PTR [esi]
	add	eax, 1073741822
	lea	edx, 0[0+eax*4]
	mov	eax, DWORD PTR 4[esi]
	add	eax, edx
	mov	edx, DWORD PTR [eax]
	mov	eax, DWORD PTR -44[ebp]
	mov	DWORD PTR 4[eax], edx
	mov	eax, DWORD PTR [esi]
	add	eax, 1073741821
	lea	edx, 0[0+eax*4]
	mov	eax, DWORD PTR 4[esi]
	add	eax, edx
	mov	edx, DWORD PTR [eax]
	mov	eax, DWORD PTR -44[ebp]
	mov	DWORD PTR [eax], edx
	mov	eax, DWORD PTR -44[ebp]
	mov	edx, DWORD PTR 4[eax]
	mov	eax, DWORD PTR -44[ebp]
	mov	eax, DWORD PTR [eax]
	sub	esp, 8
	push	edx
	push	eax
	call	strcmp@PLT
	add	esp, 16
	test	eax, eax
	jne	.L144
	sub	esp, 8
	push	DWORD PTR -40[ebp]
	push	DWORD PTR -44[ebp]
	call	free_global_memory
	add	esp, 16
	mov	eax, DWORD PTR stderr@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	push	eax
	push	40
	push	1
	lea	eax, .LC30@GOTOFF[ebx]
	push	eax
	call	fwrite@PLT
	add	esp, 16
	mov	eax, -1
	jmp	.L135
.L144:
	mov	eax, DWORD PTR -44[ebp]
	mov	edx, DWORD PTR 8[eax]
	mov	eax, DWORD PTR -44[ebp]
	mov	eax, DWORD PTR 4[eax]
	sub	esp, 8
	push	edx
	push	eax
	call	strcmp@PLT
	add	esp, 16
	test	eax, eax
	jne	.L145
	sub	esp, 8
	push	DWORD PTR -40[ebp]
	push	DWORD PTR -44[ebp]
	call	free_global_memory
	add	esp, 16
	mov	eax, DWORD PTR stderr@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	push	eax
	push	42
	push	1
	lea	eax, .LC31@GOTOFF[ebx]
	push	eax
	call	fwrite@PLT
	add	esp, 16
	mov	eax, -1
	jmp	.L135
.L145:
	mov	eax, DWORD PTR -44[ebp]
	mov	edx, DWORD PTR 8[eax]
	mov	eax, DWORD PTR -44[ebp]
	mov	eax, DWORD PTR [eax]
	sub	esp, 8
	push	edx
	push	eax
	call	strcmp@PLT
	add	esp, 16
	test	eax, eax
	jne	.L146
	sub	esp, 8
	push	DWORD PTR -40[ebp]
	push	DWORD PTR -44[ebp]
	call	free_global_memory
	add	esp, 16
	mov	eax, DWORD PTR stderr@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	push	eax
	push	41
	push	1
	lea	eax, .LC32@GOTOFF[ebx]
	push	eax
	call	fwrite@PLT
	add	esp, 16
	mov	eax, -1
	jmp	.L135
.L146:
	mov	eax, DWORD PTR 4[esi]
	add	eax, 4
	mov	eax, DWORD PTR [eax]
	sub	esp, 8
	lea	edx, .LC33@GOTOFF[ebx]
	push	edx
	push	eax
	call	strcmp@PLT
	add	esp, 16
	test	eax, eax
	je	.L147
	mov	eax, DWORD PTR 4[esi]
	add	eax, 4
	mov	eax, DWORD PTR [eax]
	sub	esp, 8
	lea	edx, .LC34@GOTOFF[ebx]
	push	edx
	push	eax
	call	strcmp@PLT
	add	esp, 16
	test	eax, eax
	jne	.L148
.L147:
	mov	eax, DWORD PTR -44[ebp]
	mov	DWORD PTR 16777284[eax], 3
	jmp	.L149
.L148:
	mov	eax, DWORD PTR 4[esi]
	add	eax, 4
	mov	eax, DWORD PTR [eax]
	sub	esp, 8
	lea	edx, .LC35@GOTOFF[ebx]
	push	edx
	push	eax
	call	strcmp@PLT
	add	esp, 16
	test	eax, eax
	je	.L150
	mov	eax, DWORD PTR 4[esi]
	add	eax, 4
	mov	eax, DWORD PTR [eax]
	sub	esp, 8
	lea	edx, .LC36@GOTOFF[ebx]
	push	edx
	push	eax
	call	strcmp@PLT
	add	esp, 16
	test	eax, eax
	jne	.L151
.L150:
	mov	eax, DWORD PTR -44[ebp]
	mov	DWORD PTR 16777284[eax], 4
	jmp	.L149
.L151:
	mov	eax, DWORD PTR 4[esi]
	add	eax, 4
	mov	eax, DWORD PTR [eax]
	sub	esp, 8
	lea	edx, .LC37@GOTOFF[ebx]
	push	edx
	push	eax
	call	strcmp@PLT
	add	esp, 16
	test	eax, eax
	je	.L152
	mov	eax, DWORD PTR 4[esi]
	add	eax, 4
	mov	eax, DWORD PTR [eax]
	sub	esp, 8
	lea	edx, .LC38@GOTOFF[ebx]
	push	edx
	push	eax
	call	strcmp@PLT
	add	esp, 16
	test	eax, eax
	jne	.L153
.L152:
	mov	eax, DWORD PTR -44[ebp]
	mov	DWORD PTR 16777284[eax], 0
	jmp	.L149
.L153:
	mov	eax, DWORD PTR 4[esi]
	add	eax, 4
	mov	eax, DWORD PTR [eax]
	sub	esp, 8
	lea	edx, .LC39@GOTOFF[ebx]
	push	edx
	push	eax
	call	strcmp@PLT
	add	esp, 16
	test	eax, eax
	je	.L154
	mov	eax, DWORD PTR 4[esi]
	add	eax, 4
	mov	eax, DWORD PTR [eax]
	sub	esp, 8
	lea	edx, .LC40@GOTOFF[ebx]
	push	edx
	push	eax
	call	strcmp@PLT
	add	esp, 16
	test	eax, eax
	jne	.L155
.L154:
	mov	eax, DWORD PTR -44[ebp]
	mov	DWORD PTR 16777284[eax], 1
	jmp	.L149
.L155:
	mov	eax, DWORD PTR 4[esi]
	add	eax, 4
	mov	eax, DWORD PTR [eax]
	sub	esp, 8
	lea	edx, .LC41@GOTOFF[ebx]
	push	edx
	push	eax
	call	strcmp@PLT
	add	esp, 16
	test	eax, eax
	je	.L156
	mov	eax, DWORD PTR 4[esi]
	add	eax, 4
	mov	eax, DWORD PTR [eax]
	sub	esp, 8
	lea	edx, .LC42@GOTOFF[ebx]
	push	edx
	push	eax
	call	strcmp@PLT
	add	esp, 16
	test	eax, eax
	jne	.L157
.L156:
	mov	eax, DWORD PTR -44[ebp]
	mov	DWORD PTR 16777284[eax], 2
	jmp	.L149
.L157:
	sub	esp, 8
	push	DWORD PTR -40[ebp]
	push	DWORD PTR -44[ebp]
	call	free_global_memory
	add	esp, 16
	mov	eax, DWORD PTR 4[esi]
	add	eax, 4
	mov	edx, DWORD PTR [eax]
	mov	eax, DWORD PTR stderr@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	sub	esp, 4
	push	edx
	lea	edx, .LC43@GOTOFF[ebx]
	push	edx
	push	eax
	call	fprintf@PLT
	add	esp, 16
	mov	eax, -1
	jmp	.L135
.L149:
	mov	eax, DWORD PTR 4[esi]
	add	eax, 8
	mov	eax, DWORD PTR [eax]
	sub	esp, 8
	lea	edx, .LC44@GOTOFF[ebx]
	push	edx
	push	eax
	call	strcmp@PLT
	add	esp, 16
	test	eax, eax
	je	.L158
	mov	eax, DWORD PTR 4[esi]
	add	eax, 8
	mov	eax, DWORD PTR [eax]
	sub	esp, 8
	lea	edx, .LC45@GOTOFF[ebx]
	push	edx
	push	eax
	call	strcmp@PLT
	add	esp, 16
	test	eax, eax
	jne	.L159
.L158:
	mov	eax, DWORD PTR -44[ebp]
	mov	DWORD PTR 16777280[eax], 0
	jmp	.L160
.L159:
	mov	eax, DWORD PTR 4[esi]
	add	eax, 8
	mov	eax, DWORD PTR [eax]
	sub	esp, 8
	lea	edx, .LC46@GOTOFF[ebx]
	push	edx
	push	eax
	call	strcmp@PLT
	add	esp, 16
	test	eax, eax
	je	.L161
	mov	eax, DWORD PTR 4[esi]
	add	eax, 8
	mov	eax, DWORD PTR [eax]
	sub	esp, 8
	lea	edx, .LC47@GOTOFF[ebx]
	push	edx
	push	eax
	call	strcmp@PLT
	add	esp, 16
	test	eax, eax
	jne	.L162
.L161:
	mov	eax, DWORD PTR -44[ebp]
	mov	DWORD PTR 16777280[eax], 222
	jmp	.L160
.L162:
	sub	esp, 8
	push	DWORD PTR -40[ebp]
	push	DWORD PTR -44[ebp]
	call	free_global_memory
	add	esp, 16
	mov	eax, DWORD PTR 4[esi]
	add	eax, 8
	mov	edx, DWORD PTR [eax]
	mov	eax, DWORD PTR stderr@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	sub	esp, 4
	push	edx
	lea	edx, .LC48@GOTOFF[ebx]
	push	edx
	push	eax
	call	fprintf@PLT
	add	esp, 16
	mov	eax, -1
	jmp	.L135
.L160:
	mov	eax, DWORD PTR -44[ebp]
	mov	eax, DWORD PTR 16777284[eax]
	test	eax, eax
	je	.L163
	mov	eax, DWORD PTR -44[ebp]
	mov	eax, DWORD PTR 16777284[eax]
	cmp	eax, 1
	je	.L163
	mov	eax, DWORD PTR -44[ebp]
	mov	eax, DWORD PTR 16777284[eax]
	cmp	eax, 2
	jne	.L164
.L163:
	mov	eax, DWORD PTR 4[esi]
	add	eax, 12
	mov	eax, DWORD PTR [eax]
	sub	esp, 8
	lea	edx, .LC49@GOTOFF[ebx]
	push	edx
	push	eax
	call	strcmp@PLT
	add	esp, 16
	test	eax, eax
	je	.L165
	mov	eax, DWORD PTR 4[esi]
	add	eax, 12
	mov	eax, DWORD PTR [eax]
	sub	esp, 8
	lea	edx, .LC50@GOTOFF[ebx]
	push	edx
	push	eax
	call	strcmp@PLT
	add	esp, 16
	test	eax, eax
	jne	.L166
.L165:
	mov	eax, DWORD PTR -44[ebp]
	mov	eax, DWORD PTR 16777284[eax]
	test	eax, eax
	jne	.L167
	mov	eax, DWORD PTR AES_Rounds@GOT[ebx]
	mov	DWORD PTR [eax], 10
.L167:
	mov	eax, DWORD PTR -44[ebp]
	mov	DWORD PTR 32[eax], 128
	jmp	.L168
.L166:
	mov	eax, DWORD PTR 4[esi]
	add	eax, 12
	mov	eax, DWORD PTR [eax]
	sub	esp, 8
	lea	edx, .LC33@GOTOFF[ebx]
	push	edx
	push	eax
	call	strcmp@PLT
	add	esp, 16
	test	eax, eax
	je	.L169
	mov	eax, DWORD PTR 4[esi]
	add	eax, 12
	mov	eax, DWORD PTR [eax]
	sub	esp, 8
	lea	edx, .LC51@GOTOFF[ebx]
	push	edx
	push	eax
	call	strcmp@PLT
	add	esp, 16
	test	eax, eax
	jne	.L170
.L169:
	mov	eax, DWORD PTR -44[ebp]
	mov	eax, DWORD PTR 16777284[eax]
	test	eax, eax
	jne	.L171
	mov	eax, DWORD PTR AES_Rounds@GOT[ebx]
	mov	DWORD PTR [eax], 12
.L171:
	mov	eax, DWORD PTR -44[ebp]
	mov	DWORD PTR 32[eax], 192
	jmp	.L168
.L170:
	mov	eax, DWORD PTR 4[esi]
	add	eax, 12
	mov	eax, DWORD PTR [eax]
	sub	esp, 8
	lea	edx, .LC52@GOTOFF[ebx]
	push	edx
	push	eax
	call	strcmp@PLT
	add	esp, 16
	test	eax, eax
	je	.L172
	mov	eax, DWORD PTR 4[esi]
	add	eax, 12
	mov	eax, DWORD PTR [eax]
	sub	esp, 8
	lea	edx, .LC53@GOTOFF[ebx]
	push	edx
	push	eax
	call	strcmp@PLT
	add	esp, 16
	test	eax, eax
	jne	.L173
.L172:
	mov	eax, DWORD PTR -44[ebp]
	mov	eax, DWORD PTR 16777284[eax]
	test	eax, eax
	jne	.L174
	mov	eax, DWORD PTR AES_Rounds@GOT[ebx]
	mov	DWORD PTR [eax], 14
.L174:
	mov	eax, DWORD PTR -44[ebp]
	mov	DWORD PTR 32[eax], 256
	jmp	.L168
.L173:
	sub	esp, 8
	push	DWORD PTR -40[ebp]
	push	DWORD PTR -44[ebp]
	call	free_global_memory
	add	esp, 16
	mov	eax, DWORD PTR 4[esi]
	add	eax, 12
	mov	edx, DWORD PTR [eax]
	mov	eax, DWORD PTR stderr@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	sub	esp, 4
	push	edx
	lea	edx, .LC54@GOTOFF[ebx]
	push	edx
	push	eax
	call	fprintf@PLT
	add	esp, 16
	mov	eax, -1
	jmp	.L135
.L168:
	jmp	.L175
.L164:
	mov	eax, DWORD PTR -44[ebp]
	mov	eax, DWORD PTR 16777284[eax]
	cmp	eax, 3
	jne	.L176
	mov	eax, DWORD PTR -44[ebp]
	mov	DWORD PTR 32[eax], 448
	jmp	.L175
.L176:
	mov	eax, DWORD PTR -44[ebp]
	mov	eax, DWORD PTR 16777284[eax]
	cmp	eax, 4
	jne	.L175
	mov	eax, DWORD PTR 4[esi]
	add	eax, 12
	mov	eax, DWORD PTR [eax]
	sub	esp, 8
	lea	edx, .LC49@GOTOFF[ebx]
	push	edx
	push	eax
	call	strcmp@PLT
	add	esp, 16
	test	eax, eax
	je	.L177
	mov	eax, DWORD PTR 4[esi]
	add	eax, 12
	mov	eax, DWORD PTR [eax]
	sub	esp, 8
	lea	edx, .LC53@GOTOFF[ebx]
	push	edx
	push	eax
	call	strcmp@PLT
	add	esp, 16
	test	eax, eax
	jne	.L178
.L177:
	mov	eax, DWORD PTR -44[ebp]
	mov	DWORD PTR 32[eax], 256
	jmp	.L175
.L178:
	mov	eax, DWORD PTR 4[esi]
	add	eax, 12
	mov	eax, DWORD PTR [eax]
	sub	esp, 8
	lea	edx, .LC33@GOTOFF[ebx]
	push	edx
	push	eax
	call	strcmp@PLT
	add	esp, 16
	test	eax, eax
	je	.L179
	mov	eax, DWORD PTR 4[esi]
	add	eax, 12
	mov	eax, DWORD PTR [eax]
	sub	esp, 8
	lea	edx, .LC55@GOTOFF[ebx]
	push	edx
	push	eax
	call	strcmp@PLT
	add	esp, 16
	test	eax, eax
	jne	.L180
.L179:
	mov	eax, DWORD PTR -44[ebp]
	mov	DWORD PTR 32[eax], 512
	jmp	.L175
.L180:
	mov	eax, DWORD PTR 4[esi]
	add	eax, 12
	mov	eax, DWORD PTR [eax]
	sub	esp, 8
	lea	edx, .LC52@GOTOFF[ebx]
	push	edx
	push	eax
	call	strcmp@PLT
	add	esp, 16
	test	eax, eax
	je	.L181
	mov	eax, DWORD PTR 4[esi]
	add	eax, 12
	mov	eax, DWORD PTR [eax]
	sub	esp, 8
	lea	edx, .LC56@GOTOFF[ebx]
	push	edx
	push	eax
	call	strcmp@PLT
	add	esp, 16
	test	eax, eax
	jne	.L175
.L181:
	mov	eax, DWORD PTR -44[ebp]
	mov	DWORD PTR 32[eax], 1024
.L175:
	mov	eax, DWORD PTR -44[ebp]
	mov	eax, DWORD PTR 32[eax]
	shr	eax, 3
	mov	edx, eax
	mov	eax, DWORD PTR -44[ebp]
	mov	DWORD PTR 32[eax], edx
	mov	eax, DWORD PTR -44[ebp]
	mov	eax, DWORD PTR 32[eax]
	sub	esp, 8
	push	1
	push	eax
	call	calloc@PLT
	add	esp, 16
	mov	edx, eax
	mov	eax, DWORD PTR -44[ebp]
	mov	DWORD PTR 28[eax], edx
	mov	eax, DWORD PTR -44[ebp]
	mov	eax, DWORD PTR 28[eax]
	test	eax, eax
	jne	.L182
	sub	esp, 8
	push	DWORD PTR -40[ebp]
	push	DWORD PTR -44[ebp]
	call	free_global_memory
	add	esp, 16
	mov	eax, DWORD PTR stderr@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	push	eax
	push	28
	push	1
	lea	eax, .LC29@GOTOFF[ebx]
	push	eax
	call	fwrite@PLT
	add	esp, 16
	mov	eax, -1
	jmp	.L135
.L182:
	mov	eax, DWORD PTR -44[ebp]
	mov	DWORD PTR 16[eax], 140
	mov	eax, DWORD PTR -44[ebp]
	mov	eax, DWORD PTR 16[eax]
	sub	esp, 8
	push	eax
	push	1
	call	calloc@PLT
	add	esp, 16
	mov	edx, eax
	mov	eax, DWORD PTR -44[ebp]
	mov	DWORD PTR 12[eax], edx
	mov	eax, DWORD PTR -44[ebp]
	mov	eax, DWORD PTR 12[eax]
	test	eax, eax
	jne	.L183
	sub	esp, 8
	push	DWORD PTR -40[ebp]
	push	DWORD PTR -44[ebp]
	call	free_global_memory
	add	esp, 16
	mov	eax, DWORD PTR stderr@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	push	eax
	push	28
	push	1
	lea	eax, .LC29@GOTOFF[ebx]
	push	eax
	call	fwrite@PLT
	add	esp, 16
	mov	eax, -1
	jmp	.L135
.L183:
	mov	eax, DWORD PTR -44[ebp]
	mov	ecx, DWORD PTR 32[eax]
	mov	eax, DWORD PTR -44[ebp]
	mov	edx, DWORD PTR 28[eax]
	mov	eax, DWORD PTR -44[ebp]
	mov	eax, DWORD PTR 8[eax]
	sub	esp, 4
	push	ecx
	push	edx
	push	eax
	call	readfromfile@PLT
	add	esp, 16
	mov	DWORD PTR -48[ebp], eax
	mov	eax, DWORD PTR -44[ebp]
	mov	eax, DWORD PTR 32[eax]
	cmp	DWORD PTR -48[ebp], eax
	jne	.L184
	mov	eax, DWORD PTR -44[ebp]
	mov	eax, DWORD PTR 8[eax]
	sub	esp, 8
	push	eax
	lea	eax, .LC57@GOTOFF[ebx]
	push	eax
	call	printf@PLT
	add	esp, 16
	jmp	.L185
.L184:
	cmp	DWORD PTR -48[ebp], 0
	jle	.L186
	mov	eax, DWORD PTR -44[ebp]
	mov	eax, DWORD PTR 32[eax]
	cmp	DWORD PTR -48[ebp], eax
	jge	.L186
	mov	eax, DWORD PTR -44[ebp]
	mov	eax, DWORD PTR 32[eax]
	sub	esp, 4
	push	eax
	push	DWORD PTR -48[ebp]
	lea	eax, .LC58@GOTOFF[ebx]
	push	eax
	call	printf@PLT
	add	esp, 16
	sub	esp, 8
	push	DWORD PTR -40[ebp]
	push	DWORD PTR -44[ebp]
	call	free_global_memory
	add	esp, 16
	mov	eax, -1
	jmp	.L135
.L186:
	cmp	DWORD PTR -48[ebp], 0
	je	.L187
	cmp	DWORD PTR -48[ebp], -1
	jne	.L185
.L187:
	mov	eax, DWORD PTR -44[ebp]
	mov	eax, DWORD PTR 8[eax]
	sub	esp, 8
	push	256
	push	eax
	call	x_strnlen@PLT
	add	esp, 16
	mov	DWORD PTR -48[ebp], eax
	cmp	DWORD PTR -48[ebp], 7
	jle	.L188
	cmp	DWORD PTR -48[ebp], 256
	jg	.L188
	mov	eax, DWORD PTR -44[ebp]
	mov	esi, DWORD PTR 32[eax]
	mov	eax, DWORD PTR -44[ebp]
	mov	ecx, DWORD PTR 28[eax]
	mov	edx, DWORD PTR -48[ebp]
	mov	eax, DWORD PTR -44[ebp]
	mov	eax, DWORD PTR 8[eax]
	sub	esp, 12
	push	esi
	push	ecx
	push	edx
	push	eax
	push	DWORD PTR -44[ebp]
	call	KDFCLOMUL
	add	esp, 32
	sub	esp, 12
	lea	eax, .LC59@GOTOFF[ebx]
	push	eax
	call	puts@PLT
	add	esp, 16
	jmp	.L185
.L188:
	sub	esp, 8
	push	DWORD PTR -40[ebp]
	push	DWORD PTR -44[ebp]
	call	free_global_memory
	add	esp, 16
	mov	eax, DWORD PTR stderr@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	sub	esp, 4
	push	DWORD PTR -48[ebp]
	lea	edx, .LC60@GOTOFF[ebx]
	push	edx
	push	eax
	call	fprintf@PLT
	add	esp, 16
	mov	eax, -1
	jmp	.L135
.L185:
	mov	eax, DWORD PTR -44[ebp]
	mov	eax, DWORD PTR 32[eax]
	sal	eax, 3
	sub	esp, 8
	push	eax
	lea	eax, .LC61@GOTOFF[ebx]
	push	eax
	call	printf@PLT
	add	esp, 16
	mov	eax, DWORD PTR -44[ebp]
	mov	eax, DWORD PTR 16777284[eax]
	cmp	eax, 4
	ja	.L189
	sal	eax, 2
	mov	eax, DWORD PTR .L191@GOTOFF[eax+ebx]
	add	eax, ebx
	jmp	eax
	.section	.rodata
	.align 4
	.align 4
.L191:
	.long	.L195@GOTOFF
	.long	.L194@GOTOFF
	.long	.L193@GOTOFF
	.long	.L192@GOTOFF
	.long	.L190@GOTOFF
	.text
.L195:
	mov	eax, DWORD PTR -44[ebp]
	mov	DWORD PTR 24[eax], 16
	mov	eax, DWORD PTR AES_Rounds@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	add	eax, 1
	sal	eax, 4
	mov	DWORD PTR -32[ebp], eax
	jmp	.L189
.L194:
	mov	eax, DWORD PTR -44[ebp]
	mov	DWORD PTR 24[eax], 16
	mov	DWORD PTR -32[ebp], 1620
	jmp	.L189
.L193:
	mov	eax, DWORD PTR -44[ebp]
	mov	DWORD PTR 24[eax], 16
	mov	DWORD PTR -32[ebp], 4260
	jmp	.L189
.L192:
	mov	eax, DWORD PTR -44[ebp]
	mov	DWORD PTR 24[eax], 8
	mov	DWORD PTR -32[ebp], 4168
	jmp	.L189
.L190:
	mov	eax, DWORD PTR -44[ebp]
	mov	edx, DWORD PTR 32[eax]
	mov	eax, DWORD PTR -44[ebp]
	mov	DWORD PTR 24[eax], edx
	mov	DWORD PTR -32[ebp], 168
	nop
.L189:
	mov	eax, DWORD PTR -44[ebp]
	mov	eax, DWORD PTR 24[eax]
	sub	esp, 8
	push	1
	push	eax
	call	calloc@PLT
	add	esp, 16
	mov	edx, eax
	mov	eax, DWORD PTR -44[ebp]
	mov	DWORD PTR 20[eax], edx
	mov	eax, DWORD PTR -44[ebp]
	mov	eax, DWORD PTR 20[eax]
	test	eax, eax
	jne	.L196
	sub	esp, 8
	push	DWORD PTR -40[ebp]
	push	DWORD PTR -44[ebp]
	call	free_global_memory
	add	esp, 16
	mov	eax, DWORD PTR stderr@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	push	eax
	push	28
	push	1
	lea	eax, .LC29@GOTOFF[ebx]
	push	eax
	call	fwrite@PLT
	add	esp, 16
	mov	eax, -1
	jmp	.L135
.L196:
	mov	eax, DWORD PTR -44[ebp]
	mov	eax, DWORD PTR 16777280[eax]
	test	eax, eax
	jne	.L197
	sub	esp, 12
	push	0
	call	time@PLT
	add	esp, 16
	sub	esp, 12
	push	eax
	call	srand@PLT
	add	esp, 16
	mov	eax, DWORD PTR -44[ebp]
	mov	edx, DWORD PTR 24[eax]
	mov	eax, DWORD PTR -44[ebp]
	mov	eax, DWORD PTR 20[eax]
	sub	esp, 8
	push	edx
	push	eax
	call	vector_init
	add	esp, 16
	mov	edx, DWORD PTR -44[ebp]
	mov	edx, DWORD PTR 24[edx]
	sub	edx, 2
	cmp	eax, edx
	jnb	.L197
	sub	esp, 8
	push	DWORD PTR -40[ebp]
	push	DWORD PTR -44[ebp]
	call	free_global_memory
	add	esp, 16
	mov	eax, DWORD PTR stderr@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	push	eax
	push	41
	push	1
	lea	eax, .LC62@GOTOFF[ebx]
	push	eax
	call	fwrite@PLT
	add	esp, 16
	mov	eax, -1
	jmp	.L135
.L197:
	mov	eax, DWORD PTR -44[ebp]
	mov	eax, DWORD PTR 16777284[eax]
	test	eax, eax
	jne	.L198
	sub	esp, 8
	push	1
	push	DWORD PTR -32[ebp]
	call	calloc@PLT
	add	esp, 16
	mov	DWORD PTR rijndael_ctx@GOTOFF[ebx], eax
	mov	eax, DWORD PTR rijndael_ctx@GOTOFF[ebx]
	mov	DWORD PTR -36[ebp], eax
	mov	eax, DWORD PTR rijndael_ctx@GOTOFF[ebx]
	test	eax, eax
	jne	.L199
	sub	esp, 8
	push	DWORD PTR -40[ebp]
	push	DWORD PTR -44[ebp]
	call	free_global_memory
	add	esp, 16
	mov	eax, DWORD PTR stderr@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	push	eax
	push	28
	push	1
	lea	eax, .LC29@GOTOFF[ebx]
	push	eax
	call	fwrite@PLT
	add	esp, 16
	mov	eax, -1
	jmp	.L135
.L199:
	mov	eax, DWORD PTR -44[ebp]
	mov	eax, DWORD PTR 32[eax]
	sal	eax, 3
	mov	ecx, eax
	mov	eax, DWORD PTR -44[ebp]
	mov	edx, DWORD PTR 28[eax]
	mov	eax, DWORD PTR rijndael_ctx@GOTOFF[ebx]
	sub	esp, 4
	push	ecx
	push	edx
	push	eax
	call	rijndael_key_encrypt_init@PLT
	add	esp, 16
	mov	edx, DWORD PTR AES_Rounds@GOT[ebx]
	mov	DWORD PTR [edx], eax
	jmp	.L200
.L198:
	mov	eax, DWORD PTR -44[ebp]
	mov	eax, DWORD PTR 16777284[eax]
	cmp	eax, 2
	jne	.L201
	sub	esp, 8
	push	DWORD PTR -32[ebp]
	push	1
	call	calloc@PLT
	add	esp, 16
	mov	DWORD PTR twofish_ctx@GOTOFF[ebx], eax
	mov	eax, DWORD PTR twofish_ctx@GOTOFF[ebx]
	mov	DWORD PTR -36[ebp], eax
	mov	eax, DWORD PTR twofish_ctx@GOTOFF[ebx]
	test	eax, eax
	jne	.L202
	sub	esp, 8
	push	DWORD PTR -40[ebp]
	push	DWORD PTR -44[ebp]
	call	free_global_memory
	add	esp, 16
	mov	eax, DWORD PTR stderr@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	push	eax
	push	28
	push	1
	lea	eax, .LC29@GOTOFF[ebx]
	push	eax
	call	fwrite@PLT
	add	esp, 16
	mov	eax, -1
	jmp	.L135
.L202:
	mov	eax, DWORD PTR -44[ebp]
	mov	eax, DWORD PTR 32[eax]
	mov	ecx, eax
	mov	eax, DWORD PTR -44[ebp]
	mov	edx, DWORD PTR 28[eax]
	mov	eax, DWORD PTR twofish_ctx@GOTOFF[ebx]
	sub	esp, 4
	push	ecx
	push	edx
	push	eax
	call	twofish_init@PLT
	add	esp, 16
	jmp	.L200
.L201:
	mov	eax, DWORD PTR -44[ebp]
	mov	eax, DWORD PTR 16777284[eax]
	cmp	eax, 1
	jne	.L203
	sub	esp, 8
	push	DWORD PTR -32[ebp]
	push	1
	call	calloc@PLT
	add	esp, 16
	mov	DWORD PTR serpent_ctx@GOTOFF[ebx], eax
	mov	eax, DWORD PTR serpent_ctx@GOTOFF[ebx]
	mov	DWORD PTR -36[ebp], eax
	mov	eax, DWORD PTR serpent_ctx@GOTOFF[ebx]
	test	eax, eax
	jne	.L204
	sub	esp, 8
	push	DWORD PTR -40[ebp]
	push	DWORD PTR -44[ebp]
	call	free_global_memory
	add	esp, 16
	mov	eax, DWORD PTR stderr@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	push	eax
	push	28
	push	1
	lea	eax, .LC29@GOTOFF[ebx]
	push	eax
	call	fwrite@PLT
	add	esp, 16
	mov	eax, -1
	jmp	.L135
.L204:
	mov	eax, DWORD PTR -44[ebp]
	mov	edx, DWORD PTR 28[eax]
	mov	eax, DWORD PTR -44[ebp]
	mov	eax, DWORD PTR 32[eax]
	sal	eax, 3
	mov	ecx, eax
	mov	eax, DWORD PTR serpent_ctx@GOTOFF[ebx]
	sub	esp, 4
	push	edx
	push	ecx
	push	eax
	call	serpent_init@PLT
	add	esp, 16
	jmp	.L200
.L203:
	mov	eax, DWORD PTR -44[ebp]
	mov	eax, DWORD PTR 16777284[eax]
	cmp	eax, 3
	jne	.L205
	sub	esp, 8
	push	DWORD PTR -32[ebp]
	push	1
	call	calloc@PLT
	add	esp, 16
	mov	DWORD PTR blowfish_ctx@GOTOFF[ebx], eax
	mov	eax, DWORD PTR blowfish_ctx@GOTOFF[ebx]
	mov	DWORD PTR -36[ebp], eax
	mov	eax, DWORD PTR blowfish_ctx@GOTOFF[ebx]
	test	eax, eax
	jne	.L206
	sub	esp, 8
	push	DWORD PTR -40[ebp]
	push	DWORD PTR -44[ebp]
	call	free_global_memory
	add	esp, 16
	mov	eax, DWORD PTR stderr@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	push	eax
	push	28
	push	1
	lea	eax, .LC29@GOTOFF[ebx]
	push	eax
	call	fwrite@PLT
	add	esp, 16
	mov	eax, -1
	jmp	.L135
.L206:
	mov	eax, DWORD PTR -44[ebp]
	mov	eax, DWORD PTR 32[eax]
	mov	ecx, eax
	mov	eax, DWORD PTR -44[ebp]
	mov	edx, DWORD PTR 28[eax]
	mov	eax, DWORD PTR blowfish_ctx@GOTOFF[ebx]
	sub	esp, 4
	push	ecx
	push	edx
	push	eax
	call	blowfish_init@PLT
	add	esp, 16
	jmp	.L200
.L205:
	mov	eax, DWORD PTR -44[ebp]
	mov	eax, DWORD PTR 16777284[eax]
	cmp	eax, 4
	jne	.L200
	sub	esp, 8
	push	DWORD PTR -32[ebp]
	push	1
	call	calloc@PLT
	add	esp, 16
	mov	DWORD PTR threefish_ctx@GOTOFF[ebx], eax
	mov	eax, DWORD PTR threefish_ctx@GOTOFF[ebx]
	mov	DWORD PTR -36[ebp], eax
	mov	eax, DWORD PTR threefish_ctx@GOTOFF[ebx]
	test	eax, eax
	jne	.L207
	sub	esp, 8
	push	DWORD PTR -40[ebp]
	push	DWORD PTR -44[ebp]
	call	free_global_memory
	add	esp, 16
	mov	eax, DWORD PTR stderr@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	push	eax
	push	28
	push	1
	lea	eax, .LC29@GOTOFF[ebx]
	push	eax
	call	fwrite@PLT
	add	esp, 16
	mov	eax, -1
	jmp	.L135
.L207:
	mov	eax, DWORD PTR -44[ebp]
	mov	ecx, DWORD PTR 28[eax]
	mov	eax, DWORD PTR -44[ebp]
	mov	edx, DWORD PTR 28[eax]
	mov	eax, DWORD PTR -44[ebp]
	mov	eax, DWORD PTR 32[eax]
	lea	esi, 0[0+eax*8]
	mov	eax, DWORD PTR threefish_ctx@GOTOFF[ebx]
	push	ecx
	push	edx
	push	esi
	push	eax
	call	threefish_init@PLT
	add	esp, 16
.L200:
	mov	eax, DWORD PTR -44[ebp]
	mov	eax, DWORD PTR 16777284[eax]
	mov	eax, DWORD PTR ALGORITM_NAME@GOTOFF[ebx+eax*4]
	sub	esp, 8
	push	eax
	lea	eax, .LC63@GOTOFF[ebx]
	push	eax
	call	printf@PLT
	add	esp, 16
	mov	eax, DWORD PTR -44[ebp]
	mov	esi, DWORD PTR [eax]
	mov	eax, DWORD PTR -44[ebp]
	mov	eax, DWORD PTR 16777280[eax]
	sub	esp, 12
	push	eax
	call	operation_variant
	add	esp, 16
	mov	eax, DWORD PTR OPERATION_NAME@GOTOFF[ebx+eax*4]
	sub	esp, 4
	push	esi
	push	eax
	lea	eax, .LC64@GOTOFF[ebx]
	push	eax
	call	printf@PLT
	add	esp, 16
	sub	esp, 12
	push	DWORD PTR -44[ebp]
	call	filecrypt
	add	esp, 16
	mov	DWORD PTR -52[ebp], eax
	mov	eax, DWORD PTR -52[ebp]
	add	eax, 8
	cmp	eax, 8
	ja	.L208
	sal	eax, 2
	mov	eax, DWORD PTR .L210@GOTOFF[eax+ebx]
	add	eax, ebx
	jmp	eax
	.section	.rodata
	.align 4
	.align 4
.L210:
	.long	.L218@GOTOFF
	.long	.L217@GOTOFF
	.long	.L216@GOTOFF
	.long	.L215@GOTOFF
	.long	.L214@GOTOFF
	.long	.L213@GOTOFF
	.long	.L212@GOTOFF
	.long	.L211@GOTOFF
	.long	.L209@GOTOFF
	.text
.L209:
	mov	eax, DWORD PTR -44[ebp]
	mov	esi, DWORD PTR [eax]
	mov	eax, DWORD PTR -44[ebp]
	mov	eax, DWORD PTR 16777280[eax]
	sub	esp, 12
	push	eax
	call	operation_variant
	add	esp, 16
	mov	eax, DWORD PTR OPERATION_NAME@GOTOFF[ebx+eax*4]
	sub	esp, 4
	push	esi
	push	eax
	lea	eax, .LC65@GOTOFF[ebx]
	push	eax
	call	printf@PLT
	add	esp, 16
	jmp	.L208
.L211:
	mov	eax, DWORD PTR -44[ebp]
	mov	esi, DWORD PTR [eax]
	mov	eax, DWORD PTR -44[ebp]
	mov	eax, DWORD PTR 16777280[eax]
	sub	esp, 12
	push	eax
	call	operation_variant
	add	esp, 16
	mov	edx, DWORD PTR OPERATION_NAME@GOTOFF[ebx+eax*4]
	mov	eax, DWORD PTR stderr@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	push	esi
	push	edx
	lea	edx, .LC66@GOTOFF[ebx]
	push	edx
	push	eax
	call	fprintf@PLT
	add	esp, 16
	jmp	.L208
.L212:
	mov	eax, DWORD PTR -44[ebp]
	mov	edx, DWORD PTR 4[eax]
	mov	eax, DWORD PTR stderr@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	sub	esp, 4
	push	edx
	lea	edx, .LC67@GOTOFF[ebx]
	push	edx
	push	eax
	call	fprintf@PLT
	add	esp, 16
	jmp	.L208
.L213:
	mov	eax, DWORD PTR -44[ebp]
	mov	edx, DWORD PTR [eax]
	mov	eax, DWORD PTR stderr@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	sub	esp, 4
	push	edx
	lea	edx, .LC68@GOTOFF[ebx]
	push	edx
	push	eax
	call	fprintf@PLT
	add	esp, 16
	jmp	.L208
.L214:
	mov	eax, DWORD PTR -44[ebp]
	mov	edx, DWORD PTR 4[eax]
	mov	eax, DWORD PTR stderr@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	sub	esp, 4
	push	edx
	lea	edx, .LC69@GOTOFF[ebx]
	push	edx
	push	eax
	call	fprintf@PLT
	add	esp, 16
	jmp	.L208
.L215:
	mov	eax, DWORD PTR -44[ebp]
	mov	edx, DWORD PTR [eax]
	mov	eax, DWORD PTR stderr@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	sub	esp, 4
	push	edx
	lea	edx, .LC70@GOTOFF[ebx]
	push	edx
	push	eax
	call	fprintf@PLT
	add	esp, 16
	jmp	.L208
.L216:
	mov	eax, DWORD PTR stderr@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	push	eax
	push	30
	push	1
	lea	eax, .LC71@GOTOFF[ebx]
	push	eax
	call	fwrite@PLT
	add	esp, 16
	jmp	.L208
.L217:
	mov	eax, DWORD PTR stderr@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	push	eax
	push	31
	push	1
	lea	eax, .LC72@GOTOFF[ebx]
	push	eax
	call	fwrite@PLT
	add	esp, 16
	jmp	.L208
.L218:
	mov	eax, DWORD PTR -44[ebp]
	mov	edx, DWORD PTR [eax]
	mov	eax, DWORD PTR stderr@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	sub	esp, 4
	push	edx
	lea	edx, .LC73@GOTOFF[ebx]
	push	edx
	push	eax
	call	fprintf@PLT
	add	esp, 16
	nop
.L208:
	sub	esp, 8
	push	DWORD PTR -32[ebp]
	push	DWORD PTR -36[ebp]
	call	cipher_free
	add	esp, 16
	sub	esp, 8
	push	DWORD PTR -40[ebp]
	push	DWORD PTR -44[ebp]
	call	free_global_memory
	add	esp, 16
	mov	eax, 0
.L135:
	lea	esp, -12[ebp]
	pop	ecx
	.cfi_restore 1
	.cfi_def_cfa 1, 0
	pop	ebx
	.cfi_restore 3
	pop	esi
	.cfi_restore 6
	pop	ebp
	.cfi_restore 5
	lea	esp, -4[ecx]
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE19:
	.size	main, .-main
	.section	.rodata
	.align 8
.LC14:
	.long	0
	.long	1079574528
	.align 8
.LC15:
	.long	-1717986918
	.long	1069128089
	.section	.text.__x86.get_pc_thunk.ax,"axG",@progbits,__x86.get_pc_thunk.ax,comdat
	.globl	__x86.get_pc_thunk.ax
	.hidden	__x86.get_pc_thunk.ax
	.type	__x86.get_pc_thunk.ax, @function
__x86.get_pc_thunk.ax:
.LFB20:
	.cfi_startproc
	mov	eax, DWORD PTR [esp]
	ret
	.cfi_endproc
.LFE20:
	.section	.text.__x86.get_pc_thunk.bx,"axG",@progbits,__x86.get_pc_thunk.bx,comdat
	.globl	__x86.get_pc_thunk.bx
	.hidden	__x86.get_pc_thunk.bx
	.type	__x86.get_pc_thunk.bx, @function
__x86.get_pc_thunk.bx:
.LFB21:
	.cfi_startproc
	mov	ebx, DWORD PTR [esp]
	ret
	.cfi_endproc
.LFE21:
	.ident	"GCC: (Debian 10.2.1-6) 10.2.1 20210110"
	.section	.note.GNU-stack,"",@progbits
