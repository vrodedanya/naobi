#include <utility>
#include <naobi/data/variable.hpp>
#include "naobi/utils/logger.hpp"

naobi::variable::variable(std::string name, naobi::variable::Type type) :
	_name(std::move(name)),
	_type(type)
{
	switch (_type)
	{
		case Type::INTEGER:
			_value = 0;
			break;
		case Type::UNDEFINED:
		default:
			LOG(varible.constructor, logger::CRITICAL, "CRITICAL VARIABLE WITH UNDEFINED TYPE");
			std::exit(1);
	}
}

naobi::variable::sptr naobi::operator+=(const naobi::variable::sptr& variable1, const naobi::variable::sptr& variable2)
{
	if (variable1->type() == variable::Type::INTEGER && variable1->type() == variable2->type())
	{
		int a = std::get<int>(variable1->value()) + std::get<int>(variable2->value());
		auto newVariable = std::make_shared<naobi::variable>("temp", variable::Type::INTEGER);
		newVariable->value() = a;
		return newVariable;
	}
	return nullptr;
}
