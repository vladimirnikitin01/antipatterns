//
// Created by dmitry on 5/16/20.
//

#ifndef ANTIPATTERNS_RANDOMDEVICE_H
#define ANTIPATTERNS_RANDOMDEVICE_H

#include <random>

class RandomDevice {
public:
  static RandomDevice &Instance();

  template <typename T>
  T RandomNumber(T first, T second);

private:
  RandomDevice();

  std::random_device random_device_;
  std::mt19937 m_twister_;
};

template <typename T> T RandomDevice::RandomNumber(T first, T second) {
  if constexpr (std::is_integral<T>::value) {
    std::uniform_int_distribution<T> dist(first, second);
    return dist(m_twister_);
  } else if constexpr(std::is_floating_point<T>::value) {
    std::uniform_real_distribution<T> dist(first, second);
    return dist(m_twister_);
  }
}

#endif // ANTIPATTERNS_RANDOMDEVICE_H
