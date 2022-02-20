#ifndef NAOBI_PARSER_HPP
#define NAOBI_PARSER_HPP

#include <vector>
#include <string>

namespace naobi
{
	class parser
	{
	public:
		enum split_mods
		{
			SAVE_SPLITTER = 1,
			SAVE_BLOCKS = 2,
		};
	public:
		parser() = delete;
		static std::vector<std::string> split(const std::string& text, const std::string& splitter, int splitMods = 0) noexcept;
		static std::string removeExtraSpaces(const std::string& str) noexcept;
		static std::string removeSym(const std::string& str, char symbolToRemove) noexcept;
		static std::string replaceSym(const std::string& str, char symbolToReplace, char replacementCharacter) noexcept;
		static std::string placeAfter(const std::string& str, char symbolAfter, const std::string& symbolToPlace) noexcept;

		static std::string dirName(const std::string& path) noexcept;
		static std::string fileName(const std::string& path) noexcept;
	};
}

#endif //NAOBI_PARSER_HPP
