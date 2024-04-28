#include "ControlBitTestExpression.h"

#include "ConditionsStore.h"
#include "text/Format.h"

#include <set>
#include <stack>

using namespace std;

namespace {
	static const set<char> OPERATORS = {'!', '&', '|'};
	static const set<char> BRACKETS = {'(', ')'};
	static const set<char> PREFIXES = {'b', 'B', 'o', 'O', 'e', 'E', 'g', 'G'};
	static const set<char> ALWAYS_TRUE = {'p', 'P'};

	vector<string> SplitExpression(const string &expression)
	{
		vector<string> result;
		size_t i = 0;
		while(i < expression.size())
		{
			if(PREFIXES.count(expression[i]) || ALWAYS_TRUE.count(expression[i]))
			{
				size_t j = expression.find_first_of(" ()", i);
				if(j == string::npos)
					j = expression.size();
				result.emplace_back(&expression[i], j - i);
				static const map<string, string> CAPITALISE = {
					{"b", "B"},
					{"p", "P"},
					{"o", "O"},
					{"e", "E"},
					{"g", "G"}
				};
				result.back() = Format::Replace(result.back(), CAPITALISE);
				i = j - 1;
			}
			else if(OPERATORS.count(expression[i]) || BRACKETS.count(expression[i]))
				result.emplace_back() += expression[i];
			++i;
		}
		return result;
	}

	bool Evaluate(const vector<string> &expression, const ConditionsStore &conditions)
	{
		stack<bool> operands;
		stack<char> operators;

		function<bool(char)> isOperator = [](char c)
		{
			static const set<char> operators = {'&', '|', '!'};
			return operators.count(c);
		};

		function<void()> performOperations = [&operands, &operators]()
		{
			auto performOperation = [](char op, bool operand1, bool operand2 = false) -> bool
			{
				switch(op) {
					case '&':
						return operand1 && operand2;
					case '|':
						return operand1 || operand2;
					case '!':
						return !operand1;
					default:
						return false;
				}
			};

			char op = operators.top();
			operators.pop();

			bool second = operands.top();
			operands.pop();
			bool first = operands.top();
			operands.pop();
			operands.push(performOperation(op, first, second));
		};

		for(const string &term : expression)
		{
			if(term.empty())
				continue;

			const char &firstChar = term.front();
			if(firstChar == ' ')
				continue;

			if(firstChar == '(')
				operators.push(firstChar);
			else if(firstChar == ')')
			{
				while(!operators.empty() && operators.top() != '(')
					performOperations();
				operators.pop();
				if(!operators.empty() && operators.top() == '!')
				{
					operators.pop();
					operands.top() = !operands.top();
				}
			}
			else if(isOperator(firstChar))
			{
				while(operands.size() > 1 && !operators.empty() && isOperator(operators.top()) && ((firstChar != '!' && firstChar <= operators.top()) || (firstChar == '!' && firstChar < operators.top())))
					performOperations();
				operators.push(firstChar);
			}
			else
			{
				if(ALWAYS_TRUE.count(term[0]))
					operands.push(true);
				else if(PREFIXES.count(term[0]))
					operands.push(conditions.Has(term));
				else
					return false;
				if(!operators.empty() && operators.top() == '!')
				{
					operators.pop();
					operands.top() = !operands.top();
				}
			}
		}

		while(!operators.empty() && operands.size() > 1)
			performOperations();

		return operands.top();
	}
}

void ControlBitTestExpression::Load(const char *source, size_t length)
{
	string result(source);
	if(result.empty())
		return;
	expression = SplitExpression(result);
}

bool ControlBitTestExpression::Test(const ConditionsStore &conditions) const
{
	if(expression.empty())
		return true;
	return Evaluate(expression, conditions);
}
