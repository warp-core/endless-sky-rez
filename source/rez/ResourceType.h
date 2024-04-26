#include "Resource.h"

#include <map>
#include <string>
#include <vector>

class ResourceType {
public:
	void SetCode(char in[4]);
	void AddResource(Resource toAdd);

	const char *GetCode() const;
	const std::string &GetCodeString() const;

	std::vector<Resource>::const_iterator begin() const;
	std::vector<Resource>::const_iterator end() const;


private:
	char code[4];
	std::string codeASCII;
	std::vector<Resource> resources;
	std::map<std::string, std::string> attributes;
};
