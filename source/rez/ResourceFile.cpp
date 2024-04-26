#include "ResourceFile.h"

#include "../Files.h"
#include "ResourceFileStream.h"


#include <cstdint>
#include <string>

using namespace std;

namespace {
	const string MAP_NAME = "resource.map";
	const uint32_t REZ_SIGNATURE = 'BRGR';
	const uint32_t REZ_VERSION = 1;
	const uint32_t RESOURCE_OFFSET_LENGTH = 12;
	const uint32_t MAP_HEADER_LENGTH = 8;
	const uint32_t TYPE_INFO_LENGTH = 12;
	const uint32_t RESOURCE_INFO_LENGTH = 266;

	const uint32_t BASE_HEADER_LENGTH = 12 + static_cast<uint32_t>(MAP_NAME.size() + 1);

	bool ParseResource(ResourceFileStream &data, ResourceType &type,
			const vector<uint64_t> &offsets, const vector<uint64_t> &sizes, const int firstIndex)
	{
		uint32_t index = data.ReadLong();

		char resourceCode[4];
		for(int i = 0; i < 4; ++i)
			resourceCode[i] = data.ReadByte();
		for(int i = 0; i < 4; ++i)
			if(resourceCode[i] != type.GetCode()[i])
				return false;

		int16_t id = data.ReadShort();
		size_t nextOffset = data.GetPosition() + 256;
		string name = data.ReadCString();

		// Go to the location of this resource's data and read it.
		data.SetPosition(offsets[index - firstIndex]);
		vector<char> resourceData = data.ReadBytes(sizes[index - firstIndex]);

		type.AddResource(Resource(id, name, resourceData));

		data.SetPosition(nextOffset);

		return true;
	}

	bool ParseTypes(vector<ResourceType> &types, ResourceFileStream &data, const vector<uint64_t> &offsets,
			const vector<uint64_t> &sizes, const size_t mapOffset, const int firstIndex)
	{
		ResourceType &type = types.emplace_back();
		char typeCode[4];
		for(int i = 0; i < 4; ++i)
			typeCode[i] = data.ReadByte();
		type.SetCode(typeCode);
		uint64_t typeOffset = data.ReadLong();
		uint64_t count = data.ReadLong();

		const size_t savedPosition = data.GetPosition();
		data.SetPosition(mapOffset + typeOffset);
		for(int j = 0; j < count; ++j)
			if(!ParseResource(data, type, offsets, sizes, firstIndex))
				return false;

		data.SetPosition(savedPosition);

		return true;
	}
}

ResourceFile::ResourceFile(const string &path)
{
	vector<char> data = Files::ReadRaw(path);
	ResourceFileStream fileStream(data);
	Read(fileStream);
}

vector<ResourceType>::const_iterator ResourceFile::begin() const
{
	return types.begin();
}

vector<ResourceType>::const_iterator ResourceFile::end() const
{
	return types.end();
}

void ResourceFile::Read(ResourceFileStream &data)
{
	// Configure reading mode.
	data.SetEndian(ResourceFileStream::ParseMode::BIG_ENDIAN);
	data.SetMode(ResourceFileStream::Mode::ADVANCE);

	if(data.ReadLong() != REZ_SIGNATURE)
		return;

	data.SetEndian(ResourceFileStream::ParseMode::LITTLE_ENDIAN);
	if(data.ReadLong() != REZ_VERSION)
		return;

	// Read the header.
	uint32_t headerLength = data.ReadLong();
	data.ReadLong();
	uint32_t firstIndex = data.ReadLong();
	uint32_t count = data.ReadLong();
	uint32_t expectedHeaderLength = (count * RESOURCE_OFFSET_LENGTH) + BASE_HEADER_LENGTH;
	if(headerLength != expectedHeaderLength)
		return;

	vector<uint64_t> offsets;
	vector<uint64_t> sizes;
	offsets.reserve(count);
	sizes.reserve(count);
	for(int it = 0; it < count; ++it)
	{
		offsets.push_back(static_cast<uint64_t>(data.ReadLong()));
		sizes.push_back(static_cast<uint64_t>(data.ReadLong()));
		data.ReadLong();
	}

	// Read the resource map.
	if(data.ReadCString() != MAP_NAME)
		return;

	data.SetEndian(ResourceFileStream::ParseMode::BIG_ENDIAN);
	const int64_t mapOffset = offsets.back();
	data.SetPosition(mapOffset);
	data.ReadLong();
	uint32_t typeCount = data.ReadLong();
	for(int i = 0; i < typeCount; ++i)
		if(!ParseTypes(types, data, offsets, sizes, mapOffset, firstIndex))
			return;
}
