project "GloryLocalize"
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
	}

	vpaths
	{
		["Module"] = { "GloryLocalize.*", "LocalizeModule.*" },
		["Resource"] = { "StringTable.*", "StringsOverrideTable.*" },
	}

	includedirs
	{
		"%{BaseIncludeDir.localize}",

		"%{GloryIncludeDir.enginecore}",
		"%{GloryIncludeDir.engine}",

		"%{IncludeDir.glm}",
		"%{IncludeDir.ECS}",
		"%{IncludeDir.Reflect}",
		"%{IncludeDir.Version}",
		"%{IncludeDir.Utils}",
		"%{IncludeDir.yaml_cpp}",
	}

	libdirs
	{
		"%{LibDirs.glory}",
		"%{LibDirs.yaml_cpp}",
	}

	links
	{
		"GloryEngineCore",
		"GloryEngine",
		"GloryLocalizeModule",
		"GloryReflect",
		"GloryECS",

		"yaml-cpp",

		"GloryUtils",
		"GloryUtilsVersion",
	}

	defines
	{
		"GLORY_LOCALIZE_EXPORTS",
	}

	postbuildcommands
	{
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
		symbols "On"

	filter "configurations:Release"
		runtime "Release"
		defines "NDEBUG"
		optimize "On"
