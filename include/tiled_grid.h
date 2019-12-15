/**
 * @copyright 2018 TwoD
 * @author Brian Cairl
 *
 * @file tiled_grid.h
 * @brief Tiled grid implementations
 */
#ifndef TWOD_TILED_GRID_H
#define TWOD_TILED_GRID_H

// C++ Standard Library
#include <iterator>
#include <memory>
#include <type_traits>
#include <utility>

// TwoD
#include <twod/coordinates.h>
#include <twod/grid.h>

namespace twod
{

template<typename GridT>
struct Tile
{
  std::unique_ptr<GridT> data;
  Indices origin = Indices::Zero();
};


template<typename CellT, int Height, int Width, int TileHeight = Height/2, int TileWidth=Width/2>
class FixedTiledGrid :
  public GridBase<FixedTiledGrid<CellT, Height, Width, TileHeight, TileWidth>>
{
  static_assert(Height >= TileHeight, "FixedTiledGrid: invalid TileHeight");
  static_assert(Width >= TileWidth, "FixedTiledGrid: invalid TileWidth");

  using GridBaseType = GridBase<FixedTiledGrid>;
public:
  constexpr static const int TileRows = Height / TileHeight;
  constexpr static const int TileCols = Width / TileWidth;
  constexpr static const std::size_t TileCount = TileRows * TileCols;

  using TileGrid = FixedGrid<CellT, TileHeight, TileWidth>;
  using TileType = Tile<TileGrid>;

  constexpr FixedTiledGrid(const CellT& default_value) :
    default_value_{default_value},
    view_{*this}
  {}

  inline FixedGrid<bool, TileRows, TileCols> mask() const
  {
    FixedGrid<bool, TileRows, TileCols> mask{false};

    auto mask_itr = mask.begin();
    for (auto tile_itr = tiles_.begin(); tile_itr != tiles_.end(); ++tile_itr, ++mask_itr)
    {
      *mask_itr = static_cast<bool>(tile_itr->data);
    }
    return mask;
  }

  inline std::size_t active() const
  {
    return std::count_if(tiles_.begin(),
                         tiles_.end(),
                         [](const TileType& t) -> bool
                         {
                           return static_cast<bool>(t.data);
                         });
  }

  inline const TileType& tile(Indices index) const
  {
    return tiles_[index];
  }

  constexpr static int rows()
  {
    return TileRows;
  }

  constexpr static int cols()
  {
    return TileCols;
  }

private:
  inline const CellT& access_impl(const Indices& pt) const
  {
    const Indices tile_pt{pt.x / TileHeight, pt.y / TileWidth};

    const TileType& tile = tiles_[tile_pt];
    if (tile.data)
    {
      return tile.data->operator[](pt - tile.origin);
    }
    return default_value_;
  }

  inline CellT& access_impl(const Indices& pt)
  {
    const Indices tile_pt{pt.x / TileHeight, pt.y / TileWidth};

    TileType& tile = tiles_[tile_pt];
    if (!tile.data)
    {
      tile.origin.x = tile_pt.x * TileHeight;
      tile.origin.y = tile_pt.y * TileWidth;
      tile.data = std::make_unique<TileGrid>(default_value_);
    }
    return tile.data->operator[](pt - tile.origin);
  }

  inline auto begin_impl()
  {
    return view_.begin();
  }

  inline auto end_impl()
  {
    return view_.end();
  }

  inline const auto begin_impl() const
  {
    return view_.cbegin();
  }

  inline const auto end_impl() const
  {
    return view_.cend();
  }

  /// Cell value to return when tile is compressed
  CellT default_value_;

  /// Grid tiles
  FixedGrid<TileType, TileRows, TileCols> tiles_;

  /// View used to iterate over FixedTiledGrid
  View<FixedTiledGrid, FixedOriginExtentsBounds<0, 0, Height, Width>> view_;

  friend GridBaseType;
};


template<typename CellT, int Height, int Width, int TileHeight, int TileWidth>
struct GridTraits<FixedTiledGrid<CellT, Height, Width, TileHeight, TileWidth>>
{
  using cell_type = CellT;
  using bounds_type = FixedOriginExtentsBounds<0, 0, Height, Width>;
};

}  // namespace twod

#endif // TWOD_TILED_GRID_H