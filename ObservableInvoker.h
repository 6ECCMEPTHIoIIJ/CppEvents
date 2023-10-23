#pragma once

template<typename ...TArgs>
class ObservableInvoker;

#include "ObservableConnector.h"

template<typename ...TArgs>
class ObservableInvoker {
 public:
  ObservableInvoker() = delete;
  ObservableInvoker(const ObservableInvoker& other) = delete;
  ObservableInvoker(ObservableInvoker&& other) noexcept = delete;

  explicit ObservableInvoker(ObservableConnector<TArgs...>* connector) : _connector(connector) {}

  auto operator=(const ObservableInvoker& other) -> ObservableInvoker& = delete;
  auto operator=(ObservableInvoker&& other) noexcept -> ObservableInvoker& = delete;

  auto Invoke(TArgs&& ... args) -> void {
    _connector->Invoke(std::forward<TArgs>(args)...);
  }

  virtual ~ObservableInvoker() = default;

 private:
  ObservableConnector<TArgs...>* _connector;
}; // ObservableInvoker
