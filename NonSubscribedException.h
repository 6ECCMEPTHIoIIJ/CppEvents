#pragma once

#include <stdexcept>

struct NonSubscribedException : public std::runtime_error {
  NonSubscribedException() : std::runtime_error("") {}
};