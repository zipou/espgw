#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

class Ble {

  public:
    Ble();
    void init();
    void scan();
};
