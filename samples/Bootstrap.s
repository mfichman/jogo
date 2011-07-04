	.section __DWARF,__debug_frame,regular,debug
Lsection__debug_frame:
	.section __DWARF,__debug_info,regular,debug
Lsection__debug_info:
	.section __DWARF,__debug_abbrev,regular,debug
Lsection__debug_abbrev:
	.section __DWARF,__debug_aranges,regular,debug
Lsection__debug_aranges:
	.section __DWARF,__debug_macinfo,regular,debug
Lsection__debug_macinfo:
	.section __DWARF,__debug_line,regular,debug
Lsection__debug_line:
	.section __DWARF,__debug_loc,regular,debug
Lsection__debug_loc:
	.section __DWARF,__debug_pubnames,regular,debug
Lsection__debug_pubnames:
	.section __DWARF,__debug_pubtypes,regular,debug
Lsection__debug_pubtypes:
	.section __DWARF,__debug_inlined,regular,debug
Lsection__debug_inlined:
	.section __DWARF,__debug_str,regular,debug
Lsection__debug_str:
	.section __DWARF,__debug_ranges,regular,debug
Lsection__debug_ranges:
	.section __DWARF,__debug_abbrev,regular,debug
Ldebug_abbrev0:
	.section __DWARF,__debug_info,regular,debug
Ldebug_info0:
	.section __DWARF,__debug_line,regular,debug
Ldebug_line0:
	.text
Ltext0:
.globl _Bootstrap_write
_Bootstrap_write:
LFB3:
	.file 1 "../runtime/Bootstrap.c"
	.loc 1 3 0
	pushl	%ebp
LCFI0:
	movl	%esp, %ebp
LCFI1:
	subl	$24, %esp
LCFI2:
	.loc 1 4 0
	movl	16(%ebp), %eax
	movl	%eax, 8(%esp)
	movl	12(%ebp), %eax
	movl	%eax, 4(%esp)
	movl	8(%ebp), %eax
	movl	%eax, (%esp)
	call	_write$UNIX2003
	.loc 1 5 0
	leave
	ret
LFE3:
.globl _Bootstrap_read
_Bootstrap_read:
LFB4:
	.loc 1 7 0
	pushl	%ebp
LCFI3:
	movl	%esp, %ebp
LCFI4:
	subl	$24, %esp
LCFI5:
	.loc 1 8 0
	movl	16(%ebp), %eax
	movl	%eax, 8(%esp)
	movl	12(%ebp), %eax
	movl	%eax, 4(%esp)
	movl	8(%ebp), %eax
	movl	%eax, (%esp)
	call	_read$UNIX2003
	.loc 1 9 0
	leave
	ret
LFE4:
	.section __DWARF,__debug_frame,regular,debug
Lframe0:
	.set L$set$0,LECIE0-LSCIE0
	.long L$set$0
LSCIE0:
	.long	0xffffffff
	.byte	0x1
	.ascii "\0"
	.byte	0x1
	.byte	0x7c
	.byte	0x8
	.byte	0xc
	.byte	0x4
	.byte	0x4
	.byte	0x88
	.byte	0x1
	.align 2
LECIE0:
LSFDE0:
	.set L$set$1,LEFDE0-LASFDE0
	.long L$set$1
LASFDE0:
	.set L$set$2,Lframe0-Lsection__debug_frame
	.long L$set$2
	.long	LFB3
	.set L$set$3,LFE3-LFB3
	.long L$set$3
	.byte	0x4
	.set L$set$4,LCFI0-LFB3
	.long L$set$4
	.byte	0xe
	.byte	0x8
	.byte	0x85
	.byte	0x2
	.byte	0x4
	.set L$set$5,LCFI1-LCFI0
	.long L$set$5
	.byte	0xd
	.byte	0x5
	.align 2
LEFDE0:
LSFDE2:
	.set L$set$6,LEFDE2-LASFDE2
	.long L$set$6
LASFDE2:
	.set L$set$7,Lframe0-Lsection__debug_frame
	.long L$set$7
	.long	LFB4
	.set L$set$8,LFE4-LFB4
	.long L$set$8
	.byte	0x4
	.set L$set$9,LCFI3-LFB4
	.long L$set$9
	.byte	0xe
	.byte	0x8
	.byte	0x85
	.byte	0x2
	.byte	0x4
	.set L$set$10,LCFI4-LCFI3
	.long L$set$10
	.byte	0xd
	.byte	0x5
	.align 2
LEFDE2:
	.text
Letext0:
	.section __DWARF,__debug_loc,regular,debug
