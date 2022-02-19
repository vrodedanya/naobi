#ifndef NAOBI_MODULE_HPP
#define NAOBI_MODULE_HPP

#include <optional>
#include <utility>
#include <vector>
#include <string>
#include <memory>

#include <naobi/data/function.hpp>
#include <naobi/data/variable.hpp>

namespace naobi
{
	class module
	{
	public:
		explicit module(std::string name) : _name(std::move(name))
		{
		}
		std::optional<std::string> addFunction(naobi::function newFunction);
		std::optional<std::string> addConst(naobi::variable newConst);
		std::optional<std::string> addModule(naobi::module* newModule);


	private:
		std::string _name;
		std::vector<naobi::function> _functions;
		std::vector<naobi::variable> _consts;
		std::vector<module*> _modules;
	};
}

#endif //NAOBI_MODULE_HPP
