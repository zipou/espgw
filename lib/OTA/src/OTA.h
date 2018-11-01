#include <ArduinoOTA.h>

class OTA {

  public:
    OTA();
    void init(char* hostname);
    void loop();
  protected:
    // WebServer* _WebServer;
};
