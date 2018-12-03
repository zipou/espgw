
// #define ICACHE_RAM_ATTR IRAM_ATTR

#include "const.h"
#include <Arduino.h>
// #include <ESP8266mDNS.h>
// #include <ESPmDNS.h>
// #include <ArduinoOTA.h>
#include <WiFi.h>
#include <WiFiClient.h>
// #include <BlynkSimpleEsp32.h>

// #include <ESPmDNS.h>
// #include <WiFiUdp.h>
// #include <ArduinoOTA.h>

#include <ArduinoJson.h>

// #include <EasyOTA.h>
// EasyOTA OTA("arduino_hostname");

#include <MqttLib.h>
MqttLib mqttlib;

#include <RFLib.h>
RFLib rf;

#include <WifiLib.h>
WifiLib wifi = WifiLib(WIFISSID, WIFIPASSWD);

#include <DS18B20.h>
DS18B20 tempChip;

#include <OTA.h>
OTA otaUpdate;

// #include <Ble.h>
// Ble ble;

String getChipId() {
    uint8_t chipid[6];
    esp_efuse_mac_get_default(chipid);
    String id= "";
    for (int i = 0; i<= 5; i++) {
      id += String(chipid[i], HEX);
    }
    return id;
}

void getTemperature() {
  Serial.println("Trying to read temperature");
  float temp = tempChip.getTemperature();
  Serial.println(temp);
}

void restartEsp() {
  Serial.println("Restarting ESP");
  ESP.restart();
}


void sendTemperature() {
  // Serial.println("Sending Info through mqtt");

  float temp = tempChip.getTemperature();

  StaticJsonBuffer<250> jsonBuffer;
  JsonObject& message = jsonBuffer.createObject();
  message["temperature"] = temp;

  JsonObject& root = jsonBuffer.createObject();
  root["sensor"] = getChipId();
  root["protocol"] = "temperature";
  root["data"] = message;
  char buffer[250];
  root.printTo(buffer);
  mqttlib.publish(MQTT_TOPIC_OUT, buffer);

}

void mqttErrorCallback() {
  Serial.println("Error with MQTT, restarting ESP");
  restartEsp();
}

void mqttConnectCallback() {
  mqttlib.subscribe(MQTT_TOPIC_IN);
  mqttlib.subscribe(MQTT_TOPIC_IN_RAW);
}

void mqttCallback(const char* topic, const char* message) {
  if (strcmp(topic, MQTT_TOPIC_IN) == 0) {
      StaticJsonBuffer<200> jsonBuffer;
      JsonObject& root = jsonBuffer.parseObject(message);
      const char* protocol = root["protocol"];
      const char* data = root["data"];

      rf.send((char*) protocol, (char*) data);
  }
}

void rfCallback(const char* protocol, const char* message) {
  // Serial.println("RF Callback HAS BEEN CALED");
  // Serial.println(protocol);
  // Serial.println(message);
  StaticJsonBuffer<250> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  root["sensor"] = getChipId();
  root["protocol"] = protocol;
  root["data"] = message;
  char buffer[250];
  root.printTo(buffer);
  mqttlib.publish(MQTT_TOPIC_OUT, buffer);
}

// void readTemperature() {
//   // Serial.println("Reading Temp...");
//   // float temp = tempChip.getTemperature();
//   // Serial.println(temp);
//   // return temp;
// }


#include <Ticker.h>
Ticker timer1(sendTemperature, TEMP_INTERVAL);
// Ticker timer2(restartEsp, RESTART_INTERVAL);

void setup() {

  Serial.begin(SERIAL_CONSOLE_SPEED);
  pinMode(BUILTIN_LED, OUTPUT);
  digitalWrite(BUILTIN_LED, HIGH);

  wifi.connect();
  // ArduinoOTA.setPassword(ARDUINO_PASS);
  // ArduinoOTA.begin();

  MqttLibCallback mqttCb = &mqttCallback;
  // MqttLibCallback mqttCb = &mqttCallback;
  char clientId[15];
  getChipId().toCharArray(clientId, 15);
  Serial.println("ClientId");
  Serial.println(clientId);
  mqttlib.setErrorCallback(mqttErrorCallback);
  mqttlib.setConnectCallback(mqttConnectCallback);
  mqttlib.init(MQTT_HOST, MQTT_PORT, MQTT_USERNAME, MQTT_PASSWORD, clientId);
  mqttlib.setCallback(mqttCb);
  // mqttlib.subscribe(MQTT_TOPIC_IN);
  // mqttlib.subscribe(MQTT_TOPIC_IN_RAW);

  rf.init(RF_TRANSMITTER_PIN, RF_RECEIVER_PIN);
  RFLibCallback afunc = &rfCallback;
  rf.setCallback(afunc);


  otaUpdate.init(8080);


  if (ENABLE_TEMP_SENSOR) {
    tempChip.init(DHTPIN);
    timer1.start();
  }
  // timer2.start();

  digitalWrite(BUILTIN_LED, LOW);
}

void loop() {
  if (ENABLE_TEMP_SENSOR) {
    timer1.update();
  }
  // timer2.update();

  if (ENABLE_RF_RECEIVE) {
    rf.loop();
  }

  if (ENABLE_RF_SEND) {
    mqttlib.loop();
  }

  wifi.checkAndReconnect();
  otaUpdate.loop();
  // delay(10);
}
