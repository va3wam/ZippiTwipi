/***************************************************************************************************
 * @file amResetButton.cpp
 * @author theAgingApprentice
 * @brief Adafruit reset button library for ESP32 Arduino 
 * @details Controls the RGB LED of the reset button.
 * @copyright Copyright (c) 2021 the Aging Apprentice
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
 * YYYY-MM-DD Dev        Description
 * ---------- ---------- ---------------------------------------------------------------------------
 * 2021-03-03 Old Squire Added timer logic using ESP32TimerInterrupt.h
 * 2021-02-24 Old Squire Program created
 ***************************************************************************************************/

#include <Arduino.h> // Arduino Core for ESP32. Comes with Platform.io
#include <soc/rtc.h> // Required for rtc_clk_apb_freq_get()
#include <amResetButton.h> // Header file for linking.
#include <zippy_gpio_pins.h> // Pin definitions for ZippiTiwppi robot

// Define variables shared between parent and child classes here
volatile uint32_t _interrupt0Counter;
portMUX_TYPE _timer0Mux = portMUX_INITIALIZER_UNLOCKED; // Serialize accress to code block
ESP32Timer ITimer0(0);

/**
 * @brief Hardware timer 0 interrupt handler function
 * @details Used to control flashing of LEDs on reset button
=================================================================================================== */
void IRAM_ATTR _onTimer0() 
{
   portENTER_CRITICAL_ISR(&_timer0Mux); // Use mux variable to prevent race condiiton with loop()
   _interrupt0Counter++; 
   portEXIT_CRITICAL_ISR(&_timer0Mux);
} //onTimer0()

/**
 * @brief This is the constructor for this class.
===================================================================================================*/
amResetButton::amResetButton() // Constructor for this class
   :ESP32TimerInterrupt() // Constructor for parent class Timer()
{
   _cycleCount = 0; // Current count through colour cycle. 
   // Set values for the three LEDs in the reset button 
   _redLED.channel = 0;
   _redLED.frequency = 500;
   _redLED.gpioPin = resetRedLED;
   _redLED.resolution = 8;
   _greenLED.channel = 1;
   _greenLED.frequency = 500;
   _greenLED.gpioPin = resetGreenLED;
   _greenLED.resolution = 8;
   _blueLED.channel = 2;
   _blueLED.frequency = 500;
   _blueLED.gpioPin = resetBlueLED;
   _blueLED.resolution = 8;

   // Set values for the colours to be used to set the reset button RGB LED
   _colour[RED].name = "red";
   _colour[RED].redDutyCycle = 0;
   _colour[RED].greenDutyCycle = 255;
   _colour[RED].blueDutyCycle = 255;
   _colour[GREEN].name = "green";
   _colour[GREEN].redDutyCycle = 255;
   _colour[GREEN].greenDutyCycle = 0;
   _colour[GREEN].blueDutyCycle = 255;
   _colour[BLUE].name = "blue";
   _colour[BLUE].redDutyCycle = 255;
   _colour[BLUE].greenDutyCycle = 255;
   _colour[BLUE].blueDutyCycle = 0;
   _colour[PINK].name = "pink";
   _colour[PINK].redDutyCycle = 128;
   _colour[PINK].greenDutyCycle = 255;
   _colour[PINK].blueDutyCycle = 0;
   _colour[CYANNE].name = "cyanne";
   _colour[CYANNE].redDutyCycle = 255;
   _colour[CYANNE].greenDutyCycle = 128;
   _colour[CYANNE].blueDutyCycle = 0;
   _colour[AQUA].name = "aqua";
   _colour[AQUA].redDutyCycle = 255;
   _colour[AQUA].greenDutyCycle = 128;
   _colour[AQUA].blueDutyCycle = 128;
   _colour[WHITE].name = "white";
   _colour[WHITE].redDutyCycle = 128;
   _colour[WHITE].greenDutyCycle = 128;
   _colour[WHITE].blueDutyCycle = 128;
   _colour[BLACK].name = "black";
   _colour[BLACK].redDutyCycle = 0;
   _colour[BLACK].greenDutyCycle = 0;
   _colour[BLACK].blueDutyCycle = 0;

   // Setup rest button LEDs. For a good article about PWM see https://makeabilitylab.github.io/physcomp/esp32/led-fade.html
   _ledMode = ledModeCycle; // Default LED behaviour is to cyccle through the colours
   ledcSetup(_redLED.channel, _redLED.frequency, _redLED.resolution); // Configure red LED PWM properties
   ledcAttachPin(_redLED.gpioPin, _redLED.channel); // Attach PWM channel to pin connected to red LED on reset button
   ledcSetup(_greenLED.channel, _greenLED.frequency, _greenLED.resolution); // Configure green LED PWM properties
   ledcAttachPin(_greenLED.gpioPin, _greenLED.channel); // Attach PWM channel to pin connected to green LED on reset button
   ledcSetup(_blueLED.channel, _blueLED.frequency, _blueLED.resolution); // Configure blue LED PWM properties
   ledcAttachPin(_blueLED.gpioPin, _blueLED.channel); // Attach PWM channel to pin connected to blue LED on reset button

   // Interval in microsecs
   if(ITimer0.attachInterruptInterval(TIMER0_INTERVAL_MS * 1000, _onTimer0)) // Attach timer to ISR
   {
      Serial.print(F("<amResetButton::amResetButton> Starting  ITimer0 OK, millis() = ")); Serial.println(millis());
   } //if
   else
   {
      Serial.println(F("<amResetButton::amResetButton> Can't set ITimer0. Select another freq. or timer"));
   } //else
} //amResetButton::amResetButton()

