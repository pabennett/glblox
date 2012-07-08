#!/usr/bin/env python

"""A Python wrapper for the block rendering library written in c++, this wrapper
provides access to the underlying functions contained within the c++ library
by using Python ctypes.""" 

__author__ = "Peter Bennett"
__copyright__ = "Copyright 2012, Peter A Bennett"
__license__ = "LGPL"
__maintainer__ = "Peter Bennett"
__email__ = "pab850@googlemail.com"
__contact__ = "www.bytebash.com"
__version__ = "$Rev: 2 $"
__date__ = "$Date: 2012-03-11 15:19:25 +0000 (Sun, 11 Mar 2012) $"

from os.path import abspath, dirname, join
import ctypes
from ctypes import c_float, c_int, cdll, c_void_p, c_byte, c_char_p, sizeof, c_uint, c_bool

c_float_p = ctypes.POINTER(ctypes.c_float)

here = dirname(abspath(__file__))
path = join(here, 'glblox.so')

print "Loading library: " + path
vol_lib = cdll.LoadLibrary(path)

CAM_newCAM = vol_lib.newCamera
CAM_newCAM.argtypes = c_int, c_int, c_float, c_float, c_float, c_bool
CAM_newCAM.restype = c_void_p
CAM_pers = vol_lib.C_setPerspective
CAM_pers.argtypes = c_void_p, c_int, c_int, c_float, c_float, c_float,
CAM_setPos = vol_lib.C_setPos
CAM_setPos.argtypes = c_void_p, c_float, c_float, c_float, 
CAM_getPosX = vol_lib.C_getPosX
CAM_getPosX.restype = c_float
CAM_getPosX.argtypes = c_void_p,
CAM_getPosY = vol_lib.C_getPosY
CAM_getPosY.argtypes = c_void_p,
CAM_getPosY.restype = c_float
CAM_getPosZ = vol_lib.C_getPosZ
CAM_getPosZ.restype = c_float
CAM_getPosZ.argtypes = c_void_p,
CAM_orient = vol_lib.C_Orient
CAM_orient.argtypes = c_void_p, c_float, c_float, 
CAM_move = vol_lib.C_Move
CAM_move.argtypes = c_void_p, c_float, c_float, c_float
       
class Camera:
    def __init__(self, width, height, vertFOV, frustrumNear, frustrumFar, flightMode):
        self.obj = vol_lib.newCamera(width, height, vertFOV, frustrumNear, frustrumFar, flightMode)
    def setMVP(self, location):
        vol_lib.C_setMVP(self.obj, location)
    def perspective(self, width, height, fov, fzn, fzf):
        vol_lib.C_setPerspective(self.obj, width, height, fov, fzn, fzf)
    def setPos(self, x, y ,z):
        vol_lib.C_setPos(self.obj, x, y, z)
    def orient(self, yaw, pitch):
        vol_lib.C_Orient(self.obj, yaw, pitch)
    def move(self, dx, dy, dz):
        vol_lib.C_Move(self.obj, dx, dy, dz)
    def getPos(self):
        x = vol_lib.C_getPosX(self.obj)
        y = vol_lib.C_getPosY(self.obj)
        z = vol_lib.C_getPosZ(self.obj)
        return (x,y,z)
    

WORLD_draw = vol_lib.worldDraw
WORLD_draw.argtypes = c_void_p, c_void_p, c_uint
WORLD_fillSpheres = vol_lib.worldFillSpheres
WORLD_draw.argtypes = c_void_p,
WORLD_numVertices = vol_lib.worldNumVertices
WORLD_numVertices.argtypes = c_void_p,
WORLD_numVertices.restype = c_int
WORLD_load = vol_lib.worldLoad
WORLD_load.argtypes = c_void_p, c_void_p, c_int, c_int, c_int, c_int
WORLD_loadHeightmap = vol_lib.worldLoadHeightmap
WORLD_loadHeightmap.argtypes = c_void_p, c_void_p, c_int
WORLD_deleteBlockAt = vol_lib.worldDeleteBlockAt
WORLD_deleteBlockAt.argtypes = c_void_p, c_int, c_int, c_int
WORLD_worldModifyRegionAt = vol_lib.worldModifyRegionAt
WORLD_worldModifyRegionAt.argtypes = c_void_p, c_int, c_int, c_int, c_byte, c_int
WORLD_worldSetViewDistance = vol_lib.worldSetViewDistance
WORLD_worldSetViewDistance.argtypes = c_void_p, c_int
       
class World:
    def __init__(self,dimx,dimy,dimz,chunk_size,yWrapping):
        vol_lib.initGLEW()
        self.obj = vol_lib.newWorld(dimx,dimy,dimz,chunk_size,yWrapping)
    def draw(self, program, camera):
        vol_lib.worldDraw(self.obj, camera.obj, program)
    def fillSpheres(self):
        vol_lib.worldFillSpheres(self.obj)
    def fill(self):
        vol_lib.worldFill(self.obj)
    def random(self):
        vol_lib.worldRandom(self.obj)
    def numVertices(self):
        return vol_lib.worldNumVertices(self.obj)
    def load(self, array, x, y, z, chunk_size):
        c_array_p = array.ctypes.data_as(c_char_p)
        vol_lib.worldLoad(self.obj,c_array_p, x, y, z, chunk_size)
    def loadHeightmap(self, array, chunk_size):
        c_array_p = array.ctypes.data_as(c_char_p)
        vol_lib.worldLoadHeightmap(self.obj,c_array_p, chunk_size)
    def deleteBlockAt(self, x, y, z):
        vol_lib.worldDeleteBlockAt(self.obj, x, y, z)
    def modifyRegionAt(self, x, y, z, val, radius):
        vol_lib.worldModifyRegionAt(self.obj, x, y, z, val, radius)
    def setViewDistance(self, distance):
        vol_lib.worldSetViewDistance(self.obj, distance)