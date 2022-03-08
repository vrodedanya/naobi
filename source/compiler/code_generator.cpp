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
			isCompiled = rule.checkLineAndRun(words, commands);
			if (isCompiled) break;
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
		context->variables.push_back(std::make_shared<naobi::variable>(arguments[0], type));
	}},
	{naobi::command::names::LOAD, // TODO Difficult moment: variable can be load as reference or as copy
	[](const workflow_context::sptr& context, [[maybe_unused]]const command::argumentsList& arguments){ //
		context->stack.push(context->variables[std::stoul(arguments[0])]);
	}},
	{naobi::command::names::SAVE,
	[](const workflow_context::sptr& context, [[maybe_unused]]const command::argumentsList& arguments){
		context->variables[std::stoul(arguments[0])] = context->stack.top();
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
	}},
	{naobi::command::names::PRINT,
	[](const workflow_context::sptr& context, [[maybe_unused]]const command::argumentsList& arguments){
		auto val = context->stack.top();
		std::cout << *val;
	}},
};

std::vector<naobi::code_generator::generatorRule> naobi::code_generator::_generatorRules =
{
	// Variable creating logic
	{[](const std::vector<std::string>& words) -> bool{
		return naobi::keywords::checkIsType(words[0]);
	},[](const std::vector<std::string>& words, std::vector<naobi::command>& commands){
		naobi::variable::Type type;
		if (words[0] == "integer") type = naobi::variable::Type::INTEGER;
		else type = naobi::variable::Type::UNDEFINED;
		commands.emplace_back(
				naobi::code_generator::createCommand(
				naobi::command::names::NEW,{words[1], std::to_string(static_cast<int>(type))}));
	}},
	// Create assignment logic
	{[](const std::vector<std::string>& words) -> bool{
		return words[1] == "=";
	},[](const std::vector<std::string>& words, std::vector<naobi::command>& commands){
		// todo expression handler
		naobi::variable::Type type = naobi::variable::Type::INTEGER;

		commands.emplace_back(
				naobi::code_generator::createCommand(
						naobi::command::names::PLACE, {std::to_string(static_cast<int>(type)), words[2]}));
		commands.emplace_back(
				naobi::code_generator::createCommand(
							naobi::command::names::SAVE, {"0"}));
	}},
	// Create printing
	{[](const std::vector<std::string>& words) -> bool{
		return words[0] == "println";
	},[]([[maybe_unused]]const std::vector<std::string>& words, std::vector<naobi::command>& commands){
		commands.emplace_back(
				naobi::code_generator::createCommand(
						naobi::command::names::LOAD, {"0"}));
		commands.emplace_back(
				naobi::code_generator::createCommand(
						naobi::command::names::PRINTLN, {}));
	}},
};