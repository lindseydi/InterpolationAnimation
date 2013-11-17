#pragma once
#include "vertex3.h"
#include "quaternion.h"
#include <stdio.h>
class KeyFrame
{
public:
	vertex3 position;
	quaternion orient;
	float time; //necessary?

	float *getPointer( void ) {
		return &position.x;
	}

	float at( int index ) {
		return (&position.x)[ index ];
	}

	KeyFrame(){
		this->position =  vertex3(0.0, 0.0, 0.0);
		this->orient = quaternion(1.0, 0.0, 0.0);
		this->time = 0.0;
	}

	KeyFrame(vertex3 pos, quaternion orient, float time){
		this->position = pos;
		this->orient = orient;
		this->time = time;
	}
	//Euler Angles
	KeyFrame(float posX, float posY, float posZ, float orientX, float orientY, float orientZ, float time){
		this->position = vertex3(posX, posY, posZ);
		this->orient = quaternion(orientX, orientY, orientZ);
		this->time = time;
	}
	//Quaternions
	KeyFrame(float posX, float posY, float posZ, float orientX, float orientY, float orientZ, float orientW, float time){
		this->position = vertex3(posX, posY, posZ);
		this->orient = quaternion(orientX, orientY, orientZ, orientW);
		this->time = time;
	}
	~KeyFrame(void){
		//something should go here
	}

	GLfloat getPosX(){
		return this->position.getx();
	}

	GLfloat getPosY(){
		return this->position.gety();
	}

	GLfloat getPosZ(){
		return this->position.getz();
	}

	GLfloat getOrientX(){
		return (GLfloat)this->orient.X;
	}

	GLfloat getOrientY(){
		return (GLfloat)this->orient.Y;
	}

	GLfloat getOrientZ(){
		return (GLfloat)this->orient.Z;
	}

	GLfloat getOrientW(){
		return (GLfloat)this->orient.W;
	}

	float getTime(){
		return time;
	}

	void setPos(vertex3 in){
		this->position.set(in);
	}

	void setPos(GLfloat xin, GLfloat yin, GLfloat zin){
		this->position.x = xin;
		this->position.y = yin;
		this->position.z = zin;
	}

	GLfloat* getAll(){
		GLfloat frame[7] = {position.getx(), position.gety(), position.getz(), orient.X, orient.Y, orient.Z, orient.W};
		for(int i=0; i<7; i++){
			printf(" %f", frame[i]);
		}
		printf("\n\n");
		return frame;
	}

	void print(){
		printf("%f, %f, %f, %f, %f, %f, %f \n", position.x, position.y, position.z, orient.X, orient.Y, orient.Z, orient.W);
	}

};