workspace(name="twod")
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")

# CRTP
git_repository(
    name="crtp",
    commit="f3f9e36411529f6a771d77a6f9c4c955b1eb5b90",
    remote="git@github.com:briancairl/crtp.git",
)


# GTest/GMock
http_archive(
    name="googletest",
    url="https://github.com/google/googletest/archive/release-1.8.0.zip",
    sha256="f3ed3b58511efd272eb074a3a6d6fb79d7c2e6a0e374323d1e6bcbcc1ef141bf",
    build_file="@//:third_party/googletest.BUILD",
    strip_prefix="googletest-release-1.8.0",
)
