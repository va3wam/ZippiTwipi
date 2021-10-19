/******************************************************************************
 * @file main.cpp
 *
 * @mainpage Zippy Twippi firmware. 
 * 
 * @section intro_sec Introduction
 *
 * This code is the firmware for the two wheeled robot called Zippy Twippi. 
 * Full details on how to get the circuit and chassis for this robot are found
 * [here](https://github.com/va3wam/ZippiTwipi). 
 *
 * @section dependencies Dependencies
 * 
 * This sketch depends on the libraries located in the main.h file located in
 * the include directory of this repository.
 *
 * @section author Author(s)
 *
 * Written by Old Squire and Doug Elliott.
 *
 * @section license license
 *
 * Copyright 2021 the Aging Apprentice 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to 
 * deal in the Software without restriction, including without limitation the 
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is 
 * furnished to do so, subject to the following conditions: The above copyright
 * notice and this permission notice shall be included in all copies or 
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.  
 *****************************************************************************/
#include <main.h> // Header file for all libraries needed by this program.
unsigned long timer; // Milli count for next action.

/** 
 * @brief Check to see how the boot up process went.
 * =================================================================================*/
void checkBoot()
{
   Log.traceln("<checkBoot> Checking boot status flags."); 
   if(networkConnected == true && mqttBrokerConnected == true && lcdConnected == true && mobilityStatus == true)
   {
      Log.verboseln("<checkBoot> Bootup was normal. Set RGB LED to normal colour."); 
      setStdRgbColour(BLUE); // Indicates that bootup was normal.
   } // if
   else
   {
      Log.verboseln("<checkBoot> Bootup had an issue. Set RGB LED to warning colour."); 
      setStdRgbColour(YELLOW); // Indicates that there was a bootup issue.
   } // else
} // checkBoot

/**
 * @brief Standard Arduino initialization routine.
 * ==========================================================================*/
void setup() 
{
   setupSerial(); // Set serial baud rate. 
   Log.begin(LOG_LEVEL_VERBOSE, &Serial, true);
   Log.traceln("<setup> Start of setup.");  
   Log.verboseln("<setup> Initialize I2C buses."); 
   Wire.begin(I2C_BUS0_SDA, I2C_BUS0_SCL, I2C_bus0_speed); // Init I2C bus0.
   Wire1.begin(I2C_BUS1_SDA, I2C_BUS1_SCL, I2C_bus1_speed); // Init I2C bus1.
   Log.verboseln("<setup> Initialize status RGB LED."); 
   setupStatusLed(); // Configure the status LED on the reset button.
   setStdRgbColour(WHITE); // Indicates that boot up is in progress.
   Log.verboseln("<setup> Initialize limit switches."); 
   setupLimitSwitches(); // Configure limit switches.
   Log.verboseln("<setup> Set up wifi connection."); 
   network.connect(); // Start WiFi connection.
   if(network.areWeConnected() == true) // If we are on the WiFi network.
   {
      networkConnected = true;
      Log.noticeln("<setup> Connection to network successfully estabished.");
      Log.verboseln("<setup> Initialize local web services."); 
      startWebServer(); // Start up web server.
      Log.verboseln("<setup> Initialize MQTT broker connection."); 
      bool tmp = connectToMqttBroker(network); // Connect to MQTT broker.
      if(tmp == true) // If we found an MQTT broker.
      {
         mqttBrokerConnected = true;
         Log.noticeln("<setup> Connection to MQTT broker successfully established.");
      } // if
      else // If we did not find a valid MQTT broker.
      {
         mqttBrokerConnected = false;
         Log.errorln("<setup> Connected to MQTT broker failed.");
      } // else
   } // if
   else // If we are NOT on the WiFi network.
   {
      networkConnected = false;
      Log.errorln("<setup> Not connencted to the network. No MQTT or web interface.");
   } // else
   Log.verboseln("<setup> Look for I2C devices."); 
   scanBus0(); // Scan bus0 and show connected devices.
   scanBus1(); // Scan bus1 and show connected devices.
   if(lcdConnected == true) // If an LCD was found on the I2C bus.
   {
      Log.traceln("<setup> Initialize LCD.");
      initLcd();
   } // if
   else // If an OLED was NOT found on the I2C bus.
   {
      Log.warningln("<setup> LED not connencted to I2C bus. No LED messages to be issued.");
   } //else
   if(motorControllerConnected == true) // If servo drivers found on I2C bus.
   {
      Log.traceln("<setup> Initialize DC motor driver.");
      mobilityStatus = true; 
      initMobility(); // Put servos into starting position. May replace with Doug's stuff. 
   } // if
   else // If servo drivers found on I2C bus.
   {
      Log.errorln("<setup> Motor driver not connencted to I2C bus. No motion is possible.");
      mobilityStatus = false;
   } //else
   Log.verboseln("<setup> Display robot configuration in console trace."); 
   showCfgDetails(); // Show all configuration details in one summary.
   Log.verboseln("<setup> Review status flags to see how boot sequence went."); 
   checkBoot();
   Log.traceln("<setup> End of setup."); 
   timer = millis(); // Timer for motor driver signalling.
} // setup()

/**
 * @brief Standard Arduino main loop.
 * ==========================================================================*/
void loop() 
{
   checkLimitSwitches(); // Make update to status LED on reset button.
//   monitorWebServer(); // Handle any pending web client requests. 
//   checkMqtt(); // Check the MQTT message queue for incoming commands.
} // loop()  