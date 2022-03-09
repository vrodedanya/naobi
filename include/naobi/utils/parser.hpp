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
			SPLIT_AFTER = 1,
		};
	public:
		parser() = delete;

		/**
		 * @param text - text to parse
		 * @param splitters - splitters to remove
		 * @param single - single symbols
		 * @return
		 */
		static std::vector<std::string> split(const std::string& text, const std::vector<std::string>& splitters,
											  const std::vector<std::string>& single, int mods = 0);
		static std::string join(const std::vector<std::string>& strings, const std::string& delimiter) noexcept;
		static std::string join(std::vector<std::string>::iterator& begin, std::vector<std::string>::iterator& end, const std::string& delimiter) noexcept;


		static std::string removeExtraSpaces(const std::string& str) noexcept;
		static std::string removeFirstSym(const std::string& str, char sym) noexcept;
		static std::string removeSym(const std::string& str, char symbolToRemove) noexcept;
		static std::string replaceSym(const std::string& str, char symbolToReplace, char replacementCharacter) noexcept;
		static std::string placeAfter(const std::string& str, char symbolAfter, const std::string& symbolToPlace) noexcept;

		static std::vector<std::string> removeEmpty(const std::vector<std::string>& vec) noexcept;

		static std::string dirName(const std::string& path) noexcept;
		static std::string fileName(const std::string& path) noexcept;
	};
}

#endif //NAOBI_PARSER_HPP
