#ifndef NAOBI_TYPE_HPP
#define NAOBI_TYPE_HPP

#include <string>
#include <variant>
#include <vector>

#include <naobi/utils/type/array_type.hpp>


namespace naobi::utils::type
{
	using variable_type = std::variant<long long, bool, std::string, double, utils::type::array_type>;

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

		bool operator ==(const type& rhs) const;

		bool operator !=(const type& rhs) const;

		bool operator <(const type& rhs) const;

		bool operator >(const type& rhs) const;

		bool operator <=(const type& rhs) const;

		bool operator >=(const type& rhs) const;

		names name{};
		std::vector<type> detail;
	};

	bool isStandardType(const std::string& type);

	names fromStringToName(const std::string& type);

	std::string fromNameToString(const names& name);

	variable_type getValueFrom(names type, const std::string& string);

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
