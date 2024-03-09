'''
Author: PlumpDolphin
Date: February 9, 2024

Description: 
    Provides a wrapper for Tkinter with a default styling for creating easy GUIs.

License:
    The code in this file is licensed under the
    Revised 3-Clause BSD License.
    For details, see https://opensource.org/licenses/BSD-3-Clause
'''



import time
from enum import Enum

import tkinter as tk
from tkinter import ttk
from ttkthemes import ThemedStyle



class App:
    def __init__(self, title, style = 'adapta'):
        # Create window
        self.root = tk.Tk()
        self.root.title(title)

        # Default size
        self.resize(800, 600)

        # Style window
        self.style = ThemedStyle(self.root)
        self.style.set_theme(style)

        # Create background frame
        self.frame = Element(
            self.root,
            Element.Type.FRAME,
        )
        self.frame.pack()

        self.pages = {}

    def resize(self, width, height):
        self.root.geometry(f'{width}x{height}')

    def add_page(self, name):
        page = Page(self.frame, name)
        self.pages[name] = page
        return page

    def get_page(self, name):
        return self.pages.get(name, None)

    def run(self):
        self.root.mainloop()



class Page:
    def __init__(self, root, name):
        self.root = root
        self.name = name
        self.elements = []

    def add_element(self, type, **kwargs):
        element = Element(self.root, type, **kwargs)
        self.elements.append(element)
        return element



class Element:
    class Type(Enum):
        LABEL   = ttk.Label
        BUTTON  = ttk.Button
        ENTRY   = ttk.Entry
        TEXT    = tk.Text
        FRAME   = ttk.Frame
        CANVAS  = tk.Canvas
        CHECK   = ttk.Checkbutton
        RADIO   = ttk.Radiobutton
        SCALE   = ttk.Scale
        LIST    = tk.Listbox
        SCROLL  = ttk.Scrollbar
        MENU    = tk.Menu
        MESSAGE = tk.Message

    class Placement(Enum):
        PACK  = (tk.Pack.pack,   tk.Pack.forget)
        GRID  = (tk.Grid.grid,   tk.Grid.forget)
        PLACE = (tk.Place.place, tk.Place.forget)

        @property
        def show(self):
            return self.value[0]

        @property
        def hide(self):
            return self.value[1]

        
        

    def __init__(self, parent, type, **kwargs):
        self.widget = type.value(parent, **kwargs)
        self.elements = []

    def add_element(self, type, **kwargs):
        element = Element(self, type, **kwargs)
        self.elements.append(element)
        return element
    
    def pack(self, **kwargs):
        self.widget.pack(**kwargs)
        self.placement = Element.Placement.PACK
        self.placement_args = kwargs 
    
    def grid(self, **kwargs):
        self.widget.grid(**kwargs)
        self.placement = Element.Placement.GRID
    
    def place(self, **kwargs):
        self.widget.place(**kwargs)
        self.placement = Element.Placement.PLACE

    def hide(self):
        self.placement.hide(self.widget)
    
    def show(self):
        if self.placement:
            self.placement.show(self.widget, **self.placement_args)



def button_click():
    label.hide()

    time.sleep(2)

    label.show()


if __name__ == '__main__':
    app = App('Plumpy Test')

    page = app.add_page('Home')

    label = page.add_element(
        Element.Type.BUTTON,
        text = 'Hello application!',
        command = button_click
    )
    label.pack(pady=20)

    app.run()