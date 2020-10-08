/**
 * @copyright 2018 TwoD
 * @author Brian Cairl
 *
 * @file coordinates.h
 */

#ifndef TWOD_COORDINATES_H
#define TWOD_COORDINATES_H

// C++ Standard Library
#include <cmath>
#include <tuple>

namespace twod
{

template <typename CoordT> struct Coordinates
{
  CoordT x;
  CoordT y;

  Coordinates() = default;

  constexpr Coordinates(const Coordinates&) = default;

  constexpr Coordinates(CoordT _x, CoordT _y) : x{_x}, y{_y} {}

  template <typename OtherCoordT>
  constexpr Coordinates(const Coordinates<OtherCoordT>& other) :
      Coordinates{static_cast<CoordT>(other.x), static_cast<CoordT>(other.y)}
  {}

  constexpr bool operator==(const Coordinates& other) const { return this->x == other.x and this->y == other.y; }

  constexpr bool operator!=(const Coordinates& other) const { return !this->operator==(other); }

  constexpr bool operator<(const Coordinates& other) const
  {
    return std::tie(this->x, this->y) < std::tie(other.x, other.y);
  }

  constexpr bool all_gt(const Coordinates& other) const { return this->x > other.x and this->y > other.y; }

  constexpr bool all_ge(const Coordinates& other) const { return this->x >= other.x and this->y >= other.y; }

  constexpr bool all_lt(const Coordinates& other) const { return this->x < other.x and this->y < other.y; }

  constexpr bool all_le(const Coordinates& other) const { return this->x <= other.x and this->y <= other.y; }

  constexpr Coordinates abs() const { return Coordinates{std::abs(this->x), std::abs(this->y)}; }

  constexpr Coordinates floor() const { return Coordinates{std::floor(this->x), std::floor(this->y)}; }

  constexpr int area() const { return x * y; }

  constexpr bool isZero() const { return *this == Coordinates::Zero(); }

  constexpr static Coordinates Zero() { return Coordinates{0, 0}; }

  constexpr Coordinates operator-() const { return Coordinates{-this->x, -this->y}; }

  inline Coordinates& operator-=(const Coordinates other)
  {
    this->x -= other.x;
    this->y -= other.y;
    return *this;
  }

  inline Coordinates& operator+=(const Coordinates other)
  {
    this->x += other.x;
    this->y += other.y;
    return *this;
  }

  friend constexpr Coordinates operator-(const Coordinates lhs, const Coordinates rhs)
  {
    return Coordinates{lhs.x - rhs.x, lhs.y - rhs.y};
  }

  friend constexpr Coordinates operator+(const Coordinates lhs, const Coordinates rhs)
  {
    return Coordinates{lhs.x + rhs.x, lhs.y + rhs.y};
  }

  template <typename ScalarT> friend constexpr Coordinates<ScalarT> operator*(const Coordinates lhs, ScalarT scale)
  {
    return Coordinates<ScalarT>{lhs.x * scale, lhs.y * scale};
  }

  template <typename ScalarT> friend constexpr Coordinates<ScalarT> operator/(const Coordinates lhs, ScalarT scale)
  {
    return Coordinates<ScalarT>{lhs.x / scale, lhs.y / scale};
  }
};

/// Grid access index pair
using Indices = Coordinates<int>;

/// Grid sizing
using Extents = Coordinates<int>;

}  // namespace twod

#endif  // TWOD_COORDINATES_H