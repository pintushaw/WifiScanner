/*
  ------------------------------------------------------------
  Project : ESP32 Portable Wi-Fi Scanner
  Episode : 3 - TFT Wi-Fi Scanner Dashboard
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

// ============================================================
// Battery
// ============================================================

float readBatteryVoltage();
int calculateBatteryPercentage(float voltage);

TouchManager touch;
DisplayManager display;
BLEScan* pBLEScan;

#define BLE_SCAN_TIME 5
unsigned long lastScanTime = 0;
const unsigned long SCAN_INTERVAL = 10000;
unsigned long lastBatteryUpdate = 0;
const unsigned long BATTERY_UPDATE_INTERVAL = 10000;

enum Screen
{
    SCREEN_HOME,
    SCREEN_WIFI,
    SCREEN_BLE,
    SCREEN_BATTERY
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
    Serial.println("Episode 6");
    Serial.println("--------------------------------------------");

    // Put ESP32 in Station mode
    WiFi.mode(WIFI_STA);

    // Disconnect from any previously connected AP
    WiFi.disconnect(true);

    delay(100);

    // Initialize TFT
    display.begin();
    // Initialize Touch
    touch.begin();
    //display.drawDashboard(); // Old display 
    display.drawHome();    // Change in dispay for touch 
    
    // Initialize BLE
    BLEDevice::init("");
    pBLEScan = BLEDevice::getScan();

    
    pBLEScan->setActiveScan(true);
    pBLEScan->setInterval(100);
    pBLEScan->setWindow(99);

    Serial.println("BLE Scanner Initialized");

    // // Initialize TFT
    // display.begin();

    // // Initialize Touch
    // touch.begin();
    // Initialize Battery ADC
    analogSetPinAttenuation(
        BATTERY_ADC_PIN,
        ADC_11db
    );
    Serial.println("Battery ADC Initialized");
    
}

// void loop()
// {
//     Serial.println("Loop Started");
//     scanNetworks();
//     Serial.println("Scan Finished");
//     scanBLE();
//     delay(WIFI_SCAN_INTERVAL);
// }


void loop()
{
    int x;
    int y;
    
    // ====================================================
    // AUTOMATIC SCREEN REFRESH
    // ====================================================

    if (millis() - lastScanTime >= SCAN_INTERVAL)
    {
        if (currentScreen == SCREEN_WIFI)
        {
            Serial.println("Auto refreshing Wi-Fi scan...");

            scanNetworks(false);

            lastScanTime = millis();

            display.drawWiFiNavigation();
        }
        else if (currentScreen == SCREEN_BLE)
        {
            Serial.println("Auto refreshing BLE scan...");

            scanBLE(false);

            lastScanTime = millis();

            display.drawBLENavigation();
        }
    }

    // ====================================================
    // BATTERY SCREEN REFRESH
    // ====================================================

    if (
        currentScreen == SCREEN_BATTERY &&
        millis() - lastBatteryUpdate >= BATTERY_UPDATE_INTERVAL
    )
    {
        if (!isBatteryConnected())
        {
            Serial.println("Battery disconnected");

            display.drawBatteryDisconnected();
        }
        else
        {
            float batteryVoltage = readBatteryVoltage();

            int batteryPercent =
                calculateBatteryPercentage(batteryVoltage);

            Serial.print("Battery Voltage: ");
            Serial.print(batteryVoltage, 2);
            Serial.print(" V | Battery: ");
            Serial.print(batteryPercent);
            Serial.println("%");

            display.drawBattery(
                batteryVoltage,
                batteryPercent
            );
        }

        // Always redraw HOME button
        display.drawBatteryNavigation();

        lastBatteryUpdate = millis();
    }

    // ====================================================
    // TOUCH
    // ====================================================


    if (!touch.getPoint(x, y))
        return;

    Serial.print("Touch X=");
    Serial.print(x);

    Serial.print(" Y=");
    Serial.println(y);

    switch (currentScreen)
    {
        // ====================================================
        // HOME
        // ====================================================

        case SCREEN_HOME:

        // WiFi
        if (
            x >= 10 &&
            x <= 155 &&
            y >= 50 &&
            y <= 105
        )
        {
            Serial.println("WiFi selected");

            currentScreen = SCREEN_WIFI;

            scanNetworks(true);
            
            lastScanTime = millis();

            display.drawWiFiNavigation();
        }

        // BLE
        else if (
            x >= 165 &&
            x <= 310 &&
            y >= 50 &&
            y <= 105
        )
        {
            Serial.println("BLE selected");

            currentScreen = SCREEN_BLE;

            scanBLE(true);
            lastScanTime = millis();
            display.drawBLENavigation();
        }

        // Scan - dummy
        else if (
            x >= 10 &&
            x <= 155 &&
            y >= 125 &&
            y <= 180
        )
        {
            Serial.println("Scan selected - Coming soon");
        }

        // Battery 
        else if (
            x >= 165 &&
            x <= 310 &&
            y >= 125 &&
            y <= 180
        )
        {
            Serial.println("Battery selected");

            currentScreen = SCREEN_BATTERY;

            // Check whether battery is connected
            if (!isBatteryConnected())
            {
                Serial.println("Battery disconnected");

                display.drawBatteryDisconnected();
                display.drawBatteryNavigation();
            }
            else
            {
                float batteryVoltage = readBatteryVoltage();

                int batteryPercent =
                    calculateBatteryPercentage(batteryVoltage);

                Serial.print("Battery Voltage: ");
                Serial.print(batteryVoltage, 2);
                Serial.print(" V | Battery: ");
                Serial.print(batteryPercent);
                Serial.println("%");

                display.drawBattery(
                    batteryVoltage,
                    batteryPercent
                );

                display.drawBatteryNavigation();
            }
        }
        break;
           


        // ====================================================
        // WIFI
        // ====================================================

        case SCREEN_WIFI:

            if (display.isHomeButtonTouched(x, y))
            {
                Serial.println("Returning to HOME");

                currentScreen = SCREEN_HOME;

                display.drawHome();
            }

            break;


        // ====================================================
        // BLE
        // ====================================================

        case SCREEN_BLE:

            if (display.isHomeButtonTouched(x, y))
            {
                Serial.println("Returning to HOME");

                currentScreen = SCREEN_HOME;

                display.drawHome();
            }

            break;
        // ====================================================
        // BATTERY
        // ====================================================

        case SCREEN_BATTERY:

            if (display.isHomeButtonTouched(x, y))
            {
                Serial.println("Returning to HOME");

                currentScreen = SCREEN_HOME;

                display.drawHome();
            }

            break;
    }

    // Wait until finger is released
    while (touch.touched())
    {
        delay(10);
    }

    delay(100);
}

// void loop()
// {
//     Serial.println("LOOP");
//     delay(1000);
// }
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

void scanNetworks(bool showScanning)
{
    //Prepare screen 
    display.clearNetworkArea();
    display.drawHeader("Wi-Fi");
    display.drawColumnHeader();
    display.drawFooter(0);
    display.drawWiFiNavigation();
    
    // Show scanning message
    if (showScanning)
    {
        display.drawMessage("Scanning...");
    }

    Serial.println("Starting Wi-Fi scan...");
   
    WiFi.scanDelete();

    int numberOfNetworks = WiFi.scanNetworks(false, true);

       
    display.drawFooter(numberOfNetworks);
    Serial.print("Networks=");
    Serial.println(numberOfNetworks);
    display.clearNetworkArea();

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
void scanBLE(bool showScanning)
{
    Serial.println();
    Serial.println("============================================");
    Serial.println("Starting BLE Scan...");
    Serial.println("============================================");
    // Prepare BLE Screen 
    display.drawHeader("BLE");
    
    // Clear previous BLE screen
    display.clearBLEArea();

    // Draw BLE column header
    display.drawBLEHeader();

    display.drawBLEFooter(0);
    // Show scanning message
    if (showScanning)
    {
        display.drawMessage("Scanning...");
    }
    
    BLEScanResults* foundDevices =
        pBLEScan->start(BLE_SCAN_TIME, false);

    int deviceCount = foundDevices->getCount();

    Serial.print("BLE Devices Found : ");
    Serial.println(deviceCount);
    display.clearBLEArea();

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

// ============================================================
// Read Battery Voltage
// ============================================================

float readBatteryVoltage()
{
    uint32_t adcVoltage =
        analogReadMilliVolts(BATTERY_ADC_PIN);

    float batteryVoltage =
        (adcVoltage * BATTERY_DIVIDER_RATIO) / 1000.0;

    return batteryVoltage;
}


// ============================================================
// Check if Battery is Connected
// ============================================================

bool isBatteryConnected()
{
    float v1 = readBatteryVoltage();
    delay(20);

    float v2 = readBatteryVoltage();
    delay(20);

    float v3 = readBatteryVoltage();

    float minVoltage = min(v1, min(v2, v3));
    float maxVoltage = max(v1, max(v2, v3));

    Serial.print("Battery Test: ");
    Serial.print(v1, 2);
    Serial.print(" V, ");
    Serial.print(v2, 2);
    Serial.print(" V, ");
    Serial.print(v3, 2);
    Serial.println(" V");

    // Valid Li-ion battery voltage range
    if (minVoltage < 3.0 || maxVoltage > 4.3)
    {
        return false;
    }

    // Detect unstable/floating ADC reading
    if ((maxVoltage - minVoltage) > 0.2)
    {
        return false;
    }

    return true;
}


// ============================================================
// Calculate Battery Percentage
// ============================================================

int calculateBatteryPercentage(float voltage)
{
    // Li-ion battery:
    // 4.20 V = 100%
    // 3.00 V = 0%

    int percentage =
        (int)(((voltage - 3.00) / (4.20 - 3.00)) * 100.0);

    return constrain(
        percentage,
        0,
        100
    );
}
