#include "MqttLib.h"

#include <ArduinoJson.h>

WiFiClientSecure _wificlient;
MQTTClient _client;

MqttLibCallback MqttLib::_callback;
MqttLibErrorCallback MqttLib::_errorCallback;

char* _username;
char* _password;
char* _clientId;

void MqttLib::setErrorCallback(MqttLibErrorCallback callback) {
  MqttLib::_errorCallback = callback;
}


void static ack(String &topic, String &payload) {

}

void MqttLib::setCallback(MqttLibCallback callback) {
  MqttLib::_callback = callback;



  struct call {
    static void a(String &topic, String &payload) {
      StaticJsonBuffer<200> jsonBuffer;
      JsonObject& root = jsonBuffer.parseObject(payload);
      const char* id = root["id"];
      Serial.println("Message Recived Through MQTT with id");
      Serial.println(id);

      MqttLib::acknowledge(payload);


      //Acknowledging
      StaticJsonBuffer<250> jsonAckBuffer;
      JsonObject& rootAck = jsonAckBuffer.createObject();
      rootAck["type"] = "acknowledge";
      rootAck["id"] = id;
      char charBuffer[250];
      rootAck.printTo(charBuffer);


      // StaticJsonBuffer<250> ackBuffer;
      // JsonObject& ackMessage = ackBuffer.createObject();
      // ackMessage["type"] = "acknowledge";
      // ackMessage["id"] = id;
      // char buffer[250];
      // ackMessage.printTo(ackBuffer);
      _client.publish("esp_ack", charBuffer);

      if (MqttLib::_callback != NULL) {
        (*MqttLib::_callback)(topic.c_str(), payload.c_str());
      }
    }
  };
  _client.onMessage(call::a);

}


void MqttLib::acknowledge(String payload) {
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(payload);
  const char* id = root["id"];
  Serial.println("Message Recived Through MQTT with id");
  Serial.println(id);
}

void MqttLib::init(char* host, int port, char* username, char* password, char* clientId) {
  _client.begin(host, port, _wificlient);
  _username= username;
  _password= password;
  _clientId= clientId;
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
  while (!_client.connect(_clientId, _username, _password)) {
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
