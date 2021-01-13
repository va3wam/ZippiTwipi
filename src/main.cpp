/*************************************************************************************************************************************
 * @file main.cpp
 * @author va3wam
 * @brief Zippi Twipi robot firmware 
 * @details Basic operating code for the Zippi Twipi robot 
 * @copyright Copyright (c) 2020 va3wam@gmail.com
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
 * YYYY-MM-DD Dev    Description
 * ---------- ------ -----------------------------------------------------------------------------------------------------------------
 * 2021-01-07 va3wam Reworked I2C and limit switch test code
 * 2020-12-12 va3wam Cleaned up loop() and messed around with reet button tri-coloured LED to test newly assembled circuit.
 * 2020-11-23 va3wam Added include of development board pin definition file
 * 2020-10-22 va3wam Program created
 *************************************************************************************************************************************/
#include <Arduino.h> // Arduino Core for ESP32. Comes with Platform.io
#include <Wire.h> // Required for serial I2C communication. Comes with Platform.io 
#include <zippiTwipi_gpio_pins.h> // Pin definitions for Adafruit Huzzah32 development board 
#include <amI2C.h> // Required for serial I2C communication 
#include <amMD25.h> // Required for motor control

// Define structure and variables for reset buttons built-in LEDs. 
// https://microcontrollerslab.com/esp32-pwm-arduino-ide-led-fading-example/ 
typedef struct
{
  int pwmChannel; // There are 16 PWM channels available. Choose any channel between 0 and 15.
  int pwmFrequency; // Frequency of the digital signal
  int pwmResolution; // ESP32 boards support PWM resolution between 1 bit to 16 bits. The optimal resolution is 8 bit.
  int pwmDutyCycle; // Up time of the PWM signal. Ranges from 0-255. 127 is a 50% duty cycle for example.
  int gpioPin; // GPIO pin connected to the LED  
}resetButtonLED; 
resetButtonLED redResetLED = {2, 500, 8, 0, resetRedLED}; // Chan 2, freq 4000Hz, 8 bit res, 255 tick up (100% duty cycle), red LED pin 
resetButtonLED blueResetLED = {1, 500, 8, 0, resetBlueLED}; // Chan 1, freq 4000Hz, 8 bit res, 255 tick up (100% duty cycle), blue LED pin
resetButtonLED greenResetLED = {0, 500, 8, 0, resetGreenLED}; // Chan 0, freq 4000Hz, 8 bit res, 255 tick up (100% duty cycle), green LED pin
int fadeAmount = 5; // How much to fade the LEDs by each loop
int brightness = 0; // How bright the LED is

// Instantiate objects
amI2C i2cBus; // Object to manage I2C buses
amMD25 motorControl; // Object to manage motors via h-bridge controller

/**
 * @brief Check the limit switches moounted at the front and back of the robot to see if it has
 * lost its balance
=================================================================================================== */
void limitSwitchMonitoring()
{
  int frontSwitch = digitalRead(frontLimitSwitch);
  int backSwitch = digitalRead(backLimitSwitch);
  if(frontSwitch == 0 && backSwitch == 0)
  {
    Serial.println("<limitSwitchMonitoring> Robot is resting on both front and back limit switches");
  }
  else
  {
    if(frontSwitch == 0 && backSwitch == 1)
    {
      Serial.println("<limitSwitchMonitoring> Robot is resting on front limit switch");
    } //if
    if(frontSwitch == 1 && backSwitch == 0)
    {
      Serial.println("<limitSwitchMonitoring> Robot is resting on back limit switch");
    } //if
  } //if
  delay(100);
} //limitSwitchMonitoring()

/**
 * @brief Set the colour of the reset button's LED
=================================================================================================== */
void setResetButtonLEDColour(int redDutyCycle, int blueDutyCycle, int greenDutyCycle)
{
  ledcWrite(redResetLED.pwmChannel, redDutyCycle); // Set the duty cycle of red LED PWM channel
  ledcWrite(blueResetLED.pwmChannel, blueDutyCycle); // Set the duty cycle of blue LED PWM channel
  ledcWrite(greenResetLED.pwmChannel, greenDutyCycle); // Set the duty cycle of green LED PWM channel
} //setResetButtonLEDColour()

/**
 * @brief Initialize the serial output with the specified baud rate measured in bits per second
=================================================================================================== */
void setupSerial(unsigned long baudRate)
{
  Serial.begin(115200); // Open a serial connection at specified baud rate 
  while (!Serial); // Wait for Serial port to be ready
} //setupSerial()

/** 
 * @brief Standard set up routine for Arduino programs 
=================================================================================================== */
void setup()
{
  // Set up serial communication
  unsigned long serialBaudRate = 115200; // Define serial baud rate 
  setupSerial(serialBaudRate); // Set serial baud rate  
  Serial.print("<setup> Start of setup. Serial baud rate is ");
  Serial.println(serialBaudRate);
  // Setup I2C buses
  i2cBus.configure(0, I2C_bus0_SDA, I2C_bus0_SCL, I2C_bus0_speed); // Set up I2C bus 0
  i2cBus.configure(1, I2C_bus1_SDA, I2C_bus1_SCL, I2C_bus1_speed); // Set up I2C bus 1
  // Setup motor controller
  byte softVer = motorControl.getMD25FirmwareVersion(); // Gets the software version of MD25
  Serial.print("<setup> MD25 firmware version is ");
  Serial.println(softVer, HEX);
  motorControl.encodeReset();
  Serial.print("<setup> Encode 1 = ");
  Serial.println(motorControl.encoder1());
  Serial.print("<setup> Encode 2 = ");
  Serial.println(motorControl.encoder2());
  motorControl.spinMotor(0,1); // motor, speed 
  delay(2000);
  Serial.print("<setup> Encode 1 = ");
  Serial.println(motorControl.encoder1());
  Serial.print("<setup> Encode 2 = ");
  Serial.println(motorControl.encoder2());
   
//    pinMode(17, INPUT_PULLDOWN);

    // Playing with analog outputs for RGB LED on reset button. Note inverted dutycycle logic, 0 = full on, 255 = full off         
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
    Serial.println("<setup> End of setup");
} //setup()

/**
 * @brief Standard looping routine for Arduino programs
=================================================================================================== */
void loop()
{
//  motorTest();
  limitSwitchMonitoring();
  ledcWrite(redResetLED.pwmChannel, 0); // Set the duty cycle of PWM channel assigned to red LED
  ledcWrite(blueResetLED.pwmChannel, 254); // Set the duty cycle of PWM channel assigned to blue LED
  ledcWrite(greenResetLED.pwmChannel, 254); // Set the duty cycle of PWM channel assigned to green LED
  delay(500);
  ledcWrite(redResetLED.pwmChannel, 254); // Set the duty cycle of PWM channel assigned to red LED
  ledcWrite(blueResetLED.pwmChannel, 0); // Set the duty cycle of PWM channel assigned to blue LED
  ledcWrite(greenResetLED.pwmChannel, 254); // Set the duty cycle of PWM channel assigned to green LED
  delay(500);
  ledcWrite(redResetLED.pwmChannel, 254); // Set the duty cycle of PWM channel assigned to red LED
  ledcWrite(blueResetLED.pwmChannel, 254); // Set the duty cycle of PWM channel assigned to blue LED
  ledcWrite(greenResetLED.pwmChannel, 0); // Set the duty cycle of PWM channel assigned to green LED
  delay(500);
} //loop()