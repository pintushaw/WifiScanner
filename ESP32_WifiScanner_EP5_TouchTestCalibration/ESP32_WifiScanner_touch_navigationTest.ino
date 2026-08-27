#include <SPI.h>
#include <TFT_eSPI.h>
#include <XPT2046_Touchscreen.h>

// ============================================================
// TOUCH PINS
// ============================================================

#define TOUCH_CLK   32
#define TOUCH_MISO  16
#define TOUCH_MOSI  17
#define TOUCH_CS    25
#define TOUCH_IRQ   26

// ============================================================
// DISPLAY
// ============================================================

#define SCREEN_WIDTH  320
#define SCREEN_HEIGHT 240

TFT_eSPI tft = TFT_eSPI();

SPIClass touchSPI(HSPI);

XPT2046_Touchscreen touch(
    TOUCH_CS,
    TOUCH_IRQ
);

// ============================================================
// TOUCH CALIBRATION
// ============================================================

const float CAL_X_RAW_X  = -0.09656;
const float CAL_X_RAW_Y  = -0.000857;
const float CAL_X_OFFSET = 362.38;

const float CAL_Y_RAW_X  = -0.001132;
const float CAL_Y_RAW_Y  = -0.07631;
const float CAL_Y_OFFSET = 287.10;

int getTouchX(int rawX, int rawY)
{
    float x =
        (CAL_X_RAW_X * rawX) +
        (CAL_X_RAW_Y * rawY) +
        CAL_X_OFFSET;

    return constrain(
        (int)x,
        0,
        SCREEN_WIDTH - 1
    );
}

int getTouchY(int rawX, int rawY)
{
    float y =
        (CAL_Y_RAW_X * rawX) +
        (CAL_Y_RAW_Y * rawY) +
        CAL_Y_OFFSET;

    return constrain(
        (int)y,
        0,
        SCREEN_HEIGHT - 1
    );
}

// ============================================================
// BUTTON
// ============================================================

struct TouchButton
{
    int x;
    int y;
    int width;
    int height;

    const char* label;
};

// ============================================================
// SCREENS
// ============================================================

enum Screen
{
    SCREEN_HOME,
    SCREEN_WIFI,
    SCREEN_BLE,
    SCREEN_SCAN,
    SCREEN_BATTERY
};

Screen currentScreen = SCREEN_HOME;

// ============================================================
// DRAW BUTTON
// ============================================================

void drawButton(
    TouchButton &button,
    bool pressed = false
)
{
    uint16_t fillColor;
    uint16_t textColor;

    if (pressed)
    {
        fillColor = TFT_BLACK;
        textColor = TFT_WHITE;
    }
    else
    {
        fillColor = TFT_DARKGREY;
        textColor = TFT_BLACK;
    }

    tft.fillRoundRect(
        button.x,
        button.y,
        button.width,
        button.height,
        8,
        fillColor
    );

    tft.drawRoundRect(
        button.x,
        button.y,
        button.width,
        button.height,
        8,
        TFT_BLACK
    );

    tft.setTextDatum(MC_DATUM);

    tft.setTextColor(
        textColor,
        fillColor
    );

    tft.setTextFont(2);

    tft.drawString(
        button.label,
        button.x + button.width / 2,
        button.y + button.height / 2
    );

    tft.setTextDatum(TL_DATUM);
}

// ============================================================
// HOME SCREEN
// ============================================================

void drawHome()
{
    tft.fillScreen(TFT_WHITE);

    tft.setTextDatum(TC_DATUM);

    tft.setTextColor(
        TFT_BLACK,
        TFT_WHITE
    );

    tft.setTextFont(2);

    tft.drawString(
        "ESP32 SCANNER",
        SCREEN_WIDTH / 2,
        15
    );

    tft.setTextDatum(TL_DATUM);

    TouchButton wifi =
        { 10, 50, 145, 65, "WiFi" };

    TouchButton ble =
        { 165, 50, 145, 65, "BLE" };

    TouchButton scan =
        { 10, 130, 145, 65, "Scan" };

    TouchButton battery =
        { 165, 130, 145, 65, "Battery" };

    drawButton(wifi);
    drawButton(ble);
    drawButton(scan);
    drawButton(battery);
}

