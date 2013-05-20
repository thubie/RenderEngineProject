#include"Matrix4x4.h"

namespace RenderEngine
{
	Matrix4x4::Matrix4x4()
	{

	}

	Matrix4x4::Matrix4x4(const Matrix4x4& other) 
	{
		memcpy(this->m_matrix,other.m_matrix,(sizeof(float) * 16));
	}

	Matrix4x4::~Matrix4x4()
	{}

	void Matrix4x4::SetZero()
	{
		memset(m_matrix,0,(sizeof(float) * 16));
	}

	void Matrix4x4::SetRow(int row,float x,float y, float z,float w)
	{
		int index = row - 1;
		m_matrix[index][0] = x;
		m_matrix[index + 1][0] = y;
		m_matrix[index + 2][0] = z;
		m_matrix[index + 3][0] = w;
	}
	
	void Matrix4x4::SetColumn(int column,float x,float y,float z,float w)
	{
		int index = column - 1;
		m_matrix[index][0] = x;
		m_matrix[index][1] = y;
		m_matrix[index][2] = z;
		m_matrix[index][3] = w;
	}

	Matrix4x4& Matrix4x4::operator=(const Matrix4x4& other)
	{
		Matrix4x4 temp;
		memcpy(temp.m_matrix,other.m_matrix,(sizeof(float) * 16));
		return temp;
	}

	//Matrix * Matrix operator
	Matrix4x4 Matrix4x4::operator*(const Matrix4x4& operand)
	{
		Matrix4x4 temp;
		temp.m_matrix[0][0] = (m_matrix[0][0] * operand.m_matrix[0][0]) + (m_matrix[1][0] * operand.m_matrix[0][1]) + (m_matrix[2][0] * operand.m_matrix[0][2]) + (m_matrix[3][0] * operand.m_matrix[0][3]);
		temp.m_matrix[1][0] = (m_matrix[0][0] * operand.m_matrix[1][0]) + (m_matrix[1][0] * operand.m_matrix[1][1]) + (m_matrix[2][0] * operand.m_matrix[1][2]) + (m_matrix[3][0] * operand.m_matrix[1][3]);
		temp.m_matrix[2][0] = (m_matrix[0][0] * operand.m_matrix[2][0]) + (m_matrix[1][0] * operand.m_matrix[2][1]) + (m_matrix[2][0] * operand.m_matrix[2][2]) + (m_matrix[3][0] * operand.m_matrix[2][3]);
		temp.m_matrix[3][0] = (m_matrix[0][0] * operand.m_matrix[3][0]) + (m_matrix[1][0] * operand.m_matrix[3][1]) + (m_matrix[2][0] * operand.m_matrix[3][2]) + (m_matrix[3][0] * operand.m_matrix[3][3]);

		temp.m_matrix[0][1] = (m_matrix[0][1] * operand.m_matrix[0][0]) + (m_matrix[1][1] * operand.m_matrix[0][1]) + (m_matrix[2][1] * operand.m_matrix[0][2]) + (m_matrix[3][1] * operand.m_matrix[0][3]);
		temp.m_matrix[1][1] = (m_matrix[0][1] * operand.m_matrix[1][0]) + (m_matrix[1][1] * operand.m_matrix[1][1]) + (m_matrix[2][1] * operand.m_matrix[1][2]) + (m_matrix[3][1] * operand.m_matrix[1][3]);
		temp.m_matrix[2][1] = (m_matrix[0][1] * operand.m_matrix[2][0]) + (m_matrix[1][1] * operand.m_matrix[2][1]) + (m_matrix[2][1] * operand.m_matrix[2][2]) + (m_matrix[3][1] * operand.m_matrix[2][3]);
		temp.m_matrix[3][1] = (m_matrix[0][1] * operand.m_matrix[3][0]) + (m_matrix[1][1] * operand.m_matrix[3][1]) + (m_matrix[2][1] * operand.m_matrix[3][2]) + (m_matrix[3][1] * operand.m_matrix[3][3]);

		temp.m_matrix[0][2] = (m_matrix[0][2] * operand.m_matrix[0][0]) + (m_matrix[1][2] * operand.m_matrix[0][1]) + (m_matrix[2][2] * operand.m_matrix[0][2]) + (m_matrix[3][2] * operand.m_matrix[0][3]);
		temp.m_matrix[1][2] = (m_matrix[0][2] * operand.m_matrix[1][0]) + (m_matrix[1][2] * operand.m_matrix[1][1]) + (m_matrix[2][2] * operand.m_matrix[1][2]) + (m_matrix[3][2] * operand.m_matrix[1][3]);
		temp.m_matrix[2][2] = (m_matrix[0][2] * operand.m_matrix[2][0]) + (m_matrix[1][2] * operand.m_matrix[2][1]) + (m_matrix[2][2] * operand.m_matrix[2][2]) + (m_matrix[3][2] * operand.m_matrix[2][3]);
		temp.m_matrix[3][2] = (m_matrix[0][2] * operand.m_matrix[3][0]) + (m_matrix[1][2] * operand.m_matrix[3][1]) + (m_matrix[2][2] * operand.m_matrix[3][2]) + (m_matrix[3][2] * operand.m_matrix[3][3]);

		temp.m_matrix[0][3] = (m_matrix[0][3] * operand.m_matrix[0][0]) + (m_matrix[1][3] * operand.m_matrix[0][1]) + (m_matrix[2][3] * operand.m_matrix[0][2]) + (m_matrix[3][3] * operand.m_matrix[0][3]);
		temp.m_matrix[1][3] = (m_matrix[0][3] * operand.m_matrix[1][0]) + (m_matrix[1][3] * operand.m_matrix[1][1]) + (m_matrix[2][3] * operand.m_matrix[1][2]) + (m_matrix[3][3] * operand.m_matrix[1][3]);
		temp.m_matrix[2][3] = (m_matrix[0][3] * operand.m_matrix[2][0]) + (m_matrix[1][3] * operand.m_matrix[2][1]) + (m_matrix[2][3] * operand.m_matrix[2][2]) + (m_matrix[3][3] * operand.m_matrix[2][3]);
		temp.m_matrix[3][3] = (m_matrix[0][3] * operand.m_matrix[3][0]) + (m_matrix[1][3] * operand.m_matrix[3][1]) + (m_matrix[2][3] * operand.m_matrix[3][2]) + (m_matrix[3][3] * operand.m_matrix[3][3]);
		return temp;
	}

