/*
    Project     :   Smart Aquaphonics and Hydrophonics System (sahs)
    Version     :   0.0.1

    Created by  :   Rhalf Wendel Caacbay
    Email       :   rhalfcaacbay@gmail.com

*/
#include<U8g2lib.h>
#include<Timer.h>
#include<Terminal.h>
#include<Buzzer.h>

#include<SoftwareSerial.h>
#include<Button.h>
#include<Device.h>
#include<Helper.h>
#include<WatchDog.h>
#include<Timestamp.h>
#include<Apdu.h>

#include <EEPROM.h>
#include "GravityTDS.h"
#define TdsSensorPin A6
GravityTDS gravityTds;
float temperature = 28.71;

#include "DFRobot_PH.h"
#define PhSensorPin A7
float voltage;
DFRobot_PH ph;

U8G2_ST7920_128X64_1_SW_SPI u8g2(U8G2_R0, /* clock=*/ 8, /* data=*/ 7, /* CS=*/ 4, /* reset=*/ 100);
Timer tDisplay(Timer::MILLIS), tFastInterrupt(Timer::MILLIS), tSlowInterrupt(Timer::MILLIS), tCommunicate(Timer::MILLIS);
Terminal terminals[] = {Terminal(5), Terminal(6), Terminal(9), Terminal(10), Terminal(11)};
Buzzer buzzer = Buzzer(13, Buzzer::PASSIVE);
//BillCoinAcceptor coinAcceptor = BillCoinAcceptor(2, 1, false);

//Storage storage = Storage();

SoftwareSerial esp8266(A0, 12);

Button buttons[] = {Button(A1), Button(A2), Button(A3), Button(A4), Button(A5)};
Helper helper = Helper();

typedef struct Data {
  uint32_t ip = 0;
  uint32_t ts = 0;
  uint32_t tmp = 0;
  uint32_t tds = 0;
  uint32_t ph = 0;
  uint32_t ec = 0;
  uint32_t dio = 0;
  uint32_t sal = 0;
  uint32_t orp = 0;
  uint32_t out = 0;
} Data;

volatile Data data;

uint8_t index = 0;

void send(byte * data, uint8_t length) {
  for (uint8_t index = 0; index < length; index++) esp8266.write(data[index]);
}


void cbCommunicate() {
  static uint8_t counter = 0;
  counter++;
  uint8_t result = counter % 15;

  Apdu apdu1;

  switch (result) {
    case 0:
      //get ts
      apdu1.cClass = 0xA0;
      apdu1.cFunc = 0x01;
      break;
    case 1:
      //get ip
      apdu1.cClass = 0xA1;
      apdu1.cFunc = 0x01;
      break;
    //=======================================================
    case 2:
      //get tds
      apdu1.cClass = 0xD0;
      apdu1.cFunc = 0x02;
      apdu1.cValue1 = data.tds;
      break;
    case 3:
      //get ec
      apdu1.cClass = 0xD1;
      apdu1.cFunc = 0x02;
      apdu1.cValue1 = data.ec;
      break;
    case 4:
      //get oc
      apdu1.cClass = 0xD2;
      apdu1.cFunc = 0x02;
      apdu1.cValue1 = data.orp;
      break;
    case 5:
      //get do
      apdu1.cClass = 0xD3;
      apdu1.cFunc = 0x02;
      apdu1.cValue1 = data.dio;
      break;
    case 6:
      //get sal
      apdu1.cClass = 0xD4;
      apdu1.cFunc = 0x02;
      apdu1.cValue1 = data.sal;
      break;
    case 7:
      //get ph
      apdu1.cClass = 0xD5;
      apdu1.cFunc = 0x02;
      apdu1.cValue1 = data.ph;
      break;
    case 8:
      //get tmp
      apdu1.cClass = 0xD6;
      apdu1.cFunc = 0x02;
      apdu1.cValue1 = data.tmp;
      break;
    //=======================================================
    case 9:
      //get io0
      apdu1.cClass = 0xE1;
      apdu1.cFunc = 0x01;
      apdu1.cParam = 0x00;
      break;
    //*******************************************************
    case 10:
      //get io0
      apdu1.cClass = 0xE1;
      apdu1.cFunc = 0x02;
      apdu1.cParam = 0x00;
      apdu1.cValue1 = data.out;
      break;
      //=======================================================
  }

  apdu1.generate();
  send(apdu1.cRaw, Apdu::SIZE);

  if (esp8266.available()) {
    byte buffer[Apdu::SIZE];
    for (uint8_t index = 0; index < Apdu::SIZE; index++) buffer[index] = esp8266.read();

    Apdu apdu2(buffer);
    if (apdu2.checkSum() == true) {
      process(apdu2);
    } else {
      Serial.println("Wrong checksum!");
    }
  }

}

