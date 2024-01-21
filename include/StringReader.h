#pragma once

#include <string>

#include "Lexer.h"
#include "JsonValue.h"

namespace Jsonify
{
	class StringReader
	{
	public:
		StringReader();

		void read(const std::string& in, JsonValue& value);

	private:
		JsonValue parseValue(Lexer& lexer);
		JsonValue parseKeyword(Lexer& lexer);
		JsonValue parseString(Lexer& lexer);
		JsonValue parseNumber(Lexer& lexer);
		JsonValue parseArray(Lexer& lexer);
		JsonValue parseDictionary(Lexer& lexer);
	};
}
