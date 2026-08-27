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
void DisplayManager::drawHeader(String mode)
{
    tft.fillRect(
        0,
        HEADER_Y,
        SCREEN_WIDTH,
        HEADER_HEIGHT,
        COLOR_HEADER);

    tft.setTextColor(COLOR_HEADER_TEXT, COLOR_HEADER);

    tft.setTextFont(4);

    tft.drawString("Network Scanner", 10, 8);

    tft.setTextFont(2);

    tft.drawRightString(
        mode +" DigitalLab",
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
    tft.setTextDatum(TR_DATUM);
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
    tft.drawString("Network Scanner", 10, 8);

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
// BLE Methods
void DisplayManager::clearBLEArea()
{
    tft.fillRect(
        0,
        LIST_Y,
        SCREEN_WIDTH,
        FOOTER_Y - LIST_Y,
        COLOR_BACKGROUND
    );
}
void DisplayManager::drawBLEHeader()
{
    // tft.fillRect(
    //     0,
    //     LIST_Y,
    //     SCREEN_WIDTH,
    //     20,
    //     COLOR_COLUMN_BG
    // );

    // tft.setTextColor(
    //     COLOR_COLUMN_TEXT,
    //     COLOR_COLUMN_BG
    // );

    // tft.setTextDatum(TL_DATUM);

    // tft.drawString(
    //     "BLE DEVICE",
    //     5,
    //     LIST_Y ,
    //     2
    // );

    // tft.setTextDatum(TR_DATUM);

    // tft.drawString(
    //     "RSSI",
    //     SCREEN_WIDTH - 5,
    //     LIST_Y ,
    //     2
    // );

    // tft.setTextDatum(TL_DATUM);
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
    tft.setTextDatum(TL_DATUM);
    tft.drawString("BLE Device",10,41);
    tft.setTextDatum(TR_DATUM);
    tft.drawString("RSSI",SCREEN_WIDTH - 5,41);

    tft.setTextDatum(TL_DATUM);
}
void DisplayManager::drawBLEDevice(
    int row,
    String name,
    int rssi)
{
    int y = LIST_Y + (row * 18);

    uint16_t signalColor;

    if (rssi > -50)
        signalColor = COLOR_SIGNAL_STRONG;
    else if (rssi > -70)
        signalColor = COLOR_SIGNAL_MEDIUM;
    else
        signalColor = COLOR_SIGNAL_WEAK;

    if (name == "")
        name = "<Unknown>";

    if (name.length() > 18)
    {
        name = name.substring(0, 15) + "...";
    }

    tft.setTextDatum(TL_DATUM);

    tft.setTextColor(
        COLOR_TEXT,
        COLOR_BACKGROUND
    );

    tft.drawString(
        name,
        5,
        y,
        2
    );

    tft.setTextDatum(TR_DATUM);

    tft.setTextColor(
        signalColor,
        COLOR_BACKGROUND
    );

    tft.drawString(
        String(rssi),
        SCREEN_WIDTH - 5,
        y,
        2
    );

    tft.setTextDatum(TL_DATUM);
}
void DisplayManager::drawBLEFooter(int deviceCount)
{
    tft.fillRect(
        0,
        FOOTER_Y,
        SCREEN_WIDTH,
        SCREEN_HEIGHT - FOOTER_Y,
        COLOR_FOOTER
    );

    tft.setTextColor(
        COLOR_FOOTER_TEXT,
        COLOR_FOOTER
    );

    tft.setTextDatum(TL_DATUM);

    String text = "BLE Devices: " + String(deviceCount);

    tft.drawString(
        text,
        5,
        FOOTER_Y + 4,
        2
    );
}
void DisplayManager::drawMode(String mode)
{
    // Clear mode area
    tft.fillRect(
        0,
        30,
        SCREEN_WIDTH,
        22,
        COLOR_BACKGROUND
    );

    tft.setTextFont(2);

    // MODE label
    tft.setTextDatum(TL_DATUM);
    tft.setTextColor(
        COLOR_TEXT,
        COLOR_BACKGROUND
    );

    tft.drawString(
        "MODE:",
        5,
        32,
        2
    );

    // Mode value
    tft.setTextColor(
        COLOR_SIGNAL_STRONG,
        COLOR_BACKGROUND
    );

    tft.drawString(
        mode,
        65,
        32,
        2
    );

    // Restore default
    tft.setTextDatum(TL_DATUM);
}
// ============================================================
// EP5 HOME MENU
// ============================================================

void DisplayManager::drawHome()
{
    tft.fillScreen(COLOR_MENU_BACKGROUND);

    tft.setTextDatum(TC_DATUM);

    tft.setTextColor(
        COLOR_MENU_TEXT,
        COLOR_MENU_BACKGROUND
    );

    tft.setTextFont(2);

    tft.drawString(
        "ESP32 SCANNER",
        SCREEN_WIDTH / 2,
        15
    );

    tft.setTextDatum(TL_DATUM);

    // WiFi button
    tft.fillRoundRect(
        10, 50, 145, 65,
        8,
        COLOR_BUTTON_NORMAL
    );

    tft.drawRoundRect(
        10, 50, 145, 65,
        8,
        COLOR_BUTTON_BORDER
    );

    tft.setTextDatum(MC_DATUM);

    tft.setTextColor(
        COLOR_BUTTON_TEXT,
        COLOR_BUTTON_NORMAL
    );

    tft.drawString(
        "WiFi",
        82,
        82
    );

    // BLE button
    tft.fillRoundRect(
        165, 50, 145, 65,
        8,
        COLOR_BUTTON_NORMAL
    );

    tft.drawRoundRect(
        165, 50, 145, 65,
        8,
        COLOR_BUTTON_BORDER
    );

    tft.setTextColor(
        COLOR_BUTTON_TEXT,
        COLOR_BUTTON_NORMAL
    );

    tft.drawString(
        "BLE",
        237,
        82
    );

    tft.setTextDatum(TL_DATUM);
}


// ============================================================
// WiFi screen navigation button
// ============================================================

void DisplayManager::drawWiFiNavigation()
{
    tft.fillRoundRect(
        210, 195, 100, 35,
        8,
        COLOR_BUTTON_NORMAL
    );

    tft.drawRoundRect(
        210, 195, 100, 35,
        8,
        COLOR_BUTTON_BORDER
    );

    tft.setTextDatum(MC_DATUM);

    tft.setTextColor(
        COLOR_BUTTON_TEXT,
        COLOR_BUTTON_NORMAL
    );

    tft.setTextFont(2);

    tft.drawString(
        "HOME",
        260,
        212
    );

    tft.setTextDatum(TL_DATUM);
}


// ============================================================
// BLE screen navigation button
// ============================================================

void DisplayManager::drawBLENavigation()
{
    tft.fillRoundRect(
        210, 195, 100, 35,
        8,
        COLOR_BUTTON_NORMAL
    );

    tft.drawRoundRect(
        210, 195, 100, 35,
        8,
        COLOR_BUTTON_BORDER
    );

    tft.setTextDatum(MC_DATUM);

    tft.setTextColor(
        COLOR_BUTTON_TEXT,
        COLOR_BUTTON_NORMAL
    );

    tft.setTextFont(2);

    tft.drawString(
        "HOME",
        260,
        212
    );

    tft.setTextDatum(TL_DATUM);
}


// ============================================================
// HOME button hit detection
// ============================================================

bool DisplayManager::isHomeButtonTouched(
    int x,
    int y
)
{
    return (
        x >= 210 &&
        x <= 310 &&
        y >= 195 &&
        y <= 230
    );
}