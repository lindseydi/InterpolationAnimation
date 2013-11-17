#include "vertex3.h"
#include <math.h>

class quaternion{
public:
		//Quaternion elements
		float X; 
		float Y;
		float Z;
		float W; // real part


	//Default Constructor
	quaternion() : X(0.0), Y(0.0), Z(0.0), W(1.0) {}
	//! Constructor
	quaternion(float x, float y, float z, float w) {
		this->X = x;
		this->Y = y;
		this->Z = z;
		this->W = w;
	}

	//! Constructor which converts euler angles (radians) to a quaternion
	quaternion(float x, float y, float z){
		*this = set(x, y, z);
	}

	//! Constructor which converts euler angles (radians) to a quaternion
	quaternion(const vertex3& vec){ 
		*this = set(vec.x, vec.y, vec.z);
	}

	~quaternion(void){
		//do nothing there are no pointers
	}

	quaternion& set(float x, float y, float z, float w)
	{
		X = x;
		Y = y;
		Z = z;
		W = w;
		return *this;
	}


	// sets new quaternion based on euler angles
	quaternion& set(float x, float y, float z)
	{
		float angle;

		angle = x * 0.5;
		const float sr = sin(angle);
		const float cr = cos(angle);

		angle = y * 0.5;
		const float sp = sin(angle);
		const float cp = cos(angle);

		angle = z * 0.5;
		const float sy = sin(angle);
		const float cy = cos(angle);

		const float cpcy = cp * cy;
		const float spcy = sp * cy;
		const float cpsy = cp * sy;
		const float spsy = sp * sy;

		X = (float)(sr * cpcy - cr * spsy);
		Y = (float)(cr * spcy + sr * cpsy);
		Z = (float)(cr * cpsy - sr * spcy);
		W = (float)(cr * cpcy + sr * spsy);

		return normalize();
	}

quaternion& quaternion::normalize()
{
		float magnitude = sqrt(W*W + X*X + Y*Y + Z*Z);
		this->W = this->W / magnitude;
		this->X = this->X /  magnitude;
		this->Y = this->Y / magnitude;
		this->Z = this->Z / magnitude;

		return *this;
}

quaternion& operator=(const quaternion& other)
{
	this->X = other.X;
	this->Y = other.Y;
	this->Z = other.Z;
	this->W = other.W;
	return *this;
}

	// multiplication operator
quaternion operator*(const quaternion& input) const
{
	quaternion tmp;

	tmp.W = (input.W * W) - (input.X * X) - (input.Y * Y) - (input.Z * Z);
	tmp.X = (input.W * X) + (input.X * W) + (input.Y * Z) - (input.Z * Y);
	tmp.Y = (input.W * Y) + (input.Y * W) + (input.Z * X) - (input.X * Z);
	tmp.Z = (input.W * Z) + (input.Z * W) + (input.X * Y) - (input.Y * X);

	//returns a new quaternion instead of alterning the input
	return tmp;
}


// multiplication operator
quaternion operator*(float scalar) const
{
	return quaternion(scalar*X, scalar*Y, scalar*Z, scalar*W);
}


//*= multiplication operator
quaternion& quaternion::operator*=(float scalar)
{
	this->X *=scalar;
	this->Y *=scalar;
	this->Z *=scalar;
	this->W *=scalar;
	return *this;
}

//*= multiplication operator
quaternion& operator*=(const quaternion& input)
{
	return *this = (input * (*this));
}

quaternion& rotate(float angle, vertex3 axis){
	quaternion local_rotation;

	//axis is a unit vector
	local_rotation.W  = cosf( angle/2);
	local_rotation.X = axis.x * sinf( angle/2 );
	local_rotation.Y = axis.y * sinf( angle/2 );
	local_rotation.Z = axis.z * sinf( angle/2 );

	*this *= local_rotation;
	return *this;
}

quaternion& rotate(float angle, float axisX, float axisY, float axisZ){
	quaternion local_rotation;

	//axis is a unit vector
	local_rotation.W  = cosf( angle/2);
	local_rotation.X = axisX * sinf( angle/2 );
	local_rotation.Y = axisY * sinf( angle/2 );
	local_rotation.Z = axisZ * sinf( angle/2 );

	return (*this = (local_rotation) * (*this));
}

float dotProduct(quaternion& input){
	return (this->X * input.X) + (this->Y * input.Y) + (this->Z * input.Z) + (this->W * input.W);
}

/*
quaternion& slerp(quaternion a, quaternion b, float u)
{
	if ( u <= 0.0f )
        {
            return a;
        }
   
        if ( u >= 1.0f )
        {
            return b;
        }
   
		float coshalftheta = a.dotProduct(b);
        //coshalftheta = std::min (1.0f, std::max (-1.0f, coshalftheta));
        quaternion c = b;
   
        // Angle is greater than 180. We can negate the angle/quat to get the
        // shorter rotation to reach the same destination.
        if ( coshalftheta < 0.0f )
        {
            coshalftheta = -coshalftheta;
            c = c*-1;
        }
 
            if ( coshalftheta > 0.99f )
            {
            // Angle is tiny - save some computation by lerping instead.
                    quaternion r (Lerp (a, c, t));
                    return r;
            }
   
        float halftheta = acos (coshalftheta);
        float sintheta = sin (halfTheta);
   
        return (std::sin ((1.0f - t) * halftheta) * a + std::sin (t * halftheta) * c) / std::sin (halftheta);
    }
*/

matrix getRotation(){
	matrix rot(4,4);
	rot(0,0) = 1-2*Y*Y -2*Z*Z;
	rot(0,1) = 2*X*Y - 2*W*Z;
	rot(0,2) = 2*X*Z + 2*W*Y;

	rot(1,0) = 2*X*Y + 2*W*Z;
	rot(1,1) = 1-2*X*X - 2*Z*Z;
	rot(1,2) = 2*Y*Z - 2*W*X;

	rot(2,0) = 2*X*Z - 2*W*Y;
	rot(2,1) = 2*Y*Z + 2*W*X;
	rot(2,2) = 1- 2*X*X - 2*Y*Y;

	rot(3,3) = 1.0;

	return rot;
}

};