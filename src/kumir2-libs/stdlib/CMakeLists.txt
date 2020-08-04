project(VM)
cmake_minimum_required(VERSION 3.0)

find_package(Kumir2 REQUIRED)

set(SOURCES
	kcore.cpp
	kfiles.cpp
	kmath.cpp
)

kumir2_add_library(
	NAME        KStdlib
	SOURCES     ${SOURCES}
	LIBRARIES
)
