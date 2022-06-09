#include <naobi/compiler/code_generator.hpp>

#include <cstring>

#include <naobi/utils/logger.hpp>
#include <utility>
#include <naobi/interpreter/workflow_context.hpp>
#include <naobi/utils/keywords.hpp>
#include <naobi/interpreter/event_manager.hpp>
#include <naobi/utils/operation.hpp>


std::vector<naobi::command> naobi::code_generator::generate(std::vector<std::string> lines)
{
	NLOG(code_generator, naobi::logger::BASIC, "Code lines:\n", lines);
	std::vector<naobi::command> commands;

	for (auto it = lines.begin() ; it != lines.end() ; it++)
	{
		auto words = parser::split(
			*it, parser::isAnyOf(" "), parser::isAnyOf("+-*/%=!<>,()"), {}, {{'"', '"'},
																			 {'{', '}'}});
		NLOG(code_generator, logger::LOW, "words:\n", words);
		if (words.empty()) continue;

		bool isCompiled = false;
		for (const auto& rule : _generatorRules)
		{
			if (!isCompiled)
			{
				if (words[0] == "if" && (it + 1) != lines.end() && (it + 1)->substr(0, std::strlen("else")) == "else")
				{
					auto temp = parser::split(
						*(it + 1), parser::isAnyOf(" "), parser::isAnyOf("+-*/%=!<>,()"), {},
						{{'"', '"'},
						 {'{', '}'}});
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


naobi::code_generator::code_generator(naobi::module::sptr module, std::map<std::string, variable::sptr> variablesTemp) :
	_module(std::move(module)),
	_variablesTemp(std::move(variablesTemp)),
	_generatorRules(
		{
			// Variable creating logic
			{[](const std::vector<std::string>& words) -> bool
			 {
				 return utils::type::fromStringToName(words[0]) != utils::type::names::UNDEFINED;
			 },
			 [this](
				 const std::vector<std::string>& words,
				 std::vector<naobi::command>& commands)
			 {
				 std::vector<std::string> wordsTemp = words;
				 wordsTemp = naobi::parser::removeEmpty(wordsTemp);
				 utils::type::names type = utils::type::fromStringToName(wordsTemp[0]);

				 auto it = wordsTemp.begin() + 1;
				 while (it != wordsTemp.cend())
				 {
					 if (_variablesTemp.find(*it) != _variablesTemp.cend())
					 {
						 NCRITICAL(code_generator, errors::ALREADY_EXIST, "CRITICAL '", *it,
								   "' is already exist");
					 }
					 auto var = std::make_shared<naobi::variable>(*it, type);
					 commands.emplace_back(
						 naobi::command::createCommand(
							 naobi::command::names::NEW,
							 {*it, std::to_string(static_cast<int>(type))}));
					 it++;
					 if (it == wordsTemp.cend() || *it != "=")
					 {
						 if (it == wordsTemp.cend())
						 {
							 _variablesTemp[var->name()] = var;
							 break;
						 }
						 else if (*it == ",")
						 {
							 it++;
							 _variablesTemp[var->name()] = var;
							 continue;
						 }
						 NCRITICAL(code_generator, errors::WRONG_FORMAT, "CRITICAL '",
								   *(it - 1), "' is not initialized");
					 }
					 it++;
					 if (it == wordsTemp.cend())
					 {
						 NCRITICAL(code_generator, errors::WRONG_FORMAT, "CRITICAL '",
								   *(it - 1), "' has empty literal");
					 }
					 auto next = std::find(it, wordsTemp.end(), ",");
					 auto t = processExpression(
						 std::vector<std::string>(
							 it, std::find(
								 it,
								 wordsTemp.end(),
								 ",")),
						 commands);
					 if (t != type)
					 {
						 NCRITICAL(code_generator, errors::TYPE_ERROR,
								   "CRITICAL can't assign value which type is '",
								   utils::type::fromNameToString(t),
								   "' to variable which type is '",
								   utils::type::fromNameToString(type), "'");
					 }
					 commands.emplace_back(
						 naobi::command::createCommand(
							 naobi::command::names::SAVE, {var->name()}));
					 _variablesTemp[var->name()] = var;
					 it = next;
					 if (it == wordsTemp.end()) break;
					 it++;
				 }

			 }},
			// If else statement
			{[](const std::vector<std::string>& words) -> bool
			 {
				 return words[0] == "if" && words.size() >= 3;
			 },
			 [this](
				 const std::vector<std::string>& words,
				 std::vector<naobi::command>& commands)
			 {
				 NLOG(code_generator, logger::LOW, "if block:\n", words);
				 auto bodyIt = std::find_if(
					 words.begin(), words.end(), [](const std::string& str)
					 {
						 return str[0] == '{';
					 });
				 processExpression(
					 std::vector<std::string>(words.begin() + 1, bodyIt),
					 commands);

				 const std::string& codeBlock = *bodyIt;
				 auto lines = parser::split(
					 codeBlock.substr(1, codeBlock.size() - 2),
					 parser::isAnyOf(";}"), {}, {{'{', '}'},
												 {'"', '"'}});
				 lines = parser::removeEmpty(lines);

				 auto tempCommands = generate(lines);
				 std::size_t tempCommandsSize = tempCommands.size();

				 if ((bodyIt + 1) != words.end() && *(bodyIt + 1) == "else")
				 {
					 tempCommandsSize++;
				 }

				 commands.emplace_back(
					 command::createCommand(
						 naobi::command::names::JUMP_IF,
						 {std::to_string(
							 tempCommandsSize)}));
				 for (const auto& command : tempCommands)
				 {
					 commands.push_back(command);
				 }

				 if ((bodyIt + 1) != words.end() && *(bodyIt + 1) == "else")
				 {
					 std::string elseBlock = *(bodyIt + 2);
					 elseBlock = elseBlock.substr(1, elseBlock.size() - 2);
					 NLOG(code_generator, logger::IMPORTANT, "Else block:\n", elseBlock);

					 auto elseLines = naobi::parser::split(
						 elseBlock, parser::isAnyOf(";}"),
						 {}, {{'{', '}',},
							  {'"', '"'}});

					 NLOG(code_generator, logger::IMPORTANT, "Else lines:\n", elseLines);

					 auto tempElseCommands = generate(elseLines);
					 std::size_t tempElseCommandsSize = tempElseCommands.size();
					 commands.emplace_back(
						 command::createCommand(
							 naobi::command::names::JUMP,
							 {std::to_string(tempElseCommandsSize)}));
					 for (const auto& command : tempElseCommands)
					 {
						 commands.push_back(command);
					 }
				 }
			 }},
			// For
			{[](const std::vector<std::string>& words) -> bool
			 {
				 return words[0] == "for" && words.size() >= 10;
			 },
			 [this](
				 const std::vector<std::string>& words,
				 std::vector<naobi::command>& commands)
			 {
				 NLOG(code_generator, logger::LOW, "for:\n", words);
				 utils::type::names type = utils::type::fromStringToName(words[1]);

				 std::string gen = parser::join(
					 words.begin() + 4,
					 findEndBracket(
						 words.begin() + 4,
						 words.end()) + 1, "");
				 NLOG(code_generator, logger::IMPORTANT, "for generator: ", gen);
				 auto pairs = parser::split(
					 gen.substr(1, gen.size() - 2),
					 parser::isAnyOf(","), {}, {{'"', '"'},
												{'{', '}'},
												{'(', ')'}});
				 if (pairs.empty())
				 {
					 NCRITICAL(code_generator, errors::WRONG_FORMAT,
							   "CRITICAL wrong for generator format: ", gen);
				 }

				 commands.push_back(
					 command::createCommand(
						 command::names::NEW, {words[2],
											   std::to_string(
												   static_cast<int>(type))}));
				 processExpression(
					 parser::split(
						 pairs[0], parser::isAnyOf(" "),
						 parser::isAnyOf("+-*/%=!<>,()"), {},
						 {{'"', '"'},
						  {'{', '}'}}), commands);
				 commands.push_back(
					 command::createCommand(command::names::SAVE, {words[2]}));

				 auto var = std::make_shared<naobi::variable>(words[2], type);
				 _variablesTemp[var->name()] = var;

				 commands.push_back(
					 command::createCommand(command::names::LOAD, {words[2]}));
				 int tempSize = static_cast<int>(commands.size());
				 processExpression(
					 parser::split(
						 pairs[1], parser::isAnyOf(" "),
						 parser::isAnyOf("+-*/%=!<>,()"), {},
						 {{'"', '"'},
						  {'{', '}'}}), commands);
				 tempSize = static_cast<int>(commands.size()) - tempSize - 1;
				 commands.push_back(command::createCommand(command::names::LESS, {}));

				 std::string codeBlock = *(findEndBracket(words.begin() + 4, words.end()) +
										   1);
				 NLOG(code_generator.forBlock, logger::BASIC, "for block:\n", codeBlock);

				 auto lines = parser::split(
					 codeBlock.substr(1, codeBlock.size() - 2),
					 parser::isAnyOf(";}"), {}, {{'"', '"'},
												 {'{', '}'}});
				 NLOG(code_generator.forBlock, logger::LOW, "for block lines:\n", lines);

				 auto tempCommands = generate(lines);
				 commands.push_back(
					 command::createCommand(
						 command::names::JUMP_IF,
						 {std::to_string(
							 tempCommands.size() +
							 4)}));
				 commands.insert(commands.cend(), tempCommands.begin(), tempCommands.end());
				 commands.push_back(
					 command::createCommand(command::names::LOAD, {words[2]}));
				 commands.push_back(command::createCommand(command::names::INC, {}));
				 commands.push_back(
					 command::createCommand(command::names::SAVE, {words[2]}));
				 int size = -(8 + static_cast<int>(tempCommands.size()) + tempSize);
				 commands.push_back(
					 command::createCommand(
						 command::names::JUMP,
						 {std::to_string(size)}));
			 }},
			// Function calling
			{[](const std::vector<std::string>& words) -> bool
			 {
				 return words[1] == "(" &&
						std::find(words.begin() + 2, words.end(), ")") != words.end() &&
						!keywords::check(words[0]);
			 },
			 [this](
				 [[maybe_unused]]const std::vector<std::string>& words,
				 std::vector<naobi::command>& commands)
			 {
				 callFunction(words, commands);
			 }},
			// Raise
			{[](const std::vector<std::string>& words) -> bool
			 {
				 return words[0] == "arise" && words.size() >= 2;
			 },
			 [this](
				 [[maybe_unused]]const std::vector<std::string>& words,
				 std::vector<naobi::command>& commands)
			 {
				 auto eventOpt = this->_module->findEvent(words[1]);
				 if (!eventOpt.has_value())
				 {
					 NCRITICAL(code_generator, errors::DOESNT_EXIST, "CRITICAL event '", words[1], "' doesn't exist");
				 }
				 auto event = eventOpt.value();
				 std::string args = parser::join(
					 words.begin() + 3, findEndBracket(words.begin() + 3, words.end()) - 1, "");
				 auto arguments = parser::split(args, parser::isAnyOf(","));
				 std::size_t pos{};
				 for (const auto& argument : arguments)
				 {
					 auto pair = parser::split(argument, parser::isAnyOf(":"));
					 if (pair.size() == 2)
					 {
						 auto argOpt = event.getArgument(pair[0]);
						 if (!argOpt.has_value())
						 {
							 NCRITICAL(code_generator, errors::DOESNT_EXIST, "CRITICAL event argument '", pair[0],
									   "' doesn't exist");
						 }
						 auto arg = argOpt.value();
						 commands.push_back(
							 command::createCommand(
								 command::names::NEW, {pair[0],
													   std::to_string(
														   static_cast<int>(std::get<1>(arg)))}));
						 auto valueSplitter = parser::split(
							 pair[1], parser::isAnyOf(" "), parser::isAnyOf("+-*/%=!<>,()"), {},
							 {{'"', '"'},
							  {'{', '}'}});
						 auto type = processExpression(valueSplitter, commands);
						 if (type != std::get<1>(arg))
						 {
							 NCRITICAL(code_generator, errors::TYPE_ERROR, "CRITICAL expected type '",
									   utils::type::fromNameToString(std::get<1>(arg)),
									   "' and got '", utils::type::fromNameToString(type), "'");
						 }
						 commands.push_back(
							 command::createCommand(
								 command::names::SAVE, {pair[0]}));
					 }
					 else
					 {
						 auto argOpt = event.getArgument(pos);
						 if (!argOpt.has_value())
						 {
							 NCRITICAL(code_generator, errors::DOESNT_EXIST, "CRITICAL event argument in pos '", pos,
									   "' doesn't exist");
						 }
						 auto arg = argOpt.value();
						 commands.push_back(
							 command::createCommand(
								 command::names::NEW, {std::get<0>(arg),
													   std::to_string(
														   static_cast<int>(std::get<1>(arg)))}));
						 auto valueSplitter = parser::split(
							 pair[0], parser::isAnyOf(" "), parser::isAnyOf("+-*/%=!<>,()"), {},
							 {{'"', '"'},
							  {'{', '}'}});
						 auto type = processExpression(valueSplitter, commands);
						 if (type != std::get<1>(arg))
						 {
							 NCRITICAL(code_generator, errors::TYPE_ERROR, "CRITICAL expected type '",
									   utils::type::fromNameToString(std::get<1>(arg)),
									   "' and got '", utils::type::fromNameToString(type), "'");
						 }
						 commands.push_back(
							 command::createCommand(
								 command::names::SAVE, {std::get<0>(arg)}));
					 }
					 pos++;
				 }

				 commands.push_back(
					 command::createCommand(command::names::ARISE, {words[1]}));
			 }},
			{[](const std::vector<std::string>& words) -> bool
			 {
				 return words[0] == "__insert" && words.size() >= 2;
			 },
			 [](
				 const std::vector<std::string>& words,
				 std::vector<naobi::command>& commands)
			 {
				 auto it = command::stringToCommand.find(words[1]);
				 if (it == command::stringToCommand.end())
				 {
					 NCRITICAL(code_generator, errors::INVALID_ARGUMENT,
							   "CRITICAL failed to find command '", words[1], "'");
				 }
				 commands.push_back(
					 command::createCommand(
						 it->second,
						 std::vector<std::string>(
							 words.begin() + 2,
							 words.end())));
			 }
			},
			{[](const std::vector<std::string>& words) -> bool
			 {
				 return words[0] == "return" && words.size() >= 2;
			 },
			 [this](
				 [[maybe_unused]]const std::vector<std::string>& words,
				 std::vector<naobi::command>& commands)
			 {
				 processExpression(
					 std::vector<std::string>(words.begin() + 1, words.end()),
					 commands);
				 commands.emplace_back(command::createCommand(command::names::RETURN, {}));
			 }},
			{[](const std::vector<std::string>& words) -> bool
			 {
				 return words[0] == "catch" && words.size() == 3;
			 },
			 [this](
				 [[maybe_unused]]const std::vector<std::string>& words,
				 std::vector<naobi::command>& commands)
			 {
				 NLOG(code_generator, logger::IMPORTANT, "catch: ", words);
				 auto except = _module->findException(words[1]);
				 if (!except.has_value())
				 {
					 NCRITICAL(code_generator, errors::DOESNT_EXIST, "CRITICAL exception with name '", words[1],
							   "' doesn't exist");
				 }
				 commands.push_back(command::createCommand(command::names::CATCH, {words[1]}));
				 const std::string& codeBlock = words[2];
				 auto lines = parser::split(
					 codeBlock.substr(1, codeBlock.size() - 2),
					 parser::isAnyOf(";}"), {}, {{'{', '}'},
												 {'"', '"'}});
				 lines = parser::removeEmpty(lines);

				 addVariable(
					 words[1] + ".name",
					 std::make_shared<naobi::variable>(words[1] + ".name", utils::type::names::STRING));
				 addVariable(
					 words[1] + ".description",
					 std::make_shared<naobi::variable>(words[1] + ".description", utils::type::names::STRING));
				 auto tempCommands = generate(lines);
				 std::size_t tempCommandsSize = tempCommands.size();
				 commands.push_back(
					 command::createCommand(command::names::JUMP, {std::to_string(tempCommandsSize)}));
				 commands.insert(commands.end(), tempCommands.begin(), tempCommands.end());
			 }},
			{[](const std::vector<std::string>& words) -> bool
			 {
				 return words[0] == "throw" && words.size() >= 3;
			 },
			 [this](
				 [[maybe_unused]]const std::vector<std::string>& words,
				 std::vector<naobi::command>& commands)
			 {
				 NLOG(code_generator, logger::IMPORTANT, "throw: ", words);
				 auto except = _module->findException(words[1]);
				 if (!except.has_value())
				 {
					 NCRITICAL(code_generator, errors::DOESNT_EXIST, "CRITICAL exception with name '", words[1],
							   "' doesn't exist");
				 }
				 auto type = processExpression(
					 std::vector<std::string>(words.begin() + 2, findEndBracket(words.begin() + 2, words.end())),
					 commands);
				 if (type != utils::type::names::STRING)
				 {
					 NCRITICAL(code_generator, errors::TYPE_ERROR, "CRITICAL exception take only string argument");
				 }
				 commands.push_back(command::createCommand(command::names::THROW, {words[1]}));
			 }},
			// Create assignment logic (LOW priority )
			{[](const std::vector<std::string>& words) -> bool
			 {
				 return words[1] == "=";
			 },
			 [this](
				 const std::vector<std::string>& words,
				 std::vector<naobi::command>& commands)
			 {
				 if (words.size() < 3)
				 {
					 NCRITICAL(code_generator, errors::WRONG_FORMAT,
							   "CRITICAL bad assignment operator format");
				 }

				 auto var = _variablesTemp.find(words[0]);
				 if (var == _variablesTemp.cend())
				 {
					 NCRITICAL(code_generator, errors::WRONG_FORMAT, "CRITICAL '", words[0],
							   "' doesn't exist");
				 }
				 auto type = processExpression(
					 std::vector<std::string>(words.begin() + 2, words.end()), commands);
				 if (type != var->second->type())
				 {
					 NCRITICAL(code_generator, errors::TYPE_ERROR,
							   "CRITICAL can't assign value which type is '",
							   utils::type::fromNameToString(type),
							   "' to variable which type is '",
							   utils::type::fromNameToString(var->second->type()), "'");
				 }
				 commands.emplace_back(
					 naobi::command::createCommand(
						 naobi::command::names::SAVE, {words[0]}));
			 }},
		})
{
}

naobi::utils::type::names
naobi::code_generator::processExpression(const std::vector<std::string>& words, std::vector<naobi::command>& commands)
{
	std::stack<naobi::utils::type::names> types;
	std::stack<naobi::operation::sptr> stack;
	bool isOperatorPrev{true};
	NLOG(processExpression, logger::IMPORTANT, "Expression to process:\n", words);
	for (auto it = words.cbegin() ; it != words.cend() ; it++)
	{
		NLOG(processExpression, logger::IMPORTANT, "Word: ", *it);
		if (!utils::type::isLiteral(*it))
		{
			if (isOperation(*it))
			{
				if (isOperatorPrev)
				{
					if (*it == "-")
					{
						if (types.top() == utils::type::names::INTEGER || types.top() == utils::type::names::FLOAT)
						{
							commands.push_back(command::createCommand(command::names::NEG, {}));
							continue;
						}
						else
						{
							NCRITICAL(processExpression, errors::TYPE_ERROR, "CRITICAL type ",
									  utils::type::fromNameToString(types.top()),
									  " doesn't have - operator");
						}
					}
					else
					{
						NCRITICAL(processExpression, errors::WRONG_FORMAT, "CRITICAL wrong operator placement");
					}
				}
				std::string op = *it;
				if ((*it == "=" && *(it + 1) == "=") || *it == "<" || *it == ">" || (*it == "<" && *(it + 1) == "=") ||
					(*it == ">" && *(it + 1) == "=") || (*it == "!" && *(it + 1) == "="))
				{
					op += *(it + 1);
					it++;
				}
				auto operation = operation_manager::get(op);
				if (operation == nullptr)
				{
					NCRITICAL(processExpression, errors::UNKNOWN_OPERATOR, "CRITICAL Bad operator '", op, "'");
				}
				while (!stack.empty() && stack.top() != nullptr && *stack.top() >= *operation)
				{
					auto second = types.top();
					types.pop();
					auto first = types.top();
					types.pop();
					auto func = stack.top()->call(first, second);
					if (func.second == nullptr)
					{
						NCRITICAL(processExpression, errors::TYPE_ERROR, "CRITICAL can't execute operation '",
								  operation->getOperator(), "' for types '", utils::type::fromNameToString(first),
								  "' and '", utils::type::fromNameToString(second), "'");
					}
					commands.emplace_back(command::createCommand(stack.top()->getCommandAnalogue(), {}));
					types.push(func.first);
					stack.pop();
				}
				stack.push(operation);
				isOperatorPrev = true;
			}
			else if (*it == "(" || *it == ")")
			{
				isOperatorPrev = false;
				if (*it == "(")
				{
					stack.push(nullptr);
				}
				else
				{
					bool isBracket = false;
					while (!stack.empty())
					{
						if (stack.top() == nullptr)
						{
							stack.pop();
							isBracket = true;
							break;
						}
						commands.emplace_back(command::createCommand(stack.top()->getCommandAnalogue(), {}));
						stack.pop();
					}
					if (!isBracket)
					{
						NCRITICAL(processExpression, errors::INCORRECT_BRACKETS, "CRITICAL brackets in ", words,
								  " are not correct");
					}
				}
			}
			else
			{
				isOperatorPrev = false;
				if ((it + 1) != words.cend() && *(it + 1) == "(")
				{
					auto t = callFunction(std::vector<std::string>(it, findEndBracket(it, words.end()) + 1), commands);
					if (t == utils::type::names::UNDEFINED)
					{
						NCRITICAL(code_generator, errors::TYPE_ERROR, "CRITICAL function return undefined type");
					}
					types.push(t);
					it = findEndBracket(it, words.end());
					continue;
				}
				auto var = _variablesTemp.find(*it);
				if (var == _variablesTemp.cend())
				{
					NCRITICAL(processExpression, errors::DOESNT_EXIST, "CRITICAL variable '", *it, "' not found");
				}
				commands.emplace_back(command::createCommand(naobi::command::names::LOAD, {*it}));
				types.push(var->second->type());
			}
		}
		else
		{
			isOperatorPrev = false;
			utils::type::names type = utils::type::checkType(*it);
			std::string temp = *it;
			if (type == utils::type::names::STRING)
			{
				temp = temp.substr(1, temp.size() - 2);
			}
			commands.emplace_back(
				command::createCommand(
					naobi::command::names::PLACE,
					{std::to_string(static_cast<int>(type)), temp}));
			types.push(type);
		}
	}
	while (!stack.empty())
	{
		auto operation = stack.top();

		auto second = types.top();
		types.pop();
		if (types.empty())
		{
			types.push(second);
			break;
		}
		auto first = types.top();
		types.pop();
		auto func = operation->call(first, second);
		if (func.second == nullptr)
		{
			NCRITICAL(processExpression, errors::TYPE_ERROR, "CRITICAL can't execute operation '",
					  operation->getOperator(), "' for types '", utils::type::fromNameToString(first),
					  "' and '", utils::type::fromNameToString(second), "'");
		}
		commands.emplace_back(command::createCommand(operation->getCommandAnalogue(), {}));
		types.push(func.first);
		stack.pop();
	}
	return types.top();
}

bool naobi::code_generator::addVariable(const std::string& name, const naobi::variable::sptr& var)
{
	if (_variablesTemp.find(name) != _variablesTemp.cend())
	{
		return false;
	}
	_variablesTemp[name] = var;
	return true;
}

naobi::utils::type::names
naobi::code_generator::callFunction(const std::vector<std::string>& functionCallWords, std::vector<command>& commands)
{
	NLOG(code_generator, logger::IMPORTANT, "Function: ", functionCallWords);
	auto functions = _module->findFunction(functionCallWords[0]);
	if (functions.empty())
	{
		if (generateFunction(functionCallWords))
		{
			return callFunction(functionCallWords, commands);
		}
		NCRITICAL(code_generator, errors::DOESNT_EXIST, "CRITICAL function '", functionCallWords[0], "' not found");
	}
	auto argsString = parser::join(functionCallWords.begin() + 2, functionCallWords.end() - 1, "");
	auto args = parser::split(argsString, parser::isAnyOf(","), {}, {{'(', ')'}});
	NLOG(code_generator, logger::IMPORTANT, "function call arguments: ", args);
	functions.erase(
		std::remove_if(
			functions.begin(), functions.end(),
			[size = args.size()](const naobi::function::sptr& function)
			{
				return function->getArguments().size() != size;
			}), functions.end());
	if (functions.empty())
	{
		if (generateFunction(functionCallWords))
		{
			return callFunction(functionCallWords, commands);
		}
		NCRITICAL(code_generator, errors::INVALID_ARGUMENT, "CRITICAL can't find function with arguments number '",
				  args.size(), "'");
	}

	commands.push_back(command::createCommand(command::names::ALLOCATE, {}));

	std::size_t pos = 0;
	auto functionIterator = functions.begin();

	std::vector<command> temp;
	for (auto arg = args.begin() ; arg != args.end() ;)
	{
		auto pair = parser::split(*arg, parser::isAnyOf(":"), {}, {{'(', ')'}, {'"', '"'}});
		std::string value;
		function::argument_type argInFunction;
		if (pair.size() == 1)
		{
			value = pair[0];
			auto argInFunctionOpt = (*functionIterator)->getArgument(pos);
			if (!argInFunctionOpt.has_value())
			{
				functionIterator++;
				if (functionIterator == functions.end())
				{
					if (generateFunction(functionCallWords))
					{
						return callFunction(functionCallWords, commands);
					}
					NCRITICAL(code_generator, errors::INVALID_ARGUMENT, "CRITICAL argument with position ", pos,
							  " doesn't exist");
				}
				else
				{
					pos = 0;
					arg = args.begin();
					temp.clear();
					continue;
				}
			}
			argInFunction = argInFunctionOpt.value();
		}
		else if (pair.size() == 2)
		{
			auto name = pair[0];
			value = pair[1];
			auto argInFunctionOpt = (*functionIterator)->getArgument(name);
			if (!argInFunctionOpt.has_value())
			{
				functionIterator++;
				if (functionIterator == functions.end())
				{
					if (generateFunction(functionCallWords))
					{
						return callFunction(functionCallWords, commands);
					}
					NCRITICAL(code_generator, errors::DOESNT_EXIST, "CRITICAL argument with name ", name,
							  " doesn't exist");
				}
				else
				{
					pos = 0;
					arg = args.begin();
					temp.clear();
					continue;
				}
			}
			argInFunction = argInFunctionOpt.value();
		}
		else
		{
			NCRITICAL(code_generator, errors::INVALID_ARGUMENT, "CRITICAL invalid argument ", pair);
		}
		auto valueSplitter = parser::split(
			value, parser::isAnyOf(" "), parser::isAnyOf("+-*/%=!<>,()"), {},
			{{'"', '"'},
			 {'{', '}'}});
		auto type = processExpression(valueSplitter, temp);
		if (type != argInFunction.second)
		{
			functionIterator++;
			if (functionIterator == functions.end())
			{
				if (generateFunction(functionCallWords))
				{
					return callFunction(functionCallWords, commands);
				}
				NCRITICAL(code_generator, errors::TYPE_ERROR, "CRITICAL argument '", argInFunction.first, "' is '",
						  utils::type::fromNameToString(argInFunction.second), "' provided '",
						  utils::type::fromNameToString(type), "'");
			}
			else
			{
				pos = 0;
				arg = args.begin();
				temp.clear();
				continue;
			}
		}
		temp.emplace_back(
			command::createCommand(
				command::names::TRANSFER,
				{argInFunction.first}));
		pos++;
		arg++;
	}

	commands.insert(commands.end(), temp.begin(), temp.end());
	commands.emplace_back(
		command::createCommand(
			naobi::command::names::CALL,
			{functionCallWords[0],
			 std::to_string((*functionIterator)->getNumber())}));
	return (*functionIterator)->getReturnType();
}

bool naobi::code_generator::generateFunction(const std::vector<std::string>& functionCallWords)
{
	auto templateFunction = _module->findTemplateFunction(functionCallWords[0]);
	if (templateFunction == nullptr)
	{
		NLOG(code_generator, logger::IMPORTANT, "WARNING can't find function template with name '",
			 functionCallWords[0], "'");
		return false;
	}
	NLOG(code_generator, logger::IMPORTANT, "begin generating function from template with name '",
		 templateFunction->getName(), "'");
	NLOG(code_generator, logger::LOW, "function call: ", functionCallWords);

	auto newFunction = std::make_shared<naobi::function>(templateFunction->getName());

	auto argsString = parser::join(functionCallWords.begin() + 2, functionCallWords.end() - 1, "");
	auto args = parser::split(argsString, parser::isAnyOf(","), {}, {{'(', ')'}, {'"', '"'}});

	if (args.size() != templateFunction->getArguments().size())
	{
		NLOG(code_generator, logger::IMPORTANT, "WARNING expected number of args ",
			 templateFunction->getArguments().size(), " and got ", args.size());
		return false;
	}
	NLOG(code_generator, logger::IMPORTANT, "generating function call arguments: ", args);

	std::size_t pos{};
	std::vector<function::argument_type> arguments;
	code_generator generator(_module);
	arguments.resize(args.size());
	std::map<std::string, utils::type::names> alreadySubstituted;
	std::string code = templateFunction->getCode();
	for (auto arg = args.begin() ; arg != args.end() ;)
	{
		auto pair = parser::split(*arg, parser::isAnyOf(":"), {}, {{'(', ')'}});
		std::string value;
		template_function::argument_type argInFunction;
		if (pair.size() == 1)
		{
			value = pair[0];
			auto argInFunctionOpt = templateFunction->getArgument(pos);
			if (!argInFunctionOpt.has_value())
			{
				NLOG(code_generator, logger::IMPORTANT, "WARNING bad ", pos, " argument");
				return false;
			}
			argInFunction = argInFunctionOpt.value();
		}
		else if (pair.size() == 2)
		{
			auto name = pair[0];
			value = pair[1];
			auto argInFunctionOpt = templateFunction->getArgument(name);
			if (!argInFunctionOpt.has_value())
			{
				NLOG(code_generator, logger::IMPORTANT, "WARNING bad ", name, " argument");
				return false;
			}
			argInFunction = argInFunctionOpt.value();
		}
		else
		{
			NLOG(code_generator, logger::IMPORTANT, "WARNING invalid argument ", pair);
			return false;
		}
		auto valueSplitter = parser::split(
			value, parser::isAnyOf(" "), parser::isAnyOf("+-*/%=!<>,()"), {},
			{{'"', '"'},
			 {'{', '}'}});
		std::vector<command> plug;
		auto type = processExpression(valueSplitter, plug);
		if (utils::type::isStandardType(argInFunction.second))
		{
			if (type != utils::type::fromStringToName(argInFunction.second))
			{
				NLOG(code_generator, logger::IMPORTANT, "WARNING wrong type provided '",
					 utils::type::fromNameToString(type), "' expected '", argInFunction.second, "'");
				return false;
			}
			arguments[templateFunction->getPosOfArgument(argInFunction.first)] = std::make_pair(
				argInFunction.first,
				utils::type::fromStringToName(
					argInFunction.second));
			generator.addVariable(argInFunction.first, std::make_shared<variable>(argInFunction.first, type));
		}
		else
		{
			auto it = alreadySubstituted.find(argInFunction.second);
			if (it != alreadySubstituted.end())
			{
				arguments[templateFunction->getPosOfArgument(argInFunction.first)] = std::make_pair(
					argInFunction.first,
					it->second);
			}
			else
			{
				alreadySubstituted[argInFunction.second] = type;
				arguments[templateFunction->getPosOfArgument(argInFunction.first)] = std::make_pair(
					argInFunction.first,
					type);
			}
			generator.addVariable(argInFunction.first, std::make_shared<variable>(argInFunction.first, type));
			std::size_t startPos{};
			std::string substitute = utils::type::fromNameToString(type);
			while ((startPos = code.find(argInFunction.second, startPos)) != std::string::npos)
			{
				code.replace(startPos, argInFunction.second.size(), substitute);
				startPos += substitute.size();
			}
		}

		pos++;
		arg++;
	}

	newFunction->setArguments(arguments);
	auto lines = parser::split(
		code, parser::isAnyOf(";}"), {}, {{'{', '}'},
										  {'"', '"'}});
	auto commands = generator.generate(lines);
	newFunction->setCommands(commands);

	if (templateFunction->getReturnType() == "undefined")
	{
		newFunction->setReturnType(utils::type::names::UNDEFINED);
	}
	else
	{
		if (utils::type::isStandardType(templateFunction->getReturnType()))
		{
			newFunction->setReturnType(utils::type::fromStringToName(templateFunction->getReturnType()));
		}
		else
		{
			auto it = alreadySubstituted.find(templateFunction->getReturnType());
			if (it == alreadySubstituted.end())
			{
				NLOG(code_generator, logger::IMPORTANT, "WARNING failed to substitute type of template which is ",
					 templateFunction->getReturnType());
				return false;
			}
			newFunction->setReturnType(it->second);
		}
	}
	_module->addFunction(newFunction);
	return true;
}
