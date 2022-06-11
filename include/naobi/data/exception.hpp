#ifndef NAOBI_EXCEPTION_HPP
#define NAOBI_EXCEPTION_HPP

#include <string>
#include <memory>
#include <utility>


namespace naobi
{
	struct exception
	{
		exception() = default;

		exception(std::string _name, std::string _description) :
			name(std::move(_name)), description(std::move(_description))
		{}

		std::string name;
		std::string description;
	};
}

#endif //NAOBI_EXCEPTION_HPP
