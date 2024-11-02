#Better solution needed
import bpy
import xml.etree.ElementTree as ET

all_shaders = []
all_shaders_pointers = []

def Vector3ToElement(elementName: str, elementValue) -> ET.Element:
    vector3Element = ET.Element("Vector3")
    vector3Element.text = elementName
    value = ET.SubElement(vector3Element, "value")
    x = round(elementValue[0], 5)
    y = round(elementValue[1], 5)
    z = round(elementValue[2], 5)
    value.text = str(x) + " " + str(y) + " " + str(z)
    return vector3Element

def TextureToElement(elementName: str, elementValue) -> ET.Element:
    element = ET.Element("Texture")
    element.text = elementName
    value = ET.SubElement(element, "value")
    try:
        value.text = "resources\\" + elementValue.filepath.split("resources\\")[1]
    except IndexError:
        print("Warning! Texture \"" + elementValue.name + "\" is not saved inside resource folder! Full path to texture will be used!")
        value.text = elementValue.filepath
    return element

class color_only(bpy.types.PropertyGroup):
    color: bpy.props.FloatVectorProperty(name= "color", description= "Color of object", default=(0.0, 0.0, 0.0), min = 0.0, max = 1.0, precision = 3)
    
    @classmethod
    def draw(self, layout):
        data = bpy.context.object.SmoothieShaderSettings.color_only
        layout.prop(data, "color")
        return None
    
    @classmethod
    def getPropertyElement(self):
        data = bpy.context.object.SmoothieShaderSettings.color_only
        property_element = ET.Element("property")
        property_element.append(Vector3ToElement("color", data.color))
        return property_element

    @classmethod
    def getShaderLocation(self):
        out = ET.Element("fragmentShader")
        out.text = "shaders/pbr/color_only.glsl"
        return out

all_shaders.append(("color_only", "pbr/color_only.glsl", "color only shader"))
all_shaders_pointers.append(color_only)

class pbr(bpy.types.PropertyGroup):
    diffuse_texture: bpy.props.PointerProperty(type = bpy.types.Image, name = "Diffuse image", description = "Color of the surface of object. No lighting data should be in this image.")
    normal_map: bpy.props.PointerProperty(type = bpy.types.Image, name = "Normal image", description = "Normal map for surface to use.")
    metalic_texture: bpy.props.PointerProperty(type = bpy.types.Image, name = "Metalic image", description = "Black-white image. White - surface is metal, black - non metal surface")
    roughness_texture: bpy.props.PointerProperty(type = bpy.types.Image, name = "Roughness image", description = "Black-white image. While - surface is more rough, black - surface is smooth")
    
    
    @classmethod
    def draw(self, layout):
        data = bpy.context.object.SmoothieShaderSettings.pbr
        layout.prop(data, "diffuse_texture")
        layout.prop(data, "normal_map")
        layout.prop(data, "metalic_texture")
        layout.prop(data, "roughness_texture")
        return None
    
    @classmethod
    def getPropertyElement(self):
        data = bpy.context.object.SmoothieShaderSettings.pbr
        property_element = ET.Element("property")
        property_element.append(TextureToElement("diffuse", data.diffuse_texture))
        property_element.append(TextureToElement("normalMap", data.normal_map))
        property_element.append(TextureToElement("metalnessMap", data.metalic_texture))
        property_element.append(TextureToElement("roughnessMap", data.roughness_texture))
        
        return property_element

    @classmethod
    def getShaderLocation(self):
        out = ET.Element("fragmentShader")
        out.text = "shaders/pbr/pbr.glsl"
        return out


all_shaders.append(("pbr", "pbr/pbr.glsl", "Basic pbr shader for rendering"))
all_shaders_pointers.append(pbr)

class shader(bpy.types.PropertyGroup):
    selected_shader: bpy.props.StringProperty(name = "selected_shader", default = "color_only")
    
    pbr: bpy.props.PointerProperty(type = pbr, name = "pbr")
    color_only: bpy.props.PointerProperty(type = color_only, name = "color_only")

    @classmethod
    def getPropertyElement(self):
        name = str(bpy.context.object.SmoothieShaderSettings.selected_shader)
        property_element =  globals()[name].getPropertyElement()
        return property_element
    
    @classmethod
    def getShaderLocation(self):
        name = str(bpy.context.object.SmoothieShaderSettings.selected_shader)
        shaderName_element =  globals()[name].getShaderLocation()
        return shaderName_element
    
    @classmethod
    def draw(self, layout):
        name = str(bpy.context.object.SmoothieShaderSettings.selected_shader)
        globals()[name].draw(layout)
        return None
    