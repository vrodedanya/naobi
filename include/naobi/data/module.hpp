#ifndef NAOBI_MODULE_HPP
#define NAOBI_MODULE_HPP

#include <utility>
#include <vector>
#include <string>
#include <memory>
#include <optional>

#include <naobi/data/function.hpp>
#include <naobi/data/variable.hpp>

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
		bool addConst(const naobi::variable::sptr& newConst);
		bool addModule(const naobi::module::sptr& newModule);

		naobi::function::sptr getFunction(const std::string& functionName);
		naobi::variable::sptr getConst(const std::string& constName);
		naobi::module::sptr getModule(const std::string& moduleName);

		[[nodiscard]] std::string name() const {return _name;}


	private:
		std::string _name;
		std::vector<naobi::function::sptr> _functions;
		std::vector<naobi::variable::sptr> _consts;
		std::vector<naobi::module::sptr> _modules;
	};
}

#endif //NAOBI_MODULE_HPP
