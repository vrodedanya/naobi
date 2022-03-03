#ifndef NAOBI_CODE_GENERATOR_HPP
#define NAOBI_CODE_GENERATOR_HPP

#include <vector>
#include <string>
#include <map>
#include <memory>
#include <functional>

namespace naobi
{
	struct workflow_context;
	using commandImpl = std::function<void(std::shared_ptr<workflow_context> ptr)>;

	class code_generator
	{
	public:
		enum class commandName
		{

		};
	public:
		std::vector<commandImpl> generate(const std::vector<std::string>& line);

	private:
		static inline std::map<commandName, commandImpl> _commands;
	};
}

#endif //NAOBI_CODE_GENERATOR_HPP
