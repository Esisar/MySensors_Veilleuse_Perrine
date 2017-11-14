#ifdef __IN_ECLIPSE__
//This is a automatic generated file
//Please do not modify this file
//If you touch this file your change will be overwritten during the next build
//This file has been generated on 2017-11-13 11:46:01

#include "Arduino.h"
#define MY_DEBUG
#define MY_RADIO_NRF24
#define MY_REPEATER_FEATURE
#include "Arduino.h"
#include <MySensors.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_NeoPixel.h>
#include <TH02_dev.h>
void presentation() ;
void setup() ;
void loop() ;
void receive(const MyMessage &message) ;
void turnOff() 	;
void set_rgb(int r, int g, int b, int l) ;
void rainbowCycle(uint8_t wait) ;
uint32_t Wheel(byte WheelPos) ;

#include "MySensors_Veilleuse_Perrine.ino"


#endif
