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
	};
}

#endif //NAOBI_COMPILER_HPP
