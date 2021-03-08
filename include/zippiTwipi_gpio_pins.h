/*************************************************************************************************************************************
 * @file zippiTwipi_gpio_pins.h
 * @author va3wam
 * @brief Include file that defines the GPIO pins on a Adafruit Huzzah32 featherboard with an Espressif ESP-Wrover module 
 * @details Defines the GPIO numbers by the role they play on the ZippiTwipi robot
 * @version 0.0.1
 * @copyright Copyright (c) 2020
 * Version YYYY-MM-DD Description
 * ------- ---------- ----------------------------------------------------------------------------------------------------------------
 * 0.0.2   2020-12-10 Changed BUS0 pins. Note this pin configuration is now no longer compatable with the TWIPe robot but is now 
 *                    compatable with other Adafruit featherboards from an I2C stand point. 
 * 0.0.1   2020-11-23 File created. 
 *************************************************************************************************************************************
 Note that the physical pin count starts at the reset button on long pin side of the Huzzah32 development board
 *************************************************************************************************************************************/
#include <huzzah32_gpio_pins.h> // Mapping of ESP32 pins to Huzzah32 development board pins
#ifndef _ZIPPITWIPI_GPIO_PINS_H // Start of conditional preprocessor code that only allows this library to be included once
#define _ZIPPITWIPI_GPIO_PINS_H // Preprocessor variable used by above check
#define frontLimitSwitch pinLbl32 // Front limit switch, physical pin 32 
#define backLimitSwitch pinLbl14 // Back limit switch, physical pin 14 
#define I2C_bus0_SDA pinLblSDA // Serial Data Line (SDA) for I2C bus 0, physical pin 17
#define I2C_bus0_SCL pinLblSCL // Serial Clock Line (SCL) for I2C bus 0, physical pin 18
#define I2C_bus1_SDA pinLblTX // Serial Data Line (SDA) for I2C bus 1, physical pin 15
#define I2C_bus1_SCL pinLbl21 // Serial Clock Line (SCL) for I2C bus 1, physical pin 16
#define resetRedLED pinLbl27 // Red LED in reset button, physical pin 23
#define resetBlueLED pinLbl33 // Blue LED in reset button, physical pin 22
#define resetGreenLED pinLbl15 // Green LED in reset button, physical pin 21
#endif // End of conditional preprocessor code