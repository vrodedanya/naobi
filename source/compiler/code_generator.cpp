#include <naobi/compiler/code_generator.hpp>

#include <naobi/utils/logger.hpp>
#include <naobi/interpreter/workflow_context.hpp>
#include <naobi/utils/keywords.hpp>


std::vector<naobi::command> naobi::code_generator::generate(const std::vector<std::string> &lines)
{
	LOG(code_generator, naobi::logger::BASIC, "Code lines:\n", lines);
	std::vector<naobi::command> commands;

	for (const auto& line : lines)
	{
		auto words = naobi::parser::removeEmpty(
				naobi::parser::split(line, {" "}, {"+", "-", "*", "/", "=", "(", ")"}));
		LOG(compiler.processModule, logger::LOW, "words:\n", words);

		bool isCompiled = false;
		for (const auto& rule : _generatorRules)
		{
			if (!isCompiled)
				isCompiled = rule.checkLineAndRun(words, commands);
			else
				rule.checkLineAndRun(words, commands);
		}
		if (!isCompiled)
		{
			LOG(compiler.processModule, logger::CRITICAL, "CRITICAL failed to identify line:\n", line);
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
	{naobi::command::names::NEW,
	[](const workflow_context::sptr& context, [[maybe_unused]]const command::argumentsList& arguments){
		auto type = static_cast<naobi::variable::Type>(std::stoi(arguments[1]));
		auto var = std::make_shared<naobi::variable>(arguments[0], type);
		context->variables[var->name()] = var;
	}},
	{naobi::command::names::LOAD, // TODO Difficult moment: variable can be load as reference or as copy
	[](const workflow_context::sptr& context, [[maybe_unused]]const command::argumentsList& arguments){ //
		context->stack.push(context->variables[arguments[0]]);
	}},
	{naobi::command::names::SAVE,
	[](const workflow_context::sptr& context, [[maybe_unused]]const command::argumentsList& arguments){
		context->variables[arguments[0]]->value() = context->stack.top()->value();
		context->stack.pop();
	}},
	{naobi::command::names::PLACE,
	[](const workflow_context::sptr& context, [[maybe_unused]]const command::argumentsList& arguments){
		auto type = static_cast<naobi::variable::Type>(std::stoi(arguments[0]));
		auto var = std::make_shared<naobi::variable>("temp", type);
		if (type == naobi::variable::Type::INTEGER) var->value() = std::stoi(arguments[1]);
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
		naobi::variable::Type type;
		if (naobi::code_generator::isNumber(str))
		{
			type = naobi::variable::Type::INTEGER;
		}
		if (type == naobi::variable::Type::INTEGER)
		{
			auto var = std::make_shared<naobi::variable>("temp", type);
			var->value() = std::stoi(str);
			context->stack.push(var);
		}
	}},
};


naobi::code_generator::code_generator() :
	_generatorRules(
{
	// Variable creating logic
	{[](const std::vector<std::string>& words) -> bool{
		return naobi::keywords::checkIsType(words[0]);
	},
	 [this](const std::vector<std::string>& words, std::vector<naobi::command>& commands){
		naobi::variable::Type type;
		std::vector<std::string> wordsTemp = words;
		std::for_each(wordsTemp.begin(), wordsTemp.end(), [](auto& elem) {elem = naobi::parser::removeSym(elem, ',');});
		wordsTemp = naobi::parser::removeEmpty(wordsTemp);
		if (wordsTemp[0] == "integer") type = naobi::variable::Type::INTEGER;
		else type = naobi::variable::Type::UNDEFINED;

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
							naobi::command::names::NEW,{*it, std::to_string(static_cast<int>(type))}));
			it++;
			if (it == wordsTemp.cend() || *it != "=")
			{
				LOG(code_generator, logger::CRITICAL, "CRITICAL '", *(it - 1), "' is not initialized");
				std::exit(1);
			}
			it++;
			if (it == wordsTemp.cend()) // TODO add checking is literal
			{
				LOG(code_generator, logger::CRITICAL, "CRITICAL '", *(it - 1), "' has empty literal");
				std::exit(1);
			}
			commands.emplace_back(
					naobi::code_generator::createCommand(
							naobi::command::names::PLACE, {std::to_string(static_cast<int>(type)), *it}));
			// TODO add assigning value for providing some checks in compile time
			commands.emplace_back(
					naobi::code_generator::createCommand(
							naobi::command::names::SAVE, {var->name()}));
			_variablesTemp[var->name()] = var;
			it++;
		}

	}},
	// Create printing TODO should be one logic for all functions. Standard functions will be provided by shared standard library all append in modules
	{[](const std::vector<std::string>& words) -> bool{
		return words[0] == "println" || words[0] == "print";
	},
	 []([[maybe_unused]]const std::vector<std::string>& words, std::vector<naobi::command>& commands){
				commands.emplace_back(
						naobi::code_generator::createCommand(
								naobi::command::names::LOAD, {words[2]}));
				commands.emplace_back(
						naobi::code_generator::createCommand(
								words[0] == "println" ? naobi::command::names::PRINTLN : naobi::command::names::PRINT, {}));
			}},
	{[](const std::vector<std::string>& words) -> bool{
		return std::find(words.begin(), words.end(), "input") != words.cend();
	},
	[]([[maybe_unused]]const std::vector<std::string>& words, std::vector<naobi::command>& commands){
				commands.emplace_back(
						naobi::code_generator::createCommand(
								naobi::command::names::INPUT, {}));
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

		// todo expression handler
		naobi::variable::Type type = naobi::variable::Type::INTEGER; // TODO checking type of result variable
		if (_variablesTemp.find(words[0]) == _variablesTemp.cend())
		{
			LOG(code_generator, logger::CRITICAL, "CRITICAL '", words[0], "' doesn't exist");
			std::exit(1);
		}
		if (_variablesTemp.find(words[2]) != _variablesTemp.cend())
		{
			commands.emplace_back(
					naobi::code_generator::createCommand(
							naobi::command::names::LOAD, {words[2]}));
		}
		else if (isLiteral(words[2]))
		{
			commands.emplace_back(
					naobi::code_generator::createCommand(
							naobi::command::names::PLACE, {std::to_string(static_cast<int>(type)), words[2]}));
		}
		// todo check function and it returns value
			commands.emplace_back(
					naobi::code_generator::createCommand(
							naobi::command::names::SAVE, {words[0]}));
	}},
}
)
{

}

bool naobi::code_generator::isLiteral(const std::string &string)
{
	return isNumber(string) || (string.front() == '"' && string.back() == '"');
}

bool naobi::code_generator::isNumber(const std::string &string)
{
	if (string.empty()) return false;
	if (string.size() == 1) return std::isdigit(string[0]);
	bool isAllNumbers = std::all_of(string.cbegin() + 1, string.cend(), [](const auto& elem){return isdigit(elem) || elem == '.';});
	return (string[0] == '-' && isAllNumbers) || (isAllNumbers && std::isdigit(string[0]));
}
