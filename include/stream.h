/**
 * @copyright 2018 TwoD
 * @author Brian Cairl
 *
 * @file stream.h
 */

#ifndef TWOD_STREAM_H
#define TWOD_STREAM_H

// C++ Standard Library
#include <ostream>
#include <iomanip>

// TwoD
#include <twod/fwd.h>

namespace twod
{


template<typename CoordT>
inline std::ostream& operator<<(std::ostream& os, const Coordinates<CoordT>& pt)
{
  return os << pt.x << ", " << pt.y;
}


template<typename BoundsT>
inline std::ostream& operator<<(std::ostream& os, const BoundsBase<BoundsT>& bounds)
{
  return os << '(' << bounds.origin() << ", " << bounds.origin() + bounds.extents() << ')';
}


template<typename Derived, typename CellT>
inline std::ostream& operator<<(std::ostream& os, const GridBase<Derived, CellT>& grid)
{
  using Base = const GridBase<Derived, CellT>;

  int new_line_count = 0;
  for (auto itr = RowViewIterator<Base>{grid}; itr != ViewIteratorEnd{}; ++itr)
  {
    os << std::setw(4) << *itr;
    if (++new_line_count == grid.extents().y)
    {
      os << '\n';
      new_line_count = 0;
    }
  }
  return os;
}

template<typename GridT>
inline std::ostream& operator<<(std::ostream& os, const Tile<GridT>& tile)
{
  if (tile.data)
  {
    os << "origin: " << tile.origin << '\n';
    os << "tile:\n" << *tile.data;
  }
  else
  {
    os << "tile: <not expanded>";
  }
  return os;
}

}  // namespace twod

#endif // TWOD_STREAM_H