#include <naobi/standard/standard_module.hpp>

naobi::standard::standard() : module("standard")
{
	{
		auto function = std::make_shared<naobi::function>("println");
		function->addArgument("variable", utils::type::names::STRING);
		std::vector<command> commands;
		commands.emplace_back(command::createCommand(command::names::LOAD, {"variable"}));
		commands.emplace_back(command::createCommand(command::names::PRINTLN, {}));
		function->setCommands(commands);
		addFunction(function);
	}
	{
		auto function = std::make_shared<naobi::function>("println");
		function->addArgument("variable", utils::type::names::FLOAT);
		std::vector<command> commands;
		commands.emplace_back(command::createCommand(command::names::LOAD, {"variable"}));
		commands.emplace_back(command::createCommand(command::names::PRINTLN, {}));
		function->setCommands(commands);
		addFunction(function);
	}
	{
		auto function = std::make_shared<naobi::function>("println");
		function->addArgument("variable", utils::type::names::BOOLEAN);
		std::vector<command> commands;
		commands.emplace_back(command::createCommand(command::names::LOAD, {"variable"}));
		commands.emplace_back(command::createCommand(command::names::PRINTLN, {}));
		function->setCommands(commands);
		addFunction(function);
	}
	{
		auto function = std::make_shared<naobi::function>("println");
		function->addArgument("variable", utils::type::names::INTEGER);
		std::vector<command> commands;
		commands.emplace_back(command::createCommand(command::names::LOAD, {"variable"}));
		commands.emplace_back(command::createCommand(command::names::PRINTLN, {}));
		function->setCommands(commands);
		addFunction(function);
	}
	{
		auto function = std::make_shared<naobi::function>("print");
		function->addArgument("variable", utils::type::names::TEMPLATE);
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
