#include <ESPiLight.h>

typedef void (*RFLibCallback)(const char* protocol, const char* message);

class RFLib {

   public:
    RFLib();
    void init(int sendPin, int receivePin);
    void initReceiver(int receivePin);
    void loop();
    void send(char* protocol, char* message);
    void setCallback(RFLibCallback callback );
    static RFLibCallback _callback;
  private:
    ESPiLight* _rf;
};
