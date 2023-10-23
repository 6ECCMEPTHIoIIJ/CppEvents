#pragma once

#include <functional>

template<typename TReturn, typename  ...TArgs>
using Function = std::function<TReturn(TArgs...)>;