/*
  ------------------------------------------------------------
  Project : ESP32 Portable Wi-Fi Scanner
  Episode : 1 - Basic Wi-Fi Scanner
  Author  : DigitalLab.org
  ------------------------------------------------------------
*/

#include <WiFi.h>

#define SCAN_INTERVAL 10000   // Scan every 10 seconds

void printHeader();
void scanNetworks();
void printNetworkInfo(int index);
String getEncryptionType(wifi_auth_mode_t encryption);

void setup()
{
    Serial.begin(115200);

    delay(1000);

    Serial.println();
    Serial.println("--------------------------------------------");
    Serial.println("ESP32 Portable Wi-Fi Scanner");
    Serial.println("Episode 1");
    Serial.println("--------------------------------------------");

    // Put ESP32 in Station mode
    WiFi.mode(WIFI_STA);

    // Disconnect from any previously connected AP
    WiFi.disconnect(true);

    delay(100);
}

void loop()
{
    scanNetworks();

    Serial.println();
    Serial.println("Next Scan in 10 Seconds...");
    Serial.println();

    delay(SCAN_INTERVAL);
}

/******************************************************
 * Scan nearby Wi-Fi Networks
 ******************************************************/
void scanNetworks()
{
    Serial.println("Scanning nearby Wi-Fi networks...");
    Serial.println();

    int numberOfNetworks = WiFi.scanNetworks();

    if (numberOfNetworks == 0)
    {
        Serial.println("No Wi-Fi Networks Found.");
        return;
    }

    Serial.print("Total Networks Found : ");
    Serial.println(numberOfNetworks);

    printHeader();

    for (int i = 0; i < numberOfNetworks; i++)
    {
        printNetworkInfo(i);
    }

    WiFi.scanDelete();
}

/******************************************************
 * Print Table Header
 ******************************************************/
void printHeader()
{
    Serial.println("--------------------------------------------------------------------------------------------");
    Serial.printf("%-3s %-25s %-8s %-8s %-12s %-20s\n",
                  "No",
                  "SSID",
                  "RSSI",
                  "CH",
                  "Security",
                  "BSSID");
    Serial.println("--------------------------------------------------------------------------------------------");
}

/******************************************************
 * Print Network Information
 ******************************************************/
void printNetworkInfo(int index)
{
    String ssid = WiFi.SSID(index);

    if (ssid == "")
        ssid = "<Hidden>";

    int rssi = WiFi.RSSI(index);

    int channel = WiFi.channel(index);

    String encryption = getEncryptionType(WiFi.encryptionType(index));

    String mac = WiFi.BSSIDstr(index);

    Serial.printf("%-3d %-25s %-8d %-8d %-12s %-20s\n",
                  index + 1,
                  ssid.c_str(),
                  rssi,
                  channel,
                  encryption.c_str(),
                  mac.c_str());
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