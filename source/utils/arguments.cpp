#include <naobi/utils/arguments.hpp>

#include <algorithm>


naobi::arguments::arguments(char** begin, char** end) :
	_begin(begin),
	_end(end)
{

}

bool naobi::arguments::find_flag(const std::string& name) const
{
	return (std::find(_begin, _end, name) != _end);
}

std::optional<int> naobi::arguments::find_int(const std::string& name) const
{
	auto it = std::find(_begin, _end, name);
	if (it == _end || it + 1 == _end) return {};
	return std::stoi(std::string(*(it + 1)));
}

std::optional<std::string> naobi::arguments::find_str(const std::string& name) const
{
	auto it = std::find(_begin, _end, name);
	if (it == _end || it + 1 == _end) return {};
	return *(it + 1);
}

