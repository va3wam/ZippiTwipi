/*************************************************************************************************************************************
 * @file zippiTwipi_gpio_pins.h
 * @author va3wam
 * @brief Include file that defines the GPIO pins on a Adafruit Huzzah32 featherboard with an Espressif ESP-Wrover module 
 * @details Defines the GPIO numbers by the role they play on the ZippiTwipi robot
 * @version 0.0.1
 * @copyright Copyright (c) 2020
 * Version YYYY-MM-DD Description
 * ------- ---------- ----------------------------------------------------------------------------------------------------------------
 * 0.0.1   2020-11-23 File created. 
 *************************************************************************************************************************************/
#include <huzzah32_gpio_pins.h> // Mapping of ESP32 pins to Huzzah32 development board pins
#ifndef _ZIPPITWIPI_GPIO_PINS_H // Start of conditional preprocessor code that only allows this library to be included once
#define _ZIPPITWIPI_GPIO_PINS_H // Preprocessor variable used by above check
#define frontLimitSwitch pinLblA2 // Front limit switch 
#define backLimitSwitch pinLblA3 // Back limit switch 
#define leftEncoder pinLblA4 // Left motor shaft encoder
#define rightEncoder pinLblA5 // Right motor shaft encoder
#define I2C_bus0_SDA pinLblSCL // Serial Data Line (SDA) for I2C bus 0
#define I2C_bus0_SCL pinLbl14 // Serial Clock Line (SCL) for I2C bus 0  
#define I2C_bus1_SDA pinLblTX // Serial Data Line (SDA) for I2C bus 1
#define I2C_bus1_SCL pinLbl21 // Serial Clock Line (SCL) for I2C bus 1 
#define mainPowerLED pinLbl13 // LED in main power button
#define resetRedLED pinLbl12 // Red LED in reset button
#define resetBlueLED pinLbl27 // Blue LED in reset button
#define resetGreenLED pinLbl33 // Blue LED in reset button
#endif // End of conditional preprocessor code