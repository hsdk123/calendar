#include "converter_solar_lunar.h"
#include <secalendars/calendars/lunar.h>
#include <secalendars/calendars/lunar_00_types.h>

#include <date/julian.h>

using namespace SECalendar;

std::optional<LunarDate> SECalendar::Converters::convert_solar_to_lunar(
	const date::year_month_day& julian_date,
	const LunarCalendar_Abstract& lunar_calendar)
{
	std::optional<LunarDate> ret;
	{
		// find solar day difference between 'julian_date' and solar_first_day
		const auto& solar_first_day = lunar_calendar.GetFirstDay_Solar();

		const auto julian_input_date = julian::year_month_day(julian_date);
		const auto julian_lunar_start_date = julian::year_month_day(solar_first_day);
		const auto julian_diff_days = julian_input_date.operator date::sys_days() - julian_lunar_start_date.operator date::sys_days();

		// calculate the corresponding lunar date by adding 'julian_diff_days' to start of lunar calendar.
		return lunar_calendar.CalcLunarDate(julian_diff_days.count());
		//ret.emplace(lunar_date);
	}
	//return ret;
}

std::optional<date::year_month_day> SECalendar::Converters::convert_lunar_to_solar(
	const LunarDate& lunar_date,
	const LunarCalendar_Abstract& lunar_calendar)
{
	std::optional<date::year_month_day> ret;
	{
		// a. calculate number of days past first lunar day
		auto lunar_diff_days = 0;
		{
			const auto& lunar_first_date = lunar_calendar.GetFirstDay_Lunar();
			const auto& lunar_year_days = lunar_calendar.GetYearDays();
			const auto& lunar_year_month_table = lunar_calendar.GetYearMonthTable();

			const auto dst_year = lunar_date.date.year().operator int();
			const auto dst_month = lunar_date.date.month().operator unsigned int();
			auto lunar_first_date_year = lunar_first_date.year().operator int();

			// add days from years
			{
				// error check: out of range
				if (dst_year < lunar_first_date_year) {
					return {};
				}

				auto i = 0u;
				for (; i < lunar_year_days.size(); ++i) {
					if (lunar_first_date_year + i >= dst_year) {
						break;
					}
					lunar_diff_days += lunar_year_days[i];
				}

				// error check: out of range
				if (i == lunar_year_days.size()) {
					return {};
				}
			}
			// add days from months
			{
				const auto& inspect_month_data = lunar_year_month_table[dst_year - lunar_first_date_year];
				auto leap_month_passed = false;

				auto i = 0u;
				for (; i < inspect_month_data.size(); ++i) {
					const auto& cur_month_days_total_str = inspect_month_data[i];
					const auto day_type_enum = static_cast<LunarMonthType>(cur_month_days_total_str - '0');
					const auto cur_month_is_leap_month = (day_type_enum == LunarMonthType::days_29_leap_month || day_type_enum == LunarMonthType::days_30_leap_month);

					if (i + (leap_month_passed ? -1 : 0) + 1 >= dst_month) {
						if (lunar_date.is_leap_month == cur_month_is_leap_month) {
							break;
						}
					}

					if (day_type_enum == LunarMonthType::no_month) {
						continue;
					}
					else if (day_type_enum == LunarMonthType::days_29) {
						lunar_diff_days += 29;
					}
					else if (day_type_enum == LunarMonthType::days_30) {
						lunar_diff_days += 30;
					}
					else if (day_type_enum == LunarMonthType::days_29_leap_month) {
						lunar_diff_days += 29;
						leap_month_passed = true;
					}
					else if (day_type_enum == LunarMonthType::days_30_leap_month) {
						lunar_diff_days += 30;
						leap_month_passed = true;
					}
				}

				// error check: out of range
				if (i == lunar_year_days.size()) {
					return {};
				}
			}
			// add days
			lunar_diff_days += (lunar_date.date.day().operator unsigned int() - lunar_first_date.day().operator unsigned int());
		}

		// b. add lunar diff days to first solar day in lunar calendar
		{
			const auto& lunar_calendar_first_solar_day = lunar_calendar.GetFirstDay_Solar();
			const auto converted_solar_day = lunar_calendar_first_solar_day.operator date::sys_days() + date::days(lunar_diff_days);
			const auto result = date::year_month_day(converted_solar_day);

			ret.emplace(result);
		}
	}
	return ret;
}
