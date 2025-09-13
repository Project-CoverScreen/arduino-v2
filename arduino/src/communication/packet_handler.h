#pragma once
#include <cstdint>

class PacketHandler {
public:
	bool onPacketReceived(const uint8_t* data, uint16_t length);
};