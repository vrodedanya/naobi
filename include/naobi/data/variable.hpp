#ifndef NAOBI_VARIABLE_HPP
#define NAOBI_VARIABLE_HPP

#include <string>
#include <utility>
#include <memory>
#include <variant>

namespace naobi
{
	class variable
	{
	public:
		using variable_type = std::variant<int>;
		using uptr = std::unique_ptr<variable>;
		using sptr = std::shared_ptr<variable>;

		enum class Type
		{
			UNDEFINED,
			INTEGER,
		};
	public:
		variable(std::string name, Type type);

		[[nodiscard]] std::string name() const {return _name;}

		[[nodiscard]] Type type() const {return _type;}

		variable_type& value(){return _value;}

	private:
		std::string _name;
		variable_type _value{};
		Type _type{Type::INTEGER};
	};

	naobi::variable::sptr operator += (const naobi::variable::sptr& variable1, const naobi::variable::sptr& variable2);

}

#endif //NAOBI_VARIABLE_HPP
