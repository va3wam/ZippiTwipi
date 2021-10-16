#ifndef statusLEDs_h // Start of precompiler check to avoid dupicate inclusion of this code block.

#define statusLEDs_h // Precompiler macro used for precompiler check.

#include <main.h> // Header file for all libraries needed by this program.
#include <amResetButton.h> // Required for controlling reset button LED. Includes use of one of the timers.

amResetButton resetButton; // Control the reset button's integrated RGB LED

/**
 * @brief Initialize the serial output.
 * ==========================================================================*/
void setupStatusLed()
{
   resetButton.setLedMode(ledModeCycle); // What behaviour you want for the LED (on, off, bink, cycle)
} //setupStatusLed()

/**
 * @brief Change status LED per settings.
 * ==========================================================================*/
void updateStatusLed()
{
   resetButton.updateLed(); // Handle the behaviour of the reset button LED
} //updateStatusLed()

#endif // End of precompiler protected code block