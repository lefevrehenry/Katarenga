# - Try to find GLTK
# Once done this will define
# GLTK_FOUND - System has GLTK
# GLTK_INCLUDE_DIRS - The GLTK include directories
# GLTK_LIBRARIES - The libraries needed to use GLTK
# GLTK_DEFINITIONS - Compiler switches required for using GLTK

find_path(_GLTK_INCLUDE_DIR GLTK/gltk.h)
find_library(_GLTK_LIBRARY NAMES GLTK)

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set GLTK_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(
    GLTK
    DEFAULT_MSG
    _GLTK_INCLUDE_DIR
    _GLTK_LIBRARY
)

if(GLTK_FOUND)
    # Create imported target GLTK
    add_library(GLTK SHARED IMPORTED)

    set_target_properties(GLTK PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${_GLTK_INCLUDE_DIR}"
        INTERFACE_LINK_LIBRARIES "${_GLTK_LIBRARY}"
    )
endif()

unset(_GLTK_INCLUDE_DIR CACHE)
unset(_GLTK_LIBRARY CACHE)
