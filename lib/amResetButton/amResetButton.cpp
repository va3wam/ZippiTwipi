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
 * 2021-02-24 Old Squire Program created
 ***************************************************************************************************/
#include <Arduino.h> // Arduino Core for ESP32. Comes with Platform.io
#include <amResetButton.h> // Header file for linking.
#include <zippiTwipi_gpio_pins.h> // Pin definitions for ZippiTiwppi robot

/**
 * @brief This is the constructor for this class.
===================================================================================================*/
amResetButton::amResetButton()
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

   // Setup rest button LEDs. For a good article about PWM see https://makeabilitylab.github.io/physcomp/esp32/led-fade.html
   ledcSetup(_redLED.channel, _redLED.frequency, _redLED.resolution); // Configure red LED PWM properties
   ledcAttachPin(_redLED.gpioPin, _redLED.channel); // Attach PWM channel to pin connected to red LED on reset button
   ledcSetup(_greenLED.channel, _greenLED.frequency, _greenLED.resolution); // Configure green LED PWM properties
   ledcAttachPin(_greenLED.gpioPin, _greenLED.channel); // Attach PWM channel to pin connected to green LED on reset button
   ledcSetup(_blueLED.channel, _blueLED.frequency, _blueLED.resolution); // Configure blue LED PWM properties
   ledcAttachPin(_blueLED.gpioPin, _blueLED.channel); // Attach PWM channel to pin connected to blue LED on reset button

} //amResetButton::amResetButton()

/**
 * @brief Initialize the LEDs integrated into the rest button.
 ==================================================================================================*/
/*
void amResetButton::init()
{
   _cycleCount = 0; // Set colour counter back to 0
} //amResetButton::_resetColourCount()
*/

/**
 * @brief Advance the colour of the reset button LED by 1.
 * @details The reset button has preset RGB values for its LED. This function cycles through the 
 * colours using a counter that wraps around to 0 once the max colour in the sequence is exceeded.
===================================================================================================*/
void amResetButton::cycleColour()
{
   setColour(_cycleCount);
   Serial.print("<amResetButton::cycleColour> Colour name = ");
   Serial.println(_colour[_cycleCount].name);
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
   _cycleCount = clr; // Set colour counter to reflect what the LED was just set to
   if(_cycleCount >= _numColoursSupported) // Do not exceed maximum known colours
   {
      _cycleCount = 0; // Start back at the first colour
   } //if
   Serial.print("<amResetButton::cycleColour> Set colour to = ");
   Serial.println(_colour[_cycleCount].name);

   switch(_cycleCount)
   {
      case RED: // If the LED is to be coloured red
         Serial.println("<amResetButton::setColour> Set dutycycles for RED");
         ledcWrite(_redLED.channel, _colour[RED].redDutyCycle); // Set the PWM duty cycle of the red LED
         ledcWrite(_greenLED.channel, _colour[RED].greenDutyCycle); // Set the PWM duty cycle of the green LED
         ledcWrite(_blueLED.channel, _colour[RED].blueDutyCycle); // Set the PWM duty cycle of the blue LED
         break;
      case GREEN: // If the LED is to be coloured green
         Serial.println("<amResetButton::setColour> Set dutycycles for GREEN");
         ledcWrite(_redLED.channel, _colour[GREEN].redDutyCycle); // Set the PWM duty cycle of the red LED
         ledcWrite(_greenLED.channel, _colour[GREEN].greenDutyCycle); // Set the PWM duty cycle of the green LED
         ledcWrite(_blueLED.channel, _colour[GREEN].blueDutyCycle); // Set the PWM duty cycle of the blue LED
         break;
      case BLUE: // If the LED is to be coloured blue
         Serial.println("<amResetButton::setColour> Set dutycycles for BLUE");
         ledcWrite(_redLED.channel, _colour[BLUE].redDutyCycle); // Set the PWM duty cycle of the red LED
         ledcWrite(_greenLED.channel, _colour[BLUE].greenDutyCycle); // Set the PWM duty cycle of the green LED
         ledcWrite(_blueLED.channel, _colour[BLUE].blueDutyCycle); // Set the PWM duty cycle of the blue LED
         break;
      case PINK: // If the LED is to be coloured pink
         Serial.println("<amResetButton::setColour> Set dutycycles for PINK");
         ledcWrite(_redLED.channel, _colour[PINK].redDutyCycle); // Set the PWM duty cycle of the red LED
         ledcWrite(_greenLED.channel, _colour[PINK].greenDutyCycle); // Set the PWM duty cycle of the green LED
         ledcWrite(_blueLED.channel, _colour[PINK].blueDutyCycle); // Set the PWM duty cycle of the blue LED
         break;
      case CYANNE: // If the LED is to be coloured cyanne
         Serial.println("<amResetButton::setColour> Set dutycycles for CYANNE");
         ledcWrite(_redLED.channel, _colour[CYANNE].redDutyCycle); // Set the PWM duty cycle of the red LED
         ledcWrite(_greenLED.channel, _colour[CYANNE].greenDutyCycle); // Set the PWM duty cycle of the green LED
         ledcWrite(_blueLED.channel, _colour[CYANNE].blueDutyCycle); // Set the PWM duty cycle of the blue LED
         break;
      case AQUA: // If the LED is to be coloured aqua
         Serial.println("<amResetButton::setColour> Set dutycycles for AQUA");
         ledcWrite(_redLED.channel, _colour[AQUA].redDutyCycle); // Set the PWM duty cycle of the red LED
         ledcWrite(_greenLED.channel, _colour[AQUA].greenDutyCycle); // Set the PWM duty cycle of the green LED
         ledcWrite(_blueLED.channel, _colour[AQUA].blueDutyCycle); // Set the PWM duty cycle of the blue LED
         break;
      default: // If no other colour matches then default to white
         Serial.println("<amResetButton::setColour> Set dutycycles for WHITE");
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