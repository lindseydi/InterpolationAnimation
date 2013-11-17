#pragma once


#include <stdlib.h>
#include <stdio.h>
#include <glut.h>
#include <math.h>
#include <Windows.h>
#include <iostream>
#include <istream>
#include <string.h>
#include <fstream>
#include <string>
#include <vector>
#include "vertex3.h"
#include "polygon.h"
#include "matrix.h"
using namespace std;

#define PI 3.14159265

class ModelView
{
	vector <vertex3> vertices;
	vector <polygon> edges;
	vector <vertex3> normals;
	bool chess;

public:
	ModelView(void){
		vertices.clear();
		edges.clear();
		normals.clear();
	}

	ModelView(string filename, bool chess){
		this->chess = chess;
		read_file(filename);
	}

	ModelView(int vert_count, int edge_count, bool chessin){
		vertices.resize(vert_count);
		normals.resize(vert_count);
		edges.resize(edge_count);
		chess = chessin;
	}

	vertex3 calcTriangleNorm(vertex3 vec0, vertex3 vec1, vertex3 vec2)
{
	vertex3 edge1 = vec1 - vec0;
	edge1.normalize();
	vertex3 edge2 = vec2 - vec0;
	edge2.normalize();
	vertex3 normal = edge1.crossProduct(edge2);
	normal.normalize();
	return normal;
}

void generatenormals(void){
	int i;
	for(i=0; i<vertices.size(); i++){
		normals[i].zero();
	}
	
	for(i=0; i<edges.size(); i++){
		int type = edges[i].getType();
		int firstv = edges[i].geta();
		int secondv = edges[i].getb();
		int thirdv = edges[i].getc();
		int fourthv = edges[i].getd();
		vertex3 normal = calcTriangleNorm(vertices[firstv], vertices[secondv], vertices[thirdv]);
			normals[firstv] = normals[firstv] + normal;
			normals[secondv] = normals[secondv] + normal;
			normals[thirdv] = normals[thirdv] + normal;
			normals[fourthv] = normals[fourthv] + normal;
	}
	for(i=0; i<vertices.size(); i++){
		normals[i].normalize();
	}
}
	void read_file(string filename)
{
	int vert_count;
	int i;
	ifstream inFile;  
	inFile.open(filename);
	if (!inFile) {
		cerr << "Can't open input file "  << endl;
		//exit(1);
	}
	string str;
	int edge_count;
	inFile >> str;
	inFile >> str;
	vert_count = atoi(str.c_str());
	//vertex3 *vertices = ((vertex3*)  malloc(vert_count*sizeof(vertex3)));
	inFile >> str;
	edge_count = atoi(str.c_str());
	float x, y, z;


	vertices.resize(vert_count);
	normals.resize(vert_count);
	for(i=0; i<vert_count; i++){
		inFile >> str;
		x = (GLfloat) atof( str.c_str());
		inFile >> str;
		y = (GLfloat) atof(str.c_str());
		inFile >> str;
		z = (GLfloat) atof(str.c_str());
		vertex3 vertex(x, y, z);
		vertices[ i ] = vertex;
	}
	edges.resize(edge_count);
	for(i=0; i<edge_count; i++){
		inFile >> str;
		int numEdges = atoi(str.c_str());
		//triangle
		if(numEdges == 3){
			inFile >> str;
			int a = atoi(str.c_str()) - 1;
			inFile >> str;
			int b = atoi(str.c_str()) - 1;
			inFile >> str;
			int c = atoi(str.c_str()) -1;

			//Put this information into the global vector connect
			polygon poly(a, b, c);
			edges[i] = poly;
		}
		//rectangle
		if(numEdges == 4){
			 inFile >> str;
 			int a = atoi(str.c_str()) - 1;
			inFile >> str;
			int b = atoi(str.c_str()) - 1;
			inFile >> str;
			int c = atoi(str.c_str()) -1;
			inFile >> str;
			int d = atoi(str.c_str()) -1;

			polygon poly(a, b, c, d);
			edges[i] = poly;
		}
	}

	generatenormals();
}

void translate_object(float x, float y, float z){
	matrix translate(4, 4);
	for(int i=0; i<4; i++){
		translate(i, i) = 1.0;
	}
	translate(0, 4) = x;
	translate(1, 4) = y;
	translate(2, 4) = z;
	glMultMatrixf(translate.getGLPtr());
}

void scale_object(float x, float y, float z){
	matrix scale(4, 4);
	scale(0, 0) = x;
	scale(1, 1) = y;
	scale(2, 2) = z;
	scale(3,3) = 1.0;
	glMultMatrixf(scale.getGLPtr());
}

void rotate_object(float pitch, float yaw, float roll){
	matrix rotatePitch(4, 4);
	matrix rotateYaw(4, 4);
	matrix rotateRoll(4,4);
	matrix product(4,4);

	rotatePitch(0, 0) = 1;
	rotatePitch(1, 1) = cos((PI/180.0)*pitch);
	rotatePitch(1, 2) = -sin((PI/180.0)*pitch);
	rotatePitch(2, 1) = sin((PI/180.0)*pitch);
	rotatePitch(2, 2) = cos((PI/180.0)*pitch);
	rotatePitch(3, 3) = 1.0;


	rotateYaw(0, 0) = cos((PI/180.0)*yaw);
	rotateYaw(0, 2) = sin((PI/180.0)*yaw);
	rotateYaw(1, 1) = 1.0;
	rotateYaw(2, 0) = -sin((PI/180.0)*yaw);
	rotateYaw(2, 0) = sin((PI/180.0)*yaw);
	rotateYaw(2, 2) = cos((PI/180.0)*yaw);
	rotateYaw(3, 3) = 1.0;

	rotateRoll(0, 0) = cos((PI/180.0)*roll);
	rotateRoll(0, 1) = -sin((PI/180.0)*roll);
	rotateRoll(1, 0) = sin((PI/180.0)*roll);
	rotateRoll(1, 1) = cos((PI/180.0)*roll);
	rotateRoll(2, 2) = 1.0;
	rotateRoll(3, 3) = 1.0;

	product = rotatePitch * rotateYaw * rotateRoll;
	glMultMatrixf(product.getGLPtr());
}

void draw_object(float obj_colorR, float obj_colorG, float obj_colorB, float transX, float transY, float transZ, int m, int m2)
{
	int i;
	int firstv;
	int secondv;
	int thirdv;
	int fourthv;
	int typepoly;
	GLuint m_wall_texture_id = m;
	GLuint m_wall_texture_id2 = m2;

	glEnable(GL_COLOR_MATERIAL); //Enable color
	glEnable( GL_TEXTURE_2D );
	glColor3f(obj_colorR, obj_colorG, obj_colorB);
	if(chess){
		//translate_object(transX, transY, transZ);
		//scale_object(.1f, .1f, .1f);
		//rotate_object(-90.0, 0.0, 0.0);
		//Good Translation for Chess pieces
	    //glTranslatef(transX, transY, transZ);
		//glRotatef(90.0, -1.0, 0.0, 0.0);
		//glScalef(0.1f, 0.1f, 0.1f);
		
		//Instead of these push transformation matrix!
		//glLoadMatrix*( elements16 );

	}
 for (i=0 ; i < edges.size(); i++){
	 typepoly = edges[i].getType();
	 if(typepoly == 3){
		firstv = edges[i].geta();
		secondv = edges[i].getb();
		thirdv = edges[i].getc();
	 	glBegin (GL_TRIANGLES);
			glNormal3fv(normals[firstv].getPointer());
			glVertex3fv (vertices[firstv].getPointer());
			glNormal3fv(normals[secondv].getPointer());
			glVertex3fv (vertices[secondv].getPointer());
			glNormal3fv(normals[thirdv].getPointer());
			glVertex3fv (vertices[thirdv].getPointer());
			//cout << "Should be drawing triangles " << i << endl;
			//glFlush();
	 }
	 if(typepoly == 4){
	 	glBegin (GL_QUADS);
		firstv = edges[i].geta();
		secondv = edges[i].getb();
		thirdv = edges[i].getc();
		fourthv = edges[i].getd();
		if(i == 6){
			 //Enable texture
			glBindTexture( GL_TEXTURE_2D, m2);
		}
		 if(i>11){
			 glGenTextures(1, &m_wall_texture_id);
			 glBindTexture( GL_TEXTURE_2D, m_wall_texture_id);
		 }
			glColor3f(obj_colorR, obj_colorG, obj_colorB);
			glNormal3fv(normals[firstv].getPointer());
			glTexCoord2f( 0.0f, 0.0f );
			glVertex3fv (vertices[firstv].getPointer());
			glNormal3fv(normals[secondv].getPointer());
			glTexCoord2f(1.0f, 0.0f);
			glVertex3fv (vertices[secondv].getPointer());
			glNormal3fv(normals[thirdv].getPointer());
			glTexCoord2f(1.0f, 1.0f);
			glVertex3fv (vertices[thirdv].getPointer());
			glNormal3fv(normals[fourthv].getPointer());
			glTexCoord2f(0.0f, 1.0f);
			glVertex3fv (vertices[fourthv].getPointer());
			//cout << "Should be drawing triangles " << i << endl;
			//glFlush();
	 }
     glEnd();
	}
	glDisable( GL_TEXTURE_2D );

	//To draw vertex normals
	//For debugging purposes
    #if 0
	glBegin (GL_LINES);
	for (i=0 ; i < vertices.size(); i++){
		glColor3f( 1.0, 0.0, 1.0 );
		glVertex3fv (vertices[i].getPointer());
		glVertex3fv( (vertices[i] + normals[i] * .5f ).getPointer());
	}
    glEnd();
    #endif
	
	glColor3f(0.0, 0.0, 1.0);
	/*
	if(chess){
	//Rotate back to normal and draw a teapot
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glTranslatef(-transX, -transY, -transZ);
	}
	*/
	//glTranslatef(0,-8,-4.5);
    //glutSolidTeapot(2);
}		
};

