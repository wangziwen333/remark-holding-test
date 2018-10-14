// ģ�庯��
// �ṩFIFO�Ķ���ģʽ
// ���г�ջΪ��������
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
		// @function���������������ݹ���������
		void SetFilterHandler(const FilterHandlerType& filter_handler) { filter_handler_ = filter_handler; };
	protected:
		// ����������Ĭ��ֵ��Ĭ��Ϊ�����ˡ�
		FilterHandlerType filter_handler_ = bind(&DataFilter::FilterHandler, this, std::placeholders::_1, std::placeholders::_2);
	private:
		//@function�����������ݹ�������
		//@param��list_size����ǰ���ݶ��д�С��
		//@param��in_value����ǰ��������Ԫ�أ�
		//@return��true����������Ҫ��������ݣ���false�������߲���Ҫ��������ݣ�
		virtual bool FilterHandler(const int list_size, const TValue& in_value) const { return true; };
	};

	template <class TValue>
	class ProducerAndConsumer : public DataFilter <TValue>
	{
	public:
		// @function�������ߺ������ڶ���ĩβ������������ݡ�
		virtual void Producer(const TValue& in_value);
		virtual void Consumer(const TValue& in_value) = 0;
		// @function���������߳�������ģʽ
		void Run();
		~ProducerAndConsumer();
		// @function������������ֱ���߳��˳��ŷ��أ�ͨ����Ϣ֪ͨ�����˳�����
		void WartForStop();
	private:
		std::list <TValue> list_value_;
		std::mutex mutex_;
		std::condition_variable condition_varable_;
		// �Ƿ�ֹͣ�������߳�
		volatile bool is_to_stop_ = false;
		// �������߳��Ƿ���������
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
