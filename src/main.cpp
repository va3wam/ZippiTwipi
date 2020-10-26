/*************************************************************************************************************************************
 * @file main.cpp
 * @author va3wam
 * @brief Zippi Twipi robot firmware 
 * @details Basic operating code for the Zippi Twipi robot 
 * @copyright Copyright (c) 2020
 * YYYY-MM-DD Dev    Description
 * ---------- ------ -----------------------------------------------------------------------------------------------------------------
 * 2020-10-22 va3wam Program created
 *************************************************************************************************************************************/
#include <Arduino.h> // Arduino Core for ESP32. Comes with Platform.io
#include <Wire.h> // Required for serial IIC communication. Comes with Platform.io 

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Define I2C bus0 - wire() - constants, classes and global variables 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define I2C_bus0_speed 400000 // Define speed of I2C bus 2. Note 400KHz is the upper speed limit for ESP32 I2C
#define I2C_bus0_SDA 22 // Define pin on the board used for Serial Data Line (SDA) for I2C bus 0
#define I2C_bus0_SCL 14 // Define pin on the board used for Serial Clock Line (SCL) for I2C bus 0

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Define I2C bus1 - wire1() - constants, classes and global variables 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define I2C_bus1_speed 100000 // Define speed of I2C bus 2. Note 100KHz is the upper speed limit for ESP32 I2C
#define I2C_bus1_SDA 17 // Define pin on the board used for Serial Data Line (SDA) for I2C bus 1
#define I2C_bus1_SCL 21 // Define pin on the board used for Serial Clock Line (SCL) for I2C bus 1

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Define I2C device addresses 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define MPU6050_I2C_ADD 0x68 // GY521 I2C address
#define leftOLED_I2C_ADD 0x3D // OLED used for robot's left eye I2C adddress
#define rightOLED_I2C_ADD 0x3C // OLED used for robot' right eye I2C address
#define MD25ADDRESS 0xB0 >> 1 // Wire Library only uses 7 bit addresses so you need to shift address one bit to the right

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Define MD25 registers 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define CMD                 (byte)0x00                        // Values of 0 eing sent using write have to be cast as a byte to stop them being misinterperted as NULL
                                                              // This is a but with arduino 1
#define SOFTWAREREG         0x0D                              // Byte to read the software version
#define SPEED1              (byte)0x00                        // Byte to send speed to first motor
#define SPEED2              0x01                              // Byte to send speed to second motor
#define ENCODERONE          0x02                              // Byte to read motor encoder 1
#define ENCODERTWO          0x06                              // Byte to read motor encoder 2
#define VOLTREAD            0x0A                              // Byte to read battery volts
#define RESETENCODERS       0x20

/*************************************************************************************************************************************
 * @brief Identify a device based on its I2C address
 * @param deviceAddress I2C address of the device to be identified
 *************************************************************************************************************************************/
void identifyDevice(int deviceAddress)
{
  Serial.print ("<identifyDevice> Device with I2C address ");
  Serial.print (deviceAddress, DEC);
  Serial.print (" (0x");
  Serial.print (deviceAddress, HEX);
  Serial.print (")");
  switch (deviceAddress) 
  {
    case rightOLED_I2C_ADD:    
      Serial.println(" identified as Right OLED");
      break;
    case leftOLED_I2C_ADD:    
      Serial.println(" identified as Left OLED");
      break;
    case MD25ADDRESS:    
      Serial.println(" identified as MD25 motor controller");
      break;
    case MPU6050_I2C_ADD:
      Serial.println(" identified as MPU6050");
      break;
    default: 
      Serial.println("Identified as UKNOWN");
      break;
  } //switch
} //identifyDevice()

/*************************************************************************************************************************************
 * @brief Scan I2C bus0 aka wire()
 * @param deviceAddress 
 *************************************************************************************************************************************/
void scanBus0()
{
  Serial.println ("<scanBus0> Scan I2C bus 0 looking for devices...");
  byte count = 0;
  for (byte i = 8; i < 120; i++)
  { 
    Wire.beginTransmission (i);
    if (Wire.endTransmission () == 0)
    {
      Serial.print ("<scanBus0> Found address: ");
      Serial.print (i, DEC);
      Serial.print (" (0x");
      Serial.print (i, HEX);
      Serial.println (")");
      count++;
      identifyDevice(i); // Identify what device has been found 
      delay (1);
    } // end of good response
  } // end of for loop
  Serial.println ("<scanBus0> Done");
  Serial.print ("<scanBus0> Found ");
  Serial.print (count, DEC);
  Serial.println (" device(s)");
} //scanBus0()

/*************************************************************************************************************************************
 * @brief Scan I2C bus1 aka wire1()
 * @param deviceAddress 
 *************************************************************************************************************************************/
void scanBus1()
{
  Serial.println ("<scanBus1> Scan I2C bus 1 looking for devices...");
  byte count = 0;
  for (byte i = 8; i < 120; i++)
  { 
    Wire1.beginTransmission (i);
    if (Wire1.endTransmission () == 0)
    {
      Serial.print ("<scanBus1> Found address: ");
      Serial.print (i, DEC);
      Serial.print (" (0x");
      Serial.print (i, HEX);
      Serial.println (")");
      count++;
      identifyDevice(i); // Identify what device has been found 
      delay(1);
    } // end of good response
  } // end of for loop
  Serial.println ("<scanBus1> Done");
  Serial.print ("<scanBus1> Found ");
  Serial.print (count, DEC);
  Serial.println (" device(s)");
} //scanBus1()

/** 
 * @brief Function that gets the software version 
=================================================================================================== */
byte getMD25FirmwareVersion()
{                                               
  Serial.println("<getMD25FirmwareVersion> Retrieving firmware version from MD25");
  Wire.beginTransmission(MD25ADDRESS); // Request token to transmit on I2C bus
  Wire.write(SOFTWAREREG); // Send byte to read software version as a single byte
  Wire.endTransmission(); // End request for token
  Wire.requestFrom(MD25ADDRESS, 1); // Request 1 byte from MD25 address register
  while(Wire.available() < 1); // Wait for reply to request for address to arrive
  byte software = Wire.read(); // Read it in
  Serial.print("<getMD25FirmwareVersion> Response to request for software version is ");
  Serial.println(software, HEX);
  return(software); // Return address to 
} //getMD25FirmwareVersion()

/** 
 * @brief Standard set up routine for Arduino programs 
=================================================================================================== */
void setup()
{
    Serial.begin(115200); // Open a serial connection at 115200bps
    while (!Serial) ;     // Wait for Serial port to be ready
    Serial.println("<setup> Start of setup");
    Wire.begin(I2C_bus0_SDA,I2C_bus0_SCL,I2C_bus0_speed);
    scanBus0();
    Wire1.begin(I2C_bus1_SDA,I2C_bus1_SCL,I2C_bus1_speed);
    scanBus1();
    byte softVer = getMD25FirmwareVersion(); // Gets the software version of MD25
    Serial.print("<setup> MD25 firmware version is ");
    Serial.println(softVer, HEX);
    Serial.println("<setup> End of setup");
} //setup()

/**
 * @brief Standard looping routine for Arduino programs
=================================================================================================== */
void loop()
{
} //loop()