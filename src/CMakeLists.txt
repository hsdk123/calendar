# library declaration
add_library(CoreCalendar STATIC)

target_include_directories(CoreCalendar PUBLIC ./)

target_sources(CoreCalendar PRIVATE
	00.cpp
)
