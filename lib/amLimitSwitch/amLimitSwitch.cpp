/*************************************************************************************************************************************
 * @file amLimitSwitch.cpp
 * @author theAgingApprentice
 * @brief Monitor limit switches. 
 * @details Retrieve chip configuration details from both FreeRTOS and the ESP32 Arduino framework.
 * @copyright Copyright (c) 2021 the Aging Apprentice
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files 
 * (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, 
 * merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished 
 * to do so, subject to the following conditions:
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF 
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE 
 * FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN 
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 * 
 * YYYY-MM-DD Dev        Description
 * ---------- ---------- -------------------------------------------------------------------------------------------------------------
 * 2021-03-04 Old Squire Program created
 *************************************************************************************************************************************/
#include <Arduino.h> // Arduino Core for ESP32. Comes with Platform.io
#include <amLimitSwitch.h> // Arduino Core for ESP32. Comes with Platform.io
 
/**
 * @brief This is the constructor for this class.
===================================================================================================*/
amLimitSwitch::amLimitSwitch()
{
   pinMode(frontLimitSwitch,INPUT_PULLUP); // Set pin with front limit switch connected to it as input with an internal pullup resistor
   pinMode(backLimitSwitch,INPUT_PULLUP); // Set pin with back limit switch connected to it as input with an internal pullup resistor        
} //amLimitSwitch::amLimitSwitch()

/**
 * @brief Check to see if the specified switch is contacting the ground.
 * @param String switchName Front or Back
 * @return bool True/False
===================================================================================================*/
bool amLimitSwitch::check(int8_t switchName)
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
} //amLimitSwitch::check()
