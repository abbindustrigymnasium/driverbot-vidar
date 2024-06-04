#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define motorPinRightDir  0 //D2
#define motorPinRightSpeed 5 //D1

#include "MQTTConnector.h"
#include "Credentials.h"

#include <Servo.h>

Servo servo;

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

boolean mqttInitCompleted = false;
String clientId = "IoTPractice-" + String(ESP.getChipId());

bool headlights = false;

void startup(){
  servo.attach(D4);
  servo.write(90);
  performConnect();
  
 
};
/* Incoming data callback. */
void dataCallback(char* topic, byte* payload, unsigned int length)
{
  char payloadStr[length + 1];
  strncpy(payloadStr, (char*)payload, length);
  payloadStr[length] = u8'\0';
  
  //Define pins
  pinMode(motorPinRightDir, OUTPUT);
  pinMode(motorPinRightSpeed, OUTPUT);
  //Lights
  pinMode(D8, OUTPUT);
  pinMode(D5, OUTPUT);
  pinMode(D6, OUTPUT);
  pinMode(D7, OUTPUT);

  
  
  //Forward movement after recieving the MQTT message
  if (strcmp(payloadStr, "forward") == 0) {
      int speed = 1000;
     

      digitalWrite(motorPinRightDir, 1);
      analogWrite(motorPinRightSpeed, speed);
 
  }

  //Backwards movement after recieving the MQTT message
  if (strcmp(payloadStr, "backwards") == 0) {
      int speed = 600;
      int dir = 0;

      digitalWrite(motorPinRightDir, dir);
      analogWrite(motorPinRightSpeed, speed);
      
  }

  //Stop when the MQTT message is recieved (It will run indefinately otherwise)
  if (strcmp(payloadStr, "STOP") == 0) {
      int speed = 0;
     

      digitalWrite(motorPinRightDir, 0);
      analogWrite(motorPinRightSpeed, speed);
 
  }
  
  //Turn left when the MQTT message is recieved
  if (strcmp(payloadStr, "left") == 0) {
      servo.write(180);
  }

  //Turn right when the MQTT message is recieved
  if (strcmp(payloadStr, "right") == 0) {
      servo.write(0); 
  }

  //Center the steering when the message is recieved
  if (strcmp(payloadStr, "Midskepps") == 0) {
      servo.write(90);
  }

  //Blink three times when the MQTT message is recieved
  if (strcmp(payloadStr, "blinkersV") == 0) {
      digitalWrite(D5, HIGH);
      delay(500);
      digitalWrite(D5, LOW);
      delay(500);
      digitalWrite(D5, HIGH);
      delay(500);
      digitalWrite(D5, LOW);
      delay(500);
      digitalWrite(D5, HIGH);
      delay(500);
      digitalWrite(D5, LOW);
  }

  //Blink three times when the MQTT message is recieved
  if (strcmp(payloadStr, "blinkersH") == 0) {
      digitalWrite(D6, HIGH);
      delay(500);
      digitalWrite(D6, LOW);
      delay(500);
      digitalWrite(D6, HIGH);
      delay(500);
      digitalWrite(D6, LOW);
      delay(500);
      digitalWrite(D6, HIGH);
      delay(500);
      digitalWrite(D6, LOW);
  }

  //Toggle the headlights when the MQTT message is recieved
  if (strcmp(payloadStr, "Headlights") == 0) {
      headlights = !headlights;
  }
  if ((headlights) == true) {
    digitalWrite(D7, HIGH);
  }

  if ((headlights) == false) {
    digitalWrite(D7, LOW);
  }
  
}


void performConnect()
{
  uint16_t connectionDelay = 1000;
  while (!mqttClient.connected())
  {
    Serial.printf("Trace   : Attempting MQTT connection...\n");
    if (mqttClient.connect(clientId.c_str(), MQTT_USERNAME, MQTT_KEY))
    {
      Serial.printf("Trace   : Connected to Broker.\n");

      /* Subscription to your topic after connection was succeeded.*/
      MQTTSubscribe(TOPIC);
    }
    else
    {
      Serial.printf("Error!  : MQTT Connect failed, rc = %d\n", mqttClient.state());
      Serial.printf("Trace   : Trying again in %d msec.\n", connectionDelay);
      delay(connectionDelay);
    }
  }
}

boolean MQTTPublish(const char* topic, char* payload)
{
  boolean retval = false;
  if (mqttClient.connected())
  {
    retval = mqttClient.publish(topic, payload);
  }
  return retval;
}

boolean MQTTSubscribe(const char* topicToSubscribe)
{
  boolean retval = false;
  if (mqttClient.connected())
  {
    retval = mqttClient.subscribe(topicToSubscribe);
  }
  return retval;
}

boolean MQTTIsConnected()
{
  return mqttClient.connected();
}

void MQTTBegin()
{
  mqttClient.setServer(MQTT_BROKER, MQTT_BROKER_PORT);
  mqttClient.setCallback(dataCallback);
  mqttInitCompleted = true;
}

void MQTTLoop()
{
  if(mqttInitCompleted)
  {
    mqttClient.loop();
    delayMicroseconds(12);
  }
}