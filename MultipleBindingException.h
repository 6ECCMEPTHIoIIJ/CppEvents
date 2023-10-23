#pragma once

#include <stdexcept>

struct MultipleBindingException : public std::runtime_error {
  MultipleBindingException() : std::runtime_error("") {}
};
