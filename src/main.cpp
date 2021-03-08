/***************************************************************************************************
 * @file main.cpp
 * @author the Aging Apprentice
 * @brief Zippy Twipy robot firmware 
 * @details Basic operating code for the Zippy Twipy robot 
 * @copyright Copyright (c) 2020 theAgingApprentice@protonmail.com
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
 * 2021-03-08 Old Squire Dropped use am amWifi() and replaced with amNetwork().
 * 2021-03-04 Old Squire Moved limit switch logic to amLimitSwitch().
 * 2021-03-03 Old Squire Moved all timer logic to amReset().
 * 2021-02-28 Old Squire Moved all networking logic to amWiFi().
 * 2021-02-27 Old Squire Moved timer information to amResetButton().
 * 2021-02-26 Old Squire Moved formatting, LCD and chip functions out into their own class libraries. 
 * 2021-02-25 Old Squire Moved control logic for RBG LED integrated into reset button to library. 
 * 2021-01-07 Old Squire Reworked I2C and limit switch test code
 * 2020-12-12 Old Squire Cleaned up loop() and messed around with reset button tri-coloured LED to 
 *                       test newly assembled circuit.
 * 2020-11-23 Old Squire Added include of development board pin definition file
 * 2020-10-22 Old Squire Program created
 ***************************************************************************************************/
#include <Arduino.h> // Arduino Core for ESP32. Comes with Platform.io
#include <huzzah32_gpio_pins.h> // Pin definitions for Adafruit Huzzah32 development board 
#include <zippiTwipi_gpio_pins.h> // Pin definitions for ZippiTiwppi robot
#include <amI2C.h> // Required for serial I2C communication 
#include <amMD25.h> // Required for motor control
#include <amLCD.h> // Required for LCD control
#include <amResetButton.h> // Required for controlling reset button LED. Includes use of one of the timers
#include <amFormat.h> // Library of handy variable format conversion functions
#include <amChip.h> // Used to access details about the core (CPU) that the Arduino framework runs on
#include <amNetwork.h> // Required for all networking including the webota functions. 
#include <amLimitSwitch.h> // Required for balance limit switches

String firmwareVersion = "0.0.1"; // Semver formatted version number for this firmware code

// Instantiate library objects
amI2C i2cBus; // Object to manage I2C buses
amMD25 motorControl; // Object to manage motors via h-bridge controller
amResetButton resetButton; // Control the reset button's integrated RGB LED
amFormat format; // Accept various variable type/formats and return a different variable type/format
amChip appCpu; // Access information about the ESP32 application microprocessor
amNetwork network; // All network related things (OTA, HTTP server, MQTT, WiFi)
amLimitSwitch balanceSwitch; // Limit switches used to detect when the robot is leaning one way or the other 
lcdStruct lcdProperty; // Declare structure that holds relevant variables for the LCD
amLCD lcd(lcdProperty.i2cAdd, lcdProperty.numCols, lcdProperty.numRows); // Create LCD object

/**
 * @brief Check the limit switches moounted at the front and back of the robot to see if it has
 * lost its balance
=================================================================================================== */
void limitSwitchMonitoring()
{
   if(balanceSwitch.check(frontSwitch) == false && balanceSwitch.check(backSwitch) == false)
   {
      Serial.println("<limitSwitchMonitoring> Both switches are pressed");
   } //if
   else if(balanceSwitch.check(frontSwitch) == false)
   {
      Serial.println("<limitSwitchMonitoring> Front switch is pressed");
   } //else if
   else if(balanceSwitch.check(backSwitch) == false)
   {
      Serial.println("<limitSwitchMonitoring> Back switch is pressed");
   } //else if
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
 * @brief Capture and show the environment details of the robot
=================================================================================================== */
void showCfgDetails()
{
   Serial.println("<showCfgDetails> Robot Configuration Report");
   Serial.println("<showCfgDetails> ==========================");
   Serial.print("<showCfgDetails> ... Robot firmware version = "); 
   Serial.println(firmwareVersion);
   appCpu.cfgToConsole(); // Display core0 information on the console
   i2cBus.cfgToConsole(); // Display I2C information on the console
   resetButton.cfgToConsole(); // Display timer0 information on the console
   motorControl.cfgToConsole(); // Display motor controller information on the console
   network.cfgToConsole(); // Display network information on the console
} //showCfgDetails()

/** 
 * @brief Standard set up routine for Arduino programs 
=================================================================================================== */
void setup()
{
   setupSerial(serialBaudRate); // Set serial baud rate  
   Serial.println("<setup> Start of setup");
   i2cBus.configure(i2cBusNumber0, I2C_bus0_SDA, I2C_bus0_SCL, I2C_bus0_speed); // Set up I2C bus 0 
   i2cBus.configure(i2cBusNumber1, I2C_bus1_SDA, I2C_bus1_SCL, I2C_bus1_speed); // Set up I2C bus 1
   // Motor setup
   lcd.centre("Boot Process",lcdRow1);
   lcd.centre("Motors",lcdRow2);
   motorControl.encoderReset();
   // Reset button setup  
   lcd.centre("LED",lcdRow2);
   resetButton.setLedMode(ledModeCycle); // What behaviour you want for the LED (on, off, bink, cycle)
   // Network setup
   lcd.centre("Network",lcdRow2);
   bool isWiFi = network.connect();
   Serial.print("<setup> isWiFi = "); Serial.println(isWiFi);
   // Summarize configuration
   lcd.centre("Summary",lcdRow2);
   showCfgDetails();
   // Display key info on LCD
   lcd.centre("IP:" + format.ipToString(network.getIpAddress()), lcdRow1);
   lcd.centre("MAC:" + format.noColonMAC(network.getMacAddress()),lcdRow2);
   Serial.println("<setup> End of setup");
} //setup()

/**
 * @brief Standard looping routine for Arduino programs
=================================================================================================== */
void loop()
{
   limitSwitchMonitoring(); // Check for limit switch activation 
   resetButton.updateLed(); // Handle the behaviour of the reset button LED
   network.checkForClientRequest(); // Look to see if there is an Over The Air firmware update incoming
} //loop()