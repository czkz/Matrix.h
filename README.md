# Matrix.h
Matrix library for C++17.

- Header-only, single file
- No dependencies
- Compile-time dimension checking
- Fully constexpr
- OpenGL-compatible
- Public domain (0BSD)

# Installation
Copy `Matrix.h` into your project folder.

# Examples

```cpp
Matrix<2, 3> A ({
    1, 2, 3,
    4, 5, 6,
});
Matrix<3, 4> B ({
    1, 2, 3, 4,
    5, 6, 7, 8,
    9, 8, 7, 6,
});
Matrix<2, 2> C ({
    1, 2,
    3, 4,
});
// Matrix       D = A * C;  // Compile error, can't multiply A and C
// Matrix<3, 3> D = A * B;  // Compile error, can't assign 2x4 matrix into a 3x3 matrix
Matrix<2, 4> D = A * B;     // OK, explicit
Matrix       E = A * B;     // OK, implicit
```
See tests.cpp for more examples.

# Tests
### 100% branch coverage.
```bash
# Tests (requires doctest)
meson setup build/
meson compile -C build/ tests
./build/tests

# Tests with ASAN (requires doctest, libsanitizer)
meson setup build/ -Db_sanitize=address,undefined
meson compile -C build/ tests
./build/tests

# Code coverage (requires doctest, lcov or gcov)
meson setup build_cov/ -Db_coverage=true
meson test -C build_cov/
ninja -C build_cov/ coverage
```
