cmake_minimum_required(VERSION 3.20)
project(c)

set(CMAKE_CXX_STANDARD 17)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK REQUIRED gtk4)
pkg_check_modules(HDY REQUIRED libadwaita-1)
pkg_check_modules(XML REQUIRED libxml++-2.6)
set(CPR_FORCE_USE_SYSTEM_CURL OFF)
set(CPR_BUILD_TESTS OFF)

add_subdirectory(cpr)

include_directories(${GTK_INCLUDE_DIRS} ${GTKMM_INCLUDE_DIRS} ${HDY_INCLUDE_DIRS} ${XML_INCLUDE_DIRS} ${GST_INCLUDE_DIRS})
link_directories(${GTK_LIBRARY_DIRS} ${GTKMM_LIBRARY_DIRS} ${HDY_LIBRARY_DIRS} ${XML_LIBRARY_DIRS} ${GST_LIBRARY_DIRS})

add_executable(c main.c youtube/Channel.cpp youtube/Channel.h youtube/Video.cpp youtube/Video.h youtube/Subscriptions.cpp youtube/Subscriptions.h youtube/CSubs.cpp youtube/CSubs.h gui/MainApplication.c gui/MainApplication.h)

add_definitions(${GTK_CFLAGS_OTHER} ${GTKMM_CFLAGS_OTHER} ${HDY_CFLAGS_OTHER} ${XML_CFLAGS_OTHER} ${GST_CFLAGS_OTHER})
target_link_libraries(c ${GTK_LIBRARIES} ${GTKMM_LIBRARIES} ${HDY_LIBRARIES} ${XML_LIBRARIES} ${GST_LIBRARIES} cpr::cpr)