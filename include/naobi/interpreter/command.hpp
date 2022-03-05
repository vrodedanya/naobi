#ifndef NAOBI_COMMAND_HPP
#define NAOBI_COMMAND_HPP

namespace naobi
{
	struct workflow_context;

	struct command
	{
		using argumentsList = std::vector<std::string>;
		using implementation = std::function<void(std::shared_ptr<workflow_context> ptr, const argumentsList& arguments)>;
		enum class names
		{
		// CommandName <-> argumentsCount
			NEW, // 2 - name, type
			ADD, // 0
			LOAD, // 1 - number
			SAVE, // 1 - number
			PLACE, // 1 - value, type
		};

		names name;
		implementation impl;
		argumentsList arguments;
	};
}

#endif //NAOBI_COMMAND_HPP
