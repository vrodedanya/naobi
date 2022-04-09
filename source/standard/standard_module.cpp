#include <naobi/standard/standard_module.hpp>
#include <naobi/compiler/code_generator.hpp>

naobi::standard::standard() : module("standard")
{
	{
		auto function = std::make_shared<naobi::function>("println");
		std::vector<naobi::command> commands;
		commands.emplace_back(code_generator::createCommand(naobi::command::names::NOPE, {}));
		commands.emplace_back(code_generator::createCommand(naobi::command::names::PRINTLN, {}));
		commands.emplace_back(code_generator::createCommand(naobi::command::names::RETURN, {}));
		function->setCommands(commands);
		addFunction(function);
	}
	{
		auto function = std::make_shared<naobi::function>("print");
		std::vector<naobi::command> commands;
		commands.emplace_back(code_generator::createCommand(naobi::command::names::NOPE, {}));
		commands.emplace_back(code_generator::createCommand(naobi::command::names::PRINTLN, {}));
		commands.emplace_back(code_generator::createCommand(naobi::command::names::RETURN, {}));
		function->setCommands(commands);
		addFunction(function);
	}
	{
		auto function = std::make_shared<naobi::function>("input");
		std::vector<naobi::command> commands;
		commands.emplace_back(code_generator::createCommand(naobi::command::names::NOPE, {}));
		commands.emplace_back(code_generator::createCommand(naobi::command::names::INPUT, {}));
		commands.emplace_back(code_generator::createCommand(naobi::command::names::RETURN, {}));
		function->setCommands(commands);
		addFunction(function);
	}
}
