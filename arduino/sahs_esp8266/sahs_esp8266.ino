#include<Apdu.h>
#include<SoftwareSerial.h>
#include<Timer.h>
#include <NTPClient.h>
#include "FirebaseESP8266.h"
#include <ESP8266WiFi.h>
#include "WiFiUdp.h"

Timer tWifi(Timer::MILLIS);
Timer tCommunicate(Timer::MILLIS);
Timer tDatabaseRead(Timer::SECONDS);
Timer tDatabaseWrite(Timer::SECONDS);


SoftwareSerial ss(D1, D0);

//1. Change the following info
#define FIREBASE_HOST "sahs-9d452.firebaseio.com"
#define FIREBASE_AUTH "doRgWsaDwpPW9CnnGhWs2I72bDkq1ExQMva2soWs"
#define WIFI_SSID "caacbay.2g"
#define WIFI_PASSWORD "XbF3HTxD"

//2. Define FirebaseESP8266 data object for data sending and receiving
FirebaseData fbData;

//database
//uint32_t devices_index = 1;
//uint32_t id = 0;
//uint32_t dt = 0;

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

} Data;

volatile Data data;
//uint32_t sensor_do = 0;
//uint32_t sensor_ec = 0;
//uint32_t sensor_ph = 0;
//uint32_t sensor_sa = 0;
//uint32_t sensor_td = 0;

String db_root = "devices";
String db_id = "0";
String db_datas = "datas";
//

typedef struct Io {
  uint8_t enabled;
  uint8_t state;
  uint32_t interval;
  //  uint32_t on[2];
} Io;

const uint8_t IO_LENGTH = 5;
Io io[IO_LENGTH];

const long utcOffsetInSeconds = 0;
//char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);



void cbWifi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.println(WiFi.localIP());
  timeClient.begin();
}

void cbCommunicate() {
  timeClient.update();
  data.ip = Apdu::swapByte(WiFi.localIP());
  data.ts = timeClient.getEpochTime();

  //  Serial.print(daysOfTheWeek[timeClient.getDay()]);
  //  Serial.print(", ");
  //  Serial.print(timeClient.getHours());
  //  Serial.print(":");
  //  Serial.print(timeClient.getMinutes());
  //  Serial.print(":");
  //  Serial.println(timeClient.getSeconds());
  //  Serial.println(timeClient.getFormattedTime());
  //  Serial.println(timeClient.getEpochTime());

  if (ss.available()) {
    byte buffer[Apdu::SIZE];
    for (uint8_t index = 0; index < Apdu::SIZE; index++) buffer[index] = ss.read();

    Apdu apdu(buffer);
    if (apdu.checkSum()) process(apdu);
  }
}

void send(byte * data, uint8_t length) {
  for (uint8_t index = 0; index < length; index++) ss.write(data[index]);
}

void process(Apdu apdu) {

  switch (apdu.cRaw[0]) {
    case 0xA0:
      if (apdu.cRaw[1] == 0x01) {
        apdu.cValue1 = data.ts;
      }
      break;
    case 0xA1:
      if (apdu.cRaw[1] == 0x01) {
        apdu.cValue1 = data.ip;
      }
      break;
    case 0xD0:
      if (apdu.cRaw[1] == 0x02) {
        data.tds = apdu.cValue1;
      }
      break;
    case 0xD1:
      if (apdu.cRaw[1] == 0x02) {
        data.ec = apdu.cValue1;
      }
      break;
    case 0xD2:
      if (apdu.cRaw[1] == 0x02) {
        data.orp = apdu.cValue1;
      }
      break;
    case 0xD3:
      if (apdu.cRaw[1] == 0x02) {
        data.dio = apdu.cValue1;
      }
      break;
    case 0xD4:
      if (apdu.cRaw[1] == 0x02) {
        data.sal = apdu.cValue1;
      }
      break;
    case 0xD5:
      if (apdu.cRaw[1] == 0x02) {
        data.ph = apdu.cValue1;
      }
      break;
    case 0xD6:
      if (apdu.cRaw[1] == 0x02) {
        data.tmp = apdu.cValue1;
      }
      break;

    case 0xE1:
      //read enabled
      if (apdu.cFunc == 0x01) {
        if (apdu.cParam == 0x00) {
          for (uint8_t index = 0; index < IO_LENGTH; index++) {
            if (io[index].enabled == 1) {
              bitSet(apdu.cValue1, index);
            }
            if (io[index].enabled == 0) {
              bitClear(apdu.cValue1, index);
            }
          }
        }
      }
      //=================================
      if (apdu.cFunc == 0x02) {
        if (apdu.cParam == 0x00) {
          for (uint8_t index = 0; index < IO_LENGTH; index++) {
            if (bitRead(apdu.cValue1, index) == 1) {
              io[index].state = 1;
            }
            if (bitRead(apdu.cValue1, index) == 0) {
               io[index].state = 0;
            }
          }
        }
      }
      break;
  }
  apdu.generate();
  send(apdu.cRaw, Apdu::SIZE);
}

