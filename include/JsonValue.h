#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <stdexcept>

#include "JsonSerdes.h"

namespace Jsonify
{
	class JsonValue
	{
	public:
		typedef std::unordered_map<std::string, JsonValue>::iterator Iterator;

		struct Null
		{
		};
		
		JsonValue();

		template<typename T>
		inline JsonValue(T t)
			: type(Type::Null), b(0)
		{
			JsonSerde::serialize(*this, t);
		};

		JsonValue(double n);
		JsonValue(int n);
		JsonValue(float n);
		JsonValue(bool b);

		JsonValue(std::string s);
		JsonValue(const char* s);
		
		JsonValue(Null);

		JsonValue(const JsonValue& other);
		JsonValue(JsonValue&& other) noexcept;
		

		bool isString() const;
		bool isNumber() const;
		bool isDictionary() const;
		bool isArray() const;
		bool isBoolean() const;
		bool isNull() const;
		bool isTruthful() const;

		JsonValue& operator=(JsonValue&& other) noexcept;
		JsonValue& operator=(const JsonValue& other);

		bool operator==(const JsonValue& other) const;
		bool operator!=(const JsonValue& other) const;

		//map
		JsonValue& operator[](const std::string& key);
		const JsonValue& operator[](const std::string& key) const;
		void remove(const std::string& key);
		bool contains(const std::string& key) const;
		
		//array
		JsonValue& operator[](std::size_t index);
		const JsonValue& operator[](std::size_t index) const;
		void push_back(JsonValue& val);
		void push_back(JsonValue&& val);
		void reserve(std::size_t amount);
		void resize(std::size_t to);

		std::size_t size() const;

		//boolean
		bool operator!() const;
		explicit operator bool() const;

		template<typename T, typename... Args>
		inline T as(Args&&... args) const
		{
			T res(std::forward<Args>(args)...);
			
			JsonSerde::deserialize(*this, res);

			return res;
		};

		Iterator begin();
		Iterator end();

		~JsonValue();
		
		friend struct JsonSerde;
		friend class StringWriter;
		friend class StringReader;
	private:
		enum class Type
		{
			String,
			Number,
			Dictionary,
			Array,
			Boolean,
			Null,
		};

		Type type;
		void setType(Type type);
	
		union
		{
			std::string s;
			std::vector<JsonValue> v;
			std::unordered_map<std::string, JsonValue> m;

			double n;
			bool b;
		};
	};

	
	//int
	template<>
	inline static void JsonSerde::serialize(JsonValue& val, const int& from)
	{
		if (val.type != JsonValue::Type::Number) throw std::runtime_error("Type mismatch, expected a number");

		val = from;
	}

	template<>
	inline static void JsonSerde::deserialize(const JsonValue& val, int& res)
	{
		if (val.type != JsonValue::Type::Number) throw std::runtime_error("Type mismatch, expected a number");

		res = (int)val.n;
	}


	//float
	template<>
	inline static void JsonSerde::serialize(JsonValue& val, const float& from)
	{
		if (val.type != JsonValue::Type::Number) throw std::runtime_error("Type mismatch, expected a number");

		val = from;
	}

	template<>
	inline static void JsonSerde::deserialize(const JsonValue& val, float& res)
	{
		if (val.type != JsonValue::Type::Number) throw std::runtime_error("Type mismatch, expected a number");

		res = (float)val.n;
	}

	//double
	template<>
	inline static void JsonSerde::serialize(JsonValue& val, const double& from)
	{
		if (val.type != JsonValue::Type::Number) throw std::runtime_error("Type mismatch, expected a number");

		val = from;
	}

	template<>
	inline static void JsonSerde::deserialize(const JsonValue& val, double& res)
	{
		if (val.type != JsonValue::Type::Number) throw std::runtime_error("Type mismatch, expected a number");

		res = val.n;
	}

	//boolean
	template<>
	inline static void JsonSerde::serialize(JsonValue& val, const bool& from)
	{
		if (val.type != JsonValue::Type::Boolean) throw std::runtime_error("Type mismatch, expected a boolean");

		val = from;
	}

	template<>
	inline static void JsonSerde::deserialize(const JsonValue& val, bool& res)
	{
		if (val.type != JsonValue::Type::Boolean) throw std::runtime_error("Type mismatch, expected a bool");

		res = val.b;
	}

	//null
	template<>
	inline static void JsonSerde::serialize(JsonValue& val, const JsonValue::Null& from)
	{
		if (val.type != JsonValue::Type::Null) throw std::runtime_error("Type mismatch, expected null");

		val.setType(JsonValue::Type::Null);
	}

	template<>
	inline static void JsonSerde::deserialize(const JsonValue& val, JsonValue::Null& res)
	{
		if (val.type != JsonValue::Type::Null) throw std::runtime_error("Type mismatch, expected null");
	}

	//string
	template<>
	inline static void JsonSerde::serialize(JsonValue& val, const std::string& from)
	{
		if (val.type != JsonValue::Type::String) throw std::runtime_error("Type mismatch, expected a string");

		val = from;
	}

	template<>
	inline static void JsonSerde::deserialize(const JsonValue& val, std::string& res)
	{
		if (val.type != JsonValue::Type::String) throw std::runtime_error("Type mismatch, expected a string");
		
		res = val.s;
	}
}