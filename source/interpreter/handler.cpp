#include <naobi/interpreter/handler.hpp>

#include <iostream>

#include <naobi/interpreter/event_manager.hpp>
#include <naobi/utils/logger.hpp>


naobi::handler::handler()
{
	event_manager::updateContexts(_contexts);
}

void naobi::handler::execute()
{
	using namespace std::chrono;

	for (auto context = _contexts.begin() ; !_contexts.empty(); context++)
	{
		if (context == _contexts.end()) context = _contexts.begin();
		(*context)->beginClock = high_resolution_clock::now();

		while (duration_cast<microseconds>(high_resolution_clock::now() - (*context)->beginClock).count() < MAX_TIME_PER_CONTEXT)
		{
			if ((*context)->ip == (*context)->workflow->commands().cend())
			{
				auto nextContext = std::next(context);
				if (nextContext == _contexts.end()) nextContext = _contexts.begin();
				event_manager::pushEvent((*context)->workflow->name() + "End");
				_contexts.erase(context);
				context = nextContext;
				break;
			}
			(*context)->ip->impl(*context, (*context)->ip->arguments);
			(*context)->ip++;
		}
		event_manager::updateContexts(_contexts);
		LOG(handler.execute, logger::IMPORTANT, "Contexts at the moment: ", _contexts.size());
	}
}
