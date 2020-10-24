/**
 * @copyright 2019 TwoD
 * @author Brian Cairl
 */

// C++ Standard Library
#include <memory>
#include <type_traits>

// GTest
#include <gtest/gtest.h>

// TwoD
#include <twod/grid.h>
#include <twod/stream.h>


using namespace twod;


TEST(MappedGrid, InitSizeConstructor)
{
  constexpr Extents extents{20, 10};

  std::unique_ptr<int[]> block{new int[extents.area()]};

  MappedGrid<int> grid{extents, block.get()};

  ASSERT_EQ(grid.extents(), extents);
  ASSERT_FALSE(grid.empty());
}


TEST(MappedGrid, CopyConstructor)
{
  constexpr Extents extents{20, 10};

  std::unique_ptr<int[]> block{new int[extents.area()]};

  std::fill(block.get(), block.get() + extents.area(), 1);

  const MappedGrid<int> initial_grid{extents, block.get()};

  ASSERT_EQ(initial_grid.data(), block.get());
  ASSERT_EQ(initial_grid.extents(), (extents));
  ASSERT_FALSE(initial_grid.empty());

  for (const auto& v : initial_grid)
  {
    ASSERT_EQ(v, 1);
  }

  MappedGrid<int> grid{initial_grid};

  ASSERT_EQ(grid.data(), block.get());
  ASSERT_EQ(grid.extents(), extents);
  ASSERT_FALSE(grid.empty());

  for (const auto& v : grid)
  {
    ASSERT_EQ(v, 1);
  }
}


TEST(MappedGrid, CopyAssignment)
{
  constexpr Extents extents{20, 10};

  std::unique_ptr<int[]> block{new int[extents.area()]};

  std::fill(block.get(), block.get() + extents.area(), 1);

  const MappedGrid<int> initial_grid{extents, block.get()};

  ASSERT_EQ(initial_grid.data(), block.get());
  ASSERT_EQ(initial_grid.extents(), extents);
  ASSERT_FALSE(initial_grid.empty());

  for (const auto& v : initial_grid)
  {
    ASSERT_EQ(v, 1);
  }

  MappedGrid<int> grid{extents, nullptr};

  grid = initial_grid;

  ASSERT_EQ(grid.data(), block.get());
  ASSERT_EQ(grid.extents(), extents);
  ASSERT_FALSE(grid.empty());

  for (const auto& v : grid)
  {
    ASSERT_EQ(v, 1);
  }
}


TEST(MappedGrid, Swap)
{
  constexpr Extents extents{20, 10};

  std::unique_ptr<int[]> block{new int[extents.area()]};

  std::fill(block.get(), block.get() + extents.area(), 1);

  MappedGrid<int> grid{extents, block.get()};
  ASSERT_EQ(grid.extents(), extents);

  const auto* prev_data_ptr = grid.data();

  MappedGrid<int> swap_to_grid{Extents::Zero(), nullptr};
  swap_to_grid.swap(grid);

  ASSERT_EQ(grid.data(), nullptr);
  ASSERT_EQ(grid.extents(), Extents::Zero());

  ASSERT_EQ(swap_to_grid.data(), prev_data_ptr);
  ASSERT_EQ(swap_to_grid.extents(), extents);
}


TEST(MappedGrid, Within)
{
  const MappedGrid<int> grid{Extents{20, 10}, nullptr};
  ASSERT_TRUE(grid.within(Indices{1, 1}));
}


TEST(MappedGrid, NotWithin)
{
  const MappedGrid<int> grid{Extents{20, 10}, nullptr};
  ASSERT_FALSE(grid.within(Indices{21, 11}));
}


TEST(MappedGrid, ColViewIterator)
{
  constexpr Extents extents{20, 10};

  std::unique_ptr<int[]> block{new int[extents.area()]};

  std::fill(block.get(), block.get() + extents.area(), 1);

  MappedGrid<int> grid{extents, block.get()};
  ASSERT_EQ(grid.extents(), extents);

  ColViewIterator<MappedGrid<int>> itr{grid};

  for (ColViewIterator<MappedGrid<int>> itr{grid}; itr != ColViewIterator<MappedGrid<int>>{grid, ViewIteratorEnd{}};
       ++itr)
  {
    *itr = 5;
  }

  for (ColViewIterator<const MappedGrid<int>> itr{grid};
       itr != ColViewIterator<const MappedGrid<int>>{grid, ViewIteratorEnd{}};
       ++itr)
  {
    ASSERT_EQ(*itr, 5);
  }
}


TEST(MappedGrid, ColViewIteratorEndTag)
{
  constexpr Extents extents{20, 10};

  std::unique_ptr<int[]> block{new int[extents.area()]};

  MappedGrid<int> grid{extents, block.get()};
  ASSERT_EQ(grid.extents(), extents);

  ColViewIterator<MappedGrid<int>> itr{grid};

  for (ColViewIterator<MappedGrid<int>> itr{grid}; itr != ViewIteratorEnd{}; ++itr)
  {
    *itr = 5;
  }

  for (ColViewIterator<const MappedGrid<int>> itr{grid}; itr != ViewIteratorEnd{}; ++itr)
  {
    ASSERT_EQ(*itr, 5);
  }
}


TEST(MappedGrid, RowViewIterator)
{
  constexpr Extents extents{20, 10};

  std::unique_ptr<int[]> block{new int[extents.area()]};

  MappedGrid<int> grid{extents, block.get()};
  ASSERT_EQ(grid.extents(), extents);

  RowViewIterator<MappedGrid<int>> itr{grid};

  for (RowViewIterator<MappedGrid<int>> itr{grid}; itr != RowViewIterator<MappedGrid<int>>{grid, ViewIteratorEnd{}};
       ++itr)
  {
    *itr = 5;
  }

  for (RowViewIterator<const MappedGrid<int>> itr{grid};
       itr != RowViewIterator<const MappedGrid<int>>{grid, ViewIteratorEnd{}};
       ++itr)
  {
    ASSERT_EQ(*itr, 5);
  }
}


TEST(MappedGrid, RowViewIteratorEndTag)
{
  constexpr Extents extents{20, 10};

  std::unique_ptr<int[]> block{new int[extents.area()]};

  MappedGrid<int> grid{extents, block.get()};
  ASSERT_EQ(grid.extents(), extents);

  RowViewIterator<MappedGrid<int>> itr{grid};

  for (RowViewIterator<MappedGrid<int>> itr{grid}; itr != ViewIteratorEnd{}; ++itr)
  {
    *itr = 5;
  }

  for (RowViewIterator<const MappedGrid<int>> itr{grid}; itr != ViewIteratorEnd{}; ++itr)
  {
    ASSERT_EQ(*itr, 5);
  }
}


TEST(MappedGrid, ViewBoundsIteration)
{
  constexpr Extents extents{20, 10};

  std::unique_ptr<int[]> block{new int[extents.area()]};

  MappedGrid<int> grid{extents, block.get()};
  ASSERT_EQ(grid.extents(), extents);

  auto view = grid.view(Bounds{Indices{2, 2}, Extents{5, 5}});

  view.fill(5);
  for (const auto& c : view)
  {
    ASSERT_EQ(c, 5);
  }
}


int main(int argc, char** argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
