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
#include <Wire.h> // Required for serial I2C communication. Comes with Platform.io 
#include <WiFi.h> // Required to connect to WiFi network. Comes with Platform.io
#include <huzzah32_gpio_pins.h> // Pin definitions for Adafruit Huzzah32 development board 
#include <zippiTwipi_gpio_pins.h> // Pin definitions for ZippiTiwppi robot
#include <amI2C.h> // Required for serial I2C communication 
#include <amMD25.h> // Required for motor control
#include <amLCD.h> // Required for LCD control
#include <amResetButton.h> // Required for controlling reset button LED. Includes use of one of the timers
#include <amFormat.h> // Library of handy variable format conversion functions
#include <amChip.h> // Used to access details about the core (CPU) that the Arduino framework runs on
#include <amWiFi.h> // Required for all networking including the webota functions. 

String firmwareVersion = "0.0.1"; // Semver formatted version number for this firmware code

// Instantiate library objects
amI2C i2cBus; // Object to manage I2C buses
amMD25 motorControl; // Object to manage motors via h-bridge controller
amResetButton myResetButton; // Control the reset button's integrated RGB LED
amFormat format; // Accept various variable type/formats and return a different variable type/format
amChip appCpu; // Access information about the ESP32 application microprocessor
amWiFi network; // WiFi and OTA control

uint32_t timerClockSpeed; // Clock speed of the hardware timers (MHz)

// Define structure and variables for hardware interrupt timer 
volatile int interrupt0Counter; // Track how many times the interrupt has fired and not been processed
int totalInterrupt0Counter; // Track how many times the interrupt has fired in total
int currColourCnt; // Track what colour is currently active for the reset button LED
hw_timer_t * timer0 = NULL; // Pointer to hardware timer0
portMUX_TYPE timer0Mux = portMUX_INITIALIZER_UNLOCKED; // Used to prevent race conditins updating counters
lcdStruct lcdProperty; // Declare structure that holds relevant variables for the LCD
amLCD lcd(lcdProperty.i2cAdd, lcdProperty.numCols, lcdProperty.numRows); // Create LCD object

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
   } //if
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
   } //else
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
 * @brief Capture and show the environment details of the robot
=================================================================================================== */
void showCfgDetails()
{
   Serial.println("<showCfgDetails> Robot Configuration Report");
   Serial.println("<showCfgDetails> ==========================");
   Serial.print("<showCfgDetails> ... Robot firmware version = "); 
   Serial.println(firmwareVersion);
   appCpu.cfgToConsole(); // Display core0 information on the console
   myResetButton.cfgToConsole(); // Display timer0 information on the console
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

   lcd.centre("Boot Process",lcdRow1);
   lcd.centre("Motors",lcdRow2);
   motorControl.encoderReset();

   lcd.centre("Limit Switches",lcdRow2);
   pinMode(frontLimitSwitch,INPUT_PULLUP); // Set pin with front limit switch connected to it as input with an internal pullup resistor
   pinMode(backLimitSwitch,INPUT_PULLUP); // Set pin with back limit switch connected to it as input with an internal pullup resistor        

   lcd.centre("LED",lcdRow2);
//   myResetButton.setColour(PINK); // What colour to use for blinking the reset button LED or setting it to a solid colour
   myResetButton.setLedMode(ledModeCycle); // What behaviour you want for the LED (on, off, bink, cycle)

   lcd.centre("Network",lcdRow2);
   network.connect();

   lcd.centre("Summary",lcdRow2);
   showCfgDetails();

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
   myResetButton.updateLed(); // Handle the behaviour of the reset button LED
   webota.handle(); // Check for OTA messages. Can also use child network.handle();
} //loop()