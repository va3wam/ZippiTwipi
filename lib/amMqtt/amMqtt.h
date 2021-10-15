#ifndef amMqtt_h // Preprocessor directive aimed at ensuring that this file is only loaded once.
#define amMqtt_h // Preprocessor marco aimed at ensuring that this file is only loaded once.

/*
This example uses FreeRTOS softwaretimers as there is no built-in Ticker library
Need to move driving logic to amNetwork so that we have access to IP address etc.
*/

#include <WiFi.h> // Required to connect to WiFi network. Comes with Platform.io
#include "freertos/FreeRTOS.h" // Required for FreeRTOS primatives
#include "freertos/timers.h" // Required for FreeRTOS software timers
#include <AsyncMqttClient.h> // Required for  MQTT pub/sub. https://github.com/marvinroger/async-mqtt-client

#define MQTT_HOST IPAddress(192, 168, 2, 21)
#define MQTT_PORT 1883

AsyncMqttClient mqttClient;
TimerHandle_t mqttReconnectTimer;
TimerHandle_t wifiReconnectTimer;
IPAddress mqttBrokerIp(192, 168, 2, 21);

/**
 * @brief Connect to the Mqtt broker.
===================================================================================================*/
void connectToMqtt() 
{
  Serial.println("<amMqtt::connectToMqtt> Connecting to MQTT...");
  Serial.print("<amMqtt::connectToMqtt> Broker IP is ");
  Serial.println(MQTT_HOST);
  Serial.print("<amMqtt::connectToMqtt> Broker PORT ");
  Serial.println(MQTT_PORT);
  mqttClient.connect();
} //connectToMqtt()

/**
 * @brief Event handler for new connections from robot to Mqtt broker.
 * @param bool sessionPresent session ID.
===================================================================================================*/
void onMqttConnect(bool sessionPresent) 
{
  Serial.println("<amMqtt::onMqttConnect> Connected to MQTT.");
  Serial.print("...<amMqtt::onMqttConnect> Session present: ");
  Serial.println(sessionPresent);

  uint16_t packetIdSub = mqttClient.subscribe("test/lol", 1); // Subscribe to topic tree
  Serial.print("...<amMqtt::onMqttConnect> Subscribing at QoS 1, packetId: ");
  Serial.println(packetIdSub);
//  mqttClient.publish("test/lol", 0, true, "test 1");
//  Serial.println("...<amMqtt::onMqttConnect> Publishing at QoS 0");
  uint16_t packetIdPub1 = mqttClient.publish("test/lol", 1, true, "test 2");
  Serial.print("...<amMqtt::onMqttConnect> Publishing at QoS 1, packetId: ");
//  Serial.println(packetIdPub1);
//  uint16_t packetIdPub2 = mqttClient.publish("test/lol", 2, true, "test 3");
//  Serial.print("...<amMqtt::onMqttConnect> Publishing at QoS 2, packetId: ");
//  Serial.println(packetIdPub2);
} //onMqttConnect()

/**
 * @brief Event handler for disconnection from Mqtt broker.
 * @details Will attempt to re-establish connection to the Mqtt broker so long as there is a WiFi
 * connection available.
 * @param AsyncMqttClientDisconnectReason reason provides reason code for disconnection from broker.
===================================================================================================*/
void onMqttDisconnect(AsyncMqttClientDisconnectReason reason) 
{
  Serial.println("<amMqtt::onMqttDisconnect> Disconnected from MQTT.");

  if (WiFi.isConnected()) 
  {
    xTimerStart(mqttReconnectTimer, 0);
  } //if
} //onMqttDisconnect()

/**
 * @brief Event handler for subscribing ACK from the Mqtt broker.
 * @param uint16_t packetId. Unique ID for packet.
 * @param uint8_t qos. Quality of service setting for the packet.
===================================================================================================*/
void onMqttSubscribe(uint16_t packetId, uint8_t qos) 
{
//  Serial.println("<amMqtt::onMqttSubscribe> Subscribe acknowledged.");
//  Serial.print("...<amMqtt::onMqttSubscribe> packetId: ");
//  Serial.println(packetId);
//  Serial.print("...<amMqtt::onMqttSubscribe> qos: ");
//  Serial.println(qos);
} //onMqttSubscribe()

