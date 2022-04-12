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
		static void updateContexts(std::list<workflow_context::sptr>& _contexts);

		static void pushEvent(const std::string& event);
		static void addWorkflow(const std::string& event, const workflow::sptr& workflow);
	private:
		static inline std::multimap<std::string, workflow::sptr> _workflows;
		static inline std::queue<std::string> _events;
	};
}

#endif //NAOBI_EVENT_MANAGER_HPP
