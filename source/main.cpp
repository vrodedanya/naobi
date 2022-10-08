#include <naobi/compiler/compiler.hpp>
#include <naobi/utils/logger.hpp>
#include <naobi/utils/arguments.hpp>
#include <naobi/interpreter/event_manager.hpp>


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

		bool isOnlyCompile = arguments.find_flag("--compile");
		bool isOnlyExecute = arguments.find_flag("--execute");

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

		if (isOnlyCompile)
		{
			std::ofstream file(argv[argc - 1] + std::string(".byte"), std::ios::out);
			file << compiler;
			file.close();
			return EXIT_SUCCESS;
		}

		naobi::event beginEvent;
		beginEvent.setName("begin");
		naobi::event_manager eventManager;
		for (const auto& workflow : compiler.getWorkflows())
		{
			eventManager.addWorkflow(workflow->target(), workflow);
		}
		eventManager.eventPool().push(beginEvent);
		try
		{
			eventManager.execute();
		}
		catch (const std::exception& exception)
		{
			NCRITICAL(handler.execute, naobi::errors::EXCEPTION, "CRITICAL got exception in runtime: ",
					  exception.what());
			return EXIT_FAILURE;
		}
	}
	return EXIT_SUCCESS;
}