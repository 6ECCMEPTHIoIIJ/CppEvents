#pragma once

template<typename T, typename ...TArgs>
concept CFunctor = requires(T functor, TArgs&&... args) {
  {functor(args...)};
};