#include <utility>
#include <naobi/data/variable.hpp>

#include <naobi/utils/logger.hpp>


naobi::variable::variable(std::string name, utils::type::type type) :
	_name(std::move(name)),
	_type(std::move(type))
{
	switch (_type.name)
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
	if (var._type.name == naobi::utils::type::names::INTEGER)
	{
		os << std::get<long long>(var._value);
	}
	else if (var._type.name == naobi::utils::type::names::BOOLEAN)
	{
		os << std::boolalpha << std::get<bool>(var._value);
	}
	else if (var._type.name == naobi::utils::type::names::STRING)
	{
		os << std::get<std::string>(var._value);
	}
	else if (var._type.name == naobi::utils::type::names::FLOAT)
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

void naobi::variable::invert()
{
	if (_type.name == naobi::utils::type::names::INTEGER)
	{
		_value = std::get<long long>(_value) * -1;
	}
	else if (_type.name == naobi::utils::type::names::FLOAT)
	{
		_value = std::get<double>(_value) * -1;
	}
}

naobi::variable& naobi::variable::operator =(naobi::variable&& var) noexcept
{
	this->_name = var._name;
	this->_value = var._value;
	this->_type = var._type;
	return *this;
}


bool naobi::operator ==(const naobi::variable::sptr& var1, bool var2)
{
	if (var1->type().name == utils::type::names::BOOLEAN)
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
