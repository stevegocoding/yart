
#include "boost/make_shared.hpp"

#include "glut/glut.h"
#include "cml/cml.h"
#include <iomanip>
#include <algorithm>
#include "../../yart_core/ray.h"
#include "../../yart_core/transform.h"
#include "../../yart_core/triangle_mesh_impl.h"
#include "../../yart_core/scene_obj.h"

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

c_ray ray; 
triangle_face_ptr tri; 
triangle_mesh_ptr mesh; 
const aiScene *scene = NULL;
uint32_t face_idx = 3; 
float view_angle_xz = 0.0f;
float view_angle_yz = 0.0f;

void setup()
{
	scene = aiImportFile("../data/models/cube.ply", aiProcess_Triangulate | aiProcess_MakeLeftHanded); 

	aiLogStream stream; 
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_STDOUT, NULL);
	aiAttachLogStream(&stream);

	// Create triangle mesh
	triangle_mesh_impl_ptr mesh_impl = triangle_mesh_impl_ptr(new c_assimp_mesh_impl(scene->mMeshes[0])); 
	mesh = triangle_mesh_ptr(new c_triangle_mesh(mesh_impl)); 

	// create the scene object 
	c_transform o2w = make_translate(vector3f(0.0f, 0.0f,5.0f)) * make_scale(3.0f, 3.0f, 1.0f); 
	c_transform w2o = inverse_transform(o2w); 
	scene_object_ptr mesh_obj = make_simple_scene_obj(w2o, o2w, mesh); 

	print_aiscene_info(cout, scene);

	ray.o = vector3f(0, 0, 0); 
	ray.d = normalize(vector3f(0.3f, 0.15f, 0.95f)); 

	triangle_mesh_ptr tri_mesh = boost::dynamic_pointer_cast<c_triangle_mesh>(mesh_obj->get_geometry_shape()); 
	assert(tri_mesh); 

	tri = tri_mesh->get_triangle_face(face_idx); 
}

void cleanup()
{
	aiReleaseImport(scene);
	aiDetachAllLogStreams();
}

void setup_view()
{
	glMatrixMode(GL_PROJECTION);   //changes the current matrix to the projection matrix

	//sets up the projection matrix for a perspective transform
	gluPerspective(
		45,    //view angle
		1.0,    //aspect ratio
		0.5f,   //near clip
		200.0); //far clip

	float r = 50.0f;

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	float eyex = r*cosf(rad(view_angle_xz));
	float eyey = 0.0f; 
	float eyez = r*sinf(rad(view_angle_xz)); 
	gluLookAt(0,0,20
		,0,0,-1,0,1,0);
 
	cout << eyex << ' ' << eyey << ' ' << eyez << std::endl; 
}

void draw_ray()
{
	glBegin(GL_LINES);
	glColor3f(1.0f, 0.0f, 0.0f); 

	glVertex3f(ray.o[x], ray.o[y], -ray.o[z]);
	glVertex3f(ray.d[x]*100, ray.d[y]*100, -ray.d[z]*100);

	glEnd(); 
}

void draw_triangle()
{
	glBegin(GL_TRIANGLES);
	
	triangle_face face = mesh->get_face(face_idx);

	point3f p1 = mesh->get_vert(face[0]);
	point3f p2 = mesh->get_vert(face[1]);
	point3f p3 = mesh->get_vert(face[2]);

	glColor3f(1.0f, 1.0f, 1.0f); 
	glVertex3f(p1[x], p1[y], -p1[z]);
	glColor3f(1.0f, 1.0f, 1.0f); 
	glVertex3f(p2[x], p2[y], -p2[z]);
	glColor3f(1.0f, 1.0f, 1.0f); 
	glVertex3f(p3[x], p3[y], -p3[z]);

	//glColor3f(1.0f, 1.0f, 1.0f); 
	//glVertex3f(1, 1, 0);
	//glColor3f(1.0f, 1.0f, 1.0f); 
	//glVertex3f(-2, 2, 0);
	//glColor3f(1.0f, 1.0f, 1.0f); 
	//glVertex3f(3, 4, 0);

	glEnd(); 
}

void render_scene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//glPushMatrix();       //saves the current matrix on the top of the matrix stack
	//glTranslatef(0,0,-5); //translates the current matrix 0 in x, 0 in y and -100 in z

	glDisable(GL_CULL_FACE);
	// glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );


	
	glPushMatrix(); 
	//glTranslatef(0,0,-5); //translates the current matrix 0 in x, 0 in y and -100 in z
	glRotatef(view_angle_xz, 0, 1.0f, 0); 
	glRotatef(view_angle_yz, 1.0f, 0, 0); 

	draw_ray(); 
	draw_triangle(); 

	

	float t_hit = 0.0f;
	float ray_eps = 0.0f; 
	bool is_hit = tri->intersects(ray, &t_hit, &ray_eps, diff_geom_ptr()); 

	if (is_hit) 
	{
		vector3f hit_pt = ray.o + ray.d * t_hit;
		std::cout << "Hit point: " << std::endl; 
		
		print_vector3f_row(cout, hit_pt); 

		glPointSize(2.0f); 
		glBegin(GL_POINTS); 
		glColor3f(0, 0, 1);
		glVertex3f(hit_pt[x], hit_pt[y], -hit_pt[z]); 
		glEnd(); 
	} 
	else 
	{
		std::cout << "Missed!" << endl; 
	}

	glPopMatrix(); 
	glutSwapBuffers();      //swaps the front and back buffers
	
}

void on_key(unsigned char key, int x, int y)
{
	if (key == 'a')
	{
		view_angle_xz -= 5.0f;
		view_angle_xz = view_angle_xz < 0 ? 360 : view_angle_xz; 
	}
	else if (key == 'd')
	{
		view_angle_xz += 5.0f; 
		view_angle_xz = view_angle_xz > 360 ? 0 : view_angle_xz;  
	}

	else if (key == 'w')
	{
		view_angle_yz += 5.0f; 
		view_angle_yz = view_angle_yz > 360 ? 0 : view_angle_yz;  
	}

	else if (key == 's')
	{
		view_angle_yz -= 5.0f;
		view_angle_yz = view_angle_yz < 0 ? 360 : view_angle_yz; 
	}
	
	glutPostRedisplay(); 
}


int main(int argc, char** argv)
{
	glutInit(&argc, argv); 
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow("Test Ray Intersection");
	glutDisplayFunc(render_scene);    
	glutKeyboardFunc(on_key); 
	
	setup();

	setup_view(); 

	glutMainLoop(); 
	
	return 0; 
}