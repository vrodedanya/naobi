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
		auto templateFunction = std::make_shared<naobi::template_function>("print");
		templateFunction->addArgument("variable", "TYPE");
		templateFunction->setCode("__insert LOAD variable;__insert PRINT;");
		addTemplateFunction(templateFunction);
	}
	{
		auto function = std::make_shared<naobi::function>("exit");
		function->addArgument("variable", utils::type::names::INTEGER);
		std::vector<command> commands;
		commands.emplace_back(command::createCommand(command::names::LOAD, {"variable"}));
		commands.emplace_back(command::createCommand(command::names::EXIT, {}));
		function->setCommands(commands);
		addFunction(function);
	}
	{
		auto function = std::make_shared<naobi::function>("input");
		function->setReturnType(utils::type::names::STRING);
		std::vector<naobi::command> commands;
		commands.emplace_back(command::createCommand(naobi::command::names::INPUT, {}));
		function->setCommands(commands);
		addFunction(function);
	}
	{
		auto function = std::make_shared<naobi::function>("str");
		function->setReturnType(utils::type::names::STRING);
		function->addArgument("var", utils::type::names::INTEGER);
		std::vector<naobi::command> commands;
		commands.emplace_back(command::createCommand(naobi::command::names::LOAD, {"var"}));
		commands.emplace_back(command::createCommand(naobi::command::names::I2S, {}));
		function->setCommands(commands);
		addFunction(function);
	}
	{
		auto function = std::make_shared<naobi::function>("str");
		function->setReturnType(utils::type::names::STRING);
		function->addArgument("var", utils::type::names::FLOAT);
		std::vector<naobi::command> commands;
		commands.emplace_back(command::createCommand(naobi::command::names::LOAD, {"var"}));
		commands.emplace_back(command::createCommand(naobi::command::names::F2S, {}));
		function->setCommands(commands);
		addFunction(function);
	}
	{
		auto function = std::make_shared<naobi::function>("str");
		function->setReturnType(utils::type::names::STRING);
		function->addArgument("var", utils::type::names::BOOLEAN);
		std::vector<naobi::command> commands;
		commands.emplace_back(command::createCommand(naobi::command::names::LOAD, {"var"}));
		commands.emplace_back(command::createCommand(naobi::command::names::B2S, {}));
		function->setCommands(commands);
		addFunction(function);
	}
	{
		auto function = std::make_shared<naobi::function>("int");
		function->setReturnType(utils::type::names::INTEGER);
		function->addArgument("var", utils::type::names::BOOLEAN);
		std::vector<naobi::command> commands;
		commands.emplace_back(command::createCommand(naobi::command::names::LOAD, {"var"}));
		commands.emplace_back(command::createCommand(naobi::command::names::B2I, {}));
		function->setCommands(commands);
		addFunction(function);
	}
	{
		auto function = std::make_shared<naobi::function>("int");
		function->setReturnType(utils::type::names::INTEGER);
		function->addArgument("var", utils::type::names::FLOAT);
		std::vector<naobi::command> commands;
		commands.emplace_back(command::createCommand(naobi::command::names::LOAD, {"var"}));
		commands.emplace_back(command::createCommand(naobi::command::names::F2I, {}));
		function->setCommands(commands);
		addFunction(function);
	}
	{
		auto function = std::make_shared<naobi::function>("int");
		function->setReturnType(utils::type::names::INTEGER);
		function->addArgument("var", utils::type::names::STRING);
		std::vector<naobi::command> commands;
		commands.emplace_back(command::createCommand(naobi::command::names::LOAD, {"var"}));
		commands.emplace_back(command::createCommand(naobi::command::names::S2I, {}));
		function->setCommands(commands);
		addFunction(function);
	}
	{
		auto function = std::make_shared<naobi::function>("float");
		function->setReturnType(utils::type::names::FLOAT);
		function->addArgument("var", utils::type::names::INTEGER);
		std::vector<naobi::command> commands;
		commands.emplace_back(command::createCommand(naobi::command::names::LOAD, {"var"}));
		commands.emplace_back(command::createCommand(naobi::command::names::I2F, {}));
		function->setCommands(commands);
		addFunction(function);
	}
	{
		auto function = std::make_shared<naobi::function>("float");
		function->setReturnType(utils::type::names::FLOAT);
		function->addArgument("var", utils::type::names::STRING);
		std::vector<naobi::command> commands;
		commands.emplace_back(command::createCommand(naobi::command::names::LOAD, {"var"}));
		commands.emplace_back(command::createCommand(naobi::command::names::S2F, {}));
		function->setCommands(commands);
		addFunction(function);
	}
	{
		auto function = std::make_shared<naobi::function>("bool");
		function->setReturnType(utils::type::names::BOOLEAN);
		function->addArgument("var", utils::type::names::INTEGER);
		std::vector<naobi::command> commands;
		commands.emplace_back(command::createCommand(naobi::command::names::LOAD, {"var"}));
		commands.emplace_back(command::createCommand(naobi::command::names::I2B, {}));
		function->setCommands(commands);
		addFunction(function);
	}
	{
		auto function = std::make_shared<naobi::function>("bool");
		function->setReturnType(utils::type::names::BOOLEAN);
		function->addArgument("var", utils::type::names::STRING);
		std::vector<naobi::command> commands;
		commands.emplace_back(command::createCommand(naobi::command::names::LOAD, {"var"}));
		commands.emplace_back(command::createCommand(naobi::command::names::S2B, {}));
		function->setCommands(commands);
		addFunction(function);
	}
	{
		auto exception = naobi::exception();
		exception.name = "CastException";
		addException(exception);
	}
	{
		auto exception = naobi::exception();
		exception.name = "CppException";
		addException(exception);
	}
}
