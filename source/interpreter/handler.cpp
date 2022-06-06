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
				naobi::event event;
				event.setName((*context)->workflow->name() + "End");
				_eventManager.pushEvent(event);
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
			try
			{
				(*context)->ip->impl(*context, (*context)->ip->arguments);
			}
			catch (const naobi::naobi_exception& except)
			{
				auto exception = naobi::exception();
				exception.name = except.name;
				exception.description = except.description;
				catchException(exception, *context);
			}
			catch (const std::exception& except)
			{
				auto exception = naobi::exception();
				exception.name = "CppException";
				exception.description = except.what();
				catchException(exception, *context);
			}
			(*context)->ip++;
		}
		_eventManager.updateContexts(_contexts);
		NLOG(handler.execute, logger::IMPORTANT, "Contexts at the moment: ", _contexts.size());
	}
}

void naobi::handler::catchException(const naobi::exception& exception, naobi::workflow_context::sptr& context)
{
	while (!(context->ip->name == command::names::CATCH && exception.name == context->ip->arguments.front()))
	{
		context->ip++;
		if (context->ip == context->workflow->commands().cend())
		{
			NCRITICAL(handler, errors::EXCEPTION, "CRITICAL can't catch exception with name '", exception.name, "'\n",
					  exception.description);
		}
		if (context->ip->name == command::names::RETURN)
		{
			context->ip->impl(context, context->ip->arguments);
		}
	}
	context->ip++;
	auto var1 = std::make_shared<naobi::variable>(exception.name + ".name", utils::type::names::STRING);
	auto var2 = std::make_shared<naobi::variable>(exception.name + ".description", utils::type::names::STRING);
	var1->value() = exception.name;
	var2->value() = exception.description;
	context->variables[exception.name + ".name"] = var1;
	context->variables[exception.name + ".description"] = var2;
}
