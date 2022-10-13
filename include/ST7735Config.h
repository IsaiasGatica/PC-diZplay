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

    img.createSprite(80,80);
    img.fillScreen(TFT_BLACK);
    img.setFreeFont(&Orbitron_Medium_27);
    img.setTextColor(TFT_WHITE, TFT_BLACK);
    img.setTextSize(1);
    img.drawString("GPU:", 0, 25);
    img.pushSprite(0,0);


    img2.createSprite(80,80); //Sprite para el valor númerico.

}