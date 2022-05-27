#include <naobi/data/variable.hpp>

#include "naobi/utils/logger.hpp"

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

naobi::variable::sptr naobi::operator +=(naobi::variable::sptr& variable1, const naobi::variable::sptr& variable2)
{
	if (variable1->type() == utils::type::names::INTEGER && variable1->type() == variable2->type())
	{
		int a = std::get<int>(variable1->value()) + std::get<int>(variable2->value());
		variable1->value() = a;
		return variable1;
	}
	else if (variable1->type() == utils::type::names::BOOLEAN && variable1->type() == variable2->type())
	{
		bool a = std::get<bool>(variable1->value()) + std::get<bool>(variable2->value());
		variable1->value() = a;
		return variable1;
	}
	else if (variable1->type() == utils::type::names::FLOAT && variable1->type() == variable2->type())
	{
		double a = std::get<double>(variable1->value()) + std::get<double>(variable2->value());
		variable1->value() = a;
		return variable1;
	}
	else if (variable1->type() == utils::type::names::STRING && variable1->type() == variable2->type())
	{
		std::string a = std::get<std::string>(variable1->value()) + std::get<std::string>(variable2->value());
		variable1->value() = a;
		return variable1;
	}
	else if (variable1->type() == utils::type::names::STRING && variable2->type() == utils::type::names::INTEGER)
	{
		std::string a = std::get<std::string>(variable1->value()) + std::to_string(std::get<int>(variable2->value()));
		variable1->value() = a;
		return variable1;
	}
	else
	{
		NLOG(variable, logger::CRITICAL, "CRITICAL RUNTIME variable are not the same types: ",
			 utils::type::fromNameToString(variable1->type()), " and ",
			 utils::type::fromNameToString(variable2->type()));
		std::exit(1);
	}
	return nullptr;
}

naobi::variable::sptr naobi::operator -=(naobi::variable::sptr& variable1, const naobi::variable::sptr& variable2)
{
	if (variable1->type() == utils::type::names::INTEGER && variable1->type() == variable2->type())
	{
		int a = std::get<int>(variable1->value()) - std::get<int>(variable2->value());
		variable1->value() = a;
		return variable1;
	}
	else if (variable1->type() == utils::type::names::BOOLEAN && variable1->type() == variable2->type())
	{
		int a = std::get<bool>(variable1->value()) - std::get<bool>(variable2->value());
		variable1->value() = a;
		return variable1;
	}
	else if (variable1->type() == utils::type::names::FLOAT && variable1->type() == variable2->type())
	{
		double a = std::get<double>(variable1->value()) - std::get<double>(variable2->value());
		variable1->value() = a;
		return variable1;
	}
	else if (variable1->type() == utils::type::names::STRING && variable1->type() == variable2->type())
	{
		const std::string& var2 = std::get<std::string>(variable2->value());
		std::size_t enter;
		std::size_t length = var2.size();
		std::string a = std::get<std::string>(variable1->value());
		while ((enter = a.find(var2)) != std::string::npos)
		{
			a = a.erase(enter, length);
		}
		variable1->value() = a;
		return variable1;
	}
	else
	{
		NLOG(variable, logger::CRITICAL, "CRITICAL RUNTIME variable are not the same types: ",
			 utils::type::fromNameToString(variable1->type()), " and ",
			 utils::type::fromNameToString(variable2->type()));
		std::exit(1);
	}
	return nullptr;
}

naobi::variable::sptr naobi::operator *=(naobi::variable::sptr& variable1, const naobi::variable::sptr& variable2)
{
	if (variable1->type() == utils::type::names::INTEGER && variable1->type() == variable2->type())
	{
		int a = std::get<int>(variable1->value()) * std::get<int>(variable2->value());
		variable1->value() = a;
		return variable1;
	}
	else if (variable1->type() == utils::type::names::BOOLEAN && variable1->type() == variable2->type())
	{
		bool a = std::get<bool>(variable1->value()) && std::get<bool>(variable2->value());
		variable1->value() = a;
		return variable1;
	}
	else if (variable1->type() == utils::type::names::FLOAT && variable1->type() == variable2->type())
	{
		double a = std::get<double>(variable1->value()) * std::get<double>(variable2->value());
		variable1->value() = a;
		return variable1;
	}
	else if (variable1->type() == utils::type::names::STRING && variable2->type() == utils::type::names::INTEGER)
	{
		std::string temp;
		for (int i = 0 ; i < std::get<int>(variable2->value()) ; i++)
		{
			temp += std::get<std::string>(variable1->value());
		}
		variable1->value() = temp;
		return variable1;
	}
	else
	{
		NLOG(variable, logger::CRITICAL, "CRITICAL RUNTIME variable are not the same types: ",
			 utils::type::fromNameToString(variable1->type()), " and ",
			 utils::type::fromNameToString(variable2->type()));
		std::exit(1);
	}
	return nullptr;
}

