#include <naobi/interpreter/command.hpp>

#include <iostream>
#include <iomanip>

#include <naobi/utils/logger.hpp>
#include <naobi/interpreter/workflow_context.hpp>
#include <naobi/interpreter/event_manager.hpp>
#include <naobi/utils/operation.hpp>


std::map<naobi::command::names, naobi::command::implementation> naobi::command::commands =
	{
		{naobi::command::names::ADD,
		 [](
			 const naobi::workflow_context::sptr& context,
			 [[maybe_unused]]const naobi::command::argumentsList& args)
		 {
			 auto second = context->stack.top();
			 context->stack.pop();
			 auto first = context->stack.top();
			 context->stack.pop();

			 context->stack.push(
				 naobi::operation_manager::get("+")->call(
					 first->type(),
					 second->type()).second(
					 first, second));
		 }},
		{naobi::command::names::SUB,
		 [](
			 const naobi::workflow_context::sptr& context,
			 [[maybe_unused]]const naobi::command::argumentsList& args)
		 {
			 auto second = context->stack.top();
			 context->stack.pop();
			 auto first = context->stack.top();
			 context->stack.pop();

			 context->stack.push(
				 naobi::operation_manager::get("-")->call(
					 first->type(),
					 second->type()).second(
					 first, second));
		 }},
		{naobi::command::names::MUL,
		 [](
			 const naobi::workflow_context::sptr& context,
			 [[maybe_unused]]const naobi::command::argumentsList& args)
		 {
			 auto second = context->stack.top();
			 context->stack.pop();
			 auto first = context->stack.top();
			 context->stack.pop();

			 context->stack.push(
				 naobi::operation_manager::get("*")->call(
					 first->type(),
					 second->type()).second(
					 first, second));
		 }},
		{naobi::command::names::DIV,
		 [](
			 const naobi::workflow_context::sptr& context,
			 [[maybe_unused]]const naobi::command::argumentsList& args)
		 {
			 auto second = context->stack.top();
			 context->stack.pop();
			 auto first = context->stack.top();
			 context->stack.pop();

			 context->stack.push(
				 naobi::operation_manager::get("/")->call(
					 first->type(),
					 second->type()).second(
					 first, second));
		 }},
		{naobi::command::names::NEG,
		 [](
			 const naobi::workflow_context::sptr& context,
			 [[maybe_unused]]const naobi::command::argumentsList& args)
		 {
			 context->stack.top()->invert();
		 }},
		{naobi::command::names::NEW,
		 [](
			 const naobi::workflow_context::sptr& context,
			 [[maybe_unused]]const naobi::command::argumentsList& args)
		 {
			 auto type = utils::type::toType(args[1]);
			 auto var = std::make_shared<naobi::variable>(args[0], type);
			 context->variables[var->name()] = var;
		 }},
		{naobi::command::names::LOAD,
		 [](
			 const naobi::workflow_context::sptr& context,
			 [[maybe_unused]]const naobi::command::argumentsList& args)
		 { //
			 context->stack.push(context->variables[args[0]]->copy());
		 }},
		{naobi::command::names::SAVE,
		 [](
			 const naobi::workflow_context::sptr& context,
			 [[maybe_unused]]const naobi::command::argumentsList& args)
		 {
			 context->variables[args[0]]->value() = context->stack.top()->value();
			 context->stack.pop();
		 }},
		{naobi::command::names::PLACE,
		 [](
			 const naobi::workflow_context::sptr& context,
			 [[maybe_unused]]const naobi::command::argumentsList& args)
		 {
			 auto type = utils::type::toType(args[0]);
			 auto var = std::make_shared<naobi::variable>("temp", type);
			 var->value() = utils::type::getValueFrom(type, args[1]);
			 context->stack.push(var);
		 }},
		{naobi::command::names::PRINTLN,
		 [](
			 const naobi::workflow_context::sptr& context,
			 [[maybe_unused]]const naobi::command::argumentsList& args)
		 {
			 auto val = context->stack.top();
			 std::cout << *val << std::endl;
			 context->stack.pop();
		 }},
		{naobi::command::names::PRINT,
		 [](
			 const naobi::workflow_context::sptr& context,
			 [[maybe_unused]]const naobi::command::argumentsList& args)
		 {
			 auto val = context->stack.top();
			 std::cout << *val;
			 context->stack.pop();
		 }},
		{naobi::command::names::INPUT,
		 [](
			 const naobi::workflow_context::sptr& context,
			 [[maybe_unused]]const naobi::command::argumentsList& args)
		 {
			 std::string str;
			 std::cin >> str;
			 auto var = std::make_shared<naobi::variable>(
				 "temp",
				 utils::type::names::STRING);
			 var->value() = str;
			 context->stack.push(var);
		 }},
		{naobi::command::names::RETURN,
		 [](
			 const naobi::workflow_context::sptr& context,
			 [[maybe_unused]]const naobi::command::argumentsList& args)
		 {
			 auto address = context->returnPoints.top();
			 context->ip = address;
			 context->returnPoints.pop();
		 }},
		{naobi::command::names::CALL,
		 [](
			 const naobi::workflow_context::sptr& context,
			 [[maybe_unused]]const naobi::command::argumentsList& args)
		 {
			 auto address = context->ip;
			 context->returnPoints.push(address);
			 auto it = context->workflow->module()->findFunctionWithNumber(
				 args[0],
				 std::stoull(
					 args[1]));
			 context->ip = it->commands().begin();
		 }},
		{naobi::command::names::JUMP,
		 [](
			 const naobi::workflow_context::sptr& context,
			 [[maybe_unused]]const naobi::command::argumentsList& args)
		 {
			 context->ip += std::stoi(args[0]);
		 }},
		{naobi::command::names::JUMP_IF,
		 [](
			 const naobi::workflow_context::sptr& context,
			 [[maybe_unused]]const naobi::command::argumentsList& args)
		 {
			 if (context->stack.top() != true)
			 {
				 context->ip += std::stoi(args[0]);
			 }
			 context->stack.pop();
		 }},
		{naobi::command::names::NOPE,
		 [](
			 [[maybe_unused]]const naobi::workflow_context::sptr& context,
			 [[maybe_unused]]const naobi::command::argumentsList& args) noexcept
		 {
		 }},
		{naobi::command::names::EQ,
		 [](
			 [[maybe_unused]]const naobi::workflow_context::sptr& context,
			 [[maybe_unused]]const naobi::command::argumentsList& args) noexcept
		 {
			 auto second = context->stack.top();
			 context->stack.pop();
			 auto first = context->stack.top();
			 context->stack.pop();

			 context->stack.push(
				 naobi::operation_manager::get("==")->call(
					 first->type(),
					 second->type()).second(
					 first, second));
		 }},
		{naobi::command::names::GREATER,
		 [](
			 [[maybe_unused]]const naobi::workflow_context::sptr& context,
			 [[maybe_unused]]const naobi::command::argumentsList& args) noexcept
		 {
			 auto second = context->stack.top();
			 context->stack.pop();
			 auto first = context->stack.top();
			 context->stack.pop();

			 context->stack.push(
				 naobi::operation_manager::get(">")->call(
					 first->type(),
					 second->type()).second(
					 first, second));
		 }},
		{naobi::command::names::LESS,
		 [](
			 [[maybe_unused]]const naobi::workflow_context::sptr& context,
			 [[maybe_unused]]const naobi::command::argumentsList& args) noexcept
		 {
			 auto second = context->stack.top();
			 context->stack.pop();
			 auto first = context->stack.top();
			 context->stack.pop();

			 context->stack.push(
				 naobi::operation_manager::get("<")->call(
					 first->type(),
					 second->type()).second(
					 first, second));
		 }},
		{naobi::command::names::GREATER_OR_EQ,
		 [](
			 [[maybe_unused]]const naobi::workflow_context::sptr& context,
			 [[maybe_unused]]const naobi::command::argumentsList& args) noexcept
		 {
			 auto second = context->stack.top();
			 context->stack.pop();
			 auto first = context->stack.top();
			 context->stack.pop();

			 context->stack.push(
				 naobi::operation_manager::get(">=")->call(
					 first->type(),
					 second->type()).second(
					 first, second));
		 }},
		{naobi::command::names::LESS_OR_EQ,
		 [](
			 [[maybe_unused]]const naobi::workflow_context::sptr& context,
			 [[maybe_unused]]const naobi::command::argumentsList& args) noexcept
		 {
			 auto second = context->stack.top();
			 context->stack.pop();
			 auto first = context->stack.top();
			 context->stack.pop();

			 context->stack.push(
				 naobi::operation_manager::get("<=")->call(
					 first->type(),
					 second->type()).second(
					 first, second));
		 }},
		{naobi::command::names::NOT_EQ,
		 [](
			 [[maybe_unused]]const naobi::workflow_context::sptr& context,
			 [[maybe_unused]]const naobi::command::argumentsList& args) noexcept
		 {
			 auto second = context->stack.top();
			 context->stack.pop();
			 auto first = context->stack.top();
			 context->stack.pop();

			 context->stack.push(
				 naobi::operation_manager::get("!=")->call(
					 first->type(),
					 second->type()).second(
					 first, second));
		 }},
		{naobi::command::names::EXIT,
		 [](
			 [[maybe_unused]]const naobi::workflow_context::sptr& context,
			 [[maybe_unused]]const naobi::command::argumentsList& args) noexcept
		 {
			 auto top = context->stack.top();
			 std::exit(std::get<int>(top->value()));
		 }},
		{naobi::command::names::INC,
		 [](
			 [[maybe_unused]]const naobi::workflow_context::sptr& context,
			 [[maybe_unused]]const naobi::command::argumentsList& args) noexcept
		 {
			 auto top = context->stack.top();
			 auto var = std::make_shared<naobi::variable>("temp", top->type());
			 var->value() = 1;
			 context->stack.push(
				 naobi::operation_manager::get("+")->call(
					 var->type(),
					 top->type()).second(
					 var, top));
		 }},
		{naobi::command::names::DEC,
		 [](
			 [[maybe_unused]]const naobi::workflow_context::sptr& context,
			 [[maybe_unused]]const naobi::command::argumentsList& args) noexcept
		 {
			 auto top = context->stack.top();
			 auto var = std::make_shared<naobi::variable>("temp", top->type());
			 var->value() = 1;
			 context->stack.push(
				 naobi::operation_manager::get("-")->call(
					 top->type(),
					 var->type()
														 ).second(
					 top, var));
		 }},
		{naobi::command::names::ARISE,
		 [](
			 [[maybe_unused]]const naobi::workflow_context::sptr& context,
			 [[maybe_unused]]const naobi::command::argumentsList& args) noexcept
		 {
			 auto event = context->workflow->module()->findEvent(args[0]).value();
			 for (auto& argument : event.getArguments())
			 {
				 auto argName = std::get<0>(argument);
				 auto it = context->variables.find(argName);
				 std::get<2>(argument) = it->second;
				 context->variables.erase(it);
			 }
			 context->eventManager->eventPool().push(event);
		 }},
		{naobi::command::names::MOD,
		 [](
			 [[maybe_unused]]const naobi::workflow_context::sptr& context,
			 [[maybe_unused]]const naobi::command::argumentsList& args) noexcept
		 {
			 auto second = context->stack.top();
			 context->stack.pop();
			 auto first = context->stack.top();
			 context->stack.pop();

			 context->stack.push(
				 naobi::operation_manager::get("%")->call(
					 first->type(),
					 second->type()).second(
					 first, second));
		 }},
		{naobi::command::names::I2S, [](
			const naobi::workflow_context::sptr& context,
			const naobi::command::argumentsList& args)
		{
			auto top = context->stack.top();
			top->type() = utils::type::names::STRING;
			top->value() = std::to_string(std::get<int>(top->value()));
		}},
		{naobi::command::names::F2S, [](
			const naobi::workflow_context::sptr& context,
			const naobi::command::argumentsList& args)
		{
			auto top = context->stack.top();
			std::ostringstream oss;
			oss << std::setprecision(8) << std::noshowpoint
				<< std::get<double>(top->value());
			top->type() = utils::type::names::STRING;
			top->value() = oss.str();
		}},
		{naobi::command::names::B2S, [](
			const naobi::workflow_context::sptr& context,
			const naobi::command::argumentsList& args)
		{
			auto top = context->stack.top();
			top->type() = utils::type::names::STRING;
			top->value() = std::get<bool>(top->value()) ? "true" : "false";
		}},
		{naobi::command::names::S2I, [](
			const naobi::workflow_context::sptr& context,
			const naobi::command::argumentsList& args)
		{
			auto top = context->stack.top();
			top->type() = utils::type::names::INTEGER;
			try
			{
				top->value() = std::stoi(std::get<std::string>(top->value()));
			}
			catch (const std::invalid_argument& exception)
			{
				throw naobi::naobi_exception("CastException", "Failed to cast string to integer");
			}
		}},
		{naobi::command::names::F2I, [](
			const naobi::workflow_context::sptr& context,
			const naobi::command::argumentsList& args)
		{
			auto top = context->stack.top();
			top->type() = utils::type::names::INTEGER;
			top->value() = static_cast<int>(std::get<double>(top->value()));
		}},
		{naobi::command::names::B2I, [](
			const naobi::workflow_context::sptr& context,
			const naobi::command::argumentsList& args)
		{
			auto top = context->stack.top();
			top->type() = utils::type::names::INTEGER;
			top->value() = static_cast<int>(std::get<bool>(top->value()));
		}},
		{naobi::command::names::I2F, [](
			const naobi::workflow_context::sptr& context,
			const naobi::command::argumentsList& args)
		{
			auto top = context->stack.top();
			top->type() = utils::type::names::FLOAT;
			top->value() = static_cast<double>(std::get<int>(top->value()));
		}},
		{naobi::command::names::S2F, [](
			const naobi::workflow_context::sptr& context,
			const naobi::command::argumentsList& args)
		{
			auto top = context->stack.top();
			top->type() = utils::type::names::FLOAT;
			try
			{
				top->value() = std::stod(std::get<std::string>(top->value()));
			}
			catch (const std::invalid_argument& exception)
			{
				throw naobi::naobi_exception("CastException", "Failed to cast string to float");
			}
		}},
		{naobi::command::names::I2B, [](
			const naobi::workflow_context::sptr& context,
			const naobi::command::argumentsList& args)
		{
			auto top = context->stack.top();
			top->type() = utils::type::names::BOOLEAN;
			top->value() = static_cast<bool>(std::get<int>(top->value()));
		}},
		{naobi::command::names::S2B, [](
			const naobi::workflow_context::sptr& context,
			const naobi::command::argumentsList& args)
		{
			auto top = context->stack.top();
			top->type() = utils::type::names::BOOLEAN;
			top->value() = std::get<std::string>(top->value()) == "true";
		}},
		{naobi::command::names::CATCH, [](
			const naobi::workflow_context::sptr& context,
			const naobi::command::argumentsList& args) noexcept
		{

		}},
		{naobi::command::names::THROW, [](
			const naobi::workflow_context::sptr& context,
			const naobi::command::argumentsList& args)
		{
			throw naobi::naobi_exception(args[0], std::get<std::string>(context->stack.top()->value()));
		}},
	};

