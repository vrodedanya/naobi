#ifndef NAOBI_COMPOSITION_HPP
#define NAOBI_COMPOSITION_HPP

#include <naobi/data/module.hpp>
#include <naobi/data/workflow.hpp>

namespace naobi
{
	struct composition
	{
		std::vector<naobi::workflow> workflows;
		naobi::module* rootModule;
	};
}

#endif //NAOBI_COMPOSITION_HPP
