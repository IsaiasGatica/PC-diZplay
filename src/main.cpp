#include <Arduino.h>
#include <Wire.h>
#include <TFT_eSPI.h>
#include <font.h>
#include <Org_01.h>
#include <7bit.h>
#include <Gpu3.h>
#include <Adafruit_APDS9960.h>


Adafruit_APDS9960 apds;


TFT_eSPI tft = TFT_eSPI();


void setup(){
  tft.init();
  tft.setRotation(3);
  tft.fillScreen(TFT_BLACK);
  tft.setFreeFont(&Orbitron_Medium_27);
  tft.setTextColor(TFT_WHITE,TFT_BLACK);
  tft.setTextSize(1);
  tft.drawString("GPU: 48C",0,25);

  // tft.setSwapBytes(true);
  // tft.pushImage(10,5,64,64,Gpu3);


  delay(500);
  // tft.drawString("48",85,15,7);

  Serial.begin(115200);
  
  if(!apds.begin()){
    Serial.println("failed to initialize device! Please check your wiring.");
  }
  else Serial.println("Device initialized!");

  //gesture mode will be entered once proximity mode senses something close
  apds.enableProximity(true);
  apds.enableGesture(true);
}







void loop(){

  //read a gesture from the device
    uint8_t gesture = apds.readGesture();
    if(gesture == APDS9960_DOWN) Serial.println("v");
    if(gesture == APDS9960_UP){
      Serial.println("^");
      tft.setTextColor(TFT_WHITE,TFT_BLACK);
      tft.drawString("LOD: 35%",0,25);
    } 
    if(gesture == APDS9960_LEFT){
      tft.fillScreen(TFT_BLACK);

      Serial.println("<");
      tft.setTextColor(TFT_WHITE,TFT_BLACK);
      tft.drawString("GPU: 48C",0,25);
    } 
    if(gesture == APDS9960_RIGHT) {
      tft.fillScreen(TFT_BLACK);

      Serial.println(">");
      tft.setTextColor(TFT_WHITE,TFT_BLACK);
      tft.drawString("CPU: 36C",0,25);
    }


}

