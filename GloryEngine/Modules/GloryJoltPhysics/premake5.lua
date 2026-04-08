project "GloryJoltPhysics"
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

		"%{rootDir}/submodules/JoltPhysics/Jolt/**.*",
	}

	vpaths
	{
		["Module"] = { "Physics.*", "GloryJoltPhysics.*", "JoltPhysicsModule.*", "JoltCharacterManager.*", "JoltShapeManager.*" },
		["Collision"] = { "BroadPhaseImpl.*", "LayerCollisionFilter.*", "JoltDebugRenderer.*" },
		["Helpers"] = { "Helpers.*", "ShapeHandlers.*" },
		["ECS"] = { "CharacterControllerSystem.*", "JoltComponents.*", "PhysicsSystem.*" }
	}

	includedirs
	{
		"%{BaseIncludeDir.physics}",
		"%{rootDir}/submodules/JoltPhysics",

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

		"{moduleBaseOutDir}",
	}

	links
	{
		"GloryEngineCore",
		"GloryEngine",
		"GloryPhysicsModule",
		"GloryReflect",
		"GloryECS",

		"yaml-cpp",

		"GloryUtils",
		"GloryUtilsVersion",
	}

	defines
	{
		"GLORY_JOLT_EXPORTS",
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

		defines
		{
			"_DEBUG",
			"JPH_DEBUG_RENDERER"
		}

	filter "configurations:Release"
		runtime "Release"
		defines "NDEBUG"
		optimize "On"
