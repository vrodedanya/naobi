#ifndef NAOBI_FUNCTION_HPP
#define NAOBI_FUNCTION_HPP

#include <string>
#include <memory>
#include <optional>

#include "naobi/utils/command.hpp"
#include "naobi/utils/type.hpp"


namespace naobi
{
	class function
	{
	public:
		using uptr = std::unique_ptr<function>;
		using sptr = std::shared_ptr<function>;
		using argument_type = std::pair<std::string, utils::type::type>;

	public:
		explicit function(std::string name);

		[[nodiscard]] std::string name() const
		{ return _name; }

		[[nodiscard]] std::size_t getNumber() const;

		void setNumber(std::size_t number);

		bool addArgument(const std::string& name, const utils::type::type& type);

		std::optional<argument_type> getArgument(const std::string& name);

		std::optional<argument_type> getArgument(std::size_t pos);

		[[nodiscard]] const utils::type::type& getReturnType() const;

		void setReturnType(const utils::type::type& returnType);

		void setArguments(const std::vector<argument_type>& arguments);

		[[nodiscard]] const std::vector<argument_type>& getArguments() const;

		void setCommands(const std::vector<naobi::command>& commands);

		std::vector<naobi::command>& commands()
		{ return _commands; }

		[[nodiscard]] const std::vector<naobi::command>& commands() const
		{ return _commands; }

		friend std::ostream& operator <<(std::ostream& stream, const function& function);

		friend const std::ostream& operator >>(const std::ostream& stream, function& function);

	private:
		std::vector<naobi::command> _commands;
		std::vector<argument_type> _arguments;
		utils::type::type _returnType{utils::type::names::UNDEFINED};
		std::string _name;
		std::size_t _number{};
	};

	std::ostream& operator << (std::ostream& stream, const function& function);
	const std::ostream& operator >> (const std::ostream& stream, function& function);
}

#endif //NAOBI_FUNCTION_HPP
