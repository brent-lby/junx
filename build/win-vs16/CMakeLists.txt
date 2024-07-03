cmake_minimum_required(VERSION 3.20)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
set(CMAKE_SUPPORT_WINDOWS_EXPORT_ALL_SYMBOLS 1)

IF (CMAKE_SIZEOF_VOID_P EQUAL 8)
	set(JUNX_ARCH X86_64)
ELSE(CMAKE_SIZEOF_VOID_P EQUAL 8)
	set(JUNX_ARCH i386)
ENDIF(CMAKE_SIZEOF_VOID_P EQUAL 8)

message("Current Arch is ${JUNX_ARCH}")
add_definitions(-DWINDOWS)
SET(CMAKE_C_FLAGS_DEBUG "$ENV{CFLAGS} /WX /W4 /Zi /Od /utf-8")
	
SET(CMAKE_C_FLAGS_RELEASE "$ENV{CFLAGS} /WX /W4 /Zi /O2 /utf-8")

project(libjunx)
set(CMAKE_DEBUG_POSTFIX "-debug")
set(CMAKE_RELEASE_POSTFIX "-release")

get_filename_component(JUNX_PROJECT_DIR "${CMAKE_SOURCE_DIR}/../../" ABSOLUTE NAME)
get_filename_component(PFC_NAME ${CMAKE_SOURCE_DIR} NAME)
get_filename_component(PFC_NAME ${PFC_NAME} NAME)

set(JUNX_OUTPUT_DIR ${JUNX_PROJECT_DIR}/output/${PFC_NAME}/junx)
set(JUNX_LIBSRC_DIR "${JUNX_PROJECT_DIR}/src/junx")
set(JUNX_OUTPUT_LIB_DIR ${JUNX_PROJECT_DIR}/output/${PFC_NAME}/junx/lib)

message("PFC_NAME is -> <${PFC_NAME}>")
message("JUNX_PROJECT_DIR is -> <${JUNX_PROJECT_DIR}>")
message("JUNX_OUTPUT_DIR is -> <${JUNX_OUTPUT_DIR}>")

set(LIBRARY_OUTPUT_PATH ${JUNX_OUTPUT_DIR}/lib)
message("LIBRARY_OUTPUT_PATH is -> <${LIBRARY_OUTPUT_PATH}>")

function(get_current_directory_depth depth_var)
  set(current_path "${CMAKE_CURRENT_SOURCE_DIR}")
  set(depth 0)
  
  while(NOT "${current_path}" STREQUAL "/")
    get_filename_component(parent_path "${current_path}" PATH)
    if(NOT "${parent_path}" STREQUAL "${current_path}")
      math(EXPR depth "${depth} + 1")
      set(current_path "${parent_path}")
    else()
      # Reached the root directory or a loop occurred.
      break()
    endif()
  endwhile()
  
  set(${depth_var} ${depth} PARENT_SCOPE)
endfunction()

get_current_directory_depth(dir_depth)

macro(source_group_by_dir source_files)
    if(MSVC)
        	set(sgbd_cur_dir ${CMAKE_CURRENT_SOURCE_DIR})
        	foreach(sgbd_file ${${source_files}})  
          string(REGEX REPLACE ${sgbd_cur_dir}/\(.*\) \\1 sgbd_fpath ${sgbd_file})

          string(REGEX REPLACE "\(.*\)/.*" \\1 sgbd_group_name ${sgbd_fpath})
          string(COMPARE EQUAL ${sgbd_fpath} ${sgbd_group_name} sgbd_nogroup)

          if(sgbd_nogroup)
          set(sgbd_group_name "/")
          endif(sgbd_nogroup)            

          string(REPLACE "/" "/;" fplist "${sgbd_group_name}")
            
          list(LENGTH fplist len)

          list(SUBLIST fplist ${dir_depth} -1 sgbd_file_list)
					string(REPLACE ";" "<->"  grppathname ${sgbd_file_list})
          source_group(${grppathname} FILES ${sgbd_file})   
        endforeach(sgbd_file)
    endif(MSVC)
