#include <naobi/interpreter/handler.hpp>

#include <iostream>

#include <naobi/interpreter/event_manager.hpp>


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
				_contexts.erase(context);
				context = _contexts.begin();
				break;
			}

			(*context)->ip->impl(*context, (*context)->ip->arguments);
			(*context)->ip++;
		}
		event_manager::updateContexts(_contexts);
	}
}
