#pragma once

template<typename ...TArgs>
class ObservableConnector;

#include <unordered_set>

#include "ICloseable.h"
#include "Observer.h"
#include "ObservableInvoker.h"
#include "MultipleSubscriptionException.h"

template<typename ...TArgs>
class ObservableConnector {
  friend class Observer<TArgs...>;
  friend class ObservableInvoker<TArgs...>;

 public:
  ObservableConnector() = default;
  ObservableConnector(const ObservableConnector& other) = delete;
  ObservableConnector(ObservableConnector&& other) noexcept = delete;

  auto operator=(const ObservableConnector& other) -> ObservableConnector& = default;
  auto operator=(ObservableConnector&& other) noexcept -> ObservableConnector& = default;

  virtual ~ObservableConnector() = default;

 private:
  auto Subscribe(Observer<TArgs...>* subscriber) -> ICloseable* {
    if (_subscribers->contains(subscriber)) throw MultipleSubscriptionException();

    _subscribers->insert(subscriber);
    return new Unsubscriber(_subscribers, subscriber);
  }

  auto Invoke(TArgs&& ... args) -> void {
    for (auto subscriber : *_subscribers) {
      subscriber->Invoke();
    }
  }

  std::unordered_set<Observer<TArgs...>*>* _subscribers =
      new std::unordered_set<Observer<TArgs...>*>();

  class Unsubscriber final : public ICloseable {
   public:
    Unsubscriber() = delete;
    Unsubscriber(const Unsubscriber& other) = delete;
    Unsubscriber(Unsubscriber&& other) noexcept = delete;

    Unsubscriber(std::unordered_set<Observer<TArgs...>*>* subscribers, Observer<TArgs...>* subscriber) :
        _subscribers(subscribers), _subscriber(subscriber) {}

    auto operator=(const Unsubscriber& other) -> Unsubscriber& = default;
    auto operator=(Unsubscriber&& other) noexcept -> Unsubscriber& = default;

    virtual auto Close() -> void override {
      _subscribers->erase(_subscriber);
      _subscriber = nullptr;
      _subscribers = nullptr;
    }

    virtual ~Unsubscriber() override {
      if (_subscribers == nullptr || _subscriber == nullptr) return;
      this->Close();
    }

   private:
    std::unordered_set<Observer<TArgs...>*>* _subscribers;
    Observer<TArgs...>* _subscriber;
  };
}; // ObservableConnector