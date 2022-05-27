#ifndef NAOBI_TEMPLATE_FUNCTION_HPP
#define NAOBI_TEMPLATE_FUNCTION_HPP

#include <string>
#include <vector>
#include <memory>
#include <optional>


namespace naobi
{
	class template_function
	{
	public:
		using uptr = std::unique_ptr<template_function>;
		using sptr = std::shared_ptr<template_function>;

		using argument_type = std::pair<std::string, std::string>; // name - type

	public:
		explicit template_function(std::string name);

		[[nodiscard]] const std::string& getName() const;

		void setName(const std::string& name);

		void addArgument(const std::string& name, const std::string& type);

		[[nodiscard]] const std::vector<argument_type>& getArguments() const;

		void setArguments(const std::vector<argument_type>& arguments);

		std::optional<argument_type> getArgument(const std::string& name);

		std::optional<argument_type> getArgument(std::size_t pos);

		std::size_t getPosOfArgument(const std::string& name);

		[[nodiscard]] const std::string& getReturnType() const;

		void setReturnType(const std::string& returnType);

		[[nodiscard]] const std::string& getCode() const;

		void setCode(const std::string& code);

	private:
		std::string _name;
		std::vector<argument_type> _arguments;
		std::string _returnType{"undefined"};
		std::string _code;
	};
}

#endif //NAOBI_TEMPLATE_FUNCTION_HPP
