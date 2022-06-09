# TwoD

## Description

Collection of generic grid and grid viewing containers. Includes dynamic, fixed size and memory-mapped grid containers. Compiles with C++17.

Grids and views are implemented with a common CRTP base. Functions working on grids can be written like so to accept any grid-like type:

```c++
template<typename DerivedT, typename BoundsT>
void doThing(twod::GridBase<DerivedT, BoundsT>& grid, ...)
{
  grid[twod::Indices{1, 1}] = twod::cell_t<DerivedT>{...};

  /// etc.
}
```

## Running Tests

In the `WORKSPACE` folder
```
bazel test ... --test_output=all --cache_test_results=no

```

## TODO

- [ ] `TiledGrid` is currently this strange "space saving" grid that "expands" when you access the tile that an element is on. This includes assignment over range iteration. I don't think this is really a practical thing, at least not in this form. I will eventually change it so that all the memory is allocated up-front, and the tiles are simply such that accesses to nearby cells provide better memory locality when accessing elements from the same tile. Maybe this is more useful for searching applications?
