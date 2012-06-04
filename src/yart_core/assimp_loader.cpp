#include "assimp_loader.h" 
#include "triangle_mesh.h"
#include "scene.h"
#include "light.h"

uint32_t assimp_load_meshes(const aiScene *scene, std::vector<triangle_mesh_ptr>& meshes)
{
	for (uint32_t i = 0; i < scene->mNumMeshes; ++i)
	{
		vertices_array verts; 
		face_indices_array faces; 
		normals_array normals;
		tangents_array tangents; 
		uvs_array uvs;

		uint32_t num_verts = scene->mMeshes[i]->mNumVertices;
		uint32_t num_faces = scene->mMeshes[i]->mNumFaces; 

		bool has_normals = scene->mMeshes[i]->HasNormals(); 
		bool has_tangents = scene->mMeshes[i]->HasTangentsAndBitangents(); 
		bool has_uvs = scene->mMeshes[i]->HasTextureCoords(0); 
		
		// Vertex Data
		for (uint32_t v = 0; v < scene->mMeshes[i]->mNumVertices; ++v)
		{
			point3f vert(scene->mMeshes[i]->mVertices[v].x, scene->mMeshes[i]->mVertices[v].y, scene->mMeshes[i]->mVertices[v].z);
			verts.push_back(vert);

			if (has_normals)
			{
				vector3f normal(scene->mMeshes[i]->mNormals[v].x, scene->mMeshes[i]->mNormals[v].y, scene->mMeshes[i]->mNormals[v].z);
				normals.push_back(normal); 
			}
			if (has_tangents)
			{
				vector3f tangent(scene->mMeshes[i]->mTangents[v].x, scene->mMeshes[i]->mTangents[v].y, scene->mMeshes[i]->mTangents[v].z);
				tangents.push_back(tangent); 
			}
			if (has_uvs)
			{
				uv _uv(scene->mMeshes[i]->mTextureCoords[0][v].x, scene->mMeshes[i]->mTextureCoords[0][v].y);
				uvs.push_back(_uv);
			}
		}
		
		// Triangle Face Data
		for (uint32_t f = 0; f < scene->mMeshes[i]->mNumFaces; ++f)
		{
			tri_indices face(scene->mMeshes[i]->mFaces[f].mIndices[0], scene->mMeshes[i]->mFaces[f].mIndices[1], scene->mMeshes[i]->mFaces[f].mIndices[2]);
			faces.push_back(face);
		} 
		
		triangle_mesh_ptr mesh = triangle_mesh_ptr(new c_triangle_mesh(verts, normals, tangents, uvs, faces));
		meshes.push_back(mesh);
	}

	return scene->mNumMeshes;
}

/*
scene_ptr load_scene_from_file(const std::string& file_name)
{
	Assimp::Importer importer;
	
	// Remove lines and points
	importer.SetPropertyInteger(AI_CONFIG_PP_SBP_REMOVE, aiPrimitiveType_LINE|aiPrimitiveType_POINT);
	
	const aiScene *scene = importer.ReadFile(file_name, 
		//aiProcess_GenNormals |		// Generate normals
		aiProcess_Triangulate |			// Only triangles
		aiProcess_MakeLeftHanded
		// aiProcess_PreTransformVertices |
		);
	
	assert(scene);
	
	std::vector<triangle_mesh_ptr> meshes; 
	std::vector<light_ptr> lights; 
	assimp_load_meshes(scene, meshes);
	// load_lights(scene, lights);

	aiReleaseImport(scene);

	scene_ptr s = scene_ptr(new c_scene(meshes, lights));
	return s; 
}
*/


void assimp_import_scene(const std::string& file_name, const aiScene **pp_scene)
{
	assert(*pp_scene == NULL); 
	
	/*
	Assimp::Importer importer;

	// Remove lines and points
	importer.SetPropertyInteger(AI_CONFIG_PP_SBP_REMOVE, aiPrimitiveType_LINE|aiPrimitiveType_POINT);

	const aiScene *scene = importer.ReadFile(file_name, 
		//aiProcess_GenNormals |		// Generate normals
		aiProcess_Triangulate |			// Only triangles
		aiProcess_MakeLeftHanded
		// aiProcess_PreTransformVertices |
		);
	
	assert(scene);
	*/

	const aiScene *scene = aiImportFile(file_name.c_str(), aiProcess_Triangulate | aiProcess_MakeLeftHanded); 
	assert(scene);

	*pp_scene = scene;  
}

void assimp_release_scene(const aiScene *scene)
{
	assert(scene);

	aiReleaseImport(scene); 
}