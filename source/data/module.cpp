#include <naobi/data/module.hpp>

#include <optional>

std::optional<std::string> naobi::module::addModule(naobi::module *newModule)
{
	// TODO check module for existing
	_modules.push_back(newModule);
	return {};
}

std::optional<std::string> naobi::module::addConst(naobi::variable newConst)
{
	_consts.push_back(newConst);
	return {};
}

std::optional<std::string> naobi::module::addFunction(naobi::function newFunction)
{
	_functions.push_back(newFunction);
	return {};
}
