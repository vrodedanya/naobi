#ifndef NAOBI_HANDLER_HPP
#define NAOBI_HANDLER_HPP

#include <list>
#include <memory>
#include <mutex>
#include <queue>

#include <naobi/interpreter/workflow_context.hpp>


namespace naobi
{
	class event_manager;

	class handler
	{
	public:
		using sptr = std::shared_ptr<handler>;

		explicit handler(event_manager* eventManager);


		const int MAX_TIME_PER_CONTEXT = 5;

		void pushContext(const naobi::workflow_context::sptr& newContext);

		void execute();

		[[nodiscard]] std::size_t contextsCount() const {return _contexts.size() + _bufferContexts.size();}

		[[nodiscard]] bool isWork() const;

		void setIsWork(bool isWork);

	private:
		void pullContexts();
		void catchException(const naobi::exception& exception, naobi::workflow_context::sptr& context);

	private:
		std::list<naobi::workflow_context::sptr> _contexts;
		std::queue<naobi::workflow_context::sptr> _bufferContexts;
		std::mutex _mutex;
		event_manager* _eventManager;
		bool _isWork{true};
	};
}

#endif //NAOBI_HANDLER_HPP
