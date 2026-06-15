cmake_minimum_required (VERSION 3.15)
project(rusm)

set(CMAKE_C_STANDARD 11)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
find_package(Gettext REQUIRED)

add_executable(rusm src/main.c
    src/translator.c
    src/translator.h
    src/cross.c
    src/cross.h
)

if(GETTEXT_FOUND)
	set(LANGUAGES ru)

	foreach(LANG ${LANGUAGES})
           set(PO_FILE "${CMAKE_CURRENT_SOURCE_DIR}/po/${LANG}.po")
	   set(MO_DIR "${CMAKE_CURRENT_BINARY_DIR}/locale/${LANG}/LC_MESSAGES")
	   set(MO_FILE "${MO_DIR}/rusm.mo")

	   file(MAKE_DIRECTORY ${MO_DIR})

	   add_custom_command(
		OUTPUT ${MO_FILE}
		COMMAND ${GETTEXT_MSGFMT_EXECUTABLE} -o ${MO_FILE} ${PO_FILE}
		DEPENDS ${PO_FILE}
	   )
	   list (APPEND MO_FILES ${MO_FILE})
	   endforeach()
	   add_custom_target(translations ALL DEPENDS ${MO_FILES})
	   add_dependencies(rusm translations)
endif()


install(TARGETS rusm
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
