#include <iostream>
#include <fmt/format.h>
#include <fmt/ostream.h>
#include <thread>
#include <queue>
#include <mutex>
#include <shared_mutex>
#include <random>

template<class T>
class ThreadSafeQueue
{
  size_t count_{0};
  std::queue<T> items_;
  mutable std::shared_mutex mtx_;

public:
  void put(const T& item)
  {
    std::unique_lock lck{mtx_};

    items_.push(item);
    ++count_;
  }

  const T& get() const
  {
    std::shared_lock lck{mtx_};

    return items_.front();
  }

  T own_top()
  {
    std::unique_lock lck{mtx_};

    T val = items_.front();
    items_.pop();

    return val;
  }

  void pop()
  {
    std::unique_lock lck{mtx_};

    items_.pop();
    --count_;
  }

  [[nodiscard]] bool empty() const
  {
    std::shared_lock lck{mtx_};

    return items_.empty();
  }

  size_t count() const
  {
    std::shared_lock lck{mtx_};

    return count_;
  }
};

class Task
{ 
  uint32_t id_;
  std::chrono::seconds time_;
public:
  explicit Task(uint32_t id, const std::chrono::seconds& time)
    : id_{id}
    , time_{time}
  {}

  uint32_t id() const
  {
    return id_;
  }

  const auto time() const
  {
    return time_;
  }
};


//Out
void print(const std::string& s)
{
  static std::mutex mtx;

  std::unique_lock lck{mtx};
  
  fmt::print("Thread: {0} - {1}\n", std::this_thread::get_id(), s);
};


//Producer & Consumer
std::mutex mtx;
std::condition_variable cv;

template<class T>
void consumer(ThreadSafeQueue<T>& queue)
{
  print("Consumer started.");

  while (true)
  {
    print("Consumer before lock.");
    std::unique_lock lck{mtx};
    cv.wait(lck, [&](){return !queue.empty(); });
    print("Consumer after wait.");

    if (queue.empty())
    {
      continue;
    }

    auto task = queue.own_top();

    lck.unlock();

    print(fmt::format("Task - id: {0}, delay: {1}. Start waiting.", task.id(), task.time().count()));
    std::this_thread::sleep_for(std::chrono::seconds(task.time()));
    print(fmt::format("Task - id: {0} Stop waiting. Done!", task.id()));
  }
}


template<class T>
void producer(ThreadSafeQueue<T>& queue)
{
  print("Producer started.");

  auto rd = std::random_device{};
  auto gen = std::mt19937{rd()};
  auto task_delay_dist = std::uniform_int_distribution<uint32_t>{1, 10};
  auto task_count_dist = std::uniform_int_distribution<uint32_t>{5, 10};
  auto producer_delay_dist = std::uniform_int_distribution<uint32_t>{1, 10};
  const size_t loops = 5;
  static uint32_t task_id = 0;

  for (size_t i = 0; i < loops; i++)
  {
    const auto sleep_for = producer_delay_dist(gen);
    print(fmt::format("Producer - start sleep for: {0} s.", sleep_for));

    std::this_thread::sleep_for(std::chrono::seconds(sleep_for));

    print("Producer - woke up.");

    const auto tasks = task_count_dist(gen);

    print(fmt::format("Producer - numer of tasks: {0}", tasks));

    for (size_t j = 0; j < tasks; j++)
    {
      {
        std::unique_lock lck{mtx};

        const uint32_t task_delay = task_delay_dist(gen);
        auto task = Task{task_id++, std::chrono::seconds(task_delay)};
        queue.put(task);
      }

      cv.notify_one();
    }

  }
}

int main()
{
  auto queue = ThreadSafeQueue<Task>{};

  auto prod_thd = std::thread{producer<Task>, std::ref(queue)};

  constexpr size_t consumers_amout = 4;
  auto consumers = std::array<std::thread, consumers_amout>{};

  for (auto& c : consumers)
  {
    c = std::thread{consumer<Task>, std::ref(queue)};
  }

  prod_thd.join();

  for (auto& c : consumers)
  {
    c.join();
  }

}