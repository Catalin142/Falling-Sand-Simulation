workspace "Falling Sand Simulation"
	architecture "x64"
	configurations { "Debug", "Release" }
	startproject "Falling Sand Simulation"
	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include "Falling Sand Simulation"
