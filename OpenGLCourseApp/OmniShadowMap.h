#pragma once
#include "ShadowMap.h"

namespace SMN
{
	class OmniShadowMap :public ShadowMap
	{
	   public:

		   OmniShadowMap();

		   bool Init(GLuint width, GLuint height)override;
		   void Write()override;
		   void Read(GLenum textureUnit)override;

		   ~OmniShadowMap();
	};
}