Ldebug_loc0:
LLST0:
	.set L$set$11,LFB3-Ltext0
	.long L$set$11
	.set L$set$12,LCFI0-Ltext0
	.long L$set$12
	.word	0x2
	.byte	0x74
	.byte	0x4
	.set L$set$13,LCFI0-Ltext0
	.long L$set$13
	.set L$set$14,LCFI1-Ltext0
	.long L$set$14
	.word	0x2
	.byte	0x74
	.byte	0x8
	.set L$set$15,LCFI1-Ltext0
	.long L$set$15
	.set L$set$16,LFE3-Ltext0
	.long L$set$16
	.word	0x2
	.byte	0x75
	.byte	0x8
	.long	0x0
	.long	0x0
LLST1:
	.set L$set$17,LFB4-Ltext0
	.long L$set$17
	.set L$set$18,LCFI3-Ltext0
	.long L$set$18
	.word	0x2
	.byte	0x74
	.byte	0x4
	.set L$set$19,LCFI3-Ltext0
	.long L$set$19
	.set L$set$20,LCFI4-Ltext0
	.long L$set$20
	.word	0x2
	.byte	0x74
	.byte	0x8
	.set L$set$21,LCFI4-Ltext0
	.long L$set$21
	.set L$set$22,LFE4-Ltext0
	.long L$set$22
	.word	0x2
	.byte	0x75
	.byte	0x8
	.long	0x0
	.long	0x0
	.section __DWARF,__debug_info,regular,debug
	.long	0x1fd
	.word	0x2
	.set L$set$23,Ldebug_abbrev0-Lsection__debug_abbrev
	.long L$set$23
	.byte	0x4
	.byte	0x1
	.ascii "GNU C 4.2.1 (Apple Inc. build 5666) (dot 3)\0"
	.byte	0x1
	.ascii "../runtime/Bootstrap.c\0"
	.ascii "/Users/matt/Code/apollo/samples\0"
	.long	Ltext0
	.long	Letext0
	.set L$set$24,Ldebug_line0-Lsection__debug_line
	.long L$set$24
	.byte	0x2
	.byte	0x1
	.byte	0x6
	.ascii "signed char\0"
	.byte	0x2
	.byte	0x1
	.byte	0x8
	.ascii "unsigned char\0"
	.byte	0x2
	.byte	0x2
	.byte	0x5
	.ascii "short int\0"
	.byte	0x2
	.byte	0x2
	.byte	0x7
	.ascii "short unsigned int\0"
	.byte	0x2
	.byte	0x4
	.byte	0x5
	.ascii "int\0"
	.byte	0x2
	.byte	0x4
	.byte	0x7
	.ascii "unsigned int\0"
	.byte	0x2
	.byte	0x8
	.byte	0x5
	.ascii "long long int\0"
	.byte	0x2
	.byte	0x8
	.byte	0x7
	.ascii "long long unsigned int\0"
	.byte	0x2
	.byte	0x4
	.byte	0x5
	.ascii "long int\0"
	.byte	0x3
	.byte	0x4
	.byte	0x7
	.byte	0x2
	.byte	0x1
	.byte	0x6
	.ascii "char\0"
	.byte	0x2
	.byte	0x4
	.byte	0x7
	.ascii "long unsigned int\0"
	.byte	0x4
	.byte	0x1
	.set L$set$25,LASF0-Lsection__debug_str
	.long L$set$25
	.byte	0x1
	.byte	0x3
	.byte	0x1
	.long	0xbf
	.long	LFB3
	.long	LFE3
	.set L$set$26,LLST0-Lsection__debug_loc
	.long L$set$26
	.long	0x174
	.byte	0x5
	.ascii "fd\0"
	.byte	0x1
	.byte	0x3
	.long	0xbf
	.byte	0x2
	.byte	0x91
	.byte	0x0
	.byte	0x5
	.ascii "buf\0"
	.byte	0x1
	.byte	0x3
	.long	0x174
	.byte	0x2
	.byte	0x91
	.byte	0x4
	.byte	0x5
	.ascii "len\0"
	.byte	0x1
	.byte	0x3
	.long	0xbf
	.byte	0x2
	.byte	0x91
	.byte	0x8
	.byte	0x0
	.byte	0x6
	.byte	0x4
	.long	0x17a
	.byte	0x7
	.long	0x110
	.byte	0x4
	.byte	0x1
	.set L$set$27,LASF1-Lsection__debug_str
	.long L$set$27
	.byte	0x1
	.byte	0x7
	.byte	0x1
	.long	0xbf
	.long	LFB4
	.long	LFE4
	.set L$set$28,LLST1-Lsection__debug_loc
	.long L$set$28
	.long	0x1c6
	.byte	0x5
	.ascii "fd\0"
	.byte	0x1
	.byte	0x7
	.long	0xbf
	.byte	0x2
	.byte	0x91
	.byte	0x0
	.byte	0x5
	.ascii "buf\0"
	.byte	0x1
	.byte	0x7
	.long	0x1c6
	.byte	0x2
	.byte	0x91
	.byte	0x4
	.byte	0x5
	.ascii "len\0"
	.byte	0x1
	.byte	0x7
	.long	0xbf
	.byte	0x2
	.byte	0x91
	.byte	0x8
	.byte	0x0
	.byte	0x6
	.byte	0x4
	.long	0x110
	.byte	0x8
	.long	0xbf
	.long	0x1d7
	.byte	0x9
	.byte	0x0
	.byte	0xa
	.ascii "__CFConstantStringClassReference\0"
	.long	0x1cc
	.byte	0x1
	.byte	0x1
	.byte	0x1
	.byte	0x0
	.section __DWARF,__debug_abbrev,regular,debug
	.byte	0x1
	.byte	0x11
	.byte	0x1
	.byte	0x25
	.byte	0x8
	.byte	0x13
	.byte	0xb
	.byte	0x3
	.byte	0x8
	.byte	0x1b
	.byte	0x8
	.byte	0x11
	.byte	0x1
	.byte	0x12
	.byte	0x1
	.byte	0x10
	.byte	0x6
	.byte	0x0
	.byte	0x0
	.byte	0x2
	.byte	0x24
	.byte	0x0
	.byte	0xb
	.byte	0xb
	.byte	0x3e
	.byte	0xb
	.byte	0x3
	.byte	0x8
	.byte	0x0
	.byte	0x0
	.byte	0x3
	.byte	0x24
	.byte	0x0
	.byte	0xb
	.byte	0xb
	.byte	0x3e
	.byte	0xb
	.byte	0x0
	.byte	0x0
	.byte	0x4
	.byte	0x2e
	.byte	0x1
	.byte	0x3f
	.byte	0xc
	.byte	0x3
	.byte	0xe
	.byte	0x3a
	.byte	0xb
	.byte	0x3b
	.byte	0xb
	.byte	0x27
	.byte	0xc
	.byte	0x49
	.byte	0x13
	.byte	0x11
	.byte	0x1
	.byte	0x12
	.byte	0x1
	.byte	0x40
	.byte	0x6
	.byte	0x1
	.byte	0x13
	.byte	0x0
	.byte	0x0
	.byte	0x5
	.byte	0x5
	.byte	0x0
	.byte	0x3
	.byte	0x8
	.byte	0x3a
	.byte	0xb
	.byte	0x3b
	.byte	0xb
	.byte	0x49
	.byte	0x13
	.byte	0x2
	.byte	0xa
	.byte	0x0
	.byte	0x0
	.byte	0x6
	.byte	0xf
	.byte	0x0
	.byte	0xb
	.byte	0xb
	.byte	0x49
	.byte	0x13
	.byte	0x0
	.byte	0x0
	.byte	0x7
	.byte	0x26
	.byte	0x0
	.byte	0x49
	.byte	0x13
	.byte	0x0
	.byte	0x0
	.byte	0x8
	.byte	0x1
	.byte	0x1
	.byte	0x49
	.byte	0x13
	.byte	0x1
	.byte	0x13
	.byte	0x0
	.byte	0x0
	.byte	0x9
	.byte	0x21
	.byte	0x0
	.byte	0x0
	.byte	0x0
	.byte	0xa
	.byte	0x34
	.byte	0x0
	.byte	0x3
	.byte	0x8
	.byte	0x49
	.byte	0x13
	.byte	0x3f
	.byte	0xc
	.byte	0x34
	.byte	0xc
	.byte	0x3c
	.byte	0xc
	.byte	0x0
	.byte	0x0
	.byte	0x0
	.section __DWARF,__debug_pubnames,regular,debug
	.long	0x35
	.word	0x2
	.set L$set$29,Ldebug_info0-Lsection__debug_info
	.long L$set$29
	.long	0x201
	.long	0x12d
	.ascii "Bootstrap_write\0"
	.long	0x17f
	.ascii "Bootstrap_read\0"
	.long	0x0
	.section __DWARF,__debug_pubtypes,regular,debug
	.long	0xe
	.word	0x2
	.set L$set$30,Ldebug_info0-Lsection__debug_info
	.long L$set$30
	.long	0x201
	.long	0x0
	.section __DWARF,__debug_aranges,regular,debug
	.long	0x1c
	.word	0x2
	.set L$set$31,Ldebug_info0-Lsection__debug_info
	.long L$set$31
	.byte	0x4
	.byte	0x0
	.word	0x0
	.word	0x0
	.long	Ltext0
	.set L$set$32,Letext0-Ltext0
	.long L$set$32
	.long	0x0
	.long	0x0
	.section __DWARF,__debug_str,regular,debug
LASF1:
	.ascii "Bootstrap_read\0"
LASF0:
	.ascii "Bootstrap_write\0"
	.subsections_via_symbols
