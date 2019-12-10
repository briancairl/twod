/**
 * @copyright 2018-2019 TwoD
 * @author Brian Cairl
 *
 * @file grid.h
 * @brief Grid container and view implementations
 */
#ifndef TWOD_GRID_H
#define TWOD_GRID_H

// C++ Standard Library
#include <array>
#include <memory>
#include <type_traits>
#include <utility>

// CRTP
#include <crtp/crtp.h>

// TwoD
#include <twod/bounds.h>
#include <twod/coordinates.h>
#include <twod/fwd.h>
#include <twod/primitives.h>

namespace twod
{

/**
 * @brief Grid type traits helper type
 */
template<typename GridT>
struct GridTraits;


/// Convenience cell-type using template, based on GridTraits
template<typename GridT>
using cell_t = typename GridTraits<GridT>::cell_type;


/**
 * @brief CRTP grid base interface
 *
 * @tparam Derived  CRTP derived grid object
 */
template<typename Derived, typename BoundsT>
class GridBase : public BoundsT
{
public:
  /**
   * @brief Returns grid view of size \p extents
   *
   *        View is anchored at bottom left corning to parent coordinates \p origin
   *
   * @param bounds
   * @return view
   */
  template<typename ViewBoundsT>
  inline View<Derived, ViewBoundsT> view(const ViewBoundsT& bounds)
  {
    return View<Derived, ViewBoundsT>{*derived(), bounds};
  }

  /**
   * @brief Returns grid view of size \p extents
   *
   *        View is anchored at bottom left corning to parent coordinates \p origin
   *
   * @param bounds
   * @return view
   */
  template<typename ViewBoundsT>
  inline View<const Derived, ViewBoundsT> view(const ViewBoundsT& bounds) const
  {
    return View<const Derived, ViewBoundsT>{*derived(), bounds};
  }

  /**
   * @brief Returns view for the entirety of the current grid
   * @return view
   */
  inline auto view()
  {
    return view(bounds());
  }

  /**
   * @brief Returns view for the entirety of the current grid
   * @return view
   */
  inline auto view() const
  {
    return view(bounds());
  }

  /**
   * @brief Generic assignment from <code>GridBase</code> derivative
   *
   * @tparam OtherDerived  CRTP-derived <code>GridBase</code> object
   *
   * @param other  other grid
   * @return <code>*this</code>
   */
  template<typename OtherDerived, typename OtherBoundsT>
  inline Derived& operator=(const GridBase<OtherDerived, OtherBoundsT>& other)
  {
    std::copy(other.begin(), other.end(), this->begin());
    return *derived();
  }

  /**
   * @brief Compound cell-wise addition from <code>GridBase</code> derivative
   *
   * @tparam OtherDerived  CRTP-derived <code>GridBase</code> object
   *
   * @param other  other grid
   * @return <code>*this</code>
   */
  template<typename OtherDerived>
  inline Derived& operator+=(const GridBase<OtherDerived, BoundsT>& other)
  {
    auto this_itr = derived()->begin();
    for (auto other_itr = other.begin(); other_itr != other.end(); ++other_itr, ++this_itr)
    {
      *this_itr += *other_itr;
    }
    return *derived();
  }

  /**
   * @brief Compound cell-wise subtraction from <code>GridBase</code> derivative
   *
   * @tparam OtherDerived  CRTP-derived <code>GridBase</code> object
   *
   * @param other  other grid
   * @return <code>*this</code>
   */
  template<typename OtherDerived>
  inline Derived& operator-=(const GridBase<OtherDerived, BoundsT>& other)
  {
    auto this_itr = derived()->begin();
    for (auto other_itr = other.begin(); other_itr != other.end(); ++other_itr, ++this_itr)
    {
      *this_itr -= *other_itr;
    }
    return *derived();
  }

  /**
   * @brief Compound cell scaling
   *
   * @tparam ScalarT  scalar value type
   *
   * @param scale  scaling value
   * @return <code>*this</code>
   */
  template<typename ScalarT>
  inline Derived& operator*=(const ScalarT scale)
  {
    for (auto& c : *derived())
    {
      c *= scale;
    }
    return *derived();
  }

  /**
   * @brief Compound cell division
   *
   * @tparam ScalarT  scalar value type
   *
   * @param scale  scaling value
   * @return <code>*this</code>
   */
  template<typename ScalarT>
  inline Derived& operator/=(const ScalarT scale)
  {
    for (auto& c : *derived())
    {
      c /= scale;
    }
    return *derived();
  }

