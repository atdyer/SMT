#ifndef OVERLAYSHADER_H
#define OVERLAYSHADER_H

#include "OpenGL/Shaders/GLShader.h"

class OverlayShader : public GLShader
{
	public:
		OverlayShader();
		~OverlayShader();

	protected:

		std::string	vertexSource;
		std::string	fragSource;

		void	CompileShader();
		void	UpdateUniforms();
		void	UpdateCameraUniform();

		ShaderType	GetShaderType();
};

#endif // OVERLAYSHADER_H
