#include <naobi/compiler/code_generator.hpp>

#include <cstring>

#include <naobi/utils/logger.hpp>
#include <naobi/interpreter/workflow_context.hpp>
#include <naobi/utils/keywords.hpp>
#include <naobi/interpreter/event_manager.hpp>


std::vector<naobi::command> naobi::code_generator::generate(std::vector<std::string> lines)
{
	LOG(code_generator, naobi::logger::BASIC, "Code lines:\n", lines);
	std::vector<naobi::command> commands;

	for (auto it = lines.begin() ; it != lines.end() ; it++)
	{
		auto words = naobi::parser::removeEmpty(
				naobi::parser::split(*it, {" "}, {"+", "-", "*", "/", "=", "!", "<", ">", "(", ")", ","}));
		LOG(code_generator, logger::LOW, "words:\n", words);

		bool isCompiled = false;
		for (const auto& rule : _generatorRules)
		{
			if (!isCompiled)
			{
				if (words[0] == "if" && (it+1) != lines.end() && (it+1)->substr(0, std::strlen("else")) == "else")
				{
					auto temp = naobi::parser::removeEmpty(
							naobi::parser::split(*(it + 1), {" "}, {"+", "-", "*", "/", "=", "!", "<", ">", "(", ")"}));
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
			LOG(compiler.processModule, logger::CRITICAL, "CRITICAL failed to identify line:\n", *it);
			std::exit(EXIT_FAILURE);
		}
	}

	return commands;
}

naobi::command
naobi::code_generator::createCommand(naobi::command::names name, const naobi::command::argumentsList &arguments)
{
	naobi::command command;
	command.name = name;
	command.arguments = arguments;
	auto it = _commands.find(name);
	if (it != _commands.cend())
	{
		command.impl = it->second;
	}
	else
	{
		LOG(code_generator, naobi::logger::CRITICAL, "CRITICAL BROKEN COMMAND");
		std::exit(1);
	}
	return command;
}


std::map<naobi::command::names, naobi::command::implementation> naobi::code_generator::_commands =
{
	{naobi::command::names::ADD,
	[](const workflow_context::sptr& context, [[maybe_unused]]const command::argumentsList& arguments){
		auto top = context->stack.top();
		context->stack.pop();
		context->stack.top() += top;
	}},
	{naobi::command::names::SUB,
	[](const workflow_context::sptr& context, [[maybe_unused]]const command::argumentsList& arguments){
		auto top = context->stack.top();
		context->stack.pop();
		context->stack.top() -= top;
	}},
	{naobi::command::names::MUL,
	[](const workflow_context::sptr& context, [[maybe_unused]]const command::argumentsList& arguments){
		auto top = context->stack.top();
		context->stack.pop();
		context->stack.top() *= top;
	}},
	{naobi::command::names::DIV,
	[](const workflow_context::sptr& context, [[maybe_unused]]const command::argumentsList& arguments){
		auto top = context->stack.top();
		context->stack.pop();
		context->stack.top() /= top;
	}},
	{naobi::command::names::NEW,
	[](const workflow_context::sptr& context, [[maybe_unused]]const command::argumentsList& arguments){
		auto type = utils::type::toType(arguments[1]);
		auto var = std::make_shared<naobi::variable>(arguments[0], type);
		context->variables[var->name()] = var;
	}},
	{naobi::command::names::LOAD,
	[](const workflow_context::sptr& context, [[maybe_unused]]const command::argumentsList& arguments){ //
		context->stack.push(context->variables[arguments[0]]->copy());
	}},
	{naobi::command::names::SAVE,
	[](const workflow_context::sptr& context, [[maybe_unused]]const command::argumentsList& arguments){
		context->variables[arguments[0]]->value() = context->stack.top()->value();
		context->stack.pop();
	}},
	{naobi::command::names::PLACE,
	[](const workflow_context::sptr& context, [[maybe_unused]]const command::argumentsList& arguments){
		auto type = utils::type::toType(arguments[0]);
		auto var = std::make_shared<naobi::variable>("temp", type);
		var->value() = utils::type::getValueFrom(type, arguments[1]);
		context->stack.push(var);
	}},
	{naobi::command::names::PRINTLN,
	[](const workflow_context::sptr& context, [[maybe_unused]]const command::argumentsList& arguments){
		auto val = context->stack.top();
		std::cout << *val << std::endl;
		context->stack.pop();
	}},
	{naobi::command::names::PRINT,
	[](const workflow_context::sptr& context, [[maybe_unused]]const command::argumentsList& arguments){
		auto val = context->stack.top();
		std::cout << *val;
		context->stack.pop();
	}},
	{naobi::command::names::INPUT,
	[](const workflow_context::sptr& context, [[maybe_unused]]const command::argumentsList& arguments){
		std::string str;
		std::cin >> str;
		utils::type::names type = utils::type::checkTypeFromInput(str);
		auto var = std::make_shared<naobi::variable>("temp", type);
		var->value() = utils::type::getValueFrom(type, str);
		context->stack.push(var);
	}},
	{naobi::command::names::RETURN,
	[](const workflow_context::sptr& context, [[maybe_unused]]const command::argumentsList& arguments){
		auto address = context->returnPoints.top();
		context->ip = address;
		context->returnPoints.pop();
	}},
	{naobi::command::names::CALL,
	[](const workflow_context::sptr& context, [[maybe_unused]]const command::argumentsList& arguments){
		auto address = context->ip;
		context->returnPoints.push(address);
		auto it = context->workflow->module()->findFunction(arguments[0]);
		context->ip = it->commands().begin();
	}},
	{naobi::command::names::JUMP,
	[](const workflow_context::sptr& context, [[maybe_unused]]const command::argumentsList& arguments)
	{
		context->ip += std::stoi(arguments[0]);
	}},
	{naobi::command::names::JUMP_IF,
	[](const workflow_context::sptr& context, [[maybe_unused]]const command::argumentsList& arguments)
	{
		if (context->stack.top() != true)
		{
			context->ip += std::stoi(arguments[0]);
		}
	}},
	{naobi::command::names::NOPE,
	[]([[maybe_unused]]const workflow_context::sptr& context, [[maybe_unused]]const command::argumentsList& arguments) noexcept{
	}},
	{naobi::command::names::EQ,
	[]([[maybe_unused]]const workflow_context::sptr& context, [[maybe_unused]]const command::argumentsList& arguments) noexcept{
		auto first = context->stack.top();
		context->stack.pop();
		auto second = context->stack.top();
		context->stack.pop();
		context->stack.push(first == second);
	}},
	{naobi::command::names::GREATER,
	[]([[maybe_unused]]const workflow_context::sptr& context, [[maybe_unused]]const command::argumentsList& arguments) noexcept{
		auto first = context->stack.top();
		context->stack.pop();
		auto second = context->stack.top();
		context->stack.pop();
		context->stack.push(second > first);
	}},
	{naobi::command::names::LESS,
	[]([[maybe_unused]]const workflow_context::sptr& context, [[maybe_unused]]const command::argumentsList& arguments) noexcept{
		auto first = context->stack.top();
		context->stack.pop();
		auto second = context->stack.top();
		context->stack.pop();
		context->stack.push(second < first);
	}},
	{naobi::command::names::GREATER_OR_EQ,
	[]([[maybe_unused]]const workflow_context::sptr& context, [[maybe_unused]]const command::argumentsList& arguments) noexcept{
		auto first = context->stack.top();
		context->stack.pop();
		auto second = context->stack.top();
		context->stack.pop();
		context->stack.push(second >= first);
	}},
	{naobi::command::names::LESS_OR_EQ,
	[]([[maybe_unused]]const workflow_context::sptr& context, [[maybe_unused]]const command::argumentsList& arguments) noexcept{
		auto first = context->stack.top();
		context->stack.pop();
		auto second = context->stack.top();
		context->stack.pop();
		context->stack.push(second <= first);
	}},
	{naobi::command::names::NOT_EQ,
	[]([[maybe_unused]]const workflow_context::sptr& context, [[maybe_unused]]const command::argumentsList& arguments) noexcept{
		auto first = context->stack.top();
		context->stack.pop();
		auto second = context->stack.top();
		context->stack.pop();
		context->stack.push(second != first);
	}},
	{naobi::command::names::EXIT,
	[]([[maybe_unused]]const workflow_context::sptr& context, [[maybe_unused]]const command::argumentsList& arguments) noexcept{
		auto top = context->stack.top();
		std::exit(std::get<int>(top->value()));
	}},
	{naobi::command::names::INC,
	[]([[maybe_unused]]const workflow_context::sptr& context, [[maybe_unused]]const command::argumentsList& arguments) noexcept{
		auto top = context->stack.top();
		auto var = std::make_shared<naobi::variable>("temp", top->type());
		var->value() = 1;
		context->stack.top() += var;
	}},
	{naobi::command::names::DEC,
	[]([[maybe_unused]]const workflow_context::sptr& context, [[maybe_unused]]const command::argumentsList& arguments) noexcept{
		auto top = context->stack.top();
		auto var = std::make_shared<naobi::variable>("temp", top->type());
		var->value() = 1;
		context->stack.top() -= var;
	}},
	{naobi::command::names::ARISE,
	[]([[maybe_unused]]const workflow_context::sptr& context, [[maybe_unused]]const command::argumentsList& arguments) noexcept{
		event_manager::pushEvent(arguments[0]);
	}},
	{naobi::command::names::MOD,
	[]([[maybe_unused]]const workflow_context::sptr& context, [[maybe_unused]]const command::argumentsList& arguments) noexcept{
		auto top = context->stack.top();
		context->stack.pop();
		context->stack.top() %= top;
	}},
};


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
				LOG(code_generator, logger::CRITICAL, "CRITICAL '", *it, "' is already exist");
				std::exit(1);
			}
			auto var = std::make_shared<naobi::variable>(*it, type);
			commands.emplace_back(
					naobi::code_generator::createCommand(
							naobi::command::names::NEW, {*it, std::to_string(static_cast<int>(type))}));
			it++;
			if (it == wordsTemp.cend() || *it != "=")
			{
				LOG(code_generator, logger::CRITICAL, "CRITICAL '", *(it - 1), "' is not initialized");
				std::exit(1);
			}
			it++;
			if (it == wordsTemp.cend())
			{
				LOG(code_generator, logger::CRITICAL, "CRITICAL '", *(it - 1), "' has empty literal");
				std::exit(1);
			}
			if (!utils::type::isLiteral(*it))
			{ // TODO for type safety need to check type of loading variable
				if (_variablesTemp.find(*it) == _variablesTemp.cend())
				{
					LOG(code_generator, logger::CRITICAL, "CRITICAL '", *it, "' doesn't exist");
					std::exit(1);
				}
				commands.emplace_back(createCommand(naobi::command::names::LOAD, {*it}));
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
							naobi::code_generator::createCommand(
									naobi::command::names::PLACE, {std::to_string(static_cast<int>(type)), value}));
				}
				else
				{
					LOG(code_generator, logger::CRITICAL, "CRITICAL variable type ('", utils::type::fromNameToString(type),
						"') is not matching literal type which is '", utils::type::fromNameToString(utils::type::checkType(*it)), "'");
					std::exit(1);
				}
			}
			// TODO add assigning value for providing some checks in compile time
			commands.emplace_back(
					naobi::code_generator::createCommand(
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
		LOG(code_generator, logger::LOW, "if block:\n", words);
		auto bodyIt = findEndBracket(words.begin() + 1, words.end());
		processExpression(std::vector<std::string>(words.begin() + 2 , bodyIt), commands);

		std::string codeBlock = *(bodyIt + 1);
		codeBlock = naobi::parser::removeFirstSym(codeBlock.substr(1, codeBlock.size() - 2), ' ');
		auto lines = naobi::parser::removeEmpty(naobi::parser::split(codeBlock, {";", "}"}, {}));
		std::for_each(lines.begin(), lines.end(), [](auto& elem){elem = naobi::parser::removeFirstSym(elem, ' ');});
		lines = naobi::parser::removeEmpty(lines);

		auto tempCommands = generate(lines);
		std::size_t tempCommandsSize = tempCommands.size();

		if (*(bodyIt + 2) == "else")
		{
			tempCommandsSize++;
		}

		commands.emplace_back(createCommand(naobi::command::names::JUMP_IF, {std::to_string(tempCommandsSize)}));
		for (const auto& command : tempCommands)
		{
			commands.push_back(command);
		}

		if (*(bodyIt + 2) == "else")
		{
			std::string elseBlock = *(bodyIt + 3);
			elseBlock = elseBlock.substr(1, elseBlock.size() - 2);
			LOG(code_generator, logger::IMPORTANT, "Else block:\n", elseBlock);

			auto elseLines = naobi::parser::removeEmpty(naobi::parser::split(elseBlock, {";", "}"}, {}));
			std::for_each(elseLines.begin(), elseLines.end(), [](auto& elem){elem = naobi::parser::removeFirstSym(elem, ' ');});
			elseLines = naobi::parser::removeEmpty(elseLines);

			LOG(code_generator, logger::IMPORTANT, "Else lines:\n", elseLines);

			auto tempElseCommands = generate(elseLines);
			std::size_t tempElseCommandsSize = tempElseCommands.size();
			commands.emplace_back(createCommand(naobi::command::names::JUMP, {std::to_string(tempElseCommandsSize)}));
			for (const auto& command : tempElseCommands)
			{
				commands.push_back(command);
			}
		}
	}},
	// For
	{[](const std::vector<std::string>& words) -> bool{
		return words[0] == "for" && words.size() == 10;
	},
	[this](const std::vector<std::string>& words, std::vector<naobi::command>& commands){
		LOG(code_generator, logger::LOW, "for:\n", words);
		utils::type::names type = utils::type::fromStringToName(words[1]);
		commands.push_back(createCommand(command::names::NEW, {words[2], std::to_string(static_cast<int>(type))}));
		commands.push_back(createCommand(command::names::PLACE, {std::to_string(static_cast<int>(type)), words[5]}));
		commands.push_back(createCommand(command::names::SAVE, {words[2]}));

		auto var = std::make_shared<naobi::variable>(words[2], type);
		_variablesTemp[var->name()] = var;

		commands.push_back(createCommand(command::names::LOAD, {words[2]}));
		commands.push_back(createCommand(command::names::PLACE, {std::to_string(static_cast<int>(type)), words[7]}));
		commands.push_back(createCommand(command::names::LESS, {}));

		std::string codeBlock = words[9];
		LOG(code_generator.forBlock, logger::BASIC, "for block:\n", codeBlock);

		codeBlock = naobi::parser::removeFirstSym(codeBlock.substr(1, codeBlock.size() - 2), ' ');
		auto lines = naobi::parser::removeEmpty(naobi::parser::split(codeBlock, {";", "}"}, {}));
		std::for_each(lines.begin(), lines.end(), [](auto& elem){elem = naobi::parser::removeFirstSym(elem, ' ');});
		lines = naobi::parser::removeEmpty(lines);
		LOG(code_generator.forBlock, logger::LOW, "for block lines:\n", lines);

		auto tempCommands = generate(lines);
		commands.push_back(createCommand(command::names::JUMP_IF, {std::to_string(tempCommands.size() + 4)}));
		commands.insert(commands.cend(), tempCommands.begin(), tempCommands.end());
		commands.push_back(createCommand(command::names::LOAD, {words[2]}));
		commands.push_back(createCommand(command::names::INC, {}));
		commands.push_back(createCommand(command::names::SAVE, {words[2]}));
		int size = -(8 + static_cast<int>(tempCommands.size()));
		commands.push_back(createCommand(command::names::JUMP, {std::to_string(size)}));
	}},
	// Function calling
	{[](const std::vector<std::string>& words) -> bool{
		return words[1] == "(" && std::find(words.begin() + 2, words.end(), ")") != words.end() && !keywords::check(words[0]);
	},
	 [this]([[maybe_unused]]const std::vector<std::string>& words, std::vector<naobi::command>& commands){
		auto it = _module->findFunction(words[0]);
		if (it == nullptr)
		{
			LOG(code_generator, logger::CRITICAL, "CRITICAL function '", words[0], "' not found");
			std::exit(EXIT_FAILURE);
		}
		processExpression(std::vector<std::string>(words.begin() + 2,
												   findEndBracket(words.begin() + 1, words.end())), commands);
		commands.emplace_back(code_generator::createCommand(naobi::command::names::CALL, {words[0]}));
	}},
	// Raise
	{[](const std::vector<std::string>& words) -> bool{
		return words[0] == "arise" && words.size() == 2;
	},
	[]([[maybe_unused]]const std::vector<std::string>& words, std::vector<naobi::command>& commands){
		commands.push_back(createCommand(command::names::ARISE, {words[1]}));
	}},
	// Create assignment logic (LOW priority )
	{[](const std::vector<std::string>& words) -> bool{
		return words[1] == "=";
	},
	 [this](const std::vector<std::string>& words, std::vector<naobi::command>& commands){
		if (words.size() < 3)
		{
			LOG(code_generator, logger::CRITICAL, "CRITICAL bad assignment operator format");
			std::exit(1);
		}

		if (_variablesTemp.find(words[0]) == _variablesTemp.cend())
		{
			LOG(code_generator, logger::CRITICAL, "CRITICAL '", words[0], "' doesn't exist");
			std::exit(1);
		}
		processExpression(std::vector<std::string>(words.begin() + 2, words.end()), commands);
		// todo check function and it returns value
		commands.emplace_back(
				naobi::code_generator::createCommand(
						naobi::command::names::SAVE, {words[0]}));
	}},
})
{
}

