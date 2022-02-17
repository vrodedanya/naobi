#ifndef NAOBI_COMPILER_HPP
#define NAOBI_COMPILER_HPP

#include <string>
#include <optional>
#include <vector>

namespace naobi
{
	class compiler
	{
	public:
		static std::optional<std::string> loadFile(const std::string& fileName);
		static std::vector<std::string> collectModules(const std::string& fileText);
		static std::vector<std::string> split(const std::string& text, const std::string& splitter, bool saveSplitter = false);
		static std::string removeExtraSpaces(const std::string& str);
		static std::string removeSym(const std::string& str, char symbolToRemove);
	};
}

#endif //NAOBI_COMPILER_HPP
