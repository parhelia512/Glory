project "GloryEntityScenes2"
	kind "SharedLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "Off"

	targetdir ("%{moduleOutDir}")
	objdir ("%{cfg.buildcfg}/%{cfg.platform}")

	files
	{
		"**.h",
		"**.cpp",
		"Module.yaml"
	}

	vpaths
	{
		["ECS"] = { "EntityComponentData.*", "EntityID.*", "Registry.*" },
		["Module"] = { "Components.*", "Entity.*", "EntityScene.*", "EntitySceneScenesModule.*", "EntityComponentObject.*", "EntitySceneObject.*" },
		["Serializers"] = { "EntitySceneSerializer.*", "EntitySceneObjectSerializer.*", "EntityComponentSerializer.*" },
		["Systems"] = { "EntitySystem.*", "EntitySystems.*", "EntitySystemTemplate.*", "MeshRenderSystem.*", "Systems.*", "TransformSystem.*", "CameraSystem.*", "LookAtSystem.*", "SpinSystem.*", "LightSystem.*", "MeshFilterSystem.*" },
	}

	includedirs
	{
		"%{GloryIncludeDir.core}",
		"%{IncludeDir.yaml_cpp}",
		"%{IncludeDir.shaderc}",
		"%{IncludeDir.spirv_cross}",
		"%{vulkan_sdk}/third-party/include",

		"%{IncludeDir.ECS}",
		"%{IncludeDir.Reflect}",
	}

	libdirs
	{
		"%{LibDirs.glory}",
		"%{LibDirs.assimp}",
		"%{LibDirs.shaderc}",
		"%{LibDirs.spirv_cross}",
		"%{LibDirs.yaml_cpp}",

		"%{LibDirs.GloryECS}",
	}

	links
	{
		"GloryCore",
		"shaderc",
		"shaderc_combined",
		"shaderc_shared",
		"yaml-cpp",

		"GloryECSStatic",
		"GloryReflectStatic",
	}

	defines
	{
		"GLORY_EXPORTS",
		"GLORY_UUID_DEFINED"
	}
	
	postbuildcommands
	{
		("{COPY} Module.yaml %{moduleOutDir}"),
	}

	filter "system:windows"
		systemversion "10.0.19041.0"
		toolset "v142"

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