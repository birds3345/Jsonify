#include <iostream>

#include "Jsonify.h"

struct Vector3
{
	double x, y, z;
};

template<>
void Jsonify::JsonSerde::serialize(JsonValue& val, const Vector3& from)
{
	val.setType(JsonValue::Type::Array);

	val.resize(3);
	val[0] = from.x;
	val[1] = from.y;
	val[2] = from.z;
}

template<>
void Jsonify::JsonSerde::deserialize(const JsonValue& val, Vector3& from)
{
	from.x = val[0].as<double>();
	from.y = val[1].as<double>();
	from.z = val[2].as<double>();
}

int main()
{
	Jsonify::JsonValue vec = Vector3(1,4,2);

	Jsonify::StringWriter writer({
		.pretty = true,
	});

	std::string out;
	writer.write(vec, out);

	std::cout << out << std::endl; //[1.000000, 4.000000, 2.000000]

	Vector3 deserialized = vec.as<Vector3>();

	std::cout << deserialized.x << ", " << deserialized.y << ", " << deserialized.z << std::endl; //1, 4, 2

	return 0;
}