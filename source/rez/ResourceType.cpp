#include "ResourceType.h"

#include <map>

using namespace std;

namespace {
	const string &CodeString(const char *charCode)
	{
		auto GetMapVector = []() -> const vector<pair<const char *, string>> &
		{
			static const char syst[4] = {115, -40, 115, 116};
			static const char cher[4] = {99, 104, -118, 114};
			static const char govt[4] = {103, -102, 118, 116};
			static const char junk[4] = {106, -97, 110, 107};
			static const char misn[4] = {109, -107, 115, 110};
			static const char ship[4] = {115, 104, -107, 112};
			static const char csum[4] = {99, 115, -97, 109};
			static const char dude[4] = {100, -97, 100, 101};
			static const char spob[4] = {115, 112, -102, 98};
			static const char oops[4] = {-102, 111, 112, 115};
			static const char rank[4] = {114, -118, 110, 107};
			static const char cron[4] = {99, 114, -102, 110};
			static const char desc[4] = {100, -111, 115, 99};
			static const char PICT[4] = {80, 73, 67, 74};
			static const char outf[4] = {111, -97, 116, 102};
			static const char pers[4] = {112, -111, 114, 115};
			static const char weap[4] = {119, -111, 97, 112};
			static const char STRnum[4] = {83, 84, 82, 35};
			static const char nebu[4] = {110, -111, 98, 117};
			static const char roid[4] = {114, -102, 105, 100};
			static const char boom[4] = {98, -102, -102, 109};
			static const char flet[4] = {102, 108, -111, 116};
			static const vector<pair<const char *, string>> result = {
				{syst, "syst"},
				{cher, "char"},
				{govt, "govt"},
				{junk, "junk"},
				{misn, "misn"},
				{ship, "ship"},
				{csum, "csum"},
				{dude, "dude"},
				{spob, "spob"},
				{oops, "oops"},
				{rank, "rank"},
				{cron, "cron"},
				{desc, "desc"},
				{PICT, "PICT"},
				{outf, "outf"},
				{pers, "pers"},
				{weap, "weap"},
				{STRnum, "STR#"},
				{nebu, "nebu"},
				{roid, "roid"},
				{boom, "boom"},
				{flet, "flet"}
			};
			return result;
		};

		static const string empty = "";
		static const auto &stringCodes = GetMapVector();

		auto findString = [charCode](const pair<const char *, string> &element) -> bool
		{
			for(int i = 0; i < 4; ++i)
				if(element.first[i] != charCode[i])
					return false;
			return true;
		};

		auto it = find_if(stringCodes.begin(), stringCodes.end(), findString);
		if(it != stringCodes.end())
			return it->second;
		return empty;
	}
}

void ResourceType::SetCode(char in[4])
{
	for(int i = 0; i < 4; ++i)
		code[i] = in[i];
	codeASCII = CodeString(code);
}

void ResourceType::AddResource(Resource toAdd)
{
	resources.emplace_back(toAdd);
}

const char *ResourceType::GetCode() const
{
	return code;
}

const string &ResourceType::GetCodeString() const
{
	return codeASCII;
}

vector<Resource>::const_iterator ResourceType::begin() const
{
	return resources.begin();
}

vector<Resource>::const_iterator ResourceType::end() const
{
	return resources.end();
}
