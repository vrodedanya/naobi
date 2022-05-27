#include <naobi/interpreter/handler.hpp>

#include <iostream>
#include <thread>

#include <naobi/interpreter/event_manager.hpp>
#include <naobi/utils/logger.hpp>


void naobi::handler::execute()
{
	using namespace std::chrono;
	_eventManager.updateContexts(_contexts);

	for (auto context = _contexts.begin() ; !_contexts.empty() || _eventManager.awaitedNumberOfWorkflows() != 0 ;)
	{
		if (_contexts.empty())
		{
			std::this_thread::sleep_for(0s);
			_eventManager.updateContexts(_contexts);
			continue;
		}
		else
		{
			context++;
		}
		if (context == _contexts.end()) context = _contexts.begin();
		(*context)->beginClock = high_resolution_clock::now();

		while (duration_cast<microseconds>(high_resolution_clock::now() - (*context)->beginClock).count() <
			   MAX_TIME_PER_CONTEXT)
		{
			if ((*context)->ip == (*context)->workflow->commands().cend())
			{
				_eventManager.pushEvent((*context)->workflow->name() + "End");
				if (context == _contexts.begin())
				{
					_contexts.erase(context);
					context = _contexts.begin();
				}
				else
				{
					auto nextContext = std::prev(context);
					_contexts.erase(context);
					context = nextContext;
				}
				break;
			}
			(*context)->ip->impl(*context, (*context)->ip->arguments);
			(*context)->ip++;
		}
		_eventManager.updateContexts(_contexts);
		NLOG(handler.execute, logger::IMPORTANT, "Contexts at the moment: ", _contexts.size());
	}
}
