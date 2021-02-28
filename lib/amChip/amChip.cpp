/*************************************************************************************************************************************
 * @file amChip.cpp
 * @author theAgingApprentice
 * @brief Common API for retrieving ESP32 chip configuration information. 
 * @details Retrieve chip configuration details from both FreeRTOS and the ESP32 Arduino framework.
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
 * 2021-02-27 Old Squire Moved timer related code to amReset()
 * 2021-02-25 Old Squire Program created
 *************************************************************************************************************************************/
#include <Arduino.h> // Arduino Core for ESP32. Comes with Platform.io
#include <amChip.h> // Header file for linking.

/**
 * @brief This is the constructor for this class.
===================================================================================================*/
amChip::amChip()
{
   serialSpeed = serialBaudRate;

} //amChip::amChip()

/**
 * @brief Returns the revision number of the ESP32 chip.
 * @return uint32_t ESP.getChipRevision()   
===================================================================================================*/
uint8_t amChip::getRevision()
{
   return ESP.getChipRevision();
} //amChip::getRevision()

/**
 * @brief Returns the size of the binary file in bytes.
 * @return uint32_t ESP.getSketchSize()   
===================================================================================================*/
uint32_t amChip::getCodeSize()
{
   return ESP.getSketchSize();
} //amChip::getCodeSize()

/**
 * @brief Returns the amount of free heap (sorted memory the program can use) in bytes.
 * @return uint32_t ESP.getFreeHeap()   
===================================================================================================*/
uint32_t amChip::getFreeHeap()
{
   return ESP.getFreeHeap();
} //amChip::getFreeHeap()

/**
 * @brief Returns the current speed setting of the serial port (baud rate).
 * @return uint32_t Serial.baudRate()   
===================================================================================================*/
uint32_t amChip::getSerialSpeed()
{
   return Serial.baudRate();
} //amChip::getSerialSpeed()

/**
 * @brief Returns the CPU ID of the application core.
 * @return uint32_t xPortGetCoreID()   
===================================================================================================*/
uint32_t amChip::getCpuId()
{
   return xPortGetCoreID();
} //amChip::getCpuId()

/**
 * @brief Returns the clock speed of the application core in Mhz.
 * @return uint32_t Serial.baudRate()   
===================================================================================================*/
uint32_t amChip::getCpuClock()
{
   return getCpuFrequencyMhz();
} //amChip::getCpuClock()

/**
 * @brief Send detailed chip configuration information to the console.
 * @details The ESP32-D0WDQ6 chip has dual Tensilica LX6 micro controllers. All WiFi logic runs on 
 * the PRO_CPU (core0) aka the processor core. This firmware runs on the APP_CPU (core 1) aka the 
 * application core.     
===================================================================================================*/
void amChip::cfgToConsole()
{
   Serial.println("<amChip::cfgToConsole> APP_CPU (core 1) settings:");  
   Serial.print("<amChip::cfgToConsole> ... Chip revision = "); Serial.println(getRevision()); 
   Serial.print("<amChip::cfgToConsole> ... SDK version = "); Serial.println(ESP.getSdkVersion()); 
   Serial.print("<amChip::cfgToConsole> ... Sketch size = "); Serial.print(getCodeSize()); Serial.println(" bytes");  
   Serial.print("<amChip::cfgToConsole> ... Free heap = "); Serial.print(getFreeHeap()); Serial.println(" bytes"); 
   Serial.print("<amChip::cfgToConsole> ... Serial baud rate = "); Serial.print(getSerialSpeed()); Serial.println(" Hz");
   Serial.print("<amChip::cfgToConsole> ... Arduino core = "); Serial.println(getCpuId());
   Serial.print("<amChip::cfgToConsole> ... Arduino core clock frequency = "); Serial.print(getCpuClock()); Serial.println(" MHz");
} //amChip::cfgToConsole()