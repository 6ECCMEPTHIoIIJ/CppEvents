#pragma once

#include "Function.h"

template<typename  ...TArgs>
using Action = Function<void, TArgs...>;

