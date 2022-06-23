project "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

	targetdir ( "%{wks.location}/bin/" .. odir .. "/%{prj.name}" )
	objdir ( "%{wks.location}/bin-int/" .. odir .. "/%{prj.name}" )

	files {
		"Source/**.h",
		"Source/**.cpp"
	}

	includedirs {
		"Source",
		"../Core/Source",
		"../Core/Vendor/glfw/include",
		"../Core/Vendor/glad/include",
		"../Core/Vendor/spdlog/include",
		"../Core/Vendor/stb_image/include",
		"../Core/Vendor/glm",
		"../Core/Vendor/imgui",
		"../Core/Vendor/assimp/include",
		"../Core/Vendor/entt/include"
	}

	links {
		"Core"
	}

	defines {
		"GLFW_INCLUDE_NONE"
	}

	filter "configurations:Debug"
		defines { "JAM_DEBUG_ENABLED" }
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		symbols "off"