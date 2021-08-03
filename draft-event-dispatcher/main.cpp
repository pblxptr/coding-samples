#include <string>
#include <iostream>
#include <fmt/format.h>
#include <memory>
#include <boost/signals2.hpp>
#include <fmt/format.h>

template<class T>
class EventDispatcherI;
class EventDispatcher;

using EventDispatcher_t = EventDispatcherI<EventDispatcher>;

template<class T>
using Subscriber_t = typename boost::signals2::signal<void (T)>::slot_type; 

template<class T>
struct EventDispatcherI
{
  friend T;
private:
  EventDispatcherI() = default; 
public:
  template<class TEvent>
  void publish(TEvent&& event)
  {
    static_cast<T&>(*this).do_publish(std::forward<TEvent>(event));
  }

  template<class TEvent>
  void subscribe(const Subscriber_t<TEvent>& subscriber)
  {
    static_cast<T&>(*this). template do_subscribe<TEvent>(subscriber);
  }
};

template<class T>
struct Observable
{
  public:
protected:
  void observable_publish(T event)
  {
    onEvent_(event);
  }

  void observable_subscribe(const Subscriber_t<T>& subscriber) 
  {
    onEvent_.connect(subscriber);
  }
private:
  boost::signals2::signal<void(T)> onEvent_;
};


struct Event1 {};
struct Event2 {};

struct Event1Handler1
{
  void operator()(Event1 event) const
  {
    fmt::print("Event1Handler1::handle\n");
  }
};

struct Event1Handler2
{
  void operator()(Event1 event) const
  {
    fmt::print("Event1Handler2::handle\n");
  }
};

struct Event2Handler1
{
  void operator()(Event2 event) const
  {
    fmt::print("Event2Handler1::handle\n");
  }
};

struct Event2Handler2
{
  void operator()(Event2 event) const
  {
    fmt::print("Event2Handler2::handle\n");
  }
};


struct EventDispatcher : EventDispatcherI<EventDispatcher>,
  Observable<Event1>,
  Observable<Event2>
{
public:

  template<class TEvent>
  void do_publish(TEvent&& event)
  {
    using Observable_t = Observable<std::decay_t<TEvent>>; 
    Observable_t::observable_publish(std::forward<TEvent>(event));
  }

  template<class TEvent>
  void do_subscribe(const Subscriber_t<TEvent>& subscriber)
  {
    using Observable_t = Observable<std::decay_t<TEvent>>; 
    Observable_t::observable_subscribe(subscriber);
  }
};


int main()
{
  auto dispatcher = EventDispatcher();
  auto handler1_1 = Event1Handler1{};
  auto handler1_2 = Event1Handler2{};

  auto handler2_1 = Event2Handler1{};
  auto handler2_2 = Event2Handler2{};

  dispatcher.subscribe<Event1>(handler1_1);
  dispatcher.subscribe<Event1>(handler1_2);
  dispatcher.subscribe<Event2>(handler2_1);
  dispatcher.subscribe<Event2>(handler2_2);

  dispatcher.publish(Event1{});
  dispatcher.publish(Event2{});

}
