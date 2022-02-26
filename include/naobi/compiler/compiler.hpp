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
		void compile(const std::string& fileName);

		[[nodiscard]] naobi::composition getComposition() const noexcept {return _composition;}
	private:
		void compile(const std::string& fileName, const naobi::module::sptr& parent);

		std::string processFileName(const std::string& fileName);
		void processModules(const std::vector<std::string>& lines, const naobi::module::sptr& module);

		std::optional<std::string> loadFile(const std::string& fileName);
		std::vector<std::string> collectModules(const std::vector<std::string>& lines);

	private:
		naobi::composition _composition;
	};
}

#endif //NAOBI_COMPILER_HPP
