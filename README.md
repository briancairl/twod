# TwoD

## Description

Collection of generic grid and grid viewing containers. Includes dynamic, fixed size and memeory-mapped grid containers. Compiles with C++17.

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