naobi::variable::sptr naobi::operator /=(naobi::variable::sptr& variable1, const naobi::variable::sptr& variable2)
{
	if (variable1->type() == utils::type::names::INTEGER && variable1->type() == variable2->type())
	{
		int a = std::get<int>(variable1->value()) / std::get<int>(variable2->value());
		variable1->value() = a;
		return variable1;
	}
	else if (variable1->type() == utils::type::names::BOOLEAN && variable1->type() == variable2->type())
	{
		int a = std::get<bool>(variable1->value()) / std::get<bool>(variable2->value());
		variable1->value() = a;
		return variable1;
	}
	else if (variable1->type() == utils::type::names::FLOAT && variable1->type() == variable2->type())
	{
		double a = std::get<double>(variable1->value()) / std::get<double>(variable2->value());
		variable1->value() = a;
		return variable1;
	}
	else
	{
		NLOG(variable, logger::CRITICAL, "CRITICAL RUNTIME wrong types for division operator: ",
			 utils::type::fromNameToString(variable1->type()), " and ",
			 utils::type::fromNameToString(variable2->type()));
		std::exit(1);
	}
	return nullptr;
}

naobi::variable::sptr naobi::operator %=(naobi::variable::sptr& variable1, const naobi::variable::sptr& variable2)
{
	if (variable1->type() == utils::type::names::INTEGER && variable1->type() == variable2->type())
	{
		int a = std::get<int>(variable1->value()) % std::get<int>(variable2->value());
		variable1->value() = a;
		return variable1;
	}
	else if (variable1->type() == utils::type::names::BOOLEAN && variable1->type() == variable2->type())
	{
		int a = std::get<bool>(variable1->value()) % std::get<bool>(variable2->value());
		variable1->value() = a;
		return variable1;
	}
	else
	{
		NLOG(variable, logger::CRITICAL, "CRITICAL RUNTIME variable are not the same types: ",
			 utils::type::fromNameToString(variable1->type()), " and ",
			 utils::type::fromNameToString(variable2->type()));
		std::exit(1);
	}
	return nullptr;
}


naobi::variable::sptr naobi::operator >(const naobi::variable::sptr& variable1, const naobi::variable::sptr& variable2)
{
	if (variable1->type() == utils::type::names::INTEGER && variable1->type() == variable2->type())
	{
		auto temp = std::make_shared<naobi::variable>("temp", utils::type::names::BOOLEAN);
		temp->value() = std::get<int>(variable1->value()) > std::get<int>(variable2->value());
		return temp;
	}
	else if (variable1->type() == utils::type::names::BOOLEAN && variable1->type() == variable2->type())
	{
		auto temp = std::make_shared<naobi::variable>("temp", utils::type::names::BOOLEAN);
		temp->value() = std::get<bool>(variable1->value()) > std::get<bool>(variable2->value());
		return temp;
	}
	else if (variable1->type() == utils::type::names::FLOAT && variable1->type() == variable2->type())
	{
		auto temp = std::make_shared<naobi::variable>("temp", utils::type::names::BOOLEAN);
		temp->value() = std::get<double>(variable1->value()) > std::get<double>(variable2->value());
		return temp;
	}
	else if (variable1->type() == utils::type::names::STRING && variable2->type() == utils::type::names::INTEGER)
	{
		auto temp = std::make_shared<naobi::variable>("temp", utils::type::names::BOOLEAN);
		temp->value() = std::get<std::string>(variable1->value()) > std::get<std::string>(variable2->value());
		return temp;
	}
	else
	{
		NLOG(variable, logger::CRITICAL, "CRITICAL RUNTIME variable are not the same types: ",
			 utils::type::fromNameToString(variable1->type()), " and ",
			 utils::type::fromNameToString(variable2->type()));
		std::exit(1);
	}
}

