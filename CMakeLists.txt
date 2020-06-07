cmake_minimum_required (VERSION 2.8)
project(epollftp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g -ggdb -fPIC -w")

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
add_executable(client myclient.cc clijudge.cc my_judge.cc cliftp.cc error.cc my_unp.cc)
add_executable(server myserver.cc servjudge.cc my_judge.cc servftp.cc error.cc my_unp.cc)
