solution "MySolution"
   configurations { "Debug", "Release" }

project "TEST_VtkField"
   kind "ConsoleApp"
   language "C++"
   includedirs { "include",
        "/usr/local/pkgs-modules/cpputest_3.5/include/",
        "../../message_passer/",
        "../../extent/",
        "../../hex/",
        "../../box/",
        "../../point/",
		"../"
   }
   files {
		"**.cpp",
		"../*.cpp",
        "../../message_passer/*.cpp",
        "../../extent/*.cpp",
        "../../hex/*.cpp",
        "../../box/*.cpp",
        "../../point/*.cpp",
   }

  excludes {
	}

   libdirs{ "/usr/local/pkgs-modules/cpputest_3.5/lib/"}

   links { "CppUTest" } 

   buildoptions{ "--std=c++11 -g"}

   configuration "Debug"
     defines("_DEBUG")
     flags { "Symbols" }

  configuration "Release"
     flags { "Optimize" }
     flags { "Symbols" }
     defines { "NDEBUG" }

premake.gcc.cxx = "mpic++"
