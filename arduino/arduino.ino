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


	FileController::Singleton()->readLines(0x3AA2D0AF, 0, 2);
	auto numbers_of_lines = 60;
	for (int i = 0; i < 240; i)
	{
		auto drawData = FileController::Singleton()->readLines(0x3AA2D0AF, i, numbers_of_lines);

		for (int y = 0; y < numbers_of_lines; y++)
			TFTController::Singleton()->drawLine(i + y, (uint16_t*)drawData.data() + (y * 240));

		i += numbers_of_lines;
	}

}

void loop() {

	
}