void process(Apdu apdu) {
  switch (apdu.cClass) {
    case 0xA0:
      if (apdu.cFunc == 0x01) {
        data.ts = apdu.cValue1;
      }
      break;
    case 0xA1:
      if (apdu.cFunc == 0x01) {
        data.ip = apdu.cValue1;
      }
      break;
    //    case 0xD0:
    //      if (apdu.cFunc == 0x02) {
    //        data.tds = apdu.cValue1;
    //      }
    //      break;
    //    case 0xD1:
    //      if (apdu.cFunc == 0x02) {
    //        data.ec = apdu.cValue1;
    //      }
    //      break;
    //    case 0xD2:
    //      if (apdu.cFunc == 0x02) {
    //        data.oc = apdu.cValue1;
    //      }
    //      break;
    //    case 0xD3:
    //      if (apdu.cFunc == 0x02) {
    //        data.dio = apdu.cValue1;
    //      }
    //      break;
    //    case 0xD4:
    //      if (apdu.cFunc == 0x02) {
    //        data.sal = apdu.cValue1;
    //      }
    //      break;
    //    case 0xD5:
    //      if (apdu.cFunc == 0x02) {
    //        data.ph = apdu.cValue1 = ;
    //      }
    //      break;
    //    case 0xD6:
    //      if (apdu.cFunc == 0x02) {
    //        data.tmp = apdu.cValue1;
    //      }
    //      break;
    case 0xE1:
      if (apdu.cFunc == 0x01) {
        if (apdu.cParam == 0x00) {
          //           Serial.println(apdu.toString());
          for (uint8_t bitIndex = 0; bitIndex < 5; bitIndex++) {
            if (bitRead(apdu.cValue1, bitIndex) == 1) {
              terminals[bitIndex].on();
              bitSet(data.out, bitIndex);
            }
            if (bitRead(apdu.cValue1, bitIndex) == 0) {
              terminals[bitIndex].off();
              bitClear(data.out, bitIndex);
            }
          }
        }
      }
      break;
  }
}

void cbDisplay() {
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_profont11_tr);
    uint16_t x = 8, y = 0;
    if (Timer::getSeconds() < 3) {

      u8g2.setCursor(0, 20);
      u8g2.print(F("Smart Aquaphonics and"));
      u8g2.setCursor(x, 30);
      u8g2.print(F("Hydrophonics System"));
      u8g2.setCursor( x + 40, 40);
      u8g2.print(F("0.0.1"));

      //} else if (Timer::getSeconds() >= 3 && Timer::getSeconds() < 12) {
    } else if (Timer::getSeconds() >= 3) {

      x = 0;
      u8g2.setCursor(x, 8);
      Timestamp ts(data.ts, 8);
      u8g2.print(ts.toString());

      u8g2.drawHLine(x, 10, 128);
      //=============================================================
      x = 0;
      u8g2.setCursor( x , 20);
      u8g2.print(F("TMP:"));
      u8g2.print(String(data.tmp / 100));

      u8g2.setCursor(x, 30);
      u8g2.print(F("TDS:"));
      u8g2.print(String((double)data.tds / 100));

      u8g2.setCursor(x, 40);
      u8g2.print(F("PH :"));
      u8g2.print(String((double)data.ph / 100));

      u8g2.setCursor(x, 50);
      u8g2.print(F("EC :"));
      u8g2.print(String((double)data.ec / 100));

      //=============================================================
      x += 64;
      u8g2.setCursor( x , 20);
      u8g2.print(F("DO :"));
      u8g2.print(String((double)data.dio / 100));

      u8g2.setCursor(x, 30);
      u8g2.print(F("SAL:"));
      u8g2.print(String((double)data.sal / 100));

      u8g2.setCursor(x, 40);
      u8g2.print(F("ORP:"));
      u8g2.print(String((double)data.orp / 100));

      u8g2.setCursor(x, 50);
      u8g2.print(F("OUT:"));
      u8g2.print(String(bitRead(data.out, 0)) + String(bitRead(data.out, 1) + String(bitRead(data.out, 2)) + String(bitRead(data.out, 3)) + String(bitRead(data.out, 4))));
      x -= 64;
      //=============================================================

      u8g2.drawHLine(0, 52, 128);
      uint32_t ipn = data.ip;

      String builder;
      builder += String((ipn >> 24) & 0xFF, DEC);
      builder += ".";
      builder += String((ipn >> 16) & 0xFF, DEC);
      builder += ".";
      builder += String((ipn >> 8) & 0xFF, DEC);
      builder += ".";
      builder += String((ipn >> 0) & 0xFF, DEC);

      //#define LCDWidth
      //#define ALIGN_CENTER(t)                 ((LCDWidth - (u8g2.getUTF8Width(t))) / 2)
      //#define ALIGN_RIGHT(t)                  (LCDWidth -  u8g2.getUTF8Width(t))
      //#define ALIGN_LEFT                      0
      //if (data.ip == 0 ) builder = ("XXX.XXX.XXX.XXX");
      u8g2.setCursor(0, 62);
      u8g2.print(builder);
    }
  } while ( u8g2.nextPage() );
}

