#include "RFLib.h"

RFLibCallback RFLib::_callback;

void RFLib::init(int sendPin, int receivePin) {
  _rf = new ESPiLight(sendPin);
  _rf->initReceiver(receivePin);
  Serial.println("Init RF");
}

void RFLib::initReceiver(int receivePin) {
  _rf->initReceiver(receivePin);
}

void RFLib::setCallback( RFLibCallback callback ) {
  Serial.println("Setting Callback");
  RFLib::_callback = callback;
  struct call {
    static void a(const String &protocol, const String &message, int status, int repeats, const String &deviceID) {
      if(status==VALID) {
        // Serial.print("Valid message: [");
        // Serial.print(protocol);
        // Serial.print("] ");
        // Serial.print(message);
        // Serial.println();
        if (RFLib::_callback != NULL) {
          (*RFLib::_callback)(protocol.c_str(), message.c_str());
        }
      }
    }
  };
  _rf->setCallback(call::a);
}

void RFLib::send(char* protocol, char* message) {
  //_rf.send("arctech_contact", "{\"id\" : 19970622, \"unit\" : 0, \"opened\" : 1 }");
  _rf->send(String(protocol), String(message));
};

void RFLib::loop() {
  _rf->loop();
}

RFLib::RFLib() {
}
