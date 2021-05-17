#include "widget.hpp"
#include "gadget.hpp"
#include <fmt/format.h>

int main() 
{ 
  auto w1 = std::make_unique<Widget>(Gadget{123});

  fmt::print("Widget id: {}\n", w1->get_id());
  fmt::print("Widget name: {}\n", w1->get_name());


  auto w2 = Widget{Gadget{997}};
  auto w3 = w2;
  
  fmt::print("w3 id: {}, should be 997\n", w3.get_id());
  
  w3 = *w1;

  fmt::print("w3 id: {}, should be 123\n", w3.get_id());
  
  return 0;
}
