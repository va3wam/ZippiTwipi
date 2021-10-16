#ifndef configDetails_h // Start of precompiler check to avoid dupicate inclusion of this code block.

#define configDetails_h // Precompiler macro used for precompiler check.

#include <main.h> // Header file for all libraries needed by this program.
#include <mqttBroker.h> // MQTT broker functions.
const char* HOST_NAME_PREFIX = "Zippy"; // Prefix for our unique network name.
aaChip appCpu; // Access information about the ESP32 application microprocessor (Core1).
aaNetwork network(HOST_NAME_PREFIX); // WiFi session management.
bool networkConnected = false; // Track WiFi connectivity status.
bool mqttBrokerConnected = false; // Track MQTT broker connection status.
bool ledConnected = false; // Track LED I2C connection status.
bool motorControllerConnected = false; // Track motor controller I2C connectionstatus.

/** 
 * @brief Show the environment details of this application on console.
 * =================================================================================*/
void showCfgDetails()
{
   Log.verboseln("<showCfgDetails> Robot Configuration Report");
   Log.verboseln("<showCfgDetails> ==========================");
   appCpu.cfgToConsole(); // Display core0 information on the console.
   if(networkConnected == true)
   {
      Log.verboseln("<showCfgDetails> Network connection status = TRUE");
      network.cfgToConsole(); // Display network information on the console.
      if(mqttBrokerConnected == true)
      {
         Log.verboseln("<showCfgDetails> MQTT broker connection status = TRUE");
         Log.verboseln("<showCfgDetails> MQTT broker IP address = %p", getMqttBrokerIP());
      } // if
      else
      {
         Log.verboseln("<showCfgDetails> MQTT broker connection status = FALSE");
      } // else
   } // if
   else
   {
      Log.verboseln("<showCfgDetails> Network connection status = FALSE");
   } // else
   if(ledConnected == true)
   {
      Log.verboseln("<showCfgDetails> LED connection status = TRUE.");
   } // if
   else
   {
      Log.verboseln("<showCfgDetails> LED connection status = FALSE.");
   } // else
   if(motorControllerConnected == true)
   {
      Log.verboseln("<showCfgDetails> DC motor controller connection status = TRUE.");
   } // if
   else
   {
      Log.verboseln("<showCfgDetails> DC motor controller connection status = FALSE.");
   } // else
} //showCfgDetails()

#endif // End of precompiler protected code block