//Code inspired by Michael Weiss
//Professor University of UMass at Boston
//Adapted by Lindsey DiAntonio
//9/23/2013

#pragma once
#include "matrix.h"
#include <stdio.h>
#include <stdlib.h>

using namespace std;


matrix::matrix(int rows, int columns):row(rows), col(columns){
	createArray();
	for(int i=0; i<row; i++){
		for(int j=0; j<col; j++){
			ptr[i][j]= 0;
		}
	}
}


//DECONSTRUCTOR
matrix::~matrix(){
	for(int i=0; i<row; i++){
		delete[] ptr[i];
	}
	delete ptr;
}

//make a copy
matrix::matrix(const matrix& mat):row(mat.row), col(mat.col){
	createArray();
	for(int i=0; i<row; ++i){
		for(int j=0; j<col; ++j){
			ptr[i][j] = mat.ptr[i][j];
		}
	}
}

//Allows us to things like this:
// mat(0, 3)  = 1.3;
float& matrix::operator()(int i, int j){
	return ptr[i][j];
}

matrix& matrix::operator=(const matrix& input){
	if(row != input.row || col !=input.col){
		this->~matrix();
		row= input.row;
		col=input.col;
		createArray();
	}

	for(int i=0; i<row; i++){
		for(int j=0; j<col; j++){
			ptr[i][j] = input.ptr[i][j];
		}
	}
	return *this;
}

matrix matrix::operator+(const matrix& input){
	//ADDS input into current
	matrix temp = *this;
	temp+=input;
	return temp;
}


matrix& matrix::operator+=(const matrix& input){
	//ADDS input into current
	for(int i=0; i<row; i++){
		for(int j=0; j<col; j++){
			ptr[i][j] += input.ptr[i][j];
		}
	}
	return *this;
}

//HOW TO ACCESS to multiply self vs another matrix
matrix operator*(const matrix& left, const matrix& right){
	matrix result(left.row, right.col);
	for(int i=0; i<result.row; ++i){
		for(int j=0; j<result.col; ++j){
			for(int k=0; k<left.col; ++k){
				result.ptr[i][j] += (left.ptr[i][k] * right.ptr[k][j]);
			}
		}
	}
	return result;
}

matrix operator*(const matrix& left, float c){
	matrix temp(left.row, left.col);			//makes a temporary matrix of the same size as the current
	for(int i=0; i<temp.row; ++i){
		for(int j=0; j<temp.col; ++j){
			temp.ptr[i][j] = left.ptr[i][j] * c;
			printf("temp.ptr[%d][%d]= %f", i, j, temp.ptr[i][j]);
		}
	}
	return temp;
}

matrix operator*(float c, const matrix& right){
	return (right * c);
}


matrix matrix::transpose(){
	matrix temp(this->col, this->row);
	for(int i=0; i<row; i++){
		for(int j=0; j<col; j++){
			temp.ptr[j][i] = this->ptr[i][j];
		}
	}
	return temp;
}

void matrix::print(){
	printf("\nMatrix is of size %dx%d:\n", this->row, this->col);
	for(int i=0; i<row; ++i){
		for(int j=0; j<col; ++j){
			printf("%f ", ptr[i][j]);
		}
		printf("\n");
	}
}

float* matrix::getPtr()
{
	return &ptr[0][0];
}

GLfloat* matrix::getGLPtr(){
	GLfloat* ret = new GLfloat[this->row*this->col];
	for(int i=0; i<this->row; ++i){
		for(int j=0; j<col; ++j){
			ret[i*j] = (GLfloat)this->ptr[i][j];
		}
	}
	return ret;
}