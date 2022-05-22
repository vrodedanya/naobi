#ifndef NAOBI_EVENT_MANAGER_HPP
#define NAOBI_EVENT_MANAGER_HPP

#include <vector>
#include <queue>
#include <list>

#include <naobi/data/workflow.hpp>
#include <naobi/interpreter/workflow_context.hpp>


namespace naobi
{
	class event_manager
	{
	public:
		void updateContexts(std::list<workflow_context::sptr>& _contexts);

		void pushEvent(const std::string& event);
		void addWorkflow(const std::string& event, const workflow::sptr& workflow);
		[[nodiscard]] std::size_t awaitedNumberOfWorkflows() const {return _workflows.size();};
	private:
		std::multimap<std::string, workflow::sptr> _workflows;
		std::queue<std::string> _events;
	};
}

#endif //NAOBI_EVENT_MANAGER_HPP
