cmake_minimum_required(VERSION 3.1)
project(ezipc)


#################################################################
# CONFIGURE LIBRARY BUILD

FILE(GLOB_RECURSE EZIPC_SOURCE "src/*.cpp" "src/*.c")
add_library(ezipc STATIC ${EZIPC_SOURCE})

target_include_directories(ezipc PUBLIC "./include")
target_compile_features(ezipc PUBLIC cxx_std_20)

#################################################################
# CONFIGURE TEST BUILD

FILE(GLOB_RECURSE EZIPC_TEST "tests/*.cpp" "tests/*.c")
add_executable(ezipc-tests ${EZIPC_TEST})

add_dependencies(ezipc-tests ezipc)
target_link_libraries(ezipc-tests ezipc)
target_compile_features(ezipc-tests PUBLIC cxx_std_20)
