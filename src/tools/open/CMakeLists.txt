find_package(Qt4 4.7.0 COMPONENTS QtCore QtGui REQUIRED)
include (${QT_USE_FILE})

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(USED_LIBS ${QT_LIBRARIES})

set(SOURCES 
    main.cpp
    messager.cpp
    dialog.cpp
    appmanager.cpp
    procmanager.cpp
    settings.cpp
)

set(HEADERS
    messager.h
    dialog.h
    appmanager.h
    procmanager.h
    settings.h
)

set(FORMS
    dialog.ui
)

set(MOC_HEADERS
    dialog.h
)

qt4_wrap_cpp(MOC_SOURCES ${MOC_HEADERS})
qt4_wrap_ui(UI_SOURCES ${FORMS})

if(NOT WIN32 AND NOT APPLE)
    set(SOURCES ${SOURCES} messager_unix.cpp procmanager_linux.cpp)
    set(HEADERS ${HEADERS} messager_unix.h procmanager_linux.h)
endif()

if(WIN32)
    add_definitions(-DPSAPI_VERSION=1)
    set(SOURCES ${SOURCES} procmanager_winapi.cpp messager_shm.cpp)
    set(HEADERS ${HEADERS} procmanager_winapi.h messager_shm.h)
    set(USED_LIBS ${USED_LIBS} psapi kernel32)
endif()


add_executable(kumir2-open ${UI_SOURCES} ${MOC_SOURCES} ${SOURCES})
target_link_libraries(kumir2-open ${USED_LIBS} ${STDCXX_LIB})
if (XCODE OR MSVC_IDE)
    set_target_properties (kumir2-open PROPERTIES PREFIX "../")
endif(XCODE OR MSVC_IDE)
install(TARGETS kumir2-open DESTINATION ${EXEC_DIR})