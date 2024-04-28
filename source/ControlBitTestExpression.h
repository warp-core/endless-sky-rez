#include <string>
#include <vector>

class ConditionsStore;

class ControlBitTestExpression {
public:
	void Load(const char *source, size_t length);
	bool Test(const ConditionsStore &conditions) const;


private:
	std::vector<std::string> expression;
};
