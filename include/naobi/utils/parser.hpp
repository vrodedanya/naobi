#ifndef NAOBI_PARSER_HPP
#define NAOBI_PARSER_HPP

#include <vector>
#include <string>
#include <functional>
#include <map>
#include <cassert>


namespace naobi::parser
{
	std::function<int(char)> isAnyOf(const std::string& str);

	std::function<int(char)> isEnds(const std::string& str);

	std::vector<std::string> split(
		const std::string& text, const std::function<int(char)>& isSplitter,
		const std::function<bool(char)>& isSingle = {},
		const std::map<char, char>& blocks = {},
		const std::map<char, char>& separatedBlocks = {});

	std::string join(const std::vector<std::string>& strings, const std::string& delimiter) noexcept;

	template <typename ITERATOR_BEGIN, typename ITERATOR_END>
	std::string join(const ITERATOR_BEGIN& begin, const ITERATOR_END& end, const std::string& delimiter) noexcept;


	std::string removeExtraSpaces(const std::string& str) noexcept;

	std::string removeFirstSym(const std::string& str, char sym) noexcept;

	std::string removeSym(const std::string& str, char symbolToRemove) noexcept;

	std::string replaceSym(const std::string& str, char symbolToReplace, char replacementCharacter) noexcept;

	void removeComments(std::string& str) noexcept;

	std::string placeAfter(const std::string& str, char symbolAfter, const std::string& symbolToPlace) noexcept;

	std::vector<std::string> removeEmpty(const std::vector<std::string>& vec) noexcept;

	std::string dirName(const std::string& path) noexcept;

	std::string fileName(const std::string& path) noexcept;

	template <typename ITERATOR, typename SYM>
	auto findCloseBracket(ITERATOR begin, ITERATOR end, SYM openBracket, SYM closeBracket);
}

template <typename ITERATOR_BEGIN, typename ITERATOR_END>
std::string naobi::parser::join(
	const ITERATOR_BEGIN& begin, const ITERATOR_END& end,
	const std::string& delimiter) noexcept
{
	std::string temp;

	for (auto it = begin ; it != end ; it++)
	{
		temp += *it;
		if ((it + 1) != end) temp += delimiter;
	}

	return temp;
}

template <typename ITERATOR, typename SYM>
auto naobi::parser::findCloseBracket(ITERATOR begin, ITERATOR end, SYM openBracket, SYM closeBracket)
{
	if (openBracket == closeBracket)
	{
		assert(openBracket != closeBracket);
	}
	int count{};
	for (auto it = begin ; it != end ; it++)
	{
		if (*it == openBracket)
		{
			count++;
		}
		else if (*it == closeBracket)
		{
			count--;
			if (count == 0)
			{
				return it;
			}
		}
	}
	return end;
}

#endif //NAOBI_PARSER_HPP
