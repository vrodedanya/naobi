#include <naobi/standard/standard_module.hpp>
#include <naobi/compiler/code_generator.hpp>

naobi::standard::standard() : module("standard")
{
	{
		auto function = std::make_shared<naobi::function>("println");
		function->addArgument("variable", utils::type::names::DYNAMIC);
		std::vector<command> commands;
		commands.emplace_back(code_generator::createCommand(command::names::LOAD, {"variable"}));
		commands.emplace_back(code_generator::createCommand(command::names::PRINTLN, {}));
		function->setCommands(commands);
		addFunction(function);
	}
	{
		auto function = std::make_shared<naobi::function>("print");
		function->addArgument("variable", utils::type::names::DYNAMIC);
		std::vector<command> commands;
		commands.emplace_back(code_generator::createCommand(command::names::LOAD, {"variable"}));
		commands.emplace_back(code_generator::createCommand(command::names::PRINT, {}));
		function->setCommands(commands);
		addFunction(function);
	}
	{
		auto function = std::make_shared<naobi::function>("exit");
		std::vector<command> commands;
		commands.emplace_back(code_generator::createCommand(command::names::EXIT, {}));
		function->setCommands(commands);
		addFunction(function);
	}
	{
		auto function = std::make_shared<naobi::function>("input");
		std::vector<naobi::command> commands;
		commands.emplace_back(code_generator::createCommand(naobi::command::names::INPUT, {}));
		function->setCommands(commands);
		addFunction(function);
	}
}
