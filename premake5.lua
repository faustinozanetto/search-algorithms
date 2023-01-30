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

group "Dependencies"
    include "ThirdParty/ImGui"
group ""


project "Core"
  location "Core"
  kind "ConsoleApp"
  language "C++"
  cppdialect "C++20"

  targetdir("Binaries/" .. outputDir .. "/%{prj.name}")
  objdir("Intermediates/" .. outputDir .. "/%{prj.name}")

  files {
    "%{wks.location}/Core/Source/**.h",
    "%{wks.location}/Core/Source/**.cpp",
  }

  includedirs {
    "%{IncludeDir.ImGui}",
  }

  links {
    "ImGui"
  }

  filter "system:windows"
    staticruntime "off"
    systemversion "latest"

  filter "configurations:Debug"
    defines { "WIN32_LEAN_AND_MEAN" }
    symbols "on"

  filter "configurations:Release"
    defines {  "WIN32_LEAN_AND_MEAN" }
    optimize "on"
