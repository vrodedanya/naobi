#include <naobi/compiler/compiler.hpp>

#include <filesystem>

#include <naobi/utils/parser.hpp>
#include <naobi/utils/logger.hpp>

std::optional<std::string> naobi::compiler::loadFile(const std::string &fileName)
{
	LOG(compiler.loadFile, 5, "begin loading file ", fileName);
	std::ifstream inputStream(fileName);
	if (!inputStream.is_open()) return {};
	std::stringstream buffer;
	buffer << inputStream.rdbuf();
	std::string temp = buffer.str();
	LOG(compiler.loadFile, 4, "file content:\n", naobi::parser::placeAfter('\n' + temp, '\n', " | "));
	return temp;
}

std::vector<std::string> naobi::compiler::collectModules(const std::string &fileText)
{
	LOG(compiler.collectModules, 5, "begin collectModules from\n", naobi::parser::placeAfter('\n' + fileText, '\n', " | "));
	std::vector<std::string> buffer;
	auto temp = parser::removeSym(parser::removeExtraSpaces(fileText), '\n');
	auto lines = parser::split(temp, ";", false);
	LOG(compiler.collectModules, 5, "lines:\n", lines);
	for (const auto& line : lines)
	{
		auto arguments = parser::split(line, " ");
		if (arguments.size() == 2 && arguments[0] == "import")
		{
			buffer.emplace_back(arguments[1]);
		}
	}
	LOG(compiler.collectModules, 3, "collected modules\n", buffer);
	return buffer;
}

std::optional<naobi::composition> naobi::compiler::compile(const std::string &fileName)
{
	LOG(compiler.compile, 5, "begin compiling program");
	naobi::composition composition;

	std::filesystem::path path(naobi::parser::dirName(fileName));
	std::filesystem::current_path(path);
	LOG(compiler.compile, 3, "set current directory to ", naobi::parser::dirName(fileName));

	auto moduleOpt = compile(naobi::parser::fileName(fileName), composition.workflows);
	if (moduleOpt.has_value())
	{
		LOG(compiler.compile, 5, "get module ", moduleOpt.value()->name());
		composition.rootModule = moduleOpt.value();
		return composition;
	}
	else
	{
		LOG(compiler.compile, 1, "CRITICAL failed to compile module ", naobi::parser::fileName(fileName));
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
	LOG(compiler.compile, 5, "begin compiling file ", file);

	auto fileTextOpt = compiler::loadFile(file);
	if (!fileTextOpt.has_value())
	{
		LOG(compiler.compile, 1, "failed to open file ", file);
		return {};
	}

	auto module = std::make_shared<naobi::module>(naobi::parser::fileName(file));

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
			LOG(compiler.compile, 1, "CRITICAL failed to compile module " + naobi::parser::fileName(moduleName));
			std::exit(EXIT_FAILURE);
		}
	}
	return module;
}

