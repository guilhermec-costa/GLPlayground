#pragma once

#include <array>
#include <cmath>
#include <ostream>

struct Vec3 {
  float x, y, z;

  // scalar operations
  Vec3 operator+(float scalar) {
    return Vec3{x + scalar, y + scalar, z + scalar};
  }

  Vec3 operator-(float scalar) {
    return Vec3{x - scalar, y - scalar, z - scalar};
  }

  Vec3 operator*(float scalar) {
    return Vec3{x * scalar, y * scalar, z * scalar};
  }

  Vec3 operator/(float scalar) {
    return Vec3{x / scalar, y / scalar, z / scalar};
  }

  // vector operations
  Vec3 operator+(const Vec3 &v) const {
    return Vec3{x + v.x, y + v.y, z + v.z};
  }
  Vec3 operator-(const Vec3 &v) const {
    return Vec3{x - v.x, y - v.y, z - v.z};
  }
  Vec3 operator*(const Vec3 &v) const {
    return Vec3{x * v.x, y * v.y, z * v.z};
  }
  Vec3 operator/(const Vec3 &v) const {
    return Vec3{x / v.x, y / v.y, z / v.z};
  }

  // defines how much aligned two vectors are
  float dotProduct(const Vec3 &v) const {
    return (x * v.x) + (y * v.y) + (z * v.z);
  }

  Vec3 normalize() {
    float len = length();
    if (len == 0.0f) {
      return Vec3{0.0, 0.0, 0.0};
    }
    return *this / len;
  }

  float length() const { return std::sqrt(x * x + y * y + z * z); }
};

std::ostream &operator<<(std::ostream &os, const Vec3 &v);

struct Mat4 {
  std::array<float, 16> elements;

  Mat4() = default;

  Mat4(const std::array<float, 16> &elems) : elements(elems) {}
};

std::ostream &operator<<(std::ostream &os, const Mat4 &m);