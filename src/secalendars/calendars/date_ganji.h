#pragma once
#include <date/date.h>
#include <vector>

namespace SECalendar
{
	enum class Chungan {
		gap,		// 甲
		eul,		// 乙
		byung,	// 丙
		jung,		// 丁
		mu,		// 戊
		gi,			// 己
		gyung,	// 庚
		shin,		// 辛
		im,			// 壬
		gye		// 癸
	};
	enum class Jiji {
		ja,			// 子
		chuk,		// 丑
		in,			// 寅
		myo,		// 卯
		jin,			// 辰
		sa,			// 巳
		o,			// 午
		mi,			// 未
		shin,		// 申
		yu,			// 酉
		sul,		// 戌
		he			// 亥
	};

	struct Ganji {
		Chungan		chungan = Chungan::gap;
		Jiji				jiji = Jiji::ja;
	};

	inline bool operator==(const Ganji& lhs, const Ganji& rhs) { return lhs.chungan == rhs.chungan && lhs.jiji == rhs.jiji; }
	inline bool operator!=(const Ganji& lhs, const Ganji& rhs) { return !(lhs == rhs); }

	struct GanjiDate {
		Ganji year;
		Ganji month;
		Ganji day;
		Ganji hour;
	};

	inline bool operator==(const GanjiDate& lhs, const GanjiDate& rhs) 
	{ return lhs.year == rhs.year && lhs.month == rhs.month; lhs.day == rhs.day && lhs.hour == rhs.hour; }
	inline bool operator!=(const GanjiDate& lhs, const GanjiDate& rhs) { return !(lhs == rhs); }
}

