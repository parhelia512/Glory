project "GloryUIMonoExtender"
	kind "SharedLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "Off"

	targetdir ("%{modulesOutDir}/GloryUIRenderer/Scripting/csharp")
	objdir ("%{outputDir}")

	files
	{
		"**.h",
		"**.cpp",
		"premake5.lua"
	}

	vpaths
	{
		["API"] = { "UIComponentsCSAPI.*", "UISceneCSAPI.*" },
		["Extenstion"] = { "UIMonoExtender.*" }
	}

	includedirs
	{
		"%{DepsIncludeDir}",

		"%{BaseIncludeDir.localize}",

		"%{GloryIncludeDir.enginecore}",
		"%{GloryIncludeDir.engine}",
		"%{GloryIncludeDir.ui}",
		"%{GloryIncludeDir.mono}",

		"%{IncludeDir.glm}",
		"%{IncludeDir.yaml_cpp}",
		"%{IncludeDir.ECS}",
		"%{IncludeDir.Reflect}",
		"%{IncludeDir.Version}",
		"%{IncludeDir.Utils}",
	}

	libdirs
	{
		"%{DepsLibDir}",

		"%{LibDirs.glory}",
	}

	links
	{
		"GloryEngineCore",
		"GloryEngine",
		"GloryUIRenderer",
		"GloryMonoScripting",
		"yaml-cpp",
		"mono-2.0-sgen",
		"MonoPosixHelper",

		"GloryECS",
		"GloryReflect",
		"GloryUtilsVersion",
		"GloryUtils",
	}

	defines
	{
		"GLORY_UI_MONO_EXPORTS"
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
