#include <naobi/data/template_function.hpp>
#include <utility>

naobi::template_function::template_function(std::string name) : _name(std::move(name))
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

void naobi::template_function::addArgument(const naobi::template_function::argument_type& argument)
{
	_arguments.push_back(argument);
}
