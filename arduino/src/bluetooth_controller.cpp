#include "bluetooth_controller.h"
#include <Arduino.h>

#define SVC_UUID "12345678-1234-1234-1234-1234567890AB"
#define CHAR_UUID "12345678-1234-1234-1234-1234567890AC"
NimBLEServer* server;
NimBLECharacteristic* chrHello;
ServerCallbacks serverCallbacks;
RxCallbacks rxCallbacks;

bool BluetoothController::Initialize() {
	if (!NimBLEDevice::init("FLR"))
	{
		Serial.printf("Failed to initialize BLE device\n");
		return false;
	}
	NimBLEDevice::setPower(ESP_PWR_LVL_P6);
	NimBLEDevice::setMTU(512);
	server = NimBLEDevice::createServer();
	if(!server) {
		Serial.printf("Failed to create BLE server\n");
		return false;
	}
	server->setCallbacks(&serverCallbacks);
	server->advertiseOnDisconnect(true); // relance auto l’advertising après déconnexion

	NimBLEService* svc = server->createService(SVC_UUID);
	if(!svc) {
		Serial.printf("Failed to create BLE service\n");
		return false;
	}
	chrHello = svc->createCharacteristic(
		CHAR_UUID,
		NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::WRITE
	);
	if(!chrHello) {
		Serial.printf("Failed to create BLE characteristic\n");
		return false;
	}
	chrHello->setValue("HELLO");
	chrHello->setCallbacks(&rxCallbacks);
	if(!svc->start()) {
		Serial.printf("Failed to start BLE service\n");
		return false;
	}

	NimBLEAdvertising* adv = NimBLEDevice::getAdvertising();
	adv->addServiceUUID(SVC_UUID);
	adv->setName("Je suis LFR");
	if(!adv->start()) {
		Serial.printf("Failed to start advertising\n");
		return false;
	}
	Serial.printf("BLE initialized. Advertising as 'FLR'\n");
	return true;
}

void ServerCallbacks::onConnect(NimBLEServer* s, NimBLEConnInfo& ci)
{
	Serial.printf("Connected: %s (mtu=%u)\n", ci.getAddress().toString().c_str(), ci.getMTU());
	s->updateConnParams(ci.getConnHandle(), 24, 48, 0, 400);
}

void ServerCallbacks::onDisconnect(NimBLEServer* s, NimBLEConnInfo& ci, int reason)
{
	Serial.printf("Disconnected (%d). Advertising…\n", reason);
	NimBLEDevice::startAdvertising();
}

void ServerCallbacks::onMTUChange(uint16_t mtu, NimBLEConnInfo& ci)
{
	Serial.printf("MTU updated: %u\n", mtu);
}

void RxCallbacks::onWrite(NimBLECharacteristic* chr, NimBLEConnInfo& conn)
{
	NimBLEAttValue v = chr->getValue();
	for(int i = 0; i < v.length(); i++)
	{
		Serial.printf("%02X ", v[i]);
	}
}
