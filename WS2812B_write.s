
.include "xc.inc"

.text                       ;BP (put the following data in ROM(program memory))

; This is a library, thus it can *not* contain a _main function: the C file will
; define main().  However, we
; we will need a .global statement to make available ASM functions to C code.
; All functions utilized outside of this file will need to have a leading 
; underscore (_) and be included in a comment delimited list below.

.global _write_0, _write_1, _reset, _wait_100us, _wait_1ms
    
_write_0:
    ;push LATA
    inc LATA  ;1
    repeat #2  
    nop
    
    clr LATA
    repeat #12
    nop
    ;pop lATA
    return
    
_write_1:
    ;push LATA
    inc LATA
    repeat #8
    nop
    
    clr LATA
    nop
    ;pop LATA
    return
    
    
_reset:
    clr LATA
    nop
    return

_wait_100us:
    repeat #1595
    nop
    return

_wait_1ms:
    repeat #0x3E7A
    nop
    return
    