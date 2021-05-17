#pragma once

#include <memory>

class Gadget;

class IWidget
{
public:
  IWidget() = default;
  virtual ~IWidget() = default;
  IWidget(IWidget&&) = default;
  IWidget(const IWidget&) = default;
  IWidget& operator=(IWidget&&) = default;
  IWidget& operator=(IWidget&) = default;

  virtual uint32_t get_id() const = 0;
  virtual const std::string& get_name() const = 0;
};

class Widget : public IWidget
{
public:
  explicit Widget(const Gadget&);
  ~Widget(); //needs to be defined here, since dtor is inlined and it can't be, otherwise it casues sizeof(error).
  Widget(Widget&&) = default;
  Widget(const Widget&); //custom copy ctor must be provided in order to make type with uptr copyable
  
  Widget& operator=(Widget&&) = default;
  Widget& operator=(const Widget&); //custom copy operator must be provided in order to make type with uptr copyable


  uint32_t get_id() const override;
  const std::string& get_name() const override;
private:
  struct Impl;
  std::unique_ptr<Impl> impl_;
};