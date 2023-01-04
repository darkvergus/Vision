project "VisionCore"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	files { "**.h", "**.inl", "**.cpp" }
	includedirs { "include", dependdir .. "glfw/include", dependdir .. "stb_image/include", dependdir .. "lua/include", dependdir .. "bullet3/include", dependdir .. "glew/include", dependdir .. "irrklang/include",
	"%{wks.location}/VisionAnalytics/include", "%{wks.location}/VisionAudio/include", "%{wks.location}/VisionDebug/include", "%{wks.location}/VisionMaths/include", "%{wks.location}/VisionPhysics/include",
	"%{wks.location}/VisionRendering/include", "%{wks.location}/VisionTools/include", "%{wks.location}/VisionUI/include", "%{wks.location}/VisionWindowing/include" }

	targetdir (outputdir .. "%{cfg.buildcfg}/%{prj.name}")
	objdir (objoutdir .. "%{cfg.buildcfg}/%{prj.name}")
	characterset ("MBCS")
	
	buildoptions { "/bigobj" }

	filter { "configurations:Debug" }
		defines { "DEBUG" }
		symbols "On"

	filter { "configurations:Release" }
		defines { "NDEBUG" }
		optimize "On"