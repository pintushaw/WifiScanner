#ifndef CONFIG_H
#define CONFIG_H

/***************************************************
 * Project Information
 ***************************************************/
#define PROJECT_NAME        "ESP32 WiFi Scanner"
#define PROJECT_VERSION     "1.0.5"

/***************************************************
 * Display Configuration
 ***************************************************/
#define SCREEN_WIDTH        320
#define SCREEN_HEIGHT       240

#define DISPLAY_ROTATION    3
#define DISPLAY_INVERT      false

/***************************************************
 * Layout
 ***************************************************/
#define HEADER_HEIGHT       35
#define COLUMN_HEIGHT       22
#define FOOTER_HEIGHT       35

#define HEADER_Y            0
#define COLUMN_Y            HEADER_HEIGHT
#define LIST_Y              (HEADER_HEIGHT + COLUMN_HEIGHT)
#define FOOTER_Y            (SCREEN_HEIGHT - FOOTER_HEIGHT)

/***************************************************
 * WiFi
 ***************************************************/
#define WIFI_SCAN_INTERVAL  10000
#define MAX_VISIBLE_NETWORKS 8
#define MAX_VISIBLE_BLE_DEVICES 8
/*******************************
* Home button Co-ordinates
*******************************/

#define HOME_X  250
#define HOME_Y  210
#define HOME_W  62
#define HOME_H  25
#endif