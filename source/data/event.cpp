#include <naobi/data/event.hpp>

#include <algorithm>


const std::string& naobi::event::getName() const
{
	return _name;
}

void naobi::event::setName(const std::string& name)
{
	_name = name;
}

const std::vector<naobi::event::argument>& naobi::event::getArguments() const
{
	return _arguments;
}
std::vector<naobi::event::argument>& naobi::event::getArguments()
{
	return _arguments;
}

void naobi::event::setArguments(const std::vector<argument>& arguments)
{
	_arguments = arguments;
}

bool
naobi::event::addArgument(const std::string& name, const naobi::utils::type::type& type, const naobi::variable::sptr& pointer)
{
	auto it = std::find_if(
		_arguments.begin(), _arguments.end(), [name](const auto& tuple)
		{
			return std::get<0>(tuple) == name;
		});
	if (it != _arguments.end()) return false;
	_arguments.emplace_back(name, type, pointer);
	return true;
}

std::optional<naobi::event::argument> naobi::event::getArgument(const std::string& name)
{
	auto it = std::find_if(
		_arguments.begin(), _arguments.end(), [name](const auto& tuple)
		{
			return std::get<0>(tuple) == name;
		});
	if (it == _arguments.end()) return {};
	else return *it;
}

void naobi::event::setArgument(
	const std::string& name, const naobi::variable::sptr& variable)
{
	auto it = std::find_if(
		_arguments.begin(), _arguments.end(), [name](const auto& tuple)
		{
			return std::get<0>(tuple) == name;
		});
	if (it == _arguments.end()) return;
	std::get<2>(*it) = variable;
}

bool naobi::event::operator ==(const naobi::event& rhs) const
{
	return _name == rhs._name;
}

bool naobi::event::operator !=(const naobi::event& rhs) const
{
	return !(rhs == *this);
}

bool naobi::event::operator <(const naobi::event& rhs) const
{
	return _name < rhs._name;
}

bool naobi::event::operator >(const naobi::event& rhs) const
{
	return rhs < *this;
}

bool naobi::event::operator <=(const naobi::event& rhs) const
{
	return !(rhs < *this);
}

bool naobi::event::operator >=(const naobi::event& rhs) const
{
	return !(*this < rhs);
}

std::optional<naobi::event::argument> naobi::event::getArgument(std::size_t id)
{
	if (id >= _arguments.size()) return {};
	return _arguments[id];
}

void naobi::event::setArgument(std::size_t pos, const naobi::variable::sptr& variable)
{
	if (pos >= _arguments.size()) return;
	std::get<2>(_arguments[pos]) = variable;
}

naobi::event::event(naobi::event&& e) noexcept
{
	this->_arguments = std::move(e._arguments);
	this->_name = e._name;
}

naobi::event::event(const naobi::event& e)
{
	this->_arguments = e._arguments;
	this->_name = e._name;
}
