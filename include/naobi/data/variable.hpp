#ifndef NAOBI_VARIABLE_HPP
#define NAOBI_VARIABLE_HPP

#include <string>
#include <utility>
#include <memory>

namespace naobi
{
	class variable
	{
	public:
		using uptr = std::unique_ptr<variable>;
		using sptr = std::shared_ptr<variable>;
	public:
		explicit variable(std::string name);

		[[nodiscard]] std::string name() const {return _name;}

	private:
		std::string _name;
	};
}

#endif //NAOBI_VARIABLE_HPP
