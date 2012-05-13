#include "../../yart_core/pch.h"
#include "cml/cml.h"
#include <iomanip>

using namespace std;

void print_matrix(const cml::matrix44f& mat)
{
	ios::fmtflags old_flags = cout.flags(); 
	cout.setf(ios::left, ios::adjustfield); 

	cout << std::setprecision(4) << std::setw(8) << std::setfill(' ') 
		<< mat(0, 0) << ' ' 
		<< std::setprecision(4) << std::setw(8) << std::setfill(' ')
		<< mat(0, 1) << ' '
		<< std::setprecision(4) << std::setw(8) << std::setfill(' ')
		<< mat(0, 2) << ' ' 
		<< std::setprecision(4) << std::setw(8) << std::setfill(' ')
		<< mat(0, 3) << endl; 

	cout << std::setprecision(4) << std::setw(8) << std::setfill(' ') 
		<< mat(0, 0) << ' ' 
		<< std::setprecision(4) << std::setw(8) << std::setfill(' ')
		<< mat(1, 1) << ' '
		<< std::setprecision(4) << std::setw(8) << std::setfill(' ')
		<< mat(2, 2) << ' ' 
		<< std::setprecision(4) << std::setw(8) << std::setfill(' ')
		<< mat(3, 3) << endl; 
	
	cout << std::setprecision(4) << std::setw(8) << std::setfill(' ') 
		<< mat(2, 0) << ' ' 
		<< std::setprecision(4) << std::setw(8) << std::setfill(' ')
		<< mat(2, 1) << ' '
		<< std::setprecision(4) << std::setw(8) << std::setfill(' ')
		<< mat(2, 2) << ' ' 
		<< std::setprecision(4) << std::setw(8) << std::setfill(' ')
		<< mat(2, 3) << endl; 

	cout << std::setprecision(4) << std::setw(8) << std::setfill(' ') 
		<< mat(3, 0) << ' ' 
		<< std::setprecision(4) << std::setw(8) << std::setfill(' ')
		<< mat(3, 1) << ' '
		<< std::setprecision(4) << std::setw(8) << std::setfill(' ')
		<< mat(3, 2) << ' ' 
		<< std::setprecision(4) << std::setw(8) << std::setfill(' ')
		<< mat(3, 3) << endl; 

	cout.setf(old_flags); 
}
	
int main(int argc, char **argv)
{
	cml::matrix44f m;
	cml::matrix_scale(m, cml::vector3f(0.555555f, -4500.0f, -2.333333333333f)); 

	print_matrix(m);
	
	return 0;
}

