/*************************************************************************************************************************************
 * @file main.cpp
 * @author va3wam
 * @brief Zippi Twipi robot firmware 
 * @details Basic operating code for the Zippi Twipi robot 
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
 * 2021-01-07 va3wam Reworked I2C and limit switch test code
 * 2020-12-12 va3wam Cleaned up loop() and messed around with reet button tri-coloured LED to test newly assembled circuit.
 * 2020-11-23 va3wam Added include of development board pin definition file
 * 2020-10-22 va3wam Program created
 *************************************************************************************************************************************/
#include <Arduino.h> // Arduino Core for ESP32. Comes with Platform.io
#include <Wire.h> // Required for serial I2C communication. Comes with Platform.io 
#include <zippiTwipi_gpio_pins.h> // Pin definitions for Adafruit Huzzah32 development board 
#include <amI2C.h> // Required for serial I2C communication. 

// Define MD25 registers 
#define CMD (byte) 0x00 // Command register. Cast as a byte to stop 
                        // them being misinterperted as NULL. This is a bug with arduino 1
#define SOFTWAREREG 0x0D // Byte to read the software version
#define SPEED1 (byte) 0x00 // Byte to send speed to first motor
#define SPEED2 0x01 // Byte to send speed to second motor
#define ENCODERONE 0x02 // Byte to read motor encoder 1
#define ENCODERTWO 0x06 // Byte to read motor encoder 2
#define VOLTREAD 0x0A // Byte to read battery volts
#define RESETENCODERS 0x20 // Byte to reset the motor encoder counters

// Define structure and variables for reset buttons built-in LEDs. 
// https://microcontrollerslab.com/esp32-pwm-arduino-ide-led-fading-example/ 
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

// Global control variables  
bool printHeaders = true;
long currentCnt1; 
long prevCnt1 = 0;
long errFilter = 100;
long successCount1 = 0;
long failCount1 = 0;
long totalCount1 = 0;
long percentage1 = 0;

// Define I2C bus
amI2C i2cBus; // Object to manage I2C buses

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
  if(frontSwitch == 0 && backSwitch == 0)
  {
    Serial.println("<limitSwitchMonitoring> Robot is resting on both front and back limit switches");
  }
  else
  {
    if(frontSwitch == 0 && backSwitch == 1)
    {
      Serial.println("<limitSwitchMonitoring> Robot is resting on front limit switch");
    } //if
    if(frontSwitch == 1 && backSwitch == 0)
    {
      Serial.println("<limitSwitchMonitoring> Robot is resting on back limit switch");
    } //if
  } //if
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
 * @brief Initialize the serial output with the specified baud rate measured in bits per second
=================================================================================================== */
void setupSerial(unsigned long baudRate)
{
  Serial.begin(115200); // Open a serial connection at specified baud rate 
  while (!Serial); // Wait for Serial port to be ready
} //setupSerial()

/** 
 * @brief Standard set up routine for Arduino programs 
=================================================================================================== */
void setup()
{
  // Set up serial communication
  unsigned long serialBaudRate = 115200; // Define serial baud rate 
  setupSerial(serialBaudRate); // Set serial baud rate  
  Serial.print("<setup> Start of setup. Serial baud rate is ");
  Serial.println(serialBaudRate);
  // Setup I2C buses
  i2cBus.configure(0, I2C_bus0_SDA, I2C_bus0_SCL, I2C_bus0_speed); // Set up I2C bus 0
  i2cBus.configure(1, I2C_bus1_SDA, I2C_bus1_SCL, I2C_bus1_speed); // Set up I2C bus 1

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
//    ledcWrite(redResetLED.pwmChannel, 0); // Set the duty cycle of PWM channel assigned to red LED
//    ledcWrite(redResetLED.pwmChannel, redResetLED.pwmDutyCycle); // Set the duty cycle of PWM channel assigned to green LED
    // Setup rest button's blue LED
    ledcSetup(blueResetLED.pwmChannel, blueResetLED.pwmFrequency, blueResetLED.pwmResolution); // Setup PWM channel for BLUE reset LED
    ledcAttachPin(blueResetLED.gpioPin, blueResetLED.pwmChannel); // Attach PWM channel to pin connected to reset button BLUE LED
    // Setup rest button's green LED
    ledcSetup(greenResetLED.pwmChannel, greenResetLED.pwmFrequency, greenResetLED.pwmResolution); // Setup PWM channel for GREEN reset LED
    ledcAttachPin(greenResetLED.gpioPin, greenResetLED.pwmChannel); // Attach PWM channel to pin connected to reset button GREEN LED
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
  ledcWrite(redResetLED.pwmChannel, 0); // Set the duty cycle of PWM channel assigned to red LED
  ledcWrite(blueResetLED.pwmChannel, 254); // Set the duty cycle of PWM channel assigned to blue LED
  ledcWrite(greenResetLED.pwmChannel, 254); // Set the duty cycle of PWM channel assigned to green LED
  delay(500);
  ledcWrite(redResetLED.pwmChannel, 254); // Set the duty cycle of PWM channel assigned to red LED
  ledcWrite(blueResetLED.pwmChannel, 0); // Set the duty cycle of PWM channel assigned to blue LED
  ledcWrite(greenResetLED.pwmChannel, 254); // Set the duty cycle of PWM channel assigned to green LED
  delay(500);
  ledcWrite(redResetLED.pwmChannel, 254); // Set the duty cycle of PWM channel assigned to red LED
  ledcWrite(blueResetLED.pwmChannel, 254); // Set the duty cycle of PWM channel assigned to blue LED
  ledcWrite(greenResetLED.pwmChannel, 0); // Set the duty cycle of PWM channel assigned to green LED
  delay(500);
} //loop()