
// linker memmap places bss symbols at start/end of bss
extern int __bss_start__, __bss_end__;
extern int _vectors, _vectors_end;

extern void main();

// called from the assembly in start.s
void _cstart(void) {
    int *bss = &__bss_start__;
    int *bss_end = &__bss_end__;

    while (bss < bss_end) {
        *bss++ = 0;
    }

    static int * const RPI_INTERRUPT_VECTOR_BASE = 0x0;
    
    // copy in interrupt vector table and FIQ handler at end of table
    int* vectorsdst = RPI_INTERRUPT_VECTOR_BASE;
    int* vectors = &_vectors;
    int* vectors_end = &_vectors_end;
    while (vectors < vectors_end) {
        *vectorsdst++ = *vectors++;
    }

    main();

    // turn on the green ACT LED
    volatile unsigned int *GPIO_FSEL4  = (unsigned int *)0x20200010;
    volatile unsigned int *GPIO_SET1 =   (unsigned int *)0x20200020;
    *GPIO_FSEL4 = (1 << ((47-40)*3));
    *GPIO_SET1 = (1 << (47-32));
}