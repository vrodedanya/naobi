#include <naobi/interpreter/event_manager.hpp>
#include <naobi/utils/logger.hpp>

void naobi::event_manager::updateContexts(std::list<workflow_context::sptr>&_contexts)
{
	while (!_events.empty())
	{
		NLOG(event_manager.updateContexts, logger::LOW, "Event: ", _events.front());
		auto range = _workflows.equal_range(_events.front());
		for (auto workflowIterator = range.first ; workflowIterator != range.second ; workflowIterator++)
		{
			NLOG(event_manager.updateContexts, logger::LOW, "Find workflow ", workflowIterator->second->name());
			auto context = std::make_shared<naobi::workflow_context>();
			context->workflow = workflowIterator->second;
			context->ip = context->workflow->commands().begin();
			_contexts.emplace_back(context);
		}
		_events.pop();
	}
}


void naobi::event_manager::addWorkflow(const std::string &event, const naobi::workflow::sptr& workflow)
{
	NLOG(event_manager.addWorkflow, logger::IMPORTANT, "Add workflow ", workflow->name(), " on target ", event);
	_workflows.emplace(event, workflow);
}

void naobi::event_manager::pushEvent(const std::string &event)
{
	_events.push(event);
}
