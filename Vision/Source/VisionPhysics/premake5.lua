project "VisionPhysics"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	files { "**.h", "**.inl", "**.cpp" }
	includedirs { "include", dependdir .. "bullet3/include", "%{wks.location}/VisionDebug/include", "%{wks.location}/VisionMaths/include", "%{wks.location}/VisionTools/include" }
	targetdir (outputdir .. "%{cfg.buildcfg}/%{prj.name}")
	objdir (objoutdir .. "%{cfg.buildcfg}/%{prj.name}")
	characterset ("MBCS")

	filter { "configurations:Debug" }
		defines { "DEBUG" }
		symbols "On"

	filter { "configurations:Release" }
		defines { "NDEBUG" }
		optimize "On"