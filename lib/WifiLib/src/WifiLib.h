
#include <ESP8266WiFi.h>

class WifiLib {

 public:

  WifiLib();

  void connect(char* ssid, char* password);

  WiFiClientSecure getClient();

};
