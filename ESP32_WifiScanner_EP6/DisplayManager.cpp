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
 //tft.drawString("Scanning...", SCREEN_WIDTH - 5, FOOTER_Y + 5);

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
    //tft.fillScreen(COLOR_MENU_BACKGROUND);
    tft.fillScreen(COLOR_BACKGROUND);
    tft.setTextDatum(TC_DATUM);

    tft.setTextColor(
        COLOR_HEADER_TEXT,
        COLOR_BACKGROUND
    );

    tft.setTextFont(2);

    tft.drawString(
        "ESP32 SCANNER",
        SCREEN_WIDTH / 2,
        15
    );

    // -------------------------------
    // WiFi
    // -------------------------------

    tft.fillRoundRect(
        10, 50, 145, 55,
        8,
        COLOR_BUTTON_NORMAL
    );

    tft.drawRoundRect(
        10, 50, 145, 55,
        8,
        COLOR_BUTTON_BORDER
    );

    // -------------------------------
    // BLE
    // -------------------------------

    tft.fillRoundRect(
        165, 50, 145, 55,
        8,
        COLOR_BUTTON_NORMAL
    );

    tft.drawRoundRect(
        165, 50, 145, 55,
        8,
        COLOR_BUTTON_BORDER
    );

    // -------------------------------
    // Scan
    // -------------------------------

    tft.fillRoundRect(
        10, 125, 145, 55,
        8,
        COLOR_BUTTON_NORMAL
    );

    tft.drawRoundRect(
        10, 125, 145, 55,
        8,
        COLOR_BUTTON_BORDER
    );

    // -------------------------------
    // Battery
    // -------------------------------

    tft.fillRoundRect(
        165, 125, 145, 55,
        8,
        COLOR_BUTTON_NORMAL
    );

    tft.drawRoundRect(
        165, 125, 145, 55,
        8,
        COLOR_BUTTON_BORDER
    );

    // -------------------------------
    // Button text
    // -------------------------------

    tft.setTextDatum(MC_DATUM);

    tft.setTextColor(
        COLOR_BUTTON_TEXT,
        COLOR_BUTTON_NORMAL
    );

    tft.drawString("WiFi", 82, 77);
    tft.drawString("BLE", 237, 77);
    tft.drawString("Scan", 82, 152);
    tft.drawString("Battery", 237, 152);

    tft.setTextDatum(TL_DATUM);
}

// ============================================================
// EP5 BATTERY SCREEN
// ============================================================

void DisplayManager::drawBattery(float batteryVoltage,int batteryPercent)
{
    tft.fillScreen(COLOR_BACKGROUND);

    // Header
    drawHeader("Battery");

    
    // -------------------------------
    // Battery percentage
    // -------------------------------

    tft.setTextDatum(MC_DATUM);

    tft.setTextColor(
        COLOR_TEXT,
        COLOR_BACKGROUND
    );

    tft.setTextFont(4);

    tft.drawString(
        String(batteryPercent) + "%",
        SCREEN_WIDTH / 2,
        85
    );

    // -------------------------------
    // Battery bar
    // -------------------------------

    int barX = 40;
    int barY = 115;
    int barW = 240;
    int barH = 25;

    // Battery outline
    tft.drawRoundRect(
        barX,
        barY,
        barW,
        barH,
        5,
        COLOR_TEXT
    );

    // Battery fill color based on percentage
    uint16_t batteryColor;

    if (batteryPercent > 50)
    {
        batteryColor = COLOR_SIGNAL_STRONG;
    }
    else if (batteryPercent > 20)
    {
        batteryColor = COLOR_SIGNAL_MEDIUM;
    }
    else
    {
        batteryColor = COLOR_SIGNAL_WEAK;
    }

    // Battery fill
    int fillWidth =
        (barW - 4) * batteryPercent / 100;

    if (fillWidth > 0)
    {
        tft.fillRoundRect(
            barX + 2,
            barY + 2,
            fillWidth,
            barH - 4,
            3,
            batteryColor
        );
    }

    // -------------------------------
    // Battery voltage
    // -------------------------------

    tft.setTextFont(2);

    tft.drawString(
        String(batteryVoltage, 2) + " V",
        SCREEN_WIDTH / 2,
        160
    );

    // -------------------------------
    // Footer
    // -------------------------------

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

    tft.drawString(
        "Battery Monitor",
        5,
        FOOTER_Y + 5
    );

    // Restore text alignment
    tft.setTextDatum(TL_DATUM);
}

