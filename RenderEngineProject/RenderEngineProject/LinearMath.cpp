#include"LinearMath.h"

//#define PI 3.14159f
//#define EPSILON 0.000000001f

namespace RenderEngine
{

	///////////////////////////////////////////////////
	//Vector maths
	///////////////////////////////////////////////////

	//Dot Product Vec3
	float DotProduct(Vector3D& operand, Vector3D& operand2)
	{
		float result;
		result = (operand.m_x * operand2.m_x) + (operand.m_y * operand2.m_y) + (operand.m_z * operand2.m_z);
		return result;
	}

	//Matrix multiply
	float DotProduct(float x,float y,float z, float x2,float y2,float z2)
	{
		float result;
		result = (x * x2) + (y * y2) + (z * z2);
		return result;
	}

	//Dot Product Vec4
	float DotProduct(Vector4D& operand, Vector4D& operand2)
	{
		float result;
		result = (operand.m_x * operand2.m_x) + (operand.m_y * operand2.m_y) + (operand.m_z * operand2.m_z);
		return result;
	}

	//Cross product answer will be written to Vec3 Dest
	void CrossProduct(Vector3D& Dest,const Vector3D& operand, const Vector3D& operand2)
	{
		Dest.m_x = (operand.m_y * operand2.m_z) - (operand.m_z * operand2.m_y);
		Dest.m_y = (operand.m_z * operand2.m_x) - (operand.m_x * operand2.m_z);
		Dest.m_z = (operand.m_x * operand2.m_y) - (operand.m_y * operand2.m_x);
	}

	//Cross product answer will be written to Vec4 Dest
	void CrossProduct(Vector4D& Dest, const Vector4D& operand, const Vector4D& operand2)
	{
		Dest.m_x = (operand.m_y * operand2.m_z) - (operand.m_z * operand2.m_y);
		Dest.m_y = (operand.m_z * operand2.m_x) - (operand.m_x * operand2.m_z);
		Dest.m_z = (operand.m_x * operand2.m_y) - (operand.m_y * operand2.m_x);
	}

	//Compute transform axis from object position and target point
	void LookAtToAxis(const Vector3D& position,const Vector3D& target,
					  Vector3D& left,Vector3D&upDir, Vector3D& forward)
	{
		//compute the forward vector.
		forward = target - position;
		forward.Normalize();

		// compute temporal up vector based on the forward vector
		// watch out when look up/down at 90 degree
		// for example, forward vector is on the Y axis
		if(fabs(forward.m_x) < EPSILON && fabs(forward.m_z) < EPSILON)
		{
			//forward vector is pointing +Y axis
			if(forward.m_y > 0)
				upDir = Vector3D(0,0,-1);
			else
				upDir = Vector3D(0,0,1);
		}
		// in general, up is straight up
		else
		{
			upDir = Vector3D(0,1,0);
		}

		//Compute the left vector
		CrossProduct(left,upDir,forward);
		left.Normalize();

		//re-calculate the orthonormal up vector
		CrossProduct(upDir,forward,left);
		upDir.Normalize();
	}

	// compute transform axis from object position, target and up direction
	void LookAtToAxes(const Vector3D& pos, const Vector3D& target, const Vector3D& upDir,
					  Vector3D& left, Vector3D& up, Vector3D& forward)
	{
		// compute the forward vector
		forward = target - pos;
		forward.Normalize();

		// compute the left vector
		CrossProduct(left,upDir,forward);
		left.Normalize();

		// compute the orthonormal up vector
		CrossProduct(up,forward,left);
		up.Normalize();
	}

	//Calculates the length of a vector.
	float GetLength(Vector3D& vector)
	{	
		return sqrt(abs((vector.m_x * vector.m_x) + (vector.m_y * vector.m_y) + (vector.m_z * vector.m_z))); 
	}

	///////////////////////////////////////////////////
	//Matrix maths
	///////////////////////////////////////////////////

