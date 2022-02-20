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
		explicit workflow(std::string name, module* parentModule);

		[[nodiscard]] std::string name() const {return _name;}

	private:
		std::string _name;
		naobi::module::sptr _parentModule;
	};
}

#endif //NAOBI_WORKFLOW_HPP
