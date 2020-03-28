# library declaration
add_library(CoreCalendar STATIC)

target_include_directories(CoreCalendar PUBLIC ./)

# subdirectories
add_subdirectory(calendars)
add_subdirectory(converters)

#-----------------------------------------------------------------------------
include(FetchContent)
FetchContent_Declare( date_src
    GIT_REPOSITORY https://github.com/HowardHinnant/date.git
    GIT_TAG        master  # adjust tag/branch/commit as needed
)
FetchContent_MakeAvailable(date_src)
#-----------------------------------------------------------------------------

target_link_libraries(CoreCalendar PUBLIC
	date::date
)

