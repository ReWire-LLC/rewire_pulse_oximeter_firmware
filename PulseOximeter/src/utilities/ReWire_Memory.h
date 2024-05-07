#ifndef __REWIRE_MEMORY_H
#define __REWIRE_MEMORY_H

#include <Arduino.h>

#ifdef __arm__
// should use uinstd.h to define sbrk but Due causes a conflict
extern "C" char* sbrk(int incr);
#else  // __ARM__
extern char *__brkval;
#endif  // __arm__

/* The head of the free list structure */
extern uint32_t *__malloc_free_list;

namespace ReWire_Memory
{
    /*
    * The free list structure as maintained by the
    * avr-libc memory allocation routines.
    */
    struct __freelist
    {
        size_t sz;
        struct __freelist *nx;
    };

    /// <summary>
    /// Calculates the size of the free list
    /// </summary>
    size_t GetFreeListSize();

    /// <summary>
    /// Calculates the number of bytes inbetween the stack pointer and the heap pointer.
    /// </summary>
    int GetFreeMemory();

    /// <summary>
    /// Returns the size of the stack (this assumes a SAMD21 microprocessor with 32 kB of SRAM).
    /// </summary>
    int GetStackSize ();

    /// <summary>
    /// Returns the address of the start of the heap, which is functionally equivalent to the size of the heap.
    /// </summary>
    int GetHeapSize ();

    /// <summary>
    /// Prints a message to the serial line with general memory information
    /// </summary>
    void PrintMemoryMessage(bool end_with_newline = true);
}

#endif /* __REWIRE_MEMORY_H */