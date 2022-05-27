#ifndef NAOBI_HANDLER_HPP
#define NAOBI_HANDLER_HPP

#include <list>

#include <naobi/interpreter/workflow_context.hpp>
#include <naobi/interpreter/event_manager.hpp>


namespace naobi
{
	class handler
	{
	public:
		const int MAX_TIME_PER_CONTEXT = 5;

		void execute();

		event_manager& eventManager()
		{ return _eventManager; }

	private:
		std::list<naobi::workflow_context::sptr> _contexts;
		event_manager _eventManager;
	};
}

#endif //NAOBI_HANDLER_HPP
