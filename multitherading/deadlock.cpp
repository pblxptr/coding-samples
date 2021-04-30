#include <thread>
#include <mutex>
#include <fmt/format.h>
#include <fmt/ostream.h>

std::mutex mtx1;
std::mutex mtx2;

std::mutex print_mtx;

void print(const std::string& msg)
{
  std::unique_lock lock{print_mtx};

  fmt::print("{0}", msg);
}

// DEADLOCK 
void worker()
{
  for(size_t i = 0; i < 10000; i++)
  {
    print(fmt::format("Worker 1 Aquire mtx1\n"));
    mtx1.lock();

    print(fmt::format("Worker 1 Aquire mtx2\n"));
    mtx2.lock();

    print(fmt::format("Worker 1 Locks aquired\n"));

    mtx2.unlock();
    mtx1.unlock();

    print(fmt::format("Worker 1 Locks released\n"));
  }
}

void worker2()
{
  for(size_t i = 0; i < 10000; i++)
  {
    fmt::print("Worker2\n");

    print(fmt::format("Worker 2 Aquire mtx2\n"));
    mtx2.lock();

    print(fmt::format("Worker 2 Aquire mtx1\n"));
    mtx1.lock();

    print(fmt::format("Worker 2 Locks aquired\n"));

    mtx1.unlock();
    mtx2.unlock();

    print(fmt::format("Worker 2 Locks released\n"));
  }
}

//Without deadlock
// void worker()
// {
//   for(size_t i = 0; i < 10000; i++)
//   {
//     print(fmt::format("Worker 1 Aquire mtx1\n"));
//     print(fmt::format("Worker 1 Aquire mtx2\n"));

//     std::scoped_lock lock{mtx1, mtx2};

//     print(fmt::format("Worker 1 Locks aquired\n"));
//     print(fmt::format("Worker 1 Locks released\n"));
//   }
// }

// void worker2()
// {
//   for(size_t i = 0; i < 10000; i++)
//   {
//     fmt::print("Worker2\n");

//     print(fmt::format("Worker 2 Aquire mtx2\n"));
//     print(fmt::format("Worker 2 Aquire mtx1\n"));
    
//     std::scoped_lock lock{mtx2, mtx1};

//     print(fmt::format("Worker 2 Locks aquired\n"));
//     print(fmt::format("Worker 2 Locks released\n"));
//   }
// }




int main()
{
  std::thread thd1(worker);
  std::thread thd2(worker2);
  std::thread thd3(worker);
  std::thread thd4(worker2);

  thd1.join();
  thd2.join();
  thd3.join();
  thd4.join();
}