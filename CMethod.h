#pragma once

template<typename T, typename TOwner, typename ...TArgs>
concept CMethod = requires(T method, TOwner* owner, TArgs&&... args) {
  {(owner->*method)(args...)};
};