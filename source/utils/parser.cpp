#include "naobi/utils/parser.hpp"

#include <algorithm>

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

std::string naobi::parser::join(const std::vector<std::string> &strings, const std::string &delimiter) noexcept
{
	std::string temp;

	for(auto it = strings.cbegin() ; it != strings.cend() ; it++)
	{
		temp += *it;
		if ((it + 1) != strings.cend()) temp += delimiter;
	}

	return temp;
}

std::vector<std::string> naobi::parser::split(const std::string &text, const std::vector<std::string> &splitters,
											  const std::vector<std::string> &single, int mods)
{
	std::vector<std::string> buffer;
	std::string tempString;
	bool isQuote = false;
	int brackets{};
	for (auto sym : text)
	{
		if (brackets == 0 && sym == '\"')
		{
			isQuote = !isQuote;
		}
		if (sym == '{')
		{
			brackets++;
		}
		else if (sym == '}')
		{
			brackets--;
		}
		if (brackets == 0 && !isQuote && std::find(single.cbegin(), single.cend(), std::string(1, sym)) != single.cend())
		{
			if (!tempString.empty())
			{
				buffer.push_back(tempString);
				tempString.clear();
			}
			buffer.emplace_back(1, sym);
		}
		else if (brackets == 0 && !isQuote && std::find(splitters.cbegin(), splitters.cend(), std::string(1, sym)) != splitters.cend())
		{
			if (!tempString.empty())
			{
				if (mods & SPLIT_AFTER)
				{
					tempString += sym;
					buffer.push_back(tempString);
					tempString.clear();
					continue;
				}
				else
				{
					buffer.push_back(tempString);
					tempString.clear();
				}
			}
		}
		else
		{
			tempString += sym;
		}
	}
	if (!tempString.empty()) buffer.push_back(tempString);
	return buffer;
}
