# Linear algebra libraries for C++17.

- Header-only
- No dependencies
- Fully constexpr
- OpenGL-compatible
- Public domain (0BSD)

## Example
Simple Transform class:
```cpp
#pragma once
#include <Vector.h>
#include <Quaternion.h>
#include <Matrix.h>

struct Transform {
    Vector3 position;
    Quaternion rotation = Quaternion::Identity();
    Vector3 scale = Vector3(1);

    Matrix<4, 4> Matrix() const {
        return
            position.TranslationMatrix() *
            rotation.RotationMatrix() *
            scale.ScaleMatrix();
    }
};
```
