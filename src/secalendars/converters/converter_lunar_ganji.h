#pragma once
#include <secalendars/calendars/date_lunar.h>
#include <secalendars/calendars/date_ganji.h>
#include <secalendars/calendars/lunar.h>
#include <date/date.h>
#include <optional>

namespace SECalendar
{
	namespace Converters
	{
		std::optional<GanjiDate> convert_lunar_to_ganji(
			LunarDate lunar_date,
			const LunarCalendar_Abstract& lunar_calendar /*input date calendar*/
		);
	}
}

