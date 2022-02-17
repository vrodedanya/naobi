#include <naobi/compiler/compiler.hpp>

#include <fstream>
#include <sstream>

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
	auto temp = removeSym(removeExtraSpaces(fileText), '\n');
	auto lines = split(temp, ";", false);
	for (const auto& line : lines)
	{
		auto arguments = split(line, " ");
		if (arguments.size() == 2 && arguments[0] == "import")
		{
			buffer.emplace_back(arguments[1]);
		}
	}
	return buffer;
}

std::vector<std::string> naobi::compiler::split(const std::string& text, const std::string& splitter, bool saveSplitter)
{
	std::size_t currentPos{}, splitterPos{};
	std::vector<std::string> buffer;

	while (true)
	{
		splitterPos = text.find(splitter, currentPos);
		if (splitterPos == std::string::npos)
		{
			if (text.size() > currentPos) buffer.emplace_back(text.substr(currentPos));
			break;
		}
		if (currentPos == splitterPos)
		{
			currentPos = splitterPos + 1;
			continue;
		}
		if (saveSplitter)
		{
			splitterPos++;
			buffer.emplace_back(text.substr(currentPos, splitterPos - currentPos));
		}
		else
		{
			buffer.emplace_back(text.substr(currentPos, splitterPos - currentPos));
			splitterPos++;
		}
		currentPos = splitterPos;
	}
	return buffer;
}

std::string naobi::compiler::removeExtraSpaces(const std::string &str)
{
	std::string tempString;
	bool isPreviousSpace{false};
	for (const auto& ch : str)
	{
		if (std::isspace(ch))
		{
			if (!isPreviousSpace)
			{
				tempString += ch;
			}
			isPreviousSpace = true;
		}
		else
		{
			tempString += ch;
			isPreviousSpace = false;
		}
	}
	return tempString;
}

std::string naobi::compiler::removeSym(const std::string &str, char symbolToRemove)
{
	std::string buffer;
	for (const auto& ch : str)
	{
		if (ch != symbolToRemove) buffer += ch;
	}
	return buffer;
}
