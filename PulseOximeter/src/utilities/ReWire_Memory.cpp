#include "ReWire_Memory.h"

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
    /// <summary>
    /// Calculates the size of the free list
    /// </summary>
    size_t GetFreeListSize() 
    {
        struct __freelist* current;
        size_t total = 0;
        for (current = (struct __freelist *) __malloc_free_list; current; current = current->nx) 
        {
            total += (size_t) current->sz;
        }

        return total;
    }    

    /// <summary>
    /// Calculates the number of bytes inbetween the stack pointer and the heap pointer.
    /// </summary>
    int GetFreeMemory() 
    {
        char top;
        return &top - reinterpret_cast<char*>(sbrk(0));
    }

    /// <summary>
    /// Returns the size of the stack (this assumes a SAMD21 microprocessor with 32 kB of SRAM).
    /// </summary>
    int GetStackSize ()
    {
        char top;
        int top_addr = reinterpret_cast<int>(&top);

        //The SAMD21G18A contains 32 KB of RAM. 32 KB = 32768 bytes = 0x8000 bytes. Thurs 0x8000 - 1 = 0x7FFF.
        //0x20007FFF is the base of the stack, and is the last memory address in SRAM on the SAMD21G18A.
        return (0x20007FFF - top_addr); 
    }

    /// <summary>
    /// Returns the address of the start of the heap, which is functionally equivalent to the size of the heap.
    /// </summary>
    int GetHeapSize ()
    {
        //0x20000000 is the first memory address in SRAM on the SAMD21G18A.
        return (reinterpret_cast<int>(sbrk(0)) - 0x20000000); 
    }

    void PrintMemoryMessage(bool end_with_newline)
    {
        unsigned long size_between_stack_and_heap;
        unsigned long freelist_size;
        unsigned long total_free_memory_available;
        unsigned long stack_size;
        unsigned long heap_size;
        unsigned long total_memory;
        float heap_fractured_percentage;        

        size_between_stack_and_heap = ReWire_Memory::GetFreeMemory();
        freelist_size = ReWire_Memory::GetFreeListSize();
        stack_size = ReWire_Memory::GetStackSize();
        heap_size = ReWire_Memory::GetHeapSize();
        total_free_memory_available = size_between_stack_and_heap + freelist_size;
        heap_fractured_percentage = 100.0f * (float(freelist_size) / float(heap_size));
        total_memory = size_between_stack_and_heap + stack_size + heap_size;

        Serial.print("[DEBUG] Memory: Stack size = ");
        Serial.print(stack_size);
        Serial.print(", Heap size = ");
        Serial.print(heap_size);
        Serial.print(", space between = ");
        Serial.print(size_between_stack_and_heap);
        Serial.print(", heap freelist size = ");
        Serial.print(freelist_size);
        Serial.print(", heap fractured = ");
        Serial.print(heap_fractured_percentage);

        if (end_with_newline)
        {
            Serial.println("");        
        }
    }    
}