/**
 * @copyright 2018 TwoD
 * @author Brian Cairl
 */

// C++ Standard Library
#include <type_traits>
#include <vector>

// GTest
#include <gtest/gtest.h>

// TwoD
#include <twod/grid.h>
#include <twod/stream.h>


using namespace twod;


TEST(Bounds, Within)
{
  const Bounds bounds{Indices{1, 1}, Extents{5, 5}};

  ASSERT_FALSE(bounds.within(Indices{0, 0}));
  ASSERT_TRUE(bounds.within(Indices{1, 1}));
  ASSERT_TRUE(bounds.within(Indices{5, 5}));
  ASSERT_FALSE(bounds.within(Indices{6, 6}));
}


TEST(Bounds, Overlaps)
{
  const Bounds rbounds{Indices{1, 1}, Extents{5, 5}};
  const Bounds lbounds{Indices{2, 2}, Extents{3, 3}};

  ASSERT_TRUE(rbounds.overlaps(lbounds));
}


TEST(Bounds, OverlapsEdge)
{
  const Bounds rbounds{Indices{1, 1}, Extents{1, 1}};
  const Bounds lbounds{Indices{1, 2}, Extents{3, 3}};

  ASSERT_TRUE(rbounds.overlaps(lbounds));
}


TEST(Bounds, OverlapsCorner)
{
  const Bounds rbounds{Indices{1, 1}, Extents{1, 1}};
  const Bounds lbounds{Indices{2, 2}, Extents{1, 1}};

  ASSERT_TRUE(rbounds.overlaps(lbounds));
}


TEST(FixedExtentsBounds, Within)
{
  const FixedExtentsBounds<5, 5> bounds{Indices{1, 1}};

  ASSERT_FALSE(bounds.within(Indices{0, 0}));
  ASSERT_TRUE(bounds.within(Indices{1, 1}));
  ASSERT_TRUE(bounds.within(Indices{5, 5}));
  ASSERT_FALSE(bounds.within(Indices{6, 6}));
}


TEST(FixedOriginBounds, Within)
{
  const FixedOriginBounds<1, 1> bounds{Extents{5, 5}};

  ASSERT_FALSE(bounds.within(Indices{0, 0}));
  ASSERT_TRUE(bounds.within(Indices{1, 1}));
  ASSERT_TRUE(bounds.within(Indices{5, 5}));
  ASSERT_FALSE(bounds.within(Indices{6, 6}));
}


TEST(FixedOriginExtentsBounds, Within)
{
  const FixedOriginExtentsBounds<1, 1, 5, 5> bounds;

  ASSERT_FALSE(bounds.within(Indices{0, 0}));
  ASSERT_TRUE(bounds.within(Indices{1, 1}));
  ASSERT_TRUE(bounds.within(Indices{5, 5}));
  ASSERT_FALSE(bounds.within(Indices{6, 6}));
}


TEST(FixedExtentsBounds, BoundsWithinNoEdges)
{
  const FixedOriginExtentsBounds<1, 1, 5, 5> small_bounds;
  const FixedOriginExtentsBounds<0, 0, 20, 20> big_bounds;

  ASSERT_FALSE(small_bounds.within(big_bounds));
  ASSERT_TRUE(big_bounds.within(small_bounds));
}


TEST(FixedExtentsBounds, BoundsWithinOutsideTop)
{
  const FixedOriginExtentsBounds<-1, -1, 5, 5> small_bounds;
  const FixedOriginExtentsBounds<0, 0, 20, 20> big_bounds;

  ASSERT_FALSE(small_bounds.within(big_bounds));
  ASSERT_FALSE(big_bounds.within(small_bounds));
}


TEST(FixedExtentsBounds, BoundsWithinOutsideBottom)
{
  const FixedOriginExtentsBounds<16, 16, 5, 5> small_bounds;
  const FixedOriginExtentsBounds<0, 0, 20, 20> big_bounds;

  ASSERT_FALSE(small_bounds.within(big_bounds));
  ASSERT_FALSE(big_bounds.within(small_bounds));
}


TEST(FixedExtentsBounds, BoundsWithinInsideTopEdge)
{
  const FixedOriginExtentsBounds<0, 0, 5, 5> small_bounds;
  const FixedOriginExtentsBounds<0, 0, 20, 20> big_bounds;

  ASSERT_FALSE(small_bounds.within(big_bounds));
  ASSERT_TRUE(big_bounds.within(small_bounds));
}


TEST(FixedExtentsBounds, BoundsWithinInsideBottomEdge)
{
  const FixedOriginExtentsBounds<15, 15, 5, 5> small_bounds;
  const FixedOriginExtentsBounds<0, 0, 20, 20> big_bounds;

  ASSERT_FALSE(small_bounds.within(big_bounds));
  ASSERT_TRUE(big_bounds.within(small_bounds));
}


TEST(ColBoundsIterator, AllWithin)
{
  const FixedOriginExtentsBounds<1, 1, 5, 5> bounds;

  int count = 0;
  for (const auto& pt : make_col_bounds_range(bounds))
  {
    ASSERT_TRUE(bounds.within(pt));
    ++count;
  }
  ASSERT_EQ(count, bounds.extents().area());
}


TEST(RowBoundsIterator, AllWithin)
{
  const FixedOriginExtentsBounds<1, 1, 5, 5> bounds;

  int count = 0;
  for (const auto& pt : make_row_bounds_range(bounds))
  {
    ASSERT_TRUE(bounds.within(pt));
    ++count;
  }
  ASSERT_EQ(count, bounds.extents().area());
}


TEST(Intersection, OverlappingContaining)
{
  const FixedOriginExtentsBounds<1, 1, 5, 5> small_bounds;
  const FixedOriginExtentsBounds<0, 0, 20, 20> big_bounds;

  const auto result = intersection(small_bounds, big_bounds);

  ASSERT_EQ(result, small_bounds);
}


TEST(Intersection, OverlappingPoint)
{
  const FixedOriginExtentsBounds<0, 0, 5, 5> small_bounds;
  const FixedOriginExtentsBounds<5, 5, 20, 20> big_bounds;

  const auto result = intersection(small_bounds, big_bounds);
  constexpr FixedOriginExtentsBounds<5, 5, 0, 0> expected;

  ASSERT_EQ(result, expected);
}


TEST(Intersection, NonOverlapping)
{
  const FixedOriginExtentsBounds<0, 0, 5, 5> small_bounds;
  const FixedOriginExtentsBounds<6, 6, 20, 20> big_bounds;

  const auto result = intersection(small_bounds, big_bounds);
  constexpr FixedOriginExtentsBounds<6, 6, 0, 0> expected;

  ASSERT_EQ(result, expected);
}


int main(int argc, char **argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
