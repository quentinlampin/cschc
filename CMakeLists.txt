cmake_minimum_required(VERSION 3.14)

set(CMAKE_C_FLAGS_DEBUG_INIT "-Wall -g -O0 -fsanitize=address")
set(CMAKE_C_FLAGS_RELWITHDEBINFO_INIT "-Wall -g -O0 -fsanitize=address")
set(CMAKE_C_FLAGS_RELEASE_INIT "-Wall")


# library declaration
project(cschc 
    VERSION 0.1.0 
    DESCRIPTION "C implementation of Static Context Header Compression (SCHC), RFC 8724"
    LANGUAGES C
)

include_directories(
    ${PROJECT_SOURCE_DIR}/include/
    ${PROJECT_SOURCE_DIR}/include/utils/
    ${PROJECT_SOURCE_DIR}/include/parser/
    ${PROJECT_SOURCE_DIR}/include/parser/protocol/
    ${PROJECT_SOURCE_DIR}/include/core/
)

add_library(cschc
    ${PROJECT_SOURCE_DIR}/source/utils/log.c
    ${PROJECT_SOURCE_DIR}/source/utils/binarie.c
    ${PROJECT_SOURCE_DIR}/source/parser/parser.c
    ${PROJECT_SOURCE_DIR}/source/core/matching_operators.c
)


if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND BUILD_TESTING)
    # tests
    enable_testing()

    # - Utils
    add_executable(test-utils ${PROJECT_SOURCE_DIR}/test/test_utils.c)
    target_link_libraries(test-utils PRIVATE cschc)
    add_test(NAME test-utils COMMAND $<TARGET_FILE:test-utils>)

    # - IPv6 parser
    add_executable(test-ipv6-parser ${PROJECT_SOURCE_DIR}/test/test_ipv6_parser.c)
    target_link_libraries(test-ipv6-parser PRIVATE cschc)
    add_test(NAME test-ipv6-parser COMMAND $<TARGET_FILE:test-ipv6-parser>)

    # - UDP parser
    add_executable(test-udp-parser ${PROJECT_SOURCE_DIR}/test/test_udp_parser.c)
    target_link_libraries(test-udp-parser PRIVATE cschc)
    add_test(NAME test-udp-parser COMMAND $<TARGET_FILE:test-udp-parser>)

    # - Matching Operators
    add_executable(test-matching-operators ${PROJECT_SOURCE_DIR}/test/test_matching_operators.c)
    target_link_libraries(test-matching-operators PRIVATE cschc)
    add_test(NAME test-matching-operators COMMAND $<TARGET_FILE:test-matching-operators>)
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