/**
 * @copyright 2018 TwoD
 * @author Brian Cairl
 *
 * @file fwd.h
 * @brief Forward declarations
 */

#ifndef TWOD_FWD_H
#define TWOD_FWD_H

namespace twod
{

// Forward
template <typename CoordT> struct Coordinates;
template <typename GridT> struct Tile;
template <typename DerivedT> class GridBase;
template <typename DerivedT> class BoundsBase;
template <typename ViewT> class ColViewIterator;
template <typename ViewT> class RowViewIterator;
template <typename ParentT, typename BoundsT> class View;
class ViewIteratorEnd;

}  // namespace twod

#endif  // TWOD_FWD_H