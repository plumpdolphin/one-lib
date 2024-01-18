'''
Author: PlumpDolphin
Date: January 18, 2024

Description: 
    Provides unopinionated classes to generate valid code for 
    XML-based formats including XML, HTML, and SVG.

License:
    The code in this file is licensed under the
    Revised 3-Clause BSD License.
    For details, see https://opensource.org/licenses/BSD-3-Clause
'''



class XMLElement:
    '''Implements base class representation and functionality for XML-derived structures.'''
    VOID = []

    FMT_VOID     = '<%s%s/>'
    FMT_STANDARD = '<%s%s>%s</%s>'

    def __init__(self, tag: str, parent = None, **kwargs):
        '''Initialize XML element from tag name. Attributes are optional.'''
        self.tag = tag.lower()
        self.children = []

        if parent:
            parent.add(self)

        self.attrs = kwargs

    def add(self, element):
        '''Add element to children'''
        element.parent = self
        self.children.append(element)

    def comment(self, text):
        '''Adds a comment to the XML element.'''
        self.children.append(f'<!--{text}-->')
    
    def __str__(self):
        '''Convert Element object into XML string representation.'''
        # Create list of key value pairs in XML attribute format
        arr = [
            f'{str(key)}="{str(value)}"'
            for key, value in self.attrs.items()
        ]

        # Append attributes
        attributes = ''
        if len(arr) > 0:
            attributes = ' ' + ' '.join(arr)

        # If element is a void element, return
        if (self.tag in self.VOID or 
            (not self.VOID and not self.children)):
            return self.FMT_VOID % (self.tag, attributes)
            #return f'<{self.tag}{attributes}/>'
        
        # Else continue adding child elements
        children = ''.join([str(x) for x in self.children])

        # Return whole element
        return self.FMT_STANDARD % (self.tag, attributes, children, self.tag)
        # return f'<{self.tag}{attributes}>{children}</{self.tag}>'


    @property
    def inner(self):
        return ''.join([str(x) for x in self.children])
    
    @inner.setter
    def inner_setter(self, value):
        self.children = self.from_str(value)



class SVGElement(XMLElement):
    DEFAULT_SIZE = (100, 100)

    VOID = [
        'circle',
        'ellipse',
        'line',
        'path',
        'polygon',
        'rect',
        'stop',
        'use',
    ]

    def __init__(self, tag: str, parent = None, **kwargs):
        '''Initialize SVG element from tag name. Attributes are optional'''
        # Run XMLElement's initialization 
        super().__init__(tag, parent=parent, **kwargs)

        # Create viewport attribute from width and height if not present
        if self.tag == 'svg' and not self.attrs.get('viewport'):
            # Get width and height, if present, else use the default size
            width = self.attrs.get('width', self.DEFAULT_SIZE[0])
            height = self.attrs.get('height', self.DEFAULT_SIZE[1])

            self.attrs['viewport'] = f'0 0 {width} {height}'



class HTMLElement(XMLElement):
    VOID = [
        'area', 
        'base', 
        'br', 
        'col', 
        'command', 
        'embed', 
        'hr', 
        'img', 
        'input', 
        'keygen', 
        'link', 
        'meta',
        'param', 
        'source', 
        'track', 
        'wbr'
    ]

    FMT_VOID = '<%s%s>'




'''Syntax Examples
root = SVGElement('svg',
    xmlns = 'http://www.w3.org/2000/svg',
    width = 300,
    height = 300
)

group = SVGElement('g',
    parent = root
)

group.comment('This group is an example.')

group.add( SVGElement('circle', cx=50, cy=50, r=20) )

circle = SVGElement('circle',
    parent = group,
    cx = 150,
    cy = 150,
    r = 10
)

open('output.svg', 'w').write( str(root) )
'''