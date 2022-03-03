#ifndef NAOBI_WORKFLOW_HPP
#define NAOBI_WORKFLOW_HPP

#include <vector>
#include <string>

#include <naobi/data/module.hpp>
#include <naobi/compiler/code_generator.hpp>

namespace naobi
{
	class workflow
	{
	public:
		using uptr = std::unique_ptr<workflow>;
		using sptr = std::shared_ptr<workflow>;
	public:
		explicit workflow(std::string name, module::sptr parentModule);

		[[nodiscard]] std::string name() const {return _name;}

		[[nodiscard]] int invoke() const {return _invoke;}
		void setInvoke(int invoke) {_invoke = invoke;}

		[[nodiscard]] const std::vector<naobi::commandImpl>& commands() const {return _commands;}
		void setCommands(const std::vector<naobi::commandImpl>& commands){_commands = commands;}

	private:
		std::string _name;
		naobi::module::sptr _parentModule;
		std::vector<naobi::commandImpl> _commands;
		int _invoke{-1};
	};
}

#endif //NAOBI_WORKFLOW_HPP
