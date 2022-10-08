#include <naobi/data/function.hpp>

#include <optional>
#include "naobi/utils/indent_scope.hpp"


naobi::function::function(std::string name) :
	_name(std::move(name))
{
}

void naobi::function::setCommands(const std::vector<naobi::command>& commands)
{
	_commands.clear();
	_commands.emplace_back(command::createCommand(command::names::NOPE, {}));
	_commands.insert(_commands.end(), commands.begin(), commands.end());
	_commands.emplace_back(command::createCommand(command::names::RETURN, {}));
}

bool naobi::function::addArgument(const std::string& name, const utils::type::type& type)
{
	auto it = std::find_if(
		_arguments.begin(), _arguments.end(), [name](const auto& pair)
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
	auto it = std::find_if(
		_arguments.begin(), _arguments.end(), [name](const auto& pair)
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

const std::vector<naobi::function::argument_type>& naobi::function::getArguments() const
{
	return _arguments;
}

const naobi::utils::type::type& naobi::function::getReturnType() const
{
	return _returnType;
}

void naobi::function::setReturnType(const naobi::utils::type::type& returnType)
{
	_returnType = returnType;
}

std::size_t naobi::function::getNumber() const
{
	return _number;
}

void naobi::function::setNumber(std::size_t n)
{
	function::_number = n;
}

void naobi::function::setArguments(const std::vector<argument_type>& arguments)
{
	_arguments = arguments;
}

std::ostream& naobi::operator <<(std::ostream& stream, const naobi::function& function)
{
	// comment block
	stream << "{\n";
	{
		naobi::indent_scope indent(stream);
		stream << "name: " << function.name() << "\n";
		stream << "return: " << utils::type::fromNameToString(function._returnType.name) << "\n";
		stream << "index: " << function._number <<  "\n";

		std::size_t index{};
		stream << "commands: [\n";
		for (const auto& command : function._commands)
		{
			naobi::indent_scope inner_indent(stream);
			stream << command::commandAsString(command.name) << ' ';
			index = 0;
			for (const auto& arg : command.arguments)
			{
				stream << arg;
				if (++index < command.arguments.size())
				{
					stream << ' ';
				}
			}
			stream << "\n";
		}
		stream << "]\n";
	}
	stream << "}";
	return stream;
}

const std::ostream& naobi::operator >>(const std::ostream& stream, naobi::function& function)
{
	return stream;
}
