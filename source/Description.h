#ifndef DESCRIPTION_H_
#define DESCRIPTION_H_

#include <string>

class Resource;

class Description {
public:
	void Load(const Resource &resource);
	std::string GetString() const;


private:
	std::string content;
};

#endif