void cbDatabaseWrite() {
  digitalWrite(LED_BUILTIN, HIGH);
  //Communicating with database
  Serial.println("cbDatabaseWrite started");
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
  //===================================================================
  FirebaseJson json1;
  json1.set("ts", (int)data.ts);
  json1.set("te", (int)data.tmp);
  json1.set("do", (int)data.dio);
  json1.set("ec", (int)data.ec);
  json1.set("td", (int)data.tds);
  json1.set("or", (int)data.orp);
  json1.set("sa", (int)data.sal);
  json1.set("ph", (int)data.ph);

  if (Firebase.pushJSON(fbData, "/" + db_root + "/" + db_id + "/" + db_datas + "/", json1)) {
    //    Serial.println(fbData.dataPath());
    //    Serial.println(fbData.pushName());
    //    Serial.println(fbData.dataPath() + "/" + fbData.pushName());
  } else {
    Serial.println(fbData.errorReason());
  }

  for (uint8_t index = 0; index < IO_LENGTH; index++) {
    if (Firebase.setInt(fbData, "/" + db_root + "/" + db_id + "/io/" + String(index) + "/state", io[index].state)) {

    } else {
      Serial.println(fbData.errorReason());
    }
    delay(100);
  }

   digitalWrite(LED_BUILTIN, LOW);
}

void cbDatabaseRead() {
   digitalWrite(LED_BUILTIN, HIGH);
  //Communicating with database
  Serial.println("cbDatabaseRead started");
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
  //===================================================================
  for (uint8_t index = 0; index < IO_LENGTH; index++) {
    if (Firebase.getJSON(fbData, "/" + db_root + "/" + db_id + "/io/" + String(index))) {
      //Success
      //Serial.print("Get variant data success, type = ");
      //Serial.println(fbData.dataType());
      if (fbData.dataType() == "json") {
        FirebaseJson &fj = fbData.jsonObject();
        FirebaseJsonData fjd;

        fj.get(fjd, "enabled");
        io[index].enabled = fjd.intValue;
        //        fj.get(fjd, "state");
        //        io[index].state = fjd.intValue;
        fj.get(fjd, "interval");
        io[index].interval = fjd.intValue;

        Serial.print("io:");
        Serial.print(index);
        Serial.print(",enabled:");
        Serial.print(io[index].enabled);
        Serial.print(",state:");
        Serial.print(io[index].state);
        Serial.print(",interval:");
        Serial.print(io[index].interval);
        Serial.println();
        //        fj.get(fjd, "on");
        //        FirebaseJsonArray arr;
        //        fjd.getArray(arr);
        //
        //
        //        for (size_t i = 0; i < arr.size(); i++) {
        //          arr.get(fjd, "[" + String(i) + "]" + "/start");
        //          if (fjd.success) io[index].on[i] = fjd.intValue;
        //        }
      }
    } else {
      //Failed?, get the error reason from fbData
      Serial.print("Error in get, ");
      Serial.println(fbData.errorReason());
    }
  }
   digitalWrite(LED_BUILTIN, LOW);
}



void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  
  tWifi.begin(Timer::ONCE, 1000, cbWifi);

  tCommunicate.begin(Timer::FOREVER, 1000, cbCommunicate);
  tDatabaseRead.begin(Timer::FOREVER, 5, cbDatabaseRead);
  tDatabaseWrite.begin(Timer::FOREVER, 10, cbDatabaseWrite);

  tWifi.start();
  tCommunicate.start();
  tDatabaseRead.start();
  tDatabaseWrite.start();

  Serial.begin(9600);
  ss.begin(9600);
  randomSeed(analogRead(A0));

  
}

void loop() {
  tWifi.run();
  tCommunicate.run();
  tDatabaseRead.run();
  tDatabaseWrite.run();
}
