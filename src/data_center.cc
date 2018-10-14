#include "data_center.h"
#include <time.h>
#include <algorithm>

DataCenter::DataCenter()
{
	srand(static_cast<uint32_t>(time(nullptr)));
}

std::string DataCenter::GetRandString()
{
	static const char BASE_STRING[] = "0123456789abcdefghijklmnopqrstuvwxwzABCDEFGHIJKLMNOPQRSTUVWXYZ";
	static const int BASE_STRING_COUNT = sizeof(BASE_STRING) - 1;
	std::string result;
	for (auto i = 0; i < 12; i++)
	{
		result += BASE_STRING[static_cast<uint32_t>(rand()) % BASE_STRING_COUNT];
	}
	return move(result);
}

std::string DataCenter::SortedString(const std::string& str)
{
	auto result = str;
	std::sort(&result[0], &result[0] + str.length());
	return move(result);
}