	void MultiMat4x4(Matrix4x4& dest,const Matrix4x4& operand, const Matrix4x4& operand2)
	{
		dest.m_matrix[0][0] = (operand.m_matrix[0][0] * operand2.m_matrix[0][0]) + (operand.m_matrix[1][0] * operand2.m_matrix[0][1]) + (operand.m_matrix[2][0] * operand2.m_matrix[0][2]) + (operand.m_matrix[3][0] * operand2.m_matrix[0][3]);
		dest.m_matrix[1][0] = (operand.m_matrix[0][0] * operand2.m_matrix[1][0]) + (operand.m_matrix[1][0] * operand2.m_matrix[1][1]) + (operand.m_matrix[2][0] * operand2.m_matrix[1][2]) + (operand.m_matrix[3][0] * operand2.m_matrix[1][3]);
		dest.m_matrix[2][0] = (operand.m_matrix[0][0] * operand2.m_matrix[2][0]) + (operand.m_matrix[1][0] * operand2.m_matrix[2][1]) + (operand.m_matrix[2][0] * operand2.m_matrix[2][2]) + (operand.m_matrix[3][0] * operand2.m_matrix[2][3]);
		dest.m_matrix[3][0] = (operand.m_matrix[0][0] * operand2.m_matrix[3][0]) + (operand.m_matrix[1][0] * operand2.m_matrix[3][1]) + (operand.m_matrix[2][0] * operand2.m_matrix[3][2]) + (operand.m_matrix[3][0] * operand2.m_matrix[3][3]);

		dest.m_matrix[0][1] = (operand.m_matrix[0][1] * operand2.m_matrix[0][0]) + (operand.m_matrix[1][1] * operand2.m_matrix[0][1]) + (operand.m_matrix[2][1] * operand2.m_matrix[0][2]) + (operand.m_matrix[3][1] * operand2.m_matrix[0][3]);
		dest.m_matrix[1][1] = (operand.m_matrix[0][1] * operand2.m_matrix[1][0]) + (operand.m_matrix[1][1] * operand2.m_matrix[1][1]) + (operand.m_matrix[2][1] * operand2.m_matrix[1][2]) + (operand.m_matrix[3][1] * operand2.m_matrix[1][3]);
		dest.m_matrix[2][1] = (operand.m_matrix[0][1] * operand2.m_matrix[2][0]) + (operand.m_matrix[1][1] * operand2.m_matrix[2][1]) + (operand.m_matrix[2][1] * operand2.m_matrix[2][2]) + (operand.m_matrix[3][1] * operand2.m_matrix[2][3]);
		dest.m_matrix[3][1] = (operand.m_matrix[0][1] * operand2.m_matrix[3][0]) + (operand.m_matrix[1][1] * operand2.m_matrix[3][1]) + (operand.m_matrix[2][1] * operand2.m_matrix[3][2]) + (operand.m_matrix[3][1] * operand2.m_matrix[3][3]);

		dest.m_matrix[0][2] = (operand.m_matrix[0][2] * operand2.m_matrix[0][0]) + (operand.m_matrix[1][2] * operand2.m_matrix[0][1]) + (operand.m_matrix[2][2] * operand2.m_matrix[0][2]) + (operand.m_matrix[3][2] * operand2.m_matrix[0][3]);
		dest.m_matrix[1][2] = (operand.m_matrix[0][2] * operand2.m_matrix[1][0]) + (operand.m_matrix[1][2] * operand2.m_matrix[1][1]) + (operand.m_matrix[2][2] * operand2.m_matrix[1][2]) + (operand.m_matrix[3][2] * operand2.m_matrix[1][3]);
		dest.m_matrix[2][2] = (operand.m_matrix[0][2] * operand2.m_matrix[2][0]) + (operand.m_matrix[1][2] * operand2.m_matrix[2][1]) + (operand.m_matrix[2][2] * operand2.m_matrix[2][2]) + (operand.m_matrix[3][2] * operand2.m_matrix[2][3]);
		dest.m_matrix[3][2] = (operand.m_matrix[0][2] * operand2.m_matrix[3][0]) + (operand.m_matrix[1][2] * operand2.m_matrix[3][1]) + (operand.m_matrix[2][2] * operand2.m_matrix[3][2]) + (operand.m_matrix[3][2] * operand2.m_matrix[3][3]);

		dest.m_matrix[0][3] = (operand.m_matrix[0][3] * operand2.m_matrix[0][0]) + (operand.m_matrix[1][3] * operand2.m_matrix[0][1]) + (operand.m_matrix[2][3] * operand2.m_matrix[0][2]) + (operand.m_matrix[3][3] * operand2.m_matrix[0][3]);
		dest.m_matrix[1][3] = (operand.m_matrix[0][3] * operand2.m_matrix[1][0]) + (operand.m_matrix[1][3] * operand2.m_matrix[1][1]) + (operand.m_matrix[2][3] * operand2.m_matrix[1][2]) + (operand.m_matrix[3][3] * operand2.m_matrix[1][3]);
		dest.m_matrix[2][3] = (operand.m_matrix[0][3] * operand2.m_matrix[2][0]) + (operand.m_matrix[1][3] * operand2.m_matrix[2][1]) + (operand.m_matrix[2][3] * operand2.m_matrix[2][2]) + (operand.m_matrix[3][3] * operand2.m_matrix[2][3]);
		dest.m_matrix[3][3] = (operand.m_matrix[0][3] * operand2.m_matrix[3][0]) + (operand.m_matrix[1][3] * operand2.m_matrix[3][1]) + (operand.m_matrix[2][3] * operand2.m_matrix[3][2]) + (operand.m_matrix[3][3] * operand2.m_matrix[3][3]);
	}

