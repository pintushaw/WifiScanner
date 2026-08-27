#include <SPI.h>
#include <XPT2046_Touchscreen.h>

#define TOUCH_CLK   32
#define TOUCH_MISO  16
#define TOUCH_MOSI  17
#define TOUCH_CS    25
#define TOUCH_IRQ   26

SPIClass touchSPI(HSPI);

XPT2046_Touchscreen touch(TOUCH_CS, TOUCH_IRQ);

void waitForRelease()
{
    while (touch.touched())
    {
        delay(20);
    }

    delay(300);
}

void readCorner(const char* name)
{
    Serial.println();
    Serial.println("--------------------------------");
    Serial.print("Touch: ");
    Serial.println(name);
    Serial.println("--------------------------------");

    // Wait for touch
    while (!touch.touched())
    {
        delay(20);
    }

    TS_Point p = touch.getPoint();

    Serial.print(name);
    Serial.print(" -> X = ");
    Serial.print(p.x);

    Serial.print("  Y = ");
    Serial.print(p.y);

    Serial.print("  Z = ");
    Serial.println(p.z);

    waitForRelease();
}

void setup()
{
    Serial.begin(115200);
    delay(1000);

    Serial.println();
    Serial.println("XPT2046 Calibration Test");

    touchSPI.begin(
        TOUCH_CLK,
        TOUCH_MISO,
        TOUCH_MOSI,
        -1
    );

    Serial.println("SPI initialized");

    touch.begin(touchSPI);

    Serial.println("Touch initialized");
    Serial.println();
    Serial.println("Calibration starting...");
}

void loop()
{
    readCorner("TOP-LEFT");
    readCorner("TOP-RIGHT");
    readCorner("BOTTOM-RIGHT");
    readCorner("BOTTOM-LEFT");

    Serial.println();
    Serial.println("================================");
    Serial.println("Calibration complete");
    Serial.println("================================");
    Serial.println();

    while (true)
    {
        delay(1000);
    }
}