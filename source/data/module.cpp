#include <naobi/data/module.hpp>

#include <algorithm>

#include <naobi/utils/indent_scope.hpp>


naobi::module::module(std::string name)
	: _name(std::move(name))
{
	{
		auto event = naobi::event();
		event.setName("begin");
		addEvent(event);
	}
}

bool naobi::module::addModule(const naobi::module::sptr& newModule)
{
	if (getModule(newModule->name()) != nullptr) return false;
	_modules.emplace_back(newModule);
	return true;
}

bool naobi::module::addConst(const naobi::variable::sptr& newConst)
{
	if (getConst(newConst->name()) != nullptr) return false;
	_consts.emplace_back(newConst);
	return true;
}

bool naobi::module::addFunction(const naobi::function::sptr& newFunction)
{
	auto it = std::find_if(
		_functions.begin(), _functions.end(), [newFunction](function::sptr& function)
		{
			return newFunction->name() == function->name() &&
				   std::equal(
					   newFunction->getArguments().cbegin(), newFunction->getArguments().cend(),
					   function->getArguments().cbegin(),
					   [](const function::argument_type& first, const function::argument_type& second)
					   {
						   return first.second.name == second.second.name;
					   });
		});
	if (it != _functions.end()) return false;
	auto functions = findFunction(newFunction->name());
	newFunction->setNumber(functions.empty() ? 0 : functions.size());
	_functions.emplace_back(newFunction);
	return true;
}

std::vector<naobi::function::sptr> naobi::module::getFunction(const std::string& functionName)
{
	std::vector<naobi::function::sptr> buffer;
	for (const auto& function : _functions)
	{
		if (function->name() == functionName)
		{
			buffer.push_back(function);
		}
	}
	return buffer;
}

naobi::variable::sptr naobi::module::getConst(const std::string& constName)
{
	auto it = std::find_if(
		_consts.cbegin(), _consts.cend(), [constName](const naobi::variable::sptr& ptr)
		{
			return ptr->name() == constName;
		});
	if (it == _consts.cend()) return nullptr;
	return *it;
}

naobi::module::sptr naobi::module::getModule(const std::string& moduleName)
{
	auto it = std::find_if(
		_modules.cbegin(), _modules.cend(), [moduleName](const naobi::module::sptr& ptr)
		{
			return ptr->name() == moduleName;
		});
	if (it == _modules.cend()) return nullptr;
	return *it;
}

naobi::module::sptr naobi::module::findModule(const std::string& moduleName)
{
	auto module_ptr = getModule(moduleName);
	if (module_ptr != nullptr) return module_ptr;
	for (const auto& element : _modules)
	{
		auto ptr = element->findModule(moduleName);
		if (ptr != nullptr) return ptr;
	}
	return nullptr;
}

std::vector<naobi::function::sptr> naobi::module::findFunction(const std::string& functionName)
{
	auto functions = getFunction(functionName);
	for (const auto& element : _modules)
	{
		auto inner_functions = element->findFunction(functionName);
		functions.insert(functions.end(), inner_functions.begin(), inner_functions.end());
	}
	return functions;
}

naobi::function::sptr naobi::module::findFunctionWithNumber(const std::string& functionName, std::size_t num)
{
	auto functions = getFunction(functionName);
	for (const auto& element : _modules)
	{
		auto inner_functions = element->findFunction(functionName);
		functions.insert(functions.end(), inner_functions.begin(), inner_functions.end());
	}
	return *std::find_if(
		functions.begin(), functions.end(), [num](function::sptr& func)
		{
			return num == func->getNumber();
		});
}

bool naobi::module::addTemplateFunction(const naobi::template_function::sptr& newTemplate)
{
	auto it = std::find_if(
		_templateFunctions.begin(), _templateFunctions.end(),
		[name = newTemplate->getName()](const template_function::sptr& func)
		{
			return name == func->getName();
		});
	if (it != _templateFunctions.end())
	{
		return false;
	}
	_templateFunctions.push_back(newTemplate);
	return true;
}

