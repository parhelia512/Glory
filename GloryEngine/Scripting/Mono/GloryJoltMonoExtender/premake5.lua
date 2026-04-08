project "GloryJoltMonoExtender"
	kind "SharedLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "Off"

	targetdir ("%{modulesOutDir}/GloryJoltPhysics/Scripting/csharp")
	objdir ("%{outputDir}")

	files
	{
		"**.h",
		"**.cpp",
		"premake5.lua"
	}

	vpaths
	{
		["API"] = { "PhysicsCSAPI.*", "PhysicsComponentsCSAPI.*" },
		["Extenstion"] = { "JoltMonoExtender.*" }
	}

	includedirs
	{
		"%{DepsIncludeDir}",

		"%{BaseIncludeDir.physics}",

		"%{GloryIncludeDir.enginecore}",
		"%{GloryIncludeDir.engine}",
		"%{GloryIncludeDir.jolt}",
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
		"GloryJoltPhysics",
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
		"GLORY_JOLT_MONO_EXPORTS"
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
