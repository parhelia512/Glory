project "GloryEntityScenes"
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
		"Module.yaml",
		"premake5.lua",
		"Assets/**.*",
		"Resources/**.*",
	}

	vpaths
	{
		["Module"] = { "GloryEntityScenes.*", "Components.*", "Entity.*", "EntityScene.*", "EntitySceneScenesModule.*", "EntityComponentObject.*", "EntitySceneObject.*" },
		["Serializers"] = { "ScriptedComponentSerializer.*", "EntitySceneSerializer.*", "EntitySceneObjectSerializer.*", "EntityComponentSerializer.*" },
		["Systems"] = { "CharacterControllerSystem.*", "ModelRenderSystem.*", "ScriptedSystem.*", "MeshRenderSystem.*", "Systems.*", "TransformSystem.*", "CameraSystem.*", "LookAtSystem.*", "SpinSystem.*", "LightSystem.*", "MeshFilterSystem.*", "PhysicsSystem.*" },
		["Helpers"] = { "ComponentTypes.*" },
	}

	includedirs
	{
		"%{DepsIncludeDir}",

		"%{GloryIncludeDir.core}",
		"%{GloryIncludeDir.threads}",
		"%{GloryIncludeDir.jobs}",

		"%{IncludeDir.yaml_cpp}",
		"%{IncludeDir.glm}",

		"%{IncludeDir.ECS}",
		"%{IncludeDir.Reflect}",
		"%{IncludeDir.Version}",
	}

	libdirs
	{
		"%{DepsLibDir}",

		"%{LibDirs.glory}",
		"%{LibDirs.yaml_cpp}",
	}

	links
	{
		"GloryCore",
		"shaderc",
		"shaderc_combined",
		"shaderc_shared",
		"yaml-cpp",

		"GloryECS",
		"GloryReflect",
		"GloryUtilsVersion",

		--todo: When asset management is contained in its own lib these links are no more needed
		"GloryJobs",
		"GloryThreads",
	}

	defines
	{
		"GLORY_EXPORTS",
		"GLORY_UUID_DEFINED"
	}

	postbuildcommands
	{
		("{COPY} Module.yaml %{moduleOutDir}"),
		--("{COPY} ./Assets %{moduleOutDir}/Assets"),
		--("{COPY} ./Resources %{moduleOutDir}/Resources"),
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

	filter "configurations:Release"
		runtime "Release"
		defines "NDEBUG"
		optimize "On"