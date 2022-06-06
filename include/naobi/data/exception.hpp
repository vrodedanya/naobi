#ifndef NAOBI_EXCEPTION_HPP
#define NAOBI_EXCEPTION_HPP

#include <string>
#include <memory>


namespace naobi
{
	struct exception
	{
		exception() = default;

		exception(const std::string& _name, const std::string& _description) : name(_name), description(_description)
		{}

		std::string name;
		std::string description;
	};
}

#endif //NAOBI_EXCEPTION_HPP
