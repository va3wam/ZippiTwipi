#ifndef setupSerial_h // Start of precompiler check to avoid dupicate inclusion of this code block.

#define setupSerial_h // Precompiler macro used for precompiler check.

#include <main.h> // Header file for all libraries needed by this program.
const unsigned long SERIAL_BAUD_RATE = 115200; // Serial terminal baud rate.

/**
 * @brief Initialize the serial output.
 * ==========================================================================*/
void setupSerial()
{
   Serial.begin(SERIAL_BAUD_RATE); // Open a serial at specified baud rate. 
   while(!Serial); // Wait for Serial port to be ready.
} //setupSerial()

#endif // End of precompiler protected code block