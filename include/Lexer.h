#pragma once

#include <cstddef>
#include <deque>
#include <string>
#include <string_view>

#include "Location.h"

namespace Jsonify
{
	struct Token
	{
		enum class Type
		{
			Boolean,
			Number,
			String,
			BrokenString,
			Null,

			Char,
			
			Unknown,

			Eof,
		};

		Type type;

		Location location;

		std::string_view rawValue;
	};

	class Lexer
	{
	public:
		Lexer(const std::string& source);
		
		const Token& readToken() const;
		const Token& nextToken();
		const Token& peekToken(int amount = 1);

		bool isEnd() const;
	private:
		const Token parseToken();

		std::deque<Token> tokens;

		char consume();
		char readChar() const;
		char peekChar(std::size_t offset = 1) const;

		std::string source;

		int lineNumber;
		std::size_t pointer;
	};

}