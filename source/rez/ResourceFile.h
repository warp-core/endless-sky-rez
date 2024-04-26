
#include "ResourceType.h"

#include <vector>

class ResourceFileStream;

class ResourceFile {
public:
	ResourceFile() = delete;
	ResourceFile(const std::string &path);

	std::vector<ResourceType>::const_iterator begin() const;
	std::vector<ResourceType>::const_iterator end() const;


private:
	void Read(ResourceFileStream &data);


private:
	std::vector<ResourceType> types;
};
