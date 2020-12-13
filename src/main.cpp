/*************************************************************************************************************************************
 * @file main.cpp
 * @author va3wam
 * @brief Zippi Twipi robot firmware 
 * @details Basic operating code for the Zippi Twipi robot 
 * @copyright Copyright (c) 2020
 * YYYY-MM-DD Dev    Description
 * ---------- ------ -----------------------------------------------------------------------------------------------------------------
 * 2020-12-12 va3wam Cleaned up loop() and messed around with reet button tri-coloured LED to test newly assembled circuit.
 * 2020-11-23 va3wam Added include of development board pin definition file
 * 2020-10-22 va3wam Program created
 *************************************************************************************************************************************/
#include <Arduino.h> // Arduino Core for ESP32. Comes with Platform.io
#include <Wire.h> // Required for serial IIC communication. Comes with Platform.io 
#include <zippiTwipi_gpio_pins.h> // Pin definitions for Adafruit Huzzah32 development board 

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Define I2C bus0 - wire() - constants, classes and global variables 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define I2C_bus0_speed 400000 // Define speed of I2C bus 2. Note 400KHz is the upper speed limit for ESP32 I2C

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Define I2C bus1 - wire1() - constants, classes and global variables 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define I2C_bus1_speed 100000 // Define speed of I2C bus 2. Note 100KHz is the upper speed limit for MD25 H-Bridge

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
#define CMD (byte) 0x00 // Command register. Cast as a byte to stop 
                        // them being misinterperted as NULL. This is a bug with arduino 1
#define SOFTWAREREG 0x0D // Byte to read the software version
#define SPEED1 (byte) 0x00 // Byte to send speed to first motor
#define SPEED2 0x01 // Byte to send speed to second motor
#define ENCODERONE 0x02 // Byte to read motor encoder 1
#define ENCODERTWO 0x06 // Byte to read motor encoder 2
#define VOLTREAD 0x0A // Byte to read battery volts
#define RESETENCODERS 0x20

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Define structure and variables for reset buttons built-in LEDs. 
/// @ref https://microcontrollerslab.com/esp32-pwm-arduino-ide-led-fading-example/ 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct
{
  int pwmChannel; // There are 16 PWM channels available. Choose any channel between 0 and 15.
  int pwmFrequency; // Frequency of the digital signal
  int pwmResolution; // ESP32 boards support PWM resolution between 1 bit to 16 bits. The optimal resolution is 8 bit.
  int pwmDutyCycle; // Up time of the PWM signal. Ranges from 0-255. 127 is a 50% duty cycle for example.
  int gpioPin; // GPIO pin connected to the LED  
}resetButtonLED; 
resetButtonLED redResetLED = {2, 500, 8, 0, resetRedLED}; // Chan 2, freq 4000Hz, 8 bit res, 255 tick up (100% duty cycle), red LED pin 
resetButtonLED blueResetLED = {1, 500, 8, 0, resetBlueLED}; // Chan 1, freq 4000Hz, 8 bit res, 255 tick up (100% duty cycle), blue LED pin
resetButtonLED greenResetLED = {0, 500, 8, 0, resetGreenLED}; // Chan 0, freq 4000Hz, 8 bit res, 255 tick up (100% duty cycle), green LED pin
int fadeAmount = 5; // How much to fade the LEDs by each loop
int brightness = 0; // How bright the LED is

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Global control variables  
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool printHeaders = true;

long currentCnt1; 
long prevCnt1 = 0;
long errFilter = 100;
long successCount1 = 0;
long failCount1 = 0;
long totalCount1 = 0;
long percentage1 = 0;

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
 * @brief This function resets the encoder values to 0
=================================================================================================== */
void encodeReset()
{                                       
  Wire.beginTransmission(MD25ADDRESS);
  Wire.write(CMD);
  Wire.write(0x20); // Putting the value 0x20 to reset encoders
  Wire.endTransmission(); 
} //encodeReset()

