/*************************************************************************************************************************************
 * @file amFormat.cpp
 * @author theAgingApprentice
 * @brief Child class that extends the parent class LiquidCrystal_I2C and adds some handy functions to control I2C LCD device.  
 * @details Extends WebOTA class used for OTA code downloading. https://github.com/scottchiefbaker/ESP-WebOTA. Adds Access Point 
 * scanning and selection logic.
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
 * 2021-02-25 Old Squire Program created
 *************************************************************************************************************************************/
#include <Arduino.h> // Arduino Core for ESP32. Comes with Platform.io
#include <WiFi.h> // Required to connect to WiFi network. Comes with Platform.io
#include <amWiFi.h> // Required for Webserver data type
#include <known_networks.h> // Defines Access points and passwords that the robot can scan for and connect to

/**
 * @brief This is the constructor for this class.
 * @details This class extends the LiquidCrystal_I2C class
===================================================================================================*/
amWiFi::amWiFi() // Constructor for this class
   :WebOTA() // Constructor for parent class WebOTA
{
//   this-> xxx(); // Call function defined in parent class LiquidCrystal_I2C. Initialize LCD. 
} //amWiFi::amWiFi()

/**
 * @brief This function translates the type of encryption that an Access Point (AP) advertises (an an ENUM) 
 * and returns a more human readable description of what that encryption method is.
 * @param wifi_auth_mode_t encryptionType is the ENUM that the Access Point advertises for encryption.
 * @return String containing human readable label for the type of encryption the Access Point uses.
 */
String amWiFi::_translateEncryptionType(wifi_auth_mode_t encryptionType)
{
   switch (encryptionType)
   {
      case (WIFI_AUTH_OPEN):
         return "Open";
      case (WIFI_AUTH_WEP):
         return "WEP";
      case (WIFI_AUTH_WPA_PSK):
         return "WPA_PSK";
      case (WIFI_AUTH_WPA2_PSK):
         return "WPA2_PSK";
      case (WIFI_AUTH_WPA_WPA2_PSK):
         return "WPA_WPA2_PSK";
      case (WIFI_AUTH_WPA2_ENTERPRISE):
         return "WPA2_ENTERPRISE";
      default:
         return "UNKNOWN";
   } //switch
} //amWiFi::translateEncryptionType()

/**
 * @brief Scan the 2.4Ghz band for known Access Points and select the one with the strongest signal.
 * @return const char _MySSID which is the SSID of the best AP candidate to connect to
=================================================================================================== */
const char* amWiFi::_lookForAP()
{
   _mySSID = "unknown"; //  At the start no known Access Point has been foundto connect to
   int numberOfNetworks = WiFi.scanNetworks(); // Used to track how many APs are detected by the scan
   int StrongestSignal = -127; // Used to find the strongest signal. Set as low as possible to start
   int SSIDIndex = 0; // Contains the SSID index number from the known list of APs
   bool APknown; // Flag to indicate if the current AP appears in the known AP list
//   Serial.println("<amWiFi::lookForAP> Scanning for WiFi Access Points.");
//   Serial.print("<amWiFi::lookForAP> Number of networks found: ");
   Serial.println(numberOfNetworks);

   // Loop through all detected APs
   for (int i = 0; i < numberOfNetworks; i++)
   {
      APknown = false;
//      Serial.print("<amWiFi::lookForAP> Network name: ");
//      Serial.println(WiFi.SSID(i));
//      Serial.print("<amWiFi::lookForAP> Signal strength: ");
//      Serial.println(WiFi.RSSI(i));
//      Serial.print("<amWiFi::lookForAP> MAC address: ");
//      Serial.println(WiFi.BSSIDstr(i));
//      Serial.print("<amWiFi::lookForAP> Encryption type: ");
//      String encryptionTypeDescription = _translateEncryptionType(WiFi.encryptionType(i));
//      Serial.println(encryptionTypeDescription);
   
      // Scan table of known APs to see if the current AP is known to us
      for (int j = 0; j < numKnownAPs; j++)
      {
         // If the current scanned AP appears in the known AP list note the index value and flag found
         if (WiFi.SSID(i) == SSID[j])
         {
            APknown = true;
            SSIDIndex = j;
//            Serial.println("<amWiFi::lookForAP> This is a known network");
         } //if
      }   //for

    // If the current AP is known and has a stronger signal than the others that have been checked
    // then store it in the variables that will be used to connect to the AP later
      if ((APknown == true) && (WiFi.SSID(i).toInt() > StrongestSignal))
      {
         _mySSID = SSID[SSIDIndex].c_str();
         _myPassword = Password[SSIDIndex].c_str();
         StrongestSignal = WiFi.SSID(i).toInt();
//         Serial.println("<amWiFi::lookForAP> This is the strongest signal so far");
      } //if
//      Serial.println("<amWiFi::lookForAP> -----------------------");
   } //for

//   Serial.print("<amWiFi::lookForAP> Best known Access Point candidate = ");
//   Serial.println(_mySSID);
   return _mySSID;
}  //amWiFi::lookForAP()

