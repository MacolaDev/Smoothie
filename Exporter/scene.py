import bpy #type: ignore
import xml.etree.ElementTree as ET
import mathutils #type: ignore
from model import WriteModelFile, ModelFileDefinition

#Definition class for writing scene files
class SceneFileDefinition:
    filepath: str

    def __init__(self, filepath: str):
        self.filepath = filepath

def vector4ToString(vector) -> str:
    x = vector[0]
    y = vector[1]
    z = vector[2]
    w = vector[3]
    return f"{x:.4f} {y:.4f} {z:.4f} {w:.4f}"

def getLocalMatrix(model_node: ET.SubElement, object) -> ET.SubElement:
    
    #Convert scale position and rotation to XZY space and then create a local matrix with that data
    object_scale = mathutils.Vector((object.scale.x, object.scale.z, object.scale.y))
    object_location = mathutils.Vector((object.location.x, object.location.z, object.location.y))
    object_rotation_x = object.rotation_euler.x
    object_rotation_y = object.rotation_euler.y
    object_rotation_z = object.rotation_euler.z
    object_rotation = mathutils.Euler(mathutils.Vector((object_rotation_x, object_rotation_y, object_rotation_z)), 'XZY').to_matrix()
    new_matrix = mathutils.Matrix.LocRotScale(object_location, object_rotation, object_scale)
    
    row0 = ET.SubElement(model_node, "row0")
    row0.text = vector4ToString(new_matrix[0])
    
    row1 = ET.SubElement(model_node, "row1")
    row1.text = vector4ToString(new_matrix[1])
    
    row2 = ET.SubElement(model_node, "row2")
    row2.text = vector4ToString(new_matrix[2])
    return model_node

def getModelFilePath(sceneFilePath: str, objectName: str):

    base_file_path = sceneFilePath.split("\\")
    base_file_path.pop(-1)
    base_file_path.append(objectName + ".smodel")
    filepath = "\\".join(base_file_path)
    return filepath
        
def writeSceneFile(filedefiniton: SceneFileDefinition): 
    
    scene_objects = bpy.context.scene.objects

    scene_meshes = []
    scene_lights = []
    for object in scene_objects:
        if object.type == "MESH":
            scene_meshes.append(object)
        if object.type == "LIGHT":
            scene_lights.append(object)
    
    
    root = ET.Element("root")
    skybox = ET.SubElement(root, "skybox")

    #TODO: Change it to something better
    skybox.text = "resources/hdr_maps/glencairn_expressway_4k.hdr"
    
    models = ET.SubElement(root, "models")
    for model in scene_meshes:
        modelFileDefinition = ModelFileDefinition
        
        model.select_set(True)
        bpy.context.view_layer.objects.active = model

        #Write matrix
        model_node = ET.SubElement(models, "model")
        model_matrix = ET.SubElement(model_node, "matrix")
        getLocalMatrix(model_matrix, model)
        
        #Write path to model file
        model_file = ET.SubElement(model_node, "file")
        model_file.text = getModelFilePath(filedefiniton.filepath, model.name)

        #Write model file
        modelFileDefinition.saveTextures = True
        modelFileDefinition.filepath = model_file.text
        
        WriteModelFile(modelFileDefinition)
        model.select_set(False)
        
        
    #lights = ET.SubElement(root, "lights")

    ET.ElementTree(root).write(filedefiniton.filepath)


# test = SceneFileDefinition
# test.filepath = "D:\\SoftwareDev_Projects\\Smoothie\\Demo\\TestSmoothieExport\\TestScene\\untilted.sscene"
# writeSceneFile(test)


