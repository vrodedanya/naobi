#include <utility>
#include <naobi/data/workflow.hpp>
#include "naobi/utils/indent_scope.hpp"


naobi::workflow::workflow(std::string name, naobi::event target, module::sptr parentModule) :
	_name(std::move(name)),
	_target(std::move(target)),
	_parentModule(std::move(parentModule))
{
}

std::ostream& naobi::operator << (std::ostream& stream, const naobi::workflow& flow)
{
	stream << "{\n";
	{
		naobi::indent_scope indent{ stream };
		std::size_t index{};
		stream << "name: " << flow.name() << "\n";
		stream << "target: " << flow.target().getName() << "\n";
		stream << "invoke: " << flow.invoke() << "\n";
		stream << "commands: [\n";

		for (const auto& command : flow._commands)
		{
			naobi::indent_scope inner_scope{ stream };

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
const std::ostream& naobi::operator >> (const std::ostream& stream, naobi::workflow& flow)
{
	return stream;
}