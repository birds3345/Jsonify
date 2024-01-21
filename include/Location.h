#pragma once

#include <cstddef>

namespace Jsonify
{
	struct Location
	{
		std::size_t start;
		std::size_t end;

		int line;
	};
}