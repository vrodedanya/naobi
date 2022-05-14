#include <naobi/data/function.hpp>
#include <naobi/compiler/code_generator.hpp>
#include <optional>

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

bool naobi::function::addArgument(const std::string& name, utils::type::names type)
{
	auto it = std::find_if(_arguments.begin(), _arguments.end(), [name](const auto& pair)
	{
		return name == pair.first;
	});
	if (it != _arguments.cend())
	{
		return false;
	}
	_arguments.emplace_back(name, type);
	return true;
}

std::optional<naobi::function::argument_type> naobi::function::getArgument(const std::string& name)
{
	auto it = std::find_if(_arguments.begin(), _arguments.end(), [name](const auto& pair)
	{
		return name == pair.first;
	});
	if (it == _arguments.end()) return {};
	else return *it;
}

std::optional<naobi::function::argument_type> naobi::function::getArgument(std::size_t pos)
{
	if (pos >= _arguments.size()) return {};
	return _arguments[pos];
}

const std::vector<naobi::function::argument_type> &naobi::function::getArguments() const
{
	return _arguments;
}
