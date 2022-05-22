#include <naobi/compiler/code_generator.hpp>

#include <cstring>

#include <naobi/utils/logger.hpp>
#include <utility>
#include <naobi/interpreter/workflow_context.hpp>
#include <naobi/utils/keywords.hpp>
#include <naobi/interpreter/event_manager.hpp>


std::vector<naobi::command> naobi::code_generator::generate(std::vector<std::string> lines)
{
	NLOG(code_generator, naobi::logger::BASIC, "Code lines:\n", lines);
	std::vector<naobi::command> commands;

	for (auto it = lines.begin() ; it != lines.end() ; it++)
	{
		auto words = parser::split(*it, parser::isAnyOf(" "), parser::isAnyOf("+-*/=!<>,()"), {}, {{'"', '"'}, {'{','}'}});
		NLOG(code_generator, logger::LOW, "words:\n", words);
		if (words.empty()) continue;

		bool isCompiled = false;
		for (const auto& rule : _generatorRules)
		{
			if (!isCompiled)
			{
				if (words[0] == "if" && (it+1) != lines.end() && (it+1)->substr(0, std::strlen("else")) == "else")
				{
					auto temp = parser::split(*(it + 1), parser::isAnyOf(" "), parser::isAnyOf("+-*/=!<>,()"), {}, {{'"', '"'}, {'{','}'}});
					words.insert(words.end(), temp.begin(), temp.end());
					lines.erase(it + 1);
				}
				isCompiled = rule.checkLineAndRun(words, commands);
			}
			else
			{
				rule.checkLineAndRun(words, commands);
			}
		}
		if (!isCompiled)
		{
			NCRITICAL(compiler.processModule, errors::UNKNOWN_LINE, "CRITICAL failed to identify line:\n", *it);
		}
	}

	return commands;
}


