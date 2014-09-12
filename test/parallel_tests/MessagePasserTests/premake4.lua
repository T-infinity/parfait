Parallel = "/ump/fldmd/home/cdruyorj/Projects/mesh-tools/src/Parfait-parallel/"
Parfait = "/ump/fldmd/home/cdruyorj/Projects/mesh-tools/src/Parfait/"

solution "MySolution"
   configurations { "Debug", "Release" }

project "TEST_MessagePasser.x"
   kind "ConsoleApp"
   language "C++"
   includedirs { "include",
   Parfait.."include",
   	Parallel.."include",
        "/usr/local/pkgs-modules/cpputest_3.5/include/",
		"../"
   }
   files {
		"**.cpp",
		"../*cpp",
		Parfait.."src/*cpp",
   		Parallel.."src/*cpp",
   }

  excludes {
	}

   libdirs{ "/usr/local/pkgs-modules/cpputest_3.5/lib/"}

   links { "CppUTest" } 

   buildoptions{ "--std=c++11 -g",
   "-DPARFAIT_WITH_MPI",
   }

   configuration "Debug"
     defines("_DEBUG")
     flags { "Symbols" }

  configuration "Release"
     flags { "Optimize" }
     defines { "NDEBUG" }
     flags { "Symbols" }

premake.gcc.cxx = "mpic++"
