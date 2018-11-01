#include "RFLib.h"

RFLibCallback RFLib::_callback;

void RFLib::init(int sendPin, int receivePin) {
  _rc = RCSwitch();
  _rc.enableReceive(receivePin);
  //_rf->initReceiver(receivePin);
  Serial.println("Init RF");
}

void RFLib::initReceiver(int receivePin) {
  // _rf->initReceiver(receivePin);
}

void RFLib::setCallback( RFLibCallback callback ) {
  Serial.println("Setting Callback");
  RFLib::_callback = callback;
  // ESPILight receive function
  // struct call {
  //   static void a(const String &protocol, const String &message, int status, int repeats, const String &deviceID) {
  //     if(status==VALID) {
  //       // Serial.print("Valid message: [");
  //       // Serial.print(protocol);
  //       // Serial.print("] ");
  //       // Serial.print(message);
  //       // Serial.println();
  //       if (RFLib::_callback != NULL) {
  //         (*RFLib::_callback)(protocol.c_str(), message.c_str());
  //       }
  //     }
  //   }
  // };
  // _rf->setCallback(call::a);
}

void RFLib::send(char* protocol, char* message) {
  //_rf.send("arctech_contact", "{\"id\" : 19970622, \"unit\" : 0, \"opened\" : 1 }");
  // _rf->send(String(protocol), String(message));
};

void RFLib::sendRaw(char* string) {
  uint16_t codes[255];
  // get pulse train from string (format see: pilight USB Nano)
  // int length = _rf->stringToPulseTrain(String(string), codes, 255);
  // transmit the pulse train
  // _rf->sendPulseTrain(codes, length);
}

void RFLib::loop() {
  // _rf->loop();
  if (_rc.available()) {
    unsigned long value = _rc.getReceivedValue();
    char valueCharArray[10];
    ltoa(value, valueCharArray, 10);
    (*RFLib::_callback)("rcswitch", valueCharArray);
    _rc.resetAvailable();
  }
}

RFLib::RFLib() {
}
