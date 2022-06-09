#include <naobi/interpreter/handler.hpp>

#include <iostream>
#include <thread>

#include <naobi/interpreter/event_manager.hpp>
#include <naobi/utils/logger.hpp>


void naobi::handler::execute()
{
	while(isWork())
	{
		using namespace std::chrono;
		pullContexts();
		NLOG(handler, logger::BASIC, "Got ", _contexts.size(), " contexts");
		while (_contexts.empty())
		{
			std::this_thread::sleep_for(5us);
			if (!isWork()) return;
			pullContexts();
		}
		for (auto context = _contexts.begin() ; isWork() && context != _contexts.end() ; context++)
		{
			(*context)->beginClock = high_resolution_clock::now();
			while (duration_cast<microseconds>(high_resolution_clock::now() - (*context)->beginClock).count() <
				   MAX_TIME_PER_CONTEXT)
			{
				if ((*context)->ip == (*context)->workflow->commands().cend())
				{
					naobi::event e;
					e.setName((*context)->workflow->name() + "End");
					_eventManager->eventPool().push(e);
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
		}
	}
	NLOG(handler, logger::LOW, "Finishing working");
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
	(*context->variables)[exception.name + ".name"] = var1;
	(*context->variables)[exception.name + ".description"] = var2;
}

void naobi::handler::pushContext(const naobi::workflow_context::sptr& newContext)
{
	std::lock_guard<std::mutex> guard(_mutex);
	_bufferContexts.push(newContext);
}

void naobi::handler::pullContexts()
{
	std::lock_guard<std::mutex> guard(_mutex);
	while (!_bufferContexts.empty())
	{
		_contexts.push_back(_bufferContexts.front());
		_bufferContexts.pop();
	}
}

bool naobi::handler::isWork() const
{
	return _isWork;
}

void naobi::handler::setIsWork(bool isWork)
{
	_isWork = isWork;
}

naobi::handler::handler(naobi::event_manager* eventManager) : _eventManager(eventManager)
{}
