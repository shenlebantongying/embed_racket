cmake_minimum_required(VERSION 3.26)
project(embed_racket)

set(CMAKE_CXX_STANDARD 17)

find_package(Iconv)

find_package(PkgConfig REQUIRED)
pkg_check_modules(ALL_THINGS REQUIRED IMPORTED_TARGET zlib liblz4 ncurses)

set(T_arch tarm64osx)
set(Chez_Dir ${CMAKE_SOURCE_DIR}/ChezScheme/${T_arch}/boot/${T_arch})
add_executable(cas main.cpp)

set_target_properties(cas
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

target_link_libraries(cas
        ${Chez_Dir}/libkernel.a
        Iconv::Iconv
        PkgConfig::ALL_THINGS
)

target_include_directories(cas PRIVATE ${Chez_Dir})

configure_file(${Chez_Dir}/petite.boot ${CMAKE_BINARY_DIR}/bin/petite.boot COPYONLY)
configure_file(${Chez_Dir}/scheme.boot ${CMAKE_BINARY_DIR}/bin/scheme.boot COPYONLY)

