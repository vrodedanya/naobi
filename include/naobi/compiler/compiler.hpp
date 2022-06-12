#ifndef NAOBI_COMPILER_HPP
#define NAOBI_COMPILER_HPP

#include <string>
#include <optional>
#include <vector>

#include <naobi/data/workflow.hpp>
#include <naobi/data/module.hpp>
#include <naobi/compiler/rule.hpp>


namespace naobi
{
	class compiler
	{
	public:
		using compilerRule = naobi::rule<std::function<bool(const std::vector<std::string>& line)>,
			std::function<void(const std::vector<std::string>& line, const naobi::module::sptr& module)>>;

	public:
		compiler();

		void compile(const std::string& fileName);

		void compileText(const std::string& text);

		[[nodiscard]] std::vector<workflow::sptr> getWorkflows() const
		{ return _workflows; }

	private:
		void compile(const std::string& fileName, const naobi::module::sptr& parent);

		std::string processFileName(const std::string& fileName);

		void processImportingModules(const std::vector<std::string>& lines, const naobi::module::sptr& module);

		void processModule(const std::vector<std::string>& lines, const naobi::module::sptr& module);

		std::optional<std::string> loadFile(const std::string& fileName);

		std::vector<std::string> collectModules(const std::vector<std::string>& lines);

	private:
		std::vector<compilerRule> _rules;
		std::vector<workflow::sptr> _workflows;
		naobi::module::sptr _root;
	};
}

#endif //NAOBI_COMPILER_HPP
