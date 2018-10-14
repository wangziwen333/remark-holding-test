// 不带参数构造函数单例，不可继承、拷贝
// 通过友元方式实现
// 用例如下：
// class a
// {
//		DEFINE_SINGLETON_CONSTRUCT_NO_PARAM(a);
// }
// 表名a为无参数构造函数单例
#pragma once
#include <mutex>

namespace utils
{
	class noncopyable
	{
	protected:
		noncopyable() = default;
		~noncopyable() = default;
		noncopyable(const noncopyable&) = delete;
		noncopyable& operator=(const noncopyable&) = delete;
	};

	template <typename T>
	class SingletonConstructNoParam : private noncopyable
	{
	public:
		static T* GetInstance()
		{
			if (nullptr == instance_)
			{
				std::lock_guard<std::mutex> lock(mutex_);
				if (nullptr == instance_)
				{
					instance_ = new T;
				}
			}
			return instance_;
		}
	private:
		SingletonConstructNoParam();
		static T* instance_;
		static std::mutex mutex_;
	};
	template <typename T> T* SingletonConstructNoParam<T>::instance_ = nullptr;
	template <typename T> std::mutex SingletonConstructNoParam<T>::mutex_;

#define DEFINE_SINGLETON_CONSTRUCT_NO_PARAM(class_name); \
	public: \
	friend class utils::SingletonConstructNoParam<class_name>; \
	typedef utils::SingletonConstructNoParam<class_name> singleton; \
	private: \
	class_name(); \
	~class_name() {} 
} // namespace utils
