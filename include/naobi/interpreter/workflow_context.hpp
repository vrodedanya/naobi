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
		using variable_storage = std::map<std::string, naobi::variable::sptr>;

		event_manager* eventManager;
		std::shared_ptr<variable_storage> variables{std::make_shared<variable_storage>()};
		std::stack<std::shared_ptr<std::map<std::string, naobi::variable::sptr>>> variablesStack;
		std::stack<naobi::variable::sptr> stack;
		std::stack<std::vector<command>::const_iterator> returnPoints;
		std::vector<command>::const_iterator ip;
		naobi::workflow::sptr workflow;
		std::chrono::high_resolution_clock::time_point beginClock;
	};
}

#endif //NAOBI_WORKFLOW_CONTEXT_HPP
