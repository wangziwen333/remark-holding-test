#include <stdlib.h>
#include "data_center.h"
#include "my_producer_and_consumer.h"
#include <chrono>
#include <thread>

// 线程停止标志位
volatile bool g_is_to_stop = false;

// 同步处理模式测试用例
void TestSyncDealMode()
{
	printf("\ttest sync deal mode start ...\n");
	g_is_to_stop = false;
	auto count = 0;
	while (!g_is_to_stop)
	{
		DataCenter::singleton::GetInstance()->SortedString(DataCenter::singleton::GetInstance()->GetRandString());
		count++;
	}
	printf("\ttest sync deal mode finished: [numbers=%d]\n", count);
}

// 生产消费模式测试用例
void TestProducerAndConsumerMode()
{
	printf("\ttest producer and consumer mode start ...\n");
	g_is_to_stop = false;
	MyProducerAndConsumer my_producer_and_consumer;
	my_producer_and_consumer.Run();
	while (!g_is_to_stop)
	{
		my_producer_and_consumer.Producer(DataCenter::singleton::GetInstance()->GetRandString());
	}
	auto result = my_producer_and_consumer.GetReport();
	printf("\ttest producer and consumer mode finished: [produced_numbers=%d,consumed_numbers=%d]\n", result.first, result.second);
	my_producer_and_consumer.WartForStop();
}

// linux环境特有设置进程亲和力函数
#ifdef __linux__
//#include<sys/sysinfo.h>
//#include<unistd.h>
void SetCpuNumber(const uint8_t cpu_number)
{
	cpu_set_t mask;

	// 先进行清空，然后设置掩码
	CPU_ZERO(&mask);
	CPU_SET(cpu_number, &mask);

	//设置进程的亲和力
	if (sched_setaffinity(0, sizeof(mask), &mask) == -1)
	{
		printf("warning: could not set CPU affinity, continuing...\n");
	}
}
#endif

int main()
{
	auto const test_seconds = 100;
	for (auto i = 0; i < 2; i++)
	{
		if (0 == i)
		{
			printf("--------------------multiple cpu test---------------------\n");
		}
		else
		{
			// 如果非linux环境，则不进行单核CPU测试
#ifdef __linux__
			printf("--------------------single cpu test-----------------------\n");
			SetCpuNumber(0);
#else
			break;
#endif
		}
	
		// 单线程模式测试
		printf("%d seconds test start ...\n", 2 * test_seconds);
		std::thread t1([] { TestSyncDealMode(); });
		std::this_thread::sleep_for(std::chrono::seconds(test_seconds));
		g_is_to_stop = true;
		t1.join();

		// 生产消费模式测试
		std::thread t2([] { TestProducerAndConsumerMode(); });
		std::this_thread::sleep_for(std::chrono::seconds(test_seconds));
		g_is_to_stop = true;
		t2.join();
	}
	return EXIT_SUCCESS;
}
