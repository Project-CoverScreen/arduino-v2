#include "packet_handler.h"
#include "packet_base.h"
#include <Arduino.h>

#include "packets/health_status.h"

bool PacketHandler::onPacketReceived(const uint8_t* data, uint16_t length)
{
	if(length < sizeof(PacketBase))
	{
		Serial.println("Packet too small");
		return false;
	}

	auto packet = reinterpret_cast<const PacketBase*>(data);
	if (packet->magic != 'LFR ')
	{
		Serial.println("Invalid packet magic");
		return false;
	}
	InternalPacketAnswer answer = { 0, nullptr };
	switch (packet->type)
	{
	case PacketType::PACKET_HEALTH:
		{
			if (length != sizeof(HealthStatusPacket))
			{
				Serial.println("Invalid HealthStatusPacket size");
				return false;
			}
			auto casted_packet = (HealthStatusPacket*)data;
			answer = (new HealthStatusPacketHandler(casted_packet))->handle(HealthStatus::HEALTH_OK)->serialize();
			Serial.printf("Received HealthStatusPacket: status=%d\n", casted_packet->status);
			break;
		}
	}

	if (answer.size && answer.buffer)
	{
		// i should answer but idk how to yet
		free(answer.buffer);
	}
    return true;
}
