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
 * 2021-03-03 Old Squire Added timer logic using ESP32TimerInterrupt.h
 * 2021-02-27 Old Squire Added cfgToConsole()
 * 2021-02-24 Old Squire Program created
 *************************************************************************************************************************************/
#ifndef amResetButton_h // Preprocessor conditional compilation. Prevents re-declaration of types, enums and static variables.
#define amResetButton_h // Macro definition used by conditional compilation.

#define _numColoursSupported 7 // Number of colours LED can be set to
#define RED 0 // Reset button LED red 
#define GREEN 1 // Reset button LED green 
#define BLUE 2 // Reset button LED blue 
#define PINK 3 // Reset button LED pink 
#define CYANNE 4 // Reset button LED cyanne
#define AQUA 5 // Reset button LED aqua 
#define WHITE 6 // Reset button LED white 
#define BLACK 7 // Reset button LED white 
#define ledTimer 0 // Timer that will be used to control LED on reset button
#define ledFreq 1000 // Micro seconds per timer0 (led timer) interrupt occurence (1 per second)
#define ledAutoReload true // Set to automatically restart the timer when it expires 
#define ledModeOff 0 // Sets the behavior of the rest buttons LED to off
#define ledModeOn 1 // Sets the behavior of the rest buttons LED to blink one colour (see blinkColour()) 
#define ledModeBlink 2 // Sets the behavior of the rest buttons LED to blink one colour 
#define ledModeCycle 3 // Sets the behavior of the rest buttons LED to cycle through colours 
#define TIMER0_INTERVAL_MS 1000 // Frequency of timer event in milliseconds (1000ms = 1 second)
#define TIMER_INTERRUPT_DEBUG 0 // Required for ESP32TimerInterrupt library. Always set to 0
#define _TIMERINTERRUPT_LOGLEVEL_ 0 // Required for ESP32TimerInterrupt library. Always set to 0
#include <ESP32TimerInterrupt.h> //https://github.com/khoih-prog/ESP32TimerInterrupt/tree/master/examples.

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

// Define amResetButton class
class amResetButton : public ESP32TimerInterrupt // Indicate that we are extending ESP32TimerInterrupt class with our class
{
   public:
      amResetButton(); // Constructor
      void setLedMode(int8_t mode); // Determines the behavior of the LED
      void updateLed(); // Changes the LED based on what mode it is in
      int32_t getTmr0Cnt();
      void decrementTmr0Cnt();
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
      void _blinkLED(); // Blinks the reset buttons LED
      int8_t _ledColour; // What colour the LED should be while blinking or when solid on 
      int8_t _ledMode; // What mode the LED is in which determines its behaviour
      int8_t _cycleCount; // Current count through colour cycle
      bool _ledBlink; // Used to track blinking of LED
      struct_led _redLED; // Red LED structure
      struct_led _greenLED; // Green LED structure
      struct_led _blueLED; // Blue LED structure
      struct_Colour _colour[_numColoursSupported]; // Array of colours structure indexed by _cycleCount
};

#endif // End of preprocessor conditional compilation.