/**
 * @brief Sets the variable _ledMode which is used to control the behavior of the LED.
 * @param int8_t mode determines which mode (behaviour) the LED is set to  
===================================================================================================*/
void amResetButton::setLedMode(int8_t mode)
{
   _ledMode = mode;
   switch(_ledMode)
   {
      case ledModeOff: // If the LED mode is set to off, do nothing as it is already set
         setColour(BLUE);
         break;
      case ledModeOn: // If the LED mode is set to on, do nothing as it is already set
         setColour(_ledColour);
         break;
      case ledModeBlink: // If the LED mode is set to blink
         setColour(_ledColour);
         break;
      case ledModeCycle: // If the LED mode is set to cycle
         // Do nothing
         break;
      default: // If LED mode is unknown act as if the LED is off
         setColour(GREEN);
         break;
   } //switch
} //amResetButton::setLedMode()

/**
 * @brief Changes the LED based on what mode it is in.
===================================================================================================*/
void amResetButton::updateLed()
{
   uint32_t tmp = getTmr0Cnt();
   if(tmp > 0)
   {
      switch(_ledMode)
      {
         case ledModeOff: // If the LED mode is set to off, do nothing as it is already set
            break;
         case ledModeOn: // If the LED mode is set to on, do nothing as it is already set
            break;
         case ledModeBlink: // If the LED mode is set to blink
            _blinkLED();
            break;
         case ledModeCycle: // If the LED mode is set to cycle
            cycleColour();
            break;
         default: // If LED mode is unknown act as if the LED is off
            break;
      } //switch
      decrementTmr0Cnt();
   } //if
} //amResetButton::updateLed()

/**
 * @brief Reduces the variable that tracks unserviced interrupts for timer0 by 1. 
===================================================================================================*/
void amResetButton::decrementTmr0Cnt()
{
   portENTER_CRITICAL_ISR(&_timer0Mux); // Use mux variable to prevent race condiiton with loop()
   _interrupt0Counter --; 
   portEXIT_CRITICAL_ISR(&_timer0Mux);
} //amResetButton::decrementTmr0Cnt()

