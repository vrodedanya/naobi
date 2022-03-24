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
		case Type::BOOLEAN:
			_value = false;
			break;
		case Type::UNDEFINED:
		default:
			LOG(varible.constructor, logger::CRITICAL, "CRITICAL VARIABLE WITH UNDEFINED TYPE");
			std::exit(1);
	}
}

naobi::variable::sptr naobi::operator+=(naobi::variable::sptr& variable1, const naobi::variable::sptr& variable2)
{
	if (variable1->type() == variable::Type::INTEGER && variable1->type() == variable2->type())
	{
		int a = std::get<int>(variable1->value()) + std::get<int>(variable2->value());
		variable1->value() = a;
		return variable1;
	}
	else if (variable1->type() == variable::Type::BOOLEAN && variable1->type() == variable2->type())
	{
		int a = std::get<bool>(variable1->value()) + std::get<bool>(variable2->value());
		variable1->value() = a;
		return variable1;
	}
	return nullptr;
}
naobi::variable::sptr naobi::operator -= (naobi::variable::sptr& variable1, const naobi::variable::sptr& variable2)
{
	if (variable1->type() == variable::Type::INTEGER && variable1->type() == variable2->type())
	{
		int a = std::get<int>(variable1->value()) - std::get<int>(variable2->value());
		variable1->value() = a;
		return variable1;
	}
	else if (variable1->type() == variable::Type::BOOLEAN && variable1->type() == variable2->type())
	{
		int a = std::get<bool>(variable1->value()) - std::get<bool>(variable2->value());
		variable1->value() = a;
		return variable1;
	}
	return nullptr;
}

naobi::variable::sptr naobi::operator *= (naobi::variable::sptr& variable1, const naobi::variable::sptr& variable2)
{
	if (variable1->type() == variable::Type::INTEGER && variable1->type() == variable2->type())
	{
		int a = std::get<int>(variable1->value()) * std::get<int>(variable2->value());
		variable1->value() = a;
		return variable1;
	}
	else if (variable1->type() == variable::Type::BOOLEAN && variable1->type() == variable2->type())
	{
		int a = std::get<bool>(variable1->value()) * std::get<bool>(variable2->value());
		variable1->value() = a;
		return variable1;
	}
	return nullptr;
}

naobi::variable::sptr naobi::operator /= (naobi::variable::sptr& variable1, const naobi::variable::sptr& variable2)
{
	if (variable1->type() == variable::Type::INTEGER && variable1->type() == variable2->type())
	{
		int a = std::get<int>(variable1->value()) / std::get<int>(variable2->value());
		variable1->value() = a;
		return variable1;
	}
	else if (variable1->type() == variable::Type::BOOLEAN && variable1->type() == variable2->type())
	{
		int a = std::get<bool>(variable1->value()) / std::get<bool>(variable2->value());
		variable1->value() = a;
		return variable1;
	}
	return nullptr;
}

naobi::variable::sptr naobi::operator == (const naobi::variable::sptr& variable1, const naobi::variable::sptr& variable2)
{
	if (variable1->type() == variable::Type::INTEGER && variable1->type() == variable2->type())
	{
		auto var = std::make_shared<naobi::variable>("temp", variable::Type::BOOLEAN);
		var->value() = std::get<int>(variable1->value()) == std::get<int>(variable2->value());
		return var;
	}
	else if (variable1->type() == variable::Type::BOOLEAN && variable1->type() == variable2->type())
	{
		auto var = std::make_shared<naobi::variable>("temp", variable::Type::BOOLEAN);
		var->value() = std::get<bool>(variable1->value()) == std::get<bool>(variable2->value());
		return var;
	}
	else return nullptr;
}

bool naobi::operator == (const naobi::variable::sptr& var1, bool var2)
{
	if (var1->type() == variable::Type::BOOLEAN)
	{
		return std::get<bool>(var1->value()) == var2;
	}
	LOG(variable, logger::CRITICAL, "RUNTIME CRITICAL Variable should be boolean type!\n",
		"Please, report this error to https://github.com/vrodedanya/naobi/issues");
	std::exit(EXIT_FAILURE);
}

bool naobi::operator != (const naobi::variable::sptr& var1, bool var2)
{
	return !(var1 == var2);
}

std::ostream& naobi::operator << (std::ostream& os, const naobi::variable& var)
{
	if (var._type == naobi::variable::Type::INTEGER)
	{
		os << std::get<int>(var._value);
	}
	else if (var._type == naobi::variable::Type::BOOLEAN)
	{
		os << std::boolalpha << std::get<bool>(var._value);
	}
	else
	{
		os << "UNDEFINED VARIABLE TYPE";
	}
	return os;
}

naobi::variable &naobi::variable::operator=(const naobi::variable &var)
{
	_value = var.value();
	return *this;
}

naobi::variable::sptr naobi::variable::copy()
{
	auto var = std::make_shared<naobi::variable>(_name, _type);
	var->_value = _value;
	return var;
}
