#include <naobi/compiler/compiler.hpp>
#include <naobi/utils/logger.hpp>
#include <naobi/utils/arguments.hpp>
#include <naobi/interpreter/handler.hpp>
#include <naobi/interpreter/event_manager.hpp>

int main(int argc, char* argv[])
{
	if (argc > 1)
	{
		naobi::arguments arguments(argv, argv + argc);
		naobi::logger::enable();
		naobi::logger::enableLoggingToStdErr();
		auto tempLevel = arguments.find_int("--level");
		if (tempLevel.has_value()) naobi::logger::setLevel(tempLevel.value());
		else naobi::logger::setLevel(naobi::logger::CRITICAL);
		auto tempOutput = arguments.find_str("--logOutput");
		if (tempOutput.has_value()) naobi::logger::setLoggingFile(tempOutput.value());
		if (arguments.find_flag("--enable-date"))
			naobi::logger::enableDate();
		if (arguments.find_flag("--enable-level"))
			naobi::logger::enableLevel();


		naobi::compiler compiler;
		compiler.compile(argv[argc - 1]);
		
		naobi::event_manager::pushEvent("begin");

		naobi::handler handler;

		handler.execute();
	}
	return EXIT_SUCCESS;
}