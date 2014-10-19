GlBlox - Rendering experiments
==============================

![GlBlox](http://www.bytebash.com/files/glblox.png "GlBlox")

Description
-----------

GlBlox is an OpenGL volume renderer that draws volumes as a blocky mesh.


Details
-------

* Core rendering engine implemented in C++ with a Python wrapper for prototyping.
* Uses the Pyglet Python module to provide OpenGL window manager and input
* Generate volumes using heightmaps, perlin noise or simple procedures.

Features implemented
--------------------

* Volume storage classes, volume loading and volume modification.
* Mesh generators for optimised or standard 'blocky' meshes.
* Quaternion based freelook camera and mouse controls.
* Frustrum culling.
* Back-face culling.
* Block shading using colourmaps (GLSL)
* Collision detection.
* Chunk paging for large worlds.

Dependencies
------------

The main program has been written in Python 2.7 and requires the following additional modules:

[Pyglet](http://www.pyglet.org/ "Pyglet")

The GLBlox core engine was written in C++ and requires the following additional libraries:

[glm](http://glm.g-truc.net/0.9.5/index.html "GLM")
[boost](http://www.boost.org/ "Boost")
[glew](http://glew.sourceforge.net/ "GLEW")

Features to add
---------------

* Volume storage and loading with compression + memory optimisations.
* Block picking and volume modification.
* Lighting effects
* GUI
* Further volume generation procedures, such as 3D simplex.

Further reading
--------------------

http://www.bytebash.com