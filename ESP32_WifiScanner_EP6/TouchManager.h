#ifndef TOUCH_MANAGER_H
#define TOUCH_MANAGER_H

#include <SPI.h>
#include <XPT2046_Touchscreen.h>

class TouchManager
{
public:

    void begin();

    bool touched();

    bool getPoint(
        int &x,
        int &y
    );

private:

    SPIClass touchSPI = SPIClass(HSPI);

    XPT2046_Touchscreen touch =
        XPT2046_Touchscreen(25, 26);

    int getTouchX(
        int rawX,
        int rawY
    );

    int getTouchY(
        int rawX,
        int rawY
    );
};

#endif