project "Core"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

	targetdir ( "%{wks.location}/bin/" .. odir .. "/%{prj.name}" )
	objdir ( "%{wks.location}/bin-int/" .. odir .. "/%{prj.name}" )

	pchheader "gfxpch.h"
	pchsource "Source/gfxpch.cpp"

	files {
		"Source/**.h",
		"Source/**.cpp",
		"Vendor/imgui/backends/imgui_impl_glfw.cpp",
		"Vendor/imgui/backends/imgui_impl_opengl3.cpp",
		"Vendor/stb_image/src/stb_image.cpp"
	}

	defines {
		"GLFW_INCLUDE_NONE"
	}

	includedirs {
		"Source",
		"Vendor/glfw/include",
		"Vendor/glad/include",
		"Vendor/spdlog/include",
		"Vendor/stb_image/include",
		"Vendor/glm",
		"Vendor/imgui",
		"Vendor/assimp/include",
		"Vendor/assimp/include",
		"Vendor/entt/include"
	}

	links {
		"glfw",
		"glad",
		"ImGui",
		"assimp"
	}

	filter "configurations:Debug"
		defines { "JAM_DEBUG_ENABLED" }
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		symbols "off"