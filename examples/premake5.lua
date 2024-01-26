workspace "JsonifyExamples"
	architecture "x64"

	configurations {
		"Debug",
		"Release",
	}

	startproject "JsonifyExamples"

project "JsonifyExamples"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"

	includedirs {"../include"}

	targetdir "bin/%{cfg.buildcfg}"
	objdir "obj/%{cfg.buildcfg}"

	files {"Basic.cpp"}

	links {"Jsonify"}

	filter "configurations:Debug"
		runtime "Debug"
		optimize "Off"
		symbols "On"

	filter "configurations:Release"
		runtime "Release"
		optimize "On"
		symbols "Off"

include "../"