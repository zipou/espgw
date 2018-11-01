#include "OTA.h"
// #include <Update.h>
// #include <ESPmDNS.h>

void OTA::init(char* hostname) {
  ArduinoOTA.setHostname(hostname);
  ArduinoOTA.begin();
  // Serial.println("Init OTA server");
  // // _port = port;
  // // Serial.println(_port);

  // // _WebServer = new WebServer(_port);

  // _WebServer->on("/", HTTP_GET, [&](){
  //   Serial.println('HTTP GET');
  //   _WebServer->sendHeader("Connection", "close");
  //   _WebServer->send(200, "text/html", "TUTU");
  // });

  // _WebServer->on("/update", HTTP_POST, [&](){
  //   Serial.println('HTTP POST');
  //   _WebServer->sendHeader("Connection", "close");
  //   _WebServer->send(200, "text/plain", (Update.hasError())?"FAIL":"OK");
  //   ESP.restart();
  // },[&](){
  //   HTTPUpload& upload = _WebServer->upload();
  //   if(upload.status == UPLOAD_FILE_START){
  //     Serial.setDebugOutput(true);
  //     // WiFiUDP::stopAll();
  //     Serial.printf("Update: %s\n", upload.filename.c_str());
  //     // uint32_t maxSketchSpace = (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000;
  //     if(!Update.begin()){
  //       Serial.println("Error after begin");
  //       Update.printError(Serial);
  //     }
  //   } else if(upload.status == UPLOAD_FILE_WRITE){
  //     if(Update.write(upload.buf, upload.currentSize) != upload.currentSize){
  //       // Serial.println("Error after write");
  //       Update.printError(Serial);
  //     }
  //   } else if(upload.status == UPLOAD_FILE_END){
  //     if(Update.end(true)){ //true to set the size to the current progress
  //       Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
  //     } else {
  //       Serial.println("Error Unknonw");
  //       Update.printError(Serial);
  //     }
  //     Serial.setDebugOutput(false);
  //   }
  //   yield();
  // });
  // _WebServer->begin();
}

void OTA::loop() {
  ArduinoOTA.handle();
  // // Serial.println("OTA LOOP");
  // _WebServer->handleClient();
}

OTA::OTA() {
}