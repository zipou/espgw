#include <RCSwitch.h>

typedef void (*RFLibCallback)(const char* protocol, const char* message);

class RFLib {

   public:
    RFLib();
    void init(int sendPin, int receivePin);
    void initReceiver(int receivePin);
    void loop();
    void send(char* protocol, char* message);
    void sendRaw(char* string);
    void setCallback(RFLibCallback callback );
    static RFLibCallback _callback;
  private:
    RCSwitch _rc;
};
