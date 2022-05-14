#include <naobi/compiler/compiler.hpp>

#include <filesystem>
#include <algorithm>

#include <naobi/utils/parser.hpp>
#include <naobi/utils/logger.hpp>
#include <naobi/utils/keywords.hpp>
#include <naobi/compiler/code_generator.hpp>
#include <naobi/standard/standard_module.hpp>
#include <naobi/interpreter/event_manager.hpp>

void naobi::compiler::compile(const std::string &fileName)
{
	LOG(compiler.compile, logger::LOW, "begin compiling program");

	if (fileName.find('/') != std::string::npos)
	{
		std::filesystem::path path(naobi::parser::dirName(fileName));
		if (!std::filesystem::is_directory(path))
		{
			LOG(compiler.compile, logger::CRITICAL, "CRITICAL directory ", naobi::parser::dirName(fileName), " doesn't exist");
			exit(EXIT_FAILURE);
		}
		std::filesystem::current_path(path);
		LOG(compiler.compile, logger::IMPORTANT, "set current directory to ", naobi::parser::dirName(fileName));
	}

	compile(naobi::parser::fileName(fileName), nullptr);
}

void naobi::compiler::compileText(const std::string& text)
{
	const char* moduleName = "console";
	auto module = std::make_shared<naobi::module>(moduleName);
	_root = module;

	std::string fileContent = text;
	naobi::parser::removeComments(fileContent);
	LOG(compiler.compile, logger::LOW, "after removing comments:\n", fileContent);

	auto temp = parser::replaceSym(parser::removeExtraSpaces(fileContent), '\n', ' ');
	auto lines = parser::split(temp, parser::isEnds(";}"),{},{{'{','}'},{'"','"'}});
	LOG(compiler.compile, logger::LOW, "lines:\n", lines);

	processImportingModules(lines, module);

	processModule(lines, module);

	LOG(compiler.compile, logger::SUCCESS, "compiled '", moduleName, "'");
}

void naobi::compiler::compile(const std::string &fileName, const naobi::module::sptr& parent)
{
	if (fileName == "standard;")
	{
		if (parent == nullptr)
		{
			LOG(compiler.compile, logger::CRITICAL, "CRITICAL standard module without parent");
			std::exit(EXIT_FAILURE);
		}
		parent->addModule(std::shared_ptr<naobi::module>(new naobi::standard()));
		return;
	}
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
	if (parent != nullptr)
	{
		parent->addModule(module);
	}
	else
	{
		_root = module;
	}
	std::string fileContent = fileTextOpt.value();
	naobi::parser::removeComments(fileContent);
	LOG(compiler.compile, logger::LOW, "after removing comments:\n", fileContent);

	auto temp = parser::replaceSym(parser::removeExtraSpaces(fileContent), '\n', ' ');
	auto lines = parser::split(temp, parser::isEnds(";}"),{},{{'{','}'},{'"','"'}});
	LOG(compiler.compile, logger::LOW, "lines:\n", lines);

	processImportingModules(lines, module);

	processModule(lines, module);

	LOG(compiler.compile, logger::SUCCESS, "compiled '", file, "'");
}

std::string naobi::compiler::processFileName(const std::string &fileName)
{
	std::string file = naobi::parser::removeSym(fileName, ';');
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

void naobi::compiler::processImportingModules(const std::vector<std::string> &lines, const naobi::module::sptr& module)
{
	auto modulesNames = compiler::collectModules(lines);
	for (const auto& moduleName : modulesNames)
	{
		std::string file = processFileName(moduleName);
		if (file == module->name())
		{
			LOG(compiler.processImportingModules, logger::CRITICAL, "CRITICAL module '", file, "' import itself");
			exitOn({"import", moduleName});
		}
		auto ptr = _root->findModule(file);
		if (ptr != nullptr)
		{
			module->addModule(ptr);
			continue;
		}
		compile(moduleName, module);
	}
}

void naobi::compiler::processModule(const std::vector<std::string> &lines, const naobi::module::sptr& module)
{
	for (const auto& line : lines)
	{
		LOG(compiler.processModule, logger::LOW, "process line '",line,"'");

		auto words = parser::split(line, parser::isAnyOf(" "), {}, {{'"', '"'}}, {{'{','}'}});
		LOG(compiler.processModule, logger::LOW, "words:\n", words);

		if (words.empty()) continue;

		bool isCompiled{};
		for (const auto& rule : _rules)
		{
			isCompiled = rule.checkLineAndRun(words, module);
			if (isCompiled) break;
		}
		if (!isCompiled)
		{
			LOG(compiler.processModule, logger::CRITICAL, "CRITICAL failed to identify line:\n", line);
			std::exit(EXIT_FAILURE);
		}
	}
}

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
	LOG(compiler.collectModules, logger::LOW, "begin collect modules");
	std::vector<std::string> buffer;
	for (const auto& line : lines)
	{
		auto arguments = parser::split(line, parser::isAnyOf(" "));
		if (arguments.size() == 2 && arguments[0] == "import")
		{
			buffer.emplace_back(arguments[1]);
		}
	}
	LOG(compiler.collectModules, logger::IMPORTANT, "collected modules\n", buffer);
	return buffer;
}

