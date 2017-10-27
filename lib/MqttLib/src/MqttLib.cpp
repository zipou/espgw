#include "MqttLib.h"

WiFiClientSecure _wificlient;
MQTTClient _client;

MqttLibCallback MqttLib::_callback;
MqttLibErrorCallback MqttLib::_errorCallback;

char* _username;
char* _password;

void MqttLib::setErrorCallback(MqttLibErrorCallback callback) {
  MqttLib::_errorCallback = callback;
}

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
    if (! MqttLib::connect()) {
        return;
    }
  }
  _client.publish(topic, message);
}

bool MqttLib::connect() {
  int i = 0;
  while (!_client.connect("arduino", _username, _password)) {
    Serial.print(".");
    delay(1000);
    if (i >= 60) {
      if (MqttLib::_errorCallback != NULL) {
        (*MqttLib::_errorCallback)();
      }
      return false;
    }
  }
  Serial.println("MQTT connected");
  return true;
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
