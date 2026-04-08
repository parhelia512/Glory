project "GlorySDLWindow"
	kind "SharedLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "Off"

	targetdir ("%{moduleOutDir}")
	objdir ("%{outputDir}")

	files
	{
		"**.h",
		"**.cpp",
		"Module.yaml",
		"premake5.lua",
		"Assets/**.*",
		"Resources/**.*",
	}

	vpaths
	{
		["Core"] = { "SDLWindow.h", "SDLWindow.cpp", "SDLWindowExceptions.h" },
		["Module"] = { "GlorySDLWindow.*", "SDLWindowModule.h", "SDLWindowModule.cpp" }
	}

	includedirs
	{
		"%{DepsIncludeDir}",

		"%{GloryIncludeDir.enginecore}",
		"%{GloryIncludeDir.engine}",

		"%{IncludeDir.glm}",
		"%{IncludeDir.Reflect}",
		"%{IncludeDir.Version}",
		"%{IncludeDir.Utils}",
		"%{IncludeDir.ECS}",
		"%{IncludeDir.yaml_cpp}",
	}

	libdirs
	{
		"%{DepsLibDir}",

		"%{LibDirs.glory}",
		"%{LibDirs.yaml_cpp}",
	}

	links
	{
		"SDL2",

		"GloryEngineCore",
		"GloryEngine",
		"GloryReflect",
		"GloryECS",

		"yaml-cpp",

		"GloryUtilsVersion",
		"GloryUtils",
	}

	defines
	{
		"GLORY_SDL_WINDOW_EXPORTS"
	}

	postbuildcommands
	{
		("{COPY} %{DepsBinDir}/SDL2?.dll %{moduleOutDir}/Dependencies"),
		("{COPY} ./Module.yaml %{moduleOutDir}"),
	}

	filter "system:windows"
		systemversion "latest"
		toolset "v143"

	filter "platforms:Win32"
		architecture "x86"
		defines "WIN32"

	filter "platforms:x64"
		architecture "x64"

	filter "configurations:Debug"
		runtime "Debug"
		defines "_DEBUG"
		symbols "On"

	filter "configurations:Release"
		runtime "Release"
		defines "NDEBUG"
		optimize "On"
