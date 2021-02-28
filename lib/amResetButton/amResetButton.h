/*************************************************************************************************************************************
 * @file amResetButton.h
 * @author theAgingApprentice
 * @brief Adafruit reset button library for ESP32 Arduino 
 * @details Controls the RGB LED of the reset button.
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
 * 2021-02-27 Old Squire Added cfgToConsole()
 * 2021-02-24 Old Squire Program created
 *************************************************************************************************************************************/
#ifndef amResetButton_h
#define amResetButton_h

#define _numColoursSupported 7 // Number of colours LED can be set to
#define RED 0 // Reset button LED red 
#define GREEN 1 // Reset button LED green 
#define BLUE 2 // Reset button LED blue 
#define PINK 3 // Reset button LED pink 
#define CYANNE 4 // Reset button LED cyanne
#define AQUA 5 // Reset button LED aqua 
#define WHITE 6 // Reset button LED white 
#define ledTimer 0 // Timer that will be used to control LED on reset button
#define ledFreq 1000 // Micro seconds per timer0 (led timer) interrupt occurence (1 per second)
#define ledAutoReload true // Set to automatically restart the timer when it expires 

// Define structure reset buttons three built-in LEDs. 
// https://microcontrollerslab.com/esp32-pwm-arduino-ide-led-fading-example/ 
typedef struct
{
   int channel; // There are 16 PWM channels available. Choose any channel between 0 and 15.
   int frequency; // Frequency of the digital signal
   int resolution; // ESP32 PWM resolution between 1 bit to 16 bits. The optimal resolution is 8 bit.
   int gpioPin; // GPIO pin connected to the LED  
}struct_led; 

// Define structure for LCD colours 
typedef struct
{
   String name; // Name associated with colour properties
   int redDutyCycle; // Up time of the PWM signal. Ranges from 0-255. 127 is a 50% duty cycle for example.
   int greenDutyCycle; // Up time of the PWM signal. Ranges from 0-255. 127 is a 50% duty cycle for example.
   int blueDutyCycle; // Up time of the PWM signal. Ranges from 0-255. 127 is a 50% duty cycle for example.
}struct_Colour; 

// Define amI2C class
class amResetButton
{
   public:
      amResetButton(); // Constructor
      void cfgToConsole();
      void cycleColour(); // Advance to the next colour in the cycle
      void setColour(int8_t ledColour); // Set the colour of the LED on the reset button
      int8_t getCurClrNum(); // Return the current colour setting counter as an 8 bit integer 
      String getCurClrStr(); // Return the current colour setting counter as a String
      uint8_t getTmrClockNum();
      uint32_t getTmrClockSpeed();
      uint32_t getTmrFreq();
      bool getTmrAutoReload();
   private:
      void _resetColourCount(); // Reset the colour counter to 0
      int8_t _cycleCount; // Current count through colour cycle
      struct_led _redLED;
      struct_led _greenLED;
      struct_led _blueLED;
      struct_Colour _colour[_numColoursSupported]; // Array of colours structure indexed by _cycleCount 
};

#endif