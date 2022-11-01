/*
   Configuraciones principales y funciones relacionadas al ST7735

   -startupST7735(): Configuraciones iniciales y pantalla por default
   -drawtext y drawvalor : Se encargan de dibujar los textos y numeros respectivamente.
   -CambioPantalla(Pantalla) : Cambiar el valor del texto dependiendo del gesto realizado.

*/

#include <TFT_eSPI.h>
#include <Fuentes/font.h>
#include <Fuentes/Org_01.h>
#include <Fuentes/7bit.h>
#include <Fuentes/Gpu3.h>

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite img = TFT_eSprite(&tft);
TFT_eSprite img2 = TFT_eSprite(&tft);

enum Menus
{
  Gputemp,
  Gpuload,
  Gpufan,
  Cputemp,
  Cpuload,
  RAM

} Menus;

uint8_t Pantalla = Gputemp;
uint8_t Pantalla0;
uint8_t GputempSerial;

void startupST7735()
{
  tft.init();
  tft.setRotation(3);

  img.createSprite(80, 80);
  img.fillScreen(TFT_BLACK);
  img.setFreeFont(&Orbitron_Medium_27);
  img.setTextColor(TFT_WHITE, TFT_BLACK);
  img.setTextSize(1);
  img.drawString("GPU:", 0, 25);
  img.pushSprite(0, 0);

  img2.createSprite(80, 80); // Sprite para el valor númerico.
  img2.fillScreen(TFT_BLACK);
  img2.pushSprite(80, 0);
}

void drawtext(const char *text, int posx, int posy)
{
  img.fillScreen(TFT_BLACK);
  img.setTextColor(TFT_WHITE, TFT_BLACK);
  img.drawString(text, posx, posy);
  img.pushSprite(0, 0);
}

void drawvalor(String valor)
{

  img2.fillScreen(TFT_BLACK);
  img2.setTextSize(1);
  img2.drawString(valor, 0, 20, 7);
  img2.pushSprite(80, 0);
}

void CambioPantalla(uint8_t Pantalla)
{
  switch (Pantalla)
  {

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