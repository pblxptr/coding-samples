#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <fmt/format.h>
#include <string_view>
#include "common.hpp"

int main()
{
  const auto qeue_dsc = mq_open(queue_name.data(), O_RDWR | O_CREAT, 0666, NULL);

  if (qeue_dsc == -1)
  {
    fmt::print("Fail: mq_open(). Errno: {0}\n", strerror(errno));
    return 1;
  }

  auto new_attrs = mq_attr{};
  mq_getattr(qeue_dsc, &new_attrs);

  fmt::print("Attributes. Maxmsg: {0}, max msg size: {1}\n", new_attrs.mq_maxmsg, new_attrs.mq_msgsize);

  auto buffer = std::array<char, MAX_MSG_SIZE>{};
  unsigned int priority{};

  size_t msg_received{};
  do 
  {
    std::fill(buffer.begin(), buffer.end(), '\0');

    auto ret_code = mq_receive(qeue_dsc, buffer.data(), MAX_MSG_SIZE, &priority);

    if (ret_code == -1)
    {
      fmt::print("Error while receiving. Errno: {0}\n", strerror(errno));

      break;
    }

    auto str = std::string{buffer.begin(), buffer.end()};

    fmt::print("Received msg: {0}\n", str);


  } while(++msg_received != 10);


  fmt::print("Closing mq.\n");
  mq_unlink(queue_name.data());
  mq_close(qeue_dsc);
}