/**
 * @brief Returns the counter value for timer0. 
 * @return uint8_t _interrupt0Counter shows how many timer the timer has fired and not been serviced  
===================================================================================================*/
int32_t amResetButton::getTmr0Cnt()
{
   return _interrupt0Counter;
} //amResetButton::getTmr0Cnt()

/**
 * @brief Returns the number of the clock used to control the LED. 
 * @return uint8_t ledTimer  
===================================================================================================*/
uint8_t amResetButton::getTmrClockNum()
{
   return (uint32_t)ledTimer;
} //amResetButton::getTmrClockNum()

/**
 * @brief Returns the clock speed of the application core in Mhz.
 * @return uint32_t rtc_clk_apb_freq_get() / 1000000   
===================================================================================================*/
uint32_t amResetButton::getTmrClockSpeed()
{
   return rtc_clk_apb_freq_get() / 1000000; // Convert timer clock speed(Hz) to MHz by dividing by 1 million
} //amResetButton::getTmr0Clock()

/**
 * @brief Returns the LED interrupt frequency in Hz.
 * @return uint32_t ledFreq   
===================================================================================================*/
uint32_t amResetButton::getTmrFreq()
{
   return (uint32_t)ledFreq; 
} //amResetButton::getTmrFreq()

/**
 * @brief Returns the LED interrupt frequency in Hz.
 * @return uint32_t ledFreq   
===================================================================================================*/
bool amResetButton::getTmrAutoReload()
{
   return (bool) ledAutoReload; 
} //amResetButton::getTmrAutoReload()

/**
 * @brief Send detailed timer configuration information to the console.
 * @details The Adafruit Rugged Metal Pushbutton - 16mm 6V RGB Momentary is used for this robots 
 * reset button. This button has an integrated RGB LED. A timer ISR is used to control the various
 * behaviors of the LED. This function sends the details of the timer to the console.     
===================================================================================================*/
void amResetButton::cfgToConsole()
{
   Serial.println("<amResetButton::cfgToConsole> Reset button LED timer settings:");  
   Serial.print("<amResetButton::cfgToConsole> ... LED timer number (0-3) = "); Serial.println(getTmrClockNum());
   Serial.print("<amResetButton::cfgToConsole> ... LED timer clock frequency = "); Serial.print(getTmrClockSpeed()); Serial.println("MHz");
   Serial.print("<amResetButton::cfgToConsole> ... LED interrupt frequency = "); Serial.print(getTmrFreq()); Serial.println("MHz");
   Serial.print("<amResetButton::cfgToConsole> ... LED timer auto-reload (True/False) = "); Serial.println(getTmrAutoReload());
} //amResetButton::cfgToConsole()

/**
 * @brief Blinks the rest buttons LED. 
===================================================================================================*/
void amResetButton::_blinkLED()
{
   if(_ledBlink == 0)
   {
      setColour(BLUE);
      _ledBlink = 1;
   } //if
   else
   {
      setColour(RED);
      _ledBlink = 0;
   } //else
} //amResetButton::_blinkLED()

/**
 * @brief Advance the colour of the reset button LED by 1.
 * @details The reset button has preset RGB values for its LED. This function cycles through the 
 * colours using a counter that wraps around to 0 once the max colour in the sequence is exceeded.
===================================================================================================*/
void amResetButton::cycleColour()
{
   setColour(_cycleCount);
   _cycleCount ++; // Increment colour counter 
   if(_cycleCount >= _numColoursSupported) // Do not exceed maximum known colours
   {
      _cycleCount = 0; // Start back at the first colour
   } //if
} //amResetButton::cycleColour()

