/**
 * @copyright 2018 TwoD
 * @author Brian Cairl
 */

// C++ Standard Library
#include <type_traits>

// GTest
#include <gtest/gtest.h>

// TwoD
#include <twod/stream.h>
#include <twod/tiled_grid.h>


using namespace twod;


TEST(FixedTiledGrid, UniformInitialValueConstructor)
{
  FixedTiledGrid<int, 20, 20, 10, 10> grid{5};

  for (const auto& v : grid)
  {
    ASSERT_EQ(v, 5);
  }
}


TEST(FixedTiledGrid, SingleTileAssign)
{
  FixedTiledGrid<int, 20, 20, 20, 20> grid{5};

  grid[Indices{5, 5}] = 6;

  EXPECT_EQ((grid[Indices{5, 5}]), 6);
}


TEST(FixedTiledGrid, Assign)
{
  FixedTiledGrid<int, 20, 20, 5, 5> grid{5};

  grid[Indices{5, 5}] = 6;
  grid[Indices{18, 19}] = 9;

  ASSERT_TRUE((grid.mask()[Indices{1, 1}]));
  ASSERT_TRUE((grid.mask()[Indices{3, 3}]));
  ASSERT_EQ(grid.active(), 2UL);

  EXPECT_EQ((grid[Indices{5, 5}]), 6);
  EXPECT_EQ((grid[Indices{18, 19}]), 9);
}


TEST(FixedTiledGrid, AssignGrid)
{
  FixedTiledGrid<int, 20, 20, 5, 5> grid{1};

  grid.view(FixedOriginExtentsBounds<1, 1, 2, 2>{}) = FixedGrid<int, 2, 2>{5};

  EXPECT_EQ((grid[Indices{0, 0}]), 1);
  EXPECT_EQ((grid[Indices{1, 1}]), 5);
  EXPECT_EQ((grid[Indices{1, 2}]), 5);
  EXPECT_EQ((grid[Indices{2, 1}]), 5);
  EXPECT_EQ((grid[Indices{2, 2}]), 5);
  EXPECT_EQ((grid[Indices{3, 3}]), 1);
}


TEST(FixedTiledGrid, AssignIterated)
{
  FixedTiledGrid<int, 2000, 2000, 50, 50> grid{1};

  for (auto& c : grid)
  {
    c = 2;
  }

  ASSERT_EQ(grid.active(), (FixedTiledGrid<int, 2000, 2000, 50, 50>::TileCount));
}


int main(int argc, char** argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
