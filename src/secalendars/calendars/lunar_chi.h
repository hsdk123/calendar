#pragma once
#include "lunar.h"
#include <unordered_map>
#include <vector>
#include <string>
#include <date/date.h>

namespace SECalendar
{
	// Usage: Chinese 농력 calendar
	class LunarCalendar_Chi final : public LunarCalendar_Abstract
	{
	public:
		LunarCalendar_Chi();
	};
}

