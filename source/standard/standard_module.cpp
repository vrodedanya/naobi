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
		function->addArgument("variable", utils::type::type(utils::type::names::INTEGER));
		std::vector<command> commands;
		commands.emplace_back(command::createCommand(command::names::LOAD, {"variable"}));
		commands.emplace_back(command::createCommand(command::names::EXIT, {}));
		function->setCommands(commands);
		addFunction(function);
	}
	{
		auto function = std::make_shared<naobi::function>("input");
		function->setReturnType(utils::type::type(utils::type::names::STRING));
		std::vector<naobi::command> commands;
		commands.emplace_back(command::createCommand(naobi::command::names::INPUT, {}));
		function->setCommands(commands);
		addFunction(function);
	}
	{
		auto function = std::make_shared<naobi::function>("str");
		function->setReturnType(utils::type::type(utils::type::names::STRING));
		function->addArgument("var", utils::type::type(utils::type::names::INTEGER));
		std::vector<naobi::command> commands;
		commands.emplace_back(command::createCommand(naobi::command::names::LOAD, {"var"}));
		commands.emplace_back(command::createCommand(naobi::command::names::I2S, {}));
		function->setCommands(commands);
		addFunction(function);
	}
	{
		auto function = std::make_shared<naobi::function>("str");
		function->setReturnType(utils::type::type(utils::type::names::STRING));
		function->addArgument("var", utils::type::type(utils::type::names::FLOAT));
		std::vector<naobi::command> commands;
		commands.emplace_back(command::createCommand(naobi::command::names::LOAD, {"var"}));
		commands.emplace_back(command::createCommand(naobi::command::names::F2S, {}));
		function->setCommands(commands);
		addFunction(function);
	}
	{
		auto function = std::make_shared<naobi::function>("str");
		function->setReturnType(utils::type::type(utils::type::names::STRING));
		function->addArgument("var", utils::type::type(utils::type::names::BOOLEAN));
		std::vector<naobi::command> commands;
		commands.emplace_back(command::createCommand(naobi::command::names::LOAD, {"var"}));
		commands.emplace_back(command::createCommand(naobi::command::names::B2S, {}));
		function->setCommands(commands);
		addFunction(function);
	}
	{
		auto function = std::make_shared<naobi::function>("int");
		function->setReturnType(utils::type::type(utils::type::names::INTEGER));
		function->addArgument("var", utils::type::type(utils::type::names::BOOLEAN));
		std::vector<naobi::command> commands;
		commands.emplace_back(command::createCommand(naobi::command::names::LOAD, {"var"}));
		commands.emplace_back(command::createCommand(naobi::command::names::B2I, {}));
		function->setCommands(commands);
		addFunction(function);
	}
	{
		auto function = std::make_shared<naobi::function>("int");
		function->setReturnType(utils::type::type(utils::type::names::INTEGER));
		function->addArgument("var", utils::type::type(utils::type::names::FLOAT));
		std::vector<naobi::command> commands;
		commands.emplace_back(command::createCommand(naobi::command::names::LOAD, {"var"}));
		commands.emplace_back(command::createCommand(naobi::command::names::F2I, {}));
		function->setCommands(commands);
		addFunction(function);
	}
	{
		auto function = std::make_shared<naobi::function>("int");
		function->setReturnType(utils::type::type(utils::type::names::INTEGER));
		function->addArgument("var", utils::type::type(utils::type::names::STRING));
		std::vector<naobi::command> commands;
		commands.emplace_back(command::createCommand(naobi::command::names::LOAD, {"var"}));
		commands.emplace_back(command::createCommand(naobi::command::names::S2I, {}));
		function->setCommands(commands);
		addFunction(function);
	}
	{
		auto function = std::make_shared<naobi::function>("float");
		function->setReturnType(utils::type::type(utils::type::names::FLOAT));
		function->addArgument("var", utils::type::type(utils::type::names::INTEGER));
		std::vector<naobi::command> commands;
		commands.emplace_back(command::createCommand(naobi::command::names::LOAD, {"var"}));
		commands.emplace_back(command::createCommand(naobi::command::names::I2F, {}));
		function->setCommands(commands);
		addFunction(function);
	}
	{
		auto function = std::make_shared<naobi::function>("float");
		function->setReturnType(utils::type::type(utils::type::names::FLOAT));
		function->addArgument("var", utils::type::type(utils::type::names::STRING));
		std::vector<naobi::command> commands;
		commands.emplace_back(command::createCommand(naobi::command::names::LOAD, {"var"}));
		commands.emplace_back(command::createCommand(naobi::command::names::S2F, {}));
		function->setCommands(commands);
		addFunction(function);
	}
	{
		auto function = std::make_shared<naobi::function>("bool");
		function->setReturnType(utils::type::type(utils::type::names::BOOLEAN));
		function->addArgument("var", utils::type::type(utils::type::names::INTEGER));
		std::vector<naobi::command> commands;
		commands.emplace_back(command::createCommand(naobi::command::names::LOAD, {"var"}));
		commands.emplace_back(command::createCommand(naobi::command::names::I2B, {}));
		function->setCommands(commands);
		addFunction(function);
	}
	{
		auto function = std::make_shared<naobi::function>("bool");
		function->setReturnType(utils::type::type(utils::type::names::BOOLEAN));
		function->addArgument("var", utils::type::type(utils::type::names::STRING));
		std::vector<naobi::command> commands;
		commands.emplace_back(command::createCommand(naobi::command::names::LOAD, {"var"}));
		commands.emplace_back(command::createCommand(naobi::command::names::S2B, {}));
		function->setCommands(commands);
		addFunction(function);
	}
	{
		auto templateFunction = std::make_shared<naobi::template_function>("get");
		templateFunction->setReturnType("ELEMENT");
		templateFunction->addArgument("variable", "CONTAINER<ELEMENT>");
		templateFunction->addArgument("index", "integer");
		templateFunction->setCode("__insert LOAD index;"
								  "__insert LOAD variable;"
								  "__insert GET;");
		addTemplateFunction(templateFunction);
	}
	{
		auto function = std::make_shared<naobi::function>("get");
		function->setReturnType(utils::type::type(utils::type::names::STRING));
		function->addArgument("variable", utils::type::type(utils::type::names::STRING));
		function->addArgument("index", utils::type::type(utils::type::names::INTEGER));
		std::vector<naobi::command> commands;
		commands.emplace_back(command::createCommand(naobi::command::names::LOAD, {"index"}));
		commands.emplace_back(command::createCommand(naobi::command::names::LOAD, {"variable"}));
		commands.emplace_back(command::createCommand(naobi::command::names::GET, {}));
		function->setCommands(commands);
		addFunction(function);
	}
	{
		auto templateFunction = std::make_shared<naobi::template_function>("set");
		templateFunction->setReturnType("CONTAINER<ELEMENT>");
		templateFunction->addArgument("variable", "CONTAINER<ELEMENT>");
		templateFunction->addArgument("sub", "ELEMENT");
		templateFunction->addArgument("index", "integer");
		templateFunction->setCode("__insert LOAD index;"
								  "__insert LOAD sub;"
								  "__insert LOAD variable;"
								  "__insert SET;");
		addTemplateFunction(templateFunction);
	}
	{
		auto function = std::make_shared<naobi::function>("set");
		function->setReturnType(utils::type::type(utils::type::names::STRING));
		function->addArgument("variable", utils::type::type(utils::type::names::STRING));
		function->addArgument("sub", utils::type::type(utils::type::names::STRING));
		function->addArgument("index", utils::type::type(utils::type::names::INTEGER));
		std::vector<naobi::command> commands;
		commands.emplace_back(command::createCommand(naobi::command::names::LOAD, {"index"}));
		commands.emplace_back(command::createCommand(naobi::command::names::LOAD, {"sub"}));
		commands.emplace_back(command::createCommand(naobi::command::names::LOAD, {"variable"}));
		commands.emplace_back(command::createCommand(naobi::command::names::SET, {}));
		function->setCommands(commands);
		addFunction(function);
	}
	{
		auto templateFunction = std::make_shared<naobi::template_function>("len");
		templateFunction->setReturnType("integer");
		templateFunction->addArgument("variable", "TYPE");
		templateFunction->setCode("__insert LOAD variable;"
								  "__insert LEN;");
		addTemplateFunction(templateFunction);
	}
	{
		auto templateFunction = std::make_shared<naobi::template_function>("append");
		templateFunction->setReturnType("TYPE");
		templateFunction->addArgument("variable", "TYPE");
		templateFunction->addArgument("additional", "TYPE2");
		templateFunction->setCode("__insert LOAD variable;"
								  "__insert LOAD additional;"
								  "__insert ADD;");
		addTemplateFunction(templateFunction);
	}
	{
		auto function = std::make_shared<naobi::function>("append");
		function->setReturnType(utils::type::type(utils::type::names::STRING));
		function->addArgument("variable", utils::type::type(utils::type::names::STRING));
		function->addArgument("additional", utils::type::type(utils::type::names::STRING));
		std::vector<naobi::command> commands;
		commands.emplace_back(command::createCommand(naobi::command::names::LOAD, {"variable"}));
		commands.emplace_back(command::createCommand(naobi::command::names::LOAD, {"additional"}));
		commands.emplace_back(command::createCommand(naobi::command::names::ADD, {}));
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
