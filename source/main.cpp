#include <naobi/compiler/compiler.hpp>
#include <naobi/utils/logger.hpp>

int main(int argc, char* argv[])
{
	if (argc == 2)
	{
		naobi::logger::enable();
		naobi::logger::enableLoggingToStdOut();
		naobi::logger::setLevel(1);
		naobi::logger::setLoggingFile("log.txt");
		LOG(main, 5, "begin main");

		auto compositionOpt = naobi::compiler::compile(argv[1]);
		if (compositionOpt.has_value())
		{

		}
	}
	return EXIT_SUCCESS;
}