#include <utility>
#include <naobi/data/workflow.hpp>

naobi::workflow::workflow(std::string name, module::sptr parentModule) :
		_name(std::move(name)),
		_parentModule(std::move(parentModule))
{

}
