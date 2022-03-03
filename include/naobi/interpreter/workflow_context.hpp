#ifndef NAOBI_WORKFLOW_CONTEXT_HPP
#define NAOBI_WORKFLOW_CONTEXT_HPP

#include <vector>
#include <functional>
#include <memory>

#include <naobi/data/workflow.hpp>
#include <naobi/data/variable.hpp>

namespace naobi
{
	struct workflow_context
	{
		using sptr = std::shared_ptr<workflow_context>;

		std::vector<naobi::variable::sptr> variables;
		std::vector<naobi::variable::sptr> stack;
		std::vector<std::function<void(naobi::workflow_context::sptr ptr)>>::const_iterator ip;
		naobi::workflow::sptr workflow;
	};
}

#endif //NAOBI_WORKFLOW_CONTEXT_HPP
