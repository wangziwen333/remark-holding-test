// 自定义生产消费模式
// 继承于ProducerAndConsumer模板类
// 通过Producer函数产生生产者
// 通过调用run函数产生消费者
// 通过虚函数Consumer，调用消费者具体的处理逻辑
#pragma once
#include "producer_and_consumer.h"
#include <string>
#include <atomic>

class MyProducerAndConsumer : public utils::ProducerAndConsumer<std::string>
{
public:
	// @function：生产者函数，提供生产者调用次数统计。
	virtual void Producer(const std::string& in_value) override final;
	// @function：获取生产者调用次数与消费者调用次数报告信息
	// @return：pair（first：生产者调用次数；second：消费者调用次数）
	std::pair <int, int> GetReport() const;
private:
	//@function：如若当前队列长度大于100000，则丢弃生产者产生的数据。
	virtual bool FilterHandler(const int list_size, const std::string& in_value) const override final;
	//@function：消费者处理逻辑
	//@param：in_value(消费者需要处理的字符串)
	virtual void Consumer(const std::string& in_value) override final;
private:
	// 生产者调用次数统计
	volatile int produced_number_ = 0;
	// 消费者调用次数统计
	volatile int consumed_number_ = 0;
};

