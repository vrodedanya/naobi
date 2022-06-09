#ifndef NAOBI_COMMAND_HPP
#define NAOBI_COMMAND_HPP

#include <functional>
#include <map>
#include <memory>


namespace naobi
{
	struct workflow_context;

	struct command
	{
		using argumentsList = std::vector<std::string>;

		using implementation = std::function<void(
			std::shared_ptr<workflow_context> ptr,
			const argumentsList& arguments)>;

		enum class names
		{
			// CommandName <-> argumentsCount
			NEW, // 2 - name, type
			INC,
			DEC,
			ADD, // 0
			SUB,
			MUL,
			DIV,
			NEG,
			MOD,
			EQ,
			GREATER,
			LESS,
			GREATER_OR_EQ,
			LESS_OR_EQ,
			NOT_EQ,
			ALLOCATE,
			LOAD, // 1 - number
			SAVE, // 1 - number
			TRANSFER, // 1 - name
			PLACE, // 1 - value, type
			PRINTLN,
			PRINT,
			INPUT,
			RETURN,
			CALL, // function name
			JUMP,
			JUMP_IF,
			EXIT,
			NOPE,
			ARISE,
			I2S,
			F2S,
			B2S,
			S2I,
			F2I,
			B2I,
			I2F,
			S2F,
			I2B,
			S2B,
			CATCH,
			THROW,
		};

		static naobi::command createCommand(command::names com, const command::argumentsList& arguments);

		names name;
		implementation impl;
		argumentsList arguments;

		static std::map<command::names, implementation> commands;
		static std::map<std::string, command::names> stringToCommand;
	};
}

#endif //NAOBI_COMMAND_HPP
