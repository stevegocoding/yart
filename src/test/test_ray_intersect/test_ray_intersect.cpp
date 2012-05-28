#include "pch.h"
#include "transform.h"
#include "triangle_mesh_impl.h"
#include "scene_primitive.h"
#include "matte_material.h"

#include "cml/cml.h"
#include <iomanip>

#include "assimp/assimp.h"
#include "assimp/aiScene.h"
#include "assimp/aiPostProcess.h"

using namespace std;

void print_matrix(std::ostream& os, const cml::matrix44f& mat, int prec = 4, int width = 8)
{
	std::ios::fmtflags old_flags = os.flags(); 
	os.setf(ios::left, ios::adjustfield); 

	// row #1
	os << std::setprecision(prec) << std::setw(width) << std::setfill(' ') 
		<< mat(0, 0) << ' ' 
		<< std::setprecision(prec) << std::setw(width) << std::setfill(' ')
		<< mat(0, 1) << ' '
		<< std::setprecision(prec) << std::setw(width) << std::setfill(' ')
		<< mat(0, 2) << ' ' 
		<< std::setprecision(prec) << std::setw(width) << std::setfill(' ')
		<< mat(0, 3) << endl; 

	// row #2
	os << std::setprecision(prec) << std::setw(width) << std::setfill(' ') 
		<< mat(1, 0) << ' ' 
		<< std::setprecision(prec) << std::setw(width) << std::setfill(' ')
		<< mat(1, 1) << ' '
		<< std::setprecision(prec) << std::setw(width) << std::setfill(' ')
		<< mat(1, 2) << ' ' 
		<< std::setprecision(prec) << std::setw(width) << std::setfill(' ')
		<< mat(1, 3) << endl; 
	
	// row #3  
	os << std::setprecision(prec) << std::setw(width) << std::setfill(' ') 
		<< mat(2, 0) << ' ' 
		<< std::setprecision(prec) << std::setw(width) << std::setfill(' ')
		<< mat(2, 1) << ' '
		<< std::setprecision(prec) << std::setw(width) << std::setfill(' ')
		<< mat(2, 2) << ' ' 
		<< std::setprecision(prec) << std::setw(width) << std::setfill(' ')
		<< mat(2, 3) << endl; 

	// row #4
	os << std::setprecision(prec) << std::setw(width) << std::setfill(' ') 
		<< mat(3, 0) << ' ' 
		<< std::setprecision(prec) << std::setw(width) << std::setfill(' ')
		<< mat(3, 1) << ' '
		<< std::setprecision(prec) << std::setw(width) << std::setfill(' ')
		<< mat(3, 2) << ' ' 
		<< std::setprecision(prec) << std::setw(width) << std::setfill(' ')
		<< mat(3, 3) << endl; 

	os.setf(old_flags); 
}

void print_vector3f_col(std::ostream& os, const cml::vector3f& vec, int prec = 4, int width = 8)
{
	std::ios::fmtflags old_flags = os.flags(); 
	os.setf(ios::left, ios::adjustfield); 

	os << std::setprecision(prec) << std::setw(width) << std::setfill(' ') 
		<< vec[0] << ' ' << std::endl 
		<< std::setprecision(prec) << std::setw(width) << std::setfill(' ')
		<< vec[1] << ' ' << std::endl 
		<< std::setprecision(prec) << std::setw(width) << std::setfill(' ')
		<< vec[2] << ' ' << std::endl; 

	os.setf(old_flags); 
}

void print_vector4f_col(std::ostream& os, const cml::vector4f& vec, int prec = 4, int width = 8)
{
	std::ios::fmtflags old_flags = os.flags(); 
	os.setf(ios::left, ios::adjustfield); 

	os << std::setprecision(prec) << std::setw(width) << std::setfill(' ') 
		<< vec[0] << ' ' << std::endl 
		<< std::setprecision(prec) << std::setw(width) << std::setfill(' ')
		<< vec[1] << ' ' << std::endl 
		<< std::setprecision(prec) << std::setw(width) << std::setfill(' ')
		<< vec[2] << ' ' << std::endl 
		<< std::setprecision(prec) << std::setw(width) << std::setfill(' ')
		<< vec[3] << endl; 

	os.setf(old_flags); 
}

void print_vector3f_row(std::ostream& os, const cml::vector3f& vec, int prec = 4, int width = 8)
{
	std::ios::fmtflags old_flags = os.flags(); 
	os.setf(ios::left, ios::adjustfield); 

	os << std::setprecision(prec) << std::setw(width) << std::setfill(' ') 
		<< vec[0] << ' ' 
		<< std::setprecision(prec) << std::setw(width) << std::setfill(' ')
		<< vec[1] << ' ' 
		<< std::setprecision(prec) << std::setw(width) << std::setfill(' ')
		<< vec[2] << ' ' << std::endl; 

	os.setf(old_flags); 

}

