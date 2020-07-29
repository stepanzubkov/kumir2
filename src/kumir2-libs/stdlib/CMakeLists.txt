project(VM)
cmake_minimum_required(VERSION 3.0)

find_package(Kumir2 REQUIRED)

set(SOURCES
	kumirstdlib.cpp
)

kumir2_add_library(
	NAME        Stdlib
	SOURCES     ${SOURCES}
	LIBRARIES
)
