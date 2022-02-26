#include "naobi/utils/parser.hpp"

#include <algorithm>

std::vector<std::string> naobi::parser::split(const std::string& text, const std::string& splitter, int splitMods) noexcept
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

std::string naobi::parser::removeExtraSpaces(const std::string &str) noexcept
{
	std::string tempString;
	bool isPreviousSpace{false};
	bool isQuote{false};
	for (const auto& ch : str)
	{
		if (std::isspace(ch))
		{
			if (!isPreviousSpace || isQuote)
			{
				tempString += ch;
			}
			isPreviousSpace = true;
		}
		else if (ch == '\"')
		{
			isQuote = !isQuote;
			tempString += ch;
		}
		else
		{
			tempString += ch;
			isPreviousSpace = false;
		}
	}
	return tempString;
}

std::string naobi::parser::removeSym(const std::string &str, char symbolToRemove) noexcept
{
	std::string buffer;
	bool isQuote{false};

	for (const auto& ch : str)
	{
		if (ch == '\"')
		{
			isQuote = !isQuote;
			buffer += ch;
		}
		else if (ch != symbolToRemove || isQuote) buffer += ch;
	}
	return buffer;
}

std::string naobi::parser::placeAfter(const std::string &str, char symbolAfter, const std::string& symbolToPlace) noexcept
{
	std::string buffer;
	bool isQuote{false};

	for (const auto& ch : str)
	{
		if (ch == '\"') isQuote = !isQuote;
		buffer += ch;
		if (symbolAfter == ch && !isQuote) buffer += symbolToPlace;
	}
	return buffer;
}

std::string naobi::parser::replaceSym(const std::string &str, char symbolToReplace, char replacementCharacter) noexcept
{
	std::string buffer;
	for (const auto& ch : str)
	{
		if (ch != symbolToReplace) buffer += ch;
		else buffer += replacementCharacter;
	}
	return buffer;
}

std::string naobi::parser::dirName(const std::string &path) noexcept
{
	std::size_t entry = path.find_last_of("/\\");
	return path.substr(0, entry);
}

std::string naobi::parser::fileName(const std::string &path) noexcept
{
	std::size_t entry = path.find_last_of("/\\");
	return path.substr(entry + 1);
}