void print_vector3d_row(std::ostream& os, const cml::vector3d& vec, int prec = 4, int width = 8)
{
	std::ios::fmtflags old_flags = os.flags(); 
	os.setf(ios::left, ios::adjustfield); 

	os << std::setprecision(prec) << std::setw(width) << std::setfill(' ') 
		<< vec[0] << ' ' 
		<< std::setprecision(prec) << std::setw(width) << std::setfill(' ')
		<< vec[1] << ' ' 
		<< std::setprecision(prec) << std::setw(width) << std::setfill(' ')
		<< vec[2] << ' ' << std::endl; 

	os.setf(old_flags); 
}

void print_aiscene_info(std::ostream& os, const aiScene *scene)
{
	assert(scene); 

	std::cout << 
		"Mesh File Information: "<< std::endl;
	std::cout << "Meshes: " << scene->mNumMeshes << std::endl; 
	
	for (unsigned int i = 0; i < scene->mNumMeshes; ++i)
	{
		std::cout << "Mesh #" << i << std::endl;
		std::cout << "Vertices: " << scene->mMeshes[i]->mNumVertices << std::endl; 
		std::cout << "Faces: " << scene->mMeshes[i]->mNumFaces << std::endl; 
		std::cout << "---------------------------------------------------" << std::endl;

		std::cout << "Vertices:" << std::endl;
		for (unsigned int v = 0; v < scene->mMeshes[i]->mNumVertices; ++v)
		{
			print_vector3f_row(cout, aivec3_to_cmlvec3(scene->mMeshes[i]->mVertices[v]));
		}

		std::cout << "Triangles:" << std::endl;
		
		for (unsigned int f = 0; f < scene->mMeshes[i]->mNumFaces; ++f)
		{
			unsigned int i0 = scene->mMeshes[i]->mFaces[f].mIndices[0]; 
			unsigned int i1 = scene->mMeshes[i]->mFaces[f].mIndices[1]; 
			unsigned int i2 = scene->mMeshes[i]->mFaces[f].mIndices[2]; 
			
			std::cout << "Triangle #" << f;
			std::cout << " | "; 
			print_vector3d_row(cout, vector3d(i0, i1, i2), 2, 2);  

			std::cout << "v1: "; 
			print_vector3d_row(cout, aivec3_to_cmlvec3(scene->mMeshes[i]->mVertices[i0]));
			std::cout << "v2: "; 
			print_vector3d_row(cout, aivec3_to_cmlvec3(scene->mMeshes[i]->mVertices[i1]));
			std::cout << "v3: "; 
			print_vector3d_row(cout, aivec3_to_cmlvec3(scene->mMeshes[i]->mVertices[i2]));
			std::cout << std::endl; 
		}
	}
}
	
int main(int argc, char **argv)
{
	/*
	const aiScene *scene = aiImportFile("../data/models/cube.ply", aiProcess_Triangulate | aiProcess_MakeLeftHanded); 

	aiLogStream stream; 
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_STDOUT, NULL);
	aiAttachLogStream(&stream);

	// Create triangle mesh
	triangle_mesh_impl_ptr mesh_impl = triangle_mesh_impl_ptr(new c_assimp_mesh_impl(scene->mMeshes[0])); 
	shape_ptr mesh = triangle_mesh_ptr(new c_triangle_mesh(mesh_impl)); 
	
	// create the scene object 
	c_transform o2w = make_translate(vector3f(0.0f, 0.0f,1.0f)); 
	c_transform w2o = inverse_transform(o2w); 
	material_ptr mat = make_matte_material(c_spectrum(0.8f, 0.8f, 0.8f), 0.0f); 
	scene_object_ptr mesh_obj = make_simple_scene_obj(w2o, o2w, mesh, mat); 

	print_aiscene_info(cout, scene);
	
	c_ray ray; 
	ray.o = vector3f(0, 0, 0); 
	ray.d = normalize(vector3f(0.8, -0.8, 0.1)); 
	
	triangle_mesh_ptr tri_mesh = boost::dynamic_pointer_cast<c_triangle_mesh>(mesh_obj->get_geometry_shape()); 
	assert(tri_mesh); 
	
	triangle_face_ptr tri = tri_mesh->get_triangle_face(3); 
	
	float t_hit = 0.0f;
	float ray_eps = 0.0f; 
	bool is_hit = tri->intersects(ray, &t_hit, &ray_eps, diff_geom_ptr());

	if (is_hit) 
	{
		vector3f hit_pt = ray.o + ray.d * t_hit;
		std::cout << "Hit point: " << std::endl; 

		print_vector3f_row(cout, hit_pt); 
	} 
	else 
	{
		std::cout << "Missed!" << endl; 
	}
	
	aiReleaseImport(scene);
	aiDetachAllLogStreams();
	*/ 
	
	return 0;
}