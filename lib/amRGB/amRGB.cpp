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

// Define structure and variables for hardware interrupt timer 
volatile int interrupt0Counter; // Track how many times the interrupt has fired and not been processed
int totalInterrupt0Counter; // Track how many times the interrupt has fired in total
int currColourCnt; // Track what colour is currently active for the reset button LED
hw_timer_t * timer0 = NULL; // Pointer to hardware timer0
portMUX_TYPE timer0Mux = portMUX_INITIALIZER_UNLOCKED; // Used to prevent race conditins updating counters
 
/**
 * @brief Hardware timer 0 interrupt handler function
 * @details Used to control flashing of LEDs on reset button
=================================================================================================== */
void IRAM_ATTR onTimer0() 
{
  portENTER_CRITICAL_ISR(&timer0Mux); // Use mux variable to prevent race condiiton with loop()
  interrupt0Counter++; 
  portEXIT_CRITICAL_ISR(&timer0Mux);
} //onTimer0()

/**
 * @brief This is the constructor for this class.
===================================================================================================*/
amRGB::amRGB()
{

} //amRGB::amRGB()

/**
 * @brief Configure pins that control  RGB LEDs on the reset button.
===================================================================================================*/
void amRGB::bob(uint32_t timerClockSpeed, int resetRedLED, int resetBlueLED, int resetGreenLED, int frontLimitSwitch, int backLimitSwitch) // Clock speed of the hardware timers (MHz)
{
   resetButtonLED redResetLED = {2, 500, 8, 0, resetRedLED}; // Chan 2, freq 4000Hz, 8 bit res, 255 tick up (100% duty cycle), red LED pin 
   resetButtonLED blueResetLED = {1, 500, 8, 0, resetBlueLED}; // Chan 1, freq 4000Hz, 8 bit res, 255 tick up (100% duty cycle), blue LED pin
   resetButtonLED greenResetLED = {0, 500, 8, 0, resetGreenLED}; // Chan 0, freq 4000Hz, 8 bit res, 255 tick up (100% duty cycle), green LED pin
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

   // Setup timer0
   timer0 = timerBegin(ledTimer, timerClockSpeed, true); // Set timer 0 to a 1 MHz frequency 
   timerAttachInterrupt(timer0, &onTimer0, true);
   timerAlarmWrite(timer0, ledFreq, true);
   timerAlarmEnable(timer0);
      
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

/**
 * @brief Cycle through LED colours 
 * @details Each call causes the colour to switch to the next colour in the cycle
=================================================================================================== */
void amRGB::cycleLed(int interrupt0Counter)
{
   if (interrupt0Counter > 0) 
   {
      portENTER_CRITICAL(&timer0Mux);
      interrupt0Counter--;
      portEXIT_CRITICAL(&timer0Mux);
      totalInterrupt0Counter++;
      switch(currColourCnt)
      {
         case 0: // RED
            currColourCnt ++;
            ledcWrite(redResetLED.pwmChannel, 0); // Set the duty cycle of PWM channel assigned to red LED
            ledcWrite(blueResetLED.pwmChannel, 255); // Set the duty cycle of PWM channel assigned to blue LED
            ledcWrite(greenResetLED.pwmChannel, 255); // Set the duty cycle of PWM channel assigned to green LED
            break;
         case 1: // BLUE
            currColourCnt ++;
            ledcWrite(redResetLED.pwmChannel, 255); // Set the duty cycle of PWM channel assigned to red LED
            ledcWrite(blueResetLED.pwmChannel, 0); // Set the duty cycle of PWM channel assigned to blue LED
            ledcWrite(greenResetLED.pwmChannel, 255); // Set the duty cycle of PWM channel assigned to green LED
            break;
         case 2: // GREEN
            currColourCnt ++;
            ledcWrite(redResetLED.pwmChannel, 255); // Set the duty cycle of PWM channel assigned to red LED
            ledcWrite(blueResetLED.pwmChannel, 255); // Set the duty cycle of PWM channel assigned to blue LED
            ledcWrite(greenResetLED.pwmChannel, 0); // Set the duty cycle of PWM channel assigned to green LED
            break;
         case 3: // PINK
            currColourCnt ++;
            ledcWrite(redResetLED.pwmChannel, 128); // Set the duty cycle of PWM channel assigned to red LED
            ledcWrite(blueResetLED.pwmChannel, 0); // Set the duty cycle of PWM channel assigned to blue LED
            ledcWrite(greenResetLED.pwmChannel, 255); // Set the duty cycle of PWM channel assigned to green LED
            break;
         case 4: // CYANNE
            currColourCnt ++;
            ledcWrite(redResetLED.pwmChannel, 255); // Set the duty cycle of PWM channel assigned to red LED
            ledcWrite(blueResetLED.pwmChannel, 0); // Set the duty cycle of PWM channel assigned to blue LED
            ledcWrite(greenResetLED.pwmChannel, 128); // Set the duty cycle of PWM channel assigned to green LED
            break;
         default: // WHITE
            currColourCnt = 0;
            ledcWrite(redResetLED.pwmChannel, 255); // Set the duty cycle of PWM channel assigned to red LED
            ledcWrite(blueResetLED.pwmChannel, 128); // Set the duty cycle of PWM channel assigned to blue LED
            ledcWrite(greenResetLED.pwmChannel, 128); // Set the duty cycle of PWM channel assigned to green LED
            break;
      } //switch
      Serial.print("An interrupt as occurred. Total number: ");
      Serial.println(totalInterrupt0Counter);
   } //if   
}