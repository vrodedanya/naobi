#ifndef NAOBI_TYPE_HPP
#define NAOBI_TYPE_HPP

#include <string>
#include <variant>
#include <vector>
#include <memory>

namespace naobi
{
	class variable;
}


namespace naobi::utils::type
{
	using float_t = double;
	using integer_t = long long;
	using bool_t = bool;
	using string_t = std::string;
	using array_t = std::vector<std::shared_ptr<variable>>;

	using variable_type = std::variant<integer_t, bool_t, string_t, float_t, array_t>;

	/**
	 * @brief Enums for standard types
	 */
	enum class names
	{
		INTEGER,
		BOOLEAN,
		STRING,
		FLOAT,
		ARRAY,
		UNDEFINED,
	};

	class type
	{
	public:
		type() = default;
		explicit type(names typeName);
		type(names typeName, std::vector<type> typeDetail);

		bool operator ==(const type& rhs) const;

		bool operator !=(const type& rhs) const;

		bool operator <(const type& rhs) const;

		bool operator >(const type& rhs) const;

		bool operator <=(const type& rhs) const;

		bool operator >=(const type& rhs) const;

		names name{};
		std::vector<type> detail;
	};

	type generateType(const std::vector<std::string>& type);

	bool isStandardType(const std::string& type);

	names fromStringToName(const std::string& type);

	std::string fromNameToString(const names& name);

	variable_type getValueFrom(type& type, const std::string& string);

	names toType(const std::string& string);

	names checkType(const std::string& string);

	names checkTypeFromInput(const std::string& string);

	bool validate(const std::string& string, names type);

	bool isLiteral(const std::string& string);

	bool isInteger(const std::string& string);

	bool isBoolean(const std::string& string);

	bool isString(const std::string& string);

	bool isFloat(const std::string& string);

	bool isArray(const std::string& string);
}

#endif //NAOBI_TYPE_HPP
