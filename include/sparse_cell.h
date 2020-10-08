////////////////////////////////////////////////////////////
//
// TwoD - 2D grid interface/manipulation library
//
// Copyright (C) 2019 Brian Cairl (briancairl@gmail.com)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
////////////////////////////////////////////////////////////


#ifndef TWOD_SPARSE_VALUE_H
#define TWOD_SPARSE_VALUE_H

// TwoD
#include <twod/coordinates.h>

namespace twod
{
namespace detail
{

template <typename CellT> struct SparseCell
{
  /// Value at position
  CellT value;

  /// Location information
  Indices position;

  SparseCell(CellT _value, Indices _position) : value{_value}, position{_position} {}

  /// Value LT comparison overload
  constexpr bool operator<(const SparseCell& other) const { return this->value < other.value; }
};

}  // namespace detail


template <typename CellT> struct SparseCell : detail::SparseCell<CellT>
{
  SparseCell(CellT _value, Indices _position) : detail::SparseCell<CellT>{_value, _position} {}

  /// Cast to position overload
  operator const Indices() const { return this->position; }

  /// Cast to value overload
  operator const CellT&() const { return this->value; }
};


template <> struct SparseCell<Indices> : detail::SparseCell<Indices>
{
  SparseCell(Indices _value, Indices _position) : detail::SparseCell<Indices>{_value, _position} {}

  /// Cast to position overload
  operator const Indices() const { return this->position; }
};


template <typename GridT> using sparse_cell_t = SparseCell<cell_t<GridT>>;

}  // namespace twod

#endif  // TWOD_SPARSE_VALUE_H
