#ifndef _LIBRARY_AND_PIN_H_
#define _LIBRARY_AND_PIN_H_
// 라이브러리 선언

#include <Wire.h>
#include <SPI.h>
#include <Esp.h>
#include <Arduino.h>

#include <HAS2_Wifi.h>

#include <Adafruit_NeoPixel.h>
#include <Nextion.h>
#include <Adafruit_PN532.h>

#include <SimpleTimer.h>
#include <esp_task_wdt.h>

#include <SecureOTA.h>
#include "secrets.h"


// 핀 선언

// #define SERIAL1_RX_PIN 36 // 미사용
// #define SERIAL1_TX_PIN 32

#define SERIAL2_RX_PIN 39   // 디스플레이
#define SERIAL2_TX_PIN 33

#define NEOPIXEL_PIN_SQUARE 25      // [15구] 네오픽셀 3개중 1개 
#define NEOPIXEL_PIN_ROUND  26      // [24구] 네오픽셀 3개중 1개  
#define NEOPIXEL_PIN_SIDE   27      // [96구] 네오픽셀 3개중 1개 

#define PN532_SCK                       (18)
#define PN532_MISO                      (19)
#define PN532_MOSI                      (23)
#define PN532_SS                        (5) 
#endif