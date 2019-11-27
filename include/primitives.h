/**
 * @copyright 2019 TwoD
 * @author Brian Cairl
 *
 * @file primitives.h
 * @brief Grid primitive base definitions
 */
#ifndef TWOD_PRIMITIVES_H
#define TWOD_PRIMITIVES_H

// TwoD
#include <twod/coordinates.h>
#include <twod/fwd.h>

namespace twod
{

/**
 * @brief Helper object which implements compile-time determined extent attributes which satisfy GridBase interface
 *
 * @tparam Height  compile-time grid height
 * @tparam Width  compile-time grid width
 */
template<int Height, int Width>
class FixedExtentsBase
{
protected:
  /// @copydoc GridBase::extents
  constexpr static Extents extents_impl()
  {
    return extents_;
  }

  /// Grid extents
  constexpr static const Extents extents_{Height, Width};

  template<typename OtherDerived> friend class BoundsBase;
  template<typename OtherDerived, typename OtherCellT> friend class GridBase;
};


/**
 * @brief Helper object which implements runtime determined extent attributes which satisfy GridBase interface
 */
class ResizableExtentsBase
{
protected:
  ResizableExtentsBase(const ResizableExtentsBase&) = default;

  /// Extents constructor
  explicit ResizableExtentsBase(const Extents& extents) :
    extents_{extents}
  {}

  /// @copydoc GridBase::extents
  inline Extents extents_impl() const
  {
    return extents_;
  }

  /// Grid extents
  Extents extents_;

  template<typename OtherDerived> friend class BoundsBase;
  template<typename OtherDerived, typename OtherCellT> friend class GridBase;
};


/**
 * @brief Helper object which
 *
 * @tparam X  compile-time origin X-coordinate
 * @tparam Y  compile-time origin T-coordinate
 */
template<int X, int Y>
class FixedOriginBase
{
protected:
  /// @copydoc GridBase::origin
  constexpr static Indices origin_impl()
  {
    return origin_;
  }

  /// Origin location
  constexpr static const Indices origin_{X, Y};

  template<typename OtherDerived> friend class BoundsBase;
  template<typename OtherDerived, typename OtherCellT> friend class GridBase;
};


/**
 * @brief Helper object which
 */
class DynamicOriginBase
{
protected:
  DynamicOriginBase(const DynamicOriginBase&) = default;

  /// Indices constructor
  explicit DynamicOriginBase(const Indices& origin) :
    origin_{origin}
  {}

  /// @copydoc GridBase::origin
  inline Indices origin_impl() const
  {
    return origin_;
  }

  /// Origin location
  Indices origin_;

  template<typename OtherDerived> friend class BoundsBase;
  template<typename OtherDerived, typename OtherCellT> friend class GridBase;
};



template<typename Derived,
         typename ContainerT,
         typename SizeT = typename ContainerT::size_type,
         typename IteratorT = typename ContainerT::iterator,
         typename ConstIteratorT = typename ContainerT::const_iterator>
class ContainerAccessBase
{
public:
  template<typename... Args>
  explicit ContainerAccessBase(Args&&... args) :
    data_{std::forward<Args>(args)...}
  {}

  inline SizeT toLinearIndex(const Indices& pt) const
  {
    return (derived()->extents().x * pt.y) + pt.x;
  }

protected:
  ContainerT data_;

private:
  IMPLEMENT_CRTP_BASE_CLASS(ContainerAccessBase, Derived);

  inline const auto& access_impl(const Indices& pt) const
  {
    return data_[toLinearIndex(pt)];
  }

  inline auto& access_impl(const Indices& pt)
  {
    return data_[toLinearIndex(pt)];
  }

  inline IteratorT begin_impl()
  {
    return data_.begin();
  }

  inline IteratorT end_impl()
  {
    return data_.end();
  }

  inline ConstIteratorT begin_impl() const
  {
    return data_.begin();
  }

  inline ConstIteratorT end_impl() const
  {
    return data_.end();
  }

  template<typename OtherDerived, typename OtherBoundsT>
  friend class GridBase;
};


template<typename Derived, typename PtrT>
class RawAccessBase
{
  static_assert(
    std::is_pointer_v<PtrT>,
    "RawAccessBase: expecting PtrT to be a pointer type"
  );
public:
  explicit RawAccessBase(PtrT data) :
    data_{data}
  {}

  inline int toLinearIndex(const Indices& pt) const
  {
    return (derived()->extents().x * pt.y) + pt.x;
  }

  inline void swap(Derived& other)
  {
    const PtrT other_data{other.data_};
    const PtrT this_data{this->data_};

    derived()->swap_resize_impl(other);
    this->data_ = other_data;
    other.data_ = this_data;
  }

  inline PtrT data()
  {
    return data_;
  }

  inline const PtrT data() const
  {
    return data_;
  }

protected:
  PtrT data_;

private:
  IMPLEMENT_CRTP_BASE_CLASS(RawAccessBase, Derived);

  inline const auto& access_impl(const Indices& pt) const
  {
    return data_[toLinearIndex(pt)];
  }

  inline auto& access_impl(const Indices& pt)
  {
    return data_[toLinearIndex(pt)];
  }

  inline PtrT begin_impl()
  {
    return data_;
  }

  inline PtrT end_impl()
  {
    return data_ + derived()->extents().area();
  }

  inline const PtrT begin_impl() const
  {
    return data_;
  }

  inline const PtrT end_impl() const
  {
    return data_ + derived()->extents().area();
  }

  template<typename OtherDerived, typename OtherBoundsT>
  friend class GridBase;
};

}  // namespace twod

#endif // TWOD_PRIMITIVES_H