#include <utility>
#include <naobi/data/workflow.hpp>


naobi::workflow::workflow(std::string name, std::string target, module::sptr parentModule) :
	_name(std::move(name)),
	_target(target),
	_parentModule(std::move(parentModule))
{

}
