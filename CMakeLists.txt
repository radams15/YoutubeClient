cmake_minimum_required(VERSION 3.20)
project(Youtube)

set(CMAKE_CXX_STANDARD 17)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK REQUIRED gtk4)
pkg_check_modules(CURL REQUIRED libcurl)
pkg_check_modules(GIO REQUIRED gio-2.0)
pkg_check_modules(HDY REQUIRED libadwaita-1)
pkg_check_modules(XML REQUIRED libxml++-2.6)

include_directories(${CURL_INCLUDE_DIRS} ${GTK_INCLUDE_DIRS} ${GIO_INCLUDE_DIRS} ${GTKMM_INCLUDE_DIRS} ${HDY_INCLUDE_DIRS} ${XML_INCLUDE_DIRS} ${GST_INCLUDE_DIRS})
link_directories(${CURL_LIBRARY_DIRS} ${GTK_LIBRARY_DIRS} ${GIO_LIBRARY_DIRS} ${GTKMM_LIBRARY_DIRS} ${HDY_LIBRARY_DIRS} ${XML_LIBRARY_DIRS} ${GST_LIBRARY_DIRS})

add_executable(Youtube main.c youtube/Channel.cpp youtube/Channel.h youtube/Video.cpp youtube/Video.h youtube/Subscriptions.cpp youtube/Subscriptions.h youtube/CSubs.cpp youtube/CSubs.h gui/MainApplication.c gui/MainApplication.h youtube/Net.cpp youtube/Net.h)

add_definitions(${GTK_CFLAGS_OTHER} ${GIO_CFLAGS_OTHER} ${GTKMM_CFLAGS_OTHER} ${HDY_CFLAGS_OTHER} ${XML_CFLAGS_OTHER} ${GST_CFLAGS_OTHER})
target_link_libraries(Youtube ${CURL_LIBRARIES} ${GTK_LIBRARIES} ${GIO_LIBRARIES} ${GTKMM_LIBRARIES} ${HDY_LIBRARIES} ${XML_LIBRARIES} ${GST_LIBRARIES})

install(TARGETS Youtube
        RUNTIME DESTINATION bin)

#install(FILES ${CMAKE_BINARY_DIR}/_deps/curl-build/lib/libcurl-d.so ${CMAKE_BINARY_DIR}/cpr/cpr/libcpr.so.1
#        RUNTIME DESTINATION lib)