std::map<std::string, naobi::command::names> naobi::command::stringToCommand
	{
		{"NEW", naobi::command::names::NEW},
		{"INC", naobi::command::names::INC},
		{"DEC", naobi::command::names::DEC},
		{"ADD", naobi::command::names::ADD},
		{"SUB", naobi::command::names::SUB},
		{"MUL", naobi::command::names::MUL},
		{"DIV", naobi::command::names::DIV},
		{"NEG", naobi::command::names::NEG},
		{"MOD", naobi::command::names::MOD},
		{"EQ", naobi::command::names::EQ},
		{"GREATER", naobi::command::names::GREATER},
		{"LESS", naobi::command::names::LESS},
		{"GREATER_OR_EQ", naobi::command::names::GREATER_OR_EQ},
		{"LESS_OR_EQ", naobi::command::names::LESS_OR_EQ},
		{"NOT_EQ", naobi::command::names::NOT_EQ},
		{"LOAD", naobi::command::names::LOAD},
		{"SAVE", naobi::command::names::SAVE},
		{"PLACE", naobi::command::names::PLACE},
		{"PRINTLN", naobi::command::names::PRINTLN},
		{"PRINT", naobi::command::names::PRINT},
		{"INPUT", naobi::command::names::INPUT},
		{"RETURN", naobi::command::names::RETURN},
		{"CALL", naobi::command::names::CALL},
		{"JUMP", naobi::command::names::JUMP},
		{"JUMP_IF", naobi::command::names::JUMP_IF},
		{"EXIT", naobi::command::names::EXIT},
		{"NOPE", naobi::command::names::NOPE},
		{"ARISE", naobi::command::names::ARISE},
		{"I2S", naobi::command::names::I2S},
		{"F2S", naobi::command::names::F2S},
		{"B2S", naobi::command::names::B2S},
		{"S2I", naobi::command::names::S2I},
		{"F2I", naobi::command::names::F2I},
		{"B2I", naobi::command::names::B2I},
		{"I2F", naobi::command::names::I2F},
		{"S2F", naobi::command::names::S2F},
		{"I2B", naobi::command::names::I2B},
		{"S2B", naobi::command::names::S2B},
		{"CATCH", naobi::command::names::CATCH},
		{"THROW", naobi::command::names::THROW},
	};

naobi::command
naobi::command::createCommand(naobi::command::names com, const naobi::command::argumentsList& args)
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