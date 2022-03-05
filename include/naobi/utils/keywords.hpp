#ifndef NAOBI_KEYWORDS_HPP
#define NAOBI_KEYWORDS_HPP

#include <string>
#include <vector>

namespace naobi
{
	class keywords
	{
	public:
		static inline bool check(const std::string& word);
		static inline bool checkIsType(const std::string& word);
	private:
		static inline std::vector<std::string> _keywords = {
			"workflow",
			"target",
			"invoke",
			"function",
			"const",
		};
		static inline std::vector<std::string> _types = {
			"integer",
		};
	};

	bool keywords::checkIsType(const std::string &word)
	{
		return std::find(_types.begin(), _types.end(), word) != _types.end();
	}

	bool keywords::check(const std::string &word)
	{
		return std::find(_keywords.begin(), _keywords.end(), word) != _keywords.end();
	}
}

#endif //NAOBI_KEYWORDS_HPP
