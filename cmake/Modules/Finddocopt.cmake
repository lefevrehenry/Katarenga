# Try to find docopt and validate that it is installed as it should be
# Once done it will define
# - DOCOPT_FOUND
# - a target 'docopt'

# We won't be using PkgConfig and maybe this should change in the future,
# all we are about to do is check if we can find the file <docopt/docopt.h> and
# the libdocopt library (static or shared we don't care)

find_path(_DOCOPT_INCLUDE_DIR docopt/docopt.h)
find_library(_DOCOPT_LIBRARY docopt)

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set DOCOPT_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(
    docopt
    DEFAULT_MSG
    _DOCOPT_INCLUDE_DIR
    _DOCOPT_LIBRARY
)

if(DOCOPT_FOUND)
    # Create imported target docopt
    add_library(docopt SHARED IMPORTED)

    set_target_properties(docopt PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${_DOCOPT_INCLUDE_DIR}"
        INTERFACE_LINK_LIBRARIES "${_DOCOPT_LIBRARY}"
    )
endif()

unset(_DOCOPT_INCLUDE_DIR CACHE)
unset(_DOCOPT_LIBRARY CACHE)
