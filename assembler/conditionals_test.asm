#include "jimbo_ruleset.asm"
init:
	mov 0x0, a
	add a, 0x0
	jnz jumped
	add a, 0x1
	jnz jumped
	mov 0x00, b
	hlt
jumped:
	mov 0xaa, b
	hlt

	
