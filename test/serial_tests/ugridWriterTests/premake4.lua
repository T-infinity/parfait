Parfait = "/ump/fldmd/home/cdruyorj/Projects/mesh-tools/src/Parfait/"
Vis = "/ump/fldmd/home/cdruyorj/Projects/mesh-tools/src/Parfait-vis/"
Parallel = "/ump/fldmd/home/cdruyorj/Projects/mesh-tools/src/Parfait-parallel/"
yoga = "/ump/fldmd/home/cdruyorj/Projects/yoga/"
xml = "/ump/fldmd/home/cdruyorj/Projects/mesh-tools/src/tinyxml/"

solution "MySolution"
   configurations { "Debug", "Release" }

project "TEST_ugridWriter.x"
   kind "ConsoleApp"
   language "C++"
   includedirs { "include",
			Parfait.."include",
			Vis.."include",
			Parallel.."include",
			yoga.."include",
			xml,
   }
   files {
		"**.cpp",
		Parfait.."src/*cpp",
       Vis.."src/*cpp",
       --Parallel.."src/*cpp",
       yoga.."src/*cpp",
   }

  excludes {
	}

   libdirs {
		--"/usr/local/pkgs-modules/ParMETIS_4.0.3-openmpi_1.6.5_intel_2013-intel.2013.4.183_64/lib",
		--"/misc/local/pkgs-modules/intel_2013.0.079/composer_xe_2013_sp1.3.174/compiler/lib/intel64",
		xml,
			}

   links { 
			"CppUTest", 
			--"parmetis",
			--"metis",
			--"irc",
			--"imf",
			"tinyxml",
		}

   buildoptions{ "--std=c++11 -g"}

   configuration "Debug"
     defines{"_DEBUG",
				--"PARFAIT_WITH_PARMETIS",
			}
     flags { "Symbols", }

  configuration "Release"
     flags { "Optimize", }
     defines { "NDEBUG",
				--"PARFAIT_WITH_PARMETIS",
			}
premake.gcc.cxx = "mpic++"
