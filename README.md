Parfait is an unstructured mesh toolkit developed at NASA Langley.


# Building
The Parfait toolkit is header only. If you just want to use the C++ toolkit simply

```
mkdir build
cd build
cmake .. -DHEADER_ONLY=TRUE
make -j install
```

## nlohmann_json
If not strictly installing header files Parfait requires nlohmann_json (https://github.com/nlohmann/json).
You can install nlohmann::json using their provided cmake based build system, and add the `Findnlohmann_json.cmake` path to your `CMAKE_PREFIX_PATH` environment variable.
Or you can install their single header in an `nlohmann` folder somewhere on your machine, and build with:

```
mkdir build
cd build
cmake .. -Dnlohmann_json_path=/path/to/nlohmann -DBUILD_TESTS=FALSE
make -j install
```
Note: Your path _must_ point to the folder containing the nlohmann folder.  Do not point to json.hpp.

This will install the Parfait header files, and some of the Parfait utilities.  
Some Parfait utilities require the VTK libraries to be installed, and for the FindVTK.cmake file to be discoverable by cmake.

## Unit Tests
To enable building Parfait's unit tests you must have Catch2 installed. 
You can install Catch2 using their provided cmake and add the FindCatch2.cmake path to your `CMAKE_PREFIX_PATH` environment variable.
Or, you can simple install the Catch2 catch.hpp header file, and build with:

```
mkdir build
cd build
cmake .. -DCatch2_path=/path/to/catch.hpp
make -j install
```

