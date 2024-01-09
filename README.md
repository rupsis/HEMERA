# HEMERA

## Building
```
make
```

## Rendering
Once the project is built, use the `render.sh` script to render the scene.

```
bash render.sh <image_name>
```

**Note:** the `render.sh` uses ffmpeg to convert PPM to PNG


## C++ Things
- Const pointer arguments
- Const before function body
- Shared_ptr<>
- auto
- virtual const = 0

TODO (Things to read about):

- [Ray Sphere Intersection](https://www.youtube.com/watch?v=5P2UYWX6KRc)
- Schlick Approximation
- Snell's law
- BVH [Overview video](https://www.youtube.com/watch?v=EqvtfIqneKA)
    - Ray Slab Intersection