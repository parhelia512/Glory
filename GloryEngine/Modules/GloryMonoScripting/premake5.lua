project "GloryMonoScripting"
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
		["Module"] = { "GloryMono.*", "GloryMonoScipting.*" },
		["API"] = { "EntityCSAPI.*", "InputCSAPI.*", "CoreCSAPI.*", "MathCSAPI.*", "PhysicsCSAPI.*", "ScriptingMethodsHelper.*", "AudioCSAPI.*" },
		["Resource"] = { "MonoScriptLoader.*", "MonoScript.*" },
		["Mono"] = { "Assembly.*", "AssemblyDomain.*", "MonoLibManager.*", "MonoManager.*", "CoreLibManager.*", "IMonoLibManager.*" },
	}

	includedirs
	{
		"%{DepsIncludeDir}",

		"%{BaseIncludeDir.audio}",

		"%{GloryIncludeDir.enginecore}",
		"%{GloryIncludeDir.engine}",

		"%{IncludeDir.glm}",
		"%{IncludeDir.yaml_cpp}",
		"%{IncludeDir.Utils}",
		"%{IncludeDir.ECS}",
		"%{IncludeDir.Reflect}",
		"%{IncludeDir.Version}",
	}

	libdirs
	{
		"%{DepsLibDir}",

		"%{LibDirs.glory}",
		"%{LibDirs.yaml_cpp}",

		"{moduleBaseOutDir}",
	}

	links
	{
		"GloryEngineCore",
		"GloryEngine",
		"GloryAudioModule",

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
		"GLORY_MONO_EXPORTS"
	}

	postbuildcommands
	{
		("{COPY} ./Module.yaml %{moduleOutDir}"),
		("{COPY} ./Assets %{moduleOutDir}/Assets"),
		("{COPY} ./Resources %{moduleOutDir}/Resources"),

		("{COPY} \"%{monoDir}/lib/mono/4.5/*\" %{moduleOutDir}/Dependencies/mono/4.5/"),
		("{COPY} %{DepsBinDir}/mono-2.0-sgen.dll %{moduleOutDir}/Dependencies/"),
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
