solution "MySolution"
   configurations { "Debug", "Release" }

project "TEST_ParMetisPrepper"
   kind "ConsoleApp"
   language "C++"
   includedirs { "include",
        "/ump/fldmd/home/cdruyorj/cdruyor_local/cpputest/include",
		"../",
		"../../mesh_io",
		"../../mesh_interfaces",
		"../../message_passer",
		"../../connectivity",
		"../../vectors",
		"../../partitioning",
		"../../range_loop",
		"../../binary/",
		"../../point/",
		"../../extent/",
		"../../high_level_mesh_searching/",
		"../../trees/adt/",
   }
   files {
        "AllTests.cpp",
		"**.cpp",
		"../../*cpp",
		"../../mesh_io/*.cpp",
		"../../message_passer/*.cpp",
		"../../mesh_interfaces/*.cpp",
		"../../partitioning/*.cpp",
		"../../binary/*cpp",
		"../../point/*cpp",
		"../../extent/*cpp",
		"../../trees/adt/*cpp",
   }

  excludes {
	}

   libdirs {"/ump/fldmd/home/cdruyorj/cdruyor_local/cpputest/lib",
   		"/usr/local/pkgs-modules/ParMETIS_4.0.3-openmpi_1.6.5_intel_2013-intel.2013.4.183_64/lib",
		"/misc/local/pkgs-modules/intel_2013.0.079/composer_xe_2013_sp1.3.174/compiler/lib/intel64"
	}

   links { "CppUTest",
    	"parmetis",
    	"metis",
		"irc",
		"imf"
	} 

   buildoptions{ "--std=c++11 -g"}

   configuration "Debug"
     defines("_DEBUG")
     flags { "Symbols" }

  configuration "Release"
     flags { "Optimize" }
     defines { "NDEBUG" }

premake.gcc.cxx = "mpic++"
