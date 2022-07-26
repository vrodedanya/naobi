#ifndef NAOBI_COLORS_HPP
#define NAOBI_COLORS_HPP


namespace naobi::colors
{
	using color = const char*;

	const color RESET = "\033[0m";
	const color BLACK = "\033[30m";
	const color RED = "\033[31m";
	const color GREEN = "\033[32m";
	const color YELLOW = "\033[33m";
	const color BLUE = "\033[34m";
	const color MAGENTA = "\033[35m";
	const color CYAN = "\033[36m";
	const color WHITE = "\033[37m";
	const color BOLDBLACK = "\033[1m\033[30m";
	const color BOLDRED = "\033[1m\033[31m";
	const color BOLDGREEN = "\033[1m\033[32m";
	const color BOLDYELLOW = "\033[1m\033[33m";
	const color BOLDBLUE = "\033[1m\033[34m";
	const color BOLDMAGENTA = "\033[1m\033[35m";
	const color BOLDCYAN = "\033[1m\033[36m";
	const color BOLDWHITE = "\033[1m\033[37m";
}

#endif //NAOBI_COLORS_HPP
