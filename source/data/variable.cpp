#include <naobi/data/variable.hpp>

#include <naobi/utils/logger.hpp>


naobi::variable::variable(std::string name, utils::type::names type) :
	_name(std::move(name)),
	_type(type)
{
	switch (_type)
	{
		case utils::type::names::INTEGER:
			_value = 0;
			break;
		case utils::type::names::BOOLEAN:
			_value = false;
			break;
		case utils::type::names::STRING:
			_value = "";
			break;
		case utils::type::names::FLOAT:
			_value = 0.0;
			break;
		case utils::type::names::UNDEFINED:
		default:
			NLOG(varible.constructor, logger::CRITICAL, "CRITICAL variable with undefined type");
			std::exit(1);
	}
}

std::ostream& naobi::operator <<(std::ostream& os, const naobi::variable& var)
{
	if (var._type == naobi::utils::type::names::INTEGER)
	{
		os << std::get<int>(var._value);
	}
	else if (var._type == naobi::utils::type::names::BOOLEAN)
	{
		os << std::boolalpha << std::get<bool>(var._value);
	}
	else if (var._type == naobi::utils::type::names::STRING)
	{
		os << std::get<std::string>(var._value);
	}
	else if (var._type == naobi::utils::type::names::FLOAT)
	{
		os << std::get<double>(var._value);
	}
	else
	{
		os << "UNDEFINED VARIABLE TYPE";
	}
	return os;
}

naobi::variable& naobi::variable::operator =(const naobi::variable& var)
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


bool naobi::operator ==(const naobi::variable::sptr& var1, bool var2)
{
	if (var1->type() == utils::type::names::BOOLEAN)
	{
		return std::get<bool>(var1->value()) == var2;
	}
	NCRITICAL(variable, errors::TYPE_ERROR, "RUNTIME CRITICAL Variable should be boolean type!\n",
		 "Please, report this error to https://github.com/vrodedanya/naobi/issues");
}

bool naobi::operator !=(const naobi::variable::sptr& var1, bool var2)
{
	return !(var1 == var2);
}