  /**
   * @brief Cell wise inequality comparison <code>GridBase</code> derivative
   *
   * @tparam OtherDerived  CRTP-derived <code>GridBase</code> object
   *
   * @param other  other grid
   *
   * @retval true  if any cell of \p other is not equal to corresponding cell in <code>*this</code>
   * @retval false  otherwise
   */
  template<typename OtherDerived>
  inline bool operator!=(const GridBase<OtherDerived, BoundsT>& other) const
  {
    // Compare grid extents, first
    if (this->extents() != other.extents())
    {
      return true;
    }

    // Compare values
    return !std::equal(other.begin(), other.end(), derived()->begin());
  }

  /**
   * @brief Cell wise equality comparison <code>GridBase</code> derivative
   *
   * @tparam OtherDerived  CRTP-derived <code>GridBase</code> object
   *
   * @param other  other grid
   *
   * @retval true  if all cells of \p other are equal to corresponding cells in <code>*this</code>
   * @retval false  otherwise
   */
  template<typename OtherDerived>
  inline bool operator==(const GridBase<OtherDerived, BoundsT>& other) const
  {
    return not this->operator!=(other);
  }

  /**
   * @brief Sets all cells to a uniform value
   * @param value  cell value
   * @return <code>*this</code>
   */
  template<typename AssignT>
  inline Derived& fill(const AssignT& value)
  {
    for (auto& c : *derived())
    {
      c = value;
    }
    return *derived();
  }

  /**
   * @brief Returns grid bounds
   */
  constexpr const BoundsT& bounds() const
  {
    return static_cast<const BoundsT&>(*this);
  }

  /**
   * @brief Return mutable reference to element
   * @param pt  index pair associated with grid element
   * @return reference to cell
   */
  constexpr auto& operator[](const Indices& pt)
  {
    return derived()->access_impl(pt + this->origin());
  }

  /**
   * @brief Return immutable reference to element
   * @param pt  index pair associated with grid element
   * @return reference to cell
   */
  constexpr const auto& operator[](const Indices& pt) const
  {
    return derived()->access_impl(pt + this->origin());
  }

  /**
   * @brief Returns iterator (mutable) to first element
   */
  inline auto begin()
  {
    return derived()->begin_impl();
  }

  /**
   * @brief Returns iterator (mutable) to one past last element
   */
  inline auto end()
  {
    return derived()->end_impl();
  }

  /**
   * @brief Returns iterator (immutable) to first element
   */
  inline auto begin() const
  {
    return derived()->begin_impl();
  }

  /**
   * @brief Returns iterator (immutable) to one past last element
   */
  inline auto end() const
  {
    return derived()->end_impl();
  }

  /**
   * @brief Returns iterator (immutable) to first element
   */
  inline auto cbegin() const
  {
    return derived()->begin_impl();
  }

  /**
   * @brief Returns iterator (immutable) to one past last element
   */
  inline auto cend() const
  {
    return derived()->end_impl();
  }

protected:
  template<typename... Args>
  explicit GridBase(Args&&... args) :
    BoundsT{std::forward<Args>(args)...}
  {}

  inline void set_extents(const Extents& extents)
  {
    BoundsT::set_extents(extents);
  }

private:
  IMPLEMENT_CRTP_BASE_CLASS(GridBase, Derived);
};


template<typename ViewIteratorT>
struct ViewIteratorTraits;


template<typename ViewIteratorT>
using view_t = typename ViewIteratorTraits<ViewIteratorT>::ViewType;


/// End-tag object
struct ViewIteratorEnd {};


/**
 * @brief View iterator
 */
template<typename DerivedT>
class ViewIteratorBase
{
public:
  /**
   * @brief Pre-increment overload
   */
  inline DerivedT& operator++()
  {
    return derived()->increment_impl();
  }
  /**
   * @brief Post-increment overload
   */
  inline DerivedT operator++(int)
  {
    const DerivedT prev{*derived()};
    derived()->increment_impl();
    return prev;
  }

  /**
   * @brief Mutable cell value dereference operator
   */
  inline auto& operator*()
  {
    return view_->operator[](this->pt_);
  }

  /**
   * @brief Immutable cell value dereference operator
   */
  inline const auto& operator*() const
  {
    return view_->operator[](this->pt_);
  }

  /**
   * @brief Mutable cell value dereference operator
   */
  inline auto* operator->()
  {
    return std::addressof(this->operator*());
  }

  /**
   * @brief Immutable cell value dereference operator
   */
  inline const auto* operator->() const
  {
    return std::addressof(this->operator*());
  }

  /**
   * @brief Equality operator
   */
  inline bool operator==(const DerivedT& other) const
  {
    return (this->view_ == other.view_) and
           (this->pt_ == other.pt_);
  }

