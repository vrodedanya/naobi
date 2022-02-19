#ifndef NAOBI_COMPILER_HPP
#define NAOBI_COMPILER_HPP

#include <string>
#include <optional>
#include <vector>

#include <naobi/data/workflow.hpp>
#include <naobi/data/module.hpp>
#include <naobi/compiler/composition.hpp>

namespace naobi
{
	class compiler
	{
	public:
		static std::optional<std::string> loadFile(const std::string& fileName);
		static std::vector<std::string> collectModules(const std::string& fileText);

		static std::optional<composition> compile(const std::string& fileName);
	private:
		static std::optional<naobi::module*> compile(const std::string& fileName, std::vector<naobi::workflow>& workflows);


	};
}

#endif //NAOBI_COMPILER_HPP
