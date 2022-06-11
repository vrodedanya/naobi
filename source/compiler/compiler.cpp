#include <naobi/compiler/compiler.hpp>

#include <filesystem>
#include <algorithm>
#include <memory>

#include <naobi/utils/parser.hpp>
#include <naobi/utils/logger.hpp>
#include <naobi/utils/keywords.hpp>
#include <naobi/compiler/code_generator.hpp>
#include <naobi/standard/standard_module.hpp>
#include <naobi/interpreter/event_manager.hpp>


void naobi::compiler::compile(const std::string& fileName)
{
	NLOG(compiler.compile, logger::LOW, "begin compiling program");

	if (fileName.find('/') != std::string::npos)
	{
		std::filesystem::path path(naobi::parser::dirName(fileName));
		if (!std::filesystem::is_directory(path))
		{
			NCRITICAL(compiler.compile, errors::FAILED_TO_OPEN_FILE, "CRITICAL directory ",
					  naobi::parser::dirName(fileName), " doesn't exist");
		}
		std::filesystem::current_path(path);
		NLOG(compiler.compile, logger::IMPORTANT, "set current directory to ", naobi::parser::dirName(fileName));
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
	NLOG(compiler.compile, logger::LOW, "after removing comments:\n", fileContent);

	auto temp = parser::replaceSym(parser::removeExtraSpaces(fileContent), '\n', ' ');
	auto lines = parser::split(
		temp, parser::isEnds(";}"), {}, {{'{', '}'},
										 {'"', '"'}});
	NLOG(compiler.compile, logger::LOW, "lines:\n", lines);

	processImportingModules(lines, module);

	processModule(lines, module);

	NLOG(compiler.compile, logger::SUCCESS, "compiled '", moduleName, "'");
}

void naobi::compiler::compile(const std::string& fileName, const naobi::module::sptr& parent)
{
	if (fileName == "standard;")
	{
		if (parent == nullptr)
		{
			NCRITICAL(compiler.compile, errors::NOT_SPECIFIED, "CRITICAL standard module without parent");
		}
		parent->addModule(std::shared_ptr<naobi::module>(new naobi::standard()));
		return;
	}
	NLOG(compiler.compile, logger::BASIC, "process file name");
	std::string file = processFileName(fileName);

	NLOG(compiler.compile, logger::LOW, "begin compiling file ", file);

	auto fileTextOpt = compiler::loadFile(file);
	if (!fileTextOpt.has_value())
	{
		NCRITICAL(compiler.compile, errors::FAILED_TO_OPEN_FILE, "CRITICAL failed to open file '", file, "'");
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
	NLOG(compiler.compile, logger::LOW, "after removing comments:\n", fileContent);

	auto temp = parser::replaceSym(parser::removeExtraSpaces(fileContent), '\n', ' ');
	auto lines = parser::split(
		temp, parser::isEnds(";}"), {}, {{'{', '}'},
										 {'"', '"'}});
	NLOG(compiler.compile, logger::LOW, "lines:\n", lines);

	processImportingModules(lines, module);

	processModule(lines, module);

	NLOG(compiler.compile, logger::SUCCESS, "compiled '", file, "'");
}

std::string naobi::compiler::processFileName(const std::string& fileName)
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

void naobi::compiler::processImportingModules(const std::vector<std::string>& lines, const naobi::module::sptr& module)
{
	auto modulesNames = compiler::collectModules(lines);
	for (const auto& moduleName : modulesNames)
	{
		std::string file = processFileName(moduleName);
		if (file == module->name())
		{
			NCRITICAL(compiler.processImportingModules, errors::IMPORT_ITSELF, "CRITICAL module '", file,
					  "' import itself");
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

void naobi::compiler::processModule(const std::vector<std::string>& lines, const naobi::module::sptr& module)
{
	for (const auto& line : lines)
	{
		NLOG(compiler.processModule, logger::LOW, "process line '", line, "'");

		auto words = parser::split(
			line, parser::isEnds(" >"), {}, {{'"', '"'}}, {{'{', '}'},
														   {'(', ')'},
														   {'<', '>'}});
		std::for_each(
			words.begin(), words.end(), [](std::string& elem)
			{
				if (!elem.empty() && elem.back() == ' ')
				{
					elem = elem.substr(0, elem.size() - 1);
				}
			});
		words.erase(
			std::remove_if(
				words.begin(), words.end(), [](const std::string& str)
				{
					return str.empty() || (str.size() == 1 && str[0] == ' ');
				}), words.end());
		NLOG(compiler.processModule, logger::LOW, "words:\n", words);

		if (words.empty()) continue;

		bool isCompiled{};
		for (const auto& rule : _rules)
		{
			isCompiled = rule.checkLineAndRun(words, module);
			if (isCompiled) break;
		}
		if (!isCompiled)
		{
			NCRITICAL(compiler.processModule, errors::UNKNOWN_LINE, "CRITICAL failed to identify line:\n", line);
		}
	}
}

std::optional<std::string> naobi::compiler::loadFile(const std::string& fileName)
{
	NLOG(compiler.loadFile, logger::LOW, "begin loading file ", fileName);
	std::ifstream inputStream(fileName);
	if (!inputStream.is_open()) return {};
	std::stringstream buffer;
	buffer << inputStream.rdbuf();
	std::string temp = buffer.str();
	NLOG(compiler.loadFile, logger::BASIC, "file content:\n", naobi::parser::placeAfter('\n' + temp, '\n', " | "));
	return temp;
}

std::vector<std::string> naobi::compiler::collectModules(const std::vector<std::string>& lines)
{
	NLOG(compiler.collectModules, logger::LOW, "begin collect modules");
	std::vector<std::string> buffer;
	for (const auto& line : lines)
	{
		auto arguments = parser::split(line, parser::isAnyOf(" "));
		if (arguments.size() == 2 && arguments[0] == "import")
		{
			buffer.emplace_back(arguments[1]);
		}
	}
	NLOG(compiler.collectModules, logger::IMPORTANT, "collected modules\n", buffer);
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
			{[](const std::vector<std::string>& line) -> bool
			 {
				 return !line.empty() && line[0] == "workflow";
			 }, [this](const std::vector<std::string>& line, const naobi::module::sptr& module)
			 {
				 std::string name;
				 std::string target;
				 int invoke = -1;

				 name = getParamValue(line, "workflow");
				 if (name.empty())
				 {
					 NCRITICAL(compiler.compile, errors::NOT_SPECIFIED,
							   "CRITICAL failed to create workflow '", name, "'\n",
							   "Can't find workflow name");
				 }
				 if (naobi::keywords::check(name))
				 {
					 NCRITICAL(compiler.compile, errors::KEYWORD_AS_NAME, "CRITICAL '", name,
							   "' is keyword!");
				 }
				 target = getParamValue(line, "target");
				 if (target.empty())
				 {
					 target = "begin";
				 }
				 if (naobi::keywords::check(target))
				 {
					 NCRITICAL(compiler.compile, errors::KEYWORD_AS_NAME, "CRITICAL '", target,
							   "' is keyword!");
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
				 else
				 {
					 invoke = 1;
				 }
				 auto e = module->findEvent(target);
				 if (!e.has_value())
				 {
					 NCRITICAL(compiler, errors::DOESNT_EXIST, "Target '", target, "' doesn't exist");
				 }
				 auto tempWorkflow = std::make_shared<naobi::workflow>(name, e.value(), module);
				 tempWorkflow->setInvoke(invoke);

				 std::string codeBlock = line.back().substr(1, line.back().size() - 2);
				 auto lines = parser::split(
					 codeBlock, parser::isAnyOf(";}"), {}, {{'{', '}'},
															{'"', '"'}});
				 naobi::code_generator generator(module);
				 for (const auto& arg : e->getArguments())
				 {
					 std::string varName = e->getName() + "." + std::get<0>(arg);
					 auto var = std::make_shared<naobi::variable>(varName, std::get<1>(arg));
					 generator.addVariable(varName, var);
				 }
				 auto commands = generator.generate(lines);
				 tempWorkflow->setCommands(commands);

				 NLOG(compiler.compile, naobi::logger::BASIC, "Create workflow with name '", name, "'",
					  " and target '", target, "', invoke times = ", invoke);
				 this->_workflows.push_back(tempWorkflow);
			 }},
			// Template function logic
			{[](const std::vector<std::string>& line) -> bool
			 {
				 return !line.empty() && line[0] == "function" && line.size() >= 4 &&
						line[1].front() == '<';
			 },
			 [](const std::vector<std::string>& line, const naobi::module::sptr& module)
			 {
				 std::string name = line[2];
				 auto templateFunction = std::make_shared<naobi::template_function>(name);

				 std::vector<std::string> types = parser::split(
					 parser::removeSym(line[1].substr(1, line[1].size() - 2), ' '),
					 parser::isAnyOf(","));

				 auto arguments = parser::split(
					 line[3].substr(1, line[3].size() - 2),
					 parser::isAnyOf(","));
				 NLOG(compiler.compile, logger::IMPORTANT, "Arguments of function: ", arguments);
				 for (const auto& argument : arguments)
				 {
					 auto words = parser::split(argument, parser::isAnyOf(" "));
					 if (words.size() != 2)
					 {
						 NCRITICAL(compiler.compile, errors::INVALID_ARGUMENT,
								   "CRITICAL wrong argument: ", words);
					 }
					 auto type = words[0];
					 auto argName = words[1];
					 templateFunction->addArgument(argName, type);
				 }


				 auto retIterator = std::find(line.begin(), line.end(), "->");
				 if (retIterator != line.end() && (retIterator + 1) != line.end())
				 {
					 auto returnType = (*(retIterator + 1));
					 templateFunction->setReturnType(returnType);
				 }
				 else
				 {
					 templateFunction->setReturnType("undefined");
				 }
				 templateFunction->setCode(line.back().substr(1, line.back().size() - 2));

				 if (module->addTemplateFunction(templateFunction))
				 {
					 NLOG(compiler.compile, logger::IMPORTANT, "Added template function with name ",
						  name);
				 }
				 else
				 {
					 NCRITICAL(compiler.compile, errors::ALREADY_EXIST,
							   "CRITICAL template function with name ",
							   name, " is already exist");
				 }
			 }
			},
			// Function logic
			{[](const std::vector<std::string>& line) -> bool
			 {
				 return !line.empty() && line[0] == "function" && line.size() >= 4;
			 }, [](const std::vector<std::string>& line, const naobi::module::sptr& module)
			 {
				 std::string name = getParamValue(line, "function");
				 if (name.empty())
				 {
					 NCRITICAL(compiler.compile, errors::NOT_SPECIFIED,
							   "CRITICAL failed to get function name");
				 }
				 auto function = std::make_shared<naobi::function>(name);
				 code_generator generator(module);

				 auto arguments = parser::split(
					 line[2].substr(1, line[2].size() - 2),
					 parser::isAnyOf(","));
				 NLOG(compiler.compile, logger::IMPORTANT, "Arguments of function: ", arguments);
				 for (const auto& argument : arguments)
				 {
					 auto words = parser::split(argument, parser::isAnyOf(" "));
					 if (words.size() != 2)
					 {
						 NCRITICAL(compiler.compile, errors::INVALID_ARGUMENT,
								   "CRITICAL wrong argument: ", words);
					 }
					 auto type = utils::type::type(utils::type::fromStringToName(words[0]));
					 auto argName = words[1];
					 auto variable = std::make_shared<naobi::variable>(argName, type);
					 generator.addVariable(argName, variable);
					 function->addArgument(argName, type);
				 }

				 auto retIterator = std::find(line.begin(), line.end(), "->");
				 if (retIterator != line.end() && (retIterator + 1) != line.end())
				 {
					 auto returnType = utils::type::type(utils::type::fromStringToName(*(retIterator + 1)));
					 function->setReturnType(returnType);
				 }
				 else
				 {
					 function->setReturnType(utils::type::type(utils::type::names::UNDEFINED));
				 }

				 std::string codeBlock = line.back().substr(1, line.back().size() - 2);
				 auto lines = parser::split(
					 codeBlock, parser::isAnyOf(";}"), {}, {{'{', '}'},
															{'"', '"'}});

				 auto commands = generator.generate(lines);
				 function->setCommands(commands);

				 if (module->addFunction(function))
				 {
					 NLOG(compiler.compile, logger::IMPORTANT, "Added function with name ", name);
				 }
				 else
				 {
					 NCRITICAL(compiler.compile, errors::ALREADY_EXIST, "CRITICAL function with name ",
							   name, " and this arguments ", line[2], " is already exist");
				 }
			 }},
			{[](const std::vector<std::string>& line) -> bool
			 {
				 return !line.empty() && line[0] == "exception" && line.size() == 2;
			 },
			 [](
				 [[maybe_unused]]const std::vector<std::string>& line,
				 [[maybe_unused]]const naobi::module::sptr& module) noexcept
			 {
				module->addException(naobi::exception(line[1].substr(0, line[1].size() - 1), ""));
			 }},
			{[](const std::vector<std::string>& line) -> bool
			 {
				 return !line.empty() && line[0] == "event" && line.size() == 3;
			 },
			 [](
				 [[maybe_unused]]const std::vector<std::string>& line,
				 [[maybe_unused]]const naobi::module::sptr& module) noexcept
			 {
				NLOG(compiler, logger::IMPORTANT, "Event: ", line);
				naobi::event event;
				event.setName(line[1]);
				auto args = parser::removeFirstSym(line[2].substr(1, line[2].size() - 2), ' ');
				auto arguments = parser::removeEmpty(parser::split(args, parser::isAnyOf(";")));
				NLOG(compiler, logger::LOW, "arguments: ", arguments);
				for (const auto& argument : arguments)
				{
					auto pair = parser::split(argument, parser::isAnyOf(" "));
					if (pair.size() != 2)
					{
						NCRITICAL(compiler, errors::INVALID_ARGUMENT, "CRITICAL wrong event argument: ", pair);
					}
					if (!event.addArgument(pair[1], utils::type::type(utils::type::fromStringToName(pair[0])), nullptr))
					{
						NCRITICAL(compiler, errors::ALREADY_EXIST, "CRITICAL argument '",pair, "'", "is already exist");

					}
				}

				module->addEvent(event);
			 }},
			// Import plug
			{[](const std::vector<std::string>& line) -> bool
			 {
				 return !line.empty() && line[0] == "import";
			 },
			 [](
				 [[maybe_unused]]const std::vector<std::string>& line,
				 [[maybe_unused]]const naobi::module::sptr& module) noexcept
			 {
			 }},
		}
		  )
{}
