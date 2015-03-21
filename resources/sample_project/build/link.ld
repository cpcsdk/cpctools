MEMORY
{
	m: org = 0x0000, len = 0x10000
	b1: org = 0xc000, len = 0x4000
	b2: org = 0xc000, len = 0x4000
}

SECTIONS
{
	text : { *(.text) *(.data) *(.bss) } > m
	bank1 : { *(.bank1) } > b1
	bank2 : { *(.bank2) } > b2
}