  /**
   * @brief Inequality operator
   */
  inline bool operator!=(const DerivedT& other) const
  {
    return !this->operator==(other);
  }

  /**
   * @brief Equality operator
   */
  inline bool operator==(ViewIteratorEnd end) const
  {
    return derived()->eq_end_impl(end);
  }

  /**
   * @brief Inequality operator
   */
  inline bool operator!=(ViewIteratorEnd end) const
  {
    return !this->operator==(end);
  }

  /**
   * @brief Returns active view-relative index pair associated with iterator
   */
  const Indices& coords() const
  {
    return pt_;
  }

  /**
   * @brief Returns pointer to parent view
   */
  const view_t<DerivedT>* const view() const
  {
    return view_;
  }

protected:
  /**
   * @brief Initialization constructor
   * @param view  parent grid view
   * @param pt  initial view-relative point
   */
  ViewIteratorBase(view_t<DerivedT>* const view, const Indices& pt) :
    view_{view},
    pt_{pt}
  {}

  /// Parent grid pointer
  view_t<DerivedT>* view_;

  /// View-relative index
  Indices pt_;

  template<typename OtherViewT> friend class ColViewIterator;
  template<typename OtherViewT> friend class RowViewIterator;

private:
  IMPLEMENT_CRTP_BASE_CLASS(ViewIteratorBase, DerivedT);
};


template<typename ViewT>
struct ViewIteratorTraits<ColViewIterator<ViewT>>
{
  using ViewType = ViewT;
};


/**
 * @brief Column-major view iterator
 *
 * @tparam GridT  parent grid type
 */
template<typename ViewT>
class ColViewIterator :
  public ViewIteratorBase<ColViewIterator<ViewT>>
{
  using ViewBase = ViewIteratorBase<ColViewIterator<ViewT>>;
public:
  /**
   * @brief Initialization constructor
   * @param view  parent grid view
   * @param pt  initial view-relative point
   */
  explicit ColViewIterator(ViewT& view, const Indices& pt = Indices::Zero()) :
    ViewBase{std::addressof(view), pt}
  {}

  /**
   * @brief End iterator constructor
   * @param view  parent grid view
   * @param pt  initial view-relative point
   */
  explicit ColViewIterator(ViewT& view, ViewIteratorEnd) :
    ViewBase{std::addressof(view), Indices{0, view.extents().y}}
  {}

private:
  /**
   * @brief Pre-increment overload
   */
  inline ColViewIterator& increment_impl()
  {
    ++this->pt_.x;
    if (this->pt_.x == this->view_->extents().x)
    {
      this->pt_.x = 0;
      ++this->pt_.y;
    }
    return *this;
  }

  /**
   * @brief End tag equality check
   */
  inline bool eq_end_impl(ViewIteratorEnd) const
  {
    return this->pt_.y == this->view_->extents().y;
  }

  friend ViewBase;
};


template<typename ViewT>
struct ViewIteratorTraits<RowViewIterator<ViewT>>
{
  using ViewType = ViewT;
};


/**
 * @brief Row-major view iterator
 *
 * @tparam GridT  parent grid type
 */
template<typename ViewT>
class RowViewIterator :
  public ViewIteratorBase<RowViewIterator<ViewT>>
{
  using ViewBase = ViewIteratorBase<RowViewIterator<ViewT>>;
public:
  /**
   * @brief Initialization constructor
   * @param view  parent grid view
   * @param pt  initial view-relative point
   */
  explicit RowViewIterator(ViewT& view, const Indices& pt = Indices::Zero()) :
    ViewBase{std::addressof(view), pt}
  {}

  /**
   * @brief End iterator constructor
   * @param view  parent grid view
   * @param pt  initial view-relative point
   */
  explicit RowViewIterator(ViewT& view, ViewIteratorEnd) :
    ViewBase{std::addressof(view), Indices{view.extents().x, 0}}
  {}

private:
  /**
   * @brief Pre-increment overload
   */
  inline RowViewIterator& increment_impl()
  {
    ++this->pt_.y;
    if (this->pt_.y == this->view_->extents().y)
    {
      this->pt_.y = 0;
      ++this->pt_.x;
    }
    return *this;
  }

  /**
   * @brief End tag equality check
   */
  inline bool eq_end_impl(ViewIteratorEnd) const
  {
    return this->pt_.x == this->view_->extents().x;
  }

  friend ViewBase;
};


template<typename ParentT, typename BoundsT>
class View : public GridBase<View<ParentT, BoundsT>, BoundsT>
{
  using GBase = GridBase<View<ParentT, BoundsT>, BoundsT>;
public:
  using GBase::operator=;

