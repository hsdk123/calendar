#pragma once
#include <unordered_map>
#include <vector>
#include <string>

namespace SECalendar
{
	enum class LunarMonthType : char
	{
		no_month = 0,
		days_29,
		days_30,
		days_29_leap_month,
		days_30_leap_month
	};

	inline int days_from_lunar_month_type(const LunarMonthType& type) {
		if (type == LunarMonthType::days_29 || type == LunarMonthType::days_29_leap_month) {
			return 29;
		}
		else if (type == LunarMonthType::days_30 || type == LunarMonthType::days_30_leap_month) {
			return 30;
		}
		return 0;
	}
}

