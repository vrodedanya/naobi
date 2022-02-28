#ifndef NAOBI_WORKFLOW_HPP
#define NAOBI_WORKFLOW_HPP

#include <vector>
#include <string>

#include <naobi/data/module.hpp>

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

	private:
		std::string _name;
		naobi::module::sptr _parentModule;
		int _invoke{-1};
	};
}

#endif //NAOBI_WORKFLOW_HPP
