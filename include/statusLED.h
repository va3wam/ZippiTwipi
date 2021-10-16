#ifndef statusLEDs_h // Start of precompiler check to avoid dupicate inclusion of this code block.

#define statusLEDs_h // Precompiler macro used for precompiler check.

#include <main.h> // Header file for all libraries needed by this program.

const int8_t numColoursSupported = 8; // Number of colours LED can be set to.
const int8_t RED = 0; // Reset button LED red. 
const int8_t GREEN = 1; // Reset button LED green. 
const int8_t BLUE = 2; // Reset button LED blue. 
const int8_t PINK = 3; // Reset button LED pink. 
const int8_t CYANNE = 4; // Reset button LED cyanne.
const int8_t AQUA = 5; // Reset button LED aqua. 
const int8_t WHITE = 6; // Reset button LED white. 
const int8_t BLACK = 7; // Reset button LED white. 

const double_t PWM_FREQ = 500; // 500Hz.
const int8_t PWM_RESOLUTION = 16; // ESP32 can go up to 16 bit PWM resolution.
const int8_t PWM_RED_CHANNEL = 0; // ESP32 has 16 channels which can generate 16 independent waveforms.
const int8_t PWM_GREEN_CHANNEL = 1; // ESP32 has 16 channels which can generate 16 independent waveforms.
const int8_t PWM_BLUE_CHANNEL = 2; // ESP32 has 16 channels which can generate 16 independent waveforms.

// Define structure for LCD colours 
typedef struct
{
   String name; // Name associated with colour properties
   int redDutyCycle; // Up time of the PWM signal. Ranges from 0-255. 127 is a 50% duty cycle for example.
   int greenDutyCycle; // Up time of the PWM signal. Ranges from 0-255. 127 is a 50% duty cycle for example.
   int blueDutyCycle; // Up time of the PWM signal. Ranges from 0-255. 127 is a 50% duty cycle for example.
}struct_Colour; 
struct_Colour statusColour[numColoursSupported]; // Array of colours

/**
 * @brief Set up a structure array of pre-defined colours for RGB LED.
 * @details The rest button has an embedded RGB LED that contains three
 * seperate red, green and blue LEDs. These three LEDs can be set to different
 * duty cycles in order to create a wide range of colours that the RGB LED can
 * display. This function creates an array of known colours in a structure 
 * that we can use to makes it easy to set the RGB LED to one of our
 * predefined colours.
 * ==========================================================================*/
void createPredefinedColours()
{
   Log.traceln("<createPredefinedColours> Creating array of colours.");
   statusColour[RED].name = "RED";
   statusColour[RED].redDutyCycle = 255;
   statusColour[RED].greenDutyCycle = 0;
   statusColour[RED].blueDutyCycle = 0;
   Log.verboseln("<createPredefinedColours> Red (%d) settings  - red = %d, green = %d, blue = %d.", RED, statusColour[RED].redDutyCycle, statusColour[RED].greenDutyCycle, statusColour[RED].blueDutyCycle);

   statusColour[GREEN].name = "GREEN";
   statusColour[GREEN].redDutyCycle = 0;
   statusColour[GREEN].greenDutyCycle = 255;
   statusColour[GREEN].blueDutyCycle = 0;
   Log.verboseln("<createPredefinedColours> Green (%d) settings  - red = %d, green = %d, blue = %d.", GREEN, statusColour[GREEN].redDutyCycle, statusColour[GREEN].greenDutyCycle, statusColour[GREEN].blueDutyCycle);

   statusColour[BLUE].name = "BLUE";
   statusColour[BLUE].redDutyCycle = 0;
   statusColour[BLUE].greenDutyCycle = 0;
   statusColour[BLUE].blueDutyCycle = 255;
   Log.verboseln("<createPredefinedColours> Blue (%d) settings  - red = %d, green = %d, blue = %d.", BLUE, statusColour[BLUE].redDutyCycle, statusColour[BLUE].greenDutyCycle, statusColour[BLUE].blueDutyCycle);

   statusColour[PINK].name = "PINK";
   statusColour[PINK].redDutyCycle = 128;
   statusColour[PINK].greenDutyCycle = 255;
   statusColour[PINK].blueDutyCycle = 0;
   Log.verboseln("<createPredefinedColours> Pink (%d) settings  - red = %d, green = %d, blue = %d.", PINK, statusColour[PINK].redDutyCycle, statusColour[PINK].greenDutyCycle, statusColour[PINK].blueDutyCycle);

   statusColour[CYANNE].name = "CYANNE";
   statusColour[CYANNE].redDutyCycle = 255;
   statusColour[CYANNE].greenDutyCycle = 128;
   statusColour[CYANNE].blueDutyCycle = 0;
   Log.verboseln("<createPredefinedColours> Cyanne (%d) settings  - red = %d, green = %d, blue = %d.", CYANNE, statusColour[CYANNE].redDutyCycle, statusColour[CYANNE].greenDutyCycle, statusColour[CYANNE].blueDutyCycle);

   statusColour[AQUA].name = "AQUA";
   statusColour[AQUA].redDutyCycle = 255;
   statusColour[AQUA].greenDutyCycle = 128;
   statusColour[AQUA].blueDutyCycle = 128;
   Log.verboseln("<createPredefinedColours> Aqua (%d) settings  - red = %d, green = %d, blue = %d.", AQUA, statusColour[AQUA].redDutyCycle, statusColour[AQUA].greenDutyCycle, statusColour[AQUA].blueDutyCycle);

   statusColour[WHITE].name = "WHITE";
   statusColour[WHITE].redDutyCycle = 255;
   statusColour[WHITE].greenDutyCycle = 255;
   statusColour[WHITE].blueDutyCycle = 255;
   Log.verboseln("<createPredefinedColours> White (%d) settings  - red = %d, green = %d, blue = %d.", WHITE, statusColour[WHITE].redDutyCycle, statusColour[WHITE].greenDutyCycle, statusColour[WHITE].blueDutyCycle);

   statusColour[BLACK].name = "BLACK";
   statusColour[BLACK].redDutyCycle = 0;
   statusColour[BLACK].greenDutyCycle = 0;
   statusColour[BLACK].blueDutyCycle = 0;
   Log.verboseln("<createPredefinedColours> Black (%d) settings  - red = %d, green = %d, blue = %d.", BLACK, statusColour[BLACK].redDutyCycle, statusColour[BLACK].greenDutyCycle, statusColour[BLACK].blueDutyCycle);
} // createPredefinedColours()

