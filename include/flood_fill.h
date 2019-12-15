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


#ifndef TWOD_FLOOD_FILL_H
#define TWOD_FLOOD_FILL_H

// C++ Standard Library
#include <queue>
#include <memory>
#include <type_traits>
#include <utility>
#include <vector>

// TwoD
#include <twod/grid.h>
#include <twod/coordinates.h>
#include <twod/sparse_cell.h>

namespace twod
{

template<typename GridT /* = deduced */,
         typename SeedIteratorT /* = deduced */,
         typename ValueUpdaterFnT /* = deduced */,
         typename ExpansionValidatorFnT /* = deduced */,
         typename SparseCellCompareT = std::less<sparse_cell_t<GridT>>,
         typename SparseCellAllocatorT = std::allocator<sparse_cell_t<GridT>>>
void flood_fill(GridT&& grid,
                SeedIteratorT first,
                SeedIteratorT last,
                const ValueUpdaterFnT& value_updater,
                const ExpansionValidatorFnT& expansion_validator,
                const SparseCellCompareT& value_compare = SparseCellCompareT{})
{
  static_assert(is_grid<GridT>(), "'GridT' must be derived from twod::GridBase");

  // Create queue, max-sorted on seed value
  std::priority_queue<sparse_cell_t<GridT>,
                      std::vector<sparse_cell_t<GridT>, SparseCellAllocatorT>,
                      SparseCellCompareT> seeds{
    first,
    last,
    SparseCellCompareT{value_compare},
    std::vector<sparse_cell_t<GridT>, SparseCellAllocatorT>{}
  };

  while (!seeds.empty())
  {
    // Get next seed from queue
    const auto curr = seeds.top();
    seeds.pop();

    // Get updated value
    const auto next_value = value_updater(curr);

    // Enqueue seeds in a 3x3 region around current seed
    for (const auto& offset : make_col_bounds_range(FixedOriginExtentsBounds<-1, -1, 3, 3>{}))
    {
      const Indices next_position{curr + offset};

      // Validate enqueued seed value
      if (grid.within(next_position) and expansion_validator(sparse_cell_t<GridT>{grid[next_position], next_position}))
      {
        seeds.emplace(next_value, next_position);
        grid[next_position] = next_value;
      } 
    }
  }
}


template<typename GridT /* = deduced */,
         typename SeedGeneratorFnT /* = deduced */,
         typename ValueUpdaterFnT /* = deduced */,
         typename ExpansionValidatorFnT /* = deduced */,
         typename SparseCellCompareT = std::less<sparse_cell_t<GridT>>,
         typename SparseCellAllocatorT = std::allocator<sparse_cell_t<GridT>>>
void flood_fill(GridT&& grid,
                const SeedGeneratorFnT& seed_generator,
                const ValueUpdaterFnT& value_updater,
                const ExpansionValidatorFnT& expansion_validator,
                const SparseCellCompareT& value_compare = SparseCellCompareT{})
{
  static_assert(is_grid<GridT>(), "'GridT' must be derived from twod::GridBase");

  // Collect seeds from grid
  std::vector<sparse_cell_t<GridT>, SparseCellAllocatorT> seeds;
  for (ColViewIterator<std::remove_reference_t<GridT>> itr{static_cast<GridT&>(grid)}; itr != twod::ViewIteratorEnd{}; ++itr)
  {
    if (seed_generator(*itr))
    {
      seeds.emplace_back(*itr, itr.coords());
    }
  }

  // Run known-seed flood-fill
  return flood_fill(grid, seeds.begin(), seeds.end(), value_updater, expansion_validator, value_compare);
}

}  // namespace twod

#endif // TWOD_FLOOD_FILL_H
