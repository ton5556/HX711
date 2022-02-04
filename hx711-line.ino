void Line_Notify1(String message1) ;
#include "HX711.h"
#include <WiFiClientSecure.h>
#include <ESP8266WiFi.h>
#include <TridentTD_LineNotify.h>

#define WIFI_SSID ""        // ใส่ชื่อ Wifi
#define WIFI_PASSWORD ""     // ใส่รหัส Wifi
#define LINE_TOKEN_PIR ""
#define calibration_factor 4390.0 //This value is obtained using the SparkFun_HX711_Calibration sketch
#define zero_factor 18230 //This large value is obtained using the SparkFun_HX711_Calibration sketch
#define LED 17

#define DOUT  D5
#define CLK  D6

HX711 scale;
String message1 = "";//พี่ใส่ข้อความที่จะให้เเจ้งเตือนในไลน์ตรงนี้้้้้นะ

void setup() {
  Serial.begin(9600);
  Serial.println("Demo of zeroing out a scale from a known value");
  scale.set_scale(calibration_factor); //This value is obtained by using the SparkFun_HX711_Calibration sketch
  scale.set_offset(zero_factor); //Zero out the scale using a previously known zero_factor
  Serial.println("Readings:");
  Serial.println("connecting");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.print("connecting");
  while(WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(500);
    pinMode(LED, OUTPUT);
  }
}
void loop(){
  Serial.print("Reading: ");
  Serial.print(scale.get_units(), 1); //scale.get_units() returns a float
  Serial.print(" kg"); //You can change to kg but you'll need to change the calibration_factor
  Serial.println();
  if ((scale.get_units() > 1) && (WiFi.status() == WL_CONNECTED)){
    while(scale.get_units() > 1);
    Serial.println("เขื่อมต่อเเล้ว");
    digitalWrite(LED, HIGH);
    Line_Notify1(message1);
  }
}
void Line_Notify1(String message) {
  WiFiClientSecure client;
  client.setInsecure();
  if (!client.connect("notify-api.line.me", 443)) {
    Serial.println("connection failed");
    delay(200);
    return;
  }
//} 
  String req = "";
  req += "POST /api/notify HTTP/1.1\r\n";
  req += "Host: notify-api.line.me\r\n";
  req += "Authorization: Bearer " + String(LINE_TOKEN_PIR) + "\r\n";
  req += "Cache-Control: no-cache\r\n";
  req += "User-Agent: ESP8266\r\n";
  req += "Content-Type: application/x-www-form-urlencoded\r\n";
  req += "Content-Length: " + String(String("message=" + message1).length()) + "\r\n";
  req += "\r\n";
  req += "message=" + message1;
  client.print(req);
  delay(20);
  
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      break;
    }
  }
}
  