static std::string getParamValue(const std::vector<std::string>& line, const std::string& name)
{
	auto it = std::find(line.begin(), line.end(), name);
	if (it == line.cend() || it + 1 == line.cend()) return "";
	else return *(it + 1);
}

naobi::compiler::compiler() :
_rules(
{
	// Workflow logic
	{[](const std::vector<std::string> &line) -> bool {
		return !line.empty() && line[0] == "workflow";
	},[](const std::vector<std::string> &line, const naobi::module::sptr &module)
	{
		std::string name;
		std::string target;
		int invoke = -1;

		name = getParamValue(line, "workflow");
		if (name.empty())
		{
			LOG(compiler.compile, naobi::logger::CRITICAL, "CRITICAL failed to create workflow '", name, "'\n", "Can't find workflow name");
			exitOn(line);
		}
		if (naobi::keywords::check(name))
		{
			LOG(compiler.compile, naobi::logger::CRITICAL, "CRITICAL '", name, "' is keyword!");
			exitOn(line);
		}
		target = getParamValue(line, "target");
		if (target.empty())
		{
			target = "begin";
		}
		if (naobi::keywords::check(target))
		{
			LOG(compiler.compile, naobi::logger::CRITICAL, "CRITICAL '", target, "' is keyword!");
			exitOn(line);
		}
		auto temp = getParamValue(line, "invoke");
		if (temp == "always")
		{
			invoke = -1;
		}
		else if (temp == "once")
		{
			invoke = 1;
		}
		else if (!temp.empty())
		{
			invoke = std::stoi(temp);
		}

		auto tempWorkflow = std::make_shared<naobi::workflow>(name, module);
		tempWorkflow->setInvoke(invoke);

		std::string codeBlock = line.back().substr(1, line.back().size() - 2);
		auto lines = parser::split(codeBlock, parser::isAnyOf(";}"), {}, {{'{','}'}, {'"','"'}});
		naobi::code_generator generator(module);
		auto commands = generator.generate(lines);
		tempWorkflow->setCommands(commands);

		LOG(compiler.compile, naobi::logger::BASIC, "Create workflow with name '", name, "'", " and target '", target,"', invoke times = ", invoke);

		event_manager::addWorkflow(target, tempWorkflow);
	}},
	// Function logic
	{[](const std::vector<std::string> &line) -> bool{
		return !line.empty() && line[0] == "function";
	},[](const std::vector<std::string> &line, const naobi::module::sptr &module){
		std::string name = getParamValue(line, "function");
		if (name.empty())
		{
			LOG(compiler.compile, logger::CRITICAL, "CRITICAL failed to get function name");
			exitOn(line);
		}
		auto function = std::make_shared<naobi::function>(name);
		module->addFunction(function);
	}},
	// Import plug
	{[](const std::vector<std::string> &line) -> bool{
		return !line.empty() && line[0] == "import";
	},
	[]([[maybe_unused]]const std::vector<std::string> &line, [[maybe_unused]]const naobi::module::sptr &module) noexcept
	{
	}},
}
)
{}

void naobi::compiler::exitOn(const std::vector<std::string>& lineToExit)
{
	LOG(compiler.compile, naobi::logger::CRITICAL, lineToExit);
	std::exit(EXIT_FAILURE);
}