// ============================================================
// SIMPLE SCREEN HEADER
// ============================================================

void drawHeader(const char* title)
{
    tft.fillScreen(TFT_WHITE);

    tft.setTextDatum(TC_DATUM);

    tft.setTextColor(
        TFT_BLACK,
        TFT_WHITE
    );

    tft.setTextFont(2);

    tft.drawString(
        title,
        SCREEN_WIDTH / 2,
        15
    );

    tft.setTextDatum(TL_DATUM);
}

// ============================================================
// WIFI SCREEN
// ============================================================

void drawWiFiScreen()
{
    drawHeader("WiFi SCANNER");

    tft.setTextColor(TFT_BLACK);

    tft.drawString(
        "SSID",
        10,
        50
    );

    tft.drawString(
        "RSSI",
        250,
        50
    );

    tft.drawString(
        "Network-1",
        10,
        80
    );

    tft.drawString(
        "-42 dBm",
        250,
        80
    );

    tft.drawString(
        "Network-2",
        10,
        110
    );

    tft.drawString(
        "-67 dBm",
        250,
        110
    );

    tft.drawString(
        "Network-3",
        10,
        140
    );

    tft.drawString(
        "-81 dBm",
        250,
        140
    );

    TouchButton home =
        { 210, 190, 100, 35, "HOME" };

    drawButton(home);
}

// ============================================================
// BLE SCREEN
// ============================================================

void drawBLEScreen()
{
    drawHeader("BLE SCANNER");

    tft.setTextColor(TFT_BLACK);

    tft.drawString(
        "Device",
        10,
        50
    );

    tft.drawString(
        "RSSI",
        250,
        50
    );

    tft.drawString(
        "ESP32 Device",
        10,
        80
    );

    tft.drawString(
        "-48 dBm",
        250,
        80
    );

    tft.drawString(
        "BLE Sensor",
        10,
        110
    );

    tft.drawString(
        "-72 dBm",
        250,
        110
    );

    TouchButton home =
        { 210, 190, 100, 35, "HOME" };

    drawButton(home);
}

// ============================================================
// SCAN SCREEN
// ============================================================

void drawScanScreen()
{
    drawHeader("SCAN MODE");

    TouchButton wifi =
        { 10, 60, 145, 55, "WiFi Scan" };

    TouchButton ble =
        { 165, 60, 145, 55, "BLE Scan" };

    TouchButton both =
        { 80, 130, 160, 50, "Scan Both" };

    TouchButton home =
        { 110, 195, 100, 35, "HOME" };

    drawButton(wifi);
    drawButton(ble);
    drawButton(both);
    drawButton(home);
}

// ============================================================
// BATTERY SCREEN
// ============================================================

void drawBatteryScreen()
{
    drawHeader("BATTERY");

    tft.setTextDatum(MC_DATUM);

    tft.setTextColor(
        TFT_BLACK,
        TFT_WHITE
    );

    tft.setTextFont(4);

    tft.drawString(
        "78%",
        SCREEN_WIDTH / 2,
        75
    );

    tft.setTextFont(2);

    tft.drawString(
        "3.92 V",
        SCREEN_WIDTH / 2,
        120
    );

    tft.setTextFont(2);

    tft.setTextDatum(TL_DATUM);

    TouchButton home =
        { 110, 190, 100, 35, "HOME" };

    drawButton(home);
}

// ============================================================
// SCREEN DRAW
// ============================================================

void drawCurrentScreen()
{
    switch (currentScreen)
    {
        case SCREEN_HOME:
            drawHome();
            break;

        case SCREEN_WIFI:
            drawWiFiScreen();
            break;

        case SCREEN_BLE:
            drawBLEScreen();
            break;

        case SCREEN_SCAN:
            drawScanScreen();
            break;

        case SCREEN_BATTERY:
            drawBatteryScreen();
            break;
    }
}

