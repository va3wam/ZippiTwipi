/***************************************************************************************************
 * @file main.cpp
 * @author the Aging Apprentice
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
 * YYYY-MM-DD Dev        Description
 * ---------- ---------- ---------------------------------------------------------------------------
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
#include <esp32-hal-cpu.h> // Required for getCpuFrequencyMhz()
#include <soc/rtc.h> // Required for rtc_clk_apb_freq_get()
#include <amI2C.h> // Required for serial I2C communication 
#include <amMD25.h> // Required for motor control
#include <amLCD.h> // Required for LCD control
#include <amResetButton.h> // Required for controlling reset button LED
#include <amFormat.h> // Library of handy variable format conversion functions
#include <amChip.h> // Used to access details about the core (CPU) that the Arduino framework runs on
#include <WebOTA.h> // Required for OTA code downloading. // https://github.com/scottchiefbaker/ESP-WebOTA
#define ledTimer 0 // Timer 0 will be used to control LED on reset button
#define ledFreq 1000 // Micro seconds per timer0 (led timer) interrupt occurence (1 per second)

String firmwareVersion = "0.0.5"; // Semver formatted version number for this firmware code

// Instantiate library objects
amI2C i2cBus; // Object to manage I2C buses
amMD25 motorControl; // Object to manage motors via h-bridge controller
amResetButton myResetButton; // Control the reset button's integrated RGB LED
amFormat format; // Accept various variable type/formats and return a different variable type/format
amChip appCpu; // Access information about the ESP32 application microprocessor

// Define variables to store environment information 
uint32_t arduinoCore; // The ESP32 comes with 2 Xtensa 32-bit LX6 microprocessors: core 0 and core 1. Arduino IDE runs on core 1.
uint32_t cpuClockSpeed; // Clock speed of the core that Arduino is running on (MHz)
uint32_t timerClockSpeed; // Clock speed of the hardware timers (MHz)

// Define structure and variables for hardware interrupt timer 
volatile int interrupt0Counter; // Track how many times the interrupt has fired and not been processed
int totalInterrupt0Counter; // Track how many times the interrupt has fired in total
int currColourCnt; // Track what colour is currently active for the reset button LED
hw_timer_t * timer0 = NULL; // Pointer to hardware timer0
portMUX_TYPE timer0Mux = portMUX_INITIALIZER_UNLOCKED; // Used to prevent race conditins updating counters

// Define IP address variables
String myMACaddress; // MAC address of this SOC. Used to uniquely identify this robot
String myIPAddress = "-no IP address-"; // IP address of the SOC.
String myAccessPoint = "-no access point-"; // WiFi Access Point that we managed to connected to
String myHostName = "-no hostname-"; // Name by which we are known by the Access Point
String myHostNamePrefix = "ZipyTwipy"; // Suffix to add to WiFi host name for this robot

// Define structure and variables for the MD25 motor controller 
byte md25FirmwareVersion; // Firmware version of MD25 motor controller

// Define structure and variables for the LCD 
typedef struct
{
   int numCols = lcdNumCols;
   int numRows = lcdNumRows;
   int i2cAdd = lcdI2cAddress;
}lcdStruct; 
lcdStruct lcdProperty; // Declare structure that holds relevant variables for the LCD
amLCD lcd(lcdProperty.i2cAdd, lcdProperty.numCols, lcdProperty.numRows); // Create LCD object

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
 * @brief Cycle through LED colours 
 * @details Each call causes the colour to switch to the next colour in the cycle
=================================================================================================== */
void cycleLed()
{
   if(interrupt0Counter > 0) 
   {
      portENTER_CRITICAL(&timer0Mux);
      interrupt0Counter--;
      portEXIT_CRITICAL(&timer0Mux);
      totalInterrupt0Counter++;
      myResetButton.cycleColour();
//      myResetButton.setColour(AQUA);
   } //if   
} //cycleLed()

/** 
 * @brief Setup all network aspects of the robot
 * @details Connect to WiFi Access Point, Set up a web server to accept OTA firmware update, create
 * a unique host name for this robot usiing its MAC address 
=================================================================================================== */
void setupNetwork()
{
   // Setup Over The Air updates
   init_wifi("MN_LIVINGROOM", "5194741299", "/webota"); // Defaults to 8080 and "/webota"
   // Put key networking information into variables 
//   myIPAddress = ipToString(WiFi.localIP()); // Convert IP address to string for displaying on OLED
   myIPAddress = format.ipToString(WiFi.localIP()); // Convert IP address to string for displaying on OLED
   myAccessPoint = WiFi.SSID(); // Record the name of the Access Point we connect to
//   myMACaddress = formatMAC();
   myMACaddress = format.noColonMAC(WiFi.macAddress()); // Convert MAC address to string with no colons
   String tmpHostNameVar = myHostNamePrefix + myMACaddress; // Format the host name we will use
   WiFi.setHostname((char *)tmpHostNameVar.c_str()); // Set our hostname
   myHostName = WiFi.getHostname(); // Record the host name we are known by at the Access Point
} //setupNetwork()

