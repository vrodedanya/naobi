#include <naobi/utils/parser.hpp>

#include <algorithm>


struct split_data
{
	std::vector<std::string> resultVector;
	std::string stringBuffer;
	int blockIndex = 0;
	char currentBlock{};
	char closeBracket{};
	bool wasSeparatedBlock{};
};

static void processGuardedBlock(
	split_data& data, const char& sym, const std::function<int(char)>& isSplitter)
{
	if (data.closeBracket == sym)
	{
		data.blockIndex--;
	}
	else if (data.currentBlock == sym)
	{
		data.blockIndex++;
	}

	data.stringBuffer += sym;
	if (data.blockIndex == 0)
	{
		if (data.wasSeparatedBlock || isSplitter(sym))
		{
			data.resultVector.emplace_back(data.stringBuffer);
			data.stringBuffer.clear();
		}
		data.wasSeparatedBlock = false;
		data.closeBracket = 0;
	}
}

std::vector<std::string> naobi::parser::split(
	const std::string& text, const std::function<int(char)>& isSplitter,
	const std::function<bool(char)>& isSingle,
	const std::map<char, char>& blocks,
	const std::map<char, char>& separatedBlocks)
{
	assert(isSplitter);
	split_data data;
	auto pushAndClearBuffer = [&]()
	{
		if (!data.stringBuffer.empty())
		{
			data.resultVector.emplace_back(data.stringBuffer);
			data.stringBuffer.clear();
		}
	};

	for (const auto& sym : text)
	{
		if (data.blockIndex == 0)
		{
			if (int mod; (mod = isSplitter(sym)) != 0)
			{
				if (mod == 2)
				{
					data.stringBuffer += sym;
				}
				pushAndClearBuffer();
			}
			else if (isSingle && isSingle(sym))
			{
				pushAndClearBuffer();
				data.stringBuffer += sym;
				data.resultVector.emplace_back(data.stringBuffer);
				data.stringBuffer.clear();
			}
			else if (blocks.find(sym) != blocks.end())
			{
				data.currentBlock = sym;
				data.blockIndex++;
				data.stringBuffer += sym;
				data.wasSeparatedBlock = false;
				data.closeBracket = blocks.at(sym);
			}
			else if (separatedBlocks.find(sym) != separatedBlocks.end())
			{
				data.currentBlock = sym;
				data.blockIndex++;
				pushAndClearBuffer();
				data.stringBuffer += sym;
				data.wasSeparatedBlock = true;
				data.closeBracket = separatedBlocks.at(sym);
			}
			else
			{
				data.stringBuffer += sym;
			}
		}
		else
		{
			processGuardedBlock(data, sym, isSplitter);
		}
	}
	pushAndClearBuffer();
	std::for_each(
		data.resultVector.begin(), data.resultVector.end(), [](auto& str)
		{ str = parser::removeFirstSym(str, ' '); });
	return data.resultVector;
}

std::string naobi::parser::removeExtraSpaces(const std::string& str) noexcept
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

std::string naobi::parser::removeSym(const std::string& str, char symbolToRemove) noexcept
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

std::string
naobi::parser::placeAfter(const std::string& str, char symbolAfter, const std::string& symbolToPlace) noexcept
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

std::string naobi::parser::replaceSym(const std::string& str, char symbolToReplace, char replacementCharacter) noexcept
{
	std::string buffer;
	for (const auto& ch : str)
	{
		if (ch != symbolToReplace) buffer += ch;
		else buffer += replacementCharacter;
	}
	return buffer;
}

std::string naobi::parser::dirName(const std::string& path) noexcept
{
	std::size_t entry = path.find_last_of("/\\");
	return path.substr(0, entry);
}

std::string naobi::parser::fileName(const std::string& path) noexcept
{
	std::size_t entry = path.find_last_of("/\\");
	return path.substr(entry + 1);
}

std::string naobi::parser::join(const std::vector<std::string>& strings, const std::string& delimiter) noexcept
{
	std::string temp;

	for (auto it = strings.cbegin() ; it != strings.cend() ; it++)
	{
		temp += *it;
		if ((it + 1) != strings.cend()) temp += delimiter;
	}

	return temp;
}


std::string naobi::parser::removeFirstSym(const std::string& str, char sym) noexcept
{
	return !str.empty() && str[0] == sym ? str.substr(1) : str;
}

std::vector<std::string> naobi::parser::removeEmpty(const std::vector<std::string>& vec) noexcept
{
	std::vector<std::string> buffer;
	buffer.reserve(vec.size());
	for (const auto& string : vec)
	{
		if (!string.empty() && !(string[0] == ' ' && string.size() == 1)) buffer.push_back(string);
	}
	return buffer;
}


void naobi::parser::removeComments(std::string& str) noexcept
{
	std::size_t commentPos;
	while ((commentPos = str.find("//")) != std::string::npos)
	{
		str.erase(commentPos, str.find('\n', commentPos) - commentPos);
	}
}

std::function<int(char)> naobi::parser::isAnyOf(const std::string& str)
{
	return [symbols = str](auto ch) noexcept -> int
	{ return symbols.find(ch) != std::string::npos; };
}

std::function<int(char)> naobi::parser::isEnds(const std::string& str)
{
	return [symbols = str](auto ch) noexcept -> int
	{
		auto temp = symbols.find(ch);
		if (temp != std::string::npos) return 2;
		else return 0;
	};
}

