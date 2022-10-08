#include <naobi/data/template_function.hpp>

#include <algorithm>

#include <naobi/data/module.hpp>


naobi::template_function::template_function(std::string name, module* parent) :
	_name(std::move(name)),
	m_parent(parent)
{}

const std::string& naobi::template_function::getName() const
{
	return _name;
}

void naobi::template_function::setName(const std::string& name)
{
	_name = name;
}

const std::vector<naobi::template_function::argument_type>& naobi::template_function::getArguments() const
{
	return _arguments;
}

void naobi::template_function::setArguments(const std::vector<argument_type>& arguments)
{
	_arguments = arguments;
}

const std::string& naobi::template_function::getReturnType() const
{
	return _returnType;
}

void naobi::template_function::setReturnType(const std::string& returnType)
{
	_returnType = returnType;
}

const std::string& naobi::template_function::getCode() const
{
	return _code;
}

void naobi::template_function::setCode(const std::string& code)
{
	_code = code;
}

void naobi::template_function::addArgument(const std::string& name, const std::string& type)
{
	_arguments.emplace_back(name, type);
}

std::optional<naobi::template_function::argument_type> naobi::template_function::getArgument(const std::string& name)
{
	auto it = std::find_if(
		_arguments.begin(), _arguments.end(), [name](const auto& pair)
		{
			return name == pair.first;
		});
	if (it == _arguments.end()) return {};
	else return *it;
}

std::optional<naobi::template_function::argument_type> naobi::template_function::getArgument(std::size_t pos)
{
	if (pos >= _arguments.size()) return {};
	return _arguments[pos];
}

std::size_t naobi::template_function::getPosOfArgument(const std::string& name)
{
	auto it = std::find_if(
		_arguments.begin(), _arguments.end(), [name](const auto& pair)
		{
			return name == pair.first;
		});
	if (it == _arguments.end()) return {};
	else return static_cast<size_t>(std::distance(_arguments.begin(), it));
}
