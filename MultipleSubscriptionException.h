#pragma once

#include <stdexcept>

struct MultipleSubscriptionException : public std::runtime_error {
  MultipleSubscriptionException() : std::runtime_error("") {}
};