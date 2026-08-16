#include "DisplayManager.h"
#include "Theme.h"
#include "Config.h"


void DisplayManager::begin()
{
    tft.init();

    tft.setRotation(DISPLAY_ROTATION);

    tft.invertDisplay(DISPLAY_INVERT);

    tft.setTextDatum(TL_DATUM);

    tft.fillScreen(COLOR_BACKGROUND);
}
void DisplayManager::drawHeader()
{
    tft.fillRect(
        0,
        HEADER_Y,
        SCREEN_WIDTH,
        HEADER_HEIGHT,
        COLOR_HEADER);

    tft.setTextColor(COLOR_HEADER_TEXT, COLOR_HEADER);

    tft.setTextFont(4);

    tft.drawString("Wi-Fi Scanner", 10, 8);

    tft.setTextFont(2);

    tft.drawRightString(
        "DigitalLab",
        SCREEN_WIDTH - 5,
        12,
        2);
}

void DisplayManager::drawColumnHeader()
{
    tft.fillRect(
        0,
        COLUMN_Y,
        SCREEN_WIDTH,
        COLUMN_HEIGHT,
        COLOR_COLUMN_BG);

    tft.setTextColor(
        COLOR_COLUMN_TEXT,
        COLOR_COLUMN_BG);

    tft.setTextFont(2);

    tft.drawString("SSID",5,41);

    tft.drawString("RSSI",165,41);

    tft.drawString("CH",225,41);

    tft.drawString("SEC",270,41);
}

void DisplayManager::drawFooter(int networkCount)
{
    tft.fillRect(
        0,
        FOOTER_Y,
        SCREEN_WIDTH,
        FOOTER_HEIGHT,
        COLOR_FOOTER);

    tft.setTextColor(
        COLOR_FOOTER_TEXT,
        COLOR_FOOTER);

    tft.setTextFont(2);

   tft.setTextDatum(TL_DATUM);
   tft.drawString("Networks: " + String(networkCount), 5, FOOTER_Y + 5);

   tft.setTextDatum(TR_DATUM);
   tft.drawString("Scanning...", SCREEN_WIDTH - 5, FOOTER_Y + 5);

   tft.setTextDatum(TL_DATUM);
}



void DisplayManager::clearNetworkArea()
{
    tft.fillRect(
        0,
        LIST_Y,
        SCREEN_WIDTH,
        FOOTER_Y - LIST_Y,
        COLOR_BACKGROUND);
}

void DisplayManager::drawNetwork(
        int row,
        String ssid,
        int rssi,
        int channel,
        String security)
{
    int y = LIST_Y + (row * 18);
    
    uint16_t signalColor;
    
    if(rssi > -50)
        signalColor = COLOR_SIGNAL_STRONG;
    else if(rssi > -70)
        signalColor = COLOR_SIGNAL_MEDIUM;
    else
        signalColor = COLOR_SIGNAL_WEAK;
    if (ssid.length() > 15)
    {
        ssid = ssid.substring(0, 10) + "...";
    }    
    
    tft.setTextFont(2);

    tft.setTextColor(COLOR_TEXT, COLOR_BACKGROUND);

    //tft.drawString(ssid,5,y);

    tft.setTextColor(signalColor, COLOR_BACKGROUND);

    //tft.drawRightString(String(rssi),205,y,2);

    tft.setTextColor(COLOR_TEXT, COLOR_BACKGROUND);

    //tft.drawRightString(String(channel),255,y,2);

    //tft.drawString(security,270,y);
    tft.setTextDatum(TL_DATUM);
    
    tft.drawString(ssid, 5, y);

    tft.setTextColor(signalColor, COLOR_BACKGROUND);
    tft.drawString(String(rssi), 165, y);
    
    tft.setTextColor(COLOR_TEXT, COLOR_BACKGROUND);
    tft.drawString(String(channel), 225, y);

    tft.drawString(security, 270, y);
    
}
void DisplayManager::drawDashboard()
{
    tft.fillScreen(COLOR_BACKGROUND);

    // Header
    tft.fillRect(0, HEADER_Y, SCREEN_WIDTH, HEADER_HEIGHT, COLOR_HEADER);

    tft.setTextColor(COLOR_HEADER_TEXT, COLOR_HEADER);
    tft.setTextFont(4);

    tft.setTextDatum(TL_DATUM);
    tft.drawString("Wi-Fi Scanner", 10, 8);

    tft.setTextFont(2);
    tft.setTextDatum(TR_DATUM);
    tft.drawString("DigitalLab", SCREEN_WIDTH - 5, 12);

    // Restore default
    tft.setTextDatum(TL_DATUM);

    drawColumnHeader();
    drawFooter(0);
}
void DisplayManager::refreshSPI()
{
    tft.startWrite();
    tft.endWrite();
}