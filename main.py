#!/usr/bin/env python

"""This module uses the glblox c++ library to create and render voxel volumes
as a triangular mesh. The OpenGL wrapper Pyglet is used to create a window
within Python and the Gletools library is used to compile fragment and vertex
shaders.""" 

__author__ = "Peter Bennett"
__copyright__ = "Copyright 2012, Peter A Bennett"
__license__ = "LGPL"
__maintainer__ = "Peter Bennett"
__email__ = "pab850@googlemail.com"
__contact__ = "www.bytebash.com"
__version__ = "$Rev: 2 $"
__date__ = "$Date: 2012-03-11 15:19:25 +0000 (Sun, 11 Mar 2012) $"

import pyglet
from pyglet.gl import *
from gletools import ShaderProgram

from pyglet.window import key
from pyglet.clock import schedule
from pyglet import font

from lib import Camera, World

import Image
import numpy as np
from ctypes import c_byte


program = ShaderProgram.open('shaders/main.shader')

# Set up the Window (pyglet)
config = Config(buffers=2, samples=4)
window = pyglet.window.Window(caption='GlBlox', width=1152, height=864, config=config, vsync=False)
window.set_exclusive_mouse(True)
# Misc Setup
console_font = font.load('DejaVu Sans Mono', 14, bold=False, italic=False)
fps = pyglet.clock.ClockDisplay(color=(1,1,1,1),
                                font=console_font)                                                    
label = pyglet.text.Label("Tempstring", 
                          font_name='DejaVu Sans Mono',
                          font_size=14, 
                          x=window.width * 0.005, 
                          y=window.height * 0.98)                                      
pyglet.clock.ClockDisplay()
wiremode = False

# Set up the camera (glblox Lib)
camera = Camera(window.width, window.height, 65, 0.1, 2000.0, True)
camera.perspective(window.width, window.height, 65, 0.1, 2000.0)

# Set up the World (glblox Lib)
chunk_size = 64
fast_meshes = True

#Open a heightmap image
im = Image.open('savelevels/world.png')
# Convert image to greyscale
im = im.convert("L")
# Get the dimensions of the image
(im_width,im_height) = im.size
# Convert the image data into a numpy array
data = np.array(im.getdata())
# Create a world large enough to hold this image
wx = im_width / chunk_size
wz = im_height / chunk_size
wy = int(max(data)/chunk_size) + 1
print "Created a world of " + str(wx) + "x" + str(wy) + "x" + str(wz) + " chunks..."
world = World(wx, wy, wz, chunk_size, fast_meshes)
# The number of blocks in the volume
arraysize = chunk_size*chunk_size*chunk_size
# The volume height in chunks
vol_height_chunks = (max(data)/chunk_size)
# Dictionary of arrays (chunks)
datablocks = {}
for x in xrange(0, wx):
    for y in xrange(0, wy):
        for z in xrange(0, wz):
            datablocks[(x,y,z)] = np.zeros((arraysize), dtype=c_byte) 

# Break the image into tiles representing pillars of chunks    
for tile_x in xrange(0,im_width/chunk_size):
    for tile_y in xrange(0,im_height/chunk_size):
        print "Processing tile: " + str(tile_x) + "," + str(tile_y)
        # Load the tile data into the pillar of chunks.
        for x in xrange(0,chunk_size):
            for z in xrange(0, chunk_size):
                chunk_x_idx = (tile_x * chunk_size)
                chunk_z_idx = (tile_y * chunk_size)
                # Y ranges from 0 to the intensity of the pixel at this x,z
                for y in xrange(0, data[chunk_x_idx + x + ((chunk_z_idx + z) * im_height)]):
                    # Get the block coords
                    block_x = x
                    block_y = ((y - (y/chunk_size) * chunk_size) * chunk_size)
                    block_z = (z * chunk_size * chunk_size)
                    # Set the current block to solid
                    datablocks[(tile_x,y/chunk_size,tile_y)][block_x + block_y + block_z] = 1

print "Loading World data into chunks..."
for (x,y,z),chunk in datablocks.items():
    print "Loading chunk: " + str(x) + "," + str(y) + "," + str(z)
    world.load(chunk, x, y, z, chunk_size)
            
# Set up the Keyboard handler (pyglet)
keys = key.KeyStateHandler()
window.push_handlers(keys)
# Key Hold Events
def update(dt):
    m = min(dt, 0.17)*20
    if keys[key.W]:
        camera.move(0,0,m)
    elif keys[key.S]:
        camera.move(0,0,-m)
    if keys[key.A]:
        camera.move(-m,0,0)
    elif keys[key.D]:
        camera.move(m,0,0)
        

    if keys[key.T]:
        world.fillSpheres()    
schedule(update)         

# Set up the Mouse handler (pyglet)
@window.event
def on_mouse_motion(x, y, dx, dy):
    camera.orient(dx*0.08, -dy*0.08)
    
# MAIN RENDER LOOP
@window.event
def on_draw():
    window.clear()
    # Update camera
    camera.setMVP(program.id)
    # Wireframe Mode
    if keys[key.G]:
        glPolygonMode(GL_FRONT, GL_LINE)
    else:
        glPolygonMode(GL_FRONT, GL_FILL)
    # Draw World   
    world.draw(program.id, camera)
    # Show vertex count
    label.text = ("Vertices: {:,d}".format(world.numVertices()))
    label.draw()    
    # Show FPS       
    fps.draw()

# Initialisation
if __name__ == '__main__':
    print "Program launching..."
    glClearColor(0.05, 0.10, 0.15, 1.0);
    glEnable(GL_CULL_FACE)
    glEnable(GL_DEPTH_TEST)
    glCullFace(GL_BACK)
    pyglet.app.run()