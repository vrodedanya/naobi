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
		return newFunction->name() == function->name() && std::equal(newFunction->getArguments().cbegin(),newFunction->getArguments().cend(),
																	 function->getArguments().cbegin());
	});
	if (it != _functions.end()) return false;
	_functions.emplace_back(newFunction);
	return true;
}

naobi::function::sptr naobi::module::getFunction(const std::string& functionName)
{
	auto it = std::find_if(_functions.cbegin(), _functions.cend(), [functionName](const naobi::function::sptr& ptr)
	{
		return ptr->name() == functionName;
	});
	if (it == _functions.cend()) return nullptr;
	return *it;
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

naobi::module::sptr	 naobi::module::getModule(const std::string& moduleName)
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

naobi::function::sptr naobi::module::findFunction(const std::string &functionName)
{
	auto function_ptr = getFunction(functionName);
	if (function_ptr != nullptr) return function_ptr;
	for (const auto& element : _modules)
	{
		auto ptr = element->findFunction(functionName);
		if (ptr != nullptr) return ptr;
	}
	return nullptr;
}
