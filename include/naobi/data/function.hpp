#ifndef NAOBI_FUNCTION_HPP
#define NAOBI_FUNCTION_HPP

#include <string>
#include <memory>
#include "naobi/interpreter/command.hpp"

namespace naobi
{
	class function
	{
	public:
		using uptr = std::unique_ptr<function>;
		using sptr = std::shared_ptr<function>;

	public:
		explicit function(std::string name);

		[[nodiscard]] std::string name() const {return _name;}

		void setCommands(const std::vector<naobi::command>& commands);

		std::vector<naobi::command>& commands(){return _commands;}
		[[nodiscard]] const std::vector<naobi::command>& commands() const {return _commands;}

	private:
		std::vector<naobi::command> _commands;
		std::string _name;
	};
}

#endif //NAOBI_FUNCTION_HPP
