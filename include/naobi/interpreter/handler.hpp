#ifndef NAOBI_HANDLER_HPP
#define NAOBI_HANDLER_HPP

#include <naobi/interpreter/workflow_context.hpp>
#include <naobi/compiler/composition.hpp>

namespace naobi
{
	class handler
	{
	public:
		explicit handler(const naobi::composition& composition);
		void execute();
	private:
		std::vector<naobi::workflow_context::sptr> _contexts;
	};
}

#endif //NAOBI_HANDLER_HPP
