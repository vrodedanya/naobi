#include <naobi/data/workflow.hpp>

naobi::workflow::workflow(std::string name, module *parentModule) :
		_name(std::move(name)),
		_parentModule(parentModule)
{

}
