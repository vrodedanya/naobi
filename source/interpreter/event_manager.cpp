#include <naobi/interpreter/event_manager.hpp>

#include <naobi/utils/logger.hpp>


void naobi::event_manager::handleEvent(const naobi::event& event)
{
	NLOG(event_manager.handleEvents, logger::IMPORTANT, "Event: ", event.getName());
	auto range = _workflows.equal_range(event);
	for (auto workflowIterator = range.first ; workflowIterator != range.second ;)
	{
		NLOG(event_manager.handleEvents, logger::BASIC, "Find workflow ", workflowIterator->second->name());

		if (workflowIterator->second->invoke() == 0)
		{
			auto next = std::next(workflowIterator);
			_workflows.erase(workflowIterator);
			if (next == range.second) break;
			workflowIterator = next;
			continue;
		}

		auto tempContext = std::make_shared<naobi::workflow_context>();
		tempContext->eventManager = this;
		tempContext->workflow = workflowIterator->second;
		tempContext->ip = tempContext->workflow->commands().begin();
		for (const auto& args : event.getArguments())
		{
			(*tempContext->variables)[event.getName() + "." + std::get<0>(args)] = std::get<2>(args);
		}

		auto temp = findHandler();
		temp->pushContext(tempContext);

		if (workflowIterator->second->invoke() > 0)
		{
			workflowIterator->second->setInvoke(workflowIterator->second->invoke() - 1);
		}
		if (workflowIterator->second->invoke() == 0)
		{
			auto next = std::next(workflowIterator);
			_workflows.erase(workflowIterator);
			if (next == range.second) break;
			workflowIterator = next;
			continue;
		}

		workflowIterator++;
	}
}


void naobi::event_manager::addWorkflow(const naobi::event& event, const naobi::workflow::sptr& workflow)
{
	NLOG(event_manager.addWorkflow, logger::LOW, "Add workflow ", workflow->name(), " on target ",
		 event.getName());
	_workflows.emplace(event, workflow);
}

void naobi::event_manager::execute()
{
	using namespace std::chrono_literals;
	while (true)
	{
		if (_eventPool.isNotEmpty())
		{
			handleEvent(_eventPool.pop());
		}
		else
		{
			if (awaitedNumberOfWorkflows() == 0)
			{
				NLOG(event_manager, logger::LOW, "Zero workflows awaited");
				for (auto& handler : _handlers)
				{
					if (handler->contextsCount() == 0) handler->setIsWork(false);
				}
				if (std::all_of(_handlers.begin(),_handlers.end(), [](const handler::sptr& a){
					return !a->isWork();
				}))
				{
					break;
				}
			}
			std::this_thread::sleep_for(0s);
		}
	}
}

naobi::event_manager::~event_manager()
{
	for (auto& thread : _threads)
	{
		if (thread.joinable())
		{
			thread.join();
		}
	}
}

naobi::handler::sptr naobi::event_manager::findHandler()
{
	if (_handlers.size() < std::thread::hardware_concurrency())
	{
		NLOG(event_manager, logger::BASIC, "Create new handler");
		auto tempHandler = std::make_shared<handler>(this);
		_threads.emplace_back(&handler::execute, tempHandler);
		_handlers.push_back(tempHandler);
		return tempHandler;
	}
	else
	{
		auto tempHandler = std::min_element(_handlers.begin(), _handlers.end(), [](const naobi::handler::sptr& a, const naobi::handler::sptr& b)
		{
			return a->contextsCount() < b->contextsCount();
		});
		NLOG(event_manager, logger::BASIC, "Find handler with id ", std::distance(_handlers.begin(), tempHandler));
		return *tempHandler;
	}
}
