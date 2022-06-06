#ifndef NAOBI_EVENT_HPP
#define NAOBI_EVENT_HPP

#include <string>
#include <vector>
#include <optional>

#include <naobi/utils/type.hpp>
#include <naobi/data/variable.hpp>


namespace naobi
{
	class event
	{
	public:
		using argument = std::tuple<std::string, utils::type::names, naobi::variable::sptr>;

		[[nodiscard]] const std::string& getName() const;

		void setName(const std::string& name);

		[[nodiscard]] std::vector<argument>& getArguments();

		void setArguments(const std::vector<argument>& arguments);

		void setArgument(const std::string& name, const naobi::variable::sptr& variable);

		void setArgument(std::size_t pos, const naobi::variable::sptr& variable);

		bool addArgument(const std::string& name, utils::type::names type, const naobi::variable::sptr& pointer);

		std::optional<argument> getArgument(const std::string& name);

		std::optional<argument> getArgument(std::size_t id);

		bool operator ==(const event& rhs) const;

		bool operator !=(const event& rhs) const;

		bool operator <(const event& rhs) const;

		bool operator >(const event& rhs) const;

		bool operator <=(const event& rhs) const;

		bool operator >=(const event& rhs) const;

	private:
		std::string _name;
		std::vector<argument> _arguments;
	};
}

#endif //NAOBI_EVENT_HPP
