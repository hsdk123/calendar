#include <gtest/gtest.h>
#include <secalendars/calendars/lunar_kor.h>
#include <secalendars/calendars/lunar_chi.h>
#include <secalendars/calendars/lunar_00_types.h>

using namespace SECalendar;

void test_consitency(
    const std::vector<std::string/*month codes for that year*/>& year_month_table,
    const std::vector<int>& year_days)
{
    ASSERT_EQ(year_month_table.size(), year_days.size());

    // test day consistency
    const auto n = year_days.size();
    for (auto i = 0u; i < n; ++i) {
        const auto& year_days_total = year_days[i];

        auto count = 0;
        for (const auto& day_type : year_month_table[i])
        {
            // a. check proper range in data.
            const auto num = day_type - '0';
            ASSERT_TRUE(0 <= num && num <= 4);

            // b. add count
            const auto day_type_enum = static_cast<LunarMonthType>(num);
            count += days_from_lunar_month_type(day_type_enum);
        }

        ASSERT_EQ(year_days_total, count);
    }
}

TEST(LunarCalendar, DayConsistency)
{
    {
        LunarCalendar_Kor calendar;
        test_consitency(calendar.GetYearMonthTable(), calendar.GetYearDays());
    }
    {
        LunarCalendar_Chi calendar;
        test_consitency(calendar.GetYearMonthTable(), calendar.GetYearDays());
    }
}

TEST(LunarCalendar, OutOfBounds)
{
    {
        LunarCalendar_Kor calendar;
        {
            auto bounds_before_date = date::year_month_day(calendar.GetFirstDay_Lunar().operator date::sys_days() - date::days(1));
            const auto res = calendar.CalcLunarDateFrom(LunarDate(bounds_before_date), 1);
            ASSERT_TRUE(!res.has_value());
        }
        {
            auto bounds_before_date = date::year_month_day(calendar.GetFirstDay_Lunar().operator date::sys_days() + date::days(100000));
            const auto res = calendar.CalcLunarDateFrom(LunarDate(bounds_before_date), 1);
            ASSERT_TRUE(!res.has_value());
        }
    }
    {
        LunarCalendar_Chi calendar;
        {
            auto bounds_before_date = date::year_month_day(calendar.GetFirstDay_Lunar().operator date::sys_days() - date::days(1));
            const auto res = calendar.CalcLunarDateFrom(LunarDate(bounds_before_date), 1);
            ASSERT_TRUE(!res.has_value());
        }
        {
            auto bounds_before_date = date::year_month_day(calendar.GetFirstDay_Lunar().operator date::sys_days() + date::days(100000));
            const auto res = calendar.CalcLunarDateFrom(LunarDate(bounds_before_date), 1);
            ASSERT_TRUE(!res.has_value());
        }
    }
}

