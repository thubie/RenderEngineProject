#include"RenderTarget.h"

namespace RenderEngine
{
	int RenderTarget::GetStride() const
	{
		return GetWidth();
	}
}