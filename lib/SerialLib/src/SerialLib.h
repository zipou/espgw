#include <SoftwareSerial.h>

typedef void (*SerialLibCallback)(const char* json);

class SerialLib {

  public:
    SerialLib();
    ~SerialLib();
    SerialLibCallback _callback;
    void init(int rx, int tx, int speed, int buffer_size);
    void send(const char* json);
    void receive();
    void setCallback(SerialLibCallback callback);
    static char delimiter;
  private:
    SoftwareSerial* _serial;
    int _buffer_size;
};
