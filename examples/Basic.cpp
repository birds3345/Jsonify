#include "Jsonify.h"

#include <iostream>


int main()
{
	Jsonify::JsonValue val;
	val["var1"] = "example";
	val["var2"] = 4;
	val["var3"].push_back(false);

	Jsonify::JsonValue dict;
	dict["a"]["b"] = Jsonify::JsonValue::Null();

	val["var3"].push_back(dict);

	Jsonify::StringWriter writer({
		.pretty = true,
	});

	std::string out;
	writer.write(val, out);

	/*
	{
	   "var1" : "example",
	   "var2" : 4.000000,
	   "var3" : [false, {
		  "a" : {
			 "b" : null
		  }
	   }]
	}
	*/
	std::cout << out << std::endl;
	


	Jsonify::StringReader reader;
	Jsonify::JsonValue res;

	reader.read(out, res);



	std::string out2;
	writer.write(res, out2);

	/*
	{
	   "var1" : "example",
	   "var2" : 4.000000,
	   "var3" : [false, {
		  "a" : {
			 "b" : null
		  }
	   }]
	}
	*/
	std::cout << out2 << std::endl;
}