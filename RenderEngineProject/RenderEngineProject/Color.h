#pragma once

namespace RenderEngine
{
	struct Color
	{
		Color();
		Color(unsigned char r,unsigned char g, unsigned char b,unsigned char a);
		void SetColor(unsigned char r,unsigned char g, unsigned char b,unsigned char a = 255);

		Color operator+ (const Color& rhs)const;
		Color operator- (const Color& rhs)const;
		Color &operator=(const Color& rhs);
		Color &operator+=(const Color&  rhs);
		Color &operator*= (float scalar);  
		
		friend Color operator+(const Color &c1, const Color &c2);
		friend Color operator-(const Color &c1, const Color &c2);
		friend Color operator*(float l, const Color &color);
		friend Color operator*(const Color &c1, const Color &c2);
		friend Color operator/(const Color &c, float l);

		//Win32/GDI uses BGRA format 
		unsigned char m_b;
		unsigned char m_g;
		unsigned char m_r;
		unsigned char m_a;
	};

}