#include <naobi/compiler/compiler.hpp>

#include <fstream>
#include <filesystem>

#include <naobi/utils/parser.hpp>

std::optional<std::string> naobi::compiler::loadFile(const std::string &fileName)
{
	std::ifstream inputStream(fileName);
	if (!inputStream.is_open()) return {};
	std::stringstream buffer;
	buffer << inputStream.rdbuf();
	return buffer.str();
}

std::vector<std::string> naobi::compiler::collectModules(const std::string &fileText)
{
	std::vector<std::string> buffer;
	auto temp = parser::removeSym(parser::removeExtraSpaces(fileText), '\n');
	auto lines = parser::split(temp, ";", false);
	for (const auto& line : lines)
	{
		auto arguments = parser::split(line, " ");
		if (arguments.size() == 2 && arguments[0] == "import")
		{
			buffer.emplace_back(arguments[1]);
		}
	}
	return buffer;
}

std::optional<naobi::composition> naobi::compiler::compile(const std::string &fileName)
{
	naobi::composition composition;

	std::filesystem::path path(naobi::parser::dirName(fileName));
	std::filesystem::current_path(path);
	auto moduleOpt = compile(naobi::parser::fileName(fileName), composition.workflows);
	if (moduleOpt.has_value())// get some error. Todo add logging
	{
		composition.rootModule = moduleOpt.value();
		return composition;
	}
	else return {};
}

std::optional<naobi::module*> naobi::compiler::compile(const std::string &fileName, std::vector<naobi::workflow> &workflows)
{
	auto fileTextOpt = compiler::loadFile(fileName);
	if (!fileTextOpt.has_value()) return {};

	auto* module = new naobi::module(fileName);

	auto modulesNames = compiler::collectModules(fileTextOpt.value());
	for (const auto& moduleName : modulesNames)
	{
		auto moduleOpt = compile(moduleName, workflows);
		if (moduleOpt.has_value())
		{
			module->addModule(moduleOpt.value());
		}
		else
		{
			// TODO add error handling
		}
	}
	return module;
}

