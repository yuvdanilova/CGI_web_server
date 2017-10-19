cmake_minimum_required(VERSION 3.3)
project(cgi)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    cgi.h
    form_not_found_response.cpp
    form_response.cpp
    form_static_response.cpp
    get_mime.cpp
    main.cpp
    parse.cpp
    README.md)

add_executable(cgi ${SOURCE_FILES})