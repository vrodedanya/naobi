#ifndef NAOBI_EVENT_MANAGER_HPP
#define NAOBI_EVENT_MANAGER_HPP

#include <vector>
#include <queue>
#include <list>

#include <naobi/data/workflow.hpp>
#include <naobi/data/event.hpp>
#include <naobi/interpreter/workflow_context.hpp>


namespace naobi
{
	class event_manager
	{
	public:
		void updateContexts(std::list<workflow_context::sptr>& _contexts);

		void pushEvent(const naobi::event& event);

		void addWorkflow(const naobi::event& event, const workflow::sptr& workflow);

		[[nodiscard]] std::size_t awaitedNumberOfWorkflows() const
		{ return _workflows.size(); };
	private:
		std::multimap<naobi::event, workflow::sptr> _workflows;
		std::queue<naobi::event> _events;
	};
}

#endif //NAOBI_EVENT_MANAGER_HPP
