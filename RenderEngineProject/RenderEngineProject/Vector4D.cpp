#include"Vector4D.h"


namespace RenderEngine
{
	//Default costructor 
	Vector4D::Vector4D() : m_x(0),m_y(0),m_z(0),m_w(1)
	{}

	Vector4D::Vector4D(float x,float y,float z,float w) : m_x(x),m_y(y),m_z(z),m_w(w)
	{}

	void Vector4D::SetValues(float x,float y,float z,float w)
	{
		m_x = x;
		m_y = y;
		m_z = z;
		m_w = w;
	}

	void Vector4D::SetValues(Vector3D& vector)
	{
		m_x = vector.m_x;
		m_y = vector.m_y;
		m_z = vector.m_z;
		m_w = 1.0f;
	}

	void Vector4D::Normalize()
	{
		float invLength = 1 / sqrtf(m_x * m_x + m_y*m_y + m_z*m_z + m_w * m_w);
		m_x *= invLength;
		m_y *= invLength;
		m_z *= invLength;
		m_w *= invLength;
	}

	//operator overloading
	Vector4D Vector4D::operator+ (const Vector4D& rhs) const
	{
		Vector4D temp;
		temp.m_x = this->m_x + rhs.m_x;
		temp.m_y = this->m_y + rhs.m_y;
		temp.m_z = this->m_z + rhs.m_z;
		temp.m_w = this->m_w + rhs.m_w;

		return temp;
	}

	Vector4D Vector4D::operator- (const Vector4D& rhs) const
	{
		Vector4D temp;
		temp.m_x = this->m_x - rhs.m_x;
		temp.m_y = this->m_y - rhs.m_y;
		temp.m_z = this->m_z - rhs.m_z;
		temp.m_w = this->m_w - rhs.m_w;

		return temp;
	}

	//Vector scalar multiplication and division
	Vector4D Vector4D::operator* (const double scalar)
	{
		Vector4D temp;
		temp.m_x = this->m_x * scalar;
		temp.m_y = this->m_y * scalar;
		temp.m_z = this->m_z * scalar;
		temp.m_w = this->m_w * scalar;

		return temp;
	}

	Vector4D Vector4D::operator* (const float scalar)
	{
		Vector4D temp;
		temp.m_x = this->m_x * scalar;
		temp.m_y = this->m_y * scalar;
		temp.m_z = this->m_z * scalar;
		temp.m_w = this->m_w * scalar;

		return temp;
	}

	Vector4D Vector4D::operator/ (const double scalar)
	{
		Vector4D temp;
		temp.m_x = this->m_x / scalar;
		temp.m_y = this->m_y / scalar;
		temp.m_z = this->m_z / scalar;
		temp.m_w = this->m_w / scalar;

		return temp;
	}

	Vector4D Vector4D::operator/ (const float scalar)
	{
		Vector4D temp;
		temp.m_x = this->m_x / scalar;
		temp.m_y = this->m_y / scalar;
		temp.m_z = this->m_z / scalar;
		temp.m_w = this->m_w / scalar;

		return temp;
	}

	Vector4D Vector4D::operator+ (const double& scalar) const
	{
		Vector4D temp;
		temp.m_x = m_x + scalar;
		temp.m_y = m_y + scalar;
		temp.m_z = m_z + scalar;
		temp.m_w = m_w + scalar;
		return temp;

	}

	Vector4D Vector4D::operator+ (const float& scalar) const
	{
		Vector4D temp;
		temp.m_x = m_x + scalar;
		temp.m_y = m_y + scalar;
		temp.m_z = m_z + scalar;
		temp.m_w = m_w + scalar;
		return temp;
	}

	Vector4D Vector4D::operator- (const double& scalar) const
	{
		Vector4D temp;
		temp.m_x = m_x - scalar;
		temp.m_y = m_y - scalar;
		temp.m_z = m_z - scalar;
		temp.m_w = m_w - scalar;
		return temp;
	}

	Vector4D Vector4D::operator- (const float& scalar) const
	{
		Vector4D temp;
		temp.m_x = m_x - scalar;
		temp.m_y = m_y - scalar;
		temp.m_z = m_z - scalar;
		temp.m_w = m_w - scalar;
		return temp;
	}

}