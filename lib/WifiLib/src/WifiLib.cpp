
#include <WifiLib.h>

WiFiClientSecure netSsl;
// IPAddress dns(8, 8, 8, 8);

void WifiLib::connect(char* ssid, char* password) {
  WiFi.begin(ssid, password);
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

WiFiClientSecure WifiLib::getClient() {
  return netSsl;
}

WifiLib::WifiLib() {
}
