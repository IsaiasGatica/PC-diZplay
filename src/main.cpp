/*


  28/10/2022
  Funcionando con una implementacion inicial del filter JSON.
  Mas info en "WifiMonitorPC.h"

  27/10/2022
  Luego de intentar manipular el openhardwaremonitor.dll con visual studio y no
  tener buenos resultados, se intenta obtener los datos de la PC a traves del
  webserver de librehardwaremonitor. Se lograr en primer instancia, el problema
  que surgio a raiz de esto es la gran cantidad de memoria que ocupa el JSON.
  No siendo posible obtener estos datos y al mismo tiempo ejecutar el programa
  principal (Pantalla+gestos).

  09/10/2022
  Funcionando gestos y pantalla. Falta implementar la lectura de
  temperatura y su función correspondiente para visualizar en la pantalla.
  Se utilizó la siguiente libreria para el sensor de gestos:
  https://github.com/Dgemily/APDS-9960_Gesture_Sensor_esp8266_Library.git
  Importante: Se agregó un capacitor de 220uf en la alimentación.

*/

#include <Arduino.h>
#include <Wire.h>
#include <TFT_eSPI.h>
#include <ST7735Config.h>
#include <APDSConfig.h>
#include <WiFiMonitorPC.h>

void IRAM_ATTR interruptRoutine()
{
  isr_flag = 1;
}

void setup()
{

  Serial.begin(115200);

  startupWifi();

  Wire.begin(APDS9960_SDA, APDS9960_SCL);

  startupST7735();
  startupAPDS();

  pinMode(digitalPinToInterrupt(APDS_INT), INPUT);
  attachInterrupt(digitalPinToInterrupt(APDS_INT), interruptRoutine, FALLING);
}

void gestos()
{
  if (apds.isGestureAvailable())
  {
    switch (apds.readGesture())
    {
    case DIR_UP:
      // Serial.println("^");
      if (Pantalla == Gputemp)
      {
        Pantalla = Gpuload;
      }
      else if (Pantalla == Cputemp)
      {
        Pantalla = Cpuload;
      }
      else if (Pantalla == Gpufan)
      {
        Pantalla = Gputemp;
      }
      break;

    case DIR_DOWN:
      // Serial.println("v");

      if (Pantalla == Gpuload)
      {
        Pantalla = Gputemp;
      }
      else if (Pantalla == Gputemp)
      {
        Pantalla = Gpufan;
      }
      else if (Pantalla == Cpuload)
      {
        Pantalla = Cputemp;
      }
      break;
    case DIR_RIGHT:
      // Serial.println(">");
      if (Pantalla == Gputemp || Pantalla == Gpuload)
      {
        Pantalla = Cputemp;
      }
      else if (Pantalla == Cputemp || Pantalla == Cpuload)
      {
        Pantalla = RAM;
      }
      else if (Pantalla == Reloj)
      {
        Pantalla = Gputemp;
      }
      break;
    case DIR_LEFT:
      // Serial.println("<");
      if (Pantalla == Gputemp || Pantalla == Gpuload)
      {
        Pantalla = Reloj;
      }
      else if (Pantalla == Cputemp || Pantalla == Cpuload)
      {
        Pantalla = Gputemp;
      }
      else if (Pantalla == RAM)
      {
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
  if (isr_flag == 1)
  {
    detachInterrupt(digitalPinToInterrupt(APDS_INT));
    gestos();
    CambioPantalla(Pantalla);
    isr_flag = 0;
    attachInterrupt(digitalPinToInterrupt(APDS_INT), interruptRoutine, FALLING);
  }

  drawvalor(getJSONdata(Pantalla));
}
