#ifndef NAOBI_TYPE_HPP
#define NAOBI_TYPE_HPP

#include <string>
#include <variant>


namespace naobi::utils
{
	class type
	{
	public:
		type() = delete;

		using variable_type = std::variant<long long, bool, std::string, double>;

		enum class names
		{
			INTEGER,
			BOOLEAN,
			STRING,
			FLOAT,
			UNDEFINED,
		};

		static bool isStandardType(const std::string& type);

		static names fromStringToName(const std::string& type);

		static std::string fromNameToString(const names& name);

		static variable_type getValueFrom(names type, const std::string& string);

		static names toType(const std::string& string);

		static names checkType(const std::string& string);

		static names checkTypeFromInput(const std::string& string);

		static bool validate(const std::string& string, names type);

		static bool isLiteral(const std::string& string);

		static bool isInteger(const std::string& string);

		static bool isBoolean(const std::string& string);

		static bool isString(const std::string& string);

		static bool isFloat(const std::string& string);
	};
}

#endif //NAOBI_TYPE_HPP
