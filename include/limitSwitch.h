#ifndef limitSwitch_h // Start of precompiler check to avoid dupicate inclusion of this code block.

#define limitSwitch_h // Precompiler macro used for precompiler check.

#include <main.h> // Header file for all libraries needed by this program.

const int8_t NO_SWITCH = 0; // No limit switch is pressed.
const int8_t FRONT_SWITCH = 1; // The front limit switch is pressed.
const int8_t BACK_SWITCH = 2; // The back limit switch is pressed.
int8_t memSwitch = 0; // Track what the switch was set to when last checked.

/**
 * @brief Set up the limit switches used to detect when the robot falls over.
 * @details The robot has front and  back limit switches that contact the 
 * ground when the robot leans too far forward or backward. This routine
 * initializes the GPIO pins connected to those limit switches as input and 
 * configures the pins with a weak pullup resistor.   
 * ==========================================================================*/
void setupLimitSwitches()
{
   Log.traceln("<setupLimitSwitches> Set weak pullup resistor for GPIO pin %d (forward limit switch) and %d (backward limit switch).", frontLimitSwitch, backLimitSwitch);
   pinMode(frontLimitSwitch, INPUT_PULLUP); // Set GPIO pin to input.
   pinMode(backLimitSwitch, INPUT_PULLUP); // Set GPIO pin to input.
} // setupLimitSwitches()

/**
 * @brief Check to see if the specified switch is contacting the ground.
==============================================================================*/
void checkLimitSwitches()
{
   if((digitalRead(frontLimitSwitch) == false)) // Front limit switch pressed?
   {
      if(memSwitch != FRONT_SWITCH) // Was not pressed during last check?
      {
         Log.verboseln("<checkLimitSwitches> Front limit switch tripped. memSwitch = %d", memSwitch);
         memSwitch = FRONT_SWITCH;
         saveRgbColour();
         setStdRgbColour(PINK);
         return;
      } //if
      else // Was pressed during last check. No need to do anything further.
      {
         return;
      } // else
   } //if
   if(digitalRead(backLimitSwitch) == false) // Back limit switch pressed?
   {
      if(memSwitch != BACK_SWITCH) // Was not pressed during last check?
      {
         Log.verboseln("<checkLimitSwitches> Back limit switch tripped. memSwitch = %d", memSwitch);
         memSwitch = BACK_SWITCH;
         saveRgbColour();
         setStdRgbColour(AQUA); // Temporarily set RGB LED to indicate robot lean
         return;
      } // if
      else // Was pressed during last check. No need to do anything further.
      {
         return;
      } // else
   } // if 
   if(memSwitch != NO_SWITCH) // No switched pressed this time nor the last time we checked. 
   {
      Log.verboseln("<checkLimitSwitches> No limit switch tripped. memSwitch = %d", memSwitch);
      memSwitch = NO_SWITCH; // Track the fact that now no bumper switches are pressed.
      loadRgbColour(); // Set RGB LED back to colour it was before the switches tripped.
   } // if
   return;
} // checkLimitSwitches()

#endif // End of precompiler protected code block