#include <WiFiClientSecure.h>
#include <MQTTClient.h>

typedef void (*MqttLibCallback)(const char* topic, const char* payload);
typedef void (*MqttLibErrorCallback)();

class MqttLib {

   public:
     MqttLib();
     void init(char* host, int  port, char* username, char* password);
     bool connect();
     void loop();
     void publish(const char* topic, const char* message);
     void ack(String &topic, String &payload);
     void acknowledge(String payload);
     void subscribe(const char* topic);
     void setCallback(MqttLibCallback _callback);
     void setErrorCallback(MqttLibErrorCallback _errorCallback);
     static MqttLibCallback _callback;
     static MqttLibErrorCallback _errorCallback;
   protected:
};
