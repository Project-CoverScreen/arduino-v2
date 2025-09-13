#include <NimBLEDevice.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <SPI.h>
#include "src/bluetooth_controller.h"

#define TFT_CS   10
#define TFT_DC   8
#define TFT_RST  9
#define TFT_BL   1

#define TFT_SCLK 14
#define TFT_MOSI 13

SPIClass spiTFT = SPIClass(HSPI); 

Adafruit_ST7789 tft = Adafruit_ST7789(&spiTFT, TFT_CS, TFT_DC, TFT_RST);
BluetoothController ble_ctrl = BluetoothController();


void setup() {
	Serial.begin(115200);
	sleep(2);
	pinMode(TFT_BL, OUTPUT);
	digitalWrite(TFT_BL, HIGH);
	if (!ble_ctrl.Initialize())
	{
		Serial.println("womp womp; restarting");
		delay(1000);
		ESP.restart();
	}
	spiTFT.begin(TFT_SCLK, -1, TFT_MOSI, TFT_CS); 
	tft.init(240, 240);
	tft.setRotation(0);

}

void loop() {
	// Écran rouge
	tft.fillScreen(ST77XX_RED);
	delay(1000);

	// Écran bleu
	tft.fillScreen(ST77XX_GREEN);
	delay(1000);
}