workspace "Engine"
	architecture "x64"
	startproject "Sandbox"

	configurations {
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] =     "Renderent/vendor/glfw/include"
IncludeDir["GLAD"] =     "Renderent/vendor/GLAD/include"
IncludeDir["ImGui"] =    "Renderent/vendor/imgui"
IncludeDir["glm"] =      "Renderent/vendor/glm"
IncludeDir["stb_image"] = "Renderent/vendor/stb_image"
IncludeDir["box2d"] = "Sandbox/vendor/box2d/include"

include "Renderent/vendor/glfw"
include "Renderent/vendor/GLAD"
include "Renderent/vendor/imgui"

include "Sandbox/vendor/box2d"

project "Renderent"
	location "Renderent"
	kind "StaticLib"
	language "C++"
	staticruntime "on"
	cppdialect "C++17"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "repch.h"
	pchsource "Renderent/src/repch.cpp"

	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/stb_image/**.cpp",
	}

	includedirs {
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.GLAD}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}"
	}

	defines {
		"_CRT_SECURE_NO_WARNINGS",
		"GLFW_INCLUDE_NONE"
	}

	links {
		"GLFW",
		"GLAD",
		"ImGui"
	}
	
	filter "system:windows"

		systemversion "latest"
		defines {
			"RE_PLATFORM_WINDOWS",
			"RE_BUILD_DLL"
		}
	filter "system:linux"
		systemversion "latest"
		defines {
			"RE_PLATFORM_LINUX"
		}

	filter "configurations:Debug" 
		defines "RE_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "RE_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		runtime "Release"
		defines "RE_DIST"
		optimize "on"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs {
		"Renderent/vendor/spdlog/include",
		"Renderent/src",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.box2d}"
	}

	links {
		"Renderent",
		"box2d"
	}
	
	filter "system:windows"
		staticruntime "On"
		systemversion "latest"
		defines {
			"RE_PLATFORM_WINDOWS"
		}

	filter "system:linux"
		staticruntime "on"
		systemversion "latest"
		defines {
			"RE_PLATFORM_LINUX"
		}
		links {
			"Renderent",
			"GLFW",
			"GLAD",
			"ImGui",
			"GL",
			"X11",
			"dl",
			"pthread"
		}

	filter "configurations:Debug" 
		defines "RE_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "RE_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		runtime "Release"
		defines "RE_DIST"