	Matrix4x4 Matrix4x4::operator+(const Matrix4x4& operand)
	{
		int i,j;
		Matrix4x4 temp;
		for(i = 0; i < 4; i++)
		{
			for(j = 0; j < 4; j++)
				temp.m_matrix[i][j] += operand.m_matrix[i][j];
		}
		return temp;
	}

	Matrix4x4 Matrix4x4::operator-(const Matrix4x4& operand)
	{
		int i,j;
		Matrix4x4 temp;
		for(i = 0; i < 4; i++)
		{
			for(j = 0; j < 4; j++)
				temp.m_matrix[i][j] -= operand.m_matrix[i][j];
		}
		return temp;
	}
	
	//Matrix * scalar operators
	Matrix4x4 Matrix4x4::operator*(double scalar)
	{
		int i,j;
		Matrix4x4 temp;
		for(i = 0; i < 4; i++)
		{
			for(j = 0; j < 4; j++)
				temp.m_matrix[i][j] *= scalar;
		}
		return temp;
	}

	Matrix4x4 Matrix4x4::operator*(float scalar)
	{
		int i,j;
		Matrix4x4 temp;
		for(i = 0; i < 4; i++)
		{
			for(j = 0; j < 4; j++)
				temp.m_matrix[i][j] *= scalar;
		}
		return temp;
	}
	
	Matrix4x4 Matrix4x4::operator+(double scalar)
	{
		int i,j;
		Matrix4x4 temp;
		for(i = 0; i < 4; i++)
		{
			for(j = 0; j < 4; j++)
				temp.m_matrix[i][j] += scalar;
		}
		return temp;
	}

	Matrix4x4 Matrix4x4::operator+(float scalar)
	{
		int i,j;
		Matrix4x4 temp;
		for(i = 0; i < 4; i++)
		{
			for(j = 0; j < 4; j++)
				temp.m_matrix[i][j] += scalar;
		}
		return temp;
	}
	
	Matrix4x4 Matrix4x4::operator-(double scalar)
	{
		int i,j;
		Matrix4x4 temp;
		for(i = 0; i < 4; i++)
		{
			for(j = 0; j < 4; j++)
				temp.m_matrix[i][j] -= scalar;
		}
		return temp;
	}

	Matrix4x4 Matrix4x4::operator-(float scalar)
	{
		int i,j;
		Matrix4x4 temp;
		for(i = 0; i < 4; i++)
		{
			for(j = 0; j < 4; j++)
				temp.m_matrix[i][j] -= scalar;
		}
		return temp;
	}

}
