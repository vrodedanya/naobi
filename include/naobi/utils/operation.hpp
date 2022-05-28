#ifndef NAOBI_OPERATION_HPP
#define NAOBI_OPERATION_HPP

#include <string>
#include <map>
#include <functional>
#include <memory>

#include <naobi/utils/type.hpp>
#include <naobi/data/variable.hpp>
#include <naobi/interpreter/command.hpp>


namespace naobi
{
	class operation
	{
	public:
		using sptr = std::shared_ptr<operation>;
		using uptr = std::unique_ptr<operation>;
		using key = std::pair<utils::type::names, utils::type::names>;
		using impl = std::pair<utils::type::names, std::function<variable::sptr(variable::sptr, variable::sptr)>>;
		using implementations = std::map<key, impl>;

		operation(
			std::string operatorName, int priority, naobi::command::names commandAnalogue,
			implementations implement);

		[[nodiscard]] const std::string& getOperator() const;

		void setOperator(const std::string& operatorName);

		[[nodiscard]] int getPriority() const;

		void setPriority(int priority);

		impl call(utils::type::names first, utils::type::names second);

		[[nodiscard]] command::names getCommandAnalogue() const;

		void setCommandAnalogue(command::names commandAnalogue);

		bool operator <(const operation& rhs) const;

		bool operator >(const operation& rhs) const;

		bool operator <=(const operation& rhs) const;

		bool operator >=(const operation& rhs) const;

	private:
		std::string _operator;
		int _priority{};
		naobi::command::names _commandAnalogue;
		implementations _implementations;
	};

	class operation_manager
	{
	public:
		static operation::sptr get(const std::string& operation);

	private:
		static std::vector<operation::sptr> _operations;
	};
}

#endif //NAOBI_OPERATION_HPP
