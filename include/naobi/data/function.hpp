#ifndef NAOBI_FUNCTION_HPP
#define NAOBI_FUNCTION_HPP

#include <string>
#include <memory>

namespace naobi
{
	class function
	{
	public:
		using uptr = std::unique_ptr<function>;
		using sptr = std::shared_ptr<function>;

	public:
		explicit function(std::string name);

		[[nodiscard]] std::string name() const {return _name;}

	private:
		std::string _name;
	};
}

#endif //NAOBI_FUNCTION_HPP
