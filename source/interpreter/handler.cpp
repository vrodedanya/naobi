#include <naobi/interpreter/handler.hpp>

naobi::handler::handler(const naobi::composition& composition)
{
	for (const auto& workflow : composition.workflows)
	{
		auto context = std::make_shared<naobi::workflow_context>();
		context->workflow = workflow;
		context->ip = context->workflow->commands().begin();
		_contexts.emplace_back(context);
	}
}

void naobi::handler::execute()
{

}
