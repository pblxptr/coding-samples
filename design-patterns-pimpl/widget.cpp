#include "widget.hpp"
#include "gadget.hpp"

#include <string>

struct Widget::Impl {
  Impl(const std::string& name, const Gadget& gadget)
    : name_{name}
    , gadget_{gadget}
  {}

  std::string name_;
  Gadget gadget_;
};

Widget::Widget(const Gadget& gadget)
  : impl_{std::make_unique<Widget::Impl>(
    "Default name",
    gadget
  )}
{}

Widget::~Widget() = default;

Widget::Widget(const Widget& rhs)
  : impl_{std::make_unique<Widget::Impl>(*rhs.impl_)}
{}

Widget& Widget::operator=(const Widget& rhs)
{
  *impl_ = *rhs.impl_;

  return *this;
}


uint32_t Widget::get_id() const
{
  return impl_->gadget_.id_;
}

const std::string& Widget::get_name() const
{
  return impl_->name_;
}