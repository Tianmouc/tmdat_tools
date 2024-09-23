cmake_minimum_required(VERSION 3.0.0)
project("tmdat_cut")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O2")


include_directories(./inc)
set(file1 ./example/main.cpp)
set(file2 ./src/data_cutter.cpp)
set(file3 ./src/utils.cpp)
    

add_executable(tmdat_cut ${file1} ${file2} ${file3} )
target_link_libraries(tmdat_cut PRIVATE pthread)

add_library(tmdat_cut_lib SHARED ${file1} ${file2} ${file3})
target_link_libraries(tmdat_cut PRIVATE pthread)


