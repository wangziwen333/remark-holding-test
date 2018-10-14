#include "my_producer_and_consumer.h"
#include "data_center.h"

bool MyProducerAndConsumer::FilterHandler(const int list_size, const std::string& in_value) const
{
	static const int MAX_LIST = 100000;
	if (list_size < MAX_LIST)
	{
		return true;
	}
	fprintf(stderr, "error: more than max list\n");
	return false;
}

void MyProducerAndConsumer::Consumer(const std::string& in_value)
{
	DataCenter::singleton::GetInstance()->SortedString(in_value);
	consumed_number_++;
}

void MyProducerAndConsumer::Producer(const std::string& in_value)
{
	ProducerAndConsumer::Producer(in_value);
	produced_number_++;
}

std::pair<int, int> MyProducerAndConsumer::GetReport() const
{
	return std::make_pair(static_cast<int>(produced_number_), consumed_number_);
}