/** 
 * @brief Function to reads the value of encoder 1 as a long
=================================================================================================== */
long encoder1()
{                                            
  Wire.beginTransmission(MD25ADDRESS); // Send byte to get a reading from encoder 1
  Wire.write(ENCODERONE);
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
long encoder2()
{                                            
  Wire.beginTransmission(MD25ADDRESS);           
  Wire.write(ENCODERTWO);
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
void stopMotor()
{                                       
  Wire.beginTransmission(MD25ADDRESS);
  Wire.write(SPEED2);
  Wire.write(128); // Sends a value of 128 to motor 2 this value stops the motor
  Wire.endTransmission();
  
  Wire.beginTransmission(MD25ADDRESS);
  Wire.write(SPEED1);
  Wire.write(128); // Sends a value of 128 to motor 2 this value stops the motor
  Wire.endTransmission();
}  //stopMotor()

/**
 * @brief Run motor test code
=================================================================================================== */
void motorTest()
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

/**
 * @brief Check the limit switches moounted at the front and back of the robot to see if it has
 * lost its balance
=================================================================================================== */
void limitSwitchMonitoring()
{
  int frontSwitch = digitalRead(frontLimitSwitch);
  int backSwitch = digitalRead(backLimitSwitch);
  Serial.print(frontSwitch);
  Serial.print(",");
  Serial.println(backSwitch);
  delay(100);
} //limitSwitchMonitoring()

/**
 * @brief Set the colour of the reset button's LED
=================================================================================================== */
void setResetButtonLEDColour(int redDutyCycle, int blueDutyCycle, int greenDutyCycle)
{
  ledcWrite(redResetLED.pwmChannel, redDutyCycle); // Set the duty cycle of red LED PWM channel
  ledcWrite(blueResetLED.pwmChannel, blueDutyCycle); // Set the duty cycle of blue LED PWM channel
  ledcWrite(greenResetLED.pwmChannel, greenDutyCycle); // Set the duty cycle of green LED PWM channel
} //setResetButtonLEDColour()

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
    encodeReset();
    Wire.beginTransmission(MD25ADDRESS); // Request start transmit to the MD25 H-bridge
    Wire.write(SPEED1); // Indicate motor1 speed register
    Wire.write(20); // 1-127 = backwards, 128 = stop, 129-255 = forward                                           
    Wire.endTransmission(); // End transmit
    pinMode(17, INPUT_PULLDOWN);

    // Playing with digital outputs for RGB LED on reset button. Note inverted logic, 1 = off, 0 = on
//    pinMode(redResetLED.gpioPin, OUTPUT); // Set pin connected to reset buttons red LED to output mode
//    pinMode(blueResetLED.gpioPin, OUTPUT); // Set pin connected to reset buttons blue LED to output mode
//    pinMode(greenResetLED.gpioPin, OUTPUT); // Set pin connected to reset buttons green LED to output mode 
//    digitalWrite(redResetLED.gpioPin,1);
//    digitalWrite(blueResetLED.gpioPin,1);
//    digitalWrite(greenResetLED.gpioPin,0);

    // Playing with analog outputs for RGB LED on reset button. Note inverted dutycycle logic, 0 = full on, 255 = full off         
    // Setup rest button's red LED. For a good article about PWM see https://makeabilitylab.github.io/physcomp/esp32/led-fade.html
    ledcSetup(redResetLED.pwmChannel, redResetLED.pwmFrequency, redResetLED.pwmResolution); // Setup PWM channel for RED reset LED
    ledcAttachPin(redResetLED.gpioPin, redResetLED.pwmChannel); // Attach PWM channel to pin connected to reset button RED LED
    ledcWrite(redResetLED.pwmChannel, 0); // Set the duty cycle of PWM channel assigned to red LED
    delay(1000);
    ledcDetachPin(redResetLED.gpioPin);
//    ledcWrite(redResetLED.pwmChannel, redResetLED.pwmDutyCycle); // Set the duty cycle of PWM channel assigned to green LED

    // Setup rest button's blue LED
//    ledcSetup(blueResetLED.pwmChannel, blueResetLED.pwmFrequency, blueResetLED.pwmResolution); // Setup PWM channel for BLUE reset LED
//    ledcAttachPin(blueResetLED.gpioPin, blueResetLED.pwmChannel); // Attach PWM channel to pin connected to reset button BLUE LED
    // Setup rest button's green LED
//    ledcSetup(greenResetLED.pwmChannel, greenResetLED.pwmFrequency, greenResetLED.pwmResolution); // Setup PWM channel for GREEN reset LED
//    ledcAttachPin(greenResetLED.gpioPin, greenResetLED.pwmChannel); // Attach PWM channel to pin connected to reset button GREEN LED
    // Setup balance limit switches
    pinMode(frontLimitSwitch,INPUT_PULLUP); // Set pin with front limit switch connected to it as input with an internal pullup resistor
    pinMode(backLimitSwitch,INPUT_PULLUP); // Set pin with back limit switch connected to it as input with an internal pullup resistor
    Serial.println("<setup> End of setup");
} //setup()

/**
 * @brief Standard looping routine for Arduino programs
=================================================================================================== */
void loop()
{
//  motorTest();
  limitSwitchMonitoring();
} //loop()