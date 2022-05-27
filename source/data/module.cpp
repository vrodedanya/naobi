#include <naobi/data/module.hpp>

#include <algorithm>

naobi::module::module(std::string name)
		: _name(std::move(name))
{

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
	auto it = std::find_if(_functions.begin(), _functions.end(), [newFunction](function::sptr& function)
	{
		return newFunction->name() == function->name() &&
			   std::equal(newFunction->getArguments().cbegin(), newFunction->getArguments().cend(),
						  function->getArguments().cbegin(),
						  [](const function::argument_type& first, const function::argument_type& second)
						  {
							  return first.second == second.second;
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
	auto it = std::find_if(_consts.cbegin(), _consts.cend(), [constName](const naobi::variable::sptr& ptr)
	{
		return ptr->name() == constName;
	});
	if (it == _consts.cend()) return nullptr;
	return *it;
}

naobi::module::sptr naobi::module::getModule(const std::string& moduleName)
{
	auto it = std::find_if(_modules.cbegin(), _modules.cend(), [moduleName](const naobi::module::sptr& ptr)
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
	return *std::find_if(functions.begin(), functions.end(), [num](function::sptr& func)
	{
		return num == func->getNumber();
	});
}
