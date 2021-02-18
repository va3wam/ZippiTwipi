/***************************************************************************************************
 * @file main.cpp
 * @author va3wam
 * @brief Zippi Twipi robot firmware 
 * @details Basic operating code for the Zippi Twipi robot 
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
 * 2021-01-07 va3wam Reworked I2C and limit switch test code
 * 2020-12-12 va3wam Cleaned up loop() and messed around with reet button tri-coloured LED to test 
 *                   newly assembled circuit.
 * 2020-11-23 va3wam Added include of development board pin definition file
 * 2020-10-22 va3wam Program created
 ***************************************************************************************************/
#include <Arduino.h> // Arduino Core for ESP32. Comes with Platform.io
#include <Wire.h> // Required for serial I2C communication. Comes with Platform.io 
#include <huzzah32_gpio_pins.h> // Pin definitions for Adafruit Huzzah32 development board 
#include <zippiTwipi_gpio_pins.h> // Pin definitions for ZippiTiwppi robot
#include <amI2C.h> // Required for serial I2C communication 
#include <amMD25.h> // Required for motor control
//#include <amRGB.h> // Required to control tri-colours LED on reset button
#include <esp32-hal-cpu.h> // Required for getCpuFrequencyMhz()
#include <soc/rtc.h> // Required for rtc_clk_apb_freq_get()

#define ledTimer 0 // Timer 0 will be used to control LED on reset button
#define ledFreq 1000000 // Micro seconds per timer0 (led timer) interrupt occurence (1 per second)

// Define structure and variables for reset buttons built-in LEDs. 
// https://microcontrollerslab.com/esp32-pwm-arduino-ide-led-fading-example/ 
typedef struct
{
   int pwmChannel; // There are 16 PWM channels available. Choose any channel between 0 and 15.
   int pwmFrequency; // Frequency of the digital signal
   int pwmResolution; // ESP32 PWM resolution between 1 bit to 16 bits. The optimal resolution is 8 bit.
   int pwmDutyCycle; // Up time of the PWM signal. Ranges from 0-255. 127 is a 50% duty cycle for example.
   int gpioPin; // GPIO pin connected to the LED  
}resetButtonLED; 
resetButtonLED redResetLED;
resetButtonLED blueResetLED;
resetButtonLED greenResetLED;

int fadeAmount = 5; // How much to fade the LEDs by each loop
int brightness = 0; // How bright the LED is

// Instantiate library objects
amI2C i2cBus; // Object to manage I2C buses
amMD25 motorControl; // Object to manage motors via h-bridge controller
//amRGB resetButton; // Object to manage tri-colour LED on reset button

// Define variables to store environment information 
uint32_t arduinoCore; // The ESP32 comes with 2 Xtensa 32-bit LX6 microprocessors: core 0 and core 1. Arduino IDE runs on core 1.
uint32_t cpuClockSpeed; // Clock speed of the core that Arduino is running on (MHz)
uint32_t timerClockSpeed; // Clock speed of the hardware timers (MHz)

// Define structure and variables for hardware interrupt timer 
volatile int interrupt0Counter; // Track how many times the interrupt has fired and not been processed
int totalInterrupt0Counter; // Track how many times the interrupt has fired in total
int currColourCnt; // Track what colour is currently active for the reset button LED
hw_timer_t * timer0 = NULL; // Pointer to hardware timer0
portMUX_TYPE timer0Mux = portMUX_INITIALIZER_UNLOCKED; // Used to prevent race conditins updating counters

