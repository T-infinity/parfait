solution "MySolution"
   configurations { "Debug", "Release" }

project "TEST_ParallelMeshReDistributor"
   kind "ConsoleApp"
   language "C++"
   includedirs { "include",
        "/usr/local/pkgs-modules/cpputest_3.5/include/",
		"../",
		"../../mesh_io",
		"../../point",
		"../../extent",
		"../../high_level_mesh_searching",
		"../../trees/adt",
		"../../mesh_interfaces",
		"../../message_passer",
		"../../connectivity",
		"../../vectors",
		"../../visualization",
		"../../partitioning",
		"../../range_loop",
		"../../binary",

   }
   files {
        "AllTests.cpp",
		"**.cpp",
		"../*cpp",
		"../../mesh_io/*.cpp",
		"../../point/*.cpp",
		"../../extent/*.cpp",
		"../../message_passer/*.cpp",
		"../../mesh_interfaces/*.cpp",
		"../../partitioning/*cpp",
		"../../binary/*cpp",
   }

  excludes {
	}

   libdirs {"/usr/local/pkgs-modules/cpputest_3.5/lib/",
   			"/usr/local/pkgs-modules/ParMETIS_4.0.3-openmpi_1.6.5_intel_2013-intel.2013.4.183_64/lib",
			"/misc/local/pkgs-modules/intel_2013.0.079/composer_xe_2013_sp1.3.174/compiler/lib/intel64",
   }

   links { "CppUTest",
   		   "parmetis",
	   	   "metis",
		   "irc",
		   "imf",
	   } 

   buildoptions{ "--std=c++11 -g"}

   configuration "Debug"
     defines("_DEBUG")
     flags { "Symbols" }

  configuration "Release"
     flags { "Optimize" }
     defines { "NDEBUG" }

premake.gcc.cxx = "mpic++"
