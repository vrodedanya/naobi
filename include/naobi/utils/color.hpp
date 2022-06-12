#ifndef NAOBI_COLOR_HPP
#define NAOBI_COLOR_HPP

#include <iostream>


namespace naobi
{
	class color
	{
	public:
		const char* color;

		static inline const char* RESET = "\033[0m";
		static inline const char* BLACK = "\033[30m";
		static inline const char* RED = "\033[31m";
		static inline const char* GREEN = "\033[32m";
		static inline const char* YELLOW = "\033[33m";
		static inline const char* BLUE = "\033[34m";
		static inline const char* MAGENTA = "\033[35m";
		static inline const char* CYAN = "\033[36m";
		static inline const char* WHITE = "\033[37m";
		static inline const char* BOLDBLACK = "\033[1m\033[30m";
		static inline const char* BOLDRED = "\033[1m\033[31m";
		static inline const char* BOLDGREEN = "\033[1m\033[32m";
		static inline const char* BOLDYELLOW = "\033[1m\033[33m";
		static inline const char* BOLDBLUE = "\033[1m\033[34m";
		static inline const char* BOLDMAGENTA = "\033[1m\033[35m";
		static inline const char* BOLDCYAN = "\033[1m\033[36m";
		static inline const char* BOLDWHITE = "\033[1m\033[37m";
	};
}

#endif //NAOBI_COLOR_HPP
