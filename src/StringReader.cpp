#include "StringReader.h"

#include <format>

namespace Jsonify
{
	StringReader::StringReader()
	{
	}

	void StringReader::read(const std::string& in, JsonValue& value)
	{
		Lexer lexer(in);

		lexer.nextToken();

		JsonValue res = parseValue(lexer);

		if (!lexer.isEnd())
			throw std::runtime_error(std::format("Json string unexpectedly continued (line {}) after first object", lexer.readToken().location.line));

		value = res;
	}

	JsonValue StringReader::parseValue(Lexer& lexer)
	{
		if (lexer.isEnd())
			throw std::runtime_error("Json string unexpectedly ended when parsing value");

		const Token& tok = lexer.readToken();

		switch (tok.type)
		{
		case Token::Type::Null:
		case Token::Type::Boolean:
			return parseKeyword(lexer);

		case Token::Type::String:
			return parseString(lexer);

		case Token::Type::Number:
			return parseNumber(lexer);

		case Token::Type::Char:
		{
			if (tok.rawValue.compare("[") == 0)
			{
				return parseArray(lexer);
			}
			else if (tok.rawValue.compare("{") == 0)
			{
				return parseDictionary(lexer);
			}
			
			throw std::runtime_error(std::format("Unknown character '{}' at line {}", tok.rawValue, tok.location.line));
		}

		default:
			throw std::runtime_error(std::format("Unknown token \"{}\" at line {}", tok.rawValue, tok.location.line));
		}
	}

	JsonValue StringReader::parseKeyword(Lexer& lexer)
	{
		if (lexer.readToken().type == Token::Type::Boolean)
		{
			bool res = lexer.readToken().rawValue == "true";

			lexer.nextToken();

			return res;
		}
		
		lexer.nextToken();

		return JsonValue();
	}
	
	JsonValue StringReader::parseString(Lexer& lexer)
	{
		std::string res = std::string(lexer.readToken().rawValue);

		lexer.nextToken();

		return res;
	}

	JsonValue StringReader::parseNumber(Lexer& lexer)
	{
		try
		{
			size_t sz;
			double num = std::stod(std::string(lexer.readToken().rawValue), &sz);
			
			if (sz != lexer.readToken().rawValue.size())
				throw std::runtime_error(std::format("Malformed number on line {}", lexer.readToken().location.line));

			lexer.nextToken();

			return num;
		}
		catch (const std::invalid_argument& ex)
		{
			throw std::runtime_error(std::format("Number on line {} is malformed ({})", lexer.readToken().location.line, ex.what()));
		}
		catch (const std::out_of_range& ex)
		{
			throw std::runtime_error(std::format("Number on line {} is out of range ({})", lexer.readToken().location.line, ex.what()));
		}
	}

	JsonValue StringReader::parseArray(Lexer& lexer)
	{
		lexer.nextToken();

		JsonValue arr;
		arr.setType(JsonValue::Type::Array);

		while (!lexer.isEnd() && lexer.readToken().rawValue.compare("]") != 0)
		{
			arr.push_back(parseValue(lexer));

			if (lexer.isEnd() || lexer.readToken().rawValue.compare(",") != 0) break;

			lexer.nextToken();
		}

		if (lexer.isEnd())
			throw std::runtime_error(std::format("Json string unexpectedly ended while parsing array"));

		if (lexer.readToken().rawValue.compare("]") != 0)
			throw std::runtime_error(std::format("Array does not have an ending bracket on line {}", lexer.readToken().location.line));
		
		lexer.nextToken();

		return arr;
	}

	JsonValue StringReader::parseDictionary(Lexer& lexer)
	{
		lexer.nextToken();

		JsonValue dict;
		dict.setType(JsonValue::Type::Dictionary);

		while (!lexer.isEnd() && lexer.readToken().rawValue.compare("}") != 0)
		{
			const Token& tokKey = lexer.readToken();
			
			if (tokKey.type != Token::Type::String || tokKey.type == Token::Type::BrokenString)
				throw std::runtime_error(std::format("Missing or malformed key for dictionary on line {}", tokKey.location.line));

			std::string key = std::string(tokKey.rawValue);
			
			if (lexer.nextToken().type != Token::Type::Char || lexer.readToken().rawValue.compare(":") != 0)
				throw std::runtime_error(std::format("Expected a ':' on line {}", lexer.readToken().location.line));

			lexer.nextToken();

			dict[key] = parseValue(lexer);

			if (lexer.isEnd() || lexer.readToken().rawValue.compare(",") != 0) break;

			lexer.nextToken();
		}

		if (lexer.isEnd())
			throw std::runtime_error(std::format("Json string unexpectedly ended while parsing dictionary"));

		if (lexer.readToken().rawValue.compare("}") != 0)
			throw std::runtime_error(std::format("Dictionary did not have an ending bracket on line {}", lexer.readToken().location.line));

		lexer.nextToken();

		return dict;
	}
}