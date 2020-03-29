#pragma once
#include "date_lunar.h"
#include <unordered_map>
#include <vector>
#include <string>
#include <optional>
#include <date/date.h>

namespace SECalendar
{
	class LunarCalendar_Abstract
	{
	protected: 
		/*to make sure this can't be constructed as is*/
		LunarCalendar_Abstract() {}
		virtual ~LunarCalendar_Abstract() {}

	public:
		using TableType = std::vector<std::string/*month codes for that year*/>;
		const TableType& GetYearMonthTable() const { return _year_month_table; }

		const std::vector<int>& GetYearDays() const { return _year_days; }

		const date::year_month_day GetFirstDay_Solar() const { return _julian_first_day; };

		const date::year_month_day GetFirstDay_Lunar() const { return _lunar_first_day; };

		std::optional<LunarDate> CalcLunarDate(
			int days_after_calendar_start) const;

		std::optional<LunarDate> CalcLunarDateFrom(
			const LunarDate& from,
			int days_after_from) const;

	protected:
		date::year_month_day _lunar_first_day;
		date::year_month_day _julian_first_day;

		TableType _year_month_table;
		std::vector<int> _year_days;
	};
}

