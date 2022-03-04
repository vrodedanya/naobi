#include <naobi/compiler/code_generator.hpp>

#include <naobi/utils/logger.hpp>
#include <naobi/interpreter/workflow_context.hpp>


std::vector<naobi::command> naobi::code_generator::generate(const std::vector<std::string> &lines)
{
	LOG(code_generator, naobi::logger::BASIC, "Code lines:\n", lines);
	return {};
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
					context->stack.push(context->variables[std::stoi(arguments[0])]);
				}},
				{naobi::command::names::SAVE,
				 [](const workflow_context::sptr& context, [[maybe_unused]]const command::argumentsList& arguments){
					context->variables[std::stoi(arguments[0])] = context->stack.top();
					context->stack.pop();
			 	}}

		};
