solution "MySolution"
   configurations { "Debug", "Release" }

project "TEST_VtkGeneric.x"
   kind "ConsoleApp"
   language "C++"
   includedirs { "include",
        "/usr/local/pkgs-modules/cpputest_3.5/include/",
        "../../mesh_interfaces/",
        "../../message_passer/",
        "../../extent/",
        "../../hex/",
        "../../box/",
        "../../point/",
        "../../range_loop/",
		"../"
   }
   files {
       "*.cpp",
		"../*.cpp",
        "../../mesh_interfaces/*.cpp",
        "../../message_passer/*.cpp",
        "../../extent/*.cpp",
        "../../hex/*.cpp",
        "../../box/*.cpp",
        "../../point/*.cpp",
   }

  excludes {
      "../basic_elem_vtk_writer.cpp",
      "../vtk_cart_block_writer.cpp"
	}

   libdirs{ "/usr/local/pkgs-modules/cpputest_3.5/lib/"}

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
