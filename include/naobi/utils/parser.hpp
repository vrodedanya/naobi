#ifndef NAOBI_PARSER_HPP
#define NAOBI_PARSER_HPP

#include <vector>
#include <string>
#include <functional>
#include <map>
#include <cassert>

namespace naobi::parser
{
	enum split_mods
	{
		SPLIT_AFTER = 1,
		SPLIT_BLOCK = 2,
	};

	std::function<int(char)> isAnyOf(const std::string& str);

	std::function<int(char)> isEnds(const std::string& str);

	std::vector<std::string> split(const std::string& text, const std::function<int(char)>& isSplitter,
										  const std::function<bool(char)>& isSingle = {}, const std::map<char, char>& blocks = {},
										  const std::map<char, char>& separatedBlocks = {});

	std::string join(const std::vector<std::string>& strings, const std::string& delimiter) noexcept;
	std::string join(std::vector<std::string>::iterator& begin, std::vector<std::string>::iterator& end, const std::string& delimiter) noexcept;


	std::string removeExtraSpaces(const std::string& str) noexcept;
	std::string removeFirstSym(const std::string& str, char sym) noexcept;
	std::string removeSym(const std::string& str, char symbolToRemove) noexcept;
	std::string replaceSym(const std::string& str, char symbolToReplace, char replacementCharacter) noexcept;
	void removeComments(std::string& str) noexcept;
	std::string placeAfter(const std::string& str, char symbolAfter, const std::string& symbolToPlace) noexcept;

	std::vector<std::string> removeEmpty(const std::vector<std::string>& vec) noexcept;
	std::string dirName(const std::string& path) noexcept;
	std::string fileName(const std::string& path) noexcept;
}

#endif //NAOBI_PARSER_HPP
