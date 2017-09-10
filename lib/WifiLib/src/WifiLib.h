
// #include <ESP8266WiFi.h>
// #include <WiFi.h>
#include <WiFiClientSecure.h>

class WifiLib {

 public:

  WifiLib();

  void connect(char* ssid, char* password);

  WiFiClientSecure getClient();

};
