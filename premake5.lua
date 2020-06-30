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

include "Renderent/vendor/glfw"
include "Renderent/vendor/GLAD"
include "Renderent/vendor/imgui"

project "Renderent"
	location "Renderent"
	kind "SharedLib"
	language "C++"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "repch.h"
	pchsource "Renderent/src/repch.cpp"

	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs {
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.GLAD}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}"
	}

	links {
		"GLFW",
		"GLAD",
		"ImGui"
	}
	
	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"
		defines {
			"RE_PLATFORM_WINDOWS",
			"RE_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

		postbuildcommands {
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/sandbox")
		}

	filter "configurations:Debug" 
		defines "RE_DEBUG"
		buildoptions "/MDd"
		symbols "On"

	filter "configurations:Release"
		defines "RE_RELEASE"
		buildoptions "/MD"
		optimize "On"

	filter "configurations:Dist"
		buildoptions "/MD"
		defines "RE_DIST"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs {
		"Renderent/vendor/spdlog/include",
		"Renderent/src",
		"%{IncludeDir.glm}"
	}

	links {
		"Renderent"
	}
	
	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"
		defines {
			"RE_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug" 
		defines "RE_DEBUG"
		buildoptions "/MDd"
		symbols "On"

	filter "configurations:Release"
		defines "RE_RELEASE"
		buildoptions "/MD"
		optimize "On"

	filter "configurations:Dist"
		buildoptions "/MD"
		defines "RE_DIST"

