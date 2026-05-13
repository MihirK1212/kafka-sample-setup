#include <iostream>
#include <string>
#include <librdkafka/rdkafkacpp.h>

int main() {
    std::string brokers = "localhost:9092";
    std::string topic_name = "test-topic";

    std::string errstr;

    RdKafka::Conf* conf = RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL);

    conf->set("bootstrap.servers", brokers, errstr);

    RdKafka::Producer* producer =
        RdKafka::Producer::create(conf, errstr);

    if (!producer) {
        std::cerr << "Producer creation failed: "
                  << errstr << std::endl;
        return 1;
    }

    delete conf;

    for (int i = 0; i < 10; ++i) {
        std::string msg = "hello kafka " + std::to_string(i);

        RdKafka::ErrorCode resp = producer->produce(
            topic_name,
            RdKafka::Topic::PARTITION_UA,
            RdKafka::Producer::RK_MSG_COPY,
            const_cast<char*>(msg.c_str()),
            msg.size(),
            nullptr,
            0,
            0,
            nullptr
        );

        if (resp != RdKafka::ERR_NO_ERROR) {
            std::cerr << "Produce failed: "
                      << RdKafka::err2str(resp)
                      << std::endl;
        } else {
            std::cout << "Produced: " << msg << std::endl;
        }

        producer->poll(0);
    }

    producer->flush(5000);

    delete producer;

    return 0;
}