naobi::code_generator::code_generator(naobi::module::sptr module, const std::map<std::string, variable::sptr>& variablesTemp) :
	_module(std::move(module)),
	_variablesTemp(variablesTemp),
	_generatorRules({
	// Variable creating logic
	{[](const std::vector<std::string>& words) -> bool{
		return utils::type::fromStringToName(words[0]) != utils::type::names::UNDEFINED;
	},
	 [this](const std::vector<std::string>& words, std::vector<naobi::command>& commands){
		std::vector<std::string> wordsTemp = words;
		std::for_each(wordsTemp.begin(), wordsTemp.end(), [](auto& elem) {elem = naobi::parser::removeSym(elem, ',');});
		wordsTemp = naobi::parser::removeEmpty(wordsTemp);
		utils::type::names type = utils::type::fromStringToName(wordsTemp[0]);

		auto it = wordsTemp.begin() + 1;
		while (it != wordsTemp.cend())
		{
			if (_variablesTemp.find(*it) != _variablesTemp.cend())
			{
				NCRITICAL(code_generator, errors::ALREADY_EXIST, "CRITICAL '", *it, "' is already exist");
			}
			auto var = std::make_shared<naobi::variable>(*it, type);
			commands.emplace_back(
					naobi::command::createCommand(
							naobi::command::names::NEW, {*it, std::to_string(static_cast<int>(type))}));
			it++;
			if (it == wordsTemp.cend() || *it != "=")
			{
				NCRITICAL(code_generator, errors::WRONG_FORMAT, "CRITICAL '", *(it - 1), "' is not initialized");
			}
			it++;
			if (it == wordsTemp.cend())
			{
				NCRITICAL(code_generator, errors::WRONG_FORMAT, "CRITICAL '", *(it - 1), "' has empty literal");
			}
			if (!utils::type::isLiteral(*it))
			{
				auto varIterator = _variablesTemp.find(*it);
				if (varIterator == _variablesTemp.cend())
				{
					NCRITICAL(code_generator, errors::DOESNT_EXIST, "CRITICAL '", *it, "' doesn't exist");
				}
				if (varIterator->second->type() != type)
				{
					NCRITICAL(code_generator, errors::TYPE_ERROR, "CRITICAL can't assign '",
							 utils::type::fromNameToString(varIterator->second->type()), "' type to '",
							 utils::type::fromNameToString(type), "' type");
				}
				commands.emplace_back(command::createCommand(naobi::command::names::LOAD, {*it}));
			}
			else
			{
				if (utils::type::validate(*it, type))
				{
					std::string value = *it;
					if (type == utils::type::names::STRING)
					{
						value = value.substr(1, value.size() - 2);
					}
					commands.emplace_back(
							naobi::command::createCommand(
									naobi::command::names::PLACE, {std::to_string(static_cast<int>(type)), value})); // todo can be expression
				}
				else
				{
					NCRITICAL(code_generator, errors::TYPE_ERROR, "CRITICAL variable type ('", utils::type::fromNameToString(type),
						"') is not matching literal type which is '", utils::type::fromNameToString(utils::type::checkType(*it)), "'");
				}
			}
			// TODO add assigning value for providing some checks in compile time
			commands.emplace_back(
					naobi::command::createCommand(
							naobi::command::names::SAVE, {var->name()}));
			_variablesTemp[var->name()] = var;
			it++;
		}

	}},
	// If else statement
	{[](const std::vector<std::string>& words) -> bool{
		return words[0] == "if" && words[1] == "(";
	},
	[this](const std::vector<std::string>& words, std::vector<naobi::command>& commands){
		NLOG(code_generator, logger::LOW, "if block:\n", words);
		auto bodyIt = findEndBracket(words.begin() + 1, words.end());
		processExpression(std::vector<std::string>(words.begin() + 2 , bodyIt), commands);

		std::string codeBlock = *(bodyIt + 1);
		auto lines = parser::split(codeBlock.substr(1, codeBlock.size() - 2), parser::isAnyOf(";}"), {}, {{'{','}'}, {'"','"'}});
		lines = parser::removeEmpty(lines);

		auto tempCommands = generate(lines);
		std::size_t tempCommandsSize = tempCommands.size();

		if (*(bodyIt + 2) == "else")
		{
			tempCommandsSize++;
		}

		commands.emplace_back(command::createCommand(naobi::command::names::JUMP_IF, {std::to_string(tempCommandsSize)}));
		for (const auto& command : tempCommands)
		{
			commands.push_back(command);
		}

		if (*(bodyIt + 2) == "else")
		{
			std::string elseBlock = *(bodyIt + 3);
			elseBlock = elseBlock.substr(1, elseBlock.size() - 2);
			NLOG(code_generator, logger::IMPORTANT, "Else block:\n", elseBlock);

			auto elseLines = naobi::parser::split(elseBlock, parser::isAnyOf(";}"), {}, {{'{','}',},{'"','"'}});

			NLOG(code_generator, logger::IMPORTANT, "Else lines:\n", elseLines);

			auto tempElseCommands = generate(elseLines);
			std::size_t tempElseCommandsSize = tempElseCommands.size();
			commands.emplace_back(command::createCommand(naobi::command::names::JUMP, {std::to_string(tempElseCommandsSize)}));
			for (const auto& command : tempElseCommands)
			{
				commands.push_back(command);
			}
		}
	}},
	// For
	{[](const std::vector<std::string>& words) -> bool{
		return words[0] == "for" && words.size() >= 10;
	},
	[this](const std::vector<std::string>& words, std::vector<naobi::command>& commands){
		NLOG(code_generator, logger::LOW, "for:\n", words);
		utils::type::names type = utils::type::fromStringToName(words[1]);

		std::string gen = parser::join(words.begin() + 4, findEndBracket(words.begin() + 4, words.end()) + 1, "");
		NLOG(code_generator, logger::IMPORTANT, "for generator: ", gen);
		auto pairs = parser::split(gen.substr(1, gen.size() - 2), parser::isAnyOf(","), {}, {{'"','"'}, {'{', '}'}, {'(',')'}});
		if (pairs.empty())
		{
			NCRITICAL(code_generator, errors::WRONG_FORMAT, "CRITICAL wrong for generator format: ", gen);
		}

		commands.push_back(command::createCommand(command::names::NEW, {words[2], std::to_string(static_cast<int>(type))}));
		processExpression(parser::split(pairs[0], parser::isAnyOf(" "), parser::isAnyOf("+-*/=!<>,()"), {},
										{{'"', '"'},
										 {'{', '}'}}), commands);
		commands.push_back(command::createCommand(command::names::SAVE, {words[2]}));

		auto var = std::make_shared<naobi::variable>(words[2], type);
		_variablesTemp[var->name()] = var;

		commands.push_back(command::createCommand(command::names::LOAD, {words[2]}));
		int tempSize = static_cast<int>(commands.size());
		processExpression(parser::split(pairs[1], parser::isAnyOf(" "), parser::isAnyOf("+-*/=!<>,()"), {},
										{{'"', '"'},
										 {'{', '}'}}), commands);
		tempSize = static_cast<int>(commands.size()) - tempSize - 1;
		commands.push_back(command::createCommand(command::names::LESS, {}));

		std::string codeBlock = *(findEndBracket(words.begin() + 4, words.end()) + 1);
		NLOG(code_generator.forBlock, logger::BASIC, "for block:\n", codeBlock);

		auto lines = parser::split(codeBlock.substr(1, codeBlock.size() - 2), parser::isAnyOf(";}"), {}, {{'"', '"'},{'{','}'}});
		NLOG(code_generator.forBlock, logger::LOW, "for block lines:\n", lines);

		auto tempCommands = generate(lines);
		commands.push_back(command::createCommand(command::names::JUMP_IF, {std::to_string(tempCommands.size() + 4)}));
		commands.insert(commands.cend(), tempCommands.begin(), tempCommands.end());
		commands.push_back(command::createCommand(command::names::LOAD, {words[2]}));
		commands.push_back(command::createCommand(command::names::INC, {}));
		commands.push_back(command::createCommand(command::names::SAVE, {words[2]}));
		int size = -(8 + static_cast<int>(tempCommands.size()) + tempSize);
		commands.push_back(command::createCommand(command::names::JUMP, {std::to_string(size)}));
	}},
	// Function calling
	{[](const std::vector<std::string>& words) -> bool{
		return words[1] == "(" && std::find(words.begin() + 2, words.end(), ")") != words.end() && !keywords::check(words[0]);
	},
	 [this]([[maybe_unused]]const std::vector<std::string>& words, std::vector<naobi::command>& commands){
		 callFunction(words, commands);
	}},
	// Raise
	{[](const std::vector<std::string>& words) -> bool{
		return words[0] == "arise" && words.size() == 2;
	},
	[]([[maybe_unused]]const std::vector<std::string>& words, std::vector<naobi::command>& commands){
		commands.push_back(command::createCommand(command::names::ARISE, {words[1]}));
	}},
	{[](const std::vector<std::string>& words) -> bool{
		return words[0] == "return" && words.size() >= 2;
	},
	[this]([[maybe_unused]]const std::vector<std::string>& words, std::vector<naobi::command>& commands){
		processExpression(std::vector<std::string>(words.begin() + 1, words.end()), commands);
		commands.emplace_back(command::createCommand(command::names::RETURN,{}));
	}},
	// Create assignment logic (LOW priority )
	{[](const std::vector<std::string>& words) -> bool{
		return words[1] == "=";
	},
	 [this](const std::vector<std::string>& words, std::vector<naobi::command>& commands){
		if (words.size() < 3)
		{
			NCRITICAL(code_generator, errors::WRONG_FORMAT, "CRITICAL bad assignment operator format");
		}

		if (_variablesTemp.find(words[0]) == _variablesTemp.cend())
		{
			NCRITICAL(code_generator, errors::WRONG_FORMAT, "CRITICAL '", words[0], "' doesn't exist");
		}
		processExpression(std::vector<std::string>(words.begin() + 2, words.end()), commands);
		commands.emplace_back(
				naobi::command::createCommand(
						naobi::command::names::SAVE, {words[0]}));
	}},
})
{
}

