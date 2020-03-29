#pragma once
#include <date/date.h>

namespace SECalendar
{
	struct LunarDate {
		date::year_month_day date;
		bool is_leap_month;
		int hour;
		int min;

		LunarDate(const date::year_month_day& _date, bool _is_leap_month = false, int _hour = 0, int _min = 0)
			: date(_date), is_leap_month(_is_leap_month), hour(_hour), min(_min)
		{}
	};

	inline bool operator==(const LunarDate& lhs, const LunarDate& rhs) { return lhs.date == rhs.date && lhs.is_leap_month == rhs.is_leap_month; }
	inline bool operator!=(const LunarDate& lhs, const LunarDate& rhs) { return !(lhs == rhs); }
}

