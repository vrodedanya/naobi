#ifndef NAOBI_EXPRESSION_HPP
#define NAOBI_EXPRESSION_HPP

#include <vector>
#include <string>
#include <stack>

#include "command.hpp"
#include <naobi/utils/type.hpp>
#include <naobi/utils/operation.hpp>


namespace naobi
{
	class code_generator;

	class expression
	{
	public:
		explicit expression(std::vector<std::string>&& words, code_generator* generator);

		utils::type::type process(std::vector<naobi::command>& commands);

	private:
		void processLiteral(std::vector<std::string>::const_iterator& it, std::vector<naobi::command>& commands);

		void processOperation(std::vector<std::string>::const_iterator& it, std::vector<naobi::command>& commands);

		void processBracket(std::vector<std::string>::const_iterator& it, std::vector<naobi::command>& commands);

		void processFunction(std::vector<std::string>::const_iterator& it, std::vector<naobi::command>& commands);

		void processVariable(std::vector<std::string>::const_iterator& it, std::vector<naobi::command>& commands);

		void popStack(std::vector<naobi::command>& commands);

	private:
		std::vector<std::string> _words;
		code_generator* _generator;

		std::stack<naobi::utils::type::type> _types;
		std::stack<naobi::operation::sptr> _stack;

		bool _isPrevOperator{true};
	};
}

#endif //NAOBI_EXPRESSION_HPP
