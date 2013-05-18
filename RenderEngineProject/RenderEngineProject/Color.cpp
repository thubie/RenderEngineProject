#pragma once

#include"Color.h"

namespace RenderEngine
{
	Color::Color() : m_r(0),m_g(0),m_b(0),m_a(0)
	{}

	Color::Color(unsigned char r,unsigned char g,unsigned char b,unsigned char a) : m_r(r),m_g(g),m_b(b),m_a(a)
	{}

	void Color::SetColor(unsigned char r, unsigned char g,unsigned char b,unsigned char a)
	{
		m_r = r;
		m_g = g;
		m_b = b;
		m_a = a;
	}

	///*inline*/ Color &Color::operator=(const Color& rhs)
	//{
	//	m_r = rhs.m_r;
	//	m_g = rhs.m_g;
	//	m_b = rhs.m_b;
	//	m_a = rhs.m_a;

	//	return *this;
	//}

	inline Color Color::operator+(const Color& rhs) const
	{
		return Color(m_r + rhs.m_r, m_g + rhs.m_g, m_b + rhs.m_b, m_a + rhs.m_a);
	}

	inline Color Color::operator-(const Color& rhs) const
	{
		return Color(m_r + rhs.m_r, m_g + rhs.m_g, m_b + rhs.m_b, m_a + rhs.m_a);
	}

	inline Color &Color::operator+=(const Color& rhs)
	{
		m_r += rhs.m_r;
		m_g += rhs.m_g;
		m_b += rhs.m_b;
		m_a += rhs.m_a;

		return *this;
	}

	inline Color &Color::operator*=(float scalar)
	{
		*this = scalar * *this;
		return *this;
	}

	

	inline Color operator+(const Color &color1,const Color &color2)
	{
		return Color(color1.m_r + color2.m_r, color1.m_g + color2.m_g,
			color1.m_b + color2.m_b, color1.m_a + color2.m_a);
	}

	inline Color operator-(const Color &color1,const Color &color2)
	{
		return Color(color1.m_r - color2.m_r, color1.m_g - color2.m_g,
			color1.m_b - color2.m_b, color1.m_a - color2.m_a);
	}

	inline Color operator*(float scalar, const Color &color)
	{
		unsigned char r = scalar * color.m_r;
		unsigned char g = scalar * color.m_g;
		unsigned char b = scalar * color.m_b;
		unsigned char a = scalar * color.m_a;

		return Color(r,g,b,a);
	}

	inline Color operator*(const Color &color1, const Color &color2)
	{
		unsigned char r = color1.m_r * color2.m_r;
		unsigned char g = color1.m_g * color2.m_g;
		unsigned char b = color1.m_b * color2.m_b;
		unsigned char a = color1.m_a * color2.m_a;

		return Color(r,g,b,a);
	}

	inline Color operator/(const Color &color1,float scalar)
	{
		//reciprical mul is faster then divide
		scalar = 1.0f / scalar;

		unsigned char r = color1.m_r * scalar;
		unsigned char g = color1.m_g * scalar;
		unsigned char b = color1.m_b * scalar;
		unsigned char a = color1.m_a * scalar;

		return Color(r,g,b,a);
	}
	
	
}