-- premake5.lua
workspace "LearningRT"
   architecture "x64"
   configurations { "Debug", "Release", "Dist" }
   startproject "LearningRT"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
include "Walnut/WalnutExternal.lua"

include "LearningRT"