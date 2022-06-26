#include <utility>
#include <naobi/data/workflow.hpp>


naobi::workflow::workflow(std::string name, naobi::event target, module::sptr parentModule) :
	_name(std::move(name)),
	_target(std::move(target)),
	_parentModule(std::move(parentModule))
{
}
