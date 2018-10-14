// 模板函数
// 提供FIFO的队列模式
// 队列出栈为批量操作
#pragma once
#include <thread>
#include <functional>
#include <list>
#include <mutex>
#include <condition_variable>
#include <iostream>
#include <chrono>

namespace utils
{
	template <class TValue>
	class DataFilter
	{
		using FilterHandlerType = std::function <bool(const int list_size, const TValue& in_value)>;
	public:
		// @function：设置生产者数据过滤器函数
		void SetFilterHandler(const FilterHandlerType& filter_handler) { filter_handler_ = filter_handler; };
	protected:
		// 过滤器函数默认值，默认为不过滤。
		FilterHandlerType filter_handler_ = bind(&DataFilter::FilterHandler, this, std::placeholders::_1, std::placeholders::_2);
	private:
		//@function：生产者数据过滤器。
		//@param：list_size（当前数据队列大小）
		//@param：in_value（当前处理数据元素）
		//@return：true（消费者需要处理该数据）、false（消费者不需要处理该数据）
		virtual bool FilterHandler(const int list_size, const TValue& in_value) const { return true; };
	};

	template <class TValue>
	class ProducerAndConsumer : public DataFilter <TValue>
	{
	public:
		// @function：生产者函数，在队列末尾添加生产者数据。
		virtual void Producer(const TValue& in_value);
		virtual void Consumer(const TValue& in_value) = 0;
		// @function：启动单线程消费者模式
		void Run();
		~ProducerAndConsumer();
		// @function：阻塞函数，直到线程退出才返回（通过消息通知机制退出）。
		void WartForStop();
	private:
		std::list <TValue> list_value_;
		std::mutex mutex_;
		std::condition_variable condition_varable_;
		// 是否停止消费者线程
		volatile bool is_to_stop_ = false;
		// 消费者线程是否正在运行
		volatile bool is_running_ = false;
	};

	template <class TValue>
	void ProducerAndConsumer<TValue>::WartForStop()
	{
		if (is_running_)
		{
			is_to_stop_ = true;
			while (is_running_)
			{
				std::this_thread::sleep_for(std::chrono::seconds(1));
				std::unique_lock<std::mutex> lock(mutex_);
				condition_varable_.notify_one();
			}
		}
	}

	template <class TValue>
	ProducerAndConsumer<TValue>::~ProducerAndConsumer()
	{
		is_to_stop_ = true;
		is_running_ = false;
	}

	template <class TValue>
	void ProducerAndConsumer<TValue>::Producer(const TValue& in_value)
	{
		std::unique_lock<std::mutex> lock(mutex_);
		if (DataFilter<TValue>::filter_handler_(list_value_.size(), in_value))
		{
			list_value_.push_back(in_value);
		}
		condition_varable_.notify_one();
	}

	template <class TValue>
	void ProducerAndConsumer<TValue>::Run()
	{
		std::thread t([&] {
			is_running_ = true;
			while (!is_to_stop_)
			{
				decltype(list_value_) list_value;
				{
					std::unique_lock<std::mutex> lock(mutex_);
					while (!is_to_stop_ && list_value_.empty())
					{
						condition_varable_.wait(lock);
					}
					swap(list_value_, list_value);
				}
				try
				{
					for (auto&& element : list_value)
					{
						Consumer(element);
					}
				}
				catch (std::exception& e)
				{
					std::cout << e.what() << std::endl;
				}
			}
			is_running_ = false;
		});
		t.detach();
	}
}
