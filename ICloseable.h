#pragma once

struct ICloseable {
  virtual auto Close() -> void = 0;
  virtual ~ICloseable() = default;
};
