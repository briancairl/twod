/**
 * @copyright 2019 TwoD
 * @author Brian Cairl
 *
 * @file bounds.h
 * @brief Grid bounds definitions
 */
#ifndef TWOD_BOUNDS_H
#define TWOD_BOUNDS_H

// TwoD
#include <twod/coordinates.h>
#include <twod/crtp.h>
#include <twod/fwd.h>
#include <twod/primitives.h>

namespace twod
{

/**
 * @brief
 */
template<typename Derived>
class BoundsBase
{
public:
  /**
   * @brief Equality comparison overload
   */
  constexpr bool operator==(const Derived& other) const
  {
    return this->origin() == other.origin() and
           this->extents() == other.extents();
  }

  /**
   * @brief Returns grid coordinate origin
   */
  constexpr bool operator!=(const Derived& other) const
  {
    return !this->operator==(other);
  }

  /**
   * @brief Returns grid coordinate origin
   */
  constexpr Indices origin() const
  {
    return derived()->origin_impl();
  }

  /**
   * @brief Returns grid coordinate extents
   */
  constexpr Extents extents() const
  {
    return derived()->extents_impl();
  }

  /**
   * @brief Returns center point of bounds
   */
  constexpr Indices center() const
  {
    return origin() + extents() / 2;
  }

  /**
   * @brief Check if grid (effectively) contains no values
   */
  constexpr bool empty() const
  {
    return this->extents() == Extents::Zero();
  }

  /**
   * @brief Check if point falls within grid extents
   * @param pt  index pair associated with grid element
   * @retval true  if point lies within grid
   * @retval false  otherwise
   */
  constexpr bool within(const Indices& pt) const
  {
    return pt.all_ge(this->origin()) and
           pt.all_lt(this->origin() + this->extents());
  }

  /**
   * @brief Check if bounds overlap
   * @param other  other bounds object
   * @retval true  if bounds overlap
   * @retval false  otherwise
   */
  template<typename OtherDerived>
  constexpr bool overlaps(const BoundsBase<OtherDerived>& other) const
  {
    return (this->origin() - other.origin()).abs().all_le(this->extents() +
                                                          other.extents());
  }

private:
  IMPLEMENT_CRTP_BASE_CLASS(Derived);
};


struct Bounds :
  public BoundsBase<Bounds>,
  public DynamicOriginBase,
  public ResizableExtentsBase
{
  template<typename Derived>
  Bounds(const BoundsBase<Derived>& bounds) :
    DynamicOriginBase{bounds.origin()},
    ResizableExtentsBase{bounds.extents()}
  {}

  Bounds(const Indices& origin, const Extents& extents) :
    DynamicOriginBase{origin},
    ResizableExtentsBase{extents}
  {}
};


template<int OriginX, int OriginY>
struct FixedOriginBounds :
  public BoundsBase<FixedOriginBounds<OriginX, OriginY>>,
  public FixedOriginBase<OriginX, OriginY>,
  public ResizableExtentsBase
{
  template<typename Derived>
  FixedOriginBounds(const BoundsBase<Derived>& bounds) :
    ResizableExtentsBase{bounds.extents()}
  {}

  FixedOriginBounds(const Extents& extents) :
    ResizableExtentsBase{extents}
  {}
};


template<int Height, int Width>
struct FixedExtentsBounds :
  public BoundsBase<FixedExtentsBounds<Height, Width>>,
  public DynamicOriginBase,
  public FixedExtentsBase<Height, Width>
{
  template<typename Derived>
  FixedExtentsBounds(const BoundsBase<Derived>& bounds) :
    DynamicOriginBase{bounds.origin()}
  {}

  FixedExtentsBounds(const Indices& origin) :
    DynamicOriginBase{origin}
  {}
};


template<int OriginX, int OriginY, int Height, int Width>
struct FixedOriginExtentsBounds :
  public BoundsBase<FixedOriginExtentsBounds<OriginX, OriginY, Height, Width>>,
  public FixedOriginBase<OriginX, OriginY>,
  public FixedExtentsBase<Height, Width>
{
};


/// End-tag object
struct BoundsIteratorEnd {};


/**
 * @brief Base bounds point iterator
 */
template<typename Derived>
class BoundsIteratorBase
{
public:
  /**
   * @brief Initialization constructor
   * @param bounds  parent bounds
   */
  template<typename BoundsT>
  explicit BoundsIteratorBase(const BoundsBase<BoundsT>& bounds) :
    pt_{bounds.origin()},
    origin_{pt_},
    past_corner_{bounds.origin() + bounds.extents()}
  {}

  /**
   * @brief End iterator constructor
   * @param bounds  parent bounds
   */
  template<typename BoundsT>
  explicit BoundsIteratorBase(const BoundsBase<BoundsT>& bounds,
                              BoundsIteratorEnd _end) :
    pt_{bounds.extents() - Indices{1, 1}},
    origin_{pt_},
    past_corner_{bounds.origin() + bounds.extents()}
  {}

