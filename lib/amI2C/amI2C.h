/*************************************************************************************************************************************
 * @file amI2C.h
 * @author va3wam
 * @brief I2C bus library for ESP32 Arduino 
 * @details Twowire serial implementation of I2C bus. Supports two buses known as bus0 and bus1.
 * @copyright Copyright (c) 2020 va3wam@gmail.com
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
 * YYYY-MM-DD Dev    Description
 * ---------- ------ -----------------------------------------------------------------------------------------------------------------
 * 2021-01-08 va3wam Program created
 *************************************************************************************************************************************/
#ifndef amI2C_h
#define amI2C_h

// Define I2C bus constants, classes and global variables 
#define I2C_bus0_speed 400000 // Define speed of I2C bus 0. Note 400KHz is the upper speed limit for ESP32 I2C
#define I2C_bus1_speed 100000 // Define speed of I2C bus 2. Note 100KHz is the upper speed limit for MD25 H-Bridge
#define numOfI2cBuses 2 // Define how many I2C buses the robot has on it's circuit board
#define maxDevicesPerBus 4 // Define the maximum number of devices that can be on an I2C bus
typedef struct 
{
  int agentCnt; // Number of agent devices on this bus
  int agentAddress[maxDevicesPerBus]; // Addresses of the agent devices on this bus
  String agentDescription[maxDevicesPerBus]; // Descriptions of the agent devices on this bus 
}i2cBusData; // i2c bus configuration structure

// Define I2C device addresses 
#define MPU6050_I2C_ADD 0x68 // GY521 I2C address
#define leftOLED_I2C_ADD 0x3D // OLED used for robot's left eye I2C adddress
#define rightOLED_I2C_ADD 0x3C // OLED used for robot' right eye I2C address
#define MD25ADDRESS 0xB0 >> 1 // Wire Library only uses 7 bit addresses so you need to shift address one bit to the right
#define LED_I2C_ADDRESS 0x3F // 16x2 LED used as main display

// Define amI2C class
class amI2C
{
  public:
    amI2C();
    void configure(int bus, int sdaPin, int sclPin, uint32_t speed);
  private:
    String _identifyDevice(int deviceAddress);
    void _scanI2Cbus(int busNum);
    i2cBusData _i2cBusInfo[numOfI2cBuses]; // Define array of I2C bus objects used in this robot 
};

#endif