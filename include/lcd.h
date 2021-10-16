#ifndef lcd_h // Start of precompiler check to avoid dupicate inclusion of this code block.

#define lcd_h // Precompiler macro used for precompiler check.

#include <main.h>
const uint8_t lcdColumns = 16; // Number of characters that fint on one row of LCD.
const uint8_t lcdRows = 2; // Number of rows on the LCD unit used for this robot. 
LiquidCrystal_I2C lcd(LCD16x2, lcdColumns, lcdRows); // Define LCD object.

/**
 * @brief Places a text message centrered horizontally.
 * 
 * @param msg Text message to be displayed.
 * @param row Row to display the message on.
 * ==========================================================================*/
void placeTextHcentre(String msg, int8_t row) 
{
   int8_t column; // Column to start message in so that it is centreed on LCD.
   
   if(row == 0 || row == 1)
   {
      Log.verboseln("<placeTextHcentre> Row specified is valid.");
   } // if
   else
   {
      Log.verboseln("<placeTextHcentre> Row specified is not valid. Will write on row 0.");
      row = 0;
   } // else
   if(row >= lcdColumns)
   {
      Log.verboseln("<placeTextHcentre> Message too long to center. Starting in column 0.");
      column = 0;
   }  // if
   else
   {
      column = (lcdColumns - msg.length()) / 2; 
      Log.verboseln("<placeTextHcentre> To centre message <%s> start in column %d.", msg.c_str(), column);
   }  // else
   lcd.setCursor(column, row);
   lcd.print(msg);
} // placeTextHcentre()

/**
 * @brief Display the splash screen.
 * ==========================================================================*/
void displaySplashScreen() 
{
   int8_t row0 = 0; // First row of LCD.
   int8_t row1 = 1; // Second row of LCD.
   String robotIP = "R:" + WiFi.localIP().toString(); 
   String mqttBrokerIP = "B:" + getMqttBrokerIP().toString();
   placeTextHcentre(robotIP, row0);
   placeTextHcentre(mqttBrokerIP, row1);
} // displaySplashScreen()

/**
 * @brief Initiaize LED display.
 * ==========================================================================*/
void initLed() 
{
   Log.verboseln("<initLed> 2x16 LED test");
   lcd.init(I2C_BUS0_SDA, I2C_BUS0_SCL); // initialize the lcd 
   lcd.setBacklight(true);
   displaySplashScreen();
} // initLed()

#endif // End of precompiler protected code block