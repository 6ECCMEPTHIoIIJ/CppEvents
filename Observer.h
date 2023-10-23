#pragma once

template<typename ...TArgs>
class Observer;

#include "ICloseable.h"
#include "ObservableConnector.h"
#include "ObservableInvoker.h"
#include "Function.h"
#include "Action.h"
#include "CFunctor.h"
#include "CMethod.h"
#include "MultipleBindingException.h"
#include "MultipleSubscriptionException.h"
#include "NonSubscribedException.h"
#include "NonBoundInvokingException.h"

template<typename ...TArgs>
class Observer {
  friend class ObservableConnector<TArgs...>;

 public:
  Observer() = default;
  Observer(const Observer& other) = delete;
  Observer(Observer&& other) noexcept = delete;

  auto operator=(const Observer& other) -> Observer& = delete;
  auto operator=(Observer&& other) noexcept -> Observer& = delete;

  auto Subscribe(ObservableConnector<TArgs...>* observable) -> void {
    if (_unsubscriber != nullptr) throw MultipleSubscriptionException();
    _unsubscriber = observable->Subscribe(this);
  }

  auto Unsubscribe() -> void {
    if (_unsubscriber == nullptr) throw NonSubscribedException();

    _unsubscriber->Close();
    delete _unsubscriber;
    _unsubscriber = nullptr;
  }

  template<typename TFunctor, typename ...TBindArgs>
  requires CFunctor<TFunctor, TArgs..., TBindArgs...>
  auto Bind(const TFunctor& functor, TBindArgs&& ... bindArgs) -> void {
    if (_action != nullptr) throw MultipleBindingException();

    _action = new Action<TArgs...>(
        [functor, ...bindArgs = std::forward<TBindArgs>(bindArgs)](TArgs&& ... args) -> void {
          functor(std::forward<TArgs>(args)..., bindArgs...);
        });
  }

  template<typename TMethod, typename TOwner, typename ...TBindArgs>
  requires CMethod<TMethod, TOwner, TArgs..., TBindArgs...>
  auto Bind(const TMethod& method, TOwner* owner, TBindArgs&& ... bindArgs) -> void {
    if (_action != nullptr) throw MultipleBindingException();

    _action = new Action<TArgs...>(
        [method, owner, ...bindArgs = std::forward<TBindArgs>(bindArgs)](TArgs&& ... args) -> void {
          (owner->*method)(std::forward<TArgs>(args)..., bindArgs...);
        });
  }

  template<typename ...TBindArgs>
  auto Bind(Observer<TArgs..., TBindArgs...>* observer, TBindArgs&& ... bindArgs) -> void {
    if (_action != nullptr) throw MultipleBindingException();

    _action = [observer, ...bindArgs = std::forward<TBindArgs>(bindArgs)](TArgs&& ... args) -> void {
      observer->Invoke(std::forward<TArgs>(args)..., bindArgs...);
    };
  }

  template<typename ...TBindArgs>
  auto Bind(ObservableInvoker<TArgs..., TBindArgs...>* invoker, TBindArgs&& ... bindArgs) -> void {
    if (_action != nullptr) throw MultipleBindingException();

    _action = [invoker, ...bindArgs = std::forward<TBindArgs>(bindArgs)](TArgs&& ... args) -> void {
      invoker->Invoke(std::forward<TArgs>(args)..., bindArgs...);
    };
  }

  virtual ~Observer() {
    if (_unsubscriber == nullptr) return;
    Unsubscribe();
  }

 private:
  auto Invoke(TArgs&& ... args) -> void {
    if (_action == nullptr) throw NonBoundInvokingException();
    std::invoke(*_action, std::forward<TArgs>(args)...);
  }

  Action<TArgs...>* _action = nullptr;
  ICloseable* _unsubscriber = nullptr;
}; // Observer