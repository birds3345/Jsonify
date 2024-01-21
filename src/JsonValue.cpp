#include "JsonValue.h"

namespace Jsonify
{
	JsonValue::JsonValue()
		: type(Type::Null), b(0)
	{
	}

	JsonValue::JsonValue(double n)
		: type(Type::Number), n(n)
	{
	}

	JsonValue::JsonValue(int n)
		: type(Type::Number), n(n)
	{
	}

	JsonValue::JsonValue(float n)
		: type(Type::Number), n(n)
	{
	}

	JsonValue::JsonValue(bool b)
		: type(Type::Boolean), b(b)
	{
	}

	JsonValue::JsonValue(std::string s)
		: type(Type::String), s(s)
	{
	}

	JsonValue::JsonValue(const char* s)
		: type(Type::String), s(s)
	{
	}

	JsonValue::JsonValue(Null)
		: type(Type::Null), b(0)
	{
	}

	JsonValue::JsonValue(const JsonValue& other)
		: type(other.type), b(0)
	{
		switch (type)
		{
		case Type::String:
			new (&s) std::string(other.s);
			break;

		case Type::Dictionary:
			new (&m) std::unordered_map(other.m);
			break;

		case Type::Array:
			new (&v) std::vector(other.v);
			break;

		case Type::Number:
			n = other.n;
			break;

		case Type::Boolean:
			b = other.b;
			break;
		}
	}

	JsonValue::JsonValue(JsonValue&& other) noexcept
		: type(Type::Null), b(0)
	{
		setType(other.type);

		switch (type)
		{
		case Type::String:
			s = std::move(other.s);
			break;

		case Type::Dictionary:
			m = std::move(other.m);
			break;

		case Type::Array:
			v = std::move(other.v);
			break;

		case Type::Number:
			n = other.n;
			break;

		case Type::Boolean:
			b = other.b;
			break;
		}
	}

	bool JsonValue::isString() const
	{
		return type == Type::String;
	}

	bool JsonValue::isNumber() const
	{
		return type == Type::Number;
	}

	bool JsonValue::isDictionary() const
	{
		return type == Type::Dictionary;
	}

	bool JsonValue::isArray() const
	{
		return type == Type::Array;
	}

	bool JsonValue::isBoolean() const
	{
		return type == Type::Boolean;
	}

	bool JsonValue::isNull() const
	{
		return type == Type::Null;
	}

	bool JsonValue::isTruthful() const
	{
		return type == Type::Boolean && b || type != Type::Null;
	}

	JsonValue& JsonValue::operator=(JsonValue&& other) noexcept
	{
		setType(other.type);

		switch (type)
		{
		case Type::String:
			s = std::move(other.s);
			break;

		case Type::Dictionary:
			m = std::move(other.m);
			break;

		case Type::Array:
			v = std::move(other.v);
			break;

		case Type::Number:
			n = other.n;
			break;

		case Type::Boolean:
			b = other.b;
			break;
		}

		return *this;
	}
	
	JsonValue& JsonValue::operator=(const JsonValue& other)
	{
		setType(other.type);

		switch (type)
		{
		case Type::String:
			s = other.s;	
			break;

		case Type::Dictionary:
			m = other.m;
			break;

		case Type::Array:
			v = other.v;
			break;

		case Type::Number:
			n = other.n;
			break;

		case Type::Boolean:
			b = other.b;
			break;
		}

		return *this;
	}

	bool JsonValue::operator==(const JsonValue& other) const
	{
		if (type != other.type) return false;

		switch (type)
		{
		case Type::String:
			return s == other.s;

		case Type::Number:
			return n == other.n;

		case Type::Boolean:
			return b == other.b;

		case Type::Dictionary:
			return m == other.m;

		case Type::Array:
			return v == other.v;

		case Type::Null:
			return true;

		}

		return false;
	}

	bool JsonValue::operator!=(const JsonValue& other) const
	{
		return !(*this == other);
	}

	JsonValue& JsonValue::operator[](const std::string& key)
	{
		setType(Type::Dictionary);

		return m[key];
	}

	const JsonValue& JsonValue::operator[](const std::string& key) const
	{
		if (type != Type::Dictionary) throw std::runtime_error("Type is not a dictionary");

		return m.at(key);
	}

	void JsonValue::remove(const std::string& key)
	{
		m.erase(key);
	}

	bool JsonValue::contains(const std::string& key) const
	{
		if (type != Type::Dictionary) throw std::runtime_error("Type is not a dictionary");

		return m.find(key) != m.end();
	}

	JsonValue& JsonValue::operator[](std::size_t index)
	{
		setType(Type::Array);
		
		return v[index];
	}

	const JsonValue& JsonValue::operator[](std::size_t index) const
	{
		if (type != Type::Array) throw std::runtime_error("Type is not an array");

		return v[index];
	}

	void JsonValue::push_back(JsonValue& val)
	{
		setType(Type::Array);

		v.push_back(val);
	}

	void JsonValue::push_back(JsonValue&& val)
	{
		setType(Type::Array);

		v.push_back(val);
	}

	void JsonValue::reserve(std::size_t amount)
	{
		setType(Type::Array);

		v.reserve(amount);
	}

	void JsonValue::resize(std::size_t to)
	{
		setType(Type::Array);

		v.resize(to);
	}

	std::size_t JsonValue::size() const
	{
		if (type == Type::Array)
		{
			return v.size();
		}
		else if (type == Type::Dictionary)
		{
			return m.size();
		}
		else
		{
			throw std::runtime_error("Unable to get size of a type that is not Dictionary or Array");
		}

		return 0;
	}

	bool JsonValue::operator!() const
	{
		return !isTruthful();
	}

	JsonValue::operator bool() const
	{
		return isTruthful();
	}

	JsonValue::Iterator JsonValue::begin()
	{
		if (type != Type::Dictionary) throw std::runtime_error("Only dictionaries are iterable");
		
		return m.begin();
	}

	JsonValue::Iterator JsonValue::end()
	{
		if (type != Type::Dictionary) throw std::runtime_error("Only dictionaries are iterable");

		return m.end();
	}

	JsonValue::~JsonValue()
	{
		setType(Type::Null);
	}

	void JsonValue::setType(Type type)
	{
		if (this->type == type) return;
		if (type != Type::Null &&
			this->type != Type::Null &&
			(this->type == Type::Array || this->type == Type::Dictionary || type == Type::Array || type == Type::Dictionary)) throw std::runtime_error("Attempted to change type to an incompatible type");

		switch (this->type)
		{
		case Type::Array:
			v.~vector();
			break;

		case Type::Dictionary:
			m.~unordered_map();
			break;

		case Type::String:
			s.~basic_string();
			break;

		}

		this->type = type;

		switch (this->type)
		{
		case Type::Array:
			new (&v) std::vector<JsonValue>;
			break;

		case Type::Dictionary:
			new (&m) std::unordered_map<std::string, JsonValue>;
			break;

		case Type::String:
			new (&s) std::string;
			break;

		case Type::Boolean:
			b = false;
			break;

		case Type::Number:
			n = 0;
			break;
		}
	}
}