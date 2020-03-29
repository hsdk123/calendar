#include "lunar.h"
#include "lunar_00_types.h"

using namespace SECalendar;

namespace {
const std::pair<int/*month*/, bool/*is_leap_month*/> get_month(
    const LunarCalendar_Abstract::TableType& _year_month_table,
    const int _year,
    int& days_after_calendar_start)
{
    auto month = 1;

    // find correct month
    const auto n = _year_month_table[_year - 1881].size();
    auto i = 0u;
    bool leap_month_passed = false;

    for (; i < n; ++i) {
        const auto& cur_month_days_total_str = _year_month_table[_year - 1881][i];
        const auto day_type_enum = static_cast<LunarMonthType>(cur_month_days_total_str - '0');
        if (day_type_enum == LunarMonthType::no_month) {
            continue;
        }
        else if (day_type_enum == LunarMonthType::days_29) {
            if (days_after_calendar_start >= 29) {
                days_after_calendar_start -= 29; month += 1;
                continue;
            }
            else {
                break;
            }
        }
        else if (day_type_enum == LunarMonthType::days_30) {
            if (days_after_calendar_start >= 30) {
                days_after_calendar_start -= 30; month += 1;
                continue;
            }
            else {
                break;
            }
        }
        else if (day_type_enum == LunarMonthType::days_29_leap_month) {
            if (days_after_calendar_start >= 29) {
                days_after_calendar_start -= 29; month += 1;
                leap_month_passed = true;
                continue;
            }
            else {
                break;
            }
        }
        else if (day_type_enum == LunarMonthType::days_30_leap_month) {
            if (days_after_calendar_start >= 30) {
                days_after_calendar_start -= 30; month += 1;
                leap_month_passed = true;
                continue;
            }
            else {
                break;
            }
        }
    }

    if (i == n && days_after_calendar_start != 0) {
        return { 0, false };
    }

    // leap month modification
    // algorithm: 
    // a. order) normal-month, then leap-month (both called the same month)
    // ex. 음력 3월 평달, 음력 3월 윤달, 음력 4월 평달, ....
    // b. leap month only occurs at most once per year
    // (concept comes to compensate fact that sun takes 365 days to orbit 
    //  while moon can change roughly 12~13 times in that period)
    bool cur_month_is_leap = false;
    {
        const auto& cur_month_days_total_str = _year_month_table[_year - 1881][month - 1];
        const auto day_type_enum = static_cast<LunarMonthType>(cur_month_days_total_str - '0');
        cur_month_is_leap = day_type_enum == LunarMonthType::days_29_leap_month || day_type_enum == LunarMonthType::days_30_leap_month;

        if (leap_month_passed || cur_month_is_leap) {
            month -= 1;
        }
    }

    return { month, cur_month_is_leap };
}
}

std::optional<LunarDate>
SECalendar::LunarCalendar_Abstract::CalcLunarDate(int days_after_calendar_start) const
{
    // find correct year
    auto year = 1881;
    {
        auto n = _year_days.size();
        auto i = 0;
        for (i = 0u; i < n; ++i) {
            const auto& cur_year_days_total = _year_days[i];
            if (days_after_calendar_start >= cur_year_days_total) {
                ++year;
                days_after_calendar_start -= cur_year_days_total;
            }
            else {
                break;
            }
        }
        // error check.
        if (i == n && days_after_calendar_start != 0) {
            return {};
        }
    }    

    // find correct month
    const auto [month, is_leap_month] = get_month(_year_month_table, year, days_after_calendar_start);
    if (month == 0) {
        return {};
    }

    // calculate day
    auto day = 1;
    day += days_after_calendar_start;

    std::optional<LunarDate> ret;
    {
        ret.emplace(LunarDate(date::year{ year } / month / day, is_leap_month));
    }
    return ret;
}

// TODO: assuming days_after_from >= 0 right now.
std::optional<LunarDate> SECalendar::LunarCalendar_Abstract::CalcLunarDateFrom(const LunarDate& from, int days_after_from) const
{
    std::optional<LunarDate> ret;

    // a. calculate days to next year
    const auto from_lunar_year = from.date.year().operator int();
    const auto lunar_first_year = _lunar_first_day.year().operator int();
    {
        // error check: bounds check
        {
            const auto delta_from = from.date.operator date::sys_days();
            const auto delta_lunar_first = _lunar_first_day.operator date::sys_days();
            if ((delta_from < delta_lunar_first /*before*/) || 
                (from_lunar_year - lunar_first_year >= _year_month_table.size() /*after*/)) {
                return {};
            }
        }

        const auto& cur_year_month_days_total_str = _year_month_table[from_lunar_year - lunar_first_year];
        auto month_idx = (from.date.month().operator unsigned int() - 1) + (from.is_leap_month ? 1 : 0);
        
        // add current month remaining days
        {
            const auto cur_lunar_month_days_rem = 
                (days_from_lunar_month_type(static_cast<LunarMonthType>(cur_year_month_days_total_str[month_idx] - '0'))
                    - from.date.day().operator unsigned int());

            // case: within cur lunar month.
            if (cur_lunar_month_days_rem >= days_after_from) {
                ret.emplace(from);
                ret->date = date::year_month_day(ret->date.operator date::sys_days() + date::days(days_after_from));
                return ret;
            }

            ++month_idx;
            days_after_from -= cur_lunar_month_days_rem;
        }

        // add remaining days in year
        for ( ; month_idx <= cur_year_month_days_total_str.size(); ++month_idx) {
            const auto& cur_month_days = (days_from_lunar_month_type(static_cast<LunarMonthType>(cur_year_month_days_total_str[month_idx] - '0')));

            // case: within cur lunar month.
            if (cur_month_days >= days_after_from) {
                ret.emplace(from);
                ret->date = date::year_month_day(ret->date.operator date::sys_days() + date::days(days_after_from));
                return ret;
            }

            days_after_from -= cur_month_days;
        }
    }

    // b. calculate from next year
    {
        // traverse each year
        auto year_i = (from_lunar_year + 1 - lunar_first_year);
        for (; year_i < _year_month_table.size(); ++year_i) {
            const auto& cur_year_month_days_total_str = _year_month_table[year_i];

            // traverse each month
            for (auto month_idx = 0; month_idx <= cur_year_month_days_total_str.size(); ++month_idx) {
                const auto& cur_month_days = (days_from_lunar_month_type(static_cast<LunarMonthType>(cur_year_month_days_total_str[month_idx] - '0')));

                // case: within cur lunar month: <= this should be final exit point.
                if (cur_month_days >= days_after_from) {
                    ret.emplace(from);
                    ret->date = date::year_month_day(ret->date.operator date::sys_days() + date::days(days_after_from));
                    return ret;
                }

                days_after_from -= cur_month_days;
            }
        }
    }

    // error: passed calendar end bounds
    return {};
}
