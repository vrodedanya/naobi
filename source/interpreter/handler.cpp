#include <naobi/interpreter/handler.hpp>
#include <iostream>

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
	for (auto context = _contexts.begin() ; !_contexts.empty(); context++)
	{
		if (context == _contexts.end()) context = _contexts.begin();
		(*context)->beginClock = std::chrono::system_clock::now();

		while (std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now() - (*context)->beginClock).count() < MAX_TIME_PER_CONTEXT)
		{
			if ((*context)->ip == (*context)->workflow->commands().cend())
			{
				_contexts.erase(context);
				break;
			}

			(*context)->ip->impl(*context, (*context)->ip->arguments);
			(*context)->ip++;
			//std::cout << "Time: " << std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now() - (*context)->beginClock).count() << std::endl;
		}
	}
}