/**
 * @brief Event handler for unsubscribing ACK from the Mqtt broker.
 * @param uint16_t packetId. Unique ID for packet.
===================================================================================================*/
void onMqttUnsubscribe(uint16_t packetId) 
{
//  Serial.println("<amMqtt::onMqttUnsubscribe> Unsubscribe acknowledged.");
//  Serial.print("...<amMqtt::onMqttUnsubscribe> packetId: ");
//  Serial.println(packetId);
} //onMqttUnsubscribe()

/**
 * @brief Event handler for incoming messages from the Mqtt broker.
 * @param char* topic. Topic tree name. 
 * @param char* payload. Content of message. 
 * @param AsyncMqttClientMessageProperties properties. QOS of message. 
 * @param size_t len. Length of message payload?  
 * @param size_t index. No idea. 
 * @param size_t total. Total length of message including headers etc?
===================================================================================================*/
void onMqttMessage(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total) 
{
  Serial.println("<amMqtt::onMqttMessage> Publish received.");
  Serial.print("...<amMqtt::onMqttMessage> topic: ");
  Serial.println(topic);
  Serial.print("...<amMqtt::onMqttMessage> qos: ");
  Serial.println(properties.qos);
  Serial.print("...<amMqtt::onMqttMessage> dup: ");
  Serial.println(properties.dup);
  Serial.print("...<amMqtt::onMqttMessage> retain: ");
  Serial.println(properties.retain);
  Serial.print("...<amMqtt::onMqttMessage> len: ");
  Serial.println(len);
  Serial.print("...<amMqtt::onMqttMessage> index: ");
  Serial.println(index);
  Serial.print("...<amMqtt::onMqttMessage> total: ");
  Serial.println(total);
} //onMqttMessage()

/**
 * @brief Event handler for publish ACK from the Mqtt broker.
 * @param uint16_t packetId. Unique ID for packet that was delivered.
===================================================================================================*/
void onMqttPublish(uint16_t packetId) 
{
  Serial.println("<amMqtt::onMqttPublish> Publish acknowledged.");
  Serial.print("...<amMqtt::onMqttMessage> packetId: ");
  Serial.println(packetId);
} //onMqttPublish()

/**
 * @brief Convert a string to an IP address.
 * @param String ipToConvert. An IP address in string format.
 * @return IPAddress. An IP address returnred in IP Address format.
===================================================================================================*/
//IPAddress stringToIP(String ipToConvert)
//{
//   IPAddress brokerIpAddress = brokerIpAddress.fromString(ipToConvert.c_str());
//   return brokerIpAddress;
//} //stringToIP()

/**
 * @brief Initialize event handlers for Mqtt broker communicatin then set up required subscriptions.
 * @param uint16_t packetId. Unique ID for packet that was delivered.
===================================================================================================*/
void initMqtt(String brokerIP) 
{
   mqttReconnectTimer = xTimerCreate("mqttTimer", pdMS_TO_TICKS(2000), pdFALSE, (void*)0, reinterpret_cast<TimerCallbackFunction_t>(connectToMqtt));
   mqttClient.onConnect(onMqttConnect);
   mqttClient.onDisconnect(onMqttDisconnect);
   mqttClient.onSubscribe(onMqttSubscribe);
   mqttClient.onUnsubscribe(onMqttUnsubscribe);
   mqttClient.onMessage(onMqttMessage);
   mqttClient.onPublish(onMqttPublish);
   Serial.println("<amMqtt::initMqtt> Configuring MQTT settings...");
   Serial.print("<amMqtt::initMqtt>...Broker IP set to ");
   Serial.println(brokerIP);
   Serial.print("<amMqtt::initMqtt>...Broker PORT set to ");
   Serial.println(MQTT_PORT);
   mqttClient.setServer(mqttBrokerIp, MQTT_PORT);
   connectToMqtt();
  // Take subscriptions out of setup and put here instead
} //initMqtt()

/**
 * @brief Initialize event handlers for Mqtt broker communicatin then set up required subscriptions.
 * @param uint16_t packetId. Unique ID for packet that was delivered.
===================================================================================================*/
void changeMqttIP(String brokerIP)
{
   mqttClient.setServer(mqttBrokerIp, MQTT_PORT);
   Serial.println("<amMqtt::changeMqttIP> Disconnect from MQTT broker");
   mqttClient.disconnect();
   Serial.println("<amMqtt::changeMqttIP> Connect to MQTT broker");
   mqttClient.connect();
   connectToMqtt();
} //changeMqttIP()

#endif // End of preprocessor directive