# - Try to find ZMQ
# Once done this will define
# - ZMQ_FOUND
# - a target 'ZMQ'

find_path(_ZMQ_INCLUDE_DIR zmq.h)
find_library(_ZMQ_LIBRARY NAMES zmq)

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set ZMQ_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(
    ZMQ
    DEFAULT_MSG
    _ZMQ_INCLUDE_DIR
    _ZMQ_LIBRARY
)

if(ZMQ_FOUND)
    # Create imported target ZMQ
    add_library(ZMQ SHARED IMPORTED)

    set_target_properties(ZMQ PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${_ZMQ_INCLUDE_DIR}"
        INTERFACE_LINK_LIBRARIES "${_ZMQ_LIBRARY}"
    )
endif()

# unset(_ZMQ_INCLUDE_DIR CACHE)
# unset(_ZMQ_LIBRARY CACHE)
