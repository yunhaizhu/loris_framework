MACRO(SUBDIRLIST result curdir)
    FILE(GLOB children RELATIVE ${curdir} ${curdir}/*)
    SET(dirlist "")
    FOREACH (child ${children})
        IF (IS_DIRECTORY ${curdir}/${child})
            LIST(APPEND dirlist ${child})
        ENDIF ()
    ENDFOREACH ()
    SET(${result} ${dirlist})
ENDMACRO()

MACRO(SUBDIR_PUBLIC_INCLUDE_LIST result curdir)
    FILE(GLOB children RELATIVE ${curdir} ${curdir}/*)
    message(STATUS "children: ${children}")
    SET(dirlist "")
    FOREACH (child ${children})
        IF (IS_DIRECTORY ${curdir}/${child})
            LIST(APPEND dirlist ${CMAKE_CURRENT_SOURCE_DIR}/${child}/public/include)
        ENDIF ()
    ENDFOREACH ()
    SET(${result} ${dirlist})
ENDMACRO()

SUBDIRLIST(SUBDIRS ${CMAKE_CURRENT_SOURCE_DIR})
SUBDIR_PUBLIC_INCLUDE_LIST(SUB_PUBLIC_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR})

message(STATUS "SUBDIRS: ${SUBDIRS}")
message(STATUS "SUB_PUBLIC_INCLUDE_DIRS: ${SUB_PUBLIC_INCLUDE_DIRS}")

FOREACH (subdir ${SUBDIRS})
    ADD_SUBDIRECTORY(${subdir})
ENDFOREACH ()

