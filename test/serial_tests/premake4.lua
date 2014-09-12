solution "MySolution"
   configurations { "Debug", "Release" }

project "ALL_PARFAIT_TESTS.x"
   kind "ConsoleApp"
   language "C++"
   includedirs { "../../include",
   }
   files {
       "../../src/*.cpp",
        "**.cpp",
   }

  excludes {
	}

   links { "CppUTest" } 

   buildoptions{ "--std=c++11",}

   configuration "Debug"
     defines("_DEBUG")
     flags { "Symbols" }

  configuration "Release"
     flags { "Optimize" }
     defines { "NDEBUG" }
     flags { "Symbols" }

premake.gcc.cxx = "g++"
