#ifndef NAOBI_CODE_GENERATOR_HPP
#define NAOBI_CODE_GENERATOR_HPP

#include <vector>
#include <string>
#include <map>
#include <memory>
#include <functional>

#include <naobi/utils/command.hpp>
#include <naobi/utils/rule.hpp>
#include <naobi/utils/expression.hpp>
#include <naobi/data/module.hpp>


namespace naobi
{
	class code_generator
	{
	public:
		using generatorRule = naobi::rule<std::function<bool(const std::vector<std::string>& words)>,
			std::function<void(const std::vector<std::string>& words, std::vector<naobi::command>& commands)>>;

	public:
		explicit code_generator(
			naobi::module::sptr module,
			std::map<std::string, variable::sptr> variablesTemp = std::map<std::string, variable::sptr>());

		std::vector<naobi::command> generate(std::vector<std::string> line);

		bool addVariable(const std::string& name, const variable::sptr& var);

		static std::string methodToFunction(const std::string& method);

	private:
		naobi::utils::type::type
		callFunction(std::vector<std::string> functionCallWords, std::vector<command>& commands);

		bool generateFunction(const std::vector<std::string>& functionCallWords);

		void creatingVariableLogic(const std::vector<std::string>& words, std::vector<naobi::command>& commands);

		void ifElseLogic(const std::vector<std::string>& words, std::vector<naobi::command>& commands);

		void forLogic(const std::vector<std::string>& words, std::vector<naobi::command>& commands);

		void ariseLogic(const std::vector<std::string>& words, std::vector<naobi::command>& commands);

		void insertLogic(const std::vector<std::string>& words, std::vector<naobi::command>& commands);

		void catchLogic(const std::vector<std::string>& words, std::vector<naobi::command>& commands);

		void throwLogic(const std::vector<std::string>& words, std::vector<naobi::command>& commands);

		void assignmentLogic(const std::vector<std::string>& words, std::vector<naobi::command>& commands);

	private:
		naobi::module::sptr _module;
		std::map<std::string, variable::sptr> _variablesTemp;
		std::vector<generatorRule> _generatorRules; // rules to generate code

		friend expression;
	};
}

#endif //NAOBI_CODE_GENERATOR_HPP