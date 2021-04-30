#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <fmt/format.h>
#include <spdlog/spdlog.h>

static uint32_t counter = 0;
std::mutex mtx;

void worker_lock()
{
    mtx.lock();
    
    std::cout << "Thread:"  << std::this_thread::get_id() << " incrementing value.\n"; 
    counter++;

    mtx.unlock();
}

void do_lock()
{
  auto hw_cores = std::thread::hardware_concurrency();
  auto threads = std::vector<std::thread>{hw_cores};

  for (auto& t : threads)
  {
    t = std::thread{worker_lock};
  }

  for (auto& t : threads)
  {
    t.join();
  }
}

//Try lock
static uint32_t counter2 = 0;
std::mutex mtx2;


void worker_try_lock()
{
  do {
    std::cout << "Cannot aquire the lock.\n";
  } while(!mtx2.try_lock());

  std::cout << "Thread:"  << std::this_thread::get_id() << " incrementing value2.\n"; 
  counter++;

  mtx2.unlock();
}


void do_try_lock()
{
  auto hw_cores = std::thread::hardware_concurrency();
  auto threads = std::vector<std::thread>{hw_cores};

  for (auto& t : threads)
  {
    t = std::thread{worker_try_lock};
  }

  for (auto& t : threads)
  {
    t.join();
  }
}


int main()
{
  do_lock();
  do_try_lock();
}