naobi::template_function::sptr naobi::module::getTemplateFunction(const std::string& functionName)
{
	auto it = std::find_if(
		_templateFunctions.begin(), _templateFunctions.end(),
		[name = functionName](const template_function::sptr& func)
		{
			return name == func->getName();
		});
	if (it == _templateFunctions.end())
	{
		return nullptr;
	}
	return *it;
}

naobi::template_function::sptr naobi::module::findTemplateFunction(const std::string& functionName)
{
	auto temp = getTemplateFunction(functionName);
	if (temp != nullptr)
	{
		return temp;
	}
	for (const auto& element : _modules)
	{
		temp = element->findTemplateFunction(functionName);
		if (temp != nullptr) return temp;
	}
	return nullptr;
}

bool naobi::module::addException(const naobi::exception& exception)
{
	if (std::find_if(
		_exceptions.begin(), _exceptions.end(), [exception](const naobi::exception& excep)
		{
			return excep.name == exception.name;
		}) != _exceptions.end())
	{
		return false;
	}
	_exceptions.push_back(exception);
	return true;
}

std::optional<naobi::exception> naobi::module::getException(const std::string& exceptionName)
{
	auto it = std::find_if(
		_exceptions.begin(), _exceptions.end(), [exceptionName](const naobi::exception& excep)
		{
			return exceptionName == excep.name;
		});
	if (it == _exceptions.end()) return {};
	else return *it;
}

std::optional<naobi::exception> naobi::module::findException(const std::string& exceptionName)
{
	auto temp = getException(exceptionName);
	if (temp.has_value())
	{
		return temp;
	}
	for (const auto& element : _modules)
	{
		temp = element->findException(exceptionName);
		if (temp.has_value()) return temp;
	}
	return {};
}

bool naobi::module::addEvent(const naobi::event& event)
{
	if (std::find_if(
		_events.begin(), _events.end(), [event](const naobi::event& e)
		{
			return e.getName() == event.getName();
		}) != _events.end())
	{
		return false;
	}
	_events.push_back(event);
	return true;
}

std::optional<naobi::event> naobi::module::getEvent(const std::string& eventName)
{
	auto it = std::find_if(
		_events.begin(), _events.end(), [eventName](const naobi::event& excep)
		{
			return eventName == excep.getName();
		});
	if (it == _events.end()) return {};
	else return *it;
}

std::optional<naobi::event> naobi::module::findEvent(const std::string& eventName)
{
	auto temp = getEvent(eventName);
	if (temp.has_value())
	{
		return temp;
	}
	for (const auto& element : _modules)
	{
		temp = element->findEvent(eventName);
		if (temp.has_value()) return temp;
	}
	return {};
}

std::ostream& naobi::operator << (std::ostream& stream, const naobi::module& module)
{
	stream << "{\n";
	{
		naobi::indent_scope indent(stream);
		stream << "name: " << module.name() << "\n";
		if (!module._functions.empty())
		{
			stream << "functions: [\n";
			for (const auto& function : module._functions)
			{
				naobi::indent_scope inner_indent(stream);
				stream << *function << "\n";
			}
			stream << "]\n";
		}
		if (!module._events.empty())
		{
			stream << "events: [\n";
			for (const auto& event : module._events)
			{
				naobi::indent_scope inner_indent(stream);
				stream << event << "\n";
			}
			stream << "]\n";
		}
		if (!module._modules.empty())
		{
			stream << "modules: [\n";
			for (const auto& childModule : module._modules)
			{
				naobi::indent_scope inner_indent(stream);
				stream << *childModule << "\n";
			}
			stream << "]\n";
		}
	}

	stream << "}";

	return stream;
}

const std::ostream& naobi::operator >> (const std::ostream& stream, naobi::module& module)
{
	return stream;
}
