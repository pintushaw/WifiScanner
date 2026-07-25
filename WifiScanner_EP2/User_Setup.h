#ifndef USER_SETUP_H
#define USER_SETUP_H

// Display Driver
//#define ILI9341_DRIVER
#define ST7789_DRIVER
// ESP32 SPI Pins
#define TFT_MISO 19
#define TFT_MOSI 23
#define TFT_SCLK 18

// Control Pins
#define TFT_CS   5
#define TFT_DC   2
#define TFT_RST  4

// SPI Frequency
#define SPI_FREQUENCY 20000000
#define SPI_READ_FREQUENCY 20000000

// Fonts
#define LOAD_GLCD
#define LOAD_FONT2
#define LOAD_FONT4
#define LOAD_FONT6
#define LOAD_FONT7
#define LOAD_FONT8
#define LOAD_GFXFF

#define SMOOTH_FONT

#endif