import bpy #type: ignore
import SmoothieExporter

#SmoothieExporter uses data from this file to generete content 
class FileBuffer:
    geometryFilePath: str

    numberOfvertices: int
    vertexCoordinates: list
    vertexUVs: list
    vertexNormals: list
    vertexTangents: list
    vertexBitangents: list
    vertexColors: list

    numberOfIndices: int
    indices: list

#Buffer used to process data in SmoothieExporter
class ProcessBuffer:
    indexes: list
    data: list

def WriteGeometryFile(filepath: str):

    #Create duplicated object to work on
    bpy.ops.object.mode_set(mode='OBJECT')
    original_object = bpy.context.object
    original_object.select_set(True)
    bpy.context.view_layer.objects.active = original_object
    bpy.ops.object.duplicate()
    
    #Make sharp edges from UV islands and cut them 
    bpy.ops.object.editmode_toggle()
    bpy.ops.uv.select_all(action='SELECT')
    bpy.ops.uv.seams_from_islands(mark_seams=False, mark_sharp=True)
    bpy.ops.object.modifier_add(type='EDGE_SPLIT')
    bpy.context.object.modifiers["EdgeSplit"].use_edge_angle = False
    bpy.ops.object.mode_set(mode='OBJECT')
    bpy.ops.object.modifier_apply(modifier="EdgeSplit")

    #Triangulate mesh
    bpy.ops.object.modifier_add(type='TRIANGULATE')
    bpy.ops.object.modifier_apply(modifier="Triangulate")
    bpy.ops.object.mode_set(mode='OBJECT')

    mesh_data = bpy.context.object.data
    filebuffer = FileBuffer
    filebuffer.geometryFilePath = filepath
    
    #Get index data
    loops = [0] * len(mesh_data.loops)
    mesh_data.loops.foreach_get('vertex_index', loops)
    
    filebuffer.indices = loops.copy()
    filebuffer.indices.reverse()
    filebuffer.numberOfIndices = len(mesh_data.loops)

    #Get vertex coordinates
    filebuffer.numberOfvertices = len(mesh_data.vertices)
    mesh_coordinates = [0] * filebuffer.numberOfvertices * 3
    mesh_data.vertices.foreach_get('co', mesh_coordinates)
    filebuffer.vertexCoordinates = mesh_coordinates

    #UV data
    if len(mesh_data.uv_layers) != 0:
        uv_layer = mesh_data.uv_layers[0].data
        
        uv_data = [0] * len(mesh_data.loops) * 2
        uv_layer.foreach_get("uv", uv_data)
        
        uvsForBlender = ProcessBuffer
        uvsForBlender.indexes = loops
        uvsForBlender.data = uv_data
        filebuffer.vertexUVs = SmoothieExporter.calculate_uv_data(uvsForBlender)
    else:
        print("No UV layer on the mesh")
        return None


    #Normal, tangent, bitangent data
    mesh_data.calc_tangents(uvmap=mesh_data.uv_layers[0].name)
    
    tangents_list = [0] * len(mesh_data.loops) * 3
    mesh_data.loops.foreach_get("tangent", tangents_list)

    bitangents_list = [0] * len(mesh_data.loops) * 3
    mesh_data.loops.foreach_get("bitangent", bitangents_list)
    
    tangentsForExporter = ProcessBuffer
    tangentsForExporter.indexes = loops
    tangentsForExporter.data = tangents_list
    filebuffer.vertexTangents = SmoothieExporter.calculate_tangent_data(tangentsForExporter)
    
    bitangentsForExporter = ProcessBuffer
    bitangentsForExporter.indexes = loops
    bitangentsForExporter.data = bitangents_list
    filebuffer.vertexBitangents = SmoothieExporter.calculate_tangent_data(bitangentsForExporter)
    
    normals = [0] * 3 * filebuffer.numberOfvertices
    mesh_data.vertices.foreach_get('normal', normals)
    filebuffer.vertexNormals = normals
    
    #TODO: Add vector colors buffers

    SmoothieExporter.write_geometry_file(filebuffer)
    
    #Delete duplucated object
    bpy.ops.object.delete(use_global=False, confirm=True)
