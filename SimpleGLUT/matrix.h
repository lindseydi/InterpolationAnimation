#ifndef MATRIX_H
#define MATRIX_H
#pragma once
#include <stdlib.h>
#include <GL/glut.h>
class matrix{
public:
	matrix(int rows, int columns);

	~matrix();
	matrix(const matrix& mat);
	//Operators
	float &operator()(int x, int y);
	matrix& operator=(const matrix& input);
	matrix operator+(const matrix& input);
	matrix& operator+=(const matrix& input);
	friend matrix operator*(const matrix& left, const matrix& right);
	friend matrix operator*(const matrix& left, float c);
	friend matrix operator*(float c, const matrix& right);

	//other funcs
	matrix transpose();
	void print();
	float* getPtr();
	GLfloat* getGLPtr();

private:
	int row, col;
	float **ptr;

	void createArray(){
	this->ptr = new float*[row];
		for(int i=0; i<row; i++){
			ptr[i] = new float[col];
		}
	}
public :
	void ToArray( float *v ) {
		for ( int i = 0; i < row; i++ ) {
			for ( int j = 0; j < col; j++ ) {
				v[ i * col + j ] = ptr[ i ][ j ];
			}
		}
	}
};



#endif // MATRIX_H ///:~