#include <naobi/compiler/compiler.hpp>

int main(int argc, char* argv[])
{
	if (argc == 2)
	{
		auto compositionOpt = naobi::compiler::compile(argv[1]);
		if (compositionOpt.has_value())
		{

		}
	}
	return EXIT_SUCCESS;
}