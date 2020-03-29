add_executable(test_CoreCalendar)

enable_testing()

#-----------------------------------------------------------------
# GoogleTest
include(FetchContent)
set(GTEST_MSVC_SEARCH "MT" FORCE)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        master
)
FetchContent_MakeAvailable(googletest)
#-----------------------------------------------------------------

target_link_libraries(test_CoreCalendar PUBLIC
	gtest_main
	CoreCalendar
)

target_include_directories(test_CoreCalendar PUBLIC
	${GTEST_INCLUDE_DIRS}
	${PROJECT_SOURCE_DIR}/src/
)

# Subdirectories
add_subdirectory(secalendars)
add_subdirectory(seframeworks)