  View(ParentT& parent) :
    parent_{std::addressof(parent)}
  {}

  View(ParentT& parent, BoundsT bounds) :
    GBase{bounds},
    parent_{std::addressof(parent)}
  {}

private:
  /**
   * @brief Return mutable reference to element
   * @param pt  index pair associated with grid element
   * @return reference to cell
   */
  constexpr auto& access_impl(const Indices& pt)
  {
    return parent_->operator[](pt);
  }

  /**
   * @brief Return immutable reference to element
   * @param pt  index pair associated with grid element
   * @return reference to cell
   */
  constexpr const auto& access_impl(const Indices& pt) const
  {
    return parent_->operator[](pt);
  }

  /**
   * @brief Returns iterator (mutable) to first element
   */
  inline auto begin_impl()
  {
    return ColViewIterator<View>{*this};
  }

  /**
   * @brief Returns iterator (immutable) to first element
   */
  inline auto begin_impl() const
  {
    return ColViewIterator<const View>{*this};
  }

  /**
   * @brief Returns iterator (immutable) to one past last element
   */
  inline auto end_impl() const
  {
    return ViewIteratorEnd{};
  }

  /// Pointer to parent grid being viewed
  ParentT* parent_;

  friend GBase;
};


template<typename ParentT, typename BoundsT>
struct GridTraits<View<ParentT, BoundsT>> : GridTraits<ParentT> {};


template<typename CellT,
         typename AllocatorT = std::allocator<CellT>>
class Grid :
  public GridBase<Grid<CellT, AllocatorT>, FixedOriginBounds<0, 0>>,
  public RawAccessBase<Grid<CellT, AllocatorT>, CellT*>
{
  using GBase = GridBase<Grid, FixedOriginBounds<0, 0>>;
  using StorageBase = RawAccessBase<Grid, CellT*>;
public:
  Grid() :
    GBase{Extents::Zero()},
    StorageBase{nullptr}
  {}

  explicit Grid(const Extents& extents) :
    GBase{extents},
    StorageBase{allocator_.allocate(extents.area())}
  {
    construct();
  }

  Grid(const Extents& extents, const CellT& val) :
    GBase{extents},
    StorageBase{allocator_.allocate(extents.area())}
  {
    construct(val);
  }

  Grid(const Extents& extents, const AllocatorT& alloc) :
    GBase{extents},
    StorageBase{alloc.allocate(extents.area())},
    allocator_{alloc}
  {
    construct();
  }

  Grid(const Extents& extents, const CellT& val, const AllocatorT& alloc) :
    GBase{extents},
    StorageBase{alloc.allocate(extents.area())},
    allocator_{alloc}
  {
    construct(val);
  }

  Grid(const Grid& other) :
    Grid{}
  {
    Grid::operator=(other);
  }

  Grid(Grid&& other) :
    Grid{}
  {
    Grid::operator=(std::move(other));
  }

  ~Grid()
  {
    clear();
  }

  inline void clear()
  {
    // Don't do anything if data isn't set
    if (Grid::data_ == nullptr)
    {
      return;
    }

    // Deallocate + destory
    for (auto d = Grid::begin(); d != Grid::end(); ++d)
    {
      allocator_.destroy(d);
    }
    allocator_.deallocate(Grid::data_, Grid::extents().area());

    // Reset data pointer and extents
    Grid::data_ = nullptr;
    GBase::set_extents(Extents::Zero());
  }

  inline void resize(const Extents& extents, const CellT& value)
  {
    if (extents.isZero())
    {
      Grid::clear();
    }
    else if (Grid::extents() == extents)
    {
      std::fill(Grid::begin(), Grid::end(), value);
    }
    else
    {
      Grid::clear();
      GBase::set_extents(extents);
      Grid::data_ = allocator_.allocate(extents.area());
      Grid::construct(value);
    }
  }

  inline void resize(const Extents& extents)
  {
    if (extents.isZero())
    {
      Grid::clear();
    }
    else if (Grid::extents() == extents)
    {
      return;
    }
    else
    {
      Grid::clear();
      GBase::set_extents(extents);
      Grid::data_ = allocator_.allocate(extents.area());
      Grid::construct();
    }
  }

  inline Grid& operator=(Grid&& other)
  {
    GBase::set_extents(other.extents());
    Grid::data_ = other.data_;
    other.data_ = nullptr;
    other.set_extents(Extents::Zero());
    return *this;
  }

  inline Grid& operator=(const Grid& other)
  {
    if (other.data_ == nullptr or other.extents().isZero())
    {
      Grid::clear();
    }
    else
    {
      Grid::resize(other.extents());
      std::copy(other.begin(), other.end(), Grid::begin());
    }
    return *this;
  }

private:
  friend StorageBase;

  template<typename... CellArgs>
  inline void construct(CellArgs&&... cell_args)
  {
    for (auto d = Grid::begin(); d != Grid::end(); ++d)
    {
      allocator_.construct(d, std::forward<CellArgs>(cell_args)...);
    }
  }

  inline void swap_resize_impl(Grid& other)
  {
    const auto tmp_extents = other.extents();
    other.set_extents(Grid::extents());
    Grid::set_extents(tmp_extents);
  }

  /// Allocator
  AllocatorT allocator_;
};


template<typename CellT, typename AllocatorT>
struct GridTraits<Grid<CellT, AllocatorT>>
{
  using cell_type = CellT;
};


template<typename CellT>
class MappedGrid :
  public GridBase<MappedGrid<CellT>, FixedOriginBounds<0, 0>>,
  public RawAccessBase<MappedGrid<CellT>, CellT*>
{
  using GBase = GridBase<MappedGrid, FixedOriginBounds<0, 0>>;
  using StorageBase = RawAccessBase<MappedGrid, CellT*>;
public:
  MappedGrid(MappedGrid&&) = delete;

  MappedGrid(const MappedGrid& other) :
    GBase{other.extents()},
    StorageBase{other.data_}
  {}

  MappedGrid(const Extents& extents, CellT* mem) :
    GBase{extents},
    StorageBase{mem}
  {}

  inline MappedGrid& operator=(const MappedGrid& other)
  {
    GBase::set_extents(other.extents());
    this->data_ = other.data_;
    return *this;
  }

private:
  inline void swap_resize_impl(MappedGrid& other)
  {
    const auto tmp_extents = other.extents();
    other.set_extents(MappedGrid::extents());
    MappedGrid::set_extents(tmp_extents);
  }

  friend GBase;
  friend StorageBase;
};


template<typename CellT>
struct GridTraits<MappedGrid<CellT>>
{
  using cell_type = CellT;
};


template<typename CellT, int Height, int Width>
class FixedGrid :
  public GridBase<FixedGrid<CellT, Height, Width>, FixedOriginExtentsBounds<0, 0, Height, Width>>,
  public ContainerAccessBase<FixedGrid<CellT, Height, Width>, std::array<CellT, Height * Width>>
{
  using GBase = GridBase<FixedGrid, FixedOriginExtentsBounds<0, 0, Height, Width>>;
  using StorageBase = ContainerAccessBase<FixedGrid, std::array<CellT, Height * Width>>;
public:
  using GBase::operator=;

  FixedGrid() = default;

  constexpr explicit FixedGrid(const CellT& initial_value)
  {
    this->data_.fill(initial_value);
  }
};


template<typename CellT, int Height, int Width>
struct GridTraits<FixedGrid<CellT, Height, Width>>
{
  using cell_type = CellT;
};


template<typename CellT, int Height, int Width>
class FixedMappedGrid :
  public GridBase<FixedMappedGrid<CellT, Height, Width>, FixedOriginExtentsBounds<0, 0, Height, Width>>,
  public RawAccessBase<FixedMappedGrid<CellT, Height, Width>, CellT*>
{
  using GBase = GridBase<FixedMappedGrid, FixedOriginExtentsBounds<0, 0, Height, Width>>;
  using StorageBase = RawAccessBase<FixedMappedGrid, CellT*>;
public:
  using GBase::operator=;

  explicit FixedMappedGrid(CellT* mem) :
    StorageBase{mem}
  {}

  friend GBase;
};


template<typename CellT, int Height, int Width>
struct GridTraits<FixedMappedGrid<CellT, Height, Width>>
{
  using cell_type = CellT;
};

}  // namespace twod


// C++ Standard Library
#include <iterator>

namespace std
{

template<typename ViewT>
struct iterator_traits<::twod::RowViewIterator<ViewT>>
{
  using iterator_category = std::bidirectional_iterator_tag;
  using difference_type = int;
  using value_type = ::twod::cell_t<ViewT>;
  using reference_type = value_type&;
  using pointer_type = value_type*;
};


template<typename ViewT>
struct iterator_traits<::twod::ColViewIterator<ViewT>>
{
  using iterator_category = std::bidirectional_iterator_tag;
  using difference_type = int;
  using value_type = ::twod::cell_t<ViewT>;
  using reference_type = value_type&;
  using pointer_type = value_type*;
};

}  // namespace std

#endif // TWOD_GRID_H