/*************************************************************************************************************************************
 * @file amFormat.cpp
 * @author theAgingApprentice
 * @brief Child class that extends the parent class LiquidCrystal_I2C and adds some handy functions to control I2C LCD device.  
 * @details Adds functions to esiy display things on the LCD.
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
#include <Arduino.h> // Arduino Core for ESP32. Comes with Platform.io
#include <amLCD.h> // Header file for linking.

/**
 * @brief This is the constructor for this class.
 * @details This class extends the LiquidCrystal_I2C class
 * @param uint8_t lcd_Addr is the I2C address of the LCD device
 * @param uint8_t uint8_t lcd_cols defines the number of columns that the LCD has
 * @param uint8_t lcd_rows defines the number of rows that the LCD has
===================================================================================================*/
amLCD::amLCD(uint8_t lcd_Addr,uint8_t lcd_cols,uint8_t lcd_rows) // Constructor for this class
   :LiquidCrystal_I2C(lcd_Addr,lcd_cols,lcd_rows) // Constructor for parent class LiquidCrystal_I2C
{
   this->init(); // Call function defined in parent class LiquidCrystal_I2C. Initialize LCD. 
   this->clear(); // Call function defined in parent class LiquidCrystal_I2C. Clear LCD.
   this->backlight(); // Call function defined in parent class LiquidCrystal_I2C. Turn on backlight.
} //amLCD::amLCD()

/**
 * @brief Displays a message centred on the specified row.
 * @param String msg specifies the message that you want to display on the LCD.
 * @param int row spacifies which row of the LCD that you want the message to appear on.
===================================================================================================*/
void amLCD::centre(String msg, int row)
{
   String message; // Message to display
   // Make sure we do not try to print on a row bigger than the max row of the LCD
   if(row > lcdNumRows - 1) row = lcdNumRows; // Row count starts at 0 so number of rows = physical size - 1
   // Make sure the message is not longer than the width of the LCD
   int len = msg.length();
   if(len > lcdNumCols) // If message is longer than width of LCD
   {
      message = msg.substring(0,lcdNumRows); // Truncate message at max width of LCD
   } //if
   else // Message less than length of LCD
   {
      message = msg; // Keep full message
   } //else
   int startCol = (lcdNumCols - len) / 2; // Figure out start column for mesage to be centred 
   this->setCursor(0,row); // Call function defined in parent class LiquidCrystal_I2C. Go to start of current row.
   this->print("                "); // Call function defined in parent class LiquidCrystal_I2C. Print spaces to clear current row.
   this->setCursor(startCol,row); // Call function defined in parent class LiquidCrystal_I2C. Go to column that will cause the message to be centred.
   this->print(message); // Call function defined in parent class LiquidCrystal_I2C. Print the message.
} //amLCD::centre()