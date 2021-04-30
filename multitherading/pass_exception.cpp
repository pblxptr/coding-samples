#include <iostream>
#include <stdexcept>
#include <thread>

void background_work(std::exception_ptr& e)
{
  try 
  {
    throw std::runtime_error{"Error!"};
  }
  catch(...)
  {
    e = std::current_exception();
  }
}

int main()
{
  std::exception_ptr thd_excpt;

  std::thread thd{&background_work, std::ref(thd_excpt)};

  thd.join();

  try
  {
    std::rethrow_exception(thd_excpt);
  }
  catch(const std::exception& e)
  {
    std::cerr << e.what() << '\n';
  }
  
}