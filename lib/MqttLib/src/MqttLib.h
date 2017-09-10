#include <WiFiClientSecure.h>
#include <MQTTClient.h>

typedef void (*MqttLibCallback)(const char* topic, const char* payload);

class MqttLib {

   public:
     MqttLib();
     void init(char* host, int  port, char* username, char* password);
     void connect();
     void loop();
     void publish(const char* topic, const char* message);
     void subscribe(const char* topic);
     void setCallback(MqttLibCallback _callback);
     static MqttLibCallback _callback;
   protected:
};
