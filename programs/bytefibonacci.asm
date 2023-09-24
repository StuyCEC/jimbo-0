init:
	mih, 0x00
	mil, 0x0D
	cot
	out, 0x00
	mov a, 0x00
	mov b, 0x01
	mov c, 0x00
main:
	mov a, c
	add a, b
	out a
	mov c, b
	mov b, a
	jmp m
