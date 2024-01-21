#include "Lexer.h"

#include <cctype>

inline bool isNewline(char c)
{
	return c == '\n';
}

inline bool isEscape(char c)
{
	return
		c == '\n' ||
		c == '\r' ||
		c == '\t' ||
		c == '\a' ||
		c == '\b';
}

namespace Jsonify
{
	Lexer::Lexer(const std::string& source)
		: lineNumber(1), pointer(0), source(source)
	{
	}

	const Token& Lexer::nextToken()
	{
		if (tokens.size() > 0)
			tokens.pop_front();

		if (tokens.size() == 0)
			tokens.push_back(parseToken());

		return tokens.front();
	}

	const Token& Lexer::peekToken(int amount)
	{
		if (tokens.size() >= amount + 1)
			return tokens[amount];

		std::size_t am = amount - tokens.size() + 1;
		for (std::size_t i = 0; i < am; i++)
		{
			tokens.push_back(parseToken());
		}

		return tokens[amount];
	}

	bool Lexer::isEnd() const
	{
		return readToken().type == Token::Type::Eof;
	}

	const Token& Lexer::readToken() const
	{
		return tokens.front();
	}

	const Token Lexer::parseToken()
	{
		if (!readChar())
			return { Token::Type::Eof, { pointer, pointer, lineNumber }, std::string_view() };

		if (readChar() == ',' || readChar() == ':' || readChar() == '{' || readChar() == '}' || readChar() == '[' || readChar() == ']')
		{
			Token tok = { Token::Type::Char, { pointer, pointer, lineNumber }, std::string_view(source.data() + pointer, 1) };

			consume();

			return tok;
		}

		if (readChar() == '"')
		{
			consume();

			std::size_t strStart = pointer;

			while (readChar() && readChar() != '"' && !isNewline(readChar()))
			{
				consume();
			}

			std::string_view string = std::string_view(source.data() + strStart, pointer - strStart);

			if (readChar() == '"')
			{
				Token tok = { Token::Type::String, { strStart, pointer - 1, lineNumber }, string };

				consume();

				return tok;
			}
			else
			{
				return { Token::Type::BrokenString, { strStart, pointer - 1, lineNumber }, string };
			}
		}

		if (isalpha(readChar()))
		{
			std::size_t identStart = pointer;

			while (isalpha(readChar()) || isdigit(readChar()))
			{
				consume();
			}

			std::string_view identifer = std::string_view(source.data() + identStart, pointer - identStart);

			Token tok;

			if (identifer.compare("null") == 0)
			{
				tok = { Token::Type::Null, { identStart, pointer - 1, lineNumber }, identifer };
			}
			else if (identifer.compare("true") == 0 || identifer.compare("false") == 0)
			{
				tok = { Token::Type::Boolean, { identStart, pointer - 1, lineNumber }, identifer };
			}
			else
			{
				tok = { Token::Type::Unknown, { identStart, pointer - 1, lineNumber }, identifer };
			}

			return tok;
		}

		if (isdigit(readChar()) || readChar() == '-')
		{
			std::size_t numberStart = pointer;

			consume();

			while (isdigit(readChar()) || readChar() == 'e' || readChar() == '-' || readChar() == '+' || readChar() == '.')
			{
				consume();
			}

			std::string_view number = std::string_view(source.data() + numberStart, pointer - numberStart);

			return { Token::Type::Number, { numberStart, pointer - 1, lineNumber }, number };
		}

		if (readChar() == ' ')
		{
			while (readChar() == ' ')
			{
				consume();
			}

			return parseToken();
		};

		if (isEscape(readChar()))
		{
			while (isEscape(readChar()))
			{
				consume();
			}

			return parseToken();
		}

		Token tok = { Token::Type::Unknown, { pointer, pointer, lineNumber }, std::string_view(source.data() + pointer, 1) };
		consume();

		return tok;
	}


	char Lexer::consume()
	{
		char c = source[pointer];

		if (c == '\n')
		{
			lineNumber++;
		}

		pointer++;

		return c;
	}

	char Lexer::readChar() const
	{
		return source[pointer];
	}

	char Lexer::peekChar(std::size_t offset) const
	{
		if (pointer + offset >= source.length())
			return '\0';

		return source[pointer + offset];
	}
}