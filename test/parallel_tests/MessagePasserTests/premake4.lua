Parfait = "../../../"

solution "MySolution"
   configurations { "Debug", "Release" }

project "TEST_MessagePasser.x"
   kind "ConsoleApp"
   language "C++"
   includedirs { "include",
   Parfait.."include",
        "/usr/local/pkgs-modules/cpputest_3.5/include/",
		"../"
   }
   files {
		"**.cpp",
		"../*cpp",
		Parfait.."src/*cpp",
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
