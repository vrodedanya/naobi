#include <naobi/compiler/compiler.hpp>

#include <filesystem>
#include <algorithm>

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

std::vector<std::string> naobi::compiler::collectModules(const std::vector<std::string>& lines)
{
	LOG(compiler.collectModules, logger::LOW, "begin collectModules");
	std::vector<std::string> buffer;
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

void naobi::compiler::compile(const std::string &fileName)
{
	LOG(compiler.compile, logger::LOW, "begin compiling program");

	std::filesystem::path path(naobi::parser::dirName(fileName));
	std::filesystem::current_path(path);
	LOG(compiler.compile, logger::IMPORTANT, "set current directory to ", naobi::parser::dirName(fileName));

	compile(naobi::parser::fileName(fileName), nullptr);
}

void naobi::compiler::compile(const std::string &fileName, const naobi::module::sptr& parent)
{
	LOG(compiler.compile, logger::BASIC, "process file name");
	std::string file = processFileName(fileName);

	LOG(compiler.compile, logger::LOW, "begin compiling file ", file);

	auto fileTextOpt = compiler::loadFile(file);
	if (!fileTextOpt.has_value())
	{
		LOG(compiler.compile, logger::CRITICAL, "CRITICAL failed to open file '", file, "'");
		std::exit(EXIT_FAILURE);
	}

	auto module = std::make_shared<naobi::module>(file);
	if (parent == nullptr)
	{
		_composition.rootModule = module;
	}
	else
	{
		parent->addModule(module);
	}

	auto temp = parser::removeSym(parser::removeExtraSpaces(fileTextOpt.value()), '\n');
	auto lines = parser::split(temp, ";", false);
	LOG(compiler.compile, logger::LOW, "lines:\n", lines);

	processModules(lines, module);

	LOG(compiler.compile, logger::SUCCESS, "compiled '", file, "'");
}

std::string naobi::compiler::processFileName(const std::string &fileName)
{
	std::string file = fileName;
	std::size_t pos = file.find_last_of('.');
	if (pos == std::string::npos || file.substr(pos) != ".naobi")
	{
		file += ".naobi";
	}
	pos = 0;
	if (file[0] == '.' && file[1] == '.') pos = 2;
	else if (file[0] == '.') pos = 1;
	file = naobi::parser::replaceSym(file.substr(pos, file.find_last_of('.') - pos), '.', '/') + ".naobi";
	return file;
}

void naobi::compiler::processModules(const std::vector<std::string> &lines, const naobi::module::sptr& module)
{
	auto modulesNames = compiler::collectModules(lines);
	for (const auto& moduleName : modulesNames)
	{
		std::string file = processFileName(moduleName);
		if (file == module->name())
		{
			LOG(compiler.processModules, logger::CRITICAL, "CRITICAL module '", file, "' import itself");
			std::exit(EXIT_FAILURE);
		}
		auto ptr = _composition.rootModule->findModule(file);
		if (ptr != nullptr)
		{
			module->addModule(ptr);
			continue;
		}
		compile(moduleName, module);
	}
}

