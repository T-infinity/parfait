solution "MySolution"
   configurations { "Debug", "Release" }

project "TEST_VtkSurface.x"
   kind "ConsoleApp"
   language "C++"
   includedirs { "include",
        "/usr/local/pkgs-modules/cpputest_3.5/include/",
		"../",
        "../../mesh_interfaces/",
        "../../message_passer/",
		"../../hex/",
		"../../point/",
		"../../extent/",
		"../../box/",
		"../../range_loop/",
   }
   files {
        "AllTests.cpp",
		"**.cpp",
		"../*cpp",
		"../../message_passer/*cpp",
		"../../mesh_interfaces/*cpp",		
		"../../extent/*cpp",
		"../../box/*cpp",
		"../../point/*cpp",
   }

  excludes {
	}

   libdirs {"/usr/local/pkgs-modules/cpputest_3.5/lib/"}

   links { "CppUTest" } 

   buildoptions{ "--std=c++11 -g"}

   configuration "Debug"
     defines("_DEBUG")
     flags { "Symbols" }

  configuration "Release"
     flags { "Optimize" }
     defines { "NDEBUG" }
     flags { "Symbols" }

premake.gcc.cxx = "mpic++"
