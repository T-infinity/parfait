solution "MySolution"
   configurations { "Debug", "Release" }

project "parfait"
   kind "StaticLib"
   language "C++"
   includedirs { "../include",
   }
   files {
       "../src/*.cpp",
        "**.cpp",
   }

  excludes {
	}

   links { } 

   buildoptions{ "--std=c++11"}

   configuration "Debug"
     defines("_DEBUG")
     flags { "Symbols" }

  configuration "Release"
     flags { "Optimize" }
     defines { "NDEBUG" }
     flags { "Symbols" }

--premake.gcc.cxx = "mpic++"
