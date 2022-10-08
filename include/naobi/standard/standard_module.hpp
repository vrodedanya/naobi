#ifndef NAOBI_STANDARD_MODULE_HPP
#define NAOBI_STANDARD_MODULE_HPP

#include <naobi/data/module.hpp>


namespace naobi
{
	class standard :
		public naobi::module,
		public std::enable_shared_from_this<standard>
	{
	public:
		standard();
	};
}

#endif //NAOBI_STANDARD_MODULE_HPP
