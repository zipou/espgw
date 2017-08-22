#include "SerialLib.h"
#include "Arduino.h"

char SerialLib::delimiter = 10;

SerialLib::SerialLib() {
}

SerialLib::~SerialLib() {
   if (NULL != _serial){
       delete _serial;
   }
}

void SerialLib::init(int rx, int tx, int speed, int buffer_size) {
    if (NULL != _serial) {
        delete _serial;
    }
    _serial= new SoftwareSerial(rx, tx);
    _serial->begin(speed);
    _buffer_size = buffer_size;
    // Serial.println("BUFFER");
    // Serial.println(_buffer_size);

}

void SerialLib::send(const char* json) {
    // Serial.println(json);
    const char* car = ";";
    char buffer[_buffer_size];
    strncpy(buffer, json, sizeof(buffer));
    strncat(buffer, car, sizeof(buffer));
    Serial.println("buffer");
    Serial.print(buffer);
    _serial->print(buffer);
}

void SerialLib::setCallback(SerialLibCallback callback) {
    _callback = callback;
}

void SerialLib::receive() {
    if ( _serial->available() ) {
      char buffer[_buffer_size];
      _serial->readBytesUntil(59, buffer, sizeof(buffer));
      // Serial.println(buffer);
      // char c = _serial->read();
      // if (c == 13) {
      //   Serial.println("Carriage Return");
      // }
      // Serial.write(c);
      // Serial.printf("%d", c);
      if (NULL != _callback) {
        _callback(buffer);
      }
    }
}
