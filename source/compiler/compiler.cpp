#include <naobi/compiler/compiler.hpp>

#include <fstream>
#include <sstream>

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

