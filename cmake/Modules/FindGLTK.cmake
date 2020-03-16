# - Try to find GLTK
# Once done this will define
# GLTK_FOUND - System has GLTK
# GLTK_INCLUDE_DIR - The GLTK include directories
# GLTK_LIBRARY - The libraries needed to use GLTK
# GLTK - the target to link with

find_path(GLTK_INCLUDE_DIR GLTK/gltk.h)
find_library(GLTK_LIBRARY NAMES GLTK)

find_package(glfw3 3.2.1 EXACT REQUIRED)

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set GLTK_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(
    GLTK
    DEFAULT_MSG
    GLTK_INCLUDE_DIR
    GLTK_LIBRARY
)

if(GLTK_FOUND AND NOT TARGET GLTK)
    # Create imported target GLTK
    add_library(GLTK INTERFACE IMPORTED)

    list(APPEND GLTK_INCLUDE_DIR
        "${GLTK_INCLUDE_DIR}/GLTK")

    set_target_properties(GLTK PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${GLTK_INCLUDE_DIR}"
        INTERFACE_LINK_LIBRARIES "${GLTK_LIBRARY};glfw"
    )
endif()
