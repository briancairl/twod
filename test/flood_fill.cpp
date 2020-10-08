// C++ Standard Library
#include <iostream>

// GTest
#include <gtest/gtest.h>

// TwoD
#include <twod/flood_fill.h>
#include <twod/grid.h>
#include <twod/stream.h>


using namespace twod;


TEST(FloodFill, DescendingFillFromCenter)
{
  Grid<int> grid{Extents{10, 10}};

  grid[Indices{5, 4}] = 10;
  grid[Indices{4, 5}] = 10;
  grid[Indices{4, 4}] = 10;
  grid[Indices{5, 5}] = 10;

  flood_fill(
    grid,
    [](const int v) { return v > 0; },
    [](const SparseCell<int>& sc) { return std::max(1, sc - 1); },
    [](const SparseCell<int>& sc) { return sc == 0; });

  ASSERT_EQ((grid[Indices{0, 0}]), 6) << grid;
  ASSERT_EQ((grid[Indices{0, 9}]), 6) << grid;
  ASSERT_EQ((grid[Indices{9, 0}]), 6) << grid;
  ASSERT_EQ((grid[Indices{9, 9}]), 6) << grid;
}


TEST(FloodFill, DISABLED_DescendingFillFromCenterView) /* TODO(bug) something is wrong with view */
{
  Grid<int> grid{Extents{10, 10}};

  grid[Indices{5, 4}] = 10;
  grid[Indices{4, 5}] = 10;
  grid[Indices{4, 4}] = 10;
  grid[Indices{5, 5}] = 10;

  flood_fill(
    grid.view(FixedOriginExtentsBounds<2, 2, 5, 5>{}),
    [](const int v) { return v > 0; },
    [](const SparseCell<int>& sc) { return std::max(1, sc - 1); },
    [](const SparseCell<int>& sc) { return sc == 0; });

  ASSERT_EQ((grid[Indices{2, 2}]), 10) << grid;
  ASSERT_EQ((grid[Indices{2, 6}]), 7) << grid;
  ASSERT_EQ((grid[Indices{6, 2}]), 7) << grid;
  ASSERT_EQ((grid[Indices{6, 6}]), 7) << grid;
}


TEST(FloodFill, DescendingByScalingFillFromCenter)
{
  Grid<float> grid{Extents{10, 10}};

  grid[Indices{5, 4}] = 10.f;
  grid[Indices{4, 5}] = 10.f;
  grid[Indices{4, 4}] = 10.f;
  grid[Indices{5, 5}] = 10.f;

  flood_fill(
    grid,
    [](const float v) { return v > 0; },
    [](const SparseCell<float>& sc) { return sc * 0.8f; },
    [](const SparseCell<float>& sc) { return sc == 0.f; });

  ASSERT_NEAR((grid[Indices{0, 0}]), 4.096f, 1e-5f) << grid;
  ASSERT_NEAR((grid[Indices{0, 9}]), 4.096f, 1e-5f) << grid;
  ASSERT_NEAR((grid[Indices{9, 0}]), 4.096f, 1e-5f) << grid;
  ASSERT_NEAR((grid[Indices{9, 9}]), 4.096f, 1e-5f) << grid;
}
