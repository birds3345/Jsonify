#include "StringWriter.h"

#include <format>

inline void appendIndents(std::string& out, int indents)
{
	for (int i = 0; i < indents; i++)
		out.append("   ");
};

namespace Jsonify
{
	StringWriter::StringWriter(Settings settings)
		: settings(settings)
	{
	}

	void StringWriter::write(JsonValue& value, std::string& out)
	{
		write(value, out, 0);
	}

	void StringWriter::write(JsonValue& value, std::string& out, int indents)
	{
		switch (value.type)
		{
		case JsonValue::Type::Dictionary:
		{
			out.append("{");
			if (settings.pretty)
				out.append("\n");

			int i = 0;
			for (auto& [k, v] : value)
			{
				if (settings.pretty) appendIndents(out, indents + 1);

				out.append("\"");
				out.append(k);
				out.append("\"");

				if (settings.pretty)
					out.append(" : ");
				else
					out.append(":");

				write(v, out, indents + 1);

				if (++i < value.size())
				{
					out.append(",");

					if (settings.pretty) out.append("\n");
				}
			}

			if (settings.pretty)
			{
				out.append("\n");
				appendIndents(out, indents);
			}

			out.append("}");

			break;
		}

		case JsonValue::Type::Array:
		{
			out.append("[");

			for (std::size_t i = 0; i < value.size(); i++)
			{
				write(value[i], out, indents);

				if (i < value.size() - 1)
				{
					out.append(",");

					if (settings.pretty) out.append(" ");
				}
			}

			out.append("]");

			break;
		}

		case JsonValue::Type::Boolean:
			out.append(value.b ? "true" : "false");
			break;

		case JsonValue::Type::Null:
			out.append("null");
			break;

		case JsonValue::Type::String:
		{
			out.append("\"");
			out.append(value.s);
			out.append("\"");

			break;
		}

		case JsonValue::Type::Number:
			out.append(std::format("{}", value.n));
			break;
		}
	}
}