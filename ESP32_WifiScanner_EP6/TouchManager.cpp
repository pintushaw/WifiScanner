#include "TouchManager.h"
#include "Config.h"

// ============================================================
// Touch pins
// ============================================================

#define TOUCH_CLK   32
#define TOUCH_MISO  16
#define TOUCH_MOSI  17
#define TOUCH_CS    25
#define TOUCH_IRQ   26

// ============================================================
// 2D Touch Calibration
// Calibrated for 320 x 240 display
// ============================================================

const float CAL_X_RAW_X  = -0.09656;
const float CAL_X_RAW_Y  = -0.000857;
const float CAL_X_OFFSET = 362.38;

const float CAL_Y_RAW_X  = -0.001132;
const float CAL_Y_RAW_Y  = -0.07631;
const float CAL_Y_OFFSET = 287.10;


// ============================================================
// Initialize touch
// ============================================================

void TouchManager::begin()
{
    touchSPI.begin(
        TOUCH_CLK,
        TOUCH_MISO,
        TOUCH_MOSI,
        -1
    );

    touch.begin(touchSPI);

    Serial.println("Touch initialized");
}


// ============================================================
// Check whether screen is touched
// ============================================================

bool TouchManager::touched()
{
    return touch.touched();
}


// ============================================================
// Convert raw X coordinate
// ============================================================

int TouchManager::getTouchX(
    int rawX,
    int rawY
)
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


// ============================================================
// Convert raw Y coordinate
// ============================================================

int TouchManager::getTouchY(
    int rawX,
    int rawY
)
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
// Get calibrated touch coordinates
// ============================================================

bool TouchManager::getPoint(
    int &x,
    int &y
)
{
    if (!touch.touched())
        return false;

    TS_Point p = touch.getPoint();

    x = getTouchX(
        p.x,
        p.y
    );

    y = getTouchY(
        p.x,
        p.y
    );

    return true;
}
