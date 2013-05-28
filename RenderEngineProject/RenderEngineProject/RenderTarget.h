#pragma once
#include"Color.h"

namespace RenderEngine
{
	class RenderTarget
	{
		public:
			virtual Color *GetColorBuffer() const = 0;
			virtual float *GetDepthBuffer() const = 0;
			virtual int GetWidth() const = 0;
			virtual int GetHeight() const = 0;
			virtual int GetStride() const;
			virtual void Flip() = 0;
			virtual void SetFrameBufferColor(Color,unsigned int buffNum) = 0;
	};
	
}