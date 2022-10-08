#ifndef NAOBI_MODULE_HPP
#define NAOBI_MODULE_HPP

#include <vector>
#include <string>
#include <memory>
#include <optional>

#include <naobi/data/function.hpp>
#include <naobi/data/template_function.hpp>
#include <naobi/data/variable.hpp>
#include <naobi/data/exception.hpp>
#include <naobi/data/event.hpp>


namespace naobi
{
	class module
	{
	public:
		using uptr = std::unique_ptr<module>;
		using sptr = std::shared_ptr<module>;
	public:
		explicit module(std::string name);

		bool addFunction(const naobi::function::sptr& newFunction);

		bool addTemplateFunction(const naobi::template_function::sptr& newTemplate);

		bool addConst(const naobi::variable::sptr& newConst);

		bool addModule(const naobi::module::sptr& newModule);

		bool addException(const naobi::exception& exception);

		bool addEvent(const naobi::event& event);

		std::vector<naobi::function::sptr> getFunction(const std::string& functionName);

		naobi::template_function::sptr getTemplateFunction(const std::string& functionName);

		naobi::variable::sptr getConst(const std::string& constName);

		naobi::module::sptr getModule(const std::string& moduleName);

		std::optional<naobi::exception> getException(const std::string& exceptionName);

		std::optional<naobi::event> getEvent(const std::string& eventName);

		naobi::module::sptr findModule(const std::string& moduleName);

		std::vector<naobi::function::sptr> findFunction(const std::string& functionName);

		naobi::function::sptr findFunctionWithNumber(const std::string& functionName, std::size_t num);

		naobi::template_function::sptr findTemplateFunction(const std::string& functionName);

		std::optional<naobi::exception> findException(const std::string& exceptionName);

		std::optional<naobi::event> findEvent(const std::string& eventName);

		[[nodiscard]] std::string name() const
		{ return _name; }

		friend std::ostream& operator << (std::ostream& stream, const module& module);
		friend const std::ostream& operator >> (const std::ostream& stream, module& module);

	private:
		std::string _name;
		std::vector<naobi::function::sptr> _functions;
		std::vector<naobi::template_function::sptr> _templateFunctions;
		std::vector<naobi::variable::sptr> _consts;
		std::vector<naobi::module::sptr> _modules;
		std::vector<naobi::exception> _exceptions;
		std::vector<naobi::event> _events;
	};

	std::ostream& operator << (std::ostream& stream, const module& module);
	const std::ostream& operator >> (const std::ostream& stream, module& module);
}

#endif //NAOBI_MODULE_HPP
