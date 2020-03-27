# library declaration
add_library(CoreCalendar STATIC)

target_include_directories(CoreCalendar PUBLIC ./)

add_subdirectory(calendars)

