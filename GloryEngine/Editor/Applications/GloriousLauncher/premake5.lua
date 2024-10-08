project "GloriousLauncher"
	language "C++"
	cppdialect "C++17"
	staticruntime "Off"

	targetdir ("%{buildDir}/Launcher")
	objdir ("%{outputDir}")

	pchheader "pch.h"
	pchsource "pch.cpp"

	files
	{
		"**.h",
		"**.cpp",
		"premake5.lua"
	}

	vpaths
	{
	}

	includedirs
	{
		"%{DepsIncludeDir}",

		"%{GloryIncludeDir.core}",
		"%{IncludeDir.Version}",
		"%{IncludeDir.yaml_cpp}",
		"%{IncludeDir.Reflect}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.FA}",
		"%{IncludeDir.Utils}",
		"%{IncludeDir.ECS}",
		"%{GloryIncludeDir.api}",
		"%{rapidjson}",

		"%{IncludeDir.glm}",
		"%{DepIncludesDir}",

		"%{rootDir}/third-party/tinyfiledialogs"
	}

	libdirs
	{
		"%{DepsLibDir}",

		"%{LibDirs.glory}",

		"%{LibDirs.ImGui}",
		"%{LibDirs.yaml_cpp}",
	}

	links
	{
		"GloryCore",
		"GloryAPI",
		"ImGui",
		"yaml-cpp",
		"tinyfiledialogs",
		"GloryReflect",
		"GloryUtilsVersion",
		"GloryUtils",
		"GloryECS",

		--todo: When asset management is contained in its own lib these links are no more needed
		"GloryJobs",
		"GloryThreads",
		"GloryUtils",
		"SDL2",
	}

	dependson
	{
		"Glorious",
	}

	postbuildcommands
	{
		("{COPY} ./Fonts %{buildDir}/Launcher/Fonts"),
		("{COPY} %{DepsBinDir}/SDL2?.dll %{buildDir}/Launcher"),
		("{COPY} %{DepsBinDir}/glew32*.dll %{buildDir}/Launcher"),
		("{COPY} imgui.ini %{buildDir}/Launcher"),
		("{COPY} %{IncludeDir.FA}/FA %{buildDir}/Launcher/Fonts/FA"),
		("{COPY} %{engineOutDir}/GloryAPI.dll %{buildDir}/Launcher"),
		("{COPY} %{DepsBinDir}/libcurl*.dll %{buildDir}/Launcher"),

		-- Copy editor to launcher
		("{COPY} %{editorBuildDir} %{buildDir}/Launcher/Editor/Any"),

	}

	postbuildcommands
	{
		("{COPY} %{DepsBinDir}/libcurl*.dll %{engineOutDir}"),
	}

	filter "system:windows"
		systemversion "10.0.19041.0"
		toolset "v143"
		files { 'windows.rc', '../Icon/windows.ico' }
		vpaths { ['Resources/*'] = { 'windows.rc', '../Icon/windows.ico' } }

		defines
		{
			"_CONSOLE"
		}

	filter "platforms:Win32"
		architecture "x86"
		defines "WIN32"

		libdirs
		{
			--"%{vulkanDir}/Third-Party/Bin32"
		}

		postbuildcommands
		{
			--("{COPY} %{vulkanDir}/Third-Party/Bin32/*.dll %{buildDir}/Launcher")
		}

	filter "platforms:x64"
		architecture "x64"

		libdirs
		{
			--"%{vulkanDir}/Third-Party/Bin"
		}

		postbuildcommands
		{
			--("{COPY} %{vulkanDir}/Third-Party/Bin/*.dll %{buildDir}/Launcher")
		}

	filter "configurations:Debug"
		kind "ConsoleApp"
		runtime "Debug"
		defines "_DEBUG"
		symbols "On"
		debugdir "%{buildDir}/Launcher"

		links
		{
			"glew32d"
		}

	filter "configurations:Release"
		kind "ConsoleApp"
		runtime "Release"
		defines "NDEBUG"
		optimize "On"

		links
		{
			"glew32"
		}
