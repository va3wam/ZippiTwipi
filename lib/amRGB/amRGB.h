/***************************************************************************************************
 * @file amRGB.h
 * @author va3wam
 * @brief Arduino library for controlling the colour of an RGB LED 
 * @details Specifically written for the Adafruit tri colour momentary on button 
 * @copyright Copyright (c) 2020 va3wam@gmail.com
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and 
 * associated documentation files (the "Software"), to deal in the Software without restriction, 
 * including without limitation the rights to use, copy, modify, merge, publish, distribute, 
 * sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is 
 * furnished to do so, subject to the following conditions: The above copyright notice and this 
 * permission notice shall be included in all copies or substantial portions of the Software.
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT 
 * NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND 
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, 
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 * 
 * YYYY-MM-DD Dev    Description
 * ---------- ------ -------------------------------------------------------------------------------
 * 2021-01-20 va3wam Program created
 ***************************************************************************************************/
#ifndef amRGB_h // Preprocessor include guard. See https://en.wikipedia.org/wiki/Include_guard
#define amRGB_h // Include guard variable
#define ledTimer 0 // LED on reset button controlled by timer0
#define ledFreq 1000000 // Micro seconds per timer0 (led timer) interrupt occurence (1 per second)

// Define structure and variables for reset buttons built-in LEDs. 
// https://microcontrollerslab.com/esp32-pwm-arduino-ide-led-fading-example/ 
typedef struct
{
   int pwmChannel; // There are 16 PWM channels available. Choose any channel between 0 and 15.
   int pwmFrequency; // Frequency of the digital signal
   int pwmResolution; // ESP32 PWM resolution between 1 bit to 16 bits. The optimal resolution is 8 bit.
   int pwmDutyCycle; // Up time of the PWM signal. Ranges from 0-255. 127 is a 50% duty cycle for example.
   int gpioPin; // GPIO pin connected to the LED  
}resetButtonLED; 
resetButtonLED redResetLED;
resetButtonLED blueResetLED;
resetButtonLED greenResetLED;

// Define amMD25 class
class amRGB
{
   public:
      amRGB(); // Constructor
      void bob(uint32_t timerClockSpeed, int resetRedLED, int resetBlueLED, int resetGreenLED, int frontLimitSwitch, int backLimitSwitch); // Clock speed of the hardware timers (MHz);
      void setResetButtonLEDColour(int redDutyCycle, int blueDutyCycle, int greenDutyCycle);
      void cycleLed(int interrupt0Counter);
//      uint32_t timerClockSpeed; // Clock speed of the hardware timers (MHz)
   private:
 
      int fadeAmount;
      int brightness;
      // Define structure and variables for hardware interrupt timer 
      volatile int interrupt0Counter; // Track how many times the interrupt has fired and not been processed
      int totalInterrupt0Counter; // Track how many times the interrupt has fired in total
      int currColourCnt; // Track what colour is currently active for the reset button LED
      hw_timer_t * timer0; // Pointer to hardware timer0
      portMUX_TYPE timer0Mux; // Used to prevent race conditins updating counters
};

#endif // End of preprocessor include guard.