void
naobi::code_generator::processExpression(const std::vector<std::string> &words, std::vector<naobi::command> &commands)
{
	std::stack<naobi::command> stack;
	NLOG(processExpression, logger::IMPORTANT, "Expression to process:\n", words);
	for (auto it = words.cbegin(); it != words.cend(); it++)
	{
		NLOG(processExpression, logger::IMPORTANT, "Word: ", *it);
		if (!utils::type::isLiteral(*it))
		{
			if (isOperation(*it))
			{
				if ((*it == "=" && *(it + 1) == "=") || *it == "<" || *it == ">" || (*it == "<" && *(it + 1) == "=") ||
					(*it == ">" && *(it + 1) == "=") || (*it == "!" && *(it + 1) == "="))
				{
					while (!stack.empty() && (stack.top().name == naobi::command::names::ADD ||
											  stack.top().name == naobi::command::names::SUB ||
											  stack.top().name == naobi::command::names::MUL ||
											  stack.top().name == naobi::command::names::DIV ||
											  stack.top().name == naobi::command::names::MOD ||
											  stack.top().name == naobi::command::names::EQ ||
											  stack.top().name == naobi::command::names::GREATER ||
											  stack.top().name == naobi::command::names::LESS ||
											  stack.top().name == naobi::command::names::GREATER_OR_EQ ||
											  stack.top().name == naobi::command::names::LESS_OR_EQ ||
											  stack.top().name == naobi::command::names::NOT_EQ
											  ))
					{
						commands.emplace_back(stack.top());
						stack.pop();
					}
					if (*it == "=" && *(it + 1) == "=")
					{
						stack.push(command::createCommand(command::names::EQ, {}));
						it++;
					}
					else if (*it == "<" && *(it + 1) == "=")
					{
						stack.push(command::createCommand(command::names::LESS_OR_EQ, {}));
						it++;
					}
					else if (*it == ">" && *(it + 1) == "=")
					{
						stack.push(command::createCommand(command::names::GREATER_OR_EQ, {}));
						it++;
					}
					else if (*it == "!" && *(it + 1) == "=")
					{
						stack.push(command::createCommand(command::names::NOT_EQ, {}));
						it++;
					}
					else if (*it == ">")
					{
						stack.push(command::createCommand(command::names::GREATER, {}));
					}
					else if (*it == "<")
					{
						stack.push(command::createCommand(command::names::LESS, {}));
					}
				}
				else if (*it == "+" || *it == "-")
				{
					while (!stack.empty() && (stack.top().name == naobi::command::names::ADD ||
											  stack.top().name == naobi::command::names::SUB ||
											  stack.top().name == naobi::command::names::MUL ||
											  stack.top().name == naobi::command::names::DIV ||
											  stack.top().name == naobi::command::names::MOD))
					{
						commands.emplace_back(stack.top());
						stack.pop();
					}
					stack.push(command::createCommand(*it == "+" ? naobi::command::names::ADD : naobi::command::names::SUB, {}));
				}
				else if (*it == "*" || *it == "/" || *it == "%")
				{
					while (!stack.empty() && (stack.top().name == naobi::command::names::MUL ||
											  stack.top().name == naobi::command::names::DIV ||
											  stack.top().name == naobi::command::names::MOD))
					{
						commands.emplace_back(stack.top());
						stack.pop();
					}
					if (*it == "*")
					{
						stack.push(command::createCommand(naobi::command::names::MUL, {}));

					}
					else if (*it == "/")
					{
						stack.push(command::createCommand(naobi::command::names::DIV, {}));
					}
					else if (*it == "%")
					{
						stack.push(command::createCommand(naobi::command::names::MOD, {}));
					}
				}
				else
				{
					NCRITICAL(processExpression, errors::UNKNOWN_OPERATOR, "CRITICAL Bad operator '", *it, "'");
				}
			}
			else if ((it + 1) != words.cend() && *(it + 1) == "(")
			{
				callFunction(std::vector<std::string>(it, findEndBracket(it, words.end()) + 1), commands);
				it = findEndBracket(it, words.end());
			}
			else if (*it == "(" || *it == ")")
			{
				if (*it == "(")
				{
					stack.push(command::createCommand(naobi::command::names::NOPE, {}));
				}
				else
				{
					bool isBracket = false;
					while (!stack.empty())
					{
						if (stack.top().name == naobi::command::names::NOPE)
						{
							stack.pop();
							isBracket = true;
						}
						commands.emplace_back(stack.top());
						stack.pop();
					}
					if (!isBracket)
					{
						NCRITICAL(processExpression, errors::INCORRECT_BRACKETS, "CRITICAL brackets in ", words, " are not correct");
					}
				}
			}
			else
			{
				if (_variablesTemp.find(*it) == _variablesTemp.cend())
				{
					NCRITICAL(processExpression, errors::DOESNT_EXIST, "CRITICAL variable '", *it, "' not found");
				}
				commands.emplace_back(command::createCommand(naobi::command::names::LOAD, {*it}));
			}
		}
		else
		{
			utils::type::names type = utils::type::checkType(*it);
			std::string temp = *it;
			if (type == utils::type::names::STRING)
			{
				temp = temp.substr(1, temp.size() - 2);
			}
			commands.emplace_back(
					command::createCommand(naobi::command::names::PLACE, {std::to_string(static_cast<int>(type)), temp}));
		}
	}
	while (!stack.empty())
	{
		commands.emplace_back(stack.top());
		stack.pop();
	}
}

