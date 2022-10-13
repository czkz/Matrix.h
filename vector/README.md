# Vector.h
2D/3D Vector library for C++17.

- Header-only, single file
- No dependencies (optional Matrix.h dependency)
- Fully constexpr
- OpenGL-compatible
- Public domain (0BSD)

## Installation
Copy `Vector.h` into your project folder.

## Examples
A part of logic for boids simulation:
```cpp
const Vector2 toOther = all[i].pos - pos;
const float distSqr = toOther.MagnitudeSqr();

if (distSqr > sqr(visionDist)) continue;

if (Vector2::Dot(vel, toOther) > 0) {
    sumPos += all[i].pos;
    sumVel += all[i].vel;
    nSeen++;
}
```

Aiming logic for an FPS bot:
```cpp
Vector3 enemyViewangles = game::Utility.ViewanglesToPlayer(target);
const Vector3 aimCenter = CUtility::ViewanglesLerp(enemyViewangles, localViewangles, fAimShift);
Vector3 wantAimAt = aimCenter + RandOnCircle((float)rand() / RAND_MAX * std::min(fAimShift, 1-fAimShift));
CUtility::ClampViewAngles(wantAimAt);
Vector3 aimingDirection = CUtility::ViewanglesLerp(localViewangles, wantAimAt, fSmoothSpeed);
cmd.viewangles = aimingDirection;
mIVEngineClient->SetViewAngles(cmd.viewangles);
```
