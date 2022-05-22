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

		template <typename CHECKING_VALUE, typename... AArgs>
		bool checkLineAndRun(const CHECKING_VALUE& line, AArgs&&... aargs) const;

	private:
		CHECKER _checker;
		ACTION _action;
	};

	template <typename CHECKER, typename ACTION>
	rule<CHECKER, ACTION>::rule(CHECKER checker, ACTION action) :
		_checker(checker),
		_action(action)
	{
	}

	template <typename CHECKER, typename ACTION>
	template <typename CHECKING_VALUE, typename... AArgs>
	bool rule<CHECKER, ACTION>::checkLineAndRun(const CHECKING_VALUE &line, AArgs&&... aargs) const
	{
		if (_checker(line))
		{
			_action(line, std::forward<AArgs>(aargs)...);
			return true;
		}
		return false;
	}
}

#endif //NAOBI_RULE_HPP
