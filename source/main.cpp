#include <naobi/compiler/compiler.hpp>
#include <naobi/utils/logger.hpp>

int main(int argc, char* argv[])
{
	if (argc == 2)
	{
		naobi::logger::enable();
		naobi::logger::enableLoggingToStdOut();
		naobi::logger::setLevel(naobi::logger::SUCCESS);
		naobi::logger::setLoggingFile("log.txt");
		naobi::logger::enableDate();
		naobi::logger::enableLevel();
		LOG(main, naobi::logger::LOW, "begin main");

		naobi::compiler compiler;
		compiler.compile(argv[1]);
		auto composition = compiler.getComposition();
		std::cout<<std::endl;
	}
	return EXIT_SUCCESS;
}