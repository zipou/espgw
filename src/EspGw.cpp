
#define ICACHE_RAM_ATTR IRAM_ATTR
/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

#include <const.h>
#include <Arduino.h>
// #include <ESP8266mDNS.h>
// #include <ArduinoOTA.h>

#include <ArduinoJson.h>

#include <MqttLib.h>
MqttLib mqttlib;

#include <RFLib.h>
RFLib rf;

#include <WifiLib.h>
WifiLib wifi;

void mqttCallback(const char* topic, const char* message) {
  // Serial.println("MQTTCallback");
  // Serial.println(topic);
  // Serial.println(message);
  if (strcmp(topic, MQTT_TOPIC_IN) == 0) {
      StaticJsonBuffer<200> jsonBuffer;
      JsonObject& root = jsonBuffer.parseObject(message);
      const char* protocol = root["protocol"];
      const char* data = root["data"];
      rf.send((char*) protocol, (char*) data);
      // Serial.println(protocol);
      // Serial.println(data);
      // Serial.println("PROTOCOL CODE SENT");
  }
  if (strcmp(topic, MQTT_TOPIC_IN_RAW) == 0) {
    rf.sendRaw((char*) message);
    // Serial.println("RAW CODE SENT");
  }
}

void serialCallback(const char* json) {
  // Serial.println("SerialCallback");
  // Serial.println(json);
  mqttlib.publish(MQTT_TOPIC_OUT, json);
}

void rfCallback(const char* protocol, const char* message) {
  // Serial.println("RF Callback HAS BEEN CALED");
  // Serial.println(protocol);
  // Serial.println(message);
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  root["protocol"] = protocol;
  root["data"] = message;
  char buffer[150];
  root.printTo(buffer);
  mqttlib.publish(MQTT_TOPIC_OUT, buffer);
}

void setup() {
  Serial.begin(SERIAL_CONSOLE_SPEED);
  pinMode(BUILTIN_LED, OUTPUT);
  digitalWrite(BUILTIN_LED, LOW);

  wifi.connect(WIFISSID, WIFIPASSWD);
  // ArduinoOTA.setPassword(ARDUINO_PASS);
  // ArduinoOTA.begin();

  MqttLibCallback mqttCb = &mqttCallback;
  mqttlib.init(MQTT_HOST, MQTT_PORT, MQTT_USERNAME, MQTT_PASSWORD);
  mqttlib.setCallback(mqttCb);
  mqttlib.subscribe(MQTT_TOPIC_IN);
  mqttlib.subscribe(MQTT_TOPIC_IN_RAW);
  rf.init(RF_TRANSMITTER_PIN, RF_RECEIVER_PIN);
  RFLibCallback afunc = &rfCallback;
  rf.setCallback(afunc);

  digitalWrite(BUILTIN_LED, HIGH);
}

void loop() {
  // ArduinoOTA.handle();

  // delay(1000);
  // digitalWrite(D4, LOW);
  // delay(500);
  // digitalWrite(D4, HIGH);
  rf.loop();
  delay(10);
  mqttlib.loop();
  delay(10);
}
