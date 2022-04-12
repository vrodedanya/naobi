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
		using generatorRule = naobi::rule<std::function<bool(const std::vector<std::string>& words)>,
				std::function<void(const std::vector<std::string>& words, std::vector<naobi::command>& commands)>>;

	public:
		explicit code_generator(naobi::module::sptr module, const std::map<std::string, variable::sptr>& variablesTemp = std::map<std::string, variable::sptr>());

		std::vector<naobi::command> generate(std::vector<std::string> line);
		static naobi::command createCommand(command::names name, const command::argumentsList& arguments);

		static bool isOperation(const std::string& string){return std::string("+-*/=%<>!").find(string) != std::string::npos;}

	private:
		void processExpression(const std::vector<std::string>& words, std::vector<naobi::command>& commands);

		template<typename ITERATOR>
		ITERATOR findEndBracket(ITERATOR begin, ITERATOR end);

	private:
		naobi::module::sptr _module;
		std::map<std::string, variable::sptr> _variablesTemp;
		std::vector<generatorRule> _generatorRules; // rules to generate code

		static std::map<command::names, command::implementation> _commands; // commands implementation
	};
}

template <typename ITERATOR>
ITERATOR naobi::code_generator::findEndBracket(ITERATOR begin, ITERATOR end)
{
	int brackets = 0;
	for (auto it = begin ; it != end ; it++)
	{
		if (*it == ")")
		{
			brackets--;
			if (brackets == 0) return it;
		}
		else if (*it == "(") brackets++;
	}
	return end;
}

#endif //NAOBI_CODE_GENERATOR_HPP
