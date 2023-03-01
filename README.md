# RayTracer

A CPU raytracer. Currently only supports triangles, spheres and their transformations (so we support spheroids too). Basic material properties are also supported, such as diffuse and specular reflections, ambience, shininess, light reflections and shadows. A KD tree with median split is used to drastically the amount of intersection checks needed, making renders of scenes with high poly count bearable. This project was the final homework in the [UC San DiegoX: CSE167x Computer Graphics online course](https://courses.edx.org/courses/course-v1:UCSanDiegoX+CSE167x+2T2018/information/)

![Cornell box](https://github.com/photown/RayTracer/blob/main/images/cornell_box_render.png?raw=true)

![Stanford dragon](https://github.com/photown/RayTracer/blob/main/images/stanford_dragon.png?raw=true)

## How to set up
You should be able to load and compile this solution in Visual Studio 2022 by loading `RayTracer.sln`.

## How to run
Make sure `cornell_box_input.txt` is set in `Debug > RayTracer Debug Properties > Debugging > Command Arguments`

Then start a Release build. When the rendering completes, you should see `cornell_box_example.png` in the source directory.

## Future improvements
- Move this to CUDA
- More basic shapes
- Soft shadows
- Textures and normal maps
- Animations?
