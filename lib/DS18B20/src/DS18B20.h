#include <OneWire.h>

class DS18B20 {
  public:
    DS18B20();
    void init(int pin);
    float getTemperature();
    boolean readTemperature();
    static float _temperature;
  private:
    OneWire* _oneWire;
};
