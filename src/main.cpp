/***************************************************************************************************
 * @file main.cpp
 * @author va3wam
 * @brief Zippi Twipi robot firmware 
 * @details Basic operating code for the Zippi Twipi robot 
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
 * 2021-01-07 va3wam Reworked I2C and limit switch test code
 * 2020-12-12 va3wam Cleaned up loop() and messed around with reet button tri-coloured LED to test 
 *                   newly assembled circuit.
 * 2020-11-23 va3wam Added include of development board pin definition file
 * 2020-10-22 va3wam Program created
 ***************************************************************************************************/
#include <Arduino.h> // Arduino Core for ESP32. Comes with Platform.io
#include <Wire.h> // Required for serial I2C communication. Comes with Platform.io 
#include <huzzah32_gpio_pins.h> // Pin definitions for Adafruit Huzzah32 development board 
#include <zippiTwipi_gpio_pins.h> // Pin definitions for ZippiTiwppi robot
#include <amI2C.h> // Required for serial I2C communication 
#include <amMD25.h> // Required for motor control
#include <amRGB.h> // Required to control tri-colours LED on reset button
#include <esp32-hal-cpu.h> // Required for getCpuFrequencyMhz()
#include <soc/rtc.h> // Required for rtc_clk_apb_freq_get()

// Instantiate library objects
amI2C i2cBus; // Object to manage I2C buses
amMD25 motorControl; // Object to manage motors via h-bridge controller
amRGB resetButton; // Object to manage tri-colour LED on reset button

// Define variables to store environment information 
uint32_t arduinoCore; // The ESP32 comes with 2 Xtensa 32-bit LX6 microprocessors: core 0 and core 1. Arduino IDE runs on core 1.
uint32_t cpuClockSpeed; // Clock speed of the core that Arduino is running on (MHz)
uint32_t timerClockSpeed; // Clock speed of the hardware timers (MHz)

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
      motorControl.stopMotor(2); // motor (0=left, 1=right, 2=both) 
      Serial.print("<setup> Encode 1 = ");
      Serial.println(motorControl.getEncoder1());
      Serial.print("<setup> Encode 2 = ");
      Serial.println(motorControl.getEncoder2());
   }
   else
   {
      if(frontSwitch == 0 && backSwitch == 1)
      {
         Serial.println("<limitSwitchMonitoring> Robot is resting on front limit switch");
         motorControl.spinMotor(2,255); // motor (0=left, 1=right, 2=both), speed 
      } //if
      if(frontSwitch == 1 && backSwitch == 0)
      {
         Serial.println("<limitSwitchMonitoring> Robot is resting on back limit switch");
         motorControl.spinMotor(2,0); // motor (0=left, 1=right, 2=both), speed 
      } //if
   } //if
  delay(100);
} //limitSwitchMonitoring()



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

   // Capture environment information  
   arduinoCore = xPortGetCoreID(); // Core is Arduino runing on
   cpuClockSpeed = getCpuFrequencyMhz(); // Clock frequency of core that Arduino is running on (MHz) 
   timerClockSpeed = rtc_clk_apb_freq_get() / 1000000; // Get clock speed of timers(Hz) and convert to MHz
   Serial.print("<setup> Running on core ");
   Serial.print(arduinoCore);
   Serial.print(" at a clock frequency of ");
   Serial.print(cpuClockSpeed);
   Serial.print("MHz with a timer clock speed of ");
   Serial.print(timerClockSpeed); 
   Serial.println("MHz");

   // Setup I2C buses
   i2cBus.configure(0, I2C_bus0_SDA, I2C_bus0_SCL, I2C_bus0_speed); // Set up I2C bus 0 
   i2cBus.configure(1, I2C_bus1_SDA, I2C_bus1_SCL, I2C_bus1_speed); // Set up I2C bus 1

   // Setup motor controller
   byte softVer = motorControl.getFirmwareVersion(); // Gets the software version of MD25
   Serial.print("<setup> MD25 firmware version is ");
   Serial.println(softVer, HEX);
   motorControl.encoderReset();
//    pinMode(17, INPUT_PULLDOWN);
  
   // Setup rest button's red LED. For a good article about PWM see https://makeabilitylab.github.io/physcomp/esp32/led-fade.html
   resetButton.bob(timerClockSpeed,27); //   (timerClockSpeed, 27, 33, 15, 14, 32);
//   resetButton.configure(timerClockSpeed, resetRedLED, resetBlueLED, resetGreenLED, frontLimitSwitch, backLimitSwitch);
   Serial.println("<setup> End of setup");

} //setup()

/**
 * @brief Standard looping routine for Arduino programs
=================================================================================================== */
void loop()
{
//  motorTest();
   limitSwitchMonitoring();
} //loop()