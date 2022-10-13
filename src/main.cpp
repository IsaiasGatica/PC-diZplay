/**
* 09/10/2022
* Funcionando gestos y pantalla. Falta implementar la lectura de 
* temperatura y su función correspondiente para visualizar en la pantalla.
* Se utilizó la siguiente libreria para el sensor de gestos:
* https://github.com/Dgemily/APDS-9960_Gesture_Sensor_esp8266_Library.git
* Importante: Se agregó un capacitor de 220uf en la alimentación.

*/

  

#include <Arduino.h>  
#include <Wire.h>
#include <TFT_eSPI.h>
#include <ST7735Config.h>
#include <APDSConfig.h>


void IRAM_ATTR interruptRoutine() {
  isr_flag = 1;
}

void setup()
{
  
  Wire.begin(APDS9960_SDA,APDS9960_SCL);

  startupST7735();
  startupAPDS();

  pinMode(digitalPinToInterrupt(APDS_INT), INPUT);
  attachInterrupt(digitalPinToInterrupt(APDS_INT), interruptRoutine, FALLING);

  Serial.begin(115200);
}

void drawtext(const char *text, int posx, int posy)
{
  img.fillScreen(TFT_BLACK);
  img.setTextColor(TFT_WHITE, TFT_BLACK);
  img.drawString(text, posx, posy);
  img.pushSprite(0,0);


}

void drawvalor(uint8_t valor){
  
  img2.fillScreen(TFT_BLACK);
  img2.setTextSize(1);
  img2.drawString(String(valor), 0, 20,7);
  img2.pushSprite(80,0);


}

void CambioPantalla(){
   switch (Pantalla){

    case Gputemp:
      drawtext("GPU:", 0, 25);
      break;
    case Gpuload:
      drawtext("GPU:", 0, 25);
      break;
    case Cputemp:
      drawtext("CPU:", 0, 25);
      break;
    case Cpuload:
      drawtext("CPU:", 0, 25);
      break;
    case RAM:
      drawtext("RAM:", 0, 25);
      break;
    case Gpufan:
      drawtext("FAN:", 0, 25);
      break;
    default:

    break;

  }
}


void gestos(){
  if ( apds.isGestureAvailable() ) 
  {  
    switch (apds.readGesture())
      {
        case DIR_UP:
          // Serial.println("^");
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
        
        case DIR_DOWN:
          // Serial.println("v");

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
        case DIR_RIGHT:
          // Serial.println(">");
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
        case DIR_LEFT:
          // Serial.println("<");
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
  }
}
void loop()
{
if( isr_flag == 1 ) {
    detachInterrupt(digitalPinToInterrupt(APDS_INT));
    gestos();
    CambioPantalla();
    isr_flag = 0;
    attachInterrupt(digitalPinToInterrupt(APDS_INT), interruptRoutine, FALLING);
  }


if (Serial.available()>0){
    GputempSerial= Serial.parseInt();
    
  }
  
    drawvalor(GputempSerial);



}






