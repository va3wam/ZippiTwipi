/***************************************************************************************************
 * @file amMD25.cpp
 * @author va3wam
 * @brief Arduino library for controlling the MD25 dual h-bride motor  controller 
 * @details Used I2C communication. Maximum clock frequency  of bus is 100000bps 
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
 * 2021-01-10 va3wam Program created
 ***************************************************************************************************/
#include <Arduino.h> // Arduino Core for ESP32. Comes with Platform.io
#include <Wire.h> // Required for serial I2C communication. Comes with Platform.io
#include <amI2C.h> // Required for I2C address of MD25
#include <amMD25.h> // Header file for linking. 

    // Define global variables
/*
    bool printHeaders = true;
    long currentCnt1; 
    long prevCnt1 = 0;
    long errFilter = 100;
    long successCount1 = 0;
    long failCount1 = 0;
    long totalCount1 = 0;
    long percentage1 = 0; 
*/

/**
 * @brief This is the constructor for this class.
===================================================================================================*/
amMD25::amMD25()
{

} //amI2Cbus::amI2Cbus()

/** 
 * @brief Function that gets the software version 
=================================================================================================== */
byte amMD25::getMD25FirmwareVersion()
{                                               
  Serial.println("<getMD25FirmwareVersion> Retrieving firmware version from MD25");
  Wire.beginTransmission(MD25ADDRESS); // Request token to transmit on I2C bus
  Wire.write(MD25RegSoftwareRev); // Send byte to read software version as a single byte
  Wire.endTransmission(); // End request for token
  Wire.requestFrom(MD25ADDRESS, 1); // Request 1 byte from MD25 address register
  while(Wire.available() < 1); // Wait for reply to request for address to arrive
  byte software = Wire.read(); // Read it in
  Serial.print("<getMD25FirmwareVersion> Response to request for software version is ");
  Serial.println(software, HEX);
  return(software); // Return address to 
} //getMD25FirmwareVersion()

/** 
 * @brief This function resets the encoder values to 0
=================================================================================================== */
void amMD25::encodeReset()
{                                       
  Wire.beginTransmission(MD25ADDRESS); // Start conversation with motor controller
  Wire.write(MD25RegCmd); // Set up command register
  Wire.write(MD25CmdResetEncoders); // Send command to reset motor encoders
  Wire.endTransmission(); 
} //encodeReset()

/** 
 * @brief Function to reads the value of encoder 1 as a long
=================================================================================================== */
long amMD25::encoder1()
{                                            
  Wire.beginTransmission(MD25ADDRESS); // Send byte to get a reading from encoder 1
  Wire.write(MD25RegEncoder1a);
  Wire.endTransmission();
  
  Wire.requestFrom(MD25ADDRESS, 4); // Request 4 bytes from MD25
  while(Wire.available() < 4); // Wait for 4 bytes to arrive
  long poss1 = Wire.read(); // First byte for encoder 1, HH.
  poss1 <<= 8;
  poss1 += Wire.read(); // Second byte for encoder 1, HL
  poss1 <<= 8;
  poss1 += Wire.read(); // Third byte for encoder 1, LH
  poss1 <<= 8;
  poss1 +=Wire.read(); // Fourth byte for encoder 1, LL

  delay(50); // Wait for everything to make sure everything is sent  
  return(poss1);
} //encoder1()

/** 
 * @brief Function to reads the value of encoder 2 as a long
=================================================================================================== */
long amMD25::encoder2()
{                                            
  Wire.beginTransmission(MD25ADDRESS);           
  Wire.write(MD25RegEncoder2a);
  Wire.endTransmission();
  
  Wire.requestFrom(MD25ADDRESS, 4); // Request 4 bytes from MD25
  while(Wire.available() < 4); // Wait for 4 bytes to become available
  long poss2 = Wire.read();
  poss2 <<= 8;
  poss2 += Wire.read();                
  poss2 <<= 8;
  poss2 += Wire.read();                
  poss2 <<= 8;
  poss2 +=Wire.read();               
  
  delay(50); // Wait to make sure everything is sent   
  return(poss2);
} //encoder2()

