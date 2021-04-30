#include <thread>
#include <fmt/format.h>
#include <fmt/ostream.h>
#include <condition_variable>

std::mutex mtx;
std::condition_variable cv;
bool data_ready = false;


std::mutex print_mtx;

void print(const std::string& msg)
{
  std::unique_lock lock{print_mtx};

  fmt::print("{0}\n", msg);
}


void consumer()
{
  print("Consumer\n");

  std::unique_lock lock{mtx};
  cv.wait(lock, []() { return data_ready; });

  print("Consumer ready to work.");
}

void producer()
{
  print("Producer started.");
  print("Producer sleeping.");

  std::this_thread::sleep_for(std::chrono::seconds(5));

  print("Producent ready.");

  {
    std::unique_lock lock{mtx};
    data_ready = true;

    cv.notify_one();

  }
}

int main()
{
  std::thread thd1 {consumer};
  std::thread thd2 {producer};

  thd1.join();
  thd2.join();
}