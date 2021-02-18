/***************************************************************************************************
 * @file amRGB.cpp
 * @author va3wam
 * @brief Arduino library for controlling the tri-colour LED on the rest button 
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
#include <Arduino.h> // Arduino Core for ESP32. Comes with Platform.io
#include <amRGB.h> // Required to control tri-colours LED on reset button 

int fadeAmount = 5; // How much to fade the LEDs by each loop
int brightness = 0; // How bright the LED is

/**
 * @brief This is the constructor for this class.
===================================================================================================*/
amRGB::amRGB()
{

} //amRGB::amRGB()

/**
 * @brief Configure pins that control  RGB LEDs on the reset button.
===================================================================================================*/
void amRGB::configure(int resetRedLED, int resetBlueLED, int resetGreenLED, int frontLimitSwitch, int backLimitSwitch) // Clock speed of the hardware timers (MHz)
{
   redResetLED = {2, 500, 8, 0, resetRedLED}; // Chan 2, freq 4000Hz, 8 bit res, 255 tick up (100% duty cycle), red LED pin 
   blueResetLED = {1, 500, 8, 0, resetBlueLED}; // Chan 1, freq 4000Hz, 8 bit res, 255 tick up (100% duty cycle), blue LED pin
   greenResetLED = {0, 500, 8, 0, resetGreenLED}; // Chan 0, freq 4000Hz, 8 bit res, 255 tick up (100% duty cycle), green LED pin
   // Setup rest button's red LED. For a good article about PWM see https://makeabilitylab.github.io/physcomp/esp32/led-fade.html
   ledcSetup(redResetLED.pwmChannel, redResetLED.pwmFrequency, redResetLED.pwmResolution); // Setup PWM channel for RED reset LED
   ledcAttachPin(redResetLED.gpioPin, redResetLED.pwmChannel); // Attach PWM channel to pin connected to reset button RED LED
   // Setup rest button's blue LED
   ledcSetup(blueResetLED.pwmChannel, blueResetLED.pwmFrequency, blueResetLED.pwmResolution); // Setup PWM channel for BLUE reset LED
   ledcAttachPin(blueResetLED.gpioPin, blueResetLED.pwmChannel); // Attach PWM channel to pin connected to reset button BLUE LED
   // Setup rest button's green LED
   ledcSetup(greenResetLED.pwmChannel, greenResetLED.pwmFrequency, greenResetLED.pwmResolution); // Setup PWM channel for GREEN reset LED
   ledcAttachPin(greenResetLED.gpioPin, greenResetLED.pwmChannel); // Attach PWM channel to pin connected to reset button GREEN LED
   // Setup balance limit switches
   pinMode(frontLimitSwitch,INPUT_PULLUP); // Set pin with front limit switch connected to it as input with an internal pullup resistor
   pinMode(backLimitSwitch,INPUT_PULLUP); // Set pin with back limit switch connected to it as input with an internal pullup resistor        
} //amRGB::configure()

/**
 * @brief Set the colour of the reset button's LED
=================================================================================================== */
void amRGB::setResetButtonLEDColour(int redDutyCycle, int blueDutyCycle, int greenDutyCycle)
{
   ledcWrite(redResetLED.pwmChannel, redDutyCycle); // Set the duty cycle of red LED PWM channel
   ledcWrite(blueResetLED.pwmChannel, blueDutyCycle); // Set the duty cycle of blue LED PWM channel
   ledcWrite(greenResetLED.pwmChannel, greenDutyCycle); // Set the duty cycle of green LED PWM channel
} //setResetButtonLEDColour()