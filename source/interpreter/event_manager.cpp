#include <naobi/interpreter/event_manager.hpp>

#include <naobi/utils/logger.hpp>


void naobi::event_manager::updateContexts(std::list<workflow_context::sptr>&_contexts)
{
	while (!_events.empty())
	{
		NLOG(event_manager.updateContexts, logger::LOW, "Event: ", _events.front());
		auto range = _workflows.equal_range(_events.front());
		for (auto workflowIterator = range.first ; workflowIterator != range.second ; )
		{
			NLOG(event_manager.updateContexts, logger::LOW, "Find workflow ", workflowIterator->second->name());

			auto context = std::make_shared<naobi::workflow_context>();
			context->eventManager = this;
			context->workflow = workflowIterator->second;
			context->ip = context->workflow->commands().begin();
			_contexts.emplace_back(context);

			if (workflowIterator->second->invoke() == 0)
			{
				auto next = std::next(workflowIterator);
				_workflows.erase(workflowIterator);
				if (next == range.second) break;
				continue;
			}
			if (workflowIterator->second->invoke() > 0)
			{
				workflowIterator->second->setInvoke(workflowIterator->second->invoke() - 1);
				if (workflowIterator->second->invoke() == 0)
				{
					auto next = std::next(workflowIterator);
					_workflows.erase(workflowIterator);
					if (next == range.second) break;
					continue;
				}
			}
			workflowIterator++;
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
