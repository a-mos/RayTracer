# RayTracer
### Scenes
![scene1](https://github.com/a-mos/RayTracer/tree/master/Output/Out_1.jpg)
![scene2](https://github.com/a-mos/RayTracer/tree/master/Output/Out_2.jpg)
![scene3](https://github.com/a-mos/RayTracer/tree/master/Output/Out_3.jpg)
![scene4](https://github.com/a-mos/RayTracer/tree/master/Output/Out_4.jpg)

### Compile:
```
mkdir build
cd build
cmake .. 
make
```

### Usage:
```
./rt −out <output_path> −scene <scene_number> −threads <threads>
```
scene_number = {1, 2, 3, 4}

### Features:
* Raytracing with Phong model (Scene 1, 2)
* Distance Aided Ray Marching (Scene 3)
* Path tracing (Scene 4)
* Objects – sphere, cube, plane, triangle mesh, quadratic surface, fractal surface
