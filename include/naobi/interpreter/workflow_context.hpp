#ifndef NAOBI_WORKFLOW_CONTEXT_HPP
#define NAOBI_WORKFLOW_CONTEXT_HPP

#include <stack>
#include <vector>
#include <functional>
#include <memory>
#include <chrono>

#include <naobi/data/workflow.hpp>
#include <naobi/data/variable.hpp>
#include <naobi/interpreter/command.hpp>


namespace naobi
{

	class event_manager;

	struct workflow_context
	{
		using sptr = std::shared_ptr<workflow_context>;

		event_manager* eventManager;
		std::map<std::string, naobi::variable::sptr> variables;
		std::stack<naobi::variable::sptr> stack;
		std::stack<std::vector<command>::const_iterator> returnPoints;
		std::vector<command>::const_iterator ip;
		naobi::workflow::sptr workflow;
		std::chrono::high_resolution_clock::time_point beginClock;
	};
}

#endif //NAOBI_WORKFLOW_CONTEXT_HPP
