#include "naobi/utils/parser.hpp"

std::vector<std::string> naobi::parser::split(const std::string& text, const std::string& splitter, bool saveSplitter)
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

std::string naobi::parser::removeExtraSpaces(const std::string &str)
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

std::string naobi::parser::removeSym(const std::string &str, char symbolToRemove)
{
	std::string buffer;
	for (const auto& ch : str)
	{
		if (ch != symbolToRemove) buffer += ch;
	}
	return buffer;
}