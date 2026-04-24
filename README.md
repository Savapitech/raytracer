# Ray Tracer

A physically-based ray tracer written in C++, featuring a BVH acceleration structure, PBR shading, real-time interactive preview, and support for multiple materials and geometry types.

---

## Gallery

> Scenes rendered with the engine — Manhattan skyline, mirror spheres, glass refraction, and more.

| Scene          | Description                                                       |
| -------------- | ----------------------------------------------------------------- |
| Manhattan      | Full city geometry loaded from `.obj` files                       |
| Mirror Spheres | Reflective surfaces with Fresnel-based PBR                        |
| Glass          | Refraction with Schlick approximation & total internal reflection |
| Soft Shadows   | Stratified jittered area light sampling                           |

---

## Features

### Rendering
- **Physically-Based Rendering (PBR)** — GGX normal distribution, Smith geometry term, Schlick Fresnel
- **Reflections** — Recursive ray bouncing with configurable depth
- **Refractions** — Snell's law with total internal reflection handling
- **Soft Shadows** — Stratified grid sampling over area lights with jitter
- **Textures** — UV-mapped texture support on all geometry types

### Acceleration
- **BVH (Bounding Volume Hierarchy)** — SAH-optimized binary tree for fast ray traversal
- **AABB intersection** — Slab method with precomputed inverse directions
- **Tile-based rendering** — 16×16 tiles for better cache locality
- **Progressive refinement** — Binary pixel skipping on camera movement for real-time feedback

### Geometry
- **Sphere**
- **Triangle** (with `.obj` mesh loading)
- **Cylinder** (with caps)
- **RectangleXZ** (infinite plane segment)

### Materials
| Material | Properties |
|---|---|
| `Default` | Lambertian diffuse + specular |
| `Mirror` | Perfect reflection, low roughness |
| `Chrome` | Metallic mirror variant |
| `Glass` | Dielectric with refraction & Fresnel reflection |

### Scene & Camera
- **libconfig** scene file format (`.cfg`)
- Camera with configurable FOV, position, and direction
- Interactive camera movement (keyboard-driven, real-time)
- `.obj` file import with per-mesh transform (position, rotation, scale)

---

## Interactive Controls

| Key | Action |
|---|---|
| `Z` / `S` | Move forward / backward |
| `Q` / `D` | Strafe left / right |
| `↑` / `↓` | Move up / down |
| `Numpad 4` / `6` | Rotate camera left / right (yaw) |
| `Numpad 8` / `5` | Rotate camera up / down (pitch) |

---

## Architecture

```
main.cpp
├── ParserCmd        — Command-line argument parsing
├── Scene            — Scene loading from .cfg + .obj files
│   ├── Camera
│   ├── ShapeFactory — Sphere, Triangle, Cylinder, RectangleXZ
│   └── MaterialFactory — Mirror, Chrome, Glass, Default
├── BVH              — SAH-based bounding volume hierarchy
├── Render           — Tile renderer + progressive refinement
│   ├── FillRayBuffer — Per-pixel ray dispatch
│   ├── Shade         — Recursive shading (PBR + reflection + refraction)
│   └── Graphical     — SFML window, input, display
└── AABB             — Axis-aligned bounding box
```

---

## Build & Run

### Dependencies
- **C++17** or later
- [SFML 3](https://www.sfml-dev.org/) — window, rendering, input
- [libconfig++](https://hyperrealm.github.io/libconfig/) — scene file parsing

### Build

```bash
mkdir build && cd build
cmake ..
make -j$(nproc)
```

Or with your existing build system:

```bash
make
```

### Run

```bash
./raytracer --scene=scenes/my_scene.cfg
```

Optional flags:

```
--log=DEBUG|INFO|WARNING|ERROR|NONE
--pathtracing=true
```

---

## Scene File Format

Scene files use [libconfig](https://hyperrealm.github.io/libconfig/) syntax (`.cfg`):

```cfg
scene:
{
    fov = 60;
    pos = [0.0, 2.0, -10.0];
    dir = [0.0, 0.0, 1.0];

    objects = (
        {
            shape: {
                type = "sphere";
                pos  = [0.0, 1.0, 5.0];
                radius = 1.0;
                color = [255, 255, 255];
            }
            material: {
                type = "glass";
            }
        },
        {
            shape: {
                type = "rectangleXZ";
                x    = [-10.0, 10.0];
                z    = [-10.0, 10.0];
                y    = 0.0;
                color = [200, 200, 200];
            }
        }
    );

    obj = (
        {
            path  = "assets/manhattan.obj";
            pos   = [0.0, 0.0, 0.0];
            rot   = [0.0, 0.0, 0.0];
            scale = [1.0, 1.0, 1.0];
        }
    );
};
```

---

## 🧠 Technical Notes

### BVH — Surface Area Heuristic (SAH)
The BVH is built using a full SAH sweep across the longest axis of each node. Left/right AABB prefix arrays are precomputed to evaluate all split candidates in O(n) per node. Leaf threshold is tuned to balance traversal vs intersection cost.

### PBR Shading Model
- **NDF**: GGX / Trowbridge-Reitz
- **Geometry**: Smith with Schlick-GGX approximation
- **Fresnel**: Schlick approximation
- Metallic workflow: `F0 = lerp(0.04, albedo, metallic)`

### Soft Shadows
A 4×4 stratified grid is jittered per sample over an area light, giving smooth penumbra. Shadow rays skip transparent objects (glass).

### Progressive Rendering
On camera movement, the renderer progressively fills in pixels using a binary bitmask pattern (every 4th pixel → every 2nd → full resolution) to give immediate visual feedback.