/**
 * @brief Return the SSID of the Access Point the we are using.
 * @return String WiFi.SSID() Name of the AP  
===================================================================================================*/
String amWiFi::getAccessPointName()
{
   return WiFi.SSID();
} //amWiFi::getAccessPointName()

/**
 * @brief Return the MAC of the Access Point of our WiFi radio transmitter.
 * @return String WiFi.macAddress() MAC address of WiFi radio  
===================================================================================================*/
String amWiFi::getMacAddress()
{
   return WiFi.macAddress();
} //amWiFi::getMACaddress()

/**
 * @brief Return the IP address that was assigned to us by the DHCP service on the local router.
 * @return IPAddress WiFi.localIP() IPv4 address assigned by APs DHCP service.    
===================================================================================================*/
IPAddress amWiFi::getIpAddress()
{
   return WiFi.localIP();
} //amWiFi::getIpAddress()

/**
 * @brief Return the robot name which is both unique and predictable.
 * @return String HOST_NAME_PREFIX  + _format.noColonMAC(getMacAddress()).   
===================================================================================================*/
String amWiFi::getUniqueName()
{
   return HOST_NAME_PREFIX  + _format.noColonMAC(getMacAddress());
} //amWiFi::getUniqueName()

/**
 * @brief Return the URL that operators can use to access the OTA web page hosted by the robot.
 * @return String "http://" + _format.ipToString(getIpAddress()) + ":" + OTA_LOCAL_PORT + "/" + OTA_LOCAL_PATH.   
===================================================================================================*/
String amWiFi::getOtaUrl()
{
   return "http://" + _format.ipToString(getIpAddress()) + ":" + OTA_LOCAL_PORT + OTA_LOCAL_PATH;
} //amWiFi::getOtaUrl()

/**
 * @brief Manage the process of connecting to thr WiFi network.
 * @details Scan for a suitable Access Point and if one is found then connect to it using the 
 * init_wifi() method. This method connects to the Access Point, Starts up a webserver that serves
 * up a web page that does Over The Air updates over websockets.
===================================================================================================*/
void amWiFi::connect()
{
   _lookForAP(); // Scan the 2.4Ghz band for known Access Points and select the one with the strongest signal 
   if((String)_mySSID != "unknown") // If a suitable Access point is found 
   {
      init_wifi(_mySSID, _myPassword, OTA_LOCAL_PATH); // Connect to AP, Configure web server settings
      webota.init();	// Start webserver with the default port of 8080 and local path of "/webota"
   } //if
} //amWiFi::connect()

/**
 * @brief Send detailed network configuration information to the console.
 * @details The ESP32-D0WDQ6 chip has dual Tensilica LX6 micro controllers. All WiFi logic runs on 
 * the PRO_CPU (core0) aka the processor core. This firmware runs on the APP_CPU (core 1) aka the 
 * application core.     
===================================================================================================*/
void amWiFi::cfgToConsole()
{
   Serial.println("<amWiFi::cfgToConsole> Network settings:");  
   Serial.print("<amWiFi::cfgToConsole> ... Access Point Name = "); Serial.println(getAccessPointName()); 
   Serial.print("<amWiFi::cfgToConsole> ... Robot MAC address: "); Serial.println(getMacAddress());
   Serial.print("<amWiFi::cfgToConsole> ... Robot IP address: "); Serial.println(getIpAddress());
   Serial.print("<amWiFi::cfgToConsole> ... Robot Host Name: "); Serial.println(getUniqueName());   
   Serial.print("<amWiFi::cfgToConsole> ... Robot OTA URL: "); Serial.println(getOtaUrl());   
} //amWiFi::cfgToConsole()
