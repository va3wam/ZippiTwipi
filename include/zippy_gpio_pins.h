/*************************************************************************************************************************************
 Note that the physical pin count starts at the reset button on long pin side of the Huzzah32 development board.
 *************************************************************************************************************************************/
#include <huzzah32_gpio_pins.h> // Mapping of ESP32 pins to Huzzah32 development board pins

#ifndef _ZIPPITWIPI_GPIO_PINS_H // Start of conditional preprocessor code that only allows this library to be included once

#define _ZIPPITWIPI_GPIO_PINS_H // Preprocessor variable used by above check

#define frontLimitSwitch PIN_LBL_32 // Front limit switch, physical pin 32 
#define backLimitSwitch PIN_LBL_14 // Back limit switch, physical pin 14 
#define I2C_BUS0_SDA PIN_LBL_SDA // Serial Data Line (SDA) for I2C bus 0, physical pin 17
#define I2C_BUS0_SCL PIN_LBL_SCL // Serial Clock Line (SCL) for I2C bus 0, physical pin 18
#define I2C_BUS1_SDA PIN_LBL_TX // Serial Data Line (SDA) for I2C bus 1, physical pin 15
#define I2C_BUS1_SCL PIN_LBL_21 // Serial Clock Line (SCL) for I2C bus 1, physical pin 16
#define resetRedLED PIN_LBL_27 // Red LED in reset button, physical pin 23
#define resetBlueLED PIN_LBL_33 // Blue LED in reset button, physical pin 22
#define resetGreenLED PIN_LBL_15 // Green LED in reset button, physical pin 21

#endif // End of conditional preprocessor code