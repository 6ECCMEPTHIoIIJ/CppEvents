#pragma once

#include <stdexcept>

struct NonBoundInvokingException : public std::runtime_error {
  NonBoundInvokingException() : std::runtime_error("") {}
};
