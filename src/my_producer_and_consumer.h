// �Զ�����������ģʽ
// �̳���ProducerAndConsumerģ����
// ͨ��Producer��������������
// ͨ������run��������������
// ͨ���麯��Consumer�����������߾���Ĵ����߼�
#pragma once
#include "producer_and_consumer.h"
#include <string>
#include <atomic>

class MyProducerAndConsumer : public utils::ProducerAndConsumer<std::string>
{
public:
	// @function�������ߺ������ṩ�����ߵ��ô���ͳ�ơ�
	virtual void Producer(const std::string& in_value) override final;
	// @function����ȡ�����ߵ��ô����������ߵ��ô���������Ϣ
	// @return��pair��first�������ߵ��ô�����second�������ߵ��ô�����
	std::pair <int, int> GetReport() const;
private:
	//@function��������ǰ���г��ȴ���100000�����������߲��������ݡ�
	virtual bool FilterHandler(const int list_size, const std::string& in_value) const override final;
	//@function�������ߴ����߼�
	//@param��in_value(��������Ҫ������ַ���)
	virtual void Consumer(const std::string& in_value) override final;
private:
	// �����ߵ��ô���ͳ��
	volatile int produced_number_ = 0;
	// �����ߵ��ô���ͳ��
	volatile int consumed_number_ = 0;
};

