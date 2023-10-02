project "ASSIMPImporter"
	kind "SharedLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "Off"

	targetdir ("%{moduleOutDir}")
	objdir ("%{outputDir}")

	files
	{
		"**.h",
		"**.cpp",
		"premake5.lua",
		"Assets/**.*",
		"Resources/**.*",
	}

	includedirs
	{
		"%{DepsIncludeDir}",

		"%{DepIncludesRootDir}",
		"%{GloryIncludeDir.core}",
		"%{GloryIncludeDir.editor}",
		"%{IncludeDir.yaml_cpp}",
		"%{IncludeDir.glm}",

		"%{IncludeDir.ImGui}",
		"%{IncludeDir.Reflect}",
		"%{IncludeDir.Version}",
	}

	libdirs
	{
		"%{DepsLibDir}",

		"%{LibDirs.glory}",
		"%{LibDirs.yaml_cpp}",
		"%{LibDirs.ImGui}",
	}

	links
	{
		"GloryCore",
		"yaml-cpp",
		"GloryReflect",
		"GloryUtilsVersion",

		--todo: When asset management is contained in its own lib these links are no more needed
		"GloryJobs",
		"GloryThreads",
		"ImGui",
	}

	defines
	{
		"GLORY_EXPORTS"
	}

	prebuildcommands
	{
		("{DELETE} %{moduleOutDir}/Module.yaml"),
	}

	postbuildcommands
	{
		("{COPY} Module.yaml %{moduleOutDir}"),
		("{COPY} %{DepsBinDir}/assimp-vc143-mt*.dll %{moduleOutDir}/Dependencies"),
		("{COPY} ./Assets %{moduleOutDir}/Assets"),
	}

	filter "system:windows"
		systemversion "10.0.19041.0"
		toolset "v143"

		defines
		{
			"_LIB"
		}

	filter "platforms:Win32"
		architecture "x86"
		defines "WIN32"

	filter "platforms:x64"
		architecture "x64"

	filter "configurations:Debug"
		runtime "Debug"
		defines "_DEBUG"
		symbols "On"

		links "assimp-vc143-mtd"

	filter "configurations:Release"
		runtime "Release"
		defines "NDEBUG"
		optimize "On"

		links "assimp-vc143-mt"
