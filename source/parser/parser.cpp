#include "naobi/utils/parser.hpp"

#include <algorithm>

std::vector<std::string> naobi::parser::split(const std::string& text, const std::string& splitter, int splitMods)
{
	if (splitter.empty()) return {};

	std::size_t currentPos{}, splitterPos;
	std::vector<std::string> buffer;
	std::vector<std::pair<std::size_t, std::size_t>> blocks;


	if (splitMods & split_mods::SAVE_BLOCKS)
	{
		int pos{0}, prev{-1};
		while (static_cast<std::size_t>(pos = static_cast<int>(text.find('\"', pos))) != std::string::npos)
		{
			if (prev != -1)
			{
				blocks.emplace_back(prev, pos);
				prev = -1;
				pos++;
			}
			else prev = pos++;
		}
	}

	while (true)
	{
		splitterPos = text.find(splitter, currentPos);

		if (splitMods & split_mods::SAVE_BLOCKS)
		{
			while(std::find_if(blocks.cbegin(), blocks.cend(),
								   [splitterPos](std::pair<std::size_t, std::size_t> pair)
								   {
									   return pair.first < splitterPos && pair.second > splitterPos;
								   }) != blocks.cend())
			{
				splitterPos = text.find(splitter, splitterPos + 1);
			}
		}
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
		if (splitMods & split_mods::SAVE_SPLITTER)
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