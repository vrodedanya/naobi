#ifndef NAOBI_OUTPUT_TYPE_TRAIT_HPP
#define NAOBI_OUTPUT_TYPE_TRAIT_HPP

#include <vector>
#include <ostream>

namespace naobi
{
	template <typename T>
	std::ostream& operator << (std::ostream& os, const std::vector<T>& vec);


	template <typename T>
	std::ostream& operator << (std::ostream& os, const std::vector<T>& vec)
	{
		os << "[";
		for (auto it = vec.cbegin(); it != vec.cend(); it++)
		{
			os << *it;
			if (it != (vec.cend() - 1)) os << "|";
		}
		os << "]";
		return os;
	}
}

#endif //NAOBI_OUTPUT_TYPE_TRAIT_HPP
