#include <string>
#include <vector>

class Resource {
public:
	static std::string IDToString(int16_t id);


public:
	Resource(int64_t id, std::string name, std::vector<char> data);

	int64_t ID() const;
	const std::string &Name() const;
	const std::vector<char> &Data() const;


private:
	int64_t id = 0;
	std::string name;
	std::vector<char> data;
	size_t dataOffset = 0;
};
