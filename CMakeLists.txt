cmake_minimum_required (VERSION 3.15)
project(rusm)

set(CMAKE_C_STANDARD 11)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)

add_executable(rusm src/main.c
    src/translator.c
    src/translator.h
    src/cross.c
    src/cross.h
)

install(TARGETS rusm
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)