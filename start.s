.globl _start
_start:
    mov r0, #0xD2  // interrupts
    msr cpsr_c, r0
    mov sp, #0x8000
    mov r0, #0xD1  // fast interrupts
    msr cpsr_c, r0       
    mov sp, #0x4000       
    mov fp, #0     // FIQ has fp
    mov r0, #0xD3  // supervisor
    msr cpsr_c, r0      
    mov sp, #0x8000000
    mov fp, #0
    bl _cstart

hang: b hang
