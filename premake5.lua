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

project "Core"
  location "Core"
  kinda "ConsoleApp"
  language "C++"
  cppdialect "C++23"

  targetdir("Binaries/" .. outputDir .. "/%{prj.name}")
  objdir("Intermediates/" .. outputDir .. "/%{prj.name}")

  files {
    "%{wks.location}/Core/source/**.h",
    "%{wks.location}/Core/source/**.cpp",
  }

  filter "system:windows"
    staticruntime "off"
    systemversion "latest"