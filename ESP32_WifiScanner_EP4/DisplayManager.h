#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include <TFT_eSPI.h>

class DisplayManager
{
public:

    void begin();

    void drawDashboard();

    void drawHeader();

    void drawColumnHeader();

    void drawFooter(int networkCount);

    void clearNetworkArea();

    void drawNetwork(
        int row,
        String ssid,
        int rssi,
        int channel,
        String security
    );
    void refreshSPI();
private:

    TFT_eSPI tft = TFT_eSPI();
};

#endif