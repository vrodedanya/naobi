#ifndef NAOBI_EVENT_POOL_HPP
#define NAOBI_EVENT_POOL_HPP

#include <queue>
#include <mutex>

#include <naobi/data/event.hpp>


namespace naobi
{
	class event_pool
	{
	public:
		bool isEmpty()
		{
			std::lock_guard<std::mutex> guard(_mutex);
			return _events.empty();
		}

		bool isNotEmpty()
		{ return !isEmpty(); }

		void push(const naobi::event& event);

		naobi::event pop();

	private:
		std::queue<naobi::event> _events;
		std::mutex _mutex;
	};
}

#endif //NAOBI_EVENT_POOL_HPP
