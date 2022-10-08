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
		explicit workflow(std::string name, naobi::event target, module::sptr parentModule);

		[[nodiscard]] std::string name() const
		{ return _name; }

		[[nodiscard]] naobi::event target() const
		{ return _target; }

		[[nodiscard]] int invoke() const
		{ return _invoke; }

		void setInvoke(int invoke)
		{ _invoke = invoke; }

		[[nodiscard]] const std::vector<naobi::command>& commands() const
		{ return _commands; }

		void setCommands(const std::vector<naobi::command>& commands)
		{ _commands = commands; }

		naobi::module::sptr module()
		{ return _parentModule; }

		[[nodiscard]] naobi::module::sptr module() const
		{ return _parentModule; }

		friend std::ostream& operator << (std::ostream& stream, const workflow& flow);
		friend const std::ostream& operator >> (const std::ostream& stream, workflow& flow);
	private:
		std::string _name;
		naobi::event _target;
		naobi::module::sptr _parentModule;
		std::vector<naobi::command> _commands;
		int _invoke{-1};
	};

	std::ostream& operator << (std::ostream& stream, const workflow& flow);
	const std::ostream& operator >> (const std::ostream& stream, workflow& flow);
}

#endif //NAOBI_WORKFLOW_HPP
