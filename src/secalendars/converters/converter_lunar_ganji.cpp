#include "converter_lunar_ganji.h"
#include <secalendars/converters/converter_solar_lunar.h>

using namespace SECalendar;

namespace {
Ganji calc_year_ganji(const int lunar_year)
{
	// 년주: [天 = 年 mod 10], [地 = ((年 mod 60) mod 12)]
	const auto c = static_cast<Chungan>((lunar_year + 6) % 10);
	const auto j = static_cast<Jiji>(((lunar_year + 8) % 60) % 12);
	return { c, j };
}
Ganji calc_month_ganji(const Ganji& year_ganji, const int lunar_month)
{
	// 월주: [天 = a. 년간合化y b. x生y => 天 = x의 陽干] [地 = 월을 따름]
	Chungan c = Chungan::gap; 
	{
		if (year_ganji.chungan == Chungan::gap || year_ganji.chungan == Chungan::gi) {
			c = static_cast<Chungan>((2 + (lunar_month - 1)) % 10); 
		} // 甲己合化[土] 화생토 => 丙(2)
		else if (year_ganji.chungan == Chungan::eul || year_ganji.chungan == Chungan::gyung) {
			c = static_cast<Chungan>((4 + (lunar_month - 1)) % 10);
		} // 乙庚合化[金] 토생금 => 戊(4)
		else if (year_ganji.chungan == Chungan::byung || year_ganji.chungan == Chungan::shin) {
			c = static_cast<Chungan>((6 + (lunar_month - 1)) % 10);
		} // 丙辛合化[水] 금생수 => 庚(6)
		else if (year_ganji.chungan == Chungan::jung || year_ganji.chungan == Chungan::im) {
			c = static_cast<Chungan>((8 + (lunar_month - 1)) % 10);
		} // 丁壬合化[木] 수생목 => 壬(8)
		else if (year_ganji.chungan == Chungan::mu || year_ganji.chungan == Chungan::gye) {
			c = static_cast<Chungan>((0 + (lunar_month - 1)) % 10);
		} // 戊癸合化[火] 목생화 => 甲(0)
	}
	Jiji j = Jiji::ja;
	{
		// 1월 == 寅월(index:2) (lunarMonth on 1~scale)
		j = static_cast<Jiji>((lunar_month + 1) % 12);
	}
	return { c, j };
}
Ganji calc_day_ganji(const int julius_day)
{
	// 일주는 절기, 태음력 기준 상관없이 율리우스일 기준으로 하면 된다 -> HS: is this true? (did they just mean solar?)
	// 일주:  [天 = 율리우스일 mod 10], [지 = 율리우스일 mod 12]

	Chungan c = Chungan::gap;
	{
		c = static_cast<Chungan>(julius_day % 10);
	}
	Jiji j = Jiji::ja;
	{
		j = static_cast<Jiji>((julius_day + 2) % 12);
	}
	return { c, j };
}
Ganji calc_hour_ganji(const Ganji& day_ganji, const int solar_hour /*note: solar hour == lunar hour*/)
{
	Chungan c = Chungan::gap;
	{
		if (day_ganji.chungan == Chungan::gap || day_ganji.chungan == Chungan::gi) {
			c = static_cast<Chungan>((0 + solar_hour) % 10);
		} //甲己合化[土] 목극토 => 甲(0)
		else if (day_ganji.chungan == Chungan::eul || day_ganji.chungan == Chungan::gyung) {
			c = static_cast<Chungan>((2 + solar_hour) % 10);
		} //乙庚合化[金] 화극금 => 丙(2)
		else if (day_ganji.chungan == Chungan::byung || day_ganji.chungan == Chungan::shin) {
			c = static_cast<Chungan>((4 + solar_hour) % 10);
		} //丙辛合化[水] 토극수 => 戊(4)
		else if (day_ganji.chungan == Chungan::jung || day_ganji.chungan == Chungan::im) {
			c = static_cast<Chungan>((6 + solar_hour) % 10);
		} //丁壬合化[木] 금극목 => 庚(6)
		else if (day_ganji.chungan == Chungan::mu || day_ganji.chungan == Chungan::gye) {
			c = static_cast<Chungan>((8 + solar_hour) % 10);
		} //戊癸合化[火] 수극화 => 壬(8)
	}
	Jiji j = static_cast<Jiji>(solar_hour % 12);
	return { c, j };
}
}

std::optional<GanjiDate> 
SECalendar::Converters::convert_lunar_to_ganji(
	LunarDate lunar_date,
	const LunarCalendar_Abstract& lunar_calendar /*input date calendar*/)
{
	int input_lunar_year; 
	int input_lunar_month;
	int input_lunar_day;
	int input_lunar_hour;
	{
		// make time modification: starting 23:00, it is next day in ganji standard
		// note: some places also have concepts of 조자시, 야자시
		if (lunar_date.hour >= 23) {
			auto new_lunar_date = lunar_calendar.CalcLunarDateFrom(lunar_date, 1);
			if (!new_lunar_date) {
				return {};
			}
			lunar_date = *new_lunar_date;
			lunar_date.hour = 0;
		}

		input_lunar_year = lunar_date.date.year().operator int();

		// ganji input month: 윤월은 15일을 기준으로 나눠서 본다 (ex. 윤2월16일 자시부터는 3월16일로 본다)
		// assuming) leap months never occur at end of year (otherwise would need to update year as well)
		input_lunar_month = lunar_date.date.month().operator unsigned int();
		if (lunar_date.is_leap_month && lunar_date.date.day().operator unsigned int() >= 16) {
			++input_lunar_month;
		}

		input_lunar_day = lunar_date.date.day().operator unsigned int();
		input_lunar_hour = lunar_date.hour;
	}

	GanjiDate ganji_date;
	{
		ganji_date.year = calc_year_ganji(lunar_date.date.year().operator int());
		ganji_date.month = calc_month_ganji(ganji_date.year, lunar_date.date.month().operator unsigned int());
		{
			const auto julius_date = Converters::convert_lunar_to_solar(lunar_date, lunar_calendar);
			if (!julius_date) {
				return {};
			}
			ganji_date.day = calc_day_ganji(julius_date->day().operator unsigned int());
		}
		ganji_date.hour = calc_hour_ganji(ganji_date.day, input_lunar_hour);
	}

	return { ganji_date };
}
