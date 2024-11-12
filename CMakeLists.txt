cmake_minimum_required(VERSION 3.10)

# Set the project name and version
project(TM1814 VERSION 1.0)
# Specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_compile_definitions(_WIN32)

# Collect all source files
file(GLOB_RECURSE SOURCE_FILES "source/*.cpp")

# Add the shared library target
add_library(TM1814 SHARED ${SOURCE_FILES})

# Link the static library
target_link_libraries(TM1814 PRIVATE "${CMAKE_SOURCE_DIR}/AnalyzerSDK-master/lib_x86_64/Analyzer.lib")

# Specify include directories
target_include_directories(TM1814 PUBLIC 
        "${CMAKE_SOURCE_DIR}/AnalyzerSDK-master/include"
        )

# Set the generator for Visual Studio
set(CMAKE_GENERATOR "Visual Studio 17 2022" CACHE INTERNAL "" FORCE)

# Add a custom command to copy the DLL to c:\data
#add_custom_command(TARGET TM1814 POST_BUILD
#        COMMAND ${CMAKE_COMMAND} -E echo "Copying $<TARGET_FILE:TM1814> to C:/Program Files/Logic/resources/windows-x64/Analyzers"
#        COMMAND ${CMAKE_COMMAND} -E sleep 1
#        COMMAND powershell -Command "Start-Process cmd -ArgumentList '/c copy \"$<TARGET_FILE:TM1814>\" \"C:/Program Files/Logic/resources/windows-x64/Analyzers\"' -Verb runAs"
#)