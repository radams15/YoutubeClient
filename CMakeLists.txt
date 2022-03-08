cmake_minimum_required(VERSION 3.20)
project(Youtube)

set(CMAKE_CXX_STANDARD 17)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK REQUIRED gtk4)
pkg_check_modules(CURL REQUIRED libcurl)
pkg_check_modules(GIO REQUIRED gio-2.0)

if(${FLATPAK})
    add_compile_definitions(FLATPAK)
    add_compile_definitions(INSTALLED="/app/")
    add_compile_definitions(CONF_DIR="/.var/app/com.chev.Youtube/data/yt_saves.json")
else()
    add_compile_definitions(INSTALLED="/usr/local/")
    add_compile_definitions(CONF_DIR="/.local/share/yt_saves.json")
endif()

add_subdirectory(libYT)

include_directories(${YT_HEADER_DIR} ${GTK_INCLUDE_DIRS} ${GIO_INCLUDE_DIRS} ${HDY_INCLUDE_DIRS})
link_directories(${GTK_LIBRARY_DIRS} ${GIO_LIBRARY_DIRS} ${HDY_LIBRARY_DIRS})

add_executable(Youtube main.c gui/MainApplication.c gui/MainApplication.h)

add_definitions(${GTK_CFLAGS_OTHER} ${GIO_CFLAGS_OTHER} ${HDY_CFLAGS_OTHER})
target_link_libraries(Youtube yt ${GTK_LIBRARIES} ${GIO_LIBRARIES} ${HDY_LIBRARIES})

install(TARGETS Youtube
        RUNTIME DESTINATION bin)

install(FILES ui/FeedBox.ui ui/MainWindow.ui ui/SubBox.ui DESTINATION share/Youtube)
