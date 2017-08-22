
#include <WifiLib.h>

WiFiClientSecure netSsl;

void WifiLib::connect(char* ssid, char* password) {
  WiFi.begin(ssid, password);
  Serial.print("Checking wifi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

WiFiClientSecure WifiLib::getClient() {
  return netSsl;
}

WifiLib::WifiLib() {
}
