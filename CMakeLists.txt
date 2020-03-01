cmake_minimum_required(VERSION 3.10)
project(SL_Lib
        LANGUAGES C)
enable_testing()

# ----------------------------------------------------------------------------------------
# Set properties for all targets.
# ----------------------------------------------------------------------------------------

set(CMAKE_C_STANDARD 99)
set(CMAKE_VERBOSE_MAKEFILE off)
set(CMAKE_LINK_SEARCH_START_STATIC on)

# ----------------------------------------------------------------------------------------
# Define configuration variables.
# ----------------------------------------------------------------------------------------

set(LOCAL_LIB_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(LOCAL_SRC_DIRECTORY "${CMAKE_BINARY_DIR}/src")
set(LOCAL_TESTS_BIN_DIRECTORY "${CMAKE_BINARY_DIR}/tests/bin")
set(LOCAL_TESTS_MCHECK_REPORT_DIRECTORY "${CMAKE_BINARY_DIR}/tests/src/report")

# ----------------------------------------------------------------------------------------
# Configure paths to header files and libraries.
# ----------------------------------------------------------------------------------------

# ====                               ====
# ==== Very important note for CLion ====
# ====                               ====
#
# This lines below are necessary for CLion to work properly. CLion uses it to find and download header files on the
# guest host for code analysis.

set(SYSTEM_LOCAL_HEADERS_PATHS /usr/local/include)
set(SYSTEM_HEADERS_PATHS /usr/include)

include_directories(${SYSTEM_LOCAL_HEADERS_PATHS} ${SYSTEM_HEADERS_PATHS} src)

set(EXTERNAL_LIBS_PATHS /usr/local/lib)
link_directories(${EXTERNAL_LIBS_PATHS} ${LOCAL_LIB_DIRECTORY})

# ----------------------------------------------------------------------------------------
# Set sources paths.
# ----------------------------------------------------------------------------------------

message("lib      ${LOCAL_LIB_DIRECTORY}")
message("src      ${LOCAL_SRC_DIRECTORY}")
message("test bin ${LOCAL_TESTS_BIN_DIRECTORY}")

# ====                               ====
# ==== Very important note for CLion ====
# ====                               ====
#
# All paths to "local source files" must be specified using relative paths (relatively to the project root directory,
# which is the directory that contains **THIS** file). By using the term "local source files", we mean source files that
# belongs to the project.

# ----------------------------------------------------------
# Create the library.
# ----------------------------------------------------------

add_library(CX_Lib
        src/CX_Status.c
        src/CX_Status.h
        src/CX_BasicDictionary.c
        src/CX_BasicDictionary.h
        src/CX_Template.c
        src/CX_Template.h
        src/CX_FileText.c
        src/CX_FileText.h
        src/CX_ArrayString.h
        src/CX_ArrayString.c
        src/CX_String.c
        src/CX_String.h
        src/CX_Logger.h
        src/CX_Logger.c
        src/CX_Array.c
        src/CX_Array.h
        src/CX_BashColor.c
        src/CX_BashColor.h
        src/CX_UTest.c
        src/CX_UTest.h
        src/CX_Constants.h)
set_target_properties(
        CX_Lib
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY ${LOCAL_LIB_DIRECTORY})

add_library(CX_Lib_Test
        src/CX_Status.c
        src/CX_Status.h
        src/CX_BasicDictionary.c
        src/CX_BasicDictionary.h
        src/CX_Template.c
        src/CX_Template.h
        src/CX_FileText.c
        src/CX_FileText.h
        src/CX_ArrayString.h
        src/CX_ArrayString.c
        src/CX_String.c
        src/CX_String.h
        src/CX_Logger.h
        src/CX_Logger.c
        src/CX_Array.c
        src/CX_Array.h
        src/CX_BashColor.c
        src/CX_BashColor.h
        src/CX_UTest.c
        src/CX_UTest.h
        src/CX_Constants.h)
set_target_properties(
        CX_Lib_Test
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY ${LOCAL_LIB_DIRECTORY})
target_compile_definitions(CX_Lib_Test PRIVATE CX_UTEST)

# ----------------------------------------------------------------------------------------
# Build the tests suite.
# ----------------------------------------------------------------------------------------

#### init.c

add_library(test_init tests/src/init.c tests/src/init.h)
set_target_properties(
        test_init
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY ${LOCAL_LIB_DIRECTORY})

#### test_CX_Array.c

add_executable(test_CX_Array tests/src/test_CX_Array.c)
add_dependencies(test_CX_Array CX_Lib)
target_link_libraries(test_CX_Array libcunit.a CX_Lib)

add_executable(test_error_CX_Array  tests/src/test_error_CX_Array.c)
add_dependencies(test_error_CX_Array CX_Lib_Test)
target_compile_definitions(test_error_CX_Array INTERFACE CX_UTEST)
target_link_libraries(test_error_CX_Array libcunit.a CX_Lib_Test)

#### test_CX_Status.c

add_executable(test_CX_Status
        tests/src/test_CX_Status.c)
add_dependencies(test_CX_Status CX_Lib)
target_link_libraries(test_CX_Status libcunit.a CX_Lib test_init)

#### test_CX_BasicDictionary.c

add_executable(test_CX_Basic_Dictionary
        tests/src/test_CX_BasicDictionary.c)
add_dependencies(test_CX_Basic_Dictionary CX_Lib)
target_link_libraries(test_CX_Basic_Dictionary libcunit.a CX_Lib test_init)

#### test_CX_BasicTemplate.c

add_executable(test_CX_Basic_Template
        tests/src/test_CX_Template.c)
add_dependencies(test_CX_Basic_Template CX_Lib)
target_link_libraries(test_CX_Basic_Template libcunit.a CX_Lib test_init)

#### test_CX_FileText.c

add_executable(test_CX_File_Text
        tests/src/test_CX_FileText.c)
add_dependencies(test_CX_File_Text CX_Lib)
target_link_libraries(test_CX_File_Text libcunit.a CX_Lib test_init)

#### test_CX_StringArray.c

add_executable(test_CX_String_Array
        tests/src/test_CX_ArrayString.c)
add_dependencies(test_CX_String_Array CX_Lib)
target_link_libraries(test_CX_String_Array libcunit.a CX_Lib test_init)

#### test_CX_String.c

add_executable(test_CX_String
        tests/src/test_CX_String.c)
add_dependencies(test_CX_String CX_Lib)
target_link_libraries(test_CX_String libcunit.a CX_Lib test_init)

#### test_CX_Logger.c

add_executable(test_CX_Logger
        tests/src/test_CX_Logger.c)
add_dependencies(test_CX_Logger CX_Lib)
target_link_libraries(test_CX_Logger libcunit.a CX_Lib test_init)

# ----------------------------------------------------------------------------------------
# Set properties for all executable test targets.
#
# WARNING: do not set LINK_DIRECTORIES here, otherwise it will overwrite the previous
#          setting.
# ----------------------------------------------------------------------------------------

set(TEST_EXE
        test_CX_Status
        test_CX_Basic_Dictionary
        test_CX_Basic_Template
        test_CX_File_Text
        test_CX_String_Array
        test_CX_String
        test_CX_Logger
        test_CX_Array
        test_error_CX_Array)

set_target_properties(
        ${TEST_EXE}
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${LOCAL_TESTS_BIN_DIRECTORY}
        EXCLUDE_FROM_ALL off
        DEPENDS test_init)

add_test(test_init script/unit-tests-init.sh)
add_test(test_CX_Status ${LOCAL_TESTS_BIN_DIRECTORY}/test_CX_Status)
add_test(test_CX_Basic_Dictionary ${LOCAL_TESTS_BIN_DIRECTORY}/test_CX_Basic_Dictionary)
add_test(test_CX_Basic_Template ${LOCAL_TESTS_BIN_DIRECTORY}/test_CX_Basic_Template)
add_test(test_CX_File_Text ${LOCAL_TESTS_BIN_DIRECTORY}/test_CX_File_Text)
add_test(test_CX_String_Array ${LOCAL_TESTS_BIN_DIRECTORY}/test_CX_String_Array)
add_test(test_CX_String ${LOCAL_TESTS_BIN_DIRECTORY}/test_CX_String)
add_test(test_CX_Logger ${LOCAL_TESTS_BIN_DIRECTORY}/test_CX_Logger)
add_test(test_CX_Array ${LOCAL_TESTS_BIN_DIRECTORY}/test_CX_Array)
add_test(test_error_CX_Array ${LOCAL_TESTS_BIN_DIRECTORY}/test_error_CX_Array)
add_test(test_terminate script/unit-tests-terminate.sh)

set_tests_properties(
        ${TEST_EXE}
        PROPERTIES
        ENVIRONMENT "PROJECT_ROOT=${CMAKE_BINARY_DIR};MCHECK_REPORT=${LOCAL_TESTS_MCHECK_REPORT_DIRECTORY}")
