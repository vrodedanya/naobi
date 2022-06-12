#include <naobi/interpreter/event_pool.hpp>

void naobi::event_pool::push(const naobi::event& event)
{
	std::lock_guard<std::mutex> guard(_mutex);
	_events.push(event);
}

naobi::event naobi::event_pool::pop()
{
	std::lock_guard<std::mutex> guard(_mutex);
	auto temp = std::move(_events.front());
	_events.pop();
	return temp;
}
