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
	for (auto& context : _contexts)
	{
		while (context->ip != context->workflow->commands().cend())
		{
			context->ip->impl(context, context->ip->arguments);
			context->ip++;
		}
	}
}
