#ifndef NAOBI_CODE_GENERATOR_HPP
#define NAOBI_CODE_GENERATOR_HPP

#include <vector>
#include <string>
#include <map>
#include <memory>
#include <functional>

#include <naobi/interpreter/command.hpp>
#include <naobi/compiler/rule.hpp>

namespace naobi
{
	class code_generator
	{
	public:
		std::vector<naobi::command> generate(const std::vector<std::string>& line);

	public:
		static naobi::command createCommand(command::names name, const command::argumentsList& arguments);

	private:
		using generatorRule = naobi::rule<std::function<bool(const std::vector<std::string>& words)>,
				std::function<void(const std::vector<std::string>& words, std::vector<naobi::command>& commands)>>;
		static std::vector<generatorRule> _generatorRules;

		static std::map<command::names, command::implementation> _commands;
	};
}

#endif //NAOBI_CODE_GENERATOR_HPP
