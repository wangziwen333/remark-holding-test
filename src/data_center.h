// �����߼������࣬��������ַ����������ַ������򣬵���ģʽ�ࡣ
// Ϊͬ������ģʽ����������ģʽ�ṩ�ַ������������������
#pragma once
#include <string>
#include "singleton.h"

class DataCenter
{
	// @function�����������Ĺ��캯��������ͨ��srand������ɢ��������ӡ�
	DEFINE_SINGLETON_CONSTRUCT_NO_PARAM(DataCenter);
public:
	// @function��ͨ��������ĸ�����ֳ������飬����rand������������������ĸ�����ֵ�12λ�ַ�����
	// @return�����س���Ϊ12�������ĸ���ַ��������
	std::string GetRandString();
	// @function��ͨ��ϵͳ����sort��������������ַ�����
	// @return�������������к���ַ���
	std::string SortedString(const std::string& str);
};

