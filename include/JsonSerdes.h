#pragma once

#include <stdexcept>

namespace Jsonify
{
	class JsonValue;

	struct JsonSerde
	{
		template<typename T>
		inline static void serialize(JsonValue& val, const T& from)
		{
			throw std::runtime_error("Invalid call to \"serialize\", type provided is not allowed");
		};

		template<typename T>
		inline static void deserialize(const JsonValue& val, T& res)
		{
			throw std::runtime_error("Invalid call to \"deserialize\", type provided is not allowed");
		};

		JsonSerde() = delete;
	};
}