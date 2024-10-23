import xml.etree.ElementTree as ET
import bpy #type: ignore

from geometry import WriteGeometryFile
from textures import bake_texture

#Definition file 
class ModelFileDefinition:
    filepath: str
    saveTextures: bool

    def __init__(self, filepath: str):
        self.filepath = filepath

def WriteModelFile(fileDefinition: ModelFileDefinition):
    current_object = bpy.context.object
    
    if(current_object == None):
        print("No object selected")
        return
    
    splitted_filepath = fileDefinition.filepath.split("\\")
    file_name = (splitted_filepath[-1]).split(".")[0]
    splitted_filepath.pop(-1)
    save_folder ="\\".join(splitted_filepath)

    DiffTexture = save_folder + "\\" + file_name + "_DIFF" + ".png"
    RoughnessTexture = save_folder + "\\" + file_name + "_ROUGH" + ".png"
    MetalTexture = save_folder + "\\" + file_name + "_METAL" + ".png"
    NormalTexture = save_folder + "\\" + file_name + "_NORM" + ".png"
    
    if(fileDefinition.saveTextures):

        #Albedo map
        bake_texture("DIFFUSE", DiffTexture)

        #Roughness map
        bake_texture("ROUGHNESS", RoughnessTexture)

        #Metalness map
        bake_texture("METALIC", MetalTexture)

        #Normal map
        bake_texture("NORMAL", NormalTexture)

    geometryFile = ""
    if(current_object.type == "MESH"):
        geometryFile = fileDefinition.filepath.split(".")[0] + ".geometry"
        WriteGeometryFile(geometryFile)

    
    root = ET.Element("model")
    geometry_file = ET.SubElement(root, "geometryFile")
    geometry_file.text = geometryFile
    
    fragmentShader = ET.SubElement(root, "fragmentShader")
    fragmentShader.text = "shaders/pbr/pbr.glsl"
    
    shader_property = ET.SubElement(root, "property")
    
    #TODO: Make this not so hard-coded
    diff_texture  = ET.SubElement(shader_property, "Texture")
    diff_texture.text = "diffuse"
    diff_texture_value = ET.SubElement(diff_texture, "value")

    normalMap_texture  = ET.SubElement(shader_property, "Texture")
    normalMap_texture.text = "normalMap"
    normalMap_texture_value = ET.SubElement(normalMap_texture, "value")

    metalnessMap_texture  = ET.SubElement(shader_property, "Texture")
    metalnessMap_texture.text = "metalnessMap"
    metalnessMap_texture_value = ET.SubElement(metalnessMap_texture, "value")

    roughnessMap_texture  = ET.SubElement(shader_property, "Texture")
    roughnessMap_texture.text = "roughnessMap"
    roughnessMap_texture_value = ET.SubElement(roughnessMap_texture, "value")
    
    if(fileDefinition.saveTextures):
        diff_texture_value.text = DiffTexture
        normalMap_texture_value.text = NormalTexture
        metalnessMap_texture_value.text = MetalTexture
        roughnessMap_texture_value.text = RoughnessTexture

    ET.ElementTree(root).write(fileDefinition.filepath)
    
