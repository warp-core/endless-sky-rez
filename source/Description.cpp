#include "Description.h"

#include "rez/Resource.h"
#include "rez/ResourceFileStream.h"

#include <map>

using namespace std;

namespace {
	enum class DESCRIPTION_TYPE {
		UNRESERVED,
		STELLAR_OBJECT,
		OUTFIT_ITEM,
		MISSION,
		SHIP_CLASS,
		ESCAPE_POD_MESSAGE,
		SHIP_PILOT,
		RESERVED
	};

	const map<int16_t, DESCRIPTION_TYPE> reservedDescriptionIDs = {
		{128, DESCRIPTION_TYPE::STELLAR_OBJECT},
		{2176, DESCRIPTION_TYPE::UNRESERVED},
		{3000, DESCRIPTION_TYPE::OUTFIT_ITEM},
		{3512, DESCRIPTION_TYPE::UNRESERVED},
		{4000, DESCRIPTION_TYPE::MISSION},
		{5000, DESCRIPTION_TYPE::UNRESERVED},
		{13000, DESCRIPTION_TYPE::SHIP_CLASS},
		{13768, DESCRIPTION_TYPE::UNRESERVED},
		{13999, DESCRIPTION_TYPE::ESCAPE_POD_MESSAGE},
		{14000, DESCRIPTION_TYPE::SHIP_PILOT},
		{14768, DESCRIPTION_TYPE::UNRESERVED},
		{32760, DESCRIPTION_TYPE::RESERVED}
	};
}

void Description::Load(const Resource &resource)
{
	int16_t id = resource.ID();
	string name = resource.Name();
	ResourceFileStream data(resource.Data());
	string text = data.ReadCString();
	int16_t graphics = data.ReadSignedShort();
	char movieFile[32];
	for(int i = 0; i < 32; ++i)
		movieFile[i] = data.ReadByte();
	uint16_t flags = data.ReadShort();

	string converted;
	converted.reserve(text.size());
	for(int i = 0; i < text.size(); ++i)
	{
		if(text[i] == '\r')
			converted.push_back('\n');
		else if(text[i] == '\n')
			if(converted.size() && converted.back() == '\n')
				continue;
			else
				converted.push_back('\n');
		else
			converted.push_back(text[i]);
	}
	content = std::move(converted);
}



const string &Description::GetString() const
{
	return content;
}