void cbFastInterrupt() {
  for (index = 0; index < 5; index++) {
    //Buttons
    buttons[index].run();
    //Terminals
    terminals[index].run();
  }
}

void cbSlowInterrupt() {
  //  uint8_t static counter = 0;
  //  uint8_t result = (counter % 5);
  //  if (terminals[result].getPinState()) {
  //    terminals[result].off();
  //  }  else {
  //    terminals[result].on();
  //  }
  //  counter++;

  //randomSeed(analogRead(0));
  //uint32_t randData = random(0x8FFFFFFF);
  //data.ip = randData;
  //data.dio = randData;

  //temperature = readTemperature();  //add your temperature sensor and read it
  gravityTds.setTemperature(temperature);  // set the temperature and execute temperature compensation
  gravityTds.update();  //sample and calculate


  voltage = analogRead(PhSensorPin) / 1024.0 * 5000; // read the voltage


  data.ts += 1;
  data.tds = gravityTds.getTdsValue() * 100;
  data.ph = ph.readPH(voltage, temperature) * 100;
  data.tmp = temperature * 100;
  data.orp = 0;
  data.ec = 0;
  data.dio = 0;
  data.sal = 0;

}


void onShortPressed(uint8_t pin) {
  //Process

}

void onLongPressed(uint8_t pin) {
  for (index = 0; index < 5; index++) {
    if (buttons[index].getPin() == pin) {
      buzzer.play();
      terminals[index].reset();
    }
  }
}

void setup() {
  // put your setup code here, to run once:
  buzzer.play();

  gravityTds.setPin(TdsSensorPin);
  gravityTds.setAref(5.0);  //reference voltage on ADC, default 5.0V on Arduino UNO
  gravityTds.setAdcRange(1024);  //1024 for 10bit ADC;4096 for 12bit ADC
  gravityTds.begin();  //initialization

  ph.begin();

  Serial.begin(9600);
  esp8266.begin(9600);
  //protocol.onReceived(onReceived);
  //protocol.begin(9600);

  u8g2.begin();
  u8g2.enableUTF8Print();

  tDisplay.begin(Timer::FOREVER, 1000, cbDisplay);
  tFastInterrupt.begin(Timer::FOREVER, 25, cbFastInterrupt);
  tSlowInterrupt.begin(Timer::FOREVER, 1000, cbSlowInterrupt);
  tCommunicate.begin(Timer::FOREVER, 1000, cbCommunicate);

  tDisplay.start();
  tFastInterrupt.start();
  tSlowInterrupt.start();
  tCommunicate.start();

  for (index = 0; index < 5; index++) {
    //terminals[index].setActiveState(false);
    //buttons[index].setActiveState(false);
    buttons[index].setOnShortPressed(onShortPressed);
    buttons[index].setOnLongPressed(onLongPressed);
  }

  WatchDog::enable(WatchDog::S002);
}

void loop() {
  // put your main code here, to run repeatedly:
  tDisplay.run();
  tFastInterrupt.run();
  tSlowInterrupt.run();
  tCommunicate.run();

  WatchDog::reset();
}
