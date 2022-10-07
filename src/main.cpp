//!07/10/2022
//! Con la libreria de adafruit el sensor interrupte por proximidad cuando el sensor de gestos esta
//!deshabilitado. Se probó rapidamente una rutina de habilitar los gestos recien cuando se interrumpe pero
//!no se logro leer los gestos. Antes de seguir probando con esta liberia se va probar la de sparkfun.



#include <Arduino.h>
#include <Wire.h>
#include <TFT_eSPI.h>
#include <font.h>
#include <Org_01.h>
#include <7bit.h>
#include <Gpu3.h>
#include <Adafruit_APDS9960.h>



Adafruit_APDS9960 apds;

volatile int isr_flag = 0;

#define APDS9960_INT D6

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
 

void IRAM_ATTR isr() {
  isr_flag = 1;
}


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


  pinMode(APDS9960_INT,INPUT_PULLUP);



  attachInterrupt(APDS9960_INT, isr, FALLING);



  if (!apds.begin())
  {
    Serial.println("failed to initialize device! Please check your wiring.");
  }
  else
    Serial.println("Device initialized!");

  // gesture mode will be entered once proximity mode senses something close
  apds.enableProximity(true);
  
  //apds.setProximityInterruptThreshold(0, 175);
  apds.enableProximityInterrupt();
  
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

  if( isr_flag == 1 ) { 
     
    detachInterrupt(APDS9960_INT);        // Stop listening for interrupt
    apds.enableGesture(true);
    uint8_t gesture = apds.readGesture();
    Serial.println("INT");
    Serial.println(gesture);           // grab gesture
    isr_flag = 0;        // reset interrupt flag to zero
    attachInterrupt(APDS9960_INT, isr, FALLING);  // Start listening again
    apds.readProximity();
    apds.clearInterrupt();
    apds.enableGesture(false);
    
  }

  // read a gesture from the device
   
  // uint8_t gesture = apds.readGesture();

  // Pantalla0=Pantalla;
  
  // switch (gesture)
  // {
  //   case APDS9960_UP:
  //     Serial.println("^");
  //     if (Pantalla==Gputemp){
  //       Pantalla = Gpuload;
  //     }
  //     else if (Pantalla==Cputemp){
  //       Pantalla = Cpuload;
  //     }
  //     else if (Pantalla==Gpufan){
  //       Pantalla = Gputemp;
  //     }
  //     break;
    
  //   case APDS9960_DOWN:
  //     Serial.println("v");

  //     if (Pantalla==Gpuload){
  //       Pantalla = Gputemp;
  //     }
  //     else if (Pantalla == Gputemp){
  //       Pantalla = Gpufan;
  //     }
  //     else if (Pantalla == Cpuload){
  //       Pantalla = Cputemp;
  //     }
  //     break;
  //   case APDS9960_RIGHT:
  //     Serial.println(">");
  //     if (Pantalla==Gputemp ||Pantalla== Gpuload){
  //       Pantalla = Cputemp;
  //     }
  //     else if (Pantalla==Cputemp || Pantalla== Cpuload){
  //       Pantalla = RAM;
  //     }
  //     else if (Pantalla==RAM){
  //       Pantalla = Gputemp;
  //     }
  //     break;
  //   case APDS9960_LEFT:
  //     Serial.println("<");
  //     if (Pantalla==Gputemp || Pantalla== Gpuload){
  //       Pantalla = RAM;
  //     }
  //     else if (Pantalla==Cputemp || Pantalla== Cpuload){
  //       Pantalla = Gputemp;
  //     }
  //     else if (Pantalla==RAM){
  //       Pantalla = Cputemp;
  //     }
  //     break;
  //   default:
      
  //     break;
  // }

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






