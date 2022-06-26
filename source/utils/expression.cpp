#include <naobi/utils/expression.hpp>

#include <vector>

#include <naobi/utils/logger.hpp>
#include <naobi/utils/parser.hpp>
#include <naobi/compiler/code_generator.hpp>


naobi::expression::expression(std::vector<std::string>&& words, code_generator* generator) : _words(std::move(words)),
																							 _generator(generator)
{
}

naobi::utils::type::type naobi::expression::process(std::vector<naobi::command>& commands)
{
	NLOG(processExpression, logger::IMPORTANT, "Expression to process:\n", _words);
	for (auto it = _words.cbegin() ; it != _words.cend() ; it++)
	{
		NLOG(processExpression, logger::IMPORTANT, "Word: ", *it);
		if (!utils::type::isLiteral(*it))
		{
			if (operation::isOperation(*it))
			{
				processOperation(it, commands);
			}
			else if (*it == "(" || *it == ")")
			{
				processBracket(it, commands);
			}
			else
			{
				_isPrevOperator = false;
				if (std::next(it) != _words.cend() && *(std::next(it)) == "(")
				{
					processFunction(it, commands);
					continue;
				}
				processVariable(it, commands);
			}
		}
		else
		{
			processLiteral(it, commands);
		}
	}
	while (!_stack.empty())
	{
		popStack(commands);
	}
	return _types.top();
}

void
naobi::expression::processLiteral(std::vector<std::string>::const_iterator& it, std::vector<naobi::command>& commands)
{
	_isPrevOperator = false;
	utils::type::type type = utils::type::type(utils::type::checkType(*it));
	std::string temp = *it;
	if (type.name == utils::type::names::STRING)
	{
		temp = temp.substr(1, temp.size() - 2);
	}
	else if (type.name == utils::type::names::ARRAY)
	{
		auto str = parser::removeSym(temp.substr(1, temp.size() - 2), ' ');
		auto args = parser::split(str, parser::isAnyOf(","), {}, {{'"', '"'}});
		type.detail.emplace_back(utils::type::checkType(args[0]));
	}
	commands.emplace_back(
		command::createCommand(
			naobi::command::names::PLACE,
			{std::to_string(static_cast<int>(type.name)), temp}));

	_types.push(type);
}

void
naobi::expression::processOperation(std::vector<std::string>::const_iterator& it, std::vector<naobi::command>& commands)
{
	if (_isPrevOperator)
	{
		if (*it == "-")
		{
			auto operation = operation_manager::get("~");
			// TODO operation manager must support unary operations
			_stack.push(operation);
			return;
		}
		else
		{
			NCRITICAL(processExpression, errors::WRONG_FORMAT, "CRITICAL wrong operator placement");
		}
	}
	std::string op = *it;
	if ((*it == "=" || *it == "<" || *it == ">" || *it == "!") && *(std::next(it)) == "=")
	{
		it++;
		op += *it;
	}
	auto operation = operation_manager::get(op);
	if (operation == nullptr)
	{
		NCRITICAL(processExpression, errors::UNKNOWN_OPERATOR, "CRITICAL Bad operator '", op, "'");
	}
	while (!_stack.empty() && _stack.top() != nullptr && *_stack.top() >= *operation)
	{
		popStack(commands);
	}
	_stack.push(operation);
	_isPrevOperator = true;
}

void
naobi::expression::processBracket(std::vector<std::string>::const_iterator& it, std::vector<naobi::command>& commands)
{
	_isPrevOperator = false;
	if (*it == "(")
	{
		_stack.push(nullptr);
	}
	else
	{
		bool isBracket = false;
		while (!_stack.empty())
		{
			if (_stack.top() == nullptr)
			{
				_stack.pop();
				isBracket = true;
				break;
			}
			popStack(commands);
		}
		if (!isBracket)
		{
			NCRITICAL(processExpression, errors::INCORRECT_BRACKETS, "CRITICAL brackets in ", _words,
					  " are not correct");
		}
	}
}

void
naobi::expression::processFunction(std::vector<std::string>::const_iterator& it, std::vector<naobi::command>& commands)
{
	commands.push_back(command::createCommand(command::names::ALLOCATE, {}));
	auto type = _generator->callFunction(
		std::vector<std::string>(it, std::next(parser::findCloseBracket(it, _words.cend(), "(", ")"))),
		commands);
	if (type.name == utils::type::names::UNDEFINED)
	{
		NCRITICAL(code_generator, errors::TYPE_ERROR, "CRITICAL function return undefined type");
	}
	_types.emplace(type);
	it = parser::findCloseBracket(it, _words.cend(), "(", ")");
}


void
naobi::expression::processVariable(std::vector<std::string>::const_iterator& it, std::vector<naobi::command>& commands)
{
	auto var = _generator->_variablesTemp.find(*it);
	if (var == _generator->_variablesTemp.cend())
	{
		NCRITICAL(processExpression, errors::DOESNT_EXIST, "CRITICAL variable '", *it, "' not found");
	}
	commands.emplace_back(command::createCommand(naobi::command::names::LOAD, {*it}));
	_types.push(var->second->type());
}

void naobi::expression::popStack(std::vector<naobi::command>& commands)
{
	utils::type::type second, first;
	if (_stack.top()->getOperator() == "~")
	{
		second.name = utils::type::names::UNDEFINED;
		first = _types.top();
		_types.pop();
	}
	else
	{
		second = _types.top();
		_types.pop();
		first = _types.top();
		_types.pop();
	}

	auto func = _stack.top()->call(first, second);
	if (func.second == nullptr)
	{
		NCRITICAL(processExpression, errors::TYPE_ERROR, "CRITICAL can't execute operation '",
				  _stack.top()->getOperator(), "' for _types '", utils::type::fromNameToString(first.name),
				  "' and '", utils::type::fromNameToString(second.name), "'");
	}
	commands.emplace_back(command::createCommand(_stack.top()->getCommandAnalogue(), {}));
	_types.push(func.first);
	_stack.pop();
}
