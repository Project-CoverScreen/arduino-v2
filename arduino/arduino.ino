#include <FS.h>
#include <FSImpl.h>
#include <vfs_api.h>
#include "src/bluetooth_controller.h"
#include "config.h"
#include "src/file_controller.h"
#include "src/tft_controller.h"


BluetoothController ble_ctrl = BluetoothController();
FileController file_ctrl = FileController();
TFTController tft_ctrl = TFTController();

void error()
{
	Serial.println("womp womp; restarting");
	delay(1000);
	ESP.restart();
}

void setup() {
	Serial.begin(115200);
	sleep(2);
	pinMode(TFT_BL, OUTPUT);
	digitalWrite(TFT_BL, HIGH);
	pinMode(TFT_CS, OUTPUT);
	pinMode(SD_CS, OUTPUT);

	if (!file_ctrl.Initialize())
		return error();

	if (!ble_ctrl.Initialize())
		return error();

	if(!tft_ctrl.Initialize())
		return error();


	auto test = malloc(240 * 2);

	for (int i = 0; i < 240; i++)
	{
		srand(i * 123456789);
		uint16_t val = uint16_t(esp_random() & 0xFFFF);
		for (int y = 0; y < 240; y++)
			((uint16_t*)test)[y] = val;
		tft_ctrl.drawLine(i, (uint16_t*)test);
	}
	free(test);

}

void loop() {

	
}