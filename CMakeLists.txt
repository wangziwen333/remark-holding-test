cmake_minimum_required(VERSION 3.0)
project(remark_holding)

# 清空build目录，保证CMakeCache.txt为最新
file(REMOVE_RECURSE ${PROJECT_SOURCE_DIR}/build/*)
message(STATUS "clean build file")

#debug或release模式
if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE "Debug") 
endif()
message(STATUS "Build " ${CMAKE_BUILD_TYPE})
add_compile_options(-std=c++11)
message(STATUS "optional: -std=c++11")  

#程序名
set(OUT_NAME remark_holding_test)
#可执行程序
file(GLOB_RECURSE SOURCE_FILES "./src/*.h" "./src/*.cc")
add_executable(${OUT_NAME} ${SOURCE_FILES})

if(CMAKE_COMPILER_IS_GNUCXX)
	find_package(Threads REQUIRED)
	target_link_libraries(${OUT_NAME} Threads::Threads)
endif()
