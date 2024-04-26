#include "Resource.h"

using namespace std;

string Resource::IDToString(int16_t id)
{
	if(id < 0)
		return "";
	string result;
	while(id)
	{
		result = static_cast<char>('0' + (id % 10)) + result;
		id /= 10;
	}
	return result;
}

Resource::Resource(int64_t id, std::string name, std::vector<char> data)
	: id(id), name(name), data(data)
{
}

int64_t Resource::ID() const
{
	return id;
}

const string &Resource::Name() const
{
	return name;
}

const vector<char> &Resource::Data() const
{
	return data;
}
