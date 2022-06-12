#ifndef NAOBI_VARIABLE_HPP
#define NAOBI_VARIABLE_HPP

#include <string>
#include <utility>
#include <memory>
#include <variant>

#include "naobi/utils/type/type.hpp"


namespace naobi
{
	class variable
	{
	public:
		using uptr = std::unique_ptr<variable>;
		using sptr = std::shared_ptr<variable>;

	public:
		variable(std::string name, utils::type::type type);

		variable& operator =(const variable& var);

		variable& operator =(variable&& var) noexcept;

		[[nodiscard]] std::string name() const
		{ return _name; }

		void setName(const std::string& name)
		{ _name = name; }

		[[nodiscard]] utils::type::type& type()
		{ return _type; }

		[[nodiscard]] const utils::type::type& type() const
		{ return _type; }

		utils::type::variable_type& value()
		{ return _value; }

		[[nodiscard]] const utils::type::variable_type& value() const
		{ return _value; }

		void invert();

		variable::sptr operator [](std::size_t index) const;

		void set(const variable::sptr& sub, std::size_t index);

		[[nodiscard]] std::size_t size() const;

		naobi::variable::sptr copy();

	private:
		friend std::ostream& operator <<(std::ostream& os, const naobi::variable& var);

		std::string _name;
		utils::type::variable_type _value{};
		utils::type::type _type{utils::type::names::INTEGER};
	};

	std::ostream& operator <<(std::ostream& os, const naobi::variable& var);

	bool operator ==(const naobi::variable::sptr& var1, bool var2);

	bool operator !=(const naobi::variable::sptr& var1, bool var2);

}

#endif //NAOBI_VARIABLE_HPP