// ============================================================
// HANDLE TOUCH
// ============================================================

void handleTouch(int x, int y)
{
    switch (currentScreen)
    {
        // ----------------------------------------------------
        // HOME
        // ----------------------------------------------------

        case SCREEN_HOME:

            if (x >= 10 && x <= 155 &&
                y >= 50 && y <= 115)
            {
                Serial.println("WiFi selected");

                currentScreen =
                    SCREEN_WIFI;
            }

            else if (
                x >= 165 && x <= 310 &&
                y >= 50 && y <= 115)
            {
                Serial.println("BLE selected");

                currentScreen =
                    SCREEN_BLE;
            }

            else if (
                x >= 10 && x <= 155 &&
                y >= 130 && y <= 195)
            {
                Serial.println("Scan selected");

                currentScreen =
                    SCREEN_SCAN;
            }

            else if (
                x >= 165 && x <= 310 &&
                y >= 130 && y <= 195)
            {
                Serial.println("Battery selected");

                currentScreen =
                    SCREEN_BATTERY;
            }

            break;

        // ----------------------------------------------------
        // WIFI
        // ----------------------------------------------------

        case SCREEN_WIFI:

            if (
                x >= 210 && x <= 310 &&
                y >= 190 && y <= 225)
            {
                currentScreen =
                    SCREEN_HOME;
            }

            break;

        // ----------------------------------------------------
        // BLE
        // ----------------------------------------------------

        case SCREEN_BLE:

            if (
                x >= 210 && x <= 310 &&
                y >= 190 && y <= 225)
            {
                currentScreen =
                    SCREEN_HOME;
            }

            break;

        // ----------------------------------------------------
        // SCAN
        // ----------------------------------------------------

        case SCREEN_SCAN:

            if (
                x >= 110 && x <= 210 &&
                y >= 195 && y <= 230)
            {
                currentScreen =
                    SCREEN_HOME;
            }

            else if (
                x >= 10 && x <= 155 &&
                y >= 60 && y <= 115)
            {
                Serial.println("WiFi Scan selected");
            }

            else if (
                x >= 165 && x <= 310 &&
                y >= 60 && y <= 115)
            {
                Serial.println("BLE Scan selected");
            }

            else if (
                x >= 80 && x <= 240 &&
                y >= 130 && y <= 180)
            {
                Serial.println("Scan Both selected");
            }

            break;

        // ----------------------------------------------------
        // BATTERY
        // ----------------------------------------------------

        case SCREEN_BATTERY:

            if (
                x >= 110 && x <= 210 &&
                y >= 190 && y <= 225)
            {
                currentScreen =
                    SCREEN_HOME;
            }

            break;
    }

    drawCurrentScreen();
}

// ============================================================
// SETUP
// ============================================================

void setup()
{
    Serial.begin(115200);

    delay(1000);

    Serial.println();
    Serial.println("==============================");
    Serial.println("ESP32 EP5 NAVIGATION");
    Serial.println("==============================");

    tft.init();

    tft.setRotation(3);

    Serial.print("Width  = ");
    Serial.println(tft.width());

    Serial.print("Height = ");
    Serial.println(tft.height());

    touchSPI.begin(
        TOUCH_CLK,
        TOUCH_MISO,
        TOUCH_MOSI,
        -1
    );

    touch.begin(touchSPI);

    Serial.println("Touch initialized");

    drawHome();
}

// ============================================================
// LOOP
// ============================================================

void loop()
{
    if (!touch.touched())
        return;

    TS_Point p =
        touch.getPoint();

    int x =
        getTouchX(p.x, p.y);

    int y =
        getTouchY(p.x, p.y);

    Serial.print("Touch X=");
    Serial.print(x);

    Serial.print(" Y=");
    Serial.println(y);

    handleTouch(x, y);

    // Wait for release
    while (touch.touched())
    {
        delay(10);
    }

    delay(100);
}