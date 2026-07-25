/*
  ------------------------------------------------------------
  Project : ESP32 Portable Wi-Fi Scanner
  Episode : 2 - Display Framework Basic
  Author  : DigitalLab.org
  ------------------------------------------------------------
*/
#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();

void setup()
{
    tft.init();
    tft.invertDisplay(false);   // Try false first
    tft.setRotation(3);

    tft.fillScreen(TFT_BLACK);

    tft.setTextColor(TFT_WHITE);

    tft.setTextSize(2);

    tft.drawString("Hello DigitalLab!", 30, 20);

    tft.drawLine(20, 60, 300, 60, TFT_GREEN);

    tft.drawRect(20, 80, 120, 60, TFT_RED);

    tft.fillCircle(220, 110, 30, TFT_BLUE);

    tft.fillRoundRect(20, 170, 120, 40, 8, TFT_YELLOW);

    tft.setTextColor(TFT_BLACK);

    tft.drawCentreString("OK", 80, 182, 2);
}

void loop()
{
}