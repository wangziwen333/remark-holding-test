// 数据逻辑处理类，包含随机字符串生成与字符串排序，单例模式类。
// 为同步处理模式与生产消费模式提供字符串输入与排序输出。
#pragma once
#include <string>
#include "singleton.h"

class DataCenter
{
	// @function：不带参数的构造函数单例。通过srand函数，散播随机种子。
	DEFINE_SINGLETON_CONSTRUCT_NO_PARAM(DataCenter);
public:
	// @function：通过定义字母与数字常量数组，调用rand随机函数，随机生成字母与数字的12位字符串。
	// @return：返回长度为12的随机字母与字符串的组合
	std::string GetRandString();
	// @function：通过系统函数sort，升序排列随机字符串。
	// @return：返回升序排列后的字符串
	std::string SortedString(const std::string& str);
};

