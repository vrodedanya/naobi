#ifndef NAOBI_VARIABLE_HPP
#define NAOBI_VARIABLE_HPP

#include <string>
#include <utility>
#include <memory>
#include <variant>

#include "naobi/utils/type.hpp"


namespace naobi
{
	class variable
	{
	public:
		using uptr = std::unique_ptr<variable>;
		using sptr = std::shared_ptr<variable>;

	public:
		variable(std::string name, utils::type::names type);

		variable& operator =(const variable& var);

		[[nodiscard]] std::string name() const
		{ return _name; }

		[[nodiscard]] utils::type::names& type()
		{ return _type; }

		[[nodiscard]] const utils::type::names& type() const
		{ return _type; }

		utils::type::variable_type& value()
		{ return _value; }

		[[nodiscard]] const utils::type::variable_type& value() const
		{ return _value; }

		naobi::variable::sptr copy();

	private:
		friend std::ostream& operator <<(std::ostream& os, const naobi::variable& var);

		std::string _name;
		utils::type::variable_type _value{};
		utils::type::names _type{utils::type::names::INTEGER};
	};

	naobi::variable::sptr operator +=(naobi::variable::sptr& variable1, const naobi::variable::sptr& variable2);

	naobi::variable::sptr operator -=(naobi::variable::sptr& variable1, const naobi::variable::sptr& variable2);

	naobi::variable::sptr operator *=(naobi::variable::sptr& variable1, const naobi::variable::sptr& variable2);

	naobi::variable::sptr operator /=(naobi::variable::sptr& variable1, const naobi::variable::sptr& variable2);

	naobi::variable::sptr operator %=(naobi::variable::sptr& variable1, const naobi::variable::sptr& variable2);

	naobi::variable::sptr operator >(const naobi::variable::sptr& variable1, const naobi::variable::sptr& variable2);

	naobi::variable::sptr operator <(const naobi::variable::sptr& variable1, const naobi::variable::sptr& variable2);

	naobi::variable::sptr operator >=(const naobi::variable::sptr& variable1, const naobi::variable::sptr& variable2);

	naobi::variable::sptr operator <=(const naobi::variable::sptr& variable1, const naobi::variable::sptr& variable2);

	naobi::variable::sptr operator !=(const naobi::variable::sptr& variable1, const naobi::variable::sptr& variable2);

	naobi::variable::sptr operator ==(const naobi::variable::sptr& variable1, const naobi::variable::sptr& variable2);

	bool operator ==(const naobi::variable::sptr& var1, bool var2);

	bool operator !=(const naobi::variable::sptr& var1, bool var2);


	std::ostream& operator <<(std::ostream& os, const naobi::variable& var);

}

#endif //NAOBI_VARIABLE_HPP