// ============================================================
// Battery Disconnected Screen
// ============================================================

void DisplayManager::drawBatteryDisconnected()
{
    tft.fillScreen(COLOR_BACKGROUND);

    // Header
    drawHeader("Battery");

    // -------------------------------
    // Battery disconnected message
    // -------------------------------

    tft.setTextDatum(MC_DATUM);

    tft.setTextColor(
        COLOR_TEXT,
        COLOR_BACKGROUND
    );

    tft.setTextFont(2);

    tft.drawString(
        "Battery disconnected",
        SCREEN_WIDTH / 2,
        90
    );

    tft.setTextFont(2);

    tft.drawString(
        "Connect battery to view status",
        SCREEN_WIDTH / 2,
        120
    );

    // -------------------------------
    // Footer
    // -------------------------------

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

    tft.drawString(
        "Battery Monitor",
        5,
        FOOTER_Y + 5
    );

    // Restore text alignment
    tft.setTextDatum(TL_DATUM);
}

// ============================================================
// WiFi screen navigation button
// ============================================================

void DisplayManager::drawWiFiNavigation()
{
   tft.fillRoundRect(
        HOME_X,
        HOME_Y,
        HOME_W,
        HOME_H,
        5,
        COLOR_BUTTON_NORMAL
    );

    tft.drawRoundRect(
        HOME_X,
        HOME_Y,
        HOME_W,
        HOME_H,
        5,
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
        HOME_X + HOME_W / 2,
        HOME_Y + HOME_H / 2
    );

    tft.setTextDatum(TL_DATUM);
}


// ============================================================
// BLE screen navigation button
// ============================================================

void DisplayManager::drawBLENavigation()
{
   tft.fillRoundRect(
        HOME_X,
        HOME_Y,
        HOME_W,
        HOME_H,
        5,
        COLOR_BUTTON_NORMAL
    );

    tft.drawRoundRect(
        HOME_X,
        HOME_Y,
        HOME_W,
        HOME_H,
        5,
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
        HOME_X + HOME_W / 2,
        HOME_Y + HOME_H / 2
    );

    tft.setTextDatum(TL_DATUM);
}


// ============================================================
// HOME button hit detection
// ============================================================

bool DisplayManager::isHomeButtonTouched(int x, int y)
{
    return (
        x >= HOME_X &&
        x <= HOME_X + HOME_W &&
        y >= HOME_Y &&
        y <= HOME_Y + HOME_H
    );
}
void DisplayManager::drawMessage(const String& message)
{
    tft.setTextDatum(MC_DATUM);
    tft.setTextFont(2);

    tft.setTextColor(
        COLOR_COLUMN_TEXT,
        COLOR_BACKGROUND
    );

    tft.drawString(
        message,
        SCREEN_WIDTH / 2,
        SCREEN_HEIGHT / 2
    );
}
// ============================================================
// Battery screen navigation button
// ============================================================

void DisplayManager::drawBatteryNavigation()
{
    tft.fillRoundRect(
        HOME_X,
        HOME_Y,
        HOME_W,
        HOME_H,
        5,
        COLOR_BUTTON_NORMAL
    );

    tft.drawRoundRect(
        HOME_X,
        HOME_Y,
        HOME_W,
        HOME_H,
        5,
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
        HOME_X + HOME_W / 2,
        HOME_Y + HOME_H / 2
    );

    tft.setTextDatum(TL_DATUM);
}