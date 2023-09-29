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

#include "ReWire_MCU_Serial_Number.h"

namespace ReWire_MCU_Serial_Number
{
    /// @brief Return the serial number as a byte array
    /// @param buffer the buffer in memory where the serial number will be placed
    void as_Bytes (uint8_t *buffer)
    {
        //Load the serial number addresses into an array.
        uint32_t serialnum_addr[4];           
        serialnum_addr[0] = MCU_SERIALNUM_ADDR_0;
        serialnum_addr[1] = MCU_SERIALNUM_ADDR_1;
        serialnum_addr[2] = MCU_SERIALNUM_ADDR_2;
        serialnum_addr[3] = MCU_SERIALNUM_ADDR_3;

        //Break the 32bit words into individual bytes.
        for (uint8_t i = 0; i < MCU_SERIALNUM_NUM_ADDR; i++)
        {
            buffer[i * 4 + 0] = (uint8_t)(serialnum_addr[i] >> 24);
            buffer[i * 4 + 1] = (uint8_t)(serialnum_addr[i] >> 16);
            buffer[i * 4 + 2] = (uint8_t)(serialnum_addr[i] >> 8);
            buffer[i * 4 + 3] = (uint8_t)(serialnum_addr[i] >> 0);
        }        
    }

    /// @brief Return the serial number as a hex C string, with format unspecified.
    /// @param string_buffer the buffer in memory where the serial number will be placed
    void as_CString (char * const string_buffer)
    {
        //Default to the hexadecimal ouput, with no UUID dashes.
        uint8_t format = REWIRE_SERIAL_FORMAT_HEX;
        as_CString(string_buffer, format);
    }

    /// @brief Return the serial number as a hex C string, with format specified.
    /// @param string_buffer the buffer in memory where the serial number will be placed
    /// @param format either REWIRE_SERIAL_FORMAT_HEX or REWIRE_SERIAL_FORMAT_UUID
    void as_CString (char * const string_buffer, uint8_t format)
    {
        //Instantiate the byte buffer
        uint8_t buffer[MCU_SERIALNUM_NUM_BYTES];

        //Get the serial number as bytes.
        as_Bytes(buffer);

        //Now convert the byte array to a C-string. The buffer must be 33-bytes exactly.
        for (uint8_t i = 0, j = 0; i < 16; i++)
        {
            //For the UUID format, place a "-" character before bytes (zero-based indexing) 4, 7, 9, and 11.
            if (format == REWIRE_SERIAL_FORMAT_UUID)
            {          
                if (i == UUID_DASH_0 || i == UUID_DASH_1 || i == UUID_DASH_2 || i == UUID_DASH_3)
                {
                    string_buffer[j++] = '-';
                }
            }

            //Note: we are processing 2 digits (one byte) in a single iteration of the loop.
            uint8_t current_byte = buffer[i];
            uint8_t low_nibble = current_byte & 0x0F;
            uint8_t high_nibble = (current_byte >> 4) & 0x0F;
            char base_char = 'A';               //User upper-case hexadecimal by default.
            if (format == REWIRE_SERIAL_FORMAT_UUID)                 
            {        
                base_char = 'a';                  //Use lower-case hexadecimal for UUID format.
            }

            string_buffer[j++] = (low_nibble < 10) ? '0' + low_nibble : (base_char - 10) + low_nibble;
            string_buffer[j++] = (high_nibble < 10) ? '0' + high_nibble : (base_char - 10) + high_nibble;
        }

        //Make absolutely sure that the final byte of the string buffer is the null byte
        if (format == REWIRE_SERIAL_FORMAT_UUID)                   
        {
            //UUID format has 37 characters (last character is null).
            string_buffer[36] = 0;              
        }
        else 
        {
            //Default hexadecimal format has 2 characters for each byte (last character is null).
            string_buffer[32] = 0;              
        } 
    }

    /// @brief Return the serial number as an Arduino string, with format unspecified.
    /// @param 
    /// @return An Arduino string object with the serial number using REWIRE_SERIAL_FORMAT_HEX as the format specifier
    String as_String (void)
    {
        //Default to the hexadecimal ouput, with no UUID dashes.
        uint8_t format = REWIRE_SERIAL_FORMAT_HEX;                 
        String result = as_String(format);
        return result;
    }

    /// @brief Return the serial number as an Arduino string, with format specified.
    /// @param format the format specifier, either REWIRE_SERIAL_FORMAT_HEX or REWIRE_SERIAL_FORMAT_UUID
    /// @return An Arduino string object with the serial number
    String as_String (uint8_t format)
    {
        //Instantiate a C-string buffer to hold the unique ID.
        //Create a buffer with 37 characters to hold the UUID format (we'll trim it down if otherwise).
        char string_buffer[37];  

        //Get the unique ID as a C-string
        as_CString(string_buffer, format);

        //Convert it to a C++ Arduino String object and return it to the caller
        String result = String(string_buffer);
        result.trim();
        return result;
    }
}
