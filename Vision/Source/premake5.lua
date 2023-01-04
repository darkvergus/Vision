workspace "Vision"
	configurations { "Debug", "Release" }
	platforms { "x64" }
	startproject "VisionEditor"

outputdir = "%{wks.location}/../../Bin/"
objoutdir = "%{wks.location}/../../Bin-Int/"
dependdir = "%{wks.location}/../../Vendor/"

include "VisionAnalytics"
include "VisionAudio"
include "VisionCore"
include "VisionDebug"
include "VisionMaths"
include "VisionPhysics"
include "VisionRendering"
include "VisionTools"
include "VisionUI"
include "VisionWindowing"

include "VisionEditor"
include "VisionGame"