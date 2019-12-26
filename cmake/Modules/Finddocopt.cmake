# - Try to find docopt
# Once done this will define
# DOCOPT_FOUND - System has docopt
# DOCOPT_INCLUDE_DIR - The docopt include directories
# DOCOPT_LIBRARY - The libraries needed to use docopt
# docopt - the target to link with

find_path(DOCOPT_INCLUDE_DIR docopt/docopt.h)
find_library(DOCOPT_LIBRARY docopt)

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set DOCOPT_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(
    docopt
    DEFAULT_MSG
    DOCOPT_INCLUDE_DIR
    DOCOPT_LIBRARY
)

if(DOCOPT_FOUND)
    # Create imported target docopt
    add_library(docopt INTERFACE IMPORTED)

    set_target_properties(docopt PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${DOCOPT_INCLUDE_DIR}"
        INTERFACE_LINK_LIBRARIES "${DOCOPT_LIBRARY}"
    )
endif()
