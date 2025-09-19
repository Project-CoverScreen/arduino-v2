#pragma once
#include "../packet_base.h"
#include "../../file_controller.h"
#pragma pack(push, 1)
#include <vector>
#include <unordered_map>
#pragma pack(push, 1)
struct FileWritePacket : PacketBase {
	uint32_t file_hash;
	uint16_t line_data[240]; // 240 pixels
};
struct FileWriteResponsePacket : PacketBase {
	bool success; // 0 = no, 1 = yes
};
#pragma pack(pop)

static_assert(sizeof(FileWritePacket) == sizeof(PacketBase) + sizeof(unsigned int) + sizeof(uint16_t) * 240, "FileWritePacket size is not 490 bytes");
static constexpr size_t kLineBytes = sizeof(uint16_t) * 240;           // 480
static constexpr size_t kImageBytes = kLineBytes * 240;                  // 115200

inline std::unordered_map<uint32_t, std::vector<uint8_t>> file_write_buffers;


class FileStartWritePacketHandler {
private:
	FileWritePacket* packet;
	bool is_success = false;
public:
	FileStartWritePacketHandler(FileWritePacket* pkt) : packet(pkt) {};
	FileStartWritePacketHandler* handle()
	{
		auto& buf = file_write_buffers[packet->file_hash];
		if (buf.empty()) {
			buf.reserve(kImageBytes);
		}

		const uint8_t* lineBytes = reinterpret_cast<const uint8_t*>(packet->line_data);
		buf.insert(buf.end(), lineBytes, lineBytes + kLineBytes);

		if (buf.size() >= kImageBytes) {
			is_success = FileController::Singleton()->appendToFile(packet->file_hash,
				buf.data(),
				buf.size());
			file_write_buffers.erase(packet->file_hash);
		}
		else {
			is_success = false;
		}

		return this;
	}

	std::vector<uint8_t> serialize()
	{
		FileWriteResponsePacket response;
		response.magic = 'LFR ';
		response.direction = DIRECTION_ESP_TO_DEVICE;
		response.type = PacketType::PACKET_FILE_WRITE_RESPONSE;
		response.success = this->is_success;

		std::vector<uint8_t> vector;
		vector.resize(sizeof(FileWriteResponsePacket));
		memcpy(vector.data(), &response, sizeof(FileWriteResponsePacket));
		return {};
	}
};
