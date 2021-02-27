/*************************************************************************************************************************************
 * @file amFormat.h
 * @author theAgingApprentice
 * @brief Child class that extends the parent class LiquidCrystal_I2C and adds some handy functions. 
 * @details Converts datatypes in ways that makes it possible to use the output of one library as input for another.
 * @copyright Copyright (c) 2021 the Aging Apprentice
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
 * YYYY-MM-DD Dev        Description
 * ---------- ---------- -------------------------------------------------------------------------------------------------------------
 * 2021-02-25 Old Squire Program created
 *************************************************************************************************************************************/
#ifndef amLCD_h
#define amLCD_h
#include <Arduino.h> // Arduino Core for ESP32. Comes with Platform.io
#include <LiquidCrystal_I2C.h> // Required for LCD support

#define lcdNumCols 16 // Number of columns in model of LCD being used
#define lcdNumRows 2 // Number of rows in model of LCD being used
#define lcdI2cAddress 0x3F; // I2C address of the LCD

// Define amLCD class
class amLCD : public LiquidCrystal_I2C // Indicate that we are extending LiquidCrystal_I2C class with our class 
{
   public:
      amLCD(uint8_t lcd_Addr,uint8_t lcd_cols,uint8_t lcd_rows); // Constructor
      void centre(String msg, int row); // Centers the message on the row specified (0 or 1) 
   private:
}; //class amLCD

#endif