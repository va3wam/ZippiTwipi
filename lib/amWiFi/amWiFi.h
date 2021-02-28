/*************************************************************************************************************************************
 * @file amWiFi.h
 * @author theAgingApprentice
 * @brief Child class that extends the parent class ESP-WebOTA-master and adds some handy functions. 
 * @details Manages all network related commnication details.
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
 * 2021-02-27 Old Squire Program created
 *************************************************************************************************************************************/
#ifndef amWiFi_h
#define amWiFi_h
#include <Arduino.h> // Arduino Core for ESP32. Comes with Platform.io
#include <amFormat.h> // Library of handy variable format conversion functions
#include <WebOTA.h> // Parent class needed for Over-The-Air code loading
#define HOST_NAME_PREFIX "Zippy" 
#define OTA_LOCAL_PATH "/webota"
#define OTA_LOCAL_PORT "8080"

// Define amWiFi class
class amWiFi : public WebOTA // Indicate that we are extending WebOTA class with our class 
{
   public:
      amWiFi(); // Constructor
      void connect(); 
      void cfgToConsole();
      String getAccessPointName(); // Retrieve the name of the Access Point the robot is using.
      String getMacAddress(); // Retrieve the MAC of the Access Point of our WiFi radio transmitter. 
      IPAddress getIpAddress(); // Retrieve the IP address assigned to the robot.
      String getUniqueName(); // Retrieve the name that we will be known by on the AP and the MQTT broker.
      String getOtaUrl(); // Retrieve the URL operators can use to access the OTA web page hosted by the robot.
   private:
      const char* _lookForAP(); // Scan the 2.4GHz WiFi radio band looking for known Access Points
      String _translateEncryptionType(wifi_auth_mode_t encryptionType); // Converts AP ENUM encryption value to human readable format
      const char *_mySSID;
      const char *_myPassword;
      String _hostName;
      amFormat _format; // Accept various variable type/formats and return a different variable type/format
}; //class amWiFi

#endif