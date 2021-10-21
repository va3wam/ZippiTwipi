#ifndef mobility_h // Start of precompiler check to avoid dupicate inclusion of this code block.

#define mobility_h // Precompiler macro used for precompiler check.

#include <main.h> // Header file for all libraries needed by this program.
bool mobilityStatus = false;
 
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
#define MD25CmdAutoTimeoutOn 0x33 // Enable automatic 2 second timeout of motors when no I2C activitybool mobilityStatus = false; 

/** 
 * @brief Reports the firmware version running on the MD25 motor controller. 
=================================================================================================== */
byte getMD25Version()
{                                               
   Log.verboseln("<amMD25::getFirmwareVersion> Retrieving firmware version from MD25");
   Wire.beginTransmission(md25I2cAddress); // Request token to transmit on I2C bus
   Wire.write(MD25RegSoftwareRev); // Send byte to read software version as a single byte
   Wire.endTransmission(); // End request for token
   Wire.requestFrom(md25I2cAddress, 1); // Request 1 byte from MD25 address register
   while(Wire.available() < 1); // Wait for reply to request for address to arrive
   byte software = Wire.read(); // Read it in
   return(software); // Return address to 
} //getMD25FirmwareVersion()

/** 
 * @brief Function to spin one or both motors at specified speed
 * @param motorNumber 0=left, 1=right, 2=both
 * @param speed How many encoder ticks to move
 * @note See full detais at https://www.pishrobot.com/files/products/datasheets/md25.pdf
=================================================================================================== */
void spinMotor(int motorNumber, int speed)
{
  switch(motorNumber)
  {
      case 0: // Left motor 
         Log.verboseln("<spinMotor> Spin left motor");
         Wire.beginTransmission(md25I2cAddress); // Request start transmit to the MD25 H-bridge
         Wire.write(MD25RegMode); // Going to write to the mode register
         Wire.write(0); // Writing 0 sets speed controls to 0 (Full Reverse), 128 (Stop), 255 (Full Forward)
         Wire.endTransmission(); // End transmit
   //      delay(1); // Give bus a breather
         Wire.beginTransmission(md25I2cAddress); // Request start transmit to the MD25 H-bridge
         Wire.write(MD25RegSpeed1); // Indicate motor1 speed register
         Wire.write(speed); // 1-127 = backwards, 128 = stop, 129-255 = forward                                           
         Wire.endTransmission(); // End transmit
         break;
      case 1: // Right motor 
         Log.verboseln("<spinMotor> Spin right motor");
         Wire.beginTransmission(md25I2cAddress); // Request start transmit to the MD25 H-bridge
         Wire.write(MD25RegMode); // Going to write to the mode register
         Wire.write(0); // Writing 0 sets speed controls to 0 (Full Reverse), 128 (Stop), 255 (Full Forward)
         Wire.endTransmission(); // End transmit
   //      delay(1); // Give bus a breather
         Wire.beginTransmission(md25I2cAddress); // Request start transmit to the MD25 H-bridge
         Wire.write(MD25RegSpeed2); // Indicate motor1 speed register
         Wire.write(speed); // 1-127 = backwards, 128 = stop, 129-255 = forward                                           
         Wire.endTransmission(); // End transmit
         break;
      default: // Both motors
         Log.verboseln("<spinMotor> Spin both motors");
         Wire.beginTransmission(md25I2cAddress); // Request start transmit to the MD25 H-bridge
         Wire.write(MD25RegMode); // Going to write to the mode register
         Wire.write(2); // Writing 2 to the mode register will make speed1 control both motors speed
         Wire.endTransmission(); // End transmit
   //      delay(1); // Give bus a breather
         Wire.beginTransmission(md25I2cAddress); // Request start transmit to the MD25 H-bridge
         Wire.write(MD25RegSpeed1); // Indicate motor1 speed register
         Wire.write(speed); // 1-127 = backwards, 128 = stop, 129-255 = forward                                           
         Wire.endTransmission(); // End transmit
         break;
  } // switch
} // spinMotor()

/**
 * @brief Initialize robot drive train.
 * @details Robot moves on two DC motors driven via an I2C motor controller.
 * ==========================================================================*/
bool initMobility()
{
   bool mStatus = true; // Status of mobility platform.
   Log.traceln("<initMobility> Initialize the drive train for this platform.");
   Log.verboseln("<initMobility> MD25 driver firmware version = %d", getMD25Version());
   spinMotor(2,255);
   return mStatus;
} // initMobility. 

#endif // End of precompiler protected code block