#include "naobi/utils/type.hpp"
#include <algorithm>


bool naobi::utils::type::isLiteral(const std::string& string)
{
	return isInteger(string) || isBoolean(string) || isString(string) || isFloat(string);
}

bool naobi::utils::type::isInteger(const std::string& string)
{
	if (string.empty()) return false;
	bool isAllNumbers = std::all_of(string.cbegin() + 1, string.cend(), [](const auto& elem)
	{ return isdigit(elem); });
	return isAllNumbers && ((string[0] == '-' && string.size() >= 2) || std::isdigit(string[0]));
}

bool naobi::utils::type::isBoolean(const std::string& string)
{
	return string == "true" || string == "false";
}

bool naobi::utils::type::isString(const std::string& string)
{
	return string.front() == '"' && string.back() == '"';
}

bool naobi::utils::type::isFloat(const std::string& string)
{
	if (string.empty()) return false;

	int dot = 0;
	bool isAllNumbers = std::all_of(string.cbegin() + 1, string.cend(), [&dot](const auto& elem)
	{
		if (elem == '.')
		{
			if (dot == 1) return 0;
			dot++;
			return 1;
		}
		return isdigit(elem);
	});
	return dot != 0 && isAllNumbers && ((string[0] == '-' && string.size() >= 2) || std::isdigit(string[0]));
}

naobi::utils::type::names naobi::utils::type::checkType(const std::string& string)
{
	if (isInteger(string)) return names::INTEGER;
	else if (isBoolean(string)) return names::BOOLEAN;
	else if (isString(string)) return names::STRING;
	else if (isFloat(string)) return names::FLOAT;
	else return names::UNDEFINED;
}

naobi::utils::type::names naobi::utils::type::toType(const std::string& string)
{
	return static_cast<names>(std::stoi(string));
}

std::variant<int, bool, std::string, double>
naobi::utils::type::getValueFrom(naobi::utils::type::names type, const std::string& string)
{
	if (type == type::names::INTEGER) return std::stoi(string);
	else if (type == type::names::BOOLEAN) return (string == "true");
	else if (type == type::names::STRING) return string;
	else if (type == type::names::FLOAT) return std::stod(string);
	else return nullptr;
}

naobi::utils::type::names naobi::utils::type::checkTypeFromInput(const std::string& string)
{
	if (isInteger(string)) return names::INTEGER;
	else if (isBoolean(string)) return names::BOOLEAN;
	else if (isFloat(string)) return names::FLOAT;
	else return names::STRING;
}

naobi::utils::type::names naobi::utils::type::fromStringToName(const std::string& type)
{
	if (type == "integer") return names::INTEGER;
	else if (type == "boolean") return names::BOOLEAN;
	else if (type == "float") return names::FLOAT;
	else if (type == "string") return names::STRING;
	else return names::UNDEFINED;
}

bool naobi::utils::type::validate(const std::string& string, names type)
{
	return checkType(string) == type;
}

std::string naobi::utils::type::fromNameToString(const names& name)
{
	if (name == names::INTEGER) return "integer";
	else if (name == names::BOOLEAN) return "boolean";
	else if (name == names::FLOAT) return "float";
	else if (name == names::STRING) return "string";
	else return "undefined";
}
