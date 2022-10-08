#ifndef NAOBI_INDENT_SCOPE_HPP
#define NAOBI_INDENT_SCOPE_HPP

#include <streambuf>
#include <string>


namespace naobi
{
	class indent_scope : public std::streambuf
	{
	protected:
		int overflow(int ch) final
		{
			if (m_isAtStartOfLine && ch != '\n')
			{
				m_dest->sputn(m_indent.data(), static_cast<std::streamsize>(m_indent.size()));
			}
			m_isAtStartOfLine = (ch == '\n');
			return m_dest->sputc(static_cast<char>(ch));
		}

	public:
		explicit indent_scope(
			std::streambuf* dest, std::size_t indent = 1)
			: m_dest(dest), m_isAtStartOfLine(true), m_indent(indent, '\t'), m_owner(nullptr)
		{
		}

		explicit indent_scope(
			std::ostream& dest, std::size_t indent = 1)
			: m_dest(dest.rdbuf()), m_isAtStartOfLine(true), m_indent(indent, '\t'), m_owner(&dest)
		{
			m_owner->rdbuf(this);
		}

		~indent_scope() override
		{
			if (m_owner != nullptr)
			{
				m_owner->rdbuf(m_dest);
			}
		}

	private:
		std::streambuf* m_dest;
		bool m_isAtStartOfLine;
		std::string m_indent;
		std::ostream* m_owner;
	};
}

#endif //NAOBI_INDENT_SCOPE_HPP
