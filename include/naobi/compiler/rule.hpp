#ifndef NAOBI_RULE_HPP
#define NAOBI_RULE_HPP

#include <functional>
#include "naobi/data/module.hpp"

namespace naobi
{

	template <typename CHECKER, typename ACTION>
	class rule
	{
	public:
		rule(CHECKER checker, ACTION action);
		void checkLineAndRun(const std::vector<std::string>& line, const naobi::module::sptr&) const;

	private:
		CHECKER _checker;
		ACTION _action;
	};

	template <typename CHECKER, typename ACTION>
	void rule<CHECKER, ACTION>::checkLineAndRun(const std::vector<std::string>& line, const naobi::module::sptr& module) const
	{
		if (_checker(line))
		{
			_action(line, module);
		}
	}

	template <typename CHECKER, typename ACTION>
	rule<CHECKER, ACTION>::rule(CHECKER checker, ACTION action) :
		_checker(checker),
		_action(action)
	{
	}
}

#endif //NAOBI_RULE_HPP
