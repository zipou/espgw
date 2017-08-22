#include <const.h>
#include <Arduino.h>
#include <ESP8266mDNS.h>
#include <ArduinoOTA.h>

#include <MqttLib.h>
MqttLib mqttlib;

#include <RFLib.h>
RFLib rf;

#include <WifiLib.h>
WifiLib wifi;

#include <SerialLib.h>
SerialLib seriallib;

int is_server;

void mqttCallback(const char* topic, const char* message) {
  Serial.println("MQTTCallback");
  Serial.println(topic);
  Serial.println(message);
}

void serialCallback(const char* json) {
  Serial.println("SerialCallback");
  Serial.println(json);
  mqttlib.publish(MQTT_TOPIC_OUT, json);
}

// void myCallback(const String &protocol, const String &message) {
void rfCallback(const char* protocol, const char* message) {
  Serial.println("RF Callback HAS BEEN CALED");
  Serial.println(protocol);
  Serial.println(message);
  seriallib.send(message);
}

void setup() {
  Serial.begin(SERIAL_CONSOLE_SPEED);
  seriallib.init(SERIAL_RECEIVER_PIN, SERIAL_TRANSMITTER_PIN, SERIAL_SPEED, SERIAL_BUFFER_SIZE);

  pinMode(IS_SERVER_PIN, INPUT);
  pinMode(D4, OUTPUT);
  digitalWrite(D4, HIGH);

  is_server = digitalRead(IS_SERVER_PIN);

  wifi.connect(WIFISSID, WIFIPASSWD);
  ArduinoOTA.setPassword(ARDUINO_PASS);
  ArduinoOTA.begin();


  if (is_server == 1) {
    SerialLibCallback cbSerial = &serialCallback;
    seriallib.setCallback(cbSerial);
    MqttLibCallback mqttCb = &mqttCallback;
    mqttlib.setCallback(mqttCb);
    mqttlib.init(MQTT_HOST, MQTT_PORT, MQTT_USERNAME, MQTT_PASSWORD);
    mqttlib.subscribe(MQTT_TOPIC_IN);
    mqttlib.subscribe(MQTT_TOPIC_IN_RAW);
  } else {
    rf.init(RF_TRANSMITTER_PIN, RF_RECEIVER_PIN);
    RFLibCallback afunc = &rfCallback;
    rf.setCallback(afunc);
  }


}

void loop() {
  ArduinoOTA.handle();

  // delay(1000);
  // digitalWrite(D4, LOW);
  // delay(500);
  // digitalWrite(D4, HIGH);

  if (!is_server) {
    rf.loop();
    // Serial.println("Sending BLINK throught Serial Link");
    // const char* hello = "BLINK";
    // seriallib.send(hello);
    // digitalWrite(D4, LOW);
    // delay(500);
    // digitalWrite(D4, HIGH);
    // delay(500);
  } else {
    //Need to Listen to Serial Port for input to send
    seriallib.receive();
  }



  // rf.send("arctech_screen", "{\"id\":19970622,\"unit\":0,\"down\":1}");
  // delay(2000);
  // rf.send("arctech_screen", "{\"id\":19970622,\"unit\":0,\"up\":1}");
  // delay(2000);
  // rf.loop();
  // delay(10000);
  // delay(10);
}
