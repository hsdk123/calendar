#include <gtest/gtest.h>
#include <secalendars/calendars/lunar_kor.h>
#include <secalendars/calendars/lunar_chi.h>
#include <secalendars/converters/converter_lunar_ganji.h>

using namespace SECalendar;

TEST(Converters, lunar_to_ganji)
{
    const std::vector<std::pair<date::year_month_day/*julian: solar*/, LunarDate>> test_input_output = {
        { date::year{ 1881 } / 1 / 30, LunarDate(date::year{ 1881 } / 1 / 1, false) },
        { date::year{ 1881 } / 1 / 31, LunarDate(date::year{ 1881 } / 1 / 2, false) },
        { date::year{ 1881 } / 1 / 31, LunarDate(date::year{ 1881 } / 1 / 2, false) },
        { date::year{ 1991 } / 5 / 4, LunarDate(date::year{ 1991 } / 3 / 20, false) }
    };

    /*for (const auto& input_output : test_input_output) {
        const auto lunar_date = Converters::convert_julian_to_lunar(input_output.first, LunarCalendar_Kor());
        ASSERT_TRUE(lunar_date.has_value());
        ASSERT_EQ(*lunar_date, input_output.second);
    }*/
}

TEST(Converters, lunar_to_ganji_hour_23)
{
    const std::vector<std::pair<LunarDate, GanjiDate>> test_input_output = {
        { LunarDate(date::year{ 1881 } / 1 / 1, false, 23), GanjiDate{ Ganji{Chungan::gap, Jiji::ja}, Ganji{Chungan::gap, Jiji::ja}, Ganji{Chungan::gap, Jiji::ja}, Ganji{Chungan::gap, Jiji::ja} } }
    };

    for (const auto& input_output : test_input_output) {
        const auto lunar_date = Converters::convert_lunar_to_ganji(input_output.first, LunarCalendar_Kor());
        ASSERT_TRUE(lunar_date.has_value());
        /*ASSERT_EQ(*lunar_date, input_output.second);*/

        ASSERT_EQ(lunar_date->hour.jiji, Jiji::ja);
    }
}
