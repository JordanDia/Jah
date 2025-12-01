include "Dependencies.lua"

workspace "Jah"
	architecture "x64"
	startproject "Jah-Editor"
	toolset "v145"

	configurations {
		"Debug",
		"Release",
		"Dist"
	}

	flags { "MultiProcessorCompile" }

outputdir = "%{cfg.buildcfg}-%{cfg.architecture}"

group "Dependencies"
	include "Jah/External/GLFW"
	include "Jah/External/Glad"
	include "Jah/External/imgui"
	include "Jah/External/yaml-cpp"
	include "Jah/External/Box2D"

group "Core"
	include "Jah"
	include "Jah-ScriptCore"
	include "Jah-Editor"

group "Misc"
	include "Sandbox"


