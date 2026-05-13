# Kafka + C++ on Linux — Practical Cheat Sheet

## 0. Install Requirements

```bash
sudo apt update

# Java
sudo apt install openjdk-17-jdk -y

# C++ build tools
sudo apt install build-essential cmake pkg-config -y

# Kafka C/C++ client
sudo apt install librdkafka-dev -y
```

Verify:

```bash
java -version
g++ --version
```

---

# 1. Download Kafka

Official downloads:

* [Apache Kafka Downloads](https://kafka.apache.org/community/downloads/?utm_source=chatgpt.com)
* [Kafka Quickstart](https://kafka.apache.org/41/getting-started/quickstart/?utm_source=chatgpt.com)

Download:

```bash
wget https://downloads.apache.org/kafka/4.2.0/kafka_2.13-4.2.0.tgz
```

Extract:

```bash
tar -xzf kafka_2.13-4.2.0.tgz
cd kafka_2.13-4.2.0
```

---

# 2. Start Kafka (Modern KRaft Mode)

## Generate cluster ID

```bash
KAFKA_CLUSTER_ID="$(bin/kafka-storage.sh random-uuid)"
```

---

## Format storage

```bash
bin/kafka-storage.sh format \
  --standalone \
  -t $KAFKA_CLUSTER_ID \
  -c config/server.properties
```

---

## Start broker

```bash
bin/kafka-server-start.sh config/server.properties
```

Kafka now runs at:

```text
localhost:9092
```

---

# 3. Stop Kafka

Ctrl+C in the broker terminal.

---

# 4. Create Topic

Open another terminal.

```bash
cd kafka_2.13-4.2.0
```

Create:

```bash
bin/kafka-topics.sh \
  --create \
  --topic test-topic \
  --bootstrap-server localhost:9092
```

---

# 5. List Topics

```bash
bin/kafka-topics.sh \
  --list \
  --bootstrap-server localhost:9092
```

---

# 6. Describe Topic

```bash
bin/kafka-topics.sh \
  --describe \
  --topic test-topic \
  --bootstrap-server localhost:9092
```

---

# 7. Kafka CLI Producer

Write messages manually:

```bash
bin/kafka-console-producer.sh \
  --topic test-topic \
  --bootstrap-server localhost:9092
```

Type:

```text
hello
world
foo
bar
```

---

# 8. Kafka CLI Consumer

Read messages:

```bash
bin/kafka-console-consumer.sh \
  --topic test-topic \
  --from-beginning \
  --bootstrap-server localhost:9092
```

---

# 9. Compile C++ Kafka Program

Using `librdkafka`:

```bash
g++ producer.cpp -o producer -lrdkafka++
g++ consumer.cpp -o consumer -lrdkafka++
```

---

# 10. Run C++ Producer

```bash
./producer
```

---

# 11. Run C++ Consumer

```bash
./consumer
```

---

# 12. Typical Terminal Setup

## Terminal 1 — Kafka Broker

```bash
cd kafka_2.13-4.2.0

bin/kafka-server-start.sh config/server.properties
```

---

## Terminal 2 — Consumer

```bash
./consumer
```

---

## Terminal 3 — Producer

```bash
./producer
```

---

# 13. Common Kafka Configs

## Producer

```text
bootstrap.servers=localhost:9092
acks=all
compression.type=zstd
linger.ms=5
```

---

## Consumer

```text
group.id=my-group
auto.offset.reset=earliest
enable.auto.commit=true
```

---

# 14. Useful Kafka Commands

## Delete topic

```bash
bin/kafka-topics.sh \
  --delete \
  --topic test-topic \
  --bootstrap-server localhost:9092
```

---

## View consumer groups

```bash
bin/kafka-consumer-groups.sh \
  --bootstrap-server localhost:9092 \
  --list
```

---

## Describe consumer group

```bash
bin/kafka-consumer-groups.sh \
  --bootstrap-server localhost:9092 \
  --describe \
  --group my-group
```

---

# 15. Important Kafka Concepts

| Concept        | Meaning            |
| -------------- | ------------------ |
| Broker         | Kafka server       |
| Topic          | Stream/log         |
| Partition      | Ordered shard      |
| Offset         | Message index      |
| Producer       | Writes messages    |
| Consumer       | Reads messages     |
| Consumer Group | Parallel consumers |
| Replication    | Fault tolerance    |

---

# 16. Directory Structure You’ll Use Often

```text
kafka_2.13-4.2.0/
├── bin/
├── config/
├── libs/
├── logs/
```

---

# 17. Most Important Files

| File                            | Purpose          |
| ------------------------------- | ---------------- |
| `config/server.properties`      | Broker config    |
| `logs/`                         | Kafka logs       |
| `bin/kafka-topics.sh`           | Topic management |
| `bin/kafka-console-producer.sh` | CLI producer     |
| `bin/kafka-console-consumer.sh` | CLI consumer     |

---

# 18. Typical Dev Workflow

```text
Start Kafka
    ↓
Create topic
    ↓
Start consumer
    ↓
Run producer
    ↓
Observe messages
```

---

# 19. Minimal Mental Model

Kafka is basically:

```text
append-only distributed log
```

Producer:

```text
writes messages sequentially
```

Consumer:

```text
tracks current offset
```

Topic partition:

```text
[0][1][2][3][4][5]...
```

Consumer remembers:

```text
"I last read offset 3"
```

---

# 20. Common Errors

## Broker not running

```text
Connection refused
```

Start Kafka.

---

## Topic does not exist

```text
Unknown topic
```

Create topic first.

---

## Port already in use

```text
Address already in use
```

Kill previous Kafka process:

```bash
lsof -i :9092
kill -9 <pid>
```

---

# 21. Useful References

* [Apache Kafka Official Site](https://kafka.apache.org/?utm_source=chatgpt.com)
* [Kafka Downloads](https://kafka.apache.org/downloads.html?utm_source=chatgpt.com)
* [Kafka Quickstart Guide](https://kafka.apache.org/41/getting-started/quickstart/?utm_source=chatgpt.com)

