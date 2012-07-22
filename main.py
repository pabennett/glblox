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

from pyglet import clock
from pyglet import font

from external import Camera, World, Player

import Image

from ctypes import c_byte

import console
import controller

program = ShaderProgram.open('shaders/main.shader')

# Set up the Window (pyglet)
config = Config(buffers=2, samples=4)
window = pyglet.window.Window(caption='GlBlox', width=1152, height=864, config=config, vsync=False, fullscreen=False)
window.set_exclusive_mouse(True)

console_font = font.load('Consolas', 14, bold=False, italic=False)

status = console.StatusConsole(x=window.width * 0.005, 
                          y=window.height * 0.98,
                          width=window.width)
                             
console = console.Console(x=window.width * 0.005,
                          y=window.height * 0.70,
                          width=window.width)
      
status.addParameter('FPS')      
status.addParameter('Vertices')
status.addParameter('Position')
status.addParameter('Velocity')
status.addParameter('Chunk updates')
                                                                              
pyglet.clock.ClockDisplay()
wiremode = False

# Set up the World (glblox Lib)
chunk_size = 32
xWrap = True
yWrap = False
zWrap = True

#Open a heightmap image
im = Image.open('savelevels/world.png')
# Convert image to greyscale
im = im.convert("L")
# Get the dimensions of the image
(im_width,im_height) = im.size
im_max = im.getextrema()[1]
# Convert the image data into an array
image_data = bytearray(im.getdata())
data = (c_byte * len(image_data))()
for i in range(len(image_data)):
    data[i]= image_data[i]
# Create a world large enough to hold this image
wx = im_width / chunk_size
wz = im_height / chunk_size
wy = int(im_max/chunk_size) + 1

print "Created a world of " + str(wx) + "x" + str(wy) + "x" + str(wz) + " chunks..."
world = World(wx, wy, wz, chunk_size,xWrap,yWrap,zWrap,program.id)
#world = World(64,3,34,chunk_size,xWrap,yWrap,zWrap,program.id)
world.loadHeightmap(data,chunk_size)
  
#wx = 10
#wy = 1
#wz = 4
#world = World(wx,wy,wx,chunk_size,fast_meshes)
#world.random()

# Specify the world view distance in voxels
world.setViewDistance(100000)
world.setRandomTerrainEnabledState(False)
# Create a camera object for viewing and displaying the world
camera = Camera(window.width, window.height, 65, 0.1, 2000.0, True, program.id)
camera.perspective(window.width, window.height, 65, 0.1, 2000.0)
player = Player(camera, world, program.id)

# Set up the Keyboard handler (pyglet)
controller = controller.Controller(window, player, console)
    
def statusUpdates(dt):
    position = tuple(int(a) for a in player.getPos())
    velocity = tuple(int(a) for a in player.getVelocity())
    fps = clock.get_fps()
    
    status.setParameter('Vertices', world.numVertices())
    status.setParameter('Position', position)
    status.setParameter('Chunk updates', world.chunksAwaitingUpdate())
    status.setParameter('Velocity', velocity)
    status.setParameter('FPS', fps)

clock.schedule_interval(statusUpdates, 0.2)

    
# MAIN RENDER LOOP
@window.event
def on_draw():
    window.clear()
    
    # Update camera
    player.setCameraMVP()
    player.draw()
    
    # Check wireframe mode
    if controller.states['Wireframe Mode']:
        glPolygonMode(GL_FRONT, GL_LINE)
    else:
        glPolygonMode(GL_FRONT, GL_FILL)

    # Draw World   
    world.draw(player)
    
    glPolygonMode(GL_FRONT, GL_FILL)
    # Show Console Data
    status.draw()
    console.draw()

# Initialisation
if __name__ == '__main__':
    print "Program launching..."
    glClearColor(0.05, 0.10, 0.15, 1.0); #Night
    #glClearColor(0.95, 0.95, 0.95, 1.0); #day
    glEnable(GL_CULL_FACE)
    glEnable(GL_DEPTH_TEST)
    glCullFace(GL_BACK)
    pyglet.app.run()