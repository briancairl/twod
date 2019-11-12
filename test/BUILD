cc_test(
    name="bounds-unit-tests",
    srcs=["bounds.cpp"],
    copts=["-Iexternal/googletest/googletest/include"],
    deps=[
        "//:grid",
        "@googletest//:gtest",
    ],
    timeout="short",
)


cc_test(
    name="grid-unit-tests",
    srcs=["grid.cpp"],
    copts=["-Iexternal/googletest/googletest/include"],
    deps=[
        "//:grid",
        "@googletest//:gtest",
    ],
    timeout="short",
)


cc_test(
    name="tiled-grid-unit-tests",
    srcs=["tiled_grid.cpp"],
    copts=["-Iexternal/googletest/googletest/include"],
    deps=[
        "//:grid",
        "@googletest//:gtest",
    ],
    timeout="short",
)
