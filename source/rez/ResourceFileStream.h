#include <string>
#include <vector>

class ResourceFileStream {
public:
	enum class ParseMode {
		DEFAULT = 0,
		BIG_ENDIAN,
		LITTLE_ENDIAN
	};

	enum class Mode {
		PEEK = 0,
		ADVANCE = 1
	};


public:
	ResourceFileStream() = delete;
	ResourceFileStream(std::vector<char> data);

	uint8_t ReadByte();
	int8_t ReadSignedByte();
	uint16_t ReadShort();
	int16_t ReadSignedShort();
	uint32_t ReadLong();
	int32_t ReadSignedLong();

	std::vector<char> ReadBytes(const size_t length);

	std::string ReadCString();

	bool Move(int offset);

	size_t GetPosition() const;
	bool SetPosition(size_t newPosition);

	bool SetEndian(ParseMode newParseMode);
	bool SetMode(Mode newMode);


private:
	size_t position = 0;
	std::vector<char> data;

	ParseMode parseMode = ParseMode::BIG_ENDIAN;
	Mode mode = Mode::ADVANCE;
};
