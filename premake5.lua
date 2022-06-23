workspace "Jam"
    architecture "x86_64"
    startproject "Sandbox"

    configurations {
        "Debug",
        "Release"
    }

    flags {
        "MultiProcessorCompile"
    }

odir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Vendor"
    include "Core/Vendor/glfw"
    include "Core/Vendor/glad"
    include "Core/Vendor/imgui"
    include "Core/Vendor/assimp"
group ""

include "Core"
include "Sandbox"