bool naobi::code_generator::addVariable(const std::string &name, const naobi::variable::sptr& var)
{
	if (_variablesTemp.find(name) != _variablesTemp.cend())
	{
		return false;
	}
	_variablesTemp[name] = var;
	return true;
}

void naobi::code_generator::callFunction(const std::vector<std::string>& functionCallWords, std::vector<command>& commands)
{
	NLOG(code_generator, logger::IMPORTANT, "Function: ", functionCallWords);
	auto it = _module->findFunction(functionCallWords[0]);
	if (it == nullptr)
	{
		NCRITICAL(code_generator, errors::DOESNT_EXIST, "CRITICAL function '", functionCallWords[0], "' not found");
	}
	auto argsString = parser::join(functionCallWords.begin() + 2, functionCallWords.end() - 1, "");
	auto args = parser::split(argsString, parser::isAnyOf(","), {}, {{'(',')'}});
	NLOG(code_generator, logger::IMPORTANT, "function call arguments: ", args);
	if (args.size() != it->getArguments().size())
	{
		NCRITICAL(code_generator, errors::INVALID_ARGUMENT, "CRITICAL number of function '", functionCallWords[0], "' arguments is ",
			it->getArguments().size(), " but provided ", args.size());
	}
	std::size_t pos = 0;
	for (const auto& arg : args)
	{
		auto pair = parser::split(arg, parser::isAnyOf(":"), {}, {{'(',')'}});
		std::string value;
		function::argument_type argInFunction;
		if (pair.size() == 1)
		{
			value = pair[0];
			auto argInFunctionOpt = it->getArgument(pos);
			if (!argInFunctionOpt.has_value())
			{
				NCRITICAL(code_generator, errors::INVALID_ARGUMENT, "CRITICAL argument with position ", pos, " doesn't exist");
			}
			argInFunction = argInFunctionOpt.value();
		}
		else if (pair.size() == 2)
		{
			auto name = pair[0];
			value = pair[1];
			auto argInFunctionOpt = it->getArgument(name);
			if (!argInFunctionOpt.has_value())
			{
				NCRITICAL(code_generator, errors::DOESNT_EXIST, "CRITICAL argument with name ", name, " doesn't exist");
			}
			argInFunction = argInFunctionOpt.value();
		}
		else
		{
			NCRITICAL(code_generator, errors::INVALID_ARGUMENT, "CRITICAL invalid argument ", pair);
			std::exit(EXIT_FAILURE);
		}
		auto valueSplitter = parser::split(value, parser::isAnyOf(" "), parser::isAnyOf("+-*/=!<>,()"), {},
										   {{'"', '"'},
											{'{', '}'}});
		processExpression(valueSplitter, commands);
		commands.emplace_back(command::createCommand(command::names::NEW,
															{argInFunction.first, std::to_string(
																	static_cast<int>(argInFunction.second))}));
		commands.emplace_back(command::createCommand(command::names::SAVE,
															{argInFunction.first}));
		pos++;
	}

	commands.emplace_back(command::createCommand(naobi::command::names::CALL, {functionCallWords[0]}));
}
