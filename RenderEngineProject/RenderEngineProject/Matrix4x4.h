#pragma once
#include<memory.h>

namespace RenderEngine
{
	//Column major 
	class Matrix4x4
	{
	public:
	
		float m_matrix[4][4];

		Matrix4x4();
		Matrix4x4(const Matrix4x4& other);
		~Matrix4x4();

		//Helper functions
		void SetZero();
		void SetRow(int row,float x,float y, float z,float w);
		void SetColumn(int column,float x,float y,float z,float w);

		//Assignment operator
		Matrix4x4& operator=(const Matrix4x4& other);

		//Matrix * Matrix operator
		Matrix4x4 operator*(const Matrix4x4& operand);
		Matrix4x4 operator+(const Matrix4x4& operand);
		Matrix4x4 operator-(const Matrix4x4& operand);
	
		//Matrix * scalar operators
		Matrix4x4 operator*(double scalar);
		Matrix4x4 operator*(float scalar);
	
		Matrix4x4 operator+(double scalar);
		Matrix4x4 operator+(float scalar);
	
		Matrix4x4 operator-(double scalar);
		Matrix4x4 operator-(float scalar);


	};


}