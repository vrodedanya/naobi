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
		static std::vector<std::string> split(const std::string& text, const std::string& splitter, int splitMods = 0);
		static std::string removeExtraSpaces(const std::string& str);
		static std::string removeSym(const std::string& str, char symbolToRemove);
	};
}

#endif //NAOBI_PARSER_HPP
