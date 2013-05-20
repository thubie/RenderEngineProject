#pragma once
#include"Vector3D.h"
#include<math.h>

namespace RenderEngine
{
	struct Vector4D
	{
		float m_x,m_y,m_z,m_w;

		Vector4D();
		Vector4D(float x,float y,float z,float w);
	
		//Set values
		void SetValues(float x,float y,float z,float w);
		void SetValues(Vector3D& vector);
	
		//Normalize Vector
		void Normalize();

		//operator overloading
		Vector4D operator+ (const Vector4D& rhs) const;
		Vector4D operator- (const Vector4D& rhs) const;

		//Vector scalar multiplication and division
		Vector4D operator* (const double scalar);
		Vector4D operator* (const float scalar);	

		Vector4D operator/ (const double scalar);
		Vector4D operator/ (const float scalar);

		Vector4D operator+ (const double& scalar) const;
		Vector4D operator+ (const float& scalar) const;

		Vector4D operator- (const double& scalar) const;
		Vector4D operator- (const float& scalar) const;
	};

}