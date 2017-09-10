#include "MqttLib.h"

WiFiClientSecure _wificlient;
MQTTClient _client;

MqttLibCallback MqttLib::_callback;

char* _username;
char* _password;

void MqttLib::setCallback(MqttLibCallback callback) {
  MqttLib::_callback = callback;
  struct call {
    static void a(String &topic, String &payload) {
      Serial.println("Message Recived Through MQTT");
      if (MqttLib::_callback != NULL) {
        (*MqttLib::_callback)(topic.c_str(), payload.c_str());
      }
    }
  };
  _client.onMessage(call::a);
}

void MqttLib::init(char* host, int port, char* username, char* password) {
  _client.begin(host, port, _wificlient);
  _username= username;
  _password= password;
  MqttLib::connect();
}

void MqttLib::publish(const char* topic, const char* message) {
  if (! _client.connected()) {
    MqttLib::connect();
  }
  _client.publish(topic, message);
}

void MqttLib::connect() {
  while (!_client.connect("arduino", _username, _password)) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("MQTT connected");
}

void MqttLib::subscribe(const char *topic) {
  Serial.println("Subscribed to");
  Serial.println(topic);
  _client.subscribe(topic);
}

void MqttLib::loop() {
  _client.loop();
}

MqttLib::MqttLib() {

}
