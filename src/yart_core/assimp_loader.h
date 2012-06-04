#pragma once 

#include "prerequisites.h"

#include "assimp/assimp.hpp"
#include "assimp/assimp.h"
#include "assimp/aiPostProcess.h"
#include "assimp/aiScene.h"
#include "assimp/aiMesh.h"

void assimp_import_scene(const std::string& file_name, const aiScene **pp_scene);
void assimp_release_scene(const aiScene *scene); 
uint32_t assimp_load_meshes(const aiScene *scene, std::vector<triangle_mesh_ptr>& meshes); 
