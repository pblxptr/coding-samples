#include <string>
#include <iostream>
#include <fmt/format.h>

void f(int x) {}

template<class T>
void fwd(T&& arg)
{
  f(std::forward<T>(arg));
}

struct IPv4Header
{
  uint32_t version  : 4,
    header_length   : 4,
    type_of_service : 6,
    ECN             : 2,
    total_length    : 16;
};

struct Widget
{
  static const int XConst = 20;
};



void f_for_pointer_case(int* ptr) {}

template<class T>
void fwd_for_pointer_case(T&& arg)
{
  f_for_pointer_case(std::forward<T>(arg));
}


void f_for_function_overload_ex(int x, int y) {}
void f_for_function_overload_ex(int x) {}
void f_for_function_overload(void (*fn)(int)) {}

template<class T>
void fwd_for_function_overload(T&& arg)
{
  f_for_function_overload(std::forward<T>(arg));
}

int main()
{
  //#1 Bit fields 
  auto ipv4_header = IPv4Header{};
  f(ipv4_header.ECN);
  // fwd(ipv4_header.ECN); //fails to compile as bitfields cannot be bound to non-const references

  //#2 Static const belonging to class without specifying the storage
  f(Widget::XConst); //this compiles because in this case there is no need to address XConst. It's simply replaced by 20.
  // fwd(Widget::XConst); // fails to link, as the static XConst defined in Widget needs to have it's storage so compiler could address it. 

  //#3 Null pointers expressed as 0 or NULL.
  f_for_pointer_case(NULL);
  f_for_pointer_case(0);
  f_for_pointer_case(nullptr);
  // fwd_for_pointer_case(NULL); //fails to compile as NULL is deduced to be long
  // fwd_for_pointer_case(0); //fails to compile as 0 is decudec to be int
  fwd_for_pointer_case(nullptr);

  //#4 Function overloads
  f_for_function_overload(f_for_function_overload_ex);
  // fwd_for_function_overload(f_for_function_overload_ex); // fails to compile because fwd cannot figure out which function address should it take
  using func_t = void(*)(int);
  fwd_for_function_overload(static_cast<func_t>(f_for_function_overload_ex));
}