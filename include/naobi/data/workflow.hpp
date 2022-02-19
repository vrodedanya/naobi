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
		explicit workflow(module* parentModule) : _parentModule(parentModule){}
	private:
		std::string _name;
		module const * _parentModule;
	};
}

#endif //NAOBI_WORKFLOW_HPP
