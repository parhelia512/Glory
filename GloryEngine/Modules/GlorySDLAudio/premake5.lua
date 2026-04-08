project "GlorySDLAudio"
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
		["Module"] = { "GlorySDLAudio.*", "SDLAudioModule.*" }
	}

	includedirs
	{
		"%{DepsIncludeDir}",

		"%{BaseIncludeDir.audio}",

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

		"{moduleBaseOutDir}",
	}

	links
	{
		"SDL2",
		"SDL2_mixer",

		"GloryEngineCore",
		"GloryEngine",
		"GloryAudioModule",
		"GloryReflect",
		"GloryECS",

		"yaml-cpp",

		"GloryUtilsVersion",
		"GloryUtils",
	}

	defines
	{
		"GLORY_SDL_AUDIO_EXPORTS"
	}

	postbuildcommands
	{
		("{COPY} %{DepsBinDir}/SDL2_mixer?.dll %{moduleOutDir}/Dependencies"),
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
