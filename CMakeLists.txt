cmake_minimum_required(VERSION 3.14)

project(BubbleHawking VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 17)
set(RAYLIB_VERSION 5.5.0)

if(UNIX)
    set(CMAKE_GENERATOR "Unix Makefiles")
endif()

#find_package(EspeakNG REQUIRED)

# Find Raylib
find_package(raylib ${RAYLIB_VERSION} QUIET)
if( NOT raylib_FOUND)
	include(FetchContent)
	FetchContent_Declare(
		raylib 
		DOWNLOAD_EXTRACT_TIMESTAMP OFF 
		URL https://github.com/raysan5/raylib/archive/refs/tags/5.5.tar.gz
	)
	FetchContent_GetProperties(raylib)
	if(NOT raylib_POPULATED)
		set(FETCHCONTENT_QUIET NO)
		FetchContent_MakeAvailable(raylib)
		set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
	endif()
endif()

file(GLOB SOURCES
	src/*.cpp
)

# Create an executable
add_executable(bubbleHawking ${SOURCES})

# Link Raylib to the executable
target_link_libraries(bubbleHawking raylib)
target_link_libraries(bubbleHawking espeak-ng)
