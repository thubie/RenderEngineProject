#include"Vector3D.h"


namespace RenderEngine
{
	Vector3D::Vector3D() : m_x(0),m_y(0),m_z(0)
	{}

	Vector3D::Vector3D(float x,float y,float z) : m_x(x),m_y(y),m_z(z)
	{}

	//Set values
	void Vector3D::SetValues(float x,float y,float z)
	{
		m_x = x;
		m_y = y;
		m_z = z;
	}

	void Vector3D::Normalize()
	{
		float invLength = 1 / sqrtf(m_x * m_x + m_y*m_y + m_z*m_z);
		m_x *= invLength;
		m_y *= invLength;
		m_z *= invLength;
	}

	//operator overloading
	Vector3D Vector3D::operator+ (const Vector3D& rhs) const
	{
		Vector3D temp;
		temp.m_x = this->m_x + rhs.m_x;
		temp.m_y = this->m_y + rhs.m_y;
		temp.m_z = this->m_z + rhs.m_z;

		return temp;
	}

	Vector3D Vector3D::operator- (const Vector3D& rhs) const
	{
		Vector3D temp;
		temp.m_x = this->m_x - rhs.m_x;
		temp.m_y = this->m_y - rhs.m_y;
		temp.m_z = this->m_z - rhs.m_z;

		return temp;
	}

	//Vector scalar multiplication and division
	Vector3D Vector3D::operator* (const double scalar)
	{
		Vector3D temp;
		temp.m_x = this->m_x * scalar;
		temp.m_y = this->m_y * scalar;
		temp.m_z = this->m_z * scalar;

		return temp;
	}

	Vector3D Vector3D::operator* (const float scalar)
	{
		Vector3D temp;
		temp.m_x = this->m_x * scalar;
		temp.m_y = this->m_y * scalar;
		temp.m_z = this->m_z * scalar;

		return temp;
	}

	Vector3D Vector3D::operator/ (const double scalar)
	{
		Vector3D temp;
		temp.m_x = this->m_x / scalar;
		temp.m_y = this->m_y / scalar;
		temp.m_z = this->m_z / scalar;

		return temp;
	}

	Vector3D Vector3D::operator/ (const float scalar)
	{
		Vector3D temp;
		temp.m_x = this->m_x / scalar;
		temp.m_y = this->m_y / scalar;
		temp.m_z = this->m_z / scalar;

		return temp;
	}

	Vector3D Vector3D::operator+ (const double& scalar) const
	{
		Vector3D temp;
		temp.m_x = m_x + scalar;
		temp.m_y = m_y + scalar;
		temp.m_z = m_z + scalar;
		return temp;

	}

	Vector3D Vector3D::operator+ (const float& scalar) const
	{
		Vector3D temp;
		temp.m_x = m_x + scalar;
		temp.m_y = m_y + scalar;
		temp.m_z = m_z + scalar;
		return temp;
	}

	Vector3D Vector3D::operator- (const double& scalar) const
	{
		Vector3D temp;
		temp.m_x = m_x - scalar;
		temp.m_y = m_y - scalar;
		temp.m_z = m_z - scalar;
		return temp;
	}

	Vector3D Vector3D::operator- (const float& scalar) const
	{
		Vector3D temp;
		temp.m_x = m_x - scalar;
		temp.m_y = m_y - scalar;
		temp.m_z = m_z - scalar;
		return temp;
	}

}