/**
 * @brief Set the colour of the LED on the reset button to a specific preset colour.
 * @details The reset button has preset RGB values for its LED. This function sets the colour to
 * one of those preset values.
===================================================================================================*/
void amResetButton::setColour(int8_t clr)
{
   _cycleCount = clr; // Set colour counter to reflect what the LED was just set to.  Used for cycle process
   _ledColour = clr; // Set colour used for blinking.
   if(_cycleCount >= _numColoursSupported) // Do not exceed maximum known colours
   {
      _cycleCount = 0; // Start back at the first colour
   } //if

   switch(_cycleCount)
   {
      case RED: // If the LED is to be coloured red
         ledcWrite(_redLED.channel, _colour[RED].redDutyCycle); // Set the PWM duty cycle of the red LED
         ledcWrite(_greenLED.channel, _colour[RED].greenDutyCycle); // Set the PWM duty cycle of the green LED
         ledcWrite(_blueLED.channel, _colour[RED].blueDutyCycle); // Set the PWM duty cycle of the blue LED
         break;
      case GREEN: // If the LED is to be coloured green
         ledcWrite(_redLED.channel, _colour[GREEN].redDutyCycle); // Set the PWM duty cycle of the red LED
         ledcWrite(_greenLED.channel, _colour[GREEN].greenDutyCycle); // Set the PWM duty cycle of the green LED
         ledcWrite(_blueLED.channel, _colour[GREEN].blueDutyCycle); // Set the PWM duty cycle of the blue LED
         break;
      case BLUE: // If the LED is to be coloured blue
         ledcWrite(_redLED.channel, _colour[BLUE].redDutyCycle); // Set the PWM duty cycle of the red LED
         ledcWrite(_greenLED.channel, _colour[BLUE].greenDutyCycle); // Set the PWM duty cycle of the green LED
         ledcWrite(_blueLED.channel, _colour[BLUE].blueDutyCycle); // Set the PWM duty cycle of the blue LED
         break;
      case PINK: // If the LED is to be coloured pink
         ledcWrite(_redLED.channel, _colour[PINK].redDutyCycle); // Set the PWM duty cycle of the red LED
         ledcWrite(_greenLED.channel, _colour[PINK].greenDutyCycle); // Set the PWM duty cycle of the green LED
         ledcWrite(_blueLED.channel, _colour[PINK].blueDutyCycle); // Set the PWM duty cycle of the blue LED
         break;
      case CYANNE: // If the LED is to be coloured cyanne
         ledcWrite(_redLED.channel, _colour[CYANNE].redDutyCycle); // Set the PWM duty cycle of the red LED
         ledcWrite(_greenLED.channel, _colour[CYANNE].greenDutyCycle); // Set the PWM duty cycle of the green LED
         ledcWrite(_blueLED.channel, _colour[CYANNE].blueDutyCycle); // Set the PWM duty cycle of the blue LED
         break;
      case AQUA: // If the LED is to be coloured aqua
         ledcWrite(_redLED.channel, _colour[AQUA].redDutyCycle); // Set the PWM duty cycle of the red LED
         ledcWrite(_greenLED.channel, _colour[AQUA].greenDutyCycle); // Set the PWM duty cycle of the green LED
         ledcWrite(_blueLED.channel, _colour[AQUA].blueDutyCycle); // Set the PWM duty cycle of the blue LED
         break;
      default: // If no other colour matches then default to white
         ledcWrite(_redLED.channel, _colour[WHITE].redDutyCycle); // Set the PWM duty cycle of the red LED
         ledcWrite(_greenLED.channel, _colour[WHITE].greenDutyCycle); // Set the PWM duty cycle of the green LED
         ledcWrite(_blueLED.channel, _colour[WHITE].blueDutyCycle); // Set the PWM duty cycle of the blue LED
         break;
   } //switch
} //amResetButton::setColour()

/**
 * @brief Return an 8 bit integer representing the current reset button LED colour.
 * @return int8_t _cycleCount
===================================================================================================*/
int8_t amResetButton::getCurClrNum()
{
   return _cycleCount; // Set colour counter to reflect what the LED was just set to
} //amResetButton::getColour()

/**
 * @brief Return an 8 bit integer representing the current reset button LED colour.
 * @return int8_t _cycleCount
===================================================================================================*/
String amResetButton::getCurClrStr()
{
   return _colour[_cycleCount].name; // Set colour counter to reflect what the LED was just set to
} //amResetButton::getColour()