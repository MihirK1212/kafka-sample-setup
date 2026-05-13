#include <iostream>
#include <string>
#include <vector>
#include <librdkafka/rdkafkacpp.h>

int main() {
    std::string brokers = "localhost:9092";
    std::string topic_name = "test-topic";

    std::string errstr;

    RdKafka::Conf* conf =
        RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL);

    conf->set("bootstrap.servers", brokers, errstr);
    conf->set("group.id", "cpp-group", errstr);
    conf->set("auto.offset.reset", "earliest", errstr);

    RdKafka::KafkaConsumer* consumer =
        RdKafka::KafkaConsumer::create(conf, errstr);

    if (!consumer) {
        std::cerr << "Consumer creation failed: "
                  << errstr << std::endl;
        return 1;
    }

    delete conf;

    consumer->subscribe({topic_name});

    while (true) {
        RdKafka::Message* msg = consumer->consume(1000);

        switch (msg->err()) {
            case RdKafka::ERR_NO_ERROR:
                std::cout << "Received: "
                          << static_cast<const char*>(msg->payload())
                          << std::endl;
                break;

            case RdKafka::ERR__TIMED_OUT:
                break;

            default:
                std::cerr << "Consumer error: "
                          << msg->errstr()
                          << std::endl;
        }

        delete msg;
    }

    consumer->close();
    delete consumer;

    return 0;
}
