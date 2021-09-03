cmake_minimum_required(VERSION "3.21")

project("reassemble-data")

set(CMAKE_CXX_STANDARD 17)

set(HEADERS "reassembler.hpp")
set(SOURCES "main.cpp")

set(HEADER_DIR "include/")
set(SOURCE_DIR "src/")

list(TRANSFORM HEADERS PREPEND ${HEADER_DIR})
list(TRANSFORM SOURCES PREPEND ${SOURCE_DIR})

add_executable(
	${PROJECT_NAME}
	${SOURCES} ${HEADERS}
)
target_include_directories(
	${PROJECT_NAME} PRIVATE
	${HEADER_DIR}
)
target_sources(
	${PROJECT_NAME} PRIVATE
	${SOURCES} ${HEADERS}
)

#[[ uncomment this block to enable google test

# google test section
set(TEST_SUBPROJECT_NAME "unit_tests")

include(FetchContent)
FetchContent_Declare(
	googletest
	URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
if(MSVC)
	set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
endif()

FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
	${TEST_SUBPROJECT_NAME}
	test/test.cpp
)

target_link_libraries(
	${TEST_SUBPROJECT_NAME}
	gtest_main
)

target_include_directories(
	${TEST_SUBPROJECT_NAME}
	PRIVATE
	${HEADER_DIR}
)

include(GoogleTest)
gtest_discover_tests(${TEST_SUBPROJECT_NAME})

]]

# project compiler flags
if (MSVC)
	target_compile_options(
		${PROJECT_NAME} PRIVATE
		"/permissive-"
		"/W4" "/w14640"
		"/w14242" "/w14254" "/w14263" "/w14265"
		"/w14287" "/we4289" "/w14296" "/w14311"
		"/w14545" "/w14546" "/w14547" "/w14549"
		"/w14555" "/w14619" "/w14826" "/w14905"
		"/w14906" "/w14928"
	)
else() # gcc or clang
	target_compile_options(
		${PROJECT_NAME} PRIVATE
		"-Wall" "-Wextra" "-Wshadow" "-Wnon-virtual-dtor" "-pedantic"
		"-Wunused" "-Wconversion" "-Wsign-conversion"
		"-Wnull-dereference" "-Wdouble-promotion"
		"-Woverloaded-virtual" "-Wpedantic"
		"-Wcast-align" "-Wformat=2"
	)
endif()

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	target_compile_options(
		${PROJECT_NAME} PRIVATE
		"-Wmisleading-indentation"
		"-Wduplicated-cond"
		"-Wduplicated-branches"
		"-Wlogical-op"
		"-Wnull-dereference"
		"-Wuseless-cast"
	)
endif()
