#pragma once

namespace RenderEngine
{
	enum Planes
	{
		NEARPLANE	= 1 << 0,
		FARPLANE	= 1 << 1,
		LEFTPLANE	= 1 << 2,
		RIGHTPLANE  = 1 << 3,
		TOPPLANE	= 1 << 4,
		BOTTOMPLANE = 1 << 5,
		
		ALLPLANES = NEARPLANE  |
					FARPLANE   |
					LEFTPLANE  |
					RIGHTPLANE |
					TOPPLANE   |
					BOTTOMPLANE
	};

	class Clipper
	{
		Clipper();
		~Clipper();

	};
}