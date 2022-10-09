#include <TFT_eSPI.h>
#include <Fuentes/font.h>
#include <Fuentes/Org_01.h>
#include <Fuentes/7bit.h>
#include <Fuentes/Gpu3.h>

TFT_eSPI tft = TFT_eSPI();

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
    tft.fillScreen(TFT_BLACK);
    tft.setFreeFont(&Orbitron_Medium_27);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextSize(1);
    tft.drawString("GPU:", 0, 25);
}