# Open GL Procedural Planetarium

A simple OpenGL scene that contains procedurally generated planets, a space station, space fighter and a halo ring. 

The planets are generated by selecting a random textures for terrain and water on creation and then terrain is randomised using a perlin noise algorithm.

As uv spheres are created, the distance between points are not uniform on the spheres, leading to strange distortions in the final output. This can be fixed by creating an icosphere instead.

This program utilised my own OBJ file parser.

**Photos**

![Photo](https://imgur.com/BNKb5Z1.jpg) 

![Photo](https://imgur.com/ItB98i8.jpg)

