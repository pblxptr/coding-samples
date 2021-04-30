#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <fmt/format.h>
#include <string_view>
#include <random>
#include <thread>
#include "common.hpp"

int main()
{
  const auto queue_dsc = mq_open(queue_name.data(), O_WRONLY);

  if (queue_dsc == -1)
  {
    fmt::print("Error mq_open(). Errno {0}\n", strerror(errno));

    return 1;
  }

  auto buffer = std::array<char, MAX_MSG_SIZE>{};
  size_t msg_number{};

  auto random_device = std::random_device{};
  auto mt = std::mt19937{random_device()};
  auto num_dist = std::uniform_int_distribution<int>{};
  auto delay_dist = std::uniform_int_distribution<int>{1, 10};

  while (msg_number++ != 10)
  {
    std::fill(buffer.begin(), buffer.end(), '\0');

    auto msg = fmt::format("Test message num: {0}", num_dist(mt));
    fmt::print("Generated message: {0}\n", msg);
    std::copy(msg.begin(), msg.end(), buffer.begin());

    auto ret_code = mq_send(queue_dsc, buffer.data(), buffer.size(), 0);

    if (ret_code == -1)
    {
      fmt::print("Error while sending msg. Errno: {0}\n", strerror(errno));

      break;
    }

    std::this_thread::sleep_for(std::chrono::seconds(delay_dist(mt)));
  }

  fmt::print("Client finished sending messages.\n");

}