/**
 * @file temple.ino
 * @author YuBin Kim
 * @brief 
 * @version 0.1
 * @date 2022-11-24 ~ 2022-11-26
 * 
 * @copyright Copyright (c) 2022
 */

#include "temple.h"

//************************************************ Core1 ********************************************************************
/**
 * @brief Temple Intialize
 */
void TempleInit()
{
  // has2wifi.Setup("KT_GiGA_6C64", "ed46zx1198");                     // 와이파이 세팅
  has2wifi.Setup();
  nexInit();                                                         // 디스플레이 세팅
  MySerial2.begin(9600, SERIAL_8N1, SERIAL2_RX_PIN, SERIAL2_TX_PIN); // 디스플레이 세팅
  SensorInit();                                                      // IoT Glove 사용 센서, 모듈 세팅    
  TimerInit();                                                  // 타이머 세팅
}

/**
 * @brief 아두이노 기본 문법 (전원이 켜지면 한번만 실행)
 */
void setup()
{
  delay(1000);
  Serial.begin(115200);
  TempleInit();
  DataChange();
}

/**
 * @brief 아두이노 기본 문법 (전원이 켜져있는동안 Core1에서 계속 실행)
 */
void loop()
{
  TimerRun();
  NeoFunc();
  if(activate_bool){ ActivateFunc(); }
}