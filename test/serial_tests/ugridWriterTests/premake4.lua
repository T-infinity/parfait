Parfait = "../../../"

solution "MySolution"
   configurations { "Debug", "Release" }

project "TEST_ugridWriter.x"
   kind "ConsoleApp"
   language "C++"
   includedirs { "include",
			Parfait.."include",
   }
   files {
		"**.cpp",
		Parfait.."src/*cpp",
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
			--"tinyxml",
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
