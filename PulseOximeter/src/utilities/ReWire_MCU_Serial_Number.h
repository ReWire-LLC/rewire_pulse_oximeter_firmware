/*  ReWire_MCU_Serial_Number.h - copyright ReWire LLC, 2023

    This is adapted from code written by both David Pruitt and Drew Sloan
    for Vulintus devices.

    Microchip SAM-D SAM-E microcontrollers.
        - Each SAM microcontroller has a unique 128-bit serial number which is a
          concatenation of four 32-bit words.
        - The uniqueness of the serial number is guaranteed only when using all 
          128 bits.
        - The memory locations for each SAM variant are found in the following 
          datasheet sections:
            * SAMD11 -          Section 9.6.
            * SAMD21 -          Section 9.3.3.
            * SAML21 -          Section 11.5.
            * SAMD5x / SAME5x - Section 9.6.

*/

#ifndef __REWIRE_MCU_SERIAL_NUMBER_H__
#define __REWIRE_MCU_SERIAL_NUMBER_H__

#include <Arduino.h>

// OUTPUT FORMATS ****************************************************************************************************//
#ifndef REWIRE_SERIAL_FORMAT_HEX
  #define REWIRE_SERIAL_FORMAT_HEX                           16
#endif
#ifndef REWIRE_SERIAL_FORMAT_UUID                
  #define REWIRE_SERIAL_FORMAT_UUID                          36
#endif

#define UUID_CHAR_LEN                   (36u)
#define UUID_DASH_0                     (4u)
#define UUID_DASH_1                     (7u)
#define UUID_DASH_2                     (9u)
#define UUID_DASH_3                     (11u)

// MEMORY ADDRESSES **************************************************************************************************//
#define MCU_SERIALNUM_NUM_BYTES     (16u)
#define MCU_SERIALNUM_NUM_ADDR      (4u)
#define MCU_SERIALNUM_ADDR_0        *(volatile uint32_t *)(0x0080A00C)
#define MCU_SERIALNUM_ADDR_1        *(volatile uint32_t *)(0x0080A040)
#define MCU_SERIALNUM_ADDR_2        *(volatile uint32_t *)(0x0080A044)
#define MCU_SERIALNUM_ADDR_3        *(volatile uint32_t *)(0x0080A048)
/*
#if defined(__SAMD11__) || defined(__SAMD21__) || defined(__SAML21__)
    #define MCU_SERIALNUM_NUM_BYTES     (16u)
    #define MCU_SERIALNUM_NUM_ADDR      (4u)
    #define MCU_SERIALNUM_ADDR_0        *(volatile uint32_t *)(0x0080A00C)
    #define MCU_SERIALNUM_ADDR_1        *(volatile uint32_t *)(0x0080A040)
    #define MCU_SERIALNUM_ADDR_2        *(volatile uint32_t *)(0x0080A044)
    #define MCU_SERIALNUM_ADDR_3        *(volatile uint32_t *)(0x0080A048)
#elif defined(__SAMD51__) || defined(__SAME51__)
    #define MCU_SERIALNUM_NUM_BYTES     (16u)
    #define MCU_SERIALNUM_NUM_ADDR      (4u)
    #define MCU_SERIALNUM_ADDR_0        *(volatile uint32_t *)(0x008061FC)
    #define MCU_SERIALNUM_ADDR_1        *(volatile uint32_t *)(0x00806010)
    #define MCU_SERIALNUM_ADDR_2        *(volatile uint32_t *)(0x00806014)
    #define MCU_SERIALNUM_ADDR_3        *(volatile uint32_t *)(0x00806018)
#else
    #error Target MCU is not recognized in the serial number library.
#endif
*/

// NAMESPACE *********************************************************************************************************// 
namespace ReWire_MCU_Serial_Number
{
    void as_Bytes (uint8_t *buffer);
    void as_CString (char * const string_buffer);
    void as_CString (char * const string_buffer, uint8_t format);
    String as_String (void);
    String as_String (uint8_t format);
}

#endif      // #ifndef __REWIRE_MCU_SERIAL_NUMBER_H__