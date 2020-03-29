#include <gtest/gtest.h>
#include <secalendars/calendars/lunar_kor.h>
#include <secalendars/calendars/lunar_chi.h>
#include <secalendars/converters/converter_solar_lunar.h>

using namespace SECalendar;

TEST(Converters, solar_to_lunar)
{
    const std::vector<std::pair<date::year_month_day/*gregorian: solar*/, LunarDate>> test_input_output = {
        { date::year{ 1881 } / 1 / 30, LunarDate(date::year{ 1881 } / 1 / 1, false) },
        { date::year{ 1881 } / 1 / 31, LunarDate(date::year{ 1881 } / 1 / 2, false) },
        { date::year{ 1881 } / 1 / 31, LunarDate(date::year{ 1881 } / 1 / 2, false) },
        { date::year{ 1991 } / 5 / 4, LunarDate(date::year{ 1991 } / 3 / 20, false) }
    };

    for (const auto& input_output : test_input_output) {
        // a. solar -> lunar
        const auto lunar_date = Converters::convert_solar_to_lunar(input_output.first, LunarCalendar_Kor());
        {
            ASSERT_TRUE(lunar_date.has_value());
            ASSERT_EQ(*lunar_date, input_output.second);
        }
        // b. lunar -> solar
        const auto solar_date = Converters::convert_lunar_to_solar(*lunar_date, LunarCalendar_Kor());
        {
            ASSERT_TRUE(solar_date.has_value());
            ASSERT_EQ(*solar_date, input_output.first);
        }
    }
}

TEST(Converters, solar_to_lunar_leap_month)
{
    const std::vector<std::pair<date::year_month_day/*gregorian: solar*/, LunarDate>> test_input_output = {
        { date::year{ 2020 } / 6 / 10, LunarDate(date::year{ 2020 } / 4 / 19, true) },
        { date::year{ 2020 } / 7 / 15, LunarDate(date::year{ 2020 } / 5 / 25, false) },
        { date::year{ 2020 } / 11 / 2, LunarDate(date::year{ 2020 } / 9 / 17, false) },
    };

    for (const auto& input_output : test_input_output) {
        // a. solar -> lunar
        const auto lunar_date = Converters::convert_solar_to_lunar(input_output.first, LunarCalendar_Kor());
        {
            ASSERT_TRUE(lunar_date.has_value());
            ASSERT_EQ(*lunar_date, input_output.second);
        }
        // b. lunar -> solar
        const auto solar_date = Converters::convert_lunar_to_solar(*lunar_date, LunarCalendar_Kor());
        {
            ASSERT_TRUE(solar_date.has_value());
            ASSERT_EQ(*solar_date, input_output.first);
        }
    }
}

