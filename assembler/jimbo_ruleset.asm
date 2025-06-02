#subruledef r1r2
{
	aa => 0x0
	ba => 0x1
	ca => 0x2
	ab => 0x3
	bb => 0x4
	cb => 0x5
	ac => 0x6
	bc => 0x7
	cc => 0x8
}
#subruledef imm8r1
{
	a => 0x9
	b => 0xa
	c => 0xb
}
#subruledef r1
{
	a => 0x0
	b => 0x1
	c => 0x2
}
#subruledef mr1
{
	ma => 0x0
	mb => 0x1
	mc => 0x2
	am => 0x0
	bm => 0x1
	cm => 0x2
}
#subruledef imm16r1
{
	a => 0x3
	b => 0x4
	c => 0x5
}
#ruledef
{
	mov {r: r1r2} 			=> 0x0 @ r
	mov {value: u8}, {r: imm8r1} 	=> 0x0 @ r @ value
	ldr {r: mr1} 			=> 0x1 @ r
	ldr {value: u16}, {r: imm16r1} 	=> 0x1 @ r @ value
	str {r: mr1} 			=> 0x2 @ r
	str {r: imm16r1}, {value: u16} 	=> 0x2 @ r @ value
	add {r: r1r2} 			=> 0x3 @ r
	add {r: imm8r1}, {value: u8}	=> 0x3 @ r @ value
	adc {r: r1r2} 			=> 0x4 @ r
	adc {r: imm8r1}, {value: u8}	=> 0x4 @ r @ value
	sub {r: r1r2} 			=> 0x5 @ r
	sub {r: imm8r1}, {value: u8}	=> 0x5 @ r @ value
	sbb {r: r1r2} 			=> 0x6 @ r
	sbb {r: imm8r1}, {value: u8}	=> 0x6 @ r @ value
	nand {r: r1r2} 			=> 0x7 @ r
	nand {r: imm8r1}, {value: u8}	=> 0x7 @ r @ value
	jmp 				=> 0x8 @ 0x0
	jmp {value: u16}		=> 0x8 @ 0x1 @ value
	jnz 				=> 0x9 @ 0x0
	jnz {value: u16}		=> 0x9 @ 0x1 @ value
	jnc 				=> 0xa @ 0x0
	jnc {value: u16}		=> 0xa @ 0x1 @ value
	out {r: r1}			=> 0xb @ r
	out {value: u8}			=> 0xb @ 0x3 @ value
	cot				=> 0xc @ 0x0
	hlt				=> 0xd @ 0x0
	mil {r: r1}			=> 0xe @ 0x0
	mil {value: u8}			=> 0xe @ 0x3 @ value
	mih {r: r1}			=> 0xf @ 0x0
	mih {value: u8}			=> 0xf @ 0x3 @ value
	NAME {value: u128}		=> value
}
