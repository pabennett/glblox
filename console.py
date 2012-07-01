#!/usr/bin/env python

""" Description..."""

__author__ = "Peter Bennett"
__copyright__ = "Copyright 2012, Peter A Bennett"
__license__ = "LGPL"
__version__ = "0.1"
__maintainer__ = "Peter Bennett"
__email__ = "pab850@googlemail.com"
__contact__ = "www.bytebash.com"

import pyglet

class Console:
    """ A simple text console for displaying events."""
    def __init__(self,x,y,width):
        self.textBuffer = []
        self.bufferLength = 10
        self.x = x
        self.y = y
        self.width = width
        self.label = pyglet.text.Label("...",
                            font_name='Consolas',
                            font_size=10,
                            x=self.x,
                            y=self.y,
                            width=self.width,
                            multiline=True)  
                        
    def updateConsole(self, string):
        self.textBuffer.append(string)
        if (len(self.textBuffer)>self.bufferLength):
            self.textBuffer = self.textBuffer[1:]
        self.label.text = ''.join([str(x) + '\n' for x in self.textBuffer])
            
    def draw(self):
        self.label.draw()
        
class StatusConsole:
    """ A static console for displaying parameters that change over time. """
    def __init__(self,x,y,width):
        self.parameters = {}
        self.x = x
        self.y = y
        self.width = width
        self.label = pyglet.text.Label("...",
                                       font_name='Consolas',
                                       font_size=10,
                                       x=self.x,
                                       y=self.y,
                                       width=self.width,
                                       multiline=True)
    def addParameter(self, parameter):
        if(not self.parameters.has_key(parameter)):
            self.parameters[parameter] = None
            self.modified = True
    def setParameter(self, parameter, value):
        if(self.parameters.has_key(parameter)):
            self.parameters[parameter] = value
            self.modified = True
       
    def updateLabel(self):
        self.label.text = ''.join([str(a) + " : " + str(b) + '\n' for a,b in self.parameters.items()])
    def draw(self):
        if(self.modified):
            self.updateLabel()
        self.label.draw()
   