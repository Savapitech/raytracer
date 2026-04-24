##Must

Your raytracer must support the following features:

3 Primitives:
– Sphere
– Plane

3 Transformations:
– Translation

3 Light:
– Directional light
– Ambient light

3 Material:
– Flat color

3 Scene configuration:
– Add primitives to scene
– Set up lighting
– Set up camera

3 Interface:
– No GUI, output to a PPM file
This section lists only required features. The Architecture section lists other mandatory requirements.


##Should
Once the previous features are working properly, you should add the following ones:
3 Primitives:
– Cylinder
– Cone
3 Transformation:
– Rotation
3 Light:
– Drop shadows
3

##Could
Now that you've implemented the very basics of your raytracer, implement freely the following features:
3 Primitives:
– Limited cylinder (0.5)
– Limited cone (0.5)
– Torus (1)
– Tanglecube (1)
– Triangles (1)
– .OBJ file (1)
– Fractals (2)
– Möbius strip (2)
3 Transformations:
– Scale (0.5)
– Shear (0.5)
– Transformation matrix (2)
– Scene graph (2)
3 Light:
– Multiple directional lights (0.5)
– Multiple point lights (0.5)
– Colored light (0.5)
– Phong reflection model (2)
– Ambient occlusion (2)
3 Material:
– Transparency (0.5)
– Refraction (1)
– Reflection (0.5)
– Texturing from file (1)
– Texturing from procedural generation of chessboard (1)
– Texturing from procedural generation of Perlin noise (1)
– Normal mapping (2)
3 Scene configuration:
– Import a scene in a scene (2)
– Set up antialiasing through supersampling (0.5)
– Set up antialiasing through adaptative supersampling (1)
3 Optimizations:
– Space partitionning (2)
– Multithreading (1)
– Clustering (using network to render a scene on multiple machines) (3)
4
3 Interface:
– Display the image during and after generation (1)
– Exit during or after generation (0.5)
– Scene preview using a basic and fast renderer (2)
– Automatic reload of the scene at file change (1)