cmake_minimum_required(VERSION 3.26)
project(embed_racket)

set(CMAKE_CXX_STANDARD 23)

find_package(PkgConfig REQUIRED)
pkg_check_modules(ALL_THINGS REQUIRED IMPORTED_TARGET zlib liblz4 ncurses)

set(RACKET_DIR ${CMAKE_SOURCE_DIR}/racket/racket)

add_executable(cas main.cpp base.c)

set_target_properties(cas
        PROPERTIES
        WIN32_EXECUTABLE TRUE
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

target_link_libraries(cas
        ${RACKET_DIR}/lib/libracketcs.a
        PkgConfig::ALL_THINGS
)

target_include_directories(cas PRIVATE ${RACKET_DIR}/include)

configure_file(${RACKET_DIR}/lib/petite.boot ${CMAKE_BINARY_DIR}/bin/petite.boot COPYONLY)
configure_file(${RACKET_DIR}/lib/scheme.boot ${CMAKE_BINARY_DIR}/bin/scheme.boot COPYONLY)
configure_file(${RACKET_DIR}/lib/racket.boot ${CMAKE_BINARY_DIR}/bin/racket.boot COPYONLY)

