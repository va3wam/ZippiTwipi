/*************************************************************************************************************************************
 * @file amNetwork.h
 * @author theAgingApprentice
 * @brief Handle all of the networking logic required by the robot. 
 * @details Manages TCP/IP, HTTP server, OTA firmware loading and MQTT messaging. See https://lastminuteengineers.com/esp32-ota-web-updater-arduino-ide/
 * @copyright Copyright (c) 2021 the Aging Apprentice
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
 * YYYY-MM-DD Dev        Description
 * ---------- ---------- -------------------------------------------------------------------------------------------------------------
 * 2021-03-07 Old Squire Program created.
 *************************************************************************************************************************************/
#ifndef amNetwork_h
#define amNetwork_h
#include <Arduino.h> // Arduino Core for ESP32. Comes with Platform.io
#include <WiFi.h> // Required to connect to WiFi network. Comes with Platform.io
#include <WiFiClient.h> // Required for communicating with web browser
#include <WebServer.h> // Required for hosting an HTTP server on the ESP32
#include <ESPmDNS.h> // Required for local name resolution and redirecting of incoming cient requests
#include <Update.h> // Library used to update firmware
#include <known_networks.h> // Defines Access points and passwords that the robot can scan for and connect to
#include <amNetwork.h> // Library of handy variable format conversion functions
#include <amFormat.h> // Library of handy variable format conversion functions
#include <Preferences.h> // 

#define HOST_NAME_PREFIX "Zippy" 
#define httpPort 80

// Define amNetwork class
class amNetwork 
{
   public:
      amNetwork(); // Constructor
      bool connect(); // Handle connecting to the WiFI network 
      bool connectStatus(); // Returns the status of the WiFi connection 
      void cfgToConsole(); // Send client configuration information to the console
      void checkForClientRequest(); // Check to see if there are any web client requests waiting to be serviced
      String getAccessPointName(); // Retrieve the name of the Access Point the robot is using.
      String getMacAddress(); // Retrieve the MAC of the Access Point of our WiFi radio transmitter. 
      IPAddress getIpAddress(); // Retrieve the IP address assigned to the robot.
      String getUniqueName(); // Retrieve the name that we will be known by on the AP and the MQTT broker.
   private:
      const char* _lookForAP(); // Scan the 2.4GHz WiFi radio band looking for known Access Points
      String _translateEncryptionType(wifi_auth_mode_t encryptionType); // Converts AP ENUM encryption value to human readable format
      const char* _apSsid; // Access point SSID selected to coonnect to
      const char* _apPassword; // Password of access point selected to cnnect to
      const char* _hostName; // Name the robot will be known by on the network
      amFormat _format; // Accept various variable type/formats and return a different variable type/format
}; //class amNetwork

#endif