//
// Created by dmitry on 5/16/20.
//

#include "RandomDevice.h"
RandomDevice &RandomDevice::Instance() {
  static RandomDevice only_instance;
  return only_instance;
}

RandomDevice::RandomDevice() : random_device_{}, m_twister_(random_device_()) {

}