/**
 * @brief Allows you to create a custom colour for the reset button RGB LED.
 * @details Set the duty cycle for the three RGB channels which determines 
 * what colour the LED appears to be. 
 * ==========================================================================*/
void setResetButtonColor(int8_t red, int8_t green, int8_t blue)
{
   ledcWrite(PWM_RED_CHANNEL, red);
   ledcWrite(PWM_GREEN_CHANNEL, green);
   ledcWrite(PWM_BLUE_CHANNEL, blue);
} // setResetButtonColor()

/**
 * @brief Set status LED to one of the pre-defined colours we have set up.
 * ==========================================================================*/
void setStatusLedColour(int8_t ledColour)
{
   if(ledColour - 1 > numColoursSupported) // 8 predefined colours (7 and below valid).
   {
      ledColour = GREEN;
      Log.warningln("<setStatusLedColour> Requested colour %d unknown. Setting RGB LED to %s.", statusColour[ledColour].name.c_str());
   } // if
   else
   {
      Log.verboseln("<setStatusLedColour> Set status RGB LED to %s.", statusColour[ledColour].name.c_str());
   } // else
   ledcWrite(PWM_RED_CHANNEL, statusColour[ledColour].redDutyCycle);
   ledcWrite(PWM_GREEN_CHANNEL, statusColour[ledColour].greenDutyCycle);
   ledcWrite(PWM_BLUE_CHANNEL, statusColour[ledColour].blueDutyCycle);
} // setStatusLedColour()

/**
 * @brief Initialize the RGB LED embedded inside of the reset button.
 * @details Configure PWM frequency and resolution parameters for three
 * PWM channels then assign one channel to each of the primary colour LEDs 
 * embedded inside of the RGB LED ring located on the reset button.  
 * ==========================================================================*/
void setupStatusLed()
{
   const int8_t redLedSetting = 255; // set value from 0 - 255.
   const int8_t greenLedSetting = 0; // set value from 0 - 255.
   const int8_t blueLedSetting = 0; // set value from 0 - 255.
   createPredefinedColours(); // Create predefined colour settings.
   Log.traceln("<setupStatusLed> Initialize status RGB LED on reset button.");
   pinMode(resetRedLED, OUTPUT); // Set GPIO pin connected to red LED inside of the reset button RGB LED to output.
   pinMode(resetBlueLED, OUTPUT); // Set GPIO pin connected to green LED inside of the reset button RGB LED to output.
   pinMode(resetGreenLED, OUTPUT); // Set GPIO pin connected to blue LED inside of the reset button RGB LED to output.
   ledcSetup(PWM_RED_CHANNEL, PWM_FREQ, PWM_RESOLUTION); // Configure red LED PWM properties.
   ledcAttachPin(resetRedLED, PWM_RED_CHANNEL); // Attach PWM channel to pin connected to red LED on reset button.
   ledcSetup(PWM_GREEN_CHANNEL, PWM_FREQ, PWM_RESOLUTION); // Configure green LED PWM properties.
   ledcAttachPin(resetRedLED, PWM_GREEN_CHANNEL); // Attach PWM channel to pin connected to green LED on reset button.
   ledcSetup(PWM_BLUE_CHANNEL, PWM_FREQ, PWM_RESOLUTION); // Configure blue LED PWM properties.
   ledcAttachPin(resetBlueLED, PWM_BLUE_CHANNEL); // Attach PWM channel to pin connected to blue LED on reset button.
   Log.noticeln("<setupStatusLed> Set RGB LED colour to green.");
   setResetButtonColor(redLedSetting, greenLedSetting, blueLedSetting); // Set Reset button's RGB LED colour to Green.  
//   setStatusLedColour(GREEN);
} //setupStatusLed()

#endif // End of precompiler protected code block