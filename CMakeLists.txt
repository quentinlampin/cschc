cmake_minimum_required(VERSION 3.14)

set(CMAKE_C_FLAGS_DEBUG_INIT "-Wall -g -fsanitize=address")
set(CMAKE_C_FLAGS_RELWITHDEBINFO_INIT "-Wall -g -fsanitize=address")
set(CMAKE_C_FLAGS_RELEASE_INIT "-Wall")


# library declaration
project(cschc 
    VERSION 0.1.0 
    DESCRIPTION "C implementation of Static Context Header Compression (SCHC), RFC 8724"
    LANGUAGES C
)

include_directories(
    ${PROJECT_SOURCE_DIR}/include/
    ${PROJECT_SOURCE_DIR}/include/protocol/
)

add_library(cschc 
    ${PROJECT_SOURCE_DIR}/source/cschc.c
    ${PROJECT_SOURCE_DIR}/source/buffer.c
    ${PROJECT_SOURCE_DIR}/source/field_descriptor.c
    ${PROJECT_SOURCE_DIR}/source/header_descriptor.c
    ${PROJECT_SOURCE_DIR}/source/protocol/ipv6.c
)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND BUILD_TESTING)
    # tests
    enable_testing()

    # - buffer
    add_executable(test_buffer ${PROJECT_SOURCE_DIR}/test/test_buffer.c)
    target_link_libraries(test_buffer PRIVATE cschc)
    add_test(NAME test_buffer COMMAND $<TARGET_FILE:test_buffer>)

    # - field descriptor
    add_executable(test_field_descriptor ${PROJECT_SOURCE_DIR}/test/test_field_descriptor.c)
    target_link_libraries(test_field_descriptor PRIVATE cschc)
    add_test(NAME test_field_descriptor COMMAND $<TARGET_FILE:test_field_descriptor>)

    # - header descriptor
    add_executable(test_header_descriptor ${PROJECT_SOURCE_DIR}/test/test_header_descriptor.c)
    target_link_libraries(test_header_descriptor PRIVATE cschc)
    add_test(NAME test_header_descriptor COMMAND $<TARGET_FILE:test_header_descriptor>)

    # - header descriptor
    add_executable(test_ipv6_parser ${PROJECT_SOURCE_DIR}/test/test_ipv6_parser.c)
    target_link_libraries(test_ipv6_parser PRIVATE cschc)
    add_test(NAME test_ipv6_parser COMMAND $<TARGET_FILE:test_ipv6_parser>)

endif()


# documentation

find_package( Doxygen )

if ( DOXYGEN_FOUND )

set( DOXYGEN_OUTPUT_DIRECTORY doxygen )
set( DOXYGEN_COLLABORATION_GRAPH YES )
set( DOXYGEN_EXTRACT_ALL YES )
set( DOXYGEN_CLASS_DIAGRAMS YES )
set( DOXYGEN_HIDE_UNDOC_RELATIONS NO )
set( DOXYGEN_HAVE_DOT YES )
set( DOXYGEN_CLASS_GRAPH YES )
set( DOXYGEN_CALL_GRAPH YES )
set( DOXYGEN_CALLER_GRAPH YES )
set( DOXYGEN_COLLABORATION_GRAPH YES )
set( DOXYGEN_BUILTIN_STL_SUPPORT YES )
set( DOXYGEN_EXTRACT_PRIVATE YES )
set( DOXYGEN_EXTRACT_PACKAGE YES )
set( DOXYGEN_EXTRACT_STATIC YES )
set( DOXYGEN_EXTRACT_LOCALMETHODS YES )
set( DOXYGEN_UML_LOOK YES )
set( DOXYGEN_UML_LIMIT_NUM_FIELDS 50 )
set( DOXYGEN_TEMPLATE_RELATIONS YES )
set( DOXYGEN_DOT_GRAPH_MAX_NODES 100 )
set( DOXYGEN_MAX_DOT_GRAPH_DEPTH 0 )
set( DOXYGEN_DOT_TRANSPARENT YES )

doxygen_add_docs( doxygen ${PROJECT_SOURCE_DIR}/source
                          ${PROJECT_SOURCE_DIR}/include
)

else()

message( "Doxygen need to be installed to generate the doxygen documentation" )

endif()