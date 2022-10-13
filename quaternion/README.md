# Quaternion.h
Quaternion library for C++17.

- Header-only
- Minimal dependencies (requires Vector.h)
- Fully constexpr
- OpenGL-compatible (glsl vec4)
- Public domain (0BSD)

## Installation
Copy `Quaternion.h` and `Vector.h` into your project folder.

## Example
First person character controller:
```cpp
namespace input {
    Vector3 get_move    (GLFWwindow* window);  // W, A, S, D, Shift, Space
    Vector3 get_rotation(GLFWwindow* window);  // Mouse (yaw, pitch), Q, E (roll)
}

Vector3 camera_position = {0, 0, 0};
Quaternion camera_rotation = Quaternion::Identity();

// Called each frame
void handleUserInput() {
    camera_rotation = camera_rotation * Quaternion::Euler(input::get_rotation(window) * 0.05);
    camera_position += camera_rotation.Rotate(input::get_move(window) * 0.01);
}
```
