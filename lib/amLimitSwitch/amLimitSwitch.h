/*************************************************************************************************************************************
 * @file amLimitSwitch.h
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
#ifndef amLimitSwitch_h
#define amLimitSwitch_h
#include <Arduino.h> // Arduino Core for ESP32. Comes with Platform.io
#include <huzzah32_gpio_pins.h> // Pin definitions for Adafruit Huzzah32 development board 
#include <zippiTwipi_gpio_pins.h> // Pin definitions for ZippiTiwppi robot
#define frontSwitch 0 // Indicates front limit switch
#define backSwitch 1 // Indicates back limit switch

// Define amLimitSwitch class
class amLimitSwitch 
{
   public:
      amLimitSwitch(); // Constructor
      bool check(int8_t switchName); // Check if the specified limit switch is contacting the ground or not
   private:
}; //class amLimitSwitch

#endif