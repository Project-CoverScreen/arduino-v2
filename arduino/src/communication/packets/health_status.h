#pragma once
#include "../packet_base.h"
#pragma pack(push, 1)
struct HealthStatusPacket : PacketBase {
	HealthStatus status;
};
#pragma pack(pop)

static_assert(sizeof(HealthStatusPacket) == sizeof(PacketBase) + sizeof(unsigned char), "HealthStatusPacket size is not 7 bytes");

class HealthStatusPacketHandler {
private:
	HealthStatusPacket* packet;
public:
	HealthStatusPacketHandler(HealthStatusPacket* pkt) : packet(pkt) {};
	HealthStatusPacketHandler* handle(HealthStatus status)
	{
		if (status != HEALTH_ASKED)
			return this;
		this->packet->direction = DIRECTION_ESP_TO_DEVICE;
		this->packet->status = status;
		return this;
	}

	InternalPacketAnswer serialize()
	{
		auto allocated_buffer = malloc(sizeof(HealthStatusPacket));
		if(!allocated_buffer)
		{
			Serial.println("Failed to allocate memory for HealthStatusPacket");
			return {0, nullptr};
		}
		memcpy(allocated_buffer, this->packet, sizeof(HealthStatusPacket));

		InternalPacketAnswer answer;
		answer.size = sizeof(HealthStatusPacket);
		answer.buffer = (unsigned char*)allocated_buffer;
		return answer;
	}
};
