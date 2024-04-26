#include "ResourceFileStream.h"

#include <string>

using namespace std;

namespace {
	ResourceFileStream::ParseMode NativeEndian()
	{
		static ResourceFileStream::ParseMode result = ResourceFileStream::ParseMode::DEFAULT;
		if(result == ResourceFileStream::ParseMode::DEFAULT)
		{
			int16_t val = 0x0001;
			int8_t first = *(int8_t *)&val;
			if(first == 0x00)
				result = ResourceFileStream::ParseMode::BIG_ENDIAN;
			else
				result = ResourceFileStream::ParseMode::LITTLE_ENDIAN;
		}
		return result;
	}

	uint16_t SwapEndian(const uint16_t input)
	{
		uint16_t result = input >> 8;
		result |= (input << 8) & 0xFF00;
		return result;
	}

	uint32_t SwapEndian(const uint32_t input)
	{
		uint32_t result = input >> 24;
		result |= (input >> 8) & 0x0000FF00;
		result |= (input << 8) & 0x00FF0000;
		result |= (input << 24) & 0xFF000000;
		return result;
	}

	template <class T>
	T ConvertEndians(T input, ResourceFileStream::ParseMode outMode)
	{
		const uint8_t intersection = static_cast<int>(outMode) ^ static_cast<int>(NativeEndian());
		switch(intersection)
		{
			case 0:
				return input;
			default:
				return SwapEndian(input);
		}
	}
}

ResourceFileStream::ResourceFileStream(vector<char> data)
	: position(0), data(std::move(data))
{
}

uint8_t ResourceFileStream::ReadByte()
{
	uint8_t result = data[position];
	position += static_cast<bool>(mode);
	return result;
}

int8_t ResourceFileStream::ReadSignedByte()
{
	int8_t result = data[position];
	position += static_cast<bool>(mode);
	return result;
}

uint16_t ResourceFileStream::ReadShort()
{
	uint16_t result = *reinterpret_cast<uint16_t *>(&data[position]);
	position += 2 * static_cast<bool>(mode);
	return ConvertEndians(result, parseMode);
}

int16_t ResourceFileStream::ReadSignedShort()
{
	uint16_t result = *reinterpret_cast<uint16_t *>(&data[position]);
	position += 2 * static_cast<bool>(mode);
	return ConvertEndians(result, parseMode);
}

uint32_t ResourceFileStream::ReadLong()
{
	uint32_t result = *reinterpret_cast<uint32_t *>(&data[position]);
	position += 4 * static_cast<bool>(mode);
	return ConvertEndians(result, parseMode);
}

int32_t ResourceFileStream::ReadSignedLong()
{
	uint32_t result = *reinterpret_cast<uint32_t *>(&data[position]);
	position += 4 * static_cast<bool>(mode);
	return ConvertEndians(result, parseMode);
}

vector<char> ResourceFileStream::ReadBytes(const size_t length)
{
	vector<char> result;
	result.reserve(length);
	for(int i = 0; i < length; ++i)
		result.push_back(ReadByte());
	return result;
}

string ResourceFileStream::ReadCString()
{
	auto terminator = std::find(std::next(data.begin(), position), data.end(), 0);
	const size_t length = &*terminator - &data[position];
	string result(&data[position], length);
	position += length * static_cast<bool>(mode);
	return result;
}

size_t ResourceFileStream::GetPosition() const
{
	return position;
}

bool ResourceFileStream::SetPosition(size_t newPosition)
{
	if(newPosition >= data.size())
		return false;
	position = newPosition;
	return true;
}

bool ResourceFileStream::Move(int offset)
{
	size_t newPos = position + offset;
	if(newPos >= data.size())
		return false;
	position = newPos;
	return true;
}

bool ResourceFileStream::SetEndian(ParseMode newParseMode)
{
	switch(newParseMode)
	{
		case ResourceFileStream::ParseMode::DEFAULT:
			return false;
		default:
			break;
	}
	parseMode = newParseMode;
	return true;
}

bool ResourceFileStream::SetMode(Mode newMode)
{
	mode = newMode;
	return true;
}

