/***************************************************************************************************
 * @file amI2C.cpp
 * @author va3wam
 * @brief I2C bus library for ESP32 Arduino 
 * @details Twowire serial implementation of I2C bus. Supports two buses known as bus0 and bus1.
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
 * 2021-01-08 va3wam Program created
 ***************************************************************************************************/
#include <Arduino.h> // Arduino Core for ESP32. Comes with Platform.io
#include <Wire.h> // Required for serial I2C communication. Comes with Platform.io
#include <amI2C.h> // Header file for linking. 

/**
 * @brief This is the constructor for this class.
===================================================================================================*/
amI2C::amI2C()
{
    
} //amI2C::amI2C()

/**
 * @brief Identify a device found on an I2C bus based on its address
 * @param deviceAddress I2C address of the device to be identified
 * @return deviceDescription Text description of the device 
===================================================================================================*/
String amI2C::_identifyDevice(int deviceAddress)
{
  switch (deviceAddress) 
  {
    case rightOLED_I2C_ADD:    
      return "Right OLED"; 
      break;
    case leftOLED_I2C_ADD:    
      return "Left OLED";
      break;
    case MD25ADDRESS:    
      return "MD25 motor controller";
      break;
    case MPU6050_I2C_ADD:
      return "MPU6050 IMU";
      break;
    case LED_I2C_ADDRESS:
      return "16x2 LED display";
      break;
    default: 
      return "Unknown";
      break;
  } //switch
} //identifyDevice()

/**
 * @brief Scan I2C looking for connected devices in the valid address range
 * @param Bus array element
 * @note Addresses 0x00 - 0x07 and 0x78 - 0x7F are reserved I2C addresses
 * 0x00 - Reserved - General Call Address
 * 0x01 - Reserved for CBUS Compatibility
 * 0x02 - Reserved for I2C-compatible Bus Variants
 * 0x03 - Reserved for Future Use
 * 0x04, 0x05, 0x06, 0x07 - Reserved for Hs-mode Master
 * 0x78 0x79 0x7A 0x7B - Reserved for 10-bit I2C Addressing
 * 0x7C 0x7D 0x7E 0x7F - Reserved for Future Purposes
 * @note endTransmission() returns one of the following codes    
 * 0:success
 * 1:data too long to fit in transmit buffer
 * 2:received NACK on transmit of address
 * 3:received NACK on transmit of data
 * 4:other error 
===================================================================================================*/
void amI2C::_scanI2Cbus(int busNum)
{
  uint8_t testResult; // Result of check for device at a given address number
  byte count = 0; // Number of devices found on bus
  for (byte i = 8; i < 120; i++) // Scan through non-reserved addresses on bus
  { 
    if(busNum == 0) // Bus0 is called wire()
    {
      Wire.beginTransmission(i); // Begin a conversation with current address 
      testResult = Wire.endTransmission(); // Capture return code from address
    } // if
    else // Bus1 is called wire1()
    {
      Wire1.beginTransmission (i);
      testResult = Wire1.endTransmission();
    } //else
    if(testResult == 0) //  If the endTransmission returns a success code (0)
    {
      _i2cBusInfo[busNum].agentAddress[count] = i; // Record the device address 
      _i2cBusInfo[busNum].agentDescription[count] = _identifyDevice(i); // Record the device description  
      count++; // Increment the count for how many devices are on the bus
      delay(1); // Slow things down a biit to avoid over-whelming the I2C bus
    } // end of good response
  } // end of for loop
  _i2cBusInfo[busNum].agentCnt = count; // Record the final device count for the bus
  Serial.print("<amI2C::scanI2Cbus> Scanned i2c bus ");
  Serial.print(busNum);
  Serial.print (" and found ");
  Serial.print (count, DEC);
  Serial.println (" device(s)");
  for(byte i = 0; i < count; i++) // Report on what devices were found
  {
    Serial.print("<amI2C::scanI2Cbus> ... ");
    Serial.print(_i2cBusInfo[busNum].agentAddress[i], DEC);
    Serial.print(" (0x");
    Serial.print(_i2cBusInfo[busNum].agentAddress[i], HEX);
    Serial.print(") - ");
    Serial.println(_i2cBusInfo[busNum].agentDescription[i]);
  } //for
} //scanI2Cbus()

/**
 * @brief Initialize an I2C bus used by this application
=================================================================================================== */
void amI2C::configure(int bus, int sdaPin, int sclPin, uint32_t speed)
{
   if(bus==0)
   {
      Serial.print("<amI2C::configure> Configuring Wire I2C bus ");
      Wire.begin(sdaPin, sclPin, speed);
   } //if
   else
   {
      Serial.print("<amI2C::configure> Configuring Wire1 I2C bus ");
      Wire1.begin(sdaPin, sclPin, speed);
   } //else
   Serial.println(bus);
   Serial.print("<amI2C::configure> ... Speed = ");
   Serial.println(speed);
   Serial.print("<amI2C::configure> ... SDA pin = ");
   Serial.println(sdaPin);
   Serial.print("<amI2C::configure> ... SCL pin = ");
   Serial.println(sclPin);
  _scanI2Cbus(bus);
} //configure()