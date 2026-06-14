#include "transform.h"

#include <iostream>
#include <ostream>

std::ostream &operator<<(std::ostream &os, const Vec3 &v) {
  os << "x: " << v.x << ", y: " << v.y << ", z: " << v.z;
  return os;
}