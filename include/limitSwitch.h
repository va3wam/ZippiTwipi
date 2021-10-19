#ifndef limitSwitch_h // Start of precompiler check to avoid dupicate inclusion of this code block.

#define limitSwitch_h // Precompiler macro used for precompiler check.

#include <main.h> // Header file for all libraries needed by this program.
const bool frontSwitch = 0; // Indicates front limit switch
const bool backSwitch = 1; // Indicates back limit switch

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
 * @param String switchName Front or Back
 * @return bool True/False
===================================================================================================*/
bool checkSwitch(int8_t switchName)
{
   switch(switchName)
   {
      case frontSwitch: // If we are interested in the front limit switch
         return(digitalRead(frontLimitSwitch));
         break;
      default: // If we are interested in the back limit switch
         return(digitalRead(backLimitSwitch));
         break;
   } //switch
} // checkSwitch()

/**
 * @brief Check to see if the specified switch is contacting the ground.
===================================================================================================*/
void checkLimitSwitches()
{
//   int8_t redLedSetting; // Red LED inside reset switch RGB LED. Set value from 0 - 255.
//   int8_t greenLedSetting; // Green LED inside reset switch RGB LED. Set value from 0 - 255.
//   int8_t blueLedSetting; // Blue LED inside reset switch RGB LED. Set value from 0 - 255.

   if(checkSwitch(frontLimitSwitch) == false) // Front limit switch pressed turns LED xxx.
   {
      Log.verboseln("<checkLimitSwitches> Front limit switch tripped.");
      setStdRgbColour(PINK);
//      redLedSetting = 128; // set value from 0 - 255.
//      greenLedSetting = 255; // set value from 0 - 255.
//      blueLedSetting = 0; // set value from 0 - 255.
//      setCustRgbColour(redLedSetting, greenLedSetting, blueLedSetting); // Set Reset button's RGB LED colour.
      return;
   } //if
   
   if(checkSwitch(backLimitSwitch) == false) // Back limit switch pressed turns LED xxx.
   {
      Log.verboseln("<checkLimitSwitches> Back limit switch tripped.");
      setStdRgbColour(AQUA);
//      redLedSetting = 255; // set value from 0 - 255.
//      greenLedSetting = 128; // set value from 0 - 255.
//      blueLedSetting = 0; // set value from 0 - 255.
//      setCustRgbColour(redLedSetting, greenLedSetting, blueLedSetting); // Set Reset button's RGB LED colour.
      return;
   } // if

   // No limit switch pressed turns LED green.
//   Log.verboseln("<checkLimitSwitches> No limit switch tripped.");
   setStdRgbColour(GREEN); // Go back to old colour (may not  have been green).
//   redLedSetting = 0; // set value from 0 - 255.
//   greenLedSetting = 255; // set value from 0 - 255.
//   blueLedSetting = 0; // set value from 0 - 255.
//   setCustRgbColour(redLedSetting, greenLedSetting, blueLedSetting); // Set Reset button's RGB LED colour.
   return;
} // checkLimitSwitches()

#endif // End of precompiler protected code block