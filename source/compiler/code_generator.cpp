#include <naobi/compiler/code_generator.hpp>

#include <naobi/utils/logger.hpp>

std::vector<naobi::commandImpl> naobi::code_generator::generate(const std::vector<std::string> &line)
{
	LOG(code_generator, naobi::logger::BASIC, "Code lines:\n", line);
	return {};
}
