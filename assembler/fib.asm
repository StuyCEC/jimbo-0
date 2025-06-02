#include "jimbo_ruleset.asm"
init:
	cot
	out 0x00
	mov 0x00, a
	mov 0x01, b
	mov 0x00, c
main:
	mov ca
	add ab
	mov bc
	mov ab
	out a
	jmp main
