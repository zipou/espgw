#include <Update.h>
#include <WebServer.h>

class OTA {

  public:
    OTA();
    void init(int port);
    void loop();
  protected:
    WebServer* _WebServer;
};
