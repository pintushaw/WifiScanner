#include <SPI.h>
#include <TFT_eSPI.h>
#include <XPT2046_Touchscreen.h>

#define TOUCH_CLK   32
#define TOUCH_MISO  16
#define TOUCH_MOSI  17
#define TOUCH_CS    25
#define TOUCH_IRQ   26

#define SCREEN_WIDTH  320
#define SCREEN_HEIGHT 240

SPIClass touchSPI(HSPI);

XPT2046_Touchscreen touch(TOUCH_CS, TOUCH_IRQ);

TFT_eSPI tft = TFT_eSPI();

int targetX[] = {
    20, 160, 300,
    20, 160, 300,
    20, 160, 300
};

int targetY[] = {
    20, 20, 20,
    120, 120, 120,
    220, 220, 220
};

void drawTarget(int x, int y)
{
    tft.drawLine(
        x - 12, y,
        x + 12, y,
        TFT_WHITE
    );

    tft.drawLine(
        x, y - 12,
        x, y + 12,
        TFT_WHITE
    );

    tft.fillCircle(
        x,
        y,
        3,
        TFT_WHITE
    );
}

void waitForRelease()
{
    while (touch.touched())
    {
        delay(20);
    }

    delay(500);
}

void setup()
{
    Serial.begin(115200);
    delay(1000);

    tft.init();
    tft.setRotation(3);

    Serial.print("Width = ");
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

    Serial.println();
    Serial.println("3 x 3 TOUCH CALIBRATION");
}

void loop()
{
    static int point = 0;

    if (point >= 9)
    {
        Serial.println();
        Serial.println("==============================");
        Serial.println("CALIBRATION COMPLETE");
        Serial.println("==============================");

        while (true)
        {
            delay(1000);
        }
    }

    int x = targetX[point];
    int y = targetY[point];

    tft.fillScreen(TFT_BLACK);

    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextFont(2);

    tft.drawString(
        "Touch target " + String(point + 1),
        10,
        5
    );

    drawTarget(x, y);

    Serial.println();
    Serial.print("POINT ");
    Serial.print(point + 1);

    Serial.print(" TARGET X=");
    Serial.print(x);

    Serial.print(" Y=");
    Serial.println(y);

    while (!touch.touched())
    {
        delay(10);
    }

    TS_Point p = touch.getPoint();

    Serial.print("RAW X=");
    Serial.print(p.x);

    Serial.print(" Y=");
    Serial.print(p.y);

    Serial.print(" Z=");
    Serial.println(p.z);

    waitForRelease();

    point++;

    delay(300);
}