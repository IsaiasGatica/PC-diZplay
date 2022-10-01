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


enum Menus {

  Gputemp,
  Gpuload,
  Gpufan,
  Cputemp,
  Cpuload,
  RAM

} Menus;

uint8_t Pantalla=Gputemp;
uint8_t Pantalla0;
uint8_t GputempSerial;
 

void setup()
{
  tft.init();
  tft.setRotation(3);
  tft.fillScreen(TFT_BLACK);
  tft.setFreeFont(&Orbitron_Medium_27);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(1);
  tft.drawString("GPU:", 0, 25);

  Serial.begin(115200);


  if (!apds.begin())
  {
    Serial.println("failed to initialize device! Please check your wiring.");
  }
  else
    Serial.println("Device initialized!");

  // gesture mode will be entered once proximity mode senses something close
  apds.enableProximity(true);
  apds.enableGesture(true);
}

void drawtext(const char *text, int posx, int posy)
{
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawString(text, posx, posy);

}

void drawvalor(uint8_t valor){

  tft.setTextSize(1);
  tft.drawString(String(valor), 70, 20,7);

}



void CambioPantalla(){
   switch (Pantalla){

    case Gputemp:
      drawtext("GPU:", 0, 25);
      break;
    case Gpuload:
      drawtext("GPU:38%", 0, 25);
      break;
    case Cputemp:
      drawtext("CPU:", 0, 25);
      break;
    case Cpuload:
      drawtext("CPU:35%", 0, 25);
      break;
    case RAM:
      drawtext("RAM:35%", 0, 25);
      break;
    case Gpufan:
      drawtext("FAN:35%", 0, 25);
      break;
    default:

    break;

  }
}

void loop()
{
  // read a gesture from the device
  uint8_t gesture = apds.readGesture();

  Pantalla0=Pantalla;
  
  switch (gesture)
  {
    case APDS9960_UP:
      Serial.println("^");
      if (Pantalla==Gputemp){
        Pantalla = Gpuload;
      }
      else if (Pantalla==Cputemp){
        Pantalla = Cpuload;
      }
      else if (Pantalla==Gpufan){
        Pantalla = Gputemp;
      }
      break;
    
    case APDS9960_DOWN:
      Serial.println("v");

      if (Pantalla==Gpuload){
        Pantalla = Gputemp;
      }
      else if (Pantalla == Gputemp){
        Pantalla = Gpufan;
      }
      else if (Pantalla == Cpuload){
        Pantalla = Cputemp;
      }
      break;
    case APDS9960_RIGHT:
      Serial.println(">");
      if (Pantalla==Gputemp ||Pantalla== Gpuload){
        Pantalla = Cputemp;
      }
      else if (Pantalla==Cputemp || Pantalla== Cpuload){
        Pantalla = RAM;
      }
      else if (Pantalla==RAM){
        Pantalla = Gputemp;
      }
      break;
    case APDS9960_LEFT:
      Serial.println("<");
      if (Pantalla==Gputemp || Pantalla== Gpuload){
        Pantalla = RAM;
      }
      else if (Pantalla==Cputemp || Pantalla== Cpuload){
        Pantalla = Gputemp;
      }
      else if (Pantalla==RAM){
        Pantalla = Cputemp;
      }
      break;
    default:
      
      break;
  }

  if (Pantalla!=Pantalla0){
    CambioPantalla();
  }


  switch (Pantalla){

    case Gputemp:
      GputempSerial=45;
      drawvalor(GputempSerial);
      break;
    case Gpuload:
      break;
    case Cputemp:
      break;
    case Cpuload:
      break;
    case RAM:
      break;
    case Gpufan:
      
      break;
    default:

    break;

  }

   

}






