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

// Define easily understood references to differentiate each motor and associated encoder.
const bool LEFT_SIDE = 0; // Motor and encoder on left side of robot.
const bool RIGHT_SIDE = 1; // Motor and encoder on right side of robot.

/** 
 * @brief Reads one byte from the MD25 regster via I2C.
 * @param The register to be read.
 * @return The value stored in the specified register. 
=================================================================================================== */
byte readRegisterByte(byte reg)
{
   Wire.beginTransmission(md25I2cAddress); // Request token to transmit on I2C bus
   Wire.write(reg); // Send byte to read software version as a single byte
   Wire.endTransmission(); // End request for token
   Wire.requestFrom(md25I2cAddress, 1); // Request 1 byte from MD25 address register
   while(Wire.available() < 1); // Wait for reply to request for address to arrive
   return(Wire.read()); // Return address to   
} // readRegisterByte

/** 
 * @brief Reads one byte from the MD25 regster via I2C.
 * @param reg specifies which register to be read.
 * @return The value stored in the specified register. 
=================================================================================================== */
long readEncoderArray(byte reg)
{
   Wire.beginTransmission(md25I2cAddress); // Get the attention of the MD25 controller.
   Wire.write(reg); // Tell the controller which encoder we wish to read.
   Wire.endTransmission(); // Release control of the I2C bus.
   Wire.requestFrom(md25I2cAddress, 4); // Request 4 bytes from MD25
   while(Wire.available() < 4); // Wait for 4 bytes to arrive
   long position = Wire.read(); // First byte for encoder 1, HH.
   position <<= 8;
   position += Wire.read(); // Second byte for encoder 1, HL
   position <<= 8;
   position += Wire.read(); // Third byte for encoder 1, LH
   position <<= 8;
   position +=Wire.read(); // Fourth byte for encoder 1, LL
   return(position);
} // readEncoderArray()

/** 
 * @brief Get current reading of the specified motor encoder.
 * @param reg specifies if the left or right encoder is to be read.
 * @return value of the specified encoder. 
=================================================================================================== */
long getEncoder(bool reg)
{                                            
   if(reg == LEFT_SIDE) 
   {
      return(readEncoderArray(MD25RegEncoder1a)); // Return encoder reading for left motor.
   } // if
   else
   {
      return(readEncoderArray(MD25RegEncoder2a)); // Return encoder reading for right motor.
   } // else
} // getEncoder()

/** 
 * @brief Reports the firmware version running on the MD25 motor controller. 
=================================================================================================== */
byte getMD25Version()
{                                               
   return(readRegisterByte(MD25RegSoftwareRev));
} //getMD25Version()

/** 
 * @brief Reset both motor encoder values to 0.
=================================================================================================== */
void resetEncoder()
{                                       
  Wire.beginTransmission(md25I2cAddress); // Start conversation with motor controller
  Wire.write(MD25RegCmd); // Set up command register
  Wire.write(MD25CmdResetEncoders); // Send command to reset motor encoders
  Wire.endTransmission(); 
} //encodeReset()

void setMode(int8_t mode)
{
   String explanation[4] = {"0 (full reverse) 128 (stop) 255 (full forward)",
                            "-128 (full reverse), 0 (Stop), 127 (full forward)",
                            "Speed1 controls both motors speed. Speed2 becomes the turn value. 0 (full reverse), 128 (stop), 255 (full forward)",
                            "Speed1 control both motors speed, and Speed2 becomes the turn value. -128 (full reverse), 0 (stop), 127 (full forward)."};
   Log.verboseln("<setMode> Setting mode to %d. Now Speed registers mean: %s", mode, explanation[mode].c_str());
   Wire.beginTransmission(md25I2cAddress); // Request start transmit to the MD25 H-bridge.
   Wire.write(MD25RegMode); // Going to write to the mode register.
   Wire.write(mode); // Set mode.
   Wire.endTransmission(); // End transmit.
} // setMode

/** 
 * @brief Spin one or both motors at specified speed.
 * @details This is a blocking function which will halt execution of the main logic until the 
 * motor reaches it's end point. 
 * @param motorNumber 0=left, 1=right, 2=both.
 * @param speed How many encoder ticks to move.
 * @param distance how far the robot should move.
 * @note See full detais at https://www.pishrobot.com/files/products/datasheets/md25.pdf
=================================================================================================== */
void spinMotor(int8_t motorNumber, int8_t speed, long distance)
{
   resetEncoder(); // Set both encoders to 0.
   switch(motorNumber)
   {
      case 0: // Left motor 
         Log.verboseln("<spinMotor> Spin left motor");
         setMode(0); // Set MD25 controller to mode 0.
         Wire.beginTransmission(md25I2cAddress); // Request start transmit to the MD25 H-bridge
         Wire.write(MD25RegSpeed1); // Indicate motor1 speed register
         Wire.write(speed); // 1-127 = backwards, 128 = stop, 129-255 = forward                                           
         Wire.endTransmission(); // End transmit
         break;
      case 1: // Right motor 
         Log.verboseln("<spinMotor> Spin right motor");
         setMode(0); // Set MD25 controller to mode 0.
         Wire.beginTransmission(md25I2cAddress); // Request start transmit to the MD25 H-bridge
         Wire.write(MD25RegSpeed2); // Indicate motor1 speed register
         Wire.write(speed); // 1-127 = backwards, 128 = stop, 129-255 = forward                                           
         Wire.endTransmission(); // End transmit
         break;
      default: // Both motors
         Log.verboseln("<spinMotor> Spin both motors");
         setMode(2); // Set MD25 controller to mode 2.
         Wire.beginTransmission(md25I2cAddress); // Request start transmit to the MD25 H-bridge
         Wire.write(MD25RegSpeed1); // Indicate motor1 speed register
         Wire.write(speed); // 1-127 = backwards, 128 = stop, 129-255 = forward                                           
         Wire.endTransmission(); // End transmit
         break;
   } // switch
   Log.verboseln("<initMobility> Readings before: Left encoder = %l, right encoder = %l", getEncoder(LEFT_SIDE), getEncoder(RIGHT_SIDE));
   Log.verboseln("<initMobility> Wait for motor to  stop spinning.");
   while(getEncoder(LEFT_SIDE) < distance)
   {
      Log.verboseln("<spinMotor> ... Still spinning.");
      delay(10); // Give motor time to before taking next reading.
   } // while   
   // Stop both motors
   Wire.beginTransmission(md25I2cAddress); // Request start transmit to the MD25 H-bridge
   Wire.write(MD25RegSpeed1); // Indicate motor1 speed register
   Wire.write(128); // 1-127 = backwards, 128 = stop, 129-255 = forward                                           
   Wire.endTransmission(); // End transmit
   delay(100); // Give motor time to stop.
   Log.verboseln("<initMobility> Readings after: Left encoder = %l, right encoder = %l", getEncoder(LEFT_SIDE), getEncoder(RIGHT_SIDE));
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
   const int8_t BOTH_MOTORS = 2;
   int8_t speed = 180; // 0 - 127 backwards, 128 stop, 129 - 255 forward.
   long distance = 100; // Distance to travel in millimeters.
   spinMotor(BOTH_MOTORS, speed, distance); // Spin both motors.
   return mStatus;
} // initMobility. 

#endif // End of precompiler protected code block