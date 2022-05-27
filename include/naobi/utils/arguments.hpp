#ifndef NAOBI_ARGUMENTS_HPP
#define NAOBI_ARGUMENTS_HPP

#include <string>
#include <optional>

namespace naobi
{
	class arguments
	{
	public:
		arguments(char** begin, char** end);

		[[nodiscard]] bool find_flag(const std::string& name) const;

		[[nodiscard]] std::optional<int> find_int(const std::string& name) const;

		[[nodiscard]] std::optional<std::string> find_str(const std::string& name) const;

	private:
		char** _begin;
		char** _end;
	};
}

#endif //NAOBI_ARGUMENTS_HPP