/** 
 * @brief Function to stop motors
=================================================================================================== */
void amMD25::stopMotor()
{                                       
  Wire.beginTransmission(MD25ADDRESS);
  Wire.write(MD25RegSpeed2);
  Wire.write(128); // Sends a value of 128 to motor 2 this value stops the motor
  Wire.endTransmission();
  
  Wire.beginTransmission(MD25ADDRESS);
  Wire.write(MD25RegSpeed1);
  Wire.write(128); // Sends a value of 128 to motor 2 this value stops the motor
  Wire.endTransmission();
}  //stopMotor()

/** 
 * @brief Function to spin one or both motors at specified speed
 * @param motorNumber 0=left, 1=right, 2=both
 * @param speed How many encoder ticks to move
 * @note See full detais at https://www.pishrobot.com/files/products/datasheets/md25.pdf
=================================================================================================== */
void amMD25::spinMotor(int motorNumber, int speed)
{
  switch(motorNumber)
  {
    case 0: // Left motor 
      Serial.print("<amMD25::spinMotor> Spin left motor");
      Wire.beginTransmission(MD25ADDRESS); // Request start transmit to the MD25 H-bridge
      Wire.write(MD25RegMode); // Going to write to the mode register
      Wire.write(0); // Writing 0 sets speed controls to 0 (Full Reverse), 128 (Stop), 255 (Full Forward)
      delay(1); // Give bus a breather
      Wire.beginTransmission(MD25ADDRESS); // Request start transmit to the MD25 H-bridge
      Wire.write(MD25RegSpeed1); // Indicate motor1 speed register
      Wire.write(speed); // 1-127 = backwards, 128 = stop, 129-255 = forward                                           
      Wire.endTransmission(); // End transmit
      break;
    case 1: // Right motor 
      Serial.print("<amMD25::spinMotor> Spin right motor");
      Wire.beginTransmission(MD25ADDRESS); // Request start transmit to the MD25 H-bridge
      Wire.write(MD25RegMode); // Going to write to the mode register
      Wire.write(0); // Writing 0 sets speed controls to 0 (Full Reverse), 128 (Stop), 255 (Full Forward)
      delay(1); // Give bus a breather
      Wire.beginTransmission(MD25ADDRESS); // Request start transmit to the MD25 H-bridge
      Wire.write(MD25RegSpeed2); // Indicate motor1 speed register
      Wire.write(speed); // 1-127 = backwards, 128 = stop, 129-255 = forward                                           
      Wire.endTransmission(); // End transmit
      break;
    default: // Both motors
      Serial.println("<amMD25::spinMotor> Spin both motors");
      Wire.beginTransmission(MD25ADDRESS); // Request start transmit to the MD25 H-bridge
      Wire.write(MD25RegMode); // Going to write to the mode register
      Wire.write(2); // Writing 2 to the mode register will make speed1 control both motors speed
      delay(1); // Give bus a breather
      Wire.beginTransmission(MD25ADDRESS); // Request start transmit to the MD25 H-bridge
      Wire.write(MD25RegSpeed1); // Indicate motor1 speed register
      Wire.write(speed); // 1-127 = backwards, 128 = stop, 129-255 = forward                                           
      Wire.endTransmission(); // End transmit
      break;
  } // motorNumber;
} // spinMotor()

/**
 * @brief Run motor test code
=================================================================================================== */
/*
void amMD25::motorTest()
{
  if(printHeaders == true)
  {
    Serial.println("<loop> curr count , prev count , Filter , Succ , Fail , percent"); // Headings for output
    printHeaders = false;
  } //if
  currentCnt1 = encoder1();
  if(currentCnt1 < prevCnt1 + errFilter) // If there is a large jump in the counter
  {
    prevCnt1 = currentCnt1;
    successCount1 ++;
  } //if
  else
  {
    failCount1 ++;
    totalCount1 = successCount1 + failCount1;
    percentage1 = failCount1 / successCount1;
    Serial.print("<loop> ");
    Serial.print(currentCnt1);
    Serial.print(",");
    Serial.print(prevCnt1);
    Serial.print(",");
    Serial.print(errFilter);
    Serial.print(",");
    Serial.print(successCount1);
    Serial.print(",");
    Serial.print(failCount1);
    Serial.print(", ");
    Serial.println(percentage1);
  } //else
} // motorTest()
*/