#ifndef NAOBI_CODE_GENERATOR_HPP
#define NAOBI_CODE_GENERATOR_HPP

#include <vector>
#include <string>
#include <map>
#include <memory>
#include <functional>

#include <naobi/interpreter/command.hpp>

namespace naobi
{
	class code_generator
	{
	public:
		std::vector<naobi::command> generate(const std::vector<std::string>& line);

	private:
		static naobi::command createCommand(command::names name, const command::argumentsList& arguments);

	private:
		static std::map<command::names, command::implementation> _commands;
	};
}

#endif //NAOBI_CODE_GENERATOR_HPP
