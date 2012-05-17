#include "../../yart_core/pch.h"
#include "../../yart_core/transform.h"
#include "../../yart_core/triangle_mesh_impl.h"

#include "cml/cml.h"
#include <iomanip>

#include "assimp/assimp.h"
#include "assimp/aiScene.h"

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
	}
}
	
int main(int argc, char **argv)
{
	const aiScene *scene = aiImportFile("../data/models/cube.ply", 0); 
	
	print_aiscene_info(cout, scene);

	// Create triangle mesh
	triangle_mesh_impl_ptr mesh_impl = triangle_mesh_impl_ptr(new c_assimp_mesh_impl(scene->mMeshes[0])); 
	
	aiReleaseImport(scene);
	
	return 0;
}