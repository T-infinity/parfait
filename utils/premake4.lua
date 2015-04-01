solution "MySolution"
   configurations { "Debug", "Release" }

project "ugrid_viewer"
   kind "ConsoleApp"
   language "C++"
   includedirs { "../src",
   }
   files {
       "../src/*.cpp",
        "ugrid_viewer.cpp",
   }

  excludes {
	}

   buildoptions{ "--std=c++11", "-fprofile-arcs", "-ftest-coverage"}
   linkoptions{ "-fprofile-arcs", "-ftest-coverage"}


   configuration "Debug"
     defines("_DEBUG")
     flags { "Symbols" }

  configuration "Release"
     flags { "Optimize" }
     defines { "NDEBUG" }
     flags { "Symbols" }

project "ugrid_surface_viewer"
    kind "ConsoleApp"
    language "C++"
   includedirs { "../src",
   }
   files {
       "../src/*.cpp",
        "ugrid_surface_viewer.cpp",
   }

  excludes {
	}

   buildoptions{ "--std=c++11", "-fprofile-arcs", "-ftest-coverage"}
   linkoptions{ "-fprofile-arcs", "-ftest-coverage"}


   configuration "Debug"
     defines("_DEBUG")
     flags { "Symbols" }

  configuration "Release"
     flags { "Optimize" }
     defines { "NDEBUG" }
     flags { "Symbols" }


premake.gcc.cxx = "g++"
