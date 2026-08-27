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
// BUTTON STRUCTURE
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
// BUTTONS
// ============================================================

TouchButton buttons[] =
{
    { 10,  50, 145, 75, "WiFi"    },
    { 165, 50, 145, 75, "BLE"     },
    { 10,  135, 145, 75, "Scan"  },
    { 165, 135, 145, 75, "Battery"}
};

const int BUTTON_COUNT =
    sizeof(buttons) / sizeof(buttons[0]);

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
// DRAW COMPLETE MENU
// ============================================================

void drawMenu()
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

    for (int i = 0; i < BUTTON_COUNT; i++)
    {
        drawButton(buttons[i]);
    }
}

// ============================================================
// CHECK BUTTON
// ============================================================

int getButton(int x, int y)
{
    for (int i = 0; i < BUTTON_COUNT; i++)
    {
        TouchButton &button = buttons[i];

        if (
            x >= button.x &&
            x <= button.x + button.width &&
            y >= button.y &&
            y <= button.y + button.height
        )
        {
            return i;
        }
    }

    return -1;
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
    Serial.println("ESP32 EP5 TOUCH BUTTON TEST");
    Serial.println("==============================");

    // --------------------------------------------------------
    // TFT
    // --------------------------------------------------------

    tft.init();

    tft.setRotation(3);

    Serial.print("TFT Width  = ");
    Serial.println(tft.width());

    Serial.print("TFT Height = ");
    Serial.println(tft.height());

    // --------------------------------------------------------
    // TOUCH SPI
    // --------------------------------------------------------

    touchSPI.begin(
        TOUCH_CLK,
        TOUCH_MISO,
        TOUCH_MOSI,
        -1
    );

    touch.begin(touchSPI);

    Serial.println("Touch initialized");

    // --------------------------------------------------------
    // MENU
    // --------------------------------------------------------

    drawMenu();
}

// ============================================================
// LOOP
// ============================================================

void loop()
{
    if (!touch.touched())
        return;

    TS_Point p = touch.getPoint();

    int x = getTouchX(
        p.x,
        p.y
    );

    int y = getTouchY(
        p.x,
        p.y
    );

    Serial.print("Touch: X=");
    Serial.print(x);

    Serial.print(" Y=");
    Serial.println(y);

    int buttonIndex =
        getButton(x, y);

    if (buttonIndex >= 0)
    {
        TouchButton &button =
            buttons[buttonIndex];

        Serial.print("BUTTON PRESSED: ");
        Serial.println(button.label);

        // Visual feedback
        drawButton(
            button,
            true
        );

        delay(150);

        drawButton(
            button,
            false
        );
    }

    // Wait until finger is released
    while (touch.touched())
    {
        delay(10);
    }

    delay(50);
}