naobi::variable::sptr naobi::operator <(const naobi::variable::sptr& variable1, const naobi::variable::sptr& variable2)
{
	if (variable1->type() == utils::type::names::INTEGER && variable1->type() == variable2->type())
	{
		auto temp = std::make_shared<naobi::variable>("temp", utils::type::names::BOOLEAN);
		temp->value() = std::get<int>(variable1->value()) < std::get<int>(variable2->value());
		return temp;
	}
	else if (variable1->type() == utils::type::names::BOOLEAN && variable1->type() == variable2->type())
	{
		auto temp = std::make_shared<naobi::variable>("temp", utils::type::names::BOOLEAN);
		temp->value() = std::get<bool>(variable1->value()) < std::get<bool>(variable2->value());
		return temp;
	}
	else if (variable1->type() == utils::type::names::FLOAT && variable1->type() == variable2->type())
	{
		auto temp = std::make_shared<naobi::variable>("temp", utils::type::names::BOOLEAN);
		temp->value() = std::get<double>(variable1->value()) < std::get<double>(variable2->value());
		return temp;
	}
	else if (variable1->type() == utils::type::names::STRING && variable2->type() == utils::type::names::INTEGER)
	{
		auto temp = std::make_shared<naobi::variable>("temp", utils::type::names::BOOLEAN);
		temp->value() = std::get<std::string>(variable1->value()) < std::get<std::string>(variable2->value());
		return temp;
	}
	else
	{
		NLOG(variable, logger::CRITICAL, "CRITICAL RUNTIME variable are not the same types: ",
			 utils::type::fromNameToString(variable1->type()), " and ",
			 utils::type::fromNameToString(variable2->type()));
		std::exit(1);
	}
}

naobi::variable::sptr naobi::operator >=(const naobi::variable::sptr& variable1, const naobi::variable::sptr& variable2)
{
	if (variable1->type() == utils::type::names::INTEGER && variable1->type() == variable2->type())
	{
		auto temp = std::make_shared<naobi::variable>("temp", utils::type::names::BOOLEAN);
		temp->value() = std::get<int>(variable1->value()) >= std::get<int>(variable2->value());
		return temp;
	}
	else if (variable1->type() == utils::type::names::BOOLEAN && variable1->type() == variable2->type())
	{
		auto temp = std::make_shared<naobi::variable>("temp", utils::type::names::BOOLEAN);
		temp->value() = std::get<bool>(variable1->value()) >= std::get<bool>(variable2->value());
		return temp;
	}
	else if (variable1->type() == utils::type::names::FLOAT && variable1->type() == variable2->type())
	{
		auto temp = std::make_shared<naobi::variable>("temp", utils::type::names::BOOLEAN);
		temp->value() = std::get<double>(variable1->value()) >= std::get<double>(variable2->value());
		return temp;
	}
	else if (variable1->type() == utils::type::names::STRING && variable2->type() == utils::type::names::INTEGER)
	{
		auto temp = std::make_shared<naobi::variable>("temp", utils::type::names::BOOLEAN);
		temp->value() = std::get<std::string>(variable1->value()) >= std::get<std::string>(variable2->value());
		return temp;
	}
	else
	{
		NLOG(variable, logger::CRITICAL, "CRITICAL RUNTIME variable are not the same types: ",
			 utils::type::fromNameToString(variable1->type()), " and ",
			 utils::type::fromNameToString(variable2->type()));
		std::exit(1);
	}
}

naobi::variable::sptr naobi::operator <=(const naobi::variable::sptr& variable1, const naobi::variable::sptr& variable2)
{
	if (variable1->type() == utils::type::names::INTEGER && variable1->type() == variable2->type())
	{
		auto temp = std::make_shared<naobi::variable>("temp", utils::type::names::BOOLEAN);
		temp->value() = std::get<int>(variable1->value()) <= std::get<int>(variable2->value());
		return temp;
	}
	else if (variable1->type() == utils::type::names::BOOLEAN && variable1->type() == variable2->type())
	{
		auto temp = std::make_shared<naobi::variable>("temp", utils::type::names::BOOLEAN);
		temp->value() = std::get<bool>(variable1->value()) <= std::get<bool>(variable2->value());
		return temp;
	}
	else if (variable1->type() == utils::type::names::FLOAT && variable1->type() == variable2->type())
	{
		auto temp = std::make_shared<naobi::variable>("temp", utils::type::names::BOOLEAN);
		temp->value() = std::get<double>(variable1->value()) <= std::get<double>(variable2->value());
		return temp;
	}
	else if (variable1->type() == utils::type::names::STRING && variable2->type() == utils::type::names::INTEGER)
	{
		auto temp = std::make_shared<naobi::variable>("temp", utils::type::names::BOOLEAN);
		temp->value() = std::get<std::string>(variable1->value()) <= std::get<std::string>(variable2->value());
		return temp;
	}
	else
	{
		NLOG(variable, logger::CRITICAL, "CRITICAL RUNTIME variable are not the same types: ",
			 utils::type::fromNameToString(variable1->type()), " and ",
			 utils::type::fromNameToString(variable2->type()));
		std::exit(1);
	}
}

