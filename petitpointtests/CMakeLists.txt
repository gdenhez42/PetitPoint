cmake_minimum_required (VERSION 3.10.2)

# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
	RESULT_VARIABLE result
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
	message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
	RESULT_VARIABLE result
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
	message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
	${CMAKE_CURRENT_BINARY_DIR}/googletest-build
	EXCLUDE_FROM_ALL)

file(GLOB SOURCES "*.cpp")

if (${WIN32})
	link_directories("e:/vc_dev_libs/SDL2-2.0.9/lib/x64"
		"e:/vc_dev_libs/SDL2_ttf-2.0.15/lib/x64"
		"e:/vc_dev_libs/SDL2_image-2.0.4/lib/x64"
		../external/tinyxml2/debug)

	include_directories("e:/vc_dev_libs/SDL2-2.0.9/include"
		"e:/vc_dev_libs/SDL2_image-2.0.4/include"
		"e:/vc_dev_libs/SDL2_ttf-2.0.15/include"
		../petitpoint/include
		../petitpoint/include/tiled
		../external/tinyxml2)

else()
	link_directories(../external/tinyxml2)
	include_directories(../petitpoint/include
		../petitpoint/include/tiled
		../external/tinyxml2)

endif()

add_executable(PetitPointTest ${SOURCES})


if (${WIN32})
	target_link_libraries(PetitPointTest PetitPointLib
		gtest
		gtest_main)
else()
	target_link_libraries(PetitPointTest PetitPointLib
		gtest
		gtest_main
		pthread)
endif()


