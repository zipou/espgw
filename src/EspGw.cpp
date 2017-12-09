
#define ICACHE_RAM_ATTR IRAM_ATTR

#include <const.h>
#include <Arduino.h>
// #include <ESP8266mDNS.h>
// #include <ArduinoOTA.h>
#include <WiFi.h>
#include <WiFiClient.h>
// #include <BlynkSimpleEsp32.h>

#include <ArduinoJson.h>

#include <MqttLib.h>
MqttLib mqttlib;

#include <RFLib.h>
RFLib rf;

#include <WifiLib.h>
WifiLib wifi = WifiLib(WIFISSID, WIFIPASSWD);

// #include <DHT.h>
// DHT dht(DHTPIN, DHT22);

#include <Ticker.h>
Ticker flipper;

#include <DS18B20.h>
DS18B20 tempChip;

// #include <Ble.h>
// Ble ble;

String getChipId() {
    uint8_t chipid[6];
    esp_efuse_read_mac(chipid);
    String id= "";
    for (int i = 0; i<= 5; i++) {
      id += String(chipid[i], HEX);
    }
    return id;
}

void sendSensor() {
  Serial.println("Sending Info through mqtt");

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
  }
}

void rfCallback(const char* protocol, const char* message) {
  Serial.println("RF Callback HAS BEEN CALED");
  Serial.println(protocol);
  Serial.println(message);
  StaticJsonBuffer<250> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  root["sensor"] = getChipId();
  root["protocol"] = protocol;
  root["data"] = message;
  char buffer[250];
  root.printTo(buffer);
  mqttlib.publish(MQTT_TOPIC_OUT, buffer);
}

float readTemperature() {
  Serial.println("Reading Temp...");
  float temp = tempChip.getTemperature();
  Serial.println(temp);
  return temp;
}

// void scanBle() {
//   ble.scan();
// }

void setup() {

  // ble.init();

  Serial.begin(SERIAL_CONSOLE_SPEED);
  pinMode(BUILTIN_LED, OUTPUT);
  digitalWrite(BUILTIN_LED, HIGH);

  wifi.connect();
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

  // dht.begin();
  tempChip.init(DHTPIN);

  flipper.setCallback(sendSensor);
  flipper.setInterval(TEMP_INTERVAL);
  flipper.start();

  digitalWrite(BUILTIN_LED, LOW);
}

void loop() {
  // ArduinoOTA.handle();
  // delay(1000);
  flipper.update();
  // digitalWrite(D4, LOW);
  // delay(500);
  // digitalWrite(D4, HIGH);

  rf.loop();

  // delay(10);
  mqttlib.loop();
  wifi.checkAndReconnect();
  // delay(10);
}
