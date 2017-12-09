
// #include <ESP8266WiFi.h>
// #include <WiFi.h>
#include <WiFiClientSecure.h>

class WifiLib {

 public:
  static char* _ssid;
  static char* _password;
  WifiLib(char* ssid, char* password);

  void connect();
  void checkAndReconnect();
  boolean isConnected();

  WiFiClientSecure getClient();

};