endmacro(source_group_by_dir)

file(GLOB_RECURSE JUNX_LIBCS ${JUNX_LIBSRC_DIR}/*.c)
file(GLOB_RECURSE JUNX_LIBHS ${JUNX_LIBSRC_DIR}/*.h)

source_group_by_dir(JUNX_LIBCS)
source_group_by_dir(JUNX_LIBHS)


add_library(junx_static STATIC ${JUNX_LIBCS} ${JUNX_LIBHS})
SET_TARGET_PROPERTIES(junx_static PROPERTIES OUTPUT_NAME "libjunx-static")

add_library(junx_shared SHARED ${JUNX_LIBCS} ${JUNX_LIBHS})
SET_TARGET_PROPERTIES(junx_shared PROPERTIES OUTPUT_NAME "libjunx-shared")
target_include_directories(junx_static PUBLIC  "${JUNX_LIBSRC_DIR}")
target_include_directories(junx_shared PUBLIC  "${JUNX_LIBSRC_DIR}")
target_link_libraries(junx_shared PUBLIC "ws2_32.lib" "")

string(REPLACE "/" "\\" JUNX_LIBSRC_DIR_DOS ${JUNX_LIBSRC_DIR})
string(REPLACE "/" "\\" JUNX_OUTPUT_DIR_DOS ${JUNX_OUTPUT_DIR})
string(REPLACE "/" "\\" LIBRARY_OUTPUT_PATH_DOS ${LIBRARY_OUTPUT_PATH})

add_custom_command(TARGET junx_static POST_BUILD		
	COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:junx_static> ${LIBRARY_OUTPUT_PATH}/
	COMMAND if exist ${LIBRARY_OUTPUT_PATH_DOS}\\Debug\\libjunx-static-debug.pdb copy ${LIBRARY_OUTPUT_PATH_DOS}\\Debug\\libjunx-static-debug.pdb ${LIBRARY_OUTPUT_PATH_DOS}
	COMMAND if exist ${LIBRARY_OUTPUT_PATH_DOS}\\Release\\libjunx-static-release.pdb copy ${LIBRARY_OUTPUT_PATH_DOS}\\Release\\libjunx-static-release.pdb ${LIBRARY_OUTPUT_PATH_DOS}
	COMMAND ${CMAKE_COMMAND} -E remove_directory ${LIBRARY_OUTPUT_PATH}/Debug
	COMMAND ${CMAKE_COMMAND} -E remove_directory ${LIBRARY_OUTPUT_PATH}/Release
	COMMAND xcopy /Y /E /I "${JUNX_LIBSRC_DIR_DOS}" "${JUNX_OUTPUT_DIR_DOS}\\include\\junx"
	COMMAND del /f /s "${JUNX_OUTPUT_DIR_DOS}\\include\\junx\\*.c"
)

add_custom_command(TARGET junx_shared POST_BUILD		
	COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:junx_shared> ${LIBRARY_OUTPUT_PATH}/
	COMMAND if exist ${LIBRARY_OUTPUT_PATH_DOS}\\Debug\\libjunx-shared-debug.pdb copy ${LIBRARY_OUTPUT_PATH_DOS}\\Debug\\libjunx-shared-debug.pdb ${LIBRARY_OUTPUT_PATH_DOS}
	COMMAND if exist ${LIBRARY_OUTPUT_PATH_DOS}\\Release\\libjunx-shared-release.pdb copy ${LIBRARY_OUTPUT_PATH_DOS}\\Release\\libjunx-shared-release.pdb ${LIBRARY_OUTPUT_PATH_DOS}
	COMMAND if exist ${LIBRARY_OUTPUT_PATH_DOS}\\Debug\\libjunx-shared-debug.lib copy ${LIBRARY_OUTPUT_PATH_DOS}\\Debug\\libjunx-shared-debug.lib ${LIBRARY_OUTPUT_PATH_DOS}
	COMMAND if exist ${LIBRARY_OUTPUT_PATH_DOS}\\Release\\libjunx-shared-release.lib copy ${LIBRARY_OUTPUT_PATH_DOS}\\Release\\libjunx-shared-release.lib ${LIBRARY_OUTPUT_PATH_DOS}
	COMMAND ${CMAKE_COMMAND} -E remove_directory ${LIBRARY_OUTPUT_PATH}/Debug
	COMMAND ${CMAKE_COMMAND} -E remove_directory ${LIBRARY_OUTPUT_PATH}/Release
	COMMAND xcopy /Y /E /I "${JUNX_LIBSRC_DIR_DOS}" "${JUNX_OUTPUT_DIR_DOS}\\include\\junx"
	COMMAND del /f /s "${JUNX_OUTPUT_DIR_DOS}\\include\\junx\\*.c"
)

set(JUNX_LIBJUNX_INCLUDE_DIR ${JUNX_OUTPUT_DIR}/include)
set(JUNX_LIBJUNX_LIB_DIR ${JUNX_OUTPUT_DIR}/lib)

# for function-test
set(JUNX_FUNCTEST_DIR "${JUNX_PROJECT_DIR}/src/junx_func_test")
file(GLOB_RECURSE JUNXFUNC_TEST_CS ${JUNX_FUNCTEST_DIR}/*.c)
file(GLOB_RECURSE JUNXFUNC_TEST_HS ${JUNX_FUNCTEST_DIR}/*.h)

message("SRCS_NAME is -> <${JUNXFUNC_TEST_CS}>")

#static func-test
add_executable(junx_func_test-static ${JUNXFUNC_TEST_CS} ${JUNXFUNC_TEST_HS})
set_target_properties(junx_func_test-static PROPERTIES OUTPUT_NAME "junx_func_test-static")
set_target_properties(junx_func_test-static PROPERTIES DEBUG_POSTFIX "-debug" RELEASE_POSTFIX "-release")
set_target_properties(junx_func_test-static PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG ${JUNX_OUTPUT_DIR}/bin)
set_target_properties(junx_func_test-static PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE ${JUNX_OUTPUT_DIR}/bin)
set_target_properties(junx_func_test-static PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "$(ProjectDir)..\\..\\output\\win-vs16\\junx")
source_group_by_dir(JUNXFUNC_TEST_CS)
source_group_by_dir(JUNXFUNC_TEST_HS)
target_include_directories(junx_func_test-static PUBLIC "${JUNX_PROJECT_DIR}/src")


target_link_libraries(junx_func_test-static PUBLIC debug "libjunx-static-debug" optimized libjunx-static-release)
target_link_libraries(junx_func_test-static PUBLIC "ws2_32.lib")
target_link_directories(junx_func_test-static PUBLIC ${JUNX_OUTPUT_LIB_DIR})

#dynamic func-test
add_executable(junx_func_test-shared ${JUNXFUNC_TEST_CS} ${JUNXFUNC_TEST_HS})
set_target_properties(junx_func_test-shared PROPERTIES OUTPUT_NAME "junx_func_test-shared")
set_target_properties(junx_func_test-shared PROPERTIES DEBUG_POSTFIX "-debug" RELEASE_POSTFIX "-release")
set_target_properties(junx_func_test-shared PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG ${JUNX_OUTPUT_DIR}/bin)
set_target_properties(junx_func_test-shared PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE ${JUNX_OUTPUT_DIR}/bin)
set_target_properties(junx_func_test-shared PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "$(ProjectDir)..\\..\\output\\win-vs16\\junx")
source_group_by_dir(JUNXFUNC_TEST_CS)
source_group_by_dir(JUNXFUNC_TEST_HS)
target_include_directories(junx_func_test-shared PUBLIC "${JUNX_PROJECT_DIR}/src")
target_link_libraries(junx_func_test-shared PUBLIC debug "libjunx-shared-debug" optimized libjunx-shared-release)
target_link_libraries(junx_func_test-shared PUBLIC "ws2_32.lib")
target_link_directories(junx_func_test-shared PUBLIC ${JUNX_OUTPUT_LIB_DIR})

