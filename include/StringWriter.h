#pragma once

#include <string>

#include "JsonValue.h"

namespace Jsonify
{
	class StringWriter
	{
	public:
		struct Settings
		{
			bool pretty = false;
		};

		StringWriter(Settings settings);

		void write(JsonValue& value, std::string& out);

	private:
		void write(JsonValue& value, std::string& out, int indents);

		Settings settings;
	};
}