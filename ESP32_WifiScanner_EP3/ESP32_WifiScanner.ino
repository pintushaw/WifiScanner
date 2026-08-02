/*
  ------------------------------------------------------------
  Project : ESP32 Portable Wi-Fi Scanner
  Episode : 3 - TFT Wi-Fi Scanner Dashboard
  Author  : DigitalLab.org
  ------------------------------------------------------------
*/

#include <WiFi.h>

#include "Config.h"
#include "DisplayManager.h"

DisplayManager display;

void scanNetworks();
String getEncryptionType(wifi_auth_mode_t encryption);

void setup()
{
    Serial.begin(115200);

    delay(1000);

    Serial.println();
    Serial.println("--------------------------------------------");
    Serial.println("ESP32 Portable Wi-Fi Scanner");
    Serial.println("Episode 3");
    Serial.println("--------------------------------------------");

    // Put ESP32 in Station mode
    WiFi.mode(WIFI_STA);

    // Disconnect from any previously connected AP
    WiFi.disconnect(true);

    delay(100);

    // Initialize TFT
    display.begin();

    display.drawDashboard();
}

void loop()
{
    Serial.println("Loop Started");
    scanNetworks();
    Serial.println("Scan Finished");
    delay(WIFI_SCAN_INTERVAL);
}

/******************************************************
 * Scan nearby Wi-Fi Networks
 ******************************************************/
void scanNetworks()
{
    WiFi.scanDelete();

    int numberOfNetworks = WiFi.scanNetworks(false, true);

    display.clearNetworkArea();
    Serial.println("A");
    display.drawFooter(numberOfNetworks);
    Serial.print("Networks=");
    Serial.println(numberOfNetworks);
    if (numberOfNetworks <= 0)
        return;
    
    for (int i = 0; i < numberOfNetworks && i < MAX_VISIBLE_NETWORKS; i++)
    {
        String ssid = WiFi.SSID(i);

        if (ssid == "")
            ssid = "<Hidden>";
        
        display.drawNetwork(
            i,
            ssid,
            WiFi.RSSI(i),
            WiFi.channel(i),
            getEncryptionType(WiFi.encryptionType(i))
        );
    }
   
}

/******************************************************
 * Convert Encryption Enum to String
 ******************************************************/
String getEncryptionType(wifi_auth_mode_t encryption)
{
    switch (encryption)
    {
        case WIFI_AUTH_OPEN:
            return "OPEN";

        case WIFI_AUTH_WEP:
            return "WEP";

        case WIFI_AUTH_WPA_PSK:
            return "WPA";

        case WIFI_AUTH_WPA2_PSK:
            return "WPA2";

        case WIFI_AUTH_WPA_WPA2_PSK:
            return "WPA/WPA2";

        case WIFI_AUTH_WPA2_ENTERPRISE:
            return "WPA2-ENT";

        case WIFI_AUTH_WPA3_PSK:
            return "WPA3";

        case WIFI_AUTH_WPA2_WPA3_PSK:
            return "WPA2/WPA3";

        default:
            return "UNKNOWN";
    }
}
void scanNetworks2()
{
    display.clearNetworkArea();

    display.drawNetwork(
        0,
        "DigitalLab",
        -42,
        11,
        "WPA2");

    display.drawFooter(1);
}