	void ScaleMatrix(Matrix4x4& matrix,float xScale,float yScale, float zScale)
	{
		memset(matrix.m_matrix,0,(16 * sizeof(float)));
		matrix.m_matrix[0][0] = xScale;
		matrix.m_matrix[1][1] = yScale;
		matrix.m_matrix[2][2] = zScale;
		matrix.m_matrix[3][3] = 1;
	}

	void TranslationMatrix(Matrix4x4& matrix,float xTrans,float yTrans,float zTrans)
	{
		memset(matrix.m_matrix,0,(16 * sizeof(float)));

		matrix.m_matrix[0][0] = 1;
		matrix.m_matrix[3][0] = xTrans;
		matrix.m_matrix[1][1] = 1;
		matrix.m_matrix[3][1] = yTrans;
		matrix.m_matrix[2][2] = 1;
		matrix.m_matrix[3][2] = zTrans;
		matrix.m_matrix[3][3] = 1;
	}

	//To do change rotation to colum major!!!!
	void RotationXMatrix(Matrix4x4& matrix,float xRot)
	{
		memset(matrix.m_matrix,0,(16 * sizeof(float)));

		matrix.m_matrix[0][0] = 1;
		matrix.m_matrix[3][3] = 1;
		matrix.m_matrix[1][1] = cos(xRot * (PI/180.0f));;
		matrix.m_matrix[1][3] = -1 * sin(xRot * (PI/180.0f));
		matrix.m_matrix[2][1] = sin(xRot * (PI/180.0f));
		matrix.m_matrix[2][2] = cos(xRot * (PI/180.0f));
	}

	void RotationYMatrix(Matrix4x4& matrix,float yRot)
	{
		memset(matrix.m_matrix,0,(16 * sizeof(float)));

		//Column one info
		float radian = yRot * (PI/180.0);
		float sinVal = sin(radian);
		float cosVal = cos(radian);
		matrix.m_matrix[0][0] = cosVal;
		matrix.m_matrix[0][2] = (-1.0 * sinVal);
		matrix.m_matrix[2][0] = sinVal;
		matrix.m_matrix[2][2] = cosVal;
		
		matrix.m_matrix[1][1] = 1;
		
		
		matrix.m_matrix[3][3] = 1;
				
	}

	void RotationZMatrix(Matrix4x4& matrix,float zRot)
	{
		memset(matrix.m_matrix,0,(16 * sizeof(float)));

		matrix.m_matrix[0][0] = cos(zRot * (PI/180.0f));
		matrix.m_matrix[2][0] = -1 * sin(zRot * (PI/180.0f));
		matrix.m_matrix[1][0] = sin(zRot * (PI/180.0f));
		matrix.m_matrix[1][1] = cos(zRot * (PI/180.0f));	
		matrix.m_matrix[2][2] = 1;
		matrix.m_matrix[3][3] = 1;
	}

	void MakeIdentity(Matrix4x4& matrix)
	{
		memset(matrix.m_matrix,0,(16 * sizeof(float)));

		matrix.m_matrix[0][0] = 1;
		matrix.m_matrix[1][1] = 1;
		matrix.m_matrix[2][2] = 1;
		matrix.m_matrix[3][3] = 1;
	}

	///////////////////////////////////////////////////
	//Vector * Matrix maths (Column major!!!)
	///////////////////////////////////////////////////

	Vector3D Vec3MultiMat4x4(Matrix4x4& matrix,const Vector3D& vector)
	{
		Vector3D temp; 
		temp.m_x = (matrix.m_matrix[0][0] * vector.m_x) + (matrix.m_matrix[1][0] * vector.m_y) + (matrix.m_matrix[2][0] * vector.m_z)  + (matrix.m_matrix[3][0] * 1); 
		temp.m_y = (matrix.m_matrix[0][1] * vector.m_x) + (matrix.m_matrix[1][1] * vector.m_y) + (matrix.m_matrix[2][1] * vector.m_z)  + (matrix.m_matrix[3][1] * 1); 
		temp.m_z = (matrix.m_matrix[0][2] * vector.m_x) + (matrix.m_matrix[1][2] * vector.m_y) + (matrix.m_matrix[2][2] * vector.m_z)  + (matrix.m_matrix[3][2] * 1); 

		return temp;
	}

