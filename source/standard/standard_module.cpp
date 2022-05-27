#include <naobi/standard/standard_module.hpp>

naobi::standard::standard() : module("standard")
{
	{
		auto templateFunction = std::make_shared<naobi::template_function>("println");
		templateFunction->addArgument("variable", "TYPE");
		templateFunction->setCode("__insert LOAD variable;__insert PRINTLN;");
		addTemplateFunction(templateFunction);
	}
	{
		auto function = std::make_shared<naobi::function>("print");
		std::vector<command> commands;
		commands.emplace_back(command::createCommand(command::names::LOAD, {"variable"}));
		commands.emplace_back(command::createCommand(command::names::PRINT, {}));
		function->setCommands(commands);
		addFunction(function);
	}
	{
		auto function = std::make_shared<naobi::function>("exit");
		std::vector<command> commands;
		commands.emplace_back(command::createCommand(command::names::EXIT, {}));
		function->setCommands(commands);
		addFunction(function);
	}
	{
		auto function = std::make_shared<naobi::function>("input");
		std::vector<naobi::command> commands;
		commands.emplace_back(command::createCommand(naobi::command::names::INPUT, {}));
		function->setCommands(commands);
		addFunction(function);
	}
}
