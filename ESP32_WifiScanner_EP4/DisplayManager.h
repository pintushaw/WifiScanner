#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include <TFT_eSPI.h>

class DisplayManager
{
public:
    void drawHome();

    void drawWiFiNavigation();

    void drawBLENavigation();

    bool isHomeButtonTouched(int x, int y);
    
    void begin();

    void drawDashboard();

    void drawHeader(String mode);

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
    void clearBLEArea();

    void drawBLEHeader();

    void drawBLEDevice(
        int row,
        String name,
        int rssi
    );
    void drawMode(String mode);
    void drawBLEFooter(int deviceCount);

private:

    TFT_eSPI tft = TFT_eSPI();
};

#endif