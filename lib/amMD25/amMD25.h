/*************************************************************************************************************************************
 * @file amMD25.h
 * @author va3wam
 * @brief MD25 dual h-bridge motorcontroller library 
 * @details I2C motor controller.
 * @copyright Copyright (c) 2021 va3wam
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
#ifndef amMD25_h
#define amMD25_h

// Define MD25 registers 
#define MD25RegSpeed1 (byte) 0x00 // Motor1 speed (mode 0,1) or both motors speed (mode 2,3)
                                  // Cast as a byte to stop them being misinterperted as NULL. This is a bug with arduino 1
#define MD25RegSpeed2 0x01 // Motor2 speed (mode 0,1) or turn (mode 2,3)
#define MD25RegEncoder1a 0x02 // Byte 1 of encoder 1 
#define MD25RegEncoder1b 0x03 // Byte 2 of encoder 1
#define MD25RegEncoder1c 0x04 // Byte 3 of encoder 1
#define MD25RegEncoder1d 0x05 // Byte 4 of encoder 1
#define MD25RegEncoder2a 0x06 // Byte 1 of encoder 2
#define MD25RegEncoder2b 0x07 // Byte 2 of encoder 2
#define MD25RegEncoder2c 0x08 // Byte 3 of encoder 2
#define MD25RegEncoder2d 0x09 // Byte 4 of encoder 2
#define MD25RegBatteryVolts 0x0A // The supply battery voltatage
#define MD25RegMotorCur1 0x0B // The current through motor 1 (Left)
#define MD25RegMotorCur2 0x0C // The current through motor 2 (Right)
#define MD25RegSoftwareRev 0x0D // Software Revision Number
#define MD25RegMotorAccel 0x0E // Optional Acceleration register
#define MD25RegMode 0x0F // Used to set the funtion of Speed1 and speed2 registers
#define MD25RegCmd 0x10 // Used for reset of encoder counts and module address changes  

// Define MD25 commands. Note changing I2C address commands not listed here
#define MD25CmdResetEncoders 0x20 // Reset the motor encoder counters
#define MD25CmdAutoSpeedRegOff 0x30 // Disable automatic speed regulation
#define MD25CmdAutoSpeedRegOn 0x31 // Enable automatic speed regulation
#define MD25CmdAutoTimeoutOff 0x32 // Disable automatic 2 second timeout of motors when no I2C activity
#define MD25CmdAutoTimeoutOn 0x33 // Enable automatic 2 second timeout of motors when no I2C activity

// Define amMD25 class
class amMD25
{
  public:
    amMD25(); // Constructor
    byte getMD25FirmwareVersion();
    void encodeReset();
    long encoder1();
    long encoder2();
    void stopMotor();
    void motorTest();
    void spinMotor(int motorNumber, int speed);
  private:
};

#endif