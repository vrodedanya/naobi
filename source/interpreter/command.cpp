#include <naobi/interpreter/command.hpp>

#include <iostream>

#include <naobi/utils/logger.hpp>
#include <naobi/interpreter/workflow_context.hpp>
#include <naobi/interpreter/event_manager.hpp>
#include <naobi/utils/operation.hpp>


std::map<naobi::command::names, naobi::command::implementation> naobi::command::commands =
{
		{naobi::command::names::ADD,
				[](const naobi::workflow_context::sptr& context, [[maybe_unused]]const naobi::command::argumentsList& args){
					auto second = context->stack.top();
					context->stack.pop();
					auto first = context->stack.top();
					context->stack.pop();

					context->stack.push(naobi::operation_manager::get("+")->call(first->type(), second->type()).second(first, second));
				}},
		{naobi::command::names::SUB,
				[](const naobi::workflow_context::sptr& context, [[maybe_unused]]const naobi::command::argumentsList& args){
					auto second = context->stack.top();
					context->stack.pop();
					auto first = context->stack.top();
					context->stack.pop();

					context->stack.push(naobi::operation_manager::get("-")->call(first->type(), second->type()).second(first, second));
				}},
		{naobi::command::names::MUL,
				[](const naobi::workflow_context::sptr& context, [[maybe_unused]]const naobi::command::argumentsList& args){
					auto second = context->stack.top();
					context->stack.pop();
					auto first = context->stack.top();
					context->stack.pop();

					context->stack.push(naobi::operation_manager::get("*")->call(first->type(), second->type()).second(first, second));
				}},
		{naobi::command::names::DIV,
				[](const naobi::workflow_context::sptr& context, [[maybe_unused]]const naobi::command::argumentsList& args){
					auto second = context->stack.top();
					context->stack.pop();
					auto first = context->stack.top();
					context->stack.pop();

					context->stack.push(naobi::operation_manager::get("/")->call(first->type(), second->type()).second(first, second));
				}},
		{naobi::command::names::NEW,
				[](const naobi::workflow_context::sptr& context, [[maybe_unused]]const naobi::command::argumentsList& args){
					auto type = utils::type::toType(args[1]);
					auto var = std::make_shared<naobi::variable>(args[0], type);
					context->variables[var->name()] = var;
				}},
		{naobi::command::names::LOAD,
				[](const naobi::workflow_context::sptr& context, [[maybe_unused]]const naobi::command::argumentsList& args){ //
					context->stack.push(context->variables[args[0]]->copy());
				}},
		{naobi::command::names::SAVE,
				[](const naobi::workflow_context::sptr& context, [[maybe_unused]]const naobi::command::argumentsList& args){
					context->variables[args[0]]->value() = context->stack.top()->value();
					context->stack.pop();
				}},
		{naobi::command::names::PLACE,
				[](const naobi::workflow_context::sptr& context, [[maybe_unused]]const naobi::command::argumentsList& args){
					auto type = utils::type::toType(args[0]);
					auto var = std::make_shared<naobi::variable>("temp", type);
					var->value() = utils::type::getValueFrom(type, args[1]);
					context->stack.push(var);
				}},
		{naobi::command::names::PRINTLN,
				[](const naobi::workflow_context::sptr& context, [[maybe_unused]]const naobi::command::argumentsList& args){
					auto val = context->stack.top();
					std::cout << *val << std::endl;
					context->stack.pop();
				}},
		{naobi::command::names::PRINT,
				[](const naobi::workflow_context::sptr& context, [[maybe_unused]]const naobi::command::argumentsList& args){
					auto val = context->stack.top();
					std::cout << *val;
					context->stack.pop();
				}},
		{naobi::command::names::INPUT,
				[](const naobi::workflow_context::sptr& context, [[maybe_unused]]const naobi::command::argumentsList& args){
					std::string str;
					std::cin >> str;
					utils::type::names type = utils::type::checkTypeFromInput(str);
					auto var = std::make_shared<naobi::variable>("temp", type);
					var->value() = utils::type::getValueFrom(type, str);
					context->stack.push(var);
				}},
		{naobi::command::names::RETURN,
				[](const naobi::workflow_context::sptr& context, [[maybe_unused]]const naobi::command::argumentsList& args){
					auto address = context->returnPoints.top();
					context->ip = address;
					context->returnPoints.pop();
				}},
		{naobi::command::names::CALL,
				[](const naobi::workflow_context::sptr& context, [[maybe_unused]]const naobi::command::argumentsList& args){
					auto address = context->ip;
					context->returnPoints.push(address);
					auto it = context->workflow->module()->findFunction(args[0]);
					context->ip = it->commands().begin();
				}},
		{naobi::command::names::JUMP,
				[](const naobi::workflow_context::sptr& context, [[maybe_unused]]const naobi::command::argumentsList& args)
				{
					context->ip += std::stoi(args[0]);
				}},
		{naobi::command::names::JUMP_IF,
				[](const naobi::workflow_context::sptr& context, [[maybe_unused]]const naobi::command::argumentsList& args)
				{
					if (context->stack.top() != true)
					{
						context->ip += std::stoi(args[0]);
					}
					context->stack.pop();
				}},
		{naobi::command::names::NOPE,
				[]([[maybe_unused]]const naobi::workflow_context::sptr& context, [[maybe_unused]]const naobi::command::argumentsList& args) noexcept{
				}},
		{naobi::command::names::EQ,
				[]([[maybe_unused]]const naobi::workflow_context::sptr& context, [[maybe_unused]]const naobi::command::argumentsList& args) noexcept{
					auto second = context->stack.top();
					context->stack.pop();
					auto first = context->stack.top();
					context->stack.pop();

					context->stack.push(naobi::operation_manager::get("==")->call(first->type(), second->type()).second(first, second));
				}},
		{naobi::command::names::GREATER,
				[]([[maybe_unused]]const naobi::workflow_context::sptr& context, [[maybe_unused]]const naobi::command::argumentsList& args) noexcept{
					auto second = context->stack.top();
					context->stack.pop();
					auto first = context->stack.top();
					context->stack.pop();

					context->stack.push(naobi::operation_manager::get(">")->call(first->type(), second->type()).second(first, second));
				}},
		{naobi::command::names::LESS,
				[]([[maybe_unused]]const naobi::workflow_context::sptr& context, [[maybe_unused]]const naobi::command::argumentsList& args) noexcept{
					auto second = context->stack.top();
					context->stack.pop();
					auto first = context->stack.top();
					context->stack.pop();

					context->stack.push(naobi::operation_manager::get("<")->call(first->type(), second->type()).second(first, second));
				}},
		{naobi::command::names::GREATER_OR_EQ,
				[]([[maybe_unused]]const naobi::workflow_context::sptr& context, [[maybe_unused]]const naobi::command::argumentsList& args) noexcept{
					auto second = context->stack.top();
					context->stack.pop();
					auto first = context->stack.top();
					context->stack.pop();

					context->stack.push(naobi::operation_manager::get(">=")->call(first->type(), second->type()).second(first, second));
				}},
		{naobi::command::names::LESS_OR_EQ,
				[]([[maybe_unused]]const naobi::workflow_context::sptr& context, [[maybe_unused]]const naobi::command::argumentsList& args) noexcept{
					auto second = context->stack.top();
					context->stack.pop();
					auto first = context->stack.top();
					context->stack.pop();

					context->stack.push(naobi::operation_manager::get("<=")->call(first->type(), second->type()).second(first, second));
				}},
		{naobi::command::names::NOT_EQ,
				[]([[maybe_unused]]const naobi::workflow_context::sptr& context, [[maybe_unused]]const naobi::command::argumentsList& args) noexcept{
					auto second = context->stack.top();
					context->stack.pop();
					auto first = context->stack.top();
					context->stack.pop();

					context->stack.push(naobi::operation_manager::get("!=")->call(first->type(), second->type()).second(first, second));
				}},
		{naobi::command::names::EXIT,
				[]([[maybe_unused]]const naobi::workflow_context::sptr& context, [[maybe_unused]]const naobi::command::argumentsList& args) noexcept{
					auto top = context->stack.top();
					std::exit(std::get<int>(top->value()));
				}},
		{naobi::command::names::INC,
				[]([[maybe_unused]]const naobi::workflow_context::sptr& context, [[maybe_unused]]const naobi::command::argumentsList& args) noexcept{
					auto top = context->stack.top();
					auto var = std::make_shared<naobi::variable>("temp", top->type());
					var->value() = 1;
					context->stack.top() += var;
				}},
		{naobi::command::names::DEC,
				[]([[maybe_unused]]const naobi::workflow_context::sptr& context, [[maybe_unused]]const naobi::command::argumentsList& args) noexcept{
					auto top = context->stack.top();
					auto var = std::make_shared<naobi::variable>("temp", top->type());
					var->value() = 1;
					context->stack.top() -= var;
				}},
		{naobi::command::names::ARISE,
				[]([[maybe_unused]]const naobi::workflow_context::sptr& context, [[maybe_unused]]const naobi::command::argumentsList& args) noexcept{
					context->eventManager->pushEvent(args[0]);
				}},
		{naobi::command::names::MOD,
				[]([[maybe_unused]]const naobi::workflow_context::sptr& context, [[maybe_unused]]const naobi::command::argumentsList& args) noexcept{
					auto second = context->stack.top();
					context->stack.pop();
					auto first = context->stack.top();
					context->stack.pop();

					context->stack.push(naobi::operation_manager::get("%")->call(first->type(), second->type()).second(first, second));
				}},
};

naobi::command
naobi::command::createCommand(naobi::command::names com, const naobi::command::argumentsList &args)
{
	naobi::command command;
	command.name = com;
	command.arguments = args;
	auto it = commands.find(com);
	if (it != commands.cend())
	{
		command.impl = it->second;
	}
	else
	{
		NCRITICAL(command, errors::NOT_SPECIFIED, "CRITICAL BROKEN COMMAND");
	}
	return command;
}