naobi::variable::sptr naobi::operator ==(const naobi::variable::sptr& variable1, const naobi::variable::sptr& variable2)
{
	if (variable1->type() == utils::type::names::INTEGER && variable1->type() == variable2->type())
	{
		auto var = std::make_shared<naobi::variable>("temp", utils::type::names::BOOLEAN);
		var->value() = std::get<int>(variable1->value()) == std::get<int>(variable2->value());
		return var;
	}
	else if (variable1->type() == utils::type::names::BOOLEAN && variable1->type() == variable2->type())
	{
		auto var = std::make_shared<naobi::variable>("temp", utils::type::names::BOOLEAN);
		var->value() = std::get<bool>(variable1->value()) == std::get<bool>(variable2->value());
		return var;
	}
	else if (variable1->type() == utils::type::names::FLOAT && variable1->type() == variable2->type())
	{
		auto temp = std::make_shared<naobi::variable>("temp", utils::type::names::BOOLEAN);
		temp->value() = std::get<double>(variable1->value()) == std::get<double>(variable2->value());
		return temp;
	}
	else if (variable1->type() == utils::type::names::STRING && variable2->type() == utils::type::names::INTEGER)
	{
		auto temp = std::make_shared<naobi::variable>("temp", utils::type::names::BOOLEAN);
		temp->value() = std::get<std::string>(variable1->value()) == std::get<std::string>(variable2->value());
		return temp;
	}
	else
	{
		NLOG(variable, logger::CRITICAL, "CRITICAL RUNTIME variable are not the same types: ",
			 utils::type::fromNameToString(variable1->type()), " and ",
			 utils::type::fromNameToString(variable2->type()));
		std::exit(1);
	}
}

bool naobi::operator ==(const naobi::variable::sptr& var1, bool var2)
{
	if (var1->type() == utils::type::names::BOOLEAN)
	{
		return std::get<bool>(var1->value()) == var2;
	}
	NLOG(variable, logger::CRITICAL, "RUNTIME CRITICAL Variable should be boolean type!\n",
		 "Please, report this error to https://github.com/vrodedanya/naobi/issues");
	std::exit(EXIT_FAILURE);
}

naobi::variable::sptr naobi::operator !=(const naobi::variable::sptr& variable1, const naobi::variable::sptr& variable2)
{
	if (variable1->type() == utils::type::names::INTEGER && variable1->type() == variable2->type())
	{
		auto var = std::make_shared<naobi::variable>("temp", utils::type::names::BOOLEAN);
		var->value() = std::get<int>(variable1->value()) != std::get<int>(variable2->value());
		return var;
	}
	else if (variable1->type() == utils::type::names::BOOLEAN && variable1->type() == variable2->type())
	{
		auto var = std::make_shared<naobi::variable>("temp", utils::type::names::BOOLEAN);
		var->value() = std::get<bool>(variable1->value()) != std::get<bool>(variable2->value());
		return var;
	}
	else if (variable1->type() == utils::type::names::FLOAT && variable1->type() == variable2->type())
	{
		auto temp = std::make_shared<naobi::variable>("temp", utils::type::names::BOOLEAN);
		temp->value() = std::get<double>(variable1->value()) != std::get<double>(variable2->value());
		return temp;
	}
	else if (variable1->type() == utils::type::names::STRING && variable2->type() == utils::type::names::INTEGER)
	{
		auto temp = std::make_shared<naobi::variable>("temp", utils::type::names::BOOLEAN);
		temp->value() = std::get<std::string>(variable1->value()) != std::get<std::string>(variable2->value());
		return temp;
	}
	else
	{
		NLOG(variable, logger::CRITICAL, "CRITICAL RUNTIME variable are not the same types: ",
			 utils::type::fromNameToString(variable1->type()), " and ",
			 utils::type::fromNameToString(variable2->type()));
		std::exit(1);
	}
}

bool naobi::operator !=(const naobi::variable::sptr& var1, bool var2)
{
	return !(var1 == var2);
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
