#ifndef NAOBI_KEYWORDS_HPP
#define NAOBI_KEYWORDS_HPP

#include <string>
#include <vector>

namespace naobi
{
	class keywords
	{
	public:
		static bool check(const std::string& word);
	private:
		static inline std::vector<std::string> _keywords = {
			"workflow",
			"target",
			"invoke",
			"function",
			"const",
		};
	};

	bool keywords::check(const std::string &word)
	{
		return std::find(_keywords.begin(), _keywords.end(), word) != _keywords.end();
	}
}

#endif //NAOBI_KEYWORDS_HPP
