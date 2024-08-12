cmake_minimum_required(VERSION 3.20)

IF(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(JUNX_ARCH x86_64)
ELSE(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(JUNX_ARCH i386)
ENDIF(CMAKE_SIZEOF_VOID_P EQUAL 8)

if(CMAKE_HOST_SYSTEM_NAME MATCHES "Linux")
    message("Current OS Type Name is Linux")
    add_definitions(-DLINUX)
else()
    message(FATAL_ERROR, "Not Support OS: ${CMAKE_HOST_SYSTEM_NAME} ")
endif()

project(libjunx)
set(CMAKE_DEBUG_POSTFIX "-debug")
set(CMAKE_RELEASE_POSTFIX "-release")
set(CMAKE_CXX_STANDARD 11)

get_filename_component(JUNX_PROJECT_DIR "${CMAKE_SOURCE_DIR}/../../" ABSOLUTE NAME)
get_filename_component(PFC_NAME ${CMAKE_SOURCE_DIR} NAME)
get_filename_component(PFC_NAME ${PFC_NAME} NAME)

message("Project Dir is -> <${JUNX_PROJECT_DIR}>")
message("CMake Source Dir is -> <${CMAKE_SOURCE_DIR}>")

set(JUNX_OUTPUT_DIR ${JUNX_PROJECT_DIR}/output/${PFC_NAME}/junx)
set(JUNX_LIBSRC_DIR "${JUNX_PROJECT_DIR}/src/junx")
set(JUNX_OUTPUT_LIB_DIR ${JUNX_PROJECT_DIR}/output/${PFC_NAME}/junx/lib)

message("PFC_NAME is -> <${PFC_NAME}>")
message("JUNX_PROJECT_DIR is -> <${JUNX_PROJECT_DIR}>")
message("JUNX_OUTPUT_DIR is -> <${JUNX_OUTPUT_DIR}>")

set(LIBRARY_OUTPUT_PATH ${JUNX_OUTPUT_DIR}/lib)
message("LIBRARY_OUTPUT_PATH is -> <${LIBRARY_OUTPUT_PATH}>")

file(GLOB_RECURSE JUNX_LIBCS ${JUNX_LIBSRC_DIR}/*.c)
file(GLOB_RECURSE JUNX_LIBHS ${JUNX_LIBSRC_DIR}/*.h)

add_library(junx_static STATIC ${JUNX_LIBCS} ${JUNX_LIBHS})
set_target_properties(junx_static PROPERTIES OUTPUT_NAME "junx-static")

add_library(junx_shared SHARED ${JUNX_LIBCS} ${JUNX_LIBHS})
set_target_properties(junx_shared PROPERTIES OUTPUT_NAME "junx-shared")

set_target_properties(junx_static PROPERTIES
    COMPILE_FLAGS_RELEASE "-O2 -fno-aggressive-loop-optimizations -pthread -Wall -Werror -Wno-stringop-truncation "
    COMPILE_FLAGS_DEBUG "-g -pthread -Wall -Werror -Wno-stringop-truncation"
)

set_target_properties(junx_shared PROPERTIES
    COMPILE_FLAGS_RELEASE "-O2 -fno-aggressive-loop-optimizations -fPIC -pthread -Wall -Werror -Wno-stringop-truncation "
    COMPILE_FLAGS_DEBUG "-g -fPIC -pthread -Wall -Werror -Wno-stringop-truncation"
)


set(JUNX_LIBPILO_INCLUDE_DIR ${JUNX_OUTPUT_DIR}/include)
set(	 ${JUNX_OUTPUT_DIR}/lib)
add_custom_command(TARGET junx_static PRE_BUILD
        COMMAND rm -f ${JUNX_OUTPUT_DIR}/lib/libjunx-static-debug.a
        COMMAND rm -f ${JUNX_OUTPUT_DIR}/lib/libjunx-static-release.a
)
add_custom_command(TARGET junx_static POST_BUILD
        COMMAND mkdir -p ${JUNX_OUTPUT_DIR}/include/junx
        COMMAND cp -R ${JUNX_LIBSRC_DIR}/* ${JUNX_OUTPUT_DIR}/include/junx
        COMMAND find ${JUNX_OUTPUT_DIR}/include/junx -type f -name "*.c" -delete
)
add_custom_command(TARGET junx_static PRE_BUILD
        COMMAND rm -f ${JUNX_OUTPUT_DIR}/lib/libjunx-shared-debug.a
        COMMAND rm -f ${JUNX_OUTPUT_DIR}/lib/libjunx-shared-release.a
)
add_custom_command(TARGET junx_shared POST_BUILD
        COMMAND mkdir -p ${JUNX_OUTPUT_DIR}/include/junx
        COMMAND cp -R ${JUNX_LIBSRC_DIR}/* ${JUNX_OUTPUT_DIR}/include/junx
        COMMAND find ${JUNX_OUTPUT_DIR}/include/junx -type f -name "*.c" -delete
)

# for function-test
set(JUNX_FUNCTEST_DIR "${JUNX_PROJECT_DIR}/src/junx_func_test")
file(GLOB_RECURSE JUNXFUNC_TEST_CS ${JUNX_FUNCTEST_DIR}/*.c)
file(GLOB_RECURSE JUNXFUNC_TEST_HS ${JUNX_FUNCTEST_DIR}/*.h)


add_executable(junx_func_test-static ${JUNXFUNC_TEST_CS}  ${JUNXFUNC_TEST_HS})
set_target_properties(junx_func_test-static PROPERTIES OUTPUT_NAME "junx_func_test-static")
set_target_properties(junx_func_test-static PROPERTIES DEBUG_POSTFIX "-debug" RELEASE_POSTFIX "-release")
set_target_properties(junx_func_test-static PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG ${JUNX_OUTPUT_DIR}/bin)
set_target_properties(junx_func_test-static PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE ${JUNX_OUTPUT_DIR}/bin)
#target_include_directories(junx_func_test-static PUBLIC "${JUNX_OUTPUT_DIR}/include")
target_include_directories(junx_func_test-static PUBLIC ${JUNX_PROJECT_DIR}/src)
target_link_libraries(junx_func_test-static PUBLIC "pthread" "dl")
target_link_libraries(junx_func_test-static PUBLIC debug "junx-static-debug" optimized "junx-static-release")
target_link_directories(junx_func_test-static PUBLIC ${JUNX_OUTPUT_LIB_DIR})
add_dependencies(junx_func_test-static junx_static)
set_target_properties(junx_func_test-static PROPERTIES
    COMPILE_FLAGS_RELEASE "-O2 -fno-aggressive-loop-optimizations -pthread -Wall -Werror -Wno-stringop-truncation "
    COMPILE_FLAGS_DEBUG "-g -pthread -Wall -Werror -Wno-stringop-truncation "
)
add_custom_command(TARGET junx_func_test-static PRE_BUILD
        COMMAND rm -f ${JUNX_OUTPUT_DIR}/bin/junx_func_test-static*
)

add_executable(junx_func_test-shared ${JUNXFUNC_TEST_CS}  ${JUNXFUNC_TEST_HS})
set_target_properties(junx_func_test-shared PROPERTIES OUTPUT_NAME "junx_func_test-shared")
set_target_properties(junx_func_test-shared PROPERTIES DEBUG_POSTFIX "-debug" RELEASE_POSTFIX "-release")
set_target_properties(junx_func_test-shared PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG ${JUNX_OUTPUT_DIR}/bin)
set_target_properties(junx_func_test-shared PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE ${JUNX_OUTPUT_DIR}/bin)
target_include_directories(junx_func_test-shared PUBLIC ${JUNX_PROJECT_DIR}/src)
target_link_libraries(junx_func_test-shared PUBLIC "pthread" "dl")
target_link_libraries(junx_func_test-shared PUBLIC debug "junx-shared-debug" optimized "junx-shared-release")
target_link_directories(junx_func_test-shared PUBLIC ${JUNX_OUTPUT_LIB_DIR})
add_dependencies(junx_func_test-shared junx_shared)
set_target_properties(junx_func_test-shared PROPERTIES
    COMPILE_FLAGS_RELEASE "-O2 -fno-aggressive-loop-optimizations -pthread -Wall -Werror -Wno-stringop-truncation"
    COMPILE_FLAGS_DEBUG "-g -fPIC -pthread -Wall -Werror -Wno-stringop-truncation"
)
add_custom_command(TARGET junx_func_test-static PRE_BUILD
        COMMAND rm -f ${JUNX_OUTPUT_DIR}/bin/junx_func_test-shared*
)
