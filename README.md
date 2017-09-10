ESPGW
=====

## What is it ?

Playground project to make a 2 ways RF 443 gateway through MQTT.
Used to learn c++ and play with Arduino & ESP.

## How does it work

Due to the size of the great RF Lib [https://github.com/puuu/ESPiLight](ESPiLight) and the lack of RAM on esp8866, you can't use SSL with MQTT *AND* load the RF library.

So you have to use a esp32.

## Installation

All you need to do is to create a `const.h` file based on `const.example.h` and you should be good to go as long as you have the hardware set.
