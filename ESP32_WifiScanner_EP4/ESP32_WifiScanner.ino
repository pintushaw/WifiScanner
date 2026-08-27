/*
  ------------------------------------------------------------
  Project : ESP32 Portable Wi-Fi Scanner
  Episode : 4 - TFT Wi-Fi Scanner Dashboard
  Author  : DigitalLab.org
  ------------------------------------------------------------
*/

#include <WiFi.h>

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

#include "Config.h"
#include "DisplayManager.h"
#include "TouchManager.h"


TouchManager touch;
DisplayManager display;
BLEScan* pBLEScan;

#define BLE_SCAN_TIME 5
enum Screen
{
    SCREEN_HOME,
    SCREEN_WIFI,
    SCREEN_BLE
};

Screen currentScreen = SCREEN_HOME;
void scanNetworks();
String getEncryptionType(wifi_auth_mode_t encryption);

void setup()
{
    Serial.begin(115200);

    delay(1000);

    Serial.println();
    Serial.println("--------------------------------------------");
    Serial.println("ESP32 Portable Wi-Fi Scanner");
    Serial.println("Episode 4");
    Serial.println("--------------------------------------------");

    // Put ESP32 in Station mode
    WiFi.mode(WIFI_STA);

    // Disconnect from any previously connected AP
    WiFi.disconnect(true);

    delay(100);

    // Initialize TFT
    display.begin();

    display.drawDashboard();
    
    // Initialize BLE
    BLEDevice::init("");
    pBLEScan = BLEDevice::getScan();

    pBLEScan->setActiveScan(true);
    pBLEScan->setInterval(100);
    pBLEScan->setWindow(99);

    Serial.println("BLE Scanner Initialized");
}

void loop()
{
    Serial.println("Loop Started");
    scanNetworks();
    Serial.println("Scan Finished");
    scanBLE();
    delay(WIFI_SCAN_INTERVAL);
}

/******************************************************
 * Scan nearby Wi-Fi Networks
 ******************************************************/
void scanNetworks1()
{
    display.clearNetworkArea();
    display.drawColumnHeader();
    int numberOfNetworks = WiFi.scanNetworks();

    display.drawFooter(numberOfNetworks);

    if (numberOfNetworks == 0)
    {
        return;
    }

    for (int i = 0; i < numberOfNetworks && i < MAX_VISIBLE_NETWORKS; i++)
    {
        String ssid = WiFi.SSID(i);

        if (ssid == "")
            ssid = "<Hidden>";

        int rssi = WiFi.RSSI(i);

        int channel = WiFi.channel(i);

        String encryption = getEncryptionType(
                                WiFi.encryptionType(i));

        display.drawNetwork(
            i,
            ssid,
            rssi,
            channel,
            encryption);
    }

    WiFi.scanDelete();
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

    // display.drawNetwork(
    //     0,
    //     "DigitalLab",
    //     -42,
    //     11,
    //     "WPA2");
   // display.drawMode("Wi-Fi");
    display.drawFooter(1);
}

void scanNetworks()
{
    WiFi.scanDelete();

    int numberOfNetworks = WiFi.scanNetworks(false, true);

    display.clearNetworkArea();
    display.drawHeader("Wi-Fi");
    display.drawColumnHeader();

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
 * Scan nearby BLE devices
 ******************************************************/
void scanBLE()
{
    Serial.println();
    Serial.println("============================================");
    Serial.println("Starting BLE Scan...");
    Serial.println("============================================");

    BLEScanResults* foundDevices =
        pBLEScan->start(BLE_SCAN_TIME, false);

    int deviceCount = foundDevices->getCount();

    Serial.print("BLE Devices Found : ");
    Serial.println(deviceCount);
    display.drawHeader("BLE");
    display.drawBLEHeader();
    // Clear previous BLE screen
    display.clearBLEArea();

    // Draw BLE column header
    display.drawBLEHeader();

    // Display BLE devices
    int visibleDevices = min(
        deviceCount,
        MAX_VISIBLE_BLE_DEVICES
    );

    for (int i = 0; i < visibleDevices; i++)
    {
        BLEAdvertisedDevice device =
            foundDevices->getDevice(i);

        String name;

        if (device.haveName())
        {
            name = device.getName().c_str();
        }
        else
        {
            name = "<Unknown>";
        }

        int rssi = device.getRSSI();

        Serial.print("Device ");
        Serial.print(i + 1);
        Serial.print(" : ");
        Serial.print(name);
        Serial.print(" | RSSI: ");
        Serial.println(rssi);

        display.drawBLEDevice(
            i,
            name,
            rssi
        );
    }

    display.drawBLEFooter(deviceCount);

    pBLEScan->clearResults();

    Serial.println("BLE Scan Finished");
}