GlBlox - Rendering experiments
==============================

.. image:: https://github.com/pabennett/glblox/glblox.png

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