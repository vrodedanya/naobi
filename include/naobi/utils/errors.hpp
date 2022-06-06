#ifndef NAOBI_ERRORS_HPP
#define NAOBI_ERRORS_HPP

namespace naobi
{
	enum errors
	{
// Importing modules 10-19
		FAILED_TO_OPEN_FILE = 10,
		IMPORT_ITSELF = 11,
// Unit compile error 30-39
		UNIT_ALREADY_EXIST = 30,
// Code compile error 50-59
		UNKNOWN_LINE = 50,
		ALREADY_EXIST = 51,
		DOESNT_EXIST = 52,
		TYPE_ERROR = 53,
		WRONG_FORMAT = 54,
		UNKNOWN_OPERATOR = 55,
		INCORRECT_BRACKETS = 56,
		INVALID_ARGUMENT = 57,
// Runtime errors 90-99
		EXCEPTION = 90,
// Other 110-119
		NOT_SPECIFIED = 110,
		KEYWORD_AS_NAME = 111,
		INTERNAL_ERROR = 112,
	};

	class naobi_exception : public std::exception
	{
	public:
		naobi_exception(const std::string& _name, const std::string& _description) :
		name(_name),
		description(_description)
			{}

		std::string name;
		std::string description;

		[[nodiscard]] const char* what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW override
		{
			return description.c_str();
		}
	};
}

#endif //NAOBI_ERRORS_HPP
