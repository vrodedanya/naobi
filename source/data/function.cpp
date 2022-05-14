#include <naobi/data/function.hpp>
#include <naobi/compiler/code_generator.hpp>

naobi::function::function(std::string name) :
	_name(std::move(name))
{
}

void naobi::function::setCommands(const std::vector<naobi::command> &commands)
{
	_commands.clear();
	_commands.emplace_back(code_generator::createCommand(command::names::NOPE,{}));
	_commands.insert(_commands.end(), commands.begin(), commands.end());
	_commands.emplace_back(code_generator::createCommand(command::names::RETURN,{}));
}