	//Input vector3d will give as output a Vector4D
	Vector4D Vec3Vec4MultiMat4x4(Matrix4x4& matrix,const Vector3D& vector)
	{
		Vector4D temp; 
		temp.m_x = (matrix.m_matrix[0][0] * vector.m_x) + (matrix.m_matrix[1][0] * vector.m_y) + (matrix.m_matrix[2][0] * vector.m_z)  + (matrix.m_matrix[3][0] * 1); 
		temp.m_y = (matrix.m_matrix[0][1] * vector.m_x) + (matrix.m_matrix[1][1] * vector.m_y) + (matrix.m_matrix[2][1] * vector.m_z)  + (matrix.m_matrix[3][1] * 1); 
		temp.m_z = (matrix.m_matrix[0][2] * vector.m_x) + (matrix.m_matrix[1][2] * vector.m_y) + (matrix.m_matrix[2][2] * vector.m_z)  + (matrix.m_matrix[3][2] * 1); 
		temp.m_w = temp.m_w = (matrix.m_matrix[0][3] * 1) + (matrix.m_matrix[1][3] * 1) + (matrix.m_matrix[2][3] * 1)  + (matrix.m_matrix[3][3] * 1); 
		return temp;
	}

	Vector4D Vec4MultiMat4x4(Matrix4x4& matrix,const Vector4D& vector)
	{
		Vector4D temp; 
		temp.m_x = (matrix.m_matrix[0][0] * vector.m_x) + (matrix.m_matrix[1][0] * vector.m_y) + (matrix.m_matrix[2][0] * vector.m_z)  + (matrix.m_matrix[3][0] * vector.m_w); 
		temp.m_y = (matrix.m_matrix[0][1] * vector.m_x) + (matrix.m_matrix[1][1] * vector.m_y) + (matrix.m_matrix[2][1] * vector.m_z)  + (matrix.m_matrix[3][1] * vector.m_w); 
		temp.m_z = (matrix.m_matrix[0][2] * vector.m_x) + (matrix.m_matrix[1][2] * vector.m_y) + (matrix.m_matrix[2][2] * vector.m_z)  + (matrix.m_matrix[3][2] * vector.m_w); 
		temp.m_w = (matrix.m_matrix[0][3] * vector.m_x) + (matrix.m_matrix[1][3] * vector.m_y) + (matrix.m_matrix[2][3] * vector.m_z)  + (matrix.m_matrix[3][3] * vector.m_w); 

		return temp;
	}


	void Vec3MultiMat4x4(Vector3D& dest, Matrix4x4& matrix,Vector3D& vector)
	{
		dest.m_x = (matrix.m_matrix[0][0] * vector.m_x) + (matrix.m_matrix[1][0] * vector.m_y) + (matrix.m_matrix[2][0] * vector.m_z)  + (matrix.m_matrix[3][0] * 1); 
		dest.m_y = (matrix.m_matrix[0][1] * vector.m_x) + (matrix.m_matrix[1][1] * vector.m_y) + (matrix.m_matrix[2][1] * vector.m_z)  + (matrix.m_matrix[3][1] * 1); 
		dest.m_z = (matrix.m_matrix[0][2] * vector.m_x) + (matrix.m_matrix[1][2] * vector.m_y) + (matrix.m_matrix[2][2] * vector.m_z)  + (matrix.m_matrix[3][2] * 1); 
	}

	void Vec4MultiMat4x4(Vector4D& dest, Matrix4x4& matrix,Vector4D& vector)
	{
		dest.m_x = (matrix.m_matrix[0][0] * vector.m_x) + (matrix.m_matrix[1][0] * vector.m_y) + (matrix.m_matrix[2][0] * vector.m_z)  + (matrix.m_matrix[3][0] * vector.m_w); 
		dest.m_y = (matrix.m_matrix[0][1] * vector.m_x) + (matrix.m_matrix[1][1] * vector.m_y) + (matrix.m_matrix[2][1] * vector.m_z)  + (matrix.m_matrix[3][1] * vector.m_w); 
		dest.m_z = (matrix.m_matrix[0][2] * vector.m_x) + (matrix.m_matrix[1][2] * vector.m_y) + (matrix.m_matrix[2][2] * vector.m_z)  + (matrix.m_matrix[3][2] * vector.m_w); 
		dest.m_w = (matrix.m_matrix[0][3] * vector.m_x) + (matrix.m_matrix[1][3] * vector.m_y) + (matrix.m_matrix[2][3] * vector.m_z)  + (matrix.m_matrix[3][3] * vector.m_w);
	}
}