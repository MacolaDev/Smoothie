import bpy #type: ignore
from bpy_extras.io_utils import ExportHelper # type: ignore

from model import ModelFileDefinition, WriteModelFile
from scene import SceneFileDefinition, writeSceneFile

class SMOOTRHIE_OT_MeshExportButton(bpy.types.Operator, ExportHelper):
    """Button operator that prints to terminal"""
    bl_idname = "smoothie.mesh_export_button"
    bl_label = "Export Model"

    filename_ext = '.smodel' 
    
    filter_glob = bpy.props.StringProperty(default = '*.smodel', options = {'HIDDEN'})
    
    exportTextures: bpy.props.BoolProperty(name='Export PBR textures', description='Bakes PBR textures from active material and saves them.', default=True) # type: ignore
    
    def execute(self, context):

        modelFileDefinition = ModelFileDefinition(self.filepath)
        modelFileDefinition.saveTextures = self.exportTextures
        
        WriteModelFile(modelFileDefinition)
        
        return {'FINISHED'}
    
class SMOOTRHIE_OT_SceneExportButton(bpy.types.Operator, ExportHelper):
    """Button operator that prints to terminal"""
    bl_idname = "smoothie.scene_export_button"
    bl_label = "Export Scene"

    filename_ext = '.sscene' 
    
    filter_glob = bpy.props.StringProperty(default = '*.sscene', options = {'HIDDEN'})
    
    def execute(self, context):
        file = SceneFileDefinition(self.filepath)
        writeSceneFile(file)
        return {'FINISHED'}

class SmoothieExporter(bpy.types.Panel):    
    bl_label = "Mesh Export"
    bl_space_type = 'VIEW_3D'
    bl_region_type = 'UI'
    bl_category = "Smoothie Export"

    
    def draw(self, context):
        layout = self.layout
        layout.label(text =  "Export model file:")
        if(bpy.context.object == None):
            layout.enabled = False
        layout.operator("smoothie.mesh_export_button", text = "Export Model", icon = "CUBE")

class SmoothieSceneExporter(bpy.types.Panel):    
    bl_label = "Scene Export"
    bl_space_type = 'VIEW_3D'
    bl_region_type = 'UI'
    bl_category = "Smoothie Export"

    
    def draw(self, context):
        layout = self.layout
        layout.label(text =  "Export scene file:")
        layout.operator("smoothie.scene_export_button", text = "Export Scene", icon = "SCENE_DATA")


def register():
    bpy.utils.register_class(SMOOTRHIE_OT_SceneExportButton)
    bpy.utils.register_class(SMOOTRHIE_OT_MeshExportButton)
    bpy.utils.register_class(SmoothieExporter)
    bpy.utils.register_class(SmoothieSceneExporter)

def unregister():
    bpy.utils.unregister_class(SMOOTRHIE_OT_SceneExportButton)
    bpy.utils.unregister_class(SMOOTRHIE_OT_MeshExportButton)
    bpy.utils.unregister_class(SmoothieExporter)
    bpy.utils.unregister_class(SmoothieSceneExporter)

if __name__ == "__main__":
    register()