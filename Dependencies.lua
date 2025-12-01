IncludeDirs = {
	GLFW = "%{wks.location}/Jah/External/GLFW/include",
	Glad = "%{wks.location}/Jah/External/Glad/include",
	ImGui = "%{wks.location}/Jah/External/imgui",
	ImGuiBackends = "%{wks.location}/Jah/External/imgui/backends",
	glm = "%{wks.location}/Jah/External/glm",
	stb_image = "%{wks.location}/Jah/External/stb_image",
	spdlog = "%{wks.location}/Jah/External/spdlog/include",
	yaml_cpp = "%{wks.location}/Jah/External/yaml-cpp/include",
	ImGuizmo = "%{wks.location}/Jah/External/ImGuizmo",
	Box2D = "%{wks.location}/Jah/External/Box2D/include",
	mono = "%{wks.location}/Jah/External/mono/include",
	dotnet = "%{wks.location}/Jah/External/dotnet",
}

LibraryDir = {
	mono = "%{wks.location}/Jah/External/mono/lib/%{cfg.buildcfg}"
}

Library = {
	mono = "%{LibraryDir.mono}/libmono-static-sgen.lib"
}

-- Windows
Library.WinSock = "Ws2_32.lib"
Library.WinMM = "Winmm.lib"
Library.WinVersion = "Version.lib"
Library.BCrypt = "Bcrypt.lib"