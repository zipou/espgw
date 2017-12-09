
#include <WifiLib.h>

WiFiClientSecure netSsl;

char* WifiLib::_ssid;
char* WifiLib::_password;

// IPAddress dns(8, 8, 8, 8);

void WifiLib::connect() {
  WiFi.begin(_ssid, _password);
  Serial.print("Checking wifi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  // WiFi.setDNS(dns);
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("Gateway: ");
  Serial.println(WiFi.gatewayIP());
}

boolean WifiLib::isConnected() {
  return (WiFi.status() == WL_CONNECTED);
}

void WifiLib::checkAndReconnect() {
  if (!isConnected()) {
    connect();
  }
}

WiFiClientSecure WifiLib::getClient() {
  return netSsl;
}

WifiLib::WifiLib(char* ssid, char* password) {
  WifiLib::_ssid = ssid;
  WifiLib::_password = password;
}
