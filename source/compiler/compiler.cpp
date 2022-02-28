#include <naobi/compiler/compiler.hpp>

#include <filesystem>
#include <algorithm>

#include <naobi/utils/parser.hpp>
#include <naobi/utils/logger.hpp>
#include <naobi/utils/keywords.hpp>

void naobi::compiler::compile(const std::string &fileName)
{
	LOG(compiler.compile, logger::LOW, "begin compiling program");

	std::filesystem::path path(naobi::parser::dirName(fileName));
	if (!std::filesystem::is_directory(path))
	{
		LOG(compiler.compile, logger::CRITICAL, "CRITICAL directory ", naobi::parser::dirName(fileName), " doesn't exist");
		exit(EXIT_FAILURE);
	}
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
	_compilingFileContent = fileTextOpt.value();

	auto module = std::make_shared<naobi::module>(file);
	if (parent == nullptr)
	{
		_composition.rootModule = module;
	}
	else
	{
		parent->addModule(module);
	}

	auto temp = parser::replaceSym(parser::removeExtraSpaces(fileTextOpt.value()), '\n', ' ');
	auto lines = parser::split(temp, {";", "}"}, {}, naobi::parser::SPLIT_AFTER);
	LOG(compiler.compile, logger::LOW, "lines:\n", lines);

	processModules(lines, module);

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

void naobi::compiler::processModules(const std::vector<std::string> &lines, const naobi::module::sptr& module)
{
	auto modulesNames = compiler::collectModules(lines);
	for (const auto& moduleName : modulesNames)
	{
		std::string file = processFileName(moduleName);
		if (file == module->name())
		{
			LOG(compiler.processModules, logger::CRITICAL, "CRITICAL module '", file, "' import itself");
			exitOn({"import", moduleName});
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

void naobi::compiler::processModule(const std::vector<std::string> &lines, const naobi::module::sptr& module)
{
	for (const auto& line : lines)
	{
		LOG(compiler.processModule, logger::LOW, "process line '",line,"'");

		auto words = naobi::parser::split(line, {" "}, {});
		LOG(compiler.processModule, logger::LOW, "words:\n", words);

		for (const auto& rule : _rules)
		{
			rule.checkLineAndRun(words, module);
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
	LOG(compiler.collectModules, logger::LOW, "begin collectModules");
	std::vector<std::string> buffer;
	for (const auto& line : lines)
	{
		auto arguments = parser::split(line, {" "}, {});
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
_rules({
			   {[](const std::vector<std::string> &line) -> bool
				{
					return !line.empty() && line[0] == "workflow";
				},
				[this](const std::vector<std::string> &line, const naobi::module::sptr &module)
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
					   LOG(compiler.compile, naobi::logger::CRITICAL, "CRITICAL failed to create workflow '", name, "'\n", "Can't find target");
					   exitOn(line);
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

				   LOG(compiler.compile, naobi::logger::BASIC, "Create workflow with name '", name, "'", " and target '", target,"', invoke times = ", invoke);

				   auto tempWorkflow = std::make_shared<naobi::workflow>(name, module);
				   tempWorkflow->setInvoke(invoke);
				   this->_composition.workflows.push_back(tempWorkflow);
				},
			   },
			   {
			   [](const std::vector<std::string> &line) -> bool
			   {
				   return !line.empty() && line[0] == "function";
			   },
			   [this](const std::vector<std::string> &line, const naobi::module::sptr &module)
			   {
				   std::string name = getParamValue(line, "function");
				   if (name.empty())
				   {
					   LOG(compiler.compile, logger::CRITICAL, "CRITICAL failed to get function name");
					   exitOn(line);
				   }
				   auto function = std::make_shared<naobi::function>(name);
				   module->addFunction(function);
			   }
			   },
	   })
{}

void naobi::compiler::exitOn(const std::vector<std::string>& lineToExit)
{
	auto lines = naobi::parser::split(_compilingFileContent, {"\n"}, {});
	for (std::size_t i = 0 ; i < lines.size() ; i++)
	{
		auto words = naobi::parser::split(naobi::parser::removeSym(lines[i], ';'), {" "}, {});
		std::size_t wordsCount = words.size() > lineToExit.size() ? lineToExit.size() : words.size();
		std::size_t counter = 0;
		for (const auto& word : words)
		{
			if (std::find(lineToExit.begin(), lineToExit.end(), word) != lineToExit.end())
			{
				counter++;
			}
		}
		if (static_cast<double>(counter) > static_cast<double>(wordsCount) * 0.8)
		{
			LOG(compiler.compile, naobi::logger::CRITICAL, i + 1, " | ", lines[i]);
			break;
		}
	}
	std::exit(EXIT_FAILURE);
}
