#ifndef NAOBI_HANDLER_HPP
#define NAOBI_HANDLER_HPP

#include <list>

#include <naobi/interpreter/workflow_context.hpp>
#include <naobi/compiler/composition.hpp>

namespace naobi
{
	class handler
	{
	public:
		handler();

		void execute();

	private:
		std::list<naobi::workflow_context::sptr> _contexts;

		const int MAX_TIME_PER_CONTEXT = 33;
	};
}

#endif //NAOBI_HANDLER_HPP
