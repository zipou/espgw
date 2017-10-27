
#define ICACHE_RAM_ATTR IRAM_ATTR

#include <const.h>
#include <Arduino.h>
// #include <ESP8266mDNS.h>
// #include <ArduinoOTA.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

#include <ArduinoJson.h>

#include <MqttLib.h>
MqttLib mqttlib;

#include <RFLib.h>
RFLib rf;

#include <WifiLib.h>
WifiLib wifi;

#include <DHT.h>
DHT dht(DHTPIN, DHT22);

#include <Ticker.h>
Ticker flipper;

String getChipId() {
    uint8_t chipid[6];
    esp_efuse_read_mac(chipid);
    String id= "";
    for (int i = 0; i<= 5; i++) {
      id += String(chipid[i], HEX);
    }
    return id;
    // Serial.printf("%02x:%02x:%02x:%02x:%02x:%02x\n", chipid[0], chipid[1], chipid[2], chipid[3], chipid[4], chipid[5]);
}

void sendSensor() {

  int i = 0;
  int tryHarder = 0;
  float h = 0;
  float t = 0;
  while (i <= tryHarder && (isnan(h) || h==0 ) ) {
    Serial.println("Trying to read from DHT sensor!");
    // Serial.println(i);
    h = dht.readHumidity();
    t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit
    i = i + 1;
  }

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Serial.println("Sending Info through mqtt");

  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& message = jsonBuffer.createObject();
  message["temperature"] = t;
  message["humidity"] = h;

  JsonObject& root = jsonBuffer.createObject();
  root["sensor"] = getChipId();
  root["protocol"] = "dht";
  root["data"] = message;
  char buffer[150];
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
      // Serial.println(protocol);
      // Serial.println(data);
      // Serial.println("PROTOCOL CODE SENT");
  }
}

void rfCallback(const char* protocol, const char* message) {
  // Serial.println("RF Callback HAS BEEN CALED");
  // Serial.println(protocol);
  // Serial.println(message);
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  root["sensor"] = getChipId();
  root["protocol"] = protocol;
  root["data"] = message;
  char buffer[150];
  root.printTo(buffer);
  mqttlib.publish(MQTT_TOPIC_OUT, buffer);
}

void setup() {
  Serial.begin(SERIAL_CONSOLE_SPEED);
  pinMode(BUILTIN_LED, OUTPUT);
  digitalWrite(BUILTIN_LED, HIGH);

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

  dht.begin();

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
  // delay(10);
}
