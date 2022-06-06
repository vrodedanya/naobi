#include <naobi/compiler/compiler.hpp>
#include <naobi/utils/logger.hpp>
#include <naobi/utils/arguments.hpp>
#include <naobi/interpreter/handler.hpp>


int main(int argc, char* argv[])
{
	if (argc > 1)
	{
		naobi::arguments arguments(argv, argv + argc);
		naobi::logger::enable();
		naobi::logger::enableLoggingToStdErr();
		if (arguments.find_flag("--version") || arguments.find_flag("-v"))
		{
			std::cout << "Naobi language\n"
						 "Version " << VERSION_MAJOR << "." << VERSION_MINOR << "." << VERSION_PATCH << std::endl;
			return EXIT_SUCCESS;
		}

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

		auto script = arguments.find_str("--script");
		if (script.has_value())
		{
			compiler.compileText(script.value());
		}
		else
		{
			compiler.compile(argv[argc - 1]);
		}
		if (arguments.find_flag("--print-compile-end"))
		{
			std::cerr << "compile_end" << std::endl;
		}

		naobi::handler handler;
		naobi::event beginEvent;
		beginEvent.setName("begin");
		handler.eventManager().pushEvent(beginEvent);
		for (const auto& workflow : compiler.getWorkflows())
		{
			handler.eventManager().addWorkflow(workflow->target(), workflow);
		}

		try
		{
			handler.execute();
		}
		catch (const std::exception& exception)
		{
			NCRITICAL(handler.execute, naobi::errors::NOT_SPECIFIED, "CRITICAL got exception in runtime: ",
					  exception.what());
			return EXIT_FAILURE;
		}
	}
	return EXIT_SUCCESS;
}