#include <thread>
#include <mutex>
#include <shared_mutex>
#include <fmt/format.h>
#include <fmt/ostream.h>

class ThreadSafeCounter
{
  mutable std::shared_mutex mtx_;
  uint32_t val_{0};

public:
  uint32_t get() const
  {
    std::shared_lock lock{mtx_};

    return val_;
  }

  void reset() 
  {
    std::unique_lock lock{mtx_};
    
    val_ = 0;
  }

  void increment()
  {
    std::unique_lock lock{mtx_};

    val_++;
  }
};


int main()
{
  auto counter = ThreadSafeCounter{};

  auto work = [&counter]() {
    for (size_t i = 0; i < 10; i++) {
      counter.increment();
      fmt::print("Thread: {0}, value after inc: {1}.\n", std::this_thread::get_id(), counter.get()); 
    }
  };

  auto thd1 = std::thread(work);
  auto thd2 = std::thread(work);
  auto thd3 = std::thread(work);
  auto thd4 = std::thread(work);
  auto thd5 = std::thread(work);
  auto thd6 = std::thread(work);

  thd1.join();
  thd2.join();
  thd3.join();
  thd4.join();
  thd5.join();
  thd6.join();
}