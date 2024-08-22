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
    ${PROJECT_SOURCE_DIR}/include/protocols/
    ${PROJECT_SOURCE_DIR}/include/core/
)

add_library(cschc
    # Utils
    ${PROJECT_SOURCE_DIR}/source/utils/log.c
    ${PROJECT_SOURCE_DIR}/source/utils/binary.c
    ${PROJECT_SOURCE_DIR}/source/utils/memory.c
    # Headers
    ${PROJECT_SOURCE_DIR}/source/protocols/headers.c
    # Core
    ${PROJECT_SOURCE_DIR}/source/core/rule_descriptor.c
    ${PROJECT_SOURCE_DIR}/source/core/rule_field_descriptor.c
    ${PROJECT_SOURCE_DIR}/source/core/matching_operators.c
    ${PROJECT_SOURCE_DIR}/source/core/actions.c
    ${PROJECT_SOURCE_DIR}/source/core/context.c
    ${PROJECT_SOURCE_DIR}/source/core/compression.c
    ${PROJECT_SOURCE_DIR}/source/core/decompression.c
)


add_executable(main ${PROJECT_SOURCE_DIR}/source/main.c)
target_link_libraries(main PUBLIC cschc)


if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND BUILD_TESTING)
    # Tests
    enable_testing()

    # - Binary
    add_executable(test-binary ${PROJECT_SOURCE_DIR}/test/test_binary.c)
    target_link_libraries(test-binary PRIVATE cschc)
    add_test(NAME test-binary COMMAND $<TARGET_FILE:test-binary>)

    # - Memory
    add_executable(test-memory ${PROJECT_SOURCE_DIR}/test/test_memory.c)
    target_link_libraries(test-memory PRIVATE cschc)
    add_test(NAME test-memory COMMAND $<TARGET_FILE:test-memory>)

    # - Rule Descriptor
    add_executable(test-rule-descriptor ${PROJECT_SOURCE_DIR}/test/test_rule_descriptor.c)
    target_link_libraries(test-rule-descriptor PRIVATE cschc)
    add_test(NAME test-rule-descriptor COMMAND $<TARGET_FILE:test-rule-descriptor>)

    # - Rule Field Descriptor
    add_executable(test-rule-field-descriptor ${PROJECT_SOURCE_DIR}/test/test_rule_field_descriptor.c)
    target_link_libraries(test-rule-field-descriptor PRIVATE cschc)
    add_test(NAME test-rule-field-descriptor COMMAND $<TARGET_FILE:test-rule-field-descriptor>)

    # - Matching Operators
    add_executable(test-matching-operators ${PROJECT_SOURCE_DIR}/test/test_matching_operators.c)
    target_link_libraries(test-matching-operators PRIVATE cschc)
    add_test(NAME test-matching-operators COMMAND $<TARGET_FILE:test-matching-operators>)

    # - Compression Decompression Actions
    add_executable(test-comp-decomp-actions ${PROJECT_SOURCE_DIR}/test/test_actions.c)
    target_link_libraries(test-comp-decomp-actions PRIVATE cschc)
    add_test(NAME test-comp-decomp-actions COMMAND $<TARGET_FILE:test-comp-decomp-actions>)

    # - Compression
    add_executable(test-compression ${PROJECT_SOURCE_DIR}/test/test_compression.c)
    target_link_libraries(test-compression PRIVATE cschc)
    add_test(NAME test-compression COMMAND $<TARGET_FILE:test-compression>)

    # - Decompression
    add_executable(test-decompression ${PROJECT_SOURCE_DIR}/test/test_decompression.c)
    target_link_libraries(test-decompression PRIVATE cschc)
    add_test(NAME test-decompression COMMAND $<TARGET_FILE:test-decompression>)
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