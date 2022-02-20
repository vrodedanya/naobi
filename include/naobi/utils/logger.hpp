#ifndef NAOBI_LOGGER_HPP
#define NAOBI_LOGGER_HPP

#include <fstream>
#include <iostream>
#include <chrono>

#include <naobi/utils/output_type_trait.hpp>
#include <naobi/utils/parser.hpp>

#define LOG(addressee, level, ...) \
	naobi::logger::log(#addressee, level, ##__VA_ARGS__)

namespace naobi
{
	class logger
	{
	public:
		logger() = delete;

		template <typename... AArgs>
		static void log(const std::string& addressee, int level, AArgs&&... aargs);
		static inline void setLoggingFile(const std::string& fileName);

		static void enable(){_isEnabled = true;}
		static void disable(){_isEnabled = false;}
		static void enableLoggingToStdOut(){_useStdOut = true;}
		static void disableLoggingToStdOut(){_useStdOut = false;}
		static void setLevel(int level){_currentLevel = level;}
	private:
		template <typename T, typename... AArgs>
		static void log_rec(T&& type, AArgs&&... aargs);

		template <typename T>
		static void log_rec(T&& type);

		template <typename T>
		static inline void print(T&& text);
		template <typename T>
		static inline void println(T&& text);
	private:
		static inline std::string _currentAddressee;
		static inline std::ofstream _file;
		static inline bool _isEnabled{};
		static inline bool _useStdOut{};
		static inline int _currentLevel{};
	};

	void logger::setLoggingFile(const std::string &fileName)
	{
		if (_file.is_open()) _file.close();
		_file.open(fileName, std::ios_base::out);
	}

	template <typename... AArgs>
	void logger::log(const std::string &addressee, int level, AArgs&&... aargs)
	{
		if (!_isEnabled) return;
		if (level > _currentLevel) return;
		std::time_t current_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		if (addressee != _currentAddressee)
		{
			println("[" + addressee + "]:");
			_currentAddressee = addressee;
		}
		print("<" + naobi::parser::removeSym(std::ctime(&current_time) + std::string(">"), '\n'));
		print(" (" + std::to_string(level) + ")");
		print(" -> ");

		log_rec(std::forward<AArgs>(aargs)...);
	}

	template <typename T, typename... AArgs>
	void logger::log_rec(T &&type, AArgs &&... aargs)
	{
		print(std::forward<T>(type));
		log_rec(std::forward<AArgs>(aargs)...);
	}

	template <typename T>
	void logger::log_rec(T &&type)
	{
		println(std::forward<T>(type));
	}

	template <typename T>
	void logger::print(T&& text)
	{
		if (_useStdOut) std::cout << text;
		if (_file.is_open()) _file << text;
	}

	template <typename T>
	void logger::println(T&& text)
	{
		if (_useStdOut) std::cout << text << std::endl;
		if (_file.is_open()) _file << text << std::endl;
	}
}

#endif //NAOBI_LOGGER_HPP
