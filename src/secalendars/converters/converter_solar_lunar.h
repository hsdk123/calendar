#pragma once
#include <secalendars/calendars/lunar.h>
#include <secalendars/calendars/date_lunar.h>
#include <date/date.h>
#include <optional>

namespace SECalendar
{
namespace Converters
{
	// solar: currently uses gregorian calendar
	std::optional<LunarDate> convert_solar_to_lunar(
		const date::year_month_day& julian_date,
		const LunarCalendar_Abstract& lunar_calendar /*conversion method*/
	);

	// solar: currently uses gregorian calendar
	std::optional<date::year_month_day> convert_lunar_to_solar(
		const LunarDate& lunar_date,
		const LunarCalendar_Abstract& lunar_calendar /*conversion method*/
	);
}
}

