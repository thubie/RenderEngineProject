#pragma once
#include<math.h>

namespace RenderEngine
{
	struct Vector3D
	{
		float m_x,m_y,m_z;

		Vector3D();
		Vector3D(float x,float y,float z);

		//Set values
		void SetValues(float x,float y,float z);

		//Normalize Vector
		void Normalize();

		//operator overloading
		Vector3D operator+ (const Vector3D& rhs) const;
		Vector3D operator- (const Vector3D& rhs) const;

		//Vector scalar multiplication and division
		Vector3D operator* (const double scalar);
		Vector3D operator* (const float scalar);

		Vector3D operator/ (const double scalar);
		Vector3D operator/ (const float scalar);

		Vector3D operator+ (const double& scalar) const;
		Vector3D operator+ (const float& scalar) const;

		Vector3D operator- (const double& scalar) const;
		Vector3D operator- (const float& scalar) const;
	};

}