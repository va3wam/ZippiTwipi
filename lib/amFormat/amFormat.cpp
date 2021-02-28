/*************************************************************************************************************************************
 * @file amFormat.cpp
 * @author theAgingApprentice
 * @brief A collection of handy formatting functions. 
 * @details Converts datatypes in ways that makes it possib;e to use the output of one library as input for another.
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
#include <amFormat.h> // Header file for linking.

/**
 * @brief This is the constructor for this class.
===================================================================================================*/
amFormat::amFormat()
{
   
} //amFormat::amFormat()

/** 
 * @brief Strips the colons off the MAC address of this device
 * @param String containing a MAC address with colons (:) embedded in it
 * @return String with MAC address containing no colons
 * =============================================================================== */
String amFormat::noColonMAC(String macAddress)
{
//   Serial.println("<amFormat::noColonMAC> Removing colons from MAC address");
   macAddress.remove(2, 1);        // Remove first colon from MAC address
   macAddress.remove(4, 1);        // Remove second colon from MAC address
   macAddress.remove(6, 1);        // Remove third colon from MAC address
   macAddress.remove(8, 1);        // Remove forth colon from MAC address
   macAddress.remove(10, 1);       // Remove fifth colon from MAC address
//   Serial.print("<amFormat::noColonMAC> Formatted MAC address without colons = ");
//   Serial.println(macAddress);
   return macAddress;
}  //amFormat::noColonMAC()

/** 
 * @brief Converts a string to upper case
 * @details See https://stackoverflow.com/questions/735204/convert-a-string-in-c-to-upper-case
 * @param String any valid string
 * @return String converted to all caps
 * =============================================================================== */
String amFormat::StringToUpper(String strToConvert) // convert the argument string to upper case
{
    std::transform(strToConvert.begin(), strToConvert.end(), strToConvert.begin(), ::toupper);
    return strToConvert;
} //amFormat::StringToUpper()

/** 
 * @brief Converts an IP address to a string
 * @param IPAddress any valid IP address 
 * @return String containing an IP address
 * =============================================================================== */
String amFormat::ipToString(IPAddress ip)
{
   String s = "";
   for (int i = 0; i < 4; i++)
   {
      s += i ? "." + String(ip[i]) : String(ip[i]);
   } //for
//   Serial.print("<ipToString> IP Address = ");
//   Serial.println(s);
   return s;
}  //amFormat::ipToString()
