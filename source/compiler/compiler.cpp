#include <naobi/compiler/compiler.hpp>

#include <filesystem>

#include <naobi/utils/parser.hpp>
#include <naobi/utils/logger.hpp>

std::optional<std::string> naobi::compiler::loadFile(const std::string &fileName)
{
	LOG(compiler.loadFile, logger::LOW, "begin loading file ", fileName);
	std::ifstream inputStream(fileName);
	if (!inputStream.is_open()) return {};
	std::stringstream buffer;
	buffer << inputStream.rdbuf();
	std::string temp = buffer.str();
	LOG(compiler.loadFile, logger::BASIC, "file content:\n", naobi::parser::placeAfter('\n' + temp, '\n', " | "));
	return temp;
}

std::vector<std::string> naobi::compiler::collectModules(const std::string &fileText)
{
	LOG(compiler.collectModules, logger::LOW, "begin collectModules from\n", naobi::parser::placeAfter('\n' + fileText, '\n', " | "));
	std::vector<std::string> buffer;
	auto temp = parser::removeSym(parser::removeExtraSpaces(fileText), '\n');
	auto lines = parser::split(temp, ";", false);
	LOG(compiler.collectModules, logger::LOW, "lines:\n", lines);
	for (const auto& line : lines)
	{
		auto arguments = parser::split(line, " ");
		if (arguments.size() == 2 && arguments[0] == "import")
		{
			buffer.emplace_back(arguments[1]);
		}
	}
	LOG(compiler.collectModules, logger::IMPORTANT, "collected modules\n", buffer);
	return buffer;
}

std::optional<naobi::composition> naobi::compiler::compile(const std::string &fileName)
{
	LOG(compiler.compile, logger::LOW, "begin compiling program");
	naobi::composition composition;

	std::filesystem::path path(naobi::parser::dirName(fileName));
	std::filesystem::current_path(path);
	LOG(compiler.compile, logger::IMPORTANT, "set current directory to ", naobi::parser::dirName(fileName));

	auto moduleOpt = compile(naobi::parser::fileName(fileName), composition.workflows);
	if (moduleOpt.has_value())
	{
		LOG(compiler.compile, logger::LOW, "get module ", moduleOpt.value()->name());
		LOG(compiler.compile, logger::SUCCESS, "compiled ", moduleOpt.value()->name());

		composition.rootModule = moduleOpt.value();
		return composition;
	}
	else
	{
		LOG(compiler.compile, logger::CRITICAL, "CRITICAL failed to compile module '", naobi::parser::fileName(fileName), "'");
		std::exit(EXIT_FAILURE);
	}
}

std::optional<naobi::module::sptr> naobi::compiler::compile(const std::string &fileName, std::vector<naobi::workflow> &workflows)
{
	std::string file = fileName;
	std::size_t pos = file.find_last_of('.');
	if (pos == std::string::npos || file.substr(pos) != ".naobi")
	{
		file += ".naobi";
	}
	file = naobi::parser::replaceSym(file.substr(0, file.find_last_of('.')), '.', '/') + ".naobi";
	LOG(compiler.compile, logger::LOW, "begin compiling file ", file);

	auto fileTextOpt = compiler::loadFile(file);
	if (!fileTextOpt.has_value())
	{
		LOG(compiler.compile, logger::CRITICAL, "failed to open file '", file, "'");
		return {};
	}

	auto module = std::make_shared<naobi::module>(naobi::parser::fileName(file));

	auto modulesNames = compiler::collectModules(fileTextOpt.value());
	for (const auto& moduleName : modulesNames)
	{
		auto moduleOpt = compile(moduleName, workflows);
		if (moduleOpt.has_value())
		{
			LOG(compiler.compile, logger::LOW, "get module ", moduleOpt.value()->name());
			LOG(compiler.compile, logger::SUCCESS, "compiled ", moduleOpt.value()->name());
			module->addModule(moduleOpt.value());
		}
		else
		{
			LOG(compiler.compile, logger::CRITICAL, "CRITICAL failed to compile module '" + naobi::parser::fileName(moduleName), "'");
			std::exit(EXIT_FAILURE);
		}
	}
	return module;
}

