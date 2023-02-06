workspace "Search Algorithms"
architecture "x86_64"

configurations {
  "Debug",
  "Release",
}

flags {
  "MultiProcessorCompile"
}

outputDir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["ImGui"] =  "%{wks.location}/ThirdParty/ImGui"
IncludeDir["Glfw"] =  "%{wks.location}/ThirdParty/Glfw/include"
IncludeDir["Glad"] =  "%{wks.location}/ThirdParty/Glad/include"
IncludeDir["Glm"] =  "%{wks.location}/ThirdParty/Glm"

group "Dependencies"
    include "ThirdParty/Glfw"
    include "ThirdParty/Glad"
    include "ThirdParty/ImGui"
group ""


project "Core"
  location "Core"
  kind "ConsoleApp"
  language "C++"
  cppdialect "C++20"

  targetdir("Binaries/" .. outputDir .. "/%{prj.name}")
  objdir("Intermediates/" .. outputDir .. "/%{prj.name}")

  pchheader "pch.h"
  pchsource "Core/Source/pch.cpp"

  files {
    "%{wks.location}/Core/Source/**.h",
    "%{wks.location}/Core/Source/**.cpp",
  }

  includedirs {
    "%{wks.location}/Core/Source",
    "%{IncludeDir.Glfw}",
    "%{IncludeDir.Glad}",
    "%{IncludeDir.Glm}",
    "%{IncludeDir.ImGui}",
  }

  links {
    "ImGui",
    "Glfw",
    "Glad",
  }

  filter "system:windows"
    staticruntime "off"
    systemversion "latest"

  filter "configurations:Debug"
    defines { "WIN32_LEAN_AND_MEAN", "GLFW_INCLUDE_NONE" }
    symbols "on"

  filter "configurations:Release"
    defines {  "WIN32_LEAN_AND_MEAN", "GLFW_INCLUDE_NONE" }
    optimize "on"
