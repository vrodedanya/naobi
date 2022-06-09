#ifndef NAOBI_EVENT_MANAGER_HPP
#define NAOBI_EVENT_MANAGER_HPP

#include <vector>
#include <queue>
#include <list>
#include <thread>

#include <naobi/data/workflow.hpp>
#include <naobi/data/event.hpp>
#include <naobi/interpreter/workflow_context.hpp>
#include <naobi/interpreter/event_pool.hpp>
#include <naobi/interpreter/handler.hpp>


namespace naobi
{
	class event_manager
	{
	public:
		virtual ~event_manager();

		void addWorkflow(const naobi::event& event, const workflow::sptr& workflow);

		void execute();

		void handleEvent(const naobi::event& event);

		handler::sptr findHandler();

		[[nodiscard]] std::size_t awaitedNumberOfWorkflows() const
		{ return _workflows.size(); };

		event_pool& eventPool()
		{ return _eventPool; }

	private:
		std::multimap<naobi::event, workflow::sptr> _workflows;
		std::vector<handler::sptr> _handlers;
		std::vector<std::thread> _threads;
		event_pool _eventPool;
	};
}

#endif //NAOBI_EVENT_MANAGER_HPP