void
naobi::code_generator::processExpression(const std::vector<std::string> &words, std::vector<naobi::command> &commands)
{
	std::stack<naobi::command> stack;
	LOG(processExpression, logger::IMPORTANT, "Expression to process:\n", words);
	for (auto it = words.cbegin(); it != words.cend(); it++)
	{
		LOG(processExpression, logger::IMPORTANT, "Word: ", *it);
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
						stack.push(createCommand(command::names::EQ, {}));
						it++;
					}
					else if (*it == "<" && *(it + 1) == "=")
					{
						stack.push(createCommand(command::names::LESS_OR_EQ, {}));
						it++;
					}
					else if (*it == ">" && *(it + 1) == "=")
					{
						stack.push(createCommand(command::names::GREATER_OR_EQ, {}));
						it++;
					}
					else if (*it == "!" && *(it + 1) == "=")
					{
						stack.push(createCommand(command::names::NOT_EQ, {}));
						it++;
					}
					else if (*it == ">")
					{
						stack.push(createCommand(command::names::GREATER, {}));
					}
					else if (*it == "<")
					{
						stack.push(createCommand(command::names::LESS, {}));
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
					stack.push(createCommand(*it == "+" ? naobi::command::names::ADD : naobi::command::names::SUB, {}));
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
						stack.push(createCommand(naobi::command::names::MUL, {}));

					}
					else if (*it == "/")
					{
						stack.push(createCommand(naobi::command::names::DIV, {}));
					}
					else if (*it == "%")
					{
						stack.push(createCommand(naobi::command::names::MOD, {}));
					}
				}
				else
				{
					LOG(processExpression, logger::CRITICAL, "CRITICAL Bad operator '", *it, "'");
					std::exit(EXIT_FAILURE);
				}
			}
			else if ((it + 1) != words.cend() && *(it + 1) == "(")
			{
				auto func = _module->findFunction(*it);
				if (func == nullptr)
				{
					LOG(processExpression, logger::CRITICAL, "CRITICAL function '", *it, "' not found");
					std::exit(EXIT_FAILURE);
				}
				if ((it + 2) != words.cend() && *(it + 2) != ")") // TODO function may contain more arguments
				{
					processExpression(std::vector<std::string>(it + 2, it + 3), commands);
				}
				commands.emplace_back(code_generator::createCommand(naobi::command::names::CALL, {*it}));
				it = findEndBracket(it, words.end());
			}
			else if (*it == "(" || *it == ")")
			{
				if (*it == "(")
				{
					stack.push(code_generator::createCommand(naobi::command::names::NOPE, {}));
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
						LOG(processExpression, logger::CRITICAL, "CRITICAL brackets in ", words, " are not correct");
						std::exit(EXIT_FAILURE);
					}
				}
			}
			else
			{
				if (_variablesTemp.find(*it) == _variablesTemp.cend())
				{
					LOG(processExpression, logger::CRITICAL, "CRITICAL variable '", *it, "' not found");
					std::exit(EXIT_FAILURE);
				}
				commands.emplace_back(code_generator::createCommand(naobi::command::names::LOAD, {*it}));
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
					createCommand(naobi::command::names::PLACE, {std::to_string(static_cast<int>(type)), temp}));
		}
	}
	while (!stack.empty())
	{
		commands.emplace_back(stack.top());
		stack.pop();
	}
}
