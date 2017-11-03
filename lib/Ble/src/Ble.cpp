/*
   Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleScan.cpp
   Ported to Arduino ESP32 by Evandro Copercini
*/

#include "Ble.h"
#include <Arduino.h>

int scanTime = 5; //In seconds

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) {
      Serial.printf("Advertised Device: %s \n", advertisedDevice.toString().c_str());
    }
};

void Ble::init() {
  // Serial.begin(115200);
  BLEDevice::init("");
}

void Ble::scan() {
  Serial.println("Scanning...");
  BLEScan* pBLEScan = BLEDevice::getScan(); //create new scan
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster
  BLEScanResults foundDevices = pBLEScan->start(scanTime);
  // Serial.print("Devices found: ");
  // Serial.println(foundDevices.getCount());
  Serial.println("Scan done!");
}

Ble::Ble() {

}
