-- Module Definition
local module = {
    XMLElement = {
        VOID = {},

        FMT_ATTRIBUTE = '%s="%s"',
        FMT_COMMENT   = '<!--%s-->',
        FMT_STANDARD  = '<%s%s>%s</%s>',
        FMT_VOID      = '<%s%s/>',

        --- @param tag string
        --- @param parent table
        --- @param attrs table
        --- @return table
        new = function(self, tag, parent, attrs) end

        --- @param element table
        --- @return nil
        add = function(self, element) end

        --- @param text string
        --- @return nil
        comment = function(self, text) end

        --- @return table
        __tostring = function(self) end
    },
    SVGElement = {
        __index = XMLElement,

        VOID = {
            'circle',
            'ellipse',
            'line',
            'path',
            'polygon',
            'rect',
            'stop',
            'use'
        },

        DEFAULT_SIZE = {100, 100},
    },
    HTMLElement = {
        __index = XMLElement,

        VOID = {
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
        },

        FMT_VOID = '<%S%S>',
    },
}





-- Function Implementations
--- XMLElement Functions
module.XMLElement.new = function(self, tag, parent, attrs)
    local obj = {
        children = {},
        parent = parent,
        tag = tag:lower(),
        attrs = attrs or {},
    }

    if parent then
        parent.add(obj)
    end

    setmetatable(obj, self)
    obj.__index = self
    return obj
end


module.XMLElement.add = function(self, element)
    element.parent = self
    table.insert(self.children, element)
end


module.XMLElement.comment = function(self, text)
    local element = string.format(self.FMT_COMMENT, text)
    table.insert(self.children, element)
end


module.XMLElement.__tostring = function(self)
    local arr = {}
    for key, value in pairs(self.attrs) do
        table.insert(arr, string.format(self.FMT_ATTRIBUTE, key, value))
    end

    local attributes = #arr > 0 and ' ' .. table.concat(arr, ' ') or ''

    if #self.VOID > 0 or #self.children == 0 then
        return string.format(self.FMT_VOID, self.tag, attributes)
    else
        local children = table.concat(self.children, '')
        return string.format(self.FMT_STANDARD, self.tag, attributes, children, self.tag)
    end
end




--- SVGElement Functions
module.SVGElement.new = function(self, tag, parent, attrs)
    local obj = XMLElement:new(tag, parent, attrs)
    setmetatable(obj, self)
    obj.__index = self

    if obj.tag == 'svg' and not obj.attrs.viewport then
        local width = obj.attrs.width or self.DEFAULT_SIZE[1]
        local height = obj.attrs.height or self.DEFAULT_SIZE[2]
        obj.attrs.viewport = string.format('0 0 %d %d', width, height)
    end

    return obj
end