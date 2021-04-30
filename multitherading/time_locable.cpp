#include <thread>
#include <mutex>
#include <fmt/format.h>
#include <random>
#include <fmt/ostream.h>

std::timed_mutex mtx;
static int counter = 0;
static uint32_t dealay;

void worker()
{
  if (!mtx.try_lock_for(std::chrono::seconds(5)))
  {
    fmt::print("Thread: {0} - failed to aquire a lock.\n", std::this_thread::get_id());

    return;
  }

  fmt::print("Thread {0} - got a mtx!\n", std::this_thread::get_id());
  std::this_thread::sleep_for(std::chrono::seconds(dealay));

  counter++;

  fmt::print("Thread {0} - releasing a mtx!\n", std::this_thread::get_id());

  mtx.unlock();
}

int main()
{
  auto rd = std::random_device{};
  auto gen = std::mt19937{rd()};
  auto dist = std::uniform_int_distribution<uint32_t>{1, 10};
  dealay = dist(gen);

  fmt::print("Waiting for: {0}.\n", dealay);

  std::thread thd1(worker);
  std::thread thd2(worker);

  thd1.join();
  thd2.join();
}