/** 
 * @brief Capture and show the environment details of the robot
=================================================================================================== */
void showCfgDetails()
{
   arduinoCore = xPortGetCoreID(); // Core is Arduino runing on
   cpuClockSpeed = getCpuFrequencyMhz(); // Clock frequency of core that Arduino is running on (MHz) 
   timerClockSpeed = rtc_clk_apb_freq_get() / 1000000; // Convert timer clock speed(Hz) to MHz by dividing by 1 million
   // Robot general info
   Serial.print("<showCfgDetails> ... Robot firmware version = "); 
   Serial.println(firmwareVersion);

   appCpu.cfgToConsole(); // Display core0 information on the console
   // Move this to the amMD25 class 
   Serial.print("<showCfgDetails> ... MD25 firmware version = ");
   Serial.println(md25FirmwareVersion, HEX);

   // Move this to the amWiFi class which will extend the ESP-WebOTA class (also add AP scanning logic in here)    
   Serial.print("<showCfgDetails> ... Access Point name: ");
   Serial.println(myAccessPoint);
   Serial.print("<showCfgDetails> ... Robot MAC address: ");
   Serial.println(myMACaddress);
   Serial.print("<showCfgDetails> ... Robot IP address: ");
   Serial.println(myIPAddress);
   Serial.print("<showCfgDetails> ... Robot Host Name: ");
   Serial.println(myHostName);   
   Serial.print("<showCfgDetails> ... Robot OTA URL: ");
   Serial.println("HTTP://" + myIPAddress + ":8080/webota");   
} //showCfgDetails()

/** 
 * @brief Configure and activate Timer0 used to control the RGB LED on the reset button
=================================================================================================== */
void setupTimer0()
{
   // Setup timer0
   Serial.print("<setupTimer0> Configure timer");
   Serial.println(ledTimer); 
   timer0 = timerBegin(ledTimer, timerClockSpeed, true); // Set timer 0 to a 1 MHz frequency 
   timerAttachInterrupt(timer0, &onTimer0, true); // Call onTimer0() each time ths interrupt fires
   timerAlarmWrite(timer0, ledFreq, true); // Set frequency interrupt fires and reset counter to repeat
   timerAlarmEnable(timer0); // Enable interrupt   
} //setupTimer0()

/** 
 * @brief Standard set up routine for Arduino programs 
=================================================================================================== */
void setup()
{
   // Setup debug tracing. So far only to the local console but remote options may be added
   setupSerial(serialBaudRate); // Set serial baud rate  
   Serial.println("<setup> Start of setup");
   // Setup I2C buses
   i2cBus.configure(i2cBusNumber0, I2C_bus0_SDA, I2C_bus0_SCL, I2C_bus0_speed); // Set up I2C bus 0 
   i2cBus.configure(i2cBusNumber1, I2C_bus1_SDA, I2C_bus1_SCL, I2C_bus1_speed); // Set up I2C bus 1
   // Setup motor controller
   lcd.centre("Boot Process",0);
   lcd.centre("Motors",1);
   md25FirmwareVersion = motorControl.getFirmwareVersion(); // Gets the software version of MD25
   motorControl.encoderReset();
   // Setup balance limit switches
   lcd.centre("Limit Switches",1);
   pinMode(frontLimitSwitch,INPUT_PULLUP); // Set pin with front limit switch connected to it as input with an internal pullup resistor
   pinMode(backLimitSwitch,INPUT_PULLUP); // Set pin with back limit switch connected to it as input with an internal pullup resistor        
   // Setup networking
   lcd.centre("Network",1);
   setupNetwork(); 
   lcd.centre("Timer0",1);
   setupTimer0();
   // Summarize the running conditions of the robot
   lcd.centre("Summary",1);
   showCfgDetails();
   lcd.centre("IP:" + myIPAddress,0);
   lcd.centre("MAC:" + myMACaddress,1);
   Serial.println("<setup> End of setup");
} //setup()

/**
 * @brief Standard looping routine for Arduino programs
=================================================================================================== */
void loop()
{
//  motorTest();
   limitSwitchMonitoring(); // Check for limit switch activation 
   cycleLed(); // Update LED as needed
   webota.handle(); // Check for OTA messages
} //loop()