  /**
   * @brief Pre-increment overload
   */
  inline Derived& operator++()
  {
    return derived()->increment_impl();
  }
  /**
   * @brief Post-increment overload
   */
  inline Derived operator++(int)
  {
    const Derived prev{*derived()};
    derived()->increment_impl();
    return prev;
  }

  /**
   * @brief Immutable cell value dereference operator
   */
  inline const Indices operator*() const
  {
    return this->pt_;
  }

  /**
   * @brief Equality operator
   */
  inline bool operator==(const Derived& other) const
  {
    return (this->pt_ == other.pt_);
  }

  /**
   * @brief Inequality operator
   */
  inline bool operator!=(const Derived& other) const
  {
    return !this->operator==(other);
  }

  /**
   * @brief Equality operator
   */
  inline bool operator==(BoundsIteratorEnd end) const
  {
    return derived()->eq_end_impl(end);
  }

  /**
   * @brief Inequality operator
   */
  inline bool operator!=(BoundsIteratorEnd end) const
  {
    return !this->operator==(end);
  }

  /**
   * @brief Returns indices past bounds corner point (origin + extents + (1, 1))
   */
  inline Indices past_corner() const
  {
    return past_corner_;
  }

protected:
  /// Bounds-relative index
  Indices pt_;

  /// Bounds origin point
  Indices origin_;

  /// Bounds reset point
  Indices past_corner_;

private:
  IMPLEMENT_CRTP_BASE_CLASS(Derived);
};


/**
 * @brief Column-major bounds iterator
 *
 * @tparam BoundsT  parent grid type
 */
template<typename BoundsT>
class ColBoundsIterator :
  public BoundsIteratorBase<ColBoundsIterator<BoundsT>>
{
  using Base = BoundsIteratorBase<ColBoundsIterator<BoundsT>>;
public:
  using Base::Base;

private:
  /**
   * @brief Pre-increment overload
   */
  inline ColBoundsIterator& increment_impl()
  {
    ++this->pt_.x;
    if (this->pt_.x == this->past_corner_.x)
    {
      this->pt_.x = this->origin_.x;
      ++this->pt_.y;
    }
    return *this;
  }

  /**
   * @brief End tag equality check
   */
  inline bool eq_end_impl(BoundsIteratorEnd) const
  {
    return this->pt_.y == this->past_corner_.y;
  }

  friend Base;
};


/**
 * @brief Row-major bounds iterator
 *
 * @tparam BoundsT  parent grid type
 */
template<typename BoundsT>
class RowBoundsIterator :
  public BoundsIteratorBase<RowBoundsIterator<BoundsT>>
{
  using Base = BoundsIteratorBase<RowBoundsIterator<BoundsT>>;
public:
  using Base::Base;

private:
  /**
   * @brief Pre-increment overload
   */
  inline RowBoundsIterator& increment_impl()
  {
    ++this->pt_.y;
    if (this->pt_.y == this->past_corner_.y)
    {
      this->pt_.y = this->origin_.y;
      ++this->pt_.x;
    }
    return *this;
  }

  /**
   * @brief End tag equality check
   */
  inline bool eq_end_impl(BoundsIteratorEnd) const
  {
    return this->pt_.x == this->past_corner_.x;
  }

  friend Base;
};


/**
 * @brief Range helper for iterating over bounds
 *
 * @tparam BoundsIteratorT  bounds iterator type
 */
template<typename BoundsIteratorT>
class BoundsIteratorRange
{
public:
  inline const BoundsIteratorT& begin() const
  {
    return begin_;
  }

  static inline BoundsIteratorEnd end()
  {
    return BoundsIteratorEnd{};
  }

  inline const BoundsIteratorT& cbegin() const
  {
    return begin_;
  }

  static inline BoundsIteratorEnd cend()
  {
    return BoundsIteratorEnd{};
  }

private:
  template<typename BoundsT>
  BoundsIteratorRange(const BoundsBase<BoundsT>& bounds) :
    begin_{bounds}
  {}

  mutable BoundsIteratorT begin_;

  template<typename BoundsT>
  friend inline BoundsIteratorRange<ColBoundsIterator<BoundsT>>
    make_col_bounds_range(const BoundsBase<BoundsT>&);

  template<typename BoundsT>
  friend inline BoundsIteratorRange<RowBoundsIterator<BoundsT>>
    make_row_bounds_range(const BoundsBase<BoundsT>&);
};

/**
 * @brief Helper for makeing column bounds iterator
 *
 * @tparam BoundsT  parent grid type
 */
template<typename BoundsT>
inline BoundsIteratorRange<ColBoundsIterator<BoundsT>>
  make_col_bounds_range(const BoundsBase<BoundsT>& bounds)
{
  return BoundsIteratorRange<ColBoundsIterator<BoundsT>>{bounds};
}

/**
 * @brief Helper for makeing row bounds iterator
 *
 * @tparam BoundsT  parent grid type
 */
template<typename BoundsT>
inline BoundsIteratorRange<RowBoundsIterator<BoundsT>>
  make_row_bounds_range(const BoundsBase<BoundsT>& bounds)
{
  return BoundsIteratorRange<RowBoundsIterator<BoundsT>>{bounds};
}

}  // namespace twod

#endif // TWOD_BOUNDS_H