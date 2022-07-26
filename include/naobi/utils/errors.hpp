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
		naobi_exception(std::string name, std::string description) :
			_name(std::move(name)),
			_description(std::move(description))
		{}

		[[nodiscard]] const std::string& getName() const
		{
			return _name;
		}

		[[nodiscard]] const std::string& getDescription() const
		{
			return _description;
		}

		[[nodiscard]] const char* what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW override
		{
			return _description.c_str();
		}

	private:
		std::string _name;
		std::string _description;
	};
}

#endif //NAOBI_ERRORS_HPP