/**
 * @brief Hardware timer 0 interrupt handler function
 * @details Used to control flashing of LEDs on reset button
=================================================================================================== */
void IRAM_ATTR onTimer0() 
{
  portENTER_CRITICAL_ISR(&timer0Mux); // Use mux variable to prevent race condiiton with loop()
  interrupt0Counter++; 
  portEXIT_CRITICAL_ISR(&timer0Mux);
} //onTimer0()

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
      motorControl.stopMotor(2); // motor (0=left, 1=right, 2=both) 
      Serial.print("<setup> Encode 1 = ");
      Serial.println(motorControl.getEncoder1());
      Serial.print("<setup> Encode 2 = ");
      Serial.println(motorControl.getEncoder2());
   }
   else
   {
      if(frontSwitch == 0 && backSwitch == 1)
      {
         Serial.println("<limitSwitchMonitoring> Robot is resting on front limit switch");
         motorControl.spinMotor(2,255); // motor (0=left, 1=right, 2=both), speed 
      } //if
      if(frontSwitch == 1 && backSwitch == 0)
      {
         Serial.println("<limitSwitchMonitoring> Robot is resting on back limit switch");
         motorControl.spinMotor(2,0); // motor (0=left, 1=right, 2=both), speed 
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
 * @brief Configure pins that control  RGB LEDs on the reset button.
===================================================================================================*/
void configRGB() 
{
   redResetLED = {2, 500, 8, 0, resetRedLED}; // Chan 2, freq 4000Hz, 8 bit res, 255 tick up (100% duty cycle), red LED pin 
   blueResetLED = {1, 500, 8, 0, resetBlueLED}; // Chan 1, freq 4000Hz, 8 bit res, 255 tick up (100% duty cycle), blue LED pin
   greenResetLED = {0, 500, 8, 0, resetGreenLED}; // Chan 0, freq 4000Hz, 8 bit res, 255 tick up (100% duty cycle), green LED pin
   // Setup rest button's red LED. For a good article about PWM see https://makeabilitylab.github.io/physcomp/esp32/led-fade.html
   ledcSetup(redResetLED.pwmChannel, redResetLED.pwmFrequency, redResetLED.pwmResolution); // Setup PWM channel for RED reset LED
   ledcAttachPin(redResetLED.gpioPin, redResetLED.pwmChannel); // Attach PWM channel to pin connected to reset button RED LED
   // Setup rest button's blue LED
   ledcSetup(blueResetLED.pwmChannel, blueResetLED.pwmFrequency, blueResetLED.pwmResolution); // Setup PWM channel for BLUE reset LED
   ledcAttachPin(blueResetLED.gpioPin, blueResetLED.pwmChannel); // Attach PWM channel to pin connected to reset button BLUE LED
   // Setup rest button's green LED
   ledcSetup(greenResetLED.pwmChannel, greenResetLED.pwmFrequency, greenResetLED.pwmResolution); // Setup PWM channel for GREEN reset LED
   ledcAttachPin(greenResetLED.gpioPin, greenResetLED.pwmChannel); // Attach PWM channel to pin connected to reset button GREEN LED
   // Setup balance limit switches
   pinMode(frontLimitSwitch,INPUT_PULLUP); // Set pin with front limit switch connected to it as input with an internal pullup resistor
   pinMode(backLimitSwitch,INPUT_PULLUP); // Set pin with back limit switch connected to it as input with an internal pullup resistor        
} //configRGB()

/**
 * @brief Initialize the serial output with the specified baud rate measured in bits per second
=================================================================================================== */
void setupSerial(unsigned long baudRate)
{
   Serial.begin(115200); // Open a serial connection at specified baud rate 
   while (!Serial); // Wait for Serial port to be ready
} //setupSerial()

/**
 * @brief Cycle through LED colours 
 * @details Each call causes the colour to switch to the next colour in the cycle
=================================================================================================== */
void cycleLed()
{
   if(interrupt0Counter > 0) 
   {
      portENTER_CRITICAL(&timer0Mux);
      interrupt0Counter--;
      portEXIT_CRITICAL(&timer0Mux);
      totalInterrupt0Counter++;
      switch(currColourCnt)
      {
         case 0: // RED
            currColourCnt ++;
            ledcWrite(redResetLED.pwmChannel, 0); // Set the duty cycle of PWM channel assigned to red LED
            ledcWrite(blueResetLED.pwmChannel, 255); // Set the duty cycle of PWM channel assigned to blue LED
            ledcWrite(greenResetLED.pwmChannel, 255); // Set the duty cycle of PWM channel assigned to green LED
            break;
         case 1: // BLUE
            currColourCnt ++;
            ledcWrite(redResetLED.pwmChannel, 255); // Set the duty cycle of PWM channel assigned to red LED
            ledcWrite(blueResetLED.pwmChannel, 0); // Set the duty cycle of PWM channel assigned to blue LED
            ledcWrite(greenResetLED.pwmChannel, 255); // Set the duty cycle of PWM channel assigned to green LED
            break;
         case 2: // GREEN
            currColourCnt ++;
            ledcWrite(redResetLED.pwmChannel, 255); // Set the duty cycle of PWM channel assigned to red LED
            ledcWrite(blueResetLED.pwmChannel, 255); // Set the duty cycle of PWM channel assigned to blue LED
            ledcWrite(greenResetLED.pwmChannel, 0); // Set the duty cycle of PWM channel assigned to green LED
            break;
         case 3: // PINK
            currColourCnt ++;
            ledcWrite(redResetLED.pwmChannel, 128); // Set the duty cycle of PWM channel assigned to red LED
            ledcWrite(blueResetLED.pwmChannel, 0); // Set the duty cycle of PWM channel assigned to blue LED
            ledcWrite(greenResetLED.pwmChannel, 255); // Set the duty cycle of PWM channel assigned to green LED
            break;
         case 4: // CYANNE
            currColourCnt ++;
            ledcWrite(redResetLED.pwmChannel, 255); // Set the duty cycle of PWM channel assigned to red LED
            ledcWrite(blueResetLED.pwmChannel, 0); // Set the duty cycle of PWM channel assigned to blue LED
            ledcWrite(greenResetLED.pwmChannel, 128); // Set the duty cycle of PWM channel assigned to green LED
            break;
         default: // WHITE
            currColourCnt = 0;
            ledcWrite(redResetLED.pwmChannel, 255); // Set the duty cycle of PWM channel assigned to red LED
            ledcWrite(blueResetLED.pwmChannel, 128); // Set the duty cycle of PWM channel assigned to blue LED
            ledcWrite(greenResetLED.pwmChannel, 128); // Set the duty cycle of PWM channel assigned to green LED
            break;
      } //switch
      Serial.print("An interrupt has occurred. Total number: ");
      Serial.println(totalInterrupt0Counter);
   } //if   
} //amRGB::cycleLed()

/** 
 * @brief Standard set up routine for Arduino programs 
=================================================================================================== */
void setup()
{
   // Set up serial communication
   unsigned long serialBaudRate = 115200; // Define serial baud rate 
   setupSerial(serialBaudRate); // Set serial baud rate  
   Serial.println("<setup> Start of setup");

   // Capture environment information  
   arduinoCore = xPortGetCoreID(); // Core is Arduino runing on
   cpuClockSpeed = getCpuFrequencyMhz(); // Clock frequency of core that Arduino is running on (MHz) 
   timerClockSpeed = rtc_clk_apb_freq_get() / 1000000; // Get clock speed of timers(Hz) and convert to MHz
   Serial.print("<setup> ... Serial baud rate = ");
   Serial.println(serialBaudRate);
   Serial.print("<setup> ... Arduino core = ");
   Serial.println(arduinoCore);
   Serial.print("<setup> ... CPU clock frequency = ");
   Serial.print(cpuClockSpeed);
   Serial.println("MHz");

   // Setup I2C buses
   i2cBus.configure(i2cBusNumber0, I2C_bus0_SDA, I2C_bus0_SCL, I2C_bus0_speed); // Set up I2C bus 0 
   i2cBus.configure(i2cBusNumber1, I2C_bus1_SDA, I2C_bus1_SCL, I2C_bus1_speed); // Set up I2C bus 1

   // Setup motor controller
   byte softVer = motorControl.getFirmwareVersion(); // Gets the software version of MD25
   Serial.print("<setup> MD25 motor controller firmware version = ");
   Serial.println(softVer, HEX);
   motorControl.encoderReset();

   // Setup timer0
   Serial.print("<setup> Configure timer");
   Serial.println(ledTimer); 
   Serial.print("<setup> Clock frequency = ");
   Serial.print(timerClockSpeed); 
   Serial.println("MHz");
   Serial.print("<setup> Interrupt frequency = ");
   Serial.print(ledFreq); 
   Serial.println("MHz");
   Serial.print("<setup> Reload = true");
   timer0 = timerBegin(ledTimer, timerClockSpeed, true); // Set timer 0 to a 1 MHz frequency 
   timerAttachInterrupt(timer0, &onTimer0, true); // Call onTimer0() each time ths interrupt fires
   timerAlarmWrite(timer0, ledFreq, true); // Set frequency interrupt fires and reset counter to repeat
   timerAlarmEnable(timer0); // Enable interrupt

   // Setup rest button's red LED. For a good article about PWM see https://makeabilitylab.github.io/physcomp/esp32/led-fade.html
   configRGB();
   Serial.println("<setup> End of setup");

} //setup()

/**
 * @brief Standard looping routine for Arduino programs
=================================================================================================== */
void loop()
{
//  motorTest();
   limitSwitchMonitoring();
   cycleLed();
} //loop()